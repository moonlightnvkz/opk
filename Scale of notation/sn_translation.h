//
// Created by moonlightnvkz on 23.11.16.
//

#pragma once

#include <stdlib.h>

int *sn_translate_to_dec(int *number, size_t size, unsigned base_from, size_t *length);

int *sn_translate_from_dec(int *number, size_t size, unsigned base_to, size_t *length);
