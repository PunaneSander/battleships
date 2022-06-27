cmake_minimum_required(VERSION 3.20)
project(Projekt C)

set(CMAKE_C_STANDARD 99)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

add_executable(Projekt main.c interpreter.c logic.c enemy.c enemy.h display.c battle.c battle.h planning.c planning.h)
