// SPDX-License-Identifier: 0BSD
// Copyright (C) 2026 Grigoriy Loboda

#pragma once

#include "libc_subset.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

tl_bool hal_read(int switch_number);
void hal_write(int light_number, tl_bool value);

#ifdef __cplusplus
}
#endif // __cplusplus
