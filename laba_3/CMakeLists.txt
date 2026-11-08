cmake_minimum_required(VERSION 3.20)
project(lab_03 CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(lab_03_tests
    test/test_point2d.cpp
    test/test_pointset.cpp
    src/Point2D.cpp
    src/PointSet.cpp
)
target_include_directories(lab_03_tests PRIVATE src)
target_link_libraries(lab_03_tests GTest::gtest_main)

add_executable(lab_03_demo
    src/main.cpp
    src/Point2D.cpp
    src/PointSet.cpp
)
target_include_directories(lab_03_demo PRIVATE src)

if(MSVC)
    target_compile_options(lab_03_tests PRIVATE /W4)
    target_compile_options(lab_03_demo PRIVATE /W4)
else()
    target_compile_options(lab_03_tests PRIVATE -Wall -Wextra)
    target_compile_options(lab_03_demo PRIVATE -Wall -Wextra)
endif()

include(GoogleTest)
gtest_discover_tests(lab_03_tests)