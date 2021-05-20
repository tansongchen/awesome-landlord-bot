#include <algorithm>
#include <iostream>
#include "base.h"

using namespace std;

Level card_to_level(Card card) { return card / 4 + card / 53; }

vector<Combination> combinations(const vector<Level> &universe, unsigned k) {
  vector<Combination> result;
  if (k == 0) return result;
  int n = universe.size();
  int expn = 1 << n;
  int bits = (1 << k) - 1;
  while (bits < expn) {
    Combination combination;
    for (int i = 0; i != n; ++i) if ((bits >> i) & 1) combination.insert(universe[i]);
    int x = bits & -bits;
    int y = bits + x;
    int z = (bits & ~y);
    bits = z / x;
    bits >>= 1;
    bits |= y;
    result.push_back(combination);
  }
  return result;
}

Counter::Counter(): array<Count, maximumLevel>{} {}

Counter::Counter(map<Level, Count> m): array<Count, maximumLevel>{} {
  for (const auto &item : m) (*this)[item.first] = item.second;
}

Counter::Counter(const Group &group): array<Count, maximumLevel>{} {
  for (const Card &card: group) ++(*this)[card_to_level(card)];
}

Group Counter::get_group(const Group &myCards) const {
  Group group;
  Counter counter(*this);
  for (const auto card : myCards) {
    Level l = card_to_level(card);
    if (counter[l]) {
      group.insert(card);
      --counter[l];
    }
  }
  return group;
}

Hand::Hand(Level _level, Level _length, Count _size, Count _cosize): level(_level), length(_length), size(_size), cosize(_cosize) {}

Hand::Hand(const Counter &counter) {
  auto it1 = max_element(counter.rbegin(), counter.rend());
  size = *it1;
  level = it1 - counter.rbegin();
  auto it2 = find_if_not(it1, counter.rend(), [this](Count count){ return count == this->size; });
  length = it1 - it2;
  cosize = it2 == counter.rend() ? 0 : *it2;
}

Counter Hand::get_counter() const {
  Counter counter;
  for (auto l = level; l != level - length; --l) counter[l] = size;
  if (cosize) for (const auto &l : attached) counter[l] = cosize;
  return counter;
}

bool Hand::operator==(const Hand &hand) const {
  return level == hand.level;
}
