#ifndef CCONSTRUCT_STR_C
#define CCONSTRUCT_STR_C

//! str.c: provides string manipulation functions.

#include <string.h>

/// Remove prefix from str
char* cc_strremstart(char* s, char* prefix) {
    size_t prefix_len = strlen(prefix);
    size_t s_len = strlen(s);

    if (prefix_len >= s_len) return s;

    size_t i = 0;
    while (*s == prefix[i]) {
        i++;
        s++;
    }
    return s;
}

char* cc_strremend(char* s, char* suffix) {
    size_t suffix_len = strlen(suffix);
    size_t s_len = strlen(s);

    if (suffix_len >= s_len) return s;

    size_t i = s_len - 1, j = suffix_len - 1;
    while (s[i] == suffix[j] && j > 0) {
        i--;
        j--;
    }
    s[i] = '\0';
    return s;
}


#endif // CCONSTRUCT_STR_C