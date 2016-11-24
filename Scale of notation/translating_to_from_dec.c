//
// Created by moonlightnvkz on 23.11.16.
//

#include <stdio.h>
//#include <string.h>
#include <assert.h>
#include "translating_to_from_dec.h"

static int *resize(int *arr, size_t new_size) {
    int *temp = realloc(arr, new_size);
    if (temp == NULL) {
        fprintf(stderr, "Failed to reallocate memory while "
                "increasing size of array:%s:%s:%d", __FILE__, __FUNCTION__, __LINE__);
        exit(1);
    }
    return temp;
}

static size_t big_number_add(int *a, size_t size_a, int b, unsigned radix) {
    a[0] += b;
    size_t i = 0;
    do {
        a[i + 1] += a[i] / radix;
        a[i] %= radix;
        ++i;
    } while(a[i] != 0);
    if (i >= size_a) return i;
    else return size_a;
}

static size_t big_number_mult(int *a, size_t size_a, int b, unsigned radix) {
    for (size_t i = 0; i < size_a; ++i) {
        a[i] *= b;
    }
    size_t i = 0;
    do {
        a[i + 1] += a[i] / radix;
        a[i] %= radix;
        ++i;
    } while(a[i] != 0);
    if (i >= size_a) return i;
    else return size_a;
}

int *translate_to_dec(int *number, size_t size, unsigned radix, size_t *length) {
    assert(radix >= 2);
    size_t alloc = size;
    size_t greatest_radix = 0;
    int *dec = calloc(sizeof(int), size);
    if (dec == NULL) {
        fprintf(stderr, "Failed to allocate memory for"
                "decimal interpretation:%s:%s:%d", __FILE__, __FUNCTION__, __LINE__);
        exit(1);
    }
    size_t dec_size = 0;
    for (size_t i = 0; i < size; ++i) {
        if (greatest_radix > alloc / 2) {
            alloc *= 2;
            dec = resize(dec, dec_size * sizeof(int));
        }
        dec_size = big_number_mult(dec, dec_size, radix, 10);
        dec_size = big_number_add(dec, dec_size, number[i], 10);
        greatest_radix++;
    }
    *length = dec_size;
    return dec;
}

static int big_number_div(int *a, size_t *size, int b, unsigned radix)
{
    int carry = 0;
    for (size_t i = *size - 1;; --i) {
        int curr = a[i] + carry * radix;
        a[i] = curr / b;
        carry = curr % b;
        if (i == 0) {
            break;
        }
    }
    while (*size > 0 && a[*size - 1] == 0) {
        --*(size);
    }
    return carry;
}

int *translate_from_dec(int *number, size_t size, unsigned radix, size_t *length) {
    assert(radix >= 2);
    size_t alloc = size;
    int *dec = calloc(sizeof(int), size);
    if (dec == NULL) {
        fprintf(stderr, "Failed to allocate memory for"
                "decimal interpretation:%s:%s:%d", __FILE__, __FUNCTION__, __LINE__);
        exit(1);
    }
    size_t greatest_radix = 0;
    do {
        if (greatest_radix > alloc / 2) {
            alloc *= 2;
            dec = resize(dec, alloc * sizeof(int));
        }
        dec[greatest_radix] = big_number_div(number, &size, radix, 10);
        greatest_radix++;
    } while(size > 0);
    *length = greatest_radix;
    return dec;
}