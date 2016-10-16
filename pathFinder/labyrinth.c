#include "labyrinth.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "mystack.h"

void labyrinth_start_set(Labyrinth *labyrinth, int x, int y)
{
    labyrinth->start_x = x;
    labyrinth->start_y = y;
}

void labyrinth_generate(Labyrinth *labyrinth, int width, int height, int filling)
{
    assert(filling >= 0);
    labyrinth->width = width;
    labyrinth->height = height;
    labyrinth->field = malloc(sizeof(Cell *) * width);
    assert(labyrinth->field != NULL);
    for (int i = 0 ; i < width; ++i)
    {
        labyrinth->field[i] = malloc(sizeof(Cell) * height);
        assert(labyrinth->field[i] != NULL);
        for (int j = 0; j < height; ++ j)
        {
            labyrinth->field[i][j].x = i;
            labyrinth->field[i][j].y = j;
            labyrinth->field[i][j].visited = 0;
            if (((i % 2 == 0 || j % 2 == 0) && rand() % 100 < filling) ||
                    i == 0 || j == 0 || i == width - 1 || j == height - 1)
                labyrinth->field[i][j].type = CELL_WALL;
            else
                labyrinth->field[i][j].type = CELL_BLANK;
            labyrinth->field[i][j].marker = -1;
        }
    }

    // Labyrinth generation
    Stack stack;
    stack_tune(&stack, 50, 5);
    stack_create(&stack);
    labyrinth->field[1][1].visited = 1;
    stack_push(&stack, &labyrinth->field[1][1]);

    int exits_amount = EXITS_AMOUNT;
    while(stack_size(&stack) != 0)
    {
        Cell *c = (Cell *)stack_peek(&stack);
        Cell* neighbours[4] = {0};
        int nCount = 0;
        int dx[4] = {-2, 0, 2, 0};
        int dy[4] = {0, -2, 0, 2};
        for (size_t i = 0; i < 4; ++i)
        {
            if (c->x + dx[i] >= 0 && c->x + dx[i] < labyrinth->width &&
                c->y + dy[i] >= 0 && c->y + dy[i] < labyrinth->height &&
                labyrinth->field[c->x + dx[i]][c->y + dy[i]].visited == 0)
                neighbours[nCount++] = &labyrinth->field[c->x + dx[i]][c->y + dy[i]];
        }

        if (nCount != 0)
        {
            Cell *next = neighbours[rand() % nCount];
            Cell *wall = &labyrinth->field[(c->x + next->x) / 2][(c->y + next->y) / 2];
            wall->type = 0;
            stack_push(&stack, next);
            next->visited = 1;
        }
        else
        {
            if (exits_amount > 0)
            {
                labyrinth->field[c->x][c->y].type = CELL_EXIT;
                exits_amount--;
            }
            stack_pop(&stack);
        }
    }
    stack_destroy(&stack);
}

void labyrinth_destroy(Labyrinth *labyrinth)
{
    for (int i = 0; i < labyrinth->width; ++i)
    {
        free(labyrinth->field[i]);
    }
    free(labyrinth->field);
}

