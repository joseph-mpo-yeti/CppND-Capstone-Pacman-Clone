cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(Pacman)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

file(GLOB project_SRCS src/*.cpp)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

include_directories(include)

add_executable(pacman ${project_SRCS})

target_link_libraries(pacman pthread sfml-graphics sfml-audio)