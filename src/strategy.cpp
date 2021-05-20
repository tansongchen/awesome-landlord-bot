#include "strategy.h"

#include <algorithm>

Evaluator evaluator;
vector<AttackingSelector> attacking_sequence;
vector<DefendingSelector> defending_sequence;

inline void update(Value *best_value, Hand *best_hand, const Hand &hand, Counter *counter) {
  auto value = evaluate(counter);
  if (value > *best_value) {
    *best_value = value;
    *best_hand = hand;
  }
}

unsigned suggest(Counter *counter) {
  Value value = evaluate(counter);
  return value >= 20 ? 3 : value >= 15 ? 2 : value >= 10 ? 1 : 0;
}

Value evaluate(Counter *counter) {
  Hand hand = attack(counter);
  for (Level l = hand.level; l != hand.level - hand.length; --l) (*counter)[l] -= hand.size;
  if (hand.cosize) for (const Level &l : hand.attached) (*counter)[l] -= hand.cosize;
  auto resValue = evaluate(counter);
  for (Level l = hand.level; l != hand.level - hand.length; --l) (*counter)[l] += hand.size;
  if (hand.cosize) for (const Level &l : hand.attached) (*counter)[l] += hand.cosize;
  return evaluator(hand) + resValue;
}

Hand defend(Counter *counter, const Hand &last_hand) {
  for (auto &selector : defending_sequence) {
    const Hand hand = (*selector)(counter, last_hand);
    if (hand == not_found) continue;
    return hand;
  }
  return not_found;
}

Hand attack(Counter *counter) {
  for (auto &selector : attacking_sequence) {
    const Hand hand = (*selector)(counter);
    if (hand == not_found) continue;
    return hand;
  }
  return not_found;
}
