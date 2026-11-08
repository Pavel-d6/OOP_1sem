cmake_minimum_required(VERSION 3.16)
project(lab2 CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(poly src/Polynomial.cpp)
target_include_directories(poly PUBLIC src)

add_executable(app src/main.cpp)
target_link_libraries(app poly)

include(FetchContent)
FetchContent_Declare(googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(tests test/tests.cpp)
target_link_libraries(tests poly GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(tests)