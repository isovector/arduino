#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "roomba.h"

void drive(Direction dir, int speed);
void with_distance(void (*callback)(int));

#endif

