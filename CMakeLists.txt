#    Filename : CmakeLists.txt
# Description : Definition of all source and header files of one package.
#               To add this package to a project, include this file in the corresponding CMakeLists.txt file.
#      Author : Rafael Cavalcanti
#     Release :
#   Copyright : 
#    Revision : 
# ------------------------------------------------------------------------------------------------
#     History : 
# ================================================================================================
cmake_minimum_required (VERSION 3.7)

project(Chat)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_FLAGS -pthread)

find_package(Threads)

add_executable(Server 
                src/server.c
                src/string.c
)

add_executable(Client 
                src/client.c
                src/string.c
                src/cypher_lib.c
)

include_directories(
                src/
)

target_link_libraries(Server ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (Client ${CMAKE_THREAD_LIBS_INIT})