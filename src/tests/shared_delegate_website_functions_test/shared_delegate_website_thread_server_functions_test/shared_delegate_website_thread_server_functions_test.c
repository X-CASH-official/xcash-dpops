#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "define_macros_test.h"
#include "initialize_and_reset_structs_define_macros.h"

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
  struct voters voters[MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count = 0;

  // define macros
  #define SHARED_DELEGATE_WEBSITE_THREAD_SERVER_FUNCTIONS_TOTAL_TEST 2

  // initialize the voters struct
  INITIALIZE_VOTERS_STRUCT(count,MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE,"shared_delegate_website_thread_server_functions_test",data,current_date_and_time,current_UTC_date_and_time);

  memset(database_name,0,sizeof(database_name));
  memcpy(database_name,DATABASE_NAME_TEST,sizeof(DATABASE_NAME_TEST)-1);
  memset(shared_delegates_database_name,0,sizeof(shared_delegates_database_name));
  memcpy(shared_delegates_database_name,DATABASE_NAME_TEST,sizeof(DATABASE_NAME_TEST)-1);
  delete_database(database_name,0);  
  insert_document_into_collection_json(database_name,"reserve_bytes_1","{\"message_settings\": \"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATABASE_HASH\",\"block_height\": \"" XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT_TEST "\"}",0);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",RESERVE_PROOFS_TEST_DATA,0);
  insert_document_into_collection_json(database_name,"delegates",DELEGATES_TEST_DATA,0);
  insert_document_into_collection_json(database_name,"statistics",DATABASE_COLLECTION_STATISTICS_DATA,0);
  insert_document_into_collection_json(database_name,"blocks_found","{\"block_height\":\"" XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT_TEST "\",\"block_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\",\"block_date_and_time\":\"10\",\"block_reward\":\"15\",\"block_count\":\"10\"}",0);
  insert_document_into_collection_json(database_name,"public_addresses","{\"public_address\":\"" TEST_WALLET "\",\"current_total\":\"5\",\"total\":\"10\",\"inactivity_count\":\"15\"}",0);
  insert_document_into_collection_json(database_name,"public_addresses_payments","{\"public_address\":\"" TEST_WALLET "\",\"date_and_time\":\"5\",\"total\":\"10\",\"tx_hash\":\"TX_HASH\",\"tx_key\":\"TX_KEY\"}",0);
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
  network_functions_test_error_settings = 2;
 
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
  RESET_ERROR_MESSAGES;
  
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
  RESET_ERROR_MESSAGES;

  // test calculate_block_reward_for_each_delegate
  if (get_delegates_total_voters(voters) != -1)
  {
    color_print("PASSED! Test for get_delegates_total_voters","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for get_delegates_total_voters","red");
  }
  RESET_ERROR_MESSAGES;



  // check all errors for each test
  delete_database(database_name,0);
  POINTER_RESET_VOTERS_STRUCT(count,MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE);
  RESET_ERROR_MESSAGES;
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));

  if (add_block_to_blocks_found() == 0)
  {
    for (count = 0; count < error_message.total; count++)
    {
      if (strncmp(error_message.data[count],"The database is empty",BUFFER_SIZE) == 0)
      {
        break;
      }
    }
  }  
  if (count != error_message.total)
  {
    color_print("PASSED! Test for add_block_to_blocks_found checking for The database is empty","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for add_block_to_blocks_found checking for The database is empty","red");
  }
  RESET_ERROR_MESSAGES;

  if (calculate_block_reward_for_each_delegate(100) == 0)
  {
    for (count = 0; count < error_message.total; count++)
    {
      if (strncmp(error_message.data[count],"The delegate does not have enough voters to distribute the block rewards with",BUFFER_SIZE) == 0)
      {
        break;
      }
    }
  }  
  if (count != error_message.total)
  {
    color_print("PASSED! Test for calculate_block_reward_for_each_delegate checking for The delegate does not have enough voters to distribute the block rewards with","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for calculate_block_reward_for_each_delegate checking for The delegate does not have enough voters to distribute the block rewards with","red");
  }
  RESET_ERROR_MESSAGES;

  // write the end test message
  fprintf(stderr,"\033[1;33m\n\n%s\nshared_delegate website thread server functions test - Passed test: %d, Failed test: %d\n%s\n\n\n\033[0m",TEST_OUTLINE,count_test,SHARED_DELEGATE_WEBSITE_THREAD_SERVER_FUNCTIONS_TOTAL_TEST-count_test,TEST_OUTLINE);
  return count_test;

  #undef SHARED_DELEGATE_WEBSITE_THREAD_SERVER_FUNCTIONS_TOTAL_TEST
}