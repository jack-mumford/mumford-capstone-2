#include <raylib.h>
#include <cmath>
#include "textures.h"
#include "player.h"
#include "level.h"
#include "enemy.h"
#include "combat.h"

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

static EM_BOOL on_web_resize(int /*event_type*/, const EmscriptenUiEvent* e, void* /*user*/) {
    SetWindowSize((int)e->windowInnerWidth, (int)e->windowInnerHeight);
    return EM_TRUE;
}
#endif


// ---- Game state (file-scope so emscripten loop callback can access it) ------
struct GameState {
    Camera3D  camera;
    Texture2D ground_tex;
    Player    player;
    Platform  platforms[MAX_PLATFORMS];
    int       platform_count;
    Enemy     enemies[MAX_ENEMIES];
    int       enemy_count;
    float     shockwave_timer;
    Vector3   shockwave_pos;
    bool      game_over;
};

static GameState g;

static void reset_game() {
    g.player          = player_init();
    g.shockwave_timer = 0.0f;
    g.shockwave_pos   = {};
    enemies_init(g.enemies, g.enemy_count);
    g.game_over = false;
}

// ---- Single-frame update + draw --------------------------------------------
static void UpdateDrawFrame() {
    float dt = GetFrameTime();

    // --- Game over screen ---
    if (g.game_over) {
        if (IsKeyPressed(KEY_R)) reset_game();

        BeginDrawing();
        ClearBackground({ 10, 10, 15, 255 });

        int sw = GetScreenWidth();
        int sh = GetScreenHeight();
        DrawRectangle(0, 0, sw, sh, { 0, 0, 0, 160 });

        const char* died_text    = "YOU DIED";
        const char* restart_text = "Press R to restart";
        int died_fs    = 80;
        int restart_fs = 28;
        DrawText(died_text,    sw/2 - MeasureText(died_text,    died_fs)    / 2, sh/2 - 70, died_fs,    RED);
        DrawText(restart_text, sw/2 - MeasureText(restart_text, restart_fs) / 2, sh/2 + 30, restart_fs, { 200, 200, 200, 220 });

        EndDrawing();
        return;
    }

    // --- Update ---
    bool was_slamming = g.player.is_slamming;

    player_update(g.player);
    check_platform_collision(g.player, g.platforms, g.platform_count);

    // Detect slam landing: was slamming, now grounded
    bool slam_landed = was_slamming && !g.player.is_slamming && g.player.is_grounded;
    if (slam_landed) {
        g.shockwave_pos   = g.player.position;
        g.shockwave_timer = SHOCKWAVE_DURATION;

        float r2 = SLAM_RADIUS * SLAM_RADIUS;
        for (int i = 0; i < g.enemy_count; i++) {
            if (!g.enemies[i].is_alive) continue;
            float dx = g.enemies[i].position.x - g.player.position.x;
            float dz = g.enemies[i].position.z - g.player.position.z;
            if (dx*dx + dz*dz <= r2) {
                g.enemies[i].health    -= SLAM_DAMAGE;
                g.enemies[i].hurt_timer = HURT_FLASH_TIME;
                if (g.enemies[i].health <= 0.0f) {
                    g.enemies[i].health   = 0.0f;
                    g.enemies[i].is_alive = false;
                }
            }
        }
    }

    if (g.shockwave_timer > 0.0f) g.shockwave_timer -= dt;

    enemies_update(g.enemies, g.enemy_count, g.player.position);

    // Enemy hits player at swing midpoint
    for (int i = 0; i < g.enemy_count; i++) {
        Enemy& e = g.enemies[i];
        if (!e.is_alive || !e.is_attacking || e.attack_hit) continue;
        if (e.attack_timer <= ENEMY_HIT_POINT) {
            e.attack_hit = true;
            float dx = g.player.position.x - e.position.x;
            float dz = g.player.position.z - e.position.z;
            float d2 = dx*dx + dz*dz;
            if (d2 <= 1.5f * 1.5f) {
                if (!g.player.is_blocking) {
                    g.player.health    -= 15.0f;
                    g.player.hurt_timer = 0.2f;
                    if (g.player.health <= 0.0f) {
                        g.player.health = 0.0f;
                        g.game_over     = true;
                    }
                }
            }
        }
    }

    // Sword attack (F or LMB)
    bool attack_input = IsKeyPressed(KEY_F) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    if (attack_input && g.player.attack_cooldown <= 0.0f) {
        int target = nearest_enemy(g.enemies, g.enemy_count, g.player.position, ATTACK_RANGE);
        if (target >= 0) {
            float dx = g.enemies[target].position.x - g.player.position.x;
            float dz = g.enemies[target].position.z - g.player.position.z;
            g.player.facing_angle = atan2f(dx, -dz);

            g.enemies[target].health    -= ATTACK_DAMAGE;
            g.enemies[target].hurt_timer = HURT_FLASH_TIME;
            if (g.enemies[target].health <= 0.0f) {
                g.enemies[target].health   = 0.0f;
                g.enemies[target].is_alive = false;
            }
        }
        g.player.is_attacking    = true;
        g.player.attack_timer    = ATTACK_DURATION;
        g.player.attack_cooldown = ATTACK_COOLDOWN;
    }

    // Camera follows player
    g.camera.target   = g.player.position;
    g.camera.position = {
        g.player.position.x,
        g.player.position.y + 10.0f,
        g.player.position.z + 11.0f
    };

    // --- Draw ---
    BeginDrawing();
    ClearBackground({ 20, 20, 30, 255 });

    BeginMode3D(g.camera);

    DrawPlane({ 0.0f, 0.0f, 0.0f }, { 20.0f, 20.0f }, DARKGREEN);
    DrawGrid(20, 1.0f);
    level_draw(g.platforms, g.platform_count);

    // Shockwave ring
    if (g.shockwave_timer > 0.0f) {
        float frac   = g.shockwave_timer / SHOCKWAVE_DURATION;
        float radius = SLAM_RADIUS * (1.0f - frac);
        float ring_y = g.shockwave_pos.y - 0.48f;
        unsigned char alpha = (unsigned char)(255 * frac);
        Color ring_col = { 255, 120, 0, alpha };
        for (float dr = 0.0f; dr <= 0.18f; dr += 0.06f) {
            DrawCircle3D(
                { g.shockwave_pos.x, ring_y, g.shockwave_pos.z },
                radius + dr, { 1.0f, 0.0f, 0.0f }, 90.0f, ring_col);
        }
    }

    enemies_draw(g.enemies, g.enemy_count);
    player_draw(g.player);

    EndMode3D();

    DrawFPS(10, 10);
    DrawText("F/LMB attack   RMB/Q block   Ctrl(air) slam", 10, 30, 18, { 200, 200, 200, 180 });

    // Player health bar
    {
        int bar_w = 200, bar_h = 16, bar_x = 10, bar_y = GetScreenHeight() - 36;
        float frac = g.player.health / g.player.max_health;
        DrawRectangle(bar_x, bar_y, bar_w, bar_h, DARKGRAY);
        Color hp_col = frac > 0.5f ? GREEN : (frac > 0.25f ? ORANGE : RED);
        DrawRectangle(bar_x, bar_y, (int)(bar_w * frac), bar_h, hp_col);
        DrawRectangleLines(bar_x, bar_y, bar_w, bar_h, WHITE);
        DrawText(TextFormat("HP  %.0f / %.0f", g.player.health, g.player.max_health),
                 bar_x + 4, bar_y + 1, 14, WHITE);
    }

    EndDrawing();
}

// ---- Main ------------------------------------------------------------------
int main() {
#ifdef PLATFORM_WEB
    // Start at full browser window size; resize callback keeps it in sync
    int init_w = EM_ASM_INT(return window.innerWidth;);
    int init_h = EM_ASM_INT(return window.innerHeight;);
    InitWindow(init_w, init_h, "4D Pixel Game");
    emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, true, on_web_resize);
#else
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(0, 0, "4D Pixel Game");
#endif
    SetTargetFPS(60);

    g.camera.position   = { 0.0f, 10.0f, 11.0f };
    g.camera.target     = { 0.0f,  0.0f,  0.0f };
    g.camera.up         = { 0.0f,  1.0f,  0.0f };
    g.camera.fovy       = 40.0f;
    g.camera.projection = CAMERA_PERSPECTIVE;

    g.ground_tex = generate_ground_texture();

    reset_game();
    level_init(g.platforms, g.platform_count);

#ifdef PLATFORM_WEB
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }
#endif

    UnloadTexture(g.ground_tex);
    CloseWindow();
    return 0;
}
