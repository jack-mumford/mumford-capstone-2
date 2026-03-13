#pragma once
#include <raylib.h>

static const int MAX_ENEMIES = 6;

static const float ENEMY_ATTACK_DURATION = 0.5f;  // full swing duration
static const float ENEMY_HIT_POINT      = 0.25f; // timer value when damage applies

struct Enemy {
    Vector3 position;
    float   health;
    float   max_health;
    bool    is_alive;
    float   hurt_timer;      // counts down after taking a hit (flash effect)
    float   facing_angle;    // for the stick figure orientation
    float   walk_phase;      // for leg animation while chasing
    bool    is_attacking;
    bool    attack_hit;      // true once damage has been applied this swing
    float   attack_timer;    // counts down during swing
    float   attack_cooldown; // time until next attack allowed
};

void enemies_init(Enemy enemies[], int& count);
void enemies_update(Enemy enemies[], int count, Vector3 player_pos);
void enemies_draw(const Enemy enemies[], int count);
