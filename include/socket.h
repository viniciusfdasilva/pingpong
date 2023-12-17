#ifndef __SOCKET_H__

    #define SOCKET_ERROR_CODE  -1             // Socket create, Connection server, Receive buffer to server code error
    #define SYSTEM_EXIT_FAILED  1            // Operating System program error response 
    #define HOST_IP            "127.0.0.1"  // IPV4 loopback address
    #define SERVER_PORT        8481        // Server port
    #define UNIXDOAMIN_ADDRESS "pingpong.socket"
    #define MAX_CONNECTIONS     1         // Num max client connected simultaneously 

    #define NUM_OF_ARGUMENTS    3
    #define TCP_SOCKET_FLAG     1
    #define UDP_SOCKET_FLAG     2
    #define UNIX_SOCKET_FLAG    3
    
    typedef struct sockaddr_in socket_address_ipv4;
    typedef struct sockaddr_un socket_address_unix;
    typedef struct sockaddr_in socket_address;

    extern int create_socket(int sin_family, int sock);
    extern socket_address_unix config_unixdomain_server_address();
    extern socket_address_ipv4 config_tcp_upd_server_address();
    extern void connect_to_server(void* server_address, int client_socket);
    extern void bind_server(void* server_address, int socket_type, int server_socket);
    extern int accept_connection(int client_socket, void* client_address, int server_socket);

#endif // __SOCKET_H__