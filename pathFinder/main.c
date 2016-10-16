#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "console.h"
#include "mystack.h"
#include "constants.h"
#include "cell.h"
#include "labyrinth.h"

static int field_x, field_y; // top-left corner
static int field_width, field_height;
static int point_x, point_y;
static Labyrinth labyrinth;
static Path path;
static int draw_wave_flag;
static int draw_path_flag;
static int edit_mode_flag;

/* Output char using given color pair at given position. */
static void con_charAt(int ch, int color, int x, int y) {
    con_gotoXY(x, y);
    con_setColor(color);
    con_outTxt("%c", ch);
}

static void init_colors() {
    use_default_colors();
    for (int i = 0; i < COLORS; ++i)
        if (i >= RESERVED_COLORS)
            con_initPair(i + 1, i, i);
    con_initPair(COLOR_BORDER, CON_COLOR_BLACK, CON_COLOR_BLUE);
    con_initPair(COLOR_FIELD, CON_COLOR_GREEN, CON_COLOR_GREEN);
    con_initPair(COLOR_POINT, CON_COLOR_YELLOW, CON_COLOR_GREEN);
    con_initPair(COLOR_TEXT, COLOR_WHITE, -1);
    con_initPair(COLOR_START, CON_COLOR_CYAN, CON_COLOR_CYAN);
    con_initPair(COLOR_EXIT, CON_COLOR_YELLOW, CON_COLOR_YELLOW);
    con_initPair(COLOR_PATH, CON_COLOR_RED, CON_COLOR_RED);
}

void drawPoint(Cell *cell)
{
    if (cell->type == CELL_EXIT)
    {
        attron(A_STANDOUT);
        con_charAt(CHAR_EXIT, COLOR_EXIT, field_x + cell->x, field_y + cell->y);
        attroff(A_STANDOUT);
    } else if (cell->type == CELL_WALL)
    {
        con_charAt(CHAR_BORDER, COLOR_BORDER, field_x + cell->x, field_y + cell->y);
    } else if (cell->type == CELL_BLANK)
    {
        attron(A_DIM);
        con_charAt(CHAR_FIELD, COLOR_FIELD, field_x + cell->x, field_y + cell->y);
        attroff(A_DIM);
    } else {
        con_charAt(CHAR_POINT, COLOR_POINT, field_x + cell->x, field_y + cell->y);
    }
}

int oddRand(int min, int max)
{
    return ((rand() % ((max - min + min % 2) >> 1) + ((min - min % 2) >> 1)) << 1) + 1;
}

void draw_labyrinth()
{
    for (int i = 0; i < labyrinth.width; ++i)
    {
        for (int j = 0; j < labyrinth.height; ++j)
        {
            drawPoint(&labyrinth.field[i][j]);
        }
    }
    con_charAt(CHAR_START, COLOR_START, labyrinth.start_x, labyrinth.start_y);
}

void wavemap_reset()
{
    for (int i = 0; i < labyrinth.width; ++i)
        for (int j = 0; j < labyrinth.height; ++j)
            labyrinth.field[i][j].marker = -1;
}

void draw_wavemap()
{
    for (int i = 0; i < labyrinth.width; ++i)
    {
        for (int j = 0; j < labyrinth.height; ++j)
        {
            if (labyrinth.field[i][j].type == CELL_EXIT)
                continue;
            if (labyrinth.field[i][j].marker != -1)
             con_charAt(' ', labyrinth.field[i][j].marker %
                        (COLORS - RESERVED_COLORS) + RESERVED_COLORS,
                        field_x + i, field_y + j);
        }
    }
}

void draw_path()
{
    for (size_t i = 0; i < path.len; ++i)
    {
        con_charAt('-', COLOR_PATH, path.x[i] + field_x, path.y[i] + field_y);
    }
}

void redraw()
{
    draw_labyrinth();
    if (draw_wave_flag)
        draw_wavemap();

    if (draw_path_flag)
        draw_path();
    con_charAt(CHAR_POINT, COLOR_POINT, point_x, point_y);
}

static void initial_draw() {
    con_clearScr();
    con_gotoXY(TITLE_X, TITLE_Y);
    con_setColor(COLOR_TEXT);
    con_outTxt("Use arrows, find an exit (");
    Cell exit = {TITLE_X + strlen("Use arrows, find an exit (") - field_x, TITLE_Y - field_y, 0, CELL_EXIT, 0};
    drawPoint(&exit);
    con_setColor(COLOR_TEXT);
    con_outTxt("). F2 - shortest path, F3 - wave,");
    con_gotoXY(TITLE_X, TITLE_Y + 1);
    con_outTxt("F4 - edit mode, F5 - change cell type, F6 - set start, Esc - quit.");

    draw_labyrinth();

    if (draw_wave_flag)
        draw_wavemap();
    if (draw_path_flag)
        draw_path();
    con_charAt(CHAR_POINT, COLOR_POINT, point_x, point_y);

}

