#ifndef FSM_H
#define FSM_H   0

#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include "event.h"
#include "lcd.h"

typedef enum {
    ST_IDLE,
    ST_MAINMENU,
    ST_SAVE,
    ST_LOAD,
    ST_MIN_SET,
    ST_MAX_SET,
    MAX_STATES
}   state_t;

typedef  state_t(*action_t)(void);
extern const action_t stateTable[MAX_STATES][MAX_EVENTS-3];
#define getEventHnadler(STATE,EVENT) (stateTable[STATE][EVENT])

state_t incFireAngle(void);
state_t decFireAngle(void);
state_t openMenu(void);
state_t nextMenu(void);
state_t previousMenu(void);
state_t pickMenu(void);
state_t incMinFireAngle(void);
state_t decMinFireAngle(void);
state_t setMinFireAngle(void);
state_t incMaxFireAngle(void);
state_t decMaxFireAngle(void);
state_t setMaxFireAngle(void);
state_t nextSaveSlot(void);
state_t previousSaveSlot(void);
state_t saveToFile(void);
state_t nextDataFile(void);
state_t previousDataFile(void);
state_t loadFromFile(void);
void updateDisplay(state_t state);

#endif