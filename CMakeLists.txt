cmake_minimum_required(VERSION 3.16)
project(Advent_of_Code_2020 C)

set(CMAKE_C_STANDARD 99)

add_executable(Advent_of_Code_2020 main.c
        challenges/day1.c challenges/day1.h
        challenges/day2.c challenges/day2.h
        challenges/day3.c challenges/day3.h
        challenges/day4.c challenges/day4.h
        challenges/day5.c challenges/day5.h
        challenges/day6.c challenges/day6.h
        challenges/day7.c challenges/day7.h
        challenges/day8.c challenges/day8.h
        challenges/day9.c challenges/day9.h)