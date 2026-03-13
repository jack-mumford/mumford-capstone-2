#include "item.h"
#include "catch_amalgamated.hpp"

TEST_CASE("item_in_pickup_range returns true when player is within radius") {
    Item item = { {0.0f, 0.0f, 0.0f}, false };
    Vector3 player_pos = { 0.5f, 0.0f, 0.0f };
    REQUIRE(item_in_pickup_range(item, player_pos, 1.2f) == true);
}

TEST_CASE("item_in_pickup_range returns false when player is outside radius") {
    Item item = { {0.0f, 0.0f, 0.0f}, false };
    Vector3 player_pos = { 2.0f, 0.0f, 0.0f };
    REQUIRE(item_in_pickup_range(item, player_pos, 1.2f) == false);
}

TEST_CASE("item_in_pickup_range returns false when item is already collected") {
    Item item = { {0.0f, 0.0f, 0.0f}, true };
    Vector3 player_pos = { 0.1f, 0.0f, 0.0f };  // very close, but collected
    REQUIRE(item_in_pickup_range(item, player_pos, 1.2f) == false);
}
