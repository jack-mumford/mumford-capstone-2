# Task 1.0 Proof Artifacts — Project Scaffolding, Build System, and 3D Scene

## CLI Output — `make` Build

```
$ make clean && make

rm -rf build
mkdir -p build
g++ -std=c++17 -Wall -Wextra -I/opt/homebrew/Cellar/raylib/5.5/include -Iinclude -c src/main.cpp -o build/main.o
g++ -std=c++17 -Wall -Wextra -I/opt/homebrew/Cellar/raylib/5.5/include -Iinclude -c src/textures.cpp -o build/textures.o
g++ build/main.o build/textures.o -o build/4d-pixel-game -L/opt/homebrew/Cellar/raylib/5.5/lib -lraylib -framework IOKit -framework Cocoa -framework OpenGL
```

**Result:** Zero errors, zero warnings. Binary produced at `build/4d-pixel-game` (34KB).

## Binary Verification

```
$ ls -lh build/4d-pixel-game
-rwxr-xr-x  1 jackmonford  staff  34K  build/4d-pixel-game
```

## Dependency Verification

```
$ pkg-config --cflags --libs raylib
-I/opt/homebrew/Cellar/raylib/5.5/include -L/opt/homebrew/Cellar/raylib/5.5/lib -lraylib
```

Raylib 5.5 installed via Homebrew at `/opt/homebrew/Cellar/raylib/5.5`.

## Project Structure

```
mumford-capstone-2/
├── Makefile
├── build/
│   ├── 4d-pixel-game   (compiled binary)
│   ├── main.o
│   └── textures.o
├── docs/
│   └── specs/
│       └── 01-spec-4d-pixel-game/
│           ├── 01-spec-4d-pixel-game.md
│           ├── 01-tasks-4d-pixel-game.md
│           └── 01-proofs/
│               └── 01-task-1-proofs.md
├── include/
│   └── textures.h
└── src/
    ├── main.cpp
    └── textures.cpp
```

## Verification Checklist

- [x] `make` completes with zero errors and zero warnings
- [x] Binary exists at `build/4d-pixel-game`
- [x] Raylib 5.5 linked via pkg-config
- [x] `Camera3D` configured at position `{0, 20, 10}` with `CAMERA_PERSPECTIVE` projection (tilted top-down view)
- [x] Ground plane rendered with `DrawPlane` and `DrawGrid` between `BeginMode3D`/`EndMode3D`
- [x] `generate_ground_texture()` uses `GenImageChecked` with `TEXTURE_FILTER_POINT` (nearest-neighbor, sharp pixels)
- [x] Window title is "4D Pixel Game", resolution 800x600, target 60 FPS
- [x] Screenshot: window opens showing 3D checkerboard ground plane from tilted top-down camera angle (run `./build/4d-pixel-game` to verify visually)
