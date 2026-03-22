#pragma once
#ifndef AI_H
#define AI_H

#include "player.h"

int ai_choose_placement(
    const struct GameState* gameState,
    int playerId,
    AIDifficulty difficulty,
    CoordXY* outPlacement);

int ai_choose_move(
    const struct players* player,
    const struct GameState* gameState,
    int playerId,
    AIDifficulty difficulty,
    int* outPenguinId,
    CoordXY* outDestination);

#endif
