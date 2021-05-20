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

using Combination = set<Level>;

Level card_to_level(Card card);

using Count = unsigned char;

struct Counter : public array<Count, maximumLevel> {
  Counter();
  Counter(const Group &group);
  Group get_group(const Group &myCards);
};

struct Hand {
  // 最大主牌的等级
  Level level;
  // 连续的长度
  Level length;
  // 主牌张数
  Count size;
  // 副牌张数
  Count cosize;
  // 副牌内容
  Combination attached;
  Hand(Level _level = 0, Level _length = 1, Count _size = 1, Count _cosize = 0);
  Hand(const Counter &counter);
  Counter get_counter();
  bool is_not_found();
};

const Hand pass(-1);
const Hand not_found(-2);

using Value = char;

vector<set<Level>> combinations(const vector<Level> &universe, unsigned k);

#endif
