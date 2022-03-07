#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "define_macros_test.h"

#include "insert_database_functions.h"
#include "delete_database_functions.h"
#include "count_database_functions.h"
#include "read_database_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "string_functions.h"
#include "variables_test.h"
#include "delegate_server_functions.h"
#include "remote_data_functions.h"
#include "remote_data_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: remote_data_functions_test
Description: Test the remote_data functions
Return: The number of passed remote_data_functions test
-----------------------------------------------------------------------------------------------------------
*/

int remote_data_functions_test(void)
{
  // Variables
  char data[BUFFER_SIZE];
  int count;

  // define macros 
  #define REMOTE_DATA_FUNCTIONS_TEST_ERROR(DATA,FUNCTION_NAME,TEST) \
  memcpy(result_test,DATA,sizeof(DATA)-1); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS); \
  sleep(5); \
  for (count = 0; count < error_message.total; count++) \
  { \
    if (strncmp(error_message.data[count],TEST,BUFFER_SIZE) == 0) \
    { \
      break; \
    } \
  } \
  if (count != error_message.total) \
  { \
    fprintf(stderr,"\033[1;32mPASSED! Test for %s checking for %s\033[0m\n",FUNCTION_NAME,TEST); \
    count_test++; \
  } \
  else \
  { \
    fprintf(stderr,"\033[1;31mFAILED! Test for %s checking for %s\033[0m\n",FUNCTION_NAME,TEST); \
  } \
  delete_database(database_name); \
  memset(result_test,0,sizeof(result_test)); \
  memset(data_test,0,sizeof(data_test)); \
  memset(data,0,sizeof(data));

  // reset the variables
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memset(data,0,sizeof(data));
  count_test = 0;
  error_message.total = 0;
 
  // write the start test message
  fprintf(stderr,"\033[1;34m%s\nremote data functions test - Total test: %d\n%s\n\n\033[0m",TEST_OUTLINE,REMOTE_DATA_FUNCTIONS_TOTAL_TEST,TEST_OUTLINE);
 
  // run the test
  network_functions_test_error_settings = 2;

  // get_address_settings - return address
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_REMOTE_DATA_GET_ADDRESS_SETTINGS|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|",159);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"address}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for get_address_settings - return address\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for get_address_settings - return address\033[0m\n");
  }

  // get_address_settings - return address due to not finding the address
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_REMOTE_DATA_GET_ADDRESS_SETTINGS|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|",159);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"address}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for get_address_settings - return address due to not finding the address\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for get_address_settings - return address due to not finding the address\033[0m\n");
  }

  // get_address_settings - return saddress
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_REMOTE_DATA_GET_ADDRESS_SETTINGS|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|",159);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"saddress}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for get_address_settings - return saddress\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for get_address_settings - return saddress\033[0m\n");
  }

  // get_address_settings - return paddress
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"\",\"saddress\":\"\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_REMOTE_DATA_GET_ADDRESS_SETTINGS|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|",159);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"paddress}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for get_address_settings - return paddress\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for get_address_settings - return paddress\033[0m\n");
  }

  // get_address_settings - return saddress from expired name with saddress_list
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_REMOTE_DATA_GET_ADDRESS_SETTINGS|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|",159);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"saddress}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for get_address_settings - return saddress from expired name with saddress_list\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for get_address_settings - return saddress from expired name with saddress_list\033[0m\n");
  }

  // get_address_settings - return paddress from expired name with paddress_list
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_REMOTE_DATA_GET_ADDRESS_SETTINGS|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|",159);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"paddress}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for get_address_settings - return paddress from expired name with paddress_list\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for get_address_settings - return paddress from expired name with paddress_list\033[0m\n");
  }









  // /remotedatagetaddresssettings - return address
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetaddresssettings?parameter1=XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"{\"address_settings\":\"address\"}",sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for /remotedatagetaddresssettings - return address","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for /remotedatagetaddresssettings - return address","red");
  }

  // /remotedatagetaddresssettings - return address due to not finding the address
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetaddresssettings?parameter1=XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"{\"address_settings\":\"address\"}",sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for /remotedatagetaddresssettings - return address due to not finding the address","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for /remotedatagetaddresssettings - return address due to not finding the address","red");
  }

  // /remotedatagetaddresssettings - return saddress
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetaddresssettings?parameter1=XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"{\"address_settings\":\"saddress\"}",sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for /remotedatagetaddresssettings - return saddress","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for /remotedatagetaddresssettings - return saddress","red");
  }

  // /remotedatagetaddresssettings - return paddress
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"\",\"saddress\":\"\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetaddresssettings?parameter1=XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"{\"address_settings\":\"paddress\"}",sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for /remotedatagetaddresssettings - return paddress","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for /remotedatagetaddresssettings - return paddress","red");
  }

  // /remotedatagetaddresssettings - return saddress from expired name with saddress_list
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetaddresssettings?parameter1=XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"{\"address_settings\":\"saddress\"}",sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for /remotedatagetaddresssettings - return saddress from expired name with saddress_list","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for /remotedatagetaddresssettings - return saddress from expired name with saddress_list","red");
  }

  // /remotedatagetaddresssettings - return paddress from expired name with paddress_list
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetaddresssettings?parameter1=XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"{\"address_settings\":\"paddress\"}",sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for /remotedatagetaddresssettings - return paddress from expired name with paddress_list","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for /remotedatagetaddresssettings - return paddress from expired name with paddress_list","red");
  }









  // get_address_from_name - get address
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_GET_ADDRESS_FROM_NAME|" TEST_REMOTE_DATA_ADDRESS_NAME "|",60);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for get_address_from_name - get address\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for get_address_from_name - get address\033[0m\n");
  }

  // get_address_from_name - get saddress
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_GET_ADDRESS_FROM_NAME|" TEST_REMOTE_DATA_PRIVATE_TRANSACTIONS_NAME "|",61);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for get_address_from_name - get saddress\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for get_address_from_name - get saddress\033[0m\n");
  }

  // get_address_from_name - get paddress
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"\",\"saddress\":\"\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_GET_ADDRESS_FROM_NAME|" TEST_REMOTE_DATA_PUBLIC_TRANSACTIONS_NAME "|",61);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for get_address_from_name - get paddress\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for get_address_from_name - get paddress\033[0m\n");
  }

  // get_address_from_name - get website
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"\",\"saddress\":\"\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_GET_ADDRESS_FROM_NAME|TEST.website|",62);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"" TEST_REMOTE_DATA_ADDRESS_NAME "}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for get_address_from_name - get website\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for get_address_from_name - get website\033[0m\n");
  }

  // get_address_from_name - error due to expired
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"1500000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_GET_ADDRESS_FROM_NAME|" TEST_REMOTE_DATA_ADDRESS_NAME "|",60);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Error: Could not get the address settings}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for get_address_from_name - error due to expired\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for get_address_from_name - error due to expired\033[0m\n");
  }

  // get_address_from_name - error due to empty tx hash
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"\"}");
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_GET_ADDRESS_FROM_NAME|" TEST_REMOTE_DATA_ADDRESS_NAME "|",60);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Error: Could not get the address settings}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for get_address_from_name - error due to empty tx hash\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for get_address_from_name - error due to empty tx hash\033[0m\n");
  }









  // /remotedatagetaddressfromname - get address
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetaddressfromname?parameter1=" TEST_REMOTE_DATA_ADDRESS_NAME " HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"{\"remote_data\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\"}",sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for /remotedatagetaddressfromname - get address","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for /remotedatagetaddressfromname - get address","red");
  }

  // /remotedatagetaddressfromname - get saddress
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetaddressfromname?parameter1=" TEST_REMOTE_DATA_PRIVATE_TRANSACTIONS_NAME " HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"{\"remote_data\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\"}",sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for /remotedatagetaddressfromname - get saddress","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for /remotedatagetaddressfromname - get saddress","red");
  }

  // /remotedatagetaddressfromname - get paddress
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"\",\"saddress\":\"\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetaddressfromname?parameter1=" TEST_REMOTE_DATA_PUBLIC_TRANSACTIONS_NAME " HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"{\"remote_data\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\"}",sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for /remotedatagetaddressfromname - get paddress","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for /remotedatagetaddressfromname - get paddress","red");
  }

  // /remotedatagetaddressfromname - get website
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"\",\"saddress\":\"\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetaddressfromname?parameter1=TEST.website HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"{\"remote_data\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\"}",sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for /remotedatagetaddressfromname - get website","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for /remotedatagetaddressfromname - get website","red");
  }

  // /remotedatagetaddressfromname - error due to expired
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"1500000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetaddressfromname?parameter1=" TEST_REMOTE_DATA_ADDRESS_NAME " HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"{\"Error\":\"Could not get the remote_data\"}",sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for /remotedatagetaddressfromname - error due to expired","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for /remotedatagetaddressfromname - error due to expired","red");
  }

  // /remotedatagetaddressfromname - error due to empty tx hash
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetaddressfromname?parameter1=" TEST_REMOTE_DATA_ADDRESS_NAME " HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"{\"Error\":\"Could not get the remote_data\"}",sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for /remotedatagetaddressfromname - error due to empty tx hash","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for /remotedatagetaddressfromname - error due to empty tx hash","red");
  }
  








  // get_information_from_name - get information
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_GET_INFORMATION_FROM_NAME|" TEST_REMOTE_DATA_NAME "|",58);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Information for test:\r\naddress: XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\r\nprivate only address (saddress): XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\r\npublic only address (paddress): XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\r\npublic only address (paddress): XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\r\nwebsite: " TEST_REMOTE_DATA_ADDRESS_NAME "\r\ntimestamp: 10000000000\r\nregister delegate: XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\r\nregister amount: 100\r\ntx_hash: TX_HASH\r\n\r\n",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for get_information_from_name - get information\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for get_information_from_name - get information\033[0m\n");
  }

  // get_information_from_name - error due to not in database
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_GET_INFORMATION_FROM_NAME|test1|",59);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Error: Could not get the information}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for get_information_from_name - error due to not in database\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for get_information_from_name - error due to not in database\033[0m\n");
  }









  // /remotedatagetinformationfromname - get information
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetinformationfromname?parameter1=test HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}",sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for /remotedatagetinformationfromname - get information","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for /remotedatagetinformationfromname - get information","red");
  }

  // /remotedatagetinformationfromname - error due to not in database
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetinformationfromname?parameter1=test1 HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"{\"Error\":\"Could not get the information\"}",sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for /remotedatagetinformationfromname  - error due to not in database","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for /remotedatagetinformationfromname - error due to not in database","red");
  }









  // update_remote_data - update data
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_UPDATE_REMOTE_DATA|website|data|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",249);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Updated the remote data}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for update_remote_data - update data\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for update_remote_data - update data\033[0m\n");
  }

  // update_remote_data - invalid signature
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_UPDATE_REMOTE_DATA|website|data|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",249);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Could not update the remote data}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for update_remote_data - invalid signature\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for update_remote_data - invalid signature\033[0m\n");
  }

  // update_remote_data - invalid item
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_UPDATE_REMOTE_DATA|data|data|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",246);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Could not update the remote data}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for update_remote_data - invalid data\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for update_remote_data - invalid data\033[0m\n");
  }

  // update_remote_data - invalid value
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_UPDATE_REMOTE_DATA|website|00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",355);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Could not update the remote data}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for update_remote_data - invalid value\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for update_remote_data - invalid value\033[0m\n");
  }

  // update_remote_data - expired name
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"1500000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_UPDATE_REMOTE_DATA|website|00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",355);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Could not update the remote data}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for update_remote_data - invalid value\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for update_remote_data - invalid value\033[0m\n");
  }

  // update_remote_data - not in database
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_UPDATE_REMOTE_DATA|website|data|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",249);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Could not update the remote data}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for update_remote_data - not in database\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for update_remote_data - not in database\033[0m\n");
  }









  // remote_data_save_name - save name
  delete_database(database_name);
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_SAVE_NAME|" TEST_REMOTE_DATA_NAME "|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",244);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Saved the name}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for remote_data_save_name - save name\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for remote_data_save_name - save name\033[0m\n");
  }

  // remote_data_save_name - name already in database
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_SAVE_NAME|" TEST_REMOTE_DATA_NAME "|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",244);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Could not save the name}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for remote_data_save_name - name already in database\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for remote_data_save_name - name already in database\033[0m\n");
  }

  // remote_data_save_name - address is already in the database
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"TEST1\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_SAVE_NAME|" TEST_REMOTE_DATA_NAME "|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",244);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Could not save the name}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for remote_data_save_name - address is already in the database\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for remote_data_save_name - address is already in the database\033[0m\n");
  }

  // remote_data_save_name - invalid signature
  delete_database(database_name);
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_SAVE_NAME|" TEST_REMOTE_DATA_NAME "|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",244);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Could not save the name}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for remote_data_save_name - invalid signature\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for remote_data_save_name - invalid signature\033[0m\n");
  }

  // remote_data_save_name - invalid name
  delete_database(database_name);
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_SAVE_NAME|test000000000000000000000000000000000000000000000000000000000000000000000000000000000000000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",331);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Could not save the name}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for remote_data_save_name - invalid name\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for remote_data_save_name - invalid name\033[0m\n");
  }









  // remote_data_purchase_name - purchase name
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"|\",\"paddress_list\":\"|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"\"}");
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_PURCHASE_NAME|XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v|XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv|0000000000000000000000000000000000000000000000000000000000000000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",506);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Purchased the name}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for remote_data_purchase_name - purchase name\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for remote_data_purchase_name - purchase name\033[0m\n");
  }

  // remote_data_purchase_name - saddress already in database
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"paddress\":\"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_PURCHASE_NAME|XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v|XCA1VXWDkYm2u56cbe6b3VajEe4qFLGhVgxeNcgnA3ht1gWvrnZTYr8PJtpkWzagR7c76dDdeyFgoeKZj55W78bp4T8L8n6wLW|0000000000000000000000000000000000000000000000000000000000000000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",506);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Could not purchase the name}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for remote_data_purchase_name - saddress already in database\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for remote_data_purchase_name - saddress already in database\033[0m\n");
  }

  // remote_data_purchase_name - paddress already in database
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"paddress\":\"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_PURCHASE_NAME|XCA1VXWDkYm2u56cbe6b3VajEe4qFLGhVgxeNcgnA3ht1gWvrnZTYr8PJtpkWzagR7c76dDdeyFgoeKZj55W78bp4T8L8n6wLW|XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv|0000000000000000000000000000000000000000000000000000000000000000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",506);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Could not purchase the name}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for remote_data_purchase_name - paddress already in database\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for remote_data_purchase_name - paddress already in database\033[0m\n");
  }

  // remote_data_purchase_name - saddress same as address or paddress
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"paddress\":\"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_PURCHASE_NAME|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|0000000000000000000000000000000000000000000000000000000000000000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",507);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Could not purchase the name}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for remote_data_purchase_name - saddress same as address or paddress\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for remote_data_purchase_name - saddress same as address or paddress\033[0m\n");
  }

  // remote_data_purchase_name - paddress same as address or saddress
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"paddress\":\"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_PURCHASE_NAME|XCA1VXWDkYm2u56cbe6b3VajEe4qFLGhVgxeNcgnA3ht1gWvrnZTYr8PJtpkWzagR7c76dDdeyFgoeKZj55W78bp4T8L8n6wLW|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|0000000000000000000000000000000000000000000000000000000000000000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",507);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Could not purchase the name}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for remote_data_purchase_name - saddress same as address or paddress\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for remote_data_purchase_name - saddress same as address or paddress\033[0m\n");
  }

  // remote_data_purchase_name - tx_hash has already been used
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_PURCHASE_NAME|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|0000000000000000000000000000000000000000000000000000000000000000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",506);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Could not purchase the name}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for remote_data_purchase_name - tx_hash has already been used\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for remote_data_purchase_name - tx_hash has already been used\033[0m\n");
  }

  // remote_data_purchase_name - tx_hash invalid payment to delegate
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_PURCHASE_NAME|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|0000000000000000000000000000000000000000000000000000000000000000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",506);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Could not purchase the name}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for remote_data_purchase_name - tx_hash invalid payment to delegate\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for remote_data_purchase_name - tx_hash invalid payment to delegate\033[0m\n");
  }

  // remote_data_purchase_name - tx_hash invalid amount to delegate
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_PURCHASE_NAME|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|0000000000000000000000000000000000000000000000000000000000000000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",506);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Could not purchase the name}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for remote_data_purchase_name - tx_hash invalid amount to delegate\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for remote_data_purchase_name - tx_hash invalid amount to delegate\033[0m\n");
  }

  // remote_data_purchase_name - tx_hash invalid from address
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_PURCHASE_NAME|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|0000000000000000000000000000000000000000000000000000000000000000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",506);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Could not purchase the name}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for remote_data_purchase_name - tx_hash invalid from address\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for remote_data_purchase_name - tx_hash invalid from address\033[0m\n");
  }









  // /remotedatagetdelegatesinformation
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"amount\":\"100\"}");
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"amount\":\"1000\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetdelegatesinformation HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"[{\"name\":\"delegate\",\"public_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"amount\":\"100\"},{\"name\":\"delegate\",\"public_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"amount\":\"1000\"}]",sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for /remotedatagetdelegatesinformation","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for /remotedatagetdelegatesinformation","red");
  }









  // /remotedatagetblockproducerinformation
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"amount\":\"100\"}");
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"amount\":\"1000\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetblockproducerinformation?parameter1=XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"{\"amount\":\"100\"}",sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for /remotedatagetblockproducerinformation","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for /remotedatagetblockproducerinformation","red");
  }

  // /remotedatagetblockproducerinformation - delegate not found
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"amount\":\"1000\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetblockproducerinformation?parameter1=XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"{\"Error\":\"Could not get the delegates remote data information\"}",sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for /remotedatagetblockproducerinformation - delegate not found","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for /remotedatagetblockproducerinformation - delegate not found","red");
  }









  // delegates_set_amount - set amount
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"amount\":\"100\"}");
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_DELEGATES_SET_AMOUNT|1000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",255);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Set the amount}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for delegates_set_amount - set amount\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for delegates_set_amount - set amount\033[0m\n");
  }

  // delegates_set_amount - invalid signature
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"amount\":\"100\"}");
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_DELEGATES_SET_AMOUNT|1000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",255);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Set the amount}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for delegates_set_amount - invalid signature\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for delegates_set_amount - invalid signature\033[0m\n");
  }

  // delegates_set_amount - invalid amount
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"amount\":\"100\"}");
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_DELEGATES_SET_AMOUNT|TEST|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",255);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Set the amount}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for delegates_set_amount - invalid amount\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for delegates_set_amount  - invalid amount\033[0m\n");
  }

  // delegates_set_amount - not in database
  delete_database(database_name);
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_DELEGATES_SET_AMOUNT|1000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",255);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Set the amount}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for delegates_set_amount - not in database\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for delegates_set_amount - not in database\033[0m\n");
  }









  // renewal_start
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_RENEWAL_START|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",243);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Started to renew the name}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for renewal_start\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for renewal_start\033[0m\n");
  }

  // renewal_start - invalid signature
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_RENEWAL_START|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",243);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Could not start to renew the name}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for renewal_start - invalid signature\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for renewal_start - invalid signature\033[0m\n");
  }

  // renewal_start - not in database
  delete_database(database_name);
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_RENEWAL_START|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",243);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Could not start to renew the name}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for renewal_start - not in database\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for renewal_start - not in database\033[0m\n");
  }









  // renewal_end
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_RENEWAL_END|0000000000000000000000000000000000000000000000000000000000000000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",243);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Renewed the name}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for renewal_end\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for renewal_end\033[0m\n");
  }

  // renewal_end - invalid signature
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_RENEWAL_END|0000000000000000000000000000000000000000000000000000000000000000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",243);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Could not renew the name}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for renewal_end - invalid signature\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for renewal_end - invalid signature\033[0m\n");
  }

  // renewal_end - invalid tx_hash
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_RENEWAL_END|0000000000000000000000000000000000000000000000000000000000000000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",243);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Could not renew the name}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for renewal_end - invalid tx_hash\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for renewal_end - invalid tx_hash\033[0m\n");
  }

  // renewal_end - not in database
  delete_database(database_name);
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_RENEWAL_END|0000000000000000000000000000000000000000000000000000000000000000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",243);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"Could not renew the name}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for renewal_end - not in database\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for renewal_end - not in database\033[0m\n");
  }









  // add_delegates_to_remote_data_delegates
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_1);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_20);
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"amount\":\"1000\"}");
  memset(data_test,0,sizeof(data_test));
  add_delegates_to_remote_data_delegates();
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetdelegatesinformation HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"[{\"name\":\"delegate\",\"public_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"amount\":\"0\"},{\"name\":\"delegate\",\"public_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"amount\":\"1000\"}]",sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for add_delegates_to_remote_data_delegates","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for add_delegates_to_remote_data_delegates","red");
  }









  // maintain_slist_and_plist
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"TEST\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"TEST1\",\"address\":\"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv\",\"saddress\":\"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv\",\"paddress\":\"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv\",\"saddress_list\":\"\",\"paddress_list\":\"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"TEST2\",\"address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"saddress\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"paddress\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"saddress_list\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v|\",\"paddress_list\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"TEST3\",\"address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"saddress\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"paddress\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"saddress_list\":\"\",\"paddress_list\":\"\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  maintain_slist_and_plist();
  memset(data_test,0,sizeof(data_test));
  memset(result_test,0,sizeof(result_test));
  memcpy(result_test,"{\"name\":\"TEST\"}",15);
  if (read_document_field_from_collection(database_name,"remote_data",result_test,"saddress_list",data_test) == 1 && strncmp(data_test,"",BUFFER_SIZE) == 0)
  {
    memset(data_test,0,sizeof(data_test));
    memset(result_test,0,sizeof(result_test));
    memcpy(result_test,"{\"name\":\"TEST1\"}",16);
    if (read_document_field_from_collection(database_name,"remote_data",result_test,"paddress_list",data_test) == 1 && strncmp(data_test,"",BUFFER_SIZE) == 0)
    {
      memset(data_test,0,sizeof(data_test));
      memset(result_test,0,sizeof(result_test));
      memcpy(result_test,"{\"name\":\"TEST2\"}",16);
      if (read_document_field_from_collection(database_name,"remote_data",result_test,"saddress_list",data_test) == 1 && strncmp(data_test,"",BUFFER_SIZE) == 0 && read_document_field_from_collection(database_name,"remote_data",result_test,"paddress_list",data_test) == 1 && strncmp(data_test,"",BUFFER_SIZE) == 0)
      {
        memset(data_test,0,sizeof(data_test));
        memset(result_test,0,sizeof(result_test));
        memcpy(result_test,"{\"name\":\"TEST3\"}",16);
        if (read_document_field_from_collection(database_name,"remote_data",result_test,"saddress_list",data_test) == 1 && strncmp(data_test,"",BUFFER_SIZE) == 0 && read_document_field_from_collection(database_name,"remote_data",result_test,"paddress_list",data_test) == 1 && strncmp(data_test,"",BUFFER_SIZE) == 0)
        {
          color_print("PASSED! Test for maintain_slist_and_plist","green");
          count_test++;
        }
        else
        {
          color_print("FAILED! Test for maintain_slist_and_plist","red");
        }
      }
      else
      {
        color_print("FAILED! Test for maintain_slist_and_plist","red");
      }
    }
    else
    {
      color_print("FAILED! Test for maintain_slist_and_plist","red");
    }
  }
  else
  {
    color_print("FAILED! Test for maintain_slist_and_plist","red");
  }









  // check_for_expired_names
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"TEST\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"1500000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"TEST1\",\"address\":\"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv\",\"saddress\":\"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv\",\"paddress\":\"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv\",\"saddress_list\":\"\",\"paddress_list\":\"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"1500000000\",\"reserve_delegate_address\":\"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"TEST2\",\"address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"saddress\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"paddress\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"saddress_list\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v|\",\"paddress_list\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  check_for_expired_names();
  memset(result_test,0,sizeof(result_test));
  memcpy(result_test,"{\"name\":\"TEST\"}",15);
  if (count_documents_in_collection(database_name,"remote_data",result_test) == 0)
  {
    memset(data_test,0,sizeof(data_test));
    memset(result_test,0,sizeof(result_test));
    memcpy(result_test,"{\"name\":\"TEST1\"}",16);
    if (read_document_field_from_collection(database_name,"remote_data",result_test,"paddress_list",data_test) == 1 && strncmp(data_test,"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv|XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv|",BUFFER_SIZE) == 0 && read_document_field_from_collection(database_name,"remote_data",result_test,"saddress_list",data_test) == 1 && strncmp(data_test,"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv|",BUFFER_SIZE) == 0)
    {
      memset(data_test,0,sizeof(data_test));
      memset(result_test,0,sizeof(result_test));
      memcpy(result_test,"{\"name\":\"TEST2\"}",16);
      if (read_document_field_from_collection(database_name,"remote_data",result_test,"address",data_test) == 1 && strncmp(data_test,"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v",BUFFER_SIZE) == 0)
      {
        color_print("PASSED! Test for check_for_expired_names","green");
        count_test++;
      }
      else
      {
        color_print("FAILED! Test for check_for_expired_names","red");
      }
    }
    else
    {
      color_print("FAILED! Test for check_for_expired_names","red");
    }
  }
  else
  {
    color_print("FAILED! Test for check_for_expired_names","red");
  }









  // sync_check_functions_test
  network_functions_test_error_settings = 2;
  delete_database(database_name);
  RESET_ERROR_MESSAGES;
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"amount\":\"1000\"}");
  INITIALIZE_NETWORK_DATA_NODES_TEST;

  if (sync_check_remote_data_database(1) == 1)
  {
    color_print("PASSED! Test for sync_check_remote_data_database from a random block verifier","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sync_check_remote_data_database from a random block verifier","red");
  }
  if (sync_check_remote_data_database(3) == 1)
  {
    color_print("PASSED! Test for sync_check_remote_data_database from a random network data node","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sync_check_remote_data_database from a random network data node","red");
  }
  if (sync_check_majority_remote_data_database() == 1)
  {
    color_print("PASSED! Test for sync_check_majority_remote_data_database","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sync_check_majority_remote_data_database","red");
  }

  if (sync_check_remote_data_delegates_database(1) == 1)
  {
    color_print("PASSED! Test for sync_check_remote_data_delegates_database from a random block verifier","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sync_check_remote_data_delegates_database from a random block verifier","red");
  }
  if (sync_check_remote_data_delegates_database(3) == 1)
  {
    color_print("PASSED! Test for sync_check_remote_data_delegates_database from a random network data node","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sync_check_remote_data_delegates_database from a random network data node","red");
  }
  if (sync_check_majority_remote_data_delegates_database() == 1)
  {
    color_print("PASSED! Test for sync_check_majority_remote_data_delegates_database","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sync_check_majority_remote_data_delegates_database","red");
  }

  for (count = 0; count < NETWORK_DATA_NODES_AMOUNT; count++)
  {
    memset(network_data_nodes_list.network_data_nodes_public_address[count],0,sizeof(network_data_nodes_list.network_data_nodes_public_address[count]));
    memset(network_data_nodes_list.network_data_nodes_IP_address[count],0,sizeof(network_data_nodes_list.network_data_nodes_IP_address[count]));
  }
  INITIALIZE_NETWORK_DATA_NODES;









  // sync_server_functions_test
  delete_database(database_name);
  RESET_ERROR_MESSAGES;
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"amount\":\"1000\"}");
  // initialize the previous, current and next block verifiers list
  INITIALIZE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_TEST;

  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  RESET_ERROR_MESSAGES;
  memcpy(data_test,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"data_hash\": \"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000fde35120678435cf108dcc6da942ecf2\",\r\n}",250);
  if (sign_data(data_test) == 1 && send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,data_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && strncmp(result_test,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"remote_data_database\": \"true\"",135) == 0)
  {
    color_print("PASSED! Test for server_receive_data_socket_block_verifiers_to_block_verifiers_remote_data_database_sync_check_update","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for server_receive_data_socket_block_verifiers_to_block_verifiers_remote_data_database_sync_check_update","red");
  }
  RESET_ERROR_MESSAGES;

  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  delete_database(database_name);
  RESET_ERROR_MESSAGES;
  memcpy(data_test,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n}",106);
  if (sign_data(data_test) == 1 && send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,data_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && strncmp(result_test,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"remote_data_database\": \"{\"name\" : \"" TEST_REMOTE_DATA_NAME "\", \"address\" : \"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\", \"saddress\" : \"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\", \"paddress\" : \"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\", \"saddress_list\" : \"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\", \"paddress_list\" : \"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\", \"website\" : \"TEST_REMOTE_DATA_ADDRESS_NAME\", \"smart_contract_hash\" : \"\", \"timestamp\" : \"10000000000\", \"reserve_delegate_address\" : \"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\", \"reserve_delegate_amount\" : \"100\", \"tx_hash\" : \"TX_HASH\" }\"",1025) == 0)
  {
    color_print("PASSED! Test for server_receive_data_socket_block_verifiers_to_block_verifiers_remote_data_database_download_file_update","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for server_receive_data_socket_block_verifiers_to_block_verifiers_remote_data_database_download_file_update","red");
  }
  RESET_ERROR_MESSAGES;

  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  RESET_ERROR_MESSAGES;
  memcpy(data_test,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DELEGATES_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"data_hash\": \"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000fde35120678435cf108dcc6da942ecf2\",\r\n}",250);
  if (sign_data(data_test) == 1 && send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,data_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && strncmp(result_test,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"remote_data_database\": \"true\"",135) == 0)
  {
    color_print("PASSED! Test for server_receive_data_socket_block_verifiers_to_block_verifiers_remote_data_delegates_database_sync_check_update","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for server_receive_data_socket_block_verifiers_to_block_verifiers_remote_data_delegates_database_sync_check_update","red");
  }
  RESET_ERROR_MESSAGES;

  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  delete_database(database_name);
  RESET_ERROR_MESSAGES;
  memcpy(data_test,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n}",116);
  if (sign_data(data_test) == 1 && send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,data_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && strncmp(result_test,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"remote_data_delegates_database\": \"{\"name\" : \"delegate\", \"public_address\" : \"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\", \"amount\" : \"1000\" }\"",316) == 0)
  {
    color_print("PASSED! Test for server_receive_data_socket_block_verifiers_to_block_verifiers_remote_data_delegates_database_download_file_update","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for server_receive_data_socket_block_verifiers_to_block_verifiers_remote_data_delegates_database_download_file_update","red");
  }
  RESET_ERROR_MESSAGES;









  // sync_server_functions_test
  delete_database(database_name);
  RESET_ERROR_MESSAGES;
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"amount\":\"1000\"}");
  
  // sync_remote_data_database
  if (sync_remote_data_database(0,XCASH_DPOPS_delegates_IP_address) == 1)
  {
    color_print("PASSED! Test for sync_remote_data_database","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sync_remote_data_database","red");
  }
  if (sync_remote_data_delegates_database(0,XCASH_DPOPS_delegates_IP_address) == 1)
  {
    color_print("PASSED! Test for sync_remote_data_delegates_database","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sync_remote_data_delegates_database","red");
  }

  
  // write the end test message
  fprintf(stderr,"\033[1;33m\n\n%s\nremote data functions test - Passed test: %d, Failed test: %d\n%s\n\n\n\033[0m",TEST_OUTLINE,count_test,REMOTE_DATA_FUNCTIONS_TOTAL_TEST-count_test,TEST_OUTLINE);
  return count_test;
}
