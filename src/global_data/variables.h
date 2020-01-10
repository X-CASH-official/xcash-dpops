#ifndef VARIABLES_H_   /* Include guard */
#define VARIABLES_H_

#include <sys/epoll.h>
#include <pthread.h>

#include "structures.h"
#include <mongoc/mongoc.h>
#include <bson/bson.h>

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
char xcash_wallet_public_address[XCASH_WALLET_LENGTH+1]; // Holds your wallets public address
unsigned char secret_key_data[crypto_vrf_SECRETKEYBYTES+1]; // Holds the secret key for signing block verifier messages
char secret_key[VRF_SECRET_KEY_LENGTH+1]; // Holds the secret key text for signing block verifier messages
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
pthread_mutex_t lock;
pthread_mutex_t database_lock;
pthread_cond_t thread_settings_lock;

pthread_t server_threads[100];
int epoll_fd;
struct epoll_event events_copy;
int server_socket;

char network_data_nodes_database_data[NETWORK_DATA_NODES_AMOUNT][DATA_HASH_LENGTH+1];
char current_block_height[BUFFER_SIZE_NETWORK_BLOCK_DATA]; // The current block height
char previous_block_hash[BLOCK_HASH_LENGTH+1]; // The current block height
int error_message_count; // The error message count
int main_network_data_node_create_block; // 1 if the main network data node can create a block, 0 if not
int database_settings; // 1 if the database can have documents added to it, 0 if not
int log_file_settings; // 0 to use the terminal, 1 to use a log file, 2 to use a log file with color output
char log_file[BUFFER_SIZE_NETWORK_BLOCK_DATA]; // The log file
char XCASH_DPOPS_delegates_IP_address[BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH]; // The  block verifiers IP address to run the server on
int xcash_wallet_port; // The xcash wallet port
char database_name[BUFFER_SIZE_NETWORK_BLOCK_DATA];
char shared_delegates_database_name[BUFFER_SIZE_NETWORK_BLOCK_DATA];
int network_functions_test_settings;
int network_functions_test_error_settings; // 1 to display errors, 0 to not display errors when running the reset variables allocated on the heap test
int network_functions_test_server_messages_settings; // 1 to display server messages, 0 to not display server messages when running the test

int delegates_website; // 1 if the running the delegates websites, 0 if not
int shared_delegates_website; // 1 if the running the shared delegates websites, 0 if not
int total_threads; // The total threads
double fee; // the fee
long long int minimum_amount; // the minimum amount to send a payment
char voter_inactivity_count[10]; // the number of days to wait to remove an inactive delegates information from the database

#endif