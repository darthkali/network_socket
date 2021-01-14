cmake_minimum_required(VERSION 3.17)
project(network_socket C)

set(CMAKE_C_STANDARD 11)

add_executable(network_socket
        windows/main.c)
