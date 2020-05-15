cmake_minimum_required(VERSION 3.16)
project(SocketLib2)

set(CMAKE_CXX_STANDARD 14)

add_library(SocketLib2 SHARED socketlib2.h socketlib2.cpp socketlib2_accept.cpp socketlib2_send.cpp socketlib2_receive.cpp socketlib2_manage_resources.cpp)

target_precompile_headers(SocketLib2
        PRIVATE <iostream>
        PRIVATE <future>
        PRIVATE <list>
        PRIVATE <thread>
        )

add_executable(test test.cpp)

target_link_libraries(SocketLib2 ws2_32)
target_link_libraries(test SocketLib2)
target_link_libraries(test ws2_32)

add_custom_command(TARGET SocketLib2 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SocketLib2> E:\\\\dynamicprogrammingsolutions\\mt4600_experts\\MQL4\\Libraries\\
        )