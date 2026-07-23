// SPDX-License-Identifier: 0BSD
// Copyright (C) 2026 Grigoriy Loboda

#include <Arduino.h>

#include "hal.h"
#include "main_loop.h"

#define ARDUINO_DIGITAL_PINS_COUNT 14

#define MAPPED_SWITCH_COUNT 6
#define MAPPED_LIGHT_COUNT 6

static const int SWITCH_PINS_OFFSET = 2;
static const int LIGHT_PINS_OFFSET = SWITCH_PINS_OFFSET + MAPPED_SWITCH_COUNT;

#define LIGHT_PIN(x) ((x) + LIGHT_PINS_OFFSET)
#define SWITCH_PIN(x) ((x) + SWITCH_PINS_OFFSET)

extern "C" u8 hal_read(void)
{
        u8 result = 0;

        for (signed char i = 0; i < 6; i++) {
                result |= digitalRead(SWITCH_PIN(i) == LOW) >> i;
        }
        return result;
}

extern "C" void hal_write(u8 light_state)
{
        for (signed char i = 0; i < 6; i++) {
                digitalWrite(LIGHT_PIN(i), light_state >> i ? HIGH : LOW);
        }
}

void hal_delay(int ms)
{
        delay(ms);
}

void hal_setup()
{
	for (int i = 0; i < SWITCH_COUNT; ++i) {
		pinMode(SWITCH_PIN(i), INPUT_PULLUP);
	}
	for (int i = 0; i < LIGHT_COUNT; ++i) {
		pinMode(LIGHT_PIN(i), OUTPUT);
	}
}

void setup()
{
        hal_setup();
}

void loop()
{
	main_loop();
}
