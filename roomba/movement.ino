#include "movement.h"
/* #include <vector> */

static const int RIGHT_WHEEL_FORWARD = 10;
static const int RIGHT_WHEEL_BACKWARD = 11;
static const int LEFT_WHEEL_FORWARD = 6;
static const int LEFT_WHEEL_BACKWARD = 9;

static const long MAX_RANGE = 32;

static const float smoothing_coef = 0.8;
static long next_pred = INVALID_DISTANCE;


void with_distance(void (*callback)(int)) {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(RIGHT_WHEEL_FORWARD);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)

  if (distance > MAX_RANGE) distance = MAX_RANGE;

  if (next_pred == INVALID_DISTANCE) {
    next_pred = distance;
  } else {
    next_pred = smoothing_coef * distance + (1 - smoothing_coef) * next_pred;
  }

  callback(next_pred);
}

void drive(Direction dir, int speed) {
  Serial.print("drive: ");
  Serial.print(DirectionName[dir]);
  Serial.print(" @ ");
  Serial.println(speed);
  switch (dir) {
#define EXPAND(x) \
    case x: go_##x(speed); break;
#include "direction.h"
#undef EXPAND
  }
}

void go_REVERSE(int power) {
  analogWrite(LEFT_WHEEL_FORWARD,0);
  analogWrite(LEFT_WHEEL_BACKWARD,power);
  analogWrite(RIGHT_WHEEL_FORWARD,0);
  analogWrite(RIGHT_WHEEL_BACKWARD,power);
}

void go_FORWARD(int power) {
  analogWrite(LEFT_WHEEL_FORWARD,power);
  analogWrite(LEFT_WHEEL_BACKWARD,0);
  analogWrite(RIGHT_WHEEL_FORWARD,power);
  analogWrite(RIGHT_WHEEL_BACKWARD,0);
}

void go_TURN_LEFT(int power) {
  analogWrite(LEFT_WHEEL_FORWARD,0);
  analogWrite(LEFT_WHEEL_BACKWARD,power);
  analogWrite(RIGHT_WHEEL_FORWARD,power);
  analogWrite(RIGHT_WHEEL_BACKWARD,0);
}

void go_TURN_RIGHT(int power) {
  analogWrite(LEFT_WHEEL_FORWARD,power);
  analogWrite(LEFT_WHEEL_BACKWARD,0);
  analogWrite(RIGHT_WHEEL_FORWARD,0);
  analogWrite(RIGHT_WHEEL_BACKWARD,power);
}

void go_DRIVE_LEFT(int power) {
  analogWrite(LEFT_WHEEL_FORWARD, power);
  /* analogWrite(LEFT_WHEEL_BACKWARD,0); */
  /* analogWrite(RIGHT_WHEEL_FORWARD,power); */
  /* analogWrite(RIGHT_WHEEL_BACKWARD,0); */
}

void go_DRIVE_RIGHT(int power) {
  /* analogWrite(LEFT_WHEEL_FORWARD,power); */
  /* analogWrite(LEFT_WHEEL_BACKWARD,0); */
  analogWrite(RIGHT_WHEEL_FORWARD, power);
  /* analogWrite(RIGHT_WHEEL_BACKWARD,0); */
}

void go_STOP() {
  go_FORWARD(0);
}

void go_STOP(int unused_because_im_a_bad_programmer) {
  go_STOP();
}

