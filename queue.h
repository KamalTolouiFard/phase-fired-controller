#ifndef QUEUE_H
#define QUEUE_H 0

#include <avr/interrupt.h>

void queueEvent(uint8_t eventcode);
uint8_t getLastEvent(void);

#endif