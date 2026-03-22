#define _CRT_SECURE_NO_WARNINGS
#include "board.h"
#include "penguin.h"
#include "player.h"
#include "function.h"

Board create_board(int rows, int cols)
{
    Board board = { 0 };
    if (rows <= 0 || cols <= 0) {
        return board;
    }

    int** row_ptrs = (int**)malloc((size_t)rows * sizeof(int*));
    int* data = (int*)malloc((size_t)rows * (size_t)cols * sizeof(int));
    if (!row_ptrs || !data) {
        free(row_ptrs);
        free(data);
        return board;
    }

    for (int r = 0; r < rows; ++r) {
        row_ptrs[r] = data + ((size_t)r * (size_t)cols);
    }

    board.arr = row_ptrs;
    board.rows = rows;
    board.cols = cols;
    return board;
}

Board generate_board(CoordXY rowsAndCols)
{
    Board newBoard = create_board(rowsAndCols.X, rowsAndCols.Y);
    if (!newBoard.arr) {
        return newBoard;
    }

    for (int i = 0; i < newBoard.rows; i++) {
        for (int j = 0; j < newBoard.cols; j++) {
            newBoard.arr[i][j] = (rand() % MAX_FISH) + MIN_FISH;
        }
    }

    return newBoard;
}

// free memory
void board_destroy(Board* board)
{
    if (!board || !board->arr) {
        return;
    }

    free(board->arr[0]); // contiguous data block
    free(board->arr);
    board->arr = NULL;
    board->rows = 0;
    board->cols = 0;
}


void show_board(Board board)
{
    if (!board.arr || board.rows <= 0 || board.cols <= 0) {
        return;
    }

    if (board.arr == NULL) {
        printf("Board is NULL.\n");
        return;
    }

    printf("Board:\nXY\t");
    for (int i = 1; i < board.cols + 1; i++)
    {
        printf("%d\t", i);
    }
    printf("\n\n");
    for (int i = 0; i < board.rows; i++) {
        printf("%d", i + 1);
        for (int j = 0; j < board.cols; j++) {
            switch (board.arr[i][j])
            {
            case TILE_P1:
                printf("\tP1");
                break;
            case TILE_P2:
                printf("\tP2");
                break;
            default:
                printf("\t0%d ", board.arr[i][j]);
                break;
            }
        }
        printf("\n\n");
    }
}

