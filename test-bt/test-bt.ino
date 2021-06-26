#include "ConstColor.h"
#include "EvolveColor.h"
#include "Many.h"
#include "Bounce.h"
#include "Strobe.h"
#include "Aurora.h"
#include "GlobalColor.h"

/* #include "Remote.h" */


void setNexus() {
  delete program;
  Many<7> *many = new Many<7>();
  many->add(evolve_color(0, 160, 255, 5, 120));
  Program *ec = evolve_color(0, 160, 255, 50, 200);
  many->add(new Bounce(ec, 100, -itof(10, 0), 30));
  many->add(new Bounce(ec, 140, -itof(25, 0), 30));
  many->add(new Bounce(ec, 180, -itof(26, 0), 30));
  many->add(new Bounce(ec, 220, -itof(20, 0), 30));
  many->add(new Bounce(ec, 260, -itof(22, 0), 30));
  many->add(new Bounce(ec, 300, -itof(30, 0), 30));
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
  /* irrecv.enableIRIn(); */

  strip.begin();
  strip2.begin();

  program = new GlobalColor();
  /* setNexus(); */
  setAurora();
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

void loop() {
  /* if (irrecv.decode()) { */
  /*   char *raw = (void*)&irrecv.decodedIRData.decodedRawData; */
  /*   global_color = {raw[0], raw[1], raw[2]}; */
  /*   setGlobal(); */
  /*   switch (irrecv.decodedIRData.protocol) { */
  /*     case 7: */
  /*     switch (irrecv.decodedIRData.command) { */
  /*       case 69: */
  /*         setNexus(); */
  /*         break; */
  /*       case 70: */
  /*         global_color = {0, 255, 0}; */
  /*         setAurora(); */
  /*         break; */
  /*       case 71: */
  /*         global_color = {0, 0, 255}; */
  /*         setGlobal(); */
  /*         break; */
  /*     } */
  /*     break; */
  /*   } */
  /*   irrecv.resume(); */
  /* } */

  unsigned long now = millis();
  time delta = max(1, milli_diff(now, last_time));
  program->evolve(delta);
  last_time = now;

  push_lights();
}

