#include <Adafruit_NeoPixel.h>
#define PIN       9
#define NUMPIXELS 89

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 25

void setup() {
  pinMode(8, OUTPUT);
  pinMode(10, OUTPUT);
  digitalWrite(8, HIGH);
  digitalWrite(10, LOW);
  Serial.begin(9600);
  pixels.begin();
}

void loop() {
  pixels.clear();

  if (Serial.available() > 2) {
    char r, g, b;
    r = Serial.read();
    g = Serial.read();
    b = Serial.read();

    for(int i=0; i<NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(r, g, b));
      pixels.show();
    }
  }

  delay(DELAYVAL);
}
