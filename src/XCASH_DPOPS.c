#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <pthread.h>
#include <sys/sysinfo.h>
#include <sys/resource.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "shared_delegate_website_thread_server_functions.h"

#include "block_verifiers_synchronize_functions.h"
#include "block_verifiers_synchronize_check_functions.h"
#include "block_verifiers_thread_server_functions.h"
#include "block_verifiers_update_functions.h"
#include "database_functions.h"
#include "insert_database_functions.h"
#include "read_database_functions.h"
#include "delete_database_functions.h"
#include "network_daemon_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "network_wallet_functions.h"
#include "server_functions.h"
#include "string_functions.h"
#include "VRF_functions.h"

#include "XCASH_DPOPS_test.h"
#include "XCASH_DPOPS.h"

/*
-----------------------------------------------------------------------------------------------------------
Global Variables
-----------------------------------------------------------------------------------------------------------
*/

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
pthread_mutex_t verify_network_block_lock;
pthread_mutex_t vote_lock;
pthread_cond_t thread_settings_lock;
pthread_mutex_t add_reserve_proof_lock;

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
int test_settings; // 1 when the test are running, 0 if not
int vrf_data_verify_count; // holds the amount of block verifiers signatures that are verified for the current network block
int debug_settings; // 1 to show all incoming and outgoing message from the server

int delegates_website; // 1 if the running the delegates websites, 0 if not
int shared_delegates_website; // 1 if the running the shared delegates websites, 0 if not
int total_threads; // The total threads
double fee; // the fee
long long int minimum_amount; // the minimum amount to send a payment
char voter_inactivity_count[10]; // the number of days to wait to remove an inactive delegates information from the database
int shared_delegate_payment_time_hour; // the hour for the shared delegate to send the payments
int shared_delegate_payment_time_minute; // the minute for the shared delegate to send the payments



/*
-----------------------------------------------------------------------------------------------------------
Global Variables for this file
-----------------------------------------------------------------------------------------------------------
*/

mongoc_uri_t* uri_thread_pool;
bson_error_t error;



/*
-----------------------------------------------------------------------------------------------------------
Name: initialize_data
Description: Initializes the global variables
-----------------------------------------------------------------------------------------------------------
*/

