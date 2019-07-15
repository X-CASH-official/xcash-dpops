#ifndef SERVER_FUNCTIONS_H_   /* Include guard */
#define SERVER_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

void start_new_round();
void start_current_round_start_blocks();
int start_part_4_of_round();
void update_block_verifiers_list();
void update_databases();
void add_block_verifiers_round_statistics(const char* BLOCK_HEIGHT);
void add_round_statistics();
void check_if_databases_are_synced();
int calculate_main_nodes_roles();
int server_received_data_xcash_proof_of_stake_test_data(const int CLIENT_SOCKET, const char* MESSAGE);
int server_receive_data_socket_node_to_network_data_nodes_get_previous_current_next_block_verifiers_list(const int CLIENT_SOCKET);
int server_receive_data_socket_node_to_network_data_nodes_get_current_block_verifiers_list(const int CLIENT_SOCKET);
int server_receive_data_socket_nodes_to_block_verifiers_reserve_bytes_database_sync_check_all_update(const int CLIENT_SOCKET);
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
int server_receive_data_socket_node_to_block_verifiers_add_reserve_proof(const int CLIENT_SOCKET, const char* MESSAGE);
int server_receive_data_socket_block_verifiers_to_block_verifiers_invalid_reserve_proofs(const char* MESSAGE);
int server_receive_data_socket_nodes_to_block_verifiers_register_delegates(const int CLIENT_SOCKET, const char* MESSAGE);
int server_receive_data_socket_nodes_to_block_verifiers_remove_delegates(const int CLIENT_SOCKET, const char* MESSAGE);
int server_receive_data_socket_nodes_to_block_verifiers_update_delegates(const int CLIENT_SOCKET, const char* MESSAGE);
int server_receive_data_socket_main_node_to_node_message_part_4(const char* MESSAGE);
int server_receive_data_socket_node_to_node(const char* MESSAGE);
int server_receive_data_socket_block_verifiers_to_block_verifiers_vrf_data(const char* MESSAGE);
int server_receive_data_socket_block_verifiers_to_block_verifiers_block_blob_signature(const char* MESSAGE);
int create_server(const int MESSAGE_SETTINGS);
#endif