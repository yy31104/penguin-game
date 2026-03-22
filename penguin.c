#define _CRT_SECURE_NO_WARNINGS
#include "function.h"
#include "player.h"
#include "board.h"


void set_penguin(int curPlayer, struct GameState* gameState, int curPeng, CoordXY coordXY, struct players* players_l)
{
    switch (curPlayer) {
    case 1:
        players_l->playr[0].score += gameState->board.arr[coordXY.X][coordXY.Y];
        gameState->board.arr[coordXY.X][coordXY.Y] = TILE_P1;
        players_l->playr[0].penguins[curPeng].penguinCoords.X = coordXY.X;
        players_l->playr[0].penguins[curPeng].penguinCoords.Y = coordXY.Y;
        players_l->playr[0].penguins[curPeng].can_make_any_move = 1;
        break;
    case 2:
        players_l->playr[1].score += gameState->board.arr[coordXY.X][coordXY.Y];
        gameState->board.arr[coordXY.X][coordXY.Y] = TILE_P2;
        players_l->playr[1].penguins[curPeng].penguinCoords.X = coordXY.X;
        players_l->playr[1].penguins[curPeng].penguinCoords.Y = coordXY.Y;
        players_l->playr[1].penguins[curPeng].can_make_any_move = 1;
        break;
    }
}

void move_penguin(int curPlayer, struct GameState* gameState, int curPeng, CoordXY coordXY, struct players* players_l)
{
    switch (curPlayer) {
    case 1:
        players_l->playr[0].score += gameState->board.arr[coordXY.X][coordXY.Y];
        gameState->board.arr[players_l->playr[0].penguins[curPeng].penguinCoords.X][players_l->playr[0].penguins[curPeng].penguinCoords.Y] = TILE_HOLE;
        players_l->playr[0].penguins[curPeng].penguinCoords.X = coordXY.X;
        players_l->playr[0].penguins[curPeng].penguinCoords.Y = coordXY.Y;
        gameState->board.arr[players_l->playr[0].penguins[curPeng].penguinCoords.X][players_l->playr[0].penguins[curPeng].penguinCoords.Y] = TILE_P1;
        break;
    case 2:
        players_l->playr[1].score += gameState->board.arr[coordXY.X][coordXY.Y];
        gameState->board.arr[players_l->playr[1].penguins[curPeng].penguinCoords.X][players_l->playr[1].penguins[curPeng].penguinCoords.Y] = TILE_HOLE;
        players_l->playr[1].penguins[curPeng].penguinCoords.X = coordXY.X;
        players_l->playr[1].penguins[curPeng].penguinCoords.Y = coordXY.Y;
        gameState->board.arr[players_l->playr[1].penguins[curPeng].penguinCoords.X][players_l->playr[1].penguins[curPeng].penguinCoords.Y] = TILE_P2;
        break;
    }
}
