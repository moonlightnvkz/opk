#include "insertionsort.h"
#include <assert.h>

static void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void insertion_sort(int arr[], size_t size)
{
    assert(arr != NULL);
    if (size == 0 || size == 1)
        return;
    for (size_t i = 1; i < size; ++i)
        for (size_t j = i; j > 0 && arr[j - 1] > arr[j]; --j)
            swap(arr + j - 1, arr + j);
}
