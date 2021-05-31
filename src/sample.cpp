#include "strategy.h"

#include <random>
#include <vector>
#include <set>
#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>

using namespace std;

int main() {
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
    // defending_absolute_dominant_selector,
    same_category_selector,
    defending_bomb_rocket_selector,
    pass_selector
  };

  vector<Card> allCards = {};
  for (Card card = 0; card != 54; ++card) allCards.push_back(card);
  vector<int> stats(70);
  for (unsigned i = 0; i != 10000; ++i) {
    random_shuffle(allCards.begin(), allCards.end());
    Group myCards(allCards.begin(), allCards.begin() + 17);
    Counter counter(myCards);
    Pair pair = evaluate(&counter);
    ++stats[pair.value + 25];
  }
//   Counter counter({0, 5, 9, 11, 12, 13, 28, 36, 38, 41, 42, 43, 45, 48, 49, 51, 52});
//   evaluate(&counter);

}
