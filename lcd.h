#ifndef LCD_H
#define LCD_H   0

#include <stdio.h>
#include "utils.h"

#define LCD_CLEAR_DISPLAY       0x01
#define LCD_HOME                0x02
#define LCD_ENTRYMODE_SET       0x04
#define LCD_CONTROL             0x08
#define LCD_SHIFT               0x10
#define LCD_FUNCTION            0x20
#define LCD_SET_CGRAM_ADR       0x40
#define LCD_SET_DDRAM_ADR       0x80

#define LCD_ENTRY_LEFT_TO_RIGHT 0x02
#define LCD_ENTRY_SHIFTED       0x01

#define LCD_ENABLE              0x04
#define LCD_CURSOR_SHOW         0x03

#define LCD_1LINE               0x00
#define LCD_2LINE               0x08
#define LCD_5X08DOTS            0x00
#define LCD_5X10DOTS            0x04

void beginDisplay(void);
void moveTo(uint8_t x, uint8_t y);
void clearDisplay(void);

#endif