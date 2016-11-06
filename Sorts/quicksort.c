#include "quicksort.h"
#include <assert.h>

static void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void quick_sort(int arr[], size_t size)
{
    assert(arr != NULL);
    if (size == 0 || size == 1)
        return;
    int *div = arr;
    size_t i = 1;
    size_t j = size-1;
    while (i <= j)
    {
        while (i < size && arr[i] < *div)
            ++i;
        while (j > 0 && arr[j] > *div)
            --j;
        if (i <= j)
        {
            swap(arr+i, arr+j);
            ++i;
            --j;
        }
    }
    swap(div, arr+j);
    quick_sort(arr, j);
    quick_sort(arr + j + 1, size - 1 - j);
}
