#include <stdlib.h>
#include <unistd.h>

#include "../include/pipe.h"
#include "../include/utils.h"


int *create_pipe()
{
    int pipe_descriptors[NUM_OF_PIPE_DESCRIPTORS];

    if(pipe(pipe_descriptors) == PIPE_ERROR_CODE)
    {
        panic("Pipe creation failed");
    }

    return &pipe_descriptors[0];
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

void client_write_pipe(int* descriptors, pid_t child, int buffer_size, int *buffer)
{
    close(descriptors[0]);
    write(descriptors[1], buffer, sizeof(descriptors));
    close(descriptors[1]);
}

void client_read_pipe(int* descriptors, pid_t child, int buffer_size, int *buffer)
{
    if (child == CHILD_PROCESS) {
        close(descriptors[1]);
        
        read(descriptors[0], buffer, sizeof(buffer));
        close(descriptors[0]);
    }else
    {
        server_write_pipe(descriptors, child, buffer_size, buffer);
    }
}


void server_read_pipe(int* descriptors, pid_t child, int buffer_size, int *buffer)
{

    int* received_buffer = (int*)malloc(buffer_size*sizeof(int));

    if (child == CHILD_PROCESS) 
    {
        close(descriptors[1]);

        read(descriptors[0], received_buffer, sizeof(buffer));

        close(descriptors[0]);
        exit(EXIT_SUCCESS);
    }else
    {
        client_write_pipe(descriptors, child, buffer_size, buffer);
    }
}


void server_write_pipe(int* descriptors, pid_t child, int buffer_size, int *buffer)
{
    close(descriptors[0]);
    write(descriptors[1], buffer, sizeof(buffer));
    close(descriptors[1]);
}