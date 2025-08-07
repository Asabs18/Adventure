# Graph Adventure Game

A dynamic dungeon exploration game with procedural generation, atmospheric lighting, and intuitive navigation. Built in C using the Raylib graphics library.

## Features

### 🎲 Procedural Generation
- **Dynamic Room Count**: Each playthrough generates 5-12 unique rooms
- **Random Layouts**: Rooms are positioned using scatter-based algorithms with collision detection
- **Guaranteed Connectivity**: Advanced graph algorithms ensure all rooms are reachable
- **Strategic Design**: Start and end rooms are always at least 2 steps apart using BFS pathfinding

### 🎯 Gameplay Elements
- **Collectible Items**: Find the hidden star in your exploration
- **Victory Conditions**: Reach the end room to complete the dungeon
- **Replay Value**: Press 'R' to generate an entirely new dungeon layout

## Technical Architecture

### Core Systems
- **Graph Generation**: Implements connected graph theory with validation
- **BFS Pathfinding**: Ensures minimum distance constraints and connectivity
- **Dynamic Memory Management**: Efficient room and connection handling

### Code Structure
```
src/
├── main.c          # Application entry point
├── room.c/.h       # Room generation and graph algorithms
├── game.c/.h       # Core game logic and text-based interface
└── gui.c/.h        # Graphics rendering and user interface
```

## Requirements

### Dependencies
- **Raylib**: Graphics and windowing library
- **C Compiler**: MSVC, GCC, or Clang with C90+ support
- **Operating System**: Windows, macOS, or Linux

### Build Tools
- Visual Studio (Windows)
- Make/CMake (Cross-platform)
- vcpkg (Package management)

## Installation

### Windows (Visual Studio)
```bash
# Clone the repository
git clone https://github.com/yourusername/adventure-game.git
cd adventure-game

# Install Raylib via vcpkg
vcpkg install raylib

# Compile the project
cl main.c room.c game.c gui.c /I"path\to\vcpkg\include" /link /LIBPATH:"path\to\vcpkg\lib" raylib.lib
```

### Linux/macOS
```bash
# Install dependencies
# Ubuntu/Debian:
sudo apt install libraylib-dev

# macOS:
brew install raylib

# Compile
gcc -o adventure main.c room.c game.c gui.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

## Controls

| Action | Input |
|--------|--------|
| Move to Room | Left Click on connected room |
| Restart Game | Press 'R' (when game is won) |
| Exit Game | Press 'ESC' or close window |
| Resize Window | Drag window borders (responsive layout) |

## Configuration

### Customizable Constants
```c
#define MIN_ROOMS 5          // Minimum rooms per dungeon
#define MAX_ROOMS 12         // Maximum rooms per dungeon
#define ROOM_RADIUS 40       // Visual room size
```

### Display Settings
- **Minimum Resolution**: 800x600
- **Default Resolution**: 1024x768
- **Resizable Window**: Adaptive layout scaling
- **Target Framerate**: 60 FPS