#include <algorithm>

#include "../environment.h"
#include "../strategy.h"

Hand same_category_selector(Counter *counter, const Hand &last_hand) {
  Level count = 0;
  Pair best_pair = evaluate(counter);
  best_pair.round += 1;
  Hand best_hand(not_found), hand;
  Level &level = hand.level, &length = hand.length;
  Count &size = hand.size, &cosize = hand.cosize;
  Combination &attached = hand.attached;
  length = last_hand.length;
  size = last_hand.size;
  cosize = last_hand.cosize;

  if (last_hand == rocket) return not_found;

  // last hand is bomb
  if (length == 1 && size == 4 && cosize == 0) {
    for (level = last_hand.level + 1; level != maximumLevel; ++level) {
      if ((*counter)[level] == 4) return Hand(level, 1, 4);
    }
    return not_found;
  }
  // last hand is not bomb, I do not split bomb
  Level maximum = length > 1 ? maximumChainableLevel : maximumLevel;
  for (level = last_hand.level - length + 2; level != maximum; ++level) {
    if ((*counter)[level] < size || (*counter)[level] == 4)
      count = 0;
    else {
      ++count;
      if (count >= length) {
        for (Level i = 0; i != length; ++i) (*counter)[level - i] -= size;
        if (cosize) {
          vector<Level> attachables;
          copy_if(allLevels.begin(), allLevels.end(),
                  back_inserter(attachables), [=](Level l) {
                    return (*counter)[l] >= cosize && (*counter)[l] < 4 &&
                           (l > level || l <= level - length);
                  });
          auto k = size == 4 ? 2 * length : length;
          for (const auto &combination : combinations(attachables, k)) {
            attached = combination;
            for (const auto &l : combination) (*counter)[l] -= cosize;
            updater(&best_pair, &best_hand, hand, counter);
            for (const auto &l : combination) (*counter)[l] += cosize;
          }
        } else {
          updater(&best_pair, &best_hand, hand, counter);
        }
        for (Level i = 0; i != length; ++i) (*counter)[level - i] += size;
      }
    }
  }
  return best_hand;
}
