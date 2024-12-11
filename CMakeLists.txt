cmake_minimum_required(VERSION 3.20)
project(mini-rts-server)

include_directories(src)
configure_file(config.txt config.txt)

add_executable(mini-rts-server)
add_subdirectory(src)