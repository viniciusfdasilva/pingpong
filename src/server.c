#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define FALSE              0
#define TRUE               1
#define HOST_IP            "127.0.0.1"    // IPV4 loopback address
#define SERVER_PORT        8481          // Server port
#define MAX_CONNECTIONS     10          // Num max client connected simultaneously 
#define SOCKET_ERROR_CODE  -1         // Socket create, Connection server, Receive buffer to server code error
#define SYSTEM_EXIT_FAILED  1        // Operating System program error response 
#define SYSTEM_EXIT_SUCCESS 0       // Operating System program success response
#define KB                  1024

typedef struct sockaddr_in socket_address;

int create_socket()
{
    int server_socket;

    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(server_socket == SOCKET_ERROR_CODE)
    {
        perror("Socket create failed!");
        exit(SYSTEM_EXIT_FAILED);
    }

    return server_socket;
}

socket_address config_server_address()
{
    socket_address server_address;

    server_address.sin_family      = AF_INET; // IPV4 ips family
    server_address.sin_port        = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = inet_addr(HOST_IP);

    return server_address;
}

void bind_server(int server_socket, socket_address server_address)
{

    struct sockaddr* address = (struct sockaddr*)&server_address;

    int server_bind_response = bind(server_socket, address, sizeof(server_address));

    if(server_bind_response == SOCKET_ERROR_CODE)
    {
        perror("Bind server error");
        close(server_socket);
        exit(SYSTEM_EXIT_FAILED); 
    }
}

int accept_connection(int client_socket, int server_socket, socket_address client_address, socklen_t client_addr_len)
{
    struct sockaddr *address = (struct sockaddr*)&client_address;

    client_socket = accept(server_socket, address, &client_addr_len);

    if(client_socket == SYSTEM_EXIT_FAILED)
    {
        perror("Accept connection error!");
        exit(SYSTEM_EXIT_FAILED);
 
    }else{
        return client_socket;
    }

}

void send_buffer(int client_socket, int buffer[], int buffer_size)
{
    
    send(client_socket, buffer, buffer_size, 0);
}


int receive_buffer(int client_socket, int buffer_size)
{
    ssize_t bytes_read;

    int received_buffer[buffer_size];

    while((bytes_read = recv(client_socket, received_buffer, buffer_size, 0)) > 0)
    {

        received_buffer[bytes_read] = '\0';

        send_buffer(client_socket, received_buffer, buffer_size);
    }
}

void controlc_handler()
{
    printf("\nProgram finished!\n");
    exit(SYSTEM_EXIT_SUCCESS);
}


void server_listen(int client_socket, int server_socket, socket_address client_address, socklen_t client_addr_len, int buffer_size)
{
    
    if (listen(server_socket, MAX_CONNECTIONS) == SOCKET_ERROR_CODE)
    {
        perror("Listening mode error");
        close(server_socket);
        exit(SYSTEM_EXIT_FAILED);
    }

    printf("[SERVER] - Server TCP listening on port %d...\n", SERVER_PORT);

    client_socket = accept_connection(client_socket, server_socket, client_address, client_addr_len);    
    receive_buffer(client_socket, buffer_size);
}

int main(int argc, char** argv)
{

    size_t buffer_size = 8192;
    int buffer[buffer_size];

    int server_socket, client_socket;

    server_socket = create_socket();

    socket_address server_address, client_address;
    socklen_t client_addr_len = sizeof(client_address);

    server_address = config_server_address();

    bind_server(server_socket, server_address);
    
    server_listen(client_socket, server_socket, client_address, client_addr_len, buffer_size);
}
