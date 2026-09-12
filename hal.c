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
#ifndef PWM_TRANS_SPEED
#define PWM_TRANS_SPEED 12
#endif

/* baud rate for logging (see also: print.h) */
#ifndef BAUD
#define BAUD 9600
#endif
#include <util/setbaud.h>

static volatile unsigned char light_state_current;
static volatile unsigned char light_state_prev;
static volatile unsigned char trans_level;

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

static void setup_lights(void);

static const unsigned char LIGHT_MASK =
    (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3) | (1 << PC4) | (1 << PC5);
static const unsigned char SWITCH_MASK =
    (1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6);
static const unsigned char SWITCH_SHIFT = 2;

void hal_setup(void)
{
	cli();

	// switches: PD2 PD3 PD4 PD5 PD6 (5 items, internally pulled up)

	DDRD &= ~SWITCH_MASK;
	PORTD |= SWITCH_MASK;

	// pin change interrupts on switches pins

	PCICR = (1 << PCIE2);
	// pins PD2..PD6 correspond to PCINT18..PCINT22
	PCMSK2 = (1 << PCINT18) | (1 << PCINT19) | (1 << PCINT20) |
		 (1 << PCINT21) | (1 << PCINT22);

	// initialize lights subsystem
	setup_lights();

	// USART0 is used for printing (see also print.h)
	// params: 8data, no parity
	// baudrate provided by -DBAUD=... cc option, fallbacks to 9600

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

	// sleeping mode
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);

	// power reduction by disabling unneeded modules
	PRR = (1 << PRTWI) | (1 << PRSPI) | (1 << PRADC) | (1 << PRTIM2);

	sei();
}

static inline void hal_write_internal(unsigned char light_state);

static void setup_lights(void)
{
	DDRC |= LIGHT_MASK;
	PORTC &= ~LIGHT_MASK;

	// timer 0 (PWM in software)
	TCCR0A = 0;
	TCCR0B = (0 << CS02) | (0 << CS01) | (1 << CS00);

	// timer 1 (lights state transition)
	TCCR1A = 0;
	TCCR1B = (0 << CS12) | (0 << CS11) | (1 << CS10);

	light_state_prev = 0;
	light_state_current = 0;

	hal_write_internal(0);
}

static inline void hal_write_internal(unsigned char light_state)
{
	PORTC = (PORTC & ~LIGHT_MASK) | (light_state & LIGHT_MASK);
}

void hal_write(unsigned char light_state)
{
	if (light_state != light_state_current) {
		cli();
		light_state_prev = light_state_current;
		light_state_current = light_state;
		trans_level = 1;
		sleep_inhibitors.timers = 1;
		OCR0A = trans_level;
		TIMSK0 |= (1 << TOIE0) | (1 << OCIE0A);
		TIMSK1 |= (1 << TOIE1);
		sei();
	}
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
		TIMSK0 &= ~((1 << TOIE0) | (1 << OCIE0A));
		TIMSK1 &= ~(1 << TOIE1);
		sleep_inhibitors.timers = 0;
		hal_write_internal(light_state_current);
	} else {
		OCR0A = trans_level;
	}
}

ISR(TIMER0_OVF_vect)
{
	hal_write_internal(light_state_current);
}

ISR(TIMER0_COMPA_vect)
{
	hal_write_internal(light_state_prev);
}

void hal_delay(unsigned char milli)
{
	while (milli--)
		_delay_ms(1);
}

/* must not be called from any ISR */
void hal_sleep_until_switch_change(void)
{
	while (!has_input_changed && !is_sleep_inhibited())
		;
	while (!has_input_changed)
		sleep_mode();
	has_input_changed = 0;
}

ISR(PCINT2_vect)
{
	has_input_changed = 1;
}

unsigned char hal_read(void)
{
	unsigned char result = PIND;

	// note ~ operator (internal pullups)
	result = (~(result | (~SWITCH_MASK))) >> SWITCH_SHIFT;
	return result;
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
