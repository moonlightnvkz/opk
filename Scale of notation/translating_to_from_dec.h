//
// Created by moonlightnvkz on 23.11.16.
//

#pragma once

#include <stdlib.h>

int *translate_to_dec(int *number, size_t size, unsigned radix, size_t *length);

int *translate_from_dec(int *number, size_t size, unsigned radix, size_t *length);