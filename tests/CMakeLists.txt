find_package(Catch2 3 REQUIRED)

IF (${Catch2_FOUND})
    MESSAGE(STATUS "Found Catch2")
ELSE (${Catch2_FOUND})
    MESSAGE(STATUS "Cound not find Catch2. Check to make sure its installed on your system!")
ENDIF (${Catch2_FOUND})

add_executable(test_mongodb test_mongodb.cpp)

target_compile_features(test_mongodb PRIVATE cxx_std_17)

target_include_directories(test_mongodb
    PRIVATE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

# How we link with Catch2!
target_link_libraries(test_mongodb
  PRIVATE
    Catch2::Catch2WithMain
    mongodb
    mongo::mongocxx_shared
)
