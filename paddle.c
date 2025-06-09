#include "paddle.h"

void Paddle_init(Paddle* p, int x, int y, int width) {
    p->x = x;
    p->y = y;
    p->width = width;
}

void Paddle_moveLeft(Paddle* p, int minX) {
    if (p->x > minX) {
        p->x -= 2;
    }
}

void Paddle_moveRight(Paddle* p, int maxX) {
    if (p->x + p->width < maxX) {
        p->x += 2;
    }
}

void Paddle_moveLeftDefault(Paddle* p) {
    Paddle_moveLeft(p, 0);
}

void Paddle_moveRightDefault(Paddle* p) {
    Paddle_moveRight(p, 40);
}
