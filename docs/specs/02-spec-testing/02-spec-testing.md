# 02-spec-testing.md

## Introduction/Overview

Add a two-tier automated test suite to the 4D Pixel Game: unit tests using Catch2 for pure game logic, and E2E tests using Playwright against the WebAssembly build running in a real browser. Unit tests run in CI on every push; E2E tests run locally on demand. Together they provide confidence that core game mechanics are correct and that the deployed web build is playable.

## Goals

- Introduce Catch2 as the unit test framework and wire it to a `make test-unit` target
- Cover all major game logic systems (player, combat, enemy AI, collision, items) with unit tests
- Introduce Playwright to automate the browser build for E2E scenarios
- Run unit tests automatically in GitHub Actions CI on every push to master
- Keep E2E tests runnable locally with a single command (`make test-e2e`)

## User Stories

**As a developer**, I want to run `make test-unit` so that I can verify game logic is correct without launching the game.

**As a developer**, I want to run `make test-e2e` so that I can confirm the web build loads and responds to input correctly in a real browser.

**As a developer**, I want unit tests to run in CI on every push so that regressions are caught before they reach the deployed GitHub Pages site.

**As a developer**, I want clear pass/fail output for every test so that I know exactly which system is broken when a test fails.

## Demoable Units of Work

### Unit 1: Unit Test Harness and Pure Logic Tests

**Purpose:** Prove that Catch2 is integrated, the build compiles a separate test binary, and the already-pure game logic functions are covered by passing tests.

**Functional Requirements:**
- The system shall include Catch2 v3 as a single-header dependency in `tests/unit/`
- The system shall compile a test binary via `make test-unit` that does not open a window
- The system shall provide tests for `item_in_pickup_range` covering: in-range, out-of-range, and already-collected cases
- The system shall provide tests for `check_platform_collision` covering: landing on top, falling past the side, and player below the platform
- The system shall provide tests for nearest-enemy selection logic covering: single alive enemy, multiple enemies with one closer, and all enemies dead
- The system shall provide tests for damage calculation logic covering: attack reduces health, slam AoE hits enemies within radius and misses outside
- The system shall provide tests for player boundary clamping: position is clamped to [-9, 9] on X and Z
- All test functions shall be pure (no Raylib window, no `InitWindow` call); any logic currently mixed with Raylib input calls shall be extracted into standalone helper functions

**Proof Artifacts:**
- CLI: `make test-unit` output shows all test cases passing with Catch2's summary line (e.g., `All tests passed (N assertions in M test cases)`) demonstrates the harness works and logic is correct

### Unit 2: E2E Test Suite with Playwright

**Purpose:** Prove the web build loads without crash and responds to keyboard input correctly in a real browser, covering all five defined gameplay scenarios.

**Functional Requirements:**
- The system shall include a `tests/e2e/` directory with a `package.json` that declares Playwright as a dev dependency
- The system shall serve `build/web/` on a local HTTP server as part of `make test-e2e` (e.g., via `npx serve` or `python -m http.server`)
- The system shall have an E2E test that asserts the canvas element is visible and has non-zero dimensions after page load (Game loads and renders)
- The system shall have an E2E test that sends `W` key input and asserts the canvas pixel state changes between frames (Player moves)
- The system shall have an E2E test that waits for the "YOU DIED" text to appear in the DOM or canvas (Player takes damage and dies) — achieved by waiting long enough for enemies to kill a stationary player
- The system shall have an E2E test that sends repeated `F` key presses near an enemy spawn position and asserts the canvas changes (Player kills enemy)
- The system shall have an E2E test that triggers game-over, presses `R`, and asserts the "YOU DIED" overlay disappears (Restart works)
- The system shall output a screenshot of the canvas at the end of each test scenario as a Playwright artifact

**Proof Artifacts:**
- CLI: `make test-e2e` output shows all Playwright tests passing
- Screenshots: Playwright-generated canvas screenshots for each of the five scenarios demonstrate the game state at the end of each test

### Unit 3: CI Integration for Unit Tests

**Purpose:** Prove that unit tests run automatically on every push to master and block merges when tests fail.

