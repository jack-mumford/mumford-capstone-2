#pragma once
#include <raylib.h>
#include "player.h"

static const int MAX_PLATFORMS = 8;

struct Platform {
    Vector3 position;  // center of platform
    Vector3 size;      // width, height, depth
};

void level_init(Platform platforms[], int& count);
void level_draw(const Platform platforms[], int count);
bool check_platform_collision(Player& player, const Platform platforms[], int count);
