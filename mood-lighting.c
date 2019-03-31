#include <FastLED.h>
#define LED_PIN     7
#define NUM_LEDS    60

CRGB leds[NUM_LEDS];
CRGB colors[COLORS];

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
  leds[i * 2] = CRGB((int)(cos(rw) * 12 + 12), (int)(sin(gw) * 12 + 12), (int)(sin(bw) * 12 + 12));
  FastLED.show();
  i += dir;

  if (i <= 0 || i >= 29) {
    dir *= -1;
  }


  delay(500);
}
