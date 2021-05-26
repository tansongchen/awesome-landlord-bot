# AwesomeLandlordBot

An agent for the game "FightTheLandlord" (a.k.a. Doudizhu) implemented with Recursive Hand Cards Partition algorithm and Reinforcement Learning improvements. This is written by Songchen Tan, Shang Liu and Keer Zhang to participate in the 13th Peking University Game Contest. See an introduction of this game at http://www.botzone.org/games#FightThelandlord2.

## Getting Started

1. Clone this repository;
2. Make sure you have `cmake >= 3.20` installed;
3. Build with `mkdir build && cd build && cmake .. && cmake --build .`, and depending on your compiler choice, you may need option like `cmake .. -D CMAKE_CXX_COMPILER=g++`;
4. Run tests with `ctest`;
5. Run main program with `./main`.

## Code Structure

### `src/`

- `base.h` and `base.cpp` contains definitions of basic data structures like `Card`, `Counter` and `Hand`;
- `strategy.h` and `strategy.cpp` contains the core workflow of the RHCP (Recursive Hand Card Partitioning) algorithm, in terms of functions `attack`, `defend` and `evaluate`;
  - To make developing and testing convenient, the `attack` (actively playing cards with no prior hands) as well as the `defend` (passively playing cards with prior hands) are divided in to several stages;
  - In each stage, a selector tries to find an optimal hand that is of some particular category; if it cannot find such a hand, it raises `not_found` and the algorithm moves to the next stage;
  - For example, in the `defend` function, one may first try to beat the previous hand with a bigger hand of the same category; if not found, then try to beat the previous hand with Bomb or Rocker; if not found, then pass.
  - The selectors share the same interface, namely `AttackingSelector` and `DefendingSelector`, and several possible selectors are implemented in the `strategy` subfolder.
- `environment.h` and `environment.cpp` contains the game playing environment, and they are mostly copied from BotZone; as a result, they are not advised to be modified;
- `main.cpp` specifies the strategy with global pointers and launch the game.

### `test/`

- `base.cpp` contains basic tests for definitions in `src/base.h`;
- `main.cpp` serves as the entry point for `Catch2` and launch all tests.
