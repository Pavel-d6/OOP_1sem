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