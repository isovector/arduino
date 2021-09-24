#ifndef EVOLVE_COLOR_H
#define EVOLVE_COLOR_H

#include "Fixed.h"
#include "Program.h"


class EvolveColor : public Program {
public:
  EvolveColor(uint8_t phase, uint8_t speed, uint8_t sat, uint8_t val, uint8_t v_factor)
    : m_phase(phase), m_speed(speed), m_sat(sat), m_val(val), m_v_factor(v_factor)
    {}

  ~EvolveColor() {
  }

  CRGB eval(const int v) const {
    return CHSV(m_phase + m_v_factor * v, m_sat, m_val);
  }

  void evolve(const time delta) {
    m_phase += delta * m_speed;
  }

private:
  uint8_t m_phase;
  uint8_t m_speed;
  uint8_t m_sat;
  uint8_t m_val;
  uint8_t m_v_factor;
};

Program *evolve_color(uint8_t phase, uint8_t speed, uint8_t sat, uint8_t val, uint8_t v_factor) {
  return new EvolveColor(phase, speed, sat, val, v_factor);
}


#endif

