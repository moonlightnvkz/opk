#include "heapsort.h"
#include <assert.h>
#include <stdio.h>

static void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void sink(int arr[], size_t size, size_t i)
{
    assert(arr != NULL);
    while (2 * (i + 1) - 1 < size)
    {
        int max_child_index;
        if (2 * (i + 1) - 1 == size - 1 || arr[2 * (i + 1) - 1] > arr[2 * (i + 1)])
            max_child_index = 2 * (i + 1) - 1;
        else
            max_child_index = 2 * (i + 1);
        if (arr[i] < arr[max_child_index])
        {
            swap(arr + i, arr + max_child_index);
            i = max_child_index;
        } else {
            break;
        }
    }
}

void heap_sort(int arr[], size_t size)
{
    assert(arr != NULL);
    if (size == 0 || size == 1)
        return;

    for (int i = size / 2 - 1;; --i)
    {
        sink(arr, size, i);
        if (i == 0)
            break;
    }

    for (int i = size - 1; i > 0; --i)
    {
        swap(arr, arr + i);
        sink(arr, i, 0);
    }
}
