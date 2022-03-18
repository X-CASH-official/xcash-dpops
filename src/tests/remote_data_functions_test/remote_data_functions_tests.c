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
  memset(current_block_height,0,sizeof(current_block_height));
  memcpy(current_block_height,"1000000000",10);
 
  // write the start test message
  fprintf(stderr,"\033[1;34m%s\nremote data functions test - Total test: %d\n%s\n\n\033[0m",TEST_OUTLINE,REMOTE_DATA_FUNCTIONS_TOTAL_TEST,TEST_OUTLINE);
 
  // run the test
  delegates_website = 1;
  network_functions_test_error_settings = 2;

  // validate_amount - valid amount
  if (validate_amount("1000") == 1)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for validate_amount - valid amount\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for validate_amount - valid amount\033[0m\n");
  }

  // validate_amount - invalid amount
  if (validate_amount("TEST") == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for validate_amount - invalid amount\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for validate_amount - invalid amount\033[0m\n");
  }









  // validate_name - valid name
  if (validate_name("TEST") == 1)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for validate_name - valid name\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for validate_name - valid name\033[0m\n");
  }

  // validate_name - invalid name
  if (validate_name("test") == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for validate_name - invalid name\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for validate_name - invalid name\033[0m\n");
  }









  // validate_name_settings - valid name
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  if (validate_name_settings(TEST_REMOTE_DATA_NAME) == 1 && validate_name_settings(TEST_REMOTE_DATA_ADDRESS_NAME) == 1 && validate_name_settings(TEST_REMOTE_DATA_PRIVATE_TRANSACTIONS_NAME) == 1 && validate_name_settings(TEST_REMOTE_DATA_PUBLIC_TRANSACTIONS_NAME) == 1)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for validate_name_settings - valid name\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for validate_name_settings - valid name\033[0m\n");
  }

  // validate_name_settings - invalid name
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  if (validate_name_settings("test") == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for validate_name_settings - invalid name\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for validate_name_settings - invalid name\033[0m\n");
  }









  // document_field_contains_string_from_collection
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  if (document_field_contains_string_from_collection(database_name,"remote_data","address","XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP") == 1 && document_field_contains_string_from_collection(database_name,"remote_data","reserve_delegate_address","XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP") == 0 && document_field_contains_string_from_collection(database_name,"remote_data","saddress","XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP") == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for document_field_contains_string_from_collection\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for document_field_contains_string_from_collection\033[0m\n");
  }









  // remote_data_update_delegates_statistics
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"" TEST_WALLET "\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  memset(data_test,0,sizeof(data_test));
  memset(result_test,0,sizeof(result_test));
  if (remote_data_update_delegates_statistics("delegate","100") == 1 && read_document_field_from_collection(database_name,"remote_data_delegates","{\"name\":\"delegate\"}","total_registered_renewed_amount",data_test) == 1 && strncmp(data_test,"1",BUFFER_SIZE) == 0 && read_document_field_from_collection(database_name,"remote_data_delegates","{\"name\":\"delegate\"}","total_amount",result_test) == 1 && strncmp(result_test,"100",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for remote_data_update_delegates_statistics\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for remote_data_update_delegates_statistics\033[0m\n");
  }









  // remote_data_validate_tx_hash - valid tx_hash
  delete_database(database_name);
  if (remote_data_validate_tx_hash("a508629c4aacdbf74996de9c865bf98a026a38589a385ebf5a04ab7e0da44376","XCA1kzoR3ZLNg5zxNmxrY8FYKtgEvPZqC2xoRpm1axCpQcrrZfoKTSkSNsASDspdt3j1WcEnQJyuuB5VPSB56WWy36A4sQtQhe","XCA1cH8Qs5hLYnzQTDuJqkJiQEZbgQsUM3BgA6vBod5T5Eindas5sikKJaLbkhM3YBW7PtoJY6BtNLkZuahksLFX5eSPDcmCLL","100") == 1)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for remote_data_validate_tx_hash - valid tx_hash\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for remote_data_validate_tx_hash - valid tx_hash\033[0m\n");
  }

  // remote_data_validate_tx_hash - tx_hash is already in the database
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"a508629c4aacdbf74996de9c865bf98a026a38589a385ebf5a04ab7e0da44376\"}");
  if (remote_data_validate_tx_hash("a508629c4aacdbf74996de9c865bf98a026a38589a385ebf5a04ab7e0da44376","XCA1kzoR3ZLNg5zxNmxrY8FYKtgEvPZqC2xoRpm1axCpQcrrZfoKTSkSNsASDspdt3j1WcEnQJyuuB5VPSB56WWy36A4sQtQhe","XCA1cH8Qs5hLYnzQTDuJqkJiQEZbgQsUM3BgA6vBod5T5Eindas5sikKJaLbkhM3YBW7PtoJY6BtNLkZuahksLFX5eSPDcmCLL","100") == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for remote_data_validate_tx_hash - tx_hash is already in the database\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for remote_data_validate_tx_hash - tx_hash is already in the database\033[0m\n");
  }
  delete_database(database_name);

  // remote_data_validate_tx_hash - invalid public transaction
  if (remote_data_validate_tx_hash("0000000000000000000000000000000000000000000000000000000000000000","XCA1kzoR3ZLNg5zxNmxrY8FYKtgEvPZqC2xoRpm1axCpQcrrZfoKTSkSNsASDspdt3j1WcEnQJyuuB5VPSB56WWy36A4sQtQhe","XCA1cH8Qs5hLYnzQTDuJqkJiQEZbgQsUM3BgA6vBod5T5Eindas5sikKJaLbkhM3YBW7PtoJY6BtNLkZuahksLFX5eSPDcmCLL","100") == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for remote_data_validate_tx_hash - invalid public transaction\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for remote_data_validate_tx_hash - invalid public transaction\033[0m\n");
  }

  // remote_data_validate_tx_hash - invalid sender
  if (remote_data_validate_tx_hash("a508629c4aacdbf74996de9c865bf98a026a38589a385ebf5a04ab7e0da44376","XCA1cH8Qs5hLYnzQTDuJqkJiQEZbgQsUM3BgA6vBod5T5Eindas5sikKJaLbkhM3YBW7PtoJY6BtNLkZuahksLFX5eSPDcmCLL","XCA1cH8Qs5hLYnzQTDuJqkJiQEZbgQsUM3BgA6vBod5T5Eindas5sikKJaLbkhM3YBW7PtoJY6BtNLkZuahksLFX5eSPDcmCLL","100") == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for remote_data_validate_tx_hash - invalid sender\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for remote_data_validate_tx_hash - invalid sender\033[0m\n");
  }

  // remote_data_validate_tx_hash - invalid receiver
  if (remote_data_validate_tx_hash("a508629c4aacdbf74996de9c865bf98a026a38589a385ebf5a04ab7e0da44376","XCA1kzoR3ZLNg5zxNmxrY8FYKtgEvPZqC2xoRpm1axCpQcrrZfoKTSkSNsASDspdt3j1WcEnQJyuuB5VPSB56WWy36A4sQtQhe","XCA1kzoR3ZLNg5zxNmxrY8FYKtgEvPZqC2xoRpm1axCpQcrrZfoKTSkSNsASDspdt3j1WcEnQJyuuB5VPSB56WWy36A4sQtQhe","100") == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for remote_data_validate_tx_hash - invalid receiver\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for remote_data_validate_tx_hash - invalid receiver\033[0m\n");
  }

  // remote_data_validate_tx_hash - invalid receiver amount
  if (remote_data_validate_tx_hash("a508629c4aacdbf74996de9c865bf98a026a38589a385ebf5a04ab7e0da44376","XCA1kzoR3ZLNg5zxNmxrY8FYKtgEvPZqC2xoRpm1axCpQcrrZfoKTSkSNsASDspdt3j1WcEnQJyuuB5VPSB56WWy36A4sQtQhe","XCA1cH8Qs5hLYnzQTDuJqkJiQEZbgQsUM3BgA6vBod5T5Eindas5sikKJaLbkhM3YBW7PtoJY6BtNLkZuahksLFX5eSPDcmCLL","1000") == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for remote_data_validate_tx_hash - invalid receiver amount\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for remote_data_validate_tx_hash - invalid receiver amount\033[0m\n");
  }











  // get_address_settings - return address
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_REMOTE_DATA_GET_ADDRESS_SETTINGS|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|",159);
  send_and_receive_data_socket(data_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS); 
  if (strncmp(data_test,"address}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for get_address_settings - return address\033[0m\n");
    count_test++;
  }
  else
  {
    color_print(data_test,"yellow");
    fprintf(stderr,"\033[1;31mFAILED! Test for get_address_settings - return address\033[0m\n");
  }

  // get_address_settings - return address due to not finding the address
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_REMOTE_DATA_GET_ADDRESS_SETTINGS|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|",159);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_REMOTE_DATA_GET_ADDRESS_SETTINGS|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|",159);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"saddress\":\"\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_REMOTE_DATA_GET_ADDRESS_SETTINGS|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|",159);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_REMOTE_DATA_GET_ADDRESS_SETTINGS|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|",159);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_REMOTE_DATA_GET_ADDRESS_SETTINGS|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|",159);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  if (strncmp(result_test,"paddress}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for get_address_settings - return paddress from expired name with paddress_list\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for get_address_settings - return paddress from expired name with paddress_list\033[0m\n");
  }









  // /remotedatagetstatistics
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1v18Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1v10Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"5\",\"total_amount\":\"100\"}");
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1v15Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"10\",\"total_amount\":\"15\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetstatistics HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"{\"total_registered_renewed_amount\":\"15\",\"total_amount\":\"115\"}",sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for /remotedatagetstatistics","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for /remotedatagetstatistics","red");
  }









  // /remotedatagetnamestatus - can register
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"TEST1\",\"address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetnamestatus?parameter1=" TEST_REMOTE_DATA_NAME " HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"{\"status\":\"can register\"}",sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for /remotedatagetnamestatus - can register","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for /remotedatagetnamestatus - can register","red");
  }

  // /remotedatagetnamestatus - can renew
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"" REMOTE_DATA_TIMESTAMP_DEFAULT_AMOUNT "\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetnamestatus?parameter1=" TEST_REMOTE_DATA_NAME " HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"{\"status\":\"can renew\"}",sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for /remotedatagetnamestatus - can renew","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for /remotedatagetnamestatus - can renew","red");
  }

  // /remotedatagetnamestatus - can not register or renew
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetnamestatus?parameter1=" TEST_REMOTE_DATA_NAME " HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"{\"status\":\"can not register or renew\"}",sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for /remotedatagetnamestatus - can not register or renew","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for /remotedatagetnamestatus - can not register or renew","red");
  }









  // /remotedatagetaddresssettings - return address
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"saddress\":\"\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_REMOTE_DATA_GET_ADDRESS_FROM_NAME|" TEST_REMOTE_DATA_ADDRESS_NAME "|",72);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  if (strncmp(result_test,"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for get_address_from_name - get address\033[0m\n");
    count_test++;
  }
  else
  {
    color_print(result_test,"yellow");
    fprintf(stderr,"\033[1;31mFAILED! Test for get_address_from_name - get address\033[0m\n");
  }

  // get_address_from_name - get saddress
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_REMOTE_DATA_GET_ADDRESS_FROM_NAME|" TEST_REMOTE_DATA_PRIVATE_TRANSACTIONS_NAME "|",73);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"saddress\":\"\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_REMOTE_DATA_GET_ADDRESS_FROM_NAME|" TEST_REMOTE_DATA_PUBLIC_TRANSACTIONS_NAME "|",73);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"saddress\":\"\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_REMOTE_DATA_GET_ADDRESS_FROM_NAME|TEST.website|",74);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"1500000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_REMOTE_DATA_GET_ADDRESS_FROM_NAME|" TEST_REMOTE_DATA_ADDRESS_NAME "|",72);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_REMOTE_DATA_GET_ADDRESS_FROM_NAME|" TEST_REMOTE_DATA_ADDRESS_NAME "|",72);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"saddress\":\"\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"saddress\":\"\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"1500000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_REMOTE_DATA_GET_INFORMATION_FROM_NAME|" TEST_REMOTE_DATA_NAME "|",70);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  if (strncmp(result_test,"Information for TEST:|address: XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|private only address (saddress): XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|public only address (paddress): XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|website: TEST.xcash|timestamp: 10000000000|register delegate: XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|register amount: 100|tx_hash: 0000000000000000000000000000000000000000000000000000000000000000|}",BUFFER_SIZE) == 0)
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODE_TO_NETWORK_DATA_NODES_REMOTE_DATA_GET_INFORMATION_FROM_NAME|test1|",71);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetinformationfromname?parameter1=" TEST_REMOTE_DATA_NAME " HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}",sizeof(data_test)) == 0)
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetinformationfromname?parameter1=TEST1 HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"{\"Error\":\"Could not get the information\"}",sizeof(data_test)) == 0)
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_UPDATE_REMOTE_DATA|website|data|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV1RhFQcbvqab79pv2ir2fPu6i2gJvvwVHgcCb8a7JrXagkPaRYNJgrNxyUKLWfSkDx3S57JRmZRQuqSgh1bdLnzKNk|",250);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_UPDATE_REMOTE_DATA|website|data|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000|",250);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_UPDATE_REMOTE_DATA|data|data|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV14JDPHFZvSSBhiQH8ngqNyUPN9YMhnQyfnVJGa3CiTNGoa8xxTaF5cFoHhTxeJpB7Y48qVjyuCv9kPQJVT4MWDFmc|",247);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_UPDATE_REMOTE_DATA|website|00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV1gxJt2R31LkXJP7Z7Xm8a7ZijESsoyL2TrhB96xDCnB624dGxFiQ7RLjKKrwtLndyem7HTjmNawLoeQoGkYctFNGh|",356);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"1500000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_UPDATE_REMOTE_DATA|website|data|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV1RhFQcbvqab79pv2ir2fPu6i2gJvvwVHgcCb8a7JrXagkPaRYNJgrNxyUKLWfSkDx3S57JRmZRQuqSgh1bdLnzKNk|",250);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_UPDATE_REMOTE_DATA|website|data|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV1RhFQcbvqab79pv2ir2fPu6i2gJvvwVHgcCb8a7JrXagkPaRYNJgrNxyUKLWfSkDx3S57JRmZRQuqSgh1bdLnzKNk|",250);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"" TEST_WALLET "\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_SAVE_NAME|" TEST_REMOTE_DATA_NAME "|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV11cjRtG1krpsjHW5bgp9wZ6ZpeToieQ91HDHK3QTAX4drZXXzg8yzrMR4NUbTb5w8XpZwC8Ly9WcWVN8er8AkR4rm|",245);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  if (strncmp(result_test,"Saved the name}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for remote_data_save_name - save name\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for remote_data_save_name - save name\033[0m\n");
  }

  // remote_data_save_name - name already in database and cant renew
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"" TEST_WALLET "\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_SAVE_NAME|" TEST_REMOTE_DATA_NAME "|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV11cjRtG1krpsjHW5bgp9wZ6ZpeToieQ91HDHK3QTAX4drZXXzg8yzrMR4NUbTb5w8XpZwC8Ly9WcWVN8er8AkR4rm|",245);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  if (strncmp(result_test,"Could not save the name}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for remote_data_save_name - name already in database and cant renew\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for remote_data_save_name - name already in database and cant renew\033[0m\n");
  }

  // remote_data_save_name - name already in database but can renew
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"" REMOTE_DATA_TIMESTAMP_DEFAULT_AMOUNT "\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"\"}");
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"" TEST_WALLET "\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_SAVE_NAME|" TEST_REMOTE_DATA_NAME "|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV11cjRtG1krpsjHW5bgp9wZ6ZpeToieQ91HDHK3QTAX4drZXXzg8yzrMR4NUbTb5w8XpZwC8Ly9WcWVN8er8AkR4rm|",245);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  if (strncmp(result_test,"Could not save the name}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for remote_data_save_name - name already in database but can renew\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for remote_data_save_name - name already in database but can renew\033[0m\n");
  }

  // remote_data_save_name - address is already in the database
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"TEST1\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"" TEST_WALLET "\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_SAVE_NAME|" TEST_REMOTE_DATA_NAME "|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV11cjRtG1krpsjHW5bgp9wZ6ZpeToieQ91HDHK3QTAX4drZXXzg8yzrMR4NUbTb5w8XpZwC8Ly9WcWVN8er8AkR4rm|",245);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"" TEST_WALLET "\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_SAVE_NAME|" TEST_REMOTE_DATA_NAME "|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000|",245);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"" TEST_WALLET "\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_SAVE_NAME|test|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV1hctHs3RzEm6FJxhL6U9JJfMkZBcL6UY9Gh9mJm2M3j7DbHVkp7mvLXFNrhn1f7PZwgWDL1GavPRBReYbgyL6y41m|",245);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_1);
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate_name_1\",\"public_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"|\",\"paddress_list\":\"|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_PURCHASE_NAME|XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v|XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv|0000000000000000000000000000000000000000000000000000000000000000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV1MJJVDraQ256iJn9H6vcSg2dyd6qBtre4UciATKPCdvggLDpbfjgbCHphLXXrUnnvBWHZidLtoPYAVRFiFQ7CvVfB|",507);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_1);
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate_name_1\",\"public_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"paddress\":\"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_PURCHASE_NAME|XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v|XCA1VXWDkYm2u56cbe6b3VajEe4qFLGhVgxeNcgnA3ht1gWvrnZTYr8PJtpkWzagR7c76dDdeyFgoeKZj55W78bp4T8L8n6wLW|0000000000000000000000000000000000000000000000000000000000000000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV1Z4LgqU5UCQ1bTcZ8qtqn1WJ8YgjonHMBZJQwFnkYnEqxJMsE6UDGMiS2wFNiKMP53aXxh6Aitime9ZsnQYwKGBKK|",507);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_1);
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate_name_1\",\"public_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"paddress\":\"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_PURCHASE_NAME|XCA1VXWDkYm2u56cbe6b3VajEe4qFLGhVgxeNcgnA3ht1gWvrnZTYr8PJtpkWzagR7c76dDdeyFgoeKZj55W78bp4T8L8n6wLW|XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv|0000000000000000000000000000000000000000000000000000000000000000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV15FhQKneotjxamrwgdavL36aUWG673s4tmPQ7XuhKeaVbRna3queCuDraAfJysAzxLB4HVihXiej6ZVuGPz5S4tze|",507);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_1);
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate_name_1\",\"public_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"paddress\":\"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_PURCHASE_NAME|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|0000000000000000000000000000000000000000000000000000000000000000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV18TUkVej7DjeYXrQivBaqXgFQ3txtcgoitLh6uo4mTHesB7Hm5YQRXtHBKVb8AFiLrDhTjixUDM5ZvQghx2fLJ6E9|",507);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_1);
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate_name_1\",\"public_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"paddress\":\"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_PURCHASE_NAME|XCA1VXWDkYm2u56cbe6b3VajEe4qFLGhVgxeNcgnA3ht1gWvrnZTYr8PJtpkWzagR7c76dDdeyFgoeKZj55W78bp4T8L8n6wLW|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|0000000000000000000000000000000000000000000000000000000000000000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV118z9QgCnBvX9Nhx3C7qwkmUPi73iUP18VTjEJ9BxYNzT7LcKo1Qay3K7HSCWcVa8CE7tGoheWyrCb15HSseUyVCF|",507);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_1);
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate_name_1\",\"public_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_PURCHASE_NAME|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|0000000000000000000000000000000000000000000000000000000000000000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV1WKX6LdgPS4f23YoTaYuRkeeLgskNTaZ3X4Gbjw6HPhFgLQwCZy5VfFt6yijXemoadLQNLNrsXRgGzNaG75vFzrYq|",507);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_1);
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate_name_1\",\"public_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_PURCHASE_NAME|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|0000000000000000000000000000000000000000000000000000000000000000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV1TpkJMGPZq6fghF8bwLdNMgWqcvACadyn3G8wkJCaCeo5KGqUSPbUEbMbphxddWPutgcp5exZ1T5KZVW5A3GxcU9b|",507);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_1);
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate_name_1\",\"public_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_PURCHASE_NAME|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|0000000000000000000000000000000000000000000000000000000000000000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV1SQkde4hnRsQ1PoCbdDQsYxJQLDodeXiZ1Vi5NJHGdDA4S1JPGM7kXDxcSeVwsaKHsK1bjcVjjYVXQhpMMDyCnpWZ|",507);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_1);
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate_name_1\",\"public_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_PURCHASE_NAME|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|0000000000000000000000000000000000000000000000000000000000000000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV1Kh3Ld4ExnUmD49vDiiKG3FeMStpUPeJedPESpPi6eSDQZVSWSu6w7VxEgwc5nQhaGx59TUsEQzDqp8BUetCFQRcf|",507);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  if (strncmp(result_test,"Could not purchase the name}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for remote_data_purchase_name - tx_hash invalid from address\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for remote_data_purchase_name - tx_hash invalid from address\033[0m\n");
  }









  // /remotedatagetblockproducerinformation
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"" TEST_WALLET "\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"amount\":\"1000\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetblockproducerinformation HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"{\"public_address\":\"" TEST_WALLET "\",\"amount\":\"100\"}",sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for /remotedatagetblockproducerinformation","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for /remotedatagetblockproducerinformation","red");
  }









  // /remotedatagetdelegatesinformation
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"amount\":\"1000\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetdelegatesinformation HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"[{\"name\":\"delegate\",\"public_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"},{\"name\":\"delegate\",\"public_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"amount\":\"1000\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}]",sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for /remotedatagetdelegatesinformation","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for /remotedatagetdelegatesinformation","red");
  }









  // delegates_set_amount - set amount
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_DELEGATES_SET_AMOUNT|1000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV165rGE98mU9kfXPxRNVcMUhb14Csma18rmTYXTK4TRaaq8Xr3Kgs9TU3hB7MbovXmm9CjJrvq5dUtEg798NSDR2vA|",256);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_DELEGATES_SET_AMOUNT|1000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000|",256);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  if (strncmp(result_test,"Could not set the amount}",BUFFER_SIZE) == 0)
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
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_DELEGATES_SET_AMOUNT|TEST|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV1NvR2Najsopc9ZbAt6r1SMs1HhpaGsmXgzYK9eewvzSSUAExYhE4w7fWCEuKz2usa2USbM24SR5LbKVQXhsaKGy3Q|",256);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  if (strncmp(result_test,"Could not set the amount}",BUFFER_SIZE) == 0)
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
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_DELEGATES_SET_AMOUNT|1000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV165rGE98mU9kfXPxRNVcMUhb14Csma18rmTYXTK4TRaaq8Xr3Kgs9TU3hB7MbovXmm9CjJrvq5dUtEg798NSDR2vA|",256);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  if (strncmp(result_test,"Could not set the amount}",BUFFER_SIZE) == 0)
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"" TEST_WALLET "\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_RENEWAL_START|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV1dVcFLcgsB8mgLBXxevDHtoHQqGk9tQWVzTr4uZtBEtV4Cn6KCmLFUqsSUmHp4TBh8gcFJjQcd2mgDDvHzLSyfJBD|",244);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  if (strncmp(result_test,"Started to renew the name}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for renewal_start\033[0m\n");
    count_test++;
  }
  else
  {
    color_print(result_test,"yellow");
    fprintf(stderr,"\033[1;31mFAILED! Test for renewal_start\033[0m\n");
  }

  // renewal_start - invalid signature
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"" TEST_WALLET "\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_RENEWAL_START|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000|",244);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"" TEST_WALLET "\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"" TEST_WALLET "\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_RENEWAL_START|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV1dVcFLcgsB8mgLBXxevDHtoHQqGk9tQWVzTr4uZtBEtV4Cn6KCmLFUqsSUmHp4TBh8gcFJjQcd2mgDDvHzLSyfJBD|",244);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_1);
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate_name_1\",\"public_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_RENEWAL_END|0000000000000000000000000000000000000000000000000000000000000000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV1cCpQWGyndN2EqReU4fff14czdRJ5zgmuFL93dzRtkSs1biJJ4kqyQkqLGjxJifmh3mG3uk9e5sUkoLbZT5cakWQP|",307);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_1);
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate_name_1\",\"public_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_RENEWAL_END|0000000000000000000000000000000000000000000000000000000000000000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000|",307);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  if (strncmp(result_test,"Could not renew the name}",BUFFER_SIZE) == 0)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for renewal_end - invalid signature\033[0m\n");
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for renewal_end - invalid signature\033[0m\n");
  }

  // renewal_end - not in database
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_1);
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate_name_1\",\"public_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"amount\":\"100\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_RENEWAL_END|0000000000000000000000000000000000000000000000000000000000000000|XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|SigV1cCpQWGyndN2EqReU4fff14czdRJ5zgmuFL93dzRtkSs1biJJ4kqyQkqLGjxJifmh3mG3uk9e5sUkoLbZT5cakWQP|",307);
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
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
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" TEST_WALLET "\",\"total_vote_count\":\"120000000\",\"IP_address\":\"" NETWORK_DATA_NODE_1_IP_ADDRESS "\",\"delegate_name\":\"delegate_name_1\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"shared_delegate_status\":\"solo\",\"delegate_fee\":\"\",\"server_specs\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"|" XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT_DATA "\",\"public_key\":\"" NETWORK_DATA_NODE_1_PUBLIC_KEY "\"}");
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_19);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_20);
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"amount\":\"1000\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  INITIALIZE_NETWORK_DATA_NODES_TEST;
  memset(data_test,0,sizeof(data_test));
  add_delegates_to_remote_data_delegates();
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,"GET /remotedatagetdelegatesinformation HTTP/",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,"[{\"name\":\"delegate\",\"public_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"amount\":\"1000\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"},{\"name\":\"delegate_name_19\",\"public_address\":\"XCA1qbzqCoX9WGsqjYgKMGH2eFjkvBRmsb2HKiumrKbjMVDLhukUShsgjFbT6osQBA3TaL9hD7wr8dW2Zq5QzRyU1mnQAQwTpJ\",\"amount\":\"1000000000\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}]",sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for add_delegates_to_remote_data_delegates","green");
    count_test++;
  }
  else
  {
    color_print(data_test,"yellow");
    color_print("FAILED! Test for add_delegates_to_remote_data_delegates","red");
  }









  // maintain_slist_and_plist
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"TEST\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"TEST1\",\"address\":\"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv\",\"saddress\":\"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv\",\"paddress\":\"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv\",\"saddress_list\":\"\",\"paddress_list\":\"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"TEST2\",\"address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"saddress\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"paddress\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"saddress_list\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v|\",\"paddress_list\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"TEST3\",\"address\":\"" TEST_WALLET "\",\"saddress\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"paddress\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"saddress_list\":\"" TEST_WALLET "|\",\"paddress_list\":\"" TEST_WALLET "|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
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
        if (read_document_field_from_collection(database_name,"remote_data",result_test,"saddress_list",data_test) == 1 && strncmp(data_test,TEST_WALLET "|",BUFFER_SIZE) == 0 && read_document_field_from_collection(database_name,"remote_data",result_test,"paddress_list",data_test) == 1 && strncmp(data_test,TEST_WALLET "|",BUFFER_SIZE) == 0)
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"TEST\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"1500000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"\"}");
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"TEST1\",\"address\":\"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv\",\"saddress\":\"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv\",\"paddress\":\"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv\",\"saddress_list\":\"\",\"paddress_list\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"1500000000\",\"reserve_delegate_address\":\"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"TEST2\",\"address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"saddress\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"paddress\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"saddress_list\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v|\",\"paddress_list\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  check_for_expired_names();
  memset(result_test,0,sizeof(result_test));
  memcpy(result_test,"{\"name\":\"TEST\"}",15);
  if (count_documents_in_collection(database_name,"remote_data",result_test) == 0)
  {
    memset(data_test,0,sizeof(data_test));
    memset(result_test,0,sizeof(result_test));
    memcpy(result_test,"{\"name\":\"TEST1\"}",16);
    if (read_document_field_from_collection(database_name,"remote_data",result_test,"paddress_list",data_test) == 1 && strncmp(data_test,"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v|XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv|",BUFFER_SIZE) == 0 && read_document_field_from_collection(database_name,"remote_data",result_test,"saddress_list",data_test) == 1 && strncmp(data_test,"XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv|",BUFFER_SIZE) == 0)
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"amount\":\"1000\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
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

  delete_database(database_name);
  RESET_ERROR_MESSAGES;
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"amount\":\"1000\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  if (sync_check_remote_data_database(3) == 1)
  {
    color_print("PASSED! Test for sync_check_remote_data_database from a random network data node","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sync_check_remote_data_database from a random network data node","red");
  }

  delete_database(database_name);
  RESET_ERROR_MESSAGES;
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"amount\":\"1000\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  if (sync_check_majority_remote_data_database() == 1)
  {
    color_print("PASSED! Test for sync_check_majority_remote_data_database","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sync_check_majority_remote_data_database","red");
  }

  delete_database(database_name);
  RESET_ERROR_MESSAGES;
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"amount\":\"1000\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  if (sync_check_remote_data_delegates_database(1) == 1)
  {
    color_print("PASSED! Test for sync_check_remote_data_delegates_database from a random block verifier","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sync_check_remote_data_delegates_database from a random block verifier","red");
  }

  delete_database(database_name);
  RESET_ERROR_MESSAGES;
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"amount\":\"1000\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  if (sync_check_remote_data_delegates_database(3) == 1)
  {
    color_print("PASSED! Test for sync_check_remote_data_delegates_database from a random network data node","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sync_check_remote_data_delegates_database from a random network data node","red");
  }

  delete_database(database_name);
  RESET_ERROR_MESSAGES;
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"amount\":\"1000\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"amount\":\"1000\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  // initialize the previous, current and next block verifiers list
  INITIALIZE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_TEST;

  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  RESET_ERROR_MESSAGES;
  memcpy(data_test,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"data_hash\": \"0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002c505deb988771196910815f240bbc1a\",\r\n}",250);
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"amount\":\"1000\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  memcpy(data_test,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n}",106);
  if (sign_data(data_test) == 1 && send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,data_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && strncmp(result_test,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"remote_data_database\": \"{\"name\" : \"" TEST_REMOTE_DATA_NAME "\", \"address\" : \"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\", \"saddress\" : \"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\", \"paddress\" : \"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\", \"saddress_list\" : \"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\", \"paddress_list\" : \"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\", \"website\" : \"TEST.xcash\", \"smart_contract_hash\" : \"\", \"timestamp\" : \"10000000000\", \"reserve_delegate_address\" : \"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\", \"reserve_delegate_amount\" : \"100\", \"tx_hash\" : \"0000000000000000000000000000000000000000000000000000000000000000\" }\"",1067) == 0)
  {
    color_print("PASSED! Test for server_receive_data_socket_block_verifiers_to_block_verifiers_remote_data_database_download_file_update","green");
    count_test++;
  }
  else
  {
    color_print(result_test,"blue");
    color_print("FAILED! Test for server_receive_data_socket_block_verifiers_to_block_verifiers_remote_data_database_download_file_update","red");
  }
  RESET_ERROR_MESSAGES;

  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  delete_database(database_name);
  RESET_ERROR_MESSAGES;
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"amount\":\"1000\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  memcpy(data_test,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DELEGATES_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"data_hash\": \"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000fff91f8d122097cc2b36128684353126\",\r\n}",260);
  if (sign_data(data_test) == 1 && send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,data_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && strncmp(result_test,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"remote_data_delegates_database\": \"true\"",155) == 0)
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"amount\":\"1000\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  memcpy(data_test,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n}",116);
  if (sign_data(data_test) == 1 && send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,data_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && strncmp(result_test,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"remote_data_delegates_database\": \"{\"name\" : \"delegate\", \"public_address\" : \"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\", \"amount\" : \"1000\", \"total_registered_renewed_amount\" : \"0\", \"total_amount\" : \"0\" }\"",379) == 0)
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
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"amount\":\"1000\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
  
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

  delete_database(database_name);
  RESET_ERROR_MESSAGES;
  insert_document_into_collection_json(database_name,"remote_data","{\"name\":\"" TEST_REMOTE_DATA_NAME "\",\"address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"paddress\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"saddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"paddress_list\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP|\",\"website\":\"" TEST_REMOTE_DATA_ADDRESS_NAME "\",\"smart_contract_hash\":\"\",\"timestamp\":\"10000000000\",\"reserve_delegate_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"reserve_delegate_amount\":\"100\",\"tx_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\"}");
  insert_document_into_collection_json(database_name,"remote_data_delegates","{\"name\":\"delegate\",\"public_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"amount\":\"1000\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}");
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
