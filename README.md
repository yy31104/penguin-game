# Penguin Game

Terminal strategy game for 2 players (human or AI).  
Place penguins, collect fish, block routes, and finish with the highest score.

## Features

- Two phases: `placement` -> `movement`
- Game modes:
  - `Human vs Human`
  - `Human vs AI`
- AI difficulty:
  - `Easy` (random legal move)
  - `Medium` (greedy heuristic)
  - `Hard` (minimax + alpha-beta, shallow depth)
- Random first player at game start
- Autosave after every valid action
- Load and continue from `save.txt`
- Stronger input validation (`fgets + strtol`)
- Board memory safety improvements (`create_board` / `board_destroy`)

## Build and Run

### Windows (GCC / clang)

```powershell
cd C:\path\to\penguin-game
gcc -std=c11 -O2 -Wall -Wextra -pedantic -o penguin.exe *.c
.\penguin.exe
```

If you use LLVM/clang on Windows, the same command works.

### Linux / macOS

```bash
gcc -std=c11 -O2 -Wall -Wextra -pedantic -o penguin *.c
./penguin
```

## Gameplay Rules

### Main menu

1. New game
2. Load and continue (`save.txt`)
3. Exit

### Setup flow (new game)

1. Choose mode: `Human vs Human` or `Human vs AI`
2. If `Human vs AI`, choose difficulty:
   - `1 Easy`
   - `2 Medium`
   - `3 Hard (minimax)`
3. Choose placement option:
   - `2` = place only on tiles with 1 fish
   - `1` = place on any fish tile (1..3)
4. Enter board size: `rows cols` (both >= 3)
5. Enter human player names (AI gets auto name, e.g. `CPU-2`)

### Placement phase

- Players alternate placing penguins.
- Default penguins per player: `2`.
- Input is `row col` (1-based in UI).
- Fish on that tile is immediately added to player's score.
- If placement is restricted to 1-fish tiles and no legal 1-fish tiles remain:
  - placement phase ends early automatically
  - game proceeds to movement phase

### Movement phase

- Choose one of your penguins, then choose destination.
- Move exactly one tile orthogonally (up/down/left/right).
- No diagonal move, no out-of-board move, no moving onto hole `0`, and no moving onto another penguin.
- Old tile becomes hole `0`.
- New tile fish is added to score.

### End condition

- If current player has no legal move, turn is skipped to the other player.
- Game ends only when **both players** have no legal moves.

## Save / Load

- Autosave file: `save.txt`
- Save occurs after each valid placement/move.
- Save format currently writes header `PENGUIN_SAVE 2`.
- Loader supports old saves (`v1`) and new saves (`v2`).
- `v2` includes AI-related fields:
  - game mode
  - AI difficulty
  - player type (human/AI)

## Project Structure

```text
ai.c / ai.h            # AI move selection (random/greedy/minimax)
board.c / board.h      # Board allocation, generation, rendering
checks.c / checks.h    # Rules validation and mobility checks
function.c / function.h# Small utility helpers
io_func.c / io_func.h  # Input parsing and menu prompts
main.c                 # Main loop and menu routing
movement_phase.c / .h  # Movement phase turn logic
penguin.c / penguin.h  # Apply placement/move on board + score updates
placement_phase.c / .h # Placement phase logic
player.c / player.h    # Player/game state models and helpers
save_load.c / save_load.h # Save and load
```

## Notes

- Build artifacts are ignored via `.gitignore` (`*.exe`, `build/`, etc.).
- If `penguin.exe` is running, recompilation may fail with `Permission denied`.
  - Close the running process first.

## License

MIT (see `LICENSE.txt`)
