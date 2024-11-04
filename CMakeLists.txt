cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/libs/mongo-cxx-driver")
project(adapter LANGUAGES CXX)

# include the basics
include(dependencies/get_cpm.cmake)
include(dependencies/dependencies.cmake)

# add library for each of the adapters
add_library(database
    src/database.cpp
    src/robot_adapter.cpp
    src/floor_adapter.cpp
)

add_library(mongodb
    src/db_manager.cpp
    src/robot_adapter.cpp
    src/task_adapter.cpp
    src/floor_adapter.cpp
)

add_library(fleet_manager
    src/fleet_manager.cpp
)

add_library(simulator
    src/simulator.cpp
    src/robot.cpp
    src/floor.cpp
    src/floorplan.cpp
)

add_library(user_interface
    src/user_interface.cpp
    src/add_robot_window.cpp
    src/add_floor_window.cpp
)

add_library(pubsub INTERFACE)

add_library(types INTERFACE)

find_package(wxWidgets REQUIRED gl core base OPTIONAL_COMPONENTS net)
include(${wxWidgets_USE_FILE})

target_include_directories(database
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE
        mongodb
        mongo::mongocxx_shared
)

target_include_directories(mongodb
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_include_directories(simulator
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        pubsub
        types
)

target_include_directories(fleet_manager
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        pubsub
        types
        database
    PRIVATE
        ${wxWidgets_INCLUDE_DIRS}
        mongodb
        mongo::mongocxx_shared
)

target_include_directories(user_interface
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE
        ${wxWidgets_INCLUDE_DIRS}
        mongodb
        mongo::mongocxx_shared
)

target_include_directories(database
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE
        mongodb
        mongo::mongocxx_shared
)

# link spdlog to adapters
target_link_libraries(simulator
    spdlog::spdlog
    types
)

target_link_libraries(user_interface
    PRIVATE
        ${wxWidgets_LIBRARIES}
        simulator
        types
        database
        mongo::mongocxx_shared
)

target_link_libraries(fleet_manager
    PRIVATE
        ${wxWidgets_LIBRARIES}
        simulator
        types
        database
        mongo::mongocxx_shared
)

target_link_libraries(database
    PRIVATE
        types
        mongo::mongocxx_shared
        
)

find_package(mongocxx REQUIRED)
if (mongocxx_FOUND)
  message(STATUS "mongocxx found!")
endif()
find_package(bsoncxx REQUIRED)
if (bsoncxx_FOUND)
  message(STATUS "bsoncxx found!")
endif()

target_link_libraries(mongodb
    mongo::mongocxx_shared
)

# creates app subdirectory
add_subdirectory(app)