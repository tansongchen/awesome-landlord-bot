#include "../strategy.h"

#include <algorithm>

Hand same_category_selector(Counter *counter, const Hand &last_hand) {
  Level count = 0;
  Value best_value;
  Hand best_hand(not_found), hand;
  auto &level = hand.level, &length = hand.length;
  auto &size = hand.size, &cosize = hand.cosize;
  length = last_hand.length;
  size = last_hand.size;
  cosize = last_hand.cosize;
  for (level = last_hand.level; level != maximumLevel; ++level) {
    if ((*counter)[level] >= size) {
      ++count;
      if (count >= length) {
        for (Level l = level; l != level - length; --l) (*counter)[level] -= size;
        if (cosize) {
          vector<Level> attachables;
          copy_if(allLevels.begin(), allLevels.end(), back_inserter(attachables), [=](Level l){ return (*counter)[l] >= cosize && (l > level || l <= level - length); });
          for (const auto &combination : combinations(attachables, cosize)) {
            for (const auto &l : combination) (*counter)[l] -= cosize;
            update(&best_value, &best_hand, hand, counter);
            for (const auto &l : combination) (*counter)[l] += cosize;
          }
        }
      }
    }
  }
  return best_hand;
}
