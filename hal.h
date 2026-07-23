// SPDX-License-Identifier: 0BSD
// Copyright (C) 2026 Grigoriy Loboda

#pragma once

#include "defs.h"

#ifdef __cplusplus
#define externC extern "C"
#else
#define externC
#endif

externC u8 hal_read(void);
externC void hal_write(u8 light_state);
