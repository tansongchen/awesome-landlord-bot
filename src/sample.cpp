#include "strategy.h"

#include <random>
#include <vector>
#include <set>
#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main() {
  evaluator = simple_evaluator;
  attacking_sequence = {
    one_shot_selector,
    attacking_absolute_dominant_selector,
    selfish_selector,
    normal_selector,
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
  for (unsigned i = 0; i != 1000; ++i) {
    auto start = high_resolution_clock::now();
    random_shuffle(allCards.begin(), allCards.end());
    Group myCards(allCards.begin(), allCards.begin() + 17);
    Counter counter(myCards);
    Pair pair = evaluate(&counter);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << pair.value << '\t' << pair.round << '\t' << duration.count() << endl;
    // ++stats[pair.value + 25];
  }
}
