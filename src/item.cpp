#include "item.h"
#include <raylib.h>
#include <cmath>

void item_draw(const Item& item) {
    if (item.collected) return;

    float bob = sinf(GetTime() * 2.5f) * 0.06f;
    float spin = GetTime() * 1.2f;  // slow spin around Y

    float cx = item.position.x;
    float cy = item.position.y + 0.35f + bob;
    float cz = item.position.z;

    // Rotate gun body/barrel around Y using manual offset
    float bx = cosf(spin) * 0.15f;
    float bz = sinf(spin) * 0.15f;

    // Gun body (handle + slide)
    DrawCubeV({ cx + bx*0.0f, cy, cz + bz*0.0f },
              { 0.08f, 0.10f, 0.22f }, DARKGRAY);
    // Barrel extension
    DrawCubeV({ cx + bx*0.0f, cy + 0.02f, cz + bz*0.0f - 0.13f },
              { 0.05f, 0.05f, 0.08f }, GRAY);
    // Grip
    DrawCubeV({ cx, cy - 0.09f, cz + 0.06f },
              { 0.07f, 0.10f, 0.07f }, DARKGRAY);

    // Pickup indicator ring on the ground
    DrawCircle3D({ cx, item.position.y + 0.02f, cz },
                 0.5f, { 1.0f, 0.0f, 0.0f }, 90.0f, { 255, 220, 0, 180 });
}

bool item_in_pickup_range(const Item& item, Vector3 player_pos, float radius) {
    if (item.collected) return false;
    float dx = player_pos.x - item.position.x;
    float dz = player_pos.z - item.position.z;
    return sqrtf(dx*dx + dz*dz) < radius;
}
