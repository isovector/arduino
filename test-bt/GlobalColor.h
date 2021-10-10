#ifndef GLOBAL_COLOR_H
#define GLOBAL_COLOR_H

#include "Program.h"

CRGB global_color = {0, 0, 0};
double global_intensity = 1;

class GlobalColor : public Program {
public:
  GlobalColor() : m_clock(0) {}

  ~GlobalColor() {
  }

  CRGB eval(int v) const {
    return CRGB( (uint8_t)((double)global_color.r * global_intensity)
               , (uint8_t)((double)global_color.g * global_intensity)
               , (uint8_t)((double)global_color.b * global_intensity)
               );
  }

  bool wants_draw() {
    if (m_clock == 0) {
      m_clock = 1;
      return true;
    }
    return false;
  }

  void evolve(time delta) {
  }

private:
  uint8_t m_clock = 0;
};

#endif

