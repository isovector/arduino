#include "roomba.h"
#include "state_machine.h"
#include "movement.h"

static const int STUCK_POLLS = 140;
static const int STUCK_INVALID_POLLS = 10;
static const int ONE_EIGHTY_DEGS = 1337;


static int last_distance = 0;
static int stuck_polls = 0;

static int relocate_poll;
static int relocate_speed;
static Direction relocate_dir ;

void new_state(State st) {
  state = st;
  stuck_polls = 0;
  Serial.print("new state: ");
  Serial.println(StateName[st]);
  start();
}

void bounce_or_stick() {
  with_distance([](int dist) {
    /* Serial.print("dist: "); */
    /* Serial.println(dist); */
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
    } else if (stuck_polls >= ((dist == INVALID_DISTANCE) ? STUCK_INVALID_POLLS : STUCK_POLLS)) {
      new_state(STUCK);
    }
  });
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
  delay(500);
  drive(REVERSE, FASTEST_SPEED);
  delay(1000);
  if (rand() % 2 == 0) {
    drive(TURN_RIGHT, FAST_SPEED);
  } else {
    drive(TURN_LEFT, FAST_SPEED);
  }
  delay(ONE_EIGHTY_DEGS);
}

void enter_RELOCATE() {
  relocate_speed = FAST_SPEED;
  relocate_poll = 0;
  if (rand() % 2 == 0) {
    relocate_dir = DRIVE_LEFT;
  } else {
    relocate_dir = DRIVE_RIGHT;
  }
  drive(FORWARD, FASTEST_SPEED);
  delay(20);
  drive(relocate_dir, relocate_speed);
}

void loop_RELOCATE() {
  const int time = 10;
  relocate_poll += time;
  if (relocate_poll >= ONE_EIGHTY_DEGS * 2) {
    relocate_poll = 0;
    relocate_speed += 10;
    drive(relocate_dir, relocate_speed);
  }

  delay(time);
  bounce_or_stick();
}


void loop_DRIVE() {
  delay(10);
  bounce_or_stick();
}

void loop_BOUNCE() {
  new_state(TURN);
}

void loop_TURN() {
  new_state(DRIVE);
}

void loop_STUCK() {
  if (rand() % 2 == 0) {
    new_state(RELOCATE);
  } else {
    new_state(RANDOM_TURN);
  }
}

void enter_RANDOM_TURN() {
  if (rand() % 2 == 0) {
    drive(TURN_RIGHT, FAST_SPEED);
  } else {
    drive(TURN_LEFT, FAST_SPEED);
  }
  delay(ONE_EIGHTY_DEGS / 2);
  drive(STOP, 0);
}

void loop_RANDOM_TURN() {
  new_state(DRIVE);
}

