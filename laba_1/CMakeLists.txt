cmake_minimum_required(VERSION 3.16)
project(lab1 CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(str_ops STATIC
    src/str_ops.cpp
)
target_include_directories(str_ops PUBLIC src)


add_executable(lab1 src/main.cpp)
target_link_libraries(lab1 PRIVATE str_ops)

option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()
    add_executable(test_variant tests/test.cpp)
    target_link_libraries(test_variant PRIVATE str_ops GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(test_variant)
endif()