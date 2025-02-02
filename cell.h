
#ifndef CELL_H
#define CELL_H

#include <raylib.h>

typedef enum Cell {
    SPAWN,
    ENEMY,
    EMPTY,
    WALL_WHITE,
    WALL_BLUE,
    WALL_ORANGE
} Cell;

Color cell_colour(Cell cell) {
    switch (cell) {
        case SPAWN:
            return BLACK;
        case ENEMY:
            return BLACK;
        case EMPTY:
            return BLACK;
        case WALL_WHITE:
            return WHITE;
        case WALL_BLUE:
            return BLUE;
        case WALL_ORANGE:
            return ORANGE;
        default:
            return BLACK;
    }
}

#endif
