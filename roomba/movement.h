#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "roomba.h"

void drive(Direction dir, int speed);
void with_distance(void (*callback)(int));

#define EXPAND(x) \
void go_##x(int);
#include "direction.h"
#undef EXPAND

#endif

