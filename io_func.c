#define _CRT_SECURE_NO_WARNINGS

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "function.h"
#include "penguin.h"
#include "checks.h"
#include "player.h"

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
        printf("Please type in the name of player %d\n", i + 1);
        if (!fgets(name, 25, stdin)) { name[0] = '\0'; }
        if (strlen(name) > 0 && name[strlen(name) - 1] == '\n') name[strlen(name) - 1] = '\0';
        strcpy(player->playr[i].name, name);
    }
}

CoordXY read_board_dimensions()
{
    int boardDimensionsCollected = 0;
    CoordXY input_gl;

    while (!boardDimensionsCollected)
    {
        char d[20];
        char* end;
        CoordXY input = functionReturnsTwoInts();
        printf("Please type in the board dimensions as two digits: (rows columns) respectively\n");
        if (!fgets(d, 20, stdin)) continue;

        long int x, y;
        x = strtol(d, &end, 0);
        y = strtol(end, &end, 0);

        if (x != 0 && y != 0)
        {
            if (x < 0 || y < 0)
            {
                x = labs(x);
                y = labs(y);
            }
            if (x > 2 && y > 2)
            {
                input.X = (int)x;
                input.Y = (int)y;
                input_gl = input;
                boardDimensionsCollected = 1;
            }
            else
                printf("Please type in dimensions bigger than 2\n");
        }
        else
            printf("The dimensions for the board are integers starting from 3\n");
    }

    return input_gl;
}

CoordXY ask_for_coordinates(struct players* playersLoc, int choosingAPenguin)
{
    int coordsCollected = 0;
    CoordXY input_gl;

    while (!coordsCollected)
    {
        char d[20];
        char* end;
        CoordXY input = functionReturnsTwoInts();
        printf("Current player is: %s(%d)\n", playersLoc->playr[playersLoc->currentPlayer - 1].name, playersLoc->currentPlayer);

        if (choosingAPenguin)
            printf("Please insert coordinates of which penguin you want to move:\n");
        else
            printf("Please insert coordinates to place a penguin: (rows columns) respectively\n");

        if (!fgets(d, 20, stdin)) continue;

        long int x, y;
        x = strtol(d, &end, 0);
        y = strtol(end, &end, 0);

        if (x != 0 && y != 0)
        {
            if (x < 0 || y < 0)
            {
                x = labs(x);
                y = labs(y);
            }
            input.X = (int)x - 1;
            input.Y = (int)y - 1;
            input_gl = input;
            coordsCollected = 1;
        }
        else
            printf("The allowed coordinates for the board are integers starting from 1\n");
    }

    return input_gl;
}

// io_func.c
int choose_penguin(struct players* player)
{
    CoordXY wishedPengCoords = ask_for_coordinates(player, 1);
    int cur = player->currentPlayer - 1;
    int max = player->maxNumOfPenForPlayer;

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

