#define _CRT_SECURE_NO_WARNINGS
#include "function.h"
#include "checks.h"
#include "penguin.h"
#include "io_func.h"
#include "board.h"
#include "save_load.h"

int CheckOutPenguins(int penID, struct players* player)
{
    int curPlayer = player->currentPlayer - 1;

    //if (player->playr[curPlayer].penguins[penID].can_make_any_move != 0)
    player->playr[curPlayer].numOfPenguinsCheckedOut++;

    player->playr[curPlayer].penguins[penID].can_make_any_move = 0;

    if (player->playr[curPlayer].numOfPenguinsCheckedOut > player->maxNumOfPenForPlayer - 1)
    {
        displayPlayersScore(player); ///Game ends
        return 1;
    }
    return 0;
}

void movement_phase(struct Penguin* penguin, struct players* player, struct GameState* gameState)
{
    /*if that penguin cant make a move then ++*/
    while (!player->someoneCantMove)
    {
        int conditionToStop = 0;

        for (int i = 0; i < player->maxNumOfPenForPlayer; ++i) {
            if (!can_penguin_make_any_move(i, player, gameState))
            {
                conditionToStop = CheckOutPenguins(i, player); /*If can't move, then check out*/
            }
            if (conditionToStop == 1)
            {
                return;
            }
        }


        // if(conditionToStop == 1)
             ///If penguins can't move, while should stop playing


        show_board(gameState->board);
        int penID = choose_penguin(player);
        if (penID == -1)
        {
            continue;
        }

        show_board(gameState->board);

        if (!(can_penguin_make_any_move(penID, player, gameState)))
        {
            printf("This penguin can't move in any way\n");
            continue;
        }

        CoordXY coordXY = ask_for_coordinates(player, 0);

        if (coordinates_are_valid(player->playr[player->currentPlayer - 1].penguins[penID].penguinCoords, coordXY, gameState))/// THE BUG IS IN LAST VARIABLE
        {
            move_penguin((player->currentPlayer), gameState, penID, coordXY, player);

            save_game("save.txt", gameState, player);   // ★ 每次成功走子后自动保存
            printf("[autosaved]\n");

            change_current_player(player);
        }
        else
        {
            printf("The given coordinates are incorrect. Please type different ones\n");
        }
    }
}