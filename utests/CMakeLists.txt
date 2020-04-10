# Description : Definition of all source and header files of one package.
#               To add this package to a project, include this file in the corresponding CMakeLists.txt file.
#      Author :
#     Release :
#   Copyright : 
#    Revision : 
# ------------------------------------------------------------------------------------------------
#     History : 
# ================================================================================================
cmake_minimum_required (VERSION 3.7)
project(TestCypherChat)

# Include diretories
include_directories(/usr/include)
link_directories(/usr/lib)

# Set the compiler options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g -O0 -Wall")
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

# Create OBJECT_DIR variable
set(OBJECT_DIR ${CMAKE_BINARY_DIR}/CMakeFiles/TestCypherChat.dir/src)
message("-- Object files will be output to: ${OBJECT_DIR}")

# Find required packages
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Set the sources
set(sources
                src/test_main.cpp
                src/test_cypher_lib.cpp
                ../src/cypher_lib.c
                )
# Set Include directories
include_directories(
                src/
                )

# Create the executable
add_executable(TestCypherChat ${sources})

# Testing
enable_testing()

add_test(output_test ${CMAKE_CURRENT_BINARY_DIR}/TestCypherChat)

# link Boost libraries to the new target
target_link_libraries(TestCypherChat gtest)
target_link_libraries(TestCypherChat gmock)
target_link_libraries(TestCypherChat pthread)