cmake_minimum_required(VERSION 2.8)
project(pingpong)

execute_process(
    COMMAND mkdir -p bin
)

add_executable(./bin/server ./src/server.c)
add_executable(./bin/client ./src/client.c)
