#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "define_macros_test.h"

#include "insert_database_functions.h"
#include "delete_database_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "string_functions.h"
#include "variables_test.h"
#include "shared_delegate_website_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: shared_delegate_website_functions_test
Description: Test the shared_delegate website functions
Return: The number of passed shared_delegate_website_functions test
-----------------------------------------------------------------------------------------------------------
*/

int shared_delegate_website_functions_test(void)
{
  // Variables
  char data[BUFFER_SIZE];

  // define macros
  #define SHARED_DELEGATE_WEBSITE_FUNCTIONS_TOTAL_TEST 4
  
  memset(database_name,0,sizeof(database_name));
  memcpy(database_name,DATABASE_NAME_TEST,sizeof(DATABASE_NAME_TEST)-1);
  memset(shared_delegates_database_name,0,sizeof(shared_delegates_database_name));
  memcpy(shared_delegates_database_name,DATABASE_NAME_TEST,sizeof(DATABASE_NAME_TEST)-1);
  shared_delegates_website = 1;
  insert_document_into_collection_json(database_name,"reserve_bytes_1",SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATABASE_HASH_TEST_DATA,0);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",RESERVE_PROOFS_TEST_DATA,0);
  insert_document_into_collection_json(database_name,"delegates",DELEGATES_TEST_DATA,0);
  insert_document_into_collection_json(database_name,"statistics",DATABASE_COLLECTION_STATISTICS_DATA,0);
  insert_document_into_collection_json(database_name,"blocks_found","{\"block_height\":\"440875\",\"block_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\",\"block_date_and_time\":\"10\",\"block_reward\":\"15\",\"block_count\":\"10\"}",0);
  insert_document_into_collection_json(database_name,"public_addresses","{\"public_address\":\"XCA1v18Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3\",\"current_total\":\"5\",\"total\":\"10\",\"inactivity_count\":\"15\"}",0);
  insert_document_into_collection_json(database_name,"public_addresses_payments","{\"public_address\":\"XCA1v18Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3\",\"date_and_time\":\"5\",\"total\":\"10\",\"tx_hash\":\"TX_HASH\",\"tx_key\":\"TX_KEY\"}",0);
  memset(result_test,0,sizeof(result_test));
  memcpy(result_test,"{\"public_address\":\"",19);
  memcpy(result_test+strlen(result_test),xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(result_test+strlen(result_test),"\",\"total_vote_count\":\"0\",\"IP_address\":\"127.0.0.1\",\"delegate_name\":\"delegate_name_1\",\"about\":\"about\",\"website\":\"website\",\"team\":\"team\",\"pool_mode\":\"false\",\"fee_structure\":\"0\",\"server_settings\":\"server settings\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"440875\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}",528);
  insert_document_into_collection_json(database_name,"delegates",result_test,0);

  // reset the variables
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memset(data,0,sizeof(data));
  count_test = 0;
  network_functions_test_settings = 0;
 
  // write the start test message
  fprintf(stderr,"\033[1;34m%s\nshared_delegate website functions test - Total test: %d\n%s\n\n\033[0m",TEST_OUTLINE,SHARED_DELEGATE_WEBSITE_FUNCTIONS_TOTAL_TEST,TEST_OUTLINE);
 
  // run the test
  CHECK_WEBSITE_SERVER_FUNCTIONS("server_receive_data_socket_shared_delegates_website_get_statistics",SERVER_RECEIVE_DATA_SOCKET_SHARED_DELEGATES_WEBSITE_GET_STATISTICS_TEST_DATA);
  CHECK_WEBSITE_SERVER_FUNCTIONS("server_receive_data_socket_get_blocks_found",SERVER_RECEIVE_DATA_SOCKET_GET_BLOCKS_FOUND_TEST_DATA);
  CHECK_WEBSITE_SERVER_FUNCTIONS("server_receive_data_socket_get_public_address_information",SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_INFORMATION_TEST_DATA);
  CHECK_WEBSITE_SERVER_FUNCTIONS("server_receive_data_socket_get_public_address_payment_information",SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_TEST_DATA);
  delete_database(DATABASE_NAME_TEST,0);

  // write the end test message
  if (count_test == SHARED_DELEGATE_WEBSITE_FUNCTIONS_TOTAL_TEST)
  {
    fprintf(stderr,"\033[1;32m\n\n%s\nshared_delegate website functions test - Passed test: %d, Failed test: 0\n%s\n\n\n\033[0m",TEST_OUTLINE,SHARED_DELEGATE_WEBSITE_FUNCTIONS_TOTAL_TEST,TEST_OUTLINE);
  }
  else
  {
    fprintf(stderr,"\033[1;31m\n\n%s\nshared_delegate website functions test - Passed test: %d, Failed test: 0\n%s\n\n\n\033[0m",TEST_OUTLINE,SHARED_DELEGATE_WEBSITE_FUNCTIONS_TOTAL_TEST-count_test,TEST_OUTLINE);
  }
  return count_test;

  #undef SHARED_DELEGATE_WEBSITE_FUNCTIONS_TOTAL_TEST
}