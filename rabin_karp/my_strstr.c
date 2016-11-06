#include "my_strstr.h"
#include <string.h>

unit_static const int Q = 7919;

unit_static int remainder_of_power(int x, int power, int div)
{
    int remainder = 1 % div;
    for (int i = 0; i < power; ++i)
        remainder = (remainder * (x % div)) % div;
    return remainder;
}

unit_static int get_hash(const char * str, size_t start, size_t end)
{
    int res = 0;
    for (size_t i = start; i < end; ++i)
        res = (res + str[i] * remainder_of_power(256, end - i - 1, Q)) % Q;
    return res;
}

unit_static int get_hash_next(const char *str, size_t start, size_t end, int prev_hash)
{
    // if we call function on first char
    if (start == 0)
        return prev_hash;
    return ((prev_hash - (str[start - 1] * remainder_of_power(256, end - start - 1, Q) % Q) + Q)
            % Q * 256 + str[end - 1]) % Q;
}

char *my_strstr(char *haystack, const char *needle)
{
    if (haystack == NULL || needle == NULL)
        return NULL;
    size_t needle_len = strlen(needle);
    int needle_hash = get_hash(needle, 0, needle_len);
    int curr_hash = get_hash(haystack, 0, needle_len);
    for (size_t i = 0; i < strlen(haystack); ++i)
    {
        curr_hash = get_hash_next(haystack, i, i + needle_len, curr_hash);
        if (curr_hash != needle_hash)
            continue;
        for (size_t j = 0; j < needle_len; ++j)
        {
            if (haystack[i + j] != needle[j])
                break;
            return haystack + i;
        }
    }
    return NULL;
}
