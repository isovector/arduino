/************************************************************
  [ Andee - with Arduino/Genuino 101 ]
  ================ 
  Displaying Gyroscope, Linear Acceleration, Gravity and GPS Values from the Smartdevice!

  Contact us at andee@annikken.com if there are 
  bugs in this sketch or if you need help with the 
  Annikken Andee
************************************************************/
// Always include these libraries. Andee 101 needs them
// to work with the Arduino!
#include <CurieBLE.h>
#include <Andee101.h>

// Every object that appears on your smartphone's screen
// needs to be declared like this:
Andee101Helper grav;
Andee101Helper gyro;
Andee101Helper lac;
Andee101Helper gps;
Andee101Helper xDisp;
Andee101Helper yDisp;
Andee101Helper zDisp;
// We're creating 7 objects

//We are initialising 4 variables to help keep track of states
//This will help to create toggle buttons
int gravState;
int gyroState;
int lacState;
int gpsState;

//Initialising 3 variables to store the values from the smartdevice
float x;
float y;
float z;

//Initialising 2 more variables to check if the UI has been updated yet
int refreshUI = 0;

// The setup() function is meant to tell Arduino what to do
// only when it starts up.
void setup() {
  Andee101.begin();// Setup communication between Annikken Andee101 and Arduino
  setInitialData();// Define object types and their appearance
  Serial.begin(9600);// Start Serial communication to view data in the Serial Terminal

  //Setting these variables to 0
  gravState = 0;
  gyroState = 0;
  lacState = 0;
  gpsState = 0;
}

// This is the function meant to define the types and the apperance of
// all the objects on your smartphone
void setInitialData()
{
  xDisp.setId(0); // Each object must have a unique ID number
  xDisp.setType(DATA_OUT);// Defines object as a display box
  xDisp.setCoord(1, 1, 32, 25); // Sets the location and size of your object
  xDisp.setTitle("X-Axis");// Sets the title of the object
  xDisp.setData("");//Leave this empty since this will be filled up with data later on
  xDisp.setColor(LIGHT_BLUE);//Sets the colour of the UI object

  yDisp.setId(1);
  yDisp.setType(DATA_OUT);
  yDisp.setCoord(34, 1, 32, 25);
  yDisp.setTitle("Y-Axis");
  yDisp.setData("");
  yDisp.setColor(BLUE);

  zDisp.setId(2);
  zDisp.setType(DATA_OUT);
  zDisp.setCoord(67, 1, 32, 25);
  zDisp.setTitle("Z-Axis");
  zDisp.setData("");
  zDisp.setColor(DARK_BLUE);

  grav.setId(3);
  grav.setType(BUTTON_IN);// Defines object as a button
  grav.setCoord(2, 27, 23, 25);
  grav.setTitle("Gravity Sensor On");
  grav.setColor(GREEN);

  gyro.setId(4);
  gyro.setType(BUTTON_IN);
  gyro.setCoord(26, 27, 24, 25);
  gyro.setTitle("Gyro Sensor On");
  gyro.setColor(GREEN);

  lac.setId(5);
  lac.setType(BUTTON_IN);
  lac.setCoord(51, 27, 24, 25);
  lac.setTitle("LAC Sensor On");
  lac.setColor(GREEN);

  gps.setId(6);
  gps.setType(BUTTON_IN);
  gps.setCoord(76, 27, 22, 25);
  gps.setTitle("GPS Sensor On");
  gps.setColor(GREEN);
}

