cmake_minimum_required(VERSION 3.15)
project(Lab8)

set(CMAKE_CXX_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(Lab8 main.cpp Figure.hpp Factory.hpp pub_sub.hpp)
target_link_libraries(Lab8 PRIVATE Threads::Threads)