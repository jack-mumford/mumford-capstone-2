#include "player.h"
#include <raylib.h>
#include <algorithm>

static const float MOVE_SPEED    = 5.0f;
static const float BOUNDARY_MIN  = -9.0f;
static const float BOUNDARY_MAX  =  9.0f;
static const float GROUND_Y      =  0.5f;  // half-height above y=0 plane

Player player_init() {
    Player p;
    p.position   = { 0.0f, GROUND_Y, 0.0f };
    p.velocity   = { 0.0f, 0.0f, 0.0f };
    p.size       = { 0.8f, 1.0f, 0.8f };
    p.is_grounded = false;
    return p;
}

void player_update(Player& player) {
    float dt = GetFrameTime();
    float speed = MOVE_SPEED * dt;

    // Horizontal movement
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    player.position.z -= speed;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))   player.position.z += speed;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))   player.position.x -= speed;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))  player.position.x += speed;

    // Boundary clamp
    player.position.x = std::clamp(player.position.x, BOUNDARY_MIN, BOUNDARY_MAX);
    player.position.z = std::clamp(player.position.z, BOUNDARY_MIN, BOUNDARY_MAX);

    // Jump (only when grounded)
    if (IsKeyPressed(KEY_SPACE) && player.is_grounded) {
        player.velocity.y  = JUMP_VELOCITY;
        player.is_grounded = false;
    }

    // Apply gravity and vertical velocity
    player.velocity.y  += GRAVITY * dt;
    player.position.y  += player.velocity.y * dt;

    // Ground floor clamp
    if (player.position.y < GROUND_Y) {
        player.position.y  = GROUND_Y;
        player.velocity.y  = 0.0f;
        player.is_grounded = true;
    }
}

void player_draw(const Player& player) {
    // Draw body
    DrawCubeV(player.position, player.size, BLUE);
    DrawCubeWiresV(player.position, player.size, DARKBLUE);
}
