# 02-validation-testing.md — Spec 02 Validation Report

**Validation Completed:** 2026-03-13
**Validation Performed By:** Claude Sonnet 4.6
**Spec:** `02-spec-testing.md`
**Task List:** `02-tasks-testing.md`

---

## 1) Executive Summary

**Overall: FAIL** — Gate A tripped by 3 HIGH issues (FR-2.4, FR-2.6, FR-2.7: E2E assertion weaker than spec FRs)

**Implementation Ready: No** — Core functionality is fully working; 3 E2E assertions need to be brought into alignment with spec FRs (or spec FRs updated to match the adopted behavioral approach). One FR-1.6 sub-requirement (attack-reduces-health test) is also absent. No CRITICAL issues were found; all gate failures are HIGH and actionable.

**Key Metrics:**

| Metric | Value |
|---|---|
| Functional Requirements Verified | 17 / 21 (81%) |
| Proof Artifacts Working | 4 / 4 (100%) |
| Files Expected vs Changed | 16 expected, 16 changed (100%) |
| Unit Tests Passing | 18 / 18 assertions, 14 / 14 test cases |
| E2E Tests Passing | 5 / 5 |
| CI Jobs Green | ✓ `test` + `build-and-deploy` |

---

## 2) Coverage Matrix

### Functional Requirements

| ID | Requirement | Status | Evidence |
|---|---|---|---|
| FR-1.1 | Catch2 v3 single-header in `tests/unit/` | **Verified** | `tests/unit/catch2/catch_amalgamated.hpp` + `.cpp` exist; commit `b6e029e` |
| FR-1.2 | `make test-unit` compiles & runs without opening window | **Verified** | Proof 2: `All tests passed (18 assertions in 14 test cases)`, no window opened |
| FR-1.3 | `item_in_pickup_range` tests: in-range, out-of-range, already-collected | **Verified** | `tests/unit/test_item.cpp` — 3 TEST_CASEs, 3 assertions; commit `b6e029e` |
| FR-1.4 | `check_platform_collision` tests: lands, misses XZ, player below | **Verified** | `tests/unit/test_collision.cpp` — 3 TEST_CASEs, 7 assertions; Proof 2 table |
| FR-1.5 | Nearest-enemy tests: single alive, multiple with closer, all dead | **Verified** | `tests/unit/test_combat.cpp:12-33` — 3 TEST_CASEs |
| FR-1.6 | Damage calculation tests: **attack reduces health** + slam AoE radius in/out | **Partial** | Slam radius math covered (`test_combat.cpp:36-46`). **No test for ATTACK_DAMAGE reducing enemy health**. FR partially unmet. |
| FR-1.7 | Player boundary clamping tests: position clamped to [-9, 9] | **Verified** | `tests/unit/test_player.cpp` — 3 TEST_CASEs (max, min, in-bounds) |
| FR-1.8 | All test functions pure: no Raylib window, no `InitWindow` | **Verified** | Proof 2: "No window opened"; `BOUNDARY_MIN`/`MAX` exposed as `inline constexpr` in `player.h:7-8` |
| FR-2.1 | `tests/e2e/` dir with `package.json` declaring Playwright | **Verified** | `tests/e2e/package.json` exists; `"@playwright/test": "^1.44.0"` |
| FR-2.2 | HTTP server for `build/web/` as part of `make test-e2e` | **Verified** | Makefile: `python3 -m http.server 8080 --directory build/web`; Proof 3 shows HTTP access log |
| FR-2.3 | E2E test: canvas visible with non-zero dimensions (Game loads) | **Verified** | `game.spec.ts:11` — `canvas.waitFor`, `box.width > 0`, `box.height > 0`; 590 ms pass |
| FR-2.4 | E2E test: W key input + **canvas pixel state changes between frames** | **Partial** | `game.spec.ts:26` — W key sent, canvas alive, zero JS errors asserted. **Pixel comparison NOT implemented.** Spec FR says "asserts the canvas pixel state changes between frames"; implementation uses behavioral assertion only. |
| FR-2.5 | E2E test: player dies (wait for enemies to kill idle player) | **Verified** | `game.spec.ts:52` — 30 s wait; Proof 3 screenshot `03-player-dies.png` shows "YOU DIED" screen visually |
| FR-2.6 | E2E test: repeated F key presses + **canvas changes** (Player kills enemy) | **Partial** | `game.spec.ts:76` — 15 F-key presses sent, canvas alive, zero JS errors asserted. **Pixel comparison NOT implemented.** Spec says "asserts the canvas changes". |
| FR-2.7 | E2E test: game-over → R key → **"YOU DIED" overlay disappears** | **Partial** | `game.spec.ts:102` — R key pressed, canvas alive, zero JS errors. **"YOU DIED" overlay disappearance not directly asserted.** Screenshots show before/after but no programmatic assertion. |
| FR-2.8 | Screenshot of canvas at end of each test scenario | **Verified** | Proof 3: 6 PNG files in `tests/e2e/screenshots/` (19 KB game scenes, 4.7 KB game-over screens) |
| FR-3.1 | `test` job added to `web-deploy.yml` before `build-and-deploy` | **Verified** | `web-deploy.yml:18-48`; commit `08a1bfd` |
| FR-3.2 | `test` job installs dependency and runs `make test-unit` | **Verified** | `web-deploy.yml:23-48`: builds Raylib 5.5 from source, `make all`, `make test-unit` |
| FR-3.3 | `build-and-deploy` lists `test` in `needs:` | **Verified** | `web-deploy.yml:51`: `needs: [test]`; CI run 23072373469 confirms gate active |
| FR-3.4 | CI runs on `ubuntu-latest`, no GUI required | **Verified** | `web-deploy.yml:19`: `runs-on: ubuntu-latest`; Proof 4: `test` job green in 1m9s |

