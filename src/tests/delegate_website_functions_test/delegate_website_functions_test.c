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
  int count;

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
  network_functions_test_settings = 1;
  network_functions_test_error_settings = 2;
 
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

  

  // check all errors for each test
  delete_database(database_name,0);
  RESET_ERROR_MESSAGES;
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));

  // server_receive_data_socket_delegates_website_get_statistics
  CHECK_WEBSITE_SERVER_FUNCTIONS_ERROR(SERVER_RECEIVE_DATA_SOCKET_DELEGATES_WEBSITE_GET_STATISTICS_TEST_DATA,"server_receive_data_socket_delegates_website_get_statistics","Could not read the statistics database collection");  
  insert_document_into_collection_json(database_name,"statistics",DATABASE_COLLECTION_STATISTICS_DATA,0);
  RESET_ERROR_MESSAGES;

  // server_receive_data_socket_get_delegates
  CHECK_WEBSITE_SERVER_FUNCTIONS_ERROR(SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_TEST_DATA,"server_receive_data_socket_get_delegates","Could not organize the delegates");  
  RESET_ERROR_MESSAGES;

  // server_receive_data_socket_get_delegates_statistics
  CHECK_WEBSITE_SERVER_FUNCTIONS_ERROR("GET /getdelegatesstatistics?parameter1= HTTP/","server_receive_data_socket_get_delegates_statistics","Invalid parameters");  
  RESET_ERROR_MESSAGES;
  
  CHECK_WEBSITE_SERVER_FUNCTIONS_ERROR(SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_STATISTICS_TEST_DATA,"server_receive_data_socket_get_delegates_statistics","Could not get the data from the delegates database collection");  
  RESET_ERROR_MESSAGES;

  // server_receive_data_socket_get_delegates_information
  CHECK_WEBSITE_SERVER_FUNCTIONS_ERROR("GET /getdelegatesinformation?parameter1= HTTP/","server_receive_data_socket_get_delegates_information","Invalid parameters");  
  RESET_ERROR_MESSAGES;
  
  CHECK_WEBSITE_SERVER_FUNCTIONS_ERROR(SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_INFORMATION_TEST_DATA,"server_receive_data_socket_get_delegates_information","Could not get the delegates information");  
  RESET_ERROR_MESSAGES;

  // server_receive_data_socket_get_delegates_voters_list
  CHECK_WEBSITE_SERVER_FUNCTIONS_ERROR("GET /getdelegatesvoterslist?parameter1= HTTP/","server_receive_data_socket_get_delegates_voters_list","Invalid parameters");  
  RESET_ERROR_MESSAGES;
  
  CHECK_WEBSITE_SERVER_FUNCTIONS_ERROR("GET /getdelegatesvoterslist?parameter1=delegate_name_1 HTTP/","server_receive_data_socket_get_delegates_voters_list","Could not find a registered delegate with the information provided");  
  insert_document_into_collection_json(database_name,"delegates",DELEGATES_TEST_DATA,0);
  RESET_ERROR_MESSAGES;

  CHECK_WEBSITE_SERVER_FUNCTIONS_ERROR(SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_VOTERS_LIST_TEST_DATA,"server_receive_data_socket_get_delegates_voters_list","There are no reserve proofs for the delegate");  
  delete_database(database_name,0);
  RESET_ERROR_MESSAGES;

  // server_receive_data_socket_get_round_statistics
  CHECK_WEBSITE_SERVER_FUNCTIONS_ERROR("GET /getroundstatistics?parameter1= HTTP/ XCA1v18Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3","server_receive_data_socket_get_round_statistics","Invalid parameters");  
  RESET_ERROR_MESSAGES;
  
  CHECK_WEBSITE_SERVER_FUNCTIONS_ERROR("GET /getroundstatistics?parameter1=100 HTTP/ XCA1v18Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3","server_receive_data_socket_get_round_statistics","Invalid block height");  
  RESET_ERROR_MESSAGES;

  CHECK_WEBSITE_SERVER_FUNCTIONS_ERROR(SERVER_RECEIVE_DATA_SOCKET_GET_ROUND_STATISTICS_TEST_DATA,"server_receive_data_socket_get_round_statistics","Could not get the rounds statistics");  
  RESET_ERROR_MESSAGES;

  // write the end test message
  fprintf(stderr,"\033[1;33m\n\n%s\ndelegate website functions test - Passed test: %d, Failed test: %d\n%s\n\n\n\033[0m",TEST_OUTLINE,count_test,DELEGATE_WEBSITE_FUNCTIONS_TOTAL_TEST-count_test,TEST_OUTLINE);
  return count_test;

  #undef DELEGATE_WEBSITE_FUNCTIONS_TOTAL_TEST
}