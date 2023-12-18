#include "../include/socket.h"
#include "../include/pipe.h"
#include "../include/utils.h"

int num_of_read_bytes = 0;

socklen_t client_addr_len;

int *descriptors;
pid_t child;

int server_socket, client_socket;


socket_address client_address;

void* server_address;

int *buffer;
size_t buffer_size;
int socket_type;

char* address;
int domain, type, protocol, sin_family, sock;

void send_buffer(int* buffer)
{
    socklen_t client_addr_len = socket_type == sizeof(client_address);
    struct sockaddr *address  = (struct sockaddr*)&client_address;

    if(socket_type == TCP_SOCKET_FLAG) send(client_socket, buffer, buffer_size, 0);
    else sendto(client_socket, buffer, buffer_size, 0, address, client_addr_len);
}

int receive_buffer()
{
    ssize_t bytes_read;

    socklen_t* client_addr_len = (socklen_t *)sizeof(client_address);
    struct sockaddr *address   = (struct sockaddr*)&client_address;
    int received_buffer[buffer_size];
    
    while((bytes_read = socket_type == UNIX_SOCKET_FLAG ?  recv(client_socket, received_buffer, buffer_size, 0) : recvfrom(client_socket, received_buffer, buffer_size, 0, address, client_addr_len)) > 0)
    {
        send_buffer(received_buffer);
    }
}

void controlc_handler()
{
    exit(SYSTEM_EXIT_SUCCESS);
}

void server_listen()
{
    
    if (listen(server_socket, MAX_CONNECTIONS) == SOCKET_ERROR_CODE)
    {
        
        close(server_socket);
        panic("Listening mode error");
    }
    
    client_socket = accept_connection(client_socket, (void*)&client_address, server_socket);    
    receive_buffer();
}

void attribuite_socket_type(int socket_type)
{
    switch (socket_type)
    {
        case TCP_SOCKET_FLAG: // TCP SOCKET

            server_socket  = create_socket(AF_INET, SOCK_STREAM);
            socket_address_ipv4 udp_ip_address = config_tcp_upd_server_address();
            server_address = (void *)&udp_ip_address;
            break;
    
        case UDP_SOCKET_FLAG: // UDP SOCKET
        
            server_socket  = create_socket(AF_INET, SOCK_DGRAM);
            socket_address_ipv4 tcp_ip_address = config_tcp_upd_server_address();
            server_address = (void *)&tcp_ip_address;
            break;
    
        case UNIX_SOCKET_FLAG: // UNIXDOMAIN SOCKET
            server_socket  = create_socket(AF_UNIX, SOCK_DGRAM);
            socket_address_unix unix_address = config_unixdomain_server_address();
            server_address = (void *)&unix_address;
            break;

        case PIPE_CONNECTION_FLAG: // PIPE CONNECTION FLAG
        
            descriptors = create_pipe();
            child       = fork_process();

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

    printf("OI0");
    get_args(argc, argv);
    printf("OI1");
    attribuite_socket_type(socket_type);

    buffer = (int*)malloc(buffer_size*sizeof(int));    
    printf("OI2");
    //bind_server(server_address, socket_type, server_socket);
    printf("OI3");
    for(int i = 0; i < num_of_read_bytes; i++)
    {
        server_listen();
    }

    printf("OI4");
    return SYSTEM_EXIT_SUCCESS;
}
