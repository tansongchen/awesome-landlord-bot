#include <algorithm>

#include "../strategy.h"

Hand defending_absolute_dominant_selector(Counter *counter,
                                          const Hand &last_hand) {
  if (last_hand == rocket) return not_found;

  Hand hand(*counter);
  if (!hand.is_valid()) return not_found;
  Counter recounter = hand.get_counter();
  bool final_round = *counter == recounter;
  bool has_bomb = (hand.size == 4 && hand.length == 1 && hand.cosize == 0);
  bool has_rocket = hand == rocket;
  if (final_round && (has_bomb || has_rocket)) return hand;
  return not_found;
}
