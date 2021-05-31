include(FetchContent)

FetchContent_Declare(
  jsoncpp
  URL http://botzone.org/static/JsonCPP_modified.zip
)
FetchContent_MakeAvailable(jsoncpp)

add_library(libcard base.cpp strategy.cpp environment.cpp)
target_include_directories(libcard PRIVATE ${jsoncpp_SOURCE_DIR})

add_subdirectory(strategy)

add_executable(main main.cpp)
target_link_libraries(main libcard libstrategy)

add_executable(sample sample.cpp)
target_link_libraries(sample libcard libstrategy)
