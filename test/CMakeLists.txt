include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v2.13.1
)
FetchContent_MakeAvailable(Catch2)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${catch2_SOURCE_DIR}/contrib")
include(Catch)

add_executable(unit_test base.cpp main.cpp)
target_link_libraries(unit_test libcard libstrategy Catch2::Catch2)
catch_discover_tests(unit_test)
