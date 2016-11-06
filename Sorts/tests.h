#ifndef TESTS_H
#define TESTS_H
#include <stdlib.h>

int is_sorted(void (*sort)(int *, size_t), int arr[], size_t size);

void test_sort(void (*sort)(int *, size_t));

#endif // TESTS_H
