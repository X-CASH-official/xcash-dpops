#ifndef BLOCK_VERIFIERS_FUNCTIONS_H_   /* Include guard */
#define BLOCK_VERIFIERS_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int start_new_round(void);
int start_blocks_create_vrf_data(void);
int start_blocks_create_data(char* message, char* network_block_string);
int start_current_round_start_blocks(void);
int data_network_node_create_block(void);
int block_verifiers_create_VRF_secret_key_and_VRF_public_key(char* message);
int block_verifiers_create_VRF_data(void);
int block_verifiers_create_block_signature(char* message);
int block_verifiers_create_vote_results(char* message);
int block_verifiers_create_block_and_update_database(void);
int block_verifiers_create_block(void);
int block_verifiers_send_data_socket(const char* MESSAGE);
#endif