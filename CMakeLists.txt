cmake_minimum_required(VERSION 3.20)
project(mini-rts-server)

include_directories(src)

add_executable(mini-rts-server)
add_subdirectory(src)