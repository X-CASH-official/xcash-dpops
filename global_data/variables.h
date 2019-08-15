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

time_t current_date_and_time;
struct tm* current_UTC_date_and_time; 

char* xcash_wallet_public_address; // Holds your wallets public address
char* block_verifiers_IP_address; // The block verifiers IP address
struct previous_block_verifiers_list previous_block_verifiers_list; // The list of block verifiers name, public address and IP address for the previous round
struct current_block_verifiers_list current_block_verifiers_list; // The list of block verifiers name, public address and IP address for the current round
struct next_block_verifiers_list next_block_verifiers_list; // The list of block verifiers name, public address and IP address for the next round
struct synced_block_verifiers synced_block_verifiers; // The list of block verifiers for syncing the databases
struct main_nodes_list main_nodes_list; // The list of main nodes public address and IP address
struct network_data_nodes_list network_data_nodes_list; // The network data nodes
struct current_round_part_vote_data* current_round_part_vote_data; // The vote data for the current part of the round
struct VRF_data VRF_data; // The list of all of the VRF data to send to the block producer.
struct VRF_data_copy* VRF_data_copy; // A copy of the VRF_data struct to save data when using multiple processes.
struct blockchain_data blockchain_data; // The data for a new block to be added to the network.
struct error_message error_message; // holds all of the error messages and the functions for an error.
struct invalid_reserve_proofs invalid_reserve_proofs; // The invalid reserve proofs that the block verifier finds every round
char* current_round_part; // The current round part (1-4)
char* current_round_part_backup_node; // The current main node in the current round part (0-5)
pthread_rwlock_t rwlock;

char* current_block_height; // The current block height
int* error_message_count; // The error message count
int reserve_proofs_settings; // 1 if reserve proofs can be added to the database, 0 if not
int main_network_data_node_create_block; // 1 if the main network data node can create a block, 0 if not


#endif