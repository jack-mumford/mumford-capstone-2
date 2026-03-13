#pragma once
#include <raylib.h>
#include "enemy.h"

// Combat constants
inline constexpr float ATTACK_RANGE    = 2.2f;
inline constexpr float ATTACK_DAMAGE   = 25.0f;
inline constexpr float ATTACK_COOLDOWN = 0.45f;
inline constexpr float ATTACK_DURATION = 0.25f;
inline constexpr float HURT_FLASH_TIME = 0.15f;

// Slam / shockwave constants
inline constexpr float SLAM_DAMAGE        = 40.0f;
inline constexpr float SLAM_RADIUS        = 2.5f;
inline constexpr float SHOCKWAVE_DURATION = 0.35f;

// Returns the index of the nearest alive enemy within range, or -1.
int nearest_enemy(const Enemy enemies[], int count, Vector3 origin, float range);
