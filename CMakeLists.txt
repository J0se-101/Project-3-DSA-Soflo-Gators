cmake_minimum_required(VERSION 3.27)
project(Project_3_DSA_Soflo_Gators)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(Project_3_DSA_Soflo_Gators
        src/TMDB_tv_dataset_v3.csv
        src/TVShow.cpp
        src/TVShow.h
        src/hashmaps.cpp
        src/hashmaps.h
        src/graph.cpp
        src/graph.h
        src/graph.h)
