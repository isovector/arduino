#include "I2Cdev.h"
#include <PID_v1.h>
#include "MPU6050_6Axis_MotionApps20.h"

MPU6050 mpu;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO


constexpr double setpoint = 183;
constexpr double Kp = 13;
constexpr double Kd = 0.5;
constexpr double Ki = 140;
constexpr int motor_output = 255;
constexpr double balance_band = 60;


double input, output;
PID pid(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);


volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady()
{
    mpuInterrupt = true;
}

void setup() {
  Serial.begin(115200);

  // initialize device
    Serial.println("Initializing I2C devices...");
    mpu.initialize();

     // verify connection
    Serial.println("Testing device connections...");
    Serial.println( mpu.testConnection()
                  ? "MPU6050 connection successful"
                  : "MPU6050 connection failed"
                  );

    // load and configure the DMP
    uint8_t devStatus = mpu.dmpInitialize();


    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(79);
    mpu.setYGyroOffset(58);
    mpu.setZGyroOffset(-35);
    mpu.setZAccelOffset(1485);

      // make sure it worked (returns 0 if so)
    if (devStatus == 0)
    {
        // turn on the DMP, now that it's ready
        Serial.println("Enabling DMP...");
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println("Enabling interrupt detection (Arduino external interrupt 0)...");
        attachInterrupt(0, dmpDataReady, RISING);
        mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println("DMP ready! Waiting for first interrupt...");
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();

        //setup PID
        pid.SetMode(AUTOMATIC);
        pid.SetSampleTime(10);
        pid.SetOutputLimits(-motor_output, motor_output);
    }
    else
    {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print("DMP Initialization failed (code ");
        Serial.print(devStatus);
        Serial.println(")");
    }

    // Initialise the Motor output pins
    pinMode (6, OUTPUT);
    analogWrite(6,LOW);
    pinMode (9, OUTPUT);
    analogWrite(9,LOW);
    pinMode (10, OUTPUT);
    analogWrite(10,LOW);
    pinMode (11, OUTPUT);
    analogWrite(11,LOW);
}

void loop() {
    // if programming failed, don't try to do anything
    if (!dmpReady) return;

    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize)
    {
        // no mpu data - performing PID calculations and output to motors
        pid.Compute();
        Serial.print(input);
        Serial.print(" =>");
        Serial.println(output);

        constexpr double half_band_width = balance_band / 2
                       , band_low        = setpoint - half_band_width
                       , band_high       = setpoint + half_band_width;
        if (input >= band_low && input <= band_high) {
          if      (output>0) Forward();
          else if (output<0) Reverse();
        } else Stop();

    }

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    uint8_t mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024)
    {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        Serial.println("FIFO overflow!");

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    }
    else if (mpuIntStatus & 0x02)
    {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        uint8_t fifoBuffer[64];
        mpu.getFIFOBytes(fifoBuffer, packetSize);

        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

        // orientation/motion vars
        Quaternion q;           // [w, x, y, z]
        VectorFloat gravity;    // [x, y, z]
        float ypr[3];           // [yaw, pitch, roll]

        mpu.dmpGetQuaternion(&q, fifoBuffer); //get value for q
        mpu.dmpGetGravity(&gravity, &q); //get value for gravity
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity); //get value for ypr

        input = ypr[1] * 180/M_PI + 180;

   }
}

void Reverse() {
    analogWrite(6,output * -1);
    analogWrite(9,0);
    analogWrite(10,output * -1);
    analogWrite(11,0);
}

void Forward() {
    analogWrite(6,0);
    analogWrite(9,output);
    analogWrite(10,0);
    analogWrite(11,output);
}

void Stop() {
    analogWrite(6,0);
    analogWrite(9,0);
    analogWrite(10,0);
    analogWrite(11,0);
}
