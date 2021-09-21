#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "GlobalColor.h"

void setup_bluetooth() {
  Serial1.begin(9600);
}

void loop_bluetooth() {
  if (Serial1.available() > 0) {
    char res[64];
    Serial1.readBytes(res, 64);

    if (res == "nexus") {
      setNexus();
    } else if (res == "aurora") {
      setAurora();
    } else if (res == "global") {
      setGlobal();
    }
  }
}

#endif