void initialize_data(void)
{
  // Variables
  char data[BUFFER_SIZE];
  size_t count = 0;
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;

  memset(data,0,sizeof(data));

  // initialize the global variables
  memset(current_block_height,0,sizeof(current_block_height));
  memset(current_round_part,0,sizeof(current_round_part));
  memset(current_round_part_backup_node,0,sizeof(current_round_part_backup_node));
  memset(secret_key,0,sizeof(secret_key));
  memset(secret_key_data,0,sizeof(secret_key_data));
  memset(log_file,0,sizeof(log_file));
  memset(XCASH_DPOPS_delegates_IP_address,0,sizeof(XCASH_DPOPS_delegates_IP_address));
  memset(database_name,0,sizeof(database_name));
  memset(shared_delegates_database_name,0,sizeof(shared_delegates_database_name));
  memset(voter_inactivity_count,0,sizeof(voter_inactivity_count));
  database_settings = 1;
  log_file_settings = 0;
  xcash_wallet_port = XCASH_WALLET_PORT;
  shared_delegate_payment_time_hour = (rand() % (23 - 0 + 1)) + 0;
  shared_delegate_payment_time_minute = (rand() % (59 - 0 + 1)) + 0;
  network_functions_test_settings = 0;
  network_functions_test_error_settings = 1;
  network_functions_test_server_messages_settings = 1;
  debug_settings = 0;

  pthread_rwlock_init(&rwlock,NULL);
  pthread_rwlock_init(&rwlock_reserve_proofs,NULL);
  pthread_mutex_init(&lock, NULL);
  pthread_mutex_init(&database_lock, NULL);
  pthread_mutex_init(&verify_network_block_lock, NULL);
  pthread_mutex_init(&vote_lock, NULL);
  pthread_cond_init(&thread_settings_lock,NULL);
  pthread_mutex_init(&add_reserve_proof_lock, NULL);

  // initialize the error_message struct
  for (count = 0; count < TOTAL_ERROR_MESSAGES; count++)
  {
    error_message.function[count] = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
    error_message.data[count] = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  }
  error_message.total = 0;

  for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++)
  {
    // initialize the previous, current and next block_verifiers_list struct 
    memset(previous_block_verifiers_list.block_verifiers_name[count],0,sizeof(previous_block_verifiers_list.block_verifiers_name[count]));
    memset(previous_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_public_address[count]));
    memset(previous_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(previous_block_verifiers_list.block_verifiers_public_key[count]));
    memset(previous_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_IP_address[count]));

    memset(current_block_verifiers_list.block_verifiers_name[count],0,sizeof(current_block_verifiers_list.block_verifiers_name[count]));
    memset(current_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_public_address[count]));
    memset(current_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(current_block_verifiers_list.block_verifiers_public_key[count]));
    memset(current_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_IP_address[count]));

    memset(next_block_verifiers_list.block_verifiers_name[count],0,sizeof(next_block_verifiers_list.block_verifiers_name[count]));
    memset(next_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_public_address[count]));
    memset(next_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(next_block_verifiers_list.block_verifiers_public_key[count]));
    memset(next_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_IP_address[count]));

    // initialize the synced_block_verifiers struct 
    memset(synced_block_verifiers.synced_block_verifiers_public_address[count],0,sizeof(synced_block_verifiers.synced_block_verifiers_public_address[count]));
    memset(synced_block_verifiers.synced_block_verifiers_public_key[count],0,sizeof(synced_block_verifiers.synced_block_verifiers_public_key[count]));
    memset(synced_block_verifiers.synced_block_verifiers_IP_address[count],0,sizeof(synced_block_verifiers.synced_block_verifiers_IP_address[count]));
    memset(synced_block_verifiers.vote_settings[count],0,sizeof(synced_block_verifiers.vote_settings[count]));
  }
  synced_block_verifiers.vote_settings_true = 0;
  synced_block_verifiers.vote_settings_false = 0; 

  // initialize the main_nodes_list struct 
  memset(main_nodes_list.block_producer_public_address,0,sizeof(main_nodes_list.block_producer_public_address));
  memset(main_nodes_list.block_producer_IP_address,0,sizeof(main_nodes_list.block_producer_IP_address));
  memset(main_nodes_list.block_producer_backup_block_verifier_1_public_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_1_public_address));
  memset(main_nodes_list.block_producer_backup_block_verifier_1_IP_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_1_IP_address));
  memset(main_nodes_list.block_producer_backup_block_verifier_2_public_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_2_public_address));
  memset(main_nodes_list.block_producer_backup_block_verifier_2_IP_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_2_IP_address));
  memset(main_nodes_list.block_producer_backup_block_verifier_3_public_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_3_public_address));
  memset(main_nodes_list.block_producer_backup_block_verifier_3_IP_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_3_IP_address));
  memset(main_nodes_list.block_producer_backup_block_verifier_4_public_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_4_public_address));
  memset(main_nodes_list.block_producer_backup_block_verifier_4_IP_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_4_IP_address));
  memset(main_nodes_list.block_producer_backup_block_verifier_5_public_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_5_public_address));
  memset(main_nodes_list.block_producer_backup_block_verifier_5_IP_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_5_IP_address));

  // initialize the network_data_nodes_list struct
  for (count = 0; count < NETWORK_DATA_NODES_AMOUNT; count++)
  {
    memset(network_data_nodes_list.network_data_nodes_public_address[count],0,sizeof(network_data_nodes_list.network_data_nodes_public_address[count]));
    memset(network_data_nodes_list.network_data_nodes_IP_address[count],0,sizeof(network_data_nodes_list.network_data_nodes_IP_address[count]));
  }

  // initialize the network data nodes
  INITIALIZE_NETWORK_DATA_NODES;

  // set the network_data_node_settings
  network_data_node_settings = 0;

  // initialize the current_round_part_vote_data struct
  memset(current_round_part_vote_data.current_vote_results,0,sizeof(current_round_part_vote_data.current_vote_results));

  // initialize the VRF_data struct 
  VRF_data.vrf_secret_key_data_round_part_4 = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  VRF_data.vrf_secret_key_round_part_4 = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(unsigned char));
  VRF_data.vrf_public_key_data_round_part_4 = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  VRF_data.vrf_public_key_round_part_4 = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(unsigned char));
  VRF_data.vrf_alpha_string_data_round_part_4 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  VRF_data.vrf_alpha_string_round_part_4 = (unsigned char*)calloc(BUFFER_SIZE,sizeof(unsigned char));
  VRF_data.vrf_proof_data_round_part_4 = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  VRF_data.vrf_proof_round_part_4 = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(unsigned char));
  VRF_data.vrf_beta_string_data_round_part_4 = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  VRF_data.vrf_beta_string_round_part_4 = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(unsigned char));
  VRF_data.block_blob = (char*)calloc(BUFFER_SIZE,sizeof(char));
  VRF_data.reserve_bytes_data_hash = (char*)calloc(DATA_HASH_LENGTH+1,sizeof(char));

  // check if the memory needed was allocated on the heap successfully
  if (VRF_data.vrf_public_key_data_round_part_4 == NULL || VRF_data.vrf_public_key_round_part_4 == NULL || VRF_data.vrf_alpha_string_data_round_part_4 == NULL || VRF_data.vrf_alpha_string_round_part_4 == NULL || VRF_data.vrf_proof_data_round_part_4 == NULL || VRF_data.vrf_proof_round_part_4 == NULL || VRF_data.vrf_beta_string_data_round_part_4 == NULL || VRF_data.vrf_beta_string_round_part_4 == NULL || VRF_data.block_blob == NULL)
  {
    memcpy(error_message.function[error_message.total],"main",4);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,data);  
    exit(0);
  }

  for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++)
  {
    VRF_data.block_verifiers_vrf_secret_key_data[count] = (char*)calloc(VRF_SECRET_KEY_LENGTH+1,sizeof(char));
    VRF_data.block_verifiers_vrf_secret_key[count] = (unsigned char*)calloc(crypto_vrf_SECRETKEYBYTES+1,sizeof(unsigned char));
    VRF_data.block_verifiers_vrf_public_key_data[count] = (char*)calloc(VRF_PUBLIC_KEY_LENGTH+1,sizeof(char));
    VRF_data.block_verifiers_vrf_public_key[count] = (unsigned char*)calloc(crypto_vrf_PUBLICKEYBYTES+1,sizeof(unsigned char));
    VRF_data.block_verifiers_random_data[count] = (char*)calloc(RANDOM_STRING_LENGTH+1,sizeof(char));
    VRF_data.block_blob_signature[count] = (char*)calloc(VRF_PROOF_LENGTH+VRF_BETA_LENGTH+1,sizeof(char));
   
    // check if the memory needed was allocated on the heap successfully
    if (VRF_data.block_blob_signature[count] == NULL || VRF_data.block_verifiers_random_data[count] == NULL)
    {
      memcpy(error_message.function[error_message.total],"main",4);
      memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
      error_message.total++;
      print_error_message(current_date_and_time,current_UTC_date_and_time,data);  
      exit(0);
    }
  }

  // initialize the blockchain_data struct 
  blockchain_data.network_version_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.timestamp_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.previous_block_hash_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.nonce_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.block_reward_transaction_version_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.unlock_block_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.block_reward_input_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.vin_type_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.block_height_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.block_reward_output_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.block_reward_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.stealth_address_output_tag_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.stealth_address_output_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.extra_bytes_size_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.transaction_public_key_tag_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.transaction_public_key_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.extra_nonce_tag_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.reserve_bytes_size_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.block_producer_public_address = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));  
  blockchain_data.blockchain_reserve_bytes.vrf_secret_key_data_round_part_4 = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_secret_key_round_part_4 = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(unsigned char));
  blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_4 = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_4 = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(unsigned char));
  blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_4 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_4 = (unsigned char*)calloc(BUFFER_SIZE,sizeof(unsigned char));
  blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_4 = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_4 = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(unsigned char));
  blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_4 = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_4 = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(unsigned char));
  blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_4 = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.previous_block_hash_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));

  // check if the memory needed was allocated on the heap successfully
  if (blockchain_data.network_version_data == NULL || blockchain_data.timestamp_data == NULL || blockchain_data.previous_block_hash_data == NULL || blockchain_data.nonce_data == NULL || blockchain_data.block_reward_transaction_version_data == NULL || blockchain_data.unlock_block_data == NULL || blockchain_data.block_reward_input_data == NULL || blockchain_data.vin_type_data == NULL || blockchain_data.block_height_data == NULL || blockchain_data.block_reward_output_data == NULL || blockchain_data.block_reward_data == NULL || blockchain_data.stealth_address_output_tag_data == NULL || blockchain_data.stealth_address_output_data == NULL || blockchain_data.extra_bytes_size_data == NULL || blockchain_data.transaction_public_key_tag_data == NULL || blockchain_data.transaction_public_key_data == NULL || blockchain_data.extra_nonce_tag_data == NULL || blockchain_data.reserve_bytes_size_data == NULL || blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data == NULL || blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name == NULL || blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data == NULL || blockchain_data.blockchain_reserve_bytes.block_producer_public_address == NULL || blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data == NULL || blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data == NULL || blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names == NULL || blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_4 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_4 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_4 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_4 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_4 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_4 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_4 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_4 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_4 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_data == NULL || blockchain_data.blockchain_reserve_bytes.previous_block_hash_data == NULL)
  {
    memcpy(error_message.function[error_message.total],"main",4);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,data);  
    exit(0);
  }
  for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++)
  {
    blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data[count] = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
    blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address[count] = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
    blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count] = (char*)calloc(1000,sizeof(char));
    blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count] = (char*)calloc(1000,sizeof(char));
    blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key_data[count] = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
    blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key[count] = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(unsigned char));
    blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key_data[count] = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
    blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key[count] = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(unsigned char));
    blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data[count] = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
    blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data_text[count] = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));

    // check if the memory needed was allocated on the heap successfully
    if (blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data[count] == NULL || blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address[count] == NULL || blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count] == NULL || blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count] == NULL || blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key_data[count] == NULL || blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key[count] == NULL || blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key_data[count] == NULL || blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key[count] == NULL)
    {
      memcpy(error_message.function[error_message.total],"main",4);
      memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
      error_message.total++;
      print_error_message(current_date_and_time,current_UTC_date_and_time,data);  
      exit(0);
    }
  }
  blockchain_data.ringct_version_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.transaction_amount_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  for (count = 0; count < MAXIMUM_TRANSACATIONS_PER_BLOCK; count++)
  {
    blockchain_data.transactions[count] = (char*)calloc(TRANSACTION_HASH_LENGTH+1,sizeof(char));

    // check if the memory needed was allocated on the heap successfully
    if (blockchain_data.transactions[count] == NULL)
    {
      memcpy(error_message.function[error_message.total],"main",4);
      memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
      error_message.total++;
      print_error_message(current_date_and_time,current_UTC_date_and_time,data);  
      exit(0);
    }
  }

  /*// initialize the invalid_reserve_proofs and the block_verifiers_invalid_reserve_proofs
  for (count = 0; count < MAXIMUM_INVALID_RESERVE_PROOFS; count++)
  {
    invalid_reserve_proofs.block_verifier_public_address[count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    invalid_reserve_proofs.public_address_created_reserve_proof[count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    invalid_reserve_proofs.public_address_voted_for[count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    invalid_reserve_proofs.reserve_proof[count] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char)); 
  }*/
  invalid_reserve_proofs.count = 0;
  return;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: create_overall_database_connection
