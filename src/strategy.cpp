#include "strategy.h"

#include <algorithm>

Evaluator evaluator;
vector<AttackingSelector> attacking_sequence;
vector<DefendingSelector> defending_sequence;

Pair::Pair(Value _value, unsigned short _round): value(_value), round(_round) {}

Score Pair::score() const {
  return value - 7 * round;
}

bool Pair::operator==(const Pair &rhs) const {
  return value == rhs.value && round == rhs.round;
}

void update(Score *best_score, Hand *best_hand, const Hand &hand,
            Counter *counter) {
  Pair pair = evaluate(counter);
  if (pair.score() > *best_score) {
    *best_score = pair.score();
    *best_hand = Hand(hand);
  }
}

unsigned suggest(Counter *counter) {
  Score score = evaluate(counter).score();
  return score >= 20 ? 3 : score >= 15 ? 2 : score >= 10 ? 1 : 0;
}

Pair evaluate(Counter *counter) {
  if (*counter == empty) return Pair();
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

Hand defend(Counter *counter, const Hand &last_hand) {
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
