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
#include "delegate_website_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: delegate_website_functions_test
Description: Test the delegate website functions
Return: The number of passed delegate_website_functions test
-----------------------------------------------------------------------------------------------------------
*/

int delegate_website_functions_test(void)
{
  // Variables
  char data[BUFFER_SIZE];

  // define macros
  #define DELEGATE_WEBSITE_FUNCTIONS_TOTAL_TEST 6
  
  memset(database_name,0,sizeof(database_name));
  memcpy(database_name,DATABASE_NAME_TEST,sizeof(DATABASE_NAME_TEST)-1);
  delegates_website = 1;
  insert_document_into_collection_json(database_name,"reserve_bytes_1",RESERVE_BYTES_TEST_DATA,0);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",RESERVE_PROOFS_TEST_DATA,0);
  insert_document_into_collection_json(database_name,"delegates",DELEGATES_TEST_DATA,0);
  insert_document_into_collection_json(database_name,"statistics",DATABASE_COLLECTION_STATISTICS_DATA,0);

  // reset the variables
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memset(data,0,sizeof(data));
  count_test = 0;
  network_functions_test_settings = 0;
 
  // write the start test message
  fprintf(stderr,"\033[1;34m%s\ndelegate website functions test - Total test: %d\n%s\n\n\033[0m",TEST_OUTLINE,DELEGATE_WEBSITE_FUNCTIONS_TOTAL_TEST,TEST_OUTLINE);
 
  // run the test
  CHECK_WEBSITE_SERVER_FUNCTIONS("server_receive_data_socket_delegates_website_get_statistics",SERVER_RECEIVE_DATA_SOCKET_DELEGATES_WEBSITE_GET_STATISTICS_TEST_DATA);
  CHECK_WEBSITE_SERVER_FUNCTIONS("server_receive_data_socket_get_delegates",SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_TEST_DATA);
  CHECK_WEBSITE_SERVER_FUNCTIONS("server_receive_data_socket_get_delegates_statistics",SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_STATISTICS_TEST_DATA);
  CHECK_WEBSITE_SERVER_FUNCTIONS("server_receive_data_socket_get_delegates_information",SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_INFORMATION_TEST_DATA);
  CHECK_WEBSITE_SERVER_FUNCTIONS("server_receive_data_socket_get_delegates_voters_list",SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_VOTERS_LIST_TEST_DATA);
  CHECK_WEBSITE_SERVER_FUNCTIONS("server_receive_data_socket_get_round_statistics",SERVER_RECEIVE_DATA_SOCKET_GET_ROUND_STATISTICS_TEST_DATA);
  sleep(5);
  delete_database(DATABASE_NAME_TEST,0);

  // write the end test message
  if (count_test == DELEGATE_WEBSITE_FUNCTIONS_TOTAL_TEST)
  {
    fprintf(stderr,"\033[1;32m\n\n%s\ndelegate website functions test - Passed test: %d, Failed test: 0\n%s\n\n\n\033[0m",TEST_OUTLINE,DELEGATE_WEBSITE_FUNCTIONS_TOTAL_TEST,TEST_OUTLINE);
  }
  else
  {
    fprintf(stderr,"\033[1;31m\n\n%s\ndelegate website functions test - Passed test: %d, Failed test: 0\n%s\n\n\n\033[0m",TEST_OUTLINE,DELEGATE_WEBSITE_FUNCTIONS_TOTAL_TEST-count_test,TEST_OUTLINE);
  }
  return count_test;

  #undef DELEGATE_WEBSITE_FUNCTIONS_TOTAL_TEST
}