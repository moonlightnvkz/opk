#include "bubblesort.h"
#include <assert.h>

static void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void bubble_sort(int arr[], size_t size)
{
    assert(arr != NULL);
    if (size == 0 || size == 1)
        return;
    int swapped = 0;
    do
    {
        swapped = 0;
        for (size_t i = 0; i < size - 1; ++i)
        {
            if (arr[i] > arr[i + 1])
            {
                swap(arr + i, arr + i + 1);
                swapped = 1;
            }
        }
        --size;
    } while (swapped);
}
