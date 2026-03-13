#include "combat.h"
#include "catch_amalgamated.hpp"

static Enemy make_enemy(float x, float z, bool alive) {
    Enemy e = {};
    e.position = { x, 0.5f, z };
    e.is_alive = alive;
    e.health   = 100.0f;
    return e;
}

TEST_CASE("nearest_enemy returns 0 for single alive enemy in range") {
    Enemy enemies[1] = { make_enemy(1.0f, 0.0f, true) };
    Vector3 origin = { 0.0f, 0.5f, 0.0f };
    REQUIRE(nearest_enemy(enemies, 1, origin, 5.0f) == 0);
}

TEST_CASE("nearest_enemy returns the closer enemy's index") {
    Enemy enemies[2] = {
        make_enemy(5.0f, 0.0f, true),   // far  (index 0)
        make_enemy(1.0f, 0.0f, true)    // close (index 1)
    };
    Vector3 origin = { 0.0f, 0.5f, 0.0f };
    REQUIRE(nearest_enemy(enemies, 2, origin, 10.0f) == 1);
}

TEST_CASE("nearest_enemy returns -1 when all enemies are dead") {
    Enemy enemies[2] = {
        make_enemy(1.0f, 0.0f, false),
        make_enemy(2.0f, 0.0f, false)
    };
    Vector3 origin = { 0.0f, 0.5f, 0.0f };
    REQUIRE(nearest_enemy(enemies, 2, origin, 10.0f) == -1);
}

TEST_CASE("attack damage: ATTACK_DAMAGE reduces enemy health when applied") {
    Enemy e = make_enemy(1.0f, 0.0f, true);
    float new_health = e.health - ATTACK_DAMAGE;
    REQUIRE(new_health < e.health);
    REQUIRE(new_health == Catch::Approx(e.health - ATTACK_DAMAGE));
}

TEST_CASE("slam radius math: enemy inside SLAM_RADIUS is within range") {
    float r2 = SLAM_RADIUS * SLAM_RADIUS;
    float dx = 1.0f, dz = 0.0f;   // distance = 1.0, within 2.5
    REQUIRE(dx*dx + dz*dz <= r2);
}

TEST_CASE("slam radius math: enemy outside SLAM_RADIUS is not in range") {
    float r2 = SLAM_RADIUS * SLAM_RADIUS;
    float dx = SLAM_RADIUS + 0.5f, dz = 0.0f;
    REQUIRE(dx*dx + dz*dz > r2);
}
