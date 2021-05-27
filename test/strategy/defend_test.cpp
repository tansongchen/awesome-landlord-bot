#include <algorithm>
#include <iostream>
#include <set>

#include "../../src/strategy.h"
#include "catch2/catch.hpp"

using namespace std;

// Hand same_category_selector(Counter *counter, const Hand &last_hand) {
//   Level count = 0;

TEST_CASE("Strategy 1: Defending final hand", "[0]") {
  evaluator = simple_evaluator;
  defending_sequence = {boom_terminator_selector, same_category_selector,
                        defend_boom_selector, pass_selector};

  SECTION("Case 1") {
    Counter counter({0, 1, 2, 3});  // 3 3 3 3
    Hand last_hand = {5, 6, 7, 8};  // 4 4 4 5
    Value value = -7 + 7;
    REQUIRE(evaluate(&counter, last_hand) == value);
    REQUIRE(defense(&counter, last_hand) == Hand(counter));
  }
  SECTION("Case 2") {
    Counter counter({redJokerLevel, blackJokerLevel});  // RJ BJ
    Hand last_hand = {0, 1, 2, 3};                      // 3 3 3 3
    Value value = 20;
    REQUIRE(evaluate(&counter, last_hand) == value);
    REQUIRE(defense(&counter, last_hand) == hand(counter));
  }
}

TEST_CASE("Strategy 2: returning same category", "[0]") {
  evaluator = simple_evaluator;
  defending_sequence = {boom_terminator_selector, same_category_selector,
                        defend_boom_selector, pass_selector};

  SECTION("Case 2.1") {
    Counter counter({0, 1, 2, 3, 40, 41, 42, 43});  // K K K K
    Hand last_hand = {0, 1, 2, 3};                  // 3 3 3 3
    Value value = 10 + 7;
    REQUIRE(evaluate(&counter, last_hand) == value);
    REQUIRE(defense(&counter, last_hand) == Hand(10, 4, 1));
  }
  SECTION("Case 2.2") {
    Counter counter({0, 1, 2, 3, 50, 51});  // 3 3 3 3 2 2
    Value value = 0;
    Hand hand(counter);
    REQUIRE(evaluate(&counter, last_hand) == value);
    REQUIRE(defense(&counter, last_hand) == hand);
  }
}

TEST_CASE("Strategy 3: returning same category", "[0]") {
  evaluator = simple_evaluator;
  defending_sequence = {boom_terminator_selector, same_category_selector,
                        defend_boom_selector, pass_selector};

  SECTION("Case 3.1") {
    Counter counter({23, 33, 41, 48, r, 51});  // RJ BJ
    Value value = {0, 1, 2, 3};                // 3 3 3 3
    Hand hand(counter);
    REQUIRE(evaluate(&counter, last_hand) == value);
    REQUIRE(defense(&counter, last_hand) == hand);
  }
  SECTION("Case 3.2") {
    Counter counter({0, 1, 2, 3, 50, 51});  // 3 3 3 3 2 2
    Value value = 0;
    Hand hand(counter);
    // on evaluating it gives the sum of Solo value
    REQUIRE(evaluate(&counter, last_hand) == value);
    // on defensing it gives all cards;
    REQUIRE(defense(&counter, last_hand) == hand);
  }
}

TEST_CASE("Strategy 4: PASS", "[0]") {
  evaluator = simple_evaluator;
  defending_sequence = {boom_terminator_selector, same_category_selector,
                        defend_boom_selector, pass_selector};

  SECTION("Case 4.1") {
    Counter counter({0, 1, 2, 3, 50, 51});  // 3 3 3 3 2 2
    Value value = 0;
    Hand hand(counter);
    // on evaluating it gives the sum of Solo value
    REQUIRE(evaluate(&counter, last_hand) == value);
    // on defensing it gives all cards;
    REQUIRE(defense(&counter, last_hand) == hand);
  }
}