int labyrinth_findPath(int current_x, int current_y, Labyrinth *labyrinth, Path *path)
{
    assert(labyrinth != NULL);
    assert(path != NULL);
    if (labyrinth->field[current_x][current_y].type == CELL_WALL)
        return 0;

    if (labyrinth->field[current_x][current_y].type == CELL_EXIT)
    {
        path->len = 0;
        return 1;
    }

    int d = 0;
    labyrinth->field[current_x][current_y].marker = d;
    int stop;
    Cell *exit = NULL;
    do
    {
        stop = 1;
        for (int i = 0; i < labyrinth->width; ++i)
            for (int j = 0; j < labyrinth->height; ++j)
                if (labyrinth->field[i][j].marker == d)
                {
                    int dx[4] = {-1, 0, 1, 0};
                    int dy[4] = {0, -1, 0, 1};
                    for (int k = 0; k < 4; ++k)
                    {
                        if (i + dx[k] >= 0 && i + dx[k] < labyrinth->width &&
                            j + dy[k] >= 0 && j + dx[k] < labyrinth->height &&
                            labyrinth->field[i + dx[k]][j + dy[k]].type != CELL_WALL &&
                            labyrinth->field[i + dx[k]][j + dy[k]].marker == -1)
                        {
                            labyrinth->field[i + dx[k]][j + dy[k]].marker = d + 1;
                            if (labyrinth->field[i + dx[k]][j + dy[k]].type == CELL_EXIT)
                                exit = &labyrinth->field[i + dx[k]][j + dy[k]];
                            stop = 0;
                        }
                    }
                }
        d++;
    } while (!stop && exit == NULL);

    if (exit == NULL)
    {
        path->len = 0;
        return 0;
    }
    path->len = exit->marker;
    int x = exit->x;
    int y = exit->y;
    d = path->len;
    while (d > 0)
    {
        path->x[d] = x;
        path->y[d] = y;
        d--;
        int dx[4] = {-1, 0, 1, 0};
        int dy[4] = {0, -1, 0, 1};
        for (int i = 0; i < 4; ++i)
        {
            if (x + dx[i] >= 0 && x + dx[i] < labyrinth->width &&
                y + dy[i] >= 0 && y + dy[i] < labyrinth->height &&
                labyrinth->field[x + dx[i]][y + dy[i]].marker == d)
            {
                x += dx[i];
                y += dy[i];
                break;
            }
        }
    }
    path->x[0] = current_x;
    path->y[0] = current_y;
    return 0;
}

void path_init(Path *ppath, int field_width, int field_height)
{
    assert(ppath != NULL);
    ppath->x = malloc(sizeof(int) * field_width * field_height);
    assert(ppath->x != NULL);
    ppath->y = malloc(sizeof(int) * field_width * field_height);
    assert(ppath->y != NULL);
    ppath->allocated = field_height * field_width;
    ppath->len = 0;
}

int labyrinth_write_to_file(Labyrinth *labyrinth, char* file_name)
{
    FILE* fout = fopen(file_name, "wt");
    if (!fout)
        return 0;
    fprintf(fout, "%d,%d", labyrinth->width, labyrinth->height);
    fprintf(fout, ",%d,%d", labyrinth->start_x, labyrinth->start_y);
    for (int i = 0; i < labyrinth->width; ++i)
        for (int j = 0; j < labyrinth->height; ++j)
            fprintf(fout, ",%d", labyrinth->field[i][j].type);
    fclose(fout);
    return 1;
}

int labyrinth_read_from_file(Labyrinth *labyrinth, char* file_name)
{
    assert(labyrinth != NULL);
    assert(file_name != NULL);
    FILE* fin = fopen(file_name, "rt");
    if (!fin)
        return 0;
    if (fscanf(fin, "%d,%d,%d,%d", &labyrinth->width, &labyrinth->height, &labyrinth->start_x, &labyrinth->start_y) < 4)
    {
        fprintf(stderr, "Error when reading data from file");
        return 0;
    }

    labyrinth->field = malloc(sizeof(Cell *) * labyrinth->width);
    assert(labyrinth->field != NULL);
    for (int i = 0; i < labyrinth->width; ++i)
    {
        labyrinth->field[i] = malloc(sizeof(Cell) * labyrinth->height);
        assert(labyrinth->field[i] != NULL);
        for (int j = 0; j < labyrinth->height; ++j)
        {
            if (fscanf(fin, ",%d", &labyrinth->field[i][j].type) == 0)
            {
                labyrinth_destroy(labyrinth);
                return 0;
            }
            labyrinth->field[i][j].x = i;
            labyrinth->field[i][j].y = j;
            labyrinth->field[i][j].visited = 0;
            labyrinth->field[i][j].marker = -1;
        }
    }
    fclose(fin);
    return 1;
}
