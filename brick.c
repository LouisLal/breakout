#include "brick.h"

void Brick_init(Brick* b, int x, int y) {
    b->x = x;
    b->y = y;
    b->alive = true;

    WORD colors[] = {
        FOREGROUND_BLUE, FOREGROUND_GREEN, FOREGROUND_RED,
        (FOREGROUND_RED | FOREGROUND_GREEN),  (FOREGROUND_GREEN | FOREGROUND_BLUE), (FOREGROUND_RED | FOREGROUND_BLUE)
    };
    int nbColors = sizeof(colors) / sizeof(colors[0]);
    b->color = colors[rand() % nbColors];
}