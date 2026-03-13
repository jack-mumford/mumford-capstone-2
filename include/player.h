#pragma once
#include <raylib.h>

static const float JUMP_VELOCITY = 8.0f;
static const float GRAVITY       = -20.0f;

inline constexpr float BOUNDARY_MIN = -9.0f;
inline constexpr float BOUNDARY_MAX =  9.0f;

struct Player {
    Vector3 position;
    Vector3 velocity;
    Vector3 size;

    bool  is_grounded;
    bool  is_moving;
    float walk_phase;     // accumulated walk cycle angle (radians)
    float facing_angle;   // yaw the character faces (radians, 0 = -Z)

    // Health
    float health;
    float max_health;
    float hurt_timer;      // brief flash when hit

    // Combat
    bool  is_attacking;
    float attack_timer;    // counts down during swing animation
    float attack_cooldown; // time until next attack is allowed
    bool  is_blocking;     // true while block key held

    // Double jump
    int   jumps_remaining; // resets to 2 on landing

    // Sprint / dash
    bool    is_sprinting;
    bool    is_dashing;
    float   dash_timer;      // counts down while dashing
    float   dash_cooldown;   // time until next dash is allowed
    bool    air_dash_used;   // true once an air dash has been used; resets on landing
    Vector3 dash_dir;        // normalised XZ direction of active dash

    // Ground slam
    bool    is_slamming;     // true while slamming down from the air
};

Player player_init();
void   player_update(Player& player);
void   player_draw(const Player& player);
