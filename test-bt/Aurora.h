#ifndef AURORA_H
#define AURORA_H

#include "Program.h"


class Aurora : public Program {
public:
  Aurora()
    : m_pos(0), m_phase(0)
    { }

  ~Aurora() {
  }

  CRGB eval(const int v) const {
    return m_data[v];
  }

  void evolve(const time delta) {
    m_pos = (m_pos + 1) % LOGICAL_LEDS;
    m_phase++;

    const int intensity = 48;
    const int threshold = intensity * 2;
    const int r = cos(m_phase * 0.02) * intensity + intensity;
    const int g = sin(m_phase * 0.03  + 10) * intensity + intensity;
    const int b = sin(m_phase * 0.05) * intensity + intensity;
    float div = 1;

    if (r + g + b > threshold) {
      div = 1.8;
    }

    m_data[m_pos] = { (int)(r / div)
                    , (int)(g / div)
                    , (int)(b / div)
                    };
  }

private:
  CRGB m_data[LOGICAL_LEDS];
  uint16_t m_pos;
  ufixed m_phase;
};

#endif


