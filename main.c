// SPDX-License-Identifier: 0BSD
// Copyright (C) 2026 Grigoriy Loboda

#include "defs.h"
#include "hal.h"
#include "puzzle.h"

int main()
{
	hal_setup();

	while (1) {
		u8 switch_state, light_state;

		switch_state = hal_read();
		light_state = puzzle_update(switch_state);
		hal_write(light_state);
		hal_delay(750);
	}
}
