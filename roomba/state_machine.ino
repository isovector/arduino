#include "roomba.h"
#include "state_machine.h"
#include "movement.h"

static int STUCK_POLLS = 140;

static int last_distance = 0;
static int stuck_polls = 0;

void new_state(State st) {
  state = st;
  stuck_polls = 0;
  Serial.print("new state: ");
  Serial.println(StateName[st]);
  start();
}


void enter_DRIVE() {
  drive(FORWARD, FASTEST_SPEED);
}

void enter_BOUNCE() {
  drive(STOP, 0);
  delay(20);
  drive(REVERSE, FAST_SPEED);
  delay(100);
  drive(REVERSE, FASTEST_SPEED);
  delay(650);
}

void enter_TURN() {
  drive(TURN_RIGHT, FASTEST_SPEED);
  delay(750);
}

void enter_STUCK() {
  drive(STOP, 0);
  delay(20);
  drive(REVERSE, FAST_SPEED);
  delay(100);
  drive(REVERSE, FASTEST_SPEED);
  delay(1400);
  if (rand() % 2 == 0) {
    drive(TURN_RIGHT, FAST_SPEED);
  } else {
    drive(TURN_LEFT, FAST_SPEED);
  }
  delay(1337);
}


void loop_DRIVE() {
  delay(10);
  with_distance([](int dist) {
    if (abs(dist - last_distance) <= 5) {
  /* Serial.print("stuck: "); */
  /* Serial.print(dist); */
  /* Serial.print(" --- "); */
  /* Serial.println(stuck_polls); */
      stuck_polls++;
    } else {
      last_distance = dist;
      stuck_polls = 0;
    }

    if (dist <= 4) {
      new_state(BOUNCE);
    } else if (stuck_polls >= STUCK_POLLS) {
      new_state(STUCK);
    }
  });
}

void loop_BOUNCE() {
  new_state(TURN);
}

void loop_TURN() {
  new_state(DRIVE);
}

void loop_STUCK() {
  new_state(DRIVE);
}

