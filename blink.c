#include <avr/io.h>
#include <util/delay.h>

/*

// Test filling up flash area with junk

#include <avr/pgmspace.h>
char data[16160] PROGMEM; 

*/

int main(void) {

	DDRB = (1 << 5);

	while(1) {
		PORTB = 0xff;
		_delay_ms(500);
		PORTB = 0x00;
		_delay_ms(250);
	}
}
