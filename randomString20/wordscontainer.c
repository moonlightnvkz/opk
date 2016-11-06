#include "wordscontainer.h"
#include <wchar.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

void wc_init(WordsContainer *wc)
{
    wc->count = 0;
    wc->_size = 100;
    if ((wc->words = malloc(sizeof(wchar_t*) * wc->_size)) == NULL)
    {
        fwprintf(stderr, L"Can't allocate memory (init)");
        wc_destroy(wc);
        exit(1);
    }
    wc->words[0] = L'\0';
}

wchar_t *wc_add(WordsContainer *wc, wchar_t *word)
{
    if (wc->count == wc->_size)
        wc_resize(wc, wc->_size * 2);
//    for (unsigned int i = 0; i < wc->count; ++i)  //delete duplicates
//    {
//        if (strcmp(word, wc->words[i]) == 0)
//        {
//            return wc->words[i];
//        }
//    }
    if ((wc->words[wc->count] = malloc(sizeof(wchar_t) * (wcslen(word) + 1))) == NULL)
    {
        fwprintf(stderr, L"Can't allocate memory for word (wc_add). Current size is %d", (unsigned int) wc->_size);
        wc_destroy(wc);
        exit(1);
    }
    wmemcpy(wc->words[wc->count], word, wcslen(word) + 1);
    wc->count++;
    return wc->words[wc->count];
}

void wc_resize(WordsContainer *wc, size_t newSize)
{
    wchar_t** newWords;
    if ((newWords = malloc(sizeof(wchar_t*) * newSize)) == NULL)
    {
        fwprintf(stderr, L"Can't allocate memory (wc_resize). Current size is %d, new size is %d",
                 (unsigned int) wc->_size, (unsigned int)newSize);
        wc_destroy(wc);
        exit(1);
    }
    for (unsigned int i = 0; i < wc->count; ++i)
    {
        wchar_t * word;
        if ((word = malloc(sizeof(wchar_t) * (wcslen(wc->words[i]) + 1))) == NULL)
        {
            fwprintf(stderr, L"Can't allocate memory for word (wc_resize). Current size is %d", (unsigned int) wc->_size);
            wc_destroy(wc);
            exit(1);
        }
        wmemcpy(word, wc->words[i], wcslen(wc->words[i]) + 1);
        newWords[i] = word;
    }
    wc_destroy(wc);
    wc->words = newWords;
    wc->_size = newSize;
}

void wc_destroy(WordsContainer *wc)
{
    for (unsigned int i = 0; i < wc->count; ++i)
    {
        free(wc->words[i]);
    }
    free(wc->words);
}
