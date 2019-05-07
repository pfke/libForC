#include "lfcStringOps.h"

#include <stddef.h>
#include <string.h>
#include <stdlib.h>

char *lfcStringOps_replace(
    const char *haystack,
    const char *needle,
    const char *replace
) {
    char *result;
    int i, cnt = 0;

    if (!haystack) { return NULL; }
    if (!needle) { return NULL; }
    if (!replace) { return NULL; }

    size_t needleLen = strlen(needle);
    size_t replaceLen = strlen(replace);

    // Counting the number of times needle word
    // occur in the string
    for (i = 0; haystack[i] != '\0'; i++) {
        if (strstr(&haystack[i], needle) == &haystack[i]) {
            cnt++;

            // Jumping to index after the needle word.
            i += needleLen - 1;
        }
    }

    // Making new string of enough length
    result = (char *)malloc(i + cnt * (replaceLen - needleLen) + 1);
    if (!result) { return NULL; }

    i = 0;
    while (*haystack) {
        // compare the substring with the result
        if (strstr(haystack, needle) == haystack) {
            strcpy(&result[i], replace);
            i += replaceLen;
            haystack += needleLen;
        } else {
            result[i++] = *haystack++;
        }
    }

    result[i] = '\0';
    return result;
}
