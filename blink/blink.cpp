#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRD |= _BV(0);

    while(1)
    {
        PORTD |= _BV(0);
        _delay_ms(500);
        PORTD &= ~(_BV(0));
        _delay_ms(500);
    }
}
