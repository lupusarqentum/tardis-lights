// SPDX-License-Identifier: 0BSD
// Copyright (C) 2026 Grigoriy Loboda

#include "avr/io.h"
#include "util/delay.h"

#include "hal.h"

void hal_setup(void)
{
	// we are using PC1..PC5 for switches (5 switches are mapped)
	// and PD2..PD7 for lights (6 lights are mapped)
	// other pins are unused

	DDRC = 0;
	DDRD = 0xFF;
	PORTC = 0xFF;
	PORTD = 0;
}

void hal_delay(u8 delay)
{
	while (delay--)
		_delay_ms(1);
}

u8 hal_read(void)
{
	u8 result = PIND;
	return (result >> 2) & 0x1F;
}

void hal_write(u8 light_state)
{
	light_state = (light_state & 0x3F) << 1;
	PORTC = light_state;
}
