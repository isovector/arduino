#include <FastLED.h>

#define LED_PIN   7
#define LED2_PIN   8
#define PHYSICAL_LEDS  300
#define BLYNK_USE_DIRECT_CONNECT

#define LOGICAL_LEDS (300 + 300 - 49)

// You could use a spare Hardware Serial on boards that have it (like Mega)
#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(0, 1); // RX, TX

#define BLYNK_PRINT DebugSerial
#include <BlynkSimpleSerialBLE.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "M4YDcZwmQbXDBBIy4zFkg4u-9I_YlS6r";

#define SINE_SIZE 128

const uint8_t sine_table[SINE_SIZE] =
  { 0x80,0x86,0x8c,0x92,0x98,0x9e,0xa5,0xaa,
    0xb0,0xb6,0xbc,0xc1,0xc6,0xcb,0xd0,0xd5,
    0xda,0xde,0xe2,0xe6,0xea,0xed,0xf0,0xf3,
    0xf5,0xf8,0xfa,0xfb,0xfd,0xfe,0xfe,0xff,
    0xff,0xff,0xfe,0xfe,0xfd,0xfb,0xfa,0xf8,
    0xf5,0xf3,0xf0,0xed,0xea,0xe6,0xe2,0xde,
    0xda,0xd5,0xd0,0xcb,0xc6,0xc1,0xbc,0xb6,
    0xb0,0xaa,0xa5,0x9e,0x98,0x92,0x8c,0x86,
    0x80,0x79,0x73,0x6d,0x67,0x61,0x5a,0x55,
    0x4f,0x49,0x43,0x3e,0x39,0x34,0x2f,0x2a,
    0x25,0x21,0x1d,0x19,0x15,0x12,0xf,0xc,
    0xa,0x7,0x5,0x4,0x2,0x1,0x1,0x0,
    0x0,0x0,0x1,0x1,0x2,0x4,0x5,0x7,
    0xa,0xc,0xf,0x12,0x15,0x19,0x1d,0x21,
    0x25,0x2a,0x2f,0x34,0x39,0x3e,0x43,0x49,
    0x4f,0x55,0x5a,0x61,0x67,0x6d,0x73,0x79
  };






CRGB leds[PHYSICAL_LEDS];
CRGB color = CRGB(255, 0, 0);

int intensity = 255;

typedef int16_t fixed;
typedef uint16_t ufixed;
typedef uint16_t time;


class Program {
public:
  virtual ~Program() {}

  virtual CRGB eval(int v) const {
    return CRGB(0, 0, 0);
  };

  virtual void evolve(time delta) {
    return;
  }
};

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

uint16_t sine(time f) {
  return sine_table[ftoi(f) % SINE_SIZE];
}

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
    int brightness = sine(fmul(m_time, m_cycle_time));

    if (brightness > 0 && 0 <= d  && d < m_falloff) {
      return CRGB( color.r * 255 / brightness
                 , color.g * 255 / brightness
                 , color.b * 255 / brightness
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

class GlobalColor : public Program {
public:
  GlobalColor() {}

  ~GlobalColor() {
  }

  CRGB eval(int v) const {
    return color;
  }

  void evolve(time delta) {
  }
};


class ConstColor : public Program {
public:
  ConstColor(const CRGB& c) : m_color(c)
    {}

  ~ConstColor() {
  }

  CRGB eval(int v) const {
    return m_color;
  }

  void evolve(time delta) {
  }

private:
  CRGB m_color;
};

Program *const_color(CRGB rgb) {
  return new ConstColor(rgb);
}


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


class Bounce : public Program {
public:
  Bounce(Program *c, int pos, fixed dir, int tail)
    : m_color_provider(c), m_pos(itof(pos, 0)), m_vel(dir), m_tail(tail)
  {}

  ~Bounce() {
    delete m_color_provider;
  }

  CRGB eval(const int v) const {
    const CRGB color = m_color_provider->eval(v);
    const int pos = ftoi(m_pos);
    if (pos == v) {
      return color;
    }

    const int d = (pos - v) * ((m_vel >= 0) ? 1 : -1);
    if (d >= 0 && d < m_tail) {
      return CRGB(color.r / d, color.g / d, color.b / d);
    }

    return CRGB(0, 0, 0);
  }

  void evolve(time delta) {
    m_color_provider->evolve(delta);

    int pos = ftoi(m_pos);
    if (pos > LOGICAL_LEDS - 1) {
      m_pos = itof(LOGICAL_LEDS - 1, 0);
      m_vel = -abs(m_vel);
    } else if (pos < 0) {
      m_pos = itof(0, 0);
      m_vel = abs(m_vel);
    }

    m_pos += fmul(m_vel, static_cast<fixed>(delta));
  }

private:
  Program *m_color_provider;
  fixed m_pos;
  fixed m_vel;
  int m_tail;
};

Program *programs[10];
Program *program;



void setup()
{
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, PHYSICAL_LEDS);
  FastLED.addLeds<WS2812, LED2_PIN, GRB>(leds, PHYSICAL_LEDS);

  // Debug console
  DebugSerial.begin(9600);

  DebugSerial.println("Waiting for connections...");

  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);

  programs[0] = new GlobalColor();
  /* programs[0] = new Strobe(new GlobalColor(), 100, itof(5, 0), 10); */
  /* programs[1] = new Bounce(const_color({0, 0xaa, 0xcc}), 80, -itof(15, 0), 10); */
/*   programs[3] = new Bounce(const_color({255, 128, 0}), LOGICAL_LEDS - 1, -itof(50, 500), 30); */
/*   programs[4] = new Bounce(evolve_color(0, 10, 20, 30, 2), LOGICAL_LEDS - 1, -itof(20, 500), 16); */
/*   programs[5] = new Bounce(evolve_color(0, 70, 40, 10, 1), LOGICAL_LEDS - 1, -itof(28, 500), 30); */
/*   programs[6] = new Bounce(evolve_color(0, 16, 17, 18, 0), LOGICAL_LEDS - 1, -itof(14, 500), 4); */

  program = new Many<1>(programs);

  Blynk.begin(Serial, auth);
}


// run the function to generate every light on the logical strip
// since 0 on the physical strips is in the middle of the room, we map idx 0 to
// (PHYSICAL_LEDS - 1) on strip 1
//
// due to memory restrictions, we have to use the same buffer to push data to each
// strip, so this must be a pure function
void push_lights() {
  for (int i = 0; i < PHYSICAL_LEDS; i++) {
    leds[i] = program->eval(PHYSICAL_LEDS - 1 - i);
  }
  FastLED[0].showLeds(intensity);

  for (int i = PHYSICAL_LEDS; i < LOGICAL_LEDS; i++) {
    leds[i - PHYSICAL_LEDS] = program->eval(i);
  }
  FastLED[1].showLeds(intensity);
}



BLYNK_WRITE(V2)
{
  int r = param[0].asInt();
  int g = param[1].asInt();
  int b = param[2].asInt();
  color = CRGB(r, g, b);
  push_lights();
  // setColor();
}

BLYNK_WRITE(V3)
{
  intensity = param.asInt();
  push_lights();
  // setColor();
}

unsigned long last_time;

void loop()
{
  /* push_lights(); */

  /* unsigned long now = millis(); */
  /* time delta = max(1, milli_diff(now, last_time)); */
  /* program->evolve(delta); */
  /* last_time = now; */

  Blynk.run();
}