**FR count: 17 Verified, 3 Partial (FR-2.4, FR-2.6, FR-2.7), 1 Partial (FR-1.6 sub-requirement) = 4 partials, 0 Unknown**

---

### Repository Standards

| Standard Area | Status | Evidence & Compliance Notes |
|---|---|---|
| GNU Make targets follow existing pattern | **Verified** | `test-unit` and `test-e2e` use same variable-expansion style as `all`/`web`; `.PHONY` updated |
| C++17 standard | **Verified** | `CXXFLAGS := -std=c++17`; test compile uses `-std=c++17` |
| No runtime deps added to main binary | **Verified** | Catch2 only in `tests/unit/catch2/`; Playwright in `tests/e2e/node_modules/` (gitignored) |
| Commit message convention (`type: description`) | **Verified** | Commits: `refactor:`, `feat:`, `fix:`, `docs:` — consistent with repo history |
| `.gitignore` entries for test artifacts | **Partial** | `tests/e2e/node_modules/`, `tests/e2e/test-results/`, `tests/e2e/screenshots/` added. `test-results/` (root-level, created by Playwright when run from project root during debugging) is untracked and absent from `.gitignore`. LOW severity. |
| `src/combat.cpp` auto-included via `wildcard` | **Verified** | Makefile: `SRCS := $(wildcard $(SRC_DIR)/*.cpp)` picks up `src/combat.cpp` automatically |

---

### Proof Artifacts

| Task | Proof Artifact | Status | Verification Result |
|---|---|---|---|
| 1.0 | CLI: `make` + `make web` succeed with zero errors | **Verified** | Proof file `02-task-1-proofs.md` exists; `combat.o` in link step; native + WASM build both green |
| 2.0 | CLI: `make test-unit` → `All tests passed (18 assertions in 14 test cases)` | **Verified** | Proof file `02-task-2-proofs.md` exists; live run confirms: `All tests passed (18 assertions in 14 test cases)` (seed 4176126875) |
| 3.0 | CLI: `make test-e2e` shows 5/5 passing; screenshots in `tests/e2e/screenshots/` | **Verified** | Proof file `02-task-3-proofs.md` exists; 6 PNGs in `screenshots/` (19 KB game scenes, 4.7 KB YOU DIED screens); ANGLE SwiftShader enables headless WebGL |
| 4.0 | GitHub Actions: `test` job green, `build-and-deploy` blocked by `test` | **Verified** | Proof file `02-task-4-proofs.md` exists; CI run 23072373469: `test` ✓ 1m9s → `build-and-deploy` ✓ 54s; `needs: [test]` confirmed in `web-deploy.yml:51` |

---

## 3) Validation Issues

