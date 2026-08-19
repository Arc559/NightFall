cmake_minimum_required(VERSION 3.16)
project(NightFall LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_executable(nightfall
    src/main.cpp
    src/nightfall.cpp
)

target_include_directories(nightfall PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(nightfall_tests
    tests/nightfall_gameplay_test.cpp
    src/nightfall.cpp
)

target_include_directories(nightfall_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_test(NAME nightfall_gameplay_test COMMAND nightfall_tests)
