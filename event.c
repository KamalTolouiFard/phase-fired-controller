#include "event.h"

#define ON_ROTARY_ENCODER_CHANGE    INT0_vect
#define ON_PLL_LOCK                 PCINT0_vect
#define ON_BUTTON_PUSH              INT1_vect

ISR(ON_BUTTON_PUSH) {
    queueEvent(EC_BUTTON_PUSH);
}

ISR(ON_ROTARY_ENCODER_CHANGE) {
    if (bit_is_clear(PIND,2) != bit_is_clear(PIND,7)) queueEvent(EC_ROTARY_ENCODER_CW);
        else queueEvent(EC_ROTARY_ENCODER_CC);
}

ISR(ON_PLL_LOCK)    {
    if (bit_is_clear(PINB,0)) queueEvent(EC_PLL_FREE);
        else queueEvent(EC_PLL_LOCKED);
}