#ifndef PROGRAM_H
#define PROGRAM_H

#include "FastLED.h"
#include "Fixed.h"

#define LOGICAL_LEDS (300 + 300 - 49 - BROKEN_LEDS)
#define PHYSICAL_LEDS 300
#define BROKEN_LEDS 54
#define LED1_PIN 6
#define LED2_PIN 14


CRGB leds1[PHYSICAL_LEDS];
CRGB leds2[LOGICAL_LEDS - PHYSICAL_LEDS];

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

  virtual Interval canvas() const {
    return {0, LOGICAL_LEDS};
  }

  virtual bool wants_draw() {
    return true;
  }

  virtual void evolve(const time delta) {
    return;
  }

  virtual void evolve(const float delta) {
    return;
  }

  virtual int identifier() const {
    return 0;
  }
};

Program *program = NULL;


void push_lights() {
  for (int i = 0; i < PHYSICAL_LEDS; i++) {
    leds1[i] = program->eval(PHYSICAL_LEDS - 1 - i);
  }
  for (int i = PHYSICAL_LEDS; i < LOGICAL_LEDS; i++) {
    leds2[i - PHYSICAL_LEDS] = program->eval(i);
  }
  FastLED.show();
}


#endif

