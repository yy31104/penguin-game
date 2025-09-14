#define _CRT_SECURE_NO_WARNINGS
#include "function.h"
#include "board.h"
#include "io_func.h"
#include "save_load.h"

#include "penguin.h"
#include "checks.h"

void InitializeData(struct players* player)
{
    player->someoneCantMove = 0;
    player->playr[0].score = 0;
    player->playr[1].score = 0;
    player->maxNumOfPenForPlayer = 2;
    player->playr[0].numOfPenguinsCheckedOut = 0;
    player->playr[1].numOfPenguinsCheckedOut = 0;
    player->playr[0].numberOfPlacedPenguins = 0;
    player->playr[1].numberOfPlacedPenguins = 0;
}

void placement_phase(struct Penguin* penguin, struct players* player, struct GameState* gameState)
{
    int resuming = (gameState->board.arr != NULL) &&
        (player->playr[0].numberOfPlacedPenguins + player->playr[1].numberOfPlacedPenguins) > 0 &&
        !gameState->hasPlacementPhaseEnded;

    if (!resuming) {
        // 原来的 InitializeData(...)、问名字、问放置规则、
        // read_board_dimensions()、generate_board()、show_board() ...
    }
    else {
        printf("Resuming placement phase...\n");
        show_board(gameState->board);
    }


    CoordXY XY_values;

    InitializeData(player);
    set_current_player(player);
    gameState->hasPlacementPhaseEnded = 0;

    askAboutOptionsOfPlacing(gameState);
    askPlayerForAName(player);

    XY_values = read_board_dimensions();

    Board board = generate_board(XY_values);
    gameState->board.arr = board.arr;
    gameState->board.cols = board.cols;
    gameState->board.rows = board.rows;
    show_board(gameState->board);

    int amountOfTimesCurPlayerChanged = 0;
    int amountOfTimesCurPlayerMustBeChanged = player->maxNumOfPenForPlayer * 2;


    //main placement loop
    while (gameState->hasPlacementPhaseEnded != 1) //loop does not start with "player.hasPlacementPhaseEnded != 1" condition
    {
        while (amountOfTimesCurPlayerChanged != amountOfTimesCurPlayerMustBeChanged)
        {
            XY_values = ask_for_coordinates(player, 0);

            int curPlayer = player->currentPlayer - 1;

            if (coordinates_are_valid(player->playr[curPlayer].penguins[player->playr[curPlayer].numberOfPlacedPenguins].penguinCoords, XY_values, gameState))
            {
                set_penguin(player->currentPlayer, gameState, player->playr[curPlayer].numberOfPlacedPenguins, XY_values, player);
                save_game("save.txt", gameState, player);   // ★ 每次放置后自动保存（覆盖）
                printf("[autosaved]\n");
                show_board(gameState->board);
                player->playr[curPlayer].numberOfPlacedPenguins++;
                change_current_player(player);

                amountOfTimesCurPlayerChanged++;

            }
            else
            {
                printf("The given coordinates are incorrect. Please type different ones\n");
            }
        } /*If amountOfTimesCurPlayerChanged equals 2, then the cycle is completed meaning everything should've been put in place*/

        gameState->hasPlacementPhaseEnded = 1;
        printf("The placing phase has ended\n");
    }
}