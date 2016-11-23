#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "structs.h"
#include "encode.h"
#include "decode.h"

#define ModeLenght 10
#define HELP "Enter must be like: \"input_path mode output_path\",\nwhere mode is \'-p\' for pack or \'-u\' for unpack.\n\n"
#define Pack 'p'
#define Unpack 'u'

void init(char InputPath[PATH_MAX], char OutputPath[PATH_MAX], char Mode[ModeLenght])
{
    printf(HELP);
    printf(">");
    scanf("%259s%10s%259s", InputPath, Mode, OutputPath);
}

int main(void)
{
    NodeStr* Nodes[256] = {[0 ... 255] = NULL};
    CharCodes Codes[256] = {[0 ... 255] = {27, {[0 ... CodesSize-1] = -1}} };
    int SymbAmount = 0;
    char CurrCode[CodesSize] = {[0 ... CodesSize-1] = -1};
    Bytes Byte = {0, 8, 0};
    FILE* Finput;
    FILE* Foutput;
    char InputPath[PATH_MAX];
    char OutputPath[PATH_MAX];
    char Mode[ModeLenght];

    init(InputPath, OutputPath, Mode);

    if(Mode[1] == Pack)
    {
        printf("Opening the file...\r\n");
        if((Finput = fopen(InputPath, "r")) == NULL){
            perror("Can't open the input file");
            exit(1);}
        if((Foutput = fopen(OutputPath, "wb")) == NULL){
            perror("Can't open the output file");
            exit(1);}

        printf("Counting frequency...\r\n");
        getFrequency(Finput, Nodes, &SymbAmount);
        BubbleSort(Nodes, SymbAmount);

        printf("Creating tree...\r\n");
        treeCreate(Nodes, SymbAmount);

        printf("Code assigment...\r\n");
        formCodesAndWriteTree(Foutput, Nodes[0], Codes, CurrCode, &Byte);
            if(Byte.CBit != 8){ // Дозапись дерева, если в Byte остались значащие биты
                fwrite(&Byte.Byte, 1, 1, Foutput);
                Byte.Byte = 0;
                Byte.CBit = 8;}

        printf("Encoding the file...\r\n");
        encodeFile(Finput, Foutput, Codes, &Byte);

        printf("Encoded");

    }else if(Mode[1] == Unpack)
    {
        printf("Opening the file...\r\n");
        if((Finput = fopen(InputPath, "rb")) == NULL){
            perror("Can't open the input file");
            exit(1);}
        if((Foutput = fopen(OutputPath, "w")) == NULL){
            perror("Can't open the output file");
            exit(1);}

        printf("Restoring tree...\r\n");
        treeRestore(Finput, Nodes);

        printf("Decoding the file...\r\n");
        decodeFile(Finput, Foutput, Nodes[0]);

        printf("Deleting the tree...\r\n");
        deleteTree(Nodes[0]);

        printf("Decoded");
    }else{
        printf("Wrong mode");
    }

    return 0;
}

