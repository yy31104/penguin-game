#pragma once
#ifndef IO_FUNC_H
#define IO_FUNC_H
#include "function.h"
#include "player.h"

/*Ask the player for board dimensions
 * Returns variable of type CoordXY*/
CoordXY read_board_dimensions();
/*An i/o void used to get the coordinates from the user|
 * playersLoc - pointer, representing of global structure players
 * choosingAPenguin - integer, responsible for the choice of showing appropriate text*/
CoordXY ask_for_coordinates(struct players* playersLoc, int choosingAPenguin);
/*Make the UI show the winner and display players scores*/
void display_winner();
/*Asks the player to select the penguin during the game|
 * player - pointer, representing of global structure players
 * Returns the ID of the penguin (from 0 to max number of penguins)*/
int choose_penguin(struct players* player);
/*A function used to display the scores of players and the winner at the end of the game
 * game - pointer, representing of global structure players*/
void displayPlayersScore(struct players* game);
/*A function used to ask the players insert their names into the program
 * player - pointer, representing of global structure players*/
void askPlayerForAName(struct players* player);
/*A function used to ask player if penguins should be placed only on tile with 1 fish
 * gamestate - pointer, representing of global structure Gamestate*/
void askAboutOptionsOfPlacing(struct GameState* gamestate);
#endif
