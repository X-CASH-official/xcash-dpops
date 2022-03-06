#ifndef REMOTE_DATA_FUNCTIONS_H_   /* Include guard */
#define REMOTE_DATA_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int get_previous_block_producer(char *previous_block_producer);
int validate_name(const char* MESSAGE);
int validate_name_settings(const char* MESSAGE);
int remote_data_validate_tx_hash(const char* tx_hash,const char* public_address,const char* delegates_public_address,const char* delegates_amount);
void server_receive_data_socket_nodes_to_network_data_nodes_remote_data_get_address_settings(const int CLIENT_SOCKET, const char* MESSAGE);
int server_receive_data_socket_remote_data_get_address_settings(const int CLIENT_SOCKET, const char* DATA);
void server_receive_data_socket_nodes_to_network_data_nodes_remote_data_get_address_from_name(const int CLIENT_SOCKET, const char* MESSAGE);
int server_receive_data_socket_remote_data_get_address_from_name(const int CLIENT_SOCKET, const char* DATA);
void server_receive_data_socket_nodes_to_network_data_nodes_remote_data_get_information_from_name(const int CLIENT_SOCKET, const char* MESSAGE);
int server_receive_data_socket_remote_data_get_information_from_name(const int CLIENT_SOCKET, const char* DATA);
void server_receive_data_socket_remote_data_nodes_to_block_verifiers_update_remote_data(const int CLIENT_SOCKET, const char* MESSAGE);
void server_receive_data_socket_remote_data_nodes_to_block_verifiers_save_name(const int CLIENT_SOCKET, const char* MESSAGE);
void server_receive_data_socket_remote_data_nodes_to_block_verifiers_purchase_name(const int CLIENT_SOCKET, const char* MESSAGE);
int server_receive_data_socket_remote_data_get_delegates_information(const int CLIENT_SOCKET);
int server_receive_data_socket_remote_data_get_block_producer_information(const int CLIENT_SOCKET, const char* DATA);
void add_delegates_to_remote_data_delegates(void);
void check_for_expired_names(void);
void maintain_slist_and_plist(void);
void* remote_data_timer_thread(void* parameters);
#endif
