#pragma once
#ifndef PLACEMENT_PHASE_H
#define PLACEMENT_PHASE_H
#include <stdio.h>

#include "player.h"

/*Placement Phase. Invoked from the main.c
 * penguin - pointer, representing of global structure Penguin
 * player - pointer, representing of global structure players
 * gamestate - pointer, representing of global structure Gamestate*/
void placement_phase(struct Penguin* penguin, struct players* player, struct GameState* gameState);

/*Initialization of required data to remove the bugs*/
void InitializeData(struct players* player);
#endif