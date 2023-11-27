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
#define MAX_CONNECTIONS     1           // Num max client connected simultaneously 
#define SOCKET_ERROR_CODE  -1          // Socket create, Connection server, Receive buffer to server code error
#define SYSTEM_EXIT_FAILED  1         // Operating System program error response 
#define SYSTEM_EXIT_SUCCESS 0        // Operating System program success response
#define NUM_OF_ARGUMENTS    3
#define TCP_SOCKET_FLAG     1
#define UDP_SOCKET_FLAG     2
#define UNIX_SOCKET_FLAG    3

int num_of_read_bytes = 0;
typedef struct sockaddr_in socket_address;

socket_address client_address;
socklen_t client_addr_len;

size_t buffer_size;
int socket_type;

int domain, type, protocol, sin_family, address, sock;

void panic(char* message)
{
    perror(message);
    exit(SYSTEM_EXIT_FAILED);
}

int create_socket()
{
    int server_socket;

    server_socket = socket(sin_family, sock, IPPROTO_TCP);

    if(server_socket == SOCKET_ERROR_CODE)
    {
        panic("Socket create failed!");
    }

    return server_socket;
}

socket_address config_server_address()
{
    socket_address server_address;
  
    server_address.sin_family      = sin_family;
    server_address.sin_port        = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = address;

    return server_address;
}

void bind_server(int server_socket, socket_address server_address)
{

    struct sockaddr* address = (struct sockaddr*)&server_address;

    int server_bind_response = bind(server_socket, address, sizeof(server_address));

    if(server_bind_response == SOCKET_ERROR_CODE)
    {
        close(server_socket);
        panic("Bind server error");
    }
}

int accept_connection(int client_socket, int server_socket, socket_address client_address, socklen_t client_addr_len)
{
    struct sockaddr *address = (struct sockaddr*)&client_address;

    client_socket = accept(server_socket, address, &client_addr_len);

    if(client_socket == SYSTEM_EXIT_FAILED)
    {
        panic("Accept connection error!");
    }else{
        return client_socket;
    }
}

void send_buffer(int client_socket, int buffer[], int buffer_size)
{
    socklen_t client_addr_size = (socklen_t)sizeof(client_address);

    if(socket_type != UDP_SOCKET_FLAG) send(client_socket, buffer, buffer_size, 0);
    else sendto(client_socket, buffer, buffer_size, 0, (const struct sockaddr *)&client_address, client_addr_size);
}

int receive_buffer(int client_socket, int buffer_size)
{
    ssize_t bytes_read;

    socklen_t* client_addr_size = (socklen_t*)sizeof(client_address);

    int received_buffer[buffer_size];
    
    for(int i = 0; i < num_of_read_bytes; i++)
    {

        while((bytes_read = socket_type == UDP_SOCKET_FLAG ? recv(client_socket, received_buffer, buffer_size, 0) : recvfrom(client_socket, received_buffer, buffer_size, 0, (struct sockaddr *)&client_address, client_addr_size)) > 0)
        {
            send_buffer(client_socket, received_buffer, buffer_size);
        }
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
        
        close(server_socket);
        panic("Listening mode error");
    }

    printf("[SERVER] - Server TCP listening on port %d...\n", SERVER_PORT);

    client_socket = accept_connection(client_socket, server_socket, client_address, client_addr_len);    
    receive_buffer(client_socket, buffer_size);
}

void attribuite_socket_type(int socket_type)
{
    switch (socket_type)
    {
        case TCP_SOCKET_FLAG: // TCP SOCKET
            sin_family      = AF_INET;
            
            address         = inet_addr(HOST_IP);
            sock            = SOCK_STREAM;
            break;
    
        case UDP_SOCKET_FLAG: // UDP SOCKET
        
            address         = INADDR_ANY;
            sock            = SOCK_DGRAM;
            break;
    
        case UNIX_SOCKET_FLAG: // UNIXDOMAIN SOCKET

            sin_family      = AF_UNIX;
            address         = inet_addr(HOST_IP);
            sock            = SOCK_STREAM;

            break;
        
        default:
            panic("Value error!");
            break;
    }
}

void get_args(int argc, char** argv)
{
    if(argc > NUM_OF_ARGUMENTS)
    {
        buffer_size       = (int)atoi(argv[1]);
        num_of_read_bytes = (int)atoi(argv[2]);
        socket_type       = (int)atoi(argv[3]);
    }else
    {
        panic("Argument is missing!");
    }
    
}

int main(int argc, char** argv)
{

    get_args(argc, argv);
    attribuite_socket_type(socket_type);

    int buffer[buffer_size];

    int server_socket, client_socket;

    server_socket = create_socket();

    socket_address server_address;
    client_addr_len = sizeof(client_address);

    server_address = config_server_address();

    bind_server(server_socket, server_address);
    
    server_listen(client_socket, server_socket, client_address, client_addr_len, buffer_size);

    return SYSTEM_EXIT_SUCCESS;
}
