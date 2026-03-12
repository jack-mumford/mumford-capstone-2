# 01-tasks-4d-pixel-game.md

## Relevant Files

- `Makefile` - Build system: compiles all `src/*.cpp` files with C++17, links Raylib via Homebrew/pkg-config, outputs binary to `build/4d-pixel-game`; `make clean` removes build artifacts
- `src/main.cpp` - Entry point: initializes window and camera, runs the main game loop, delegates to player/level update and draw calls
- `src/textures.cpp` - Procedural pixel art texture generation: creates all in-memory textures at startup using Raylib image drawing functions
- `include/textures.h` - Declares texture generation functions used by player and level modules
- `src/player.cpp` - Player logic: struct initialization, keyboard input, horizontal movement, boundary clamping, gravity, jump, and AABB ground collision
- `include/player.h` - Defines the `Player` struct (position, velocity, size, is_grounded) and declares player functions
- `src/level.cpp` - Level data: defines ground plane and the three elevated platforms as AABB structs, provides draw function and accessor for collision data
- `include/level.h` - Defines the `Platform` struct (position, size) and declares level functions

### Notes

- This is a new project — all files listed above need to be created from scratch.
- There is no test suite for this MVP; proof artifacts are gameplay screenshots and CLI build output.
- Follow the repository standard: snake_case for variables/functions, PascalCase for structs.
- All Raylib includes should use angle brackets: `#include <raylib.h>`
- The Makefile should use `pkg-config --cflags --libs raylib` to locate the Homebrew-installed Raylib automatically.

## Tasks

### [x] 1.0 Project Scaffolding, Build System, and 3D Scene

Set up the entire project structure from scratch: directory layout, Makefile, Raylib linkage, and a working game loop that renders a 3D ground plane from a tilted top-down camera.

#### 1.0 Proof Artifact(s)

- CLI: `make` runs without errors or warnings and produces a binary at `build/4d-pixel-game` demonstrates the build system is correctly configured
- Screenshot: Game window titled "4D Pixel Game" (800x600) showing a 3D ground plane rendered from a top-down tilted camera angle demonstrates the rendering pipeline works end-to-end

#### 1.0 Tasks

- [x] 1.1 Create the project directory structure: `src/`, `include/`, `build/` at the project root (the `docs/` directory already exists — do not recreate it)
- [x] 1.2 Create `Makefile` at the project root that: uses `g++` with `-std=c++17`, compiles all `src/*.cpp` files, links Raylib using `pkg-config --cflags --libs raylib`, outputs the binary to `build/4d-pixel-game`, and includes a `clean` target that removes the `build/` directory
- [x] 1.3 Create `src/main.cpp` with a minimal Raylib program: call `InitWindow(800, 600, "4D Pixel Game")`, `SetTargetFPS(60)`, a `while (!WindowShouldClose())` game loop with `BeginDrawing()`/`EndDrawing()`, and `CloseWindow()` after the loop
- [x] 1.4 Run `make` and confirm the binary compiles and runs (window opens and closes cleanly with the X button or Escape key)
- [x] 1.5 Add a `Camera3D` to `main.cpp`: position it at `{0, 20, 10}` (high above, slightly behind), target at `{0, 0, 0}`, up vector `{0, 1, 0}`, fovy `45.0f`, projection `CAMERA_PERSPECTIVE` — this gives the slight downward tilt specified in the spec
- [x] 1.6 Create `src/textures.cpp` and `include/textures.h`; add a function `generate_ground_texture()` that creates a 16x16 `Image` using `GenImageChecked(16, 16, 4, 4, DARKGREEN, GREEN)`, calls `SetTextureFilter` with `TEXTURE_FILTER_POINT` on the result, and returns a `Texture2D`
- [x] 1.7 In `main.cpp`, call `generate_ground_texture()` before the game loop; inside the loop between `BeginMode3D` and `EndMode3D`, draw a ground plane using `DrawPlane({0,0,0}, {20,20}, WHITE)` — Raylib will tile the color; also draw a simple `DrawGrid(20, 1.0f)` so the 3D perspective is visible
- [x] 1.8 Confirm the window shows the 3D scene from the tilted top-down angle and take a screenshot for the proof artifact

---

### [x] 2.0 Player Character Rendering and Movement

Add a player character (textured 3D box) to the scene, implement keyboard-driven movement on the ground plane, boundary clamping so the player can't leave the level, and camera follow so the player stays centered on screen.

#### 2.0 Proof Artifact(s)

- Screenshot: Player character visible in the 3D world with a pixel art texture demonstrates character rendering is correct
- Gameplay demo (screen recording or screenshot sequence): Player character moving in all four directions (WASD/arrow keys) with the camera tracking them demonstrates input, movement, and camera follow all work

#### 2.0 Tasks

