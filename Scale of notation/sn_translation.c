//
// Created by moonlightnvkz on 23.11.16.
//

#include <stdio.h>
#include <assert.h>
#include "sn_translation.h"

/// \return pointer to reallocated array
/// \throw exit if allocation failes
static int *resize(int *arr, size_t new_size) {
    int *temp = realloc(arr, new_size);
    if (temp == NULL) {
        fprintf(stderr, "Failed to reallocate memory while "
                "increasing size of array:%s:%s:%d", __FILE__, __FUNCTION__, __LINE__);
        exit(1);
    }
    return temp;
}

/// \brief Adds <b> (not long number) to long number <a>
/// \return new size of a big number <a>
static size_t big_number_add(int *a, size_t size_a, int b, unsigned scale_notation_base) {
    a[0] += b;
    size_t i = 0;
    do {
        a[i + 1] += a[i] / scale_notation_base;
        a[i] %= scale_notation_base;
        ++i;
    } while(a[i] != 0);
    if (i >= size_a) return i;
    else return size_a;
}

/// \brief Multiplys a long number <a> by not long <b>
/// \param size_a
/// \return new size of a big number <a>
static size_t big_number_mult(int *a, size_t size_a, int b, unsigned scale_notation_base) {
    for (size_t i = 0; i < size_a; ++i) {
        a[i] *= b;
    }
    size_t i = 0;
    do {
        a[i + 1] += a[i] / scale_notation_base;
        a[i] %= scale_notation_base;
        ++i;
    } while(a[i] != 0);
    if (i >= size_a) return i;
    else return size_a;
}

/// \param number Initial number to translation
/// \param size number of digits of <number>
/// \param base_from base of the scale notation of the number
/// \param length will put here a final number of digits of output number
/// \return pointer to output number
int *sn_translate_to_dec(int *number, size_t size, unsigned base_from, size_t *length) {
    assert(base_from >= 2);
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
        dec_size = big_number_mult(dec, dec_size, base_from, 10);
        dec_size = big_number_add(dec, dec_size, number[i], 10);
        greatest_radix++;
    }
    *length = dec_size;
    return dec;
}

/// \brief Divides a long number <a> by not long <b>
/// \return remainder of division
static int big_number_div(int *a, size_t *size_a, int b, unsigned scale_notation_base)
{
    int carry = 0;
    for (size_t i = *size_a - 1;; --i) {
        int curr = a[i] + carry * scale_notation_base;
        a[i] = curr / b;
        carry = curr % b;
        if (i == 0) {
            break;
        }
    }
    while (*size_a > 0 && a[*size_a - 1] == 0) {
        --*(size_a);
    }
    return carry;
}

/// \param number Initial number to translation
/// \param size number of digits of <number>
/// \param base_to base of the scale notation of the output number
/// \param length will put here a final number of digits of output number
/// \return pointer to output number
int *sn_translate_from_dec(int *number, size_t size, unsigned base_to, size_t *length) {
    assert(base_to >= 2);
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
        dec[greatest_radix] = big_number_div(number, &size, base_to, 10);
        greatest_radix++;
    } while(size > 0);
    *length = greatest_radix;
    return dec;
}