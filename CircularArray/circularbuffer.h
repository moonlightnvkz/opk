#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H
#include <stdlib.h>

typedef char tBufferElement;

typedef void *Pointer;

typedef struct tCircularBuffer
{
    tBufferElement *buffer;
    tBufferElement *buffer_end;
    tBufferElement *data_start;
    tBufferElement *data_end;
    size_t allocated;
    size_t size;
    size_t initial_size;
    size_t increment;
    int isTuned;

}CircularBuffer;

void cb_create(CircularBuffer *pcb);

void cb_destroy(CircularBuffer *pcb);

void cb_push(CircularBuffer *pcb, Pointer value);

size_t cb_size(CircularBuffer *pcb);

Pointer cb_pop(CircularBuffer *pcb);

int cb_full(CircularBuffer *pcb);

void cb_tune(CircularBuffer *pcb, size_t initial_size, size_t increment);

#endif // CIRCULARBUFFER_H
