cmake_minimum_required(VERSION 3.16)
project(Advent_of_Code_2020 C)

set(CMAKE_C_STANDARD 99)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -g -O3")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")

add_executable(Advent_of_Code_2020 main.c
        challenges/day01.c challenges/day01.h
        challenges/day02.c challenges/day02.h
        challenges/day03.c challenges/day03.h
        challenges/day04.c challenges/day04.h
        challenges/day05.c challenges/day05.h
        challenges/day06.c challenges/day06.h
        challenges/day07.c challenges/day07.h
        challenges/day08.c challenges/day08.h
        challenges/day09.c challenges/day09.h
        challenges/day10.c challenges/day10.h
        challenges/day11.c challenges/day11.h
        challenges/day12.c challenges/day12.h
        challenges/day13.c challenges/day13.h
        challenges/day14.c challenges/day14.h
        challenges/day15.c challenges/day15.h)