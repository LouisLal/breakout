#ifndef BRICK_H
#define BRICK_H

#include <stdbool.h>

typedef struct {
    int x, y;
    bool alive;
} Brick;

void Brick_init(Brick* b, int x, int y);

#endif // BRICK_H
