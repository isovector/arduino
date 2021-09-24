#ifndef STROBE_H
#define STROBE_H

#include "Program.h"
#include "Fixed.h"

class Strobe : public Program {
public:
  Strobe(Program *c, int pos, time cycle_time, int falloff)
    : m_color_provider(c), m_pos(pos), m_time(0), m_cycle_time(cycle_time), m_falloff(falloff)
  {}

  ~Strobe() {
    delete m_color_provider;
  }

  CRGB eval(const int v) const {
    const CRGB color = m_color_provider->eval(v);

    int d = abs(v - m_pos);
    uint16_t brightness = 0;

    if (brightness > 0 && 0 <= d  && d < m_falloff) {
      return CRGB( (color.r * brightness) >> 8
                 , (color.g * brightness) >> 8
                 , (color.b * brightness) >> 8
                 );
    }

    return {0, 0, 0};
  }

  void evolve(time delta) {
    m_color_provider->evolve(delta);
    m_time += delta;
  }

private:
  Program *m_color_provider;
  int m_pos;
  time m_time;
  time m_cycle_time;
  int m_falloff;
};


#endif

