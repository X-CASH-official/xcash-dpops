#ifndef SERVER_FUNCTIONS_H_   /* Include guard */
#define SERVER_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int create_server(const int MESSAGE_SETTINGS);
void new_socket_thread(void);
int server_limit_IP_addresses(const int SETTINGS, const char* IP_ADDRESS);
int server_limit_public_addresses(const int SETTINGS, const char* MESSAGE);
void socket_thread(int client_socket);
void* socket_receive_data_thread(void* parameters);
int server_receive_data_socket_get_files(const int CLIENT_SOCKET, const char* MESSAGE);
#endif