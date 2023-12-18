#ifndef __PIPE_H__

    #define PIPE_ERROR_CODE          -1
    #define PROCESS_ERROR_CODE       -1
    #define NUM_OF_PIPE_DESCRIPTORS   2
    #define CHILD_PROCESS             0

    extern int *create_pipe();
    extern pid_t fork_process();
    
    extern void server_write_pipe(int* descriptors, pid_t child, int buffer_size, int *buffer);
    extern void server_read_pipe( int* descriptors, pid_t child, int buffer_size, int *buffer);
    extern void client_read_pipe( int* descriptors, pid_t child, int buffer_size, int *buffer);
    extern void client_write_pipe(int* descriptors, pid_t child, int buffer_size, int *buffer);
#endif // __PIPE_H__