#include "tests.h"
#include <stdio.h>
#include <assert.h>
#include "sorts.h"

static void print_error(void (*sort)(int *, size_t), int arr[], size_t size)
{
    printf("Array { ");
    for (size_t i = 0; i < size; ++i)
        printf("%d, ", arr[i]);
    printf("} isn't sorted after the ");
         if (sort == quick_sort)     printf("quick_sort!\n");
    else if (sort == insertion_sort) printf("insertion_sort!\n");
    else if (sort == radix_sort)     printf("radix_sort!\n");
    else if (sort == heap_sort)      printf("heap_sort!\n");
    else if (sort == merge_sort)     printf("merge_sort!\n");
    else if (sort == bubble_sort)    printf("bubble_sort!\n");
    else if (sort == coctail_sort)   printf("coctail_sort!\n");
    else if (sort == selection_sort) printf("selection_sort!\n");
    else /*_______________________*/ printf("undefined sort!\n");
}

int is_sorted(void (*sort)(int *, size_t), int arr[], size_t size)
{
    if (size <= 1)
        return 1;
    sort(arr, size);
    for (size_t i = 0; i < size - 1; ++i)
        if (arr[i] > arr[i + 1])
            return 0;
    return 1;
}

void test_sort(void (*sort)(int *, size_t))
{
    {
        int arr[0] = {};
        if (!is_sorted(sort, arr, 0)) print_error(sort, arr, 0);
    }
    {
        int arr[1] = {1};
        if (!is_sorted(sort, arr, 1)) print_error(sort, arr, 1);
    }
    {
        int arr[5] = {1, -1, -2, -3, -4};
        if (!is_sorted(sort, arr, 5)) print_error(sort, arr, 5);
    }
    {
        int arr[5] = {4, 3, 2, 1, 0};
        if (!is_sorted(sort, arr, 5)) print_error(sort, arr, 5);
    }
    {
        int arr[5] = {0};
        if (!is_sorted(sort, arr, 5)) print_error(sort, arr, 5);
    }
    {
        int arr[5] = {0, 2, 1, 2, 3};
        if (!is_sorted(sort, arr, 5)) print_error(sort, arr, 5);
    }
    {
        int arr[5] = {3, 1, 7, 2, 0};
        if (!is_sorted(sort, arr, 5)) print_error(sort, arr, 5);
    }
}
