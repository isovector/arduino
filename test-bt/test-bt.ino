#include <memory>

#include "ConstColor.h"
#include "EvolveColor.h"
#include "Many.h"
#include "Bounce.h"
#include "Strobe.h"
#include "Aurora.h"
#include "GlobalColor.h"
#include "Erin.h"
#include "Timer.h"

#include "Remote.h"







void setNexus() {
  Many<7> *many = new Many<7>();
  /* many->add(evolve_color(0, 160, 255, 5, 120)); */
  many->add(new Aurora());
  std::shared_ptr<Program> ec(evolve_color(0, 1, 255, 200, 10));
  many->add(new Bounce(ec, 100, -itof(5, 15), 30));
  many->add(new Bounce(ec, 140, -itof(7, 15), 30));
  many->add(new Bounce(ec, 180, -itof(5, 0), 30));
  many->add(new Bounce(ec, 220, -itof(7, 0), 30));
  many->add(new Bounce(ec, 260, -itof(11, 0), 30));
  many->add(new Bounce(ec, 300, -itof(13, 0), 30));
  delete program;
  program = many;
}

void setAurora() {
  delete program;
  program = new Aurora();
}

void setGlobal() {
  delete program;
  program = new GlobalColor();
}

void setTimer() {
  if (program->identifier() == TIMER_ID) {
    duration += 60;
  } else {
    duration = 60;
    delete program;
    program = new Timer();
  }
}


unsigned long last_time = 0;

void setup() {
  Serial.begin(9600);

  FastLED.addLeds<1, WS2812, LED1_PIN, GRB>(leds1, PHYSICAL_LEDS);
  FastLED.addLeds<1, WS2812, LED2_PIN, GRB>(leds2, LOGICAL_LEDS - PHYSICAL_LEDS);
  FastLED.setMaxRefreshRate(60);

  irrecv.enableIRIn();

  memset(leds1, 0, sizeof(leds1));
  memset(leds2, 0, sizeof(leds2));

  pinMode(20, OUTPUT);
  digitalWrite(20, LOW);
  pinMode(21, OUTPUT);
  digitalWrite(21, HIGH);
  pinMode(22, INPUT);

  program = new Timer();
  global_color = CRGB(255, 0, 0);
  /* setGlobal(); */
  /* setNexus(); */
  /* setAurora(); */
  last_time = millis();

/*   // chair */
/*   many->add(new Strobe(const_color({255, 0, 0}), 160, 1, 60)); */
/*   // chair */
/*   many->add(new Strobe(const_color({0, 255, 0}), 30, 1, 30)); */
/*   // couch r */
/*   many->add(new Strobe(const_color({0, 0, 255}), 430, 1, 30)); */
/*   // couch l */
/*   many->add(new Strobe(const_color({255, 0, 0}), 500, 1, 30)); */
}


decode_results results;
uint32_t last_ir_code;

void loop() {
  if (irrecv.decode(&results)) {
    if (results.value != 0xFFFFFFFF) {
      last_ir_code = results.value;
    } else {
      results.value = last_ir_code;
    }
    /* Serial.println(results.value, HEX); */
    switch (results.value) {
      case 0xFF02FD: // off
        global_color = CRGB(0, 0, 0);
        setGlobal();
        break;
      case 0xFF1AE5: // R
        global_color = CRGB(255, 0, 0);
        setGlobal();
        break;
      case 0xFF9A65: // G
        global_color = CRGB(0, 255, 0);
        setGlobal();
        break;
      case 0xFFA25D: // B
        global_color = CRGB(0, 0, 255);
        setGlobal();
        break;
      case 0xFF22DD: // W
        global_color = CRGB(255, 255, 255);
        setGlobal();
        break;
      case 0xFFF00F: // Auto
        setNexus();
        break;
      case 0xFFD02F: // flash
        setTimer();
        break;
      case 0xFFB24D: // Purple
        delete program;
        program = new Erin(160, 208);
        break;
      case 0xFF3AC5: // brightness up
        global_intensity = min(1, global_intensity + 0.05);
        break;
      case 0xFFBA45: // brightness down
        global_intensity = max(0, global_intensity - 0.05);
        break;
      case 0xFF28D7: // red up
        global_color.r = (uint8_t)min(255, (int)global_color.r + 5);
        break;
      case 0xFF08F7: // red down
        global_color.r = (uint8_t)max(0, (int)global_color.r - 5);
        break;
      case 0xFFA857: // green up
        global_color.g = (uint8_t)min(255, (int)global_color.g + 5);
        break;
      case 0xFF8877: // green down
        global_color.g = (uint8_t)max(0, (int)global_color.g - 5);
        break;
      case 0xFF6897: // blue up
        global_color.b = (uint8_t)min(255, (int)global_color.b + 5);
        break;
      case 0xFF48B7: // blue down
        global_color.b = (uint8_t)max(0, (int)global_color.b - 5);
        break;
    }
    irrecv.resume();
  }

  unsigned long now = millis();
  float dt = (float)(now - last_time) / 1000;
  time delta = max(1, milli_diff(now, last_time));
  program->evolve(delta);
  program->evolve(dt);
  last_time = now;

  /* if (program->wants_draw()) { */
  /*   Serial.println("drawing"); */
    push_lights();
  /* } */

  /* delay(10000); */
}

