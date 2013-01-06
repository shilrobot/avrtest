
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

char data[16160];

int main(void) {

	DDRB = (1 << 5);

	while(1) {
		PORTB = 0xff;
		_delay_ms(500);
		PORTB = 0x00;
		_delay_ms(250);
	}
}
