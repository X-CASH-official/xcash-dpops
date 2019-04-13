#ifndef SERVER_FUNCTIONS_H_   /* Include guard */
#define SERVER_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int get_current_consensus_nodes_IP_address();
int get_updated_node_list();
int server_received_data_xcash_proof_of_stake_test_data(const int CLIENT_SOCKET, char* message);
int server_receive_data_socket_consensus_node_to_node(struct mainnode_timeout_thread_parameters* parameters, char* message);
int server_receive_data_socket_main_node_to_node_message_part_1(struct mainnode_timeout_thread_parameters* parameters, struct node_to_node_timeout_thread_parameters* node_to_node_timeout_thread_parameters, char* message);
int server_receive_data_socket_main_node_to_node_message_part_2(struct mainnode_timeout_thread_parameters* parameters, struct node_to_node_timeout_thread_parameters* node_to_node_timeout_thread_parameters, char* message);
int server_receive_data_socket_main_node_to_node_message_part_3(struct mainnode_timeout_thread_parameters* parameters, struct node_to_node_timeout_thread_parameters* node_to_node_timeout_thread_parameters, char* message);
int server_receive_data_socket_main_node_to_node_message_part_4(struct mainnode_timeout_thread_parameters* parameters, struct node_to_node_timeout_thread_parameters* node_to_node_timeout_thread_parameters, char* message);
int server_receive_data_socket_node_to_node(char* message);
int server_receive_data_socket_consensus_node_to_main_node_message_start_part_of_round(char* message);
int create_server(const int MESSAGE_SETTINGS);
#endif