# 🐧 Penguin Game (Terminal C)

A fast, two-player, hot-seat strategy game inspired by *“Hey, That’s My Fish!”*, implemented in standard C and playable entirely in the terminal. Players place and move penguins on an ice grid, collecting fish and blocking each other until no legal moves remain.

---

## ✨ Features
- 🎮 Two players: Local hot-seat with player names  
- 🔄 Two phases: Placement → Movement  
- 🎲 Procedural board: Random fish count (1–3) per tile  
- ➡️ Simple moves: One orthogonal step per turn (no diagonals)  
- 🐟 Scoring: Gain fish on every placed/moved-to tile  
- 💾 Autosave: Writes save.txt after each valid action  
- 📦 Portable C: Builds with GCC/Clang/MSVC on Windows, Linux, macOS  

---

## ⚙️ Build

Requirements: Any C11-compatible compiler (GCC, Clang, or MSVC).  
No external libraries needed.

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

---

## 🎮 How To Play

### Main Menu
1. New game  
2. Load and continue (save.txt)  
3. Exit  

### Options
- Choose whether placement is restricted to 1-fish tiles.  
  Enter 2 = Yes, 1 = No.  

### Setup
- Enter player names  
- Enter board size: rows columns (both ≥ 3), e.g., 8 8  

### Placement Phase
- Players alternate placing penguins (2 each)  
- Input coordinates: row column (1-based)  
- Immediately collect that tile’s fish  

### Movement Phase
- Select penguin: enter its current coordinates  
- Move exactly one tile up/down/left/right  
- Cannot move onto holes (0), occupied tiles, or outside board  
- Gain fish from the new tile; the old tile becomes a hole  

### End Game
- When a player cannot move any penguin, the game ends  
- Scores are shown  

---

## 🧊 Board & UI

- Coordinates: 1-based input (row column)  
- Legend:  

| Symbol | Meaning |
|--------|---------|
| 01, 02, 03 | Fish count |
| P1, P2 | Player tiles |
| 0 | Hole (vacated tile) |

---

## 💾 Save / Load
- File: save.txt in working directory  
- Autosave: after every valid placement or move ([autosaved] shown)  
- Resume: Choose option 2) Load and continue if file exists  

---

## 📜 Rules Summary
- Placement: Alternate until each has 2 penguins  
- Movement: One orthogonal step per turn  
- Scoring: Gain fish from every tile placed/moved onto  
- End: Game stops when a player has no legal moves  

---

## 🗂 Project Structure
main.c              – Menu, new/load flow
placement_phase.c   – Placement logic, options
movement_phase.c    – Turn loop, end conditions
board.c             – Board generation, rendering
checks.c            – Placement/move validation
io_func.c           – Prompts, input parsing
penguin.c           – Score/board updates
player.c            – Player state utilities
function.c          – Helpers (coords, diffs)
save_load.c         – Save/load system

---

## 🔧 Customization
- Penguins per player: Default 2 → change in placement_phase.c  
- Board size: Set at runtime (≥ 3×3)  
- Placement rule: Toggle “1-fish only” at game start  

---

## ⚠️ Limitations
- Movement: single orthogonal step only  
- Rectangular grid (simplified vs. original board game)  
- No AI (local 2P only)  

---

## 🚀 Roadmap Ideas
- AI opponents & difficulty levels  
- Variable penguin counts & fish distributions  
- Sliding/multi-step movement, obstacles, power-ups  
- GUI (SDL/ImGui) or Web UI  
- Multiple save slots & stronger validation  

---

## 📄 License
MIT License
