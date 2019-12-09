cmake_minimum_required(VERSION 3.10)

project(AOC CXX)

set(CMAKE_CXX_STANDARD 17)

add_library(intcode
  src/intcode.cpp)
target_include_directories(intcode PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

add_executable(day1 src/day1.cpp)

add_executable(day2 src/day2.cpp)
target_link_libraries(day2 intcode)

add_executable(day3 src/day3.cpp)

add_executable(day4 src/day4.cpp)

add_executable(day5 src/day5.cpp)
target_link_libraries(day5 intcode)

add_executable(day6 src/day6.cpp)

add_executable(day7 src/day7.cpp)
target_link_libraries(day7 intcode)

add_executable(day8 src/day8.cpp)

add_executable(day9 src/day9.cpp)
target_link_libraries(day9 intcode)
