#include "../strategy.h"

Value simple_evaluator(const Hand &hand) {
  const auto &size = hand.size, &cosize = hand.cosize;
  const auto &level = hand.level, &length = hand.length;
  // Pass
  if (size == 0) return 0;
  // Rocket
  if (level == redJokerLevel && length == 2) return 20;
  // Four
  if (size == 4 && length == 1 && cosize == 0) return level + 7;
  // Four with dual Solo, Four with dual Pair, Four Chain, Four with dual Solo
  // Chain, Four with dual Pair Chain
  if (size == 4) return level / 2;
  // Solo, Pair, Trio, Trio with Solo, Trio with Pair
  if (length == 1) return level - 7;
  // Trio Chain, Trio with Solo Chain, Trio with Pair Chain
  if (size == 3) return (level + 1) / 2;
  // Single Chain, Double Chain
  return level - 7 + 1;
}
