#ifndef VARIABLES_H_   /* Include guard */
#define VARIABLES_H_

#include <sys/epoll.h>
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

time_t date_and_time;
struct tm* UTC_date_and_time;

// network data nodes
int network_data_node_settings; // 1 if a network data node, 0 if not a network data node 
char verify_block_file[1000]; // The verify block file
char xcash_wallet_public_address[XCASH_WALLET_LENGTH+1]; // Holds your wallets public address
struct previous_block_verifiers_list previous_block_verifiers_list; // The list of block verifiers name, public address and IP address for the previous round
struct current_block_verifiers_list current_block_verifiers_list; // The list of block verifiers name, public address and IP address for the current round
struct next_block_verifiers_list next_block_verifiers_list; // The list of block verifiers name, public address and IP address for the next round
struct synced_block_verifiers synced_block_verifiers; // The list of block verifiers for syncing the databases
struct main_nodes_list main_nodes_list; // The list of main nodes public address and IP address
struct network_data_nodes_list network_data_nodes_list; // The network data nodes
struct current_round_part_vote_data current_round_part_vote_data; // The vote data for the current part of the round
struct VRF_data VRF_data; // The list of all of the VRF data to send to the block producer.
struct blockchain_data blockchain_data; // The data for a new block to be added to the network.
struct error_message error_message; // holds all of the error messages and the functions for an error.
struct invalid_reserve_proofs invalid_reserve_proofs; // The invalid reserve proofs that the block verifier finds every round
char current_round_part[2]; // The current round part (1-4)
char current_round_part_backup_node[2]; // The current main node in the current round part (0-5)
pthread_rwlock_t rwlock;
pthread_rwlock_t rwlock_reserve_proofs;

pthread_t server_threads[100];
int epoll_fd;
struct epoll_event events_copy;
int server_socket;

char current_block_height[1000]; // The current block height
int error_message_count; // The error message count
int main_network_data_node_create_block; // 1 if the main network data node can create a block, 0 if not
int current_block_verifier_settings; // 1 if the delegate is a block verifier, 0 if not
int database_settings; // 1 if the database can have documents added to it, 0 if not

int delegates_website; // 1 if the running the delegates websites, 0 if not
int shared_delegates_website; // 1 if the running the shared delegates websites, 0 if not
int total_threads; // The total threads
double fee; // the fee
long long int minimum_amount; // the minimum amount to send a payment

#endif