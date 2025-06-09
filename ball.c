#include "ball.h"

void Ball_init(Ball* b, int x, int y, int dx, int dy) {
    b->x = x;
    b->y = y;
    b->dx = dx;
    b->dy = dy;
}

void Ball_move(Ball* b) {
    b->x += b->dx;
    b->y += b->dy;
}

void Ball_bounceX(Ball* b) {
    b->dx = -b->dx;
}

void Ball_bounceY(Ball* b) {
    b->dy = -b->dy;
}