cmake_minimum_required(VERSION 3.10)
project(MaxCpuFrequency LANGUAGES C)

add_executable(victim victim.c)
add_executable(attacker attacker.c)
