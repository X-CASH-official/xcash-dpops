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

  // define macros
  #define TEST_RESERVE_PROOF_DELEGATES_REGISTER "ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti" // the reserve proof used for testing the delegate regster function
  #define TEST_WALLET_DELEGATES_REGISTER "XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP" // the wallet used for testing the delegate regster function
  #define TEST_SIGNATURE_ADD_RESERVE_PROOF "SigV1N2NWu1Yzg4aVrhJUjZVcRZ52CcgUghX7x5Fn9W5drC4g8B9tZn7q4yJEKbiJMRFeNRPmSPT3rBSVkJnFWAGK5PA3" // the signature used for testing the delegate regster function
  #define TEST_SIGNATURE_ADD_RESERVE_PROOF_INVALID_DATA "SigV1TKQC5vgf5WHDdovhL61tSo1AceDZvYZLziZTdXF4RNbwBoaxhNSYKAjCjFUmds2awbXpo6neuT7TS4eRVF1yD7FV" // the signature used for testing the delegate regster function for Invalid data
  #define TEST_SIGNATURE_ADD_RESERVE_PROOF_INVALID_RESERVE_PROOF "SigV1C2USs6c2BpuD5zMz4zbx7VCgYYJ1QWcjcL6gFsTqA2pKZg7StgCUdx8d4RFXsxhuXaMdqUNrfK52zHrzseWye8Zc" // the signature used for testing the delegate regster function for Invalid reserve proof
  #define TEST_SIGNATURE_DELEGATES_REGISTER "SigV11Bg1s6XgfB4aT8zk5SRHrER6AfnZBwspW6j3GVYGZtETAJD8wCpaQyp2o5rizXvcWzJe3mJs6ZXpRWfMEVfc1dCA" // the signature used for testing the delegate regster function
  #define TEST_SIGNATURE_DELEGATES_REGISTER_INVALID_DATA "SigV13GLt56kfHutU2rTpGkKstj6pikWqgA7XHjAYt7ghXcoebkx9PZmGNvYgfX4XboMebX4NcL5JFPZrPWc22oyGxbtm" // the signature used for testing the delegate regster function for Invalid data
  #define TEST_SIGNATURE_DELEGATES_REGISTER_PUBLIC_ADDRESS_ALREADY_REGISTERED "SigV1g8M1JKBjBDWjM4JLwhXyQjNfNR9D62GH4SdRVLfqNKyDfjyNeuRaVmdTp31FgJyBXaQ76tA8kMwJTfaj9usFPvcT" // the signature used for testing the delegate regster function for The delegates public address is already registered
  #define TEST_SIGNATURE_DELEGATES_REGISTER_IP_ADDRESS_ALREADY_REGISTERED "SigV1GDgpbSWb9E5iaLVX4Uo2KtahPqsQ1Vza5VDSG8dyUKv3EZgGLZQDABoiA6V9sNZkemRGwLpXtrSAYaKNw4iJc72C" // the signature used for testing the delegate regster function for The delegates IP address is already registered
  #define TEST_SIGNATURE_DELEGATES_REGISTER_PUBLIC_KEY_ALREADY_REGISTERED "SigV12BycAnvf3b7iRgJ5wfuSUzZxbREV6qHWbNaDA7yCGwsHJWBEfYwgNhdFx1MbTmb1QwZC7fPx38jAecU2oJTWE6zG" // the signature used for testing the delegate regster function for The delegates public key is already registered
  #define TEST_SIGNATURE_DELEGATES_REGISTER_DELEGATES_NAME_ALREADY_REGISTERED "SigV1PjXNojdeT9FCJoPtUCcUWtMVNWzg5KJwCN8tyFDBQowXAW3zd2Kb5ZM8191H2p5NRcK2do9sEPjUUUstiYg3BYUx" // the signature used for testing the delegate regster function for The delegates name is already registered
  #define TEST_SIGNATURE_DELEGATES_UPDATE "SigV1GgWgHi6kk6hYJHfpzUWvBkYf21PPX1ZEdi8xU4ufFuQvPnmzVAFV5EMYybZQAvfGYQDHtSiMRgRqnHVaryMzxTho" // the signature used for testing the delegate update function
  #define TEST_SIGNATURE_DELEGATES_UPDATE_INVALID_ITEM "SigV1HT4BXSPaftj5o26by2evyN9RLvT1MyMme1ni4E8zuA9b4SVKeuFkcakYcycdEfH5SpKg3CG3p3gg1QgRsCAnQWPw" // the signature used for testing the delegate update function for Invalid item to update
  #define TEST_SIGNATURE_DELEGATES_UPDATE_INVALID_ITEM_VALUE "SigV1gM13gP3p6pmdRcM5mumi8C2A69LN4oM46PhLdQLd8o3bg6Yu3xEoXkX8uv71558fZRYAzRwUr3YbAGRgew24mya4" // the signature used for testing the delegate update function for Invalid item value to update
  #define TEST_SIGNATURE_DELEGATES_REMOVE "SigV1c34L3pqB73kJyWv4qNkz3VGS2SU8kE9dRdDXToAQSGVQDhBj93ZR9YLgn8g1WkDLh8G6EoWTgPbq6ChdYptqZjP9" // the signature used for testing the delegate remove function
 
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"test\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"test\",\"address\":\"\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"test\",\"address\":\"\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"test\",\"address\":\"\",\"saddress\":\"\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"test\",\"address\":\"\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"test\",\"address\":\"\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"test\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"test\",\"address\":\"\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"test\",\"address\":\"\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"test\",\"address\":\"\",\"saddress\":\"\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"test\",\"address\":\"\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"test\",\"address\":\"\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"test\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_GET_ADDRESS_FROM_NAME|test.xcash|",60);
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"test\",\"address\":\"\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_GET_ADDRESS_FROM_NAME|test.sxcash|",61);
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"test\",\"address\":\"\",\"saddress\":\"\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_GET_ADDRESS_FROM_NAME|test.pxcash|",61);
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"test\",\"address\":\"\",\"saddress\":\"\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"test.xcash\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_GET_ADDRESS_FROM_NAME|test.website|",62);
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  sleep(5);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (strncmp(result_test,"test.xcash}",BUFFER_SIZE) == 0)
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"test\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"1500000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_GET_ADDRESS_FROM_NAME|test.xcash|",60);
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"test\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"\"}");
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_GET_ADDRESS_FROM_NAME|test.xcash|",60);
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"test\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetaddressfromname?parameter1=test.xcash HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"{\"remote_data\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\"}",sizeof(data_test)) == 0)
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"test\",\"address\":\"\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetaddressfromname?parameter1=test.sxcash HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"{\"remote_data\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\"}",sizeof(data_test)) == 0)
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"test\",\"address\":\"\",\"saddress\":\"\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetaddressfromname?parameter1=test.pxcash HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"{\"remote_data\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\"}",sizeof(data_test)) == 0)
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"test\",\"address\":\"\",\"saddress\":\"\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"test.xcash\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetaddressfromname?parameter1=test.website HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"{\"remote_data\":\"test.xcash\"}",sizeof(data_test)) == 0)
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"test\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"1500000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"TX_HASH\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetaddressfromname?parameter1=test.xcash HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"{\"Error\":\"Could not get the remote_data\"}",sizeof(data_test)) == 0)
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"test\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetaddressfromname?parameter1=test.xcash HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"{\"Error\":\"Could not get the remote_data\"}",sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for /remotedatagetaddressfromname - error due to empty tx hash","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for /remotedatagetaddressfromname - error due to empty tx hash","red");
  }

  // write the end test message
  fprintf(stderr,"\033[1;33m\n\n%s\nremote data functions test - Passed test: %d, Failed test: %d\n%s\n\n\n\033[0m",TEST_OUTLINE,count_test,REMOTE_DATA_FUNCTIONS_TOTAL_TEST-count_test,TEST_OUTLINE);
  return count_test;
  
  #undef TEST_RESERVE_PROOF_DELEGATES_REGISTER
  #undef TEST_WALLET_DELEGATES_REGISTER
  #undef TEST_SIGNATURE_ADD_RESERVE_PROOF
  #undef TEST_SIGNATURE_ADD_RESERVE_PROOF_INVALID_DATA
  #undef TEST_SIGNATURE_ADD_RESERVE_PROOF_INVALID_RESERVE_PROOF
  #undef TEST_SIGNATURE_DELEGATES_REGISTER
  #undef TEST_SIGNATURE_DELEGATES_REGISTER_INVALID_DATA
  #undef TEST_SIGNATURE_DELEGATES_REGISTER_PUBLIC_ADDRESS_ALREADY_REGISTERED
  #undef TEST_SIGNATURE_DELEGATES_REGISTER_IP_ADDRESS_ALREADY_REGISTERED
  #undef TEST_SIGNATURE_DELEGATES_REGISTER_PUBLIC_KEY_ALREADY_REGISTERED
  #undef TEST_SIGNATURE_DELEGATES_REGISTER_DELEGATES_NAME_ALREADY_REGISTERED
  #undef TEST_SIGNATURE_DELEGATES_UPDATE
  #undef TEST_SIGNATURE_DELEGATES_UPDATE_INVALID_ITEM
  #undef TEST_SIGNATURE_DELEGATES_UPDATE_INVALID_ITEM_VALUE
  #undef TEST_SIGNATURE_DELEGATES_REMOVE
  #undef DELEGATES_SERVER_FUNCTIONS_TEST_ERROR
}
