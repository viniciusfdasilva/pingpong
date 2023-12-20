cmake_minimum_required(VERSION 3.10)
project(pingpong)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

execute_process(
    COMMAND mkdir -p bin
    COMMAND mkdir -p /usr/include/pingpong/
)

set(INCLUDE_FILES
    ${INCLUDE_DIR}/pipe.h
    ${INCLUDE_DIR}/socket.h
    ${INCLUDE_DIR}/utils.h
)

set(SOURCE_FILES
    ${SRC_DIR}/socket.c
    ${SRC_DIR}/pipe.c
    ${SRC_DIR}/utils.c
)

set(INSTALL_DIR /usr/include/pingpong/)

install(FILES ${INCLUDE_FILES} DESTINATION ${INSTALL_DIR})

include_directories(${CMAKE_SOURCE_DIR}/include)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native")

add_executable(server ${SOURCE_FILES} ./src/main_server.c)
add_executable(client ${SOURCE_FILES} ./src/main_client.c)
