cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)

project(sparse_vlg)

find_package(igraph REQUIRED)
#find_package(Boost REQUIRED COMPONENTS system program_options)

add_executable(sparse_vlg main.cpp)
target_link_libraries(sparse_vlg PUBLIC igraph::igraph)