- [x] 2.1 Create `include/player.h` and define a `Player` struct with fields: `Vector3 position`, `Vector3 velocity`, `Vector3 size`, `bool is_grounded`; also declare three functions: `Player player_init()`, `void player_update(Player& player)`, `void player_draw(const Player& player)` (note: DrawCubeTexture removed in Raylib 5 — uses DrawCubeV with color instead)
- [x] 2.2 Create `src/player.cpp`; implement `player_init()` to return a `Player` with position `{0, 0.5f, 0}` (half-height above ground), size `{0.8f, 1.0f, 0.8f}`, zero velocity, and `is_grounded = false`
- [x] 2.3 Procedural player texture generated in `src/textures.cpp` (kept for ground; player uses BLUE color via DrawCubeV — Raylib 5 removed DrawCubeTexture)
- [x] 2.4 Implement `player_draw()` in `src/player.cpp` using `DrawCubeV` (BLUE) + `DrawCubeWiresV` (DARKBLUE)
- [x] 2.5 Implement keyboard input in `player_update()`: WASD + arrow keys, movement speed `5.0f * GetFrameTime()`
- [x] 2.6 Add boundary clamping in `player_update()`: clamp X/Z to `[-9.0f, 9.0f]`
- [x] 2.7 Camera follow in `main.cpp`: `camera.target = player.position`, `camera.position = {px, py+20, pz+10}`
- [x] 2.8 Wire player into `main.cpp` game loop; character renders, moves with WASD/arrows, boundary clamp works, camera tracks

---

### [ ] 3.0 Jump Mechanic, Gravity, and Elevated Platforms

Implement manual gravity, a Space-to-jump mechanic, and AABB collision detection so the player can jump onto at least three elevated platforms at different heights and falls back when walking off an edge.

#### 3.0 Proof Artifact(s)

- Gameplay demo: Player jumps from the ground and lands on an elevated platform demonstrates jump velocity, gravity, and top-surface collision all work correctly
- Gameplay demo: Player walks off a platform edge and falls to the ground (or a lower platform) demonstrates edge detection and gravity on falling are correct
- Screenshot: Level overview showing all three elevated platforms at different heights demonstrates the level design requirement is met

#### 3.0 Tasks

- [ ] 3.1 Create `include/level.h` and define a `Platform` struct with fields: `Vector3 position` (center of the platform), `Vector3 size` (width, height, depth); declare three functions: `void level_init(Platform platforms[], int& count)`, `void level_draw(const Platform platforms[], int count, Texture2D texture)`, `bool check_platform_collision(Player& player, const Platform platforms[], int count)`
- [ ] 3.2 Create `src/level.cpp`; implement `level_init()` to populate the array with at least three platforms at varied positions and heights — for example: `{-3, 1, -3}` size `{3, 0.5f, 3}`, `{3, 2, 2}` size `{3, 0.5f, 3}`, `{0, 3, -6}` size `{4, 0.5f, 2}` — each at a different Y height so the player must jump progressively higher
- [ ] 3.3 Add a `generate_platform_texture()` function to `src/textures.cpp` and its declaration to `include/textures.h`; use a different color scheme from the ground (e.g., `GenImageChecked(16, 16, 4, 4, BROWN, DARKBROWN)`) with `TEXTURE_FILTER_POINT`
- [ ] 3.4 Implement `level_draw()` in `src/level.cpp`: for each platform, call `DrawCubeTexture(texture, platform.position, platform.size.x, platform.size.y, platform.size.z, WHITE)`
- [ ] 3.5 Add gravity and jump fields to the `Player` struct in `include/player.h`: add a constant `float jump_velocity = 8.0f` and a constant `float gravity = -20.0f`; these will be used in the physics update
- [ ] 3.6 In `player_update()` in `src/player.cpp`, apply gravity each frame: `player.velocity.y += gravity * GetFrameTime()`; then apply vertical velocity to position: `player.position.y += player.velocity.y * GetFrameTime()`; clamp position to never go below ground level (`y >= 0.5f`) and set `is_grounded = true` and `velocity.y = 0` when clamped to ground
- [ ] 3.7 Add jump input to `player_update()`: if `IsKeyPressed(KEY_SPACE)` and `player.is_grounded == true`, set `player.velocity.y = player.jump_velocity` and `player.is_grounded = false`
- [ ] 3.8 Implement `check_platform_collision()` in `src/level.cpp`: for each platform, check if the player's bounding box (position ± size/2) overlaps horizontally with the platform's bounding box AND the player's bottom edge is within a small threshold (e.g., `0.1f`) of the platform's top surface (`platform.position.y + platform.size.y / 2`); if so, snap `player.position.y` to sit on top of the platform, set `player.velocity.y = 0`, and set `player.is_grounded = true`
- [ ] 3.9 In `main.cpp`, call `check_platform_collision()` every frame after `player_update()`; also set `player.is_grounded = false` at the start of each frame (before update) so it resets — it only becomes true again if a collision check confirms the player is on a surface that frame
- [ ] 3.10 Wire level into `main.cpp`: call `level_init()` before the loop, call `level_draw()` and `check_platform_collision()` inside the loop; play-test all three proof artifact scenarios (jump to platform, land, walk off edge, fall) and take screenshots/recording for the proof artifacts
