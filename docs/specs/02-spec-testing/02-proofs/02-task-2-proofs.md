# 02 Task 2.0 Proofs — Catch2 Unit Test Harness and Unit Tests

## CLI Output — `make test-unit`

```
g++ -std=c++17 -I/opt/homebrew/Cellar/raylib/5.5/include -Iinclude -Itests/unit/catch2 \
    tests/unit/catch2/catch_amalgamated.cpp \
    tests/unit/test_collision.cpp \
    tests/unit/test_combat.cpp \
    tests/unit/test_item.cpp \
    tests/unit/test_player.cpp \
    build/combat.o build/item.o build/level.o \
    -o build/test-runner -L/opt/homebrew/.../lib -lraylib -framework IOKit -framework Cocoa -framework OpenGL
./build/test-runner
Randomness seeded to: 2219087686
===============================================================================
All tests passed (18 assertions in 14 test cases)
```

No window opened. Terminal returned to prompt immediately after printing results.

## Test Coverage Summary

| File | Test Cases | Assertions | Systems Covered |
|---|---|---|---|
| `test_item.cpp` | 3 | 3 | `item_in_pickup_range` — in-range, out-of-range, collected |
| `test_collision.cpp` | 3 | 7 | `check_platform_collision` — lands, misses XZ, misses below |
| `test_combat.cpp` | 5 | 5 | `nearest_enemy` — single, closest-of-two, all-dead; slam radius in/out |
| `test_player.cpp` | 3 | 3 | Boundary clamp — max, min, in-bounds |
| **Total** | **14** | **18** | All 5 systems from spec |

## Verification

- `make test-unit` depends on `all`, so game object files are always current before tests run
- Test binary links against Raylib to resolve draw symbols in `item.o`/`level.o`; `InitWindow` is never called → zero windows opened
- `BOUNDARY_MIN`/`BOUNDARY_MAX` moved to `include/player.h` as `inline constexpr float` — accessible to test code without linking `player.cpp`
- Catch2 v3.7.1 amalgamated files in `tests/unit/catch2/` — no package manager required
