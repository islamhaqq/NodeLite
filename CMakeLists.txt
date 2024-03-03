cmake_minimum_required(VERSION 3.28)
project(nodelite)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories for nodelite core and external libraries
include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

# Source files for the main nodelite executable
set(SOURCE_FILES
        main.cpp
        # Add other source files here
)

# Define the nodelite executable
add_executable(nodelite ${SOURCE_FILES})

# Link with external libraries, if any (e.g., V8, libuv)
# target_link_libraries(nodelite libv8.a libuv.a)

enable_testing()

function(add_nodelite_test test_file)
    get_filename_component(test_name ${test_file} NAME_WE)
    add_test(NAME ${test_name}
            COMMAND nodelite ${PROJECT_SOURCE_DIR}/tests/${test_file})

endfunction(add_nodelite_test)

add_nodelite_test(http_tests.js)
add_nodelite_test(module_loading_tests.js)
add_nodelite_test(async_io_tests.js)
