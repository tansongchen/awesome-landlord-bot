#include "../strategy.h"

#include <algorithm>

Hand defending_bomb_rocket_selector(Counter *counter, const Hand &last_hand) {
  Pair best_pair(-10000, 0);
  Hand best_hand(not_found), hand(0, 1, 4, 0);
  auto &level = hand.level, &length = hand.length;
  auto &size = hand.size, &cosize = hand.cosize;

  if (last_hand == rocket) return not_found;

  // if last_hand is not bomb, find bombs
  if (length == 1 && size == 4 && cosize == 0) {
    for (level = 0; level != maximumLevel; ++level) {
      if ((*counter)[level] == size) {
        (*counter)[level] -= size;
        updater(&best_pair, &best_hand, hand, counter);
        (*counter)[level] += size;
      }
    }
  }

  // find rocket
  if ((*counter)[blackJokerLevel] && (*counter)[redJokerLevel]) {
    (*counter)[blackJokerLevel] = (*counter)[blackJokerLevel] = 0;
    updater(&best_pair, &best_hand, rocket, counter);
    (*counter)[blackJokerLevel] = (*counter)[blackJokerLevel] = 1;
  }

  if (best_pair.value >= 0) return best_hand;
  return not_found;
}
