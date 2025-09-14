#pragma once
#ifndef MOVEMENT_PHASE_H
#define MOVEMENT_PHASE_H
#include "player.h"

/*Movement Phase. Invoked from main.c
 * penguin - pointer, representing of global structure Penguin
 * player - pointer, representing of global structure players
 * gamestate - pointer, representing of global structure Gamestate*/
void movement_phase(struct Penguin* penguin, struct players* player, struct GameState* gameState);

/*A void which counts how many penguins are unable to move
 * If all penguins of a player are checked out - Game ends
 * penID - integer, representing a number of a penguin in a penguin hierarchy
 * player - pointer, representing of global structure players*/
int CheckOutPenguins(int penID, struct players* player);

#endif
