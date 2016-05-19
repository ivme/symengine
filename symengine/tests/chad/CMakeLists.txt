cmake_minimum_required(VERSION 3.4)

project(test_polynomial)

add_executable(test_polynomial test_polynomial.cpp)
target_link_libraries(test_polynomial symengine)
