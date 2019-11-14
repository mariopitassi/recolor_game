#version minimale de cmake nécessaire
cmake_minimum_required(VERSION 2.6)

#nom du projet
project (recolor C)

#positionne la variable
set(CMAKE_C_FLAGS "-std=c99 -g -Wall")

#définit le nom du programme ainsi que ses sources
add_executable(recolor_text recolor_text.c)
add_library(game game_io.o game.o)

target_link_libraries(recolor_text m game)