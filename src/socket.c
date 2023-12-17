#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../include/socket.h"
#include "../include/utils.h"

int create_socket(int sin_family, int sock)
{
    int server_socket;

    server_socket = socket(sin_family, sock, -1);

    if(server_socket == SOCKET_ERROR_CODE)
    {
        panic("Socket create failed!");
    }

    return server_socket;
}

socket_address_unix config_unixdomain_server_address()
{
    socket_address_unix server_address;
    server_address.sun_family = AF_UNIX;   
    strcpy(server_address.sun_path, UNIXDOAMIN_ADDRESS); 
    unlink(UNIXDOAMIN_ADDRESS);
    return server_address;
}

socket_address_ipv4 config_tcp_upd_server_address()
{
    socket_address_ipv4 server_address;
  
    server_address.sin_family      = AF_INET;
    server_address.sin_port        = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = inet_addr(HOST_IP);
    return server_address;
}

void connect_to_server(void *server_address, int client_socket)
{
    struct sockaddr* address = (struct sockaddr*)&server_address;

    int connection_response = connect(client_socket,
                                        address,
                                        (socklen_t)sizeof(server_address)
                                    );

    if(connection_response == SOCKET_ERROR_CODE)
    {
        
        close(client_socket);
        panic("Server connection error\n");
    }

}

void bind_server(void *server_address, int socket_type, int server_socket)
{

    struct sockaddr* address = (struct sockaddr*)&server_address;

    int server_bind_response = bind(server_socket, address, sizeof(server_address));

    if (socket_type == UNIX_SOCKET_FLAG) unlink((const char*)address);

    if(server_bind_response == SOCKET_ERROR_CODE)
    {
        close(server_socket);
        panic("Bind server error");
    }
}

int accept_connection(int client_socket, void *client_address, int server_socket)
{
    socklen_t client_addr_len = sizeof(client_address);
    struct sockaddr *address = (struct sockaddr*)&client_address;

    client_socket = accept(server_socket, address, &client_addr_len);

    if(client_socket == SYSTEM_EXIT_FAILED)
    {
        panic("Accept connection error!");
    }else{
        return client_socket;
    }
}
