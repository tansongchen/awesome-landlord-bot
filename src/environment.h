#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <array>
#include <set>
#include <vector>

#include "base.h"

using namespace std;

enum class Stage {
  BIDDING,  // 叫分阶段
  PLAYING   // 打牌阶段
};

/* 状态 */

// 阶段
extern Stage stage;
// 我的牌有哪些
extern Group own;
// 地主明示的牌有哪些
extern Group public_card;
extern array<unsigned, playerCount> history_layout;
// 大家从最开始到现在都出过什么
extern vector<Group> playing_history;
// 自己的第一回合收到的叫分决策
extern vector<unsigned> bidding_history;
// 当前要出的牌需要大过什么
extern Hand last_hand;
// 当前要出的牌需要大过谁
extern unsigned last_player;
// 大家还剩多少牌
extern array<unsigned, playerCount> remaining_cards;
// 我是几号玩家（0-地主，1-农民甲，2-农民乙）
extern unsigned pos;
// 地主位置
extern unsigned landlord;
// 地主叫分
extern unsigned final_bid;
// 剩余多少未知牌(删去自己所有初始牌，删去公共牌，删去对方的出牌)
extern Counter unobserved;

/* 输入输出 */

// 输入
void read();
// 输出叫分（0, 1, 2, 3 四种之一）
void bid(int bidValue, short value, unsigned short round);
// 输出打牌决策
void play(const set<Card> &group, const Hand &last_hand, const Hand &hand);

#endif
