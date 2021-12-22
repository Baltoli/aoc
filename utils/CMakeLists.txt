add_library(utils
  src/utils.cpp
  src/md5.c)

target_compile_features(utils
  INTERFACE cxx_std_17)

target_include_directories(utils
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(utils
  fmt::fmt)
