#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
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


#define KB 1024

float times[N];

float send_elapsed_time_ms = 0.00;
float receive_elapsed_time_ms = 0.00;

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

    return client_socket;
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
    }
}

int receive_buffer(int client_socket, int buffer_size)
{
    ssize_t bytes_read;

    int received_buffer[buffer_size];

    recv(client_socket, received_buffer, buffer_size, 0);

}


void send_buffer(int client_socket, int buffer[], size_t buffer_size)
{
    send(client_socket, buffer, buffer_size, 0);
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
        perror("Signal create error!");
        exit(SYSTEM_EXIT_FAILED);
    }

    clock_t start_send_time = clock();
    send_buffer(client_socket, buffer, buffer_size);
    clock_t end_send_time = clock();

    clock_t send_difference = (end_send_time - start_send_time);
    send_elapsed_time_ms = ((float)send_difference / (CLOCKS_PER_SEC/1000));

    clock_t start_receive_time = clock();
    receive_buffer(client_socket, buffer_size);
    clock_t end_receive_time = clock();

    clock_t receive_difference = (end_receive_time - start_receive_time);
    receive_elapsed_time_ms = ((float)receive_difference / (CLOCKS_PER_SEC/1000));
}

float calculate()
{

    float average_total = 0.00;

    for(int i = 0; i < N; i++){
        average_total += times[i];
    }

    return average_total;
}


int main(int argc, char** argv)
{

    size_t buffer_size = (int)atoi(argv[1]);

    int buffer[buffer_size];
    
    for(int i = 0; i < buffer_size; i++)
    {
        buffer[i] = (int)(i % 255);
    }
    
    int client_socket  = init_socket();
    
    socket_address server_address;
    server_address = config_server_address();

    connect_to_server(client_socket, server_address);

    for(int i = 0; i < N; i++){

        socket_listen(client_socket, buffer, buffer_size);
        times[i] = send_elapsed_time_ms + receive_elapsed_time_ms;
    }

    float average = calculate();
    
    printf("%lf\n", (float)(average/(float)N));
    
    return SYSTEM_EXIT_SUCCESS;
}
