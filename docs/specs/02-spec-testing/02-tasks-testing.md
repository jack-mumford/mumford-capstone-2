# 02-tasks-testing.md

## Relevant Files

- `include/combat.h` — **NEW** — declares `nearest_enemy()` and defines all combat/slam constants moved out of `main.cpp`
- `src/combat.cpp` — **NEW** — implements `nearest_enemy()`
- `src/main.cpp` — **MODIFY** — remove static constants and `nearest_enemy`; add `#include "combat.h"`
- `include/player.h` — **MODIFY** — expose `BOUNDARY_MIN` / `BOUNDARY_MAX` constants so tests can reference them
- `Makefile` — **MODIFY** — add `test-unit` and `test-e2e` targets; `combat.cpp` is auto-picked up by the existing `wildcard`
- `.gitignore` — **MODIFY** — add test build artifacts (`tests/unit/test-runner`, `tests/e2e/node_modules/`, `tests/e2e/screenshots/`, `tests/e2e/test-results/`)
- `tests/unit/catch2/catch_amalgamated.hpp` — **NEW (downloaded)** — Catch2 v3 single-header
- `tests/unit/catch2/catch_amalgamated.cpp` — **NEW (downloaded)** — Catch2 v3 implementation unit
- `tests/unit/test_item.cpp` — **NEW** — unit tests for `item_in_pickup_range`
- `tests/unit/test_collision.cpp` — **NEW** — unit tests for `check_platform_collision`
- `tests/unit/test_combat.cpp` — **NEW** — unit tests for `nearest_enemy` and damage radius math
- `tests/unit/test_player.cpp` — **NEW** — unit tests for player boundary clamping
- `tests/e2e/package.json` — **NEW** — pins `@playwright/test` version
- `tests/e2e/playwright.config.ts` — **NEW** — Playwright config pointing to `localhost:8080`
- `tests/e2e/game.spec.ts` — **NEW** — all five E2E gameplay scenarios
- `.github/workflows/web-deploy.yml` — **MODIFY** — add `test` job; add `needs: [test]` to `build-and-deploy`

### Notes

- The test binary links against the installed Raylib (via `pkg-config`) so symbols from `item.cpp` and `level.cpp` (which contain draw calls) resolve correctly — but no window opens because `InitWindow` is never called in tests.
- `combat.cpp` is automatically included in `SRCS` via the existing `wildcard $(SRC_DIR)/*.cpp` in the Makefile; no manual addition is needed.
- `BOUNDARY_MIN` and `BOUNDARY_MAX` must be moved from `static const` inside `player.cpp` to `player.h` so the test file can see them without linking `player.cpp` (which calls Raylib heavily).
- Run `make test-unit` on macOS to verify; the CI job runs the same command on `ubuntu-latest` with Raylib installed via `apt`.

---

## Tasks

### [x] 1.0 Extract combat logic into `combat.h` / `combat.cpp`

#### 1.0 Proof Artifact(s)

- CLI: `make` succeeds with zero errors after the refactor demonstrates `combat.h`/`combat.cpp` integrate correctly with the existing native build
- CLI: `make web` succeeds with zero errors demonstrates the web build is unaffected

#### 1.0 Tasks

- [ ] 1.1 Create `include/combat.h` — declare `nearest_enemy(const Enemy[], int, Vector3, float)` and move all eight constants from `main.cpp` into this header: `ATTACK_RANGE`, `ATTACK_DAMAGE`, `ATTACK_COOLDOWN`, `ATTACK_DURATION`, `HURT_FLASH_TIME`, `SLAM_DAMAGE`, `SLAM_RADIUS`, `SHOCKWAVE_DURATION`; include `<raylib.h>` and `"enemy.h"` at the top
- [ ] 1.2 Create `src/combat.cpp` — `#include "combat.h"` and `#include <cmath>`, then paste in the `nearest_enemy` function body (removing its `static` keyword)
- [ ] 1.3 Update `src/main.cpp` — delete the eight `static const float` constants and the `nearest_enemy` function body; add `#include "combat.h"` below the existing includes
- [ ] 1.4 Run `make` and confirm it compiles cleanly with zero errors or warnings related to the refactor
- [ ] 1.5 Run `make web` and confirm it compiles cleanly

