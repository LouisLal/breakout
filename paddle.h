#ifndef PADDLE_H
#define PADDLE_H

typedef struct {
    int x, y;
    int width;
} Paddle;

void Paddle_init(Paddle* p, int x, int y, int width);
void Paddle_moveLeft(Paddle* p, int minX);
void Paddle_moveRight(Paddle* p, int maxX);

// Versions avec valeurs par défaut
void Paddle_moveLeftDefault(Paddle* p);   // minX = 0
void Paddle_moveRightDefault(Paddle* p);  // maxX = 40

#endif // PADDLE_H