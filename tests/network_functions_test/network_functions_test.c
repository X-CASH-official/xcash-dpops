#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "define_macros_functions.h"
#include "network_daemon_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "network_wallet_functions.h"
#include "server_functions.h"
#include "string_functions.h"

#include "define_macros_test.h"
#include "variables_test.h"
#include "network_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: send_wallet_http_request_test
Description: Test the send_http_request and other HTTP request wallet functions
Return: The number of passed send_wallet_http_request test
-----------------------------------------------------------------------------------------------------------
*/

int send_wallet_http_request_test()
{  
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));

  if (data == NULL)
  {
    return 0;
  }

  // define macros
  #define SEND_WALLET_HTTP_REQUEST_TOTAL_TEST 6
  #define MESSAGE "{\r\n \"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\",\r\n}"
  
  // reset the variables
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memset(server_message,0,strnlen(server_message,BUFFER_SIZE));
  count_test = 0;

  // set the server_message
  memcpy(server_message,"XCASH_PROOF_OF_STAKE_TEST_DATA",30);

  // write the start test message
  color_print(TEST_OUTLINE,"blue");
  printf("\033[1;34msend_wallet_http_request test - Total test: %d\033[0m\n",SEND_WALLET_HTTP_REQUEST_TOTAL_TEST);
  color_print(TEST_OUTLINE,"blue");
  printf("\n");

  // run the test

  // set the current_round_part and current_round_part_backup_node
  memset(current_round_part,0,strnlen(current_round_part,BUFFER_SIZE));
  memset(current_round_part_backup_node,0,strnlen(current_round_part_backup_node,BUFFER_SIZE));
  memcpy(current_round_part,"1",1);
  memcpy(current_round_part_backup_node,"0",1);

  // test the send_http_request and get_public_address functions
  memset(xcash_wallet_public_address,0,strnlen(xcash_wallet_public_address,BUFFER_SIZE));
  if (get_public_address(0) == 1)
  {   
    color_print("PASSED! Test for sending an HTTP request and getting the public address of the opened X-CASH wallet","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sending an HTTP request and getting the public address of the opened X-CASH wallet","red");
  }

  // test the get_block_template function
  memset(data,0,strnlen(data,BUFFER_SIZE));
  if (get_block_template(data,0) == 1)
  {   
    color_print("PASSED! Test for getting the block template","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for getting the block template","red");
  }

  // test the get_current_block_height function
  memset(data,0,strnlen(data,BUFFER_SIZE));
  if (get_current_block_height(data,0) == 1)
  {   
    color_print("PASSED! Test for getting the current block height","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for getting the current block height","red");
  }

  // test the get_previous_block_hash function
  memset(data,0,strnlen(data,BUFFER_SIZE));
  if (get_previous_block_hash(data,0) == 1)
  {   
    color_print("PASSED! Test for getting the previous block hash","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for getting the previous block hash","red");
  }
  
  // create the message
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  append_string(result_test,MESSAGE);

  // test the sign_data functions
  if (sign_data(result_test,0) == 1)
  {   
    color_print("PASSED! Test for sign data to send data securely","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sign data to send data securely","red");
  }
  
  // test the verify_data functions
  if (verify_data(result_test,0,1,1) == 1)
  {   
    color_print("PASSED! Test for verify data to receive data securely","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for verify data to receive data securely","red");
  }
  


  // write the end test message
  if (count_test == SEND_WALLET_HTTP_REQUEST_TOTAL_TEST)
  {
    printf("\n");
    color_print(TEST_OUTLINE,"green");
    printf("\033[1;32msend_wallet_http_request test - Passed test: %d, Failed test: 0\033[0m\n",SEND_WALLET_HTTP_REQUEST_TOTAL_TEST);
    color_print(TEST_OUTLINE,"green");
    printf("\n\n");
  }
  else
  {
    printf("\n");
    color_print(TEST_OUTLINE,"red");
    printf("\033[1;31msend_wallet_http_request test - Passed test: %d, Failed test: %d\033[0m\n",count_test,SEND_WALLET_HTTP_REQUEST_TOTAL_TEST-count_test);
    color_print(TEST_OUTLINE,"red");
    printf("\n\n");
  } 
  pointer_reset(data);
  return count_test;

  #undef MESSAGE
  #undef SEND_WALLET_HTTP_REQUEST_TOTAL_TEST
}



/*
-----------------------------------------------------------------------------------------------------------
Name: create_server_on_separate_thread
Description: Creates the server on a separate thread
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

void* create_server_on_separate_thread()
{
  create_server(0);
  return NULL;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: send_and_receive_data_socket_test
Description: Test the create_server, send_and_receive_data_socket, send_data, receive_data functions
Return: The number of passed send_and_receive_data_socket test
-----------------------------------------------------------------------------------------------------------
*/

