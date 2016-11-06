#include "randarray.h"
#include <stdio.h>
#include <assert.h>

static void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int *get_array_random(size_t size)
{
    int * arr = malloc(sizeof(int) * size);
    if (!arr)
    {
        fprintf(stderr, "Can't allocate memory");
        exit(1);
    }
    for (size_t i = 0; i < size; ++i)
        arr[i] = i;
    for (size_t i = 0; i < size; ++i)
        swap(arr + i, arr + rand() % size);
    return arr;
}

int *get_array_copy(int *arr, size_t size)
{
    assert(arr != NULL);
    int *new_arr = malloc(sizeof(int) * size);
    if (!new_arr)
    {
        fprintf(stderr, "Can't allocate memory");
        exit(1);
    }
    for (size_t i = 0; i < size; ++i)
        new_arr[i] = arr[i];
    return new_arr;
}
