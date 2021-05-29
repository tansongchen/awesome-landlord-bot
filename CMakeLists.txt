cmake_minimum_required(VERSION 3.20)

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
