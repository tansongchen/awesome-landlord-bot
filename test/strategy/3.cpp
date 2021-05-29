#include <algorithm>
#include <iostream>
#include <set>

#include "../../src/strategy.h"
#include "catch2/catch.hpp"

using namespace std;

TEST_CASE("Full Recursive Test", "[3]") {
  evaluator = simple_evaluator;
  attacking_sequence = {
    one_shot_selector,
    attacking_absolute_dominant_selector,
    selfish_1_selector,
    selfish_2_selector,
    minimal_selector,
    attacking_bomb_rocket_selector
  };
  defending_sequence = {
    defending_absolute_dominant_selector,
    same_category_selector,
    defending_bomb_rocket_selector,
    pass_selector
  };

  SECTION("Case 1") {
    Counter counter{"44KKKK"};
    Hand last_hand{"3333"};
    REQUIRE(evaluate(&counter) == (10 + 7) + (1 - 7));
    REQUIRE(defend(&counter, last_hand) == Hand{"KKKK"});
  }
  SECTION("Case 2") {
    Counter counter{"356789T"};
    Hand last_hand{"34567"};
    REQUIRE(defend(&counter, last_hand) == Hand{"56789"});
  }
  SECTION("Case 3") {
    Counter counter{"3TKKKAAA"};
    Hand last_hand{"3338"};
    REQUIRE(defend(&counter, last_hand) == Hand{"KKK3"});
  }
  SECTION("Case 4") {
    Counter counter{"8JK2BR"};
    Hand last_hand{"3333"};
    REQUIRE(evaluate(&counter) == 27);
    REQUIRE(defend(&counter, last_hand) == rocket);
  }
  SECTION("Case 5") {
    Counter counter{"333322"};
    Hand last_hand{"6665"};
    REQUIRE(evaluate(&counter) == (0 + 7) + 5);
    REQUIRE(defend(&counter, last_hand) == Hand{"3333"});
  }
  SECTION("Case 6") {
    Counter counter{"888822"};
    Hand last_hand{"6665"};
    REQUIRE(evaluate(&counter) == (5 + 7) + 5);
    REQUIRE(defend(&counter, last_hand) == Hand{"8888"});
  }
  SECTION("Case 7") {
    Counter counter{"TTTT22"};
    Hand last_hand{"3338"};
    REQUIRE(defend(&counter, last_hand) == Hand{"TTTT"});
  }
  SECTION("Case 8") {
    Counter counter{"TT2"};
    Hand last_hand{"3338"};
    REQUIRE(defend(&counter, last_hand) == pass);
  }
}
