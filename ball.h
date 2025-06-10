#ifndef BALL_H
#define BALL_H

typedef struct {
    float x, y;
    float dx, dy;
    float speed;
    int tick;
} Ball;

void Ball_init(Ball* b, int x, int y, int dx, int dy);
void Ball_move(Ball* b);
void Ball_bounceX(Ball* b);
void Ball_bounceY(Ball* b);

#endif // BALL_H
