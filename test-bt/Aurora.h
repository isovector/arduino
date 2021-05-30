#ifndef AURORA_H
#define AURORA_H

#include "Program.h"


class Aurora : public Program {
public:
  Aurora()
    : m_pos(0), m_phase(0)
    {}

  ~Aurora() {
  }

  CRGB eval(const int v) const {
    return {0, 0, 0};
  }

  bool needs_raw_frame_buffer() const {
    return true;
  }

  void evolve(const time delta) {
    // m_pos = (m_pos + 3) % (3 * PHYSICAL_LEDS);
    m_pos = (rand() % PHYSICAL_LEDS) * 3;
    m_phase++;
    uint8_t *cs = *strip.getPixels();
    cs[m_pos] = strip.sine8(ftoi(10 + m_phase) * 4);
    cs[m_pos + 1] = strip.sine8(ftoi(50 + m_phase) * 6);
    cs[m_pos + 2] = strip.sine8(ftoi(150 + m_phase) * 14);
    delay(50);
  }

private:
  uint16_t m_pos;
  ufixed m_phase;
};

#endif


