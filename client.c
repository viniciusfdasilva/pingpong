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
#define SERVER_PORT        8077         // Server port
#define BUFFER_SIZE        8192        // Default size
#define PORT_USED_CODE     256
#define SOCKET_ERROR_CODE  -1         // Socket create, Connection server, Receive buffer to server code error
#define SYSTEM_EXIT_FAILED  1        // Operating System program error response 
#define SYSTEM_EXIT_SUCCESS 0       // Operating System program success response


char sended_buffer[BUFFER_SIZE];

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

void init_buffer()
{
    memset(&sended_buffer, '0', BUFFER_SIZE);
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

int receive_buffer(int client_socket)
{
    if(!is_server_down())
    {
        char received_buffer[BUFFER_SIZE]; // Buffer will be send by server to client
        int bytes_read = recv(client_socket, received_buffer, sizeof(received_buffer), 0);

        if(bytes_read == SOCKET_ERROR_CODE)
        {
            perror("Receive server message error!");

        }else
        {
            received_buffer[bytes_read] = '\0'; // Buffer string finalized with '\0',
            printf("Receiving [PONG] %f KB\n", ((float)BUFFER_SIZE/(float)1024));
            printf("\nServer message received successfully!\n");
        }
    }else{
        printf("Server is down!");
        exit(SYSTEM_EXIT_SUCCESS);
    }
}

void send_buffer(int client_socket)
{
    clock_t init_time, end_time, total_time;
    init_buffer(sended_buffer);


    printf("Sending [PING] %f KB\n", ((float)BUFFER_SIZE/(float)1024));
    

   
    send(client_socket, sended_buffer, strlen(sended_buffer), 0);
    receive_buffer(client_socket);

}

void controlc_handler()
{
    printf("\nProgram finished!\n");
    exit(SYSTEM_EXIT_SUCCESS);
}

void socket_listen(int client_socket)
{

    if(signal(SIGINT, controlc_handler) == SIG_ERR)
    {
        perror("Signal create error!");
        exit(SYSTEM_EXIT_FAILED);
    }

    send_buffer(client_socket);
   
}

int main(void)
{
    
    int client_socket  = init_socket();
    
    socket_address server_address;
    server_address = config_server_address();

    connect_to_server(client_socket, server_address);

    clock_t start = clock();
    socket_listen(client_socket);
    clock_t end = clock();

    double elapsed_time_ms = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;

    printf("Time: %.2f ms\n", elapsed_time_ms);


    return SYSTEM_EXIT_SUCCESS;
}