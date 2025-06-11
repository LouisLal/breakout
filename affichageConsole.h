#pragma once
#include <windows.h>

#define NB_COLS 82
#define NB_LIGNES 30

typedef struct {
    CHAR_INFO img[NB_LIGNES][NB_COLS];
    HANDLE hConsole;
    COORD bufferSize;
    COORD characterPos;
    SMALL_RECT writeArea;
} AffichageConsole;

// Initialise l'affichage
void initAffichage(AffichageConsole* aff);

// Efface le buffer d'affichage (remplit de ' ')
void clearAffichage(AffichageConsole* aff);

// Écrit un caractère à une position donnée
void putCharAffichage(AffichageConsole* aff, int x, int y, char c, WORD color);

// Affiche le buffer dans la console
void renderAffichage(AffichageConsole* aff);
