#include "postfix_calc.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "mystack.h"
extern size_t strlen(const char* str);

#define SEPARATOR '|'

static void print_error(int line)
{
    printf("Error:%s:%d", __FILE__, line);
}

static int get_priority(char o)
{
    switch (o)
    {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    }
    return 0;
}

char* resize_postfix(char* arr, size_t new_size)
{
    char* temp = realloc(arr, sizeof(char) * new_size);
    if (!temp)
    {
        fprintf(stderr, "Can't allocate memory");
        exit(1);
    }
    temp[new_size - 1] = '\0';
    return temp;
}

char* find_first_operator(char* postfix, size_t postfix_end)
{
    for (char* p = postfix; p >= postfix + postfix_end; ++p)
        if (*p == '-' || *p == '+' || *p == '*' || *p == '/')
            return p;
    return NULL;
}

bool is_operator(char o) {
    switch (o) {
    case '+':
    case '-':
    case '*':
    case '/':
        return true;
    default:
        return false;
    }
}

double read_number(char* str, size_t* idx)
{
    double res = 0;
    int int_mult = 10;
    double fract_mult = 1.0;
    while ((str[*idx] >= '0' && str[*idx] <= '9') || str[*idx] == '.') {
        if (str[*idx] == '.') {
            int_mult = 1;
            fract_mult = 0.1;
            ++(*idx);
            continue;
        }
        res = res * int_mult + (str[*idx] - '0') * fract_mult;
        if (fract_mult < 1.0)
            fract_mult /= 10;
        ++(*idx);
    }
    if (is_operator(str[*idx])) {
        --(*idx);
    }
    return res;
}

bool do_operation(char op, Stack *stack, double *res) {
    if (stack_size(stack) < 2) {
        return false;
    }
    double *float1 = (double*)stack_pop(stack);
    double *float2 = (double*)stack_pop(stack);
    switch (op) {
    case '+':
        *res = *float2 + *float1;
        break;
    case '-':
        *res = *float2 - *float1;
        break;
    case '*':
        *res = *float2 * *float1;
        break;
    case '/':
        *res = *float2 / *float1;
        break;
    default:
        return false;
    }
    free(float1);
    free(float2);
    return true;
}

double postfix_calc(char *e)
{
    Stack stack;
    stack_tune(&stack, 25, 25);
    stack_create(&stack);
    size_t postfix_size = 2*strlen(e) + 1;
    char *postfix = malloc(sizeof(char) * (postfix_size));
    postfix[postfix_size - 1] = '\0';
    size_t postfix_end = 0;
    for (size_t i = 0; i < strlen(e); ++i)
    {
        switch (e[i])
        {
        case '+':
        case '-':
        case '*':
        case '/':
            while (stack_peek(&stack) != NULL &&
                   get_priority(e[i]) <= get_priority(*(char*)stack_peek(&stack))) {
                if (postfix_end == postfix_size - 1) {
                    postfix = resize_postfix(postfix, postfix_size * 2);
                }
                postfix[postfix_end++] = *(char*)stack_pop(&stack);
            }
        case '(':
            stack_push(&stack, e + i);
            if (postfix_end == postfix_size - 1) {
                postfix = resize_postfix(postfix, postfix_size * 2);
            }
            postfix[postfix_end++] = SEPARATOR; // Record separator
            break;
        case ')':
            while (*(char*)stack_peek(&stack) != '(' && stack_size(&stack) > 0) {
                if (postfix_end == postfix_size - 1) {
                    postfix = resize_postfix(postfix, postfix_size * 2);
                }
                postfix[postfix_end++] = *(char*)stack_pop(&stack);
            }
            if (stack_size(&stack) == 0)
            {
                print_error(__LINE__);
                exit(1);
            }
            stack_pop(&stack);
            break;
        default:
            if (postfix_end == postfix_size - 1) {
                postfix = resize_postfix(postfix, postfix_size * 2);
            }
            postfix[postfix_end++] = e[i];
        }
    }
    while (stack_size(&stack) > 0) {
        char c = *(char*)stack_pop(&stack);
        if (c == '(' || c == ')') {
            print_error(__LINE__);
            exit(1);
        }
        if (postfix_end == postfix_size - 1) {
            postfix = resize_postfix(postfix, postfix_size * 2);
        }
        postfix[postfix_end++] = c;
    }
    postfix[postfix_end] = '\0';
    fprintf(stdout, "%s", postfix);

    double res;
    for (size_t i = 0; i <postfix_end; ++i)
    {
        if (is_operator(postfix[i])) {
            if (!do_operation(postfix[i], &stack, &res)) {
                print_error(__LINE__);
                exit(1);
            }
            continue;
        }
        double *num = malloc(sizeof(double));
        if (!num) {
            print_error(__LINE__);
            exit(1);
        }
        *num = read_number(postfix, &i);
        stack_push(&stack, num);
    }
    free(postfix);
    stack_destroy(&stack);
    return res;
}
