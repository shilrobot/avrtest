#include <avr/io.h>
#include <util/delay.h>

// ALL THREE are on port C
const int SHIFT_LATCH_PIN = 5; // pin 24 - to pin 10 on 595 - WHITE
const int SHIFT_CLK_PIN = 6; // pin 25 - to pin 15 on 595 - YELLOW
const int SHIFT_IN_PIN = 7; // pin 26 - to pin 2 on 595 - ORANGE

#define WRITE(__port, __pin, __value) if(__value) { (__port) |= _BV(__pin); } else { (__port) &= ~(_BV(__pin)); }

void shift_out(uint8_t data)
{
    for(int i=7; i>=0; --i)
    {
        WRITE(PORTC, SHIFT_IN_PIN, data & (1<<i));
        WRITE(PORTC, SHIFT_CLK_PIN, 1);
        WRITE(PORTC, SHIFT_CLK_PIN, 0);
    }
    WRITE(PORTC, SHIFT_LATCH_PIN, 1);
    WRITE(PORTC, SHIFT_LATCH_PIN, 0);
}

void light_pin(int pin)
{
    shift_out(1 << pin);
    _delay_ms(100);
}

int main(void)
{
    // Disable the JTAG interface on port C
    MCUCR |= _BV(JTD);
    MCUCR |= _BV(JTD);

    DDRC |= _BV(SHIFT_LATCH_PIN);
    DDRC |= _BV(SHIFT_CLK_PIN);
    DDRC |= _BV(SHIFT_IN_PIN);

    while(1)
    {
        light_pin(0);
        light_pin(1);
        light_pin(2);
        light_pin(3);
        light_pin(4);
        light_pin(5);
        light_pin(6);
        light_pin(7);
        light_pin(6);
        light_pin(5);
        light_pin(4);
        light_pin(3);
        light_pin(2);
        light_pin(1);
    }
}
