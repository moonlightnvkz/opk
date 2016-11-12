#ifndef MYSTACK_H
#define MYSTACK_H

#include <stdlib.h>

typedef void *Pointer;

typedef struct tStack {
    Pointer *data;
    size_t size;
    size_t allocated;
    size_t increment;
    size_t initialSize;
} Stack;

/* Создать пустой стек */
void stack_create(Stack *pstack);

/* Уничтожить стек, освободив выделенную память */
void stack_destroy(Stack *pstack);

/* Поместить значение value на вершину стека */
void stack_push(Stack *pstack, Pointer value);

/* Возвращает количество элементов в стеке (0, если стек пуст) */
size_t stack_size(Stack *pstack);

/* Снять значение с вершины стека. Если стек пуст, возвращает 0 */
Pointer stack_pop(Stack *pstack);

/*
 * Возвращает значение с вершины стека, не удаляя его из стека.
 * Если стек пуст, возвращает 0
 */
Pointer stack_peek(Stack *pstack);

/*
 * Настраивает параметры стека.
 * initial_size: начальный размер стека, при первом выделении памяти
 *               (по умолч.: 50)
 * increment: на сколько элементов расширять стек при последующих
 *            выделениях памяти
 */
void stack_tune(Stack *pstack, size_t initial_size, size_t increment);

#endif // MYSTACK_H
