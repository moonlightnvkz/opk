#ifdef _DEBUG
#include <stdio.h>
#include "test_my_strstr.h"
#include "my_strstr.h"

extern const int Q;
extern int remainder_of_power(int x, int power, int div);
extern int get_hash(const char * str, size_t start, size_t end);
extern int get_hash_next(const char *str, size_t start, size_t end, int prev_hash);

void remainder_failed(int x, int power, int div)
{
    fprintf(stderr, "Remainder failed on (%d, %d, %d)\n", x, power, div);
}

void get_hash_failed(char *str, size_t start, size_t end)
{
    fprintf(stderr, "Get hash failed on (%16s, %d, %d)\n", str, (int)start, (int)end);
}

void get_hash_next_failed(char *str, size_t start, size_t end, int prev_hash)
{
    fprintf(stderr, "Get hash next failed on (%16s, %d, %d, %d)\n", str, (int)start, (int)end, prev_hash);
}

void my_strstr_failed(char *haystack, char *needle)
{
    fprintf(stderr, "My strstr failed on (%s, %s)\n", haystack, needle);
}

void test_remainder()
{
    if (remainder_of_power(0, 0, 5) != 1)
        remainder_failed(0, 0, 5);
    if (remainder_of_power(1, 0, 1) != 0)
        remainder_failed(1, 0, 1);
    if (remainder_of_power(1, 5, 3) != 1)
        remainder_failed(1, 5, 3);
    if (remainder_of_power(5, 2, 25) != 0)
        remainder_failed(5, 2, 25);
    if (remainder_of_power(123, 6, 1739634) != 1012257)
        remainder_failed(123, 6, 1739634);
}

void test_get_hash()
{
    {
        char str[] = "abcdefghijklmnopqrstuvwxyz";
        if (get_hash(str, 0, 1) != 'a')
            get_hash_failed(str, 0, 1);
    }
    {
        char str[] = "abcdefghijklmnopqrstuvwxyz";
        if (get_hash(str, 0, 2) != ('a' * 256 + 'b') % Q)
            get_hash_failed(str, 0, 2);
    }
    {
        char str[] = "abcdefghijklmnopqrstuvwxyz";
        if (get_hash(str, 15, 16) != 'p' % Q)
            get_hash_failed(str, 15, 16);
    }
    {
        char str[] = "abcdefghijklmnopqrstuvwxyz";
        if (get_hash(str, 15, 18) != ('p' * 256 * 256 + 'q' * 256 + 'r') % Q)
            get_hash_failed(str, 15, 18);
    }
    {
        char str[] = "abcdefghijklmnopqrstuvwxyz";
        if (get_hash(str, 15, 21) != ('p' * 5012 % Q +
                                      'q' * 2618 % Q +
                                      'r' * 4774 % Q +
                                      's' * 2184 % Q +
                                      't' * 256  % Q +
                                      'u'        % Q   ) % Q)
            get_hash_failed(str, 15, 21);
    }
}


void test_get_hash_next()
{
    {
        char str[] = "abcdefghijklmnopqrstuvwxyz";
        int prev_hash = 'a';
        if (get_hash_next(str, 1, 2, prev_hash) != 'b')
            get_hash_next_failed(str, 1, 2, 'a');
    }
    {
        char str[] = "abcdefghijklmnopqrstuvwxyz";
        int prev_hash = ('a' * 256 + 'b') % Q;
        if (get_hash_next(str, 1, 3, prev_hash) != ('b' * 256 + 'c') % Q)
            get_hash_next_failed(str, 1, 3, ('a' * 256 + 'b') % Q);
    }
    {
        char str[] = "abcdefghijklmnopqrstuvwxyz";
        int prev_hash = 'p';
        if (get_hash_next(str, 16, 17, prev_hash) != 'q')
            get_hash_next_failed(str, 16, 17, 'p');
    }
    {
        char str[] = "abcdefghijklmnopqrstuvwxyz";
        int prev_hash = ('p' * 256 * 256 + 'q' * 256 + 'r') % Q;
        if (get_hash_next(str, 16, 19, prev_hash) != ('q' * 256 * 256 + 'r' * 256 + 's') % Q)
            get_hash_next_failed(str, 16, 19, prev_hash);
    }
    {
        char str[] = "abcdefghijklmnopqrstuvwxyz";
        int prev_hash = ('p' * 5012 % Q +
                         'q' * 2618 % Q +
                         'r' * 4774 % Q +
                         's' * 2184 % Q +
                         't' * 256  % Q +
                         'u'        % Q   ) % Q;
        if (get_hash_next(str, 16, 22, prev_hash) != ('q' * 5012 % Q +
                                                          'r' * 2618 % Q +
                                                          's' * 4774 % Q +
                                                          't' * 2184 % Q +
                                                          'u' * 256  % Q +
                                                          'v'        % Q   ) % Q)
            get_hash_next_failed(str, 16, 22, prev_hash);
    }
}

void test_my_strstr()
{
    {
        char str[] = "abcdefghijklmnopqrstuvwxyz";
        if (my_strstr(str, "a") != str)
            my_strstr_failed(str, "a");
    }
    {
        char str[] = "abbbbbbbbbbbbb";
        if (my_strstr(str, "aba") != NULL)
            my_strstr_failed(str, "aba");
    }
    {
        char str[] = "abbbbabb";
        if (my_strstr(str, "abb") != str)
            my_strstr_failed(str, "abb");
    }
    {
        char str[] = "bbababbbb";
        if (my_strstr(str, "ab") != str + 2)
            my_strstr_failed(str, "ab");
    }
    {
        char str[] = "bbabababbbb";
        if (my_strstr(str, "ababaa") != NULL)
            my_strstr_failed(str, "ababaa");
    }
    {
        char str[] = "bbba";
        if (my_strstr(str, "a") != str + 3)
            my_strstr_failed(str, "a");
    }
    {
        char str[] = "";
        if (my_strstr(str, "ab") != NULL)
            my_strstr_failed(str, "ab");
    }
    {
        char *str = NULL;
        if (my_strstr(str, "ab") != NULL)
            my_strstr_failed(str, "ab");
    }
    {
        char *str = "abab";
        if (my_strstr(str, NULL) != NULL)
            my_strstr_failed(str, "NULL");
    }
    {
        char *str = "AAbAbA";
        if (my_strstr(str, "a") != NULL)
            my_strstr_failed(str, "a");
    }
    {
        char *str = "AAbAbA";
        if (my_strstr(str, "A") != str)
            my_strstr_failed(str, "A");
    }
}
#endif
