#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <pthread.h>
#include <netdb.h>
#include <sys/sysinfo.h>
#include <sys/resource.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "define_macros_test.h"
#include "structures.h"
#include "variables.h"

#include "shared_delegate_website_thread_server_functions.h"

#include "block_verifiers_synchronize_functions.h"
#include "block_verifiers_synchronize_check_functions.h"
#include "block_verifiers_thread_server_functions.h"
#include "block_verifiers_update_functions.h"
#include "block_verifiers_functions.h"
#include "database_functions.h"
#include "count_database_functions.h"
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
struct network_data_nodes_sync_database_list network_data_nodes_sync_database_list; // Holds the network data nodes data and database hash for syncing network data nodes
struct block_verifiers_sync_database_list block_verifiers_sync_database_list; // Holds the block verifiers data and database hash for syncing the block verifiers
struct delegates_online_status delegates_online_status[MAXIMUM_AMOUNT_OF_DELEGATES]; // Holds the delegates online status
struct block_height_start_time block_height_start_time; // Holds the block height start time data
char current_round_part[2]; // The current round part (1-4)
char current_round_part_backup_node[2]; // The current main node in the current round part (0-5)
pthread_rwlock_t rwlock;
pthread_rwlock_t rwlock_reserve_proofs;
pthread_mutex_t lock;
pthread_mutex_t database_lock;
pthread_mutex_t verify_network_block_lock;
pthread_mutex_t vote_lock;
pthread_mutex_t add_reserve_proof_lock;
pthread_mutex_t invalid_reserve_proof_lock;
pthread_mutex_t database_data_IP_address_lock;

pthread_t server_threads[100];
int epoll_fd;
int server_socket;

char current_block_height[BUFFER_SIZE_NETWORK_BLOCK_DATA]; // The current block height
char previous_block_hash[BLOCK_HASH_LENGTH+1]; // The current block height
int error_message_count; // The error message count
int main_network_data_node_create_block; // 1 if the main network data node can create a block, 0 if not
int main_network_data_node_receive_block; // 1 if you have received the block from the main network data node, 0 if not
int network_data_node_valid_amount; // The amount of network data nodes that were valid
int network_data_nodes_sync_databases_settings; // 1 if a block verifier can sync from a network data node, 0 if not
int log_file_settings; // 0 to use the terminal, 1 to use a log file, 2 to use a log file with color output
char log_file[BUFFER_SIZE_NETWORK_BLOCK_DATA]; // The log file
char XCASH_DPOPS_delegates_IP_address[BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH]; // The  block verifiers IP address to run the server on
int xcash_wallet_port; // The xcash wallet port
char database_name[BUFFER_SIZE_NETWORK_BLOCK_DATA];
char shared_delegates_database_name[BUFFER_SIZE_NETWORK_BLOCK_DATA];
char database_path_write[1024]; // holds the database write path
char database_path_read[1024]; // holds the database read path
int network_functions_test_settings;
int network_functions_test_error_settings; // 1 to display errors, 0 to not display errors when running the reset variables allocated on the heap test
int network_functions_test_server_messages_settings; // 1 to display server messages, 0 to not display server messages when running the test
int test_settings; // 1 when the test are running, 0 if not
int debug_settings; // 1 to show all incoming and outgoing message from the server
int registration_settings; // 1 when the registration mode is running, 0 when it is not
int synced_network_data_nodes[BLOCK_VERIFIERS_AMOUNT]; // the synced network data nodes
size_t block_verifiers_current_block_height[BLOCK_VERIFIERS_AMOUNT]; // holds the block verifiers current block heights
int production_settings; // 0 for production, 1 for test
int production_settings_database_data_settings; // The initialize the database settings
char website_path[1024]; // holds the path to the website if running a delegates explorer or shared delegates pool
int sync_previous_current_next_block_verifiers_settings; // sync the previous, current and next block verifiers if you had to restart
int database_data_socket_settings; // 1 to allow database data up to 50MB to be received in the server, 0 to only allow message up to BUFFER_SIZE
char* server_limit_IP_address_list; // holds all of the IP addresses that are currently running on the server. This can hold up to 1 million IP addresses
char* server_limit_public_address_list; // holds all of the public addresses that are currently running on the server. This can hold up to 1 million public addresses
int invalid_block_verifiers_count; // counts how many times your node did not receive the block from the main network backup node, to indicate if your node is not syncing
int backup_network_data_node_settings; // The network data node that will attempt to create the block if the block producer and backup block producer fail
int replayed_round_settings; // 1 if the round is a replayed round, 0 if not

int delegates_website; // 1 if the running the delegates websites, 0 if not
int shared_delegates_website; // 1 if the running the shared delegates websites, 0 if not
int total_threads; // The total threads
double fee; // the fee
long long int minimum_amount; // the minimum amount to send a payment
char voter_inactivity_count[10]; // the number of days to wait to remove an inactive delegates information from the database



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
Parameters:
  parameters_count - The parameter count
  parameters - The parameters
-----------------------------------------------------------------------------------------------------------
*/

