#include <FastLED.h>

#define POT_PIN 0
#define LED_PIN   7
#define NUM_LEDS  60
#define INTENSITY 48
#define SWITCH_PIN 10
#define THRESHOLD (INTENSITY * 2)
#define PROGRAMS 6

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
  programs[5] = &goCrazy;
}


float speed = 10;
int brightness = 1024;
int z = 0;
int p = 0;
bool needs_new_state = false;
int buttonPresses = 0;

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;
int buttonState = LOW;
int lastButtonState = LOW;



void setColor(CRGB* rgb) {
  int intensity = (int)(INTENSITY * ((float)brightness / 1024));
  int r = cos(z * 0.02) * intensity + intensity;
  int g = sin(z * 0.03  + 10) * intensity + intensity;
  int b = sin(z * 0.05) * intensity + intensity;
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

void next_program(float time, bool change) {
  delay((int)(time * speed));

  if (change) {
    needs_new_state = true;
    p = rand() % PROGRAMS;
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

  if (i % 2 == 0) {
    setColor(&leds[i / 2]);
  } else {
    setColor(&leds[NUM_LEDS - 1 - (i - 1) / 2]);
  }
  FastLED.show();
  i += 1;

  next_program(25, i > 60);
}

void goRandom() {
  static int i = 0;

  setup_state([&i]() {
      i = 0;
  });

  setColor(&leds[rand() % NUM_LEDS]);
  FastLED.show();
  i += 1;

  next_program(25, i > 59);
}

void goCrazy() {
  static int i = 0;

  setup_state([&i]() {
      i = 0;
  });

  z += 10;

  setColor(&leds[rand() % NUM_LEDS]);
  FastLED.show();
  i += 1;

  next_program(25, i > 59);
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
  handleDial();

  switch (buttonPresses % 3) {
    case 0:
      z++;
    case 1:
      programs[p]();
    case 2:
      break;
  }
}


