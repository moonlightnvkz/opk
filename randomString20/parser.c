#include "parser.h"
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include "wordscontainer.h"



void parseFile(FILE *infile, WordsContainer *wc)
{
    while (!feof(infile))
    {
        wchar_t word[50] = {0};
        fwscanf(infile, L"%ls", word);
        if (iswpunct(word[wcslen(word) - 1]) && word[wcslen(word) - 1] != ',')
            word[wcslen(word) - 1] = L'\0';
        if ((wcslen(word) == 2 && iswpunct(word[0])) || iswspace(word[0]) || word[0] == L'«' || word[0] == L'»')
            continue;
        wc_add(wc, word);
    }
}
