cmake_minimum_required(VERSION 2.8)

project(CountInversions)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_REQUIRED_FLAGS -std=c++17)

find_package(Catch2 REQUIRED)

add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME} Catch2::Catch2)
