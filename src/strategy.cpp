#include "strategy.h"
#include "environment.h"

#include <algorithm>

Updater updater = nullptr;
Evaluator evaluator = nullptr;
vector<AttackingSelector> attacking_sequence;
vector<DefendingSelector> defending_sequence;

array<Score, playerCount> scores = {minimumScore, minimumScore, minimumScore};
array<map<Category, Level>, playerCount> dominant_level;
bool supportive = false;

Pair::Pair(Value _value, unsigned short _round): value(_value), round(_round) {}

Score Pair::score() const {
  return value - 7 * round;
}

bool Pair::operator==(const Pair &rhs) const {
  return value == rhs.value && round == rhs.round;
}

void update(Pair *best_pair, Hand *best_hand, const Hand &hand,
            Counter *counter) {
  Pair pair = evaluate(counter);
  if (pair.score() > best_pair->score()) {
    *best_pair = pair;
    *best_hand = Hand(hand);
  }
}

Pair evaluate(Counter *counter) {
  if (*counter == Counter()) return Pair();
  Hand hand = attack(counter);
  for (Level i = 0; i != hand.length; ++i)
    (*counter)[hand.level - i] -= hand.size;
  if (hand.cosize)
    for (const Level &l : hand.attached) (*counter)[l] -= hand.cosize;
  Pair pair = evaluate(counter);
  for (Level i = 0; i != hand.length; ++i)
    (*counter)[hand.level - i] += hand.size;
  if (hand.cosize)
    for (const Level &l : hand.attached) (*counter)[l] += hand.cosize;
  return Pair(pair.value + evaluator(hand), pair.round + 1);
}

unsigned value_to_bidding(Value value) {
  return value >= 15 ? 3 : value >= 10 ? 2 : value >= 5 ? 1 : 0;
}

Value bidding_to_value(unsigned bidding) {
  switch (bidding) {
    case 0: return 0;
    case 1: return 5;
    case 2: return 10;
    case 3: return 15;
    default: throw runtime_error("Invalid bidding");
  }
}

Hand defend(Counter *counter, const Hand &last_hand) {
  if (supportive && last_player != landlord) return pass;
  for (auto &selector : defending_sequence) {
    const Hand hand = selector(counter, last_hand);
    if (hand == not_found) continue;
    return hand;
  }
  throw runtime_error("No suitable hand found during defending!");
}

Hand attack(Counter *counter) {
  for (auto &selector : attacking_sequence) {
    const Hand hand = selector(counter);
    if (hand == not_found) continue;
    return hand;
  }
  throw runtime_error("No suitable hand found during attacking!");
}

Score inverse(unsigned);

void analysis() {
  // initial score
  Counter counter(own);
  scores[pos] = evaluate(&counter).value;
  if (pos != landlord) {
    scores[landlord] = inverse(final_bid);
  }
  for (unsigned position_in_history = 0; position_in_history != bidding_history.size(); ++position_in_history) {
    unsigned bidding_value = bidding_history[position_in_history];
    unsigned position = history_layout[position_in_history];
    Score score = inverse(bidding_value);
    scores[position] = score;
  }
  Score partial_sum = 0;
  Score empirical_total = 15;
  unsigned n_defined = 0;
  for (const auto &score : scores) if (score != minimumScore) partial_sum += score, ++n_defined;
  if (n_defined != playerCount) {
    Score fallback_score = (empirical_total - partial_sum) / (playerCount - n_defined);
    for (auto &score : scores) {
      if (score == minimumScore) {
        score = fallback_score;
      }
    }
  }

  Count size = 1, cosize = 0;
  Level length = 1;
  for (auto &m : dominant_level) {
    size = 1; cosize = 0; {
      length = 1;
      m[{length, size, cosize}] = redJokerLevel;
      for (length = 5; length != 12; ++length) {
        m[{length, size, cosize}] = 11;
      }
    }
    size = 2; cosize = 0; {
      length = 1;
      m[{length, size, cosize}] = 12;
      for (length = 3; length != 10; ++length) {
        m[{length, size, cosize}] = 11;
      }
    }
    size = 3; {
      for (cosize = 0; cosize != 3; ++cosize) {
        for (length = 1; length != (20 / (size + cosize)) + 1; ++length) {
          m[{length, size, cosize}] = length == 1 ? 12 : 11;
        }
      }
    }
    size = 4; {
      for (cosize = 0; cosize != 3; ++cosize) {
        for (length = 1; length != (20 / (size + cosize * 2)) + 1; ++length) {
          m[{length, size, cosize}] = length == 1 ? 12 : 11;
        }
      }
    }
  }
  for (unsigned i = 0; i != playing_history.size(); ++i) {
    Group group = playing_history[i];
    Hand hand{Counter(group)};
    unsigned position_in_history = i % 3;
    unsigned position = history_layout[position_in_history];
    if (!last_hand.is_pass() && hand.is_pass()) {
      dominant_level[position][{hand.length, hand.size, hand.cosize}] = hand.level;
    } else if (!hand.is_pass()) {
      last_hand = hand;
      last_player = position;
    }
  }
  // supportive or active ?
  if (pos != landlord) {
    vector<unsigned> all = {0, 1, 2};
    unsigned teammate = *find_if(all.begin(), all.end(), [](unsigned k){ return k != pos && k != landlord; });
    if (scores[pos] <= scores[teammate] - delta && last_hand.level >= dominant_level[teammate][{last_hand.length, last_hand.size, last_hand.cosize}]) supportive = true;
  }
}
