#include <algorithm>
#include <iostream>
#include <set>

#include "../../src/strategy.h"
#include "catch2/catch.hpp"

using namespace std;

TEST_CASE("Strategy 1: the last hand", "[1]") {
  evaluator = simple_evaluator;
  attacking_sequence = {one_shot_selector};

  SECTION("Case 1") {
    Counter counter({0, 29, 41, 42, 43, 44, 46, 47});  // 3 10 K K K A A A
    Value value = (11 + 1) / 2;
    Hand hand(counter);
    // on evaluating it gives the value
    REQUIRE(evaluate(&counter) == value);
    // on attacking it gives all cards;
    REQUIRE(attack(&counter) == hand);
  }

  SECTION("Case 2") {
    Counter counter({0, 1, 2, 3, 50, 51});  // 3 3 3 3 2 2
    Value value = 0;
    Hand hand(counter);
    // on evaluating it gives the sum of Solo value
    REQUIRE(evaluate(&counter) == value);
    // on attacking it gives all cards;
    REQUIRE(attack(&counter) == hand);
  }
}
