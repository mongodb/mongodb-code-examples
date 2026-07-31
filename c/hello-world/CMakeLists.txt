cmake_minimum_required(VERSION 3.15)
project(hello-world C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(mongoc 2.0 REQUIRED)

add_executable(hello-world hello-world.c)
target_link_libraries(hello-world PRIVATE mongoc::mongoc)
