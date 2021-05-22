#include <iostream>
#include <set>
#include <algorithm>

#include "catch2/catch.hpp"
#include "../../src/strategy.h"
#include "../../src/strategy/simple_evaluator.cpp"
#include "../../src/strategy/solo_selector.cpp"
#include "../../src/strategy/pass_selector.cpp"

using namespace std;

TEST_CASE("Strategy 0: the very fallback strategy", "[0]") {
  evaluator = simple_evaluator;
  attacking_sequence = {solo_selector};
  defending_sequence = {pass_selector};

  SECTION("Case 1") {
    Counter counter({29, 41, 42, 48, 50, 51, redJoker}); // 10 K K 2 2 2 redJoker
    Hand hand(0); // Solo 3
    Value value = 0;
    for (const auto &l : allLevels) value += counter[l] * evaluator(Hand(l));
    // on evaluating it gives the sum of Solo value
    REQUIRE(evaluate(&counter) == value);
    // on attacking it gives the smallest Solo;
    REQUIRE(attack(&counter) == Hand(7));
    // on defending it can only pass
    REQUIRE(defend(&counter, hand) == pass);
  }
}
