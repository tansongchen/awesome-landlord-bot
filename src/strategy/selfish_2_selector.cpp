#include "../strategy.h"

#include <algorithm>

Hand selfish_2_selector(Counter *counter) {
  Pair best_pair(-10000, 0);
  Hand best_hand(not_found), hand;
  auto &level = hand.level, &length = hand.length;
  auto &size = hand.size, &cosize = hand.cosize;
  auto &attached = hand.attached;
  // Trio, Trio with Solo, Trio with Pair
  size = 3; length = 1;
  for (level = 0; level != selfishThresholdLevel; ++level) {
    if ((*counter)[level] == 3) {
      (*counter)[level] -= size;
      update(&best_pair, &best_hand, hand, counter);
      for (cosize = 1; cosize != 3; ++cosize) {
        vector<Level> attachables;
        copy_if(allLevels.begin(), allLevels.end(), back_inserter(attachables), [&](Level l){ return (*counter)[l] >= cosize && l != level; });
        for (const auto &combination : combinations(attachables, length)) {
          attached = combination;
          for (const auto &l : combination) (*counter)[l] -= cosize;
          update(&best_pair, &best_hand, hand, counter);
          for (const auto &l : combination) (*counter)[l] += cosize;
        }
      }
      (*counter)[level] += size;
    }
  }
  // Solo Chain
  Level continuous_number[1]{0};
  size = 1; cosize = 0; attached = {};
  for (level = 0; level != maximumChainableLevel; ++level) {
    if ((*counter)[level] >= 1) {
      ++continuous_number[0];
      for (length = 5; length <= continuous_number[0]; ++length) {
        for (unsigned i = 0; i != length; ++i) --(*counter)[level - i];
        update(&best_pair, &best_hand, hand, counter);
        for (unsigned i = 0; i != length; ++i) ++(*counter)[level - i];
      }
    } else continuous_number[0] = 0;
  }
  return best_hand;
}
