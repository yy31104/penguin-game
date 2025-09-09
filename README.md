# penguin
Penguin Game (Terminal C)
A fast, two-player, hot-seat strategy game inspired by “Hey, That’s My Fish!”, implemented in standard C and playable in the terminal. Players place and move penguins on an ice grid, collecting fish and blocking each other until no legal moves remain.

Features
Two players: Local hot‑seat with player names.
Two phases: Placement then movement.
Procedural board: Random fish count (1–3) per tile.
Simple moves: One orthogonal step per turn (no diagonals).
Scoring: Gain fish on every placed/moved‑to tile.
Autosave: Writes save.txt after each successful action; resume anytime.
Portable C: Builds with GCC/Clang/MSVC on Windows, Linux, and macOS.
Build
Requirements: Any C11-compatible compiler (GCC, Clang, or MSVC). No external libraries.

# Windows (MinGW)
gcc -std=c11 -O2 -Wall -Wextra -o penguin.exe *.c

# Windows (MSVC Developer Command Prompt)
cl /W4 /O2 /Fe:penguin.exe *.c

# Linux / macOS
gcc -std=c11 -O2 -Wall -Wextra -o penguin *.c
Run the game:

# Unix-like
./penguin

# Windows
penguin.exe
How To Play
Main menu:
New game
Load and continue (save.txt)
Exit
Options:
Choose whether placement is restricted to tiles with exactly 1 fish.
Enter 2 for yes, 1 for no.
Setup:
Enter player names.
Enter board size as rows columns (both ≥ 3), e.g., 8 8.
Placement phase:
Players alternate placing penguins until each has 2.
Enter coordinates as row column (1‑based).
On placement, you immediately collect that tile’s fish.
Movement phase:
Select which penguin to move by entering its current coordinates.
Move exactly one tile up/down/left/right (no diagonals, no jumps).
You cannot move onto holes (0), occupied tiles (P1/P2), or off the board.
Moving onto a tile collects its fish; the tile you leave becomes a hole (0).
End:
When a player has no legal move for any penguin, the game ends and scores are shown.
Board & UI
Coordinates: 1‑based input (row column).
Board legend:
01, 02, 03: fish count on the tile
P1, P2: tiles occupied by players
0: hole (a vacated tile)
Save/Load
File: save.txt in the working directory.
Autosave: After each valid placement or move, [autosaved] appears and save.txt is updated.
Resume: Choose “2) Load and continue” to resume if save.txt exists and is valid.
Rules Summary
Placement: Alternate placing until both players have 2 penguins; optional rule to allow placement only on 1‑fish tiles.
Movement: Exactly one orthogonal step per turn; no diagonals; cannot move onto holes or occupied tiles.
Scoring: Gain fish equal to the tile value each time you place or move onto a tile.
End game: When a player cannot move any penguin, final scores are displayed.
Project Structure
main.c — Menu, new/load flow, phase orchestration.
placement_phase.c / placement_phase.h — Placement logic, options, initialization.
movement_phase.c / movement_phase.h — Turn loop, penguin selection, movement, end conditions.
board.c / board.h — Board generation (random 1–3 fish), rendering, memory cleanup.
checks.c / checks.h — Validation for placement/moves, mobility checks.
io_func.c / io_func.h — Prompts, input parsing, scoreboard display.
penguin.c / penguin.h — Apply placement/movement effects (scores, board updates).
player.c / player.h — Current player utilities and state.
function.c / function.h — Helpers (coordinate struct, difference sum).
save_load.c / save_load.h — Text-based save/load (PENGUIN_SAVE 1 format).
Customization
Penguins per player: Default is 2. Change in placement_phase.c inside InitializeData by editing player->maxNumOfPenForPlayer.
Board size: Any rows x cols ≥ 3, selected at runtime.
Placement rule: Toggle “place only on 1‑fish tiles” at game start.
Limitations
Movement is a single orthogonal step (no sliding/multi‑tile moves).
Rectangular grid (simplified vs. the original board game).
No AI; local two‑player only.
Roadmap Ideas
AI opponents and difficulty levels.
Variable penguin counts and fish distributions.
Sliding/multi‑step movement, obstacles, or power‑ups.
GUI (SDL/ImGui) or web UI front‑end.
Multiple save slots and stronger save validation.
