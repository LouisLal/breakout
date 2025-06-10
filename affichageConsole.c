#include "affichageConsole.h"

void initAffichage(AffichageConsole* aff) {
    aff->hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    aff->bufferSize.X = NB_COLS;
    aff->bufferSize.Y = NB_LIGNES;
    aff->characterPos.X = 0;
    aff->characterPos.Y = 0;
    aff->writeArea.Left = 0;
    aff->writeArea.Top = 0;
    aff->writeArea.Right = NB_COLS - 1;
    aff->writeArea.Bottom = NB_LIGNES - 1;
    clearAffichage(aff);
}

void clearAffichage(AffichageConsole* aff) {
    for (int y = 0; y < NB_LIGNES; ++y) {
        for (int x = 0; x < NB_COLS; ++x) {
            aff->img[y][x].Char.AsciiChar = ' ';
            aff->img[y][x].Attributes = 0;
        }
    }
}

void putCharAffichage(AffichageConsole* aff, int x, int y, char c, WORD color) {
    if (x >= 0 && x < NB_COLS && y >= 0 && y < NB_LIGNES) {
        aff->img[y][x].Char.AsciiChar = c;
        aff->img[y][x].Attributes = color;
    }
}

void renderAffichage(AffichageConsole* aff) {
    WriteConsoleOutputA(
        aff->hConsole,
        (CHAR_INFO*)aff->img,
        aff->bufferSize,
        aff->characterPos,
        &(aff->writeArea)
    );
}
