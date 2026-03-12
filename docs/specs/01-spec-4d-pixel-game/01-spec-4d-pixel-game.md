# 01-spec-4d-pixel-game.md

## Introduction/Overview

A 3D top-down pixel art platformer written in C++ using Raylib. The player controls a character in a 3D world viewed from above, with pixel art textures applied to all geometry. The character can move freely on the ground plane and jump onto elevated platforms. This spec covers the foundational MVP — a playable character in a simple level. A future spec will layer in the "4th dimension" time mechanic on top of this foundation.

## Goals

- Render a 3D world with a top-down orthographic or perspective camera using Raylib on macOS
- Apply pixel art textures to the world geometry and player character
- Implement responsive player movement (walk/run) on the ground plane via keyboard input
- Implement a jump mechanic with gravity so the player can reach elevated platforms
- Build a simple level with a ground floor and a few elevated platforms to jump onto

## User Stories

**As a player**, I want to see a character on screen in a pixel-art 3D world so that I have something to interact with immediately upon launching the game.

**As a player**, I want to move my character around the world using the keyboard so that I can explore the environment.

**As a player**, I want my character to jump so that I can reach platforms that are higher than the ground.

**As a developer**, I want a clean Makefile-based build so that I can compile and run the game with a single command on macOS.

## Demoable Units of Work

### Unit 1: Project Setup — Window Opens and World Renders

**Purpose:** Proves the build system, Raylib dependency, and basic 3D scene are all working on macOS before any game logic is added.

**Functional Requirements:**
- The system shall compile with a single `make` command using a Makefile
- The system shall open a window titled "4D Pixel Game" at 800x600 resolution
- The system shall render a 3D scene with a flat ground plane visible from a top-down camera angle
- The system shall display a target frame rate of 60 FPS
- The ground plane shall use a pixel art texture (at minimum a solid color or checkerboard pattern)

**Proof Artifacts:**
- Screenshot: Running game window showing the 3D top-down view of the ground plane demonstrates the build and rendering pipeline works
- CLI: `make` command completes without errors demonstrates the build system is correctly configured

### Unit 2: Player Character Appears and Moves

**Purpose:** Proves the player character can be placed in the world, rendered with pixel art style, and controlled via keyboard.

**Functional Requirements:**
- The system shall render a player character (represented as a 3D box or sprite) at a starting position in the world
- The player character shall use a pixel art texture or solid color to maintain the pixel aesthetic
- The system shall move the player character along the ground plane using WASD or arrow keys
- The player character shall not be able to walk through the outer boundaries of the level
- The camera shall follow the player character and always look down at it from above

**Proof Artifacts:**
- Gameplay recording or screenshot: Player character visible in world demonstrates character is rendered correctly
- Gameplay demo: Player moving in all four directions demonstrates keyboard input and movement are working

### Unit 3: Jump Mechanic and Elevated Platforms

**Purpose:** Proves the platforming mechanic works — the player can jump, gravity pulls them back down, and elevated surfaces can be landed on.

**Functional Requirements:**
- The system shall apply gravity to the player character so that they fall when not on a solid surface
- The player character shall jump when the Space key is pressed, only if they are currently on the ground
- The level shall contain at least three elevated platform surfaces at different heights
- The player character shall land and stand on top of platform surfaces when their jump reaches the platform height
- The player character shall fall back to the ground if they walk off the edge of a platform

**Proof Artifacts:**
- Gameplay demo: Player jumps from the ground to a platform and back demonstrates jump, gravity, and platform collision all work
- Gameplay demo: Player walks off a platform edge and falls demonstrates edge collision and gravity are correct

## Non-Goals (Out of Scope)

1. **Time/4D mechanic**: The "rewind time" or "time travel" mechanic is explicitly deferred to a future spec.
2. **Object interaction**: No pickups, doors, switches, or triggered events in this version.
3. **Enemies or combat**: No enemy characters or damage system.
4. **Audio**: No sound effects or music.
5. **Multiple levels or a level select screen**: Only a single hardcoded level.
6. **Windows or Linux support**: macOS only for this spec.
7. **Scrolling camera**: The camera follows the player but the level fits within a fixed area — no infinite scrolling world.

## Design Considerations

The game uses a top-down 3D perspective camera positioned high above the player at a slight downward tilt (not perfectly vertical) — enough angle to make platform height differences clearly visible while still feeling top-down. All geometry in the world (ground, platforms, player) uses pixel art textures to create a consistent retro aesthetic. Pixel art textures should be loaded at their native low resolution and rendered without bilinear filtering (use nearest-neighbor sampling) so pixels remain sharp and blocky.

All textures (ground, platforms, player character) are procedurally generated at runtime using simple pixel patterns — no external image files are required. The player character can be represented as a simple 3D box (cube or rectangular prism) with a procedurally generated pixel art texture applied — no skeletal animation is required for the MVP.

## Repository Standards

This is a new project with no existing conventions. The following standards should be established from the start:

- **Language**: C++17 or later
- **Build system**: GNU Make with a `Makefile` at the project root; `make` builds the binary, `make clean` removes build artifacts
- **Dependencies**: Raylib (installed via Homebrew on macOS: `brew install raylib`)
- **Binary output**: Compiled binary placed in a `build/` directory
- **Source layout**: Source files in `src/`, header files in `include/`, assets (textures) in `assets/`
- **Coding style**: Snake_case for variables and functions, PascalCase for structs and classes
- **No external build tools** beyond Make (no CMake, no Xcode project files)

## Technical Considerations

- **Raylib** is the graphics and input library. It handles window creation, 3D rendering, texture loading, keyboard input, and the game loop.
- **Camera**: Use Raylib's `Camera3D` with `CAMERA_PERSPECTIVE` projection. Position the camera above and slightly behind the player at a fixed tilt angle (e.g., 45–60 degrees from vertical) so platform heights are visible. Update camera position each frame to track the player.
- **Procedural textures**: Generate textures at startup using `LoadTextureFromImage` with pixel patterns drawn via `ImageDrawPixel` or similar Raylib image drawing calls. No image files on disk are needed.
- **Physics**: Implement simple manual gravity — no physics engine. Each frame, apply a downward velocity to the player unless they are standing on a surface. A collision check against ground and platform bounding boxes determines when to stop falling.
- **Texture filtering**: Set Raylib texture filter to `TEXTURE_FILTER_POINT` (nearest-neighbor) to keep pixel art sharp.
- **Platform collision**: Represent platforms as axis-aligned bounding boxes (AABB). Check player bounding box against platform boxes each frame.
- **Dependency installation**: `brew install raylib` on macOS. The Makefile should link against the installed Raylib using pkg-config or hardcoded Homebrew paths.

## Security Considerations

No specific security considerations identified. This is a local desktop game with no networking, user accounts, or external data.

## Success Metrics

1. **Builds cleanly**: `make` compiles the project with zero errors and zero warnings on macOS
2. **Runs at target frame rate**: Game maintains 60 FPS during normal play
3. **All three demoable units pass**: Window opens, character moves, jump and platform landing work correctly
4. **Pixel aesthetic is preserved**: Textures render with sharp pixels (no blurring)

## Open Questions

No open questions at this time.
