// SPDX-License-Identifier: 0BSD
// Copyright (C) 2026 Grigoriy Loboda

#pragma once

#include "defs.h"

/* initialize hardware */
void hal_setup(void);

/* sleep for given delay in ms */
void hal_delay(u8 delay);

/* read switches state and return it as u8 */
u8 hal_read(void);

/* receive and show u8 representing 8 lights state  */
void hal_write(u8 light_state);
