// SPDX-License-Identifier: 0BSD
// Copyright (C) 2026 Grigoriy Loboda

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "print.h"
#include "hal.h"

// this reduces code size better than -Os or -Oz because it allows compiler
// to inline fun calls in ISRs, allowing for shorter prologues and epilogues
#pragma GCC optimize("O3")

// if bigger MCU will transition one state of lights into another faster
// (smooth transitions)
// set to higher values (e.g. 1000) for more instant-like transitions
#define PWM_TRANS_SPEED 12

/* baud rate for logging (see also: print.h) */
#ifndef BAUD
#define BAUD 9600
#endif
#include <util/setbaud.h>

static volatile unsigned char light_state_current;
static volatile unsigned char light_state_prev;
static volatile unsigned char trans_level;
static volatile unsigned char pwm_counter;

static volatile unsigned char has_input_changed;

// inhibitors prevent MCU from going into deeper sleep mode,
// CONTRACT: these can be set on main thread only (i.e. not in ISR)
static volatile struct {
	unsigned char uart;
	unsigned char timers;
} sleep_inhibitors;

static unsigned char is_sleep_inhibited(void)
{
	return !sleep_inhibitors.timers && !sleep_inhibitors.uart;
}

void hal_setup(void)
{
	// switches: PD2 PD3 PD4 PD5 PD6 (5 items, internally pulled up)
	// lights: PC0, PC1, PC2, PC3, PC4, PC5 (6 items)

	// switches

	DDRD &= ~0x7C;
	PORTD |= 0x7C;

	// lights

	DDRC |= 0x3F;
	PORTC &= ~0x3F;

	light_state_current = 0;
	light_state_prev = 0;
	trans_level = 255;

	cli();

	// USART0 is used for printing (see also print.h)
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

	// timer 1 (smooth lights state transition)
	TCCR1A = 0;
	TCCR1B = (0 << CS12) | (0 << CS11) | (1 << CS10);
	TIMSK1 = (1 << TOIE1);

	// pin change interrupts on switches pins

	PCICR = (1 << PCIE2);
	// switch pins PD2..PD6 correspond to PCINT18..PCINT22
	PCMSK2 = (1 << PCINT18) | (1 << PCINT19) | (1 << PCINT20) |
		 (1 << PCINT21) | (1 << PCINT22);

	// sleeping mode
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);

	// power reduction by disabling unneeded modules
	PRR = (1 << PRTWI) | (1 << PRSPI) | (1 << PRADC) | (1 << PRTIM2);

	sei();
}

void hal_delay(unsigned char milli)
{
	while (milli--)
		_delay_ms(1);
}

/* must not be called from ISR */
void hal_sleep_until_switch_change(void)
{
	while (!has_input_changed && !is_sleep_inhibited())
		;
	while (!has_input_changed)
		sleep_mode();
	has_input_changed = 0;
}

unsigned char hal_read(void)
{
	unsigned char result = PIND;

	// note ~ operator (internal pullups)
	result = (~(result >> 2)) & 0x1F;
	return result;
}

static inline void hal_write_internal(unsigned char light_state)
{
	PORTC = (PORTC & ~0x3F) | (light_state & 0x3F);
}

void hal_write(unsigned char light_state)
{
	cli();
	if (light_state != light_state_current) {
		light_state_prev = light_state_current;
		light_state_current = light_state;
		trans_level = 0;
		sleep_inhibitors.timers = 1;
		TIMSK0 |= (1 << TOIE0);
		TIMSK1 |= (1 << TOIE1);
	}
	sei();
}

void hal_start_print_buffer_transmission(void)
{
	cli();
	sleep_inhibitors.uart = 1;
	// enable UDRE interrupt
	UCSR0B |= (1 << UDRIE0);
	sei();
}

ISR(USART_UDRE_vect)
{
	if (!pr_empty()) {
		unsigned char next = pr_getch();

		UDR0 = next;
	} else {
		// disable UDRE interrupt
		UCSR0B &= ~(1 << UDRIE0);
		sleep_inhibitors.uart = 0;
	}
}

ISR(PCINT2_vect)
{
	has_input_changed = 1;
}

#define PWM_TRANS_STEP_WIDE (16000000UL / F_CPU * PWM_TRANS_SPEED)
#define PWM_TRANS_STEP                                                         \
	(PWM_TRANS_STEP_WIDE >= 255                                            \
	     ? 255                                                             \
	     : (PWM_TRANS_STEP_WIDE == 0                                       \
		    ? 1                                                        \
		    : (unsigned char)PWM_TRANS_STEP_WIDE))

ISR(TIMER1_OVF_vect)
{
	trans_level += PWM_TRANS_STEP;
	if (trans_level > 255 - PWM_TRANS_STEP) {
		TIMSK0 &= ~(1 << TOIE0);
		TIMSK1 &= ~(1 << TOIE1);
		sleep_inhibitors.timers = 0;
		hal_write_internal(light_state_current);
	}
}

ISR(TIMER0_OVF_vect)
{
	pwm_counter++;
	if (pwm_counter == 0)
		hal_write_internal(light_state_current);
	else if (pwm_counter == trans_level)
		hal_write_internal(light_state_prev);
}
