#pragma once
#ifndef CHECKS_H
#define CHECKS_H

#include "board.h"
#include "player.h"
/*Checks if given coordinates are valid and can be used
 * currentCoords - variable of type CoordXY, representing current coordinates
 * newCoords - variable of type CoordXY, representing new coordinates
 * gamestate - pointer, representing of global structure Gamestate
 * Returns true 1 if coordinates are valid, 0 if  not*/
int coordinates_are_valid(CoordXY currentCoords, CoordXY newCoords, struct GameState* gameState);

/*Checks if every player has every penguin placed| Returns 1 if true, 0 if false*/
int are_all_penguins_of_players_placed();
/*Check if current number of penguins doesn't exceed the maximum number| Returns 1 if can, 0 if not*/
int can_current_player_place_another_penguin();

/*Checks if a penguin can make at least one move
 * penID - integer, representing the ID of the penguin in penguins hierarchy (dependent on number of penguins)
 * player - pointer, representing of global structure players
 * gamestate - pointer, representing of global structure Gamestate*/
int can_penguin_make_any_move(int penID, struct players* player, struct GameState* gameState);

#endif
