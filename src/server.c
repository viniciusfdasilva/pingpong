#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/un.h>
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

typedef struct sockaddr_in socket_address_ipv4;
typedef struct sockaddr_un socket_address_unix;


socklen_t client_addr_len;

typedef union socketaddr_t
{
    socket_address_ipv4 socket_ipv4;
    socket_address_unix socket_unix;

} socketaddr_t;

socketaddr_t client_address;
socketaddr_t server_address;

size_t buffer_size;
int socket_type;

char* address;
int domain, type, protocol, sin_family, sock;

void panic(char* message)
{
    perror(message);
    exit(SYSTEM_EXIT_FAILED);
}

int create_socket()
{
    int server_socket;

    server_socket = socket(sin_family, sock, -1);

    if(server_socket == SOCKET_ERROR_CODE)
    {
        panic("Socket create failed!");
    }

    return server_socket;
}

void config_server_address()
{

    if(socket_type == UNIX_SOCKET_FLAG)
    {
        server_address.socket_ipv4.sin_family      = sin_family;
        server_address.socket_ipv4.sin_port        = htons(SERVER_PORT);
        server_address.socket_ipv4.sin_addr.s_addr = inet_addr(address);

        
    }else{

        server_address.socket_unix.sun_family = AF_UNIX;   
        strcpy(server_address.socket_unix.sun_path, address); 
        unlink((const char*)address);
    }   
}

void bind_server(int server_socket)
{

    struct sockaddr* address = socket_type == UNIX_SOCKET_FLAG ? (struct sockaddr*)&server_address.socket_unix : (struct sockaddr*)&server_address.socket_ipv4;

    int server_bind_response = bind(server_socket, address, socket_type == UNIX_SOCKET_FLAG ? sizeof(server_address.socket_unix) : sizeof(server_address.socket_ipv4));

    if (socket_type == UNIX_SOCKET_FLAG) unlink((const char*)address);

    if(server_bind_response == SOCKET_ERROR_CODE)
    {
        close(server_socket);
        panic("Bind server error");
    }
}

int accept_connection(int client_socket, int server_socket)
{
    socklen_t client_addr_len = socket_type == UNIX_SOCKET_FLAG ? sizeof(client_address.socket_ipv4) : sizeof(client_address.socket_unix);
    struct sockaddr *address = socket_type == UNIX_SOCKET_FLAG ? (struct sockaddr*)&client_address.socket_unix : (struct sockaddr*)&client_address.socket_ipv4;

    client_socket = accept(server_socket, address, &client_addr_len);

    if(client_socket == SYSTEM_EXIT_FAILED)
    {
        panic("Accept connection error!");
    }else{
        return client_socket;
    }
}

void send_buffer(int client_socket, int* buffer, int buffer_size)
{
    socklen_t client_addr_len = socket_type == UNIX_SOCKET_FLAG ? sizeof(client_address.socket_ipv4) : sizeof(client_address.socket_unix);
    struct sockaddr *address = socket_type  == UNIX_SOCKET_FLAG ? (struct sockaddr*)&client_address.socket_unix : (struct sockaddr*)&client_address.socket_ipv4;

    if(socket_type == TCP_SOCKET_FLAG) send(client_socket, buffer, buffer_size, 0);
    else sendto(client_socket, buffer, buffer_size, 0, address, client_addr_len);
}

int receive_buffer(int client_socket, int buffer_size)
{
    ssize_t bytes_read;

    socklen_t* client_addr_len = socket_type == UNIX_SOCKET_FLAG ? (socklen_t *)sizeof(client_address.socket_ipv4) : (socklen_t *)sizeof(client_address.socket_unix);
    struct sockaddr *address = socket_type  == UNIX_SOCKET_FLAG ? (struct sockaddr*)&client_address.socket_unix : (struct sockaddr*)&client_address.socket_ipv4;
    int received_buffer[buffer_size];
    
    while((bytes_read = socket_type == UNIX_SOCKET_FLAG ?  recv(client_socket, received_buffer, buffer_size, 0) : recvfrom(client_socket, received_buffer, buffer_size, 0, address, client_addr_len)) > 0)
    {
        send_buffer(client_socket, received_buffer, buffer_size);
    }
}

void controlc_handler()
{
    exit(SYSTEM_EXIT_SUCCESS);
}

void server_listen(int client_socket, int server_socket, int buffer_size)
{
    
    if (listen(server_socket, MAX_CONNECTIONS) == SOCKET_ERROR_CODE)
    {
        
        close(server_socket);
        panic("Listening mode error");
    }
    
    client_socket = accept_connection(client_socket, server_socket);    
    receive_buffer(client_socket, buffer_size);
}

void attribuite_socket_type(int socket_type)
{
    switch (socket_type)
    {
        case TCP_SOCKET_FLAG: // TCP SOCKET
            sin_family      = AF_INET;
            
            address         = HOST_IP;
            sock            = SOCK_STREAM;
            break;
    
        case UDP_SOCKET_FLAG: // UDP SOCKET
        
            address         = HOST_IP;
            sock            = SOCK_DGRAM;
            break;
    
        case UNIX_SOCKET_FLAG: // UNIXDOMAIN SOCKET

            sin_family      = AF_UNIX;
            address         = "pingpong.socket";
            sock            = SOCK_DGRAM;

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

    int* buffer = (int*)malloc(buffer_size*sizeof(int));    
    
    int server_socket, client_socket;

    server_socket = create_socket();

    config_server_address();

    bind_server(server_socket);
    
    for(int i = 0; i < num_of_read_bytes; i++)
    {
        server_listen(client_socket, server_socket, buffer_size);
    }

    return SYSTEM_EXIT_SUCCESS;
}
