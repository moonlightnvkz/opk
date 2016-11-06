#include "selectionsort.h"
#include <assert.h>

static void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void selection_sort(int arr[], size_t size)
{
    assert(arr != NULL);
    if (size == 0 || size == 1)
        return;
    for (size_t i = 0; i < size; ++i)
    {
        size_t min_index = i;
        for (size_t j = i + 1; j < size; ++j)
        {
            if (arr[j] < arr[min_index])
                min_index = j;
        }
        if (min_index != i)
            swap(arr + i, arr + min_index);
    }
}
