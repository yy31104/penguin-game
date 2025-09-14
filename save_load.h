#pragma once
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "board.h"
#include "player.h"

/* 把整个游戏进度写到文件（棋盘、玩家、分数、状态） */
int save_game(const char* filename, const struct GameState* gs, const struct players* pl);

/* 从文件读取整个游戏进度（会按文件内容重新分配并填充棋盘） */
int load_game(const char* filename, struct GameState* gs, struct players* pl);
