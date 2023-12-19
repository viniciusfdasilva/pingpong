#include <stdio.h>
#include <stdlib.h>

#include <pingpong/utils.h>

void panic(char* message)
{
    printf("%s\n",message);
    exit(SYSTEM_EXIT_FAILURE);
}

int* get_init_buffer(int buffer_size)
{
    
    int *buffer = (int*)malloc(buffer_size*sizeof(int*));

    for(int i = 0; i < buffer_size; i++)
    {
        buffer[i] = (int)(i % 255);
    }

    return buffer;
}