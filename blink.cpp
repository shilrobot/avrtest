#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

/*

// Test filling up flash area with junk

#include <avr/pgmspace.h>
char data[16160] PROGMEM; 

*/

void uart_print_char(char c)
{
    while(!(UCSR0A & _BV(UDRE0)));
    UDR0 = c;
}

void uart_print(const char* c)
{
    while(*c)
    {
        uart_print_char(*c);
        ++c;
    }
}

void uart_print_P(const char* c)
{
    while(pgm_read_byte(c))
    {
        uart_print_char(pgm_read_byte(c));
        ++c;
    }
}

const char CRLF[] PROGMEM = "\r\n";

void uart_print_line(const char* c)
{
    uart_print(c);
    uart_print_P(CRLF);
}

void uart_print_line_P(const char* c)
{
    uart_print_P(c);
    uart_print_P(CRLF);
}

void uart_init()
{
    // use wonky AVR libc header to do our calculations for us
    #define BAUD 9600
    #include <util/setbaud.h>

    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    #if USE_2X
        UCSR0A |= _BV(U2X0);
    #else
        UCSR0A &= ~_BV(U2X0);
    #endif

    // enable TX/RX
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);

    // 8-bit transfers
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
}

int main(void)
{
    uart_init();

    // Blink pin 5 on port B
    // (the LED on digital port 13 on Arduino)

    DDRD |= _BV(2);

    while(1)
    {
        PORTD |= _BV(2);
        _delay_ms(500);
        PORTD &= ~_BV(2);
        _delay_ms(250);

        uart_print_line_P(PSTR("Test writing to serial port"));
    }
}
