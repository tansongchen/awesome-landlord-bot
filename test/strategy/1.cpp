#include <algorithm>
#include <iostream>
#include <set>

#include "../../src/strategy.h"
#include "catch2/catch.hpp"

using namespace std;

TEST_CASE("Full Non-recursive Test", "[1]") {
  evaluator = simple_evaluator;
  attacking_sequence = {
    one_shot_selector,
    attacking_absolute_dominant_selector,
    minimal_selector,
    attacking_bomb_rocket_selector,
    solo_selector // when Trio is not pruned during selfish_2_selector, must include this as a fallback approach
  };
  defending_sequence = {
    defending_absolute_dominant_selector,
    defending_bomb_rocket_selector,
    pass_selector
  };

  SECTION("Case 1") {
    Counter counter{"3333"};
    Hand last_hand{"4445"};
    REQUIRE(evaluate(&counter) == 7);
    REQUIRE(defend(&counter, last_hand) == Hand(counter));
  }

  SECTION("Case 2") {
    Counter counter{"RB"};
    Hand last_hand{"3333"};
    REQUIRE(evaluate(&counter) == 20);
    REQUIRE(defend(&counter, last_hand) == rocket);
  }
}
