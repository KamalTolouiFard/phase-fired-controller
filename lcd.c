#include "lcd.h"

#define lcd_rs_h()  sbi(PORTB,3)
#define lcd_rs_l()  cbi(PORTB,3)

#define lcd_rw_h()  sbi(PORTB,4)
#define lcd_rw_l()  cbi(PORTB,4)

#define lcd_en_h()  sbi(PORTB,5)
#define lcd_en_l()  cbi(PORTB,5)

static void lcd_write(uint8_t value, uint8_t rs)
{
    uint8_t busy;
    
    DDRC &=~0x0F;   //inputs
    PORTC|= 0x0F;   //enable pull-up
    
    lcd_rw_h();     //read mode
    lcd_rs_l();     //busy flag and address counter
    
    do 
    {
        //reading...
        //read high nibble
        lcd_en_h();
        delay_us(1);
        busy = bit_is_set(PINC,3);
        lcd_en_l();
        delay_us(1);
        
        //read low  nibble
        //complete the process
        lcd_en_h();
        delay_us(1);
        lcd_en_l();
        delay_us(1);
    }   while(busy);
    
    PORTC&=~0x0F;   //disable pull-up
    DDRC |= 0x0F;   //output
    
    lcd_rw_l();             //write mode selected
    if(!rs) lcd_rs_l();     //instruction
    else lcd_rs_h();        //data
    
    //writing...
    //write high nibble
    PORTC|=(value&0xF0) >> 4;
    lcd_en_h();
    delay_us(1);
    lcd_en_l();
    delay_us(1);
    
    //write low  nibble
    PORTC&=~0x0F;
    PORTC|=(value&0x0F);
    lcd_en_h();
    delay_us(1);
    lcd_en_l();
    delay_us(1);
}

static int lcd_putc(char bData, FILE*stream) {
    lcd_write(bData,1);
    return 0;
}

static void lcd_begin(void)
{
    DDRB|=(1<<PINB5)|(1<<PINB4)|(1<<PINB3);
    DDRC|=(1<<PINC3)|(1<<PINC2)|(1<<PINC1)|(1<<PINC0);
    
    //initializing by instruction
    //based on hd44780u data
    delay_ms(20);
    PORTC|= 0x03;
    lcd_en_h();
    delay_us(1);
    lcd_en_l();
    
    delay_ms(10);
    PORTC|= 0x03;
    lcd_en_h();
    delay_us(1);
    lcd_en_l();
    
    delay_us(100);
    PORTC|= 0x03 ;
    lcd_en_h();
    delay_us(1);
    lcd_en_l();
    
    //initial 'function set' to
    //change interface
    delay_us(100);
    PORTC&=~0x0F ;
    PORTC|= 0x02 ;
    lcd_en_h();
    delay_us(1);
    lcd_en_l();
    delay_us(100);
    
    //4-bit, 2-line, 5x8 matrix
    lcd_write(LCD_FUNCTION|LCD_2LINE,0);
    
    //display off
    lcd_write(LCD_CONTROL,0);
    
    //clear
    lcd_write(LCD_CLEAR_DISPLAY,0);
    
    //default text direction(for romance languages)
    lcd_write(LCD_ENTRYMODE_SET|LCD_ENTRY_LEFT_TO_RIGHT,0);
    
    //display on
    lcd_write(LCD_CONTROL|LCD_ENABLE,0);
}

static FILE lcdout = FDEV_SETUP_STREAM(lcd_putc, NULL, _FDEV_SETUP_WRITE);
void beginDisplay(void) {
    stdout = &lcdout;
    lcd_begin();
}

void moveTo(uint8_t x, uint8_t y) {
    y = y < 1 ? 0 : 0xC0;
    x = min(x, 15);
    lcd_write(LCD_SET_DDRAM_ADR|(x+y),0);
}

void clearDisplay(void) {
    lcd_write(LCD_CLEAR_DISPLAY,0);
}