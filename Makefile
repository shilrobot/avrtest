
AVRDUDE_CONF=d:/Bulk/Downloads/arduino-1.0-windows/arduino-1.0/hardware/tools/avr/etc/avrdude.conf
SERIAL_PORT=COM3
PART=atmega168

all: blink.hex

blink.elf: blink.c
	avr-gcc -mmcu=$(PART) -DF_CPU=16000000UL -Os -Wall blink.c -o blink.elf

blink.hex: blink.elf
	avr-objcopy -O ihex blink.elf blink.hex

clean:
	rm -rf blink.elf blink.hex

# Program using Arduino
burn: blink.hex
	avrdude -C "$(AVRDUDE_CONF)" -p $(PART) -c arduino -b 19200 -U flash:w:blink.hex -P $(SERIAL_PORT)

report: blink.hex
	avr-size -C blink.elf --mcu=$(PART)

.PHONY: clean burn
