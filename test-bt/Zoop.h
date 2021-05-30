#ifndef ZOOP_H
#define ZOOP_H

#include "Fixed.h"
#include "Program.h"

class Zoop : public Program {
public:
  Zoop(Program *c, int pos, fixed dir, int tail)
    : m_color_provider(c), m_pos(itof(pos, 0)), m_vel(dir), m_tail(tail)
  {}

  ~Zoop() {
    delete m_color_provider;
  }

  CRGB eval(const int v) const {
    const CRGB color = m_color_provider->eval(v);
    const int pos = ftoi(m_pos);
    if (pos == v) {
      return color;
    }

    const int d = (pos - v) * ((m_vel >= 0) ? 1 : -1);
    if (d >= 0 && d < m_tail) {
      return CRGB(color.r / d, color.g / d, color.b / d);
    }

    return CRGB(0, 0, 0);
  }

  void evolve(time delta) {
    m_color_provider->evolve(delta);

    int pos = ftoi(m_pos);
    if (pos > LOGICAL_LEDS - 1 + m_tail) {
      m_pos = itof(0, 0);
    } else if (pos < -m_tail) {
      m_pos = itof(LOGICAL_LEDS - 1, 0);
    }

    m_pos += fmul(m_vel, static_cast<fixed>(delta));
  }

private:
  Program *m_color_provider;
  fixed m_pos;
  fixed m_vel;
  int m_tail;
};


#endif

