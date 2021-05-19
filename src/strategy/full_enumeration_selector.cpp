#include "../strategy.h"

#include <algorithm>

Hand full_enumeration_selector(Counter *counter) {
  array<Level, 4> count;
  Value best_value;
  Hand best_hand, hand;
  auto &level = hand.level, &length = hand.length;
  auto &size = hand.size, &cosize = hand.cosize;
  for (level = 0; level != maximumLevel; ++level) {
    if ((*counter)[level] > 0) {
      ++count[0];
      --(*counter)[level];
      update(&best_value, &best_hand, hand, counter);
      ++(*counter)[level];
      for (auto length = count[0]; length >= 5; --length) {
        for (auto l = level; l != level - length; --l) --(*counter)[level];
        update(&best_value, &best_hand, hand, counter);
        for (auto l = level; l != level - length; --l) ++(*counter)[level];
      }
      if ((*counter)[level] > 1) {
        ++count[1];
        (*counter)[level] -= 2;
        update(&best_value, &best_hand, hand, counter);
        (*counter)[level] += 2;
        for (auto length = count[1]; length >= 3; --length) {
          for (auto l = level; l != level - length; --l) (*counter)[level] -= 2;
          update(&best_value, &best_hand, hand, counter);
          for (auto l = level; l != level - length; --l) (*counter)[level] += 2;
        }
        if ((*counter)[level] > 2) {
          ++count[2];
          for (auto length = count[2]; length >= 1; --length) {
            for (auto l = level; l != level - length; --l) (*counter)[level] -= 3;
            for (auto cosize = 0; cosize != 3; ++cosize) {
              vector<Level> attachables;
              copy_if(allLevels.begin(), allLevels.end(), back_inserter(attachables), [=](Level l){ return (*counter)[l] >= cosize && (l > level || l <= level - length); });
              for (auto &combination : combinations(attachables, cosize)) {
                for (auto &l : combination) (*counter)[l] -= cosize;
                update(&best_value, &best_hand, hand, counter);
                for (auto &l : combination) (*counter)[l] += cosize;
              }
            }
            for (auto l = level; l != level - length; --l) (*counter)[level] += 3;
          }
          if (count[level] > 3) {
            ++count[3];
            for (auto length = count[3]; length >= 1; --length) {
              for (auto l = level; l != level - length; --l) (*counter)[level] -= 4;
              for (auto cosize = 0; cosize != 3; ++cosize) {
                vector<Level> attachables;
                copy_if(allLevels.begin(), allLevels.end(), back_inserter(attachables), [=](Level l){ return (*counter)[l] >= cosize && (l > level || l <= level - length); });
                for (auto &combination : combinations(attachables, 2 * cosize)) {
                  for (auto &l : combination) (*counter)[l] -= cosize;
                  update(&best_value, &best_hand, hand, counter);
                  for (auto &l : combination) (*counter)[l] += cosize;
                }
              }
              for (auto l = level; l != level - length; --l) (*counter)[level] += 4;
            }
          } else count[3] = 0;
        } else count[2] = count[3] = 0;
      } else count[1] = count[2] = count[3] = 0;
    } else count[0] = count[1] = count[2] = count[3] = 0;
  }
  return best_hand;
}