| Severity | Issue | Impact | Recommendation |
|---|---|---|---|
| **HIGH** | **FR-2.4 assertion weaker than spec.** `game.spec.ts:26-45` sends W key but checks only `canvas.alive + jsErrors.length == 0`. Spec FR-2.4 requires "asserts the canvas pixel state changes between frames". Evidence: task list updated to drop pixel comparison, but spec FR not updated. | Player movement cannot be verified programmatically; test may pass even if movement input is silently ignored. | **Option A (preferred):** Add pixel comparison back — ANGLE SwiftShader is confirmed working (WebGL renders, screenshots show 19 KB game scenes). Replace or supplement `jsErrors` assertion with `Buffer.compare(before, after) !== 0`. **Option B:** Update spec FR-2.4 to reflect the adopted behavioral approach and document rationale. |
| **HIGH** | **FR-2.6 assertion weaker than spec.** `game.spec.ts:76-97` spams F key but checks only `canvas.alive + zero JS errors`. Spec FR-2.6 requires "asserts the canvas changes (Player kills enemy)". | Attack input cannot be verified to produce any game-state change. | Same as FR-2.4: add pixel comparison, or update spec FR-2.6 to match behavioral assertion. |
| **HIGH** | **FR-2.7 assertion weaker than spec.** `game.spec.ts:102-126` presses R after game-over but only asserts `canvas.boundingBox().width > 0`. Spec FR-2.7 requires "asserts the 'YOU DIED' overlay disappears". | Restart cannot be programmatically confirmed; test passes even if game is frozen on YOU DIED screen (canvas still alive). | Use `page.evaluate()` to check canvas pixel center after R press (YOU DIED screen is dark, game scene is green), or compare `05-before-restart.png` pixel vs `05-after-restart.png` in the test body. Alternatively update spec FR. |
| **MEDIUM** | **FR-1.6 partial: "attack reduces health" test missing.** Spec FR-1.6 says "tests for damage calculation logic covering: attack reduces health, slam AoE hits/misses". `test_combat.cpp` covers slam radius math but has no test for `ATTACK_DAMAGE` being applied to `enemy.health`. | One half of FR-1.6 (attack damage application) is unverified by unit tests. | Add a `TEST_CASE` in `test_combat.cpp` that creates an `Enemy` with known health, applies `ATTACK_DAMAGE` subtraction, and asserts the result (pure math, no game loop needed). |
| **MEDIUM** | **Sub-tasks 1.x and 2.x not individually marked `[x]` in task file.** Parent tasks 1.0 and 2.0 are `[x]` but sub-tasks (1.1–1.5, 2.1–2.9) remain `[ ]`. | Task file does not accurately reflect completion state at sub-task level. | Mark all sub-tasks `[x]` for completed parent tasks 1.0 and 2.0 in `02-tasks-testing.md`. |
| **MEDIUM** | **Proof file 4.0 description inaccuracy.** `02-task-4-proofs.md` footer says "The CI workflow installs `libraylib-dev` via apt" but the actual workflow builds Raylib 5.5 from source via CMake (the apt package does not exist on Ubuntu 24.04). | Minor documentation inconsistency; could mislead future readers of the proof file. | Update the Verification section of `02-task-4-proofs.md` line 87–89 to accurately state "Raylib 5.5 is built from source via CMake and cached". |
| **LOW** | **Root-level `test-results/` directory untracked and not in `.gitignore`.** Created by Playwright during debugging runs from the project root; absent from `.gitignore`. | May be accidentally committed in future or clutter `git status`. | Add `test-results/` to `.gitignore`. |

---

## 4) Evidence Appendix

### A. Git Commits Analyzed

| Commit | Message | Relevant Files |
|---|---|---|
| `9a74a17` | docs: update Task 4.0 proofs with CI green run evidence | `02-task-4-proofs.md` |
| `91e6d86` | fix: build Raylib 5.5 from source in CI | `web-deploy.yml` (+20 lines) |
| `08a1bfd` | feat: add unit test CI job gating deployment | `web-deploy.yml`, `Makefile`, `02-task-4-proofs.md`, `02-tasks-testing.md` |
| `717c7ff` | feat: Playwright E2E test suite with headless WebGL support | `Makefile`, `02-task-3-proofs.md`, `02-tasks-testing.md`, `tests/e2e/*` |
| `b6e029e` | feat: Catch2 unit test harness with 14 test cases across 5 systems | `.gitignore`, `Makefile`, `02-task-2-proofs.md`, `include/player.h`, `src/player.cpp`, `tests/unit/**` |
| `d0a8eb0` | refactor: extract combat logic into combat.h / combat.cpp | `include/combat.h`, `src/combat.cpp`, `src/main.cpp`, `02-task-1-proofs.md`, spec + tasks |

