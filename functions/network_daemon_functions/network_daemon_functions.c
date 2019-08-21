#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "network_daemon_functions.h"
#include "network_functions.h"
#include "server_functions.h"
#include "string_functions.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: get_block_template
Description: Gets the block template for creating a new block
Parameters:
  result - The block template
  HTTP_SETTINGS - 1 to print the messages, otherwise 0. This is used for the testing flag to not print any success or error messages
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int get_block_template(char *result, const int HTTP_SETTINGS)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char message[BUFFER_SIZE];
  char data[BUFFER_SIZE];

  // define macros
  #define GET_BLOCK_TEMPLATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"get_block_template",18); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,sizeof(error_message.data[error_message.total]))); \
  error_message.total++; \
  return 0;
  
  memset(message,0,sizeof(message));
  memset(data,0,sizeof(data));

  // create the message
  memcpy(message,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_block_template\",\"params\":{\"wallet_address\":\"",84);
  memcpy(message+84,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(message+84+XCASH_WALLET_LENGTH,"\",\"reserve_size\":128}",21);

  if (send_http_request(data,"127.0.0.1","/json_rpc",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,message,RECEIVE_DATA_TIMEOUT_SETTINGS,"get block template",HTTP_SETTINGS) <= 0)
  {  
    GET_BLOCK_TEMPLATE_ERROR("Could not create the block template");
  }
  
  if (parse_json_data(data,"blocktemplate_blob",result, BUFFER_SIZE) == 0)
  {
    GET_BLOCK_TEMPLATE_ERROR("Could not create the block template");
  }
  return 1;
  
  #undef GET_BLOCK_TEMPLATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: submit_block_template
Description: Adds a network block to the network
Parameters:
  data - The block_blob
  MESSAGE_SETTINGS - 1 to print the messages, otherwise 0. This is used for the testing flag to not print any success or error messages
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int submit_block_template(char* data, const int HTTP_SETTINGS)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);
  const size_t DATA_LENGTH = strnlen(data,BUFFER_SIZE);

  // Variables
  char message[BUFFER_SIZE];

  // define macros
  #define SUBMIT_BLOCK_TEMPLATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"submit_block_template",21); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,sizeof(error_message.data[error_message.total]))); \
  error_message.total++; \
  return 0;
  
  memset(message,0,sizeof(message));

  // create the message
  memcpy(message,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"submit_block\",\"params\":[\"",61);
  memcpy(message+61,data,DATA_LENGTH);
  memcpy(message+61+DATA_LENGTH,"\"]}",3);
  memset(data,0,sizeof(data));

  if (send_http_request(data,"127.0.0.1","/json_rpc",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,message,RECEIVE_DATA_TIMEOUT_SETTINGS,"submit block template",HTTP_SETTINGS) <= 0)
  {  
    SUBMIT_BLOCK_TEMPLATE_ERROR("Could not submit the block template");
  }
  
  if (strstr(data,"error") != NULL)
  {
    SUBMIT_BLOCK_TEMPLATE_ERROR("Could not submit the block template");
  }
  return 1;

  #undef SUBMIT_BLOCK_TEMPLATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_block_settings
Description: Gets the block settings
Parameters:
  block_height - The block height
  HTTP_SETTINGS - 1 to print the messages, otherwise 0. This is used for the testing flag to not print any success or error messages
Return: 0 if an error has occured, 1 if the block is a proof of work block, 2 if the block is a X-CASH proof of stake block
-----------------------------------------------------------------------------------------------------------
*/

