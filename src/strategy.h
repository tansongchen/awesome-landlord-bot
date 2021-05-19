#ifndef STRATEGY_H
#define STRATEGY_H

#include <set>
#include <array>
#include <vector>

#include "base.h"

using namespace std;

using Evaluator = Value (*) (const Hand &);
using AttackingSelector = Hand (*) (Counter *);
using DefendingSelector = Hand (*) (Counter *, const Hand &);

extern Evaluator evaluator;
extern vector<AttackingSelector> attacking_sequence;
extern vector<DefendingSelector> defending_sequence;

Value evaluate(Counter *counter);
unsigned suggest(Counter *counter);
inline void update(Value *best_value, Hand *best_hand, const Hand &hand, Counter *counter);
Hand defend(Counter *counter, const Hand &last_hand);
Hand attack(Counter *counter);

#endif
