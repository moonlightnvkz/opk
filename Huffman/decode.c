#include "decode.h"

void treeRestore(FILE* Finput, NodeStr* Nodes[256])
{
    int stackSize = 0;
    Bytes Byte = {0, 0, 0};
    while(1)
    {
        if(Byte.CBit == 0)
        {
            Byte.Byte = getc(Finput);
            Byte.CBit = 8;
            Byte.Bit = 1 & Byte.Byte >> (Byte.CBit - 1);
        }else
        {
            Byte.Bit = 1 & Byte.Byte >> (Byte.CBit - 1);
        }

        if(Byte.Bit == 0 && stackSize == 1) break;

        if(Byte.Bit == 1)
        {
            stackSize++;
            Nodes[stackSize - 1] = (NodeStr*)malloc(sizeof(NodeStr));
                if(Nodes[stackSize - 1] == NULL){
                    printf("Can't allocate memory while treeRestore for new char");
                    exit(1);}
            Nodes[stackSize - 1]->Char = Byte.Byte << (8 - Byte.CBit + 1);
            Byte.Byte = getc(Finput);
            Nodes[stackSize - 1]->Char = Nodes[stackSize - 1]->Char | Byte.Byte >> (Byte.CBit - 1);

            Nodes[stackSize - 1]->Left = NULL;
            Nodes[stackSize - 1]->Right = NULL;
        }else
        {
            NodeStr* temp = (NodeStr*)malloc(sizeof(NodeStr));
                if(temp == NULL){
                    printf("Can't allocate memory while treeRestore for temp");
                    exit(1);}
            temp->Char = 27;
            temp->Left = Nodes[stackSize - 2];
            temp->Right = Nodes[stackSize - 1];
            Nodes[stackSize - 1] = NULL;
            Nodes[stackSize - 2] = temp;
            stackSize--;
        }
        Byte.CBit--;
    }
}

void readBit(FILE* Finput, Bytes* Byte, LastByte* lastByte)
{
    if(Byte->CBit == 0 && feof(Finput) == 0)
    {
        Byte->Byte = lastByte->last;
        lastByte->last = lastByte->ifeof;
        lastByte->ifeof = getc(Finput);
        Byte->CBit = 8;
    }

    Byte->Bit = 1 & Byte->Byte >> (Byte->CBit - 1);
    Byte->CBit--;
}

unsigned char readTree(FILE* Finput, NodeStr* Node, Bytes* Byte, LastByte* lastByte)
{
    NodeStr* temp = Node;
    do
    {
        readBit(Finput, Byte, lastByte);


        if(Byte->Bit == 0 && temp->Left != NULL) temp = temp->Left;
        if(Byte->Bit == 1 && temp->Right != NULL) temp = temp->Right;
    }while (temp->Left != temp->Right);
    return temp->Char;
}

void decodeFile(FILE* Finput, FILE* Foutput, NodeStr* Node)
{
    Bytes Byte = {0, 0, 0};
    unsigned char ch = 27;
    LastByte lastByte = {getc(Finput), getc(Finput)};

    while(1)
    {
        ch = readTree(Finput, Node, &Byte, &lastByte);
        if(feof(Finput) != 0 && Byte.CBit < lastByte.last){
            break;
        }
        fwrite(&ch, 1, 1, Foutput);
    }
}

void deleteTree(NodeStr* Node)
{
    if(Node != NULL)
    {
        deleteTree(Node->Left);

        deleteTree(Node->Right);

        free(Node);
        Node = NULL;
    }
}
