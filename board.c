#define _CRT_SECURE_NO_WARNINGS
#include "board.h"
#include "penguin.h"
#include "player.h"
#include "function.h"

Board generate_board(CoordXY rowsAndCols)
{
    struct GameState gameState;
    int rows, cols;
    srand((unsigned int)time(NULL));

    Board newBoard;

    newBoard.rows = rowsAndCols.X;
    newBoard.cols = rowsAndCols.Y;
    rows = newBoard.rows;
    cols = newBoard.cols;
    gameState.board.rows = rowsAndCols.X;
    gameState.board.cols = rowsAndCols.Y;


    gameState.board.arr = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++)
    {
        gameState.board.arr[i] = (int*)malloc(cols * sizeof(int));
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            gameState.board.arr[i][j] = rand() % 3 + 1;
        }
    }

    return gameState.board;
}

//free memory
void delete_board(Board board)
{
    if (!board.arr || board.rows <= 0 || board.cols <= 0) {
        return;
    }

    for (int i = 0; i < board.rows; i++)
    {
        if (board.arr[i] != NULL)
        {
            free(board.arr[i]);
        }
    }
    free(board.arr);
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
            case -1:
                printf("\tP1");
                break;
            case -2:
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

