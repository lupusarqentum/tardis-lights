// SPDX-License-Identifier: 0BSD
// Copyright (C) 2026 Grigoriy Loboda

#include <stdint.h>

#include "hal.h"
#include "print.h"
#include "puzzle.h"

/* intended to run at the start to check if everything is wired correctly */
static void test_lights_wiring(void)
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

static void log_message(uint16_t time, const char* msg, unsigned char binary)
{
	unsigned char low = time & 0xFF;
	unsigned char high = (time >> 8) & 0xFF;

	pr_string("0x");
	pr_hex(high);
	pr_hex(low);
	pr_string(": ");
	pr_string(msg);
	pr_binary(binary);
	pr_char('\n');
}

int main(void)
{
	uint16_t iteration_number = 0;
	unsigned char prev_switch, prev_light;
	unsigned char first_iteration = 1;

	hal_setup();

	test_lights_wiring();

	while (1) {
		unsigned char switch_state, light_state;

		switch_state = hal_read();
		if (first_iteration || switch_state != prev_switch) {
			light_state = puzzle_update(switch_state);
			hal_write(light_state);
			log_message(iteration_number,
				    "New switches state: ", switch_state);
			log_message(iteration_number,
				    "New lights state:   ", light_state);
			hal_start_print_buffer_transmission();
		} else {
			light_state = prev_light;
		}

		iteration_number++;
		prev_light = light_state;
		prev_switch = switch_state;
		first_iteration = 0;

		hal_delay(10);
	}
}
