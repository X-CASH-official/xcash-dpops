#ifndef VARIABLES_H_   /* Include guard */
#define VARIABLES_H_

#include <pthread.h>

#include "structures.h"

/*
-----------------------------------------------------------------------------------------------------------
Global Variables
-----------------------------------------------------------------------------------------------------------
*/

// database
mongoc_client_t* database_client;
mongoc_client_pool_t* database_client_thread_pool;

char* xcash_wallet_public_address; // Holds your wallets public address
struct block_verifiers_list block_verifiers_list; // The list of the enabled nodes name, public address and IP address
struct block_validation_nodes_list block_validation_nodes_list; // The list of block validation nodes public address
struct current_round_part_consensus_node_data current_round_part_consensus_node_data; // The network block data and VRF data for the current part of the round
struct current_round_part_vote_data current_round_part_vote_data; // The vote data for the current part of the round
struct VRF_data_block_verifiers VRF_data_block_verifiers; // The list of all block verifiers received data from the main node
struct blockchain_data blockchain_data; // The data for a new block to be added to the network.
char* nodes_public_address_list_received_data; // The list of enabled nodes public address that have sent data, to prevent multiple votes (node1|node2|)
char* server_message; // The message the server should run. It will ignore any other messages.
char* current_consensus_nodes_IP_address; // The current consensus nodes IP address
char* main_nodes_public_address; // The current main nodes public address
char* vrf_public_key_part_1; // The VRF public key from part 1 of the round. This must match the VRF public key from part 3 of the round.
char* vrf_secret_key_part_1; // The VRF secret key from part 1 of the round.
char* vrf_alpha_string_part_2; // The VRF alpha string from part 2 of the round. This must match the VRF alpha string from part 3 of the round.
char* current_round_part; // The current round part (1-4)
char* current_round_part_backup_node; // The current main node in the current round part (0-5)
#endif