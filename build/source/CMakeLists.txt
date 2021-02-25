cmake_minimum_required(VERSION 3.18)

project(pzl VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Wextra -Werror -Wpedantic)

include(CTest)
if (BUILD_TESTING)
  include(GoogleTest)
  find_package(Threads REQUIRED)
  add_executable(pzl-tests test.cpp)
  target_link_libraries(pzl-tests gtest gtest_main Threads::Threads unwind)
  gtest_discover_tests(pzl-tests)
endif()
