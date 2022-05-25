#include "../strategy.h"

#include <algorithm>

Hand normal_selector(Counter *counter) {
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
      updater(&best_pair, &best_hand, hand, counter);
      for (cosize = 1; cosize != 3; ++cosize) {
        vector<Level> attachables;
        copy_if(allLevels.begin(), allLevels.end(), back_inserter(attachables), [&](Level l){ return (*counter)[l] >= cosize && l != level; });
        for (const auto &combination : combinations(attachables, length)) {
          attached = combination;
          for (const auto &l : combination) (*counter)[l] -= cosize;
          updater(&best_pair, &best_hand, hand, counter);
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
        updater(&best_pair, &best_hand, hand, counter);
        for (unsigned i = 0; i != length; ++i) ++(*counter)[level - i];
      }
    } else continuous_number[0] = 0;
  }
  // Minimal Solo, Pair and Trio
  level = find_if(counter->begin(), counter->end(), [](Count count){ return count == 1 || count == 2 || count == 3; }) - counter->begin();
  if (level != maximumLevel) {
    length = 1;
    size = (*counter)[level];
    cosize = 0;
    (*counter)[level] -= size;
    updater(&best_pair, &best_hand, hand, counter);
    (*counter)[level] += size;
  }
  // Four with dual Solo, Four with dual Pair
  size = 4; length = 1;
  for (level = 0; level != maximumLevel; ++level) {
    if ((*counter)[level] == 4) {
      (*counter)[level] -= size;
      for (cosize = 1; cosize != 3; ++cosize) {
        vector<Level> attachables;
        copy_if(allLevels.begin(), allLevels.end(), back_inserter(attachables), [&](Level l){ return (*counter)[l] >= cosize && l != level; });
        for (const auto &combination : combinations(attachables, length * 2)) {
          attached = combination;
          for (const auto &l : combination) (*counter)[l] -= cosize;
          updater(&best_pair, &best_hand, hand, counter);
          for (const auto &l : combination) (*counter)[l] += cosize;
        }
      }
      (*counter)[level] += size;
    }
  }
  return best_hand;
}
