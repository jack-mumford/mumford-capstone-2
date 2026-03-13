# 02 Task 1.0 Proofs — Extract combat logic into `combat.h` / `combat.cpp`

## CLI Output — Native Build

```
g++ -std=c++17 -Wall -Wextra ... -c src/combat.cpp   -o build/combat.o
g++ -std=c++17 -Wall -Wextra ... -c src/enemy.cpp    -o build/enemy.o
g++ -std=c++17 -Wall -Wextra ... -c src/item.cpp     -o build/item.o
g++ -std=c++17 -Wall -Wextra ... -c src/level.cpp    -o build/level.o
g++ -std=c++17 -Wall -Wextra ... -c src/main.cpp     -o build/main.o
g++ -std=c++17 -Wall -Wextra ... -c src/player.cpp   -o build/player.o
g++ -std=c++17 -Wall -Wextra ... -c src/textures.cpp -o build/textures.o
g++ build/combat.o build/enemy.o build/item.o build/level.o build/main.o build/player.o build/textures.o -o build/4d-pixel-game ...
```

`make` completed with zero errors and zero warnings. `combat.o` appears in the link step, confirming `src/combat.cpp` is compiled and linked correctly.

## CLI Output — Web Build

```
emcc src/combat.cpp src/enemy.cpp src/item.cpp src/level.cpp src/main.cpp src/player.cpp src/textures.cpp -o build/web/index.html ...
```

`make web` completed with zero errors. Output:

```
build/web/index.html   2.4K
build/web/index.wasm   156K
build/web/index.js     180K
```

## Verification

- `include/combat.h` declares `nearest_enemy()` and all eight constants (`ATTACK_RANGE`, `ATTACK_DAMAGE`, `ATTACK_COOLDOWN`, `ATTACK_DURATION`, `HURT_FLASH_TIME`, `SLAM_DAMAGE`, `SLAM_RADIUS`, `SHOCKWAVE_DURATION`)
- `src/combat.cpp` implements `nearest_enemy()` with no Raylib calls — pure math only
- `src/main.cpp` no longer contains the eight `static const float` constants or the `nearest_enemy` function body; it `#include "combat.h"` instead
- Both native and web builds succeed with zero errors, demonstrating the refactor is source-compatible with all build targets
