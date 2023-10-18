#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "uart.h"
#include "ESP8266.h"

#define BIT_SET(a, b) ((a) |= (1ULL << (b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1ULL<<(b)))
#define BIT_FLIP(a,b) ((a) ^= (1ULL<<(b)))
#define BIT_CHECK(a,b) (!!((a) & (1ULL<<(b)))) 

#define Set_Button_As_Input_Pullup(ddr,pdr,pin) BIT_CLEAR(ddr,pin);BIT_SET(pdr,pin);
#define Button_Is_Clicked(pinr,pin) !BIT_CHECK(pinr,pin)


// B (digital pin 8 to 13)
// C (analog input pins)
// D (digital pins 0 to 7)
// https://wokwi.com/projects/376389913248575489


#define BUTTON_PIN_1 0
#define BUTTON_PIN_2 1
#define BUTTON_PIN_3 2


void HandleButtonClick(int option){
    _delay_ms(250);
    lcd_clear();
    lcd_set_cursor(0, 0);
    
    switch (option)
    {
    case 1:
        lcd_puts("GOOD!");
        addData("field1", 3);
        _delay_ms(350);
    break;
    case 2:
        lcd_puts("OKAY!");
        addData("field1", 2);
        _delay_ms(350);
    break;
    case 3:
        lcd_puts("BAD!");
        addData("field1", 1);
        _delay_ms(350);
    break;
    }

    pushData();
    lcd_message();
    return;
}

void lcd_setup_sequence()
{
    lcd_clear();
    lcd_set_cursor(0, 0);

    for(int i = 0; i < 3; i++)
    {
        lcd_clear();
        lcd_puts("Starting.");
        lcd_set_cursor(0, 0);
        _delay_ms(450);
        lcd_puts("Starting..");
        lcd_set_cursor(0, 0);
        _delay_ms(450);
        lcd_puts("Starting...");
        lcd_set_cursor(0, 0);
        _delay_ms(450);
    }
}

void lcd_message()
{
    lcd_set_cursor(0, 0);
    lcd_puts("How was class");
    lcd_set_cursor(0, 1);
    lcd_puts("today?");
}

int main(void)
{
    millis_init();
    sei();

    lcd_init();
    //_delay_ms(1000);
    init_serial();
    ESPinit();

    lcd_setup_sequence();

    Set_Button_As_Input_Pullup(DDRB, PORTB, BUTTON_PIN_1);
    Set_Button_As_Input_Pullup(DDRB, PORTB, BUTTON_PIN_2);
    Set_Button_As_Input_Pullup(DDRB, PORTB, BUTTON_PIN_3);

    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_puts("Startup complete");
    _delay_ms(750);
    lcd_clear();

    lcd_message();

    while(1) {
        if(Button_Is_Clicked(PINB,BUTTON_PIN_1)) HandleButtonClick(1);
        if(Button_Is_Clicked(PINB,BUTTON_PIN_2)) HandleButtonClick(2);
        if(Button_Is_Clicked(PINB,BUTTON_PIN_3)) HandleButtonClick(3);
    }
    return 0;
}