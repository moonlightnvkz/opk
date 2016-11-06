#define _CRT_SECURE_NO_DEPRECATE

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wchar.h>
#include "reader.h"

/* Input non-empty string line from standart input.
 * Does not include '\n' character in the end. */
wchar_t *read_line(FILE *stream) {
    size_t str_len = 0;
    size_t buf_size = 16; // some initial size
    wchar_t *buf = malloc(buf_size * sizeof(wchar_t));
    if (buf == NULL) {
        return NULL;
    }

    for (;;) {
        // read next part
        if (fgetws(buf + str_len, buf_size - str_len, stream) == NULL) {
            if (feof(stream) && str_len > 0) {
                // read all given input
                break;
            } else {
                // no input at all or some error
                perror("Error");
                free(buf);
                return NULL;
            }
        }

        size_t added_len = wcslen(buf + str_len);
        str_len += added_len;
        assert(str_len + 1 <= buf_size);
        assert(buf[str_len] == L'\0');

        // check if we finished with reading
        if ((str_len + 1 < buf_size) ||     // read less than maximum
            (buf[str_len - 1] == L'\n')) {   // read '\n' as last character
            break;
        }

        // otherwise resize buffer for reading next part
        size_t new_buf_size = buf_size * 2;
        wchar_t *new_buf = realloc(buf, sizeof(wchar_t) * new_buf_size);
        if (new_buf == NULL) {
            free(buf);
            return NULL;
        }
        buf_size = new_buf_size;
        buf = new_buf;
    }

    assert(str_len > 0);

    // trim trailing '\n' if any
    if (buf[str_len - 1] == L'\n') {
        buf[str_len - 1] = L'\0';
    }

    // cut buffer to actual length
    wchar_t *result = realloc(buf, sizeof(wchar_t) * (str_len + 1));
    if (result == NULL) {
        free(buf);
        return NULL;
    }
    return result;
}
