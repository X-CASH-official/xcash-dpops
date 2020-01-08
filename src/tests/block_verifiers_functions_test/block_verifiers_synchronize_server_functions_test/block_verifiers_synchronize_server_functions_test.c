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
#include "variables_test.h"
#include "block_verifiers_synchronize_server_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: block_verifiers_synchronize_server_functions_test
Description: Test the block_verifiers_synchronize_server functions
Return: The number of passed block_verifiers_synchronize_server_functions test
-----------------------------------------------------------------------------------------------------------
*/

int block_verifiers_synchronize_server_functions_test(void)
{
  // define macros
  #define BLOCK_VERIFIERS_SYNCHRONIZE_SERVER_FUNCTIONS_TOTAL_TEST 14
  
  // initialize the previous, current and next block verifiers list
  memcpy(previous_block_verifiers_list.block_verifiers_name[0],"delegate_1",10);
  memcpy(previous_block_verifiers_list.block_verifiers_public_address[0],"XCA1v18Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3",XCASH_WALLET_LENGTH);
  memcpy(previous_block_verifiers_list.block_verifiers_IP_address[0],"127.0.0.1",9);
  memcpy(previous_block_verifiers_list.block_verifiers_public_key[0],NEXT_BLOCK_VERIFIERS_PUBLIC_KEY,VRF_PUBLIC_KEY_LENGTH);
  memcpy(previous_block_verifiers_list.block_verifiers_name[1],"delegate_1",10);
  memcpy(previous_block_verifiers_list.block_verifiers_public_address[1],"XCA1v18Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3",XCASH_WALLET_LENGTH);
  memcpy(previous_block_verifiers_list.block_verifiers_IP_address[1],"127.0.0.1",9);
  memcpy(previous_block_verifiers_list.block_verifiers_public_key[1],NEXT_BLOCK_VERIFIERS_PUBLIC_KEY,VRF_PUBLIC_KEY_LENGTH);
  memcpy(current_block_verifiers_list.block_verifiers_name[0],"delegate_1",10);
  memcpy(current_block_verifiers_list.block_verifiers_public_address[0],"XCA1v18Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3",XCASH_WALLET_LENGTH);
  memcpy(current_block_verifiers_list.block_verifiers_IP_address[0],"127.0.0.1",9);
  memcpy(current_block_verifiers_list.block_verifiers_public_key[0],NEXT_BLOCK_VERIFIERS_PUBLIC_KEY,VRF_PUBLIC_KEY_LENGTH);
  memcpy(current_block_verifiers_list.block_verifiers_name[1],"delegate_1",10);
  memcpy(current_block_verifiers_list.block_verifiers_public_address[1],"XCA1v18Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3",XCASH_WALLET_LENGTH);
  memcpy(current_block_verifiers_list.block_verifiers_IP_address[1],"127.0.0.1",9);
  memcpy(current_block_verifiers_list.block_verifiers_public_key[1],NEXT_BLOCK_VERIFIERS_PUBLIC_KEY,VRF_PUBLIC_KEY_LENGTH);
  memcpy(next_block_verifiers_list.block_verifiers_name[0],"delegate_1",10);
  memcpy(next_block_verifiers_list.block_verifiers_public_address[0],"XCA1v18Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3",XCASH_WALLET_LENGTH);
  memcpy(next_block_verifiers_list.block_verifiers_IP_address[0],"127.0.0.1",9);
  memcpy(next_block_verifiers_list.block_verifiers_public_key[0],NEXT_BLOCK_VERIFIERS_PUBLIC_KEY,VRF_PUBLIC_KEY_LENGTH);
  memcpy(next_block_verifiers_list.block_verifiers_name[1],"delegate_1",10);
  memcpy(next_block_verifiers_list.block_verifiers_public_address[1],"XCA1v18Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3",XCASH_WALLET_LENGTH);
  memcpy(next_block_verifiers_list.block_verifiers_IP_address[1],"127.0.0.1",9);
  memcpy(next_block_verifiers_list.block_verifiers_public_key[1],NEXT_BLOCK_VERIFIERS_PUBLIC_KEY,VRF_PUBLIC_KEY_LENGTH);

  // reset the variables
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  count_test = 0;
  error_message.total = 0;
 
  // write the start test message
  fprintf(stderr,"\033[1;34m%s\nblock verifiers synchronize server functions test - Total test: %d\n%s\n\n\033[0m",TEST_OUTLINE,BLOCK_VERIFIERS_SYNCHRONIZE_SERVER_FUNCTIONS_TOTAL_TEST,TEST_OUTLINE);
 
  // run the test
  CHECK_SERVER_FUNCTIONS("server_receive_data_socket_node_to_network_data_nodes_get_previous_current_next_block_verifiers_list",SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_TEST_DATA);
  CHECK_SERVER_FUNCTIONS("server_receive_data_socket_node_to_network_data_nodes_get_current_block_verifiers_list",SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST_TEST_DATA);
  CHECK_SERVER_FUNCTIONS("server_receive_data_socket_network_data_nodes_to_network_data_nodes_database_sync_check",SERVER_RECEIVE_DATA_SOCKET_NETWORK_DATA_NODES_TO_NETWORK_DATA_NODES_DATABASE_SYNC_CHECK_TEST_DATA);
  CHECK_SERVER_FUNCTIONS("server_receive_data_socket_nodes_to_block_verifiers_reserve_bytes_database_sync_check_all_update",SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_TEST_DATA);
  
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,RESERVE_BYTES_TEST_DATA,0);
  CHECK_SERVER_FUNCTIONS("server_receive_data_socket_node_to_block_verifiers_get_reserve_bytes_database_hash",SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATABASE_HASH_TEST_DATA);
  
  CHECK_SERVER_FUNCTIONS("server_receive_data_socket_node_to_block_verifiers_check_if_current_block_verifier",SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_CHECK_IF_CURRENT_BLOCK_VERIFIER_TEST_DATA);
  CHECK_SERVER_FUNCTIONS("server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_all_update",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_TEST_DATA);
  CHECK_SERVER_FUNCTIONS("server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_download_file_update",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA);
  CHECK_SERVER_FUNCTIONS("server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_all_update",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_TEST_DATA);
  CHECK_SERVER_FUNCTIONS("server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_download_file_update",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA);
  CHECK_SERVER_FUNCTIONS("server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_sync_check_update",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_TEST_DATA);
  CHECK_SERVER_FUNCTIONS("server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_download_file_update",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA);
  CHECK_SERVER_FUNCTIONS("server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_sync_check_update",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE_TEST_DATA);
  CHECK_SERVER_FUNCTIONS("server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_download_file_update",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA);
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST,0);

  // write the end test message
  if (count_test == BLOCK_VERIFIERS_SYNCHRONIZE_SERVER_FUNCTIONS_TOTAL_TEST)
  {
    fprintf(stderr,"\033[1;32m\n\n%s\nblock verifiers synchronize server functions test - Passed test: %d, Failed test: 0\n%s\n\n\n\033[0m",TEST_OUTLINE,BLOCK_VERIFIERS_SYNCHRONIZE_SERVER_FUNCTIONS_TOTAL_TEST,TEST_OUTLINE);
  }
  else
  {
    fprintf(stderr,"\033[1;31m\n\n%s\nblock verifiers synchronize server functions test - Passed test: %d, Failed test: 0\n%s\n\n\n\033[0m",TEST_OUTLINE,BLOCK_VERIFIERS_SYNCHRONIZE_SERVER_FUNCTIONS_TOTAL_TEST-count_test,TEST_OUTLINE);
  }
  return count_test;

  #undef BLOCK_VERIFIERS_SYNCHRONIZE_SERVER_FUNCTIONS_TOTAL_TEST
}