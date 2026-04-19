#pragma once

typedef int tl_bool;

#define true 1
#define false 0

#ifdef STDINT_MISSING
typedef __INT8_TYPE__ int8_t;
typedef __INT16_TYPE__ int16_t;
typedef __INT32_TYPE__ int32_t;
typedef __INT64_TYPE__ int64_t;
typedef __UINT8_TYPE__ uint8_t;
typedef __UINT16_TYPE__ uint16_t;
typedef __UINT32_TYPE__ uint32_t;
typedef __UINT64_TYPE__ uint64_t;
typedef __SIZE_TYPE__ size_t;
typedef __PTRDIFF_TYPE__ ptrdiff_t;
#else
#include <stdint.h>
#include <stddef.h>
#endif // STDINT_MISSING

void* tl_memcpy(void* restrict dest, const void* restrict src, size_t count);
size_t tl_strlen(const char* s);
