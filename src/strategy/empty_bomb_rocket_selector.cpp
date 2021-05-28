#include "../strategy.h"

#include <algorithm>

Hand empty_bomb_rocket_selector(Counter *counter) {
  Level *it = find_if(counter->begin(), counter->end(), [](Count count){ return count == 4; });
  if (it == counter->end()) {
    if ((*counter)[blackJokerLevel] && (*counter)[redJokerLevel]) return rocket;
    return not_found;
  }
  Level level = it - counter->begin();
  return Hand(level, 1, (*counter)[level]);
}
