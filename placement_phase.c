#define _CRT_SECURE_NO_WARNINGS
#include "function.h"
#include "board.h"
#include "io_func.h"
#include "save_load.h"
#include "ai.h"

#include "penguin.h"
#include "checks.h"

static int total_placed_penguins(const struct players* player)
{
    return player->playr[0].numberOfPlacedPenguins + player->playr[1].numberOfPlacedPenguins;
}

static int count_available_placement_tiles(const struct GameState* gameState)
{
    int count = 0;
    for (int r = 0; r < gameState->board.rows; ++r) {
        for (int c = 0; c < gameState->board.cols; ++c) {
            int tile = gameState->board.arr[r][c];

            if (gameState->placingPenguinsOnlyOnOne) {
                if (tile == MIN_FISH) {
                    ++count;
                }
            }
            else if (tile >= MIN_FISH && tile <= MAX_FISH) {
                ++count;
            }
        }
    }
    return count;
}

void InitializeData(struct players* player)
{
    player->someoneCantMove = 0;
    player->maxNumOfPenForPlayer = 2;

    for (int p = 0; p < 2; ++p) {
        player->playr[p].score = 0;
        player->playr[p].numOfPenguinsCheckedOut = 0;
        player->playr[p].numberOfPlacedPenguins = 0;
        player->playr[p].name[0] = '\0';
        player->playr[p].type = PLAYER_HUMAN;

        for (int i = 0; i < 3; ++i) {
            player->playr[p].penguins[i].penID = i;
            player->playr[p].penguins[i].penguinCoords.X = -1;
            player->playr[p].penguins[i].penguinCoords.Y = -1;
            player->playr[p].penguins[i].can_make_any_move = 1;
        }
    }
}

static int setup_new_game(struct players* player, struct GameState* gameState)
{
    CoordXY dimensions;

    InitializeData(player);
    set_current_player(player);
    gameState->hasPlacementPhaseEnded = 0;
    gameState->mode = ask_game_mode();
    gameState->aiDifficulty = AI_EASY;

    if (gameState->mode == MODE_HUMAN_VS_AI) {
        player->playr[0].type = PLAYER_HUMAN;
        player->playr[1].type = PLAYER_AI;
        gameState->aiDifficulty = ask_ai_difficulty();
    }

    askAboutOptionsOfPlacing(gameState);
    askPlayerForAName(player);

    dimensions = read_board_dimensions();

    board_destroy(&gameState->board);
    gameState->board = generate_board(dimensions);
    if (!gameState->board.arr) {
        printf("Failed to allocate board memory.\n");
        return 0;
    }

    show_board(gameState->board);
    return 1;
}

void placement_phase(struct Penguin* penguin, struct players* player, struct GameState* gameState)
{
    (void)penguin;

    int resuming = (gameState->board.arr != NULL) &&
        total_placed_penguins(player) > 0 &&
        !gameState->hasPlacementPhaseEnded;

    if (resuming) {
        printf("Resuming placement phase...\n");
        show_board(gameState->board);
    }
    else if (!setup_new_game(player, gameState)) {
        return;
    }

    int placements_done = total_placed_penguins(player);
    int placements_required = player->maxNumOfPenForPlayer * 2;

    while (placements_done < placements_required) {
        if (count_available_placement_tiles(gameState) == 0) {
            printf("No legal tiles left for placement. Ending placement phase early.\n");
            break;
        }

        int curPlayer = player->currentPlayer - 1;

        if (player->playr[curPlayer].numberOfPlacedPenguins >= player->maxNumOfPenForPlayer) {
            change_current_player(player);
            continue;
        }

        CoordXY coordinates;
        int next_penguin_slot = player->playr[curPlayer].numberOfPlacedPenguins;
        CoordXY current_coords = player->playr[curPlayer].penguins[next_penguin_slot].penguinCoords;

        if (player->playr[curPlayer].type == PLAYER_AI) {
            if (!ai_choose_placement(gameState, player->currentPlayer, gameState->aiDifficulty, &coordinates)) {
                printf("AI could not find a legal placement. Ending placement phase.\n");
                break;
            }
            printf("AI %s placed a penguin at: %d %d\n",
                player->playr[curPlayer].name, coordinates.X + 1, coordinates.Y + 1);
        }
        else {
            coordinates = ask_for_coordinates(player, 0);
        }

        if (coordinates_are_valid(current_coords, coordinates, gameState)) {
            set_penguin(player->currentPlayer, gameState, next_penguin_slot, coordinates, player);
            save_game("save.txt", gameState, player);
            printf("[autosaved]\n");
            show_board(gameState->board);

            player->playr[curPlayer].numberOfPlacedPenguins++;
            placements_done++;
            change_current_player(player);
        }
        else {
            printf("The given coordinates are incorrect. Please type different ones\n");
        }
    }

    gameState->hasPlacementPhaseEnded = 1;
    if (placements_done < placements_required) {
        printf("Not all penguins were placed because no legal placement tiles remain.\n");
    }
    printf("The placing phase has ended\n");
}
