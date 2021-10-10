#ifndef BOUNCE_H
#define BOUNCE_H

#include <memory>
#include "Fixed.h"
#include "Program.h"

class Bounce : public Program {
public:
  Bounce(std::shared_ptr<Program> c, int pos, fixed dir, int tail)
    : m_color_provider(c), m_pos(itof(pos, 0)), m_vel(dir), m_tail(tail)
  {}

  ~Bounce() {
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

  Interval canvas() const {
    int pos = ftoi(m_pos);
    if (m_vel < 0) {
      return {pos, pos + m_tail};
    } else {
      return {pos - m_tail, pos};
    }
  }

  void evolve(time delta) {
    m_color_provider->evolve(delta);

    int pos = ftoi(m_pos);
    if (pos > LOGICAL_LEDS - 1 + m_tail) {
      m_pos = itof(LOGICAL_LEDS - 1, 0);
      m_vel = -abs(m_vel);
    } else if (pos < -m_tail) {
      m_pos = itof(0, 0);
      m_vel = abs(m_vel);
    }

    m_pos += fmul(m_vel, static_cast<fixed>(delta));
  }

private:
  std::shared_ptr<Program> m_color_provider;
  fixed m_pos;
  fixed m_vel;
  int m_tail;
};


#endif

