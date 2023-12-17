#ifndef __UTILS_H__

    #include <time.h>
    #include <stdio.h>
    #include <signal.h>
    #include <stdlib.h>
    #include <sys/un.h>
    #include <string.h>
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <netinet/in.h>

    #define SYSTEM_EXIT_SUCCESS 0        // Operating System program success response
    #define SYSTEM_EXIT_FAILURE 1        // Operating System program error response
    #define TRUE               1
    #define FALSE              0

    extern void panic(char* message);
    extern int* get_init_buffer(int buffer_size);

#endif // __UTILS_H__