// SPDX-License-Identifier: 0BSD
// Copyright (C) 2026 Grigoriy Loboda

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "print.h"
#include "hal.h"

#ifndef BAUD
#define BAUD 9600
#endif
#include <util/setbaud.h>

void hal_setup(void)
{
	// switches:  D2  D3  D4  D5  D6 (5 items, all internally pulled up)
	//           PD2 PD3 PD4 PD5 PD6
	// lights:  A0,  A1,  A2,  A3,  A4,  A5 (6 items)
	//         PC0, PC1, PC2, PC3, PC4, PC5
	// builtin LED at D13 (PB5) blinks occasionally for debugging purposes
	//     (it indicates that the program is alive)

	// switches

	DDRD &= ~0x7C;
	PORTD |= 0x7C;

	// lights

	DDRC |= 0x3F;
	PORTC &= ~0x3F;

	// builtin LED

	DDRB |= (1 << PB5);
	PORTB &= ~(1 << PB5);

	cli();

	// USART0 is used for printing (print.h)
	// params: 9600baud, 8data, no parity

	UBRR0H = UBRRH_VALUE; // defined by <util/setbaud.h>
	UBRR0L = UBRRL_VALUE;
#if USE_2X // defined by <util/setbaud.h>
	UCSR0A |= (1 << U2X0);
#else
	UCSR0A &= ~(1 << U2X0);
#endif
	UCSR0B = (1 << TXEN0);			   // transmitter enabled
	UCSR0C = (0 << UMSEL00) | (0 << UMSEL01) | // asynchronous mode
		 (1 << UCSZ01) | (1 << UCSZ00);	   // 8data

	// timer 1 (builtin led blink)
	TCCR1A = 0;
	TCCR1B = (0 << CS12) | (1 << CS11) | (1 << CS10);
	TIMSK1 = (1 << TOIE1);

	sei();
}

void hal_delay(unsigned char milli)
{
	while (milli--)
		_delay_ms(1);
}

unsigned char hal_read(void)
{
	unsigned char result = PIND;

	// note ~ operator; it is here because of pull up resistors being used
	result = (~(result >> 2)) & 0x1F;
	return result;
}

void hal_write(unsigned char light_state)
{
	PORTC = (PORTC & ~0x3F) | (light_state & 0x3F);
}

void hal_start_print_buffer_transmission(void)
{
	// enable UDRE interrupt
	UCSR0B |= (1 << UDRIE0);
}

ISR(USART_UDRE_vect)
{
	if (!pr_empty()) {
		unsigned char next = pr_getch();

		UDR0 = next;
	} else {
		// disable UDRE interrupt
		UCSR0B &= ~(1 << UDRIE0);
	}
}

ISR(TIMER1_OVF_vect)
{
	PINB |= (1 << PB5);
}
