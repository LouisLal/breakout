#include "brick.h"

void Brick_init(Brick* b, int x, int y) {
    b->x = x;
    b->y = y;
    b->alive = true;
}