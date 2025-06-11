#include "ball.h"

void Ball_init(Ball* ball, int x, int y, int dx, int dy) {
    ball->x = x;
    ball->y = y;
    ball->dx = dx;
    ball->dy = dy;
    ball->speed = 1;
    ball->tick = 0;
}


void Ball_move(Ball* b) {
    b->x += b->dx * b->speed;
    b->y += b->dy * b->speed;
}

void Ball_bounceX(Ball* b) {
    b->dx = -b->dx;
}

void Ball_bounceY(Ball* b) {
    b->dy = -b->dy;
}