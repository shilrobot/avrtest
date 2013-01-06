# general config stuff
SERIAL_PORT := COM3
PART := atmega168
FREQUENCY := 16000000

# input C files
SOURCE_FILES := blink.c

# intermediate files
ELF_FILE := project.elf
HEX_FILE := project.hex

all: $(HEX_FILE)

$(ELF_FILE): $(SOURCE_FILES)
	avr-gcc -mmcu=$(PART) -DF_CPU=$(FREQUENCY)UL -Os -Wall $< -o $@

$(HEX_FILE): $(ELF_FILE)
	avr-objcopy -O ihex $< $@

clean:
	rm -rf "$(HEX_FILE)" "$(ELF_FILE)"

# Program using Arduino
burn: $(HEX_FILE)
	avrdude -C "$(AVRDUDE_CONF)" -p $(PART) -c arduino -b 19200 -U flash:w:"$(HEX_FILE)" -P $(SERIAL_PORT)

report: $(ELF_FILE)
	avr-size -C "$(ELF_FILE)" --mcu=$(PART)

.PHONY: clean burn
