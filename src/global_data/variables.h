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
extern mongoc_client_pool_t* database_client_thread_pool;

// network data nodes
extern int network_data_node_settings; // 1 if a network data node, 0 if not a network data node 
extern char xcash_wallet_public_address[XCASH_WALLET_LENGTH+1]; // Holds your wallets public address
extern unsigned char secret_key_data[crypto_vrf_SECRETKEYBYTES+1]; // Holds the secret key for signing block verifier messages
extern char secret_key[VRF_SECRET_KEY_LENGTH+1]; // Holds the secret key text for signing block verifier messages
extern struct previous_block_verifiers_list previous_block_verifiers_list; // The list of block verifiers name, public address and IP address for the previous round
extern struct current_block_verifiers_list current_block_verifiers_list; // The list of block verifiers name, public address and IP address for the current round
extern struct next_block_verifiers_list next_block_verifiers_list; // The list of block verifiers name, public address and IP address for the next round
extern struct synced_block_verifiers synced_block_verifiers; // The list of block verifiers for syncing the databases
extern struct main_nodes_list main_nodes_list; // The list of main nodes public address and IP address
extern struct network_data_nodes_list network_data_nodes_list; // The network data nodes
extern struct current_round_part_vote_data current_round_part_vote_data; // The vote data for the current part of the round
extern struct current_block_verifiers_majority_vote current_block_verifiers_majority_vote; // The vote majority data for the current part of the round
extern struct VRF_data VRF_data; // The list of all of the VRF data to send to the block producer.
extern struct blockchain_data blockchain_data; // The data for a new block to be added to the network.
extern struct error_message error_message; // holds all of the error messages and the functions for an error.
extern struct invalid_reserve_proofs invalid_reserve_proofs; // The invalid reserve proofs that the block verifier finds every round
extern struct network_data_nodes_sync_database_list network_data_nodes_sync_database_list; // Holds the network data nodes data and database hash for syncing network data nodes
extern struct block_verifiers_sync_database_list block_verifiers_sync_database_list; // Holds the block verifiers data and database hash for syncing the block verifiers
extern struct delegates_online_status delegates_online_status[MAXIMUM_AMOUNT_OF_DELEGATES]; // Holds the delegates online status
extern struct block_height_start_time block_height_start_time; // Holds the block height start time data
extern struct private_group private_group; // Holds the private group data
extern char current_round_part[2]; // The current round part (1-4)
extern char current_round_part_backup_node[2]; // The current main node in the current round part (0-5)
extern pthread_rwlock_t rwlock;
extern pthread_rwlock_t rwlock_reserve_proofs;
extern pthread_mutex_t lock;
extern pthread_mutex_t database_lock;
extern pthread_mutex_t verify_network_block_lock;
extern pthread_mutex_t vote_lock;
extern pthread_mutex_t add_reserve_proof_lock;
extern pthread_mutex_t invalid_reserve_proof_lock;
extern pthread_mutex_t database_data_IP_address_lock;
extern pthread_mutex_t update_current_block_height_lock;

extern pthread_t server_threads[100];
extern int epoll_fd;
extern int server_socket;

extern char current_block_height[BUFFER_SIZE_NETWORK_BLOCK_DATA]; // The current block height
extern char previous_block_hash[BLOCK_HASH_LENGTH+1]; // The current block height
extern int error_message_count; // The error message count
extern int main_network_data_node_create_block; // 1 if the main network data node can create a block, 0 if not
extern int main_network_data_node_receive_block; // 1 if you have received the block from the main network data node, 0 if not
extern int network_data_node_valid_amount; // The amount of network data nodes that were valid
extern int log_file_settings; // 0 to use the terminal, 1 to use a log file, 2 to use a log file with color output
extern char log_file[BUFFER_SIZE_NETWORK_BLOCK_DATA]; // The log file
extern char XCASH_DPOPS_delegates_IP_address[BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH]; // The  block verifiers IP address to run the server on
extern int xcash_wallet_port; // The xcash wallet port
extern char database_name[BUFFER_SIZE_NETWORK_BLOCK_DATA];
extern char shared_delegates_database_name[BUFFER_SIZE_NETWORK_BLOCK_DATA];
extern char remote_data_database_name[BUFFER_SIZE_NETWORK_BLOCK_DATA];
extern char database_path_write[1024]; // holds the database write path
extern char database_path_write_before_majority[1024]; // holds the database write path before the majority sync
extern char database_path_read[1024]; // holds the database read path
extern int network_functions_test_settings;
extern int network_functions_test_error_settings; // 1 to display errors, 0 to not display errors when running the reset variables allocated on the heap test
extern int network_functions_test_server_messages_settings; // 1 to display server messages, 0 to not display server messages when running the test
extern int test_settings; // 1 when the test are running, 0 if not
extern int debug_settings; // 1 to show all incoming and outgoing message from the server
extern int registration_settings; // 1 when the registration mode is running, 0 when it is not
extern int synced_network_data_nodes[BLOCK_VERIFIERS_AMOUNT]; // the synced network data nodes
extern int synced_block_verifiers_nodes[DATABASE_TOTAL][BLOCK_VERIFIERS_AMOUNT]; // the synced block verifiers nodes
extern size_t block_verifiers_current_block_height[BLOCK_VERIFIERS_AMOUNT]; // holds the block verifiers current block heights
extern int production_settings; // 1 for production, 0 for test
extern int production_settings_database_data_settings; // The initialize the database settings
extern char website_path[1024]; // holds the path to the website if running a delegates explorer or shared delegates pool
extern int sync_previous_current_next_block_verifiers_settings; // sync the previous, current and next block verifiers if you had to restart
extern int database_data_socket_settings; // 1 to allow database data up to 50MB to be received in the server, 0 to only allow message up to BUFFER_SIZE
extern char* server_limit_IP_address_list; // holds all of the IP addresses that are currently running on the server. This can hold up to 1 million IP addresses
extern char* server_limit_public_address_list; // holds all of the public addresses that are currently running on the server. This can hold up to 1 million public addresses
extern int invalid_block_verifiers_count; // counts how many times your node did not receive the block from the main network backup node, to indicate if your node is not syncing
extern int backup_network_data_node_settings; // The network data node that will attempt to create the block if the block producer and backup block producer fail
extern int replayed_round_settings; // 1 if the round is a replayed round, 0 if not
extern char delegates_error_list[(MAXIMUM_BUFFER_SIZE_DELEGATES_NAME * 100) + 5000]; // Holds the list of delegates that did not complete a part of the round
extern int delegates_error_list_settings; // 1 if showing the delegates that error, 0 if not
extern char* turbo_tx_list[BLOCK_VERIFIERS_AMOUNT]; // The tx list for each block verifier

extern int delegates_website; // 1 if the running the delegates websites, 0 if not
extern int shared_delegates_website; // 1 if the running the shared delegates websites, 0 if not
extern int total_threads; // The total threads
extern double fee; // the fee
extern long long int minimum_amount; // the minimum amount to send a payment
extern char voter_inactivity_count[10]; // the number of days to wait to remove an inactive delegates information from the database

#endif
