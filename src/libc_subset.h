#pragma once

#ifdef LIBC_ABSENT

    #ifdef __cplusplus
    #else // __cplusplus
        #define bool int
        #define true 1
        #define false 0
    #endif // __cplusplus

    void* memcpy(void* dest, const void* src, int n);
    int strlen(const char* s);

#else // LIBC_ABSENT

    #include <string.h>
    #include <stdbool.h>

#endif // LIBC_ABSENT
