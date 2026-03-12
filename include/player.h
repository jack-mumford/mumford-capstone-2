#pragma once
#include <raylib.h>

struct Player {
    Vector3 position;
    Vector3 velocity;
    Vector3 size;
    bool    is_grounded;
};

Player player_init();
void   player_update(Player& player);
void   player_draw(const Player& player);
