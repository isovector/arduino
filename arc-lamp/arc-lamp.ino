#include <FastLED.h>

#define LED_PIN   7
#define NUM_LEDS  2

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}

void setColor(int i, int r, int g, int b) {
  leds[i] = CRGB(r, g, b);
}

void loop() {
  for (int i = 0; i < NUM_LEDS; i++) {
    setColor(i, 255, 255, 255);
    }
  FastLED.show();
  delay(100);
}
