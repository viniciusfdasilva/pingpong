cmake_minimum_required(VERSION 2.8)
project(pingpong)

execute_process(
    COMMAND mkdir -p bin
)

add_executable(./bin/server ./include/socket.h ./include/utils.h ./src/socket.c ./src/utils.c ./src/main_server.c)
add_executable(./bin/client ./include/socket.h ./include/utils.h ./src/socket.c ./src/utils.c ./src/main_client.c)
