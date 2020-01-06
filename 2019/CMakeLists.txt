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

add_executable(day10 src/day10.cpp)

add_executable(day11 src/day11.cpp)
target_link_libraries(day11 intcode)

add_executable(day12 src/day12.cpp)

add_executable(day13 src/day13.cpp)
target_link_libraries(day13 intcode)

add_executable(day14 src/day14.cpp)

add_executable(day15 src/day15.cpp)
target_link_libraries(day15 intcode)

add_executable(day16 src/day16.cpp)

add_executable(day17 src/day17.cpp)
target_link_libraries(day17 intcode)

add_executable(day18 src/day18.cpp)

add_executable(day19 src/day19.cpp)
target_link_libraries(day19 intcode)

add_executable(day20 src/day20.cpp)

add_executable(day21 src/day21.cpp)
target_link_libraries(day21 intcode)
