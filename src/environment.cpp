#include "environment.h"

#include <algorithm>
#include <iostream>
#include <sstream>

#include "jsoncpp/json.h"

Stage stage = Stage::BIDDING;
set<Card> landlordPublicCards;
vector<set<Card>> whatTheyPlayed[PLAYER_COUNT];
set<Card> lastValidCombo;
short cardRemaining[PLAYER_COUNT] = {17, 17, 17};
int landlordPosition = -1;
int landlordBid = -1;
vector<int> bidInput;
int myPosition;
set<Card> myCards;
array<Count, maximumLevel> unknown = {4, 4, 4, 4, 4, 4, 4, 4,
                                      4, 4, 4, 4, 4, 1, 1};
map<vector<unsigned short>, Level> outplay;

void read() {
  // 读入输入（平台上的输入是单行）
  string line;
  getline(cin, line);
  Json::Value input;
  Json::Reader reader;
  reader.parse(line, input);

  // 首先处理第一回合，得知自己是谁、有哪些牌
  {
    auto firstRequest =
        input["requests"]
             [0u];  // 下标需要是 unsigned，可以通过在数字后面加u来做到
    auto own = firstRequest["own"];
    for (unsigned i = 0; i < own.size(); i++) {
      Card card = own[i].asInt();
      myCards.insert(card);
      unknown[card_to_level(card)]--;
    }
    if (!firstRequest["bid"].isNull()) {
      // 如果还可以叫分，则记录叫分
      auto bidHistory = firstRequest["bid"];
      myPosition = bidHistory.size();
      for (unsigned i = 0; i < bidHistory.size(); i++)
        bidInput.push_back(bidHistory[i].asInt());
    }
  }

  // history里第一项（上上家）和第二项（上家）分别是谁的决策
  int whoInHistory[] = {(myPosition - 2 + PLAYER_COUNT) % PLAYER_COUNT,
                        (myPosition - 1 + PLAYER_COUNT) % PLAYER_COUNT};

  int turn = input["requests"].size();
  auto request = input["requests"][0];
  auto llpublic = request["publiccard"];
  if (!llpublic.isNull()) {
    // 第一次得知公共牌、地主叫分和地主是谁
    landlordPosition = request["landlord"].asInt();
    landlordBid = request["finalbid"].asInt();
    myPosition = request["pos"].asInt();
    cardRemaining[landlordPosition] += llpublic.size();
    for (unsigned j = 0; j < llpublic.size(); j++) {
      int card = llpublic[j].asInt();
      landlordPublicCards.insert(card);
      unknown[card_to_level(card)]--;
      if (landlordPosition == myPosition) myCards.insert(card);
    }
  }

  for (int i = 0; i < turn; i++) {
    auto request = input["requests"][i];
    auto history = request["history"];  // 每个历史中有上家和上上家出的牌
    if (history.isNull()) continue;

    stage = Stage::PLAYING;

    // 逐次恢复局面到当前
    int howManyPass = 0;
    for (int p = 0; p < 2; p++) {
      unsigned short player = whoInHistory[p];  // 是谁出的牌
      auto playerAction = history[p];           // 出的哪些牌
      set<Card> playedCards;
      // 循环枚举这个人出的所有牌(本轮次)
      for (unsigned _ = 0; _ < playerAction.size(); _++) {
        Card card = playerAction[_].asInt();
        playedCards.insert(card);  // 这里是出的一张牌
        unknown[card_to_level(card)]--;
      }
      whatTheyPlayed[player].push_back(playedCards);  // 记录这段历史
      cardRemaining[player] -= playerAction.size();   // 所有轮次的记录

      // 在出牌一轮时，才寻找未接上的牌(未出 或 回应 炸弹/火箭)
      Counter counter(playedCards);
      Hand hand(counter);
      if (i > 2 && (playedCards.empty() || hand == rocket ||
                    (hand.length == 1 && hand.size == 4))) {
        int outplayer[] = {(player - 2 + PLAYER_COUNT) % PLAYER_COUNT,
                           (player - 1 + PLAYER_COUNT) % PLAYER_COUNT};
        set<Card> lastValidCombo_;
        for (short q = 1; q >= 0; q--) {
          set<Card> playerAction = whatTheyPlayed[outplayer[q]].back();
          if (!playerAction.empty()) {
            lastValidCombo_ = playerAction;
            break;
          }
        }
        Counter counter(lastValidCombo_);
        Hand hand(counter);
        vector<unsigned short> v = {player, hand.length, hand.size,
                                    hand.cosize};
        // 实时更新(取level的最小值)
        if (outplay.find(v) == outplay.end() || outplay[v] > hand.level)
          outplay.insert(make_pair(v, hand.level));
      }

      if (playerAction.size() == 0)
        howManyPass++;
      else
        lastValidCombo = playedCards;
    }

    if (howManyPass == 2) lastValidCombo = set<Card>();

    if (i < turn - 1) {
      // 还要恢复自己曾经出过的牌
      auto playerAction = input["responses"][i];  // 出的哪些牌
      set<Card> playedCards;
      for (unsigned _ = 0; _ < playerAction.size();
           _++)  // 循环枚举自己出的所有牌
      {
        int card = playerAction[_].asInt();  // 这里是自己出的一张牌
        myCards.erase(card);                 // 从自己手牌中删掉
        playedCards.insert(card);
      }
      whatTheyPlayed[myPosition].push_back(playedCards);  // 记录这段历史
      cardRemaining[myPosition] -= playerAction.size();
    }
  }
}

void bid(int bidValue, short value, unsigned short turn_) {
  Json::Value result, debug(Json::objectValue);
  result["response"] = bidValue;

  debug["pair.score()"] = value - 7 * turn_;
  debug["pair.value"] = value;
  debug["pair.turn_"] = turn_;
  result["debug"] = debug;

  Json::FastWriter writer;
  cout << writer.write(result) << endl;
}

void play(const set<Card> &group, const Hand &last_hand, const Hand &hand) {
  Json::Value result, response(Json::arrayValue), debug(Json::objectValue);

  for (const auto &card : group) {
    response.append(card);
  }
  result["response"] = response;

  debug["last_hand"] = static_cast<string>(last_hand);
  debug["hand"] = static_cast<string>(hand);
  result["debug"] = debug;

  Json::FastWriter writer;
  cout << writer.write(result) << endl;
}