Description: Create a database connection
-----------------------------------------------------------------------------------------------------------
*/

void create_overall_database_connection(void)
{
  // Variables
  char data[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;

  memset(data,0,sizeof(data));

  // initialize the database connection
  mongoc_init();

  // create a connection to the database
  if (create_database_connection() == 0)
  {
    memcpy(error_message.function[error_message.total],"main",4);
    memcpy(error_message.data[error_message.total],"Could not create a connection for the database",46);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,data); 
    mongoc_cleanup();
    exit(0);
  }

  // create a pool of connections for the database
  uri_thread_pool = mongoc_uri_new_with_error(DATABASE_CONNECTION, &error);
  if (!uri_thread_pool)
  {
    memcpy(error_message.function[error_message.total],"main",4);
    memcpy(error_message.data[error_message.total],"Could not create a pool of connections for the database",55);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,data);
    mongoc_client_destroy(database_client);
    mongoc_cleanup();
    exit(0);
  }
  database_client_thread_pool = mongoc_client_pool_new(uri_thread_pool);
  if (!database_client_thread_pool)
  {
    memcpy(error_message.function[error_message.total],"main",4);
    memcpy(error_message.data[error_message.total],"Could not create a thread pool for the database",47);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,data); 
    mongoc_client_destroy(database_client);
    mongoc_uri_destroy(uri_thread_pool);
    mongoc_cleanup();
    exit(0);
  }
  return;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_delegates_data
