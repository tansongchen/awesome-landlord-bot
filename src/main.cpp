#include "environment.h"
#include "strategy.h"

int main() {
  read();
  cout << "read successful" << endl;
  evaluator = simple_evaluator;
  attacking_sequence = {solo_selector};
  defending_sequence = {pass_selector};
  cout << "strats load successful" << endl;
  Counter counter(myCards);
  if (stage == Stage::BIDDING) {
    int suggestion = suggest(&counter);
    int bid_value = bidInput.empty() || suggestion > bidInput.back() ? bid_value : 0;
    bid(bid_value);
  } else if (stage == Stage::PLAYING) {
    cout << "play start" << endl;
    cout << lastValidCombo << endl;
    Counter last_hand_counter(lastValidCombo);
    cout << last_hand_counter << endl;
    Hand last_hand(last_hand_counter);
    cout << last_hand << endl;
    Hand hand = last_hand == pass ? attack(&counter) : defend(&counter, last_hand);
    cout << hand << endl;
    Counter counter(hand.get_counter());
    Group group(counter.get_group(myCards));
    play(group);
  }
}
