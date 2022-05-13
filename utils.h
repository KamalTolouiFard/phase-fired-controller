#ifndef UTILS_H
#define UTILS_H 0

#include <util/delay.h>
#include <avr/io.h>

#define min(a,b)            ((a)<(b)?(a):(b))
#define constraint(l,v,h)   ((v)<(l)?(l):((v)>(h)?(h):(v)))
#define max(a,b)            ((a)>(b)?(a):(b))
#define sbi(sfr,bit)    _SFR_BYTE(sfr)|= _BV(bit)
#define cbi(sfr,bit)    _SFR_BYTE(sfr)&=~_BV(bit)
#define delay_us(us)    _delay_us(us)
#define delay_ms(ms)    _delay_ms(ms)

#endif