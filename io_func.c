#define _CRT_SECURE_NO_WARNINGS

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "function.h"
#include "penguin.h"
#include "checks.h"
#include "player.h"

static int ask_choice_in_range(const char* prompt, int minValue, int maxValue)
{
    char line[32];
    char* end = NULL;

    while (1) {
        if (prompt) {
            printf("%s", prompt);
        }

        if (!fgets(line, sizeof(line), stdin)) {
            continue;
        }
        line[strcspn(line, "\n")] = '\0';

        long value = strtol(line, &end, 10);
        if (end == line) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        while (isspace((unsigned char)*end)) {
            ++end;
        }
        if (*end != '\0') {
            printf("Invalid input. Please enter a number only.\n");
            continue;
        }

        if (value < minValue || value > maxValue) {
            printf("Please select a value between %d and %d.\n", minValue, maxValue);
            continue;
        }

        return (int)value;
    }
}

static int parse_two_ints(const char* line, long* x, long* y)
{
    char* end = NULL;

    if (!line || !x || !y) {
        return 0;
    }

    *x = strtol(line, &end, 10);
    if (end == line) {
        return 0;
    }

    char* second_start = end;
    *y = strtol(second_start, &end, 10);
    if (end == second_start) {
        return 0;
    }

    while (isspace((unsigned char)*end)) {
        ++end;
    }

    return *end == '\0';
}

GameMode ask_game_mode(void)
{
    printf("Select game mode:\n");
    printf("1 - Human vs Human\n");
    printf("2 - Human vs AI\n");
    return ask_choice_in_range("Mode: ", 1, 2) == 1
        ? MODE_HUMAN_VS_HUMAN
        : MODE_HUMAN_VS_AI;
}

AIDifficulty ask_ai_difficulty(void)
{
    printf("Select AI difficulty:\n");
    printf("1 - Easy (random)\n");
    printf("2 - Medium (greedy)\n");
    printf("3 - Hard (minimax)\n");

    int choice = ask_choice_in_range("Difficulty: ", 1, 3);
    if (choice == 1) return AI_EASY;
    if (choice == 2) return AI_MEDIUM;
    return AI_HARD;
}

void askAboutOptionsOfPlacing(struct GameState* gamestate)
{
    int choiceCollected = 0;

    printf("Please choose if placing penguins should be only on tile with 1 fish\n");
    printf("2 - yes\n1 - no\n");

    while (!choiceCollected)
    {
        char choice[20];
        char* end;
        long l_choice;

        fgets(choice, 20, stdin);
        l_choice = strtol(choice, &end, 0);

        if (l_choice == 1 || l_choice == 2)
        {
            gamestate->placingPenguinsOnlyOnOne = (int)(l_choice - 1);
            choiceCollected = 1;
        }
    }
}

void askPlayerForAName(struct players* player)
{
    char name[25];

    for (int i = 0; i < 2; ++i) {
        if (player->playr[i].type == PLAYER_AI) {
            snprintf(player->playr[i].name, sizeof(player->playr[i].name), "CPU-%d", i + 1);
            printf("Player %d is controlled by AI (%s).\n", i + 1, player->playr[i].name);
            continue;
        }

        printf("Please type in the name of player %d\n", i + 1);
        if (!fgets(name, 25, stdin)) { name[0] = '\0'; }
        if (strlen(name) > 0 && name[strlen(name) - 1] == '\n') name[strlen(name) - 1] = '\0';
        if (strlen(name) == 0) {
            snprintf(name, sizeof(name), "Player-%d", i + 1);
        }
        strcpy(player->playr[i].name, name);
    }
}

CoordXY read_board_dimensions(void)
{
    int boardDimensionsCollected = 0;
    CoordXY input_gl = functionReturnsTwoInts();

    while (!boardDimensionsCollected)
    {
        char d[20];
        printf("Please type in the board dimensions as two digits: (rows columns) respectively\n");
        if (!fgets(d, 20, stdin)) continue;
        d[strcspn(d, "\n")] = '\0';

        long int x, y;
        if (!parse_two_ints(d, &x, &y)) {
            printf("Please provide exactly two integers, e.g. 8 8\n");
            continue;
        }

        if (x >= 3 && y >= 3) {
            input_gl.X = (int)x;
            input_gl.Y = (int)y;
            boardDimensionsCollected = 1;
        }
        else {
            printf("The dimensions for the board must be integers starting from 3\n");
        }
    }

    return input_gl;
}

CoordXY ask_for_coordinates(struct players* playersLoc, int choosingAPenguin)
{
    int coordsCollected = 0;
    CoordXY input_gl = functionReturnsTwoInts();

    while (!coordsCollected)
    {
        char d[20];
        printf("Current player is: %s(%d)\n", playersLoc->playr[playersLoc->currentPlayer - 1].name, playersLoc->currentPlayer);

        if (choosingAPenguin)
            printf("Please insert coordinates of which penguin you want to move:\n");
        else
            printf("Please insert coordinates to place a penguin: (rows columns) respectively\n");

        if (!fgets(d, 20, stdin)) continue;
        d[strcspn(d, "\n")] = '\0';

        long int x, y;
        if (!parse_two_ints(d, &x, &y)) {
            printf("Please provide exactly two integers, e.g. 2 3\n");
            continue;
        }

        if (x >= 1 && y >= 1) {
            input_gl.X = (int)x - 1;
            input_gl.Y = (int)y - 1;
            coordsCollected = 1;
        }
        else {
            printf("The allowed coordinates for the board are integers starting from 1\n");
        }
    }

    return input_gl;
}

// io_func.c
int choose_penguin(struct players* player)
{
    CoordXY wishedPengCoords = ask_for_coordinates(player, 1);
    int cur = player->currentPlayer - 1;
    int max = player->playr[cur].numberOfPlacedPenguins;

    if (max <= 0) {
        printf("You don't have any placed penguins yet.\n");
        return -1;
    }

    for (int i = 0; i < max; ++i) {
        CoordXY p = player->playr[cur].penguins[i].penguinCoords;
        if (wishedPengCoords.X == p.X && wishedPengCoords.Y == p.Y) {
            return i;
        }
    }
    printf("Chosen coordinates are not correct\n");
    return -1;
}


void displayPlayersScore(struct players* game)
{
    printf("\tGame Over!\n");
    for (int i = 0; i < 2; ++i) {
        printf("\tPlayer %d score: %d\n", i + 1, game->playr[i].score);
    }
}

