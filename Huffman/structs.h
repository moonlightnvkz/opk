#ifndef STRUCTS
#define STRUCTS

#define CodesSize 257

typedef struct NodeStr // Структура узла дерева
{
    unsigned char Char;
    int Freq;
    struct NodeStr *Left, *Right;
} NodeStr;

typedef struct CharCodesStr // Для хранения кодов символов
{
    unsigned char Char;
    char Code[CodesSize];
} CharCodes;

typedef struct ByteStr // Для чтения/записи
{
    unsigned char Byte;
    char CBit; // Current Bit
    char Bit;
} Bytes;

typedef struct LastByteStr
{
    unsigned char last;
    int ifeof;
} LastByte;

#endif // STRUCTS

