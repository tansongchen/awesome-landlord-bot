add_library(
  libstrategy
  # Evaluator
  simple_evaluator.cpp
  # AttackingSelector
  one_shot_selector.cpp
  attacking_absolute_dominant_selector.cpp
  selfish_1_selector.cpp
  selfish_2_selector.cpp
  minimal_selector.cpp
  attacking_bomb_rocket_selector.cpp
  solo_selector.cpp
  # DefendingSelector
  defending_absolute_dominant_selector.cpp
  same_category_selector.cpp
  defending_bomb_rocket_selector.cpp
  pass_selector.cpp
)
target_link_libraries(libstrategy libcard)
