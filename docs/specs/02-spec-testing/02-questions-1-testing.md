# 02 Questions Round 1 - Testing

Please answer each question below (select one or more options, or add your own notes). Feel free to add additional context under any question.

---

## 1. Unit Test Framework

Which unit testing framework would you like to use for the C++ code?

- [x] (A) **Catch2** — header-only, easy to set up, widely used in game/C++ projects
- [ ] (B) **Google Test (gtest)** — more verbose but industry-standard; good IDE integration
- [ ] (C) **doctest** — minimal, fast compile times, similar syntax to Catch2
- [ ] (D) No preference — pick whatever fits best

---

## 2. What to Unit Test

Which systems should have unit tests? (Select all that apply)

- [ ] (A) **Player logic** — movement, jump/gravity, dash, slam physics
- [ ] (B) **Combat logic** — attack range, damage calculation, blocking
- [ ] (C) **Enemy AI** — chase behavior, attack detection, separation
- [ ] (D) **Platform collision** — landing, standing on edges, falling off
- [ ] (E) **Item system** — pickup range detection (`item_in_pickup_range`)
- [x] (F) All of the above

---

## 3. E2E Testing Approach

The game runs natively on macOS and as WebAssembly in the browser. Which E2E approach fits best?

- [x] (A) **Browser E2E (Playwright)** — automate the web build; screenshot the canvas, simulate keyboard input, assert game state via visual checks or JS hooks
- [ ] (B) **Headless native simulation** — drive the game loop directly in test code without opening a window (no Raylib renderer, stub out drawing calls)
- [ ] (C) **Screenshot regression** — render a frame, save it, compare against a baseline image on each run
- [ ] (D) No strong preference — recommend what makes the most sense for this stack

---

## 4. E2E Test Scenarios

Which gameplay scenarios should E2E tests cover? (Select all that apply)

- [x] (A) **Game loads and renders** — page/window opens without crash, canvas is visible
- [x] (B) **Player moves** — keyboard input produces visible player movement
- [x] (C) **Player takes damage and dies** — enemy hits player, health depletes, game-over screen appears
- [x] (D) **Player kills enemy** — attack connects, enemy health reaches zero, enemy disappears
- [x] (E) **Restart works** — pressing R on game-over screen resets game state correctly

---

## 5. CI Integration

Should tests run automatically in GitHub Actions on every push?

- [ ] (A) Yes — unit tests and E2E tests both run in CI on every push to master
- [x] (B) Yes — unit tests only in CI; E2E tests run locally or on demand
- [ ] (C) No — tests are run locally only (no CI changes)

---

## 6. Test Location and Build Integration

Where should tests live and how should they be run?

- [ ] (A) `tests/` directory at repo root; `make test` runs all tests
- [ ] (B) `tests/unit/` and `tests/e2e/` subdirectories; separate `make test-unit` and `make test-e2e` targets
- [ ] (C) Alongside source files (e.g., `src/player_test.cpp`); single `make test` target
- [x] (D) No preference
