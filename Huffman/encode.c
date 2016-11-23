#include "encode.h"

int charPositionInNodes(NodeStr* Nodes[256], unsigned char ch)
{
    for(int i = 0; Nodes[i] != NULL; ++i)
    {
        if(Nodes[i]->Char == ch) return i;
    }
    return -1;
}

void getFrequency(FILE* Finput, NodeStr* Nodes[256], int* SymbAmount)
{
    unsigned char ch;
    int charPos = 0;
    while(1)
    {
        ch = getc(Finput);
        if(feof(Finput) != 0) break;
        charPos = charPositionInNodes(Nodes, ch);
        if(charPos == -1)
        {
            Nodes[*SymbAmount] = (NodeStr*)malloc(sizeof(NodeStr));
                if(Nodes[*SymbAmount] == NULL){
                    printf("Can't allocate memory in getFreq()");
                    exit(1);}
            Nodes[*SymbAmount]->Char = ch;
            Nodes[*SymbAmount]->Freq = 1;
            Nodes[*SymbAmount]->Left = NULL;
            Nodes[*SymbAmount]->Right = NULL;
            (*SymbAmount)++;
        } else
        {
            Nodes[charPos]->Freq++;
        }
    }
}

void treeCreate(NodeStr* Nodes[256], int SymbAmount)
{
    NodeStr* temp = NULL;

    if(Nodes[1] == NULL)
    {
        temp = (NodeStr*)malloc(sizeof(NodeStr));
            if(temp == NULL){
                printf("Can't allocate memory in treeCreate()");
                exit(1);
            }
        temp->Char = 27;
        temp->Right = NULL;
        temp->Left = Nodes[0];
        temp->Freq = Nodes[0]->Freq;

        Nodes[0] = temp;
    }else while(Nodes[1] != NULL)
    {
        temp = (NodeStr*)malloc(sizeof(NodeStr));
            if(temp == NULL){
                printf("Can't allocate memory in treeCreate()");
                exit(1);
            }
        temp->Char = 27;
        temp->Right = Nodes[SymbAmount - 1];
        temp->Left = Nodes[SymbAmount - 2];
        temp->Freq = Nodes[SymbAmount - 1]->Freq + Nodes[SymbAmount - 2]->Freq;

        Nodes[SymbAmount - 1] = NULL;
        Nodes[SymbAmount - 2] = temp;
        --SymbAmount;

        BubbleSort(Nodes, SymbAmount);
    }
    temp = NULL;
}

void TreePosition(char CurrCode[CodesSize], int NodeType)
{
    int i = 0;
    while(CurrCode[i] != -1) ++i;
    if((NodeType == -1) && (i>0))
    {
        CurrCode[i-1] = -1;
    }
    else
    {
        CurrCode[i] = NodeType;
        CurrCode[i+1] = -1;
    }
}

void addCodeForChar(char Char, CharCodes Codes[256], char CurrCode[CodesSize])
{
    int i = 0;
    int j = 0;
    while(Codes[j].Code[0] != -1) ++j;
    Codes[j].Char = Char;
    while(CurrCode[i] != -1)
    {
        Codes[j].Code[i] = CurrCode[i];
        ++i;
    }
}

void formCodesAndWriteTree(FILE* Foutput, NodeStr* Node, CharCodes Codes[256], char CurrCode[CodesSize], Bytes* Byte)
{
    if(Node != NULL)
    {
        TreePosition(CurrCode, 0);
        formCodesAndWriteTree(Foutput, Node->Left, Codes, CurrCode, Byte);

        TreePosition(CurrCode, 1);
        formCodesAndWriteTree(Foutput, Node->Right, Codes, CurrCode, Byte);

        if(Node->Left == Node->Right)
        {
            addCodeForChar(Node->Char, Codes, CurrCode);
            writeBit(Foutput, 1, Byte);
            writeByte(Foutput, Node->Char, Byte);
        }else
        {
            writeBit(Foutput, 0, Byte);
        }

        TreePosition(CurrCode, -1);
        free(Node);
        Node = NULL;
    } else
    {
        TreePosition(CurrCode, -1);
    }
}

void writeBit(FILE* Foutput, unsigned char ch, Bytes* Byte)
{
    Byte->Byte = Byte->Byte | ch << (Byte->CBit - 1);
    Byte->CBit--;
    if(Byte->CBit == 0)
    {
        fwrite(&Byte->Byte, 1, 1, Foutput);
        Byte->CBit = 8;
        Byte->Byte = 0;
    }
}

void writeByte(FILE* Foutput, unsigned char ch, Bytes* Byte)
{
    if(Byte->CBit < 8)
    {
        Byte->Byte = Byte->Byte | ch >> (8 - Byte->CBit);
        fwrite(&Byte->Byte, 1, 1, Foutput);
        Byte->Byte = 0;
        Byte->Byte = Byte->Byte | ch << Byte->CBit;
    }
    else
    {
        fwrite(&ch, 1, 1, Foutput);
    }
}

int searchCode(unsigned char ch, CharCodes Codes[256])
{
    int i = 0;
    for(i = 0; Codes[i].Char != ch; ++i);
    return i;
}

void encodeFile(FILE* Finput, FILE* Foutput, CharCodes Codes[256], Bytes* Byte)
{
    unsigned char ch;
    int chIndex, i = 0;
    rewind(Finput);
    while(1)
    {
        i = 0;
        ch = getc(Finput);
        if(feof(Finput) != 0) break;
        chIndex = searchCode(ch, Codes);
        while(Codes[chIndex].Code[i] != -1)
        {
            writeBit(Foutput, Codes[chIndex].Code[i], Byte);
            ++i;
        }
    }
    if(Byte->CBit != 8) // Дозапись файла, если в Byte остались значащие биты
    {
        fwrite(&Byte->Byte, 1, 1, Foutput);
        fwrite(&Byte->CBit, 1, 1, Foutput);
        Byte->Byte = 0;
        Byte->CBit = 8;
    }else{
        Byte->CBit = 0;
        fwrite(&Byte->CBit, 1, 1, Foutput);
        Byte->CBit = 8;
    }
}
