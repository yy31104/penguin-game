#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "save_load.h"

static int** malloc_2d_int(int rows, int cols) {
    int** a = (int**)malloc(rows * sizeof * a);
    if (!a) return NULL;
    for (int i = 0; i < rows; ++i) {
        a[i] = (int*)malloc(cols * sizeof * a[i]);
        if (!a[i]) {
            for (int k = 0; k < i; ++k) free(a[k]);
            free(a);
            return NULL;
        }
    }
    return a;
}

int save_game(const char* filename, const struct GameState* gs, const struct players* pl) {
    if (!filename || !gs || !pl || !gs->board.arr) return 0;

    FILE* f = fopen(filename, "w");
    if (!f) return 0;

    fprintf(f, "PENGUIN_SAVE 1\n");
    fprintf(f, "%d %d\n", gs->board.rows, gs->board.cols);

    for (int r = 0; r < gs->board.rows; ++r) {
        for (int c = 0; c < gs->board.cols; ++c) {
            fprintf(f, "%d%c", gs->board.arr[r][c], (c + 1 == gs->board.cols) ? '\n' : ' ');
        }
    }

    fprintf(f, "%d %d\n", gs->hasPlacementPhaseEnded, gs->placingPenguinsOnlyOnOne);

    fprintf(f, "%d %d %d\n", pl->currentPlayer, pl->maxNumOfPenForPlayer, pl->someoneCantMove);

    for (int i = 0; i < 2; ++i) {
        fprintf(f, "\"%s\"\n", pl->playr[i].name);
        fprintf(f, "%d %d %d\n",
            pl->playr[i].score,
            pl->playr[i].numOfPenguinsCheckedOut,
            pl->playr[i].numberOfPlacedPenguins);

        for (int k = 0; k < pl->maxNumOfPenForPlayer; ++k) {
            struct Penguin p = pl->playr[i].penguins[k];
            fprintf(f, "%d %d %d %d\n", p.penID, p.penguinCoords.X, p.penguinCoords.Y, p.can_make_any_move);
        }
    }

    fclose(f);
    return 1;
}

int load_game(const char* filename, struct GameState* gs, struct players* pl) {
    if (!filename || !gs || !pl) return 0;

    FILE* f = fopen(filename, "r");
    if (!f) return 0;

    char magic[32] = { 0 };
    int version = 0;
    if (fscanf(f, "%31s %d", magic, &version) != 2 || strcmp(magic, "PENGUIN_SAVE") != 0) {
        fclose(f);
        return 0;
    }

    int rows = 0, cols = 0;
    if (fscanf(f, "%d %d", &rows, &cols) != 2 || rows <= 0 || cols <= 0) {
        fclose(f);
        return 0;
    }

    if (gs->board.arr) {
        delete_board(gs->board);
        gs->board.arr = NULL;
    }
    int** arr = malloc_2d_int(rows, cols);
    if (!arr) { fclose(f); return 0; }

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (fscanf(f, "%d", &arr[r][c]) != 1) { fclose(f); return 0; }
        }
    }
    gs->board.arr = arr;
    gs->board.rows = rows;
    gs->board.cols = cols;

    if (fscanf(f, "%d %d", &gs->hasPlacementPhaseEnded, &gs->placingPenguinsOnlyOnOne) != 2) {
        fclose(f); return 0;
    }

    if (fscanf(f, "%d %d %d", &pl->currentPlayer, &pl->maxNumOfPenForPlayer, &pl->someoneCantMove) != 3) {
        fclose(f); return 0;
    }
    if (pl->maxNumOfPenForPlayer < 0 || pl->maxNumOfPenForPlayer > 3) {
        fclose(f); return 0;
    }

    for (int i = 0; i < 2; ++i) {
        if (fscanf(f, " \"%24[^\"]\"", pl->playr[i].name) != 1) { fclose(f); return 0; }
        int ch; while ((ch = fgetc(f)) != '\n' && ch != EOF) {}

        if (fscanf(f, "%d %d %d",
            &pl->playr[i].score,
            &pl->playr[i].numOfPenguinsCheckedOut,
            &pl->playr[i].numberOfPlacedPenguins) != 3) {
            fclose(f); return 0;
        }

        for (int k = 0; k < pl->maxNumOfPenForPlayer; ++k) {
            struct Penguin* p = &pl->playr[i].penguins[k];
            if (fscanf(f, "%d %d %d %d",
                &p->penID, &p->penguinCoords.X, &p->penguinCoords.Y, &p->can_make_any_move) != 4) {
                fclose(f); return 0;
            }
        }
        for (int k = pl->maxNumOfPenForPlayer; k < 3; ++k) {
            pl->playr[i].penguins[k].penID = k;
            pl->playr[i].penguins[k].penguinCoords.X = 0;
            pl->playr[i].penguins[k].penguinCoords.Y = 0;
            pl->playr[i].penguins[k].can_make_any_move = 0;
        }
    }

    fclose(f);
    return 1;
}

