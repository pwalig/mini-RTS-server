target_sources(mini-rts-server PRIVATE main.cpp)

add_subdirectory(msg)
add_subdirectory(net)
add_subdirectory(rts)