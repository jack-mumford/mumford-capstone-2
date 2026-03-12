# 01 Questions Round 1 - 4D Pixel Game

Please answer each question below (select one or more options, or add your own notes). Feel free to add additional context under any question.

## 1. What does "4D" mean to you in this game?

This is the most critical question — "4D" can mean very different things and will shape the entire architecture.

- [x] (A) 3D game world + time as the 4th dimension (e.g., rewind time, time travel mechanic)
- [ ] (B) 4D geometry rendered in 3D (true mathematical 4D — tesseracts, 4D mazes, etc.)
- [ ] (C) 2D game with a "depth" layer mechanic (foreground/background switching, feels like 2.5D but called 4D)
- [ ] (D) A stylistic name — the game is really 2D or 3D but with a "4D" theme or branding
- [ ] (E) Other (describe)

## 2. What is the visual style of "pixel"?

- [x] (A) Classic 2D pixel art sprites (16x16, 32x32 characters) rendered in a 2D or 3D world
- [ ] (B) Voxel-based (3D pixels / Minecraft-style blocks)
- [ ] (C) Low-resolution 3D rendered to look pixelated (pixelation post-processing effect)
- [ ] (D) Other (describe)

## 3. What kind of game is this?

- [x] (A) Platformer (run, jump, navigate platforms)
- [ ] (B) Top-down exploration / RPG-style movement
- [ ] (C) First-person exploration
- [ ] (D) Puzzle game where movement is the mechanic
- [ ] (E) Other (describe)

## 4. What should the player character be able to do?

Select all that apply.

- [x] (A) Walk/run in 2D or 3D space
- [x] (B) Jump
- [x] (C) Interact with objects in the world
- [ ] (D) Move through the "4th dimension" (shift between states/layers/time)
- [ ] (E) Other (describe)

## 5. What rendering/graphics library should be used?

Starting from scratch in C++ means we need a windowing and rendering stack.

- [ ] (A) SDL2 (Simple DirectMedia Layer) — popular for 2D pixel games, beginner-friendly
- [ ] (B) SFML (Simple and Fast Multimedia Library) — similar to SDL2, clean C++ API
- [ ] (C) OpenGL + GLFW/GLEW — lower-level, needed for true 3D or 4D geometry rendering
- [x] (D) Raylib — simple C library, great for 2D/3D games quickly
- [ ] (E) No preference / you choose what fits best
- [ ] (F) Other (describe)

## 6. What is the minimum viable scope for this first version?

- [x] (A) Just a character on screen that can move around in a simple level/world
- [ ] (B) A character + a small demo level that showcases the 4D mechanic
- [ ] (C) A character + multiple levels + basic collision with the environment
- [ ] (D) Other (describe)

## 7. What platforms should this run on?

- [x] (A) macOS only
- [ ] (B) Linux only
- [ ] (C) Windows only
- [ ] (D) Cross-platform (macOS + Linux + Windows)
- [ ] (E) Other (describe)

## 8. What build system should be used?

- [ ] (A) CMake (industry standard, cross-platform)
- [x] (B) Make / Makefile (simple, Unix-style)
- [ ] (C) No preference / you choose
- [ ] (D) Other (describe)
