#include "fsm.h"

const char txtUpperLim[] PROGMEM = "upper\0";
const char txtLoad[] PROGMEM = "Load\0";
const char txtMainMenu[] PROGMEM = "Menu\0";
const char txtSave[] PROGMEM = "Save\0";
const char txtRangeSet[] PROGMEM = "Span\0";
const char txtExit[] PROGMEM = "Exit\0";
const char txtLowerLim[] PROGMEM = "lower\0";
const char txtSlot[] PROGMEM = "Slot-%u \0";
const char * const stringTable[] PROGMEM = {txtSave, txtLoad, txtRangeSet, txtExit};

const action_t  stateTable[MAX_STATES][MAX_EVENTS-3] =
{
    {incFireAngle,      decFireAngle,       openMenu        },
    {nextMenu,          previousMenu,       pickMenu        },
    {nextSaveSlot,      previousSaveSlot,   saveToFile      },
    {nextDataFile,      previousDataFile,   loadFromFile    },
    {incMinFireAngle,   decMinFireAngle,    setMinFireAngle },
    {incMaxFireAngle,   decMaxFireAngle,    setMaxFireAngle }
};

#define fireAngle OCR0A
static uint8_t maxFireAngle, minFireAngle;
static uint8_t index;
uint32_t dataFiles[8] EEMEM;

state_t incFireAngle(void) {
    fireAngle = fireAngle>maxFireAngle?fireAngle-1:maxFireAngle;
    return ST_IDLE;
}

state_t decFireAngle(void) {
    fireAngle = fireAngle<minFireAngle?fireAngle+1:minFireAngle;
    return ST_IDLE;
}

state_t openMenu(void) {
    index = 0;
    return ST_MAINMENU;
}

state_t nextMenu(void) {
    index = index<3?index+1:0;
    return ST_MAINMENU;
}

state_t previousMenu(void) {
    index = index>0?index-1:3;
    return ST_MAINMENU;
}

state_t pickMenu(void) {
    uint8_t var = index<3?index+2:0;
    index = 0;
    return  var;
}

state_t incMinFireAngle(void) {
    minFireAngle = minFireAngle>(maxFireAngle+1)?(minFireAngle-1):(maxFireAngle+1);
    return ST_MIN_SET;
}

state_t decMinFireAngle(void) {
    minFireAngle = minFireAngle<255?minFireAngle+1:255;
    return ST_MIN_SET;
}

state_t setMinFireAngle(void) {
    fireAngle = fireAngle<minFireAngle?fireAngle:minFireAngle;
    return ST_MAX_SET;
}

state_t incMaxFireAngle(void) {
    maxFireAngle = maxFireAngle>0?maxFireAngle-1:0;
    return ST_MAX_SET;
}

state_t decMaxFireAngle(void) {
    maxFireAngle = maxFireAngle<(minFireAngle-1)?(maxFireAngle+1):(minFireAngle-1);
    return ST_MAX_SET;
}

state_t setMaxFireAngle(void) {
    fireAngle = fireAngle>maxFireAngle?fireAngle:maxFireAngle;
    return ST_IDLE;
}

state_t nextSaveSlot(void) {
    index = index<7?index+1:0;
    return ST_SAVE;
}

state_t previousSaveSlot(void) {
    index = index>0?index-1:7;
    return ST_SAVE;
}

state_t saveToFile(void)
{
    uint8_t mem[4];
    uint8_t i, checksum;
    
    //copying...
    mem[0] = minFireAngle;
    mem[1] = fireAngle;
    mem[2] = maxFireAngle;
    
    //calculate checksum
    //A record's checksum byte is the two's complement of the least significant byte of
    //the sum of all decoded byte values in the record preceding the checksum
    checksum = 0;
    for(i=0; i<3; i++) checksum += mem[i];
    checksum = ~checksum;
    checksum++;
    
    //writing...
    mem[3] = checksum;
    eeprom_write_block((const void*)mem,(void*)&dataFiles[index],4);
    return ST_IDLE;
}

state_t nextDataFile(void) {
    index = index<7?index+1:0;
    return ST_LOAD;
}

state_t previousDataFile(void) {
    index = index>0?index-1:7;
    return ST_LOAD;
}

state_t loadFromFile(void)
{
    uint8_t mem[4];
    uint8_t i, checksum;
    
    //reading...
    eeprom_read_block((void*)mem, (const void*)&dataFiles[index],4);
    
    //validate data record
    checksum = 0;
    for(i=0; i<3; i++) checksum += mem[i];
    checksum = ~checksum;
    checksum++;
    if (checksum!=mem[3])  return ST_IDLE;
    
    //data is valid
    //copying...
    minFireAngle = mem[0];
    fireAngle = mem[1];
    maxFireAngle = mem[2];
    return ST_IDLE;
}

void updateDisplay(state_t state)
{
    clearDisplay();
    switch(state) {
        case ST_IDLE :
            printf("%3.3u",fireAngle);
            break;
        
        case ST_MAINMENU :
            printf_P(txtMainMenu);
            moveTo(0,1);
            printf_P((const char*)(pgm_read_word(&stringTable[index])));
            break;
        
        case ST_SAVE :
            printf_P(txtSave);
            moveTo(0,1);
            printf_P(txtSlot,index);
            break;
        
        case ST_LOAD :
            printf_P(txtLoad);
            moveTo(0,1);
            printf_P(txtSlot,index);
            break;
        
        case ST_MIN_SET  :
            printf_P(txtLowerLim);
            moveTo(0,1);
            printf("%3.3u",minFireAngle);
            break;
        
        case ST_MAX_SET  :
            printf_P(txtUpperLim);
            moveTo(0,1);
            printf("%3.3u",maxFireAngle);
            break;
        
        default :
            return;
            break;
    }
}