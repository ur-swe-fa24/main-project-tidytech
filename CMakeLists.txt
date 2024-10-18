cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
message("Building this awesome practicum!")
project(adapter LANGUAGES CXX)

# include the basics
include(dependencies/get_cpm.cmake)
include(dependencies/dependencies.cmake)

# add library for each of the adapters
add_library(database
    src/database.cpp
)

add_library(fleet_manager
    src/fleet_manager.cpp
)

add_library(simulator
    src/simulator.cpp
)


# include the headers for both adapters
target_include_directories(database
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_include_directories(fleet_manager
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_include_directories(simulator
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

# creates app subdirectory
add_subdirectory(app)