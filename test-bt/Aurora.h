#ifndef AURORA_H
#define AURORA_H

#include "Program.h"

enum AuroraState
  { GO_LEFT = 0
  , GO_RIGHT
  , GO_OUT
  , GO_IN
  };


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

  CRGB makeZ(ufixed z) const {
    const int intensity = 48;
    const int threshold = intensity * 2;
    const int r = cos(z * 0.02) * intensity + intensity;
    const int g = sin(z * 0.03  + 10) * intensity + intensity;
    const int b = sin(z * 0.05) * intensity + intensity;

    return { (int)r
           , (int)g
           , (int)b
           };
  }


  void evolve(const time delta) {
    m_pos = (m_pos + 1) % LOGICAL_LEDS;
    m_phase++;
    m_data[m_pos] = makeZ(m_phase);
  }

private:
  CRGB m_data[LOGICAL_LEDS];
  uint16_t m_pos;
  ufixed m_phase;
};

#endif


