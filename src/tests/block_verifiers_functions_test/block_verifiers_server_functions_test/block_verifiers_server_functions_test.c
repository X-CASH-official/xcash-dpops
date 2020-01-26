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
#include "string_functions.h"
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
  #define BLOCK_VERIFIERS_SERVER_FUNCTIONS_TOTAL_TEST 10

  // reset the variables
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  count_test = 0;
  error_message.total = 0;
 
  // write the start test message
  fprintf(stderr,"\033[1;34m%s\nblock verifiers server functions test - Total test: %d\n%s\n\n\033[0m",TEST_OUTLINE,BLOCK_VERIFIERS_SERVER_FUNCTIONS_TOTAL_TEST,TEST_OUTLINE);
 
  // run the test
  CHECK_SERVER_FUNCTIONS("server_receive_data_socket_block_verifiers_to_block_verifiers_invalid_reserve_proofs",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS_TEST_DATA);
  CHECK_SERVER_FUNCTIONS("server_receive_data_socket_block_verifiers_to_network_data_nodes_block_verifiers_current_time",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_NETWORK_DATA_NODES_BLOCK_VERIFIERS_CURRENT_TIME_TEST_DATA);
  main_network_data_node_create_block = 1;
  CHECK_SERVER_FUNCTIONS("server_receive_data_socket_main_network_data_node_to_block_verifier_start_block",SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_START_BLOCK_TEST_DATA);
  sleep(5);
  delete_collection_from_database(DATABASE_NAME_TEST,DATABASE_COLLECTION_TEST,0);
  main_network_data_node_create_block = 0;
  
  // set the current_round_part and current_round_part_backup_node
  memset(current_round_part_backup_node,0,sizeof(current_round_part_backup_node));
  memcpy(current_round_part_backup_node,"2",1);
  main_network_data_node_create_block = 1;
  memset(network_data_nodes_list.network_data_nodes_public_address[0],0,sizeof(network_data_nodes_list.network_data_nodes_public_address[0]));
  memcpy(network_data_nodes_list.network_data_nodes_public_address[0],xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  sleep(5);
  network_functions_test_error_settings = 0;
  CHECK_SERVER_FUNCTIONS("server_receive_data_socket_main_network_data_node_to_block_verifier_create_new_block",SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_CREATE_NEW_BLOCK_TEST_DATA);
  network_functions_test_error_settings = 1;
  CHECK_SERVER_FUNCTIONS("server_receive_data_socket_block_verifier_to_main_network_data_node_create_new_block",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIER_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK_TEST_DATA);
  network_functions_test_error_settings = 2;
  CHECK_SERVER_FUNCTIONS("server_receive_data_socket_main_node_to_node_message_part_4",SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_TEST_DATA);
  CHECK_SERVER_FUNCTIONS("server_receive_data_socket_node_to_node",SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NODE_TEST_DATA);
  CHECK_SERVER_FUNCTIONS("server_receive_data_socket_block_verifiers_to_block_verifiers_vrf_data",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA_TEST_DATA);
  CHECK_SERVER_FUNCTIONS("server_receive_data_socket_block_verifiers_to_block_verifiers_block_blob_signature",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE_TEST_DATA);



  // check all errors for each test
  network_functions_test_error_settings = 2;
  CHECK_SERVER_FUNCTIONS_ERROR("server_receive_data_socket_block_verifiers_to_block_verifiers_invalid_reserve_proofs",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS_TEST_DATA,1,xcash_wallet_public_address,TEST_WALLET,"Could not verify the message");
  main_network_data_node_create_block = 1;
  CHECK_SERVER_FUNCTIONS_ERROR("server_receive_data_socket_main_network_data_node_to_block_verifier_start_block",SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_START_BLOCK_TEST_DATA,1,xcash_wallet_public_address,TEST_WALLET,"Could not verify the message");
  CHECK_SERVER_FUNCTIONS_ERROR("server_receive_data_socket_main_network_data_node_to_block_verifier_create_new_block",MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_CREATE_NEW_BLOCK,1,xcash_wallet_public_address,TEST_WALLET,"Could not verify data");
  CHECK_SERVER_FUNCTIONS_ERROR("server_receive_data_socket_block_verifier_to_main_network_data_node_create_new_block",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIER_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK_TEST_DATA,1,xcash_wallet_public_address,TEST_WALLET,"Could not verify data");
  main_network_data_node_create_block = 0;
  CHECK_SERVER_FUNCTIONS_ERROR("server_receive_data_socket_main_node_to_node_message_part_4",SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_TEST_DATA,1,xcash_wallet_public_address,TEST_WALLET,"Could not verify data");
  CHECK_SERVER_FUNCTIONS_ERROR("server_receive_data_socket_main_node_to_node_message_part_4",SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_TEST_DATA,0,"","","Invalid main node");
  CHECK_SERVER_FUNCTIONS_ERROR("server_receive_data_socket_node_to_node",SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NODE_TEST_DATA,1,xcash_wallet_public_address,TEST_WALLET,"Could not verify data");
  CHECK_SERVER_FUNCTIONS_ERROR("server_receive_data_socket_block_verifiers_to_block_verifiers_vrf_data",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA_TEST_DATA,1,xcash_wallet_public_address,TEST_WALLET,"Could not verify data");
  CHECK_SERVER_FUNCTIONS_ERROR("server_receive_data_socket_block_verifiers_to_block_verifiers_block_blob_signature",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE_TEST_DATA,1,xcash_wallet_public_address,TEST_WALLET,"Could not verify data");

  // write the end test message
  fprintf(stderr,"\033[1;33m\n\n%s\nblock verifiers server functions test - Passed test: %d, Failed test: %d\n%s\n\n\n\033[0m",TEST_OUTLINE,count_test,BLOCK_VERIFIERS_SERVER_FUNCTIONS_TOTAL_TEST-count_test,TEST_OUTLINE);
  return count_test;

  #undef BLOCK_VERIFIERS_SERVER_FUNCTIONS_TOTAL_TEST
}