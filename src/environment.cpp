#include <algorithm>

#include "environment.h"
#include "jsoncpp/json.h"

Environment::Environment() {
  // 读入输入（平台上的输入是单行）
  string line;
  getline(cin, line);
  Json::Value input;
  Json::Reader reader;
  reader.parse(line, input);

  // 首先处理第一回合，得知自己是谁、有哪些牌
  {
    auto firstRequest = input["requests"][0u]; // 下标需要是 unsigned，可以通过在数字后面加u来做到
    auto own = firstRequest["own"];
    for (unsigned i = 0; i < own.size(); i++) hand.insert(own[i].asInt());
    if (!firstRequest["bid"].isNull()) {
      // 如果还可以叫分，则记录叫分
      auto bid = firstRequest["bid"];
      position = bid.size();
      for (unsigned i = 0; i < bid.size(); i++)
        biddingHistory.push_back(bid[i].asInt());
    }
  }

  // history里第一项（上上家）和第二项（上家）分别是谁的决策
  int whoInHistory[] = {(position - 2 + PLAYER_COUNT) % PLAYER_COUNT, (position - 1 + PLAYER_COUNT) % PLAYER_COUNT};

  int turn = input["requests"].size();
  for (int i = 0; i < turn; i++) {
    auto request = input["requests"][i];
    auto llpublic = request["publiccard"];
    if (!llpublic.isNull()) {
      // 第一次得知公共牌、地主叫分和地主是谁
      landlordPosition = request["landlord"].asInt();
      landlordBid = request["finalbid"].asInt();
      position = request["pos"].asInt();
      cardRemaining[landlordPosition] += llpublic.size();
      for (unsigned i = 0; i < llpublic.size(); i++) {
        leftover.insert(llpublic[i].asInt());
        if (landlordPosition == position)
          hand.insert(llpublic[i].asInt());
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
      Group playedCards;
      for (unsigned _ = 0; _ < playerAction.size();
            _++)  // 循环枚举这个人出的所有牌
      {
        Card card = playerAction[_].asInt();  // 这里是出的一张牌
        playedCards.insert(card);
      }
      whatTheyPlayed[player].push_back(playedCards);  // 记录这段历史
      cardRemaining[player] -= playerAction.size();

      if (playerAction.size() == 0)
        howManyPass++;
      else
        lastValidCombo = playedCards;
    }

    if (i < turn - 1) {
      // 还要恢复自己曾经出过的牌
      auto playerAction = input["responses"][i];  // 出的哪些牌
      Group playedCards;
      for (unsigned _ = 0; _ < playerAction.size();
            _++)  // 循环枚举自己出的所有牌
      {
        int card = playerAction[_].asInt();  // 这里是自己出的一张牌
        hand.erase(card);                 // 从自己手牌中删掉
        playedCards.insert(card);
      }
      whatTheyPlayed[position].push_back(playedCards);  // 记录这段历史
      cardRemaining[position] -= playerAction.size();
    }
  }
}

void Environment::output(const unsigned &score) {
  Json::Value result;
  result["response"] = score;

  Json::FastWriter writer;
  cout << writer.write(result) << endl;
}

void Environment::output(const Group &group) {
  Json::Value result, response(Json::arrayValue);
  for (auto &card : group) response.append(card);
  result["response"] = response;

  Json::FastWriter writer;
  cout << writer.write(result) << endl;
}

unsigned Environment::bid() {
  auto maxBidIt = max_element(biddingHistory.begin(), biddingHistory.end());
  int maxBid = maxBidIt == biddingHistory.end() ? -1 : *maxBidIt;
  unsigned bidValue = biddingSuggestion(&counter);
  return bidValue > maxBid ? bidValue : 0;
}

Group Environment::play() {
  Counter last_counter(lastValidCombo);
  Abstraction last_abstraction(last_counter);
  Abstraction abstraction = last_abstraction.size ? defend(&counter, last_abstraction) : attack(&counter);
  Group group;
  for (auto l = abstraction.level; l != abstraction.level - abstraction.length; --l) {
    for (auto _ = 0; _ != abstraction.size; ++_) {
      set<Card> v = levelToCard(l);
      for (auto card : v) {
        if (hand.find(card) != hand.end()) {
          hand.erase(card);
          group.insert(card);
        }
      }
    }
  }
  if (abstraction.cosize) for (auto l : abstraction.attached) {
    for (auto _ = 0; _ != abstraction.cosize; ++_) {
      set<Card> v = levelToCard(l);
      for (auto card : v) {
        if (hand.find(card) != hand.end()) {
          hand.erase(card);
          group.insert(card);
        }
      }
    }
  }
  return group;
}
