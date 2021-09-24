#ifndef REMOTE_H
#define REMOTE_H

// #define DECODE_NEC
#include <IRremote.h>

#define REMOTE_RECV_PIN 1

IRrecv irrecv(REMOTE_RECV_PIN);

#endif

