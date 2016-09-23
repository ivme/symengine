cmake_minimum_required(VERSION 3.4)

project(test_chad)

add_executable(test_rings test_rings.cpp)
target_link_libraries(test_rings symengine)