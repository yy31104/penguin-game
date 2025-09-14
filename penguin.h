#pragma once
#ifndef PENGUIN_H
#define PENGUIN_H

#include "function.h"
#include "player.h"
#include "board.h"


/*Places a penguin on the board and collects fish at given coordinates.
 * player - current player, struct value(look player.h)
 * penguin - current coordinates of a penguin, struct value(look penguin.h)
 * coordXY - newly put coordinates, struct value(look functions.h)
 * board - an array representing the game board, struct value(look board.h)
 * */
void set_penguin(int curPlayer, struct GameState* gameState, int curPeng, CoordXY coordXY, struct players* players_l);

/*Moves a penguin to a new position on the board.
 * player - current player, struct value(look player.h)
 * penguin - current coordinates of a penguin, struct value(look penguin.h)
 * coordXY - newly put coordinates, struct value(look functions.h)
 * board - an array representing the game board, struct value(look board.h)*/
void move_penguin(int curPlayer, struct GameState* gameState, int curPeng, CoordXY coordXY, struct players* players_l);

#endif