# creates the .exe to run
add_executable(demo1
    demo1.cpp
)

target_include_directories(demo1
    PRIVATE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_link_libraries(demo1
    PRIVATE
        fleet_manager
        simulator
        pubsub
        user_interface
        mongodb
        mongo::mongocxx_shared
)

target_compile_features(demo1 PRIVATE cxx_std_17)