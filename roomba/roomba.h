#ifndef ROOMBA_H
#define ROOMBA_H

const int trigPin = 3;
const int echoPin = 4;

const int HELLA_FASTEST_SPEED = 192;
const int FASTEST_SPEED = 127;
const int FAST_SPEED = 96;

const int RELOCATE_TIME = 25000;

// The distance returned if we get no sonar pulse
const int INVALID_DISTANCE = 9999;

enum Direction {
#define EXPAND(x) x,
#include "direction.h"
#undef EXPAND
};

const char *DirectionName[] = {
#define EXPAND(x) #x,
#include "direction.h"
#undef EXPAND
};

enum State {
#define EXPAND(x) x,
#include "state.h"
#undef EXPAND
} state;

const char *StateName[] = {
#define EXPAND(x) #x,
#include "state.h"
#undef EXPAND
};

#endif ROOMBA_H

