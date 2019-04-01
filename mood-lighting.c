#include <FastLED.h>

#define LED_PIN   7
#define NUM_LEDS  60
#define INTENSITY 64
#define THRESHOLD (INTENSITY * 2)
#define PROGRAMS 5
#define SPEED 10

typedef void (*StateFn)();

CRGB leds[NUM_LEDS];
StateFn programs[PROGRAMS];


void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);

  programs[0] = &goLeft;
  programs[1] = &goRight;
  programs[2] = &goOut;
  programs[3] = &goIn;
  programs[4] = &goRandom;
}


int z = 0;
int p = 0;
bool needs_new_state = false;


void setColor(CRGB* rgb) {
  int r = cos(z * 0.02) * INTENSITY + INTENSITY;
  int g = sin(z * 0.03  + 10) * INTENSITY + INTENSITY;
  int b = sin(z * 0.05) * INTENSITY + INTENSITY;
  float div = 1;

  if (r + g + b > THRESHOLD) {
    div = 1.8;
  }

  *rgb = CRGB((int)(r / div), (int)(g / div), (int)(b / div));
}

void setup_state(void (*func)()) {
  if (needs_new_state) {
    func();
    needs_new_state = false;
  }
}

void next_program(int time, bool change) {
  delay(time * SPEED);

  if (change) {
    needs_new_state = true;
    p += 17;
    p %= PROGRAMS;
  }
}

void goLeft() {
  static int i = 0;

  setup_state([&i]() {
      i = 0;
  });

  setColor(&leds[i]);
  FastLED.show();
  i += 1;

  next_program(25, i > 59);
}

void goRight() {
  static int i = 59;

  setup_state([&i]() {
      i = 59;
  });

  setColor(&leds[i]);
  FastLED.show();
  i -= 1;

  next_program(25, i < 0);
}

void goOut() {
  static int i = 30;

  setup_state([&i]() {
      i = 30;
  });

  setColor(&leds[i]);
  setColor(&leds[NUM_LEDS - i - 1]);
  FastLED.show();
  i += 1;

  next_program(50, i > 59);
}

void goIn() {
  static int i = 0;

  setup_state([&i]() {
      i = 0;
  });

  setColor(&leds[i]);
  setColor(&leds[NUM_LEDS - i - 1]);
  FastLED.show();
  i += 1;

  next_program(50, i > 30);
}

void goRandom() {
  static int i = 0;

  setup_state([&i]() {
      i = 0;
  });

  setColor(&leds[rand() % NUM_LEDS]);
  FastLED.show();
  i += 1;

  next_program(50, i > 59);
}

void loop() {
  z++;
  programs[p]();
}


