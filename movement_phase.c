#define _CRT_SECURE_NO_WARNINGS
#include "function.h"
#include "checks.h"
#include "penguin.h"
#include "io_func.h"
#include "board.h"
#include "save_load.h"
#include "ai.h"

int CheckOutPenguins(int penID, struct players* player)
{
    int curPlayer = player->currentPlayer - 1;
    int placedPenguins = player->playr[curPlayer].numberOfPlacedPenguins;

    if (penID < 0 || penID >= placedPenguins) {
        return 0;
    }

    if (player->playr[curPlayer].penguins[penID].can_make_any_move == 0) {
        return 0; // already checked out in a previous round
    }

    player->playr[curPlayer].penguins[penID].can_make_any_move = 0;
    player->playr[curPlayer].numOfPenguinsCheckedOut++;

    return player->playr[curPlayer].numOfPenguinsCheckedOut >= placedPenguins;
}

static void refresh_current_player_checkout_status(struct players* player, struct GameState* gameState)
{
    int curPlayer = player->currentPlayer - 1;
    int placedPenguins = player->playr[curPlayer].numberOfPlacedPenguins;

    for (int i = 0; i < placedPenguins; ++i) {
        if (!can_penguin_make_any_move(i, player, gameState)) {
            CheckOutPenguins(i, player);
        }
    }
}

static int player_has_any_legal_move(int playerId, struct players* player, struct GameState* gameState)
{
    int savedCurrentPlayer = player->currentPlayer;
    int playerIndex = playerId - 1;
    int placedPenguins = player->playr[playerIndex].numberOfPlacedPenguins;

    player->currentPlayer = playerId;
    for (int i = 0; i < placedPenguins; ++i) {
        if (can_penguin_make_any_move(i, player, gameState)) {
            player->currentPlayer = savedCurrentPlayer;
            return 1;
        }
    }

    player->currentPlayer = savedCurrentPlayer;
    return 0;
}

void movement_phase(struct Penguin* penguin, struct players* player, struct GameState* gameState)
{
    (void)penguin;

    while (1) {
        int currentPlayerId = player->currentPlayer;
        int otherPlayerId = currentPlayerId == 1 ? 2 : 1;

        refresh_current_player_checkout_status(player, gameState);

        int currentCanMove = player_has_any_legal_move(currentPlayerId, player, gameState);
        int otherCanMove = player_has_any_legal_move(otherPlayerId, player, gameState);

        if (!currentCanMove && !otherCanMove) {
            displayPlayersScore(player);
            return;
        }

        if (!currentCanMove) {
            printf("%s has no legal moves. Turn is skipped.\n",
                player->playr[currentPlayerId - 1].name);
            change_current_player(player);
            continue;
        }

        show_board(gameState->board);
        int penID = -1;
        CoordXY coordXY = { 0 };

        if (player->playr[currentPlayerId - 1].type == PLAYER_AI) {
            if (!ai_choose_move(player, gameState, currentPlayerId, gameState->aiDifficulty, &penID, &coordXY)) {
                printf("AI %s has no legal move. Turn is skipped.\n",
                    player->playr[currentPlayerId - 1].name);
                change_current_player(player);
                continue;
            }
            printf("AI %s moves penguin %d to: %d %d\n",
                player->playr[currentPlayerId - 1].name, penID + 1, coordXY.X + 1, coordXY.Y + 1);
        }
        else {
            penID = choose_penguin(player);
            if (penID == -1) {
                continue;
            }

            if (!can_penguin_make_any_move(penID, player, gameState)) {
                printf("This penguin can't move in any way\n");
                CheckOutPenguins(penID, player);
                continue;
            }

            coordXY = ask_for_coordinates(player, 0);
        }

        if (coordinates_are_valid(player->playr[player->currentPlayer - 1].penguins[penID].penguinCoords, coordXY, gameState)) {
            move_penguin(player->currentPlayer, gameState, penID, coordXY, player);

            change_current_player(player);
            save_game("save.txt", gameState, player);
            printf("[autosaved]\n");
        }
        else {
            printf("The given coordinates are incorrect. Please type different ones\n");
        }
    }
}
