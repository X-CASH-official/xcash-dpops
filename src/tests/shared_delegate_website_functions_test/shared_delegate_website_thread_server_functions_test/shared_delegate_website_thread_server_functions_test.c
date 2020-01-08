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

  // define macros
  #define SHARED_DELEGATE_WEBSITE_THREAD_SERVER_FUNCTIONS_TOTAL_TEST 1
  
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
  
  // test calculate_block_reward_for_each_delegate
  if (calculate_block_reward_for_each_delegate(100000000000) == 1)
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
  if (count_test == SHARED_DELEGATE_WEBSITE_THREAD_SERVER_FUNCTIONS_TOTAL_TEST)
  {
    fprintf(stderr,"\033[1;32m\n\n%s\nshared_delegate website thread server functions test - Passed test: %d, Failed test: 0\n%s\n\n\n\033[0m",TEST_OUTLINE,SHARED_DELEGATE_WEBSITE_THREAD_SERVER_FUNCTIONS_TOTAL_TEST,TEST_OUTLINE);
  }
  else
  {
    fprintf(stderr,"\033[1;31m\n\n%s\nshared_delegate website thread server functions test - Passed test: %d, Failed test: 0\n%s\n\n\n\033[0m",TEST_OUTLINE,SHARED_DELEGATE_WEBSITE_THREAD_SERVER_FUNCTIONS_TOTAL_TEST-count_test,TEST_OUTLINE);
  }
  return count_test;

  #undef SHARED_DELEGATE_WEBSITE_THREAD_SERVER_FUNCTIONS_TOTAL_TEST
}