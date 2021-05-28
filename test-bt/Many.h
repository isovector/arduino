#ifndef MANY_H
#define MANY_H

#include "Program.h"

template <int N>
class Many : public Program {
public:
  Many(Program *p[N]) {
    for (int i = 0; i < N; i++) {
      m_programs[i] = p[i];
    }
  }

  ~Many() {
    for (int i = 0; i < N; i++) {
      delete m_programs[i];
    }
  }

  CRGB eval(int v) const {
    int r = 0, g = 0, b = 0;

    for (int p = 0; p < N; p++) {
      if (!m_programs[p]) break;

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

  void evolve(time delta) {
    for (int p = 0; p < N; p++) {
      if (!m_programs[p]) break;

      m_programs[p]->evolve(delta);
    }
  }

private:
    Program *m_programs[N];
};


#endif

