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
#include "delegate_server_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: delegate_server_functions_test
Description: Test the delegate_server functions
Return: The number of passed delegate_server_functions test
-----------------------------------------------------------------------------------------------------------
*/

int delegate_server_functions_test(void)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char data[BUFFER_SIZE];

  // define macros
  #define DELEGATE_SERVER_FUNCTIONS_TOTAL_TEST 3

  // reset the variables
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memset(data,0,sizeof(data));
  count_test = 0;
  error_message.total = 0;
 
  // write the start test message
  fprintf(stderr,"\033[1;34m%s\ndelegate server functions test - Total test: %d\n%s\n\n\033[0m",TEST_OUTLINE,DELEGATE_SERVER_FUNCTIONS_TOTAL_TEST,TEST_OUTLINE);
 
  // run the test

  // test server_receive_data_socket_node_to_block_verifiers_add_reserve_proof
  memcpy(result_test,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF|XCA1v18Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3|RESERVE_PROOF_TEST_DATA|",165);
  memcpy(result_test+strlen(result_test),xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(result_test+strlen(result_test),"|",1);
  memcpy(data_test,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"sign\",\"params\":{\"data\":\"",60);
  memcpy(data_test+60,result_test,strnlen(result_test,sizeof(data_test)));
  memcpy(data_test+strlen(data_test),"\"}}",3);  
  send_http_request(data,"127.0.0.1","/json_rpc",XCASH_WALLET_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,data_test,RECEIVE_DATA_TIMEOUT_SETTINGS,"sign data",0);
  memset(data_test,0,sizeof(data_test));
  parse_json_data(data,"signature",data_test,MAXIMUM_BUFFER_SIZE);
  memcpy(result_test+strlen(result_test),data_test,XCASH_SIGN_DATA_LENGTH);
  memcpy(result_test+strlen(result_test),"|",1);
  send_data_socket("127.0.0.1",SEND_DATA_PORT,result_test);
  sleep(5);
  if (error_message.total == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for server_receive_data_socket_node_to_block_verifiers_add_reserve_proof\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for server_receive_data_socket_node_to_block_verifiers_add_reserve_proof\033[0m\n");
  }

  // test server_receive_data_socket_nodes_to_block_verifiers_register_delegates
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memset(data,0,sizeof(data));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE|delegate_name_1|127.0.0.1|",69);
  memcpy(result_test+strlen(result_test),NEXT_BLOCK_VERIFIERS_PUBLIC_KEY,VRF_PUBLIC_KEY_LENGTH);
  memcpy(result_test+strlen(result_test),"|",1);
  memcpy(result_test+strlen(result_test),xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(result_test+strlen(result_test),"|",1);
  memcpy(data_test,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"sign\",\"params\":{\"data\":\"",60);
  memcpy(data_test+60,result_test,strnlen(result_test,sizeof(data_test)));
  memcpy(data_test+strlen(data_test),"\"}}",3);  
  send_http_request(data,"127.0.0.1","/json_rpc",XCASH_WALLET_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,data_test,RECEIVE_DATA_TIMEOUT_SETTINGS,"sign data",0);
  memset(data_test,0,sizeof(data_test));
  parse_json_data(data,"signature",data_test,MAXIMUM_BUFFER_SIZE);
  memcpy(result_test+strlen(result_test),data_test,XCASH_SIGN_DATA_LENGTH);
  memcpy(result_test+strlen(result_test),"|",1);
  send_data_socket("127.0.0.1",SEND_DATA_PORT,result_test);
  sleep(5);
  if (error_message.total == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for server_receive_data_socket_nodes_to_block_verifiers_register_delegates\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for server_receive_data_socket_nodes_to_block_verifiers_register_delegates\033[0m\n");
  }

  // test server_receive_data_socket_nodes_to_block_verifiers_update_delegates
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memset(data,0,sizeof(data));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE|about|data|",52);
  memcpy(result_test+strlen(result_test),xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(result_test+strlen(result_test),"|",1);
  memcpy(data_test,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"sign\",\"params\":{\"data\":\"",60);
  memcpy(data_test+60,result_test,strnlen(result_test,sizeof(data_test)));
  memcpy(data_test+strlen(data_test),"\"}}",3);  
  send_http_request(data,"127.0.0.1","/json_rpc",XCASH_WALLET_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,data_test,RECEIVE_DATA_TIMEOUT_SETTINGS,"sign data",0);
  memset(data_test,0,sizeof(data_test));
  parse_json_data(data,"signature",data_test,MAXIMUM_BUFFER_SIZE);
  memcpy(result_test+strlen(result_test),data_test,XCASH_SIGN_DATA_LENGTH);
  memcpy(result_test+strlen(result_test),"|",1);
  send_data_socket("127.0.0.1",SEND_DATA_PORT,result_test);
  sleep(5);
  if (error_message.total == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for server_receive_data_socket_nodes_to_block_verifiers_update_delegates\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for server_receive_data_socket_nodes_to_block_verifiers_update_delegates\033[0m\n");
  }
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST,0);

  // write the end test message
  if (count_test == DELEGATE_SERVER_FUNCTIONS_TOTAL_TEST)
  {
    fprintf(stderr,"\033[1;32m\n\n%s\ndelegate server functions test - Passed test: %d, Failed test: 0\n%s\n\n\n\033[0m",TEST_OUTLINE,DELEGATE_SERVER_FUNCTIONS_TOTAL_TEST,TEST_OUTLINE);
  }
  else
  {
    fprintf(stderr,"\033[1;31m\n\n%s\ndelegate server functions test - Passed test: %d, Failed test: 0\n%s\n\n\n\033[0m",TEST_OUTLINE,DELEGATE_SERVER_FUNCTIONS_TOTAL_TEST-count_test,TEST_OUTLINE);
  }
  return count_test;

  #undef DELEGATE_SERVER_FUNCTIONS_TOTAL_TEST
}