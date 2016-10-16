#include "mystack.h"
#include <stdio.h>

void stack_create(Stack *pstack)
{
    pstack->size = 0;
    stack_tune(pstack, 50, 1);
    pstack->data = malloc(sizeof(tStackElement) * pstack->initialSize);
    if (!pstack->data)
    {
        fprintf(stderr, "Can't allocate memory for %d elements stack_create).", (int)pstack->initialSize);
        exit(1);
    }
    pstack->allocated = pstack->initialSize;
}

void stack_destroy(Stack *pstack)
{
    free(pstack->data);
    pstack->data = NULL;
    pstack->allocated = 0;
    pstack->increment = 0;
    pstack->size = 0;
}

void stack_push(Stack *pstack, Pointer value)
{
    if (pstack->data == NULL)
    {
        fprintf(stderr, "Can't push element: pstack->data == NULL. Stack is not created or destroyed"
                        " (stack_push).");
        exit(1);
    }
    if (pstack->size >= pstack->allocated)
    {
        tStackElement *temp = realloc(pstack->data, sizeof(tStackElement) * (pstack->size + pstack->increment));
        if (!temp)
        {
            fprintf(stderr, "Can't reallocate memory for %d element (stack_push).", (int)pstack->size + 1);
            stack_destroy(pstack);
            exit(1);
        }
        pstack->data = temp;
        pstack->allocated += pstack->increment;
    }
    pstack->data[pstack->size] = *(tStackElement*)value;
    pstack->size++;
}

size_t stack_size(Stack *pstack)
{
    return pstack->size;
}

Pointer stack_pop(Stack *pstack)
{
    if (pstack->size == 0)
        return NULL;
    if (pstack->data == NULL)
    {
        fprintf(stderr, "Can't pop element: pstack->data == NULL, but size is not a 0. (pstack_pop)");
        return NULL;
    }
    Pointer poped = &pstack->data[pstack->size - 1];
    pstack->size--;
    return poped;
}

Pointer stack_peek(Stack *pstack)
{
    if (pstack->size == 0)
        return NULL;
    if (pstack->data == NULL)
    {
        fprintf(stderr, "Can't peek element: pstack->data == NULL, but size is not a 0. (pstack_peek)");
        return NULL;
    }
    return &pstack->data[pstack->size - 1];
}

void stack_tune(Stack *pstack, size_t initial_size, size_t increment)
{
    static int isTuned;
    if (isTuned == 1)
        return;
    pstack->initialSize = initial_size;
    pstack->increment = increment;
    isTuned = 1;
}
