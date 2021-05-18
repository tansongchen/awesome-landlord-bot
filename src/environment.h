#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <iostream>
#include <set>
#include <vector>

#include "base.h"
#include "strategy.h"

using namespace std;

constexpr int PLAYER_COUNT = 3;

enum class Stage {
  BIDDING,  // 叫分阶段
  PLAYING   // 打牌阶段
};

class Environment {
 public:
  // 阶段
  Stage stage = Stage::BIDDING;
  // 读入输入（平台上的输入是单行）
  Environment();
  // 输出叫分（0, 1, 2, 3 四种之一）
  void output(const unsigned &score);
  // 输出打牌决策，begin是迭代器起点，end是迭代器终点
  void output(const Group &group);
  // 地主明示的牌有哪些
  Group leftover;
  // 大家从最开始到现在都出过什么
  vector<Group> whatTheyPlayed[PLAYER_COUNT];
  // 当前要出的牌需要大过谁
  Group lastValidCombo;
  // 大家还剩多少牌
  short cardRemaining[PLAYER_COUNT] = {17, 17, 17};
  // 地主位置
  int landlordPosition = -1;
  // 地主叫分
  int landlordBid = -1;
  // 自己的第一回合收到的叫分决策
  vector<unsigned> biddingHistory;
  unsigned bid();
  Group play();
  // 我是几号玩家（0-地主，1-农民甲，2-农民乙）
  int position;
  // 我的牌有哪些
  Group hand;
  Counter counter;
};

#endif
