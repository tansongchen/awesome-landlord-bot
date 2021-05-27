
#include <algorithm>

#include "../strategy.h"

Hand defend_boom_selector(Counter *counter, const Hand &last_hand) {
  auto length = last_hand.length;
  auto size = last_hand.size;
  auto cosize = last_hand.cosize;

  // if last_hand is bomb, then if there is rocket of mine
  if (length == 1 && size == 4 && cosize == 0) {
    if ((*counter)[blackJokerLevel] && (*counter)[redJokerLevel])
      return Hand(14, 2, 1);
    return not_found;
  }

  // if last_hand is not bomb
  for (const Level &l : allLevels)  // bomb
    if ((*counter)[l] == 4) return Hand(l, 1, 4);
  if ((*counter)[blackJokerLevel] && (*counter)[redJokerLevel])  // rocket
    return Hand(14, 2, 1);
  return not_found;
}
