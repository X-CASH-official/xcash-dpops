#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <pthread.h>
#include <sys/mman.h>
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
#include "thread_server_functions.h"

#include "xcash_proof_of_stake_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Main function
-----------------------------------------------------------------------------------------------------------
*/

int main(int parameters_count, char* parameters[])
{
  // iniltize the random number generator
  srand(time(0));

  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  size_t count = 0;
  size_t count2 = 0;

  // threads
  pthread_t thread_id_1;
  pthread_t thread_id_2;
  pthread_t thread_id_3;

  // define macros
  #define database_reset \
  mongoc_client_destroy(database_client); \
  mongoc_client_pool_destroy(database_client_thread_pool); \
  mongoc_uri_destroy(uri_thread_pool); \
  mongoc_cleanup();

  // initialize the global variables
  xcash_wallet_public_address = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  block_verifiers_IP_address = (char*)calloc(BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH,sizeof(char)); 
  current_round_part = (char*)calloc(BUFFER_SIZE,sizeof(char));
  current_round_part_backup_node = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || xcash_wallet_public_address == NULL || block_verifiers_IP_address == NULL || current_round_part == NULL || current_round_part_backup_node == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (xcash_wallet_public_address != NULL)
    {
      pointer_reset(xcash_wallet_public_address);
    }
    if (block_verifiers_IP_address != NULL)
    {
      pointer_reset(block_verifiers_IP_address);
    }
    if (current_round_part != NULL)
    {
      pointer_reset(current_round_part);
    }
    if (current_round_part_backup_node != NULL)
    {
      pointer_reset(current_round_part_backup_node);
    }
    memcpy(error_message.function[error_message.total],"main",4);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  } 

  memset(current_block_height,0,sizeof(current_block_height));

  pthread_rwlock_init(&rwlock,NULL);

  // initialize the previous block_verifiers_list struct 
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    previous_block_verifiers_list.block_verifiers_name[count] = (char*)calloc(BLOCK_VERIFIERS_NAME_TOTAL_LENGTH+1,sizeof(char));
    previous_block_verifiers_list.block_verifiers_public_address[count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    previous_block_verifiers_list.block_verifiers_IP_address[count] = (char*)calloc(BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH,sizeof(char));

    // check if the memory needed was allocated on the heap successfully
    if (previous_block_verifiers_list.block_verifiers_name[count] == NULL || previous_block_verifiers_list.block_verifiers_public_address[count] == NULL || previous_block_verifiers_list.block_verifiers_IP_address[count] == NULL)
    {
      memcpy(error_message.function[error_message.total],"main",4);
      memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
      error_message.total++;
      print_error_message;  
      exit(0);
    }
  }

  // initialize the current block_verifiers_list struct 
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    current_block_verifiers_list.block_verifiers_name[count] = (char*)calloc(BLOCK_VERIFIERS_NAME_TOTAL_LENGTH+1,sizeof(char));
    current_block_verifiers_list.block_verifiers_public_address[count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    current_block_verifiers_list.block_verifiers_IP_address[count] = (char*)calloc(BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH,sizeof(char));

    // check if the memory needed was allocated on the heap successfully
    if (current_block_verifiers_list.block_verifiers_name[count] == NULL || current_block_verifiers_list.block_verifiers_public_address[count] == NULL || current_block_verifiers_list.block_verifiers_IP_address[count] == NULL)
    {
      memcpy(error_message.function[error_message.total],"main",4);
      memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
      error_message.total++;
      print_error_message;  
      exit(0);
    }
  }

  // initialize the next_block_verifiers_list struct 
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    next_block_verifiers_list.block_verifiers_name[count] = (char*)calloc(BLOCK_VERIFIERS_NAME_TOTAL_LENGTH+1,sizeof(char));
    next_block_verifiers_list.block_verifiers_public_address[count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    next_block_verifiers_list.block_verifiers_IP_address[count] = (char*)calloc(BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH,sizeof(char));

    // check if the memory needed was allocated on the heap successfully
    if (next_block_verifiers_list.block_verifiers_name[count] == NULL || next_block_verifiers_list.block_verifiers_public_address[count] == NULL || next_block_verifiers_list.block_verifiers_IP_address[count] == NULL)
    {
      memcpy(error_message.function[error_message.total],"main",4);
      memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
      error_message.total++;
      print_error_message;  
      exit(0);
    }
  }

  // initialize the synced_block_verifiers struct 
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memset(synced_block_verifiers.synced_block_verifiers_public_address[count],0,sizeof(synced_block_verifiers.synced_block_verifiers_public_address[count]));
    memset(synced_block_verifiers.synced_block_verifiers_IP_address[count],0,sizeof(synced_block_verifiers.synced_block_verifiers_IP_address[count]));
    memset(synced_block_verifiers.vote_settings[count],0,sizeof(synced_block_verifiers.vote_settings[count]));
  }
  synced_block_verifiers.vote_settings_true = 0;
  synced_block_verifiers.vote_settings_false = 0;

  // initialize the main_nodes_list struct 
  main_nodes_list.block_producer_public_address = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  main_nodes_list.block_producer_IP_address = (char*)calloc(BUFFER_SIZE,sizeof(char));
  main_nodes_list.block_producer_backup_block_verifier_1_public_address = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  main_nodes_list.block_producer_backup_block_verifier_1_IP_address = (char*)calloc(BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH,sizeof(char));
  main_nodes_list.block_producer_backup_block_verifier_2_public_address = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  main_nodes_list.block_producer_backup_block_verifier_2_IP_address = (char*)calloc(BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH,sizeof(char));
  main_nodes_list.block_producer_backup_block_verifier_3_public_address = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  main_nodes_list.block_producer_backup_block_verifier_3_IP_address = (char*)calloc(BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH,sizeof(char));
  main_nodes_list.block_producer_backup_block_verifier_4_public_address = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  main_nodes_list.block_producer_backup_block_verifier_4_IP_address = (char*)calloc(BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH,sizeof(char));
  main_nodes_list.block_producer_backup_block_verifier_5_public_address = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  main_nodes_list.block_producer_backup_block_verifier_5_IP_address = (char*)calloc(BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH,sizeof(char));

  // initialize the network_data_nodes_list struct
  for (count = 0; count < NETWORK_DATA_NODES_AMOUNT; count++)
  {
    network_data_nodes_list.network_data_nodes_public_address[count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    network_data_nodes_list.network_data_nodes_IP_address[count] = (char*)calloc(BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH,sizeof(char));

    // check if the memory needed was allocated on the heap successfully
    if (network_data_nodes_list.network_data_nodes_public_address[count] == NULL || network_data_nodes_list.network_data_nodes_IP_address[count] == NULL)
    {
      memcpy(error_message.function[error_message.total],"main",4);
      memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
      error_message.total++;
      print_error_message;  
      exit(0);
    }
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
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    invalid_reserve_proofs.block_verifier_public_address[count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    invalid_reserve_proofs.public_address[count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    invalid_reserve_proofs.reserve_proof[count] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char)); 
  }
  invalid_reserve_proofs.count = 0;

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
  memset(current_round_part,0,strlen(current_round_part));
  memset(current_round_part_backup_node,0,strlen(current_round_part_backup_node));
  memcpy(current_round_part,"1",1);
  memcpy(current_round_part_backup_node,"0",1);

  // write the message
  color_print("X-CASH Proof Of Stake, Version 1.0.0\n","green");

  // get the wallets public address
  printf("Getting the public address\n");
  if (get_public_address(0) == 1)
  {  
    // print the public address
    memcpy(data,"Successfully received the public address:",41);
    memcpy(data+41,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
    color_print(data,"green");
  }
  else
  {
    memcpy(error_message.function[error_message.total],"main",4);
    memcpy(error_message.data[error_message.total],"Could not get the wallets public address",40);
    error_message.total++;
    print_error_message; 
    database_reset;
    pointer_reset(data);
    exit(0);
  }

  // check if the program needs to run the test
  if (parameters_count == 2)
  {
    if (strncmp(parameters[1],"--test",BUFFER_SIZE) == 0)
    {
      test();
    }
    else
    {
      memcpy(error_message.function[error_message.total],"main",4);
      memcpy(error_message.data[error_message.total],"Invalid parameters",18);
      error_message.total++;
      print_error_message; 
      printf(INVALID_PARAMETERS_ERROR_MESSAGE);
    }  
    database_reset;
    pointer_reset(data);
    exit(0);
  }

  // get the current block height
  if (get_current_block_height(current_block_height,0) == 0)
  {
    memcpy(error_message.function[error_message.total],"main",4);
    memcpy(error_message.data[error_message.total],"Could not get the current block height",38);
    error_message.total++;
    print_error_message; 
    database_reset;
    pointer_reset(data);
    exit(0);
  }

  start:

  // get the block verifiers IP address
  memset(data,0,strlen(data));
  memcpy(data,"{\"public_address\":\"",19);
  memcpy(data+19,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(data+117,"\"}",2);
  if (read_document_field_from_collection(DATABASE_NAME,"delegates",data,"IP_address",block_verifiers_IP_address,0) == 0)
  {
    memcpy(error_message.function[error_message.total],"main",4);
    memcpy(error_message.data[error_message.total],"Could not get the block verifiers IP address",44);
    error_message.total++;
    print_error_message; 
    database_reset;
    pointer_reset(data);
    exit(0);
  }

  // check if the block verifier has any of the databases
  if (strncmp(block_verifiers_IP_address,"",1) == 0)
  {
    // check if all of the databases are synced
    color_print("\nCould not find your IP address in the database. This is because your database is out of sync, or you have not registered as a delegate.\nIf this process loops a few times, then make sure you have registered by visting the delegates website.","red");
    
    // sync the databases and then recheck if the block verifier is a network data node
    if (sync_all_block_verifiers_list() == 0)
    {
      memcpy(error_message.function[error_message.total],"main",4);
      memcpy(error_message.data[error_message.total],"Could not sync the previous, current and next block verifiers list",66);
      error_message.total++;
      print_error_message; 
      database_reset;
      pointer_reset(data);
      exit(0);
    }
    /*if (check_if_databases_are_synced() == 0)
    {
      memcpy(error_message.function[error_message.total],"main",4);
      memcpy(error_message.data[error_message.total],"Could not check if the databases are synced",43);
      error_message.total++;
      print_error_message; 
      database_reset;
      pointer_reset(data);
      exit(0);
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
      memcpy(error_message.function[error_message.total],"main",4);
      memcpy(error_message.data[error_message.total],"Could not sync the previous, current and next block verifiers list",66);
      error_message.total++;
      print_error_message; 
      database_reset;
      pointer_reset(data);
      exit(0);
    }
    if (count2 != NETWORK_DATA_NODES_AMOUNT)
    {
      // check if all of the databases are synced
      /*if (check_if_databases_are_synced() == 0)
      {
        memcpy(error_message.function[error_message.total],"main",4);
        memcpy(error_message.data[error_message.total],"Could not check if the databases are synced",43);
        error_message.total++;
        print_error_message; 
        database_reset;
        pointer_reset(data);
        exit(0);
      }*/
    }
  }
  else
  {
    // sync the previous, current and next block verifiers list
    if (sync_all_block_verifiers_list() == 0)
    {
      memcpy(error_message.function[error_message.total],"main",4);
      memcpy(error_message.data[error_message.total],"Could not sync the previous, current and next block verifiers list",66);
      error_message.total++;
      print_error_message; 
      database_reset;
      pointer_reset(data);
      exit(0);
    }

    // check if all of the databases are synced
    if (check_if_databases_are_synced() == 0)
    {
      memcpy(error_message.function[error_message.total],"main",4);
      memcpy(error_message.data[error_message.total],"Could not check if the databases are synced",43);
      error_message.total++;
      print_error_message; 
      database_reset;
      pointer_reset(data);
      exit(0);
    }
  }

 
  print_start_message("Starting all of the threads");

  // start the block height timer thread
  if (pthread_create(&thread_id_1, NULL, &current_block_height_timer_thread, NULL) != 0 && pthread_detach(thread_id_1) != 0)
  {
    memcpy(error_message.function[error_message.total],"main",4);
    memcpy(error_message.data[error_message.total],"Could not sync the previous, current and next block verifiers list",66);
    error_message.total++;
    print_error_message; 
    database_reset;
    pointer_reset(data);
    exit(0);
    color_print("Could not start the current_block_height_timer_thread","red");
    database_reset;
    pointer_reset(data);
  }
  
  color_print("Started the current block height timer thread","green");

  /*// start the check_reserve_proofs_timer_thread
  if (pthread_create(&thread_id_2, NULL, &check_reserve_proofs_timer_thread, NULL) != 0 && pthread_detach(thread_id_2) != 0)
  {
    memcpy(error_message.function[error_message.total],"main",4);
    memcpy(error_message.data[error_message.total],"Could not start the check_reserve_proofs_timer_thread",53);
    error_message.total++;
    print_error_message; 
    database_reset;
    pointer_reset(data);
    exit(0);  
  }

  color_print("Started the check reserve proofs timer thread","green");

  // start the check_delegates_online_status_timer_thread
  if (pthread_create(&thread_id_3, NULL, &check_delegates_online_status_timer_thread, NULL) != 0 && pthread_detach(thread_id_3) != 0)
  {
    memcpy(error_message.function[error_message.total],"main",4);
    memcpy(error_message.data[error_message.total],"Could not start the check_delegates_online_status_timer_thread",62);
    error_message.total++;
    print_error_message; 
    database_reset;
    pointer_reset(data);
    exit(0);
  }

  color_print("Started the check delegates online status timer thread","green");*/
  
  // start the server
  for (;;)
  {
    if (create_server(1) == 0)
    {
      memcpy(error_message.function[error_message.total],"main",4);
      memcpy(error_message.data[error_message.total],"Could not start the server",26);
      error_message.total++;
      print_error_message; 
      database_reset;
      pointer_reset(data);
      exit(0);
    }
  } 

  database_reset;
  pointer_reset(data);
  return 0;  

  #undef database_reset
}