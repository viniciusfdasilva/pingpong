#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define TRUE               1
#define HOST_IP            "127.0.0.1"    // IPV4 loopback address
#define SERVER_PORT        8077          // Server port
#define BUFFER_SIZE        8192         // Default size (64bytes) to (8Kbytes)
#define MAX_CONNECTIONS     2          // Num max client connected simultaneously 
#define SOCKET_ERROR_CODE  -1         // Socket create, Connection server, Receive buffer to server code error
#define SYSTEM_EXIT_FAILED  1        // Operating System program error response 
#define SYSTEM_EXIT_SUCCESS 0       // Operating System program success response

typedef struct sockaddr_in socket_address;

char buffer[BUFFER_SIZE];

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

    struct sockaddr *address = (struct sockaddr*)&server_address;

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
        printf("Client connected: %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
        return client_socket;
    }

}

void send_buffer(int client_socket)
{
    
    printf("Sending [PONG]\n");
    send(client_socket, buffer, strlen(buffer), 0);
}


void receive_buffer(int client_socket)
{
    int bytes_read;

    printf("Receiving [PING]\n");

    while((bytes_read = recv(client_socket, buffer, sizeof(buffer), 0)) > 0)
    {
        buffer[bytes_read] = '\0';
        printf("Client message received successfully!\n");

        send_buffer(client_socket);
    }

    
}

void controlc_handler()
{
    printf("\nProgram finished!\n");
    exit(SYSTEM_EXIT_SUCCESS);
}


void server_listen(int client_socket, int server_socket, socket_address client_address, socklen_t client_addr_len)
{
    
    if (listen(server_socket, MAX_CONNECTIONS) == SOCKET_ERROR_CODE)
    {
        perror("Listening mode error");
        close(server_socket);
        exit(SYSTEM_EXIT_FAILED);
    }

    printf("Server TCP listening on port %d...\n", SERVER_PORT);

    if(signal(SIGINT, controlc_handler) == SIG_ERR)
    {
        perror("Signal create error!");
        exit(SYSTEM_EXIT_FAILED);
    }
    
    while(TRUE)
    {
        client_socket = accept_connection(client_socket, server_socket, client_address, client_addr_len);
        
        receive_buffer(client_socket);
    }
    
}

int main(void)
{
    int server_socket, client_socket;

    server_socket = create_socket();

    socket_address server_address, client_address;
    socklen_t client_addr_len = sizeof(client_address);

    server_address = config_server_address();

    bind_server(server_socket, server_address);
    
    server_listen(client_socket, server_socket, client_address, client_addr_len);
}
