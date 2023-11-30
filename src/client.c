#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define N                  10000
#define TRUE               1
#define FALSE              0
#define HOST_IP            "127.0.0.1"   // IPV4 loopback address
#define SERVER_PORT        8481         // Server port
#define PORT_USED_CODE     256
#define SOCKET_ERROR_CODE  -1         // Socket create, Connection server, Receive buffer to server code error
#define SYSTEM_EXIT_FAILED  1        // Operating System program error response 
#define SYSTEM_EXIT_SUCCESS 0       // Operating System program success response
#define NUM_OF_ARGUMENTS    3
#define TCP_SOCKET_FLAG     1
#define UDP_SOCKET_FLAG     2
#define UNIX_SOCKET_FLAG    3

float times[N];

int num_of_read_bytes = 0;

typedef struct sockaddr_in socket_address_ipv4;
typedef struct sockaddr_un socket_address_unix;

typedef union socketaddr_t
{
    socket_address_ipv4 socket_ipv4;
    socket_address_unix socket_unix;

} socketaddr_t;


socketaddr_t server_address;

size_t buffer_size;
int socket_type;

int domain, type, protocol;

float elapsed_time_ms = 0.00;

char* address;
int domain, type, protocol, sin_family, sock;

typedef struct sockaddr_in socket_address;

void panic(char* message)
{
    perror(message);
    exit(SYSTEM_EXIT_FAILED);
}

int init_socket()
{   
    int client_socket = socket(sin_family, sock, IPPROTO_TCP);

    if(client_socket == SOCKET_ERROR_CODE)
    {
        panic("Socket create failed!\n");
    }

    return client_socket;
}

socketaddr_t config_server_address()
{

    if(socket_type == UNIX_SOCKET_FLAG)
    {
        server_address.socket_ipv4.sin_family      = sin_family;
        server_address.socket_ipv4.sin_port        = htons(SERVER_PORT);
        server_address.socket_ipv4.sin_addr.s_addr = inet_addr(address);

        
    }else{

        server_address.socket_unix.sun_family = AF_UNIX;   
        strcpy(server_address.socket_unix.sun_path, address); 
        unlink(address);
    }   
}

void connect_to_server(int client_socket)

{
    struct sockaddr* address = socket_type == UNIX_SOCKET_FLAG ? (struct sockaddr*)&server_address.socket_unix : (struct sockaddr*)&server_address.socket_ipv4;
    
    int connection_response = connect(client_socket,
                                        address,
                                        socket_type == UNIX_SOCKET_FLAG ? (socklen_t)sizeof(server_address.socket_unix) : (socklen_t)sizeof(server_address.socket_ipv4)
                                    );

    if(connection_response == SOCKET_ERROR_CODE)
    {
        
        close(client_socket);
        panic("Server connection error\n");
    }
}

int receive_buffer(int client_socket, int buffer_size)
{
    int received_buffer[buffer_size];
    
    if(socket_type == TCP_SOCKET_FLAG) recv(client_socket, received_buffer, buffer_size, 0);
    else
    { 
        struct sockaddr* address    = socket_type == UNIX_SOCKET_FLAG ? (struct sockaddr*)&server_address.socket_unix : (struct sockaddr*)&server_address.socket_ipv4;
        socklen_t* server_addr_size = socket_type == UNIX_SOCKET_FLAG ? (socklen_t*)sizeof(server_address.socket_unix) : (socklen_t*)sizeof(server_address.socket_ipv4);
        recvfrom(client_socket, received_buffer, buffer_size, 0, address, server_addr_size);
    }

    for(int i = 0; i < buffer_size; i++)
    {
        printf("%d", received_buffer[i]);
    }
    printf("\nRECEBIDO\n");
}

void send_buffer(int client_socket, int buffer[], size_t buffer_size)
{
    for(int i = 0; i < num_of_read_bytes; i++)
    {
        printf("ENVIO %d\n", i);

        struct sockaddr* address    = socket_type == UNIX_SOCKET_FLAG ? (struct sockaddr*)&server_address.socket_unix : (struct sockaddr*)&server_address.socket_ipv4;
        socklen_t server_addr_size = socket_type == UNIX_SOCKET_FLAG ? (socklen_t)sizeof(server_address.socket_unix) : (socklen_t)sizeof(server_address.socket_ipv4);

        if(socket_type == TCP_SOCKET_FLAG) send(client_socket, buffer, buffer_size, 0);
        else sendto(client_socket, buffer, buffer_size, 0, address, server_addr_size);

        receive_buffer(client_socket, buffer_size);
    }
}

void controlc_handler()
{
    printf("\nProgram finished!\n");
    exit(SYSTEM_EXIT_SUCCESS);
}

void socket_listen(int client_socket, int buffer[], size_t buffer_size)
{

    if(signal(SIGINT, controlc_handler) == SIG_ERR)
    {
        panic("Signal create error!");
    }

    clock_t start_time = clock();
    send_buffer(client_socket, buffer, buffer_size);
    clock_t end_time = clock();

    elapsed_time_ms = ((float)(end_time - start_time) / (CLOCKS_PER_SEC/1000));
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

            sin_family      = AF_INET;
            address         = HOST_IP;
            sock            = SOCK_DGRAM;

            break;
    
        case UNIX_SOCKET_FLAG: // UNIXDOMAIN SOCKET

            sin_family      = AF_UNIX;
            address         = "pingpong.socket";;
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

    int buffer[buffer_size];
    
    for(int i = 0; i < buffer_size; i++)
    {
        buffer[i] = (int)(i % 255);
    }
    
    int client_socket  = init_socket();
    
    server_address = config_server_address();

    connect_to_server(client_socket);

    socket_listen(client_socket, buffer, buffer_size);

    printf("%lf\n", (float)(elapsed_time_ms));
    
    return SYSTEM_EXIT_SUCCESS;
}
