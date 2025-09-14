#pragma once
// player.h
// player.h
#ifndef PLAYER_H
#define PLAYER_H
#include "function.h"
#include "board.h"

/* 单只企鹅 */
struct Penguin {
    int     penID;
    CoordXY penguinCoords;
    int     can_make_any_move;
};

/* 单个玩家 */
struct Player {
    char           name[25];
    int            score;
    int            numOfPenguinsCheckedOut;
    int            numberOfPlacedPenguins;
    struct Penguin penguins[3];   // 你项目里最多 3 只
};

/* 玩家集合 + 全局玩家状态 */
struct players {
    struct Player playr[2];
    int currentPlayer;
    int maxNumOfPenForPlayer;
    int someoneCantMove;
};

/* 游戏整体状态（含棋盘） */
struct GameState {
    Board board;
    int hasPlacementPhaseEnded;
    int placingPenguinsOnlyOnOne;
};

/* 其余函数声明 */
void change_current_player(struct players* player);
void set_current_player(struct players* player);

#endif