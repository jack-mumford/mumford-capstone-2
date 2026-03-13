#include "enemy.h"
#include <raylib.h>
#include <cmath>

static const float ENEMY_SPEED        = 2.5f;
static const float ENEMY_BOUNDARY     = 9.0f;
static const float ENEMY_STOP_DIST    = 0.9f;
static const float ENEMY_ATTACK_RANGE = 1.2f;
static const float ENEMY_ATTACK_CD    = 2.0f;

void enemies_init(Enemy enemies[], int& count) {
    count = 3;
    //                pos                   hp      max    alive  hurt  angle  walk  atk    hit    atimer  acd
    enemies[0] = { {  4.0f, 0.5f,  2.0f }, 100.0f, 100.0f, true, 0.0f, 0.0f, 0.0f, false, false, 0.0f,  0.0f };
    enemies[1] = { { -5.0f, 0.5f, -3.0f }, 100.0f, 100.0f, true, 0.0f, 1.0f, 0.0f, false, false, 0.0f,  0.8f };
    enemies[2] = { {  1.0f, 0.5f, -6.0f }, 100.0f, 100.0f, true, 0.0f, 2.5f, 0.0f, false, false, 0.0f,  1.6f };
}

void enemies_update(Enemy enemies[], int count, Vector3 player_pos) {
    float dt = GetFrameTime();

    for (int i = 0; i < count; i++) {
        Enemy& e = enemies[i];
        if (!e.is_alive) continue;

        if (e.hurt_timer    > 0.0f) e.hurt_timer    -= dt;
        if (e.attack_cooldown > 0.0f) e.attack_cooldown -= dt;
        if (e.is_attacking) {
            e.attack_timer -= dt;
            if (e.attack_timer <= 0.0f) {
                e.is_attacking = false;
                e.attack_hit   = false;
            }
        }

        // Chase the player on XZ plane
        float dx = player_pos.x - e.position.x;
        float dz = player_pos.z - e.position.z;
        float dist = sqrtf(dx*dx + dz*dz);

        // Start attack when in range and cooled down
        if (!e.is_attacking && e.attack_cooldown <= 0.0f && dist <= ENEMY_ATTACK_RANGE) {
            e.is_attacking    = true;
            e.attack_hit      = false;
            e.attack_timer    = ENEMY_ATTACK_DURATION;
            e.attack_cooldown = ENEMY_ATTACK_CD;
        }

        if (dist > ENEMY_STOP_DIST) {
            float nx = dx / dist;
            float nz = dz / dist;
            e.position.x += nx * ENEMY_SPEED * dt;
            e.position.z += nz * ENEMY_SPEED * dt;
            e.facing_angle = atan2f(nx, -nz);
            e.walk_phase  += 9.0f * dt;

            // Boundary clamp
            if (e.position.x >  ENEMY_BOUNDARY) e.position.x =  ENEMY_BOUNDARY;
            if (e.position.x < -ENEMY_BOUNDARY) e.position.x = -ENEMY_BOUNDARY;
            if (e.position.z >  ENEMY_BOUNDARY) e.position.z =  ENEMY_BOUNDARY;
            if (e.position.z < -ENEMY_BOUNDARY) e.position.z = -ENEMY_BOUNDARY;
        }
    }

    // Separate overlapping enemies
    const float ENEMY_RADIUS = 0.5f;
    const float MIN_DIST     = ENEMY_RADIUS * 2.0f;
    for (int i = 0; i < count; i++) {
        if (!enemies[i].is_alive) continue;
        for (int j = i + 1; j < count; j++) {
            if (!enemies[j].is_alive) continue;
            float dx = enemies[j].position.x - enemies[i].position.x;
            float dz = enemies[j].position.z - enemies[i].position.z;
            float dist = sqrtf(dx*dx + dz*dz);
            if (dist < MIN_DIST && dist > 0.0001f) {
                float push = (MIN_DIST - dist) * 0.5f;
                float nx = dx / dist;
                float nz = dz / dist;
                enemies[i].position.x -= nx * push;
                enemies[i].position.z -= nz * push;
                enemies[j].position.x += nx * push;
                enemies[j].position.z += nz * push;
            }
        }
    }
}

static void draw_enemy_figure(const Enemy& e) {
    float x = e.position.x;
    float y = e.position.y;
    float z = e.position.z;

    float rw_x = cosf(e.facing_angle);
    float rw_z = sinf(e.facing_angle);

    // Flash white on hurt, otherwise purple-ish
    bool  flashing  = e.hurt_timer > 0.0f;
    Color body_col  = flashing ? WHITE   : (Color){ 160,  60, 200, 255 };
    Color head_col  = flashing ? WHITE   : (Color){ 200, 100, 240, 255 };

    // Head
    DrawSphere({ x, y + 0.38f, z }, 0.13f, head_col);

    // Torso
    DrawCubeV({ x, y + 0.08f, z }, { 0.06f, 0.28f, 0.06f }, body_col);

    // Arms — right arm swings forward during attack
    float fw_xa =  sinf(e.facing_angle);
    float fw_za = -cosf(e.facing_angle);
    float arm_off = 0.18f;
    Vector3 arm_size = { 0.05f + fabsf(rw_x)*0.25f, 0.05f, 0.05f + fabsf(rw_z)*0.25f };

    float lunge = 0.0f;
    float raise = 0.0f;
    if (e.is_attacking) {
        float t = 1.0f - (e.attack_timer / ENEMY_ATTACK_DURATION);
        lunge = sinf(t * 3.14159f) * 0.35f;  // rises then falls
        raise = sinf(t * 3.14159f) * 0.15f;
    }
    // Left arm (static)
    DrawCubeV({ x - rw_x*arm_off, y + 0.20f, z - rw_z*arm_off }, arm_size, body_col);
    // Right arm (attack lunge)
    DrawCubeV({ x + rw_x*arm_off + fw_xa*lunge, y + 0.20f + raise, z + rw_z*arm_off + fw_za*lunge }, arm_size,
              e.is_attacking ? WHITE : body_col);

    // Legs (animated)
    float fw_x =  sinf(e.facing_angle);
    float fw_z = -cosf(e.facing_angle);
    float swing = sinf(e.walk_phase) * 0.22f;
    DrawCubeV({ x - rw_x*0.10f + fw_x*swing,  y - 0.27f, z - rw_z*0.10f + fw_z*swing  }, { 0.05f, 0.42f, 0.05f }, body_col);
    DrawCubeV({ x + rw_x*0.10f - fw_x*swing,  y - 0.27f, z + rw_z*0.10f - fw_z*swing  }, { 0.05f, 0.42f, 0.05f }, body_col);

    // Health bar (3D flat slabs above the figure)
    float bar_w     = 0.8f;
    float bar_h     = 0.06f;
    float bar_depth = 0.04f;
    float bar_y     = y + 0.75f;
    float hp_frac   = e.health / e.max_health;

    // Background (dark)
    DrawCubeV({ x, bar_y, z }, { bar_w, bar_h, bar_depth }, DARKGRAY);
    // Foreground (green → red based on health)
    float filled_w  = bar_w * hp_frac;
    float filled_x  = x - bar_w * 0.5f + filled_w * 0.5f;
    Color hp_col    = hp_frac > 0.5f ? GREEN : (hp_frac > 0.25f ? ORANGE : RED);
    DrawCubeV({ filled_x, bar_y, z }, { filled_w, bar_h, bar_depth }, hp_col);
}

void enemies_draw(const Enemy enemies[], int count) {
    for (int i = 0; i < count; i++) {
        if (!enemies[i].is_alive) continue;
        draw_enemy_figure(enemies[i]);
    }
}
