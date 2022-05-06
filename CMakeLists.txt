cmake_minimum_required(VERSION 3.20)
project(mapping)

set(CMAKE_CXX_STANDARD 14)

include_directories(utils)
add_executable(mapping main.cpp sir/machine.cpp sir/machine.h sir/network.cpp sir/network.h algorithm/hilbert.cpp algorithm/hilbert.h sir/placement.cpp sir/placement.h sir/placementalgorithm.cpp sir/placementalgorithm.h algorithm/randommapping.cpp algorithm/randommapping.h algorithm/zigzag.cpp algorithm/zigzag.h utils/evaluator.cpp utils/evaluator.h algorithm/circle.cpp algorithm/circle.h algorithm/force_directed.cpp algorithm/force_directed.h algorithm/PSO.cpp algorithm/PSO.h algorithm/fd_random_init.cpp algorithm/fd_random_init.h utils/visualize.cpp utils/visualize.h)
include_directories(sir)