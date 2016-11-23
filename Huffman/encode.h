#ifndef ENCODE
#define ENCODE

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "structs.h"
#include "bubblesort.h"

int charPosInCharAr(NodeStr* Nodes[256], unsigned char ch);

void getFrequency(FILE* Finput, NodeStr* Nodes[256], int* SymbAmount);

void treeCreate(NodeStr* Nodes[256], int SymbAmount);

void TreePosition(char CurrCode[CodesSize], int NodeType);

void addCodeForChar(char Info, CharCodes Codes[256], char CurrCode[CodesSize]);

void formCodesAndWriteTree(FILE* Foutput, NodeStr* Node, CharCodes Codes[256], char CurrCode[CodesSize], Bytes* Byte);

void writeBit(FILE* Foutput, unsigned char ch, Bytes* Byte);

void writeByte(FILE* Foutput, unsigned char ch, Bytes* Byte);

int searchCode(unsigned char ch, CharCodes Codes[256]);

void encodeFile(FILE* Finput, FILE* Foutput, CharCodes Codes[256], Bytes* Byte);

#endif // ENCODE

