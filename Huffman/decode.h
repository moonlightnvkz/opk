#ifndef DECODE
#define DECODE

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "structs.h"
#include "encode.h"

void treeRestore(FILE* Finput, NodeStr* Nodes[256]);

void deleteTree(NodeStr* Node);

void decodeFile(FILE* Finput, FILE* Foutput, NodeStr* Node);

#endif // DECODE

