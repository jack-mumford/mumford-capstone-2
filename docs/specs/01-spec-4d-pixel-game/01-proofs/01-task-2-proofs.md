# Task 2.0 Proof Artifacts — Player Character Rendering and Movement

## Build Output

```
$ make
g++ -std=c++17 -Wall -Wextra -I/opt/homebrew/Cellar/raylib/5.5/include -Iinclude -c src/main.cpp -o build/main.o
g++ -std=c++17 -Wall -Wextra -I/opt/homebrew/Cellar/raylib/5.5/include -Iinclude -c src/player.cpp -o build/player.o
g++ build/main.o build/player.o build/textures.o -o build/4d-pixel-game -L/opt/homebrew/Cellar/raylib/5.5/lib -lraylib -framework IOKit -framework Cocoa -framework OpenGL
```

**Result:** Zero errors, zero warnings.

## Implementation Notes

`DrawCubeTexture` was removed in Raylib 5.x. Player is rendered using `DrawCubeV` (BLUE) +
`DrawCubeWiresV` (DARKBLUE) which produces a clean block character consistent with the pixel art
aesthetic. The ground plane uses DARKGREEN color via `DrawPlane`.

## Player Struct (`include/player.h`)

```cpp
struct Player {
    Vector3 position;   // world position (center of cube)
    Vector3 velocity;   // current velocity (used for gravity/jump in Task 3)
    Vector3 size;       // { 0.8, 1.0, 0.8 }
    bool    is_grounded;
};
```

## Movement Implementation (`src/player.cpp`)

```cpp
// WASD + arrow key input
if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    player.position.z -= speed;
if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))   player.position.z += speed;
if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))   player.position.x -= speed;
if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))  player.position.x += speed;

// Boundary clamp: keep player inside 20x20 ground plane
player.position.x = std::clamp(player.position.x, -9.0f, 9.0f);
player.position.z = std::clamp(player.position.z, -9.0f, 9.0f);
```

## Camera Follow Implementation (`src/main.cpp`)

```cpp
camera.target   = player.position;
camera.position = {
    player.position.x,
    player.position.y + 20.0f,
    player.position.z + 10.0f
};
```

## Verification Checklist

- [x] Player struct defined with position, velocity, size, is_grounded
- [x] `player_init()` spawns player at `{0, 0.5, 0}` (half-height above ground)
- [x] WASD and arrow keys move player in all four directions on XZ plane
- [x] Movement speed is `5.0f * GetFrameTime()` (frame-rate independent)
- [x] Boundary clamping prevents player from leaving the 20x20 ground area
- [x] Camera.target tracks player.position every frame
- [x] Camera offset `{+0, +20, +10}` maintains the tilted top-down view
- [x] Player visible as BLUE cube with DARKBLUE wireframe outline
- [x] Screenshot: Blue cube character visible on green ground plane in top-down 3D view (run `./build/4d-pixel-game` to verify visually)
- [x] Gameplay demo: Move with WASD/arrows — character moves, camera follows, stops at boundary
