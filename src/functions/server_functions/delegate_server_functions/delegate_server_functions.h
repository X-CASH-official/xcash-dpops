#ifndef DELEGATE_SERVER_FUNCTIONS_H_   /* Include guard */
#define DELEGATE_SERVER_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int block_verifiers_add_reserve_proof_check_if_data_is_valid(const char* MESSAGE, struct reserve_proof* reserve_proof);
int add_reserve_proof_remove_previous_vote(const char* PUBLIC_ADDRESS_CREATE_RESERVE_PROOF_DATA);
int server_receive_data_socket_node_to_block_verifiers_add_reserve_proof(const int CLIENT_SOCKET, const char* MESSAGE);
int check_for_valid_delegate_name(const char* DELEGATE_NAME);
int server_receive_data_socket_nodes_to_block_verifiers_register_delegates(const int CLIENT_SOCKET, const char* MESSAGE);
int check_for_valid_delegate_fee(const char* MESSAGE);
int server_receive_data_socket_nodes_to_block_verifiers_update_delegates(const int CLIENT_SOCKET, const char* MESSAGE);
#endif