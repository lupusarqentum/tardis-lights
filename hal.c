// SPDX-License-Identifier: 0BSD
// Copyright (C) 2026 Grigoriy Loboda

#include "avr/io.h"
#include "util/delay.h"

#include "hal.h"

void hal_setup(void)
{
	// switches:  D2  D3  D4  D5  D6 (5 items, all internally pulled up)
	//           PD2 PD3 PD4 PD5 PD6
	// lights:  A0,  A1,  A2,  A3,  A4,  A5 (6 items)
	//         PC0, PC1, PC2, PC3, PC4, PC5
	// for an additional external pull up D12 (PB4) is driven high

	DDRC |= 0x3F;
	PORTC &= ~0x3F;

	DDRB |= (1 << 4);
	PORTB |= (1 << 4);

	DDRD &= ~0x7C;
	PORTD |= 0x7C;
}

void hal_delay(unsigned char milli)
{
	while (milli--)
		_delay_ms(1);
}

unsigned char hal_read(void)
{
	unsigned char result = PIND;
	result = (~(result >> 2)) & 0x1F;
	return result;
}

void hal_write(unsigned char light_state)
{
	PORTC = (PORTC & ~0x3F) | (light_state & 0x3F);
}
