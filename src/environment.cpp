#include "environment.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <array>

#include "jsoncpp/json.h"

Stage stage = Stage::BIDDING;
Group public_card;
unsigned landlord = -1;
unsigned pos = -1;
unsigned final_bid = -1;
array<unsigned, playerCount> history_layout = {-1, -1, -1};
vector<unsigned> bidding_history;
vector<Group> playing_history;
Group own;
array<unsigned, playerCount> remaining_cards = {17, 17, 17};
Counter unobserved{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1};
unsigned last_player = -1;
Hand last_hand = pass;

template <typename Iterable>
Group iterable_to_group(const Iterable &iterable) {
  Group group;
  for (unsigned _ = 0; _ < iterable.size(); _++) {
    Card card = iterable[_].asInt();
    group.insert(card);
  }
  return group;
}

void read() {
  string line;
  getline(cin, line);
  Json::Value input;
  Json::Reader reader;
  reader.parse(line, input);
  auto requests = input["requests"];
  auto responses = input["responses"];

  // Bidding Request
  {
    auto bidding_request = requests[0u];
    own = iterable_to_group(bidding_request["own"]);
    for (const auto &card : own) --unobserved[card_to_level(card)];
    // if the bidding process didn't terminate before me
    if (!bidding_request["bid"].isNull()) {
      auto _bidding_history = bidding_request["bid"];
      for (unsigned i = 0; i < _bidding_history.size(); i++)
        bidding_history.push_back(_bidding_history[i].asInt());
    }
  }

  // First Playing Request
  if (requests.size() > 1) {
    stage = Stage::PLAYING;
    auto first_playing_request = requests[1u];
    public_card = iterable_to_group(first_playing_request["publiccard"]);
    landlord = first_playing_request["landlord"].asInt();
    final_bid = first_playing_request["finalbid"].asInt();
    pos = first_playing_request["pos"].asInt();
    remaining_cards[landlord] += public_card.size();
    if (landlord == pos) own.insert(public_card.begin(), public_card.end());
    history_layout = {(pos + 1) % playerCount, (pos + 2) % playerCount, pos};
  }

  // All Playing Requests
  for (unsigned i = 1u; i < requests.size(); ++i) {
    auto request = requests[i];
    auto history = request["history"];
    for (int position_in_history = 0; position_in_history < 2; position_in_history++) {
      Group group = iterable_to_group(history[position_in_history]);
      playing_history.push_back(group);
      for (auto &card : group) --unobserved[card_to_level(card)];
      unsigned position = history_layout[position_in_history];
      remaining_cards[position] -= group.size();
    }
    if (i != responses.size()) {
      auto response = responses[i];
      Group group = iterable_to_group(response);
      playing_history.push_back(group);
    }
  }
}

void bid(int bidValue, short value, unsigned short turn) {
  Json::Value result, debug(Json::objectValue);
  result["response"] = bidValue;

  debug["pair.score()"] = value - 7 * turn;
  debug["pair.value"] = value;
  debug["pair.turn"] = turn;
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
