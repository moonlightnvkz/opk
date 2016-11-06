#include "coctail_sort.h"
#include <assert.h>

static void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void coctail_sort(int arr[], size_t size)
{
    assert(arr != NULL);
    if (size == 0 || size == 1)
        return;
    for (size_t left = 0, right = size - 1; left < right;)
    {
        for (size_t i = left; i < right; ++i)
            if (arr[i] > arr[i + 1])
                swap(arr + i, arr + i + 1);
        --right;

        for (size_t i = right; i > left; --i)
            if (arr[i - 1] >  arr[i])
                swap(arr + i - 1, arr + i);
        ++left;
    }
}
