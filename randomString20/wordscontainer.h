#ifndef WORDSCONTAINER_H
#define WORDSCONTAINER_H
#include <stddef.h>

typedef struct WordsContainer{
    size_t count;
    size_t _size;
    wchar_t** words;
} WordsContainer;

void wc_init(WordsContainer *wc);
wchar_t* wc_add(WordsContainer *wc, wchar_t* word);
void wc_resize(WordsContainer *wc, size_t newSize);
void wc_destroy(WordsContainer *wc);
#endif // WORDSCONTAINER_H
