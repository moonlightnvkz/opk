#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wctype.h>
#include <wchar.h>
#include <locale.h>
#include <malloc.h>
#include "mygetwch.h"
#include "wordscontainer.h"
#include "parser.h"

wchar_t *addWord(wchar_t *dst, const wchar_t *src, int isFirstWordInASentence)
{
    int dstlen = -1;
    if (isFirstWordInASentence == 1)
        dstlen = wcslen(dst);
    wchar_t *temp = realloc(dst, sizeof(wchar_t) * (wcslen(dst) + wcslen(src) + 2));
    if (!temp)
    {
        free(dst);
        return NULL;
    }
    wcscat(temp, src);
    if (dstlen >= 0)
        temp[dstlen] = (wchar_t)towupper(temp[dstlen]);
    return temp;
}

int main(void)
{
    setlocale(LC_CTYPE, "");
    FILE* infile = fopen("book1.txt", "r");
    if (infile == NULL)
    {
        fwprintf(stderr, L"Can't opoen the file");
        exit(2);
    }
    WordsContainer wc;
    wc_init(&wc);
    parseFile(infile, &wc);

    srand(time(NULL));
    unsigned int sentencesAmount = rand() % 1 + 3;

    unsigned int wordsTotal = 0;
    wchar_t *textForPrint = malloc(sizeof(wchar_t));
    textForPrint[0] = L'\0';
    for (unsigned int i = 0; i < sentencesAmount; ++i)
    {
        unsigned int wordsAmount = rand() % 5 + 3;
        wordsTotal += wordsAmount;
        wchar_t * word = wc.words[rand() % wc.count];

        textForPrint = addWord(textForPrint, word, 1);
        if(!textForPrint)
        {
            fwprintf(stderr, L"Can't allocate memory for textForPrint (main)");
            wc_destroy(&wc);
            exit(1);
        }
        for (unsigned int j  = 1; j < wordsAmount; ++j)
        {
            textForPrint = addWord(textForPrint, L" ", 0);
            if(!textForPrint)
            {
                fwprintf(stderr, L"Can't allocate memory for textForPrint (main)");
                wc_destroy(&wc);
                exit(1);
            }
            word = wc.words[rand() % wc.count];
            textForPrint = addWord(textForPrint, word, 0);
            if(!textForPrint)
            {
                fwprintf(stderr, L"Can't allocate memory for textForPrint (main)");
                wc_destroy(&wc);
                exit(1);
            }
        }
        wchar_t *lastChar = textForPrint + wcslen(textForPrint) - 1;

        if (*lastChar == L',')
        {
            *lastChar = L'.';
        } else if (!iswpunct(*lastChar))
        {
            textForPrint = addWord(textForPrint, L".", 0);
            if(!textForPrint)
            {
                fwprintf(stderr, L"Can't allocate memory for textForPrint (main)");
                wc_destroy(&wc);
                exit(1);
            }
        }
        textForPrint = addWord(textForPrint, L"", 0);
        if(!textForPrint)
        {
            fwprintf(stderr, L"Can't allocate memory for textForPrint (main)");
            wc_destroy(&wc);
            exit(1);
        }

    }

    wprintf(L">");
    for (unsigned int i = 0; i < wcslen(textForPrint); ++i)
    {
        wprintf(L"%lc", textForPrint[i]);
        if (textForPrint[i] == L'\n')
            wprintf(L" ");
    }
    wprintf(L"\n>");

    unsigned int mistakes = 0;
    time_t startTime = time(NULL);
    for (unsigned int i = 0; i < wcslen(textForPrint);)
    {
        wchar_t input = getwch();
        if (input == textForPrint[i])
        {

            if (input == L'\n')
                wprintf(L"\r %lc>", input);
            else wprintf(L"%lc", input);
            ++i;
        } else {
            ++mistakes;
        }
    }
    wprintf(L"\r \n");
    double elapsedTime = difftime(time(NULL), startTime);
    wprintf(L"Great!\nMistakes were made: %d\nElapsed time: %.lf seconds\nInput speed: %.1f words per second\n",
            mistakes, elapsedTime, wordsTotal / elapsedTime);
    free(textForPrint);
    wc_destroy(&wc);
    fclose(infile);
    return 0;
}
