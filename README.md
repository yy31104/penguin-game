# 🐧 Penguin Game

A lightweight two-player strategy game for the terminal, inspired by *Hey, That's My Fish!*.
Players place and move penguins on a grid of ice tiles to gather fish and block opponents.

## Features
- Hot-seat play for two people with custom names
- Two sequential phases: placement then movement
- Board generated at runtime with 1–3 fish per tile
- Simple orthogonal movement
- Automatic saving after each valid action
- Portable C11 code (compiles with GCC/Clang/MSVC)

## Building
Any C11 compiler works; no external libraries are required.

**Windows (MinGW)**
```
gcc -std=c11 -O2 -Wall -Wextra -o penguin.exe *.c
```

**Windows (MSVC)**
```
cl /W4 /O2 /Fe:penguin.exe *.c
```

**Linux / macOS**
```
gcc -std=c11 -O2 -Wall -Wextra -o penguin *.c
```

Run the game:
```
./penguin       # Unix-like
penguin.exe     # Windows
```

## Gameplay
1. **Main Menu** – start new game, load save, or exit.
2. **Options** – choose whether placement is limited to one-fish tiles (2 = yes, 1 = no).
3. **Setup** – supply player names and board size (rows columns, ≥3).
4. **Placement** – alternate placing two penguins each; collect fish from the tile.
5. **Movement** – select a penguin and move one tile up/down/left/right onto a non-hole tile; collect fish and leave a hole behind.
6. **End** – game finishes when a player has no legal moves; scores are displayed.

## Board Reference
- Input coordinates are 1-based (`row column`).
- Symbols:

| Symbol | Meaning |
|--------|---------|
| 01, 02, 03 | Fish count |
| P1, P2     | Player tiles |
| 0          | Hole (removed tile) |

## Save & Load
- Progress is stored in `save.txt` within the working directory.
- The game autosaves after every valid placement or move.
- Choose menu option 2 to continue from an existing save.

## Project Layout
```
main.c            – Menu handling and game setup
placement_phase.c – Placement rules and options
movement_phase.c  – Turn loop and win detection
board.c           – Board generation and rendering
checks.c          – Move and placement validation
io_func.c         – User input helpers
penguin.c         – Board updates and scoring
player.c          – Player state utilities
function.c        – Miscellaneous helpers
save_load.c       – Save/load system
```

## Customization
- Penguins per player (default 2) — change in `placement_phase.c`.
- Board size — chosen at runtime (minimum 3×3).
- Optional rule restricting placement to 1-fish tiles.

## Limitations
- Only orthogonal single-step movement.
- Rectangular grid (simplified vs. original board game).
- No computer opponents (local two-player only).

## Roadmap
- AI players with difficulty levels
- More penguins and varied fish distribution
- Multi-step movement, obstacles and power-ups
- GUI or web front end
- Multiple save slots and stronger validation

## License
[MIT](LICENSE.txt)
