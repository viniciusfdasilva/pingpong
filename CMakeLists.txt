cmake_minimum_required(VERSION 2.8)
project(pingpong)

execute_process(
    COMMAND mkdir -p bin
)

set(SOURCE_FILES
    src/socket.c
    src/pipe.c
    src/utils.c
)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native")

include_directories(include)

add_executable(./bin/server ${SOURCE_FILES} ./src/main_server.c)
add_executable(./bin/client ${SOURCE_FILES} ./src/main_client.c)
