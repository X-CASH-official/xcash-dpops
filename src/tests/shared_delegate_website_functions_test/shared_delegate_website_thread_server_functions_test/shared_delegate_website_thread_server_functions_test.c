#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "define_macros_test.h"

#include "shared_delegate_website_thread_server_functions.h"
#include "insert_database_functions.h"
#include "delete_database_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "string_functions.h"
#include "variables_test.h"
#include "shared_delegate_website_thread_server_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: shared_delegate_website_thread_server_functions_test
Description: Test the shared_delegate website functions
Return: The number of passed shared_delegate_website_thread_server_functions test
-----------------------------------------------------------------------------------------------------------
*/

int shared_delegate_website_thread_server_functions_test(void)
{
  // Variables
  char data[BUFFER_SIZE];
  long long int block_reward;

  // define macros
  #define SHARED_DELEGATE_WEBSITE_THREAD_SERVER_FUNCTIONS_TOTAL_TEST 2
  
  insert_document_into_collection_json(database_name,"reserve_bytes_1",SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATABASE_HASH_TEST_DATA,0);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",RESERVE_PROOFS_TEST_DATA,0);
  insert_document_into_collection_json(database_name,"delegates",DELEGATES_TEST_DATA,0);
  insert_document_into_collection_json(database_name,"statistics",DATABASE_COLLECTION_STATISTICS_DATA,0);
  insert_document_into_collection_json(database_name,"blocks_found","{\"block_height\":\"440875\",\"block_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\",\"block_date_and_time\":\"10\",\"block_reward\":\"15\",\"block_count\":\"10\"}",0);
  insert_document_into_collection_json(database_name,"public_addresses","{\"public_address\":\"XCA1v18Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3\",\"current_total\":\"5\",\"total\":\"10\",\"inactivity_count\":\"15\"}",0);
  insert_document_into_collection_json(database_name,"public_addresses_payments","{\"public_address\":\"XCA1v18Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3\",\"date_and_time\":\"5\",\"total\":\"10\",\"tx_hash\":\"TX_HASH\",\"tx_key\":\"TX_KEY\"}",0);
  memset(result_test,0,sizeof(result_test));
  memcpy(result_test,"{\"public_address_created_reserve_proof\":\"XCA1v18Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3\",\"public_address_voted_for\":\"",169);
  memcpy(result_test+strlen(result_test),xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(result_test+strlen(result_test),"\",\"total\":\"0\",\"reserve_proof\":\"DATA\"}",37);
  insert_document_into_collection_json(DATABASE_NAME_TEST,"reserve_proofs_1",result_test,0);

  // reset the variables
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memset(data,0,sizeof(data));
  count_test = 0;
 
  // write the start test message
  fprintf(stderr,"\033[1;34m%s\nshared_delegate website thread server functions test - Total test: %d\n%s\n\n\033[0m",TEST_OUTLINE,SHARED_DELEGATE_WEBSITE_THREAD_SERVER_FUNCTIONS_TOTAL_TEST,TEST_OUTLINE);
 
  // run the test

  // test add_block_to_blocks_found
  if ((block_reward = add_block_to_blocks_found()) != 0)
  {
    color_print("PASSED! Test for add_block_to_blocks_found","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for add_block_to_blocks_found","red");
  } 
  
  // test calculate_block_reward_for_each_delegate
  if (calculate_block_reward_for_each_delegate(block_reward) == 1)
  {
    color_print("PASSED! Test for calculate_block_reward_for_each_delegate","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for calculate_block_reward_for_each_delegate","red");
  }  
  delete_database(DATABASE_NAME_TEST,0);

  // write the end test message
  fprintf(stderr,"\033[1;33m\n\n%s\nshared_delegate website thread server functions test - Passed test: %d, Failed test: %d\n%s\n\n\n\033[0m",TEST_OUTLINE,count_test,SHARED_DELEGATE_WEBSITE_THREAD_SERVER_FUNCTIONS_TOTAL_TEST-count_test,TEST_OUTLINE);
  return count_test;

  #undef SHARED_DELEGATE_WEBSITE_THREAD_SERVER_FUNCTIONS_TOTAL_TEST
}