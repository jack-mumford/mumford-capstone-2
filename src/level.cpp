#include "level.h"
#include "player.h"
#include <raylib.h>
#include <cmath>

void level_init(Platform platforms[], int& count) {
    count = 3;

    // Platform 1: low, left side  (height 1.0 — easy first jump)
    platforms[0].position = { -4.0f, 1.0f, -2.0f };
    platforms[0].size     = {  4.0f, 0.5f,  4.0f };

    // Platform 2: medium, right side  (height 2.25 — second jump)
    platforms[1].position = {  4.0f, 2.25f, 2.0f };
    platforms[1].size     = {  3.5f, 0.5f,  3.5f };

    // Platform 3: high, center-back  (height 3.75 — highest jump)
    platforms[2].position = {  0.0f, 3.75f, -6.0f };
    platforms[2].size     = {  4.0f, 0.5f,  2.5f };
}

void level_draw(const Platform platforms[], int count) {
    for (int i = 0; i < count; i++) {
        const Platform& p = platforms[i];
        DrawCubeV(p.position, p.size, BROWN);
        DrawCubeWiresV(p.position, p.size, DARKBROWN);
    }
}

bool check_platform_collision(Player& player, const Platform platforms[], int count) {
    bool on_platform = false;

    for (int i = 0; i < count; i++) {
        const Platform& plat = platforms[i];

        float plat_top = plat.position.y + plat.size.y * 0.5f;

        // Horizontal overlap check (AABB on XZ plane)
        float plat_x_min = plat.position.x - plat.size.x * 0.5f;
        float plat_x_max = plat.position.x + plat.size.x * 0.5f;
        float plat_z_min = plat.position.z - plat.size.z * 0.5f;
        float plat_z_max = plat.position.z + plat.size.z * 0.5f;

        float player_x_min = player.position.x - player.size.x * 0.5f;
        float player_x_max = player.position.x + player.size.x * 0.5f;
        float player_z_min = player.position.z - player.size.z * 0.5f;
        float player_z_max = player.position.z + player.size.z * 0.5f;

        bool overlap_x = player_x_max > plat_x_min && player_x_min < plat_x_max;
        bool overlap_z = player_z_max > plat_z_min && player_z_min < plat_z_max;

        if (!overlap_x || !overlap_z) continue;

        // Player bottom edge
        float player_bottom = player.position.y - player.size.y * 0.5f;

        // Landing: player is falling onto the platform top surface
        if (player.velocity.y <= 0.0f &&
            player_bottom <= plat_top + 0.15f &&
            player_bottom >= plat_top - 0.4f) {

            player.position.y  = plat_top + player.size.y * 0.5f;
            player.velocity.y  = 0.0f;
            player.is_grounded = true;
            on_platform = true;
        }
    }

    return on_platform;
}
