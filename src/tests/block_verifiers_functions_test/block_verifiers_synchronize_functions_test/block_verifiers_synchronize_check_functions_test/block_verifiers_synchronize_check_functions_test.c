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
#include "block_verifiers_synchronize_check_functions.h"
#include "define_macros_test.h"

#include "variables_test.h"
#include "block_verifiers_synchronize_check_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: block_verifiers_synchronize_check_functions_test
Description: Test the block_verifiers_synchronize_check functions
Return: The number of passed block_verifiers_synchronize_check_functions test
-----------------------------------------------------------------------------------------------------------
*/

int block_verifiers_synchronize_check_functions_test(void)
{
  // Variables
  int count;

  // reset the variables
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  count_test = 0;
  network_functions_test_error_settings = 2;
  delete_database(database_name,0);
  RESET_ERROR_MESSAGES;
 
  // write the start test message
  fprintf(stderr,"\033[1;34m%s\nblock verifiers synchronize check functions test - Total test: %d\n%s\n\n\033[0m",TEST_OUTLINE,BLOCK_VERIFIERS_SYNCHRONIZE_CHECK_FUNCTIONS_TOTAL_TEST,TEST_OUTLINE);
 
  // run the test
  // sync_reserve_proofs_database
  insert_document_into_collection_json(database_name,"reserve_proofs_1",RESERVE_PROOFS_TEST_DATA,0);
  insert_document_into_collection_json(database_name,"reserve_bytes_1","{\"message_settings\": \"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATABASE_HASH\",\"block_height\": \"521850\"}",0);
  insert_document_into_collection_json(database_name,"delegates",DELEGATES_TEST_DATA,0);
  insert_document_into_collection_json(database_name,"statistics",DATABASE_COLLECTION_STATISTICS_DATA,0);
  INITIALIZE_NETWORK_DATA_NODES_TEST
 

  if (sync_check_reserve_proofs_database(1) == 1)
  {
    color_print("PASSED! Test for sync_check_reserve_proofs_database from a random block verifier","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sync_check_reserve_proofs_database from a random block verifier","red");
  }

  if (sync_check_reserve_bytes_database(1,0) == 1)
  {
    color_print("PASSED! Test for sync_check_reserve_bytes_database from a random block verifier","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sync_check_reserve_bytes_database from a random block verifier","red");
  }

  if (sync_check_delegates_database(1) == 1)
  {
    color_print("PASSED! Test for sync_check_delegates_database from a random block verifier","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sync_check_delegates_database from a random block verifier","red");
  }

  if (sync_check_statistics_database(1) == 1)
  {
    color_print("PASSED! Test for sync_check_statistics_database from a random block verifier","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sync_check_statistics_database from a random block verifier","red");
  }

  if (sync_check_reserve_proofs_database(3) == 1)
  {
    color_print("PASSED! Test for sync_check_reserve_proofs_database from a random network data node","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sync_check_reserve_proofs_database from a random network data node","red");
  }

  if (sync_check_reserve_bytes_database(3,0) == 1)
  {
    color_print("PASSED! Test for sync_check_reserve_bytes_database from a random network data node","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sync_check_reserve_bytes_database from a random network data node","red");
  }

  if (sync_check_delegates_database(3) == 1)
  {
    color_print("PASSED! Test for sync_check_delegates_database from a random network data node","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sync_check_delegates_database from a random network data node","red");
  }

  if (sync_check_statistics_database(3) == 1)
  {
    color_print("PASSED! Test for sync_check_statistics_database from a random network data node","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sync_check_statistics_database from a random network data node","red");
  }

  if (sync_check_majority_reserve_proofs_database() == 1)
  {
    color_print("PASSED! Test for sync_check_majority_reserve_proofs_database","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sync_check_majority_reserve_proofs_database","red");
  }

  if (sync_check_majority_reserve_bytes_database(1) == 1)
  {
    color_print("PASSED! Test for sync_check_majority_reserve_bytes_database","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sync_check_majority_reserve_bytes_database","red");
  }

  if (sync_check_majority_delegates_database() == 1)
  {
    color_print("PASSED! Test for sync_check_majority_delegates_database","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sync_check_majority_delegates_database","red");
  }

  if (sync_check_majority_statistics_database() == 1)
  {
    color_print("PASSED! Test for sync_check_majority_statistics_database","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sync_check_majority_statistics_database","red");
  }

  for (count = 0; count < NETWORK_DATA_NODES_AMOUNT; count++)
  {
    memset(network_data_nodes_list.network_data_nodes_public_address[count],0,sizeof(network_data_nodes_list.network_data_nodes_public_address[count]));
    memset(network_data_nodes_list.network_data_nodes_IP_address[count],0,sizeof(network_data_nodes_list.network_data_nodes_IP_address[count]));
  }
  INITIALIZE_NETWORK_DATA_NODES

  // write the end test message
  fprintf(stderr,"\033[1;33m\n\n%s\nblock verifiers synchronize check functions test - Passed test: %d, Failed test: %d\n%s\n\n\n\033[0m",TEST_OUTLINE,count_test,BLOCK_VERIFIERS_SYNCHRONIZE_CHECK_FUNCTIONS_TOTAL_TEST-count_test,TEST_OUTLINE);
  return count_test;
}