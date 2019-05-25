#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <pthread.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "database_functions.h"
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
  FILE* file;
  size_t count = 0;

  // threads
  pthread_t thread_id_1;
  pthread_t thread_id_2;
  pthread_t thread_id_3;

  // iniltize the global variables
  xcash_wallet_public_address = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  nodes_public_address_list_received_data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  server_message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  vrf_public_key_part_1 = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  vrf_secret_key_part_1 = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  vrf_alpha_string_part_2 = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  current_round_part = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  current_round_part_backup_node = (char*)calloc(BUFFER_SIZE,sizeof(char));

  current_block_height = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || xcash_wallet_public_address == NULL || nodes_public_address_list_received_data == NULL || server_message == NULL || vrf_public_key_part_1 == NULL || vrf_alpha_string_part_2 == NULL || current_round_part == NULL || current_round_part_backup_node == NULL || current_block_height == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (xcash_wallet_public_address != NULL)
    {
      pointer_reset(xcash_wallet_public_address);
    }
    if (nodes_public_address_list_received_data != NULL)
    {
      pointer_reset(nodes_public_address_list_received_data);
    }
    if (server_message != NULL)
    {
      pointer_reset(server_message);
    }
    if (vrf_public_key_part_1 != NULL)
    {
      pointer_reset(vrf_public_key_part_1);
    }
    if (vrf_alpha_string_part_2 != NULL)
    {
      pointer_reset(vrf_alpha_string_part_2);
    }
    if (current_round_part != NULL)
    {
      pointer_reset(current_round_part);
    }
    if (current_round_part_backup_node != NULL)
    {
      pointer_reset(current_round_part_backup_node);
    }
    if (current_block_height != NULL)
    {
      pointer_reset(current_block_height);
    }
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  } 

  // initialize the previous block_verifiers_list struct 
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    previous_block_verifiers_list.block_verifiers_name[count] = (char*)calloc(BLOCK_VERIFIERS_NAME_TOTAL_LENGTH+1,sizeof(char));
    previous_block_verifiers_list.block_verifiers_public_address[count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    previous_block_verifiers_list.block_verifiers_IP_address[count] = (char*)calloc(BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH+1,sizeof(char));

    // check if the memory needed was allocated on the heap successfully
    if (previous_block_verifiers_list.block_verifiers_name[count] == NULL || previous_block_verifiers_list.block_verifiers_public_address[count] == NULL || previous_block_verifiers_list.block_verifiers_IP_address[count] == NULL)
    {
      color_print("Could not allocate the memory needed on the heap","red");
      exit(0);
    }
  }

  // initialize the current block_verifiers_list struct 
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    current_block_verifiers_list.block_verifiers_name[count] = (char*)calloc(BLOCK_VERIFIERS_NAME_TOTAL_LENGTH+1,sizeof(char));
    current_block_verifiers_list.block_verifiers_public_address[count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    current_block_verifiers_list.block_verifiers_IP_address[count] = (char*)calloc(BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH+1,sizeof(char));

    // check if the memory needed was allocated on the heap successfully
    if (current_block_verifiers_list.block_verifiers_name[count] == NULL || current_block_verifiers_list.block_verifiers_public_address[count] == NULL || current_block_verifiers_list.block_verifiers_IP_address[count] == NULL)
    {
      color_print("Could not allocate the memory needed on the heap","red");
      exit(0);
    }
  }

  // initialize the next block_verifiers_list struct 
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    next_block_verifiers_list.block_verifiers_name[count] = (char*)calloc(BLOCK_VERIFIERS_NAME_TOTAL_LENGTH+1,sizeof(char));
    next_block_verifiers_list.block_verifiers_public_address[count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    next_block_verifiers_list.block_verifiers_IP_address[count] = (char*)calloc(BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH+1,sizeof(char));

    // check if the memory needed was allocated on the heap successfully
    if (next_block_verifiers_list.block_verifiers_name[count] == NULL || next_block_verifiers_list.block_verifiers_public_address[count] == NULL || next_block_verifiers_list.block_verifiers_IP_address[count] == NULL)
    {
      color_print("Could not allocate the memory needed on the heap","red");
      exit(0);
    }
  }

  // initialize the main_nodes_list struct 
  main_nodes_list.block_producer_public_address = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  main_nodes_list.block_producer_IP_address = (char*)calloc(BUFFER_SIZE,sizeof(char));
  main_nodes_list.vrf_node_public_and_secret_key_public_address = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  main_nodes_list.vrf_node_public_and_secret_key_IP_address = (char*)calloc(BUFFER_SIZE,sizeof(char));
  main_nodes_list.vrf_node_random_data_public_address = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  main_nodes_list.vrf_node_random_data_IP_address = (char*)calloc(BUFFER_SIZE,sizeof(char));
  main_nodes_list.vrf_node_next_main_nodes_public_address = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  main_nodes_list.vrf_node_next_main_nodes_IP_address = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // initialize the current_round_part_consensus_node_data struct
  current_round_part_consensus_node_data.vrf_public_key = (char*)calloc(BUFFER_SIZE,sizeof(char));
  current_round_part_consensus_node_data.vrf_alpha_string = (char*)calloc(BUFFER_SIZE,sizeof(char));
  current_round_part_consensus_node_data.vrf_proof = (char*)calloc(BUFFER_SIZE,sizeof(char));
  current_round_part_consensus_node_data.vrf_beta_string = (char*)calloc(BUFFER_SIZE,sizeof(char));
  current_round_part_consensus_node_data.block_blob = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // initialize the current_round_part_vote_data struct
  current_round_part_vote_data.current_vote_results = (char*)calloc(DATA_HASH_LENGTH+1,sizeof(char));
  current_round_part_vote_data.vote_results_valid = 0;
  current_round_part_vote_data.vote_results_invalid = 0;

  if (current_round_part_consensus_node_data.vrf_public_key == NULL || current_round_part_consensus_node_data.vrf_alpha_string == NULL || current_round_part_consensus_node_data.vrf_proof == NULL || current_round_part_consensus_node_data.vrf_beta_string == NULL || current_round_part_consensus_node_data.block_blob == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // initialize the VRF_data_block_verifiers struct 
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    VRF_data_block_verifiers.public_address[count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    VRF_data_block_verifiers.vrf_public_key_round_part_1[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));
    VRF_data_block_verifiers.vrf_alpha_string_round_part_1[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));
    VRF_data_block_verifiers.vrf_proof_round_part_1[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));
    VRF_data_block_verifiers.vrf_beta_string_round_part_1[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));
    VRF_data_block_verifiers.vrf_public_key_round_part_2[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));
    VRF_data_block_verifiers.vrf_alpha_string_round_part_2[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));
    VRF_data_block_verifiers.vrf_proof_round_part_2[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));
    VRF_data_block_verifiers.vrf_beta_string_round_part_2[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));
    VRF_data_block_verifiers.vrf_public_key_round_part_3[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));
    VRF_data_block_verifiers.vrf_alpha_string_round_part_3[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));
    VRF_data_block_verifiers.vrf_proof_round_part_3[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));
    VRF_data_block_verifiers.vrf_beta_string_round_part_3[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));

    // check if the memory needed was allocated on the heap successfully
    if (VRF_data_block_verifiers.public_address[count] == NULL || VRF_data_block_verifiers.vrf_public_key_round_part_1[count] == NULL  || VRF_data_block_verifiers.vrf_alpha_string_round_part_1[count] == NULL  || VRF_data_block_verifiers.vrf_proof_round_part_1[count] == NULL  || VRF_data_block_verifiers.vrf_beta_string_round_part_1[count] == NULL || VRF_data_block_verifiers.vrf_public_key_round_part_2[count] == NULL  || VRF_data_block_verifiers.vrf_alpha_string_round_part_2[count] == NULL  || VRF_data_block_verifiers.vrf_proof_round_part_2[count] == NULL  || VRF_data_block_verifiers.vrf_beta_string_round_part_2[count] == NULL || VRF_data_block_verifiers.vrf_public_key_round_part_3[count] == NULL  || VRF_data_block_verifiers.vrf_alpha_string_round_part_3[count] == NULL  || VRF_data_block_verifiers.vrf_proof_round_part_3[count] == NULL  || VRF_data_block_verifiers.vrf_beta_string_round_part_3[count] == NULL)
    {
      color_print("Could not allocate the memory needed on the heap","red");
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
  blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_delegates_name_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_delegates_name = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_public_address_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_public_address = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_backup_nodes_names_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_backup_nodes_names = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_1 = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_1 = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_1 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_1 = (unsigned char*)calloc(BUFFER_SIZE,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_1 = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_1 = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_1 = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_1 = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_1 = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_2 = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_2 = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_2 = (unsigned char*)calloc(BUFFER_SIZE,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_2 = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_2 = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_2 = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_2 = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_2 = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_3 = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_3 = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_3 = (unsigned char*)calloc(BUFFER_SIZE,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_3 = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_3 = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_3 = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_3 = (unsigned char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_3 = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.vrf_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
  blockchain_data.blockchain_reserve_bytes.previous_block_hash_data = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));

  // check if the memory needed was allocated on the heap successfully
  if (blockchain_data.network_version_data == NULL || blockchain_data.timestamp_data == NULL || blockchain_data.previous_block_hash_data == NULL || blockchain_data.nonce_data == NULL || blockchain_data.block_reward_transaction_version_data == NULL || blockchain_data.unlock_block_data == NULL || blockchain_data.block_reward_input_data == NULL || blockchain_data.vin_type_data == NULL || blockchain_data.block_height_data == NULL || blockchain_data.block_reward_output_data == NULL || blockchain_data.block_reward_data == NULL || blockchain_data.stealth_address_output_tag_data == NULL || blockchain_data.stealth_address_output_data == NULL || blockchain_data.extra_bytes_size_data == NULL || blockchain_data.transaction_public_key_tag_data == NULL || blockchain_data.transaction_public_key_data == NULL || blockchain_data.extra_nonce_tag_data == NULL || blockchain_data.reserve_bytes_size_data == NULL || blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data == NULL || blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name == NULL || blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data == NULL || blockchain_data.blockchain_reserve_bytes.block_producer_public_address == NULL || blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data == NULL || blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data == NULL || blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names == NULL || blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name_data == NULL || blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name == NULL || blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address_data == NULL || blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address == NULL || blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count_data == NULL || blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names_data == NULL || blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names == NULL || blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name_data == NULL || blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name == NULL || blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address_data == NULL || blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address == NULL || blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count_data == NULL || blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names_data == NULL || blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names == NULL  || blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_delegates_name_data == NULL || blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_delegates_name == NULL || blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_public_address_data == NULL || blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_public_address == NULL || blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count_data == NULL || blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_backup_nodes_names_data == NULL || blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_backup_nodes_names == NULL || blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_1 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_1 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_1 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_1 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_1 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_1 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_1 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_1 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_1 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_2 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_2 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_2 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_2 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_2 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_2 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_2 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_2 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_2 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_3 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_3 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_3 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_3 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_3 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_3 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_3 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_3 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_3 == NULL || blockchain_data.blockchain_reserve_bytes.vrf_data == NULL || blockchain_data.blockchain_reserve_bytes.previous_block_hash_data == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data[count] = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
    blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address[count] = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));

    // check if the memory needed was allocated on the heap successfully
    if (blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data[count] == NULL || blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address[count] == NULL)
    {
      color_print("Could not allocate the memory needed on the heap","red");
      exit(0);
    }
  }
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count] = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
    blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count] = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));

    // check if the memory needed was allocated on the heap successfully
    if (blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count] == NULL || blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count] == NULL)
    {
      color_print("Could not allocate the memory needed on the heap","red");
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
      color_print("Could not allocate the memory needed on the heap","red");
      exit(0);
    }
  }

   // initialize the invalid_reserve_proofs and the block_verifiers_invalid_reserve_proofs
   for (count = 0; count < 100; count++)
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
    color_print("Could not create a connection for the database\n","red");
    mongoc_cleanup();
    exit(0);
  }

  // create a pool of connections for the database
  mongoc_uri_t* uri_thread_pool;
  bson_error_t error;
  uri_thread_pool = mongoc_uri_new_with_error(DATABASE_CONNECTION, &error);
  if (!uri_thread_pool)
  {
    color_print("Could not create a pool of connections for the database\n","red");
    mongoc_client_destroy(database_client);
    mongoc_cleanup();
    exit(0);
  }
  database_client_thread_pool = mongoc_client_pool_new(uri_thread_pool);
  if (!database_client_thread_pool)
  {
    color_print("Could not create a thread pool for the database\n","red");
    mongoc_client_destroy(database_client);
    mongoc_uri_destroy(uri_thread_pool);
    mongoc_cleanup();
    exit(0);
  }

  // set the current_round_part, current_round_part_backup_node and server message, this way the node will start at the begining of a round
  memset(current_round_part,0,strnlen(current_round_part,BUFFER_SIZE));
  memset(current_round_part_backup_node,0,strnlen(current_round_part_backup_node,BUFFER_SIZE));
  memcpy(current_round_part,"1",1);
  memcpy(current_round_part_backup_node,"0",1);
  memset(server_message,0,strnlen(server_message,BUFFER_SIZE));
  memcpy(server_message,"CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS|CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND",96);

  // write the message
  color_print("X-CASH Proof Of Stake, Version 1.0.0\n","green");

  // get the wallets public address
  printf("Getting the public address\n\n");
  if (get_public_address(0) == 1)
  {  
    // print the public address
    memcpy(data,"Successfully received the public address:",41);
    memcpy(data+41,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
    memcpy(data+41+XCASH_WALLET_LENGTH,"\n",1);
    color_print(data,"green");
  }
  else
  {
    color_print("Could not get the wallets public address\n","red");
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
      color_print("Invalid parameters\n","red");
      printf(INVALID_PARAMETERS_ERROR_MESSAGE);
    }  
    mongoc_client_destroy(database_client);
    mongoc_client_pool_destroy(database_client_thread_pool);
    mongoc_uri_destroy(uri_thread_pool);
    mongoc_cleanup();
    pointer_reset(data);
    exit(0);
  }

  // start the block height timer thread
  if (pthread_create(&thread_id_1, NULL, &current_block_height_timer_thread, NULL) != 0 && pthread_detach(thread_id_1) != 0)
  {
    color_print("Could not start the current_block_height_timer_thread","red");
    mongoc_client_destroy(database_client);
    mongoc_client_pool_destroy(database_client_thread_pool);
    mongoc_uri_destroy(uri_thread_pool);
    mongoc_cleanup();
    pointer_reset(data);
  } 

  // start the check_reserve_proofs_timer_thread
  if (pthread_create(&thread_id_2, NULL, &check_reserve_proofs_timer_thread, NULL) != 0 && pthread_detach(thread_id_2) != 0)
  {
    color_print("Could not start the check_reserve_proofs_timer_thread","red");
    mongoc_client_destroy(database_client);
    mongoc_client_pool_destroy(database_client_thread_pool);
    mongoc_uri_destroy(uri_thread_pool);
    mongoc_cleanup();
    pointer_reset(data);
  } 

  // start the check_delegates_online_status_timer_thread
  if (pthread_create(&thread_id_3, NULL, &check_delegates_online_status_timer_thread, NULL) != 0 && pthread_detach(thread_id_3) != 0)
  {
    color_print("Could not start the check_delegates_online_status_timer_thread","red");
    mongoc_client_destroy(database_client);
    mongoc_client_pool_destroy(database_client_thread_pool);
    mongoc_uri_destroy(uri_thread_pool);
    mongoc_cleanup();
    pointer_reset(data);
  } 
 
  // start the server
  for (;;)
  {
    if (create_server(1) == 0)
    {
      color_print("Could not start the server","red");
      mongoc_client_destroy(database_client);
      mongoc_client_pool_destroy(database_client_thread_pool);
      mongoc_uri_destroy(uri_thread_pool);
      mongoc_cleanup();
      exit(0);
    }
  } 

  pointer_reset(data);
  mongoc_client_destroy(database_client);
  mongoc_client_pool_destroy(database_client_thread_pool);
  mongoc_uri_destroy(uri_thread_pool);
  mongoc_cleanup();
  return 0; 
}