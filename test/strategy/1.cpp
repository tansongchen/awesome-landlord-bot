#include <iostream>
#include <set>
#include <algorithm>

#include "../catch2/catch.hpp"
#include "../../src/strategy.h"
#include "../../src/strategy/simple_evaluator.cpp"
#include "../../src/strategy/one_shot_selector.cpp"
#include "../../src/strategy/pass_selector.cpp"

using namespace std;

TEST_CASE("Strategy 2: the last hand", "[0]")
{
  evaluator = simple_evaluator;
  attacking_sequence = {one_shot_selector};

  SECTION("Case 1")
  {
    Counter counter({29, 41, 42, 43, 48, 50, 51, redJoker}); // 10 K K K 2 2 2 redJoker
    Value value = 14 / 2;
    // on evaluating it gives the value
    REQUIRE(evaluate(&counter) == value);
    // on attacking it gives all cards;
    REQUIRE(attack(&counter) == Hand(14, 7, 1, 7, {7, 10, 10, 10, 12, 12, 12, 14}));
  }
  SECTION("Case 2")
  {
    Counter counter({0, 1, 2, 3, redJoker}); // 3 3 3 3 redJoker
    Value value = 14 / 2;
    // on evaluating it gives the sum of Solo value
    REQUIRE(evaluate(&counter) == value);
    // on attacking it gives all cards;
    REQUIRE(attack(&counter) == Hand(14, 14, 1, 4, {0, 0, 0, 0}));
  }
}
