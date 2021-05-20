cmake_minimum_required(VERSION 3.20)
project(AwesomeLandlordBot)
add_executable(test src/base.cpp src/strategy.cpp src/environment.cpp test/base.cpp test/main.cpp)
add_executable(main src/base.cpp src/strategy.cpp src/environment.cpp src/main.cpp)
