#ifndef PROGRAM_H
#define PROGRAM_H

#include "Fixed.h"

#define LOGICAL_LEDS (300 + 300 - 49)
#define PHYSICAL_LEDS 300
#define LED_PIN 7

#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(0, 1); // RX, TX */

#define DEBUG(x) (DebugSerial.println(x))



Adafruit_NeoPixel strip = Adafruit_NeoPixel(PHYSICAL_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(LOGICAL_LEDS - PHYSICAL_LEDS, LED_PIN + 2, NEO_GRB + NEO_KHZ800);


struct CRGB {
  CRGB(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {
  }

  CRGB(uint32_t c) : rgb(c) {
  }


  union {
    struct {
      uint8_t b;
      uint8_t g;
      uint8_t r;
      uint8_t _unused;
    };
    uint32_t rgb;
  };
};

struct Interval {

  Interval(int16_t start, int16_t end) :
    start(start), end(end)
  {}

  int16_t start;
  int16_t end;
};


class Program {
public:
  virtual ~Program() {}

  virtual CRGB eval(const int v) const {
    return {0, 0, 0};
  };

  uint32_t eval_uint(const int v) const {
    CRGB c = eval(v);
    return c.rgb;
  }

  virtual Interval canvas() const {
    return {0, LOGICAL_LEDS};
  }

  virtual bool needs_raw_frame_buffer() const {
    return false;
  }


  virtual void evolve(const time delta) {
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

bool do_left = true;
void push_lights() {
  if (do_left) {
    for (int i = 0; i < PHYSICAL_LEDS; i++) {
      strip.setPixelColor(i, program->eval_uint(PHYSICAL_LEDS - 1 - i));
    }
    strip.show();
  } else {
    for (int i = PHYSICAL_LEDS; i < LOGICAL_LEDS; i++) {
      strip2.setPixelColor(i - PHYSICAL_LEDS, program->eval_uint(i));
    }
    strip2.show();
  }
  do_left = !do_left;
}


#endif

