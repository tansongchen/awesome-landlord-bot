#ifndef STRATEGY_H
#define STRATEGY_H

#include <array>
#include <set>
#include <vector>

#include "base.h"

using namespace std;

// Value of a Hand or a Counter
using Value = short;

using Score = short;
constexpr Score minimumScore = -32768;

struct Pair {
  Value value;
  unsigned short round;
  Pair(Value value = 0, unsigned short round = 0);
  Score score() const;
  bool operator==(const Pair &rhs) const;
};

// Interface for evaluating a hand : Hand -> Value
using Evaluator = Value (*)(const Hand &);
// Interface for actively playing cards : Counter -> Hand
using AttackingSelector = Hand (*)(Counter *);
// Interface for passively playing cards : Counter -> Hand -> Hand
using DefendingSelector = Hand (*)(Counter *, const Hand &);

// Global pointer pointing to the actual Evaluator used
extern Evaluator evaluator;
// Global pointers pointing to the actual AttackingSelectors used
extern vector<AttackingSelector> attacking_sequence;
// Global pointers pointing to the actual DefendingSelectors used
extern vector<DefendingSelector> defending_sequence;

/* Core workflow part */

// Evaluate a Counter representing the cards that a player has
Pair evaluate(Counter *counter);
// Actively play cards
Hand attack(Counter *counter);
// Passively play cards
Hand defend(Counter *counter, const Hand &last_hand);

// If the value of the `counter` is better than the `best_value`, update
// `best_hand` with `hand`
void update(Pair *best_pair, Hand *best_hand, const Hand &hand,
            Counter *counter);

Value simple_evaluator(const Hand &hand);
// Attack
Hand one_shot_selector(Counter *counter);
Hand attacking_absolute_dominant_selector(Counter *counter);
Hand selfish_1_selector(Counter *counter);
Hand selfish_2_selector(Counter *counter);
Hand minimal_selector(Counter *counter);
Hand attacking_bomb_rocket_selector(Counter *counter);
Hand solo_selector(Counter *counter);
// Defend
Hand defending_bomb_rocket_selector(Counter *counter, const Hand &last_hand);
Hand pass_selector(Counter *counter, const Hand &last_hand);
Hand same_category_selector(Counter *counter, const Hand &last_hand);
Hand defending_absolute_dominant_selector(Counter *counter,
                                          const Hand &last_hand);
#endif
