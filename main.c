// SPDX-License-Identifier: 0BSD
// Copyright (C) 2026 Grigoriy Loboda

#include "hal.h"
#include "puzzle.h"

/* intended to run at the start to check if everything is wired correctly */
void test_lights_wiring()
{
	/* blink all LEDs to check that they are wired */
	for (unsigned char i = 4; i--;) {
		hal_write(0xFF);
		hal_delay(150);
		hal_write(0);
		hal_delay(150);
	}

	/* blink all LEDs separately to check that no LEDs are intermixed */
	for (unsigned char i = 0; i < 8; i++) {
		unsigned char light_state = 1 << i;

		hal_write(light_state);
		hal_delay(150);
	}
}

int main()
{
	hal_setup();

	test_lights_wiring();

	while (1) {
		unsigned char switch_state, light_state;

		switch_state = hal_read();
		light_state = puzzle_update(switch_state);
		hal_write(light_state);
		hal_delay(10);
	}
}
