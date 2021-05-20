#include "../strategy.h"

Hand one_shot_selector(Counter *counter) {
  Hand hand(*counter);
  bool flag = true;
  for (const Level &l : allLevels) {
    if (l <= hand.level && l > hand.level - hand.length && (*counter)[l] != hand.size) return not_found;
    else if (hand.attached.find(l) != hand.attached.end() && (*counter)[l] != hand.cosize) return not_found;
    else if ((*counter)[l] != 0) return not_found;
  }
  return hand;
}
