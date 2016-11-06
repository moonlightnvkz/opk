#include <stdio.h>
#include "sorts.h"
#include "tests.h"
#include "compares.h"

int main(void)
{
    test_sort(merge_sort);
    test_sort(heap_sort);
    test_sort(quick_sort);
    test_sort(radix_sort);

    void (*sorts[])(int[], size_t) = {bubble_sort,
                                      coctail_sort,
                                      heap_sort,
                                      insertion_sort,
                                      merge_sort,
                                      quick_sort,
                                      radix_sort,
                                      selection_sort
                                      };
    compare_every_random(sorts, sizeof(sorts) / sizeof(sorts[0]));
    printf("\n");
    compare_single_random(sorts, sizeof(sorts) / sizeof(sorts[0]));

    return 0;
}
