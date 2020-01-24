#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "variables.h"
#include "define_macros_test.h"

#include "network_security_functions.h"
#include "network_functions.h"
#include "server_functions.h"
#include "string_functions.h"
#include "VRF_functions.h"

#include "variables_test.h"
#include "network_security_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: network_security_functions_test
Description: Test the network_security functions
Return: The number of passed network_security functions test
-----------------------------------------------------------------------------------------------------------
*/

int network_security_functions_test(void)
{  
  // define macros
  #define NETWORK_SECURITY_FUNCTIONS_TEST 44
  #define MESSAGE "{\r\n \"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\",\r\n}"
  #define DATA "{\r\n \"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA_2\",\r\n}"
  
  // reset the variables
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));

  // reset the variables
  count_test = 0;

  // write the start test message
  fprintf(stderr,"\033[1;34m%s\nnetwork security functions test - Total test: %d\n%s\n\n\033[0m",TEST_OUTLINE,NETWORK_SECURITY_FUNCTIONS_TEST,TEST_OUTLINE);

  pthread_rwlock_wrlock(&rwlock);
  // set the current_round_part and current_round_part_backup_node
  memset(current_round_part,0,strnlen(current_round_part,BUFFER_SIZE));
  memset(current_round_part_backup_node,0,strnlen(current_round_part_backup_node,BUFFER_SIZE));
  memcpy(current_round_part,"1",1);
  memcpy(current_round_part_backup_node,"0",1);
  pthread_rwlock_unlock(&rwlock);
  
  // create the message
  memset(result_test,0,sizeof(result_test));
  memcpy(result_test,MESSAGE,sizeof(MESSAGE)-1);

  // test the sign_data function
  if (sign_data(result_test,0) == 1)
  {   
    color_print("PASSED! Test for sign_data using the wallet signing key","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sign_data using the wallet signing key","red");
  }

  // test the verify_data function
  if (verify_data(result_test,0,1) == 1)
  {   
    color_print("PASSED! Test for verify_data using the wallet signing key","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for verify_data using the wallet signing key","red");
  }

  // create the message
  memset(result_test,0,sizeof(result_test));
  memcpy(result_test,DATA,sizeof(DATA)-1);

  // test the sign_data function
  if (sign_data(result_test,0) == 1)
  {   
    color_print("PASSED! Test for sign_data using the ECDSA block verifier signing key","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sign_data using the ECDSA block verifier signing key","red");
  }

  // test the verify_data function
  if (verify_data(result_test,0,1) == 1)
  {   
    color_print("PASSED! Test for verify_data using the ECDSA block verifier signing key","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for verify_data using the ECDSA block verifier signing key","red");
  }

  // validate every message to make sure that all messages are properly formated
  if (validate_data(MAIN_NODES_TO_NODES_PART_4_OF_ROUND_CREATE_NEW_BLOCK) == 1)
  {   
    color_print("PASSED! Test for validate_data for MAIN_NODES_TO_NODES_PART_4_OF_ROUND_CREATE_NEW_BLOCK","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for MAIN_NODES_TO_NODES_PART_4_OF_ROUND_CREATE_NEW_BLOCK","red");
  }
  if (validate_data(MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_START_BLOCK) == 1)
  {   
    color_print("PASSED! Test for validate_data for MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_START_BLOCK","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_START_BLOCK","red");
  }
  if (validate_data(MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_CREATE_NEW_BLOCK) == 1)
  {   
    color_print("PASSED! Test for validate_data for MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_CREATE_NEW_BLOCK","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_CREATE_NEW_BLOCK","red");
  }
  if (validate_data(BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK) == 1)
  {   
    color_print("PASSED! Test for validate_data for BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK","red");
  }
  if (validate_data(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA) == 1)
  {   
    color_print("PASSED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA","red");
  }
  if (validate_data(NODES_TO_NODES_VOTE_RESULTS) == 1)
  {   
    color_print("PASSED! Test for validate_data for NODES_TO_NODES_VOTE_RESULTS","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for NODES_TO_NODES_VOTE_RESULTS","red");
  }
  if (validate_data(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE) == 1)
  {   
    color_print("PASSED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE","red");
  }
  if (validate_data(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS) == 1)
  {   
    color_print("PASSED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS","red");
  }
  if (validate_data(NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST) == 1)
  {   
    color_print("PASSED! Test for validate_data for NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST","red");
  }
  if (validate_data(NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST) == 1)
  {   
    color_print("PASSED! Test for validate_data for NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST","red");
  }
  if (validate_data(NETWORK_DATA_NODE_TO_NODE_SEND_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST) == 1)
  {   
    color_print("PASSED! Test for validate_data for NETWORK_DATA_NODE_TO_NODE_SEND_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for NETWORK_DATA_NODE_TO_NODE_SEND_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST","red");
  }
  if (validate_data(NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST) == 1)
  {   
    color_print("PASSED! Test for validate_data for NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST","red");
  }
  if (validate_data(BLOCK_VERIFIERS_TO_NETWORK_DATA_NODE_BLOCK_VERIFIERS_CURRENT_TIME) == 1)
  {   
    color_print("PASSED! Test for validate_data for BLOCK_VERIFIERS_TO_NETWORK_DATA_NODE_BLOCK_VERIFIERS_CURRENT_TIME","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for BLOCK_VERIFIERS_TO_NETWORK_DATA_NODE_BLOCK_VERIFIERS_CURRENT_TIME","red");
  }
  if (validate_data(NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_BLOCK_VERIFIERS_CURRENT_TIME) == 1)
  {   
    color_print("PASSED! Test for validate_data for NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_BLOCK_VERIFIERS_CURRENT_TIME","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_BLOCK_VERIFIERS_CURRENT_TIME","red");
  }
  if (validate_data(NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE) == 1)
  {   
    color_print("PASSED! Test for validate_data for NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE","red");
  }
  if (validate_data(NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE) == 1)
  {   
    color_print("PASSED! Test for validate_data for NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE","red");
  }
  if (validate_data(NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE) == 1)
  {   
    color_print("PASSED! Test for validate_data for NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE","red");
  }
  if (validate_data(NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF) == 1)
  {   
    color_print("PASSED! Test for validate_data for NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF","red");
  }
  if (validate_data(NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATABASE_HASH) == 1)
  {   
    color_print("PASSED! Test for validate_data for NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATABASE_HASH","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATABASE_HASH","red");
  }
  if (validate_data(NODE_TO_BLOCK_VERIFIERS_CHECK_IF_CURRENT_BLOCK_VERIFIER) == 1)
  {   
    color_print("PASSED! Test for validate_data for NODE_TO_BLOCK_VERIFIERS_CHECK_IF_CURRENT_BLOCK_VERIFIER","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for NODE_TO_BLOCK_VERIFIERS_CHECK_IF_CURRENT_BLOCK_VERIFIER","red");
  }
  if (validate_data(BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES) == 1)
  {   
    color_print("PASSED! Test for validate_data for BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES","red");
  }
  if (validate_data(NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE) == 1)
  {   
    color_print("PASSED! Test for validate_data for NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE","red");
  }
  if (validate_data(BLOCK_VERIFIERS_TO_NODES_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD) == 1)
  {   
    color_print("PASSED! Test for validate_data for BLOCK_VERIFIERS_TO_NODES_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for BLOCK_VERIFIERS_TO_NODES_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD","red");
  }
  if (validate_data(NETWORK_DATA_NODES_TO_NETWORK_DATA_NODES_DATABASE_SYNC_CHECK) == 1)
  {   
    color_print("PASSED! Test for validate_data for NETWORK_DATA_NODES_TO_NETWORK_DATA_NODES_DATABASE_SYNC_CHECK","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for NETWORK_DATA_NODES_TO_NETWORK_DATA_NODES_DATABASE_SYNC_CHECK","red");
  }
  if (validate_data(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD) == 1)
  {   
    color_print("PASSED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD","red");
  }
  if (validate_data(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD) == 1)
  {   
    color_print("PASSED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD","red");
  }
  if (validate_data(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE) == 1)
  {   
    color_print("PASSED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE","red");
  }
  if (validate_data(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD) == 1)
  {   
    color_print("PASSED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD","red");
  }
  if (validate_data(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE) == 1)
  {   
    color_print("PASSED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE","red");
  }
  if (validate_data(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD) == 1)
  {   
    color_print("PASSED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD","red");
  }
  if (validate_data(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE) == 1)
  {   
    color_print("PASSED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE","red");
  }
  if (validate_data(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD) == 1)
  {   
    color_print("PASSED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD","red");
  }
  if (validate_data(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE) == 1)
  {   
    color_print("PASSED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE","red");
  }
  if (validate_data(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD) == 1)
  {   
    color_print("PASSED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD","red");
  }
  if (validate_data(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE) == 1)
  {   
    color_print("PASSED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE","red");
  }
  if (validate_data(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD) == 1)
  {   
    color_print("PASSED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD","red");
  }
  if (validate_data(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE) == 1)
  {   
    color_print("PASSED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE","red");
  }
  if (validate_data(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD) == 1)
  {   
    color_print("PASSED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD","red");
  }
  if (validate_data(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE) == 1)
  {   
    color_print("PASSED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE","red");
  }
  if (validate_data(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD) == 1)
  {   
    color_print("PASSED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for validate_data for BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD","red");
  }


  // write the end test message
  fprintf(stderr,"\033[1;33m\n\n%s\nblock verifiers server functions test - Passed test: %d, Failed test: %d\n%s\n\n\n\033[0m",TEST_OUTLINE,count_test,NETWORK_SECURITY_FUNCTIONS_TEST-count_test,TEST_OUTLINE);
  return count_test;

  #undef NETWORK_SECURITY_FUNCTIONS_TEST
  #undef MESSAGE
}