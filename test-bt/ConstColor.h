#ifndef CONST_COLOR_H
#define CONST_COLOR_H

#include "Program.h"


class ConstColor : public Program {
public:
  ConstColor(const CRGB& c) : m_color(c)
    {}

  ~ConstColor() {
  }

  CRGB eval(int v) const {
    return m_color;
  }

  void evolve(time delta) {
  }

private:
  CRGB m_color;
};

Program *const_color(CRGB rgb) {
  return new ConstColor(rgb);
}


#endif

