cmake_minimum_required(VERSION 3.10)
project(EmployeeManagement)

set(CMAKE_CXX_STANDARD 11)

add_executable(creator creator.cpp)
add_executable(reporter reporter.cpp)
add_executable(main main.cpp)
