#ifndef FIXED_H
#define FIXED_H

typedef int16_t fixed;
typedef uint16_t ufixed;
typedef uint16_t time;

int16_t ftoi(fixed f) {
  return f >> 4;
}

uint16_t ftoi(ufixed f) {
  return f >> 4;
}

// small is out of 1024
fixed itof(int big, int small) {
  return (big << 4) | (small >> 6);
}

template<typename T>
T fmul(T a, T b) {
  // each fixed contributes 4 fixed digits, but we only want 4,
  // so elimiate the bottom 4
  return (a * b) >> 4;
}

time milli_diff(unsigned long a, unsigned long b) {
  // want to divide by 1000, which is roughly 2^10
  // since the fixed point is 4, we need to chop off 6 bits
  return (a - b) >> 6;
}


#endif
