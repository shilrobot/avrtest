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

// *all* port C
const int SHIFT_LATCH_PIN = 5; // pin 24 - to pin 10 on 595 - WHITE
const int SHIFT_CLK_PIN = 6; // pin 25 - to pin 15 on 595 - YELLOW
const int SHIFT_IN_PIN = 7; // pin 26 - to pin 2 on 595 - ORANGE

#define WRITE(__port, __pin, __value) if(__value) { (__port) |= _BV(__pin); } else { (__port) &= ~(_BV(__pin)); }

void write_leds(uint8_t data)
{
    for(int i=7; i>=0; --i)
    {
        WRITE(PORTC, SHIFT_IN_PIN, data & (1<<i));
        //_delay_ms(1);
        WRITE(PORTC, SHIFT_CLK_PIN, 1);
        //_delay_ms(1);
        WRITE(PORTC, SHIFT_CLK_PIN, 0);
        //_delay_ms(1);
    }
    WRITE(PORTC, SHIFT_LATCH_PIN, 1);
    //_delay_ms(1);
    WRITE(PORTC, SHIFT_LATCH_PIN, 0);
    //_delay_ms(1);
}

#define DO(__pins) { for(int i=0; i<5000; ++i) {  write_leds(1<<__pins); } }

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
        DO(0);
        DO(1);
        DO(2);
        DO(3);
        DO(4);
        DO(5);
        DO(6);
        DO(7);
    }
}
