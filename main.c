// SPDX-License-Identifier: 0BSD
// Copyright (C) 2026 Grigoriy Loboda

#include <avr/io.h>

#include "hal.h"
#include "print.h"
#include "puzzle.h"

/* intended to run at the start to check if everything is wired correctly */
static void test_lights_wiring(void)
{
	/* blink all LEDs to check that they are wired */
	for (unsigned char i = 2; i--;) {
		hal_write(0xFF);
		hal_delay(200);
		hal_write(0);
		hal_delay(200);
	}

	/* blink all LEDs separately to check that no LEDs are intermixed */
	for (unsigned char i = 0; i < 8; i++) {
		unsigned char light_state = 1 << i;

		hal_write(light_state);
		hal_delay(200);
	}

	for (unsigned char i = 0; i < 8; i++) {
		unsigned char light_state = ~(1 << i);

		hal_write(light_state);
		hal_delay(250);
	}
}

static void log_states(unsigned char time, unsigned char switches,
		       unsigned char lights)
{
	pr_string("0x");
	pr_hex(time);
	pr_string(" ");
	pr_string("New state detected SW: ");
	pr_binary(switches);
	pr_string(" LED: ");
	pr_binary(lights);
	pr_char('\n');
}

int main(void)
{
	unsigned char prev_switch, prev_light;
	unsigned char input_changes_count = 0;

	hal_setup();

	pr_string("Running lights wiring selftest!\n");
	hal_start_print_buffer_transmission();

	test_lights_wiring();

	while (1) {
		unsigned char switch_state, light_state;

		switch_state = hal_read();
		if (input_changes_count == 0 || switch_state != prev_switch) {
			input_changes_count++;
			light_state = puzzle_update(switch_state);
			hal_write(light_state);
			log_states(input_changes_count, switch_state,
				   light_state);
			hal_start_print_buffer_transmission();
		} else {
			light_state = prev_light;
		}

		prev_light = light_state;
		prev_switch = switch_state;

		hal_delay(10);
		hal_sleep_until_switch_change();
		pr_string("Wake up the wicked!\n");
		hal_start_print_buffer_transmission();
	}
}