All commits reference the correct spec/task. No unrelated changes found. All changed files appear in "Relevant Files" (or are doc/proof files scoped to the spec directory).

### B. Proof Artifact File Check

| File | Exists | Size / Content |
|---|---|---|
| `02-proofs/02-task-1-proofs.md` | ✓ | Build CLI output, WASM output |
| `02-proofs/02-task-2-proofs.md` | ✓ | `All tests passed (18 assertions in 14 test cases)` |
| `02-proofs/02-task-3-proofs.md` | ✓ | 5/5 Playwright passing, 6 screenshots listed |
| `02-proofs/02-task-4-proofs.md` | ✓ | CI run 23072373469 output, both jobs green |

### C. Live `make test-unit` Run (during validation)

```
g++ -std=c++17 -I/opt/homebrew/Cellar/raylib/5.5/include -Iinclude -Itests/unit/catch2 \
    tests/unit/catch2/catch_amalgamated.cpp tests/unit/test_collision.cpp \
    tests/unit/test_combat.cpp tests/unit/test_item.cpp tests/unit/test_player.cpp \
    build/combat.o build/item.o build/level.o \
    -o build/test-runner -L/opt/homebrew/Cellar/raylib/5.5/lib -lraylib \
    -framework IOKit -framework Cocoa -framework OpenGL
./build/test-runner
Randomness seeded to: 4176126875
===============================================================================
All tests passed (18 assertions in 14 test cases)
```

Exit code 0. No window opened.

### D. Relevant Files vs Changed Files

| File | In Task List | Changed | Notes |
|---|---|---|---|
| `include/combat.h` | ✓ NEW | ✓ commit `d0a8eb0` | 18 lines, 8 constants + `nearest_enemy` |
| `src/combat.cpp` | ✓ NEW | ✓ commit `d0a8eb0` | 14 lines, pure math |
| `src/main.cpp` | ✓ MODIFY | ✓ commit `d0a8eb0` | constants + func removed, `#include "combat.h"` added |
| `include/player.h` | ✓ MODIFY | ✓ commit `b6e029e` | `BOUNDARY_MIN`/`MAX` added as `inline constexpr` at lines 7–8 |
| `src/player.cpp` | ✓ MODIFY | ✓ commit `b6e029e` | `static const` removed |
| `Makefile` | ✓ MODIFY | ✓ commits `b6e029e`, `717c7ff`, `08a1bfd` | `test-unit`, `test-e2e`, platform LDFLAGS |
| `.gitignore` | ✓ MODIFY | ✓ commit `b6e029e` | test artifacts added |
| `tests/unit/catch2/catch_amalgamated.hpp` | ✓ NEW | ✓ commit `b6e029e` | Catch2 v3.7.1 |
| `tests/unit/catch2/catch_amalgamated.cpp` | ✓ NEW | ✓ commit `b6e029e` | Catch2 v3.7.1 |
| `tests/unit/test_item.cpp` | ✓ NEW | ✓ commit `b6e029e` | 3 test cases |
| `tests/unit/test_collision.cpp` | ✓ NEW | ✓ commit `b6e029e` | 3 test cases, 7 assertions |
| `tests/unit/test_combat.cpp` | ✓ NEW | ✓ commit `b6e029e` | 5 test cases |
| `tests/unit/test_player.cpp` | ✓ NEW | ✓ commit `b6e029e` | 3 test cases |
| `tests/e2e/package.json` | ✓ NEW | ✓ commit `717c7ff` | `@playwright/test: ^1.44.0` |
| `tests/e2e/playwright.config.ts` | ✓ NEW | ✓ commit `717c7ff` | ANGLE SwiftShader, headless |
| `tests/e2e/game.spec.ts` | ✓ NEW | ✓ commit `717c7ff` | 5 scenarios |
| `.github/workflows/web-deploy.yml` | ✓ MODIFY | ✓ commits `08a1bfd`, `91e6d86` | `test` job + `needs: [test]` |

**All 16 relevant files accounted for. No unexpected files changed.**

### E. Security Check

No API keys, tokens, passwords, or credentials found in any proof artifact file. All content is CLI output, test results, and GitHub Actions log excerpts.
