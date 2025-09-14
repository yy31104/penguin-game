#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "placement_phase.h"
#include "movement_phase.h"
#include "function.h"
#include "player.h"
#include "penguin.h"
#include "save_load.h"

static void zero_all(struct players* pl, struct GameState* gs) {
    memset(pl, 0, sizeof * pl);
    memset(gs, 0, sizeof * gs);
}

int main(void)
{
    struct Penguin* penguin = (struct Penguin*)malloc(sizeof * penguin);
    struct players* player = (struct players*)malloc(sizeof * player);
    struct GameState* gs = (struct GameState*)malloc(sizeof * gs);
    zero_all(player, gs);

    for (;;) {
        int choice = 0;
        printf("\n=== Penguin Game ===\n");
        printf("1) New game\n");
        printf("2) Load and continue (save.txt)\n");
        printf("3) Exit\n");
        printf("Choose: ");
        if (scanf("%d", &choice) != 1) return 0;

        int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}

        if (choice == 1) {
            zero_all(player, gs);
            placement_phase(penguin, player, gs);
            movement_phase(penguin, player, gs);
        }
        else if (choice == 2) {
            if (load_game("save.txt", gs, player)) {
                printf("Loaded save.txt.\n");
                if (gs->hasPlacementPhaseEnded) {
                    movement_phase(penguin, player, gs);
                }
                else {
                    placement_phase(penguin, player, gs);
                    if (gs->hasPlacementPhaseEnded) {
                        movement_phase(penguin, player, gs);
                    }
                }
            }
            else {
                printf("No valid save.txt. Start a new game instead.\n");
            }
        }
        else if (choice == 3) {
            break;
        }
    }

    if (gs->board.arr) delete_board(gs->board);
    free(penguin); free(player); free(gs);
    return EXIT_SUCCESS;
}

