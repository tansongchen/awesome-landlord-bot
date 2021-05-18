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
  REQUIRE(cardToLevel(card1) == level1);
  REQUIRE(cardToLevel(card2) == level2);
  REQUIRE(cardToLevel(card3) == level3);
  REQUIRE(levelToCard(level1) == set<Card>{card1});
  REQUIRE(levelToCard(level2) == set<Card>{card2});
  REQUIRE(levelToCard(level3) == set<Card>{16, 17, 18, 19});
}
