# Sets the clock frequency being used with the target device
ifndef FREQUENCY
FREQUENCY := 16000000
endif

# Lets you override the programmer, port, etc
ifndef AVRDUDE_FLAGS
AVRDUDE_FLAGS = -c avrisp2 -P usb
endif

ifndef PROJECT_NAME
PROJECT_NAME := project
endif

ifndef PART
$(error You must specify a PART, e.g. "atmega328p")
endif

AVRDUDE = avrdude -qq -p $(PART) $(AVRDUDE_FLAGS)

# intermediate files
ELF_FILE := $(PROJECT_NAME).elf
HEX_FILE := $(PROJECT_NAME).hex

ALL_CXXFLAGS = -mmcu=$(PART) -DF_CPU=$(FREQUENCY)UL $(CXXFLAGS)

CXXFLAGS = -Os -Wall -pedantic \
			-fpack-struct -fshort-enums -ffunction-sections -fdata-sections \
			-Wl,--relax,--gc-sections

all: $(HEX_FILE)

$(ELF_FILE): $(SOURCE_FILES)
	avr-g++ $(ALL_CXXFLAGS) $< -o $@

$(HEX_FILE): $(ELF_FILE)
	avr-objcopy -O ihex $< $@

clean:
	rm -rf "$(HEX_FILE)" "$(ELF_FILE)" *.fuse.tmp

burn: $(HEX_FILE)
	$(AVRDUDE) -U flash:w:"$(HEX_FILE)"

read-fuses:
	$(AVRDUDE) -U lfuse:r:lfuse.fuse.tmp:h -U hfuse:r:hfuse.fuse.tmp:h -U efuse:r:efuse.fuse.tmp:h
	@echo -n "LFUSE: "
	@cat lfuse.fuse.tmp
	@echo -n "HFUSE: "
	@cat hfuse.fuse.tmp
	@echo -n "EFUSE: "
	@cat efuse.fuse.tmp

write-fuses:
ifndef LFUSE
	$(error LFUSE not set)
endif
ifndef HFUSE
	$(error HFUSE not set)
endif
ifndef EFUSE
	$(error EFUSE not set)
endif
	@echo "Are you ABSOLUTELY SURE you want to write the following fuse bits?"
	@echo
	@echo "  Part: $(PART)"
	@echo
	@echo "  LFUSE: $(LFUSE)"
	@echo "  HFUSE: $(HFUSE)"
	@echo "  EFUSE: $(EFUSE)"
	@echo
	@echo -n "Please type YES if so: "
	@read WRITE_FUSE_CONFIRMATION ; \
	if [ "$$WRITE_FUSE_CONFIRMATION" != "YES" ] ; then \
		echo >&2 "Aborting." ;	\
		false ; \
	fi
	$(AVRDUDE) -U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m -U efuse:w:$(EFUSE):m

report: $(ELF_FILE)
	avr-size -C "$(ELF_FILE)" --mcu=$(PART)

.PHONY: clean burn read-fuses write-fuses report
