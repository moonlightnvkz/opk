#ifndef CELL_H
#define CELL_H

#define CELLTYPE_COUNT 3

typedef enum tCellType
{
    CELL_BLANK,
    CELL_WALL,
    CELL_EXIT
} CellType;

typedef struct tCell
{
    int x;
    int y;
    int visited;    //1 - visited
    int type;
    int marker;
} Cell;

#endif // CELL_H
