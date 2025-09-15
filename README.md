# 🐧 Penguin Game

> A fast, two‑player, hot‑seat strategy game for the terminal. Place and move penguins on an ice grid, collect fish, and outmaneuver your opponent.

<p align="center">
</p>

---

## Table of Contents

* [Features](#features)
* [Quick Start](#quick-start)
* [Build Options](#build-options)
* [How to Play](#how-to-play)
* [Rules Summary](#rules-summary)
* [Save & Resume](#save--resume)
* [Project Structure](#project-structure)
* [Roadmap](#roadmap)
* [Contributing](#contributing)
* [License](#license)
* [Acknowledgements](#acknowledgements)

---

## Features

* 🎮 **Local 2‑player hot‑seat** with player names
* 🔄 **Two phases:** placement → movement
* 🎲 **Procedural board:** tiles contain **1–3** fish
* ➡️ **Simple moves:** one orthogonal step per turn (no diagonals)
* 🐟 **Scoring:** collect fish from every tile you place/move onto
* 💾 **Autosave:** a `save.txt` is written after every valid action
* 📦 **Portable:** standard C11, builds with GCC/Clang/MSVC on Windows, Linux, and macOS

---

## Quick Start

### Windows (MSVC)

```bat
cl /std:c11 /W4 /O2 /Fe:penguin.exe *.c
penguin.exe
```

### Windows (MinGW)

```bat
gcc -std=c11 -O2 -Wall -Wextra -o penguin.exe *.c
penguin.exe
```

### Linux / macOS

```bash
gcc -std=c11 -O2 -Wall -Wextra -o penguin *.c
./penguin
```

> No external libraries required.

---

## Build Options

### Option A — Visual Studio

Open the project in **Visual Studio 2022+**, build in `Release|x64`, and run. If you prefer a clean, cross‑platform setup, consider adding the CMake file below.

### Option B — CMake (cross‑platform)

Add a `CMakeLists.txt` at the repository root (adjust the source list if needed):

```cmake
cmake_minimum_required(VERSION 3.15)
project(penguin_game C)
set(CMAKE_C_STANDARD 11)

add_executable(penguin
  board.c
  checks.c
  function.c
  io_func.c
  main.c
  movement_phase.c
  penguin.c
  placement_phase.c
  player.c
  save_load.c
)
```

Then:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
# Windows: .\build\Release\penguin.exe
# Linux/macOS: ./build/penguin
```

---

## How to Play

**Main Menu**

1. New game
2. Load and continue (reads `save.txt`)
3. Exit

**Options**

* Toggle whether placement is restricted to **1‑fish** tiles (enter `2` = Yes, `1` = No)

**Setup**

* Enter player names
* Enter board size: `rows columns` (both ≥ 3), for example `8 8`

**Placement Phase**

* Players alternate placing **2** penguins each
* Input coordinates: `row column` (1‑based)
* Immediately collect that tile’s fish

**Movement Phase**

* Select a penguin by its current coordinates
* Move exactly **one** tile **up / down / left / right**
* You cannot move into holes `0`, onto occupied tiles, or outside the board
* Gain fish from the new tile; the old tile becomes a hole

**End Game**

* When a player has **no legal moves**, the game ends and scores are shown

---

## Rules Summary

* Placement: alternate until each player has **2 penguins**
* Movement: **one** orthogonal step per turn
* Scoring: gain fish from every tile placed/moved onto
* End: game stops when a player cannot move

---

## Save & Resume

* **Autosave file:** `save.txt` in the working directory
* **Autosave timing:** after every valid placement or move (UI shows `[autosaved]`)
* **Resume:** choose **Load and continue** from the main menu if `save.txt` exists

---

## Project Structure

```
board.c             # Board generation & rendering
checks.c            # Move/placement validation
function.c          # Utility helpers (coords, diffs)
io_func.c           # Prompts & input parsing
main.c              # Menu, new/load flow
movement_phase.c    # Turn loop & end conditions
penguin.c           # Score & board updates
placement_phase.c   # Placement logic & options
player.c            # Player state utilities
save_load.c         # Save/load system
```

---

## Roadmap

* Computer opponents (AI) and difficulty levels
* Variable penguin counts & fish distributions
* Sliding / multi‑step movement, obstacles, power‑ups
* GUI (SDL/ImGui) or Web UI
* Multiple save slots & tougher input validation

---

## Contributing

Pull requests are welcome! For larger changes, please open an issue first to discuss what you’d like to change.

### Development tips

* Keep build artifacts out of Git (`.vs/`, `Debug/`, `Release/`, `*.exe`, `*.pdb`, `*.user`).
* Prefer small, focused commits with clear messages.
* Add comments around tricky game logic (placement & movement validation).

---

## License

This project is licensed under the **MIT License**. See [`LICENSE.txt`](LICENSE.txt).

---

## Acknowledgements

* Inspired by the board game *Hey, That’s My Fish!* (streamlined rules and rectangular grid for a terminal experience).
* Thanks to contributors and play‑testers!
