#include <algorithm>
#include <iostream>

#include "environment.h"
#include "jsoncpp/json.h"

Stage stage = Stage::BIDDING;
set<Card> landlordPublicCards;
vector<vector<Card>> whatTheyPlayed[PLAYER_COUNT];
set<Card> lastValidCombo;
short cardRemaining[PLAYER_COUNT] = {17, 17, 17};
int landlordPosition = -1;
int landlordBid = -1;
vector<int> bidInput;
int myPosition;
set<Card> myCards;

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
    for (unsigned i = 0; i < own.size(); i++) myCards.insert(own[i].asInt());
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
  for (int i = 0; i < turn; i++) {
    auto request = input["requests"][i];
    auto llpublic = request["publiccard"];
    if (!llpublic.isNull()) {
      // 第一次得知公共牌、地主叫分和地主是谁
      landlordPosition = request["landlord"].asInt();
      landlordBid = request["finalbid"].asInt();
      myPosition = request["pos"].asInt();
      cardRemaining[landlordPosition] += llpublic.size();
      for (unsigned i = 0; i < llpublic.size(); i++) {
        landlordPublicCards.insert(llpublic[i].asInt());
        if (landlordPosition == myPosition) myCards.insert(llpublic[i].asInt());
      }
    }

    auto history = request["history"];  // 每个历史中有上家和上上家出的牌
    if (history.isNull()) continue;
    stage = Stage::PLAYING;

    // 逐次恢复局面到当前
    int howManyPass = 0;
    for (int p = 0; p < 2; p++) {
      int player = whoInHistory[p];    // 是谁出的牌
      auto playerAction = history[p];  // 出的哪些牌
      vector<Card> playedCards;
      for (unsigned _ = 0; _ < playerAction.size();
           _++)  // 循环枚举这个人出的所有牌
      {
        int card = playerAction[_].asInt();  // 这里是出的一张牌
        playedCards.push_back(card);
      }
      whatTheyPlayed[player].push_back(playedCards);  // 记录这段历史
      cardRemaining[player] -= playerAction.size();

      if (playerAction.size() == 0)
        howManyPass++;
      else
        lastValidCombo = set<Card>(playedCards.begin(), playedCards.end());
    }

    if (howManyPass == 2) lastValidCombo = set<Card>();

    if (i < turn - 1) {
      // 还要恢复自己曾经出过的牌
      auto playerAction = input["responses"][i];  // 出的哪些牌
      vector<Card> playedCards;
      for (unsigned _ = 0; _ < playerAction.size();
           _++)  // 循环枚举自己出的所有牌
      {
        int card = playerAction[_].asInt();  // 这里是自己出的一张牌
        myCards.erase(card);                 // 从自己手牌中删掉
        playedCards.push_back(card);
      }
      whatTheyPlayed[myPosition].push_back(playedCards);  // 记录这段历史
      cardRemaining[myPosition] -= playerAction.size();
    }
  }
}

void bid(int value) {
  Json::Value result;
  result["response"] = value;

  Json::FastWriter writer;
  cout << writer.write(result) << endl;
}

void play(const set<Card> &group) {
  Json::Value result, response(Json::arrayValue);
  for (auto &card : group) response.append(card);
  result["response"] = response;

  Json::FastWriter writer;
  cout << writer.write(result) << endl;
}
