#pragma once
#include <raylib.h>

static const float JUMP_VELOCITY = 8.0f;
static const float GRAVITY       = -20.0f;

struct Player {
    Vector3 position;
    Vector3 velocity;
    Vector3 size;
    bool    is_grounded;
};

Player player_init();
void   player_update(Player& player);
void   player_draw(const Player& player);
