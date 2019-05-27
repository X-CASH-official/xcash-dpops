#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "variables.h"
#include "structures.h"

#include "define_macros_functions.h"
#include "string_functions.h"
#include "network_daemon_functions.h"
#include "network_wallet_functions.h"
#include "vrf.h"
#include "crypto_vrf.h"
#include "VRF_functions.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: varint_encode
Description: Encodes varints for the get_block_template blocktemplate_blob
Parameters:
  number - The number to decode
  result - the string to store the result
Return: 1 if successfull, otherwise 0
-----------------------------------------------------------------------------------------------------------
*/

int varint_encode(long long int number, char* result)
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t length;
  size_t count = 0;
  size_t count2 = 0;
  int binary_numbers[8];
  int binary_number_copy;
  long long int number_copy = number;  

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {     
    color_print("Could not allocate the memory needed on the heap","red");  
    pointer_reset(data); 
    exit(0);
  } 

  memset(result,0,strlen(result));  

  // check if it should not be encoded
  if (number <= 0xFF)
  {
    sprintf(result,"%02llx",number);
    pointer_reset(data);
    return 1;
  }

  // convert the number to a binary string
  for (count = 0; number_copy != 0; count++)
  {
    if (number_copy % 2 == 1)
    {
      memcpy(data+count,"1",1);
    }
    else
    {
      memcpy(data+count,"0",1);
    }
    number_copy /= 2; 
  }

  // pad the string to a mulitple of 7 bits  
  for (count = strnlen(data,BUFFER_SIZE); count % 7 != 0; count++)
  {
    memcpy(result+strnlen(result,BUFFER_SIZE),"0",1);
  }

  // reverse the string
  length = strnlen(data,BUFFER_SIZE);
  for (count = 0; count <= length; count++)
  {
    memcpy(result+strnlen(result,BUFFER_SIZE),&data[length - count],1);
  }
  memset(data,0,strnlen(data,BUFFER_SIZE));
  memcpy(data,result,strnlen(result,BUFFER_SIZE));
  memset(result,0,strnlen(result,BUFFER_SIZE));

  /*
  convert each 7 bits to one byte
  set the first bit to 1 for all groups of 7 except for the first group of 7
  */
  length = strnlen(data,BUFFER_SIZE) + (strnlen(data,BUFFER_SIZE) / 7);
  count = 0;
  count2 = 0;

 for (count = 0, count2 = 0; count < length; count++)
 {
   if (count % 8 == 0 && count != 0)
   {
     // reverse the binary bits
     binary_number_copy = 0;       
     if (((binary_numbers[count2] >> 7) & 1U) == 1) {binary_number_copy |= 1UL << 0;} else {binary_number_copy &= ~(1UL << 0);}
     if (((binary_numbers[count2] >> 6) & 1U) == 1) {binary_number_copy |= 1UL << 1;} else {binary_number_copy &= ~(1UL << 1);}
     if (((binary_numbers[count2] >> 5) & 1U) == 1) {binary_number_copy |= 1UL << 2;} else {binary_number_copy &= ~(1UL << 2);}
     if (((binary_numbers[count2] >> 4) & 1U) == 1) {binary_number_copy |= 1UL << 3;} else {binary_number_copy &= ~(1UL << 3);}
     if (((binary_numbers[count2] >> 3) & 1U) == 1) {binary_number_copy |= 1UL << 4;} else {binary_number_copy &= ~(1UL << 4);}
     if (((binary_numbers[count2] >> 2) & 1U) == 1) {binary_number_copy |= 1UL << 5;} else {binary_number_copy &= ~(1UL << 5);}
     if (((binary_numbers[count2] >> 1) & 1U) == 1) {binary_number_copy |= 1UL << 6;} else {binary_number_copy &= ~(1UL << 6);}
     if (((binary_numbers[count2] >> 0) & 1U) == 1) {binary_number_copy |= 1UL << 7;} else {binary_number_copy &= ~(1UL << 7);}
     binary_numbers[count2] = binary_number_copy;
     count2++;
   } 
   if (count % 8 == 0)
   {
     if (count == 0)
     {
       // clear the binary bit to 0
       binary_numbers[count2] &= ~(1 << (count % 8));      
     }
     else
     {
       // set the binary bit to 1
       binary_numbers[count2] |= 1 << (count % 8);
     }
   }
   else
   {
     if (memcmp(data + (count - (count2+1)),"1",1) == 0)
     {
       // set the binary bit to 1
       binary_numbers[count2] |= 1 << (count % 8);
     }
     else
     {
       // clear the binary bit to 0
       binary_numbers[count2] &= ~(1 << (count % 8));
     }     
   }
 }

  // reverse the last binary_number
  length = strnlen(data,BUFFER_SIZE) / 8;
  binary_number_copy = 0;
  if (((binary_numbers[length] >> 7) & 1U) == 1) {binary_number_copy |= 1UL << 0;} else {binary_number_copy &= ~(1UL << 0);}
  if (((binary_numbers[length] >> 6) & 1U) == 1) {binary_number_copy |= 1UL << 1;} else {binary_number_copy &= ~(1UL << 1);}
  if (((binary_numbers[length] >> 5) & 1U) == 1) {binary_number_copy |= 1UL << 2;} else {binary_number_copy &= ~(1UL << 2);}
  if (((binary_numbers[length] >> 4) & 1U) == 1) {binary_number_copy |= 1UL << 3;} else {binary_number_copy &= ~(1UL << 3);}
  if (((binary_numbers[length] >> 3) & 1U) == 1) {binary_number_copy |= 1UL << 4;} else {binary_number_copy &= ~(1UL << 4);}
  if (((binary_numbers[length] >> 2) & 1U) == 1) {binary_number_copy |= 1UL << 5;} else {binary_number_copy &= ~(1UL << 5);}
  if (((binary_numbers[length] >> 1) & 1U) == 1) {binary_number_copy |= 1UL << 6;} else {binary_number_copy &= ~(1UL << 6);}
  if (((binary_numbers[length] >> 0) & 1U) == 1) {binary_number_copy |= 1UL << 7;} else {binary_number_copy &= ~(1UL << 7);}
  binary_numbers[length] = binary_number_copy;

  // create the varint encoded string
  for (count = 0, count2 = 0; count <= length; count++, count2 += 2)
  {
    sprintf(result+count2,"%02x",binary_numbers[length-count] & 0xFF);
  }

  pointer_reset(data);
  return 1;    
}



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
  int length = 0;
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
  BLOCK_HEIGHT - The block height of the network block string, 0 to get the current block height
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int network_block_string_to_blockchain_data(const char* DATA, const char* BLOCK_HEIGHT)
{
  // Constants
  const size_t DATA_LENGTH = strnlen(DATA,BUFFER_SIZE);

  // Variables
  size_t count;
  size_t count2;
  size_t count3;
  size_t number;
  char* current_block_height = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3;
  char* message_copy1;

  // define macros
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
  memset(blockchain_data.network_version_data,0,strnlen(blockchain_data.network_version_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.timestamp_data,0,strnlen(blockchain_data.timestamp_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.previous_block_hash_data,0,strnlen(blockchain_data.previous_block_hash_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.nonce_data,0,strnlen(blockchain_data.nonce_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.block_reward_transaction_version_data,0,strnlen(blockchain_data.block_reward_transaction_version_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.unlock_block_data,0,strnlen(blockchain_data.unlock_block_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.block_reward_input_data,0,strnlen(blockchain_data.block_reward_input_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.vin_type_data,0,strnlen(blockchain_data.vin_type_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.block_height_data,0,strnlen(blockchain_data.block_height_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.block_reward_output_data,0,strnlen(blockchain_data.block_reward_output_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.block_reward_data,0,strnlen(blockchain_data.block_reward_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.stealth_address_output_tag_data,0,strnlen(blockchain_data.stealth_address_output_tag_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.stealth_address_output_data,0,strnlen(blockchain_data.stealth_address_output_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.extra_bytes_size_data,0,strnlen(blockchain_data.extra_bytes_size_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.transaction_public_key_tag_data,0,strnlen(blockchain_data.transaction_public_key_tag_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.transaction_public_key_data,0,strnlen(blockchain_data.transaction_public_key_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.extra_nonce_tag_data,0,strnlen(blockchain_data.extra_nonce_tag_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.reserve_bytes_size_data,0,strnlen(blockchain_data.reserve_bytes_size_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.block_producer_public_address,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_public_address,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_delegates_name_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_delegates_name_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_delegates_name,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_delegates_name,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_public_address_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_public_address_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_public_address,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_public_address,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_backup_nodes_names_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_backup_nodes_names_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_backup_nodes_names,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_backup_nodes_names,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_1,0,strlen(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_1));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_1,0,strlen((const char*)blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_1));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_1,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_1,BUFFER_SIZE));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_1,0,strnlen((const char*)blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_1,BUFFER_SIZE));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_1,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_1,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_1,0,strnlen((const char*)blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_1,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_1,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_1,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_1,0,strnlen((const char*)blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_1,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_1,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_1,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_2,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_2,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_2,0,strnlen((const char*)blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_2,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_2,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_2,BUFFER_SIZE));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_2,0,strnlen((const char*)blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_2,BUFFER_SIZE));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_2,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_2,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_2,0,strnlen((const char*)blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_2,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_2,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_2,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_2,0,strnlen((const char*)blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_2,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_2,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_2,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_3,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_3,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_3,0,strnlen((const char*)blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_3,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_3,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_3,BUFFER_SIZE));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_3,0,strnlen((const char*)blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_3,BUFFER_SIZE));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_3,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_3,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_3,0,strnlen((const char*)blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_3,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_3,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_3,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_3,0,strnlen((const char*)blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_3,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_3,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_3,BUFFER_SIZE_NETWORK_BLOCK_DATA));  
  memset(blockchain_data.blockchain_reserve_bytes.vrf_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memset(blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data[count],0,strnlen(blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data[count],BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address[count],0,strnlen(blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address[count],BUFFER_SIZE_NETWORK_BLOCK_DATA));
  }
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memset(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count],0,strnlen(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count],BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count],0,strnlen(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count],BUFFER_SIZE_NETWORK_BLOCK_DATA));
  }
  memset(blockchain_data.ringct_version_data,0,strnlen(blockchain_data.ringct_version_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  memset(blockchain_data.transaction_amount_data,0,strnlen(blockchain_data.transaction_amount_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  for (count = 0; count < MAXIMUM_TRANSACATIONS_PER_BLOCK; count++)
  {
    memset(blockchain_data.transactions[count],0,strnlen(blockchain_data.transactions[count],BUFFER_SIZE_NETWORK_BLOCK_DATA));
  }
  count = 0;

  // network_version
  blockchain_data.network_version_data_length = 4;
  count+= blockchain_data.network_version_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.network_version_data,DATA,blockchain_data.network_version_data_length);

  // timestamp
  blockchain_data.timestamp_data_length = 10;
  count+= blockchain_data.timestamp_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.timestamp_data,&DATA[count-blockchain_data.timestamp_data_length],blockchain_data.timestamp_data_length);
  blockchain_data.timestamp = varint_decode((size_t)strtol(blockchain_data.timestamp_data, NULL, 16));

  // previous_block_hash
  blockchain_data.previous_block_hash_data_length = 64;
  count+= blockchain_data.previous_block_hash_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.previous_block_hash_data,&DATA[count-blockchain_data.previous_block_hash_data_length],blockchain_data.previous_block_hash_data_length);

  // nonce
  blockchain_data.nonce_data_length = 8;
  count+= blockchain_data.nonce_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.nonce_data,&DATA[count-blockchain_data.nonce_data_length],blockchain_data.nonce_data_length);

  // block_reward_transaction_version
  blockchain_data.block_reward_transaction_version_data_length = 2;
  count+= blockchain_data.block_reward_transaction_version_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.block_reward_transaction_version_data,&DATA[count-blockchain_data.block_reward_transaction_version_data_length],blockchain_data.block_reward_transaction_version_data_length);

  // unlock_block
  // get the current block height
  if (memcmp(BLOCK_HEIGHT,"0",1) == 0)
  {
    if (get_current_block_height(current_block_height, 0) == 0)
    {
      NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Could not get the current block height\nFunction: network_block_string_to_blockchain_data");
    }
    sscanf(current_block_height, "%zu", &number);
    number += 61;
  }
  else
  {
    sscanf(BLOCK_HEIGHT, "%zu", &number);
    number += 60;
  }  

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
  memcpy(blockchain_data.unlock_block_data,&DATA[count-blockchain_data.unlock_block_data_length],blockchain_data.unlock_block_data_length);
  blockchain_data.unlock_block = varint_decode((size_t)strtol(blockchain_data.unlock_block_data, NULL, 16));

  // block_reward_input
  blockchain_data.block_reward_input_data_length = 2;
  count+= blockchain_data.block_reward_input_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.block_reward_input_data,&DATA[count-blockchain_data.block_reward_input_data_length],blockchain_data.block_reward_input_data_length);

  // vin_type
  blockchain_data.vin_type_data_length = 2;
  count+= blockchain_data.vin_type_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.vin_type_data,&DATA[count-blockchain_data.vin_type_data_length],blockchain_data.vin_type_data_length);

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
  memcpy(blockchain_data.block_height_data,&DATA[count-blockchain_data.block_height_data_length],blockchain_data.block_height_data_length);
  blockchain_data.block_height = varint_decode((size_t)strtol(blockchain_data.block_height_data, NULL, 16));

  // block_reward_output
  blockchain_data.block_reward_output_data_length = 2;
  count+= blockchain_data.block_reward_output_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.block_reward_output_data,&DATA[count-blockchain_data.block_reward_output_data_length],blockchain_data.block_reward_output_data_length);

  // block_reward
  // since the block reward could be any number because of transactions fees, get the position of BLOCKCHAIN_RESERVED_BYTES_START to get the length of the block reward
  data3 = strstr(DATA,BLOCKCHAIN_RESERVED_BYTES_START);
  if (data3 == NULL)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  blockchain_data.block_reward_data_length = strnlen(DATA,BUFFER_SIZE) - strnlen(data3,BUFFER_SIZE) - count - 138;
  count+= blockchain_data.block_reward_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.block_reward_data,&DATA[count-blockchain_data.block_reward_data_length],blockchain_data.block_reward_data_length);
  blockchain_data.block_reward = varint_decode((size_t)strtol(blockchain_data.block_reward_data, NULL, 16));

  // stealth_address_output_tag
  blockchain_data.stealth_address_output_tag_data_length = 2;
  count+= blockchain_data.stealth_address_output_tag_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.stealth_address_output_tag_data,&DATA[count-blockchain_data.stealth_address_output_tag_data_length],blockchain_data.stealth_address_output_tag_data_length);

  // stealth_address_output
  blockchain_data.stealth_address_output_data_length = 64;
  count+= blockchain_data.stealth_address_output_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.stealth_address_output_data,&DATA[count-blockchain_data.stealth_address_output_data_length],blockchain_data.stealth_address_output_data_length);

  // extra_bytes_size
  blockchain_data.extra_bytes_size_data_length = 2;
  count+= blockchain_data.extra_bytes_size_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.extra_bytes_size_data,&DATA[count-blockchain_data.extra_bytes_size_data_length],blockchain_data.extra_bytes_size_data_length);
  blockchain_data.extra_bytes_size = varint_decode((size_t)strtol(blockchain_data.extra_bytes_size_data, NULL, 16));

  // transaction_public_key_tag
  blockchain_data.transaction_public_key_tag_data_length = 2;
  count+= blockchain_data.transaction_public_key_tag_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.transaction_public_key_tag_data,&DATA[count-blockchain_data.transaction_public_key_tag_data_length],blockchain_data.transaction_public_key_tag_data_length);

  // transaction_public_key
  blockchain_data.transaction_public_key_data_length = 64;
  count+= blockchain_data.transaction_public_key_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.transaction_public_key_data,&DATA[count-blockchain_data.transaction_public_key_data_length],blockchain_data.transaction_public_key_data_length);

  // extra_nonce_tag
  blockchain_data.extra_nonce_tag_data_length = 2;
  count+= blockchain_data.extra_nonce_tag_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.extra_nonce_tag_data,&DATA[count-blockchain_data.extra_nonce_tag_data_length],blockchain_data.extra_nonce_tag_data_length);

  // reserve_bytes_size
  blockchain_data.reserve_bytes_size_data_length = 2;
  count+= blockchain_data.reserve_bytes_size_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.reserve_bytes_size_data,&DATA[count-blockchain_data.reserve_bytes_size_data_length],blockchain_data.reserve_bytes_size_data_length);
  blockchain_data.reserve_bytes_size = varint_decode((size_t)strtol(blockchain_data.reserve_bytes_size_data, NULL, 16));


  // blockchain_reserve_bytes
  // skip the BLOCKCHAIN_RESERVED_BYTES_START
  count+= 66;

  // block_producer_delegates_name
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data_length = (strlen(DATA) - strlen(message_copy1)) - count;
  memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data,&DATA[count],blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data_length);
  // convert the hexadecimal string to a string
  for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data_length; count2++, number += 2)
  {
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memcpy(data2,&blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data[number],2);
    blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name[count2] = (int)strtol(data2, NULL, 16);
  }
  count += blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data_length + 64;
  
  // block_producer_public_address
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data_length = (strlen(DATA) - strlen(message_copy1)) - count;
  memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data,&DATA[count],blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data_length);
  // convert the hexadecimal string to a string
  for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data_length; count2++, number += 2)
  {
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memcpy(data2,&blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data[number],2);
    blockchain_data.blockchain_reserve_bytes.block_producer_public_address[count2] = (int)strtol(data2, NULL, 16);
  }
  count += blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data_length + 64;
  
  // block_producer_node_backup_count
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data_length = (strlen(DATA) - strlen(message_copy1)) - count;
  memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data,&DATA[count],blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data_length);
  // convert the hexadecimal string to a string
  for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data_length; count2++, number += 2)
  {
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memcpy(data2,&blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data[number],2);
    blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count[count2] = (int)strtol(data2, NULL, 16);
  }
  count += blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data_length + 64;

  // block_producer_backup_nodes_names
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data_length = (strlen(DATA) - strlen(message_copy1)) - count;
  memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data,&DATA[count],blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data_length);
  // convert the hexadecimal string to a string
  for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data_length; count2++, number += 2)
  {
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memcpy(data2,&blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data[number],2);
    blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names[count2] = (int)strtol(data2, NULL, 16);
  }
  count += blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data_length + 64;

  // vrf_node_public_and_secret_key_delegates_name
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name_data_length = (strlen(DATA) - strlen(message_copy1)) - count;
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name_data,&DATA[count],blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name_data_length);
  // convert the hexadecimal string to a string
  for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name_data_length; count2++, number += 2)
  {
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memcpy(data2,&blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name_data[number],2);
    blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name[count2] = (int)strtol(data2, NULL, 16);
  }
  count += blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name_data_length + 64;

  // vrf_node_public_and_secret_key_public_address
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address_data_length = (strlen(DATA) - strlen(message_copy1)) - count;
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address_data,&DATA[count],blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address_data_length);
  // convert the hexadecimal string to a string
  for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address_data_length; count2++, number += 2)
  {
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memcpy(data2,&blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address_data[number],2);
    blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address[count2] = (int)strtol(data2, NULL, 16);
  }
  count += blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address_data_length + 64;

  // vrf_node_public_and_secret_key_node_backup_count
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count_data_length = (strlen(DATA) - strlen(message_copy1)) - count;
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count_data,&DATA[count],blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count_data_length);
  // convert the hexadecimal string to a string
  for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count_data_length; count2++, number += 2)
  {
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memcpy(data2,&blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count_data[number],2);
    blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count[count2] = (int)strtol(data2, NULL, 16);
  }
  count += blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count_data_length + 64;

  // vrf_node_public_and_secret_key_backup_nodes_names
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names_data_length = (strlen(DATA) - strlen(message_copy1)) - count;
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names_data,&DATA[count],blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names_data_length);
  // convert the hexadecimal string to a string
  for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names_data_length; count2++, number += 2)
  {
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memcpy(data2,&blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names_data[number],2);
    blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names[count2] = (int)strtol(data2, NULL, 16);
  }
  count += blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names_data_length + 64;

  // vrf_node_random_data_delegates_name
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name_data_length = (strlen(DATA) - strlen(message_copy1)) - count;
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name_data,&DATA[count],blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name_data_length);
  // convert the hexadecimal string to a string
  for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name_data_length; count2++, number += 2)
  {
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memcpy(data2,&blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name_data[number],2);
    blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name[count2] = (int)strtol(data2, NULL, 16);
  }
  count += blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name_data_length + 64;

  // vrf_node_random_data_public_address
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address_data_length = (strlen(DATA) - strlen(message_copy1)) - count;
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address_data,&DATA[count],blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address_data_length);
  // convert the hexadecimal string to a string
  for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address_data_length; count2++, number += 2)
  {
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memcpy(data2,&blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address_data[number],2);
    blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address[count2] = (int)strtol(data2, NULL, 16);
  }
  count += blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address_data_length + 64;

  // vrf_node_random_data_node_backup_count
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count_data_length = (strlen(DATA) - strlen(message_copy1)) - count;
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count_data,&DATA[count],blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count_data_length);
  // convert the hexadecimal string to a string
  for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count_data_length; count2++, number += 2)
  {
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memcpy(data2,&blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count_data[number],2);
    blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count[count2] = (int)strtol(data2, NULL, 16);
  }
  count += blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count_data_length + 64;

  // vrf_node_random_data_backup_nodes_names
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names_data_length = (strlen(DATA) - strlen(message_copy1)) - count;
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names_data,&DATA[count],blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names_data_length);
  // convert the hexadecimal string to a string
  for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names_data_length; count2++, number += 2)
  {
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memcpy(data2,&blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names_data[number],2);
    blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names[count2] = (int)strtol(data2, NULL, 16);
  }
  count += blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names_data_length + 64;

  // vrf_node_next_main_nodes_delegates_name
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_delegates_name_data_length = (strlen(DATA) - strlen(message_copy1)) - count;
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_delegates_name_data,&DATA[count],blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_delegates_name_data_length);
  // convert the hexadecimal string to a string
  for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_delegates_name_data_length; count2++, number += 2)
  {
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memcpy(data2,&blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_delegates_name_data[number],2);
    blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_delegates_name[count2] = (int)strtol(data2, NULL, 16);
  }
  count += blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_delegates_name_data_length + 64;

  // vrf_node_next_main_nodes_public_address
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_public_address_data_length = (strlen(DATA) - strlen(message_copy1)) - count;
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_public_address_data,&DATA[count],blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_public_address_data_length);
  // convert the hexadecimal string to a string
  for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_public_address_data_length; count2++, number += 2)
  {
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memcpy(data2,&blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_public_address_data[number],2);
    blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_public_address[count2] = (int)strtol(data2, NULL, 16);
  }
  count += blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_public_address_data_length + 64;

  // vrf_node_next_main_nodes_node_backup_count
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count_data_length = (strlen(DATA) - strlen(message_copy1)) - count;
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count_data,&DATA[count],blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count_data_length);
  // convert the hexadecimal string to a string
  for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count_data_length; count2++, number += 2)
  {
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memcpy(data2,&blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count_data[number],2);
    blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count[count2] = (int)strtol(data2, NULL, 16);
  }
  count += blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count_data_length + 64;

  // vrf_node_next_main_nodes_backup_nodes_names
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_backup_nodes_names_data_length = (strlen(DATA) - strlen(message_copy1)) - count;
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_backup_nodes_names_data,&DATA[count],blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_backup_nodes_names_data_length);
  // convert the hexadecimal string to a string
  for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_backup_nodes_names_data_length; count2++, number += 2)
  {
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memcpy(data2,&blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_backup_nodes_names_data[number],2);
    blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_backup_nodes_names[count2] = (int)strtol(data2, NULL, 16);
  }
  count += blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_backup_nodes_names_data_length + 64;

  // vrf_public_key_round_part_1
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length_round_part_1 = (strlen(DATA) - strlen(message_copy1)) - count;
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_1,&DATA[count],blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length_round_part_1);
  // convert the hexadecimal string to a string
  for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length_round_part_1; count2++, number += 2)
  {
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memcpy(data2,&blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_1[number],2);
    blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_1[count2] = (int)strtol(data2, NULL, 16);
  }
  count += blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length_round_part_1 + 64;

  // vrf_alpha_string_round_part_1
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length_round_part_1 = (strlen(DATA) - strlen(message_copy1)) - count;
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_1,&DATA[count],blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length_round_part_1);
  // convert the hexadecimal string to a string
  for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length_round_part_1; count2++, number += 2)
  {
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memcpy(data2,&blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_1[number],2);
    blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_1[count2] = (int)strtol(data2, NULL, 16);
  }
  count += blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length_round_part_1 + 64;

  // vrf_proof_round_part_1
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length_round_part_1 = (strlen(DATA) - strlen(message_copy1)) - count;
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_1,&DATA[count],blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length_round_part_1);
  // convert the hexadecimal string to a string
  for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length_round_part_1; count2++, number += 2)
  {
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memcpy(data2,&blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_1[number],2);
    blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_1[count2] = (int)strtol(data2, NULL, 16);
  }
  count += blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length_round_part_1 + 64;

  // vrf_beta_string_round_part_1
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length_round_part_1 = (strlen(DATA) - strlen(message_copy1)) - count;
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_1,&DATA[count],blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length_round_part_1);
  // convert the hexadecimal string to a string
  for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length_round_part_1; count2++, number += 2)
  {
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memcpy(data2,&blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_1[number],2);
    blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_1[count2] = (int)strtol(data2, NULL, 16);
  }
  count += blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length_round_part_1 + 64;

  // vrf_data_round_part_1
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_1,&DATA[count],VRF_DATA_LENGTH);
  count += VRF_DATA_LENGTH + 64;

  // vrf_public_key_round_part_2
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length_round_part_2 = (strlen(DATA) - strlen(message_copy1)) - count;
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_2,&DATA[count],blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length_round_part_2);
  // convert the hexadecimal string to a string
  for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length_round_part_2; count2++, number += 2)
  {
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memcpy(data2,&blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_2[number],2);
    blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_2[count2] = (int)strtol(data2, NULL, 16);
  }
  count += blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length_round_part_2 + 64;

  // vrf_alpha_string_round_part_2
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length_round_part_2 = (strlen(DATA) - strlen(message_copy1)) - count;
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_2,&DATA[count],blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length_round_part_2);
  // convert the hexadecimal string to a string
  for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length_round_part_2; count2++, number += 2)
  {
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memcpy(data2,&blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_2[number],2);
    blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_2[count2] = (int)strtol(data2, NULL, 16);
  }
  count += blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length_round_part_2 + 64;

  // vrf_proof_round_part_2
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length_round_part_2 = (strlen(DATA) - strlen(message_copy1)) - count;
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_2,&DATA[count],blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length_round_part_2);
  // convert the hexadecimal string to a string
  for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length_round_part_2; count2++, number += 2)
  {
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memcpy(data2,&blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_2[number],2);
    blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_2[count2] = (int)strtol(data2, NULL, 16);
  }
  count += blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length_round_part_2 + 64;

  // vrf_beta_string_round_part_2
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length_round_part_2 = (strlen(DATA) - strlen(message_copy1)) - count;
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_2,&DATA[count],blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length_round_part_2);
  // convert the hexadecimal string to a string
  for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length_round_part_2; count2++, number += 2)
  {
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memcpy(data2,&blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_2[number],2);
    blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_2[count2] = (int)strtol(data2, NULL, 16);
  }
  count += blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length_round_part_2 + 64;

  // vrf_data_round_part_2
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_2,&DATA[count],VRF_DATA_LENGTH);
  count += VRF_DATA_LENGTH + 64;

  // vrf_public_key_round_part_3
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length_round_part_3 = (strlen(DATA) - strlen(message_copy1)) - count;
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_3,&DATA[count],blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length_round_part_3);
  // convert the hexadecimal string to a string
  for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length_round_part_3; count2++, number += 2)
  {
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memcpy(data2,&blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_3[number],2);
    blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_3[count2] = (int)strtol(data2, NULL, 16);
  }
  count += blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length_round_part_3 + 64;

  // vrf_alpha_string_round_part_3
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length_round_part_3 = (strlen(DATA) - strlen(message_copy1)) - count;
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_3,&DATA[count],blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length_round_part_3);
  // convert the hexadecimal string to a string
  for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length_round_part_3; count2++, number += 2)
  {
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memcpy(data2,&blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_3[number],2);
    blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_3[count2] = (int)strtol(data2, NULL, 16);
  }
  count += blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length_round_part_3 + 64;

  // vrf_proof_round_part_3
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length_round_part_3 = (strlen(DATA) - strlen(message_copy1)) - count;
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_3,&DATA[count],blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length_round_part_3);
  // convert the hexadecimal string to a string
  for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length_round_part_3; count2++, number += 2)
  {
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memcpy(data2,&blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_3[number],2);
    blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_3[count2] = (int)strtol(data2, NULL, 16);
  }
  count += blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length_round_part_3 + 64;

  // vrf_beta_string_round_part_3
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length_round_part_3 = (strlen(DATA) - strlen(message_copy1)) - count;
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_3,&DATA[count],blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length_round_part_3);
  // convert the hexadecimal string to a string
  for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length_round_part_3; count2++, number += 2)
  {
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memcpy(data2,&blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_3[number],2);
    blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_3[count2] = (int)strtol(data2, NULL, 16);
  }
  count += blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length_round_part_3 + 64;

  // vrf_data_round_part_3
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_3,&DATA[count],VRF_DATA_LENGTH);
  count += VRF_DATA_LENGTH + 64;

  // vrf_data
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_data,&DATA[count],VRF_DATA_LENGTH);
  count += VRF_DATA_LENGTH + 64;

  // next_block_verifiers_public_address_data
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data_length = (strlen(DATA) - strlen(message_copy1)) - count;
  for (count3 = 0; count3 < BLOCK_VERIFIERS_AMOUNT; count3++)
  { 
    memcpy(blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data[count3],&DATA[count],blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data_length);
    count += blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data_length + 64;
    // convert the hexadecimal string to a string
    for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data_length; count2++, number += 2)
    {
      memset(data2,0,strnlen(data2,BUFFER_SIZE));
      memcpy(data2,&blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data[count3][number],2);
      blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address[count3][count2] = (int)strtol(data2, NULL, 16);
    }
  }
  count += 64;

  // previous block hash
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.previous_block_hash_data_length = (strlen(DATA) - strlen(message_copy1)) - count;
  memcpy(blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,&DATA[count],blockchain_data.blockchain_reserve_bytes.previous_block_hash_data_length);
  count += blockchain_data.blockchain_reserve_bytes.previous_block_hash_data_length + 64;

  // block_validation_node_signature_data
  message_copy1 = strstr(DATA+count,BLOCKCHAIN_DATA_SEGMENT_STRING);
  blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data_length = (strlen(DATA) - strlen(message_copy1)) - count;
  size_t count5 = string_count(DATA,"5369675631");
  for (count3 = 0; count3 < count5; count3++)
  { 
    memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count3],&DATA[count],blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data_length);
    count += blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data_length + 64;
    // convert the hexadecimal string to a string
    for (number = 0, count2 = 0; number < blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data_length; count2++, number += 2)
    {
      memset(data2,0,strnlen(data2,BUFFER_SIZE));
      memcpy(data2,&blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count3][number],2);
      blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count3][count2] = (int)strtol(data2, NULL, 16);
    }
  }

  // set the count to the end of the reserve bytes
  message_copy1 = strstr(DATA,BLOCKCHAIN_RESERVED_BYTES_END);
  count = strnlen(DATA,BUFFER_SIZE) - strnlen(message_copy1,BUFFER_SIZE) + 62;

  // ringct_version
  blockchain_data.ringct_version_data_length = 2;
  count+= blockchain_data.ringct_version_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.ringct_version_data,&DATA[count-blockchain_data.ringct_version_data_length],blockchain_data.ringct_version_data_length);

  // transaction_amount
  // get how many bytes are left in the network_block_string
  blockchain_data.transaction_amount_data_length = (strnlen(DATA,BUFFER_SIZE) - count) % 64;
  count+= blockchain_data.transaction_amount_data_length;
  if (count > DATA_LENGTH)
  {
    NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
  }
  memcpy(blockchain_data.transaction_amount_data,&DATA[count-blockchain_data.transaction_amount_data_length],blockchain_data.transaction_amount_data_length);
  blockchain_data.transaction_amount = varint_decode((size_t)strtol(blockchain_data.transaction_amount_data, NULL, 16));

  // get all of the transactions
  for (number = 0; number < blockchain_data.transaction_amount; number++)
  {
    count+= 64;
    if (count > DATA_LENGTH)
    {
      NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_ERROR("Invalid network_block_string\nFunction: network_block_string_to_blockchain_data");
    }
    memcpy(blockchain_data.transactions[number],&DATA[count-64],64);
  }

  pointer_reset_all;
  return 1;

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
  size_t count2 = 0;
  size_t counter = 0;

  // define macros  
  #define BLOCKCHAIN_RESERVED_BYTES_LENGTH_TEXT "7c424c4f434b434841494e5f52455345525645445f42595445535f4c454e4754487c"
  #define BLOCKCHAIN_EXTRA_BYTES_LENGTH_TEXT "7c424c4f434b434841494e5f45585452415f42595445535f4c454e4754487c"
  #define BLOCKCHAIN_DATA_TO_NETWORK_BLOCK_ERROR(settings) \
  color_print(settings,"red"); \
  return 0; 

  memset(result,0,strlen(result));
  
  // network_version
  count = 0;
  blockchain_data.network_version_data_length = strnlen(blockchain_data.network_version_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+count,blockchain_data.network_version_data,blockchain_data.network_version_data_length);
  count += blockchain_data.network_version_data_length;
  
  // timestamp
  if (varint_encode((long long int)blockchain_data.timestamp,blockchain_data.timestamp_data) == 0)
  {
    BLOCKCHAIN_DATA_TO_NETWORK_BLOCK_ERROR("Could not create the varint for the timestamp\nFunction: blockchain_data_to_network_block_string");
  }
  blockchain_data.timestamp_data_length = strnlen(blockchain_data.timestamp_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+count,blockchain_data.timestamp_data,blockchain_data.timestamp_data_length);  
  count += blockchain_data.timestamp_data_length;

  // previous_block_hash
  blockchain_data.previous_block_hash_data_length = strnlen(blockchain_data.previous_block_hash_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+count,blockchain_data.previous_block_hash_data,blockchain_data.previous_block_hash_data_length);
  count += blockchain_data.previous_block_hash_data_length;

  // nonce
  blockchain_data.nonce_data_length = strnlen(blockchain_data.nonce_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+count,blockchain_data.nonce_data,blockchain_data.nonce_data_length);
  count += blockchain_data.nonce_data_length;

  // block_reward_transaction_version
  blockchain_data.block_reward_transaction_version_data_length = strnlen(blockchain_data.block_reward_transaction_version_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+count,blockchain_data.block_reward_transaction_version_data,blockchain_data.block_reward_transaction_version_data_length);
  count += blockchain_data.block_reward_transaction_version_data_length;

  // unlock_block
  if (varint_encode((long long int)blockchain_data.unlock_block,blockchain_data.unlock_block_data) == 0)
  {
    BLOCKCHAIN_DATA_TO_NETWORK_BLOCK_ERROR("Could not create the varint for the timestamp\nFunction: blockchain_data_to_network_block_string");
  }
  blockchain_data.unlock_block_data_length = strnlen(blockchain_data.unlock_block_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+count,blockchain_data.unlock_block_data,blockchain_data.unlock_block_data_length);  
  count += blockchain_data.unlock_block_data_length;

  // block_reward_input
  blockchain_data.block_reward_input_data_length = strnlen(blockchain_data.block_reward_input_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+count,blockchain_data.block_reward_input_data,blockchain_data.block_reward_input_data_length);
  count += blockchain_data.block_reward_input_data_length;

  // vin_type
  blockchain_data.vin_type_data_length = strnlen(blockchain_data.vin_type_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+count,blockchain_data.vin_type_data,blockchain_data.vin_type_data_length);
  count += blockchain_data.vin_type_data_length;

  // block_height
  if (varint_encode((long long int)blockchain_data.block_height,blockchain_data.block_height_data) == 0)
  {
    BLOCKCHAIN_DATA_TO_NETWORK_BLOCK_ERROR("Could not create the varint for the block height\nFunction: blockchain_data_to_network_block_string");
  }
  blockchain_data.block_height_data_length = strnlen(blockchain_data.block_height_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+count,blockchain_data.block_height_data,blockchain_data.block_height_data_length);  
  count += blockchain_data.block_height_data_length;

  // block_reward_output
  blockchain_data.block_reward_output_data_length = strnlen(blockchain_data.block_reward_output_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+count,blockchain_data.block_reward_output_data,blockchain_data.block_reward_output_data_length);
  count += blockchain_data.block_reward_output_data_length;

  // block_reward
  if (varint_encode((long long int)blockchain_data.block_reward,blockchain_data.block_reward_data) == 0)
  {
    BLOCKCHAIN_DATA_TO_NETWORK_BLOCK_ERROR("Could not create the varint for the block reward\nFunction: blockchain_data_to_network_block_string");
  }
  blockchain_data.block_reward_data_length = strnlen(blockchain_data.block_reward_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+count,blockchain_data.block_reward_data,blockchain_data.block_reward_data_length);  
  count += blockchain_data.block_reward_data_length;

  // stealth_address_output_tag
  blockchain_data.stealth_address_output_tag_data_length = strnlen(blockchain_data.stealth_address_output_tag_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+count,blockchain_data.stealth_address_output_tag_data,blockchain_data.stealth_address_output_tag_data_length);
  count += blockchain_data.stealth_address_output_tag_data_length;

  // stealth_address_output
  blockchain_data.stealth_address_output_data_length = strnlen(blockchain_data.stealth_address_output_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+count,blockchain_data.stealth_address_output_data,blockchain_data.stealth_address_output_data_length);
  count += blockchain_data.stealth_address_output_data_length;

  // extra_bytes_size
  memset(blockchain_data.extra_bytes_size_data,0,strlen(blockchain_data.extra_bytes_size_data));
  memcpy(blockchain_data.extra_bytes_size_data,"a3",2);
  blockchain_data.extra_bytes_size = 163;
  blockchain_data.extra_bytes_size_data_length = 2;
  memcpy(result+count,blockchain_data.extra_bytes_size_data,blockchain_data.extra_bytes_size_data_length);
  count += blockchain_data.extra_bytes_size_data_length;

  // transaction_public_key_tag
  blockchain_data.transaction_public_key_tag_data_length = strnlen(blockchain_data.transaction_public_key_tag_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+count,blockchain_data.transaction_public_key_tag_data,blockchain_data.transaction_public_key_tag_data_length);
  count += blockchain_data.transaction_public_key_tag_data_length;

  // transaction_public_key
  blockchain_data.transaction_public_key_data_length = strnlen(blockchain_data.transaction_public_key_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+count,blockchain_data.transaction_public_key_data,blockchain_data.transaction_public_key_data_length);
  count += blockchain_data.transaction_public_key_data_length;

  // extra_nonce_tag
  blockchain_data.extra_nonce_tag_data_length = strnlen(blockchain_data.extra_nonce_tag_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+count,blockchain_data.extra_nonce_tag_data,blockchain_data.extra_nonce_tag_data_length);
  count += blockchain_data.extra_nonce_tag_data_length;

  // reserve_bytes_size
  memset(blockchain_data.reserve_bytes_size_data,0,strlen(blockchain_data.reserve_bytes_size_data));
  memcpy(blockchain_data.reserve_bytes_size_data,"80",2);
  blockchain_data.reserve_bytes_size = 128;
  blockchain_data.reserve_bytes_size_data_length = 2;
  memcpy(result+count,blockchain_data.reserve_bytes_size_data,blockchain_data.reserve_bytes_size_data_length);
  count += blockchain_data.reserve_bytes_size_data_length;

  // blockchain_reserve_bytes
  memcpy(result+count,BLOCKCHAIN_RESERVED_BYTES_START,66);
  count+= 66;  

  // block_producer_delegates_name  
  blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data_length = strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  // convert the string to hexadecimal
  for (count2 = 0, counter = 0; count2 < blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data_length; count2++, counter += 2)
  {
    sprintf(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data+counter,"%02x",blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name[count2] & 0xFF);
  }
  blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data_length *= 2;
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data,blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data_length);  
  count += blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data_length;
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64;

  // block_producer_public_address  
  blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data_length = strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_public_address,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  // convert the string to hexadecimal
  for (count2 = 0, counter = 0; count2 < blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data_length; count2++, counter += 2)
  {
    sprintf(blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data+counter,"%02x",blockchain_data.blockchain_reserve_bytes.block_producer_public_address[count2] & 0xFF);
  }
  blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data_length *= 2;
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data,blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data_length);  
  count += blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data_length;
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64; 

  // block_producer_node_backup_count
  blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data_length = strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  // convert the string to hexadecimal
  if (memcmp(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,"0",1) == 0)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data,"30",2);
  }
  if (memcmp(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,"1",1) == 0)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data,"31",2);
  }
  if (memcmp(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,"2",1) == 0)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data,"32",2);
  }
  if (memcmp(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,"3",1) == 0)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data,"33",2);
  }
  if (memcmp(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,"4",1) == 0)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data,"34",2);
  }
  if (memcmp(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,"5",1) == 0)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data,"35",2);
  }
  blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data_length = 2;
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data,blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data_length);  
  count += blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data_length;
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64;  

  // block_producer_backup_nodes_names
  blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data_length = strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  // convert the string to hexadecimal
  for (count2 = 0, counter = 0; count2 < blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data_length; count2++, counter += 2)
  {
    sprintf(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data+counter,"%02x",blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names[count2] & 0xFF);
  }
  blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data_length *= 2;
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data,blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data_length);  
  count += blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data_length;
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64;

    // vrf_node_public_and_secret_key_delegates_name  
  blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name_data_length = strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  // convert the string to hexadecimal
  for (count2 = 0, counter = 0; count2 < blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name_data_length; count2++, counter += 2)
  {
    sprintf(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name_data+counter,"%02x",blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name[count2] & 0xFF);
  }
  blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name_data_length *= 2;
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name_data,blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name_data_length);  
  count += blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name_data_length;
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64;

  // vrf_node_public_and_secret_key_public_address  
  blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address_data_length = strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  // convert the string to hexadecimal
  for (count2 = 0, counter = 0; count2 < blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address_data_length; count2++, counter += 2)
  {
    sprintf(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address_data+counter,"%02x",blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address[count2] & 0xFF);
  }
  blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address_data_length *= 2;
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address_data,blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address_data_length);  
  count += blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address_data_length;
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64; 

  // vrf_node_public_and_secret_key_node_backup_count
  blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count_data_length = strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  // convert the string to hexadecimal
  if (memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count,"0",1) == 0)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count_data,"30",2);
  }
  if (memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count,"1",1) == 0)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count_data,"31",2);
  }
  if (memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count,"2",1) == 0)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count_data,"32",2);
  }
  if (memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count,"3",1) == 0)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count_data,"33",2);
  }
  if (memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count,"4",1) == 0)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count_data,"34",2);
  }
  if (memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count,"5",1) == 0)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count_data,"35",2);
  }
  blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count_data_length = 2;
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count_data,blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count_data_length);  
  count += blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count_data_length;
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64;  

  // vrf_node_public_and_secret_key_backup_nodes_names
  blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names_data_length = strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  // convert the string to hexadecimal
  for (count2 = 0, counter = 0; count2 < blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names_data_length; count2++, counter += 2)
  {
    sprintf(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names_data+counter,"%02x",blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names[count2] & 0xFF);
  }
  blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names_data_length *= 2;
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names_data,blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names_data_length);  
  count += blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names_data_length;
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64;

  // vrf_node_random_data_delegates_name  
  blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name_data_length = strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  // convert the string to hexadecimal
  for (count2 = 0, counter = 0; count2 < blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name_data_length; count2++, counter += 2)
  {
    sprintf(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name_data+counter,"%02x",blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name[count2] & 0xFF);
  }
  blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name_data_length *= 2;
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name_data,blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name_data_length);  
  count += blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name_data_length;
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64;

  // vrf_node_random_data_public_address  
  blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address_data_length = strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  // convert the string to hexadecimal
  for (count2 = 0, counter = 0; count2 < blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address_data_length; count2++, counter += 2)
  {
    sprintf(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address_data+counter,"%02x",blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address[count2] & 0xFF);
  }
  blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address_data_length *= 2;
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address_data,blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address_data_length);  
  count += blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address_data_length;
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64; 

  // vrf_node_random_data_node_backup_count
  blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count_data_length = strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  // convert the string to hexadecimal
  if (memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count,"0",1) == 0)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count_data,"30",2);
  }
  if (memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count,"1",1) == 0)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count_data,"31",2);
  }
  if (memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count,"2",1) == 0)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count_data,"32",2);
  }
  if (memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count,"3",1) == 0)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count_data,"33",2);
  }
  if (memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count,"4",1) == 0)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count_data,"34",2);
  }
  if (memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count,"5",1) == 0)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count_data,"35",2);
  }
  blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count_data_length = 2;
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count_data,blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count_data_length);  
  count += blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count_data_length;
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64;  

  // vrf_node_random_data_backup_nodes_names
  blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names_data_length = strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  // convert the string to hexadecimal
  for (count2 = 0, counter = 0; count2 < blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names_data_length; count2++, counter += 2)
  {
    sprintf(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names_data+counter,"%02x",blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names[count2] & 0xFF);
  }
  blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names_data_length *= 2;
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names_data,blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names_data_length);  
  count += blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names_data_length;
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64;

  // vrf_node_next_main_nodes_delegates_name  
  blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_delegates_name_data_length = strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_delegates_name,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  // convert the string to hexadecimal
  for (count2 = 0, counter = 0; count2 < blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_delegates_name_data_length; count2++, counter += 2)
  {
    sprintf(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_delegates_name_data+counter,"%02x",blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_delegates_name[count2] & 0xFF);
  }
  blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_delegates_name_data_length *= 2;
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_delegates_name_data,blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_delegates_name_data_length);  
  count += blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_delegates_name_data_length;
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64;

  // vrf_node_next_main_nodes_public_address  
  blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_public_address_data_length = strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_public_address,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  // convert the string to hexadecimal
  for (count2 = 0, counter = 0; count2 < blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_public_address_data_length; count2++, counter += 2)
  {
    sprintf(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_public_address_data+counter,"%02x",blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_public_address[count2] & 0xFF);
  }
  blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_public_address_data_length *= 2;
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_public_address_data,blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_public_address_data_length);  
  count += blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_public_address_data_length;
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64; 

  // vrf_node_next_main_nodes_node_backup_count
  blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count_data_length = strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  // convert the string to hexadecimal
  if (memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count,"0",1) == 0)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count_data,"30",2);
  }
  if (memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count,"1",1) == 0)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count_data,"31",2);
  }
  if (memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count,"2",1) == 0)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count_data,"32",2);
  }
  if (memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count,"3",1) == 0)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count_data,"33",2);
  }
  if (memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count,"4",1) == 0)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count_data,"34",2);
  }
  if (memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count,"5",1) == 0)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count_data,"35",2);
  }
  blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count_data_length = 2;
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count_data,blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count_data_length);  
  count += blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count_data_length;
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64;  

  // vrf_node_next_main_nodes_backup_nodes_names
  blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_backup_nodes_names_data_length = strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_backup_nodes_names,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  // convert the string to hexadecimal
  for (count2 = 0, counter = 0; count2 < blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_backup_nodes_names_data_length; count2++, counter += 2)
  {
    sprintf(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_backup_nodes_names_data+counter,"%02x",blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_backup_nodes_names[count2] & 0xFF);
  }
  blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_backup_nodes_names_data_length *= 2;
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_backup_nodes_names_data,blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_backup_nodes_names_data_length);  
  count += blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_backup_nodes_names_data_length;
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64;

  // vrf_public_key_round_part_1
  blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length_round_part_1 = strnlen(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_1,VRF_PUBLIC_KEY_LENGTH);
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_1,blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length_round_part_1);
  count += blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length_round_part_1;
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64;

  // vrf_alpha_string_round_part_1
  blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length_round_part_1 = strnlen(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_1,BUFFER_SIZE);
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_1,blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length_round_part_1);
  count += blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length_round_part_1; 
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64;

  // vrf_proof_round_part_1
  blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length_round_part_1 = strnlen(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_1,VRF_PROOF_LENGTH);
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_1,blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length_round_part_1);
  count += blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length_round_part_1;  
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64;

  // vrf_beta_string_round_part_1
  blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length_round_part_1 = strnlen(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_1,VRF_BETA_LENGTH);
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_1,blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length_round_part_1);
  count += blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length_round_part_1;
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64;

  // vrf_data_round_part_1
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_1,"74727565",VRF_DATA_LENGTH);
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_1,VRF_DATA_LENGTH);
  count += VRF_DATA_LENGTH;
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64;

  // vrf_public_key_round_part_2
  blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length_round_part_2 = strnlen(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_2,VRF_PUBLIC_KEY_LENGTH);
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_2,blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length_round_part_2);
  count += blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length_round_part_2;
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64;

  // vrf_alpha_string_round_part_2
  blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length_round_part_2 = strnlen(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_2,BUFFER_SIZE);
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_2,blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length_round_part_2);
  count += blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length_round_part_2; 
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64;

  // vrf_proof_round_part_2
  blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length_round_part_2 = strnlen(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_2,VRF_PROOF_LENGTH);
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_2,blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length_round_part_2);
  count += blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length_round_part_2;  
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64;

  // vrf_beta_string_round_part_2
  blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length_round_part_2 = strnlen(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_2,VRF_BETA_LENGTH);
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_2,blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length_round_part_2);
  count += blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length_round_part_2;
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64;

  // vrf_data_round_part_2
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_2,"74727565",VRF_DATA_LENGTH);
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_2,VRF_DATA_LENGTH);
  count += VRF_DATA_LENGTH;
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64;

  // vrf_public_key_round_part_3
  blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length_round_part_3 = strnlen(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_3,VRF_PUBLIC_KEY_LENGTH);
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_3,blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length_round_part_3);
  count += blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length_round_part_3;
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64;

  // vrf_alpha_string_round_part_3
  blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length_round_part_3 = strnlen(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_3,BUFFER_SIZE);
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_3,blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length_round_part_3);
  count += blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length_round_part_3;
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64; 

  // vrf_proof_round_part_3
  blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length_round_part_3 = strnlen(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_3,VRF_PROOF_LENGTH);
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_3,blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length_round_part_3);
  count += blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length_round_part_3;  
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64;

  // vrf_beta_string_round_part_3
  blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length_round_part_3 = strnlen(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_3,VRF_BETA_LENGTH);
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_3,blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length_round_part_3);
  count += blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length_round_part_3;
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64;

  // vrf_data_round_part_3
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_3,"74727565",VRF_DATA_LENGTH);
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_3,VRF_DATA_LENGTH);
  count += VRF_DATA_LENGTH;
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64;

  // vrf_data
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_data,"74727565",VRF_DATA_LENGTH);
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.vrf_data,VRF_DATA_LENGTH);
  count += VRF_DATA_LENGTH;
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64;

  // next_block_verifiers_public_address_data  
  blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data_length = 196;
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    // convert the string to hexadecimal
    for (count2 = 0, counter = 0; count2 < XCASH_WALLET_LENGTH; count2++, counter += 2)
    {
      sprintf(blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data[count]+counter,"%02x",blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address[count][count2] & 0xFF);
    }
    memcpy(result+strnlen(result,BUFFER_SIZE),blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data[count],blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data_length);  
    if (count+1 != BLOCK_VERIFIERS_AMOUNT)
    {
      memcpy(result+strnlen(result,BUFFER_SIZE),BLOCKCHAIN_DATA_SEGMENT_STRING,64);
    }
  }    
  count = strnlen(result,BUFFER_SIZE);
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64;

  // previous block hash
  blockchain_data.blockchain_reserve_bytes.previous_block_hash_data_length = strnlen(blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+count,blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,blockchain_data.blockchain_reserve_bytes.previous_block_hash_data_length);
  count += blockchain_data.blockchain_reserve_bytes.previous_block_hash_data_length;
  memcpy(result+count,BLOCKCHAIN_DATA_SEGMENT_STRING,64);
  count += 64;

  // block_validation_node_signature_data  
  blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data_length = BLOCK_VALIDATION_NODE_SIGNED_BLOCK_LENGTH;
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    // convert the string to hexadecimal
    for (count2 = 0, counter = 0; count2 < XCASH_SIGN_DATA_LENGTH; count2++, counter += 2)
    {
      sprintf(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count]+counter,"%02x",blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count][count2] & 0xFF);
    }
    memcpy(result+strnlen(result,BUFFER_SIZE),blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count],blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data_length);  
    if (count+1 != BLOCK_VERIFIERS_AMOUNT)
    {
      memcpy(result+strnlen(result,BUFFER_SIZE),BLOCKCHAIN_DATA_SEGMENT_STRING,64);
    }
  }  
  count = strnlen(result,BUFFER_SIZE);
  memcpy(result+count,BLOCKCHAIN_RESERVED_BYTES_END,62);
  count += 62;

  // ringct_version
  blockchain_data.ringct_version_data_length = strnlen(blockchain_data.ringct_version_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+count,blockchain_data.ringct_version_data,blockchain_data.ringct_version_data_length);
  count += blockchain_data.ringct_version_data_length;

  // transaction_amount
  if (varint_encode((long long int)blockchain_data.transaction_amount,blockchain_data.transaction_amount_data) == 0)
  {
    BLOCKCHAIN_DATA_TO_NETWORK_BLOCK_ERROR("Could not create the varint for the transaction amount\nFunction: blockchain_data_to_network_block_string");
  }
  blockchain_data.transaction_amount_data_length = strnlen(blockchain_data.transaction_amount_data,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(result+count,blockchain_data.transaction_amount_data,blockchain_data.transaction_amount_data_length);  
  count += blockchain_data.transaction_amount_data_length;

  // get all of the transactions
  for (count2 = 0; count2 < blockchain_data.transaction_amount; count2++)
  {
    memcpy(result+count,blockchain_data.transactions[count2],64);
    count += 64;
  }

  return 1;

  #undef BLOCKCHAIN_RESERVED_BYTES_LENGTH_TEXT
  #undef BLOCKCHAIN_EXTRA_BYTES_LENGTH_TEXT
  #undef BLOCKCHAIN_DATA_TO_NETWORK_BLOCK_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: verify_network_block_data
Description: Verifies a blockchain_data struct
Parameters:
  BLOCK_VALIDATION_SIGNATURES_SETTINGS - 0 to not verify the block validation signatures, 1 to verify the block validation signatures. The consensus node when submitting the block should be the only time when the block validation signatures are verified
  PREVIOUS_BLOCK_HASH_SETTINGS - 0 to not verify the previous block hash, 1 to verify the previous block hash. The blockchain_test should be the only time when the previous block hash is not verified
  TRANSACTIONS_SETTINGS - 0 to not verify that the transactions are in the network, 1 to verify that the transactions are in the network. The blockchain_test should be the only time when the transactions are not verified
  PREVIOUS_NETWORK_BLOCK_RESERVE_BYTES - The previous network blocks reserve bytes
  BLOCK_HEIGHT - The block height of the network block string, 0 to get the current block height
Return: 0 if an error has occured or it is not verified, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int verify_network_block_data(const int BLOCK_VALIDATION_SIGNATURES_SETTINGS, const int PREVIOUS_BLOCK_HASH_SETTINGS, const int TRANSACTIONS_SETTINGS, const char* BLOCK_HEIGHT, const char* PREVIOUS_NETWORK_BLOCK_RESERVE_BYTES)
{
  // Variables
  size_t count;
  size_t count2;
  size_t count3;
  size_t number;
  char* previous_block_hash = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* current_block_height = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* network_block_string = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* previous_network_block_reserve_bytes_block_verifiers_public_addresses_data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* previous_network_block_reserve_bytes_block_verifiers_public_addresses[BLOCK_VERIFIERS_AMOUNT];
  char* message_copy1;

  // define macros
  #define BLOCK_REWARD_TRANSACTION_VERSION "02"
  #define BLOCK_REWARD_INPUT "01"
  #define VIN_TYPE "ff"
  #define BLOCK_REWARD_OUTPUT "01"
  #define STEALTH_ADDRESS_OUTPUT_TAG "02"
  #define TRANSACTION_PUBLIC_KEY_TAG "01"
  #define EXTRA_NONCE_TAG "02"
  #define RINGCT_VERSION "00"
  #define BLOCK_VALIDATION_NODE_SIGNATURE_DATA "7c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c5369675631"

  #define pointer_reset_all \
  free(previous_block_hash); \
  previous_block_hash = NULL; \
  free(current_block_height); \
  current_block_height = NULL; \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; \
  free(network_block_string); \
  network_block_string = NULL; \
  free(previous_network_block_reserve_bytes_block_verifiers_public_addresses_data); \
  previous_network_block_reserve_bytes_block_verifiers_public_addresses_data = NULL;

  #define VERIFY_NETWORK_BLOCK_DATA_ERROR(settings) \
  color_print(settings,"red"); \
  pointer_reset_all; \
  return 0; 

  // check if the memory needed was allocated on the heap successfully
  if (previous_block_hash == NULL || current_block_height == NULL || data == NULL || data2 == NULL || network_block_string == NULL || previous_network_block_reserve_bytes_block_verifiers_public_addresses_data == NULL)
  {
    if (previous_block_hash != NULL)
    {
      pointer_reset(previous_block_hash);
    }
    if (current_block_height != NULL)
    {
      pointer_reset(current_block_height);
    }
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    if (network_block_string != NULL)
    {
      pointer_reset(network_block_string);
    }
    if (previous_network_block_reserve_bytes_block_verifiers_public_addresses_data != NULL)
    {
      pointer_reset(previous_network_block_reserve_bytes_block_verifiers_public_addresses_data);
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
  if (PREVIOUS_BLOCK_HASH_SETTINGS == 1)
  {
    if (get_previous_block_hash(previous_block_hash,0) == 0)
    {
      VERIFY_NETWORK_BLOCK_DATA_ERROR("Could not get the previous block hash\nFunction: verify_network_block_data");
    }
    if (blockchain_data.previous_block_hash_data_length != 64 || memcmp(blockchain_data.previous_block_hash_data,previous_block_hash,64) != 0)
    {
      VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid previous block hash\nFunction: verify_network_block_data");
    } 
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
  if (memcmp(BLOCK_HEIGHT,"0",1) == 0)
  {
    if (get_current_block_height(current_block_height,0) == 0)
    {
      VERIFY_NETWORK_BLOCK_DATA_ERROR("Could not get the current block height\nFunction: verify_network_block_data");
    }
    sscanf(current_block_height, "%zu", &number);
    if ((blockchain_data.unlock_block <= 2097091 && blockchain_data.unlock_block_data_length != 6) || (blockchain_data.unlock_block > 2097091 && blockchain_data.unlock_block_data_length != 8) || blockchain_data.unlock_block != number+61)
    {
      VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid unlock_block\nFunction: verify_network_block_data");
    }
  }
  else
  {
    sscanf(BLOCK_HEIGHT, "%zu", &number);
    if ((blockchain_data.unlock_block <= 2097091 && blockchain_data.unlock_block_data_length != 6) || (blockchain_data.unlock_block > 2097091 && blockchain_data.unlock_block_data_length != 8) || blockchain_data.unlock_block != number+60)
    {
      VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid unlock_block\nFunction: verify_network_block_data");
    }
  }  

  // block_reward_input
  if (blockchain_data.block_reward_input_data_length != 2 || memcmp(blockchain_data.block_reward_input_data,BLOCK_REWARD_INPUT,2) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid block_reward_input\nFunction: verify_network_block_data");
  }

  // vin_type
  if (blockchain_data.vin_type_data_length != 2 || memcmp(blockchain_data.vin_type_data,VIN_TYPE,2) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid vin_type\nFunction: verify_network_block_data");
  }

  // block_height
  if (memcmp(BLOCK_HEIGHT,"0",1) == 0)
  {
    if ((blockchain_data.block_height <= 2097151 && blockchain_data.block_height_data_length != 6) || (blockchain_data.block_height > 2097151 && blockchain_data.block_height_data_length != 8) || blockchain_data.block_height != number+1)
    {
      VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid block_height\nFunction: verify_network_block_data");
    }
  }
  else
  {
    if ((blockchain_data.block_height <= 2097151 && blockchain_data.block_height_data_length != 6) || (blockchain_data.block_height > 2097151 && blockchain_data.block_height_data_length != 8))
    {
      VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid block_height\nFunction: verify_network_block_data");
    }
  }
  

  // block_reward_output
  if (blockchain_data.block_reward_output_data_length != 2 || memcmp(blockchain_data.block_reward_output_data,BLOCK_REWARD_OUTPUT,2) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid block_reward_output\nFunction: verify_network_block_data");
  }

  // block_reward
  if ((blockchain_data.block_reward <= 34359738367 && blockchain_data.block_reward_data_length != 10) || (blockchain_data.block_reward > 34359738367 && blockchain_data.block_reward <= 4398046511104 && blockchain_data.block_reward_data_length != 12) || (blockchain_data.block_reward > 4398046511104 && blockchain_data.block_reward <= 562949953421312 && blockchain_data.block_reward_data_length != 14) || (blockchain_data.block_reward > 562949953421312 && blockchain_data.block_reward <= 72057594037927936 && blockchain_data.block_reward_data_length != 16) || (blockchain_data.block_reward > 72057594037927936 && blockchain_data.block_reward_data_length != 18))
  {
    printf("%zu",blockchain_data.block_reward);
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid block_reward\nFunction: verify_network_block_data");
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
  if (blockchain_data.extra_bytes_size_data_length != 2 || blockchain_data.extra_bytes_size != 163 || (((blockchain_data.transaction_public_key_tag_data_length + blockchain_data.transaction_public_key_data_length + blockchain_data.extra_nonce_tag_data_length + blockchain_data.reserve_bytes_size_data_length) / 2) + blockchain_data.reserve_bytes_size) != blockchain_data.extra_bytes_size)
  {
    printf("%zu", blockchain_data.reserve_bytes_size_data_length);
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid extra_bytes_size\nFunction: verify_network_block_data");
  }

  // transaction_public_key_tag
  if (blockchain_data.transaction_public_key_tag_data_length != 2 || memcmp(blockchain_data.transaction_public_key_tag_data,TRANSACTION_PUBLIC_KEY_TAG,2) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid transaction_public_key_tag\nFunction: verify_network_block_data");
  }

  // transaction_public_key
  if (blockchain_data.transaction_public_key_data_length != 64)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid transaction_public_key\nFunction: verify_network_block_data");
  }

  // extra_nonce_tag
  if (blockchain_data.extra_nonce_tag_data_length != 2 || memcmp(blockchain_data.extra_nonce_tag_data,EXTRA_NONCE_TAG,2) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid extra_nonce_tag\nFunction: verify_network_block_data");
  }

  // reserve_bytes_size
  if (blockchain_data.reserve_bytes_size_data_length != 2 || blockchain_data.reserve_bytes_size != 128)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid reserve_bytes_size\nFunction: verify_network_block_data");
  }

  // blockchain_reserve_bytes
  // block_producer_delegates_name
  if (blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data_length < 10 || blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data_length > 40)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid block_producer_delegates_name\nFunction: verify_network_block_data");
  }

  // block_producer_public_address
  if (blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data_length != 196 || memcmp(blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data,"584341",6) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid block_producer_public_address\nFunction: verify_network_block_data");
  }

  // block_producer_node_backup_count
  if (blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data_length != 2 || (memcmp(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,"0",1) != 0 && memcmp(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,"1",1) != 0 && memcmp(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,"2",1) != 0 && memcmp(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,"3",1) != 0 && memcmp(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,"4",1) != 0 && memcmp(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,"5",1) != 0))
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid block_producer_node_backup_count\nFunction: verify_network_block_data");
  }

  // block_producer_backup_nodes_names
  if (blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data_length < 58 || blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data_length > 208 || string_count(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data,"2c") != 4)
  {  
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid block_producer_backup_nodes_names\nFunction: verify_network_block_data");
  }

  // vrf_node_public_and_secret_key_delegates_name
  if (blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name_data_length < 10 || blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name_data_length > 40)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid vrf_node_public_and_secret_key_delegates_name\nFunction: verify_network_block_data");
  }

  // vrf_node_public_and_secret_key_public_address
  if (blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address_data_length != 196 || memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address_data,"584341",6) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid vrf_node_public_and_secret_key_public_address\nFunction: verify_network_block_data");
  }

  // vrf_node_public_and_secret_key_node_backup_count
  if (blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count_data_length != 2 || (memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count,"0",1) != 0 && memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count,"1",1) != 0 && memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count,"2",1) != 0 && memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count,"3",1) != 0 && memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count,"4",1) != 0 && memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count,"5",1) != 0))
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid vrf_node_public_and_secret_key_node_backup_count\nFunction: verify_network_block_data");
  }

  // vrf_node_public_and_secret_key_backup_nodes_names
  if (blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names_data_length < 58 || blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names_data_length > 208 || string_count(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names_data,"2c") != 4)
  {  
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid vrf_node_public_and_secret_key_backup_nodes_names\nFunction: verify_network_block_data");
  }

  // vrf_node_random_data_delegates_name
  if (blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name_data_length < 10 || blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name_data_length > 40)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid vrf_node_random_data_delegates_name\nFunction: verify_network_block_data");
  }

  // vrf_node_random_data_public_address
  if (blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address_data_length != 196 || memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address_data,"584341",6) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid vrf_node_random_data_public_address\nFunction: verify_network_block_data");
  }

  // vrf_node_random_data_node_backup_count
  if (blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count_data_length != 2 || (memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count,"0",1) != 0 && memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count,"1",1) != 0 && memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count,"2",1) != 0 && memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count,"3",1) != 0 && memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count,"4",1) != 0 && memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count,"5",1) != 0))
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid vrf_node_random_data_node_backup_count\nFunction: verify_network_block_data");
  }  

  // vrf_node_random_data_backup_nodes_names
  if (blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names_data_length < 58 || blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names_data_length > 208 || string_count(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names_data,"2c") != 4)
  {  
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid vrf_node_random_data_backup_nodes_names\nFunction: verify_network_block_data");
  }

  // vrf_node_next_main_nodes_delegates_name
  if (blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_delegates_name_data_length < 10 || blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_delegates_name_data_length > 40)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid vrf_node_next_main_nodes_delegates_name\nFunction: verify_network_block_data");
  }

  // vrf_node_next_main_nodes_public_address
  if (blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_public_address_data_length != 196 || memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_public_address_data,"584341",6) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid vrf_node_next_main_nodes_public_address\nFunction: verify_network_block_data");
  }

  // vrf_node_next_main_nodes_node_backup_count
  if (blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count_data_length != 2 || (memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count,"0",1) != 0 && memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count,"1",1) != 0 && memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count,"2",1) != 0 && memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count,"3",1) != 0 && memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count,"4",1) != 0 && memcmp(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_node_backup_count,"5",1) != 0))
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid vrf_node_next_main_nodes_node_backup_count\nFunction: verify_network_block_data");
  }  

  // vrf_node_next_main_nodes_backup_nodes_names
  if (blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_backup_nodes_names_data_length < 58 || blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_backup_nodes_names_data_length > 208 || string_count(blockchain_data.blockchain_reserve_bytes.vrf_node_next_main_nodes_backup_nodes_names_data,"2c") != 4)
  {  
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid vrf_node_next_main_nodes_backup_nodes_names\nFunction: verify_network_block_data");
  }

  // vrf_public_key_round_part_1
  if (blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length_round_part_1 != VRF_PUBLIC_KEY_LENGTH || strlen((const char*)blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_1) != crypto_vrf_PUBLICKEYBYTES || crypto_vrf_is_valid_key((const unsigned char*)blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_1) != 1)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid vrf_public_key_round_part_1\nFunction: verify_network_block_data");
  }

  // vrf_alpha_string_round_part_1
  if (blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length_round_part_1 != VRF_PUBLIC_KEY_LENGTH || memcmp(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_1,blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_1,VRF_PUBLIC_KEY_LENGTH) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid vrf_alpha_string_round_part_1\nFunction: verify_network_block_data");
  }

  // vrf_proof_round_part_1
  if (blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length_round_part_1 != VRF_PROOF_LENGTH || strlen((const char*)blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_1) != crypto_vrf_PROOFBYTES)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid vrf_proof_round_part_1\nFunction: verify_network_block_data");
  }

  // vrf_beta_string_round_part_1
  if (blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length_round_part_1 != VRF_BETA_LENGTH || strlen((const char*)blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_1) != crypto_vrf_OUTPUTBYTES)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid vrf_public_key_round_part_1\nFunction: verify_network_block_data");
  }

  // vrf_data_round_part_1
  if (crypto_vrf_verify((unsigned char*)blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_1,(const unsigned char*)blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_1,(const unsigned char*)blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_1,(const unsigned char*)blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_1,(unsigned long long)strlen((const char*)blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_1)) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid vrf_data_round_part_1\nFunction: verify_network_block_data");
  }
  memset(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_1,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_1,11));
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_1,"74727565",VRF_DATA_LENGTH);

  // vrf_public_key_round_part_2
  if (blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length_round_part_2 != VRF_PUBLIC_KEY_LENGTH || strlen((const char*)blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_2) != crypto_vrf_PUBLICKEYBYTES || crypto_vrf_is_valid_key((const unsigned char*)blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_2) != 1)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid vrf_public_key_round_part_2\nFunction: verify_network_block_data");
  }

  // vrf_alpha_string_round_part_2
  // convert the previous block hash to hexadecimal
  memset(data2,0,strnlen(data2,BUFFER_SIZE));
  for (count = 0, number = 0; count < 64; count++, number += 2)
  {
    sprintf(data2+number,"%02x",blockchain_data.previous_block_hash_data[count] & 0xFF);
  }
  if (blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length_round_part_2 != 328 || memcmp(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_2,data2,64) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid vrf_alpha_string_round_part_2\nFunction: verify_network_block_data");
  }

  // vrf_proof_round_part_2
  if (blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length_round_part_2 != VRF_PROOF_LENGTH || strlen((const char*)blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_1) != crypto_vrf_PROOFBYTES)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid vrf_proof_round_part_2\nFunction: verify_network_block_data");
  }

  // vrf_beta_string_round_part_2
  if (blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length_round_part_2 != VRF_BETA_LENGTH || strlen((const char*)blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_2) != crypto_vrf_OUTPUTBYTES)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid vrf_beta_string_round_part_2\nFunction: verify_network_block_data");
  }

  // vrf_data_round_part_2
  if (crypto_vrf_verify((unsigned char*)blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_2,(const unsigned char*)blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_2,(const unsigned char*)blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_2,(const unsigned char*)blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_2,(unsigned long long)strlen((const char*)blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_2)) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid vrf_data_round_part_2\nFunction: verify_network_block_data");
  }
  memset(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_2,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_2,11));
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_2,"74727565",VRF_DATA_LENGTH);

  // vrf_public_key_round_part_3
  if (blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length_round_part_3 != VRF_PUBLIC_KEY_LENGTH || strlen((const char*)blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_1) != crypto_vrf_PUBLICKEYBYTES || crypto_vrf_is_valid_key((const unsigned char*)blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_3) != 1 || memcmp(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_3,blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_1,VRF_PUBLIC_KEY_LENGTH) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid vrf_public_key_round_part_3\nFunction: verify_network_block_data");
  }

  // vrf_alpha_string_round_part_3
  // convert the previous block hash to hexadecimal
  memset(data2,0,strnlen(data2,BUFFER_SIZE));
  for (count = 0, number = 0; count < 64; count++, number += 2)
  {
    sprintf(data2+number,"%02x",blockchain_data.previous_block_hash_data[count] & 0xFF);
  }
  if (blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length_round_part_3 != 328 || memcmp(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_3,data2,64) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid vrf_alpha_string_round_part_3\nFunction: verify_network_block_data");
  }

  // vrf_proof_round_part_3
  if (blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length_round_part_3 != VRF_PROOF_LENGTH || strlen((const char*)blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_1) != crypto_vrf_PROOFBYTES)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid vrf_proof_round_part_3\nFunction: verify_network_block_data");
  }

  // vrf_beta_string_round_part_3
  if (blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length_round_part_3 != VRF_BETA_LENGTH || strlen((const char*)blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_3) != crypto_vrf_OUTPUTBYTES)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid vrf_public_key_round_part_3\nFunction: verify_network_block_data");
  }

  // vrf_data_round_part_3
  if (crypto_vrf_verify((unsigned char*)blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_3,(const unsigned char*)blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_3,(const unsigned char*)blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_3,(const unsigned char*)blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_3,(unsigned long long)strlen((const char*)blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_3)) != 0)
  {
    VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid vrf_data_round_part_3\nFunction: verify_network_block_data");
  }
  memset(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_3,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_3,11));
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_3,"74727565",VRF_DATA_LENGTH);

  // vrf_data
  memset(blockchain_data.blockchain_reserve_bytes.vrf_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_data,11));
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_data,"74727565",VRF_DATA_LENGTH);

  // previous_block_hash
  if (PREVIOUS_BLOCK_HASH_SETTINGS == 1)
  {
    if (blockchain_data.blockchain_reserve_bytes.previous_block_hash_data_length != 64 || memcmp(blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,previous_block_hash,64) != 0)
    {
      VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid previous block hash\nFunction: verify_network_block_data");
    }
  }    

  // block_validation_node_signature
  if (BLOCK_VALIDATION_SIGNATURES_SETTINGS == 1)
  {    
    // initialize the previous_network_block_reserve_bytes_block_verifiers_public_addresses
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      previous_network_block_reserve_bytes_block_verifiers_public_addresses[count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));

      // check if the memory needed was allocated on the heap successfully
      if (previous_network_block_reserve_bytes_block_verifiers_public_addresses[count] == NULL)
      {
        color_print("Could not allocate the memory needed on the heap","red");
        exit(0);
      }
    }

    // get the next block verifiers public addresses from the previous network blocks reserve bytes
    message_copy1 = strstr(PREVIOUS_NETWORK_BLOCK_RESERVE_BYTES,BLOCK_VALIDATION_NODE_SIGNATURE_DATA);
    count2 = strlen(PREVIOUS_NETWORK_BLOCK_RESERVE_BYTES) - (strlen(message_copy1) + 64);
    count = strlen(PREVIOUS_NETWORK_BLOCK_RESERVE_BYTES) - (strlen(message_copy1) + 64 + 26000);
    memcpy(previous_network_block_reserve_bytes_block_verifiers_public_addresses_data,&PREVIOUS_NETWORK_BLOCK_RESERVE_BYTES[count],count2 - count);
    
    for (count = 0, count2 = 0; count < BLOCK_VERIFIERS_AMOUNT; count++, count2 += 260)
    {
      memset(data2,0,strlen(data2));
      memcpy(data2,&previous_network_block_reserve_bytes_block_verifiers_public_addresses_data[count2],196);
      
      // convert the hexadecimal string to a string
      for (number = 0, count3 = 0; number < 196; count3++, number += 2)
      {
        memset(data,0,strnlen(data,BUFFER_SIZE));
        memcpy(data,&data2[number],2);
        previous_network_block_reserve_bytes_block_verifiers_public_addresses[count][count3] = (int)strtol(data, NULL, 16);
      }      
    }

    // create a network block string
    if (blockchain_data_to_network_block_string(network_block_string) == 0)
    {
      VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nThe block was not signed by the required amount of block validation nodes\nFunction: verify_network_block_data");
    }
    // replace the block validation signatures with the GET_BLOCK_TEMPLATE_RESERVED_BYTES
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    { 
      string_replace(network_block_string,blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count],GET_BLOCK_TEMPLATE_RESERVED_BYTES);
    }

    // check if at least 67 of the next block verifiers in the previous block signed the data in the current block
    for (count = 0, number = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {       
      if (memcmp(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count],"5369675631",10) == 0)
      {
        // check the signed data
        for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
        {       
          if (data_verify(0,previous_network_block_reserve_bytes_block_verifiers_public_addresses[count2],blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count],network_block_string) == 1)
          {
            number++;
            break;
          }
        }
      }
    }
    if (number < BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nThe block was not signed by the required amount of block validation nodes\nFunction: verify_network_block_data");
    }
  }

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
  if (TRANSACTIONS_SETTINGS == 1)
  {
    if (verify_blockchain_network_transactions(blockchain_data.transactions,blockchain_data.transaction_amount,1,0) == 0)
    {
      VERIFY_NETWORK_BLOCK_DATA_ERROR("Invalid network_block_string\nInvalid transactions\nFunction: verify_network_block_data");
    }
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