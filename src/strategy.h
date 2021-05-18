#ifndef STRATEGY_H
#define STRATEGY_H

#include <set>
#include <array>
#include <vector>
#include "base.h"

using namespace std;

struct Abstraction {
  // 最大主牌的等级
  Level level = -1;
  // 连续的长度
  Level length = 0;
  // 主牌张数
  Count size = 0;
  // 副牌张数
  Count cosize = 0;
  vector<Level> attached;
  Value getValue();
  Abstraction() {}
  Abstraction(const Counter &counter);
};

unsigned biddingSuggestion(Counter *counter);
Abstraction defend(Counter *counter, const Abstraction &abstraction);
Abstraction attack(Counter *counter);

#endif
