cmake_minimum_required(VERSION 3.16)
project(test C)

set(CMAKE_C_STANDARD 11)

find_package(MPI REQUIRED)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_executable(example1 example1.c)
target_link_libraries(example1 ${MPI_C_LIBRARIES})

add_executable(example2 example2.c)
target_link_libraries(example2 ${MPI_C_LIBRARIES})

add_executable(example3 example3.c)
target_link_libraries(example3 ${MPI_C_LIBRARIES})

add_executable(example4 example4.c)
target_link_libraries(example4 ${MPI_C_LIBRARIES})

add_executable(lab1 lab1.c)
target_link_libraries(lab1 ${MPI_C_LIBRARIES})

add_executable(lab1_generator lab1_generator.c)


