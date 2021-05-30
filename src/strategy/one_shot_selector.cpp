#include "../strategy.h"

Hand one_shot_selector(Counter *counter) {
  Hand hand(*counter);
  if (!hand.is_valid()) return not_found;
  Counter recounter = hand.get_counter();
  return *counter == recounter ? hand : not_found;
}
