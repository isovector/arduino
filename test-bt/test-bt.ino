#include "Adafruit_NeoPixel.h"

#include "ConstColor.h"
#include "EvolveColor.h"
#include "Many.h"
#include "Bounce.h"
#include "Strobe.h"

unsigned long last_time = 0;

void setup() {
  last_time = millis();

  Many<4> *many = new Many<4>();
  /* many->add(const_color({0, 0, 0xff})); */
  /* many->add(evolve_color(0, 160, 255, 128, 100)); */
  /* many->add(new Bounce(evolve_color(0, 160, 255, 128, 100), 100, -itof(15, 0), 10)); */

  // chair
  many->add(new Strobe(const_color({255, 0, 0}), 160, 1, 60));
  // chair
  many->add(new Strobe(const_color({0, 255, 0}), 30, 1, 30));
  // couch r
  many->add(new Strobe(const_color({0, 0, 255}), 430, 1, 30));
  // couch l
  many->add(new Strobe(const_color({255, 0, 0}), 500, 1, 30));

  program = many;

  strip.begin();
  strip2.begin();
}

void loop() {
  unsigned long now = millis();
  time delta = max(1, milli_diff(now, last_time));
  program->evolve(delta);
  last_time = now;
  push_lights();

}




/* #include <FastLED.h> */

/* #define LED_PIN   7 */
/* #define LED2_PIN   8 */
/* #define PHYSICAL_LEDS  300 */
/* #define BLYNK_USE_DIRECT_CONNECT */

/* #define LOGICAL_LEDS (300 + 300 - 49) */

/* // You could use a spare Hardware Serial on boards that have it (like Mega) */
/* #include <SoftwareSerial.h> */
/* SoftwareSerial DebugSerial(0, 1); // RX, TX */

/* #define BLYNK_PRINT DebugSerial */
/* #include <BlynkSimpleSerialBLE.h> */

/* // You should get Auth Token in the Blynk App. */
/* // Go to the Project Settings (nut icon). */
/* char auth[] = "M4YDcZwmQbXDBBIy4zFkg4u-9I_YlS6r"; */

/* /1* BlynkTimer timer; *1/ */







/* CRGB leds[PHYSICAL_LEDS]; */
/* CRGB color = CRGB(255, 0, 0); */

/* int intensity = 255; */












/* Program *programs[10]; */



/* void tick() { */
/*   push_lights(); */

/*   unsigned long now = millis(); */
/*   time delta = max(1, milli_diff(now, last_time)); */
/*   program->evolve(delta); */
/*   last_time = now; */
/* } */


/* void setup() */
/* { */
/*   FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, PHYSICAL_LEDS); */
/*   FastLED.addLeds<WS2812, LED2_PIN, GRB>(leds, PHYSICAL_LEDS); */

/*   // Debug console */
/*   DebugSerial.begin(9600); */

/*   DebugSerial.println("Waiting for connections..."); */

/*   pinMode(7, OUTPUT); */
/*   pinMode(8, OUTPUT); */

/*   programs[0] = new GlobalColor(); */
/*   programs[1] = new Bounce(const_color({0, 0xaa, 0xcc}), 80, -itof(15, 0), 10); */
/*   programs[2] = new Bounce(const_color({255, 128, 0}), LOGICAL_LEDS - 1, -itof(50, 500), 30); */
/* /1*   programs[4] = new Bounce(evolve_color(0, 10, 20, 30, 2), LOGICAL_LEDS - 1, -itof(20, 500), 16); *1/ */
/* /1*   programs[5] = new Bounce(evolve_color(0, 70, 40, 10, 1), LOGICAL_LEDS - 1, -itof(28, 500), 30); *1/ */
/* /1*   programs[6] = new Bounce(evolve_color(0, 16, 17, 18, 0), LOGICAL_LEDS - 1, -itof(14, 500), 4); *1/ */

/*   program = new Many<3>(programs); */

/*   Serial.begin(9600); */
/*   Blynk.begin(Serial, auth); */
/*   /1* timer.setInterval(62, tick); *1/ */
/* } */




/* BLYNK_WRITE(V2) */
/* { */
/*   int r = param[0].asInt(); */
/*   int g = param[1].asInt(); */
/*   int b = param[2].asInt(); */
/*   color = CRGB(r, g, b); */
/*   push_lights(); */
/*   // setColor(); */
/* } */

/* BLYNK_WRITE(V3) */
/* { */
/*   intensity = param.asInt(); */
/*   /1* push_lights(); *1/ */
/*   // setColor(); */
/* } */

/* void loop() */
/* { */
/*   Blynk.run(); */
/*   /1* timer.run(); *1/ */
/* } */


