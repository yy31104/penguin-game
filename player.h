#pragma once
// player.h
// player.h
#ifndef PLAYER_H
#define PLAYER_H
#include "function.h"
#include "board.h"

/* ��ֻ��� */
struct Penguin {
    int     penID;
    CoordXY penguinCoords;
    int     can_make_any_move;
};

/* ������� */
struct Player {
    char           name[25];
    int            score;
    int            numOfPenguinsCheckedOut;
    int            numberOfPlacedPenguins;
    struct Penguin penguins[3];   // ����Ŀ����� 3 ֻ
};

/* ��Ҽ��� + ȫ�����״̬ */
struct players {
    struct Player playr[2];
    int currentPlayer;
    int maxNumOfPenForPlayer;
    int someoneCantMove;
};

/* ��Ϸ����״̬�������̣� */
struct GameState {
    Board board;
    int hasPlacementPhaseEnded;
    int placingPenguinsOnlyOnOne;
};

/* ���ຯ������ */
void change_current_player(struct players* player);
void set_current_player(struct players* player);

#endif