int send_and_receive_data_socket_test()
{   
  // Variables
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* public_address = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* string = (char*)calloc(BUFFER_SIZE,sizeof(char));
  int settings = 1;
  pthread_t thread_id;

  // define macros
  #define SEND_AND_RECEIVE_DATA_SOCKET_TOTAL_TEST 2
  #define MESSAGE "{\r\n \"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\",\r\n}"

  // reset the variables
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memset(server_message,0,strnlen(server_message,BUFFER_SIZE));
  count_test = 0;

  // set the server_message
  memcpy(server_message,"XCASH_PROOF_OF_STAKE_TEST_DATA",30);

  // write the start test message
  color_print(TEST_OUTLINE,"blue");
  printf("\033[1;34msend data socket test - Total test: %d\033[0m\n",SEND_AND_RECEIVE_DATA_SOCKET_TOTAL_TEST);
  color_print(TEST_OUTLINE,"blue");
  printf("\n");

  // run the test

  // set the current_round_part and current_round_part_backup_node
  memset(current_round_part,0,strnlen(current_round_part,BUFFER_SIZE));
  memset(current_round_part_backup_node,0,strnlen(current_round_part_backup_node,BUFFER_SIZE));
  memcpy(current_round_part,"1",1);
  memcpy(current_round_part_backup_node,"0",1); 
  
  // test for creating the server
  if (pthread_create(&thread_id, NULL, &create_server_on_separate_thread,NULL) != 0)
  {
    color_print("FAILED! Test for creating the server","red");
    color_print("FAILED! Test for sending and receving data using sockets","red");
    settings = 0;
  }
  if (settings == 1)
  {
    if (pthread_detach(thread_id) != 0)
    {      
      color_print("FAILED! Test for creating the server","red");
      color_print("FAILED! Test for sending and receving data using sockets","red");
      settings = 0;
    }  
  }
  sleep(1);



  // test for sending and receiving data using sockets
  // create the message
  memset(message,0,strnlen(message,BUFFER_SIZE));
  append_string(message,MESSAGE);

  // sign_data
  if (settings == 1)
  {
    if (sign_data(message,0) == 0)
    { 
      color_print("FAILED! Test for creating the server","red");
      color_print("FAILED! Test for sending and receving data using sockets","red");
    }
  }
 
  if (settings == 1)
  {
    if (send_and_receive_data_socket(string,"127.0.0.1",SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"XCASH_PROOF_OF_STAKE_TEST_DATA",0) <= 0)
    {
      color_print("FAILED! Test for creating the server","red");
      color_print("FAILED! Test for sending and receving data using sockets","red");
      settings = 0;
    }
  }
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));

  // verify the message
  if (settings == 1)
  {
    if (verify_data(string,0,1,1) == 0)
    {   
      color_print("FAILED! Test for creating the server","red");
      color_print("FAILED! Test for sending and receving data using sockets","red");
      settings = 0;
    }
  }

  // parse the data
  if (settings == 1)
  {
    if (parse_json_data(string,"message_settings",data_test) == 0)
    {
      color_print("FAILED! Test for creating the server","red");
      color_print("FAILED! Test for sending and receving data using sockets","red");
      settings = 0;
    }
  }
  
  // check if the received data is correct
  if (settings == 1)
  {
    if (strncmp(data_test,"XCASH_PROOF_OF_STAKE_TEST_DATA",BUFFER_SIZE) == 0)
    {
      color_print("PASSED! Test for creating the server","green");
      color_print("PASSED! Test for sending and receving data using sockets","green");
      count_test += 2;
    }
    else
    {
      color_print("FAILED! Test for creating the server","red");
      color_print("FAILED! Test for sending and receving data using sockets","red");
      settings = 0;
    }
  }



  // write the end test message
  if (count_test == SEND_AND_RECEIVE_DATA_SOCKET_TOTAL_TEST)
  {
    printf("\n");
    color_print(TEST_OUTLINE,"green");
    printf("\033[1;32msend_and_receive_data_socket_test - Passed test: %d, Failed test: 0\033[0m\n",SEND_AND_RECEIVE_DATA_SOCKET_TOTAL_TEST);
    color_print(TEST_OUTLINE,"green");
    printf("\n\n");
  }
  else
  {
    printf("\n");
    color_print(TEST_OUTLINE,"red");
    printf("\033[1;31msend_and_receive_data_socket_test - Passed test: %d, Failed test: %d\033[0m\n",count_test,SEND_AND_RECEIVE_DATA_SOCKET_TOTAL_TEST-count_test);
    color_print(TEST_OUTLINE,"red");
    printf("\n\n");
  } 
  pointer_reset(message);
  pointer_reset(public_address);
  pointer_reset(string);
  return count_test;

  #undef SEND_AND_RECEIVE_DATA_SOCKET_TOTAL_TEST
  #undef MESSAGE
}