Description: Gets the delegates data
-----------------------------------------------------------------------------------------------------------
*/

void get_delegates_data(void)
{
  // Variables
  char data[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;

  // define macros
  #define GET_DELEGATES_DATA_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"get_delegates_data",18); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  print_error_message(current_date_and_time,current_UTC_date_and_time,data); \
  mongoc_client_destroy(database_client); \
  mongoc_client_pool_destroy(database_client_thread_pool); \
  mongoc_uri_destroy(uri_thread_pool); \
  mongoc_cleanup(); \
  exit(0);

  memset(data,0,sizeof(data));

  // get the wallets public address
  if (get_public_address() == 0)
  { 
    GET_DELEGATES_DATA_ERROR("Could not get the wallets public address");
  }

  // get the current block height
  if (get_current_block_height(current_block_height) == 0)
  {
    GET_DELEGATES_DATA_ERROR("Could not get the current block height");
  }

  // wait until the blockchain is fully synced
  if (check_if_blockchain_is_fully_synced() == 0)
  {
    color_print("The blockchain is not fully synced.\nWaiting until it is fully synced to continue","yellow");  
    do
    {
      sleep(60);
    } while (check_if_blockchain_is_fully_synced() == 0);
  }

  // get the previous block hash
  if (get_previous_block_hash(previous_block_hash) == 0)
  {
    GET_DELEGATES_DATA_ERROR("Could not get the previous block hash");
  }
  return;

  #undef GET_DELEGATES_DATA_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: set_parameters
Description: Sets the parameters
Parameters:
  parameters_count - The parameter count
  parameters - The parameters
Return: 0 if an error has occured, 1 if successfull, 2 to disable the timers
-----------------------------------------------------------------------------------------------------------
*/

int set_parameters(int parameters_count, char* parameters[])
{
  // define macros
  #define database_reset \
  mongoc_client_destroy(database_client); \
  mongoc_client_pool_destroy(database_client_thread_pool); \
  mongoc_uri_destroy(uri_thread_pool); \
  mongoc_cleanup();

  #define SET_PARAMETERS_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"set_parameters",14); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  print_error_message(current_date_and_time,current_UTC_date_and_time,data); \
  database_reset; \
  exit(0);

  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  size_t count = 0;
  size_t count2 = 0;
  size_t count3 = 0;
  size_t counter = 0;
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // set the default parameter settings
  total_threads = get_nprocs();
  delegates_website = 0;
  shared_delegates_website = 0;
  log_file_settings = 0;
  memcpy(database_name,DATABASE_NAME,sizeof(DATABASE_NAME)-1);
  memcpy(shared_delegates_database_name,DATABASE_NAME_DELEGATES,sizeof(DATABASE_NAME_DELEGATES)-1);
  database_settings = 1;
  log_file_settings = 0;
  test_settings = 0;
  memcpy(voter_inactivity_count,VOTER_INACTIVITY_COUNT,sizeof(VOTER_INACTIVITY_COUNT)-1);
  xcash_wallet_port = XCASH_WALLET_PORT;

  // set the current_round_part, current_round_part_backup_node and server message, this way the node will start at the begining of a round
  memset(current_round_part,0,sizeof(current_round_part));
  memset(current_round_part_backup_node,0,sizeof(current_round_part_backup_node));
  memcpy(current_round_part,"1",1);
  memcpy(current_round_part_backup_node,"0",1);

  // check all of the parameters to see if there is a block verifier secret key
  if (parameters_count < 3)
  {
    SET_PARAMETERS_ERROR("Could not get the block verifiers secret key.\nMake sure to run XCASH_DPOPS with the --block_verifier_secret_key parameter");
  }
  
  // check the parameters
  for (count = 0, count2 = 0; count < (size_t)parameters_count; count++)
  { 
    if (strncmp(parameters[count],"--block_verifiers_secret_key",BUFFER_SIZE) == 0)
    {
      count2 = 1;
    }
  }

  if (count2 != 1)
  {
    SET_PARAMETERS_ERROR("Could not get the block verifiers secret key.\nMake sure to run XCASH_DPOPS with the --block_verifier_secret_key parameter");
  }

  // check the parameters
  for (count = 0, count2 = 0; count < (size_t)parameters_count; count++)
  { 
    if (strncmp(parameters[count],"--block_verifiers_secret_key",BUFFER_SIZE) == 0)
    {
      if (strlen(parameters[count+1]) != VRF_SECRET_KEY_LENGTH)
      {
        SET_PARAMETERS_ERROR("Invalid block verifiers secret key");
      }

      // get the secret key for signing messages
      memcpy(secret_key,parameters[count+1],VRF_SECRET_KEY_LENGTH);
    
      // convert the hexadecimal string to a string
      for (count3 = 0, counter = 0; count3 < VRF_SECRET_KEY_LENGTH; counter++, count3 += 2)
      {
        memset(data2,0,sizeof(data2));
        memcpy(data2,&secret_key[count3],2);
        secret_key_data[counter] = (int)strtol(data2, NULL, 16);
      }
    }
    if (strncmp(parameters[count],"--test",BUFFER_SIZE) == 0)
    {
      test(0);
      database_reset;
      exit(0);
    }
    if (strncmp(parameters[count],"--quick_test",BUFFER_SIZE) == 0)
    {
      test(1);
      database_reset;
      exit(0);
    }
    if (strncmp(parameters[count],"--optimizations_test",BUFFER_SIZE) == 0)
    {
      test(2);
      database_reset;
      exit(0);
    }
    if (strncmp(parameters[count],"--debug",BUFFER_SIZE) == 0)
    {
      debug_settings = 1;
    }
    if (strncmp(parameters[count],"--XCASH_DPOPS_delegates_IP_address",BUFFER_SIZE) == 0 && count != (size_t)parameters_count)
    {
      memcpy(XCASH_DPOPS_delegates_IP_address,parameters[count+1],strnlen(parameters[count+1],sizeof(XCASH_DPOPS_delegates_IP_address)));
    }
    if (strncmp(parameters[count],"--xcash_wallet_port",BUFFER_SIZE) == 0 && count != (size_t)parameters_count)
    {
      sscanf(parameters[count+1],"%d",&xcash_wallet_port);
    }
    if (strncmp(parameters[count],"--database_name",BUFFER_SIZE) == 0 && count != (size_t)parameters_count)
    {
      memset(database_name,0,sizeof(database_name));
      memcpy(database_name,parameters[count+1],strnlen(parameters[count+1],sizeof(database_name)));
    }
    if (strncmp(parameters[count],"--shared_delegates_database_name",BUFFER_SIZE) == 0 && count != (size_t)parameters_count)
    {
      memset(shared_delegates_database_name,0,sizeof(shared_delegates_database_name));
      memcpy(shared_delegates_database_name,parameters[count+1],strnlen(parameters[count+1],sizeof(shared_delegates_database_name)));
    }
    if (strncmp(parameters[count],"--log_file",BUFFER_SIZE) == 0 && count != (size_t)parameters_count)
    {
      log_file_settings = 1;
      memcpy(log_file,parameters[count+1],strnlen(parameters[count+1],sizeof(log_file)));

      memset(data,0,sizeof(data));
      color_print("\n\n\n\n\nXCASH DPOPS - Version 1.0.0\n","green");
      memcpy(data,"Successfully received the public address:",41);
      memcpy(data+41,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      color_print(data,"green");
    }
    if (strncmp(parameters[count],"--log_file_color",BUFFER_SIZE) == 0 && count != (size_t)parameters_count)
    {
      log_file_settings = 2;
      memcpy(log_file,parameters[count+1],strnlen(parameters[count+1],sizeof(log_file)));

      memset(data,0,sizeof(data));
      color_print("\n\n\n\n\nXCASH DPOPS - Version 1.0.0\n","green");
      memcpy(data,"Successfully received the public address:",41);
      memcpy(data+41,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      color_print(data,"green");
    }
    if (strncmp(parameters[count],"--synchronize_database_from_network_data_node",BUFFER_SIZE) == 0)
    {
      check_if_databases_are_synced(2,0);
      database_reset;
      exit(0);
    }
    if (strncmp(parameters[count],"--synchronize_database_from_specific_delegate",BUFFER_SIZE) == 0 && count != (size_t)parameters_count)
    {
      color_print("Syncing the block verifiers list","yellow");
      sync_all_block_verifiers_list();
      color_print("Syncing the reserve bytes database","yellow");
      sync_reserve_bytes_database(0,0,parameters[count+1]);
      color_print("Syncing the reserve proofs database","yellow");
      sync_reserve_proofs_database(0,parameters[count+1]);
      color_print("Syncing the delegates database","yellow");
      sync_delegates_database(0,parameters[count+1]);
      color_print("Syncing the statistics database","yellow");
      sync_statistics_database(0,parameters[count+1]);
      color_print("Successfully synced all databases","yellow");
      database_reset;
      exit(0);
    }
    if (strncmp(parameters[count],"--test_data_add",BUFFER_SIZE) == 0)
    {
      memset(data,0,sizeof(data));
      
      insert_document_into_collection_json(shared_delegates_database_name,"blocks_found","{\"block_height\":\"5\",\"block_hash\":\"BLOCK_HASH\",\"block_date_and_time\":\"10\",\"block_reward\":\"15\"}",0);
      insert_document_into_collection_json(shared_delegates_database_name,"blocks_found","{\"block_height\":\"5\",\"block_hash\":\"BLOCK_HASH\",\"block_date_and_time\":\"10\",\"block_reward\":\"15\"}",0);

      memset(data,0,strlen(data));
      memcpy(data,"{\"public_address\":\"",19);
      memcpy(data+19,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      memcpy(data+117,"\",\"current_total\":\"5\",\"total\":\"10\",\"inactivity_count\":\"15\"}",59);
      insert_document_into_collection_json(shared_delegates_database_name,"public_addresses",data,0);

      memset(data,0,strlen(data));
      memcpy(data,"{\"public_address\":\"",19);
      memcpy(data+19,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      memcpy(data+117,"\",\"date_and_time\":\"5\",\"total\":\"10\",\"tx_hash\":\"TX_HASH\",\"tx_key\":\"TX_KEY\"}",73);
      insert_document_into_collection_json(shared_delegates_database_name,"public_addresses_payments",data,0);
      insert_document_into_collection_json(shared_delegates_database_name,"public_addresses_payments",data,0);

      memset(data,0,strlen(data));
      memcpy(data,"{\"public_address_created_reserve_proof\":\"",41);
      memcpy(data+41,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      memcpy(data+139,"\",\"public_address_voted_for\":\"",30);
      memcpy(data+169,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      memcpy(data+267,"\",\"total\":\"10\",\"reserve_proof\":\"15\"}",36);
      insert_document_into_collection_json(database_name,"reserve_proofs_5",data,0);
      insert_document_into_collection_json(database_name,"reserve_proofs_10",data,0);
      insert_document_into_collection_json(database_name,"reserve_proofs_10",data,0);
      insert_document_into_collection_json(database_name,"reserve_proofs_15",data,0);

      color_print("The database test data has been added successfully\n","green");
      database_reset;
      exit(0);
    }
    if (strncmp(parameters[count],"--test_data_remove",BUFFER_SIZE) == 0)
    {
      memset(data,0,strlen(data));
      
      delete_document_from_collection(shared_delegates_database_name,"blocks_found","{\"block_height\":\"5\"}",0);
      delete_document_from_collection(shared_delegates_database_name,"blocks_found","{\"block_height\":\"5\"}",0);

      memset(data,0,strlen(data));
      memcpy(data,"{\"public_address\":\"",19);
      memcpy(data+19,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      memcpy(data+117,"\"}",2);
      delete_document_from_collection(shared_delegates_database_name,"public_addresses",data,0);

      memset(data,0,strlen(data));
      memcpy(data,"{\"public_address\":\"",19);
      memcpy(data+19,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      memcpy(data+117,"\"}",2);
      delete_document_from_collection(shared_delegates_database_name,"public_addresses_payments",data,0);
      delete_document_from_collection(shared_delegates_database_name,"public_addresses_payments",data,0);

      memset(data,0,strlen(data));
      memcpy(data,"{\"public_address_voted_for\":\"",29);
      memcpy(data+29,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      memcpy(data+127,"\"}",2);
      delete_document_from_collection(database_name,"reserve_proofs_5",data,0);
      delete_document_from_collection(database_name,"reserve_proofs_10",data,0);
      delete_document_from_collection(database_name,"reserve_proofs_10",data,0);
      delete_document_from_collection(database_name,"reserve_proofs_15",data,0);

      color_print("The database test data has been removed successfully\n","green");
      database_reset;
      exit(0);
    }
    if (strncmp(parameters[count],"--disable_synchronizing_databases_and_starting_timers",BUFFER_SIZE) == 0)
    {
      return 2;
    }
    if (strncmp(parameters[count],"--delegates_website",BUFFER_SIZE) == 0)
    {
      delegates_website = 1;
    }
    if (strncmp(parameters[count],"--shared_delegates_website",BUFFER_SIZE) == 0)
    {
      shared_delegates_website = 1;
    }
    if (strncmp(parameters[count],"--fee",BUFFER_SIZE) == 0 && count != (size_t)parameters_count)
    {
      sscanf(parameters[count+1], "%lf", &fee);
    }
    if (strncmp(parameters[count],"--minimum_amount",BUFFER_SIZE) == 0 && count != (size_t)parameters_count)
    {
      sscanf(parameters[count+1], "%lld", &minimum_amount);
    }
    if (strncmp(parameters[count],"--voter_inactivity_count",BUFFER_SIZE) == 0 && count != (size_t)parameters_count)
    {
      memset(voter_inactivity_count,0,sizeof(voter_inactivity_count));
      memcpy(voter_inactivity_count,parameters[count+1],strnlen(parameters[count+1],sizeof(voter_inactivity_count)));
    }
    if (strncmp(parameters[count],"--shared_delegate_payment_time_hour",BUFFER_SIZE) == 0 && count != (size_t)parameters_count)
    {
      sscanf(parameters[count+1], "%d", &shared_delegate_payment_time_hour);
    }
    if (strncmp(parameters[count],"--shared_delegate_payment_time_minute",BUFFER_SIZE) == 0 && count != (size_t)parameters_count)
    {
      sscanf(parameters[count+1], "%d", &shared_delegate_payment_time_minute);
    }
    if (strncmp(parameters[count],"--total_threads",BUFFER_SIZE) == 0 && count != (size_t)parameters_count)
    {
      sscanf(parameters[count+1], "%d", &total_threads);
    }    
  }
  return 1;

  #undef database_reset
  #undef SET_PARAMETERS_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: print_settings
Description: Prints the delegates settings
-----------------------------------------------------------------------------------------------------------
*/

void print_settings(void)
{
  // Variables
  char data[BUFFER_SIZE];

  memset(data,0,sizeof(data));

  memset(data,0,sizeof(data));
  memcpy(data,"Settings\n\nPublic Address: ",26);
  memcpy(data+strlen(data),xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\nBlock Verifiers Secret Key: ",29);
  memcpy(data+strlen(data),secret_key,VRF_SECRET_KEY_LENGTH);
  memcpy(data+strlen(data),"\nDatabase Name: ",16);
  memcpy(data+strlen(data),database_name,strnlen(database_name,sizeof(data)));
  if (shared_delegates_website == 1)
  {
    memcpy(data+strlen(data),"\nShared Delegate Settings: YES\nFee: ",36);
    snprintf(data+strlen(data),sizeof(data)-1,"%lf",fee);
    memcpy(data+strlen(data),"\nMinimum Amount: ",17);
    snprintf(data+strlen(data),sizeof(data)-1,"%lld",minimum_amount);
    memcpy(data+strlen(data),"\nShared Delegates Database Name: ",33);
    memcpy(data+strlen(data),shared_delegates_database_name,strnlen(shared_delegates_database_name,sizeof(data)));
  }
  else
  {
    memcpy(data+strlen(data),"\nShared Delegate Settings: NO",29);
  }  
  if (delegates_website == 1)
  {
    memcpy(data+strlen(data),"\nDelegate Settings: YES",23);
  }
  else
  {
    memcpy(data+strlen(data),"\nDelegate Settings: NO",22);
  }  
  if (log_file_settings == 0)
  {
    memcpy(data+strlen(data),"\nLog file Settings: NO",22);
  }
  else
  {
    if (log_file_settings == 1)
    {
      memcpy(data+strlen(data),"\nLog file Settings: YES\nLog File Color Output: NO",49);
    }
    else
    {
      memcpy(data+strlen(data),"\nLog file Settings: YES\nLog File Color Output: YES",50);
    } 
  } 
  memcpy(data+strlen(data),"\nDelegates Server IP Address: ",30);
  if (memcmp(XCASH_DPOPS_delegates_IP_address,"",1) == 0)
  {
    memcpy(data+strlen(data),"0.0.0.0",7);
  }
  else
  {
    memcpy(data+strlen(data),XCASH_DPOPS_delegates_IP_address,strnlen(XCASH_DPOPS_delegates_IP_address,sizeof(data)));
  }
  memcpy(data+strlen(data),"\nDelegates Server Port: 18283\nXCASH Wallet Port: ",49);
  snprintf(data+strlen(data),sizeof(data)-1,"%d",xcash_wallet_port);
  memcpy(data+strlen(data),"\nTotal Threads: ",16);
  snprintf(data+strlen(data),sizeof(data)-1,"%d",total_threads);
  memcpy(data+strlen(data),"\n\n",2);
  color_print(data,"yellow");
}



/*
-----------------------------------------------------------------------------------------------------------
Name: database_sync_check
Description: Sync check the databases
-----------------------------------------------------------------------------------------------------------
*/

void database_sync_check(void)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  size_t count = 0;
  long int current_time;
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // define macros
  #define DATABASE_SYNC_CHECK_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"database_sync_check",19); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  print_error_message(current_date_and_time,current_UTC_date_and_time,data); \
  mongoc_client_destroy(database_client); \
  mongoc_client_pool_destroy(database_client_thread_pool); \
  mongoc_uri_destroy(uri_thread_pool); \
  mongoc_cleanup(); \
  exit(0);

  // sync the block verifiers list
  if (sync_all_block_verifiers_list() == 0)
  {
    DATABASE_SYNC_CHECK_ERROR("Could not sync the previous, current and next block verifiers list");
  }

  // check if the database is synced, unless this is the main network data node
  if (memcmp(xcash_wallet_public_address,NETWORK_DATA_NODE_1_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) != 0)
  {
    if (network_data_node_settings == 1)
    {
      // check if all of the databases are synced from a random network data node
      if (check_if_databases_are_synced(2,0) == 0)
      {
        DATABASE_SYNC_CHECK_ERROR("Could not check if the databases are synced");
      }
    }
    else
    {
      // check if all of the databases are synced from a random block verifier
      if (check_if_databases_are_synced(1,0) == 0)
      {
        DATABASE_SYNC_CHECK_ERROR("Could not check if the databases are synced");
      }
    }    
  }

  // check the block verifiers current time, if it is not a network data node
  if (network_data_node_settings == 0)
  {
    color_print("Checking if the current time is synchronized with the network","green");
    for (count = 0; count < NETWORK_DATA_NODES_AMOUNT; count++)
    {
      // create the message
      memset(data,0,sizeof(data));
      memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_NETWORK_DATA_NODE_BLOCK_VERIFIERS_CURRENT_TIME\",\r\n}",95);

      // sign_data
      if (sign_data(data) == 0)
      { 
        DATABASE_SYNC_CHECK_ERROR("Could not sign the data");
      }

      if (send_and_receive_data_socket(data2,network_data_nodes_list.network_data_nodes_IP_address[count],SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 0)
      {
        continue;
      }

      if (verify_data(data2,0) == 0)
      {
        continue;
     }

      // parse the message
      memset(data,0,sizeof(data));
      if (parse_json_data(data2,"current_time",data,sizeof(data)) == 0 || memcmp(data,"",1) == 0)
      {
        continue;
      }

      sscanf(data,"%ld", &current_time);

      color_print(data2,"yellow");
      fprintf(stderr,"%ld",time(NULL));


      if (labs(time(NULL) - current_time) > BLOCK_VERIFIERS_SETTINGS)
      {
        DATABASE_SYNC_CHECK_ERROR("Invalid current time");
      }
    }
  }

  color_print("Started the sync all block verifiers list timer thread","green");
  return;

  #undef DATABASE_SYNC_CHECK_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: start_timer_threads
Description: Starts the timer threads
-----------------------------------------------------------------------------------------------------------
*/

void start_timer_threads(void)
{
  // Variables
  char data[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;

  // threads
  pthread_t thread_id[5];

  // define macros
  #define START_TIMER_THREADS_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"start_timer_threads",19); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  print_error_message(current_date_and_time,current_UTC_date_and_time,data); \
  mongoc_client_destroy(database_client); \
  mongoc_client_pool_destroy(database_client_thread_pool); \
  mongoc_uri_destroy(uri_thread_pool); \
  mongoc_cleanup(); \
  exit(0);

  memset(data,0,sizeof(data));

  print_start_message(current_date_and_time,current_UTC_date_and_time,"Starting all of the threads",data);

  // start the current block height timer thread
  if (pthread_create(&thread_id[0], NULL, &current_block_height_timer_thread, NULL) != 0 && pthread_detach(thread_id[0]) != 0)
  {
    START_TIMER_THREADS_ERROR("Could not start the current_block_height_timer_thread");
  }
  
  color_print("Started the current block height timer thread","green");

  /*// start the remove_inactive_delegates_timer_thread
  if (pthread_create(&thread_id[1], NULL, &remove_inactive_delegates_timer_thread, NULL) != 0 && pthread_detach(thread_id[1]) != 0)
  {
    START_TIMER_THREADS_ERROR("Could not start the current_block_height_timer_thread");
  }
  
  color_print("Started the remove inactive delegates timer thread","green");*/

  /*// start the check_reserve_proofs_timer_thread
  if (pthread_create(&thread_id[2], NULL, &check_reserve_proofs_timer_thread, NULL) != 0 && pthread_detach(thread_id[2]) != 0)
  {
    START_TIMER_THREADS_ERROR("Could not start the check_reserve_proofs_timer_thread");
  }

  color_print("Started the check reserve proofs timer thread","green");*/

  /*// start the block height timer thread
  if (shared_delegates_website == 1)
  {
    if (pthread_create(&thread_id[3], NULL, &block_height_timer_thread, NULL) != 0 && pthread_detach(thread_id[3]) != 0)
    {
      START_TIMER_THREADS_ERROR("Could not start the block_height_timer_thread");
    }
  
    color_print("Started the current block height timer thread","green");

    // start the payment timer thread
    if (pthread_create(&thread_id[4], NULL, &payment_timer_thread, NULL) != 0 && pthread_detach(thread_id[4]) != 0)
    {
      START_TIMER_THREADS_ERROR("Could not start the block_height_timer_thread");
    }
  
    color_print("Started the payment_timer_thread","green");
  }*/
  return;

  #undef START_TIMER_THREADS_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: start_registration_mode
Description: Starts the registration mode used when the registration period is opened, but the blockchain is not mining XCASH_DPOPS blocks yet
-----------------------------------------------------------------------------------------------------------
*/

void start_registration_mode(void)
{
  // Variables
  char data[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;

  // threads
  pthread_t thread_id[2];

  // define macros
  #define START_REGISTRATION_MODE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"start_registration_mode",23); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  print_error_message(current_date_and_time,current_UTC_date_and_time,data); \
  mongoc_client_destroy(database_client); \
  mongoc_client_pool_destroy(database_client_thread_pool); \
  mongoc_uri_destroy(uri_thread_pool); \
  mongoc_cleanup(); \
  exit(0);

   memset(data,0,sizeof(data));

  // start the sync_network_data_nodes_database_timer_thread
  if (network_data_node_settings == 1)
  {
    if (pthread_create(&thread_id[0], NULL, &sync_network_data_nodes_database_timer_thread, NULL) != 0 && pthread_detach(thread_id[0]) != 0)
    {
      START_REGISTRATION_MODE_ERROR("Could not start the sync network data nodes database timer thread");
    }
    color_print("Started the sync network data nodes database timer thread","green");
  }
 
  if (pthread_create(&thread_id[1], NULL, &sync_all_block_verifiers_list_timer_thread, NULL) != 0 && pthread_detach(thread_id[1]) != 0)
  {
    START_REGISTRATION_MODE_ERROR("Could not start the sync all block verifiers list timer thread");
  }

  color_print("Started the sync all block verifiers list timer thread","green");
  return;

  #undef START_REGISTRATION_MODE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: main
Description: The start point of the program
Parameters:
  parameters_count - The parameter count
  parameters - The parameters
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int main(int parameters_count, char* parameters[])
{
  // iniltize the random number generator
  srand(time(NULL));

  // Variables
  char data[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  
  // define macros
  #define MESSAGE "{\"username\":\"XCASH\"}"
  
  #define database_reset \
  mongoc_client_destroy(database_client); \
  mongoc_client_pool_destroy(database_client_thread_pool); \
  mongoc_uri_destroy(uri_thread_pool); \
  mongoc_cleanup();

  #define MAIN_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"start_registration_mode",23); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  print_error_message(current_date_and_time,current_UTC_date_and_time,data); \
  mongoc_client_destroy(database_client); \
  mongoc_client_pool_destroy(database_client_thread_pool); \
  mongoc_uri_destroy(uri_thread_pool); \
  mongoc_cleanup(); \
  exit(0);

  memset(data,0,sizeof(data));

  initialize_data();

  // write the message
  color_print("XCASH DPOPS - Version 1.0.0\n","green");

  // check if they want to display the parameters
  if (parameters_count == 2 && strncmp(parameters[1],"--parameters",BUFFER_SIZE) == 0)
  {
    printf(INVALID_PARAMETERS_ERROR_MESSAGE);
    exit(0);
  }

  if (parameters_count == 2 && strncmp(parameters[1],"--generate_key",BUFFER_SIZE) == 0)
  {
    generate_key();
    exit(0);
  }

  create_overall_database_connection();

  get_delegates_data();

  if (set_parameters(parameters_count, parameters) == 2)
  {
    goto disable_synchronizing_databases_and_starting_timers;
  }

  // check if it should create the default database data
  memset(data,0,sizeof(data));
  if (read_document_field_from_collection(database_name,"statistics",MESSAGE,"username",data,0) == 0)
  {
    INITIALIZE_DATABASE_DATA(0);
  }

  print_settings();  

  // check if the block verifier is a network data node
  CHECK_IF_BLOCK_VERIFIERS_IS_NETWORK_DATA_NODE;     
 
  database_sync_check();  
  
  // start the server
  if (create_server(1) == 0)
  {
    MAIN_ERROR("Could not start the server");
  }

  /*// wait for enough block verifiers to start the block verification process
  for (;;)
  {
    sleep(60);
    get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);
    if (current_UTC_date_and_time->tm_min % BLOCK_TIME == 0)
    {
      if (get_delegates_online_status() >= BLOCK_VERIFIERS_VALID_AMOUNT)
      {
        break;
      }
    }
  }*/

  start_timer_threads();

  // start registration only mode
  // start_registration_mode();

  for (;;)
  {
    sleep(10);
  }

  disable_synchronizing_databases_and_starting_timers:

  // start the server
  if (create_server(1) == 0)
  {
    MAIN_ERROR("Could not start the server");
  }

  /*// wait for enough block verifiers to start the block verification process
  for (;;)
  {
    sleep(60);
    get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);
    if (current_UTC_date_and_time->tm_min % BLOCK_TIME == 0)
    {
      if (get_delegates_online_status() >= BLOCK_VERIFIERS_VALID_AMOUNT)
      {
        break;
      }
    }
  }*/  

  for (;;)
  {
    sleep(10);
  }

  database_reset;
  return 0; 
  
  #undef MESSAGE
  #undef database_reset
  #undef MAIN_ERROR
}