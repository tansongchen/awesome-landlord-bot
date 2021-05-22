add_library(
  libstrategy
  # Evaluator
  simple_evaluator.cpp
  # AttackingSelector
  solo_selector.cpp
  one_shot_selector.cpp
  full_enumeration_selector.cpp
  # DefendingSelector
  pass_selector.cpp
  same_category_selector.cpp
)
target_link_libraries(libstrategy libcard)
