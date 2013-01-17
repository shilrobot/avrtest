// Uses SPI to write to a shift register very fast (8 MHz), must faster
// than we can bit bang it.

#include <avr/io.h>
#include <util/delay.h>

const int SHIFT_LATCH_PIN = 5; // pin 24 (PC5) - to pin 10 on 595 - WHITE

// pin 3 (PB7 / SCK) - to pin 15 on 595 - YELLOW
// pin 1 (PB5 / MOSI) - to pin 2 on 595 - ORANGE
// pin 44 (PB4 / !SS) - set as an output to nowhere

#define WRITE(__port, __pin, __value) if(__value) { (__port) |= _BV(__pin); } else { (__port) &= ~(_BV(__pin)); }

// Write to the 
void shift_out(uint8_t data)
{
    // Note: SPIF is automatically cleared if you ever read a 1 from it, and then 
    // access the SPDR.
    SPDR0 = data;
    while(!(SPSR0 & _BV(SPIF0)));
    PORTC |= _BV(SHIFT_LATCH_PIN);
    PORTC &= ~_BV(SHIFT_LATCH_PIN);
}

void light_pin(int pin)
{
    // (for rough timing)
    /* for(uint16_t i=0; i<50000U; ++i) */
        shift_out(1 << pin);
    _delay_ms(100);
}

int main(void)
{
    // Disable the JTAG interface on port C, free up all port C pins
    MCUCR |= _BV(JTD);
    MCUCR |= _BV(JTD);

    DDRC |= _BV(SHIFT_LATCH_PIN);

    // Set up SPI.

    // Note: we MUST set !SS to an output or else SPI may not operate.
    // It does weird stuff (like automatically turn into a slave if it sees
    // that SS is low) if you set it to an input & don't connect it.
    DDRB |= _BV(4) | _BV(5) | _BV(7);

    // Master mode
    // CPOL = 0, CPHA = 0 (slave samples data @ rising edge of clock)
    // Clock = fosc/2 = 8 MHz
    SPCR0 = _BV(SPE0) | _BV(MSTR0);
    SPSR0 |= _BV(SPI2X0);

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
