#include <algorithm>
#include <iostream>
#include <set>

#include "../../src/strategy.h"
#include "catch2/catch.hpp"

using namespace std;

TEST_CASE("Strategy 2: Try to play a absolute dominant hand before the fallback strategy", "[2]") {
  evaluator = simple_evaluator;
  attacking_sequence = {attacking_absolute_dominant_selector, minimal_selector};

  SECTION("Case 1") {
    Counter counter({2, 3, 52, 53});
    // on evaluating it gives the value
    REQUIRE(evaluate(&counter) == 6);
    // on attacking it gives all cards;
    REQUIRE(attack(&counter) == rocket);
  }
}
