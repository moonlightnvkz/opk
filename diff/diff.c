#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "diff.h"
#include "reader.h"
#include <wchar.h>

void diff(FILE *f1, FILE *f2, Results *res)
{
    for(int count = 1; ; ++count)
    {
        wchar_t * line1 = read_line(f1);

        wchar_t * line2 = read_line(f2);
        assert(line1 == NULL || line2 == NULL);
        if (wcscmp(line1, line2) != 0)
        {
            res->line_number = count;
            res->line1 = line1;
            res->line2 = line2;
            res->res = 1;
            return;
        }
        free(line1);
        free(line2);
    }
    res->res = 0;
}
