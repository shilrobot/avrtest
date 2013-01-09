# general config stuff
SERIAL_PORT := COM4

# this is for an arduino UNO R3 (or clone)
# Use the Arduino IDE to flash the bootloader (optiboot, only 512B) & fuse bits
# Otherwise, for no bootloader use these fuse bits:
# -U lfuse:w:0xFF:m -U hfuse:w:0xDB:m -U efuse:w:0x05:m
# They are the same as the default UNO R3 fuses except no boot reset vector.
# Fuse bits HAVE to be verified & modified if you aren't using ATmega328P!
PART := atmega328p
FREQUENCY := 16000000
BOOTLOADER_BAUD_RATE := 115200

# input C files
SOURCE_FILES := blink.cpp

# intermediate files
ELF_FILE := project.elf
HEX_FILE := project.hex

CXXFLAGS := -DF_CPU=$(FREQUENCY)UL \
			-Os \
			-Wall -pedantic \
			-fpack-struct -fshort-enums -ffunction-sections -fdata-sections -Wl,--relax,--gc-sections

all: $(HEX_FILE)

$(ELF_FILE): $(SOURCE_FILES)
	avr-g++ -mmcu=$(PART) $(CXXFLAGS) $< -o $@

$(HEX_FILE): $(ELF_FILE)
	avr-objcopy -O ihex $< $@

clean:
	rm -rf "$(HEX_FILE)" "$(ELF_FILE)"

# Program using Arduino
burn: $(HEX_FILE)
	avrdude -p $(PART) -c arduino -b $(BOOTLOADER_BAUD_RATE) -D -P $(SERIAL_PORT) -U flash:w:"$(HEX_FILE)"

# Program using AVRISP mkII (faster & can use more flash, but stomps the bootloader)
burn2: $(HEX_FILE)
	avrdude -p $(PART) -c avrisp2 -P usb -U flash:w:"$(HEX_FILE)"

report: $(ELF_FILE)
	avr-size -C "$(ELF_FILE)" --mcu=$(PART)

.PHONY: clean burn
