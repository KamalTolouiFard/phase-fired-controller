#include "fsm.h"

static  event_t event;
static action_t eventHandler;
static  state_t state;

int main(void)
{
    
    while(1)
    {
        event = getLastEvent();
        if (!event) continue;
        
        event--;
        eventHandler = getEventHnadler(state,event);
        if (!eventHandler) continue;
        
        state = eventHandler();
        updateDisplay(state);
    }
}