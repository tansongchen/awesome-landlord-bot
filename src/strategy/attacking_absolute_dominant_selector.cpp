#include "../strategy.h"
#include "../environment.h"

#include <algorithm>

Hand attacking_absolute_dominant_selector(Counter *counter) {
  if ((*counter)[blackJokerLevel] && (*counter)[redJokerLevel]) {
    (*counter)[blackJokerLevel] = (*counter)[redJokerLevel] = 0;
    const Hand hand = one_shot_selector(counter);
    (*counter)[blackJokerLevel] = (*counter)[redJokerLevel] = 1;
    if (hand == not_found) {
      return not_found;
    } else {
      return rocket;
    }
  } else {
    return not_found;
  }
}
