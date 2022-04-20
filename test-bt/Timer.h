#ifndef TIMER_H
#define TIMER_H

#include "Program.h"

const int TIMER_ID = 17;
float duration = 30;

class Timer : public Program {
public:
  Timer() : m_time(0) { }

  ~Timer() {
  }

  CRGB eval(const int v) const {
    const int to = m_time / duration * LOGICAL_LEDS;
    if (m_time >= duration) {
      if (((int)(m_time * 2) - (int)duration) % 2 == 1) {
        return {0, 0, 0};
      }
      return {255, 0, 0};
    }
    if (v <= to) {
      return {128, 128, 128};
    }
    return {0, 0, 0};
  }

  void evolve(const float delta) {
    m_time += delta;
  }

  int identifier() const {
    return TIMER_ID;
  }

private:
  float m_time;
};

#endif


