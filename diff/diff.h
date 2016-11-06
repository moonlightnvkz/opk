#ifndef DIFF_H
#define DIFF_H
#include <stdbool.h>
#include <stdio.h>
#include <wchar.h>
typedef struct tResulsts
{
    int line_number;
    wchar_t* line1;
    wchar_t* line2;
    bool res;
}Results;

void diff(FILE *f1, FILE *f2, Results *res);

#endif // DIFF_H
