cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
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
    src/robot.cpp
)

add_library(user_interface
    src/main_frame.cpp
)

add_library(pubsub INTERFACE)

find_package(wxWidgets REQUIRED gl core base OPTIONAL_COMPONENTS net)
include(${wxWidgets_USE_FILE})

target_include_directories(database
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_include_directories(fleet_manager
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        pubsub
)

target_include_directories(simulator
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        pubsub
)

target_include_directories(user_interface
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE
        ${wxWidgets_INCLUDE_DIRS}
)

# link spdlog to simulator
target_link_libraries(simulator
    spdlog::spdlog
)

target_link_libraries(user_interface
    PRIVATE
        ${wxWidgets_LIBRARIES}
)

# creates app subdirectory
add_subdirectory(app)