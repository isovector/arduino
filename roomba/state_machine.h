#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "roomba.h"

#define EXPAND(x) \
void enter_##x(); \
void loop_##x();
#include "state.h"
#undef EXPAND

#endif

