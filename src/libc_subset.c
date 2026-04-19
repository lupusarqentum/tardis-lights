#include "libc_subset.h"

#ifdef LIBC_ABSENT

void* memcpy(void* dest, const void* src, int n) {
    char* dest_char = (char*)dest;
    char* src_char = (char*)src;
    for (int i = 0; i < n; ++i) {
        dest_char[i] = src_char[i];
    }
    return dest;
}

int strlen(const char* s) {
    int result = 0;
    while (s[result] != '\0') {
        result++;
    }
    return result;
}

#endif // LIBC_ABSENT
