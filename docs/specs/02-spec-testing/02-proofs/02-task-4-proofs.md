# 02 Task 4.0 Proofs — Unit Test CI Job

## Workflow Diff

Added `test` job before `build-and-deploy` in `.github/workflows/web-deploy.yml`:

```yaml
jobs:
  test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - name: Install Raylib (for compiling and linking test binary)
        run: sudo apt-get update && sudo apt-get install -y libraylib-dev
      - name: Build game objects
        run: make all
      - name: Run unit tests
        run: make test-unit

  build-and-deploy:
    needs: [test]          # ← deployment blocked until tests pass
    runs-on: ubuntu-latest
    ...
```

## Makefile Platform Fix

Removed macOS-only `-framework` flags from the unconditional `LDFLAGS` and made them platform-conditional so `make all` and `make test-unit` work on both macOS and Ubuntu:

```makefile
UNAME_S  := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
  PLATFORM_LDFLAGS := -framework IOKit -framework Cocoa -framework OpenGL
else
  PLATFORM_LDFLAGS :=
endif
LDFLAGS  := $(shell pkg-config --libs raylib) $(PLATFORM_LDFLAGS)
```

## Local Verification — `make test-unit` (macOS)

```
g++ -std=c++17 -I/opt/homebrew/Cellar/raylib/5.5/include -Iinclude -Itests/unit/catch2 \
    tests/unit/catch2/catch_amalgamated.cpp tests/unit/test_collision.cpp \
    tests/unit/test_combat.cpp tests/unit/test_item.cpp tests/unit/test_player.cpp \
    build/combat.o build/item.o build/level.o \
    -o build/test-runner -L/opt/homebrew/Cellar/raylib/5.5/lib -lraylib \
    -framework IOKit -framework Cocoa -framework OpenGL
./build/test-runner
Randomness seeded to: 272638616
===============================================================================
All tests passed (18 assertions in 14 test cases)
```

## GitHub Actions

After pushing master, the Actions run at `https://github.com/jack-mumford/mumford-capstone-2/actions` shows:
- `test` job runs `make test-unit` on `ubuntu-latest`
- `build-and-deploy` job shows `needs: [test]` in the workflow graph — deployment is blocked if tests fail

## Verification

- `make test-unit` passes locally after the platform-conditional `LDFLAGS` change (18 assertions in 14 test cases)
- `build-and-deploy` has `needs: [test]` — a failing unit test blocks the deployment job
- The CI workflow installs `libraylib-dev` via apt to provide headers and linker symbols for the test binary
