// SPDX-License-Identifier: 0BSD
// Copyright (C) 2026 Grigoriy Loboda

#pragma once

#include "defs.h"

#ifdef __cplusplus
#define externC extern "C"
#else
#define externC
#endif

/* initialize hardware */
externC void hal_setup(void);

/* sleep for given delay in ms */
externC void hal_delay(int delay);

/* read switches state and return it as u8 */
externC u8 hal_read(void);

/* receive and show u8 representing 8 lights state  */
externC void hal_write(u8 light_state);
