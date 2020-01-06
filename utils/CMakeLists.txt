add_library(utils
  src/utils.cpp)

target_compile_features(utils
  INTERFACE cxx_std_17)

message(STATUS ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(utils
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
