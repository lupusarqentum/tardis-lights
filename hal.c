// SPDX-License-Identifier: 0BSD
// Copyright (C) 2026 Grigoriy Loboda

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "print.h"
#include "hal.h"

/* if bigger MCU will transition one state of lights into another faster */
/* (smooth transitions) */
/* set to high values for instant transitions (e.g. 1000) */
#define PWM_TRANS_SPEED 12

/* baud rate for logger */
#ifndef BAUD
#define BAUD 9600
#endif
#include <util/setbaud.h>

static unsigned char light_state_current;
static unsigned char light_state_prev;
static unsigned char trans_level;
static unsigned char pwm_counter;

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

	light_state_current = 0;
	light_state_prev = light_state_current;
	trans_level = 255;

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

	// timer 0 (PWM in software)

	TCCR0A = 0;
	TCCR0B = (0 << CS02) | (0 << CS01) | (1 << CS00);

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

static void hal_write_internal(unsigned char light_state)
{
	PORTC = (PORTC & ~0x3F) | (light_state & 0x3F);
}

void hal_write(unsigned char light_state)
{
	if (light_state != light_state_current) {
		light_state_prev = light_state_current;
		light_state_current = light_state;
		trans_level = 0;
		TIMSK0 |= (1 << TOIE0);
	}
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
	PINB = (1 << PB5);
}

#define PWM_TRANS_STEP_WIDE (16000000UL / F_CPU * PWM_TRANS_SPEED)
#define PWM_TRANS_STEP                                                         \
	(PWM_TRANS_STEP_WIDE >= 255                                            \
	     ? 255                                                             \
	     : (PWM_TRANS_STEP_WIDE == 0                                       \
		    ? 1                                                        \
		    : (unsigned char)PWM_TRANS_STEP_WIDE))

ISR(TIMER0_OVF_vect)
{
	pwm_counter++;
	if (pwm_counter == 0) {
		const unsigned char step = PWM_TRANS_STEP;

		trans_level =
		    trans_level < 255 - step ? trans_level + step : 255;
		if (trans_level == 255) {
			hal_write_internal(light_state_current);
			TIMSK0 &= ~(1 << TOIE0);
			return;
		}
	}
	if (pwm_counter < trans_level)
		hal_write_internal(light_state_current);
	else
		hal_write_internal(light_state_prev);
}
