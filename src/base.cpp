#include <algorithm>
#include "base.h"

using namespace std;

Level cardToLevel(Card card) { return card / 4 + card / 53; }

set<Card> levelToCard(Level level) {
  if (level == blackJokerLevel) return {blackJoker};
  if (level == redJokerLevel) return {redJoker};
  Card card = level * 4;
  set<Card> s;
  for (int i = 0; i != 4; ++i) { s.insert(card); ++card; }
  return s;
}

vector<vector<Level>> combinations(const vector<Level> &universe, unsigned k) {
  vector<vector<Level>> result;
  unsigned n = universe.size();
  unsigned expn = 1 << n;
  unsigned bits = (1 << k) - 1;
  while (bits < expn) {
    vector<Level> combination;
    for (int i = 0; i != n; ++i) if (bits >> i & 1) combination.push_back(universe[i]);
    int x = bits & -bits;
    int y = bits + x;
    int z = (bits & ~y);
    bits = z / x;
    bits >>= 1;
    bits |= y;
  }
  return result;
}

Counter::Counter() {}

Counter::Counter(const Group &group) {
  for (const Card &card: group) ++(*this)[cardToLevel(card)];
}
