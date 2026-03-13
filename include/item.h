#pragma once
#include <raylib.h>

struct Item {
    Vector3 position;
    bool    collected;
};

void item_draw(const Item& item);
bool item_in_pickup_range(const Item& item, Vector3 player_pos, float radius = 1.2f);
