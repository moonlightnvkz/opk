#include <stdio.h>
#include "compares.h"
#include "randarray.h"
#include "sorts.h"

void measure_time(void (*sort)(int *, size_t), int arr[], size_t size, Results *results)
{
    results->array_size = size;
    results->start = clock();
    sort(arr, size);
    results->end = clock();
}

void print_headers(void (*sorts[])(int[], size_t), size_t num)
{
    printf("%7s", "Arr size");
    for (size_t i = 0; i < num; ++i)
    {
        if      (sorts[i] == quick_sort)     printf(" %7s", "Quick");
        else if (sorts[i] == radix_sort)     printf(" %7s", "Radix");
        else if (sorts[i] == heap_sort)      printf(" %7s", "Heap");
        else if (sorts[i] == merge_sort)     printf(" %7s", "Merge");
        else if (sorts[i] == insertion_sort) printf(" %7s", "Insert");
        else if (sorts[i] == bubble_sort)    printf(" %7s", "Bubble");
        else if (sorts[i] == coctail_sort)   printf(" %7s", "Coctail");
        else if (sorts[i] == selection_sort) printf(" %7s", "Select");
        else printf(" %12s", "Undef");
    }
    printf("\n");
}

void compare_every_random(void (*sorts[])(int[], size_t), size_t num)
{
    srand(time(NULL));
    print_headers(sorts, num);
    int c_size = INITIAL_SIZE;
    do
    {
        Results results = { 0, 0, 0 };
        int *arr = NULL;
        printf("%7d ", (int)c_size);
        for (size_t i = 0; i < num; ++i)
        {
            arr = get_array_random(c_size);
            measure_time(sorts[i], arr, c_size, &results);
            printf("%6.1lfms ", (double)(results.end - results.start) / (CLOCKS_PER_SEC / 1000));
            free(arr);
        }
        printf("\n");
        c_size *= MULTIPLIER;
    } while(c_size <= MAX_SIZE);
}

void compare_single_random(void (*sorts[])(int[], size_t), size_t num)
{
    srand(time(NULL));
    print_headers(sorts, num);
    int c_size = INITIAL_SIZE;
    do
    {
        Results results = { 0, 0, 0 };
        int *arr = get_array_random(c_size);
        printf("%7d ", (int)c_size);
        for (size_t i = 0; i < num; ++i)
        {
            int *copy = get_array_copy(arr, c_size);
            measure_time(sorts[i], arr, c_size, &results);
            printf("%6.1lfms ", (double)(results.end - results.start) / (CLOCKS_PER_SEC / 1000));
            free(copy);
        }
        printf("\n");
        c_size *= MULTIPLIER;
        free(arr);
    } while(c_size <= MAX_SIZE);
}
