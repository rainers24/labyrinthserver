cmake_minimum_required(VERSION 3.15)
project(labyrinth_server C)


set(CMAKE_C_STANDARD 90)
SET(CMAKE_C_FLAGS -pthread)

add_executable(labyrinth_server main.c net.h net.c game.h input.h input.c game.c libs.h helpers.c helpers.h timer.c timer.h map.c map.h)


