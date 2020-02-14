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

#include "network_daemon_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "network_wallet_functions.h"
#include "server_functions.h"
#include "string_functions.h"
#include "define_macros_test.h"
#include "VRF_functions.h"

#include "variables_test.h"
#include "network_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: create_server_on_separate_thread
Description: Creates the server on a separate thread
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

void* create_server_on_separate_thread(void* parameters)
{
  // unused parameter
  (void)parameters;
  
  create_server(0);
  pthread_exit((void *)(intptr_t)1);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: network_functions_test
Description: Test the network functions
Return: The number of passed network_functions_test
-----------------------------------------------------------------------------------------------------------
*/

int network_functions_test(void)
{  
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char message[BUFFER_SIZE];
  char public_address[BUFFER_SIZE];
  char string[BUFFER_SIZE];
  int settings = 1;
  pthread_t thread_id;

  // define macros
  #define GET_PUBLIC_ADDRESS_DATA "{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_address\"}"
  #define MESSAGE "{\r\n \"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\",\r\n}"

  memset(message,0,sizeof(message));
  memset(public_address,0,sizeof(public_address));
  memset(string,0,sizeof(string));

  // reset the variables
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  count_test = 0;

  // write the start test message
  fprintf(stderr,"\033[1;34m%s\nnetwork functions test - Total test: %d\n%s\n\n\033[0m",TEST_OUTLINE,NETWORK_FUNCTIONS_TEST,TEST_OUTLINE);

  pthread_rwlock_wrlock(&rwlock);
  // run the test

  // set the current_round_part and current_round_part_backup_node
  memset(current_round_part,0,strnlen(current_round_part,BUFFER_SIZE));
  memset(current_round_part_backup_node,0,strnlen(current_round_part_backup_node,BUFFER_SIZE));
  memcpy(current_round_part,"1",1);
  memcpy(current_round_part_backup_node,"0",1); 
  pthread_rwlock_unlock(&rwlock);

  // test send_http_request
  if (send_http_request(data_test,"127.0.0.1","/json_rpc",xcash_wallet_port,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,GET_PUBLIC_ADDRESS_DATA,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) <= 0)
  {  
    color_print("FAILED! Test for send_data","red");
    color_print("FAILED! Test for receive_data","red");
    color_print("FAILED! Test for send_http_request","red");
  }
  else
  {
    color_print("PASSED! Test for send_data","green");
    color_print("PASSED! Test for receive_data","green");
    color_print("PASSED! Test for send_http_request","green");
    count_test += 3;
  }  
  
  // Test for create_server
  if (pthread_create(&thread_id, NULL, &create_server_on_separate_thread,NULL) != 0 && pthread_detach(thread_id) != 0)
  {
    color_print("FAILED! Test for create_server","red");
    color_print("FAILED! Test for send_and_receive_data_socket","red");
    settings = 0;
  }
  sleep(10);

  // test for sending and receiving data using sockets
  // create the message
  memset(message,0,sizeof(message));
  memcpy(message,MESSAGE,sizeof(MESSAGE)-1);
  network_functions_test_error_settings = 0;

  // sign_data
  if (settings == 1)
  {
    if (sign_data(message) == 0)
    { 
      color_print("FAILED! Test for create_server","red");
      color_print("FAILED! Test for send_and_receive_data_socket","red");
    }
    memset(string,0,sizeof(string));
    if (send_and_receive_data_socket(string,"127.0.0.1",SEND_DATA_PORT,message,20) <= 0)
    {
      color_print("FAILED! Test for create_server","red");
      color_print("FAILED! Test for send_and_receive_data_socket","red");
      settings = 0;
    }
  }
  
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  network_functions_test_settings = 0;
  network_functions_test_error_settings = 1;
  
  // verify the message
  if (settings == 1 && verify_data(string,1) == 0)
  { 
    color_print("FAILED! Test for create_server","red");
    color_print("FAILED! Test for send_and_receive_data_socket","red");
    settings = 0;
  }
  
  // parse the data
  if (settings == 1 && parse_json_data(string,"message_settings",data_test,BUFFER_SIZE) == 0)
  {
    color_print("FAILED! Test for create_server","red");
    color_print("FAILED! Test for send_and_receive_data_socket","red");
    settings = 0;
  }
  
  // check if the received data is correct
  if (settings == 1)
  {
    if (strncmp(data_test,"XCASH_PROOF_OF_STAKE_TEST_DATA",BUFFER_SIZE) == 0)
    {
      color_print("PASSED! Test for create_server","green");
      color_print("PASSED! Test for send_and_receive_data_socket","green");
      count_test += 2;
    }
    else
    {
      color_print("FAILED! Test for create_server","red");
      color_print("FAILED! Test for send_and_receive_data_socket","red");
    }
  }

  // test send_data_socket
  memset(message,0,sizeof(message));
  memcpy(message,MESSAGE,sizeof(MESSAGE)-1);

  if (sign_data(message) == 1 && send_data_socket("127.0.0.1",SEND_DATA_PORT,message,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1)
  {
    sleep(10);
    if (network_functions_test_settings == 1)
    {
      color_print("PASSED! Test for send_data_socket","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for send_data_socket","red");
    }
  }
  else
  {
    color_print("FAILED! Test for send_data_socket","red");
  }



  // check all errors for each test
  // send_http_request
  RESET_ERROR_MESSAGES;
  if (send_http_request(data_test,"","/json_rpc",xcash_wallet_port,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,GET_PUBLIC_ADDRESS_DATA,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) <= 0 && strstr(error_message.data[0],"Error invalid hostname") != NULL)
  {
    color_print("PASSED! Test for send_http_request checking for invalid hostname","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for send_http_request checking for invalid hostname","red");
  }
  RESET_ERROR_MESSAGES;

  if (send_http_request(data_test,"127.0.0.1","/json_rpc",18289,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,GET_PUBLIC_ADDRESS_DATA,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) <= 0 && strstr(error_message.data[0],"Error connecting to host") != NULL)
  {
    color_print("PASSED! Test for send_http_request checking for Connection error","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for send_http_request checking for Connection error","red");
  }
  RESET_ERROR_MESSAGES;

  // send_and_receive_data_socket
  if (send_and_receive_data_socket(string,"",SEND_DATA_PORT,message,20) <= 0 && strstr(error_message.data[0],"Error invalid hostname") != NULL)
  {
    color_print("PASSED! Test for send_and_receive_data_socket checking for invalid hostname","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for send_and_receive_data_socket checking for invalid hostname","red");
  }
  RESET_ERROR_MESSAGES;

  if (send_and_receive_data_socket(string,"127.0.0.1",18289,message,20) <= 0 && strstr(error_message.data[0],"Error connecting to host") != NULL)
  {
    color_print("PASSED! Test for send_and_receive_data_socket checking for Connection error","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for send_and_receive_data_socket checking for Connection error","red");
  }
  RESET_ERROR_MESSAGES;

  // send_data_socket
  if (send_data_socket("",SEND_DATA_PORT,message,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) <= 0 && strstr(error_message.data[0],"Error invalid hostname") != NULL)
  {
    color_print("PASSED! Test for send_data_socket checking for invalid hostname","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for send_data_socket checking for invalid hostname","red");
  }
  RESET_ERROR_MESSAGES;

  // write the end test message
  fprintf(stderr,"\033[1;33m\n\n%s\nnetwork functions test - Passed test: %d, Failed test: %d\n%s\n\n\n\033[0m",TEST_OUTLINE,count_test,NETWORK_FUNCTIONS_TEST-count_test,TEST_OUTLINE);
  return count_test;
  
  #undef GET_PUBLIC_ADDRESS_DATA
  #undef MESSAGE
}