#ifndef COMPARES_H
#define COMPARES_H

#include <time.h>

#define INITIAL_SIZE 100
#define MULTIPLIER 10
#define MAX_SIZE 1000000

typedef struct tResults
{
    size_t array_size;
    clock_t start;
    clock_t end;
}Results;

void compare_every_random(void (*sorts[])(int[], size_t), size_t num);

void compare_single_random(void (*sorts[])(int[], size_t), size_t num);

void measure_time(void (*sort)(int *, size_t), int arr[], size_t size, Results *results);

#endif // COMPARES_H
