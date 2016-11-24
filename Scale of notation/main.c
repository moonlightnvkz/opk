#include <stdio.h>
#include <ctype.h>
#include "translating_to_from_dec.h"

int main() {
    printf("Enter length of the number:\n>");
    unsigned size;
    while (scanf("%d", &size) != 1) {
        printf("Wrong input. Try again...\n>");
        scanf("%c", (char*)&size);
    }

    unsigned radix_from;
    printf("Enter input radix:\n>");
    while (scanf("%d", &radix_from) != 1) {
        printf("Wrong input. Try again...\n>");
        scanf("%c", (char*)&radix_from);
    }
    unsigned radix_to;
    printf("Enter output radix:\n>");
    while (scanf("%d", &radix_to) != 1) {
        printf("Wrong input. Try again...\n>");
        scanf("%c", (char*)&radix_to);
    }

    char c = (char)getchar();

    int *number = malloc(sizeof(int) * size);
    printf("Enter the number:\n>");
    for (unsigned i = 0; i < size; ++i) {
        if (scanf("%c", &c) != 1) {
            fprintf(stderr, "Wrong input:%s:%s:%d", __FILE__, __FUNCTION__, __LINE__);
            exit(2);
        }
        if ((!isdigit(c) && !isalpha(c)) || c - '0' >= radix_from) {
            fprintf(stderr, "Wrong input:%s:%s:%d", __FILE__, __FUNCTION__, __LINE__);
            exit(2);
        }
        number[i] = (int) (c - '0');
    }

    size_t length;
    int *dec = translate_to_dec(number, size, radix_from, &length);
    int *out = translate_from_dec(dec, length, radix_to, &length);
    for (size_t i = length - 1; ; --i) {
        printf("%d", out[i]);
        if (i == 0) break;
    }
    free(dec);
    free(out);
    free(number);
    return 0;
}