---

### [x] 2.0 Set up Catch2 unit test harness and write all unit tests

#### 2.0 Proof Artifact(s)

- CLI: `make test-unit` output ending with `All tests passed (N assertions in M test cases)` demonstrates the harness compiles and all logic tests pass
- CLI: `make test-unit` completes without opening a window demonstrates no Raylib window is created

#### 2.0 Tasks

- [ ] 2.1 Move `BOUNDARY_MIN` and `BOUNDARY_MAX` from `static const` inside `src/player.cpp` to `include/player.h` (as `inline constexpr float`) so tests can reference them without linking `player.cpp`
- [ ] 2.2 Create `tests/unit/catch2/` directory; download the Catch2 v3 amalgamated release from `https://github.com/catchorg/Catch2/releases` and place `catch_amalgamated.hpp` and `catch_amalgamated.cpp` inside it
- [ ] 2.3 Add the `test-unit` target to `Makefile`:
  - Compile `tests/unit/catch2/catch_amalgamated.cpp` and all `tests/unit/test_*.cpp` files
  - Link against `build/combat.o`, `build/item.o`, `build/level.o` (pre-built from the normal build) plus Raylib (`$(LDFLAGS)`)
  - Output binary to `build/test-runner`; run it immediately after linking
  - Example target shape: `make test-unit` depends on `all` (so object files exist), then compiles and runs `build/test-runner`
- [ ] 2.4 Add `build/test-runner` to `.gitignore`
- [ ] 2.5 Write `tests/unit/test_item.cpp` — include `"item.h"` and `catch_amalgamated.hpp`; write three `TEST_CASE` blocks:
  - Player within radius → `item_in_pickup_range` returns `true`
  - Player outside radius → returns `false`
  - Item already collected → returns `false` regardless of position
- [ ] 2.6 Write `tests/unit/test_collision.cpp` — include `"level.h"` and `"player.h"`; write three `TEST_CASE` blocks:
  - Player falling onto platform top → `check_platform_collision` returns `true`, `player.is_grounded` becomes `true`, `player.position.y` snaps to platform top + half player height
  - Player beside platform (no XZ overlap) → returns `false`, position unchanged
  - Player below platform (already past it) → returns `false`
- [ ] 2.7 Write `tests/unit/test_combat.cpp` — include `"combat.h"` and `"enemy.h"`; write four `TEST_CASE` blocks:
  - Single alive enemy within range → `nearest_enemy` returns index `0`
  - Two enemies, closer one is index 1 → returns `1`
  - All enemies dead → returns `-1`
  - Slam AoE: enemy within `SLAM_RADIUS` → health reduced by `SLAM_DAMAGE`; enemy outside radius → health unchanged (test the math directly, not via game loop)
- [ ] 2.8 Write `tests/unit/test_player.cpp` — include `"player.h"` and `<algorithm>`; write two `TEST_CASE` blocks using `std::clamp` with `BOUNDARY_MIN`/`BOUNDARY_MAX`:
  - Position at `BOUNDARY_MAX + 1.0f` clamped → equals `BOUNDARY_MAX`
  - Position at `BOUNDARY_MIN - 1.0f` clamped → equals `BOUNDARY_MIN`
- [ ] 2.9 Run `make test-unit` and fix any compile errors; confirm all test cases pass and the binary exits without opening a window

---

### [ ] 3.0 Set up Playwright E2E test suite

#### 3.0 Proof Artifact(s)

- CLI: `make test-e2e` output showing all five Playwright tests passing demonstrates the web build loads and responds to input correctly
- Screenshots: `tests/e2e/screenshots/` contains five PNG files (one per scenario) demonstrating each game state was reached
- CLI: Running `make test-e2e` without Node.js installed prints the Node.js error message and exits with a non-zero code demonstrates the guard works

