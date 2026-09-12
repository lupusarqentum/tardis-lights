# SPDX-License-Identifier: 0BSD
# Copyright (C) 2026 Grigoriy Loboda

TARGET		:= tardis_lights
TARGET_ELF	:= $(TARGET).elf
TARGET_HEX	:= $(TARGET).hex

PORT := /dev/ttyUSB0

# common baud rate for communicating with bootloader and the main program
BAUDRATE	:= 57600

DEVICE		:= atmega328p
PGM		:= arduino
PGM_BAUD	:= $(BAUDRATE)
FREQUENCY	:= 16000000UL

HEADERS := print.h hal.h puzzle.h
SOURCES := print.c hal.c puzzle.c main.c
OBJECTS := print.o hal.o puzzle.o main.o

# variables below probably don't need to be changed

AVRDUDE	:= avrdude
RM 	:= rm -f
SIZE 	:= avr-size --format=berkeley
CC 	:= avr-gcc
OBJCOPY	:= avr-objcopy

CFLAGS := 		-mmcu="$(DEVICE)" -DF_CPU=$(FREQUENCY)		\
			-Os -Wall -Wextra -Wpedantic -Werror -std=c11	\
			-DBAUD=$(BAUDRATE)
AVRDUDE_OPTIONS :=	-P $(PORT) -v -p$(DEVICE) -c$(PGM) -b$(PGM_BAUD) -D -Uflash:w:$(TARGET_HEX):i
OBJCOPY_OPTIONS :=	-j .text -j .data -O ihex

CLANG_FORMAT			:= clang-format
CLANG_FORMAT_OPTIONS		:= --Werror
CLANG_FORMAT_FIX_OPTIONS	:= $(CLANG_FORMAT_OPTIONS) -i
CLANG_FORMAT_CHECK_OPTIONS	:= $(CLANG_FORMAT_OPTIONS) --dry-run
CLANG_FORMAT_SOURCES		:= $(SOURCES) $(HEADERS)

MONITOR_COMMAND := ./monitor.sh $(PORT) $(BAUDRATE)

PHONY += all
all: check build size

PHONY += clean
clean:
	$(RM) $(TARGET_ELF) $(TARGET_HEX) $(OBJECTS)

PHONY += build
build: $(TARGET_HEX)

PHONY += size
size: $(TARGET_ELF)
	$(SIZE) $(TARGET_ELF)

PHONY += flash
flash: $(TARGET_HEX)
	$(AVRDUDE) $(AVRDUDE_OPTIONS)

PHONY += monitor
monitor:
	$(MONITOR_COMMAND)

PHONY += check
check: clang-format-check

PHONY += clang-format-check
clang-format-check:
	$(CLANG_FORMAT) $(CLANG_FORMAT_CHECK_OPTIONS) $(CLANG_FORMAT_SOURCES)

PHONY += clang-format-fix
clang-format-fix:
	$(CLANG_FORMAT) $(CLANG_FORMAT_FIX_OPTIONS) $(CLANG_FORMAT_SOURCES)

$(TARGET_HEX): $(TARGET_ELF)
	$(OBJCOPY) $(OBJCOPY_OPTIONS) $(TARGET_ELF) $(TARGET_HEX)

$(TARGET_ELF): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET_ELF)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

PHONY += help
help:
	@echo "  Syntax: make [TARGET]... [VARIABLE=value]..."
	@echo
	@echo "  all                - default target = check, build, size"
	@echo "  clean              - remove all build artifacts"
	@echo "  build              - build the binary"
	@echo "  size               - display sizes of sections of the binary"
	@echo "  flash              - flash the binary, uses PORT variable"
	@echo "  monitor            - monitor program logs during execution, uses PORT variable"
	@echo "  check              - same as clang-format-check (other analyzers might appear later)"
	@echo "  clang-format-check - use clang-format to check for styling violations"
	@echo "  clang-format-fix   - use clang-format to fix styling violations"
	@echo "  help               - display this information"
	@echo
	@echo "  .PHONY targets list: $(PHONY)"
	@echo
	@echo "  DEVICE           - mcu"
	@echo "                       defaulted to: $(DEVICE)"
	@echo "  FREQUENCY        - mcu frequency"
	@echo "                       defaulted to: $(FREQUENCY)"
	@echo "  PORT             - avrdude port option"
	@echo "                       defaulted to: $(PORT)"
	@echo "  PGM              - avrdude programmer option"
	@echo "                       defaulted to: $(PGM)"
	@echo "  PGM_BAUD         - avrdude baud option"
	@echo "                       defaulted to: $(PGM_BAUD)"
	@echo "  BAUDRATE         - default baud rate for avrdude and C code"
	@echo "                       defaulted to: $(BAUDRATE)"
	@echo "  AVRDUDE_OPTIONS  - avrdude options"
	@echo "                       defaulted to: $(AVRDUDE_OPTIONS)"

.PHONY: $(PHONY)