// Arduino will run instructions here repeatedly until you power it off.
void loop() {
  Andee101.poll();//required in every Andee101 sketch
  if (refreshUI == 0 && Andee101.isConnected() == true)//If the UI has not been updated and the Andee101 is connected
  {
    for(int k = 0;k<3;k++)
    {
      gyro.update();//Each UI object has to be updated to appear in the smartdevice
      grav.update();
      lac.update();
      gps.update();
      delay(100);//delay is needed or else Arduino 101 will be crash
      xDisp.update();
      yDisp.update();
      zDisp.update();
      delay(100);
    }    
    refreshUI = 1;//This shows that the UI has been updated
  }
  else if (refreshUI == 1 && AndeeConnected == 0) //If the UI has been updated but the Andee101 is not connected
  {
    refreshUI = 0;
  }

  ////////////////////////////This is the control for the gravity sensor///////////////////////////////////////////////
  if (grav.isPressed() ) //if the grav button is pressed
  {
    // Prevent user from accidentally pressing the button again
    // until Arduino has sent an acknowledgement
    grav.ack();

    if (gravState == 0) //If gravState is low when the button is pressed
    {
      Andee101.gravInit(250, -1); //This will activate the Gravity sensor on the smartdevice.
      //250 is the time interval of each reading in milliseconds. Lowest is 250ms.
      //-1 is the number of times readings will be taken by the sensor. -1 means infinite
      delay(10);//A small delay is here to allow some time for the sensor to finish initialisation
      gravState = 1;//change the gravState here
      gyroState = 0;//Switch off the readings from other sensors
      lacState = 0;
      gpsState = 0;
    }
    else if (gravState == 1) //If gravState is high when button is pressed
    {
      gravState = 0;//switch off gravity sensor
      Andee101.gravStop();//this will tell the smart device to stop reading from its gravity sensor
    }
  }

  if (gravState == 1)//whenever gravState is HIGH
  {
    grav.setTitle("Grav Sensor off");//change the title of the button
    grav.setColor(RED);//change the colour of the button
    Andee101.getGravReading(&x, &y, &z); //this function saves the sensor reading into the variables x,y and z

    //These functions send the data to the data part of the display box UI objects
    xDisp.setData(x);
    yDisp.setData(y);
    zDisp.setData(z);
    Serial.print("x:"); Serial.println(x);
    Serial.print("y:"); Serial.println(y);
    Serial.print("z:"); Serial.println(z);

    //Update the UI objects to reflect the current values of x,y and z
    grav.update();
    xDisp.update();
    yDisp.update();
    zDisp.update();
    delay(100);
  }
  else// if gravState is LOW
  {
    grav.setTitle("Grav Sensor On");//change the title of the button
    grav.setColor(GREEN);//change the colour of the button
    grav.update();//update the grav UI
    delay(100);
  }

  //////////////////////This is the control for the Gyroscope sensor//////////////////////////////////////////////////////
  if (gyro.isPressed() )
  {
    gyro.ack();

    if (gyroState == 0)
    {
      Andee101.gyroInit(250, -1);
      delay(10);
      gyroState = 1;
      gravState = 0;
      lacState = 0;
      gpsState = 0;
    }
    else
    {
      gyroState = 0;
      Andee101.gyroStop();
    }
  }

  if (gyroState == 1)
  {
    gyro.setTitle("Gyro Sensor off");
    gyro.setColor(RED);
    Andee101.getGyroReading(&x, &y, &z);
    xDisp.setData(x);
    yDisp.setData(y);
    zDisp.setData(z);
    xDisp.update();
    yDisp.update();
    zDisp.update();
    gyro.update();
    delay(100);
  }
  else
  {
    gyro.setTitle("Gyro Sensor On");
    gyro.setColor(GREEN);
    gyro.update();
    delay(100);
  }

  //////////////////////////This is the control for the LAC(Linear Acceleration) sensor/////////////////////////////////////////
  if (lac.isPressed() )
  {
    lac.ack();

    if (lacState == 0)
    {
      Andee101.lacInit(250, -1);
      delay(10);
      lacState = 1;
      gravState = 0;
      gyroState = 0;
      gravState = 0;
    }
    else
    {
      lacState = 0;
      Andee101.lacStop();
    }
  }

  if (lacState == 1)
  {
    lac.setTitle("LAC Sensor off");
    lac.setColor(RED);
    Andee101.getLacReading(&x, &y, &z);
    xDisp.setData(x);
    yDisp.setData(y);
    zDisp.setData(z);
    xDisp.update();
    yDisp.update();
    zDisp.update();
    lac.update();
    delay(100);
  }
  else
  {
    lac.setTitle("LAC Sensor On");
    lac.setColor(GREEN);
    lac.update();
    delay(100);
  }

  /////////////////////////////This is the control for the GPS sensor///////////////////////////////////////////////
  if (gps.isPressed() )
  {
    gps.ack();

    if (gpsState == 0)
    {
      Andee101.gpsInit(5000, -1);//For GPS the minimum time interval is 5000ms. The smartphone/tablet needs some time
      delay(10);            //to achieve a lock so this timing should be enough time to get the information
      gpsState = 1;
      lacState = 0;
      gyroState = 0;
      gravState = 0;
    }
    else
    {
      gpsState = 0;
      Andee101.gpsStop();
    }
  }

  if (gpsState == 1)
  {
    gps.setTitle("Gps Sensor off");
    gps.setColor(RED);
    Andee101.getGpsReading(&x, &y, &z);
    xDisp.setData(x, 6);
    yDisp.setData(y, 6);
    zDisp.setData(z, 6);
    xDisp.update();
    yDisp.update();
    zDisp.update();
    gps.update();
    delay(100);
  }
  else
  {
    gps.setTitle("Gps Sensor On");
    gps.setColor(GREEN);
    gps.update();
    delay(100);
  }
}



