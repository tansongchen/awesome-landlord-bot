#include "../strategy.h"

Hand one_shot_selector(Counter *counter) {
  Hand hand(*counter);
  Counter recounter = hand.get_counter();
  return *counter == recounter ? hand : not_found;
}
