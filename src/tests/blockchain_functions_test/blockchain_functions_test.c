#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "variables.h"
#include "structures.h"
#include "define_macros_test.h"

#include "blockchain_functions.h"

#include "variables_test.h"
#include "blockchain_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: blockchain_functions_test
Description: Test the blockchain_functions
Return: The number of passed blockchain test
-----------------------------------------------------------------------------------------------------------
*/

int blockchain_functions_test(void)
{ 
  // define macros
  #define INVALID_NETWORK_VERSION_CODE \
  memset(blockchain_data.network_version_data,0,strlen(blockchain_data.network_version_data)); \
  memcpy(blockchain_data.network_version_data,"0000",sizeof(NETWORK_VERSION)-1);

  #define INVALID_TIMESTAMP_CODE blockchain_data.timestamp_data_length = 1;

  #define INVALID_NETWORK_BLOCK_NONCE_CODE \
  memset(blockchain_data.nonce_data,0,strlen(blockchain_data.nonce_data)); \
  memcpy(blockchain_data.nonce_data,"00001111",sizeof(NETWORK_VERSION)-1);

  #define INVALID_BLOCK_REWARD_TRANSACTION_VERSION_CODE \
  memset(blockchain_data.block_reward_transaction_version_data,0,strlen(blockchain_data.block_reward_transaction_version_data)); \
  memcpy(blockchain_data.block_reward_transaction_version_data,"0000",sizeof(BLOCK_REWARD_TRANSACTION_VERSION)-1);

  #define INVALID_UNLOCK_BLOCK_CODE blockchain_data.unlock_block = 100

  #define INVALID_BLOCK_REWARD_INPUT_CODE \
  memset(blockchain_data.block_reward_input_data,0,strlen(blockchain_data.block_reward_input_data)); \
  memcpy(blockchain_data.block_reward_input_data,"0000",sizeof(BLOCK_REWARD_INPUT)-1);

  #define INVALID_VIN_TYPE_CODE \
  memset(blockchain_data.vin_type_data,0,strlen(blockchain_data.vin_type_data)); \
  memcpy(blockchain_data.vin_type_data,"0000",sizeof(VIN_TYPE)-1);

  #define INVALID_BLOCK_HEIGHT_CODE blockchain_data.block_height = 100;

  #define INVALID_BLOCK_REWARD_OUTPUT_CODE \
  memset(blockchain_data.block_reward_output_data,0,strlen(blockchain_data.block_reward_output_data)); \
  memcpy(blockchain_data.block_reward_output_data,"0000",sizeof(BLOCK_REWARD_OUTPUT)-1);

  #define INVALID_BLOCK_REWARD_CODE blockchain_data.block_reward = 100;

  #define INVALID_STEALTH_ADDRESS_OUTPUT_TAG_CODE \
  memset(blockchain_data.stealth_address_output_tag_data,0,strlen(blockchain_data.stealth_address_output_tag_data)); \
  memcpy(blockchain_data.stealth_address_output_tag_data,"0000",sizeof(STEALTH_ADDRESS_OUTPUT_TAG)-1);

  #define INVALID_STEALTH_ADDRESS_OUTPUT_CODE blockchain_data.stealth_address_output_data_length = 1;

  #define INVALID_EXTRA_BYTES_SIZE_CODE blockchain_data.extra_bytes_size = 1;

  #define INVALID_TRANSACTION_PUBLIC_KEY_TAG_CODE \
  memset(blockchain_data.transaction_public_key_tag_data,0,strlen(blockchain_data.transaction_public_key_tag_data)); \
  memcpy(blockchain_data.transaction_public_key_tag_data,"00",sizeof(TRANSACTION_PUBLIC_KEY_TAG)-1);

  #define INVALID_EXTRA_NONCE_TAG_CODE \
  memset(blockchain_data.extra_nonce_tag_data,0,strlen(blockchain_data.extra_nonce_tag_data)); \
  memcpy(blockchain_data.extra_nonce_tag_data,"00",sizeof(EXTRA_NONCE_TAG)-1);

  #define INVALID_BLOCK_PRODUCER_DELEGATES_NAME_CODE blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data_length = 1;

  #define INVALID_BLOCK_PRODUCER_PUBLIC_ADDRESS_CODE blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data_length = 1;

  #define INVALID_BLOCK_PRODUCER_NODE_BACKUP_COUNT_CODE blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data_length = 1;

  #define INVALID_BLOCK_PRODUCER_BACKUP_NODES_NAMES_CODE blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data_length = 1;

  #define INVALID_VRF_SECRET_KEY_CODE blockchain_data.blockchain_reserve_bytes.vrf_secret_key_data_length = 1;

  #define INVALID_VRF_PUBLIC_KEY_CODE blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length = 1;

  #define INVALID_VRF_ALPHA_STRING_CODE blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length = 1;

  #define INVALID_VRF_PROOF_CODE blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length = 1;

  #define INVALID_VRF_BETA_STRING_CODE blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length = 1;

  #define INVALID_RINGCT_VERSION_CODE \
  memset(blockchain_data.ringct_version_data,0,strlen(blockchain_data.ringct_version_data)); \
  memcpy(blockchain_data.ringct_version_data,"01",sizeof(RINGCT_VERSION)-1);

  #define INVALID_TRANSACTION_AMOUNT_CODE blockchain_data.transaction_amount_data_length = 1;
  
  #define VERIFY_NETWORK_BLOCK_DATA_ERROR(CODE,TEST) \
  network_block_string_to_blockchain_data(NETWORK_BLOCK,NETWORK_BLOCK_HEIGHT,BLOCK_VERIFIERS_TOTAL_AMOUNT); \
  CODE; \
  INITIALIZE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_TEST; \
  if (verify_network_block_data(0,0,NETWORK_BLOCK_HEIGHT,BLOCK_VERIFIERS_TOTAL_AMOUNT) == 0 && strncmp(error_message.data[0],TEST,BUFFER_SIZE) == 0) \
  { \
    fprintf(stderr,"\033[1;32mPASSED! Test for verify_network_block_data checking for %s\033[0m\n",TEST); \
    count_test++; \
  } \
  else \
  { \
    fprintf(stderr,"\033[1;31mFAILED! Test for verify_network_block_data checking for %s\033[0m\n",TEST); \
  } \
  RESET_ERROR_MESSAGES;

  // Variables
  size_t count;

  // reset the variables
  memset(data_test,0,sizeof(data_test));
  count_test = 0;

  // write the start test message
  fprintf(stderr,"\033[1;34m%s\nblockchain functions test - Total test: %d\n%s\n\n\033[0m",TEST_OUTLINE,BLOCKCHAIN_TOTAL_TEST,TEST_OUTLINE);

  // run the varint_decode test
  if (varint_decode(VARINT_ENCODED_VALUE_RANDOM_VALUE) == VARINT_DECODED_VALUE_RANDOM_VALUE ||
      varint_decode(VARINT_ENCODED_VALUE_NO_ENCODING_OR_DECODING) == VARINT_DECODED_VALUE_NO_ENCODING_OR_DECODING ||
      varint_decode(VARINT_ENCODED_VALUE_START_1_BYTE) == VARINT_DECODED_VALUE_START_1_BYTE ||
      varint_decode(VARINT_ENCODED_VALUE_END_1_BYTE) == VARINT_DECODED_VALUE_END_1_BYTE ||
      varint_decode(VARINT_ENCODED_VALUE_START_2_BYTE) == VARINT_DECODED_VALUE_START_2_BYTE ||
      varint_decode(VARINT_ENCODED_VALUE_END_2_BYTE) == VARINT_DECODED_VALUE_END_2_BYTE||
      varint_decode(VARINT_ENCODED_VALUE_START_3_BYTE) == VARINT_DECODED_VALUE_START_3_BYTE ||
      varint_decode(VARINT_ENCODED_VALUE_END_3_BYTE) == VARINT_DECODED_VALUE_END_3_BYTE ||
      varint_decode(VARINT_ENCODED_VALUE_START_4_BYTE) == VARINT_DECODED_VALUE_START_4_BYTE ||
      varint_decode(VARINT_ENCODED_VALUE_END_4_BYTE) == VARINT_DECODED_VALUE_END_4_BYTE ||
      varint_decode(VARINT_ENCODED_VALUE_START_5_BYTE) == VARINT_DECODED_VALUE_START_5_BYTE ||
      varint_decode(VARINT_ENCODED_VALUE_END_5_BYTE) == VARINT_DECODED_VALUE_END_5_BYTE ||
      varint_decode(VARINT_ENCODED_VALUE_START_6_BYTE) == VARINT_DECODED_VALUE_START_6_BYTE ||
      varint_decode(VARINT_ENCODED_VALUE_END_6_BYTE) == VARINT_DECODED_VALUE_END_6_BYTE ||
      varint_decode(VARINT_ENCODED_VALUE_START_7_BYTE) == VARINT_DECODED_VALUE_START_7_BYTE ||
      varint_decode(VARINT_ENCODED_VALUE_END_7_BYTE) == VARINT_DECODED_VALUE_END_7_BYTE)
  {
    color_print("PASSED! Test for varint_decode","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for varint_decode","red");
  }

  // run the varint_encode test
  if ((varint_encode(VARINT_DECODED_VALUE_RANDOM_VALUE,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_RANDOM_VALUE,BUFFER_SIZE) == 0) ||
      (varint_encode(VARINT_DECODED_VALUE_NO_ENCODING_OR_DECODING,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_NO_ENCODING_OR_DECODING,BUFFER_SIZE) == 0) ||
      (varint_encode(VARINT_DECODED_VALUE_START_1_BYTE,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_START_1_BYTE,BUFFER_SIZE) == 0) ||
      (varint_encode(VARINT_DECODED_VALUE_END_1_BYTE,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_END_1_BYTE,BUFFER_SIZE) == 0) ||
      (varint_encode(VARINT_DECODED_VALUE_START_2_BYTE,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_START_2_BYTE,BUFFER_SIZE) == 0) ||
      (varint_encode(VARINT_DECODED_VALUE_END_2_BYTE,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_END_2_BYTE,BUFFER_SIZE) == 0) ||
      (varint_encode(VARINT_DECODED_VALUE_START_3_BYTE,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_START_3_BYTE,BUFFER_SIZE) == 0) ||
      (varint_encode(VARINT_DECODED_VALUE_END_3_BYTE,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_END_3_BYTE,BUFFER_SIZE) == 0) ||
      (varint_encode(VARINT_DECODED_VALUE_START_4_BYTE,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_START_4_BYTE,BUFFER_SIZE) == 0) ||
      (varint_encode(VARINT_DECODED_VALUE_END_4_BYTE,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_END_4_BYTE,BUFFER_SIZE) == 0) ||
      (varint_encode(VARINT_DECODED_VALUE_START_5_BYTE,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_START_5_BYTE,BUFFER_SIZE) == 0) ||
      (varint_encode(VARINT_DECODED_VALUE_END_5_BYTE,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_END_5_BYTE,BUFFER_SIZE) == 0) ||
      (varint_encode(VARINT_DECODED_VALUE_START_6_BYTE,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_START_6_BYTE,BUFFER_SIZE) == 0) ||
      (varint_encode(VARINT_DECODED_VALUE_END_6_BYTE,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_END_6_BYTE,BUFFER_SIZE) == 0) ||
      (varint_encode(VARINT_DECODED_VALUE_START_7_BYTE,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_START_7_BYTE,BUFFER_SIZE) == 0) ||
      (varint_encode(VARINT_DECODED_VALUE_END_7_BYTE,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_END_7_BYTE,BUFFER_SIZE) == 0))
  {
    color_print("PASSED! Test for varint_encode","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for varint_encode","red");
  }

  // get_generated_supply
  if (get_generated_supply(521850) - 77824360540.643250 <= 1.0)
  {
    color_print("PASSED! Test for get_generated_supply","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for get_generated_supply","red");
  }

  // run the network_block_string_to_blockchain_data test
  if (network_block_string_to_blockchain_data(NETWORK_BLOCK,NETWORK_BLOCK_HEIGHT,BLOCK_VERIFIERS_TOTAL_AMOUNT) == 1)
  {
    color_print("PASSED! Test for network_block_string_to_blockchain_data","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for network_block_string_to_blockchain_data","red");
  }

  // verify the blockchain_data
  INITIALIZE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_TEST;
  if (verify_network_block_data(0,0,NETWORK_BLOCK_HEIGHT,BLOCK_VERIFIERS_TOTAL_AMOUNT) == 1)
  {
    color_print("PASSED! Test for verify_network_block_data","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for verify_network_block_data","red");
  }

  RESET_BLOCKCHAIN_DATA;
  memset(data_test,0,strlen(data_test));
  CREATE_BLOCK_DATA;  

  if (blockchain_data_to_network_block_string(data_test,BLOCK_VERIFIERS_TOTAL_AMOUNT) == 1)
  {
    // verify the network_block_string
    if (strcmp(NETWORK_BLOCK,data_test) == 0)
    {
      color_print("PASSED! Test for blockchain_data_to_network_block_string","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for blockchain_data_to_network_block_string","red");
    }
  }
  else
  {
    color_print("FAILED! Test for blockchain_data_to_network_block_string","red");
  }



  // check all errors for each test
  VERIFY_NETWORK_BLOCK_DATA_ERROR(INVALID_NETWORK_VERSION_CODE,"Invalid network_version");
  VERIFY_NETWORK_BLOCK_DATA_ERROR(INVALID_TIMESTAMP_CODE,"Invalid timestamp");
  VERIFY_NETWORK_BLOCK_DATA_ERROR(INVALID_NETWORK_BLOCK_NONCE_CODE,"Invalid network block nonce");
  VERIFY_NETWORK_BLOCK_DATA_ERROR(INVALID_BLOCK_REWARD_TRANSACTION_VERSION_CODE,"Invalid block_reward_transaction_version");
  VERIFY_NETWORK_BLOCK_DATA_ERROR(INVALID_UNLOCK_BLOCK_CODE,"Invalid unlock_block");
  VERIFY_NETWORK_BLOCK_DATA_ERROR(INVALID_BLOCK_REWARD_INPUT_CODE,"Invalid block_reward_input");
  VERIFY_NETWORK_BLOCK_DATA_ERROR(INVALID_VIN_TYPE_CODE,"Invalid vin_type");
  VERIFY_NETWORK_BLOCK_DATA_ERROR(INVALID_BLOCK_HEIGHT_CODE,"Invalid block_height");
  VERIFY_NETWORK_BLOCK_DATA_ERROR(INVALID_BLOCK_REWARD_OUTPUT_CODE,"Invalid block_reward_output");
  VERIFY_NETWORK_BLOCK_DATA_ERROR(INVALID_BLOCK_REWARD_CODE,"Invalid block_reward");
  VERIFY_NETWORK_BLOCK_DATA_ERROR(INVALID_STEALTH_ADDRESS_OUTPUT_TAG_CODE,"Invalid stealth_address_output_tag");
  VERIFY_NETWORK_BLOCK_DATA_ERROR(INVALID_STEALTH_ADDRESS_OUTPUT_CODE,"Invalid stealth_address_output");
  VERIFY_NETWORK_BLOCK_DATA_ERROR(INVALID_EXTRA_BYTES_SIZE_CODE,"Invalid extra_bytes_size");
  VERIFY_NETWORK_BLOCK_DATA_ERROR(INVALID_TRANSACTION_PUBLIC_KEY_TAG_CODE,"Invalid transaction_public_key_tag");
  VERIFY_NETWORK_BLOCK_DATA_ERROR(INVALID_EXTRA_NONCE_TAG_CODE,"Invalid extra_nonce_tag");
  VERIFY_NETWORK_BLOCK_DATA_ERROR(INVALID_BLOCK_PRODUCER_DELEGATES_NAME_CODE,"Invalid block_producer_delegates_name");
  VERIFY_NETWORK_BLOCK_DATA_ERROR(INVALID_BLOCK_PRODUCER_PUBLIC_ADDRESS_CODE,"Invalid block_producer_public_address");
  VERIFY_NETWORK_BLOCK_DATA_ERROR(INVALID_BLOCK_PRODUCER_NODE_BACKUP_COUNT_CODE,"Invalid block_producer_node_backup_count");
  VERIFY_NETWORK_BLOCK_DATA_ERROR(INVALID_BLOCK_PRODUCER_BACKUP_NODES_NAMES_CODE,"Invalid block_producer_backup_nodes_names");
  VERIFY_NETWORK_BLOCK_DATA_ERROR(INVALID_VRF_SECRET_KEY_CODE,"Invalid vrf_secret_key");
  VERIFY_NETWORK_BLOCK_DATA_ERROR(INVALID_VRF_PUBLIC_KEY_CODE,"Invalid vrf_public_key");
  VERIFY_NETWORK_BLOCK_DATA_ERROR(INVALID_VRF_ALPHA_STRING_CODE,"Invalid vrf_alpha_string");
  VERIFY_NETWORK_BLOCK_DATA_ERROR(INVALID_VRF_PROOF_CODE,"Invalid vrf_proof");
  VERIFY_NETWORK_BLOCK_DATA_ERROR(INVALID_VRF_BETA_STRING_CODE,"Invalid vrf_beta_string");
  VERIFY_NETWORK_BLOCK_DATA_ERROR(INVALID_RINGCT_VERSION_CODE,"Invalid ringct_version");
  VERIFY_NETWORK_BLOCK_DATA_ERROR(INVALID_TRANSACTION_AMOUNT_CODE,"Invalid transaction_amount");

  // write the end test message
  fprintf(stderr,"\033[1;33m\n\n%s\nblockchain functions test - Passed test: %d, Failed test: %d\n%s\n\n\n\033[0m",TEST_OUTLINE,count_test,BLOCKCHAIN_TOTAL_TEST-count_test,TEST_OUTLINE);
  return count_test;
  
  #undef VERIFY_NETWORK_BLOCK_DATA_ERROR
  #undef INVALID_NETWORK_VERSION_CODE
  #undef INVALID_TIMESTAMP_CODE
  #undef INVALID_NETWORK_BLOCK_NONCE_CODE
  #undef INVALID_BLOCK_REWARD_TRANSACTION_VERSION_CODE
  #undef INVALID_UNLOCK_BLOCK_CODE
  #undef INVALID_BLOCK_REWARD_INPUT_CODE
  #undef INVALID_VIN_TYPE_CODE
  #undef INVALID_BLOCK_HEIGHT_CODE
  #undef INVALID_BLOCK_REWARD_OUTPUT_CODE
  #undef INVALID_BLOCK_REWARD_CODE
  #undef INVALID_STEALTH_ADDRESS_OUTPUT_TAG_CODE
  #undef INVALID_STEALTH_ADDRESS_OUTPUT_CODE
  #undef INVALID_EXTRA_BYTES_SIZE_CODE
  #undef INVALID_TRANSACTION_PUBLIC_KEY_TAG_CODE
  #undef INVALID_EXTRA_NONCE_TAG_CODE
  #undef INVALID_BLOCK_PRODUCER_DELEGATES_NAME_CODE
  #undef INVALID_BLOCK_PRODUCER_PUBLIC_ADDRESS_CODE
  #undef INVALID_BLOCK_PRODUCER_NODE_BACKUP_COUNT_CODE
  #undef INVALID_BLOCK_PRODUCER_BACKUP_NODES_NAMES_CODE
  #undef INVALID_VRF_SECRET_KEY_CODE
  #undef INVALID_VRF_PUBLIC_KEY_CODE
  #undef INVALID_VRF_ALPHA_STRING_CODE
  #undef INVALID_VRF_PROOF_CODE
  #undef INVALID_VRF_BETA_STRING_CODE
  #undef INVALID_RINGCT_VERSION_CODE
}