int get_block_settings(char* block_height, const int HTTP_SETTINGS)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char message[BUFFER_SIZE];
  char data[BUFFER_SIZE];

  // define macros
  #define GET_BLOCK_SETTINGS_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"get_block_settings",18); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,sizeof(error_message.data[error_message.total]))); \
  error_message.total++; \
  return 0;

  memset(message,0,sizeof(message));
  memset(data,0,sizeof(data));
  
  // create the message
  memcpy(message,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_block\",\"params\":{\"height\":\"",67);
  memcpy(message+67,block_height,strnlen(block_height,sizeof(message)));
  memcpy(message+strlen(message),"\"}}",3);

  if (send_http_request(data,"127.0.0.1","/json_rpc",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,message,RECEIVE_DATA_TIMEOUT_SETTINGS,"get block settings",HTTP_SETTINGS) <= 0)
  {  
    GET_BLOCK_SETTINGS_ERROR("Could not get the block settings");
  }
  memset(message,0,sizeof(message));
  
  if (parse_json_data(data,"blob",message, sizeof(message)) == 0)
  {
    GET_BLOCK_SETTINGS_ERROR("Could not get the block settings");
  }

  if (strstr(message,BLOCKCHAIN_RESERVED_BYTES_START) != NULL)
  {
    return 2; 
  }
  else
  {
    return 1; 
  }  
  return 0;
  
  #undef GET_BLOCK_SETTINGS_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_block_reserve_byte_data_hash
Description: Gets the blocks reserve byte data hash
Parameters:
  reserve_byte_data_hash - The reserve byte data hash
  BLOCK_HEIGHT - The block height
  HTTP_SETTINGS - 1 to print the messages, otherwise 0. This is used for the testing flag to not print any success or error messages
Return: 0 if an error has occured, 1 if the block is a proof of work block, 2 if the block is a X-CASH proof of stake block
-----------------------------------------------------------------------------------------------------------
*/

int get_block_reserve_byte_data_hash(char *reserve_byte_data_hash, const char* BLOCK_HEIGHT, const int HTTP_SETTINGS)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char message[BUFFER_SIZE];
  char data[BUFFER_SIZE];

  // define macros
  #define GET_BLOCK_RESERVE_BYTE_DATA_HASH_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"get_block_reserve_byte_data_hash",32); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,sizeof(error_message.data[error_message.total]))); \
  error_message.total++; \
  return 0;
  
  memset(message,0,sizeof(message));
  memset(data,0,sizeof(data));
  
  // create the message
  memcpy(message,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_block\",\"params\":{\"height\":\"",68);
  memcpy(message+68,BLOCK_HEIGHT,strnlen(BLOCK_HEIGHT,sizeof(message)));
  memcpy(message+68+strnlen(BLOCK_HEIGHT,sizeof(message)),"\"}}",3);

  if (send_http_request(data,"127.0.0.1","/json_rpc",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,message,RECEIVE_DATA_TIMEOUT_SETTINGS,"get block settings",HTTP_SETTINGS) <= 0)
  {  
    GET_BLOCK_RESERVE_BYTE_DATA_HASH_ERROR("Could not get the blocks reserve bytes data hash");
  }
  memset(message,0,sizeof(message));
  
  if (parse_json_data(data,"blob",message,sizeof(message)) == 0)
  {
    GET_BLOCK_RESERVE_BYTE_DATA_HASH_ERROR("Could not get the blocks reserve bytes data hash");
  }

  memset(reserve_byte_data_hash,0,strlen(reserve_byte_data_hash));
  memcpy(reserve_byte_data_hash,&data[strlen(data) - (strlen(strstr(data,BLOCKCHAIN_RESERVED_BYTES_START_DATA)) - 62)],128);
  
  return 1;

  #undef GET_BLOCK_RESERVE_BYTE_DATA_HASH_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: verify_blockchain_network_transactions
