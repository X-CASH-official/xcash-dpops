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

// network data nodes
int network_data_node_settings; // 1 if a network data node, 0 if not a network data node

char* xcash_wallet_public_address; // Holds your wallets public address
struct previous_block_verifiers_list previous_block_verifiers_list; // The list of block verifiers name, public address and IP address for the previous round
struct current_block_verifiers_list current_block_verifiers_list; // The list of block verifiers name, public address and IP address for the current round
struct next_block_verifiers_list next_block_verifiers_list; // The list of block verifiers name, public address and IP address for the next round
struct synced_block_verifiers_IP_addresses synced_block_verifiers_IP_addresses; // The list of block verifiers for syncing the databases
struct main_nodes_list main_nodes_list; // The list of main nodes public address and IP address
struct current_round_part_consensus_node_data current_round_part_consensus_node_data; // The network block data and VRF data for the current part of the round
struct current_round_part_vote_data current_round_part_vote_data; // The vote data for the current part of the round
struct VRF_data_block_verifiers VRF_data_block_verifiers; // The list of all block verifiers received data from the main node
struct blockchain_data blockchain_data; // The data for a new block to be added to the network.
struct invalid_reserve_proofs invalid_reserve_proofs; // The invalid reserve proofs that the block verifier finds every round
char* nodes_public_address_list_received_data; // The list of enabled nodes public address that have sent data, to prevent multiple votes (node1|node2|)
char* server_message; // The message the server should run. It will ignore any other messages.
char* vrf_public_key_part_1; // The VRF public key from part 1 of the round. This must match the VRF public key from part 3 of the round.
char* vrf_secret_key_part_1; // The VRF secret key from part 1 of the round.
char* vrf_alpha_string_part_2; // The VRF alpha string from part 2 of the round. This must match the VRF alpha string from part 3 of the round.
char* current_round_part; // The current round part (1-4)
char* current_round_part_backup_node; // The current main node in the current round part (0-5)

char* current_block_height; // The current block height


#endif