cmake_minimum_required(VERSION 2.6)

include_directories(/usr/include)
link_directories(/usr/lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(sources
		src/main_tests.c
		../src/cypher_lib.c
		)

# Set Include directories
include_directories(
        src/
		../src/
		)

add_executable(CypherLibTest ${sources})
