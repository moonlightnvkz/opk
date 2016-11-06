#include "mergesort.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

int *__merge_sort__(int arr[], size_t left, size_t right, int *buffer)
{
    assert(arr != NULL);
    if (right == left)
    {
        buffer[left] = arr[left];
        return buffer;
    }

    size_t middle = left + (right - left) / 2;
    int *left_half = __merge_sort__(arr, left, middle, buffer);
    int *right_half = __merge_sort__(arr, middle + 1, right, buffer);

    int *res = left_half == arr ? buffer : arr;

    size_t l = left, r = middle + 1;
    for (size_t i = left; i <= right; ++i)
    {
        if (l <= middle && r <= right)
        {
            if (left_half[l] <= right_half[r])
                res[i] = left_half[l++];
            else
                res[i] = right_half[r++];
        } else {
            if (l <= middle)
                res[i] = left_half[l++];
            else
                res[i] = right_half[r++];
        }
    }
    return res;
}

void merge_sort(int arr[], size_t size)
{
    assert(arr != NULL);
    if (size <= 1)
        return;
    int *buffer = malloc(sizeof(int) * size);
    if (!buffer)
    {
        fprintf(stderr, "Can't allocate memory for buffer array");
        exit(1);
    }
    int *new_arr = __merge_sort__(arr, 0, size - 1, buffer);
    if (new_arr != arr)
        memcpy(arr, new_arr, size * sizeof(int));
    free(buffer);
}
