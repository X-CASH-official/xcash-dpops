#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <pthread.h>
#include <sys/sysinfo.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "database_functions.h"
#include "network_daemon_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "network_wallet_functions.h"
#include "server_functions.h"
#include "block_verifiers_synchronize_functions.h"
#include "string_functions.h"
#include "thread_server_functions.h"

#include "XCASH_DPOPS_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Main function
-----------------------------------------------------------------------------------------------------------
*/

int main(int parameters_count, char* parameters[])
{
  // iniltize the random number generator
  srand(time(NULL));

  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  long int current_time;
  size_t count = 0;
  size_t count2 = 0; 
  int settings;

  // threads
  pthread_t thread_id[5];
  
  // define macros
  #define database_reset \
  mongoc_client_destroy(database_client); \
  mongoc_client_pool_destroy(database_client_thread_pool); \
  mongoc_uri_destroy(uri_thread_pool); \
  mongoc_cleanup();

  #define MAIN_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"main",4); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  print_error_message; \
  database_reset; \
  exit(0);

  memset(data,0,sizeof(data));

  // initialize the global variables
  memset(block_verifiers_IP_address,0,sizeof(block_verifiers_IP_address));
  memset(current_block_height,0,sizeof(current_block_height));
  memset(current_round_part,0,sizeof(current_round_part));
  memset(current_round_part_backup_node,0,sizeof(current_round_part_backup_node));
  current_block_verifier_settings = 0;
  database_settings = 1;

  pthread_rwlock_init(&rwlock,NULL);

  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    // initialize the previous, current and next block_verifiers_list struct 
    memset(previous_block_verifiers_list.block_verifiers_name[count],0,sizeof(previous_block_verifiers_list.block_verifiers_name[count]));
    memset(previous_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_public_address[count]));
    memset(previous_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_IP_address[count]));

    memset(current_block_verifiers_list.block_verifiers_name[count],0,sizeof(current_block_verifiers_list.block_verifiers_name[count]));
    memset(current_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_public_address[count]));
    memset(current_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_IP_address[count]));

    memset(next_block_verifiers_list.block_verifiers_name[count],0,sizeof(next_block_verifiers_list.block_verifiers_name[count]));
    memset(next_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_public_address[count]));
    memset(next_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_IP_address[count]));

    // initialize the synced_block_verifiers struct 
    memset(synced_block_verifiers.synced_block_verifiers_public_address[count],0,sizeof(synced_block_verifiers.synced_block_verifiers_public_address[count]));
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

  // add the network_data_nodes
  memcpy(network_data_nodes_list.network_data_nodes_public_address[0],NETWORK_DATA_NODE_1_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH);
  memcpy(network_data_nodes_list.network_data_nodes_IP_address[0],NETWORK_DATA_NODE_1_IP_ADDRESS,strnlen(NETWORK_DATA_NODE_1_IP_ADDRESS,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
  memcpy(network_data_nodes_list.network_data_nodes_public_address[1],NETWORK_DATA_NODE_2_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH);
  memcpy(network_data_nodes_list.network_data_nodes_IP_address[1],NETWORK_DATA_NODE_2_IP_ADDRESS,strnlen(NETWORK_DATA_NODE_2_IP_ADDRESS,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));

  // set the network_data_node_settings
  network_data_node_settings = 0;

  // initialize the current_round_part_vote_data struct
  memset(current_round_part_vote_data.current_vote_results,0,sizeof(current_round_part_vote_data.current_vote_results));

  // initialize the VRF_data struct 
  VRF_data.vrf_secret_key_data_round_part_4 = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  VRF_data.vrf_secret_key_round_part_4 = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  VRF_data.vrf_public_key_data_round_part_4 = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  VRF_data.vrf_public_key_round_part_4 = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  VRF_data.vrf_alpha_string_data_round_part_4 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  VRF_data.vrf_alpha_string_round_part_4 = (unsigned char*)calloc(BUFFER_SIZE,sizeof(char));
  VRF_data.vrf_proof_data_round_part_4 = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  VRF_data.vrf_proof_round_part_4 = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  VRF_data.vrf_beta_string_data_round_part_4 = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  VRF_data.vrf_beta_string_round_part_4 = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  VRF_data.block_blob = (char*)calloc(BUFFER_SIZE,sizeof(char));
  VRF_data.reserve_bytes_data_hash = (char*)calloc(DATA_HASH_LENGTH+1,sizeof(char));

  // check if the memory needed was allocated on the heap successfully
  if (VRF_data.vrf_public_key_data_round_part_4 == NULL || VRF_data.vrf_public_key_round_part_4 == NULL || VRF_data.vrf_alpha_string_data_round_part_4 == NULL || VRF_data.vrf_alpha_string_round_part_4 == NULL || VRF_data.vrf_proof_data_round_part_4 == NULL || VRF_data.vrf_proof_round_part_4 == NULL || VRF_data.vrf_beta_string_data_round_part_4 == NULL || VRF_data.vrf_beta_string_round_part_4 == NULL || VRF_data.block_blob == NULL)
  {
    memcpy(error_message.function[error_message.total],"main",4);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    VRF_data.block_verifiers_vrf_secret_key_data[count] = (char*)calloc(VRF_SECRET_KEY_LENGTH+1,sizeof(char));
    VRF_data.block_verifiers_vrf_secret_key[count] = (unsigned char*)calloc(crypto_vrf_SECRETKEYBYTES+1,sizeof(char));
    VRF_data.block_verifiers_vrf_public_key_data[count] = (char*)calloc(VRF_PUBLIC_KEY_LENGTH+1,sizeof(char));
    VRF_data.block_verifiers_vrf_public_key[count] = (unsigned char*)calloc(crypto_vrf_PUBLICKEYBYTES+1,sizeof(char));
    VRF_data.block_verifiers_random_data[count] = (char*)calloc(RANDOM_STRING_LENGTH+1,sizeof(char));
    VRF_data.block_blob_signature[count] = (char*)calloc(XCASH_SIGN_DATA_LENGTH+1,sizeof(char));
   
    // check if the memory needed was allocated on the heap successfully
    if (VRF_data.block_blob_signature[count] == NULL || VRF_data.block_verifiers_random_data[count] == NULL)
    {
      memcpy(error_message.function[error_message.total],"main",4);
      memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
      error_message.total++;
      print_error_message;  
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
  blockchain_data.blockchain_reserve_bytes.vrf_secret_key_round_part_4 = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_4 = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_4 = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_4 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_4 = (unsigned char*)calloc(BUFFER_SIZE,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_4 = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_4 = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_4 = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_4 = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_4 = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.previous_block_hash_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));

  // check if the memory needed was allocated on the heap successfully
  if (blockchain_data.network_version_data == NULL || blockchain_data.timestamp_data == NULL || blockchain_data.previous_block_hash_data == NULL || blockchain_data.nonce_data == NULL || blockchain_data.block_reward_transaction_version_data == NULL || blockchain_data.unlock_block_data == NULL || blockchain_data.block_reward_input_data == NULL || blockchain_data.vin_type_data == NULL || blockchain_data.block_height_data == NULL || blockchain_data.block_reward_output_data == NULL || blockchain_data.block_reward_data == NULL || blockchain_data.stealth_address_output_tag_data == NULL || blockchain_data.stealth_address_output_data == NULL || blockchain_data.extra_bytes_size_data == NULL || blockchain_data.transaction_public_key_tag_data == NULL || blockchain_data.transaction_public_key_data == NULL || blockchain_data.extra_nonce_tag_data == NULL || blockchain_data.reserve_bytes_size_data == NULL || blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data == NULL || blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name == NULL || blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data == NULL || blockchain_data.blockchain_reserve_bytes.block_producer_public_address == NULL || blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data == NULL || blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data == NULL || blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names == NULL || blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_4 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_4 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_4 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_4 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_4 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_4 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_4 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_4 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_4 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_data == NULL || blockchain_data.blockchain_reserve_bytes.previous_block_hash_data == NULL)
  {
    memcpy(error_message.function[error_message.total],"main",4);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data[count] = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
    blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address[count] = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
    blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count] = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
    blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count] = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
    blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key_data[count] = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
    blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key[count] = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
    blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key_data[count] = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
    blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key[count] = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
    blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data[count] = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
    blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data_text[count] = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));

    // check if the memory needed was allocated on the heap successfully
    if (blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data[count] == NULL || blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address[count] == NULL || blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count] == NULL || blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count] == NULL || blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key_data[count] == NULL || blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key[count] == NULL || blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key_data[count] == NULL || blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key[count] == NULL)
    {
      memcpy(error_message.function[error_message.total],"main",4);
      memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
      error_message.total++;
      print_error_message;  
      exit(0);
    }
  }
  blockchain_data.ringct_version_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.transaction_amount_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  for (count = 0; count < MAXIMUM_TRANSACATIONS_PER_BLOCK; count++)
  {
    blockchain_data.transactions[count] = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_TRANSACTIONS_DATA,sizeof(char));

    // check if the memory needed was allocated on the heap successfully
    if (blockchain_data.transactions[count] == NULL)
    {
      memcpy(error_message.function[error_message.total],"main",4);
      memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
      error_message.total++;
      print_error_message;  
      exit(0);
    }
  }

  // initialize the error_message struct
  for (count = 0; count < DATABASE_ARRAY_COUNT; count++)
  {
    memset(error_message.function[count],0,sizeof(error_message.function[count]));
    memset(error_message.data[count],0,sizeof(error_message.data[count]));
  }
  error_message.total = 0;

  // initialize the invalid_reserve_proofs and the block_verifiers_invalid_reserve_proofs
  for (count = 0; count < MAXIMUM_INVALID_RESERERVE_PROOFS; count++)
  {
    invalid_reserve_proofs.block_verifier_public_address[count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    invalid_reserve_proofs.public_address[count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    invalid_reserve_proofs.reserve_proof[count] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char)); 
  }
  invalid_reserve_proofs.count = 0;

  // write the message
  color_print("XCASH DPOPS - Version 1.0.0\n","green");

  // check if they want to display the parameters
  if (parameters_count == 2 && strncmp(parameters[1],"--parameters",BUFFER_SIZE) == 0)
  {
    printf(INVALID_PARAMETERS_ERROR_MESSAGE);
    exit(0);
  }

  // initialize the database connection
  mongoc_init();

  // create a connection to the database
  if (create_database_connection() == 0)
  {
    memcpy(error_message.function[error_message.total],"main",4);
    memcpy(error_message.data[error_message.total],"Could not create a connection for the database",48);
    error_message.total++;
    print_error_message; 
    mongoc_cleanup();
    exit(0);
  }

  // create a pool of connections for the database
  mongoc_uri_t* uri_thread_pool;
  bson_error_t error;
  uri_thread_pool = mongoc_uri_new_with_error(DATABASE_CONNECTION, &error);
  if (!uri_thread_pool)
  {
    memcpy(error_message.function[error_message.total],"main",4);
    memcpy(error_message.data[error_message.total],"Could not create a pool of connections for the database",55);
    error_message.total++;
    print_error_message;
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
    print_error_message; 
    mongoc_client_destroy(database_client);
    mongoc_uri_destroy(uri_thread_pool);
    mongoc_cleanup();
    exit(0);
  }

  // set the current_round_part, current_round_part_backup_node and server message, this way the node will start at the begining of a round
  memset(current_round_part,0,sizeof(current_round_part));
  memset(current_round_part_backup_node,0,sizeof(current_round_part_backup_node));
  memcpy(current_round_part,"1",1);
  memcpy(current_round_part_backup_node,"0",1);

  // get the wallets public address
  fprintf(stderr,"Getting the public address\n");
  if (get_public_address(0) == 1)
  {  
    // print the public address
    memcpy(data,"Successfully received the public address:",41);
    memcpy(data+41,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
    color_print(data,"green");
  }
  else
  {
    MAIN_ERROR("Could not get the wallets public address");
  }

  // get the current block height
  if (get_current_block_height(current_block_height,0) == 0)
  {
    MAIN_ERROR("Could not get the current block height");
  }

  // set the default parameter settings
  total_threads = get_nprocs()*2;
  delegates_website = 0;
  shared_delegates_website = 0;

  // check if the program needs to run the test
  if (parameters_count == 2)
  {    
    if (strncmp(parameters[1],"--test",BUFFER_SIZE) == 0)
    {
      test();
      database_reset;
      exit(0);
    }
    else if (strncmp(parameters[1],"--synchronize_database_from_network_data_node",BUFFER_SIZE) == 0)
    {
      check_if_databases_are_synced(2);
      database_reset;
      exit(0);
    }
    else if (strncmp(parameters[1],"--delegates_website",BUFFER_SIZE) == 0)
    {
      delegates_website = 1;
    }
    else if (strncmp(parameters[1],"--test_data_add",BUFFER_SIZE) == 0)
    {
      memset(data,0,sizeof(data));
      
      insert_document_into_collection_json(DATABASE_NAME_DELEGATES,"blocks_found","{\"block_height\":\"5\",\"block_hash\":\"BLOCK_HASH\",\"block_date_and_time\":\"10\",\"block_reward\":\"15\",\"block_count\":\"10\"}",0);
      insert_document_into_collection_json(DATABASE_NAME_DELEGATES,"blocks_found","{\"block_height\":\"5\",\"block_hash\":\"BLOCK_HASH\",\"block_date_and_time\":\"10\",\"block_reward\":\"15\",\"block_count\":\"10\"}",0);

      memset(data,0,strlen(data));
      memcpy(data,"{\"public_address\":\"",19);
      memcpy(data+19,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      memcpy(data+117,"\",\"current_total\":\"5\",\"total\":\"10\",\"inactivity_count\":\"15\"}",59);
      insert_document_into_collection_json(DATABASE_NAME_DELEGATES,"public_addresses",data,0);

      memset(data,0,strlen(data));
      memcpy(data,"{\"public_address\":\"",19);
      memcpy(data+19,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      memcpy(data+117,"\",\"date_and_time\":\"5\",\"total\":\"10\",\"tx_hash\":\"TX_HASH\",\"tx_key\":\"TX_KEY\"}",73);
      insert_document_into_collection_json(DATABASE_NAME_DELEGATES,"public_addresses_payments",data,0);
      insert_document_into_collection_json(DATABASE_NAME_DELEGATES,"public_addresses_payments",data,0);

      memset(data,0,strlen(data));
      memcpy(data,"{\"public_address_created_reserve_proof\":\"",41);
      memcpy(data+41,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      memcpy(data+139,"\",\"public_address_voted_for\":\"",30);
      memcpy(data+169,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      memcpy(data+267,"\",\"total\":\"10\",\"reserve_proof\":\"15\"}",36);
      insert_document_into_collection_json(DATABASE_NAME,"reserve_proofs_5",data,0);
      insert_document_into_collection_json(DATABASE_NAME,"reserve_proofs_10",data,0);
      insert_document_into_collection_json(DATABASE_NAME,"reserve_proofs_10",data,0);
      insert_document_into_collection_json(DATABASE_NAME,"reserve_proofs_15",data,0);

      color_print("The database test data has been added successfully\n","green");
      database_reset;
      exit(0);
    }
    else if (strncmp(parameters[1],"--test_data_remove",BUFFER_SIZE) == 0)
    {
      memset(data,0,strlen(data));
      
      delete_document_from_collection(DATABASE_NAME_DELEGATES,"blocks_found","{\"block_height\":\"5\"}",0);
      delete_document_from_collection(DATABASE_NAME_DELEGATES,"blocks_found","{\"block_height\":\"5\"}",0);

      memset(data,0,strlen(data));
      memcpy(data,"{\"public_address\":\"",19);
      memcpy(data+19,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      memcpy(data+117,"\"}",2);
      delete_document_from_collection(DATABASE_NAME_DELEGATES,"public_addresses",data,0);

      memset(data,0,strlen(data));
      memcpy(data,"{\"public_address\":\"",19);
      memcpy(data+19,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      memcpy(data+117,"\"}",2);
      delete_document_from_collection(DATABASE_NAME_DELEGATES,"public_addresses_payments",data,0);
      delete_document_from_collection(DATABASE_NAME_DELEGATES,"public_addresses_payments",data,0);

      memset(data,0,strlen(data));
      memcpy(data,"{\"public_address_voted_for\":\"",29);
      memcpy(data+29,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      memcpy(data+127,"\"}",2);
      delete_document_from_collection(DATABASE_NAME,"reserve_proofs_5",data,0);
      delete_document_from_collection(DATABASE_NAME,"reserve_proofs_10",data,0);
      delete_document_from_collection(DATABASE_NAME,"reserve_proofs_10",data,0);
      delete_document_from_collection(DATABASE_NAME,"reserve_proofs_15",data,0);

      color_print("The database test data has been removed successfully\n","green");
      database_reset;
      exit(0);
    }
    else
    {
      memcpy(error_message.function[error_message.total],"main",4);
      memcpy(error_message.data[error_message.total],"Invalid parameters",18);
      error_message.total++;
      print_error_message; 
      printf(INVALID_PARAMETERS_ERROR_MESSAGE);
      database_reset;
      exit(0);
    }      
  }
  else if (parameters_count == 6)
  {
    if (strncmp(parameters[1],"--shared_delegates_website",BUFFER_SIZE) == 0 && strncmp(parameters[2],"--fee",BUFFER_SIZE) == 0 && strncmp(parameters[4],"--minimum_amount",BUFFER_SIZE) == 0)
    {
      shared_delegates_website = 1;
      sscanf(parameters[3], "%lf", &fee);
      sscanf(parameters[5], "%lld", &minimum_amount);
    }
    else
    {
      memcpy(error_message.function[error_message.total],"main",4);
      memcpy(error_message.data[error_message.total],"Invalid parameters",18);
      error_message.total++;
      print_error_message; 
      printf(INVALID_PARAMETERS_ERROR_MESSAGE);
      database_reset;
      exit(0);
    }      
  }

  for (count = 0; count < (size_t)parameters_count; count++)
  {    
    if (strncmp(parameters[count],"--total_threads",BUFFER_SIZE) == 0)
    {
      sscanf(parameters[count+1], "%d", &total_threads);
    }
    if (strncmp(parameters[count],"--disable_synchronizing_databases_and_starting_timers",BUFFER_SIZE) == 0)
    {
      goto disable_synchronizing_databases_and_starting_timers;
    }
  }

  start:

  // get the block verifiers IP address
  memset(data,0,sizeof(data));
  memcpy(data,"{\"public_address\":\"",19);
  memcpy(data+19,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(data+117,"\"}",2);
  if (read_document_field_from_collection(DATABASE_NAME,"delegates",data,"IP_address",block_verifiers_IP_address,0) == 0)
  {
    MAIN_ERROR("Could not get the block verifiers IP address");
  }

  // check if the block verifier has any of the databases
  if (strncmp(block_verifiers_IP_address,"",1) == 0)
  {
    // check if all of the databases are synced
    color_print("\nCould not find your IP address in the database. This is because your database is out of sync, or you have not registered as a delegate.\nIf this process loops a few times, then make sure you have registered by visting the delegates website.","red");
    
    // sync the databases and then recheck if the block verifier is a network data node
    if (sync_all_block_verifiers_list() == 0)
    {
      MAIN_ERROR("Could not sync the previous, current and next block verifiers list");
    }
    /*if (check_if_databases_are_synced() == 0)
    {
      MAIN_ERROR("Could not check if the databases are synced");
    }*/
    goto start;
  }

  // check if the block verifier is a network data node
  for (count = 0; count < NETWORK_DATA_NODES_AMOUNT; count++)
  {
    if (strncmp(block_verifiers_IP_address,network_data_nodes_list.network_data_nodes_IP_address[count],BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0)
    {
      network_data_node_settings = 1;
    }
  }

  // check if all of the network data nodes are offline
  if (network_data_node_settings == 1)
  {
    for (count = 0, count2 = 0; count < NETWORK_DATA_NODES_AMOUNT; count++)
    {
      if (strncmp(network_data_nodes_list.network_data_nodes_IP_address[count],block_verifiers_IP_address,BUFFER_SIZE) != 0)
      {
        if (get_delegate_online_status(network_data_nodes_list.network_data_nodes_IP_address[count]) == 0)
        {
          count2++;
        }
      }      
    }
  }

  // check if all of the network data nodes are offline
  if (network_data_node_settings == 1)
  {
    for (count = 0, count2 = 0; count < NETWORK_DATA_NODES_AMOUNT; count++)
    {
      if (get_delegate_online_status(network_data_nodes_list.network_data_nodes_IP_address[count]) == 0)
      {
        count2++;
      }
    }
    // sync the previous, current and next block verifiers list
    if (sync_all_block_verifiers_list() == 0)
    {
      MAIN_ERROR("Could not sync the previous, current and next block verifiers list");
    }
    if (count2 != NETWORK_DATA_NODES_AMOUNT)
    {
      // check if all of the databases are synced
      /*if (check_if_databases_are_synced() == 0)
      {
        MAIN_ERROR("Could not check if the databases are synced");
      }*/
    }
  }
  else
  {
    // sync the previous, current and next block verifiers list
    if (sync_all_block_verifiers_list() == 0)
    {
      MAIN_ERROR("Could not sync the previous, current and next block verifiers list");
    }

    /*// check if all of the databases are synced
    if (check_if_databases_are_synced() == 0)
    {
      MAIN_ERROR("Could not check if the databases are synced");
    }*/
  }

  // check the block verifiers current time, if it is not a network data node
  color_print("Checking if the current time is synchronized with the network","green");
  for (count = 0, settings = 0; count < NETWORK_DATA_NODES_AMOUNT; count++)
  {
    if (memcmp(network_data_nodes_list.network_data_nodes_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0)
    {
      settings = 1;
      break;
    }
  }

  if (settings != 1)
  {
    for (count = 0; count < NETWORK_DATA_NODES_AMOUNT; count++)
    {
      // create the message
      memset(data,0,sizeof(data));
      memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_NETWORK_DATA_NODE_BLOCK_VERIFIERS_CURRENT_TIME\",\r\n}",95);

      // sign_data
      if (sign_data(data,0) == 0)
      { 
        MAIN_ERROR("Could not sign the data");
      }

      if (send_and_receive_data_socket(data2,network_data_nodes_list.network_data_nodes_IP_address[count],SEND_DATA_PORT,data,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
      {
        memcpy(error_message.function[error_message.total],"main",4);
        memcpy(error_message.data[error_message.total],"Could not receive data from ",28);
        memcpy(error_message.data[error_message.total]+28,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],sizeof(data2)));
        error_message.total++;
        print_error_message; 
        database_reset;
        exit(0);
      }

      if (verify_data(data,0,0) == 0)
      {
        memcpy(error_message.function[error_message.total],"main",4);
        memcpy(error_message.data[error_message.total],"Could not verify the data from ",31);
        memcpy(error_message.data[error_message.total]+31,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],sizeof(data2)));
        error_message.total++;
        print_error_message; 
        database_reset;
        exit(0);
     }

      // parse the message
      memset(data,0,sizeof(data));
      if (parse_json_data(data2,"current_time",data,sizeof(data)) == 0 || memcmp(data,"",1) == 0)
      {
        memcpy(error_message.function[error_message.total],"main",4);
        memcpy(error_message.data[error_message.total],"Could not receive data from ",28);
        memcpy(error_message.data[error_message.total]+28,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],sizeof(data2)));
        error_message.total++;
        print_error_message; 
        database_reset;
        exit(0);
      }

      sscanf(data,"%ld", &current_time);

      if (time(NULL) - current_time > 2)
      {
        MAIN_ERROR("Invalid current time");
      }
    }
  }
 
  // print_start_message(current_date_and_time,current_UTC_date_and_time,"Starting all of the threads");

  // start the current block height timer thread
  if (pthread_create(&thread_id[0], NULL, &current_block_height_timer_thread, NULL) != 0 && pthread_detach(thread_id[0]) != 0)
  {
    MAIN_ERROR("Could not start the current_block_height_timer_thread");
  }
  
  color_print("Started the current block height timer thread","green");

  /*// start the check_reserve_proofs_timer_thread
  if (pthread_create(&thread_id[1], NULL, &check_reserve_proofs_timer_thread, NULL) != 0 && pthread_detach(thread_id[1]) != 0)
  {
    MAIN_ERROR("Could not start the check_reserve_proofs_timer_thread");
  }

  color_print("Started the check reserve proofs timer thread","green");*/

  /*// start the check_delegates_online_status_timer_thread
  if (pthread_create(&thread_id[2], NULL, &check_delegates_online_status_timer_thread, NULL) != 0 && pthread_detach(thread_id[2]) != 0)
  {
    MAIN_ERROR("Could not start the check_delegates_online_status_timer_thread");
  }

  color_print("Started the check delegates online status timer thread","green");*/

  /*// start the block height timer thread
  if (shared_delegates_website == 1)
  {
    if (pthread_create(&thread_id[3], NULL, &block_height_timer_thread, NULL) != 0 && pthread_detach(thread_id[3]) != 0)
    {
      MAIN_ERROR("Could not start the block_height_timer_thread");
    }
  
    color_print("Started the current block height timer thread","green");

    // start the payment timer thread
    if (pthread_create(&thread_id[4], NULL, &payment_timer_thread, NULL) != 0 && pthread_detach(thread_id[4]) != 0)
    {
      MAIN_ERROR("Could not start the block_height_timer_thread");
    }
  
    color_print("Started the payment_timer_thread","green");
   }*/

   disable_synchronizing_databases_and_starting_timers:
  
  // start the server
  if (create_server(1) == 0)
  {
    MAIN_ERROR("Could not start the server");
  }

  database_reset;
  return 0;  

  #undef database_reset
}