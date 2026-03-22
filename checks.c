#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include "function.h"
#include "penguin.h"
#include "player.h"
#include "board.h"

static int is_walkable_tile(int tile)
{
    return tile >= MIN_FISH && tile <= MAX_FISH;
}

int coordinates_are_valid(CoordXY currentCoords, CoordXY newCoords, struct GameState* gameState)
{
    if (newCoords.X <= gameState->board.rows - 1 && newCoords.Y <= gameState->board.cols - 1
        && newCoords.X >= 0 && newCoords.Y >= 0)
    {
        int hiddenNumber = gameState->board.arr[newCoords.X][newCoords.Y];
        if (hiddenNumber == TILE_P2 || hiddenNumber == TILE_P1 || hiddenNumber == TILE_HOLE)
        {
            printf("There's a penguin in a way!\n");
            return 0;
        }

        if (gameState->placingPenguinsOnlyOnOne)
        {
            if (!gameState->hasPlacementPhaseEnded && hiddenNumber != MIN_FISH)
            {
                printf("Sorry, but you have chosen otherwise he-he\n");
                return 0;
            }
        }


        if (gameState->hasPlacementPhaseEnded == 1)
        {
            int DiffSum = find_sum_of_differences(newCoords, currentCoords);

            if (DiffSum >= 2)
            {
                printf("This move is not permitted. You cannot move diagonally\n");
                return 0;
            }
            else if (abs(newCoords.X - currentCoords.X) == 1 || abs(newCoords.Y - currentCoords.Y) == 1)
            {
                return 1;
            }
            else
            {
                printf("This move is not permitted\n");
                return 0;
            }
        }
        return 1;
    }

    printf("You can't go outside the board\n");
    return 0;
}


int can_penguin_make_any_move(int penID, struct players* player, struct GameState* gameState)
{
    int r, c;
    int rows, cols;
    int currentPlayerIndex;

    if (!player || !gameState || !gameState->board.arr) {
        return 0;
    }

    currentPlayerIndex = player->currentPlayer - 1;
    if (currentPlayerIndex < 0 || currentPlayerIndex > 1) {
        return 0;
    }

    if (penID < 0 || penID >= player->playr[currentPlayerIndex].numberOfPlacedPenguins) {
        return 0;
    }

    r = player->playr[currentPlayerIndex].penguins[penID].penguinCoords.X;
    c = player->playr[currentPlayerIndex].penguins[penID].penguinCoords.Y;

    rows = gameState->board.rows;
    cols = gameState->board.cols;

    if (r < 0 || c < 0 || r >= rows || c >= cols) {
        return 0;
    }

    if (currentPlayerIndex == 0 && gameState->board.arr[r][c] != TILE_P1) {
        return 0;
    }
    if (currentPlayerIndex == 1 && gameState->board.arr[r][c] != TILE_P2) {
        return 0;
    }

    if ((r + 1) < rows) {
        if (is_walkable_tile(gameState->board.arr[r + 1][c])) {

            return 1;
        }
    }
    if ((r - 1) >= 0) {
        if (is_walkable_tile(gameState->board.arr[r - 1][c])) {

            return 1;
        }
    }
    if ((c + 1) < cols) {
        if (is_walkable_tile(gameState->board.arr[r][c + 1])) {

            return 1;
        }
    }
    if ((c - 1) >= 0) {
        if (is_walkable_tile(gameState->board.arr[r][c - 1])) {

            return 1;
        }
    }

    return 0;
}

