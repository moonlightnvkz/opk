#include "radixsort.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <limits.h>

#define MAX_NUM 256 //max num in a byte

void radix_sort(int arr[], size_t size)
{
    assert(arr != NULL);
    int *temp_arr = malloc(sizeof(int) * size);
    for (unsigned int byte = 0; byte < sizeof(int); ++byte)
    {
        int counts[MAX_NUM] = {0};
        for (size_t i = 0; i < size; ++i)
            counts[(arr[i] >> (8 * byte)) & 0xFF]++;
        for (size_t i = 1; i < MAX_NUM; ++i)
            counts[i] += counts[i - 1];
        for (size_t i = size - 1;; --i)
        {
            temp_arr[counts[(arr[i] >> (8 * byte)) & 0xFF] - 1] = arr[i];
            counts[(arr[i] >> (8 * byte)) & 0xFF]--;
            if (i == 0)
                break;
        }
        memcpy(arr, temp_arr, size * sizeof(int));
    }

    // sort according to sign

    int counts[2] = {0};
    for (size_t i = 0; i < size; ++i)
        counts[arr[i] >= 0]++;
    for (size_t i = 1; i < 2; ++i)
        counts[i] += counts[i - 1];
    for (size_t i = size - 1;; --i)
    {
        temp_arr[counts[arr[i] >= 0] - 1] = arr[i];
        counts[arr[i] >= 0]--;
        if (i == 0)
            break;
    }
    memcpy(arr, temp_arr, size * sizeof(int));

    free(temp_arr);
}