/* Returns 1 if quit. */
int process_key(int key) {
    // position change
    int dx = 0;
    int dy = 0;

    switch (key) {
        case CON_KEY_ESCAPE:
            return 1;

        case CON_KEY_UP:
            if (point_y - 1 > field_y) {
                dy = -1;
            }
            break;

        case CON_KEY_DOWN:
            if (point_y + 1 < field_y + field_height - 1) {
                dy = 1;
            }
            break;

        case CON_KEY_LEFT:
            if (point_x - 1 > field_x) {
                dx = -1;
            }
            break;

        case CON_KEY_RIGHT:
            if (point_x + 1 < field_x + field_width - 1) {
                dx = 1;
            }
            break;
        case KEY_F(2):
//            if (edit_mode_flag)
//                break;
            draw_path_flag = !draw_path_flag;
            redraw();
            break;
        case KEY_F(3):
//            if (edit_mode_flag)
//                break;
            draw_wave_flag = !draw_wave_flag;
            redraw();
            break;
        case KEY_F(4):
            edit_mode_flag = !edit_mode_flag;
//            draw_path_flag = 0;
//            draw_wave_flag = 0;
            break;
        case KEY_F(5):
            if (!edit_mode_flag)
                break;
            labyrinth.field[point_x - field_x][point_y - field_y].type =
                    (labyrinth.field[point_x - field_x][point_y - field_y].type + 1) % CELLTYPE_COUNT;
            redraw();
            break;
        case KEY_F(6):
            if (!edit_mode_flag)
                break;
            labyrinth_start_set(&labyrinth, point_x, point_y);
            if (draw_wave_flag)
                draw_wavemap();
            else
                draw_labyrinth();

            if (draw_path_flag)
                draw_path();
            con_charAt(CHAR_POINT, COLOR_POINT, point_x, point_y);
            break;
    }

    if ((dx != 0 || dy != 0) &&
            (labyrinth.field[point_x - field_x + dx][point_y - field_y + dy].type != CELL_WALL || edit_mode_flag))
    {
        switch (labyrinth.field[point_x - field_x + dx][point_y - field_y + dy].type)
        {
            case CELL_WALL:
                con_charAt(CHAR_BORDER, COLOR_BORDER, point_x, point_y);
                break;
            case CELL_BLANK:
                con_charAt(CHAR_FIELD, COLOR_FIELD, point_x, point_y);
                break;
            case CELL_EXIT:
                con_charAt(CHAR_EXIT, COLOR_EXIT, point_x, point_y);
                break;
            default:
                con_charAt(CHAR_FIELD, COLOR_FIELD, point_x, point_y);
        }

        point_x += dx;
        point_y += dy;
        wavemap_reset();
        draw_labyrinth();
        labyrinth_findPath(point_x - field_x, point_y - field_y, &labyrinth, &path);
        if (draw_wave_flag)
            draw_wavemap();
        if (draw_path_flag)
            draw_path();
        con_charAt(CHAR_POINT, COLOR_POINT, point_x, point_y);
    }

    if (labyrinth.field[point_x - field_x][point_y - field_y].type == CELL_EXIT && !edit_mode_flag)
    {
        con_gotoXY(field_x + field_width / 2 - 8, field_y + field_height + 1);

        attrset(COLOR_PAIR(COLOR_TEXT) | A_BOLD);
        con_outTxt("Congratulations!");
        con_gotoXY(field_x + field_width / 2 - 14, field_y + field_height + 2);
        con_outTxt("Press Enter key to continue...");
        while (con_getKey() != CON_KEY_ENTER && con_getKey() != CON_KEY_ESCAPE);
        return 1;
    }
    return 0;
}

