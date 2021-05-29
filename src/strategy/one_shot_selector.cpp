#include "../strategy.h"

Hand one_shot_selector(Counter *counter) {
  Hand hand(*counter);
  bool valid;
  switch (hand.size) {
    case 1:
      valid = hand == rocket ||
              hand.cosize == 0 && (hand.length == 1 || hand.length >= 5);
      break;
    case 2:
      valid = hand.cosize == 0 && hand.length == 1 || hand.length >= 3;
      break;
    case 3:
      valid = hand.cosize <= 2 && hand.attached.size() == hand.length;
      break;
    case 4:
      valid = hand.cosize <= 2 && hand.attached.size() == hand.length * 2;
      break;
    default: throw runtime_error("Attack with invalid Counter!");
  }
  if (!valid) return not_found;
  Counter recounter = hand.get_counter();
  return *counter == recounter ? hand : not_found;
}
