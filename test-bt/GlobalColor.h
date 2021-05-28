#ifndef GLOBAL_COLOR_H
#define GLOBAL_COLOR_H

#include "Program.h"

CRGB global_color = {0, 0, 0};

class GlobalColor : public Program {
public:
  GlobalColor() {}

  ~GlobalColor() {
  }

  CRGB eval(int v) const {
    return global_color;
  }

  void evolve(time delta) {
  }
};

#endif

