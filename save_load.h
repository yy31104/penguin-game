#pragma once
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "board.h"
#include "player.h"


int save_game(const char* filename, const struct GameState* gs, const struct players* pl);

int load_game(const char* filename, struct GameState* gs, struct players* pl);

