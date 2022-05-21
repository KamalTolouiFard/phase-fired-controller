#ifndef EVENT_H
#define EVENT_H 0

#include "queue.h"

typedef enum {
    EC_NONE,
    EC_ROTARY_ENCODER_CW,
    EC_ROTARY_ENCODER_CC,
    EC_BUTTON_PUSH,
    EC_PLL_FREE,
    EC_PLL_LOCKED,
    MAX_EVENTS
}   event_t;

#endif