**Functional Requirements:**
- The system shall add a `test` job to `.github/workflows/web-deploy.yml` that runs before the `build-and-deploy` job
- The `test` job shall install the Catch2 dependency and run `make test-unit`
- The `build-and-deploy` job shall list `test` in its `needs:` so deployment is blocked if unit tests fail
- The CI shall run on `ubuntu-latest` and require no GUI or display server

**Proof Artifacts:**
- GitHub Actions run: passing `test` job in the workflow run list at `github.com/jack-mumford/mumford-capstone-2/actions` demonstrates unit tests execute and pass in CI

## Non-Goals (Out of Scope)

1. **E2E tests in CI**: Playwright E2E tests will not run in GitHub Actions — they require a real browser and serve overhead that complicates CI; they are a local-only tool for this spec.
2. **Mocking Raylib input functions**: `player_update` and `enemies_update` call `IsKeyDown`, `GetFrameTime`, etc. Full unit coverage of these functions would require a Raylib mock layer — that is out of scope. Only logic extracted into pure helper functions will be unit tested.
3. **Visual regression / screenshot diffing**: Screenshot comparison against a stored baseline is not included. Playwright screenshots are for human review only.
4. **Code coverage reporting**: No coverage tooling (lcov, gcov) is added in this spec.
5. **Performance benchmarks**: No timing or frame-rate assertions.

## Design Considerations

No specific UI/UX requirements. Test output is CLI-based. Playwright screenshots are saved to `tests/e2e/screenshots/` and are gitignored.

## Repository Standards

- Build system is GNU Make; new targets (`test-unit`, `test-e2e`) must follow the existing Makefile pattern
- C++ standard is C++17; test code must compile with `-std=c++17`
- No new runtime dependencies may be added to the main game binary; Catch2 and Playwright are test-only
- Commit messages follow the existing `type: short description` convention
- New directories (`tests/unit/`, `tests/e2e/`) should be added to `.gitignore` for their build artifacts (`tests/unit/test-runner`, `tests/e2e/node_modules/`, `tests/e2e/screenshots/`)

## Technical Considerations

- **Catch2 v3**: Use the single-header amalgamation (`catch_amalgamated.hpp` / `.cpp`) placed in `tests/unit/catch2/` to avoid a package manager dependency. The test binary links only against the game's object files (no Raylib) — this means any function under test must not call Raylib directly. Logic mixed with Raylib calls must be refactored into pure helper functions first.
- **New `combat.h` / `combat.cpp` module**: `nearest_enemy` (currently `static` in `main.cpp`) and all damage/range constants (`ATTACK_RANGE`, `ATTACK_DAMAGE`, `SLAM_RADIUS`, etc.) shall be moved into `include/combat.h` and `src/combat.cpp`. `main.cpp` and the test binary both include `combat.h`. This is the only source-level refactor required before tests can be written.
- **Playwright**: Node.js / npm required locally. `tests/e2e/package.json` pins the Playwright version. `make test-e2e` runs `npm install && npx playwright test`.
- **Node.js check in `make test-e2e`**: The Makefile target shall check for `node` and `npm` before proceeding and print a clear error message (`"Error: Node.js is required for E2E tests. Install from https://nodejs.org"`) with a non-zero exit code if either is missing.
- **Local HTTP server for E2E**: Playwright tests require an HTTP server (file:// won't work for WASM). The Makefile target should start one on a fixed port (e.g., 8080), run tests, then stop it.
- **Canvas pixel assertions**: Playwright's `page.screenshot()` captures the canvas. Frame-to-frame comparison can detect movement. For text assertions ("YOU DIED"), use `page.evaluate()` to read the canvas or rely on a known timing window.

## Security Considerations

- No API keys or credentials are introduced.
- Playwright screenshots may capture game state — they are gitignored and never committed.
- No specific security considerations beyond standard open-source practices.

## Success Metrics

1. **`make test-unit` passes**: All unit test cases pass with zero failures on a clean checkout on macOS
2. **`make test-e2e` passes**: All five Playwright scenarios pass against a freshly built `build/web/`
3. **CI green**: The `test` job passes on every push to master with no manual intervention
4. **Zero window opens during unit tests**: `make test-unit` completes without launching a Raylib window

## Open Questions

No open questions at this time.
