#ifndef ERIN_H
#define ERIN_H

#define BONUS_LEDS 0
#define ALL_LEDS (LOGICAL_LEDS + BONUS_LEDS)
#define MIDPOINT (ALL_LEDS / 2)

#include "Program.h"

template<typename X, typename Y>
Y lerp(X lo, Y lo_val, X hi, Y hi_val, X pos) {
  double perc = (double)(pos - lo) / (double)(hi - lo);
  return lo_val + (double)(hi_val - lo_val) * perc;
}

class Erin : public Program {
public:
  Erin(uint8_t lo, uint8_t hi)
    : m_counter(0), m_pos(0)
    , m_hue1(lo), m_sat1(255), m_val1(50)
    , m_hue2(hi), m_sat2(255), m_val2(50)
    { }

  ~Erin() {
  }

  CRGB eval(const int v) const {
    uint8_t d = abs((int)m_pos - v);
    if (d > MIDPOINT) {
      d = ALL_LEDS - d;
    }

    return CHSV( lerp((uint8_t)0, m_hue1, (uint8_t)MIDPOINT, m_hue2, d)
               , lerp((uint8_t)0, m_sat1, (uint8_t)MIDPOINT, m_sat2, d)
               , lerp((uint8_t)0, m_val1, (uint8_t)MIDPOINT, m_val2, d)
               );
    // return strip.Color( lerp((uint8_t)0, 255, (uint8_t)MIDPOINT, 0, d)
    //                   , 0
    //                   , lerp((uint8_t)0, 0, (uint8_t)MIDPOINT, 255, d)
    //                   );
  }

  void evolve(const time delta) {
    m_counter += 1;
    if (m_counter % 32 == 0) {
      m_pos = (m_pos + random(-2, 4)) % ALL_LEDS;
    }
  }

private:
  uint8_t m_counter;
  uint8_t m_pos;
  uint8_t m_hue1; uint8_t m_sat1; uint8_t m_val1;
  uint8_t m_hue2; uint8_t m_sat2; uint8_t m_val2;
};


#endif

