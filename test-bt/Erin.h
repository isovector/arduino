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
  Erin()
    : m_counter(0), m_pos(0)
    , m_hue1(0), m_sat1(255), m_val1(255)
    , m_hue2(60000), m_sat2(255), m_val2(255)
    { }

  ~Erin() {
  }

  CRGB eval(const int v) const {
    uint16_t d = abs((int)m_pos - v);
    if (d > MIDPOINT) {
      d = ALL_LEDS - d;
    }


    if (v == m_pos) {
    return strip.Color( 0
                      , 255
                      , 0
                      );
    }

    // return strip.ColorHSV( lerp((uint16_t)0, m_hue1, (uint16_t)MIDPOINT, m_hue2, d)
    //                      , lerp((uint16_t)0, m_sat1, (uint16_t)MIDPOINT, m_sat2, d)
    //                      , lerp((uint16_t)0, m_val1, (uint16_t)MIDPOINT, m_val2, d)
    //                      );
    return strip.Color( lerp((uint16_t)0, 255, (uint16_t)MIDPOINT, 0, d)
                      , 0
                      , lerp((uint16_t)0, 0, (uint16_t)MIDPOINT, 255, d)
                      );
  }

  void evolve(const time delta) {
    m_counter += 1;
    if (m_counter % 4 == 0) {
      m_pos = (m_pos + 1) % ALL_LEDS;
    }
  }

private:
  uint8_t m_counter;
  uint16_t m_pos;
  uint16_t m_hue1; uint8_t m_sat1; uint8_t m_val1;
  uint16_t m_hue2; uint8_t m_sat2; uint8_t m_val2;
};


#endif

