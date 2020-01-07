#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "define_macros_test.h"

#include "delete_database_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "variables_test.h"
#include "block_verifiers_server_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: block_verifiers_server_functions_test
Description: Test the block_verifiers_server functions
Return: The number of passed block_verifiers_server_functions test
-----------------------------------------------------------------------------------------------------------
*/

int block_verifiers_server_functions_test(void)
{
  // define macros
  #define BLOCK_VERIFIERS_SERVER_FUNCTIONS_TOTAL_TEST 9
  #define CHECK_BLOCK_VERIFIERS_SERVER_FUNCTIONS(FUNCTION_NAME,MESSAGE) \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,MESSAGE,sizeof(MESSAGE)-1); \
  if (sign_data(result_test,0) == 1 && send_data_socket("127.0.0.1",SEND_DATA_PORT,result_test) == 1) \
  { \
    sleep(5); \
    if (error_message.total == 0) \
    { \
      fprintf(stderr,"\033[1;32mPASSED! Test for %s\033[0m\n",FUNCTION_NAME); \
      count_test++; \
    } \
    else \
    { \
      fprintf(stderr,"\033[1;31mFAILED! Test for %s\033[0m\n",FUNCTION_NAME); \
    } \
  } \
  else \
  { \
    fprintf(stderr,"\033[1;31mFAILED! Test for %s\033[0m\n",FUNCTION_NAME); \
  } \
  error_message.total = 0;

  // reset the variables
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  count_test = 0;
  error_message.total = 0;
 
  // write the start test message
  color_print(TEST_OUTLINE,"blue");
  fprintf(stderr,"\033[1;34mblock verifiers server functions test - Total test: %d\033[0m\n",BLOCK_VERIFIERS_SERVER_FUNCTIONS_TOTAL_TEST);
  color_print(TEST_OUTLINE,"blue");
  fprintf(stderr,"\n");
 
  // run the test
  CHECK_BLOCK_VERIFIERS_SERVER_FUNCTIONS("server_receive_data_socket_block_verifiers_to_block_verifiers_invalid_reserve_proofs",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS_TEST_DATA);
  CHECK_BLOCK_VERIFIERS_SERVER_FUNCTIONS("server_receive_data_socket_block_verifiers_to_network_data_nodes_block_verifiers_current_time",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_NETWORK_DATA_NODES_BLOCK_VERIFIERS_CURRENT_TIME_TEST_DATA);
  CHECK_BLOCK_VERIFIERS_SERVER_FUNCTIONS("server_receive_data_socket_main_network_data_node_to_block_verifier_start_block",SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_START_BLOCK_TEST_DATA);
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST,0);
  
  // set the current_round_part and current_round_part_backup_node
  memset(current_round_part_backup_node,0,sizeof(current_round_part_backup_node));
  memcpy(current_round_part_backup_node,"2",1);
  main_network_data_node_create_block = 1;
  memset(network_data_nodes_list.network_data_nodes_public_address[0],0,sizeof(network_data_nodes_list.network_data_nodes_public_address[0]));
  memcpy(network_data_nodes_list.network_data_nodes_public_address[0],xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  CHECK_BLOCK_VERIFIERS_SERVER_FUNCTIONS("server_receive_data_socket_main_network_data_node_to_block_verifier_create_new_block",SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_CREATE_NEW_BLOCK_TEST_DATA);
  CHECK_BLOCK_VERIFIERS_SERVER_FUNCTIONS("server_receive_data_socket_block_verifier_to_main_network_data_node_create_new_block",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIER_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK_TEST_DATA);
  CHECK_BLOCK_VERIFIERS_SERVER_FUNCTIONS("server_receive_data_socket_main_node_to_node_message_part_4",SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_TEST_DATA);
  CHECK_BLOCK_VERIFIERS_SERVER_FUNCTIONS("server_receive_data_socket_node_to_node",SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NODE_TEST_DATA);
  CHECK_BLOCK_VERIFIERS_SERVER_FUNCTIONS("server_receive_data_socket_block_verifiers_to_block_verifiers_vrf_data",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA_TEST_DATA);
  CHECK_BLOCK_VERIFIERS_SERVER_FUNCTIONS("server_receive_data_socket_block_verifiers_to_block_verifiers_block_blob_signature",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE_TEST_DATA);

  // write the end test message
  if (count_test == BLOCK_VERIFIERS_SERVER_FUNCTIONS_TOTAL_TEST)
  {
    fprintf(stderr,"\n\n");
    color_print(TEST_OUTLINE,"green");
    fprintf(stderr,"\033[1;32mblock verifiers server functions test - Passed test: %d, Failed test: 0\033[0m\n",BLOCK_VERIFIERS_SERVER_FUNCTIONS_TOTAL_TEST);
    color_print(TEST_OUTLINE,"green");
    fprintf(stderr,"\n\n");
  }
  else
  {
    fprintf(stderr,"\n\n");
    color_print(TEST_OUTLINE,"red");
    fprintf(stderr,"\033[1;31mblock verifiers server functions test - Passed test: %d, Failed test: %d\033[0m\n",count_test,BLOCK_VERIFIERS_SERVER_FUNCTIONS_TOTAL_TEST-count_test);
    color_print(TEST_OUTLINE,"red");
    fprintf(stderr,"\n\n");
  }
  return count_test;

  #undef BLOCK_VERIFIERS_SERVER_FUNCTIONS_TOTAL_TEST
  #undef CHECK_BLOCK_VERIFIERS_SERVER_FUNCTIONS
}