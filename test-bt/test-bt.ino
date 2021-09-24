#include "ConstColor.h"
#include "EvolveColor.h"
#include "Many.h"
#include "Bounce.h"
#include "Strobe.h"
#include "Aurora.h"
#include "GlobalColor.h"
#include "Erin.h"

#include "Remote.h"







void setNexus() {
  delete program;
  Many<7> *many = new Many<7>();
  /* many->add(evolve_color(0, 160, 255, 5, 120)); */
  many->add(new Aurora());
  Program *ec = evolve_color(0, 1, 255, 200, 10);
  many->add(new Bounce(ec, 100, -itof(5, 15), 30));
  many->add(new Bounce(ec, 140, -itof(7, 15), 30));
  many->add(new Bounce(ec, 180, -itof(5, 0), 30));
  many->add(new Bounce(ec, 220, -itof(7, 0), 30));
  many->add(new Bounce(ec, 260, -itof(11, 0), 30));
  many->add(new Bounce(ec, 300, -itof(13, 0), 30));
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


unsigned long last_time = 0;

void setup() {
  FastLED.addLeds<1, WS2811, LED_PIN, GRB>(leds1, PHYSICAL_LEDS);
  FastLED.addLeds<1, WS2811, LED_PIN + 2, GRB>(leds2, LOGICAL_LEDS - PHYSICAL_LEDS);
  memset(leds1, 0, sizeof(leds1));
  memset(leds2, 0, sizeof(leds2));

  Serial.begin(9600);
  irrecv.enableIRIn();

  program = new GlobalColor();
  setNexus();
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
    Serial.println(results.value, HEX);
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
      case 0xFFB24D: // Purple
        delete program;
        program = new Erin(160, 192);
        break;
      case 0xFF3AC5: // brightness up
        global_intensity = min(1, global_intensity + 0.05);
        break;
      case 0xFFBA45: // brightness down
        global_intensity = max(0, global_intensity - 0.05);
        break;
    }
    irrecv.resume();
  }

  unsigned long now = millis();
  time delta = max(1, milli_diff(now, last_time));
  program->evolve(delta);
  last_time = now;

  push_lights();
}

