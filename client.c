#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define TRUE               1
#define FALSE              0
#define HOST_IP            "127.0.0.1"   // IPV4 loopback address
#define SERVER_PORT        8294         // Server port
#define PORT_USED_CODE     256
#define SOCKET_ERROR_CODE  -1         // Socket create, Connection server, Receive buffer to server code error
#define SYSTEM_EXIT_FAILED  1        // Operating System program error response 
#define SYSTEM_EXIT_SUCCESS 0       // Operating System program success response

#define KB 1024

typedef struct sockaddr_in socket_address;

int init_socket()
{   
    // AF_INET     -> IPV4 Family
    // SOCK_STREAM -> TCP Socket initialization
    // IPPROTO_TCP -> TCP Explicit protocol
    int client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(client_socket == SOCKET_ERROR_CODE)
    {
        perror("Socket create failed!\n");
        exit(SYSTEM_EXIT_FAILED);
    }
}

socket_address config_server_address()
{
    socket_address server_address;

    server_address.sin_family      = AF_INET; // IPV4 ips family
    server_address.sin_port        = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = inet_addr(HOST_IP);

    return server_address;
}


void connect_to_server(int client_socket, socket_address server_address)
{
    struct sockaddr* address = (struct sockaddr*)&server_address;
    
    int connection_response = connect(client_socket,
                                        address, 
                                        sizeof(server_address)
                                    );

    if(connection_response == SOCKET_ERROR_CODE)
    {
        perror("Server connection error\n");
        close(client_socket);
        exit(SYSTEM_EXIT_FAILED);
    }{
        printf("Server connection successfully\n\n");
    }
}

int is_server_down()
{   
    char* full_command = (char*)malloc(1000*sizeof(char));

    char* base_command = "lsof -i 4@";
    char str[5];
    sprintf(str, "%d", SERVER_PORT);

    strcat(full_command, base_command);
    strcat(full_command,HOST_IP);
    strcat(full_command, " -i:");
    strcat(full_command, str);
    strcat(full_command, " >/dev/null 2>&1");

    system(full_command) == PORT_USED_CODE ? FALSE : TRUE;
}


int receive_buffer(int client_socket, int buffer_size)
{
    int bytes_read;

    char received_buffer[buffer_size];

    printf("[CLIENT] - Receiving [PONG]\n");

    while((bytes_read = recv(client_socket, received_buffer, buffer_size, 0)) > 0)
    {

        received_buffer[bytes_read] = '\0';

        printf("VALUE RECEIVED [  %s  ]", received_buffer);

        printf("\n[CLIENT] - Server message received successfully!");

        return 0;
    }
   return 1;
}

void send_buffer(int client_socket, char sended_buffer[], int buffer_size)
{
    
    float size_in_kb = ((float)buffer_size/(float)KB);
    
    if(buffer_size >= KB){
        printf("[CLIENT] - Sending [PING] %dKB\n\n", (buffer_size/KB));
    }else{
        printf("[CLIENT] - Sending [PING] %fKB\n\n", ((float)buffer_size/(float)KB));
    }
    
    send(client_socket, sended_buffer, buffer_size, 0);
    

}

void controlc_handler()
{
    printf("\nProgram finished!\n");
    exit(SYSTEM_EXIT_SUCCESS);
}

void socket_listen(int client_socket, char sended_buffer[], int buffer_size)
{

    if(signal(SIGINT, controlc_handler) == SIG_ERR)
    {
        perror("Signal create error!");
        exit(SYSTEM_EXIT_FAILED);
    }

    while(TRUE){
        send_buffer(client_socket, sended_buffer, buffer_size);
        receive_buffer(client_socket, buffer_size);
    }
}

int main(int argc, char** argv)
{

    int buffer_size = (int)atoi(argv[1]);

    char sended_buffer[buffer_size];
    
    for(int i = 0; i < buffer_size; i++)
    {
        sended_buffer[i] = '0';
    }

    int client_socket  = init_socket();
    
    socket_address server_address;
    server_address = config_server_address();

    printf("\n==================================================================\n");
    printf("                           PING PONG PROGRAM                       \n");
    printf("==================================================================\n\n");

    connect_to_server(client_socket, server_address);

    clock_t start = clock();
    socket_listen(client_socket, sended_buffer, buffer_size);
    clock_t end = clock();

    double elapsed_time_ms = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;

    printf("\nTime: %.2f ms\n\n", elapsed_time_ms);

    printf("==================================================================\n");


    return SYSTEM_EXIT_SUCCESS;
}