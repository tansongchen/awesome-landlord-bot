#include <algorithm>
#include <iostream>
#include <set>

#include "../../src/strategy.h"
#include "catch2/catch.hpp"

using namespace std;

TEST_CASE("Attack-only Non-recursive Test", "[0]") {
  evaluator = simple_evaluator;
  attacking_sequence = {
    one_shot_selector,
    attacking_absolute_dominant_selector,
    minimal_selector,
    attacking_bomb_rocket_selector,
    solo_selector // when Trio is not pruned during selfish_2_selector, must include this as a fallback approach
  };

  SECTION("Case 1") {
    Counter counter{"3TKKKAAA"};
    Value value = (11 + 1) / 2;
    Hand hand(counter);
    // on attacking it gives all cards;
    REQUIRE(attack(&counter) == hand);
    // on evaluating it gives the value
    REQUIRE(evaluate(&counter) == Pair(value, 1));
  }

  SECTION("Case 2") {
    Counter counter{"33BR"};
    Value value = 20 - 7;
    // on attacking it gives all cards;
    REQUIRE(attack(&counter) == rocket);
    // on evaluating it gives the value
    REQUIRE(evaluate(&counter) == Pair(value, 2));
  }

  SECTION("Case 3") {
    Counter counter{"TKK22R"};
    Value value = 0 + 3 + 5 + 7;
    // on attacking it gives the smallest Solo;
    REQUIRE(attack(&counter) == Hand{"T"});
    // on evaluating it gives the sum of Solo value
    REQUIRE(evaluate(&counter) == Pair(value, 4));
  }

  SECTION("Case 4") {
    Counter counter{"3333"};
    Value value = 7;
    // on attacking it gives all cards;
    REQUIRE(attack(&counter) == Hand{"3333"});
    // on evaluating it gives the sum of Solo value
    REQUIRE(evaluate(&counter) == Pair(value, 1));
  }
}
