#ifndef BASE_H
#define BASE_H

#include <vector>
#include <set>
#include <array>

using namespace std;

using Card = unsigned char;
constexpr Card blackJoker = 52;
constexpr Card redJoker = 53;

using Group = set<Card>;

using Level = unsigned char;
constexpr Level maximumLevel = 15;
constexpr Level maximumChainableLevel = 12;
constexpr Level blackJokerLevel = 13;
constexpr Level redJokerLevel = 14;
constexpr array<Level, 15> allLevels = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};

Level cardToLevel(Card card);
set<Card> levelToCard(Level level);

using Count = unsigned char;

using Value = char;

struct Counter : public array<Count, maximumLevel> {
  Counter();
  Counter(const Group &group);
  Value getValue();
};

vector<vector<Level>> combinations(const vector<Level> &universe, unsigned k);

#endif
