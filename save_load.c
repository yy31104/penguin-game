#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "save_load.h"

static int commit_temp_save(const char* tmp_name, const char* final_name) {
    if (rename(tmp_name, final_name) == 0) {
        return 1;
    }

    remove(final_name);
    if (rename(tmp_name, final_name) == 0) {
        return 1;
    }

    remove(tmp_name);
    return 0;
}

int save_game(const char* filename, const struct GameState* gs, const struct players* pl) {
    if (!filename || !gs || !pl || !gs->board.arr) return 0;

    char tmp_name[512] = { 0 };
    if (snprintf(tmp_name, sizeof(tmp_name), "%s.tmp", filename) <= 0) {
        return 0;
    }

    FILE* f = fopen(tmp_name, "w");
    if (!f) return 0;

    fprintf(f, "PENGUIN_SAVE 2\n");
    fprintf(f, "%d %d\n", gs->board.rows, gs->board.cols);

    for (int r = 0; r < gs->board.rows; ++r) {
        for (int c = 0; c < gs->board.cols; ++c) {
            fprintf(f, "%d%c", gs->board.arr[r][c], (c + 1 == gs->board.cols) ? '\n' : ' ');
        }
    }

    fprintf(f, "%d %d %d %d\n",
        gs->hasPlacementPhaseEnded,
        gs->placingPenguinsOnlyOnOne,
        gs->mode,
        gs->aiDifficulty);

    fprintf(f, "%d %d %d\n", pl->currentPlayer, pl->maxNumOfPenForPlayer, pl->someoneCantMove);

    for (int i = 0; i < 2; ++i) {
        fprintf(f, "\"%s\"\n", pl->playr[i].name);
        fprintf(f, "%d %d %d %d\n",
            pl->playr[i].type,
            pl->playr[i].score,
            pl->playr[i].numOfPenguinsCheckedOut,
            pl->playr[i].numberOfPlacedPenguins);

        for (int k = 0; k < pl->maxNumOfPenForPlayer; ++k) {
            struct Penguin p = pl->playr[i].penguins[k];
            fprintf(f, "%d %d %d %d\n", p.penID, p.penguinCoords.X, p.penguinCoords.Y, p.can_make_any_move);
        }
    }

    if (fclose(f) != 0) {
        remove(tmp_name);
        return 0;
    }

    return commit_temp_save(tmp_name, filename);
}

int load_game(const char* filename, struct GameState* gs, struct players* pl) {
    if (!filename || !gs || !pl) return 0;

    FILE* f = fopen(filename, "r");
    if (!f) return 0;

    int ok = 0;
    struct GameState loaded_gs = { 0 };
    struct players loaded_pl = *pl;

    char magic[32] = { 0 };
    int version = 0;
    if (fscanf(f, "%31s %d", magic, &version) != 2 || strcmp(magic, "PENGUIN_SAVE") != 0) {
        goto cleanup;
    }

    int rows = 0, cols = 0;
    if (fscanf(f, "%d %d", &rows, &cols) != 2 || rows <= 0 || cols <= 0) {
        goto cleanup;
    }

    loaded_gs.board = create_board(rows, cols);
    if (!loaded_gs.board.arr) {
        goto cleanup;
    }

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (fscanf(f, "%d", &loaded_gs.board.arr[r][c]) != 1) {
                goto cleanup;
            }
        }
    }

    if (version >= 2) {
        int mode = 0;
        int difficulty = 0;
        if (fscanf(f, "%d %d %d %d",
            &loaded_gs.hasPlacementPhaseEnded,
            &loaded_gs.placingPenguinsOnlyOnOne,
            &mode,
            &difficulty) != 4) {
            goto cleanup;
        }
        if (mode < MODE_HUMAN_VS_HUMAN || mode > MODE_HUMAN_VS_AI) {
            goto cleanup;
        }
        if (difficulty < AI_EASY || difficulty > AI_HARD) {
            goto cleanup;
        }
        loaded_gs.mode = (GameMode)mode;
        loaded_gs.aiDifficulty = (AIDifficulty)difficulty;
    }
    else {
        if (fscanf(f, "%d %d", &loaded_gs.hasPlacementPhaseEnded, &loaded_gs.placingPenguinsOnlyOnOne) != 2) {
            goto cleanup;
        }
        loaded_gs.mode = MODE_HUMAN_VS_HUMAN;
        loaded_gs.aiDifficulty = AI_EASY;
    }

    if (fscanf(f, "%d %d %d", &loaded_pl.currentPlayer, &loaded_pl.maxNumOfPenForPlayer, &loaded_pl.someoneCantMove) != 3) {
        goto cleanup;
    }
    if (loaded_pl.maxNumOfPenForPlayer < 1 || loaded_pl.maxNumOfPenForPlayer > 3) {
        goto cleanup;
    }
    if (loaded_pl.currentPlayer < 1 || loaded_pl.currentPlayer > 2) {
        goto cleanup;
    }

    for (int i = 0; i < 2; ++i) {
        if (fscanf(f, " \"%24[^\"]\"", loaded_pl.playr[i].name) != 1) {
            goto cleanup;
        }
        int ch; while ((ch = fgetc(f)) != '\n' && ch != EOF) {}

        if (version >= 2) {
            int type = 0;
            if (fscanf(f, "%d %d %d %d",
                &type,
                &loaded_pl.playr[i].score,
                &loaded_pl.playr[i].numOfPenguinsCheckedOut,
                &loaded_pl.playr[i].numberOfPlacedPenguins) != 4) {
                goto cleanup;
            }
            if (type < PLAYER_HUMAN || type > PLAYER_AI) {
                goto cleanup;
            }
            loaded_pl.playr[i].type = (PlayerType)type;
        }
        else {
            if (fscanf(f, "%d %d %d",
                &loaded_pl.playr[i].score,
                &loaded_pl.playr[i].numOfPenguinsCheckedOut,
                &loaded_pl.playr[i].numberOfPlacedPenguins) != 3) {
                goto cleanup;
            }
            loaded_pl.playr[i].type = PLAYER_HUMAN;
        }

        for (int k = 0; k < loaded_pl.maxNumOfPenForPlayer; ++k) {
            struct Penguin* p = &loaded_pl.playr[i].penguins[k];
            if (fscanf(f, "%d %d %d %d",
                &p->penID, &p->penguinCoords.X, &p->penguinCoords.Y, &p->can_make_any_move) != 4) {
                goto cleanup;
            }
        }
        for (int k = loaded_pl.maxNumOfPenForPlayer; k < 3; ++k) {
            loaded_pl.playr[i].penguins[k].penID = k;
            loaded_pl.playr[i].penguins[k].penguinCoords.X = -1;
            loaded_pl.playr[i].penguins[k].penguinCoords.Y = -1;
            loaded_pl.playr[i].penguins[k].can_make_any_move = 0;
        }
    }

    if (gs->board.arr) {
        board_destroy(&gs->board);
    }
    *gs = loaded_gs;
    *pl = loaded_pl;
    ok = 1;

cleanup:
    if (!ok) {
        board_destroy(&loaded_gs.board);
    }
    fclose(f);
    return ok;
}

