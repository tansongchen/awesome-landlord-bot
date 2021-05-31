#include "environment.h"
#include "strategy.h"

int main() {
  read();
  evaluator = simple_evaluator;
  attacking_sequence = {
    one_shot_selector,
    attacking_absolute_dominant_selector,
    selfish_1_selector,
    selfish_2_selector,
    minimal_selector,
    attacking_bomb_rocket_selector
  };
  defending_sequence = {
    // defending_absolute_dominant_selector,
    same_category_selector,
    defending_bomb_rocket_selector,
    pass_selector
  };
  Counter counter(myCards);
  if (stage == Stage::BIDDING) {
    Pair pair = evaluate(&counter);
    Value value = pair.value;
    int suggestion = value >= 15 ? 3 : value >= 10 ? 2 : value >= 5 ? 1 : 0;
    int bid_value =
        bidInput.empty() || suggestion > bidInput.back() ? suggestion : 0;
    bid(bid_value, pair.value, pair.round);
  } else if (stage == Stage::PLAYING) {
    Counter last_hand_counter(lastValidCombo);
    Hand last_hand(last_hand_counter);
    Hand hand =
        last_hand == pass ? attack(&counter) : defend(&counter, last_hand);
    Counter counter(hand.get_counter());
    Group group(counter.get_group(myCards));
    play(group, last_hand, hand);
  }
}
