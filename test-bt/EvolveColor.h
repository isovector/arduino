#ifndef EVOLVE_COLOR_H
#define EVOLVE_COLOR_H

#include "Fixed.h"
#include "Program.h"
#include "Sine.h"


class EvolveColor : public Program {
public:
  EvolveColor(ufixed phase, ufixed r, ufixed g, ufixed b, int v_factor)
    : m_phase(phase), m_r(r), m_g(g), m_b(b), m_v_factor(v_factor)
    {}

  ~EvolveColor() {
  }

  CRGB eval(int v) const {
    // TODO(sandy): should use sine
    int r = sine_table[(ftoi(fmul(m_phase, m_r)) + v * m_v_factor) % SINE_SIZE];
    int g = sine_table[(ftoi(fmul(m_phase, m_g)) + v * m_v_factor) % SINE_SIZE];
    int b = sine_table[(ftoi(fmul(m_phase, m_b)) + v * m_v_factor) % SINE_SIZE];

    return CRGB(r, g, b);
  }

  void evolve(fixed delta) {
    m_phase += delta;
  }

private:
  ufixed m_phase;
  ufixed m_r;
  ufixed m_g;
  ufixed m_b;
  int m_v_factor;
};

Program *evolve_color(ufixed p, ufixed r, ufixed g, ufixed b, int v_factor) {
  return new EvolveColor(p, r, g, b, v_factor);
}


#endif

