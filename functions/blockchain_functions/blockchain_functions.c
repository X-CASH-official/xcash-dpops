#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "variables.h"
#include "structures.h"

#include "network_daemon_functions.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: varint_decode
Description: Decodes varints for the get_block_template blocktemplate_blob
Parameters:
  varint - The varint to decode
Return: The decoded varint
-----------------------------------------------------------------------------------------------------------
*/

size_t varint_decode(size_t varint)
{
  // Variables
  int length;
  int count = 0;
  int counter = 0;
  int bytecount = 0;
  size_t number = 1;
  int start = 0;

  // get the length
  if (varint <= 0xFF)
  {
    return varint;
  }
  else if (varint > 0xFF && varint < 0xFFFF)
  {
    length = 2;
  }
  else if (varint >= 0xFFFF && varint < 0xFFFFFF)
  {
    length = 3;
  }
  else if (varint >= 0xFFFFFF && varint < 0xFFFFFFFF)
  {
    length = 4;
  }
  else if (varint >= 0xFFFFFFFF && varint < 0xFFFFFFFFFF)
  {
    length = 5;
  }
  else if (varint >= 0xFFFFFFFFFF && varint < 0xFFFFFFFFFFFF)
  {
    length = 6;
  }
  else if (varint >= 0xFFFFFFFFFFFF && varint < 0xFFFFFFFFFFFFFF)
  {
    length = 7;
  }
  else if (varint >= 0xFFFFFFFFFFFFFF && varint < 0xFFFFFFFFFFFFFFFF)
  {
    length = 8;
  }

  // create a byte array for the varint
  char bytes[length];

  for (count = 0; count < length; count++)
  {
    // convert each byte to binary and read the bytes in reverse order
    bytes[count] = ((varint >> (8 * count)) & 0xFF);
  }
    
  for (count = 0, counter = 7, bytecount = 0, start = 0; count < length * 8; count++)
  {
    // loop through each bit until you find the first 1. for every bit after this:
    // if 0 then number = number * 2;
    // if 1 then number = (number * 2) + 1;
    // dont use the bit if its the first bit
    if (counter != 7)
    {
      if (bytes[bytecount] & (1 << counter)) 
      {
        if (start == 1)
        {
          number = (number * 2) + 1;
        }
      start = 1;
      }
      else
      {
        if (start == 1)
        {
          number = number * 2;
        }
      } 
    }
      
    if (counter == 0) 
    {
      counter = 7;
      bytecount++;
    }
    else
    {
      counter--;
    }
  }
 return number;    
}

