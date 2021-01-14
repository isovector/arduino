#include "roomba.h"
#include "state_machine.h"

void setup() {
    pinMode (6, OUTPUT);
    analogWrite(6,LOW);
    pinMode (9, OUTPUT);
    analogWrite(9,LOW);
    pinMode (10, OUTPUT);
    analogWrite(10,LOW);
    pinMode (11, OUTPUT);
    analogWrite(11,LOW);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);

    Serial.begin(9600);
    Serial.println("hi erin!");

    new_state(DRIVE);
    with_distance([](int dist){});
}

void start() {
  switch(state) {
#define EXPAND(x) \
    case x: enter_##x(); break;
#include "state.h"
#undef EXPAND
  }
}

void loop() {
  switch(state) {
#define EXPAND(x) \
    case x: loop_##x(); break;
#include "state.h"
#undef EXPAND
  }
}
