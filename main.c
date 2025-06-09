#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#include "paddle.h"
#include "ball.h"
#include "brick.h"

#define WIDTH 40
#define HEIGHT 20
#define PADDLE_WIDTH 7
#define BRICK_ROWS 3
#define BRICK_COLS 10
#define BRICK_WIDTH 4

void clearScreen() {
    COORD topLeft = { 0, 0 };
    DWORD written;
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD cells;
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleScreenBufferInfo(console, &screen);
    cells = screen.dwSize.X * screen.dwSize.Y;
    FillConsoleOutputCharacter(console, ' ', cells, topLeft, &written);
    FillConsoleOutputAttribute(console, screen.wAttributes, cells, topLeft, &written);
    SetConsoleCursorPosition(console, topLeft);
}

void draw(const Paddle* paddle, const Ball* ball, const Brick bricks[], int brickCount) {
    clearScreen();
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            int drawn = 0;
            // Draw bricks
            for (int i = 0; i < brickCount; ++i) {
                if (bricks[i].alive &&
                    x >= bricks[i].x &&
                    x < bricks[i].x + BRICK_WIDTH &&
                    y == bricks[i].y) {
                    printf("#");
                    drawn = 1;
                    break;
                }
            }
            if (drawn) continue;
            // Draw ball
            if (x == ball->x && y == ball->y) {
                printf("O");
            } else if (y == paddle->y &&
                       x >= paddle->x &&
                       x < paddle->x + paddle->width) {
                printf("=");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void waitNextFrame(DWORD startTick, int targetMs) {
    DWORD elapsed = GetTickCount() - startTick;
    if (elapsed < targetMs) {
        Sleep(targetMs - elapsed);
    }
}

int main() {
    Paddle paddle;
    Ball ball;
    Brick bricks[BRICK_ROWS * BRICK_COLS];

    Paddle_init(&paddle, WIDTH / 2 - PADDLE_WIDTH / 2, HEIGHT - 2, PADDLE_WIDTH);
    Ball_init(&ball, WIDTH / 2, HEIGHT / 2, 1, -1);

    int brickCount = 0;
    for (int row = 0; row < BRICK_ROWS; ++row) {
        for (int col = 0; col < BRICK_COLS; ++col) {
            Brick_init(&bricks[brickCount++], col * BRICK_WIDTH, row + 2);
        }
    }

    int running = 1;
    while (running) {
        draw(&paddle, &ball, bricks, brickCount);

        // Input
        if (_kbhit()) {
            int ch = _getch();
            if (ch == 224) { // touche spéciale
                ch = _getch(); // code de la flèche
                if (ch == 75) { // flèche gauche
                    Paddle_moveLeft(&paddle, 0);
                } else if (ch == 77) { // flèche droite
                    Paddle_moveRight(&paddle, WIDTH);
                }
            } else if (ch == 'q') {
                running = 0;
            }
        }

        // Ball movement
        Ball_move(&ball);

        // Wall collision
        if (ball.x <= 0 || ball.x >= WIDTH - 1) Ball_bounceX(&ball);
        if (ball.y <= 0) Ball_bounceY(&ball);

        // Paddle collision
        if (ball.y == paddle.y - 1 &&
            ball.x >= paddle.x &&
            ball.x < paddle.x + paddle.width) {
            Ball_bounceY(&ball);
        }

        // Brick collision
        for (int i = 0; i < brickCount; ++i) {
            if (bricks[i].alive &&
                ball.y == bricks[i].y &&
                ball.x >= bricks[i].x &&
                ball.x < bricks[i].x + BRICK_WIDTH) {
                bricks[i].alive = 0;
                Ball_bounceY(&ball);
                break;
            }
        }

        // Lose condition
        if (ball.y >= HEIGHT - 1) {
            printf("Game Over!\n");
            break;
        }

        // Win condition
        int win = 1;
        for (int i = 0; i < brickCount; ++i) {
            if (bricks[i].alive) {
                win = 0;
                break;
            }
        }
        if (win) {
            printf("You Win!\n");
            break;
        }

        DWORD start = GetTickCount();
        waitNextFrame(start, 16);
    }

    system("pause");
    return 0;
}
