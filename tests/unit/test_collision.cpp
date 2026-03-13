#include "level.h"
#include "player.h"
#include "catch_amalgamated.hpp"

// Helpers to build minimal Player/Platform structs without calling player_init()
static Player make_player(float x, float y, float z, float vy = 0.0f) {
    Player p = {};
    p.position = { x, y, z };
    p.velocity = { 0.0f, vy, 0.0f };
    p.size     = { 0.8f, 1.0f, 0.8f };
    return p;
}

static Platform make_platform(float cx, float cy, float cz,
                               float sx, float sy, float sz) {
    Platform plat;
    plat.position = { cx, cy, cz };
    plat.size     = { sx, sy, sz };
    return plat;
}

TEST_CASE("check_platform_collision lands player on platform top") {
    // Platform: center y=1.0, height=0.5 → top = 1.25
    Platform platforms[1] = { make_platform(0.0f, 1.0f, 0.0f, 4.0f, 0.5f, 4.0f) };

    // player bottom = position.y - size.y/2 = 1.80 - 0.5 = 1.30  (inside [0.75, 1.40])
    Player player = make_player(0.0f, 1.80f, 0.0f, -1.0f);  // falling

    bool result = check_platform_collision(player, platforms, 1);

    REQUIRE(result == true);
    REQUIRE(player.is_grounded == true);
    REQUIRE(player.velocity.y == 0.0f);
    // player should snap to plat_top + half player height = 1.25 + 0.5 = 1.75
    REQUIRE(player.position.y == Catch::Approx(1.75f));
}

TEST_CASE("check_platform_collision misses when no XZ overlap") {
    Platform platforms[1] = { make_platform(0.0f, 1.0f, 0.0f, 4.0f, 0.5f, 4.0f) };

    Player player = make_player(10.0f, 1.80f, 0.0f, -1.0f);  // far outside XZ range

    bool result = check_platform_collision(player, platforms, 1);

    REQUIRE(result == false);
    REQUIRE(player.is_grounded == false);
}

TEST_CASE("check_platform_collision misses when player is far below platform") {
    // Platform very high up
    Platform platforms[1] = { make_platform(0.0f, 5.0f, 0.0f, 4.0f, 0.5f, 4.0f) };
    // plat_top = 5.25; player_bottom must be in [4.75, 5.40] to land
    // player at y=0.5 → player_bottom = 0.0 — not in range
    Player player = make_player(0.0f, 0.5f, 0.0f, 0.0f);

    bool result = check_platform_collision(player, platforms, 1);

    REQUIRE(result == false);
}
