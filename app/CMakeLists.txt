# creates the .exe to run
add_executable(demo1
    demo1.cpp
)

# specifies target for libraries
target_include_directories(demo1
    PRIVATE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

# specifies location of the adapter libraries
target_link_libraries(demo1
    PRIVATE
        database
        fleet_manager
        simulator
)

target_compile_features(demo1 PRIVATE cxx_std_17)