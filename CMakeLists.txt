cmake_minimum_required(VERSION 2.8)
project(pingpong)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

execute_process(
    COMMAND mkdir -p bin
    COMMAND mkdir -p /usr/include/pingpong/
)

include_directories(${INCLUDE_DIR})

set(INCLUDE_FILES
    ${INCLUDE_DIR}/socket
    ${INCLUDE_DIR}/pipe
    ${INCLUDE_DIR}/utils
)

set(SOURCE_FILES
    ${SRC_DIR}/socket.c
    ${SRC_DIR}/pipe.c
    ${SRC_DIR}/utils.c
)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native")

add_executable(./bin/server ${SOURCE_FILES} ./src/main_server.c)
add_executable(./bin/client ${SOURCE_FILES} ./src/main_client.c)

target_include_directories(./bin/server PUBLIC ${INCLUDE_DIR})
target_include_directories(./bin/client PUBLIC ${INCLUDE_DIR})