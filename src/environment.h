#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <set>
#include <vector>

#include "base.h"

using namespace std;

constexpr int PLAYER_COUNT = 3;

enum class Stage {
  BIDDING,  // 叫分阶段
  PLAYING   // 打牌阶段
};

/* 状态 */

// 我的牌有哪些
extern set<Card> myCards;
// 地主明示的牌有哪些
extern set<Card> landlordPublicCards;
// 大家从最开始到现在都出过什么
extern vector<vector<Card>> whatTheyPlayed[PLAYER_COUNT];
// 当前要出的牌需要大过谁
extern set<Card> lastValidCombo;
// 大家还剩多少牌
extern short cardRemaining[PLAYER_COUNT];
// 我是几号玩家（0-地主，1-农民甲，2-农民乙）
extern int myPosition;
// 地主位置
extern int landlordPosition;
// 地主叫分
extern int landlordBid;
// 阶段
extern Stage stage;
// 自己的第一回合收到的叫分决策
extern vector<int> bidInput;

/* 输入输出 */

// 输入
void read();
// 输出叫分（0, 1, 2, 3 四种之一）
void bid(int bidValue, short value, unsigned short round);
// 输出打牌决策
void play(const set<Card> &group, const Hand &last_hand, const Hand &hand);

#endif
