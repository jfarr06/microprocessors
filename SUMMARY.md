# Microprocessors Project - Technical Summary

## Project Overview

This is a dungeon crawler game developed for the STM32 Nucleo F031K6 microcontroller as part of the TU857 Y2 Microprocessors (CMPU1013) coursework. The game features procedurally generated rooms, character movement, collision detection, time-based challenges, and coin collection mechanics.

**Author:** James Farrelly (C24402114)

## Hardware Requirements

### Primary Components
- **Microcontroller:** STM32 Nucleo F031K6 (ARM Cortex-M0)
- **Display:** ST7735S 128x160 LCD Display
- **Input:** 5 buttons (Up, Down, Left, Right, Enter)
- **Audio:** PWM-based sound generation capability

### Technical Specifications
- **Platform:** STM32 (ststm32)
- **Framework:** CMSIS (Cortex Microcontroller Software Interface Standard)
- **Clock:** PLL-based clock configuration
- **Display Protocol:** SPI communication with ST7735S

## Software Architecture

### Core System Components

#### 1. Scene Management (`src/scenes.c`, `include/scenes/`)
The game uses a scene-based architecture with two main scenes:

- **Menu Scene** (`menu_scene.c`): 
  - Home menu with "Play" option
  - Mode selection with 5 difficulty levels plus Return option:
    - **Easy:** Lots of coins, lots of time
    - **Medium:** Reduced coins, lots of time
    - **Hard:** Reduced coins, reduced time
    - **Endless:** Reduced coins, no time limit
    - **Random:** Random coins and time limit
  - Descriptive text displayed for each selected mode
  - Navigation using up/down buttons, selection with enter
  - Color-coded difficulty options (Lime for Easy, Yellow for Medium, Red for Hard, Orange for Endless, Cyan for Random)

- **Game Scene** (`game_scene.c`):
  - Main gameplay loop
  - Room rendering and management
  - Character movement and collision handling
  - Information box display
  - Pause menu with three states: Running, Paused, Status Screen
  - Pause menu options: Resume, Restart, Home Menu
  - Win/Loss status screen with "Press Enter" prompt

- **Scene Utilities** (`scenes.c`):
  - `step_options()`: Helper function for cycling through menu options
  - `render_options()`: Renders selectable options with visual indicator for current selection

#### 2. Room System (`src/rooms.c`, `include/rooms.h`)
Procedural room generation and management:

- **Room Structure:** 3x3 tile grid with four possible states:
  - `ROOM_TILE_STATE_WALL` (0x0): Impassable walls
  - `ROOM_TILE_STATE_HALL` (0x1): Walkable hallway
  - `ROOM_TILE_STATE_TRAN` (0x2): Transition points to adjacent rooms
  - `ROOM_TILE_STATE_COIN` (0x3): Collectible coins
  
- **Room Features:**
  - Maximum of 5 rooms (`MAX_NUM_ROOMS`)
  - Transition points on 4 sides (top, left, right, bottom)
  - Wall collision detection using coordinate sets
  - Dynamic room generation and linking

#### 3. Character System (`src/character.c`, `include/character.h`)
Player character with sprite animation:

- **Character Sprites:**
  - `char_front`: Facing down (idle)
  - `char_front_mov`: Facing down (moving)
  - `char_back`: Facing up (idle)
  - `char_back_mov`: Facing up (moving)
  - `char_right`: Facing right (idle)
  - `char_right_mov`: Facing right (moving)
  - Note: `char_left` and `char_left_mov` assets exist but are unused; left-facing uses horizontal flip of char_right sprites
  
- **Movement System:**
  - 4-directional movement (up, down, left, right)
  - Collision detection with walls and boundaries
  - Animation frame processing every 20 game frames
  - Step animation every 80 game frames
  - Sprite orientation handling (horizontal/vertical flip)

#### 4. Display System (`src/display.c`, `include/display.h`)
Graphics rendering using ST7735S LCD controller:

- **Screen Specifications:**
  - Resolution: 128x160 pixels
  - Color format: RGB16 (16-bit color)
  
- **Rendering Functions:**
  - `put_pixel()`: Individual pixel rendering
  - `put_image()`: Sprite rendering with orientation support
  - `fill_rect()`: Filled rectangle drawing
  - `draw_rect()`: Rectangle outline drawing
  - `fill_circle()`: Filled circle drawing
  - `print_text()`: Text rendering using 5x7 font
  - `print_number()`: Number rendering with scaling

#### 5. Information Box (`src/infobox.c`, `include/infobox.h`)
Game status display at the bottom of the screen:

- **Display Elements:**
  - Coin counter: Current coins / Target coins
  - Timer: Countdown in seconds
  - Status indicator: "W" (win) or "L" (loss)
  
