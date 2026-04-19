#include "libc_subset.h"

void* tl_memcpy(void* dest, const void* src, size_t count) {
    char* dest_char = (char*)dest;
    char* src_char = (char*)src;
    for (size_t i = 0; i < count; ++i) {
        dest_char[i] = src_char[i];
    }
    return dest;
}

size_t tl_strlen(const char* s) {
    size_t result = 0;
    while (s[result] != '\0') {
        result++;
    }
    return result;
}
