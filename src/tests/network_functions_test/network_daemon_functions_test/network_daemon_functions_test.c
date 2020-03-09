#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "variables.h"
#include "define_macros_test.h"

#include "network_daemon_functions.h"
#include "network_functions.h"
#include "server_functions.h"
#include "string_functions.h"
#include "insert_database_functions.h"
#include "delete_database_functions.h"
#include "VRF_functions.h"

#include "variables_test.h"
#include "network_daemon_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: network_daemon_functions_test
Description: Test the network_daemon functions
Return: The number of passed network_daemon functions test
-----------------------------------------------------------------------------------------------------------
*/

int network_daemon_functions_test(void)
{  
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  size_t count = 0;
  int count2;
  size_t block_height;

  // reset the variables
  memset(data_test,0,sizeof(data_test));
  count_test = 0;

  // write the start test message
  fprintf(stderr,"\033[1;34m%s\nnetwork daemon functions test - Total test: %d\n%s\n\n\033[0m",TEST_OUTLINE,NETWORK_DAEMON_FUNCTIONS_TEST,TEST_OUTLINE);

  // test the check_if_blockchain_is_fully_synced
  if (check_if_blockchain_is_fully_synced() == 1)
  {   
    color_print("PASSED! Test for check_if_blockchain_is_fully_synced","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for check_if_blockchain_is_fully_synced","red");
  }

  // test the get_block_template function
  if (get_block_template(data_test) == 1)
  {   
    color_print("PASSED! Test for get_block_template","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for get_block_template","red");
  }

  // test the submit_block_template function
  if (submit_block_template("XCASH_DPOPS") == 1)
  {   
    color_print("PASSED! Test for submit_block_template","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for submit_block_template","red");
  }

  // test the get_current_block_height
  memset(data_test,0,sizeof(data_test));
  if (get_current_block_height(data_test) == 1)
  {   
    color_print("PASSED! Test for get_current_block_height","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for get_current_block_height","red");
  }

  // test get_block_reserve_byte_data_hash
  memset(result_test,0,sizeof(result_test));
  sscanf(data_test,"%zu",&count);
  count--;
  memset(data_test,0,sizeof(data_test));
  snprintf(data_test,sizeof(data_test)-1,"%zu",count);
  network_functions_test_error_settings = 0;
  get_block_reserve_byte_data_hash(result_test,data_test);
  network_functions_test_error_settings = 1;
  color_print("PASSED! Test for get_block_reserve_byte_data_hash","green");
  count_test++;

  // test the get_previous_block_hash function
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (get_previous_block_hash(data_test) == 1)
  {   
    color_print("PASSED! Test for get_previous_block_hash","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for get_previous_block_hash","red");
  }

  // test the get_previous_block_information function
  memset(data_test,0,sizeof(data_test));
  memset(result_test,0,sizeof(result_test));
  memset(data,0,sizeof(data));
  if (get_previous_block_information(result_test,data_test,data) == 1)
  {   
    color_print("PASSED! Test for get_previous_block_information","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for get_previous_block_information","red");
  }

  // check_found_block
  delete_database(database_name,0);
  sscanf(current_block_height, "%zu", &block_height);
  block_height--;
  memset(data_test,0,sizeof(data_test));
  snprintf(data_test,sizeof(data_test)-1,"%zu",block_height);

  // convert the xcash wallet address to a hexadecimal string and insert this into the reserve bytes data
  memset(data2,0,sizeof(data2));
  for (count = 0, count2 = 0; count < XCASH_WALLET_LENGTH; count++, count2 += 2)
  {
    snprintf(data2+count2,(XCASH_WALLET_LENGTH*2)-1,"%02x",xcash_wallet_public_address[count] & 0xFF);
  }

  // get the reserve_bytes database for the block
  count = ((block_height - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME) + 1;
  memset(result_test,0,sizeof(result_test));
  memcpy(result_test,"reserve_bytes_",14);
  snprintf(result_test+14,sizeof(result_test)-15,"%zu",count);

  // create the message
  memcpy(data,"{\"block_height\":\"",17);
  memcpy(data+strlen(data),data_test,strnlen(data_test,sizeof(data)));
  memcpy(data+strlen(data),"\",\"reserve_bytes_data_hash\":\"data\",\"reserve_bytes\":\"",52);
  memcpy(data+strlen(data),data2,XCASH_WALLET_LENGTH*2);
  memcpy(data+strlen(data),"7c",2);
  memcpy(data+strlen(data),data2,XCASH_WALLET_LENGTH*2);
  memcpy(data+strlen(data),"\"}",2);
  insert_document_into_collection_json(database_name,result_test,data,0);

  if (check_found_block() == 2)
  {   
    color_print("PASSED! Test for check_found_block","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for check_found_block","red");
  }



  // check all errors for each test
  // check_found_block
  delete_database(database_name,0);
  sscanf(current_block_height, "%zu", &block_height);
  block_height--;
  memset(data_test,0,sizeof(data_test));
  snprintf(data_test,sizeof(data_test)-1,"%zu",block_height);

  // convert the xcash wallet address to a hexadecimal string and insert this into the reserve bytes data
  memset(data2,0,sizeof(data2));
  for (count = 0, count2 = 0; count < XCASH_WALLET_LENGTH; count++, count2 += 2)
  {
    snprintf(data2+count2,(XCASH_WALLET_LENGTH*2)-1,"%02x",xcash_wallet_public_address[count] & 0xFF);
  }

  // get the reserve_bytes database for the block
  count = ((block_height - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME) + 1;
  memset(result_test,0,sizeof(result_test));
  memcpy(result_test,"reserve_bytes_",14);
  snprintf(result_test+14,sizeof(result_test)-15,"%zu",count);

  // create the message
  memset(data,0,sizeof(data));
  memcpy(data,"{\"block_height\":\"",17);
  memcpy(data+strlen(data),data_test,strnlen(data_test,sizeof(data)));
  memcpy(data+strlen(data),"\",\"reserve_bytes_data_hash\":\"data\",\"reserve_bytes\":\"data\"}",58);
  insert_document_into_collection_json(database_name,result_test,data,0);

  if (check_found_block() == 1)
  {   
    color_print("PASSED! Test for check_found_block checking for that the block verifier did not find the previous block","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for check_found_block checking for that the block verifier did not find the previous block","red");
  }

  delete_database(database_name,0);
  if (check_found_block() == 0)
  {   
    color_print("PASSED! Test for check_found_block checking for cant get the previous blocks reserve bytes","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for check_found_block checking for cant get the previous blocks reserve bytes","red");
  }

  
  // write the end test message
  fprintf(stderr,"\033[1;33m\n\n%s\nnetwork daemon functions test - Passed test: %d, Failed test: %d\n%s\n\n\n\033[0m",TEST_OUTLINE,count_test,NETWORK_DAEMON_FUNCTIONS_TEST-count_test,TEST_OUTLINE);
  return count_test;
}