#ifndef LABYRINTH_H
#define LABYRINTH_H
#include <stdlib.h>
#include "cell.h"
#include "constants.h"

typedef struct tLabyrint
{
    Cell **field;
    int width;
    int height;
    int start_x;
    int start_y;
}Labyrinth;

typedef struct tPath
{
    int *x;
    int *y;
    size_t len;
    size_t allocated;
    size_t increment;
} Path;

void labyrinth_generate(Labyrinth *labyrinth, int width, int height, int filling);

int labyrinth_read_from_file(Labyrinth *labyrinth, char* file_name);

int labyrinth_write_to_file(Labyrinth *labyrinth, char* file_name);

void labyrinth_destroy(Labyrinth *labyrinth);

void labyrinth_start_set(Labyrinth *labyrinth, int x, int y);

int labyrinth_findPath(int current_x, int current_y, Labyrinth *labyrinth, Path *path);

void path_init(Path *ppath, int field_width, int field_height);

#endif // LABYRINTH_H