#### 3.0 Tasks

- [ ] 3.1 Create `tests/e2e/package.json` declaring `@playwright/test` as a dev dependency; pin a specific version (e.g., `"@playwright/test": "^1.44.0"`)
- [ ] 3.2 Create `tests/e2e/playwright.config.ts`:
  - Set `baseURL` to `http://localhost:8080`
  - Set `timeout` to `30000` (30 s — WASM loads slowly)
  - Set `use.headless` to `true`
  - Configure `outputDir` to `tests/e2e/test-results/`
- [ ] 3.3 Write `tests/e2e/game.spec.ts` with five `test()` blocks, each taking a screenshot at the end to `tests/e2e/screenshots/<scenario>.png`:
  - **Game loads** — navigate to `/`, wait for `canvas` selector, assert `canvas.width > 0` and `canvas.height > 0`
  - **Player moves** — wait for canvas, take screenshot A, hold `W` for 500 ms, take screenshot B, assert screenshots A and B differ (pixel comparison via `Buffer.compare`)
  - **Player dies** — navigate to `/`, hold no keys for ~20 s (enemies walk to player and attack), wait for text `"YOU DIED"` to appear via `page.locator('text=YOU DIED')` or by checking the canvas rendered text with `page.evaluate`
  - **Player kills enemy** — navigate to `/`, position player near enemy spawn (send repeated `F` presses for 3 s), screenshot before and after; assert pixel change (enemy health bar area becomes dark)
  - **Restart works** — navigate to `/`, wait for "YOU DIED" (same as above), press `R`, assert "YOU DIED" is no longer visible and canvas is rendering again
- [ ] 3.4 Add `test-e2e` target to `Makefile`:
  ```makefile
  test-e2e:
  	@command -v node >/dev/null 2>&1 || { echo "Error: Node.js is required for E2E tests. Install from https://nodejs.org"; exit 1; }
  	@command -v npm  >/dev/null 2>&1 || { echo "Error: Node.js is required for E2E tests. Install from https://nodejs.org"; exit 1; }
  	cd tests/e2e && npm install && npx playwright install chromium && \
  	  python3 -m http.server 8080 --directory ../../build/web & \
  	  SERVER_PID=$$!; \
  	  npx playwright test; TEST_EXIT=$$?; \
  	  kill $$SERVER_PID; exit $$TEST_EXIT
  ```
- [ ] 3.5 Update `.gitignore` — add `tests/e2e/node_modules/`, `tests/e2e/test-results/`, `tests/e2e/screenshots/`, and `tests/e2e/.playwright/`
- [ ] 3.6 Run `make web` to produce a fresh build, then run `make test-e2e`; fix any failures; confirm all five tests pass and five screenshots exist in `tests/e2e/screenshots/`

---

### [ ] 4.0 Add unit test CI job to GitHub Actions

#### 4.0 Proof Artifact(s)

- GitHub Actions run: `test` job shows green at `github.com/jack-mumford/mumford-capstone-2/actions` demonstrates unit tests execute and pass in CI
- GitHub Actions run: workflow graph shows `build-and-deploy` blocked by `test` in the job dependency chain demonstrates a failing test would block deployment

#### 4.0 Tasks

- [ ] 4.1 Add a `test` job to `.github/workflows/web-deploy.yml` **before** `build-and-deploy`, with this shape:
  ```yaml
  test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - name: Install Raylib (for linking test binary)
        run: sudo apt-get update && sudo apt-get install -y libraylib-dev
      - name: Build game objects
        run: make all
      - name: Run unit tests
        run: make test-unit
  ```
- [ ] 4.2 Add `needs: [test]` to the `build-and-deploy` job so it cannot start until `test` passes
- [ ] 4.3 Update `.PHONY` in the Makefile to include `test-unit` and `test-e2e`
- [ ] 4.4 Commit all changes (`combat.h`/`.cpp`, test files, Makefile, `.gitignore`, workflow) and push to `master`; watch the Actions run and confirm both `test` and `build-and-deploy` jobs go green
