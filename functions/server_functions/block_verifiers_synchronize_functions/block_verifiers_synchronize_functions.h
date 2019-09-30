#ifndef BLOCK_VERIFIERS_SYNCHRONIZE_FUNCTIONS_H_   /* Include guard */
#define BLOCK_VERIFIERS_SYNCHRONIZE_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int check_if_databases_are_synced(const int SETTINGS);
int server_receive_data_socket_node_to_network_data_nodes_get_previous_current_next_block_verifiers_list(const int CLIENT_SOCKET);
int server_receive_data_socket_node_to_network_data_nodes_get_current_block_verifiers_list(const int CLIENT_SOCKET);
int server_receive_data_socket_nodes_to_block_verifiers_reserve_bytes_database_sync_check_all_update(const int CLIENT_SOCKET);
int server_receive_data_socket_node_to_block_verifiers_get_reserve_bytes(const int CLIENT_SOCKET, const char* MESSAGE);
int server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_all_update(const int CLIENT_SOCKET, const char* MESSAGE);
int server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_update(const int CLIENT_SOCKET, const char* MESSAGE);
int server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_download_file_update(const int CLIENT_SOCKET, const char* MESSAGE);
int server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_all_update(const int CLIENT_SOCKET, const char* MESSAGE);
int server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_update(const int CLIENT_SOCKET, const char* MESSAGE);
int server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_download_file_update(const int CLIENT_SOCKET, const char* MESSAGE);
int server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_sync_check_update(const int CLIENT_SOCKET, const char* MESSAGE);
int server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_download_file_update(const int CLIENT_SOCKET, const char* MESSAGE);
int server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_sync_check_update(const int CLIENT_SOCKET, const char* MESSAGE);
int server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_download_file_update(const int CLIENT_SOCKET, const char* MESSAGE);
int sync_all_block_verifiers_list(void);
int get_synced_block_verifiers(void);
int sync_check_reserve_proofs_database(int settings);
int sync_reserve_proofs_database(const char* RESERVE_PROOFS_DATABASE, const int SETTINGS);
int sync_check_reserve_bytes_database(const int SETTINGS);
int sync_reserve_bytes_database(const int SETTINGS);
int sync_check_delegates_database(const int SETTINGS);
int sync_delegates_database(const int SETTINGS);
int sync_check_statistics_database(const int SETTINGS);
int sync_statistics_database(const int SETTINGS);
#endif