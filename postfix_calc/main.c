#include <stdio.h>
#include <malloc.h>
#include "reader.h"
#include "postfix_calc.h"

int main()
{
    char *expression = read_line("Enter expression:\n>");
    double res = postfix_calc(expression);
    free(expression);
    fprintf(stdout, "\n%lf", res);
    return 0;
}
