#include "player.h"
#include <raylib.h>
#include <algorithm>
#include <cmath>

static const float SLAM_SPEED    = 16.0f;
static const float MOVE_SPEED    =  5.0f;
static const float SPRINT_SPEED  = 10.0f;
static const float DASH_SPEED    = 22.0f;
static const float DASH_DURATION =  0.18f;
static const float DASH_COOLDOWN =  0.55f;
static const float BOUNDARY_MIN  = -9.0f;
static const float BOUNDARY_MAX  =  9.0f;
static const float GROUND_Y      =  0.5f;

Player player_init() {
    Player p = {};
    p.position        = { 0.0f, GROUND_Y, 0.0f };
    p.size            = { 0.8f, 1.0f, 0.8f };
    p.jumps_remaining = 2;
    p.health          = 100.0f;
    p.max_health      = 100.0f;
    return p;
}

void player_update(Player& player) {
    float dt = GetFrameTime();

    bool shift = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
    bool shift_just = IsKeyPressed(KEY_LEFT_SHIFT) || IsKeyPressed(KEY_RIGHT_SHIFT);

    // If grounded (set last frame), clear slam and restore jump count
    if (player.is_grounded) {
        player.is_slamming     = false;
        player.jumps_remaining = 2;
        player.air_dash_used   = false;
    }

    // --- Block (RMB or Q, can't block while attacking or dashing) ---
    player.is_blocking = (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) || IsKeyDown(KEY_Q))
                         && !player.is_attacking && !player.is_dashing;

    // --- Hurt flash tick ---
    if (player.hurt_timer > 0.0f) player.hurt_timer -= dt;

    // --- Attack / dash cooldown tick ---
    if (player.attack_cooldown > 0.0f) player.attack_cooldown -= dt;
    if (player.attack_timer    > 0.0f) {
        player.attack_timer -= dt;
        if (player.attack_timer <= 0.0f) player.is_attacking = false;
    }

    // --- Dash cooldown tick ---
    if (player.dash_cooldown > 0.0f) player.dash_cooldown -= dt;

    // --- Initiate dash on shift tap ---
    // Allowed when: cooldown ready AND (on ground OR air dash not yet used) AND not blocking
    bool can_dash = shift_just && player.dash_cooldown <= 0.0f
                    && (player.is_grounded || !player.air_dash_used)
                    && !player.is_blocking;
    if (can_dash) {
        float dx = sinf(player.facing_angle);
        float dz = -cosf(player.facing_angle);
        float kx = 0, kz = 0;
        if (IsKeyDown(KEY_W)) kz -= 1;
        if (IsKeyDown(KEY_S)) kz += 1;
        if (IsKeyDown(KEY_A)) kx -= 1;
        if (IsKeyDown(KEY_D)) kx += 1;
        if (kx != 0 || kz != 0) { dx = kx; dz = kz; }
        float len = sqrtf(dx*dx + dz*dz);
        if (len > 0) { dx /= len; dz /= len; }

        player.dash_dir      = { dx, 0.0f, dz };
        player.is_dashing    = true;
        player.dash_timer    = DASH_DURATION;
        player.dash_cooldown = DASH_COOLDOWN;
        if (!player.is_grounded) player.air_dash_used = true;
    }

    // --- Dash movement ---
    if (player.is_dashing) {
        player.dash_timer -= dt;
        if (player.dash_timer <= 0.0f) {
            player.is_dashing = false;
        } else {
            player.position.x += player.dash_dir.x * DASH_SPEED * dt;
            player.position.z += player.dash_dir.z * DASH_SPEED * dt;
        }
    }

    // --- Normal / sprint movement (suppressed during dash) ---
    float dir_x = 0.0f, dir_z = 0.0f;
    if (IsKeyDown(KEY_W)) dir_z -= 1.0f;
    if (IsKeyDown(KEY_S)) dir_z += 1.0f;
    if (IsKeyDown(KEY_A)) dir_x -= 1.0f;
    if (IsKeyDown(KEY_D)) dir_x += 1.0f;

    player.is_moving    = (dir_x != 0.0f || dir_z != 0.0f);
    player.is_sprinting = shift && !player.is_dashing && player.is_moving && player.is_grounded;

    if (!player.is_dashing && player.is_moving) {
        float spd = player.is_blocking  ? MOVE_SPEED * 0.35f :
                    player.is_sprinting ? SPRINT_SPEED : MOVE_SPEED;
        player.position.x += dir_x * spd * dt;
        player.position.z += dir_z * spd * dt;
        player.facing_angle = atan2f(dir_x, -dir_z);
        float phase_rate = player.is_sprinting ? 14.0f : 9.0f;
        player.walk_phase += phase_rate * dt;
    }
    if (player.is_dashing) {
        // Face the dash direction
        player.facing_angle = atan2f(player.dash_dir.x, -player.dash_dir.z);
        player.walk_phase  += 20.0f * dt;  // blur legs during dash
    }

    // --- Boundary clamp ---
    player.position.x = std::clamp(player.position.x, BOUNDARY_MIN, BOUNDARY_MAX);
    player.position.z = std::clamp(player.position.z, BOUNDARY_MIN, BOUNDARY_MAX);

    // --- Double jump ---
    if (IsKeyPressed(KEY_SPACE) && player.jumps_remaining > 0) {
        player.velocity.y     = JUMP_VELOCITY;
        player.jumps_remaining--;
        player.is_slamming = false;  // cancel slam if jumping
    }

    // --- Ground slam (Ctrl while airborne) ---
    bool ctrl = IsKeyPressed(KEY_LEFT_CONTROL) || IsKeyPressed(KEY_RIGHT_CONTROL);
    if (ctrl && !player.is_grounded && !player.is_slamming) {
        player.is_slamming  = true;
        player.velocity.y   = -SLAM_SPEED;
    }

    // --- Reset grounded before physics ---
    player.is_grounded = false;

    // --- Gravity + vertical integration ---
    // During slam, override gravity to keep the downward velocity constant
    if (!player.is_slamming) {
        player.velocity.y += GRAVITY * dt;
    }
    player.position.y += player.velocity.y * dt;

    // --- Ground floor clamp ---
    if (player.position.y < GROUND_Y) {
        player.position.y      = GROUND_Y;
        player.velocity.y      = 0.0f;
        player.is_grounded     = true;
        player.jumps_remaining = 2;
        player.air_dash_used   = false;
        player.is_slamming     = false;
    }
}

