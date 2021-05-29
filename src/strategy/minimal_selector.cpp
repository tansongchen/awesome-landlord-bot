#include "../strategy.h"

#include <algorithm>

Hand minimal_selector(Counter *counter) {
  Level level = find_if(counter->begin(), counter->end(), [](Count count){ return count == 1 || count == 2; }) - counter->begin();
  return level == maximumLevel ? not_found : Hand(level, 1, (*counter)[level]);
}
