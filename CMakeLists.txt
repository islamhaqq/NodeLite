cmake_minimum_required(VERSION 3.28)
project(nodelite)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

set(SOURCE_FILES
    main.cpp
)

set(TEST_FILES
    test.cpp
)

add_executable(nodelite ${SOURCE_FILES})
add_executable(nodelite_test ${TEST_FILES})

set_target_properties(nodelite PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)
