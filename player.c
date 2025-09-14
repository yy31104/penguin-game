#define _CRT_SECURE_NO_WARNINGS
#include "player.h"

void change_current_player(struct players* player)
{
    int ID = player->currentPlayer;

    if (ID == 1)
        player->currentPlayer = 2;
    else
        player->currentPlayer = 1;
}

void set_current_player(struct players* player)
{
    srand((unsigned int)time(NULL));

    player->currentPlayer = rand() % 2 + 1;
}