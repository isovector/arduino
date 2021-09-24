#ifndef GLOBAL_COLOR_H
#define GLOBAL_COLOR_H

#include "Program.h"

CRGB global_color = {0, 0, 0};
double global_intensity = 1;

class GlobalColor : public Program {
public:
  GlobalColor() {}

  ~GlobalColor() {
  }

  CRGB eval(int v) const {
    return CRGB( (uint8_t)((double)global_color.r * global_intensity)
               , (uint8_t)((double)global_color.g * global_intensity)
               , (uint8_t)((double)global_color.b * global_intensity)
               );
  }

  void evolve(time delta) {
  }
};

#endif

