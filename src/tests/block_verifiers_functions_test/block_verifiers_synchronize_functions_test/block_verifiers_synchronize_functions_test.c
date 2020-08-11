#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "insert_database_functions.h"
#include "delete_database_functions.h"
#include "block_verifiers_synchronize_functions.h"
#include "define_macros_test.h"

#include "variables_test.h"
#include "block_verifiers_synchronize_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: block_verifiers_synchronize_functions_test
Description: Test the block_verifiers_synchronize functions
Return: The number of passed block_verifiers_synchronize_functions test
-----------------------------------------------------------------------------------------------------------
*/

int block_verifiers_synchronize_functions_test(void)
{
  // Variables
  int count;

  // reset the variables
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  count_test = 0;
  network_functions_test_error_settings = 2;
  delete_database(database_name);
  RESET_ERROR_MESSAGES;
 
  // write the start test message
  fprintf(stderr,"\033[1;34m%s\nblock verifiers synchronize functions test - Total test: %d\n%s\n\n\033[0m",TEST_OUTLINE,BLOCK_VERIFIERS_SYNCHRONIZE_FUNCTIONS_TOTAL_TEST,TEST_OUTLINE);
 
  // run the test
  // sync_reserve_proofs_database
  insert_document_into_collection_json(database_name,"reserve_proofs_1",RESERVE_PROOFS_TEST_DATA);
  RESET_ERROR_MESSAGES;
  if (sync_reserve_proofs_database(0,XCASH_DPOPS_delegates_IP_address) == 1)
  {
    color_print("PASSED! Test for sync_reserve_proofs_database","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sync_reserve_proofs_database","red");
  }

  // sync_reserve_bytes_database
  insert_document_into_collection_json(database_name,"reserve_bytes_1",RESERVE_BYTES_TEST_DATA);
  RESET_ERROR_MESSAGES;
  if (sync_reserve_bytes_database(0,1,XCASH_DPOPS_delegates_IP_address) == 1)
  {
    color_print("PASSED! Test for sync_reserve_bytes_database","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sync_reserve_bytes_database","red");
  }

  // sync_delegates_database
  insert_document_into_collection_json(database_name,"delegates",DELEGATES_TEST_DATA);
  RESET_ERROR_MESSAGES;
  if (sync_delegates_database(0,XCASH_DPOPS_delegates_IP_address) == 1)
  {
    color_print("PASSED! Test for sync_delegates_database","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sync_delegates_database","red");
  }

  // sync_statistics_database
  insert_document_into_collection_json(database_name,"statistics",DATABASE_COLLECTION_STATISTICS_DATA);
  RESET_ERROR_MESSAGES;
  if (sync_statistics_database(0,XCASH_DPOPS_delegates_IP_address) == 1)
  {
    color_print("PASSED! Test for sync_statistics_database","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sync_statistics_database","red");
  }

  // sync_all_block_verifiers_list
  network_data_node_settings = 0;
  INITIALIZE_NETWORK_DATA_NODES_TEST

  if (sync_all_block_verifiers_list(1) == 1)
  {
    color_print("PASSED! Test for sync_all_block_verifiers_list for block verifiers","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sync_all_block_verifiers_list for block verifiers","red");
  }

  network_data_node_settings = 1;
  if (sync_all_block_verifiers_list(1) == 1)
  {
    color_print("PASSED! Test for sync_all_block_verifiers_list for network data nodes","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sync_all_block_verifiers_list for network data nodes","red");
  }

  // get_synced_block_verifiers
  INITIALIZE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_TEST
  network_data_node_settings = 0;
  if (get_synced_block_verifiers() == 1)
  {
    color_print("PASSED! Test for get_synced_block_verifiers","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for get_synced_block_verifiers","red");
  }



  // check all errors for each test
  delete_database(database_name);
  RESET_ERROR_MESSAGES;

  network_data_node_settings = 1;
  if (sync_all_block_verifiers_list(1) == 0)
  {
    for (count = 0; count < error_message.total; count++)
    {
      if (strncmp(error_message.data[count],"Could not organize the delegates",BUFFER_SIZE) == 0)
      {
        break;
      }
    }
    if (count != error_message.total)
    {
      color_print("PASSED! Test for sync_all_block_verifiers_list for network data nodes checking for Could not organize the delegates","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for sync_all_block_verifiers_list for network data nodes checking for Could not organize the delegates","red");
    }
  }
  else
  {
    color_print("FAILED! Test for sync_all_block_verifiers_list for network data nodes checking for Could not organize the delegates","red");
  }

  for (count = 0; count < NETWORK_DATA_NODES_AMOUNT; count++)
  {
    memset(network_data_nodes_list.network_data_nodes_public_address[count],0,sizeof(network_data_nodes_list.network_data_nodes_public_address[count]));
    memset(network_data_nodes_list.network_data_nodes_IP_address[count],0,sizeof(network_data_nodes_list.network_data_nodes_IP_address[count]));
  }
  INITIALIZE_NETWORK_DATA_NODES

  // write the end test message
  fprintf(stderr,"\033[1;33m\n\n%s\nblock verifiers synchronize functions test - Passed test: %d, Failed test: %d\n%s\n\n\n\033[0m",TEST_OUTLINE,count_test,BLOCK_VERIFIERS_SYNCHRONIZE_FUNCTIONS_TOTAL_TEST-count_test,TEST_OUTLINE);
  return count_test;
}