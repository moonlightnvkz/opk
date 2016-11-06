#include <stdio.h>
#ifdef _DEBUG
#include "test_my_strstr.h"
#endif

int main()
{
#ifdef _DEBUG
    test_remainder();
    test_get_hash();
    test_get_hash_next();
    test_my_strstr();
#endif
    return 0;
}
