#ifndef DELEGATES_WEBSITE_FUNCTIONS_H_   /* Include guard */
#define DELEGATES_WEBSITE_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_delegates_website_get_statistics(const int CLIENT_SOCKET);
int server_receive_data_socket_get_delegates(const int CLIENT_SOCKET);
int server_receive_data_socket_get_delegates_statistics(const int CLIENT_SOCKET, const char* DATA);
int server_receive_data_socket_get_delegates_information(const int CLIENT_SOCKET, const char* DATA);
int server_receive_data_socket_check_vote_status(const int CLIENT_SOCKET, const char* DATA);
int server_receive_data_socket_get_delegates_voters_list(const int CLIENT_SOCKET, const char* DATA);
int server_receive_data_socket_get_round_statistics(const int CLIENT_SOCKET, const char* DATA);
#endif