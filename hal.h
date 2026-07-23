// SPDX-License-Identifier: 0BSD
// Copyright (C) 2026 Grigoriy Loboda

#pragma once

/* initialize hardware */
void hal_setup(void);

/* sleep for given delay in ms */
void hal_delay(unsigned char delay);

/* read switches state and return it as u8 */
unsigned char hal_read(void);

/* receive and show u8 representing 8 lights state  */
void hal_write(unsigned char light_state);
