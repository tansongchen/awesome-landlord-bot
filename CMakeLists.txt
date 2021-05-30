cmake_minimum_required(VERSION 3.14)

project(
  AwesomeLandlordBot
  DESCRIPTION "An agent for the game FightTheLandlord implemented with Recursive Hand Cards Partition algorithm and Reinforcement Learning improvements"
  LANGUAGES CXX
  VERSION 0.0.1
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(CTest)

add_subdirectory(src)
add_subdirectory(test)
include(FetchContent)

FetchContent_Declare(
  Amalgamate
  GIT_REPOSITORY https://github.com.cnpmjs.org/vinniefalco/Amalgamate.git
)
FetchContent_MakeAvailable(Amalgamate)

add_executable(amalgamate ${amalgamate_SOURCE_DIR}/Amalgamate.cpp ${amalgamate_SOURCE_DIR}/juce_core_amalgam.cpp)
target_link_libraries(amalgamate pthread dl)
