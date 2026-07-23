# SPDX-License-Identifier: 0BSD
# Copyright (C) 2026 Grigoriy Loboda

HEADERS := hal.h puzzle.h
SOURCES := hal.c puzzle.c main.c
TARGET := tardis_lights
TARGET_ELF := $(TARGET).elf
TARGET_HEX := $(TARGET).hex

DEVICE := atmega328p
FREQUENCY := 16000000UL

RM 	:= rm -f
STRIP 	:= avr-strip
SIZE 	:= avr-size --format=sysv
CC 	:= avr-gcc

CFLAGS := 	-mmcu="$(DEVICE)" -DF_CPU=$(FREQUENCY) \
		-O3 -Wall -Wextra -Wpedantic -Werror -std=c23

CLANG_FORMAT := clang-format
CLANG_FORMAT_OPTIONS := --Werror
CLANG_FORMAT_FIX_OPTIONS := $(CLANG_FORMAT_OPTIONS) -i
CLANG_FORMAT_CHECK_OPTIONS := $(CLANG_FORMAT_OPTIONS) --dry-run
CLANG_FORMAT_SOURCES := $(SOURCES) $(HEADERS)

PHONY += all
all: build

PHONY += clean
clean:
	$(RM) $(TARGET_ELF) $(TARGET_HEX)

PHONY += build
build: check $(TARGET_ELF)

$(TARGET_ELF): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET_ELF)
	$(STRIP) $(TARGET_ELF)
	$(SIZE) $(TARGET_ELF)

PHONY += check
check: clang-format-check

PHONY += clang-format-check
clang-format-check:
	$(CLANG_FORMAT) $(CLANG_FORMAT_CHECK_OPTIONS) $(CLANG_FORMAT_SOURCES)

PHONY += clang-format-fix
clang-format-fix:
	$(CLANG_FORMAT) $(CLANG_FORMAT_FIX_OPTIONS) $(CLANG_FORMAT_SOURCES)

PHONY += help
help:
	@echo "  Syntax: make [TARGET]... [VARIABLE=value]..."
	@echo
	@echo "  all                - default target, same as build"
	@echo "  clean              - remove all build artifacts"
	@echo "  build              - build the program"
	@echo "  check              - same as clang-format-check (other analyzers might appear later)"
	@echo "  clang-format-check - use clang-format to check for styling violations"
	@echo "  clang-format-fix   - use clang-format to fix styling violations"
	@echo "  help               - display this information"

.PHONY: $(PHONY)
