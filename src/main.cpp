// 叫分斗地主（FightThelandlord2）主程序：神算子
// 强化学习策略
// 作者：谭淞宸 刘上 张可尔
// 游戏信息：http://www.botzone.org/games#FightThelandlord2

#include "environment.h"

int main() {
  Environment environment;
  if (environment.stage == Stage::BIDDING) {
    unsigned stake = environment.bid();
    environment.output(stake);
  } else if (environment.stage == Stage::PLAYING) {
    Group group = environment.play();
    environment.output(group);
  }
}
