#pragma once

#include "libc_subset.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

bool hal_read(int switch_number);
void hal_write(int light_number, bool value);
void hal_log(const char* message);

#ifdef __cplusplus
}
#endif // __cplusplus
