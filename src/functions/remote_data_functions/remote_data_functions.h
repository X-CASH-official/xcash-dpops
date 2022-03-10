#ifndef REMOTE_DATA_FUNCTIONS_H_   /* Include guard */
#define REMOTE_DATA_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int get_previous_block_producer(char *previous_block_producer);
int validate_amount(const char* MESSAGE);
int validate_name(const char* MESSAGE);
int validate_name_settings(const char* MESSAGE);
int document_field_contains_string_from_collection(const char* DATABASE, const char* COLLECTION, const char* FIELD_NAME, const char* DATA);
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
int server_receive_data_socket_remote_data_get_block_producer_information(const int CLIENT_SOCKET);
void server_receive_data_socket_remote_data_nodes_to_block_verifiers_delegates_set_amount(const int CLIENT_SOCKET, const char* MESSAGE);
void server_receive_data_socket_remote_data_nodes_to_block_verifiers_renewal_start(const int CLIENT_SOCKET, const char* MESSAGE);
void server_receive_data_socket_remote_data_nodes_to_block_verifiers_renewal_end(const int CLIENT_SOCKET, const char* MESSAGE);
void add_delegates_to_remote_data_delegates(void);
void check_for_expired_names(void);
void maintain_slist_and_plist(void);
void* remote_data_database_synchronization_timer_thread(void* parameters);
void* remote_data_maintenance_timer_thread(void* parameters);
void sync_delegates_remote_data_databases(void);
void server_receive_data_socket_network_data_nodes_to_network_data_nodes_database_sync_check_remote_data(const char* MESSAGE);
int sync_check_remote_data_database(int settings);
int sync_check_majority_remote_data_database(void);
int sync_check_remote_data_delegates_database(int settings);
int sync_check_majority_remote_data_delegates_database(void);
void server_receive_data_socket_block_verifiers_to_block_verifiers_remote_data_database_sync_check_update(const int CLIENT_SOCKET, const char* MESSAGE);
void server_receive_data_socket_block_verifiers_to_block_verifiers_remote_data_database_download_file_update(const int CLIENT_SOCKET);
void server_receive_data_socket_block_verifiers_to_block_verifiers_remote_data_delegates_database_sync_check_update(const int CLIENT_SOCKET, const char* MESSAGE);
void server_receive_data_socket_block_verifiers_to_block_verifiers_remote_data_delegates_database_download_file_update(const int CLIENT_SOCKET);
int sync_remote_data_database(int settings, const char* DELEGATES_IP_ADDRESS);
int sync_remote_data_delegates_database(int settings, const char* DELEGATES_IP_ADDRESS);
#endif
