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

#define TILE_HOLE 0
#define TILE_P1   (-1)
#define TILE_P2   (-2)
#define MIN_FISH  1
#define MAX_FISH  3

/*Allocates a board using one contiguous block for all cells.
 * rows - number of rows
 * cols - number of columns*/
Board create_board(int rows, int cols);

/*Function of type board, that generates the game board with specified dimensions
 * rowsAndCols - variable of type CoordXY, representing rows and columns of a board in a singular form*/
Board generate_board(CoordXY rowsAndCols);
/*Function that frees the memory and deletes the board
 * board - pointer to a board, representing a game board*/
void board_destroy(Board* board);
/*Function that displays the current state of the game board
 * board - variable of type Board, representing a game board*/
void show_board(Board board);

#endif
