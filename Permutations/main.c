#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int fact(int N)
{
    int res = 1;
    for (int i = 1; i < N; ++i, res *= i);
    return res;
}

int *permutations(int N)
{
    int size = fact(N) * N;
    int *perm_arr = malloc(sizeof(int) * size);

    int *pair_w_max_index = NULL;
    int *majoranta_w_max_index = NULL;
    for (int i = 0; i < N; ++i)
        perm_arr[i] = i;
    for (int *i = perm_arr + N; i < perm_arr + size; i += N)
    {
        for (int *j = i - N; j < i; ++j)
        {
            if (*j < *(j + 1) && j + 1 < i)
            {
                pair_w_max_index = j;
                majoranta_w_max_index = j+1;
                continue;
            }
            if (pair_w_max_index != NULL && *j > *pair_w_max_index)
                majoranta_w_max_index = j;
        }
        assert(pair_w_max_index != NULL);
        assert(majoranta_w_max_index != NULL);
        memcpy(i, i - N, (pair_w_max_index - i + N) * sizeof(int));
        *(pair_w_max_index + N) = *majoranta_w_max_index;
        for (int *j = i - 1; j > pair_w_max_index; --j)
        {
            if (j == majoranta_w_max_index)
                *(pair_w_max_index + (i - j) + N) = *pair_w_max_index;
            else
                *(pair_w_max_index + (i - j) + N) = *j;
        }
        pair_w_max_index = NULL;
        majoranta_w_max_index = NULL;
    }
    return perm_arr;
}

int main()
{
        assert(fact(0) != 0);
        assert(fact(0) == 1);
        assert(fact(1) == 1);
        assert(fact(2) == 2);
        assert(fact(3) == 6);
        assert(fact(4) == 24);
        assert(fact(5) == 120);

    // permutations of N
    const int N = 6;
    int *perm_arr = permutations(N);
    printf("{ ");
    for (int i = 0; i < fact(N) * N; ++i)
    {
        if (i % N == 0 && i != 0 && i != fact(N) * N) printf("\n  ");
        printf("%d, ", perm_arr[i]);
    }
    printf("\b\b }\n");
    free(perm_arr);
    return 0;
}