Description: Gets the transaction details
Parameters:
  transactions - The array or transactions
  AMOUNT_OF_TRANSACTIONS - The amount of transactions
  TRANSACTION_SETTINGS - 0 if the transaction is valid if it its in the blockchain or waiting to be added to the blockchain, 1 if valid if only wating to be added to the blockchain
  MESSAGE_SETTINGS - 1 to print the messages, otherwise 0. This is used for the testing flag to not print any success or error messages
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int verify_blockchain_network_transactions(char* transactions[], const size_t AMOUNT_OF_TRANSACTIONS, const int TRANSACTION_SETTINGS, const int MESSAGE_SETTINGS)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char message[BUFFER_SIZE];
  char data[BUFFER_SIZE];
  size_t count;
  size_t counter = 0;

  // define macros
  #define VERIFY_BLOCKCHAIN_NETWORK_TRANSACTIONS_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"verify_blockchain_network_transactions",38); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,sizeof(error_message.data[error_message.total]))); \
  error_message.total++; \
  return 0;

  memset(message,0,sizeof(message));
  memset(data,0,sizeof(data));  

  // create the message
  memcpy(message,"{\"txs_hashes\":[",15);
  counter += 15;
  for (count = 0; count < AMOUNT_OF_TRANSACTIONS; count++)
  {
    memcpy(message+counter,"\"",1);
    counter++;
    memcpy(message+counter,transactions[count],strnlen(transactions[count],sizeof(message)));
    counter += strnlen(transactions[count],sizeof(message));
    memcpy(message+counter,"\",",2);
    counter += 2;
  }
  memcpy(message+counter-1,"]}",2);

  if (send_http_request(data,"127.0.0.1","/get_transactions",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,message,RECEIVE_DATA_TIMEOUT_SETTINGS,"",MESSAGE_SETTINGS) <= 0)
  {  
    VERIFY_BLOCKCHAIN_NETWORK_TRANSACTIONS_ERROR("Could not verify the blockchain network transactions");
  }

  // verify the blockchain_network_transactions
  // if the results contain missed_tx or "in_pool": false then the transactions are not verified  
  if (TRANSACTION_SETTINGS == 0)
  {
    if (strstr(data,"missed_tx") != NULL)
    {
      VERIFY_BLOCKCHAIN_NETWORK_TRANSACTIONS_ERROR("Could not verify the blockchain network transactions");
    }
  }
  else
  {
    if (strstr(data,"missed_tx") != NULL || strstr(data,"\"in_pool\": false") != NULL)
    {
      VERIFY_BLOCKCHAIN_NETWORK_TRANSACTIONS_ERROR("Could not verify the blockchain network transactions");
    }
  }
  return 1;

  #undef VERIFY_BLOCKCHAIN_NETWORK_TRANSACTIONS_ERROR  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_current_block_height
Description: Gets the current block height of the network
Parameters:
  result - The string where you want the current block height to be saved to
  MESSAGE_SETTINGS - 1 to print the messages, otherwise 0. This is used for the testing flag to not print any success or error messages
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int get_current_block_height(char *result, const int MESSAGE_SETTINGS)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char data[BUFFER_SIZE];

  // define macros
  #define GET_CURRENT_BLOCK_HEIGHT_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"get_current_block_height",24); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,sizeof(error_message.data[error_message.total]))); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));

  if (send_http_request(data,"127.0.0.1","/json_rpc",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_block_count\"}",RECEIVE_DATA_TIMEOUT_SETTINGS,"get current block height",MESSAGE_SETTINGS) <= 0)
  {  
    GET_CURRENT_BLOCK_HEIGHT_ERROR("Could not get the current block height");
  }
  
  if (parse_json_data(data,"count",result, BUFFER_SIZE) == 0)
  {
    GET_CURRENT_BLOCK_HEIGHT_ERROR("Could not get the current block height");
  }
  return 1;

  #undef GET_CURRENT_BLOCK_HEIGHT_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_previous_block_hash
Description: Gets the previous block hash of the network
Parameters:
  result - The string where you want the previous block hash to be saved to
  MESSAGE_SETTINGS - 1 to print the messages, otherwise 0. This is used for the testing flag to not print any success or error messages
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int get_previous_block_hash(char *result, const int MESSAGE_SETTINGS)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char data[BUFFER_SIZE];

  // define macros
  #define GET_PREVIOUS_BLOCK_HASH_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"get_previous_block_hash",23); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,sizeof(error_message.data[error_message.total]))); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));

  if (send_http_request(data,"127.0.0.1","/json_rpc",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_last_block_header\"}",RECEIVE_DATA_TIMEOUT_SETTINGS,"get previous block hash",MESSAGE_SETTINGS) <= 0)
  {  
    GET_PREVIOUS_BLOCK_HASH_ERROR("Could not get the previous block hash");
  }
  
  if (parse_json_data(data,"hash",result, BUFFER_SIZE) == 0)
  {
    GET_PREVIOUS_BLOCK_HASH_ERROR("Could not get the previous block hash");
  }
  return 1;

  #undef GET_PREVIOUS_BLOCK_HASH_ERROR
}