#include <stdlib.h>
#include <unistd.h>

#include "../include/pipe.h"
#include "../include/utils.h"


int *create_pipe()
{
    int *pipe_descriptors = (int*)malloc(NUM_OF_PIPE_DESCRIPTORS*sizeof(int));

    if(pipe(pipe_descriptors) == PIPE_ERROR_CODE)
    {
        panic("Pipe creation failed");
    }

    return pipe_descriptors;
}

pid_t fork_process()
{
    pid_t child_pid;

    if((child_pid == fork()) == PROCESS_ERROR_CODE)
    {
        panic("Fork failed");
    }

    return child_pid;
}

float pingpong_pipe(int* descriptors, pid_t child, int buffer_size, int *buffer)
{

    int *pipe_client_to_server = create_pipe();
    int *pipe_server_to_client = create_pipe();

    if (child == CHILD_PROCESS)
    {
        close(pipe_client_to_server[1]);
        close(pipe_server_to_client[0]);

        ssize_t bytes_read;
        clock_t start_time = clock();
        bytes_read = read(pipe_client_to_server[0], buffer, sizeof(buffer));
       
        write(pipe_server_to_client[1], buffer, bytes_read);
        clock_t end_time = clock();

        close(pipe_client_to_server[0]);
        close(pipe_server_to_client[1]);

        return ((float)(end_time - start_time) / (CLOCKS_PER_SEC/1000));
    }else
    {
        close(pipe_client_to_server[0]);
        close(pipe_server_to_client[1]);

        clock_t start_time = clock();
        write(pipe_client_to_server[1], buffer, sizeof(buffer));
        
        ssize_t bytes_read = read(pipe_server_to_client[0], buffer, sizeof(buffer));
        clock_t end_time = clock();


        close(pipe_client_to_server[1]);
        close(pipe_server_to_client[0]);

        return ((float)(end_time - start_time) / (CLOCKS_PER_SEC/1000));
    }
}