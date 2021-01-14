#include "movement.h"

void with_distance(void (*callback)(int)) {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  if (valid_distance(distance)) {
    callback(distance);
  } else {
    callback(9999);
  }
}

bool valid_distance(int dist) {
  return dist < 100;
}

void drive(Direction dir, int speed) {
  Serial.print("drive: ");
  Serial.print(DirectionName[dir]);
  Serial.print(" @ ");
  Serial.println(speed);
  switch (dir) {
    case FORWARD:
      Forward(speed);
      break;
    case REVERSE:
      Reverse(speed);
      break;
    case TURN_LEFT:
      TurnLeft(speed);
      break;
    case TURN_RIGHT:
      TurnLeft(speed);
      break;
    case STOP:
      stop();
      break;
  }
}

void Reverse(int power) {
    analogWrite(6,0);
    analogWrite(9,power);
    analogWrite(10,0);
    analogWrite(11,power);
}

void Forward(int power) {
    analogWrite(6,power);
    analogWrite(9,0);
    analogWrite(10,power);
    analogWrite(11,0);
}

void TurnLeft(int power) {
    analogWrite(6,0);
    analogWrite(9,power);
    analogWrite(10,power);
    analogWrite(11,0);
}

void TurnRight(int power) {
    analogWrite(6,power);
    analogWrite(9,0);
    analogWrite(10,0);
    analogWrite(11,power);
}

void stop() {
    Forward(0);
}