void initialize_data(int parameters_count, char* parameters[])
{
  // Variables
  char data[SMALL_BUFFER_SIZE];
  size_t count = 0;
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;

  // define macros
  #define INITIALIZE_DATA_ERROR \
  memcpy(error_message.function[error_message.total],"initialize_data",15); \
  memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48); \
  error_message.total++; \
  print_error_message(current_date_and_time,current_UTC_date_and_time,data); \
  exit(0);

  memset(data,0,sizeof(data));

  // initialize the global variables
  memset(current_block_height,0,sizeof(current_block_height));
  memset(current_round_part,0,sizeof(current_round_part));
  memset(current_round_part_backup_node,0,sizeof(current_round_part_backup_node));
  memset(secret_key,0,sizeof(secret_key));
  memset(secret_key_data,0,sizeof(secret_key_data));
  memset(log_file,0,sizeof(log_file));
  memset(XCASH_DPOPS_delegates_IP_address,0,sizeof(XCASH_DPOPS_delegates_IP_address));
  memcpy(XCASH_DPOPS_delegates_IP_address,"127.0.0.1",9);
  memset(database_name,0,sizeof(database_name));
  memset(shared_delegates_database_name,0,sizeof(shared_delegates_database_name));
  memset(database_path_write,0,sizeof(database_path_write));
  memset(database_path_read,0,sizeof(database_path_read));
  memset(voter_inactivity_count,0,sizeof(voter_inactivity_count));
  log_file_settings = 0;
  xcash_wallet_port = XCASH_WALLET_PORT;
  network_functions_test_settings = 0;
  network_functions_test_error_settings = 1;
  network_functions_test_server_messages_settings = 1;
  debug_settings = 0;
  registration_settings = 0;
  block_height_start_time.block_height_start_time = 0;
  network_data_nodes_sync_databases_settings = 1;
  production_settings = 1;
  production_settings_database_data_settings = 0;
  sync_previous_current_next_block_verifiers_settings = 1;
  database_data_socket_settings = 0;
  invalid_block_verifiers_count = 0;
  replayed_round_settings = 0;

  pthread_rwlock_init(&rwlock,NULL);
  pthread_rwlock_init(&rwlock_reserve_proofs,NULL);
  pthread_mutex_init(&lock, NULL);
  pthread_mutex_init(&database_lock, NULL);
  pthread_mutex_init(&verify_network_block_lock, NULL);
  pthread_mutex_init(&vote_lock, NULL);
  pthread_mutex_init(&add_reserve_proof_lock, NULL);
  pthread_mutex_init(&invalid_reserve_proof_lock, NULL);
  pthread_mutex_init(&database_data_IP_address_lock, NULL);

  server_limit_IP_address_list = (char*)calloc(15728640,sizeof(char)); // 15 MB
  server_limit_public_address_list = (char*)calloc(15728640,sizeof(char)); // 15 MB
   
  // check if the memory needed was allocated on the heap successfully
  if (server_limit_IP_address_list == NULL || server_limit_public_address_list == NULL)
  {
    INITIALIZE_DATA_ERROR;
  }

  // initialize the error_message struct
  for (count = 0; count < TOTAL_ERROR_MESSAGES; count++)
  {
    error_message.function[count] = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
    error_message.data[count] = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));

    if (error_message.function[count] == NULL || error_message.data[count] == NULL)
    {
      INITIALIZE_DATA_ERROR;
    }
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
  synced_block_verifiers.last_refresh_time_of_synced_block_verifiers = 0;

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
    network_data_nodes_list.online_status[count] = 1;
  }

  // set the network_data_node_settings
  network_data_node_settings = 0;

  // initialize the current_round_part_vote_data struct
  memset(current_round_part_vote_data.current_vote_results,0,sizeof(current_round_part_vote_data.current_vote_results));

  // initialize the VRF_data struct 
  VRF_data.vrf_secret_key_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  VRF_data.vrf_secret_key = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(unsigned char));
  VRF_data.vrf_public_key_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  VRF_data.vrf_public_key = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(unsigned char));
  VRF_data.vrf_alpha_string_data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  VRF_data.vrf_alpha_string = (unsigned char*)calloc(BUFFER_SIZE,sizeof(unsigned char));
  VRF_data.vrf_proof_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  VRF_data.vrf_proof = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(unsigned char));
  VRF_data.vrf_beta_string_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  VRF_data.vrf_beta_string = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(unsigned char));
  VRF_data.block_blob = (char*)calloc(BUFFER_SIZE,sizeof(char));
  VRF_data.reserve_bytes_data_hash = (char*)calloc(DATA_HASH_LENGTH+1,sizeof(char));

  // check if the memory needed was allocated on the heap successfully
  if (VRF_data.vrf_public_key_data == NULL || VRF_data.vrf_public_key == NULL || VRF_data.vrf_alpha_string_data == NULL || VRF_data.vrf_alpha_string == NULL || VRF_data.vrf_proof_data == NULL || VRF_data.vrf_proof == NULL || VRF_data.vrf_beta_string_data == NULL || VRF_data.vrf_beta_string == NULL || VRF_data.block_blob == NULL)
  {
    INITIALIZE_DATA_ERROR;
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
      INITIALIZE_DATA_ERROR;
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
  blockchain_data.blockchain_reserve_bytes.vrf_secret_key_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_secret_key = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(unsigned char));
  blockchain_data.blockchain_reserve_bytes.vrf_public_key_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_public_key = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(unsigned char));
  blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_alpha_string = (unsigned char*)calloc(BUFFER_SIZE,sizeof(unsigned char));
  blockchain_data.blockchain_reserve_bytes.vrf_proof_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_proof = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(unsigned char));
  blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_beta_string = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(unsigned char));
  blockchain_data.blockchain_reserve_bytes.vrf_data_round = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.previous_block_hash_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));

  // check if the memory needed was allocated on the heap successfully
  if (blockchain_data.network_version_data == NULL || blockchain_data.timestamp_data == NULL || blockchain_data.previous_block_hash_data == NULL || blockchain_data.nonce_data == NULL || blockchain_data.block_reward_transaction_version_data == NULL || blockchain_data.unlock_block_data == NULL || blockchain_data.block_reward_input_data == NULL || blockchain_data.vin_type_data == NULL || blockchain_data.block_height_data == NULL || blockchain_data.block_reward_output_data == NULL || blockchain_data.block_reward_data == NULL || blockchain_data.stealth_address_output_tag_data == NULL || blockchain_data.stealth_address_output_data == NULL || blockchain_data.extra_bytes_size_data == NULL || blockchain_data.transaction_public_key_tag_data == NULL || blockchain_data.transaction_public_key_data == NULL || blockchain_data.extra_nonce_tag_data == NULL || blockchain_data.reserve_bytes_size_data == NULL || blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data == NULL || blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name == NULL || blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data == NULL || blockchain_data.blockchain_reserve_bytes.block_producer_public_address == NULL || blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data == NULL || blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data == NULL || blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names == NULL || blockchain_data.blockchain_reserve_bytes.vrf_public_key_data == NULL || blockchain_data.blockchain_reserve_bytes.vrf_public_key == NULL || blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data == NULL || blockchain_data.blockchain_reserve_bytes.vrf_alpha_string == NULL || blockchain_data.blockchain_reserve_bytes.vrf_proof_data == NULL || blockchain_data.blockchain_reserve_bytes.vrf_proof == NULL || blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data == NULL || blockchain_data.blockchain_reserve_bytes.vrf_beta_string == NULL || blockchain_data.blockchain_reserve_bytes.vrf_data_round == NULL || blockchain_data.blockchain_reserve_bytes.vrf_data == NULL || blockchain_data.blockchain_reserve_bytes.previous_block_hash_data == NULL)
  {
    INITIALIZE_DATA_ERROR;
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
      INITIALIZE_DATA_ERROR;
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
      INITIALIZE_DATA_ERROR;
    }
  }
  invalid_reserve_proofs.count = 0;

  for (count = 0; count < MAXIMUM_TRANSACATIONS_PER_BLOCK; count++)
  {
    blockchain_data.transactions[count] = (char*)calloc(TRANSACTION_HASH_LENGTH+1,sizeof(char));

    // check if the memory needed was allocated on the heap successfully
    if (blockchain_data.transactions[count] == NULL)
    {
      INITIALIZE_DATA_ERROR;
    }
  }

  // set the production settings
  for (count = 0; count < (size_t)parameters_count; count++)
  { 
    if (strncmp(parameters[count],"--test-mode",BUFFER_SIZE) == 0)
    {
      production_settings = 0;
      sscanf(parameters[count+1], "%d", &production_settings_database_data_settings);
    }
  }

  // initialize the network data nodes
  INITIALIZE_NETWORK_DATA_NODES;
  return;

  #undef INITIALIZE_DATA_ERROR
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
  char data[SMALL_BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;

  #define CREATE_OVERALL_DATABASE_CONNECTION_ERROR \
  memcpy(error_message.function[error_message.total],"create_overall_database_connection",34); \
  memcpy(error_message.data[error_message.total],"Could not create a connection for the database",46); \
  error_message.total++; \
  print_error_message(current_date_and_time,current_UTC_date_and_time,data); \
  mongoc_uri_destroy(uri_thread_pool); \
  mongoc_cleanup(); \
  exit(0);

  memset(data,0,sizeof(data));

  // initialize the database connection
  mongoc_init();

  // create a connection to the database
  if (!(uri_thread_pool = mongoc_uri_new_with_error(DATABASE_CONNECTION, &error)))
  {
    CREATE_OVERALL_DATABASE_CONNECTION_ERROR;
  }

  if (!(database_client_thread_pool = mongoc_client_pool_new(uri_thread_pool)))
  {
    CREATE_OVERALL_DATABASE_CONNECTION_ERROR;
  }
  return;

  #undef CREATE_OVERALL_DATABASE_CONNECTION_ERROR
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
  FILE* file;
  char data[SMALL_BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;

  // define macros
  #define GET_DELEGATES_DATA_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"get_delegates_data",18); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  print_error_message(current_date_and_time,current_UTC_date_and_time,data); \
  return

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

  // get the previous block hash
  if (get_previous_block_hash(previous_block_hash) == 0)
  {
    GET_DELEGATES_DATA_ERROR("Could not get the previous block hash");
  }

  // get the database paths
  if (production_settings == 1 && network_data_node_settings == 1)
  {
    // get the database_path_write
    memcpy(database_path_write,"cd ~ && rm -r dump_copy ; mv dump dump_copy ; ",46);

    file = popen("sudo find / -path /sys -prune -o -path /proc -prune -o -path /dev -prune -o -path /var -prune -o -type d -name 'mongodb-linux-x86_64-ubuntu1804-*' -print", "r");

    if (fgets(database_path_write+strlen(database_path_write),(int)(sizeof(database_path_write)-strlen(database_path_write)),file) == NULL)
    {
      GET_DELEGATES_DATA_ERROR("Could not get the mongo database path");
    }
    memcpy(database_path_write+strlen(database_path_write)-1,"/bin/mongodump --quiet",22);

    // get the database_path_read
    memcpy(database_path_read,"cd ~ && ",8);

    file = popen("sudo find / -path /sys -prune -o -path /proc -prune -o -path /dev -prune -o -path /var -prune -o -type d -name 'mongodb-linux-x86_64-ubuntu1804-*' -print", "r");

    if (fgets(database_path_read+strlen(database_path_read),(int)(sizeof(database_path_read)-strlen(database_path_read)),file) == NULL)
    {
      GET_DELEGATES_DATA_ERROR("Could not get the mongo database path");
    }
    memcpy(database_path_read+strlen(database_path_read)-1,"/bin/mongorestore --quiet",25);
  }

  // get the website path
  memset(website_path,0,sizeof(website_path));
  memset(data,0,sizeof(data));
  if (readlink("/proc/self/exe", data, sizeof(data)) == -1)
  {
    GET_DELEGATES_DATA_ERROR("Could not get the websites path");
  }
  memcpy(website_path,data,strnlen(data,sizeof(website_path))-17);
  delegates_website == 1 ? memcpy(website_path+strlen(website_path),DELEGATES_WEBSITE_PATH,sizeof(DELEGATES_WEBSITE_PATH)-1) : memcpy(website_path+strlen(website_path),SHARED_DELEGATES_WEBSITE_PATH,sizeof(SHARED_DELEGATES_WEBSITE_PATH)-1);
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
  #define MINIMUM_THREADS_AMOUNT 2

  #define database_reset \
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
  char data[SMALL_BUFFER_SIZE];
  char data2[SMALL_BUFFER_SIZE];
  size_t count;
  size_t count2 = 0;
  size_t count3;
  size_t counter;
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
  log_file_settings = 0;
  test_settings = 0;
  memcpy(voter_inactivity_count,VOTER_INACTIVITY_COUNT,sizeof(VOTER_INACTIVITY_COUNT)-1);
  xcash_wallet_port = XCASH_WALLET_PORT;

  // set the current_round_part, current_round_part_backup_node and server message, this way the node will start at the begining of a round
  memset(current_round_part,0,sizeof(current_round_part));
  memset(current_round_part_backup_node,0,sizeof(current_round_part_backup_node));
  memcpy(current_round_part,"1",sizeof(char));
  memcpy(current_round_part_backup_node,"0",sizeof(char));

  // check all of the parameters to see if there is a block verifier secret key
  if (parameters_count < 3)
  {
    SET_PARAMETERS_ERROR("Could not get the block verifiers secret key.\nMake sure to run xcash-dpops with the --block-verifiers-secret-key parameter");
  }
  
  // check the parameters
  for (count = 0, count2 = 0; count < (size_t)parameters_count; count++)
  { 
    if (strncmp(parameters[count],"--block-verifiers-secret-key",BUFFER_SIZE) == 0)
    {
      count2 = 1;
    }
  }

  if (count2 != 1)
  {
    SET_PARAMETERS_ERROR("Could not get the block verifiers secret key.\nMake sure to run xcash-dpops with the --block-verifiers-secret-key parameter");
  }

  // check the parameters
  for (count = 0; count < (size_t)parameters_count; count++)
  { 
    if (strncmp(parameters[count],"--block-verifiers-secret-key",BUFFER_SIZE) == 0)
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
        secret_key_data[counter] = (unsigned char)strtol(data2, NULL, 16);
      }
    }
    if (strncmp(parameters[count],"--test",BUFFER_SIZE) == 0)
    {
      get_delegates_data();
      test(0);
      database_reset;
      exit(0);
    }
    if (strncmp(parameters[count],"--quick-test",BUFFER_SIZE) == 0)
    {
      get_delegates_data();
      test(1);
      database_reset;
      exit(0);
    }
    if (strncmp(parameters[count],"--optimization-test",BUFFER_SIZE) == 0)
    {
      get_delegates_data();
      test(2);
      database_reset;
      exit(0);
    }
    if (strncmp(parameters[count],"--debug",BUFFER_SIZE) == 0)
    {
      debug_settings = 1;
    }
    if (strncmp(parameters[count],"--delegates-ip-address",BUFFER_SIZE) == 0 && count != (size_t)parameters_count)
    {
      memset(XCASH_DPOPS_delegates_IP_address,0,strlen(XCASH_DPOPS_delegates_IP_address));
      memcpy(XCASH_DPOPS_delegates_IP_address,parameters[count+1],strnlen(parameters[count+1],sizeof(XCASH_DPOPS_delegates_IP_address)));
    }
    if (strncmp(parameters[count],"--xcash-wallet-port",BUFFER_SIZE) == 0 && count != (size_t)parameters_count)
    {
      sscanf(parameters[count+1],"%d",&xcash_wallet_port);
    }
    if (strncmp(parameters[count],"--database-name",BUFFER_SIZE) == 0 && count != (size_t)parameters_count)
    {
      memset(database_name,0,sizeof(database_name));
      memcpy(database_name,parameters[count+1],strnlen(parameters[count+1],sizeof(database_name)));
    }
    if (strncmp(parameters[count],"--shared-delegates-database-name",BUFFER_SIZE) == 0 && count != (size_t)parameters_count)
    {
      memset(shared_delegates_database_name,0,sizeof(shared_delegates_database_name));
      memcpy(shared_delegates_database_name,parameters[count+1],strnlen(parameters[count+1],sizeof(shared_delegates_database_name)));
    }
    if (strncmp(parameters[count],"--log-file",BUFFER_SIZE) == 0 && count != (size_t)parameters_count)
    {
      log_file_settings = 1;
      memcpy(log_file,parameters[count+1],strnlen(parameters[count+1],sizeof(log_file)));

      memset(data,0,sizeof(data));
      color_print("\n\n\n\n\nxcash-dpops - Version 1.0.0\n","green");
      memcpy(data,"Successfully received the public address:",41);
      memcpy(data+41,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      color_print(data,"green");
    }
    if (strncmp(parameters[count],"--log-file_color",BUFFER_SIZE) == 0 && count != (size_t)parameters_count)
    {
      log_file_settings = 2;
      memcpy(log_file,parameters[count+1],strnlen(parameters[count+1],sizeof(log_file)));

      memset(data,0,sizeof(data));
      color_print("\n\n\n\n\nxcash-dpops - Version 1.0.0\n","green");
      memcpy(data,"Successfully received the public address:",41);
      memcpy(data+41,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      color_print(data,"green");
    }
    if (strncmp(parameters[count],"--synchronize-database-from-network-data-node",BUFFER_SIZE) == 0)
    {
      check_if_databases_are_synced(2,0);
      database_reset;
      exit(0);
    }
    if (strncmp(parameters[count],"--synchronize-database-from-specific-delegate",BUFFER_SIZE) == 0 && count != (size_t)parameters_count)
    {
      color_print("Syncing the block verifiers list","yellow");
      sync_all_block_verifiers_list(1,1);
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
    if (strncmp(parameters[count],"--test-data-add",BUFFER_SIZE) == 0)
    {
      memset(data,0,sizeof(data));
      
      insert_document_into_collection_json(shared_delegates_database_name,"blocks_found","{\"block_height\":\"5\",\"block_hash\":\"BLOCK_HASH\",\"block_date_and_time\":\"10\",\"block_reward\":\"15\"}");
      insert_document_into_collection_json(shared_delegates_database_name,"blocks_found","{\"block_height\":\"5\",\"block_hash\":\"BLOCK_HASH\",\"block_date_and_time\":\"10\",\"block_reward\":\"15\"}");

      memset(data,0,strlen(data));
      memcpy(data,"{\"public_address\":\"",19);
      memcpy(data+19,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      memcpy(data+117,"\",\"current_total\":\"5\",\"total\":\"10\",\"inactivity_count\":\"15\"}",59);
      insert_document_into_collection_json(shared_delegates_database_name,"public_addresses",data);

      memset(data,0,strlen(data));
      memcpy(data,"{\"public_address\":\"",19);
      memcpy(data+19,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      memcpy(data+117,"\",\"date_and_time\":\"5\",\"total\":\"10\",\"tx_hash\":\"TX_HASH\",\"tx_key\":\"TX_KEY\"}",73);
      insert_document_into_collection_json(shared_delegates_database_name,"public_addresses_payments",data);
      insert_document_into_collection_json(shared_delegates_database_name,"public_addresses_payments",data);

      memset(data,0,strlen(data));
      memcpy(data,"{\"public_address_created_reserve_proof\":\"",41);
      memcpy(data+41,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      memcpy(data+139,"\",\"public_address_voted_for\":\"",30);
      memcpy(data+169,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      memcpy(data+267,"\",\"total\":\"10\",\"reserve_proof\":\"15\"}",36);
      insert_document_into_collection_json(database_name,"reserve_proofs_5",data);
      insert_document_into_collection_json(database_name,"reserve_proofs_10",data);
      insert_document_into_collection_json(database_name,"reserve_proofs_10",data);
      insert_document_into_collection_json(database_name,"reserve_proofs_15",data);

      color_print("The database test data has been added successfully\n","green");
      database_reset;
      exit(0);
    }
    if (strncmp(parameters[count],"--test-data-remove",BUFFER_SIZE) == 0)
    {
      memset(data,0,strlen(data));
      
      delete_document_from_collection(shared_delegates_database_name,"blocks_found","{\"block_height\":\"5\"}");
      delete_document_from_collection(shared_delegates_database_name,"blocks_found","{\"block_height\":\"5\"}");

      memset(data,0,strlen(data));
      memcpy(data,"{\"public_address\":\"",19);
      memcpy(data+19,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      memcpy(data+117,"\"}",2);
      delete_document_from_collection(shared_delegates_database_name,"public_addresses",data);

      memset(data,0,strlen(data));
      memcpy(data,"{\"public_address\":\"",19);
      memcpy(data+19,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      memcpy(data+117,"\"}",2);
      delete_document_from_collection(shared_delegates_database_name,"public_addresses_payments",data);
      delete_document_from_collection(shared_delegates_database_name,"public_addresses_payments",data);

      memset(data,0,strlen(data));
      memcpy(data,"{\"public_address_voted_for\":\"",29);
      memcpy(data+29,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      memcpy(data+127,"\"}",2);
      delete_document_from_collection(database_name,"reserve_proofs_5",data);
      delete_document_from_collection(database_name,"reserve_proofs_10",data);
      delete_document_from_collection(database_name,"reserve_proofs_10",data);
      delete_document_from_collection(database_name,"reserve_proofs_15",data);

      color_print("The database test data has been removed successfully\n","green");
      database_reset;
      exit(0);
    }
    if (strncmp(parameters[count],"--disable-synchronizing-databases-and-starting-timers",BUFFER_SIZE) == 0)
    {
      return 2;
    }
    if (strncmp(parameters[count],"--registration-mode",BUFFER_SIZE) == 0)
    {
      registration_settings = 1;
      return 3;
    }
    if (strncmp(parameters[count],"--start-time",BUFFER_SIZE) == 0)
    {
      sscanf(parameters[count+1], "%d", &block_height_start_time.block_height_start_time_month);
      sscanf(parameters[count+2], "%d", &block_height_start_time.block_height_start_time_day);
      sscanf(parameters[count+3], "%d", &block_height_start_time.block_height_start_time_hour);
      sscanf(parameters[count+4], "%d", &block_height_start_time.block_height_start_time_minute);

      // if the program restarts dont wait for the start_time again
      get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);
      if ((current_UTC_date_and_time.tm_mon > block_height_start_time.block_height_start_time_month) || (current_UTC_date_and_time.tm_mon == block_height_start_time.block_height_start_time_month && current_UTC_date_and_time.tm_mday > block_height_start_time.block_height_start_time_day) || (current_UTC_date_and_time.tm_mon == block_height_start_time.block_height_start_time_month && current_UTC_date_and_time.tm_mday == block_height_start_time.block_height_start_time_day && current_UTC_date_and_time.tm_hour > block_height_start_time.block_height_start_time_hour) || (current_UTC_date_and_time.tm_mon == block_height_start_time.block_height_start_time_month && current_UTC_date_and_time.tm_mday == block_height_start_time.block_height_start_time_day && current_UTC_date_and_time.tm_hour == block_height_start_time.block_height_start_time_hour && current_UTC_date_and_time.tm_min > block_height_start_time.block_height_start_time_minute))
      {
        block_height_start_time.block_height_start_time = 0;
      }
      else
      {
        block_height_start_time.block_height_start_time = 1;
      }
    }
    if (strncmp(parameters[count],"--delegates-website",BUFFER_SIZE) == 0)
    {
      delegates_website = 1;
    }
    if (strncmp(parameters[count],"--shared-delegates-website",BUFFER_SIZE) == 0)
    {
      shared_delegates_website = 1;
    }
    if (strncmp(parameters[count],"--fee",BUFFER_SIZE) == 0 && count != (size_t)parameters_count)
    {
      sscanf(parameters[count+1], "%lf", &fee);
    }
    if (strncmp(parameters[count],"--minimum-amount",BUFFER_SIZE) == 0 && count != (size_t)parameters_count)
    {
      sscanf(parameters[count+1], "%lld", &minimum_amount);
    }
    if (strncmp(parameters[count],"--voter-inactivity-count",BUFFER_SIZE) == 0 && count != (size_t)parameters_count)
    {
      memset(voter_inactivity_count,0,sizeof(voter_inactivity_count));
      memcpy(voter_inactivity_count,parameters[count+1],strnlen(parameters[count+1],sizeof(voter_inactivity_count)));
    }
    if (strncmp(parameters[count],"--total-threads",BUFFER_SIZE) == 0 && count != (size_t)parameters_count)
    {
      if (total_threads >= MINIMUM_THREADS_AMOUNT)
      {
        sscanf(parameters[count+1], "%d", &total_threads);
      }
      else
      {
        total_threads = MINIMUM_THREADS_AMOUNT;
      }
    }    
  }
  return 1;

  #undef MINIMUM_THREADS_AMOUNT
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
  char data[SMALL_BUFFER_SIZE];

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
  delegates_website == 1 ? memcpy(data+strlen(data),"\nDelegate Settings: YES",23) : memcpy(data+strlen(data),"\nDelegate Settings: NO",22);
  if (log_file_settings == 0)
  {
    memcpy(data+strlen(data),"\nLog file Settings: NO",22);
  }
  else
  {
    log_file_settings == 1 ? memcpy(data+strlen(data),"\nLog file Settings: YES\nLog File Color Output: NO",49) : memcpy(data+strlen(data),"\nLog file Settings: YES\nLog File Color Output: YES",50);
  }
  debug_settings == 1 ? memcpy(data+strlen(data),"\nDebug Settings: YES",20) : memcpy(data+strlen(data),"\nDebug Settings: NO",19);
  memcpy(data+strlen(data),"\nDelegates Server IP Address: ",30);
  strncmp(XCASH_DPOPS_delegates_IP_address,"127.0.0.1",9) == 0 ? memcpy(data+strlen(data),"0.0.0.0",7) : memcpy(data+strlen(data),XCASH_DPOPS_delegates_IP_address,strnlen(XCASH_DPOPS_delegates_IP_address,sizeof(data)));
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
  long long int current_time;
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
  mongoc_client_pool_destroy(database_client_thread_pool); \
  mongoc_uri_destroy(uri_thread_pool); \
  mongoc_cleanup(); \
  exit(0);

  // check if all network data nodes are offline
  if (get_network_data_nodes_online_status() == 0)
  {
    // this is the first network data node on the network, or none of the network data nodes are on the network. load the current block verifiers from your database and skip the database syncing and skip the check the block verifiers current time
    if (sync_all_block_verifiers_list(1,0) == 0)
    {
      DATABASE_SYNC_CHECK_ERROR("Could not sync the previous, current and next block verifiers list");
    }
    color_print("Started the sync all block verifiers list timer thread","green");
    return;
  }

  // sync the block verifiers list
  if (sync_all_block_verifiers_list(1,1) == 0)
  {
    DATABASE_SYNC_CHECK_ERROR("Could not sync the previous, current and next block verifiers list");
  }

  // check if all of the databases are synced
  sscanf(current_block_height,"%zu", &count);
  if ((count <= XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT && check_if_databases_are_synced(3,0) == 0) || (count > XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT && check_if_databases_are_synced(1,0) == 0))
  {
    DATABASE_SYNC_CHECK_ERROR("Could not check if the databases are synced");
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

      if (send_and_receive_data_socket(data2,sizeof(data2),network_data_nodes_list.network_data_nodes_IP_address[count],SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 0)
      {
        continue;
      }

      if (verify_data(data2,0) == 0)
      {
        continue;
      }

      // parse the message
      memset(data,0,sizeof(data));
      if (parse_json_data(data2,"current_time",data,sizeof(data)) == 0 || strncmp(data,"",1) == 0)
      {
        continue;
      }

      sscanf(data,"%lld", &current_time);

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
  char data[BUFFER_SIZE_NETWORK_BLOCK_DATA];
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

  // start the block height timer thread
  if (shared_delegates_website == 1)
  {
    if (pthread_create(&thread_id[3], NULL, &block_height_timer_thread, NULL) != 0 && pthread_detach(thread_id[3]) != 0)
    {
      START_TIMER_THREADS_ERROR("Could not start the block_height_timer_thread");
    }
  
    color_print("Started the shared delegates current block height timer thread","green");

    // start the payment timer thread
    if (pthread_create(&thread_id[4], NULL, &payment_timer_thread, NULL) != 0 && pthread_detach(thread_id[4]) != 0)
    {
      START_TIMER_THREADS_ERROR("Could not start the block_height_timer_thread");
    }
  
    color_print("Started the payment_timer_thread","green");
  }
  return;

  #undef START_TIMER_THREADS_ERROR
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
  srand((unsigned int)time(NULL));

  // Variables
  char data[SMALL_BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int settings;
  int count;
  int count2;
  
  // define macros
  #define MESSAGE "{\"username\":\"XCASH\"}"
  
  #define database_reset \
  mongoc_client_pool_destroy(database_client_thread_pool); \
  mongoc_uri_destroy(uri_thread_pool); \
  mongoc_cleanup();

  #define MAIN_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"start_registration_mode",23); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  print_error_message(current_date_and_time,current_UTC_date_and_time,data); \
  mongoc_client_pool_destroy(database_client_thread_pool); \
  mongoc_uri_destroy(uri_thread_pool); \
  mongoc_cleanup(); \
  exit(0);

  memset(data,0,sizeof(data));

  initialize_data(parameters_count, parameters);

  // write the message
  color_print(XCASH_DPOPS_CURRENT_VERSION,"green");

  // check if they want to display the parameters
  if (parameters_count == 2 && strncmp(parameters[1],"--parameters",BUFFER_SIZE) == 0)
  {
    printf(INVALID_PARAMETERS_ERROR_MESSAGE);
    exit(0);
  }

  if (parameters_count == 2 && strncmp(parameters[1],"--generate-key",BUFFER_SIZE) == 0)
  {
    generate_key();
    exit(0);
  }

  create_overall_database_connection();

  settings = set_parameters(parameters_count, parameters);

  get_delegates_data();

  // check if the test are running to change the VRF secret key
  if (production_settings == 0)
  {
    memset(secret_key,0,sizeof(secret_key));
    memset(secret_key_data,0,sizeof(secret_key_data));
    if (strncmp(xcash_wallet_public_address,TEST_WALLET_1,XCASH_WALLET_LENGTH) == 0)
    {
      memcpy(secret_key,TEST_WALLET_SECRET_KEY_1,VRF_SECRET_KEY_LENGTH);
    }
    else if (strncmp(xcash_wallet_public_address,TEST_WALLET_2,XCASH_WALLET_LENGTH) == 0)
    {
      memcpy(secret_key,TEST_WALLET_SECRET_KEY_2,VRF_SECRET_KEY_LENGTH);
    }
    else if (strncmp(xcash_wallet_public_address,TEST_WALLET_3,XCASH_WALLET_LENGTH) == 0)
    {
      memcpy(secret_key,TEST_WALLET_SECRET_KEY_3,VRF_SECRET_KEY_LENGTH);
    }
    else if (strncmp(xcash_wallet_public_address,TEST_WALLET_4,XCASH_WALLET_LENGTH) == 0)
    {
      memcpy(secret_key,TEST_WALLET_SECRET_KEY_4,VRF_SECRET_KEY_LENGTH);
    }
    else if (strncmp(xcash_wallet_public_address,TEST_WALLET_5,XCASH_WALLET_LENGTH) == 0)
    {
      memcpy(secret_key,TEST_WALLET_SECRET_KEY_5,VRF_SECRET_KEY_LENGTH);
    }
    else if (strncmp(xcash_wallet_public_address,TEST_WALLET_6,XCASH_WALLET_LENGTH) == 0)
    {
      memcpy(secret_key,TEST_WALLET_SECRET_KEY_6,VRF_SECRET_KEY_LENGTH);
    }
    else if (strncmp(xcash_wallet_public_address,TEST_WALLET_7,XCASH_WALLET_LENGTH) == 0)
    {
      memcpy(secret_key,TEST_WALLET_SECRET_KEY_7,VRF_SECRET_KEY_LENGTH);
    }
    else if (strncmp(xcash_wallet_public_address,TEST_WALLET_8,XCASH_WALLET_LENGTH) == 0)
    {
      memcpy(secret_key,TEST_WALLET_SECRET_KEY_8,VRF_SECRET_KEY_LENGTH);
    }
    else if (strncmp(xcash_wallet_public_address,TEST_WALLET_9,XCASH_WALLET_LENGTH) == 0)
    {
      memcpy(secret_key,TEST_WALLET_SECRET_KEY_9,VRF_SECRET_KEY_LENGTH);
    }
    else if (strncmp(xcash_wallet_public_address,TEST_WALLET_10,XCASH_WALLET_LENGTH) == 0)
    {
      memcpy(secret_key,TEST_WALLET_SECRET_KEY_10,VRF_SECRET_KEY_LENGTH);
    }
    else if (strncmp(xcash_wallet_public_address,TEST_WALLET_11,XCASH_WALLET_LENGTH) == 0)
    {
      memcpy(secret_key,TEST_WALLET_SECRET_KEY_11,VRF_SECRET_KEY_LENGTH);
    }
    else if (strncmp(xcash_wallet_public_address,TEST_WALLET_12,XCASH_WALLET_LENGTH) == 0)
    {
      memcpy(secret_key,TEST_WALLET_SECRET_KEY_12,VRF_SECRET_KEY_LENGTH);
    }
    else if (strncmp(xcash_wallet_public_address,TEST_WALLET_13,XCASH_WALLET_LENGTH) == 0)
    {
      memcpy(secret_key,TEST_WALLET_SECRET_KEY_13,VRF_SECRET_KEY_LENGTH);
    }
    else if (strncmp(xcash_wallet_public_address,TEST_WALLET_14,XCASH_WALLET_LENGTH) == 0)
    {
      memcpy(secret_key,TEST_WALLET_SECRET_KEY_14,VRF_SECRET_KEY_LENGTH);
    }
    else if (strncmp(xcash_wallet_public_address,TEST_WALLET_15,XCASH_WALLET_LENGTH) == 0)
    {
      memcpy(secret_key,TEST_WALLET_SECRET_KEY_15,VRF_SECRET_KEY_LENGTH);
    }
    else if (strncmp(xcash_wallet_public_address,TEST_WALLET_16,XCASH_WALLET_LENGTH) == 0)
    {
      memcpy(secret_key,TEST_WALLET_SECRET_KEY_16,VRF_SECRET_KEY_LENGTH);
    }
    else if (strncmp(xcash_wallet_public_address,TEST_WALLET_17,XCASH_WALLET_LENGTH) == 0)
    {
      memcpy(secret_key,TEST_WALLET_SECRET_KEY_17,VRF_SECRET_KEY_LENGTH);
    }
    else if (strncmp(xcash_wallet_public_address,TEST_WALLET_18,XCASH_WALLET_LENGTH) == 0)
    {
      memcpy(secret_key,TEST_WALLET_SECRET_KEY_18,VRF_SECRET_KEY_LENGTH);
    }
    else if (strncmp(xcash_wallet_public_address,TEST_WALLET_19,XCASH_WALLET_LENGTH) == 0)
    {
      memcpy(secret_key,TEST_WALLET_SECRET_KEY_19,VRF_SECRET_KEY_LENGTH);
    }
    else if (strncmp(xcash_wallet_public_address,TEST_WALLET_20,XCASH_WALLET_LENGTH) == 0)
    {
      memcpy(secret_key,TEST_WALLET_SECRET_KEY_20,VRF_SECRET_KEY_LENGTH);
    }
    // convert the hexadecimal string to a string
    for (count = 0, count2 = 0; count < VRF_SECRET_KEY_LENGTH; count2++, count += 2)
    {
      memset(data,0,sizeof(data));
      memcpy(data,&secret_key[count],2);
      secret_key_data[count2] = (unsigned char)strtol(data, NULL, 16);
    }
  }

  // check if it should create the default database data
  memset(data,0,sizeof(data));
  if ((read_document_field_from_collection(database_name,"statistics",MESSAGE,"username",data) == 0) || (read_document_field_from_collection(database_name,"statistics",MESSAGE,"username",data) == 1 && count_all_documents_in_collection(database_name,"delegates") < NETWORK_DATA_NODES_AMOUNT))
  {
    delete_collection_from_database(database_name,"reserve_proofs_1");
    delete_collection_from_database(database_name,"delegates");
    delete_collection_from_database(database_name,"statistics");
    RESET_ERROR_MESSAGES;
    INITIALIZE_DATABASE_DATA(production_settings_database_data_settings);
  }

  print_settings();

  // check if the block verifier is a network data node
  CHECK_IF_BLOCK_VERIFIERS_IS_NETWORK_DATA_NODE; 

  // only network data nodes can use the registration mode
  if (registration_settings == 1 && network_data_node_settings == 0)
  {
    registration_settings = 0;
  }
 
  if (settings != 2)
  {
    database_sync_check();
    RESET_ERROR_MESSAGES;
  }
  
  // start the server
  if (create_server(1) == 0)
  {
    MAIN_ERROR("Could not start the server");
  }

  // wait until the blockchain is fully synced
  color_print("Checking if the blockchain is fully synced","yellow"); 
  
  while (check_if_blockchain_is_fully_synced() == 0)
  {
    color_print("The blockchain is not fully synced.\nWaiting until it is fully synced to continue (This might take a while)","yellow"); 
    sleep(60);
  }

  if (settings != 2)
  {
    start_timer_threads();
  }

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