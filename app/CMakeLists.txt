# creates the .exe to run
add_executable(demo1
    demo1.cpp
)

add_executable(dbtest
    db_test.cpp
)

target_include_directories(demo1
    PRIVATE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_include_directories(dbtest
    PRIVATE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_link_libraries(demo1
    PRIVATE
        database
        fleet_manager
        simulator
        pubsub
)

target_link_libraries(dbtest
    PRIVATE
        mongodb
        mongo::mongocxx_shared
)

target_compile_features(dbtest PRIVATE cxx_std_17)
target_compile_features(demo1 PRIVATE cxx_std_17)