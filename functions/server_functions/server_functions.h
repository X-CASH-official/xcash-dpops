#ifndef SERVER_FUNCTIONS_H_   /* Include guard */
#define SERVER_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

void start_new_round();
void start_current_round_start_blocks();
void start_current_round();
void data_network_node_save_previous_round_statistics();
void add_block_verifiers_round_statistics();
void add_round_statistics();
int calculate_main_nodes_roles();
int get_updated_node_list();
int server_received_data_xcash_proof_of_stake_test_data(const int CLIENT_SOCKET, char* message);
int server_receive_data_socket_node_to_network_data_nodes_get_current_block_verifiers_list(const int CLIENT_SOCKET);
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
int server_receive_data_socket_block_verifiers_to_block_verifiers_invalid_reserve_proofs(const char* message);
int server_receive_data_socket_nodes_to_block_verifiers_register_delegates(const int CLIENT_SOCKET, const char* MESSAGE);
int server_receive_data_socket_nodes_to_block_verifiers_remove_delegates(const int CLIENT_SOCKET, const char* MESSAGE);
int server_receive_data_socket_consensus_node_to_node(struct mainnode_timeout_thread_parameters* parameters, char* message);
int server_receive_data_socket_main_node_to_node_message_part_1(struct mainnode_timeout_thread_parameters* mainnode_timeout_thread_parameters, struct node_to_node_timeout_thread_parameters* node_to_node_timeout_thread_parameters, char* message);
int server_receive_data_socket_main_node_to_node_message_part_2(struct mainnode_timeout_thread_parameters* mainnode_timeout_thread_parameters, struct node_to_node_timeout_thread_parameters* node_to_node_timeout_thread_parameters, char* message);
int server_receive_data_socket_main_node_to_node_message_part_3(struct mainnode_timeout_thread_parameters* mainnode_timeout_thread_parameters, struct node_to_node_timeout_thread_parameters* node_to_node_timeout_thread_parameters, char* message);
int server_receive_data_socket_main_node_to_node_message_part_4(struct mainnode_timeout_thread_parameters* mainnode_timeout_thread_parameters, struct node_to_node_timeout_thread_parameters* node_to_node_timeout_thread_parameters, char* message);
int server_receive_data_socket_node_to_node(char* message);
int server_receive_data_socket_consensus_node_to_main_node_message_start_part_of_round(char* message);
int server_receive_data_socket_consensus_node_to_node_and_main_node_restart(char* message);
int create_server(const int MESSAGE_SETTINGS);
#endif