- **Color Coding:**
  - Red: Insufficient coins or time expired
  - Green: Victory condition met
  - Yellow: Coin target indicator
  - White: Neutral elements

#### 6. Input System (`src/io.c`, `include/io.h`)
Button state management with debouncing:

- **Input States:**
  - `held`: Currently pressed buttons
  - `trigger`: Newly pressed buttons (rising edge)
  - `release`: Released buttons (falling edge)
  
- **Button Configuration:**
  - GPIO with internal pull-up resistors
  - Active low logic
  - Bit-packed status representation

#### 7. Hardware Abstraction (`src/nucleo_f031k6/`)
Platform-specific implementations:

- **clock.c**: PLL initialization, SysTick timer, delay functions
- **serial.c**: UART communication for debug output
- **sound.c**: PWM-based audio generation
- **adc.c**: Analog-to-digital conversion (if needed)
- **common.c**: GPIO and peripheral utilities

#### 8. Music System (`src/music.c`, `include/music.h`)
Background music and audio mixing:

- **Music Structure**: Note sequences with frequency and duration
- **Audio Mixing**: Prioritizes sound effects over background music
- **Non-blocking Playback**: Timer-based advancement without blocking game loop
- **Looping Support**: Tracks can loop continuously or play once
- **Sound Effects API**: Play short sound effects that temporarily override music
- **Music Tracks** (`music_tracks.c`): Pre-defined melodies for menu and game scenes

#### 9. Utility Systems

- **Random Number Generation** (`src/rand.c`):
  - Pseudo-random number generator initialization
  - Range-based random value generation
  
- **Collision Detection** (`src/util.c`):
  - `rect_intersects_rect()`: Rectangle-to-rectangle intersection
  - `rect_intersects_circle()`: Rectangle-to-circle intersection with improved type handling
  
- **String and Math Utilities** (`src/util.c`):
  - `strlen_custom()`: Custom string length calculation
  - `digit_count()`: Count digits in a number using log10
  - `game_frame`: Global frame counter for timing
  
- **Debug System** (`include/debug.h`):
  - Conditional compilation for debug builds
  - Serial output for logging (INFO, DEBUG, TRACE levels)

## Game Mechanics

### Gameplay Loop
1. Player starts in the menu scene
2. Navigates through home menu to mode selection
3. Selects difficulty mode with descriptive information displayed
4. Spawns in initial room with coin(s)
5. Navigate between rooms using transition points
6. Can pause game at any time to access pause menu (Resume, Restart, Home Menu)
7. Collect all coins before time expires (except in Endless mode)
8. Win condition: All coins collected with time remaining
9. Loss condition: Time expires before collecting all coins
10. Status screen displays win/loss result with option to return to menu

### Collision System
- Wall collision prevents movement through obstacles
- Transition collision triggers room changes
- Character bounds checking prevents screen boundary violations
- Coin collision triggers collection and updates info box

### Animation System
- Frame-based timing using `game_frame` counter
- Character sprite changes based on movement direction
- Smooth animation at 20-frame intervals
- Movement steps every 80 frames

## Build System

### PlatformIO Configuration
The project uses PlatformIO with a hierarchical configuration:

- **Root Configuration** (`platformio.ini`):
  - Platform: ststm32
  - Board: nucleo_f031k6
  - Framework: CMSIS
  - Build flags: `-Iinclude`
  - Source filter: Excludes `.git/` and `src/` subdirectories
  
- **Extended Configuration** (`src/platformio.ini`):
  - Contains additional environment-specific settings

### Makefile Targets

```makefile
make images      # Convert BMP assets to C header files
make build       # Build the project (TGT=release or TGT=debug)
make upload      # Upload firmware to the board
make clean       # Clean build artifacts
make init        # Generate compilation database for IDE
```

### Build Environments
- **Release** (default): Optimized build
- **Debug**: Slower build with debug symbols and serial logging

### Asset Pipeline
Images are stored as BMP files in `assets/` and converted to C header data using:
```bash
python3 assets/bmp2bin.py assets/ include/images_data.h
```

## Code Organization

### Directory Structure
```
microprocessors/
├── src/                      # Source files
│   ├── main.c               # Entry point
│   ├── character.c          # Character logic
│   ├── rooms.c              # Room generation
│   ├── display.c            # Display driver
│   ├── infobox.c            # HUD system
│   ├── io.c                 # Input handling
│   ├── scenes.c             # Scene management
│   ├── st7735s.c            # LCD driver
│   ├── util.c               # Utilities
│   ├── rand.c               # RNG
│   ├── scenes/              # Scene implementations
│   │   ├── game_scene.c
│   │   └── menu_scene.c
│   └── nucleo_f031k6/       # Hardware abstraction
│       ├── clock.c
│       ├── serial.c
│       ├── sound.c
│       ├── adc.c
│       └── common.c
├── include/                 # Header files
│   ├── *.h                  # Public interfaces
│   ├── images_data.h        # Generated image data
│   ├── font5x7.h            # Font definitions
│   ├── notes.h              # Musical note frequencies
│   ├── scenes/              # Scene headers
│   └── nucleo_f031k6/       # HAL headers
├── assets/                  # Image assets (BMP)
├── .vscode/                 # VS Code configuration
├── Makefile                 # Build automation
├── platformio.ini           # PlatformIO config
└── README.md                # Basic instructions
```

