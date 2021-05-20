// 叫分斗地主（FightThelandlord2）主程序：神算子
// 强化学习策略
// 作者：谭淞宸 刘上 张可尔
// 游戏信息：http://www.botzone.org/games#FightThelandlord2

#include "strategy.h"
#include "environment.h"
#include "strategy/solo_selector.cpp"
#include "strategy/pass_selector.cpp"
#include "strategy/simple_evaluator.cpp"

int main() {
  read();
  evaluator = simple_evaluator;
  attacking_sequence = {solo_selector};
  defending_sequence = {pass_selector};
  Counter counter(myCards);
  if (stage == Stage::BIDDING) {
    int suggestion = suggest(&counter);
    int bid_value = bidInput.empty() || suggestion > bidInput.back() ? bid_value : 0;
    bid(bid_value);
  } else if (stage == Stage::PLAYING) {
    Counter last_hand_counter(lastValidCombo);
    Hand last_hand(last_hand_counter);
    Hand hand = last_hand.size ? defend(&counter, last_hand) : attack(&counter);
    Counter counter(hand.get_counter());
    Group group(counter.get_group(myCards));
    play(group);
  }
}
