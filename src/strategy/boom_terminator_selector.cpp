#include <algorithm>

#include "../strategy.h"

Hand boom_terminator_selector(Counter *counter, const Hand &last_hand) {
  Hand mycards(*counter);
  Counter recounter = mycards.get_counter();
  bool final_round = *counter == recounter;
  bool boom = (mycards.size == 4 && mycards.length == 1 && mycards.cosize == 0);
  bool rocket = (mycards.level == redJokerLevel && mycards.length == 2);
  if (final_round && (boom || rocket)) return mycards;
  return not_found;
}