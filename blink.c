#include <avr/io.h>
#include <util/delay.h>

/*

// Test filling up flash area with junk

#include <avr/pgmspace.h>
char data[16160] PROGMEM; 

*/

int main(void)
{

	// Blink pin 5 on port B
	// (the LED on digital port 13 on Arduino)

	DDRB = (1<<5);

	while(1)
	{
		PORTB |= (1<<5);
		_delay_ms(500);
		PORTB &= ~(1<<5);
		_delay_ms(250);
	}
}
