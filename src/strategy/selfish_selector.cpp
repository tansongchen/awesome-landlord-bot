#include "../strategy.h"

#include <algorithm>

Hand selfish_selector(Counter *counter) {
  Pair best_pair(-10000, 0);
  Hand best_hand(not_found), hand;
  auto &level = hand.level, &length = hand.length;
  auto &size = hand.size, &cosize = hand.cosize;
  auto &attached = hand.attached;
  // Double Chain, Trio Chain, Trio with Solo Chain, Trio with Pair Chain
  array<Level, 3> continuous_number{0, 0, 0};
  for (level = 0; level != maximumChainableLevel; ++level) {
    if ((*counter)[level] >= 2) {
      ++continuous_number[1];
      size = 2; cosize = 0;
      for (length = 3; length <= continuous_number[1]; ++length) {
        for (unsigned i = 0; i != length; ++i) (*counter)[level - i] -= 2;
        updater(&best_pair, &best_hand, hand, counter);
        for (unsigned i = 0; i != length; ++i) (*counter)[level - i] += 2;
      }
      if ((*counter)[level] >= 3) {
        size = 3; cosize = 0;
        ++continuous_number[2];
        for (length = 2; length <= continuous_number[2]; ++length) {
          for (unsigned i = 0; i != length; ++i) (*counter)[level - i] -= 3;
          updater(&best_pair, &best_hand, hand, counter);
          for (cosize = 1; cosize != 3; ++cosize) {
            vector<Level> attachables;
            copy_if(allLevels.begin(), allLevels.end(), back_inserter(attachables), [=](Level l){ return (*counter)[l] >= cosize && (l > level || l <= level - length); });
            for (const auto &combination : combinations(attachables, length)) {
              attached = combination;
              for (const auto &l : combination) (*counter)[l] -= cosize;
              updater(&best_pair, &best_hand, hand, counter);
              for (const auto &l : combination) (*counter)[l] += cosize;
            }
          }
          for (unsigned i = 0; i != length; ++i) (*counter)[level - i] += 3;
        }
      } else continuous_number[2] = 0;
    } else continuous_number[1] = continuous_number[2] = 0;
  }
  return best_hand;
}
