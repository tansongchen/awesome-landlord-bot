#include "base.h"

#include <algorithm>
#include <iostream>

using namespace std;

void output(const Group &group) {
  for (const auto &card : group) cout << card << " ";
}

Level card_to_level(Card card) { return card / 4 + card / 53; }

ostream &operator<<(ostream &os, const Combination &combinations) {
  for (const auto &level : combinations) os << level << " ";
  return os;
}

vector<Combination> combinations(const vector<Level> &universe, unsigned k) {
  vector<Combination> result;
  if (k == 0) return result;
  int n = universe.size();
  int expn = 1 << n;
  int bits = (1 << k) - 1;
  while (bits < expn) {
    Combination combination;
    for (int i = 0; i != n; ++i)
      if ((bits >> i) & 1) combination.insert(universe[i]);
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

Counter::Counter() : array<Count, maximumLevel>{} {}

Counter::Counter(const map<Level, Count> &m) : array<Count, maximumLevel>{} {
  for (const auto &item : m) (*this)[item.first] = item.second;
}

Counter::Counter(const Group &group) : array<Count, maximumLevel>{} {
  for (const Card &card : group) ++(*this)[card_to_level(card)];
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

ostream &operator<<(ostream &os, const Counter &counter) {
  for (const auto &level : allLevels) {
    cout << level << ": " << counter[level] << ", ";
  }
  return os;
}

Hand::Hand(Level _level, Level _length, Count _size, Count _cosize,
           const Combination &_attached)
    : level(_level),
      length(_length),
      size(_size),
      cosize(_cosize),
      attached(_attached) {}

Hand::Hand(const Counter &counter)
    : level(0), length(1), size(1), cosize(0), attached{} {
  const auto it1 = max_element(counter.rbegin(), counter.rend());
  size = *it1;
  level = redJokerLevel - (it1 - counter.rbegin());
  const auto it2 = find_if(it1, counter.rend(),
                           [this](Count count) { return count != this->size; });
  length = it2 - it1;
  const auto it3 = find_if(
      counter.rbegin(), counter.rend(),
      [this](Count count) { return count != this->size && count != 0; });
  if (it3 != counter.rend()) {
    cosize = *it3;
    for (const Level &l : allLevels)
      if (counter[l] == cosize) attached.insert(l);
  }
}

Counter Hand::get_counter() const {
  Counter counter;
  for (Level i = 0; i != length; ++i) counter[level - i] = size;
  if (cosize)
    for (const auto &l : attached) counter[l] = cosize;
  return counter;
}

bool Hand::operator==(const Hand &hand) const {
  return level == hand.level && length == hand.length && size == hand.size &&
         cosize == hand.cosize && attached == hand.attached;
}

ostream &operator<<(ostream &os, const Hand &hand) {
  os << "Level: " << hand.level << ", Length: " << hand.length
     << ", Size: " << hand.size << ", Cosize: " << hand.cosize
     << ", Attached: " << hand.attached;
  return os;
}
