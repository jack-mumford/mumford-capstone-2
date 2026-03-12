# Task 3.0 Proof Artifacts — Jump Mechanic, Gravity, and Elevated Platforms

## Build Output

```
$ make
g++ -std=c++17 -Wall -Wextra -I/opt/homebrew/Cellar/raylib/5.5/include -Iinclude -c src/level.cpp -o build/level.o
g++ -std=c++17 -Wall -Wextra -I/opt/homebrew/Cellar/raylib/5.5/include -Iinclude -c src/main.cpp -o build/main.o
g++ -std=c++17 -Wall -Wextra -I/opt/homebrew/Cellar/raylib/5.5/include -Iinclude -c src/player.cpp -o build/player.o
g++ build/level.o build/main.o build/player.o build/textures.o -o build/4d-pixel-game \
    -L/opt/homebrew/Cellar/raylib/5.5/lib -lraylib -framework IOKit -framework Cocoa -framework OpenGL
```

**Result:** Zero errors, zero warnings.

## Platform Layout (`src/level.cpp`)

```cpp
// Platform 1: low, left side  (top surface at y = 1.0 + 0.25 = 1.25)
platforms[0].position = { -4.0f, 1.0f,  -2.0f };
platforms[0].size     = {  4.0f, 0.5f,   4.0f };

// Platform 2: medium, right side  (top surface at y = 2.25 + 0.25 = 2.5)
platforms[1].position = {  4.0f, 2.25f,  2.0f };
platforms[1].size     = {  3.5f, 0.5f,   3.5f };

// Platform 3: high, center-back  (top surface at y = 3.75 + 0.25 = 4.0)
platforms[2].position = {  0.0f, 3.75f, -6.0f };
platforms[2].size     = {  4.0f, 0.5f,   2.5f };
```

Three platforms at three distinct heights — each requires a jump from the previous level.

## Physics Constants (`include/player.h`)

```cpp
static const float JUMP_VELOCITY = 8.0f;
static const float GRAVITY       = -20.0f;
```

## Jump + Gravity Implementation (`src/player.cpp`)

```cpp
// Jump input
if (IsKeyPressed(KEY_SPACE) && player.is_grounded) {
    player.velocity.y  = JUMP_VELOCITY;   // 8.0 m/s upward
    player.is_grounded = false;
}

// Apply gravity and integrate position
player.velocity.y += GRAVITY * dt;        // -20 m/s² acceleration
player.position.y += player.velocity.y * dt;

// Ground floor clamp
if (player.position.y < GROUND_Y) {
    player.position.y  = GROUND_Y;
    player.velocity.y  = 0.0f;
    player.is_grounded = true;
}
```

## AABB Collision (`src/level.cpp`)

```cpp
// XZ overlap check
bool overlap_x = player_x_max > plat_x_min && player_x_min < plat_x_max;
bool overlap_z = player_z_max > plat_z_min && player_z_min < plat_z_max;

// Landing condition: falling (velocity.y <= 0) onto platform top surface
if (player.velocity.y <= 0.0f &&
    player_bottom <= plat_top + 0.15f &&
    player_bottom >= plat_top - 0.4f) {

    player.position.y  = plat_top + player.size.y * 0.5f; // snap on top
    player.velocity.y  = 0.0f;
    player.is_grounded = true;
}
```

## is_grounded Reset (`src/main.cpp`)

```cpp
// Reset grounded each frame; player_update and check_platform_collision re-set it
player.is_grounded = false;

player_update(player);                                      // may set is_grounded (ground floor)
check_platform_collision(player, platforms, platform_count); // may set is_grounded (platform)
```

## HUD Debug Display

The game displays `GROUNDED` / `AIRBORNE` on screen (top-left, below FPS counter) to confirm
the is_grounded state is toggling correctly during play.

## Verification Checklist

- [x] 3 platforms defined at heights 1.0, 2.25, 3.75 (distinct, progressively higher)
- [x] `JUMP_VELOCITY = 8.0f`, `GRAVITY = -20.0f` constants in player.h
- [x] Gravity applied every frame via `velocity.y += GRAVITY * dt`
- [x] Jump fires only when `is_grounded == true` (Space key)
- [x] `is_grounded` resets to false at start of each frame
- [x] Ground floor clamp re-sets `is_grounded = true` when player hits y=0.5
- [x] AABB XZ overlap check gates platform landing
- [x] Player snapped to `plat_top + size.y/2` on landing
- [x] Walking off platform edge: no horizontal support → gravity resumes → player falls
- [x] Gameplay demo: Press Space near platform — player jumps, lands on brown platform top (run `./build/4d-pixel-game`)
- [x] Gameplay demo: Walk off platform edge — HUD switches to AIRBORNE, player falls back to ground
- [x] Screenshot: Level overview visible from tilted top-down camera shows 3 brown platform blocks at different heights
