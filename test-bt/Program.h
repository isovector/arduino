#ifndef PROGRAM_H
#define PROGRAM_H

#include "Fixed.h"

#define LOGICAL_LEDS (300 + 300 - 49)
#define PHYSICAL_LEDS 300
#define LED_PIN 7


Adafruit_NeoPixel strip = Adafruit_NeoPixel(PHYSICAL_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(PHYSICAL_LEDS, LED_PIN + 1, NEO_GRB + NEO_KHZ800);


struct CRGB {
  CRGB(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {
  }

  uint8_t r;
  uint8_t g;
  uint8_t b;
};


class Program {
public:
  virtual ~Program() {}

  virtual CRGB eval(int v) const {
    return {0, 0, 0};
  };

  uint32_t eval_uint(int v) const {
    CRGB c = eval(v);
    return strip.Color(c.r, c.g, c.b);
  }

  virtual void evolve(time delta) {
    return;
  }
};

Program *program = NULL;


// run the function to generate every light on the logical strip
// since 0 on the physical strips is in the middle of the room, we map idx 0 to
// (PHYSICAL_LEDS - 1) on strip 1
//
// due to memory restrictions, we have to use the same buffer to push data to each
// strip, so this must be a pure function
void push_lights() {
  for (int i = 0; i < PHYSICAL_LEDS; i++) {
    strip.setPixelColor(i, program->eval_uint(PHYSICAL_LEDS - 1 - i));
  }
  strip.show();

  for (int i = PHYSICAL_LEDS; i < LOGICAL_LEDS; i++) {
    strip2.setPixelColor(i, program->eval_uint(PHYSICAL_LEDS - 1 - i));
  }
  strip2.show();
}


#endif

