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

include_directories(include)

add_executable(./bin/server ${SOURCE_FILES} ./src/main_server.c)
add_executable(./bin/client ${SOURCE_FILES} ./src/main_client.c)
