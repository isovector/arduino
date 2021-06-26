#ifndef MANY_H
#define MANY_H

#include "Program.h"

template <int N>
class Many : public Program {
public:
  Many() : m_count(0), m_color({0, 127, 0}) {
    for (int i = 0; i < N; i++) {
      m_programs[i] = NULL;
    }
  }

  ~Many() {
    for (int i = 0; i < N; i++) {
      if (!m_programs[i]) break;
      delete m_programs[i];
    }
  }

  CRGB eval(const int v) const {
    uint16_t r = 0, g = 0, b = 0;

    for (int p = 0; p < m_count; p++) {
      if (!m_programs[p]) break;
      Interval i = m_programs[p]->canvas();

      if (!(i.start <= v && v <= i.end)) {
        continue;
      }

      CRGB rgb = m_programs[p]->eval(v);
      r += rgb.r;
      g += rgb.g;
      b += rgb.b;
    }

    return
      CRGB( min(255, r)
          , min(255, g)
          , min(255, b)
          );
  }

  void add(Program *p) {
    if (m_count < N) {
      m_programs[m_count++] = p;
    }
  }

  void evolve(const time delta) {
    for (int p = 0; p < N; p++) {
      if (!m_programs[p]) break;

      m_programs[p]->evolve(delta);
    }
  }

private:
  CRGB m_color;
  Program *m_programs[N];
  uint8_t m_count;
};


#endif

