cmake_minimum_required(VERSION 3.15)

project(hello-world CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(mongocxx 4.0 REQUIRED)

add_executable(hello-world main.cpp)

target_link_libraries(hello-world PRIVATE mongo::mongocxx_shared)
