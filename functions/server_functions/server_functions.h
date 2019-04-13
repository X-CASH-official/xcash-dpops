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
int server_receive_data_socket_consensus_node_to_node(const int CLIENT_SOCKET, struct mainnode_timeout_thread_parameters* parameters, char* message);
int main_node_to_node_message_part_1(const int CLIENT_SOCKET, struct mainnode_timeout_thread_parameters* parameters, char* message);
int create_server(const int MESSAGE_SETTINGS);
#endif