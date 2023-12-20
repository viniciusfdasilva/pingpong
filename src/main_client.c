#include <socket.h>
#include <pipe.h>
#include <utils.h>

#define N 10000

float times[N];

int num_of_read_bytes = 0;

int client_socket;
int* buffer;
size_t buffer_size;
int socket_type;

pid_t child;
int *descriptors;

void* server_address;

float elapsed_time_ms = 0.00;

int receive_buffer(void *server_address)
{
    int received_buffer[buffer_size];
    
    if(socket_type == TCP_SOCKET_FLAG) recv(client_socket, received_buffer, buffer_size, 0);
    else
    { 
        struct sockaddr* address    = (struct sockaddr*)&server_address;
        socklen_t* server_addr_size = (socklen_t*)sizeof(server_address);
        recvfrom(client_socket, received_buffer, buffer_size, 0, address, server_addr_size);
    }
}

void send_buffer(void *server_address)
{
 
    struct sockaddr* address   = (struct sockaddr*)&server_address;
    socklen_t server_addr_size = (socklen_t)sizeof(server_address);

    if(socket_type == TCP_SOCKET_FLAG) send(client_socket, buffer, buffer_size, 0);
    else sendto(client_socket, buffer, buffer_size, 0, address, server_addr_size);
    receive_buffer(server_address);
}

void controlc_handler()
{
    printf("\nProgram finished!\n");
    exit(SYSTEM_EXIT_SUCCESS);
}

void socket_listen()
{

    if(signal(SIGINT, controlc_handler) == SIG_ERR)
    {
        panic("Signal create error!");
    }

    clock_t start_time = clock();
    send_buffer(server_address);
    clock_t end_time = clock();

    elapsed_time_ms = ((float)(end_time - start_time) / (CLOCKS_PER_SEC/1000));
}

void attribuite_and_init_socket(int socket_type)
{
    switch (socket_type)
    {
        case TCP_SOCKET_FLAG: // TCP SOCKET
            client_socket       = create_socket(AF_INET, SOCK_STREAM);
            socket_address_ipv4 tcp_ip_address = config_tcp_upd_server_address();
            server_address = (void*)&tcp_ip_address;
            connect_to_server(server_address,client_socket);
            break;
    
        case UDP_SOCKET_FLAG: // UDP SOCKET
            client_socket       = create_socket(AF_INET, SOCK_STREAM);
            socket_address_ipv4 udp_ip_address = config_tcp_upd_server_address();
            server_address = (void*)&udp_ip_address;
            connect_to_server(server_address,client_socket);

            break;
    
        case UNIX_SOCKET_FLAG: // UNIXDOMAIN SOCKET
            client_socket       = create_socket(AF_UNIX, SOCK_DGRAM);
            socket_address_unix unix_ip_address = config_unixdomain_server_address();
            server_address = (void*)&unix_ip_address;
            connect_to_server(server_address,client_socket);
            
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

    get_args(argc, argv);
    attribuite_and_init_socket(socket_type);

    buffer = get_init_buffer(buffer_size);

    float sum_elapsed_time_ms = 0.0;

    for(int i = 0; i < num_of_read_bytes; i++)
    {
        socket_type == PIPE_CONNECTION_FLAG ? elapsed_time_ms = pingpong_pipe(descriptors, child, buffer_size, buffer) : socket_listen();

        sum_elapsed_time_ms += elapsed_time_ms;
    }

    printf("%lf\n", (float)((sum_elapsed_time_ms)/(num_of_read_bytes)));

    return SYSTEM_EXIT_SUCCESS;
}