void player_draw(const Player& player) {
    float x = player.position.x;
    float y = player.position.y;
    float z = player.position.z;

    float fw_x =  sinf(player.facing_angle);
    float fw_z = -cosf(player.facing_angle);
    float rw_x =  cosf(player.facing_angle);
    float rw_z =  sinf(player.facing_angle);

    // Leg / arm swing
    float swing = player.is_moving  ? sinf(player.walk_phase) * 0.22f : 0.0f;
    float arm_s = player.is_moving  ? -sinf(player.walk_phase) * 0.14f : 0.0f;

    // Jump / dash / slam pose
    bool airborne = !player.is_grounded;
    float leg_spread = player.is_slamming ? 0.22f : (airborne ? 0.17f : 0.10f);
    float leg_tuck   = player.is_slamming ? 0.25f : (airborne ? 0.12f : 0.0f);
    float arm_raise  = player.is_slamming ? 0.22f : (airborne ? 0.10f : 0.0f);

    // Dash lean: shift torso/head forward
    float lean = player.is_dashing ? 0.12f : 0.0f;

    // Body colour tints
    bool  flashing  = player.hurt_timer > 0.0f;
    Color torso_col = flashing            ? RED    :
                      player.is_blocking  ? SKYBLUE :
                      player.is_slamming  ? RED    :
                      player.is_dashing   ? YELLOW :
                      player.is_sprinting ? ORANGE  : WHITE;
    Color head_col  = flashing            ? RED    :
                      player.is_blocking  ? SKYBLUE :
                      player.is_slamming  ? RED    :
                      player.is_dashing   ? YELLOW : BEIGE;

    // -- Head --
    DrawSphere({ x + fw_x*lean, y + 0.38f, z + fw_z*lean }, 0.13f, head_col);

    // -- Torso --
    DrawCubeV({ x + fw_x*lean*0.5f, y + 0.08f, z + fw_z*lean*0.5f },
              { 0.06f, 0.28f, 0.06f }, torso_col);

    // -- Arms --
    float arm_off = 0.18f;
    Vector3 arm_size = { 0.05f + fabsf(rw_x)*0.25f, 0.05f, 0.05f + fabsf(rw_z)*0.25f };
    DrawCubeV({ x - rw_x*arm_off + fw_x*arm_s, y + 0.20f + arm_raise, z - rw_z*arm_off + fw_z*arm_s }, arm_size, torso_col);
    DrawCubeV({ x + rw_x*arm_off - fw_x*arm_s, y + 0.20f + arm_raise, z + rw_z*arm_off - fw_z*arm_s }, arm_size, torso_col);

    // -- Legs --
    DrawCubeV({ x - rw_x*leg_spread + fw_x*swing, y - 0.27f + leg_tuck, z - rw_z*leg_spread + fw_z*swing }, { 0.05f, 0.42f, 0.05f }, torso_col);
    DrawCubeV({ x + rw_x*leg_spread - fw_x*swing, y - 0.27f + leg_tuck, z + rw_z*leg_spread - fw_z*swing }, { 0.05f, 0.42f, 0.05f }, torso_col);

    // -- Sword --
    if (player.is_blocking) {
        // Guard: sword held vertically in front of the body
        float gx = x + fw_x*0.28f;
        float gy = y + 0.25f;
        float gz = z + fw_z*0.28f;
        // Vertical blade (no facing-dependent sizing — just a tall thin bar)
        DrawCubeV({ gx, gy + 0.20f, gz }, { 0.04f, 0.55f, 0.04f }, SKYBLUE);
        // Crossguard horizontal
        DrawCubeV({ gx, gy, gz },
                  { fabsf(rw_x)*0.30f + 0.04f, 0.04f, fabsf(rw_z)*0.30f + 0.04f }, DARKGRAY);
    } else {
        // Normal / attacking pose
        float swing_ext = player.is_attacking ? 0.30f : 0.0f;
        float sx = x + rw_x*0.32f + fw_x*(0.10f + swing_ext);
        float sy = y + 0.18f + arm_raise;
        float sz = z + rw_z*0.32f + fw_z*(0.10f + swing_ext);

        Color blade_col = player.is_attacking ? SKYBLUE : LIGHTGRAY;

        DrawCubeV({ sx, sy, sz },
                  { fabsf(rw_x)*0.22f + 0.03f, 0.04f, fabsf(rw_z)*0.22f + 0.03f }, DARKGRAY);
        DrawCubeV({ sx + fw_x*0.22f, sy + 0.02f, sz + fw_z*0.22f },
                  { fabsf(fw_x)*0.32f + 0.03f, 0.03f, fabsf(fw_z)*0.32f + 0.03f }, blade_col);
    }
}
