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
#include "block_verifiers_sign_and_verify_messages.h"
#include "structures.h"
#include "variables.h"

#include "shared_delegate_website_thread_server_functions.h"

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
#include "block_verifiers_synchronize_functions.h"
#include "string_functions.h"
#include "VRF_functions.h"

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
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  long int current_time;
  size_t count = 0;
  size_t count2 = 0;

  // threads
  pthread_t thread_id[4];
  
  // define macros
  #define MESSAGE "{\"username\":\"XCASH\"}"
  
  #define database_reset \
  mongoc_client_destroy(database_client); \
  mongoc_client_pool_destroy(database_client_thread_pool); \
  mongoc_uri_destroy(uri_thread_pool); \
  mongoc_cleanup();

  #define MAIN_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"main",4); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  print_error_message(current_date_and_time,current_UTC_date_and_time,data); \
  database_reset; \
  exit(0);

  memset(data,0,sizeof(data));

  // initialize the global variables
  memset(current_block_height,0,sizeof(current_block_height));
  memset(current_round_part,0,sizeof(current_round_part));
  memset(current_round_part_backup_node,0,sizeof(current_round_part_backup_node));
  memset(secret_key,0,sizeof(secret_key));
  memset(secret_key_data,0,sizeof(secret_key_data));
  database_settings = 1;

  pthread_rwlock_init(&rwlock,NULL);
  pthread_rwlock_init(&rwlock_reserve_proofs,NULL);
  pthread_mutex_init(&lock, NULL);

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
    VRF_data.block_blob_signature[count] = (char*)calloc(XCASH_SIGN_DATA_LENGTH+1,sizeof(char));
   
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
    blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count] = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
    blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count] = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
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
    blockchain_data.transactions[count] = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_TRANSACTIONS_DATA,sizeof(char));

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

  // initialize the invalid_reserve_proofs and the block_verifiers_invalid_reserve_proofs
  for (count = 0; count < MAXIMUM_INVALID_RESERVE_PROOFS; count++)
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
    memcpy(error_message.data[error_message.total],"Could not create a connection for the database",46);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,data); 
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

  // check if it should create the default database data
  memset(data,0,sizeof(data));
  if (read_document_field_from_collection(DATABASE_NAME,"statistics",MESSAGE,"username",data,0) == 0)
  {
    INITIALIZE_DATABASE_DATA;
  }

  // set the current_round_part, current_round_part_backup_node and server message, this way the node will start at the begining of a round
  memset(current_round_part,0,sizeof(current_round_part));
  memset(current_round_part_backup_node,0,sizeof(current_round_part_backup_node));
  memcpy(current_round_part,"1",1);
  memcpy(current_round_part_backup_node,"0",1);

  // get the wallets public address
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

  // get the previous block hash
  if (get_previous_block_hash(previous_block_hash,0) == 0)
  {
    MAIN_ERROR("Could not get the previous block hash");
  }

  // set the default parameter settings
  total_threads = get_nprocs();
  delegates_website = 0;
  shared_delegates_website = 0;

  // check the parameters
  for (count = 0, count2 = 0; count < (size_t)parameters_count; count++)
  {    
    if (strncmp(parameters[count],"--test",BUFFER_SIZE) == 0)
    {
      test();
      database_reset;
      exit(0);
    }
    if (strncmp(parameters[count],"--generate_key",BUFFER_SIZE) == 0)
    {
      generate_key();
      database_reset;
      exit(0);
    }
    if (strncmp(parameters[count],"--synchronize_database_from_network_data_node",BUFFER_SIZE) == 0)
    {
      check_if_databases_are_synced(2,0);
      database_reset;
      exit(0);
    }
    if (strncmp(parameters[count],"--test_data_add",BUFFER_SIZE) == 0)
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
    if (strncmp(parameters[count],"--test_data_remove",BUFFER_SIZE) == 0)
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
    if (strncmp(parameters[count],"--disable_synchronizing_databases_and_starting_timers",BUFFER_SIZE) == 0)
    {
      count2 = 1;
    }
    if (strncmp(parameters[count],"--delegates_website",BUFFER_SIZE) == 0)
    {
      delegates_website = 1;
    }
    if (strncmp(parameters[count],"--shared_delegates_website",BUFFER_SIZE) == 0)
    {
      shared_delegates_website = 1;
    }
    if (strncmp(parameters[count],"--fee",BUFFER_SIZE) == 0 && count+1 != (size_t)parameters_count)
    {
      sscanf(parameters[count+1], "%lf", &fee);
    }
    if (strncmp(parameters[count],"--minimum_amount",BUFFER_SIZE) == 0 && count+1 != (size_t)parameters_count)
    {
      sscanf(parameters[count+1], "%lld", &minimum_amount);
    }
    if (strncmp(parameters[count],"--total_threads",BUFFER_SIZE) == 0 && count+1 != (size_t)parameters_count)
    {
      sscanf(parameters[count+1], "%d", &total_threads);
    }    
  }

  // get the secret key for signing messages
  memcpy(secret_key,BLOCK_VERIFIERS_SECRET_KEY,sizeof(BLOCK_VERIFIERS_SECRET_KEY)-1);
  
  // convert the hexadecimal string to a string
  for (count = 0, count2 = 0; count < sizeof(BLOCK_VERIFIERS_SECRET_KEY)-1; count2++, count += 2)
  {
    memset(data2,0,sizeof(data2));
    memcpy(data2,&secret_key[count],2);
    secret_key_data[count2] = (int)strtol(data2, NULL, 16);
  }

  if (count2 == 1)
  {
    goto disable_synchronizing_databases_and_starting_timers;
  }

  // check if the block verifier is a network data node
  CHECK_IF_BLOCK_VERIFIERS_IS_NETWORK_DATA_NODE;     
 
  // sync the block verifiers list
  if (sync_all_block_verifiers_list() == 0)
  {
    MAIN_ERROR("Could not sync the previous, current and next block verifiers list");
  }

  // check if the database is synced, unless this is the main network data node
  if (memcmp(xcash_wallet_public_address,NETWORK_DATA_NODE_1_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) != 0)
  {
    // check if all of the databases are synced from a network data node, since their is no way to tell if the previous round could not reach consensus
    if (check_if_databases_are_synced(2,0) == 0)
    {
      MAIN_ERROR("Could not check if the databases are synced");
    }
  }

  // check the block verifiers current time, if it is not a network data node
  if (network_data_node_settings != 1)
  {
    color_print("Checking if the current time is synchronized with the network","green");
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
        print_error_message(current_date_and_time,current_UTC_date_and_time,data); 
        database_reset;
        exit(0);
      }

      if (verify_data(data,0,0) == 0)
      {
        memcpy(error_message.function[error_message.total],"main",4);
        memcpy(error_message.data[error_message.total],"Could not verify the data from ",31);
        memcpy(error_message.data[error_message.total]+31,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],sizeof(data2)));
        error_message.total++;
        print_error_message(current_date_and_time,current_UTC_date_and_time,data); 
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
        print_error_message(current_date_and_time,current_UTC_date_and_time,data); 
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

  /*print_start_message(current_date_and_time,current_UTC_date_and_time,"Starting all of the threads",data);

  // start the current block height timer thread
  if (pthread_create(&thread_id[0], NULL, &current_block_height_timer_thread, NULL) != 0 && pthread_detach(thread_id[0]) != 0)
  {
    MAIN_ERROR("Could not start the current_block_height_timer_thread");
  }
  
  color_print("Started the current block height timer thread","green");*/

  /*// start the check_reserve_proofs_timer_thread
  if (pthread_create(&thread_id[1], NULL, &check_reserve_proofs_timer_thread, NULL) != 0 && pthread_detach(thread_id[1]) != 0)
  {
    MAIN_ERROR("Could not start the check_reserve_proofs_timer_thread");
  }

  color_print("Started the check reserve proofs timer thread","green");*/

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