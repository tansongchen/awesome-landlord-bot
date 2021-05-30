#include <algorithm>
#include <iostream>
#include <set>

#include "../../src/strategy.h"
#include "catch2/catch.hpp"

using namespace std;

TEST_CASE("Attack-only Recursive Test", "[2]") {
  evaluator = simple_evaluator;
  attacking_sequence = {
    one_shot_selector,
    attacking_absolute_dominant_selector,
    selfish_1_selector,
    selfish_2_selector,
    minimal_selector,
    attacking_bomb_rocket_selector
  };

  SECTION("Case 1") {
    Counter counter{"3TKKKAAA"};
    Value value = (11 + 1) / 2;
    REQUIRE(attack(&counter) == Hand(counter));
    REQUIRE(evaluate(&counter) == Pair(value, 1));
  }

  SECTION("Case 2") {
    Counter counter{"33BR"};
    Value value = 20 - 7;
    REQUIRE(attack(&counter) == rocket);
    REQUIRE(evaluate(&counter) == Pair(value, 2));
  }

  SECTION("Case 3") {
    Counter counter{"3TKKKAAA2"};
    Value value = (11 + 1) / 2 + 5;
    REQUIRE(attack(&counter) == Hand{"3TKKKAAA"});
    REQUIRE(evaluate(&counter) == Pair(value, 2));
  }

  SECTION("Case 4") {
    Counter counter{"4888TKKR"};
    Value value = -2 + 0 + 3 + 7;
    REQUIRE(attack(&counter) == Hand{"4888"});
    REQUIRE(evaluate(&counter) == Pair(value, 4));
  }

  SECTION("Case 5") {
    Counter counter{"488TKKR"};
    Value value = -6 + -2 + 0 + 3 + 7;
    REQUIRE(attack(&counter) == Hand{"4"});
    REQUIRE(evaluate(&counter) == Pair(value, 5));
  }

  SECTION("Case 6") {
    Counter counter{"3333"};
    Value value = 7;
    REQUIRE(attack(&counter) == Hand{"3333"});
    REQUIRE(evaluate(&counter) == Pair(value, 1));
  }
}
