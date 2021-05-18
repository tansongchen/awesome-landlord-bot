#include <algorithm>
#include "strategy.h"

Abstraction::Abstraction(const Counter &counter) {
  auto it1 = max_element(counter.rbegin(), counter.rend());
  size = *it1;
  level = it1 - counter.rbegin();
  auto it2 = find_if_not(it1, counter.rend(), [this](Count count){ return count == this->size; });
  length = it1 - it2;
  cosize = it2 == counter.rend() ? 0 : *it2;
}

Value Abstraction::getValue() {
  // Pass
  if (size == 0) return 0;
  // Rocket
  if (level == redJokerLevel && length == 2) return 20;
  // Four
  if (size == 4 && length == 1 && cosize == 0) return level - 3 + 7;
  // Four with dual Solo, Four with dual Pair, Four Chain, Four with dual Solo Chain, Four with dual Pair Chain
  if (size == 4) return (level - 3) / 2;
  // Solo, Pair, Trio, Trio with Solo, Trio with Pair
  if (length == 1) return level - 10;
  // Trio Chain, Trio with Solo Chain, Trio with Pair Chain
  if (size == 3) return (level - 3 + 1) / 2;
  // Single Chain, Double Chain
  return level - 10 + 1;
}

Value Counter::getValue() {
  Abstraction abstraction = attack(this);
  for (Level l = abstraction.level; l != abstraction.level - abstraction.length; --l) (*this)[l] -= abstraction.size;
  if (abstraction.cosize) for (Level &l : abstraction.attached) (*this)[l] -= abstraction.cosize;
  auto resValue = getValue();
  for (Level l = abstraction.level; l != abstraction.level - abstraction.length; --l) (*this)[l] += abstraction.size;
  if (abstraction.cosize) for (Level &l : abstraction.attached) (*this)[l] += abstraction.cosize;
  return abstraction.getValue() + resValue;
}

inline void update(Value *best_value, Abstraction *best_abstraction, const Abstraction &abstraction, Counter *counter) {
  auto value = counter->getValue();
  if (value > *best_value) {
    *best_value = value;
    *best_abstraction = abstraction;
  }
}

unsigned biddingSuggestion(Counter *counter) {
  Value value = counter->getValue();
  if (value >= 20) return 3;
  if (value >= 15) return 2;
  if (value >= 10) return 1;
  return 0;
}

Abstraction defend(Counter *counter, const Abstraction &last_abstraction) {
  Level count = 0;
  Value best_value;
  Abstraction best_abstraction, abstraction;
  auto &level = abstraction.level, &length = abstraction.length;
  auto &size = abstraction.size, &cosize = abstraction.cosize;
  length = last_abstraction.length;
  size = last_abstraction.size;
  cosize = last_abstraction.cosize;
  for (level = last_abstraction.level; level != maximumLevel; ++level) {
    if ((*counter)[level] >= size) {
      ++count;
      if (count >= length) {
        for (Level l = level; l != level - length; --l) (*counter)[level] -= size;
        if (cosize) {
          vector<Level> attachables;
          copy_if(allLevels.begin(), allLevels.end(), back_inserter(attachables), [=](Level l){ return (*counter)[l] >= cosize && (l > level || l <= level - length); });
          for (const auto &combination : combinations(attachables, cosize)) {
            for (const auto &l : combination) (*counter)[l] -= cosize;
            update(&best_value, &best_abstraction, abstraction, counter);
            for (const auto &l : combination) (*counter)[l] += cosize;
          }
        }
      }
    }
  }
  return best_abstraction;
}

Abstraction attack(Counter *counter) {
  array<Level, 4> count;
  Value best_value;
  Abstraction best_abstraction, abstraction;
  auto &level = abstraction.level, &length = abstraction.length;
  auto &size = abstraction.size, &cosize = abstraction.cosize;
  for (level = 0; level != maximumLevel; ++level) {
    if ((*counter)[level] > 0) {
      ++count[0];
      --(*counter)[level];
      update(&best_value, &best_abstraction, abstraction, counter);
      ++(*counter)[level];
      for (auto length = count[0]; length >= 5; --length) {
        for (auto l = level; l != level - length; --l) --(*counter)[level];
        update(&best_value, &best_abstraction, abstraction, counter);
        for (auto l = level; l != level - length; --l) ++(*counter)[level];
      }
      if ((*counter)[level] > 1) {
        ++count[1];
        (*counter)[level] -= 2;
        update(&best_value, &best_abstraction, abstraction, counter);
        (*counter)[level] += 2;
        for (auto length = count[1]; length >= 3; --length) {
          for (auto l = level; l != level - length; --l) (*counter)[level] -= 2;
          update(&best_value, &best_abstraction, abstraction, counter);
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
                update(&best_value, &best_abstraction, abstraction, counter);
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
                  update(&best_value, &best_abstraction, abstraction, counter);
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
  return best_abstraction;
}