void out_greetings()
{
    con_gotoXY(TITLE_X + 10, TITLE_Y + 2);
    con_setColor(COLOR_TEXT);
    con_outTxt("Greetings!");
    con_gotoXY(TITLE_X, TITLE_Y + 3);
    con_outTxt("Use arrows to move ");
    con_charAt(CHAR_POINT, COLOR_POINT, TITLE_X + strlen("Use arrows to move "), TITLE_Y + 3);
    con_setColor(COLOR_TEXT);
    con_outTxt(", you need to find an exit (");
    Cell exit = {TITLE_X + strlen("Use arrows to move , you need to find an exit (") + 1 - field_x, TITLE_Y + 3 - field_y, 0, CELL_EXIT, 0};
    drawPoint(&exit);
    con_setColor(COLOR_TEXT);
    con_outTxt(").");
    con_gotoXY(TITLE_X, TITLE_Y + 4);
    con_outTxt("F2 - show the shortest path");
    con_gotoXY(TITLE_X, TITLE_Y + 5);
    con_outTxt("F3 - show the wave");
    con_gotoXY(TITLE_X, TITLE_Y + 6);
    con_outTxt("F4 - toggle edit mode");
    con_gotoXY(TITLE_X, TITLE_Y + 7);
    con_outTxt("F5 (in edit mode) - change cell type");
    con_gotoXY(TITLE_X, TITLE_Y + 8);
    con_outTxt("F6 (in edit mode) - set start position");
    con_gotoXY(TITLE_X, TITLE_Y + 9);
    con_outTxt("Esc - quit");
    con_gotoXY(TITLE_X, TITLE_Y + 10);
    con_outTxt("");
    con_gotoXY(TITLE_X, TITLE_Y + 11);
}

void greet(int *filling)
{
    out_greetings();
    con_outTxt("Enter labyrinth filling:");
    con_gotoXY(TITLE_X, TITLE_Y + 12);
    con_outTxt("> ");

    echo();
    nodelay(stdscr, FALSE);

    do{
        scanw("%d", filling);
        if (*filling < 0)
        {
            con_clearScr();
            out_greetings();
            con_outTxt("Enter labyrinth filling:");
            con_gotoXY(TITLE_X, TITLE_Y + 12);
            con_outTxt("Filling must be non-negative!");
            con_gotoXY(TITLE_X, TITLE_Y + 11);
            con_outTxt("> ");
        }

    } while (*filling < 0);
    nodelay(stdscr, TRUE);
    noecho();
}

void args_parse(int argc, char* argv[], char** in, char** out)
{
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(INPUT_FILE_ARG, argv[i]) == 0)
            *in = argv[i + 1];
        if (strcmp(OUTPUT_FILE_ARG, argv[i]) == 0)
            *out = argv[i + 1];
    }
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    int quit = 0;
    int max_x, max_y;

    con_init();
    con_hideCursor();

    init_colors();

    // calculate size of field
    con_getMaxXY(&max_x, &max_y);
    field_x = FIELD_PADDING + TITLE_Y;
    field_y = FIELD_PADDING + 1;
    field_width = (max_x - field_x - FIELD_PADDING - 2) / 3 * 3 + 2;
    field_height = (max_y - field_y - FIELD_PADDING -2 ) / 3 * 3  +2;
    assert(field_width > 2);
    assert(field_height > 2);

    char *input_file = NULL;
    char *output_file = NULL;
    args_parse(argc, argv, &input_file, &output_file);

    int filling = 100;
    if (input_file != NULL)
    {
        if (!labyrinth_read_from_file(&labyrinth, input_file))
        {
            con_gotoXY(TITLE_X, TITLE_Y);
            con_setColor(COLOR_TEXT);
            con_outTxt("An error occured when parsing file.");
            greet(&filling);
            point_x = field_x + oddRand(5, LABYRINTH_WIDTH - 5);
            point_y = field_y + oddRand(3, LABYRINTH_HEIGHT - 3);
            labyrinth_start_set(&labyrinth, point_x, point_y);
            labyrinth_generate(&labyrinth, LABYRINTH_WIDTH, LABYRINTH_HEIGHT, filling);
        } else {
            point_x = labyrinth.start_x;
            point_y = labyrinth.start_y;
        }
    } else {
        greet(&filling);
        point_x = field_x + oddRand(5, LABYRINTH_WIDTH - 5);
        point_y = field_y + oddRand(3, LABYRINTH_HEIGHT - 3);
        labyrinth_start_set(&labyrinth, point_x, point_y);
        labyrinth_generate(&labyrinth, LABYRINTH_WIDTH, LABYRINTH_HEIGHT, filling);
    }
    assert(labyrinth.width > 0);
    assert(labyrinth.height > 0);

    path_init(&path, field_width, field_height);
    labyrinth_findPath(point_x - field_x, point_y - field_y, &labyrinth, &path);

    initial_draw();

    while (!quit) {
        if (con_keyPressed()) {
            if (process_key(con_getKey())) {
                quit = 1;
            }
        }
    }

    if (output_file != NULL)
        if(!labyrinth_write_to_file(&labyrinth, output_file))
            perror("Can't write to file: ");

    labyrinth_destroy(&labyrinth);
    con_clearScr();
    con_deinit();
    return 0;
}
