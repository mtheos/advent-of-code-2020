cmake_minimum_required(VERSION 3.16)
project(Advent_of_Code_2020 C)

set(CMAKE_C_STANDARD 99)

add_executable(Advent_of_Code_2020 main.c
        challenges/day1.c challenges/day1.h challenges/day2.c challenges/day2.h)