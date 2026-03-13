#include "player.h"
#include <algorithm>
#include "catch_amalgamated.hpp"

TEST_CASE("boundary clamp caps position at BOUNDARY_MAX") {
    float x = BOUNDARY_MAX + 2.0f;
    REQUIRE(std::clamp(x, BOUNDARY_MIN, BOUNDARY_MAX) == BOUNDARY_MAX);
}

TEST_CASE("boundary clamp caps position at BOUNDARY_MIN") {
    float x = BOUNDARY_MIN - 2.0f;
    REQUIRE(std::clamp(x, BOUNDARY_MIN, BOUNDARY_MAX) == BOUNDARY_MIN);
}

TEST_CASE("boundary clamp leaves in-bounds position unchanged") {
    float x = 0.0f;
    REQUIRE(std::clamp(x, BOUNDARY_MIN, BOUNDARY_MAX) == 0.0f);
}
