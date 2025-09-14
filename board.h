#pragma once
#ifndef BOARD_H
#define BOARD_H
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "function.h"

/*A structure meant to represent the board information
 * arr - a pointer to a pointer to board array
 * rows - integer, representing a number of rows
 * cols - integer, representing a number of columns*/
typedef struct
{
    int** arr;
    int rows;
    int cols;
} Board;

/*Function of type board, that generates the game board with specified dimensions
 * rowsAndCols - variable of type CoordXY, representing rows and columns of a board in a singular form*/
Board generate_board(CoordXY rowsAndCols);
/*Function that frees the memory and deletes the board
 * board - variable of type Board, representing a game board*/
void delete_board(Board board);
/*Function that displays the current state of the game board
 * board - variable of type Board, representing a game board*/
void show_board(Board board);

#endif