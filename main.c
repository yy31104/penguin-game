#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "placement_phase.h"
#include "movement_phase.h"
#include "function.h"
#include "player.h"
#include "penguin.h"
#include "board.h"
#include "save_load.h"

static void zero_all(struct players* pl, struct GameState* gs) {
    memset(pl, 0, sizeof * pl);
    memset(gs, 0, sizeof * gs);
}

static int read_menu_choice(void)
{
    char line[32];
    char* end = NULL;

    if (!fgets(line, sizeof(line), stdin)) {
        return -1;
    }

    long value = strtol(line, &end, 10);
    if (end == line) {
        return -1;
    }

    while (*end == ' ' || *end == '\t') {
        ++end;
    }
    if (*end != '\n' && *end != '\0') {
        return -1;
    }

    return (int)value;
}

int main(void)
{
    struct Penguin penguin = { 0 };
    struct players player = { 0 };
    struct GameState gs = { 0 };
    zero_all(&player, &gs);
    srand((unsigned int)time(NULL));

    for (;;) {
        int choice;
        printf("\n=== Penguin Game ===\n");
        printf("1) New game\n");
        printf("2) Load and continue (save.txt)\n");
        printf("3) Exit\n");
        printf("Choose: ");
        choice = read_menu_choice();

        if (choice == 1) {
            zero_all(&player, &gs);
            placement_phase(&penguin, &player, &gs);
            movement_phase(&penguin, &player, &gs);
        }
        else if (choice == 2) {
            if (load_game("save.txt", &gs, &player)) {
                printf("Loaded save.txt.\n");
                if (gs.hasPlacementPhaseEnded) {
                    movement_phase(&penguin, &player, &gs);
                }
                else {
                    placement_phase(&penguin, &player, &gs);
                    if (gs.hasPlacementPhaseEnded) {
                        movement_phase(&penguin, &player, &gs);
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
        else {
            printf("Invalid choice. Please select 1, 2, or 3.\n");
        }
    }

    board_destroy(&gs.board);
    return EXIT_SUCCESS;
}

