#include "../strategy.h"

#include <algorithm>

Hand selfish_1_selector(Counter *counter) {
  Score best_score = minimumScore;
  Hand best_hand(not_found), hand;
  auto &level = hand.level, &length = hand.length;
  auto &size = hand.size, &cosize = hand.cosize;
  auto &attached = hand.attached;
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
          update(&best_score, &best_hand, hand, counter);
          for (const auto &l : combination) (*counter)[l] += cosize;
        }
      }
      (*counter)[level] += size;
    }
  }
  // Double Chain, Trio Chain, Trio with Solo Chain, Trio with Pair Chain
  array<Level, 3> continuous_number{0, 0, 0};
  for (level = 0; level != maximumLevel; ++level) {
    if ((*counter)[level] >= 2) {
      ++continuous_number[1];
      size = 2; cosize = 0;
      for (length = 3; length <= continuous_number[0]; ++length) {
        for (unsigned i = 0; i != length; ++i) (*counter)[level - i] -= 2;
        update(&best_score, &best_hand, hand, counter);
        for (unsigned i = 0; i != length; ++i) (*counter)[level - i] += 2;
      }
      if ((*counter)[level] >= 3) {
        size = 3;
        ++continuous_number[2];
        for (length = 2; length <= continuous_number[1]; ++length) {
          for (unsigned i = 0; i != length; ++i) (*counter)[level - i] -= 3;
          for (cosize = 0; cosize != 3; ++cosize) {
            vector<Level> attachables;
            copy_if(allLevels.begin(), allLevels.end(), back_inserter(attachables), [=](Level l){ return (*counter)[l] >= cosize && (l > level || l <= level - length); });
            for (auto &combination : combinations(attachables, length)) {
              attached = combination;
              for (auto &l : combination) (*counter)[l] -= cosize;
              update(&best_score, &best_hand, hand, counter);
              for (auto &l : combination) (*counter)[l] += cosize;
            }
          }
          for (unsigned i = 0; i != length; ++i) (*counter)[level - i] += 3;
        }
      } else continuous_number[2] = 0;
    } else continuous_number[1] = continuous_number[2] = 0;
  }
  return best_hand;
}
