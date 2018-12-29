const int greenPin =  11;
const int redPin =  10;
const int bluePin =  9;

int buttonState = 0;

void setup() {
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void setBrightness(int pin, int bright) {
  analogWrite(pin, 255 - bright);
}

void setColor(int r, int g, int b) {
  setBrightness(redPin, r);
  setBrightness(greenPin, g);
  setBrightness(bluePin, b);
}

float rw = 0;
float gw = 0;
float bw = 0;

void loop() {
  rw += 0.02;
  gw += 0.03;
  bw += 0.05;
  setColor((int)(sin(rw) * 128 + 128), (int)(sin(gw) * 128 + 128), (int)(sin(bw) * 128 + 128));
  delay(10);
  //setColor(0, 0, 0);
  //delay(100);
}
