// SPDX-License-Identifier: 0BSD
// Copyright (C) 2026 Grigoriy Loboda

#pragma once

/* initialize hardware */
void hal_setup(void);

/* sleep for given delay in ms */
void hal_delay(unsigned char milli);

/* wait until any switch changes state, possibly saving energy */
void hal_sleep_until_switch_change(void);

/* read switches state and return it as u8 */
unsigned char hal_read(void);

/* receive and show u8 representing 8 lights state  */
void hal_write(unsigned char light_state);

/* start consuming transmission of bytes from print buffer */
void hal_start_print_buffer_transmission(void);
