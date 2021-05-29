#include "../strategy.h"

#include <algorithm>

Hand attacking_bomb_rocket_selector(Counter *counter) {
  Level level = find_if(counter->begin(), counter->end(), [](Count count){ return count == 4; }) - counter->begin();
  if (level == maximumLevel) {
    if ((*counter)[blackJokerLevel] && (*counter)[redJokerLevel]) return rocket;
    return not_found;
  }
  return Hand(level, 1, (*counter)[level]);
}
