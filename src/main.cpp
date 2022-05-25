#include "environment.h"
#include "strategy.h"

int main() {
  read();
  evaluator = simple_evaluator;
  attacking_sequence = {
    one_shot_selector,
    attacking_absolute_dominant_selector,
    selfish_selector,
    normal_selector,
    attacking_bomb_rocket_selector
  };
  defending_sequence = {
    same_category_selector,
    defending_bomb_rocket_selector,
    pass_selector
  };
  Counter counter(own);
  if (stage == Stage::BIDDING) {
    Pair pair = evaluate(&counter);
    Value value = pair.value;
    int suggestion = value_to_bidding(value);
    int bid_value =
        bidding_history.empty() || suggestion > bidding_history.back() ? suggestion : 0;
    bid(bid_value, pair.value, pair.round);
  } else if (stage == Stage::PLAYING) {
    bool is_attack = last_hand == pass;
    if (!is_attack && supportive) updater = supportive_updater;
    Hand hand = is_attack ? attack(&counter) : defend(&counter, last_hand);
    Counter counter(hand.get_counter());
    Group group(counter.get_group(own));
    play(group, last_hand, hand);
  }
}
