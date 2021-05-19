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

Counter::Counter() {}

Counter::Counter(const Group &group) {
  for (const Card &card: group) ++(*this)[card_to_level(card)];
}

Hand::Hand(): size(0) {}

Hand::Hand(Level _level, Level _length, Count _size, Count _cosize): level(_level), length(_length), size(_size), cosize(_cosize) {}

Hand::Hand(bool error): size(-1) {}

Hand::Hand(const Counter &counter) {
  auto it1 = max_element(counter.rbegin(), counter.rend());
  size = *it1;
  level = it1 - counter.rbegin();
  auto it2 = find_if_not(it1, counter.rend(), [this](Count count){ return count == this->size; });
  length = it1 - it2;
  cosize = it2 == counter.rend() ? 0 : *it2;
}

Hand::operator Counter() {
  Counter counter;
  for (auto l = level; l != level - length; --l) counter[l] = size;
  if (cosize) for (const auto &l : attached) counter[l] = cosize;
  return counter;
}

bool Hand::is_not_found() { return size == -1; }
