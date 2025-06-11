#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#include "paddle.h"
#include "ball.h"
#include "brick.h"
#include "affichageConsole.h"

#define WIDTH 81
#define HEIGHT 30
#define PADDLE_WIDTH 13
#define BRICK_ROWS 3
#define BRICK_COLS 10
#define BRICK_WIDTH 8
#define BORDER_CHAR '*'

//Couleur de l'affichage:
#define COLOR_WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)


void draw(AffichageConsole* aff, const Paddle* paddle, const Ball* ball, const Brick bricks[], int brickCount, int score) {
    clearAffichage(aff);

    // Bordures
    for (int x = 0; x < WIDTH; ++x) {
        putCharAffichage(aff, x, 0, '-', COLOR_WHITE);                  // Haut
        putCharAffichage(aff, x, HEIGHT - 1, '-', COLOR_WHITE);        // Bas
    }
    for (int y = 0; y < HEIGHT; ++y) {
        putCharAffichage(aff, 0, y, '|', COLOR_WHITE);                 // Gauche
        putCharAffichage(aff, WIDTH - 1, y, '|', COLOR_WHITE);

    }

    // Affichage du score
    char scoreStr[32];
    sprintf(scoreStr, " Score: %d ", score);
    for (int i = 0; scoreStr[i] != '\0'; ++i) {
        putCharAffichage(aff, i + 2, 0, scoreStr[i], COLOR_WHITE);
    }

    // Briques
    for (int i = 0; i < brickCount; ++i) {
        if (bricks[i].alive) {
            for (int x = 0; x < BRICK_WIDTH; ++x) {
                putCharAffichage(aff, bricks[i].x + x, bricks[i].y, '#', bricks[i].color);
            }
        }
    }

    // Balle
    putCharAffichage(aff, ball->x, ball->y, 'O', COLOR_WHITE);

    // Raquette
    for (int x = 0; x < paddle->width; ++x) {
        putCharAffichage(aff, paddle->x + x, paddle->y, '=', COLOR_WHITE);
    }

    renderAffichage(aff);
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
    AffichageConsole aff;
    int score = 0;
    initAffichage(&aff);

    Paddle_init(&paddle, WIDTH / 2 - PADDLE_WIDTH / 2, HEIGHT - 2, PADDLE_WIDTH);
    Ball_init(&ball, WIDTH / 2, HEIGHT / 2, 0, -1);

    int brickCount = 0;
    for (int row = 0; row < BRICK_ROWS; ++row) {
        for (int col = 0; col < BRICK_COLS; ++col) {
            Brick_init(&bricks[brickCount++], 1 + col * BRICK_WIDTH, 2 + row);
        }
    }

    int running = 1;
    while (running) {
        DWORD start = GetTickCount();

        draw(&aff, &paddle, &ball, bricks, brickCount, score);

        // Contrôle utilisateur
        // Contrôle utilisateur avec GetAsyncKeyState (pas de délai auto-répétition)
        if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
            Paddle_moveLeft(&paddle, 0);
        }
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
            Paddle_moveRight(&paddle, WIDTH);
        }
        
        if (_kbhit()) {
            int ch = _getch();
            if (ch == 'q') {
                running = 0;
            }
        }

        // Déplacement de la balle
        ball.tick++;
        if (ball.tick >= ball.speed) {
            ball.tick = 0;
            Ball_move(&ball);

            if (ball.x <= 1 || ball.x >= WIDTH - 2) Ball_bounceX(&ball);
            if (ball.y <= 1) Ball_bounceY(&ball);


            if ((int)(ball.y) == paddle.y - 1 &&
                (int)(ball.x) >= paddle.x &&
                (int)(ball.x) < paddle.x + paddle.width) {

                Ball_bounceY(&ball);

                float impact = ((ball.x - paddle.x) / paddle.width) - 0.5f; // -0.5 à 0.5
                ball.dx = impact * 3.0f;  // Ajuste la force latérale
            }


            for (int i = 0; i < brickCount; ++i) {
                if (bricks[i].alive &&
                    ball.y == bricks[i].y &&
                    ball.x >= bricks[i].x &&
                    ball.x < bricks[i].x + BRICK_WIDTH) 
                {
                    score += 100; 
                    bricks[i].alive = 0;
                    Ball_bounceY(&ball);
                    break;
                }
            }
        }

        // Fin de partie : balle en bas
        if (ball.y >= HEIGHT - 2)
        {
            draw(&aff, &paddle, &ball, bricks, brickCount, score);
            Sleep(100);
            char msgFin[64];
            if(score < 1000){sprintf(msgFin, "T'es don bin trash man...\nScore: %d\nVeux-tu rejouer?", score);}
            else if (score < 2000){sprintf(msgFin, "Get gud :|\nScore: %d\nVeux-tu rejouer?", score);}
            else {sprintf(msgFin, "Get a life, c'est un jeu dans un terminal :|\nScore: %d\nVeux-tu rejouer?", score);}
            int msgboxID = MessageBox(NULL, msgFin, "Fin de la partie!", MB_YESNO);
            if (msgboxID == IDNO) {
                break;
            }
            else if(msgboxID == IDYES){
                Ball_init(&ball, WIDTH / 2, HEIGHT / 2, 0, -1);
                score = 0;
                int brickCount = 0;
                for (int row = 0; row < BRICK_ROWS; ++row) {
                    for (int col = 0; col < BRICK_COLS; ++col) {
                        Brick_init(&bricks[brickCount++], 1 + col * BRICK_WIDTH, 2 + row);
                    }
                }
            }
        }

        // Victoire
        int win = 1;
        for (int i = 0; i < brickCount; ++i) {
            if (bricks[i].alive) {
                win = 0;
                break;
            }
        }
        if (win) {
            draw(&aff, &paddle, &ball, bricks, brickCount, score);
            //MessageBoxA(NULL, "You Win!", "Victoire", MB_OK);
            
            //boucle pour replacer toutes les bricks au lieu d'arrêter le jeu
            int brickCount = 0;
            for (int row = 0; row < BRICK_ROWS; ++row) {
                for (int col = 0; col < BRICK_COLS; ++col) {
                    Brick_init(&bricks[brickCount++], 1 + col * BRICK_WIDTH, 2 + row);
                }
            }
        }

        waitNextFrame(start, 33); // environ 60 FPS
    }
    return 0;
}
