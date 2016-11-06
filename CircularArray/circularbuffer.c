#include "circularbuffer.h"
#include <malloc.h>
#include <stdio.h>
#include <assert.h>

void cb_resize(CircularBuffer *pcb, size_t new_size);

void cb_create(CircularBuffer *pcb)
{
    assert(pcb != NULL);
    cb_tune(pcb, 50, 1);
    pcb->buffer = (tBufferElement *)malloc(sizeof(tBufferElement) * pcb->initial_size);
    if (pcb->buffer == NULL)
    {
        fprintf(stderr, "Can't allocate memory for circular buffer (cb_create)");
        exit(1);
    }
    pcb->allocated = pcb->initial_size;
    pcb->buffer_end = pcb->buffer + pcb->allocated;
    pcb->data_start = pcb->data_end = pcb->buffer;
    pcb->size = 0;
}

void cb_destroy(CircularBuffer *pcb)
{
    assert(pcb != NULL);
    free(pcb->buffer);
    pcb->allocated = 0;
    pcb->buffer_end = pcb->data_start = pcb->data_end = 0;
    pcb->increment = pcb->initial_size = pcb->size = pcb->isTuned = 0;
}

void cb_push(CircularBuffer *pcb, Pointer value)
{
    assert(pcb != NULL && pcb->buffer != NULL);
    if (cb_full(pcb) && pcb->increment != 0)
        cb_resize(pcb, pcb->size + pcb->increment);
    *pcb->data_end = *(tBufferElement *)value;
    pcb->data_end++;
    if (pcb->data_end == pcb->buffer_end)
        pcb->data_end = pcb->buffer;
    pcb->size++;
}

size_t cb_size(CircularBuffer *pcb)
{
    assert(pcb != NULL);
    return pcb->size;
}

Pointer cb_pop(CircularBuffer *pcb)
{
    assert(pcb != NULL && pcb->buffer != NULL);
    if (pcb->size == 0)
        return 0;
    if (pcb->increment != 0 && pcb->allocated - pcb->size >= pcb->increment)
        cb_resize(pcb, pcb->allocated - pcb->increment);
    tBufferElement *poped = pcb->data_start;
    pcb->data_start++;
    if (pcb->data_start == pcb->buffer_end)
        pcb->data_start = pcb->buffer;
    pcb->size--;
    return poped;
}

int cb_full(CircularBuffer *pcb)
{
    assert(pcb != NULL);
    return pcb->allocated == pcb->size;
}

void cb_resize(CircularBuffer *pcb, size_t new_size)
{
    tBufferElement *temp = malloc(sizeof(tBufferElement) * new_size);
    if (!temp)
    {
        fprintf(stderr, "Can't allocate memory for new circular buffer (cb_resize)");
        exit(1);
    }
    for (struct { size_t i; tBufferElement *p; } loopy = { 0, pcb->data_start };
         loopy.i < pcb->size;
         ++loopy.p, ++loopy.i)
    {
        if (loopy.p == pcb->buffer_end)
            loopy.p = pcb->buffer;
        temp[loopy.i] = *loopy.p;

    }
    free(pcb->buffer);
    pcb->buffer = temp;
    pcb->buffer_end = pcb->buffer + new_size;
    pcb->data_start = pcb->buffer;
    pcb->data_end = pcb->buffer + pcb->size;
    if (pcb->data_end == pcb->buffer_end)
        pcb->data_end = pcb->buffer;
    pcb->allocated = new_size;
    pcb->buffer_end = pcb->buffer + pcb->allocated;
}

void cb_tune(CircularBuffer *pcb, size_t initial_size, size_t increment)
{
    assert(pcb != NULL);
    if (pcb->isTuned == 1)
        return;
    pcb->initial_size = initial_size;
    pcb->increment = increment;
    pcb->isTuned = 1;
}
