#include <algorithm>
#include <iostream>
#include <set>

#include "../../src/strategy.h"
#include "catch2/catch.hpp"

using namespace std;

TEST_CASE("Strategy 1: Defending final hand", "[0]") {
  evaluator = simple_evaluator;
  defending_sequence = {boom_terminator_selector, pass_selector};

  SECTION("Case 1") {
    Counter counter({0, 1, 2, 3});          // 3 3 3 3
    Hand last_hand(Counter({5, 6, 7, 8}));  // 4 4 4 5
    // REQUIRE(evaluate(&counter) == -7 + 7);
    REQUIRE(defend(&counter, last_hand) == Hand(counter));
  }
  SECTION("Case 2") {
    Counter counter = rocket.get_counter();  // RJ BJ
    Hand last_hand(Counter({0, 1, 2, 3}));   // 3 3 3 3
    // REQUIRE(evaluate(&counter) == 20);
    REQUIRE(defend(&counter, last_hand) == rocket);
  }
}

TEST_CASE("Strategy 2: returning same category", "[0]") {
  evaluator = simple_evaluator;
  defending_sequence = {boom_terminator_selector, same_category_selector,
                        pass_selector};

  // SECTION("Case 2.1") {
  //   Counter counter({4, 6, 40, 41, 42, 43});  // 4 4 K K K K
  //   Hand last_hand = {0, 1, 2, 3};            // 3 3 3 3
  //   // REQUIRE(evaluate(&counter) == (10 + 7) + (1 - 7););
  //   REQUIRE(defend(&counter, last_hand) == Hand(10, 4, 1)); // Level: 65535,
  //   Length: 0, Size: 0, Cosize: 0, Attached ERROR
  // }
  SECTION("Case 2.2") {
    Counter counter({3, 29, 41, 42, 43, 44, 46, 47});  // 3 10 K K K A A A
    Hand last_hand(Counter({0, 1, 2, 22}));            // 3 3 3 8
    Counter myturn({3, 41, 42, 43});                   // K K K 3
    // REQUIRE(evaluate(&counter) == (10 + 1) / 2 + (11 + 1) / 2);
    REQUIRE(defend(&counter, last_hand) == Hand(myturn));
  }
  SECTION("Case 2.3") {
    Counter counter(
        {0, 4, 8, 10, 12, 13, 14, 16, 20, 24});  // 3 4 5 5 6 6 6 7 8 9
    Hand last_hand(Counter({1, 5, 9, 13, 17}));  // 3 4 5 6 7
    Counter myturn({4, 5, 6, 7, 8});
    // REQUIRE(evaluate(&counter) == (6 - 7 + 1) + (2 - 7) + (3 - 7));
    REQUIRE(defend(&counter, last_hand) == Hand(myturn));
  }
}

TEST_CASE("Strategy 3: defending absolute domination", "[0]") {
  evaluator = simple_evaluator;
  defending_sequence = {boom_terminator_selector,
                        defending_absolute_dominant_selector, pass_selector};

  SECTION("Case 3.1") {
    Counter counter({23, 33, 41, 48, blackJoker, redJoker});
    Hand last_hand(Counter({0, 1, 2, 3}));  // 3 3 3 3
    // REQUIRE(evaluate(&counter) == 20);
    REQUIRE(defend(&counter, last_hand) == rocket);
  }
  SECTION("Case 3.2") {
    Counter counter({0, 1, 2, 3, 50, 51});      // 3 3 3 3 2 2
    Hand last_hand(Counter({11, 12, 13, 14}));  // 6 6 6 5
    // REQUIRE(evaluate(&counter) == -7 + 7 + 12 - 7);
    REQUIRE(defend(&counter, last_hand) == Hand(Counter({0, 1, 2, 3})));
  }
  // rev
  SECTION("Case 3.3") {
    Counter counter({20, 21, 22, 23, 50, 51});  // 8 8 8 8 2 2
    Hand last_hand = {11, 12, 13, 14};          // 6 6 6 5
    // REQUIRE(evaluate(&counter) == -7 + 7 + 12 - 7);
    REQUIRE(defend(&counter, last_hand) == Hand(Counter({20, 21, 22, 23})));
  }
}

TEST_CASE("Strategy 4: PASS", "[0]") {
  evaluator = simple_evaluator;
  defending_sequence = {boom_terminator_selector,
                        defending_absolute_dominant_selector, pass_selector};

  // // This condition ought to be pass! Needs Revision!
  // SECTION("Case 4.1") {
  //   Counter counter({28, 29, 30, 31, 50, 51});  // 10 10 10 10 2 2
  //   Hand last_hand(Counter({0, 1, 2, 22}));     // 3 3 3 8
  //   REQUIRE(defend(&counter, last_hand) ==
  //           Hand(Counter({29, 30, 31, 50})));  // 10 10 10 2
  // }
  SECTION("Case 4.2") {
    Counter counter({28, 29, 51});           // 10 10 2
    Hand last_hand(Counter({0, 1, 2, 22}));  // 3 3 3 8
    REQUIRE(defend(&counter, last_hand) == pass);
  }
}
