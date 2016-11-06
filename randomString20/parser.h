#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>

typedef struct WordsContainer WordsContainer;

void parseFile(FILE* infile, WordsContainer *wc);

#endif // PARSER_H
