#ifndef REMOTE_DATA_FUNCTIONS_H_   /* Include guard */
#define REMOTE_DATA_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int validate_name(const char* MESSAGE);
void server_receive_data_socket_nodes_to_network_data_nodes_remote_data_get_address_settings(const int CLIENT_SOCKET, const char* MESSAGE);
int server_receive_data_socket_remote_data_get_address_settings(const int CLIENT_SOCKET, const char* DATA);
void server_receive_data_socket_nodes_to_network_data_nodes_remote_data_get_address_from_name(const int CLIENT_SOCKET, const char* MESSAGE);
int server_receive_data_socket_remote_data_get_address_from_name(const int CLIENT_SOCKET, const char* DATA);
void server_receive_data_socket_nodes_to_network_data_nodes_remote_data_get_information_from_name(const int CLIENT_SOCKET, const char* MESSAGE);
int server_receive_data_socket_remote_data_get_information_from_name(const int CLIENT_SOCKET, const char* DATA);
void server_receive_data_socket_remote_data_nodes_to_block_verifiers_update_remote_data(const int CLIENT_SOCKET, const char* MESSAGE);
#endif
