#pragma once
// player.h
// player.h
#ifndef PLAYER_H
#define PLAYER_H
#include "function.h"
#include "board.h"

typedef enum {
    PLAYER_HUMAN = 0,
    PLAYER_AI = 1
} PlayerType;

typedef enum {
    MODE_HUMAN_VS_HUMAN = 0,
    MODE_HUMAN_VS_AI = 1
} GameMode;

typedef enum {
    AI_EASY = 0,
    AI_MEDIUM = 1,
    AI_HARD = 2
} AIDifficulty;


struct Penguin {
    int     penID;
    CoordXY penguinCoords;
    int     can_make_any_move;
};


struct Player {
    char           name[25];
    PlayerType     type;
    int            score;
    int            numOfPenguinsCheckedOut;
    int            numberOfPlacedPenguins;
    struct Penguin penguins[3];   
};


struct players {
    struct Player playr[2];
    int currentPlayer;
    int maxNumOfPenForPlayer;
    int someoneCantMove;
};


struct GameState {
    Board board;
    int hasPlacementPhaseEnded;
    int placingPenguinsOnlyOnOne;
    GameMode mode;
    AIDifficulty aiDifficulty;
};


void change_current_player(struct players* player);
void set_current_player(struct players* player);


#endif
