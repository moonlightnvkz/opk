#include <stdio.h>
#include <ctype.h>
#include "sn_translation.h"

int main() {
    printf("Enter length of the number:\n>");
    unsigned size;
    while (scanf("%d", &size) != 1) {
        printf("Wrong input. Try again...\n>");
        scanf("%c", (char*)&size);
    }

    // Base of initial scale notation
    unsigned base_from;
    printf("Enter input base:\n>");
    while (scanf("%d", &base_from) != 1) {
        printf("Wrong input. Try again...\n>");
        scanf("%c", (char*)&base_from);
    }

    // Base of output scale notation
    unsigned base_to;
    printf("Enter output base:\n>");
    while (scanf("%d", &base_to) != 1) {
        printf("Wrong input. Try again...\n>");
        scanf("%c", (char*)&base_to);
    }

    char c = (char)getchar();

    int *number = malloc(sizeof(int) * size);
    printf("Enter the number:\n>");
    for (unsigned i = 0; i < size; ++i) {
        if (scanf("%c", &c) != 1) {
            fprintf(stderr, "Wrong input:%s:%s:%d", __FILE__, __FUNCTION__, __LINE__);
            exit(2);
        }
        if ((!isdigit(c) && !isalpha(c)) || c - '0' >= base_from) {
            fprintf(stderr, "Wrong input:%s:%s:%d", __FILE__, __FUNCTION__, __LINE__);
            exit(2);
        }
        number[i] = (int) (c - '0');
    }

    size_t length;
    int *dec = sn_translate_to_dec(number, size, base_from, &length);
    int *out = sn_translate_from_dec(dec, length, base_to, &length);
    for (size_t i = length - 1; ; --i) {
        printf("%d", out[i]);
        if (i == 0) break;
    }
    free(dec);
    free(out);
    free(number);
    return 0;
}