/*
-----------------------------------------------------------------------------------------------------------
Name: network_block_string_to_blockchain_data
Description: Converts a network_block_string string to the blockchain_data struct
Parameters:
  data - The network_block_string
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int network_block_string_to_blockchain_data(const char* data)
{
  // Constants
  const size_t DATA_LENGTH = strnlen(data,BUFFER_SIZE);

  // Variables
  size_t count;
  size_t number;
  char* current_block_height = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3;

  // define macros
  #define BLOCKCHAIN_RESERVED_BYTES_START "7c424c4f434b434841494e5f52455345525645445f42595445535f53544152547c"

  #define pointer_reset_all \
  free(current_block_height); \
  current_block_height = NULL; \
  free(data2); \
  data2 = NULL;

  #define NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR(settings) \
  color_print(settings,"red"); \
  pointer_reset_all; \
  return 0; 

  // check if the memory needed was allocated on the heap successfully
  if (current_block_height == NULL || data2 == NULL)
  {
    if (current_block_height != NULL)
    {
      pointer_reset(current_block_height);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }  

  // reset the blockchain_data
  memset(blockchain_data.network_version_data,0,strnlen(blockchain_data.network_version_data,5));
  memset(blockchain_data.timestamp_data,0,strnlen(blockchain_data.timestamp_data,11));
  memset(blockchain_data.previous_block_hash_data,0,strnlen(blockchain_data.previous_block_hash_data,65));
  memset(blockchain_data.nonce_data,0,strnlen(blockchain_data.nonce_data,9));
  memset(blockchain_data.block_reward_transaction_version_data,0,strnlen(blockchain_data.block_reward_transaction_version_data,3));
  memset(blockchain_data.unlock_block_data,0,strnlen(blockchain_data.unlock_block_data,9));
  memset(blockchain_data.block_reward_input_data,0,strnlen(blockchain_data.block_reward_input_data,3));
  memset(blockchain_data.vin_type_data,0,strnlen(blockchain_data.vin_type_data,3));
  memset(blockchain_data.block_height_data,0,strnlen(blockchain_data.block_height_data,9));
  memset(blockchain_data.block_reward_output_data,0,strnlen(blockchain_data.block_reward_output_data,3));
  memset(blockchain_data.block_reward_data,0,strnlen(blockchain_data.block_reward_data,15));
  memset(blockchain_data.stealth_address_output_tag_data,0,strnlen(blockchain_data.stealth_address_output_tag_data,3));
  memset(blockchain_data.stealth_address_output_data,0,strnlen(blockchain_data.stealth_address_output_data,65));
  memset(blockchain_data.extra_bytes_size_data,0,strnlen(blockchain_data.extra_bytes_size_data,5));
  memset(blockchain_data.transaction_public_key_tag_data,0,strnlen(blockchain_data.transaction_public_key_tag_data,3));
  memset(blockchain_data.transaction_public_key_data,0,strnlen(blockchain_data.transaction_public_key_data,65));
  memset(blockchain_data.extra_nonce_tag_data,0,strnlen(blockchain_data.extra_nonce_tag_data,3));
  memset(blockchain_data.reserve_bytes_size_data,0,strnlen(blockchain_data.reserve_bytes_size_data,5));
  memset(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data,41));
  memset(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name,21));
  memset(blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data,197));
  memset(blockchain_data.blockchain_reserve_bytes.block_producer_public_address,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_public_address,99));
  memset(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data,2));
  memset(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data,249));
  memset(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names,125));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name_data,41));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name,21));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address_data,197));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address,99));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count_data,2));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names_data,249));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names,125));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name_data,41));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name,21));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address_data,197));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address,99));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count_data,2));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names_data,249));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names,125));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_1,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_1,BUFFER_SIZE));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_1,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_1,BUFFER_SIZE));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_1,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_1,BUFFER_SIZE));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_1,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_1,BUFFER_SIZE));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_2,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_2,BUFFER_SIZE));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_2,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_2,BUFFER_SIZE));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_2,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_2,BUFFER_SIZE));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_2,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_2,BUFFER_SIZE));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_3,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_3,BUFFER_SIZE));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_3,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_3,BUFFER_SIZE));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_3,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_3,BUFFER_SIZE));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_3,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_3,BUFFER_SIZE));
  memset(blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,BUFFER_SIZE));
  for (count = 0; count < BLOCK_VALIDATION_NODES_AMOUNT; count++)
  {
    memset(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count],0,strnlen(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count],BUFFER_SIZE));
    memset(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count],0,strnlen(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count],BUFFER_SIZE));
  }
  memset(blockchain_data.ringct_version_data,0,strnlen(blockchain_data.ringct_version_data,3));
  memset(blockchain_data.transaction_amount_data,0,strnlen(blockchain_data.transaction_amount_data,5));
  for (count = 0; count < 1000000; count++)
  {
    memset(blockchain_data.transactions[count],0,strnlen(blockchain_data.transactions[count],65));
  }
  count = 0;



  // network_version
  blockchain_data.network_version_data_length = 4;
  count+= blockchain_data.network_version_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.network_version_data,data,blockchain_data.network_version_data_length);

  // timestamp
  blockchain_data.timestamp_data_length = 10;
  count+= blockchain_data.timestamp_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.timestamp_data,&data[count-blockchain_data.timestamp_data_length],blockchain_data.timestamp_data_length);
  sscanf(blockchain_data.timestamp_data, "%zu", &number);
  blockchain_data.timestamp = varint_decode(number);

  // previous_block_hash
  blockchain_data.previous_block_hash_data_length = 64;
  count+= blockchain_data.previous_block_hash_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.previous_block_hash_data,&data[count-blockchain_data.previous_block_hash_data_length],blockchain_data.previous_block_hash_data_length);

  // nonce
  blockchain_data.nonce_data_length = 8;
  count+= blockchain_data.nonce_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.nonce_data,&data[count-blockchain_data.nonce_data_length],blockchain_data.nonce_data_length);

  // block_reward_transaction_version
  blockchain_data.block_reward_transaction_version_data_length = 2;
  count+= blockchain_data.block_reward_transaction_version_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.block_reward_transaction_version_data,&data[count-blockchain_data.block_reward_transaction_version_data_length],blockchain_data.block_reward_transaction_version_data_length);

  // unlock_block
  // get the current block height
  if (get_current_block_height(current_block_height, 0) == 0)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Could not get the current block height\nFunction: network_block_string_to_blockchain_data");
  }
  sscanf(current_block_height, "%zu", &number);
  number += 61;

  if (number > 2097091)
  {
    blockchain_data.unlock_block_data_length = 8;
  }
  else
  {
    blockchain_data.unlock_block_data_length = 6;
  }
  count+= blockchain_data.unlock_block_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.unlock_block_data,&data[count-blockchain_data.unlock_block_data_length],blockchain_data.unlock_block_data_length);
  sscanf(blockchain_data.unlock_block_data, "%zu", &number);
  blockchain_data.unlock_block = varint_decode(number);

  // block_reward_input
  blockchain_data.block_reward_input_data_length = 2;
  count+= blockchain_data.block_reward_input_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.block_reward_input_data,&data[count-blockchain_data.block_reward_input_data_length],blockchain_data.block_reward_input_data_length);

  // vin_type
  blockchain_data.vin_type_data_length = 2;
  count+= blockchain_data.vin_type_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.vin_type_data,&data[count-blockchain_data.vin_type_data_length],blockchain_data.vin_type_data_length);

  // block_height
  sscanf(current_block_height, "%zu", &number);
  number += 1;

  if (number > 2097151)
  {
    blockchain_data.block_height_data_length = 8;
  }
  else
  {
    blockchain_data.block_height_data_length = 6;
  }
  count+= blockchain_data.block_height_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.block_height_data,&data[count-blockchain_data.block_height_data_length],blockchain_data.block_height_data_length);
  sscanf(blockchain_data.block_height_data, "%zu", &number);
  blockchain_data.block_height = varint_decode(number);

  // block_reward_output
  blockchain_data.block_reward_output_data_length = 2;
  count+= blockchain_data.block_reward_output_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.block_reward_output_data,&data[count-blockchain_data.block_reward_output_data_length],blockchain_data.block_reward_output_data_length);

  // block_reward
  // since the block reward could be any number because of transactions fees, get the position of BLOCKCHAIN_RESERVED_BYTES_START to get the length of the block reward
  data3 = strstr(data2,BLOCKCHAIN_RESERVED_BYTES_START);
  if (data3 == NULL)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  blockchain_data.block_reward_data_length = strnlen(data2,BUFFER_SIZE) - strnlen(data3,BUFFER_SIZE) - count - 142;
  count+= blockchain_data.block_reward_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.block_reward_data,&data[count-blockchain_data.block_reward_data_length],blockchain_data.block_reward_data_length);
  sscanf(blockchain_data.block_reward_data, "%zu", &number);
  blockchain_data.block_reward = varint_decode(number);

  // stealth_address_output_tag
  blockchain_data.stealth_address_output_tag_data_length = 2;
  count+= blockchain_data.stealth_address_output_tag_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.stealth_address_output_tag_data,&data[count-blockchain_data.stealth_address_output_tag_data_length],blockchain_data.stealth_address_output_tag_data_length);

  // stealth_address_output
  blockchain_data.stealth_address_output_data_length = 64;
  count+= blockchain_data.stealth_address_output_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.stealth_address_output_data,&data[count-blockchain_data.stealth_address_output_data_length],blockchain_data.stealth_address_output_data_length);

  // extra_bytes_size
  blockchain_data.extra_bytes_size_data_length = 4;
  count+= blockchain_data.extra_bytes_size_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.extra_bytes_size_data,&data[count-blockchain_data.extra_bytes_size_data_length],blockchain_data.extra_bytes_size_data_length);
  sscanf(blockchain_data.extra_bytes_size_data, "%zu", &number);
  blockchain_data.extra_bytes_size = varint_decode(number);

  // transaction_public_key_tag
  blockchain_data.transaction_public_key_tag_data_length = 2;
  count+= blockchain_data.transaction_public_key_tag_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.transaction_public_key_tag_data,&data[count-blockchain_data.transaction_public_key_tag_data_length],blockchain_data.transaction_public_key_tag_data_length);

  // transaction_public_key
  blockchain_data.transaction_public_key_data_length = 64;
  count+= blockchain_data.transaction_public_key_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.transaction_public_key_data,&data[count-blockchain_data.transaction_public_key_data_length],blockchain_data.transaction_public_key_data_length);

  // extra_nonce_tag
  blockchain_data.extra_nonce_tag_data_length = 2;
  count+= blockchain_data.extra_nonce_tag_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.extra_nonce_tag_data,&data[count-blockchain_data.extra_nonce_tag_data_length],blockchain_data.extra_nonce_tag_data_length);

  // reserve_bytes_size
  blockchain_data.reserve_bytes_size_data_length = 4;
  count+= blockchain_data.reserve_bytes_size_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.reserve_bytes_size_data,&data[count-blockchain_data.reserve_bytes_size_data_length],blockchain_data.reserve_bytes_size_data_length);
  sscanf(blockchain_data.reserve_bytes_size_data, "%zu", &number);
  blockchain_data.reserve_bytes_size = varint_decode(number);

  // blockchain_reserve_bytes


  // ringct_version
  blockchain_data.ringct_version_data_length = 2;
  count+= blockchain_data.ringct_version_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.ringct_version_data,&data[count-blockchain_data.ringct_version_data_length],blockchain_data.ringct_version_data_length);

  // transaction_amount
  // get how many bytes are left in the network_block_string
  blockchain_data.transaction_amount_data_length = (strnlen(data2,BUFFER_SIZE) - count) % 64;
  count+= blockchain_data.transaction_amount_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.transaction_amount_data,&data[count-blockchain_data.transaction_amount_data_length],blockchain_data.transaction_amount_data_length);
  sscanf(blockchain_data.transaction_amount_data, "%zu", &number);
  blockchain_data.transaction_amount = varint_decode(number);

  // get all of the transactions
  for (number = 0; number < blockchain_data.transaction_amount; number++)
  {
    count+= 64;
    if (count > DATA_LENGTH)
    {
      NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
    }
    memcpy(blockchain_data.transactions[number],&data[count-64],64);
  }

  return 1;

  #undef BLOCKCHAIN_RESERVED_BYTES_START
  #undef pointer_reset_all
  #undef NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: blockchain_data_to_network_block_string
Description: Converts a blockchain_data struct to a network_block_string
Parameters:
  result - The network block data string
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int blockchain_data_to_network_block_string(char* result)
{
  // Variables
  size_t count = 0;
  size_t number = 0;

  memset(result,0,strnlen(result,BUFFER_SIZE));
  memcpy(result,blockchain_data.network_version_data,blockchain_data.network_version_data_length);
  count += blockchain_data.network_version_data_length;
  memcpy(result+count,blockchain_data.timestamp_data,blockchain_data.timestamp_data_length);
  count += blockchain_data.timestamp_data_length;
  memcpy(result+count,blockchain_data.previous_block_hash_data,blockchain_data.previous_block_hash_data_length);
  count += blockchain_data.previous_block_hash_data_length;
  memcpy(result+count,blockchain_data.nonce_data,blockchain_data.nonce_data_length);
  count += blockchain_data.nonce_data_length;
  memcpy(result+count,blockchain_data.block_reward_transaction_version_data,blockchain_data.block_reward_transaction_version_data_length);
  count += blockchain_data.block_reward_transaction_version_data_length;
  memcpy(result+count,blockchain_data.unlock_block_data,blockchain_data.unlock_block_data_length);
  count += blockchain_data.unlock_block_data_length;
  memcpy(result+count,blockchain_data.block_reward_input_data,blockchain_data.block_reward_input_data_length);
  count += blockchain_data.block_reward_input_data_length;
  memcpy(result+count,blockchain_data.vin_type_data,blockchain_data.vin_type_data_length);
  count += blockchain_data.vin_type_data_length;
  memcpy(result+count,blockchain_data.block_height_data,blockchain_data.block_height_data_length);
  count += blockchain_data.block_height_data_length;
  memcpy(result+count,blockchain_data.block_reward_output_data,blockchain_data.block_reward_output_data_length);
  count += blockchain_data.block_reward_output_data_length;
  memcpy(result+count,blockchain_data.block_reward_data,blockchain_data.block_reward_data_length);
  count += blockchain_data.block_reward_data_length;
  memcpy(result+count,blockchain_data.stealth_address_output_tag_data,blockchain_data.stealth_address_output_tag_data_length);
  count += blockchain_data.stealth_address_output_tag_data_length;  
  memcpy(result+count,blockchain_data.stealth_address_output_data,blockchain_data.stealth_address_output_data_length);
  count += blockchain_data.stealth_address_output_data_length;  
  memcpy(result+count,blockchain_data.extra_bytes_size_data,blockchain_data.extra_bytes_size_data_length);
  count += blockchain_data.extra_bytes_size_data_length;  
  memcpy(result+count,blockchain_data.transaction_public_key_tag_data,blockchain_data.transaction_public_key_tag_data_length);
  count += blockchain_data.transaction_public_key_tag_data_length;  
  memcpy(result+count,blockchain_data.transaction_public_key_data,blockchain_data.transaction_public_key_data_length);
  count += blockchain_data.transaction_public_key_data_length;  
  memcpy(result+count,blockchain_data.extra_nonce_tag_data,blockchain_data.extra_nonce_tag_data_length);
  count += blockchain_data.extra_nonce_tag_data_length;  
  memcpy(result+count,blockchain_data.reserve_bytes_size_data,blockchain_data.reserve_bytes_size_data_length);
  count += blockchain_data.reserve_bytes_size_data_length;

  // blockchain_reserve_bytes

  
  memcpy(result+count,blockchain_data.ringct_version_data,blockchain_data.ringct_version_data_length);
  count += blockchain_data.ringct_version_data_length;  
  memcpy(result+count,blockchain_data.transaction_amount_data,blockchain_data.transaction_amount_data_length);
  count += blockchain_data.transaction_amount_data_length;

  // get all of the transactions
  for (number = 0; number < blockchain_data.transaction_amount; number++)
  {
    memcpy(result+count,blockchain_data.transactions[number],64);
    count += 64;
  }
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: verify_network_block_data
Description: Verifies a blockchain_data struct
Parameters:
  BLOCK_VALIDATION_SIGNATURES_SETTINGS - 0 to not verify the block validation signatures, 1 to verify the block validation signatures. The consensus node when submitting the block should be the only time when the block validation signatures are verified
  TRANSACTIONS_SETTINGS - 0 to not verify that the transactions are in the network, 1 to verify that the transactions are in the network. The blockchain_test should be the only time when the transactions are not verified
Return: 0 if an error has occured or it is not verified, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int verify_network_block_data(const int BLOCK_VALIDATION_SIGNATURES_SETTINGS,const int TRANSACTIONS_SETTINGS)
{
  // Variables
  size_t count;
  size_t number;
  char* previous_block_hash = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* current_block_height = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3;

  // define macros
  #define BLOCK_REWARD_TRANSACTION_VERSION "02"
  #define BLOCK_REWARD_INPUT "01"
  #define VIN_TYPE "ff"
  #define BLOCK_REWARD_OUTPUT "01"
  #define STEALTH_ADDRESS_OUTPUT_TAG "02"
  #define TRANSACTION_PUBLIC_KEY_TAG "01"
  #define EXTRA_NONCE_TAG "02"
  #define RINGCT_VERSION "01"

  #define pointer_reset_all \
  free(previous_block_hash); \
  previous_block_hash = NULL; \
  free(current_block_height); \
  current_block_height = NULL; \
  free(data2); \
  data2 = NULL;

  #define VERIFY_NETWORK_BLOCK_DATA_ERROR(settings) \
  color_print(settings,"red"); \
  pointer_reset_all; \
  return 0; 

  // check if the memory needed was allocated on the heap successfully
  if (previous_block_hash == NULL || current_block_height == NULL || data2 == NULL)
  {
    if (previous_block_hash != NULL)
    {
      pointer_reset(previous_block_hash);
    }
    if (current_block_height != NULL)
    {
      pointer_reset(current_block_height);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  } 

  // network_version
  if (blockchain_data.network_version_data_length != 4 || memcmp(blockchain_data.network_version_data,NETWORK_VERSION,4) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid network_version\nFunction: verify_network_block_data");
  } 

  // timestamp
  if (blockchain_data.timestamp_data_length != 10)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid timestamp\nFunction: verify_network_block_data");
  }

  // previous_block_hash
  if (get_previous_block_hash(previous_block_hash,0) == 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Could not get the previous block hash\nFunction: verify_network_block_data");
  }
  if (blockchain_data.previous_block_hash_data_length != 64 || memcmp(blockchain_data.previous_block_hash_data,previous_block_hash,64) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid previous block hash\nFunction: verify_network_block_data");
  }  

  // nonce
  if (blockchain_data.nonce_data_length != 8 || (memcmp(blockchain_data.nonce_data,BLOCK_PRODUCER_NETWORK_BLOCK_NONCE,8) != 0 && memcmp(blockchain_data.nonce_data,CONSENSUS_NODE_NETWORK_BLOCK_NONCE,8) != 0))
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid network block nonce\nFunction: verify_network_block_data");
  }

  // block_reward_transaction_version
  if (blockchain_data.block_reward_transaction_version_data_length != 2 || memcmp(blockchain_data.block_reward_transaction_version_data,BLOCK_REWARD_TRANSACTION_VERSION,2) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid block_reward_transaction_version\nFunction: verify_network_block_data");
  }

  // unlock_block
  if (get_current_block_height(current_block_height,0) == 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Could not get the current block height\nFunction: verify_network_block_data");
  }
  sscanf(current_block_height, "%zu", &number);
  if ((blockchain_data.unlock_block <= 2097091 && blockchain_data.unlock_block_data_length != 6) || (blockchain_data.unlock_block > 2097091 && blockchain_data.unlock_block_data_length != 8) || blockchain_data.unlock_block != number+61)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid unlock_block\nFunction: verify_network_block_data");
  }

  // block_reward_input
  if (blockchain_data.unlock_block_data_length != 2 || memcmp(blockchain_data.unlock_block_data,BLOCK_REWARD_INPUT,2) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid block_reward_input\nFunction: verify_network_block_data");
  }

  // vin_type
  if (blockchain_data.vin_type_data_length != 2 || memcmp(blockchain_data.vin_type_data,VIN_TYPE,2) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid vin_type\nFunction: verify_network_block_data");
  }

  // block_height
  if ((blockchain_data.block_height <= 2097151 && blockchain_data.block_height_data_length != 6) || (blockchain_data.block_height > 2097151 && blockchain_data.block_height_data_length != 8) || blockchain_data.block_height != number+1)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid block_height\nFunction: verify_network_block_data");
  }

  // block_reward_output
  if (blockchain_data.block_reward_output_data_length != 2 || memcmp(blockchain_data.block_reward_output_data,BLOCK_REWARD_OUTPUT,2) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid block_reward_output\nFunction: verify_network_block_data");
  }

  // block_reward
  if ((blockchain_data.block_reward <= 34359738367 && blockchain_data.block_reward_data_length != 10) || (blockchain_data.block_reward > 34359738367 && blockchain_data.block_reward <= 4398046511104 && blockchain_data.block_reward_data_length != 12) || (blockchain_data.block_reward > 4398046511104 && blockchain_data.block_reward <= 562949953421312 && blockchain_data.block_reward_data_length != 14) || (blockchain_data.block_reward > 562949953421312 && blockchain_data.block_reward <= 72057594037927936 && blockchain_data.block_reward_data_length != 16) || (blockchain_data.block_reward > 72057594037927936 && blockchain_data.block_reward_data_length != 18))
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid block_reward\nInvalid block_height\nFunction: verify_network_block_data");
  }

  // stealth_address_output_tag
  if (blockchain_data.stealth_address_output_tag_data_length != 2 || memcmp(blockchain_data.stealth_address_output_tag_data,STEALTH_ADDRESS_OUTPUT_TAG,2) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid stealth_address_output_tag\nFunction: verify_network_block_data");
  }

  // stealth_address_output
  if (blockchain_data.stealth_address_output_data_length != 64)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid stealth_address_output\nFunction: verify_network_block_data");
  }

  // extra_bytes_size
  if (blockchain_data.extra_bytes_size_data_length != 2 || blockchain_data.extra_bytes_size < 2146 || blockchain_data.extra_bytes_size > 2420 || ((blockchain_data.transaction_public_key_tag_data_length + blockchain_data.transaction_public_key_data_length + blockchain_data.extra_nonce_tag_data_length + blockchain_data.reserve_bytes_size_data_length + blockchain_data.reserve_bytes_size) / 2) != blockchain_data.extra_bytes_size)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid block_reward\nInvalid extra_bytes_size\nFunction: verify_network_block_data");
  }

  // transaction_public_key_tag
  if (blockchain_data.transaction_public_key_tag_data_length != 2 || memcmp(blockchain_data.transaction_public_key_tag_data,TRANSACTION_PUBLIC_KEY_TAG,2) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid transaction_public_key_tag\nFunction: verify_network_block_data");
  }

  // transaction_public_key
  if (blockchain_data.transaction_public_key_data_length != 64)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid transaction_public_key\nInvalid transaction_public_key\nFunction: verify_network_block_data");
  }

  // extra_nonce_tag
  if (blockchain_data.extra_nonce_tag_data_length != 2 || memcmp(blockchain_data.extra_nonce_tag_data,EXTRA_NONCE_TAG,2) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid extra_nonce_tag\nFunction: verify_network_block_data");
  }

  // reserve_bytes_size
  if (blockchain_data.reserve_bytes_size_data_length != 4 || blockchain_data.reserve_bytes_size < 2110 || blockchain_data.reserve_bytes_size > 2384)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid block_reward\nInvalid reserve_bytes_size\nFunction: verify_network_block_data");
  }

  // blockchain_reserve_bytes



  // ringct_version
  if (blockchain_data.ringct_version_data_length != 2 || memcmp(blockchain_data.ringct_version_data,RINGCT_VERSION,2) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid ringct_version\nFunction: verify_network_block_data");
  }

  // transaction_amount
  if ((blockchain_data.transaction_amount <= 255 && blockchain_data.transaction_amount_data_length != 2) || (blockchain_data.transaction_amount > 255 && blockchain_data.transaction_amount <= 16383 && blockchain_data.transaction_amount_data_length != 4) || (blockchain_data.transaction_amount > 16383 && blockchain_data.transaction_amount_data_length != 6))
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid transaction_amount\nFunction: verify_network_block_data");
  }

  // transactions
  if (verify_blockchain_network_transactions(blockchain_data.transactions,blockchain_data.transaction_amount,0) == 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid transactions\nFunction: verify_network_block_data");
  }

  pointer_reset_all;
  return 1;

  #undef BLOCK_REWARD_TRANSACTION_VERSION
  #undef BLOCK_REWARD_INPUT
  #undef VIN_TYPE
  #undef BLOCK_REWARD_OUTPUT
  #undef STEALTH_ADDRESS_OUTPUT_TAG
  #undef TRANSACTION_PUBLIC_KEY_TAG
  #undef EXTRA_NONCE_TAG
  #undef RINGCT_VERSION
  #undef pointer_reset_all
  #undef VERIFY_NETWORK_BLOCK_DATA_ERROR
}
