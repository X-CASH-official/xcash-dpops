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
  // Variables
  size_t count;

  // reset the variables
  memset(data_test,0,sizeof(data_test));
  count_test = 0;

  // write the start test message
  color_print(TEST_OUTLINE,"blue");
  fprintf(stderr,"\033[1;34mblockchain functions test - Total test: %d\033[0m\n",BLOCKCHAIN_TEST);
  color_print(TEST_OUTLINE,"blue");
  fprintf(stderr,"\n");

  // run the varint_decode test
  if (varint_decode(VARINT_ENCODED_VALUE_1) == VARINT_DECODED_VALUE_1 ||
      varint_decode(VARINT_ENCODED_VALUE_2) == VARINT_DECODED_VALUE_2 ||
      varint_decode(VARINT_ENCODED_VALUE_3) == VARINT_DECODED_VALUE_3 ||
      varint_decode(VARINT_ENCODED_VALUE_4) == VARINT_DECODED_VALUE_4 ||
      varint_decode(VARINT_ENCODED_VALUE_5) == VARINT_DECODED_VALUE_5 ||
      varint_decode(VARINT_ENCODED_VALUE_6) == VARINT_DECODED_VALUE_6 ||
      varint_decode(VARINT_ENCODED_VALUE_7) == VARINT_DECODED_VALUE_7 ||
      varint_decode(VARINT_ENCODED_VALUE_8) == VARINT_DECODED_VALUE_8 ||
      varint_decode(VARINT_ENCODED_VALUE_9) == VARINT_DECODED_VALUE_9 ||
      varint_decode(VARINT_ENCODED_VALUE_10) == VARINT_DECODED_VALUE_10 ||
      varint_decode(VARINT_ENCODED_VALUE_11) == VARINT_DECODED_VALUE_11 ||
      varint_decode(VARINT_ENCODED_VALUE_12) == VARINT_DECODED_VALUE_12 ||
      varint_decode(VARINT_ENCODED_VALUE_13) == VARINT_DECODED_VALUE_13 ||
      varint_decode(VARINT_ENCODED_VALUE_14) == VARINT_DECODED_VALUE_14 ||
      varint_decode(VARINT_ENCODED_VALUE_15) == VARINT_DECODED_VALUE_15 ||
      varint_decode(VARINT_ENCODED_VALUE_16) == VARINT_DECODED_VALUE_16)
  {
    color_print("PASSED! Test for varint_decode","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for varint_decode","red");
  }

  // run the varint_encode test
  if ((varint_encode(VARINT_DECODED_VALUE_1,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_1,BUFFER_SIZE) == 0) ||
      (varint_encode(VARINT_DECODED_VALUE_2,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_2,BUFFER_SIZE) == 0) ||
      (varint_encode(VARINT_DECODED_VALUE_3,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_3,BUFFER_SIZE) == 0) ||
      (varint_encode(VARINT_DECODED_VALUE_4,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_4,BUFFER_SIZE) == 0) ||
      (varint_encode(VARINT_DECODED_VALUE_5,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_5,BUFFER_SIZE) == 0) ||
      (varint_encode(VARINT_DECODED_VALUE_6,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_6,BUFFER_SIZE) == 0) ||
      (varint_encode(VARINT_DECODED_VALUE_7,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_7,BUFFER_SIZE) == 0) ||
      (varint_encode(VARINT_DECODED_VALUE_8,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_8,BUFFER_SIZE) == 0) ||
      (varint_encode(VARINT_DECODED_VALUE_9,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_9,BUFFER_SIZE) == 0) ||
      (varint_encode(VARINT_DECODED_VALUE_10,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_10,BUFFER_SIZE) == 0) ||
      (varint_encode(VARINT_DECODED_VALUE_11,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_11,BUFFER_SIZE) == 0) ||
      (varint_encode(VARINT_DECODED_VALUE_12,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_12,BUFFER_SIZE) == 0) ||
      (varint_encode(VARINT_DECODED_VALUE_13,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_13,BUFFER_SIZE) == 0) ||
      (varint_encode(VARINT_DECODED_VALUE_14,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_14,BUFFER_SIZE) == 0) ||
      (varint_encode(VARINT_DECODED_VALUE_15,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_15,BUFFER_SIZE) == 0) ||
      (varint_encode(VARINT_DECODED_VALUE_16,data_test,BUFFER_SIZE) == 1 && strncmp(data_test,VARINT_ENCODED_VALUE_TEXT_16,BUFFER_SIZE) == 0))
  {
    color_print("PASSED! Test for varint_encode","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for varint_encode","red");
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
  if (verify_network_block_data(0,0,0,NETWORK_BLOCK_HEIGHT,NETWORK_BLOCK,BLOCK_VERIFIERS_TOTAL_AMOUNT) == 1)
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

  // write the end test message
  if (count_test == BLOCKCHAIN_TEST)
  {
    fprintf(stderr,"\n");
    color_print(TEST_OUTLINE,"green");
    fprintf(stderr,"\033[1;32mblockchain functions test - Passed test: %d, Failed test: 0\033[0m\n",BLOCKCHAIN_TEST);
    color_print(TEST_OUTLINE,"green");
    fprintf(stderr,"\n\n");
  }
  else
  {
    fprintf(stderr,"\n");
    color_print(TEST_OUTLINE,"red");
    fprintf(stderr,"\033[1;31mblockchain functions test - Passed test: %d, Failed test: %d\033[0m\n",count_test,BLOCKCHAIN_TEST-count_test);
    color_print(TEST_OUTLINE,"red");
    fprintf(stderr,"\n\n");
  } 
  return count_test;
}