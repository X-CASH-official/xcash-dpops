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
  #define NETWORK_FUNCTIONS_TEST 6
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
  color_print(TEST_OUTLINE,"blue");
  fprintf(stderr,"\033[1;34mnetwork functions test - Total test: %d\033[0m\n",NETWORK_FUNCTIONS_TEST);
  color_print(TEST_OUTLINE,"blue");
  fprintf(stderr,"\n");

  pthread_rwlock_wrlock(&rwlock);
  // run the test

  // set the current_round_part and current_round_part_backup_node
  memset(current_round_part,0,strnlen(current_round_part,BUFFER_SIZE));
  memset(current_round_part_backup_node,0,strnlen(current_round_part_backup_node,BUFFER_SIZE));
  memcpy(current_round_part,"1",1);
  memcpy(current_round_part_backup_node,"0",1); 
  pthread_rwlock_unlock(&rwlock);

  // test send_http_request
  if (send_http_request(data_test,"127.0.0.1","/json_rpc",xcash_wallet_port,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,GET_PUBLIC_ADDRESS_DATA,RECEIVE_DATA_TIMEOUT_SETTINGS,"get public address",0) <= 0)
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
  if (pthread_create(&thread_id, NULL, &create_server_on_separate_thread,NULL) != 0)
  {
    color_print("FAILED! Test for create_server","red");
    color_print("FAILED! Test for send_and_receive_data_socket","red");
    settings = 0;
  }
  if (settings == 1)
  {
    if (pthread_detach(thread_id) != 0)
    {
      color_print("FAILED! Test for create_server","red");
      color_print("FAILED! Test for send_and_receive_data_socket","red");
      settings = 0;
    }  
  }
  sleep(1);



  // test for sending and receiving data using sockets
  // create the message
  memset(message,0,sizeof(message));
  memcpy(message,MESSAGE,sizeof(MESSAGE)-1);

  // sign_data
  if (settings == 1)
  {
    if (sign_data(message,0) == 0)
    { 
      color_print("FAILED! Test for create_server","red");
      color_print("FAILED! Test for send_and_receive_data_socket","red");
    }
    memset(string,0,strlen(string));
    if (send_and_receive_data_socket(string,"127.0.0.1",SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"XCASH_PROOF_OF_STAKE_TEST_DATA",0) <= 0)
    {
      color_print("FAILED! Test for create_server","red");
      color_print("FAILED! Test for send_and_receive_data_socket","red");
      settings = 0;
    }
  }
  
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  
  // verify the message
  if (settings == 1)
  {
    if (verify_data(string,0,1) == 0)
    {   
      color_print("FAILED! Test for create_server","red");
      color_print("FAILED! Test for send_and_receive_data_socket","red");
      settings = 0;
    }
  }
  
  // parse the data
  if (settings == 1)
  {
    if (parse_json_data(string,"message_settings",data_test,BUFFER_SIZE) == 0)
    {
      color_print("FAILED! Test for create_server","red");
      color_print("FAILED! Test for send_and_receive_data_socket","red");
      settings = 0;
    }
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
  network_functions_test_settings = 0;
  memset(message,0,sizeof(message));
  memcpy(message,MESSAGE,sizeof(MESSAGE)-1);

  if (sign_data(message,0) == 1 && send_data_socket("127.0.0.1",SEND_DATA_PORT,message) == 1)
  {
    sleep(5);
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

  // write the end test message
  if (count_test == NETWORK_FUNCTIONS_TEST)
  {
    fprintf(stderr,"\n");
    color_print(TEST_OUTLINE,"green");
    fprintf(stderr,"\033[1;32mnetwork functions test - Passed test: %d, Failed test: 0\033[0m\n",NETWORK_FUNCTIONS_TEST);
    color_print(TEST_OUTLINE,"green");
    fprintf(stderr,"\n\n");
  }
  else
  {
    fprintf(stderr,"\n");
    color_print(TEST_OUTLINE,"red");
    fprintf(stderr,"\033[1;31mnetwork functions test - Passed test: %d, Failed test: %d\033[0m\n",count_test,NETWORK_FUNCTIONS_TEST-count_test);
    color_print(TEST_OUTLINE,"red");
    fprintf(stderr,"\n\n");
  } 
  return count_test;

  #undef NETWORK_FUNCTIONS_TEST
  #undef GET_PUBLIC_ADDRESS_DATA
  #undef MESSAGE
}