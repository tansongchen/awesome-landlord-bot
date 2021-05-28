#include "../strategy.h"

#include <algorithm>

Hand minimal_selector(Counter *counter) {
  Level *pointer = find_if(counter->begin(), counter->end(), [](Count count){ return count == 1 && count == 2; });
  if (pointer == counter->end()) return not_found;
  Level level = pointer - counter->begin();
  return Hand(level, 1, (*counter)[level]);
}
