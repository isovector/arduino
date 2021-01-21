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

    Serial.begin(9600);
    Serial.println("hello world");

    new_state(RELOCATE);
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