### Code Statistics
- **Total lines of code:** ~2,372 lines (C source only)
- **Source files:** 17 C files
- **Header files:** 24 H files
- **Primary language:** C (with some inline assembly if needed)

## Key Features

### Graphics
- 16-bit RGB color support
- Sprite rendering with orientation flip
- Shape drawing primitives (rectangles, circles)
- Text rendering with 5x7 bitmap font
- Image data stored in flash memory

### Audio
- PWM-based tone generation
- Musical note frequency definitions (C0-C8)
- Background music system with audio mixing
- Looping music tracks for menu and game scenes
- Sound effect prioritization over background music
- Non-blocking music playback using timer-based advancement

### Performance Optimization
- Dirty rectangle rendering (only redraws changed areas)
- Minimal memory allocation (static buffers where possible)
- Frame-based timing for consistent gameplay
- Efficient collision detection using axis-aligned bounding boxes

### Debug Features
- Conditional compilation for debug builds
- Serial UART output for logging
- Configurable log levels (INFO, DEBUG, TRACE)
- File and line number tracking in logs

## Memory Management

### Static vs Dynamic Allocation
- Rooms: Dynamically allocated (`malloc`/`free`)
- Scenes: Dynamically allocated
- Character: Static allocation
- Image data: Static const arrays in flash
- Font data: Static const arrays in flash

### Memory Constraints
The STM32F031K6 has:
- **Flash:** 32 KB (program storage)
- **SRAM:** 4 KB (runtime memory)
- Careful memory management required due to limited RAM

## Development Workflow

### Initial Setup
1. Install PlatformIO
2. Clone the repository
3. Run `make init` to generate intellisense data
4. Connect Nucleo F031K6 board via USB

### Build and Deploy
1. `make images` - Generate image data from BMP assets
2. `make build TGT=release` - Compile firmware
3. `make upload TGT=release` - Flash to microcontroller

### Debug Workflow
1. Build with debug target: `make build TGT=debug`
2. Connect serial terminal to view debug output
3. Monitor UART at appropriate baud rate
4. Use log messages to trace execution

## Technical Challenges Addressed

### 1. Limited Resources
- Optimized memory usage with careful allocation
- Minimized dynamic allocations
- Efficient sprite rendering

### 2. Real-Time Constraints
- Frame-based timing system
- Non-blocking input handling
- Efficient collision detection

### 3. Hardware Abstraction
- Separated platform-specific code
- Modular hardware interface
- Portable game logic

### 4. Graphics Performance
- Dirty rectangle optimization
- Minimal overdraw
- Efficient color format

## Future Enhancement Possibilities

### Gameplay
- Multiple character sprites
- Enemy AI and pathfinding
- Power-ups and special items
- More complex room layouts
- Save/load game state

### Technical
- Double buffering for smoother graphics
- Hardware sprite support (if available)
- DMA for display updates
- More sophisticated collision shapes

### Features
- High score persistence (EEPROM/Flash)
- Procedural room generation algorithms
- Mini-map display
- Particle effects
- Screen transitions

## Dependencies

### External Libraries
- **CMSIS**: ARM Cortex Microcontroller Software Interface
- **STM32F0xx HAL**: Hardware abstraction from ST
- **PlatformIO**: Build system and toolchain management

### Tool Requirements
- **Python 3**: For asset conversion scripts
- **PlatformIO**: Build system
- **GCC ARM Toolchain**: C compiler for ARM
- **OpenOCD/ST-Link**: Firmware upload

## Educational Value

This project demonstrates:
- Bare-metal embedded programming
- Real-time system design
- Memory-constrained development
- Hardware interfacing (GPIO, SPI, UART, PWM)
- Game architecture patterns
- Resource management
- Optimization techniques

## License and Attribution

**Author:** James Farrelly (C24402114)  
**Course:** TU857 Y2 Microprocessors (CMPU1013)  
**Target:** STM32 Nucleo F031K6  

This project is educational in nature and developed as coursework for Technological University Dublin (TU Dublin).

---

*Last Updated: November 2025*
*This document provides a comprehensive technical overview of the microprocessors game project. For build instructions, see README.md.*
