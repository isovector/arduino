#include <FastLED.h>

#define SPACING 25
#define POT_PIN 0
#define LED_PIN   7
#define CLOCK_PIN 5
#define LOGICAL_LEDS  ((int)(NUM_LEDS / SCALE_FACTOR))
#define NUM_LEDS  90
#define INTENSITY 48
#define SWITCH_PIN 2
#define THRESHOLD (INTENSITY * 2)
#define PROGRAMS 5
#define SCALE_FACTOR 1.5

typedef void (*StateFn)();

CRGB leds[NUM_LEDS];
StateFn programs[PROGRAMS];


void setup() {
  //FastLED.addLeds<DOTSTAR, LED_PIN, CLOCK_PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);

  programs[0] = &goLeft;
  programs[1] = &goIn;
  programs[2] = &goOut;
  programs[3] = &goRight;
  programs[4] = &sdf;
  //programs[4] = &goRandom;
  //programs[5] = &goCrazy;

  pinMode(SWITCH_PIN, INPUT);
}

bool on = true;
float speed = 8;
int brightness = 1024;
int z = 0;
int p = 0;
bool needs_new_state = false;
int buttonPresses = 0;

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 5;
int buttonState = LOW;
int lastButtonState = LOW;

int (*sdf_func)(int, int);
int sdf_time = 500;


void setColorZ(int i, int z) {
  CRGB* rgb = &leds[(int)((float)i * SCALE_FACTOR)];
  int intensity = (int)(INTENSITY * ((float)brightness / 1024));
  int r = cos(z * 0.02) * intensity + intensity;
  int g = sin(z * 0.03  + 10) * intensity + intensity;
  int b = sin(z * 0.05) * intensity + intensity;
  float div = 1;

  if (r + g + b > THRESHOLD) {
    div = 1.8;
  }

  if (on) {
    *rgb = CRGB((int)(r / div), (int)(g / div), (int)(b / div));
  } else {
    *rgb = CRGB(0, 0, 0);
  }
}

void setColor(int i) {
  setColorZ(i, z);
}

void setup_state(void (*func)()) {
  if (needs_new_state) {
    func();
    needs_new_state = false;
  }
}

void next_program(float time, bool change) {
  delay((int)(time * speed));

  if (change) {
    needs_new_state = true;
    p = rand() % PROGRAMS;
  }
}

void all() {
  for (int i = 0; i < LOGICAL_LEDS; i++) {
    setColor(i);
  }
  FastLED.show();
  delay(10);
}

int some_func(int i, int t) {
  double freq = 0.25;
  double phase = 0;
  return 5 * cos(freq * t * (t - i / SPACING));
}

void sdf() {
  for (int time = 0; time < sdf_time; time++) {
    for (int i = 0; i < LOGICAL_LEDS; i++) {
      setColorZ(i, z + some_func(i, time));
    }
    delay(10);
  }

  next_program(0, true);
}

void goLeft() {
  static int i = 0;

  setup_state([&i]() {
    i = 0;
  });

  setColor(i);
  FastLED.show();
  i += 1;

  next_program(25, i > LOGICAL_LEDS - 1);
}


void goRight() {
  static int i = LOGICAL_LEDS - 1;

  setup_state([&i]() {
    i = LOGICAL_LEDS - 1;
  });

  setColor(i);
  FastLED.show();
  i -= 1;

  next_program(25, i < 0);
}

void goOut() {
  static int i = LOGICAL_LEDS / 2;

  setup_state([&i]() {
    i = LOGICAL_LEDS / 2;
  });

  setColor(i);
  setColor(LOGICAL_LEDS - i - 1);
  FastLED.show();
  i += 1;

  next_program(50, i > LOGICAL_LEDS - 1);
}

void goIn() {
  static int i = 0;

  setup_state([&i]() {
    i = 0;
  });

  if (i % 2 == 0) {
    setColor(i / 2);
  } else {
    setColor(LOGICAL_LEDS - 1 - (i - 1) / 2);
  }
  FastLED.show();
  i += 1;

  next_program(25, i > LOGICAL_LEDS);
}

void goRandom() {
  static int i = 0;

  setup_state([&i]() {
    i = 0;
  });

  setColor(rand() % LOGICAL_LEDS);
  FastLED.show();
  i += 1;

  next_program(25, i > LOGICAL_LEDS - 1);
}

void goCrazy() {
  static int i = 0;

  setup_state([&i]() {
    i = 0;
  });

  z += 10;

  setColor(rand() % LOGICAL_LEDS);
  FastLED.show();
  i += 1;

  next_program(25, i > LOGICAL_LEDS - 1);
}

void handleButton() {
  int reading = digitalRead(SWITCH_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {
        buttonPresses++;
        //on = !on;
      }
    }
  }

  lastButtonState = reading;
}

void handleDial() {
  int dial = analogRead(POT_PIN);
  if (dial < 512) {
    brightness = dial * 2;
    speed = 10;
  } else {
    speed = 1 + (1024 - dial) / 512;
  }
}

void loop() {
  handleButton();
//  handleDial();

  switch (buttonPresses % 3) {
    case 0:
      on = true;
      z++;
      programs[p]();
      break;
    case 1:
      break;
     case 2:
      on = false;
      programs[p]();
      break;
  }
}
