#include "catch.hpp"
#include "../src/base.h"

#include <set>
#include <iostream>
using namespace std;

TEST_CASE("Card / Level conversion", "[conversion]") {
  Card card1 = blackJoker;
  Card card2 = redJoker;
  Level level1 = blackJokerLevel;
  Level level2 = redJokerLevel;
  Card card3 = 18; // one of the "7" s
  Level level3 = 4;
  REQUIRE(card_to_level(card1) == level1);
  REQUIRE(card_to_level(card2) == level2);
  REQUIRE(card_to_level(card3) == level3);
}

TEST_CASE("Counter class", "[counter]") {
  Group group = {0, 1, 2, 4, 5, blackJoker, redJoker};
  Counter counter({{0, 3}, {1, 2}, {blackJokerLevel, 1}, {redJokerLevel, 1}});
  REQUIRE(Counter(group) == counter);
  REQUIRE(counter.get_group(group) == group);
}

TEST_CASE("Level combination", "[combination]") {
  vector<Level> attachables = {3, 4, 8, 10};
  REQUIRE(combinations(attachables, 0) == vector<set<Level>>{});
  REQUIRE(combinations(attachables, 1) == vector<set<Level>>{set<Level>{3}, set<Level>{4}, set<Level>{8}, set<Level>{10}});
  REQUIRE(combinations(attachables, 2) == vector<set<Level>>{set<Level>{3, 4}, set<Level>{3, 8}, set<Level>{4, 8}, set<Level>{3, 10}, set<Level>{4, 10}, set<Level>{8, 10}});
}
