#include <algorithm>

#include "../environment.h"
#include "../strategy.h"

Hand same_category_selector(Counter *counter, const Hand &last_hand) {
  Level count = 0;
  Score best_score = minimumScore;
  Hand best_hand(not_found), hand;
  Level &level = hand.level, &length = hand.length;
  Count &size = hand.size, &cosize = hand.cosize;
  Combination &attached = hand.attached;
  length = last_hand.length;
  size = last_hand.size;
  cosize = last_hand.cosize;

  // last hand is bomb
  if (length == 1 && size == 4 && cosize == 0) {
    for (level = last_hand.level + 1; level != maximumLevel; ++level) {
      if ((*counter)[level] == 4) return Hand(level, 1, 4);
    }
    return not_found;
  }
  // last hand is not bomb, I do not split bomb
  for (level = last_hand.level - length + 2; level != maximumLevel; ++level) {
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
          for (const auto &combination : combinations(attachables, cosize)) {
            attached = combination;
            for (const auto &l : combination) (*counter)[l] -= cosize;
            update(&best_score, &best_hand, hand, counter);
            for (const auto &l : combination) (*counter)[l] += cosize;
          }
        } else {
          update(&best_score, &best_hand, hand, counter);
        }
        for (Level i = 0; i != length; ++i) (*counter)[level - i] += size;
      }
    }
  }
  return best_hand;
}
