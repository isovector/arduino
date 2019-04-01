#include <FastLED.h>

#define LED_PIN   7
#define NUM_LEDS  60
#define INTENSITY 64
#define THRESHOLD (INTENSITY * 2)

CRGB leds[NUM_LEDS];


void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}

int i = 0;
int dir = 1;

float rw = 0;
float gw = 10;
float bw = 0;

void loop() {
  rw += 0.02;
  gw += 0.03;
  bw += 0.05;

  int r = cos(rw) * INTENSITY + INTENSITY;
  int g = sin(gw) * INTENSITY + INTENSITY;
  int b = sin(bw) * INTENSITY + INTENSITY;
  float div = 1;

  if (r + g + b > THRESHOLD) {
    div = 1.8;
  }

  leds[i] = CRGB((int)(r / div), (int)(g / div), (int)(b / div));
  FastLED.show();
  i += dir;

  if (i <= 0 || i >= 59) {
    dir *= -1;
  }


  delay(250);
}


