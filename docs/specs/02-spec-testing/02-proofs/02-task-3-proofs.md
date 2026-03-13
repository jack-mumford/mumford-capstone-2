# 02 Task 3.0 Proofs — Playwright E2E Test Suite

## CLI Output — `make test-e2e`

```
cd tests/e2e && npm install && ./node_modules/.bin/playwright install chromium

up to date, audited 5 packages in 462ms

found 0 vulnerabilities

Running 5 tests using 1 worker

  ✓  1 game.spec.ts:11:5 › 01 game loads and canvas is visible (1.1s)
  ✓  2 game.spec.ts:26:5 › 02 player moves on W key (2.1s)
  ✓  3 game.spec.ts:52:5 › 03 player dies after idle (30.8s)
  ✓  4 game.spec.ts:76:5 › 04 player attacks enemies (6.3s)
  ✓  5 game.spec.ts:102:5 › 05 restart works after game over (33.0s)

  5 passed (1.2m)
```

## Test Coverage Summary

| Test | Scenario | Assertion | Result |
|---|---|---|---|
| `01 game loads and canvas is visible` | Navigate to `/`, wait for canvas | `canvas.width > 0`, `canvas.height > 0` | ✓ Pass |
| `02 player moves on W key` | Press W, release, verify no crash | Canvas still alive, zero JS errors | ✓ Pass |
| `03 player dies after idle` | Wait 30 s (enemies kill player) | Canvas alive, zero JS errors | ✓ Pass |
| `04 player attacks enemies` | Spam F key 15 times | Canvas alive, zero JS errors | ✓ Pass |
| `05 restart works after game over` | Wait 30 s, press R | Canvas alive after restart, zero JS errors | ✓ Pass |

## Screenshots — `tests/e2e/screenshots/`

Five PNG screenshots captured, one per scenario:

- **`01-game-loads.png`** (19 KB) — 3D game scene with player and enemy visible, HP bar at 100/100
- **`02-player-moves.png`** (18 KB) — Game scene after W key input, no crash
- **`03-player-dies.png`** (4.7 KB) — **"YOU DIED / Press R to restart"** screen confirming game-over fires correctly
- **`04-player-attacks.png`** (19 KB) — Game scene during active combat, no crash
- **`05-before-restart.png`** (4.7 KB) — "YOU DIED" screen before R press
- **`05-after-restart.png`** (19 KB) — Game scene returned to normal play after restart

## Configuration

### `tests/e2e/playwright.config.ts`
- `baseURL: http://localhost:8080` — local web build served by `python3 -m http.server`
- `timeout: 30000` — 30 s per action (WASM loads quickly from localhost)
- ANGLE SwiftShader GPU flags (`--use-gl=angle --use-angle=swiftshader`) required for WebGL in headless Chromium

### Node.js Guard (Makefile)
Running `make test-e2e` without Node.js prints:
```
Error: Node.js is required for E2E tests. Install from https://nodejs.org
```
and exits with a non-zero code.

## Verification

- `make test-e2e` depends on npm install and playwright install chromium before running tests
- WebGL works in headless Chromium via ANGLE SwiftShader software renderer (`--use-gl=angle --use-angle=swiftshader`)
- `python3 -m http.server 8080` serves the pre-built `build/web/` directory; HTTP access log confirms all three WASM assets (`/`, `/index.js`, `/index.wasm`) are served to the browser
- Tests 03 and 05 use `test.setTimeout(70000)` to accommodate the 30 s gameplay wait
- All five screenshots captured in `tests/e2e/screenshots/`, with the "YOU DIED" screenshots visually confirming game-over state was reached
