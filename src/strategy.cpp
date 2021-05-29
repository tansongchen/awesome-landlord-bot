#include "strategy.h"

#include <algorithm>

Evaluator evaluator;
vector<AttackingSelector> attacking_sequence;
vector<DefendingSelector> defending_sequence;

void update(Value *best_value, Hand *best_hand, const Hand &hand,
            Counter *counter) {
  Value value = evaluate(counter) + evaluator(hand);
  if (value > *best_value) {
    *best_value = value;
    *best_hand = Hand(hand);
  }
}

unsigned suggest(Counter *counter) {
  Value value = evaluate(counter);
  return value >= 20 ? 3 : value >= 15 ? 2 : value >= 10 ? 1 : 0;
}

Value evaluate(Counter *counter) {
  Hand hand = attack(counter);
  for (Level i = 0; i != hand.length; ++i)
    (*counter)[hand.level - i] -= hand.size;
  if (hand.cosize)
    for (const Level &l : hand.attached) (*counter)[l] -= hand.cosize;
  Value value = all_of(counter->begin(), counter->end(),
                       [](Count count) { return count == 0; })
                    ? 0
                    : evaluate(counter);
  for (Level i = 0; i != hand.length; ++i)
    (*counter)[hand.level - i] += hand.size;
  if (hand.cosize)
    for (const Level &l : hand.attached) (*counter)[l] += hand.cosize;
  return evaluator(hand) + value;
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
