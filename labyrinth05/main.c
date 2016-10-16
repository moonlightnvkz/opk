#include <assert.h>
#include <stdlib.h>
#include <malloc.h>
#include "console.h"

#define TITLE_X 3
#define TITLE_Y 1

#define FIELD_PADDING 3

#define CHAR_BORDER '#'
#define CHAR_FIELD  ' '
#define CHAR_POINT  '@'
#define CHAR_EXIT   ' '

#define COLOR_BORDER 1
#define COLOR_FIELD  2
#define COLOR_POINT  3
#define COLOR_EXIT   4

typedef struct tPoint
{
    int x;
    int y;    

} Point;

typedef struct tLabyrinth
{
    int width;
    int height;
    int **field;    // 0 - blank cell, 1 - wall, 2 - exit
    Point start;
    Point exit;
} Labyrinth;

static int field_x, field_y; // top-left corner
static int field_width, field_height;
static int point_x, point_y;

/* Output char using given color pair at given position. */
static void con_charAt(int ch, int color, int x, int y) {
    con_gotoXY(x, y);
    con_setColor(color);
    con_outTxt("%c", ch);
}

static void init_colors() {
    con_initPair(COLOR_BORDER, CON_COLOR_BLACK, CON_COLOR_BLUE);
    con_initPair(COLOR_FIELD, CON_COLOR_GREEN, CON_COLOR_GREEN);
    con_initPair(COLOR_POINT, CON_COLOR_RED, CON_COLOR_GREEN);
    con_initPair(COLOR_EXIT, CON_COLOR_BLUE, CON_COLOR_BLUE);
}

static void initial_draw(Labyrinth *labyrinth) {
    con_clearScr();
    con_gotoXY(TITLE_X, TITLE_Y);
    con_outTxt("Use arrows to move point, use Esc to exit.");

    {
        for (int i = 0; i < labyrinth->width; ++i) {
            for (int j = 0; j < labyrinth->height; ++j) {
                int ch;
                int color;
                if (labyrinth->field[i][j] == 1) {
                    ch = CHAR_BORDER;
                    color = COLOR_BORDER;
                }
                if (labyrinth->field[i][j] == 0) {
                    ch = CHAR_FIELD;
                    color = COLOR_FIELD;
                }
                if (labyrinth->field[i][j] == 2) {
                    ch = CHAR_EXIT;
                    color = COLOR_EXIT;
                }
                con_charAt(ch, color, field_x + i, field_y + j);
            }
        }
    }

    point_x = field_x + field_width/2;
    point_y = field_y + field_height/2;
    con_charAt(CHAR_POINT, COLOR_POINT, point_x, point_y);
}

/* Returns 1 if quit. */
int process_key(int key, Labyrinth *labyrinth) {
    // position change
    int dx = 0;
    int dy = 0;

    switch (key) {
        case CON_KEY_ESCAPE:
            return 1;

        case CON_KEY_UP:
            if (labyrinth->field[point_x - field_x][point_y - field_y - 1] == 0) {  // 0 - blank cell, 1 - wall, 2 - exit
                dy = -1;
            }
            break;

        case CON_KEY_DOWN:
            if (labyrinth->field[point_x - field_x][point_y - field_y + 1] == 0) {
                dy = 1;
            }
            break;

        case CON_KEY_LEFT:
            if (labyrinth->field[point_x - field_x - 1][point_y - field_y] == 0) {
                dx = -1;
            }
            break;

        case CON_KEY_RIGHT:
            if (labyrinth->field[point_x - field_x + 1][point_y - field_y] == 0) {
                dx = 1;
            }
            break;
    }

    if (dx != 0 || dy != 0) {
        con_charAt(CHAR_FIELD, COLOR_FIELD, point_x, point_y);
        point_x += dx;
        point_y += dy;
        con_charAt(CHAR_POINT, COLOR_POINT, point_x, point_y);
    }
    if (labyrinth->field[point_x - field_x][point_y - field_y] == 2)    // 0 - blank cell, 1 - wall, 2 - exit
        return 1;
    return 0;
}

void enter_filling(int *filling)
{
    con_gotoXY(3, 3);
    con_outTxt("Enter labyrinth filling:");
    con_gotoXY(3, 4);
    con_outTxt("> ");
    scanf("%d", filling);
}

void generate_labyrinth(Labyrinth *labyrinth, int filling)
{
    labyrinth->field = malloc(sizeof(int*) * labyrinth->width);
    if (!labyrinth->field)
        exit(1);
    for (int i = 0; i < labyrinth->width; ++i)
    {
        labyrinth->field[i] = malloc(sizeof(int) * labyrinth->height);
        if (!labyrinth->field[i])
            exit(1);
    }

    for (int i = 0; i < labyrinth->width; ++i)
        for (int j = 0; j < labyrinth->height; ++j)
        {
            if (i == 0
             || i == labyrinth->width - 1
             || j == 0
             || j == labyrinth->height - 1)
            {
                labyrinth->field[i][j] = 1; // 0 - blank cell, 1 - wall, 2 - exit
                continue;
            }
            if (rand() % 100 < filling)
                labyrinth->field[i][j] = 1;
            else
                labyrinth->field[i][j] = 0;
        }
    int exit_x = rand() % (labyrinth->width - 2) + 1;
    int exit_y = rand() % (labyrinth->height - 2) + 1;
    labyrinth->field[exit_x][exit_y] = 2;
}

int main() {
    int quit = 0;
    int max_x, max_y;

    con_init();
    con_hideCursor();

    init_colors();

    // calculate size of field
    con_getMaxXY(&max_x, &max_y);
    field_x = FIELD_PADDING + TITLE_Y + 1;
    field_y = FIELD_PADDING;
    field_width = max_x - field_x - FIELD_PADDING;
    field_height = max_y - field_y - FIELD_PADDING;
    assert(field_width > 2);
    assert(field_height > 2);

    Labyrinth labyrinth;
    labyrinth.width = field_width;
    labyrinth.height = field_height;
    int filling;

    enter_filling(&filling);

    generate_labyrinth(&labyrinth, filling);

    initial_draw(&labyrinth);

    while (!quit) {
        if (con_keyPressed()) {
            if (process_key(con_getKey(), &labyrinth)) {
                quit = 1;
            }
        }
    }

    con_clearScr();
    con_deinit();
    return 0;
}
