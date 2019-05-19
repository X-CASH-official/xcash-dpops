#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "define_macros_functions.h"
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
  reserve_bytes_length - The amount of reserve bytes to use in creating the block template
  HTTP_SETTINGS - 1 to print the messages, otherwise 0. This is used for the testing flag to not print any success or error messages
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int get_block_template(char *result, char* reserve_bytes_length, const int HTTP_SETTINGS)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);
  const size_t RESERVE_BYTES_LENGTH = strnlen(reserve_bytes_length,BUFFER_SIZE);

  // Variables
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // define macros
  #define pointer_reset_all \
  free(message); \
  message = NULL; \
  free(data); \
  data = NULL; \

  // check if the memory needed was allocated on the heap successfully
  if (message == NULL || data == NULL)
  {
    if (message != NULL)
    {
      pointer_reset(message);
    }
    if (data != NULL)
    {
      pointer_reset(data);
    }
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // create the message
  memcpy(message,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_block_template\",\"params\":{\"wallet_address\":\"",84);
  memcpy(message+84,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(message+84+XCASH_WALLET_LENGTH,"\",\"reserve_size\":",17);
  memcpy(message+101+XCASH_WALLET_LENGTH,reserve_bytes_length,RESERVE_BYTES_LENGTH);
  memcpy(message+101+XCASH_WALLET_LENGTH+RESERVE_BYTES_LENGTH,"}",1);

  if (send_http_request(data,"127.0.0.1","/json_rpc",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,message,RECEIVE_DATA_TIMEOUT_SETTINGS,"get block template",HTTP_SETTINGS) <= 0)
  {  
    pointer_reset_all;
    return 0;
  }
  
  if (parse_json_data(data,"blocktemplate_blob",result) == 0)
  {
    pointer_reset_all;
    return 0;
  }
  
  pointer_reset_all; 
  return 1;
  
  #undef pointer_reset_all
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
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // define macros
  #define BLOCKCHAIN_RESERVED_BYTES_START "7c424c4f434b434841494e5f52455345525645445f42595445535f53544152547c"
  #define pointer_reset_all \
  free(message); \
  message = NULL; \
  free(data); \
  data = NULL; \

  // check if the memory needed was allocated on the heap successfully
  if (message == NULL || data == NULL)
  {
    if (message != NULL)
    {
      pointer_reset(message);
    }
    if (data != NULL)
    {
      pointer_reset(data);
    }
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }
  
  // create the message
  memcpy(message,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_block\",\"params\":{\"height\":\"",68);
  memcpy(message+68,block_height,strnlen(block_height,BUFFER_SIZE));
  memcpy(message+68+strnlen(block_height,BUFFER_SIZE),"\"}}",3);

  if (send_http_request(data,"127.0.0.1","/json_rpc",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,message,RECEIVE_DATA_TIMEOUT_SETTINGS,"get block settings",HTTP_SETTINGS) <= 0)
  {  
    pointer_reset_all;
    return 0;
  }
  memset(message,0,strlen(message));
  
  if (parse_json_data(data,"blob",message) == 0)
  {
    pointer_reset_all;
    return 0;
  }

  if (strstr(data,BLOCKCHAIN_RESERVED_BYTES_START) != NULL)
  {
    pointer_reset_all; 
    return 2; 
  }
  else
  {
    pointer_reset_all; 
    return 1; 
  }  
  
  #undef BLOCKCHAIN_RESERVED_BYTES_START
  #undef pointer_reset_all
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
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;
  size_t counter = 0;

  // define macros
  #define pointer_reset_all \
  free(message); \
  message = NULL; \
  free(data); \
  data = NULL; \

  // check if the memory needed was allocated on the heap successfully
  if (message == NULL || data == NULL)
  {
    if (message != NULL)
    {
      pointer_reset(message);
    }
    if (data != NULL)
    {
      pointer_reset(data);
    }
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // create the message
  memcpy(message,"{\"txs_hashes\":[",15);
  counter += 15;
  for (count = 0; count < AMOUNT_OF_TRANSACTIONS; count++)
  {
    memcpy(message+counter,"\"",1);
    counter++;
    memcpy(message+counter,transactions[count],strnlen(transactions[count],BUFFER_SIZE));
    counter += strnlen(transactions[count],BUFFER_SIZE);
    memcpy(message+counter,"\",",2);
    counter += 2;
  }
  memcpy(message+counter-1,"]}",2);

  if (send_http_request(data,"127.0.0.1","/get_transactions",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,message,RECEIVE_DATA_TIMEOUT_SETTINGS,"",MESSAGE_SETTINGS) <= 0)
  {  
    pointer_reset_all;   
    return 0;
  }

  // verify the blockchain_network_transactions
  // if the results contain missed_tx or "in_pool": false then the transactions are not verified  
  if (TRANSACTION_SETTINGS == 0)
  {
    if (strstr(data,"missed_tx") != NULL)
    {
      pointer_reset_all;   
      return 0;
    }
  }
  else
  {
    if (strstr(data,"missed_tx") != NULL || strstr(data,"\"in_pool\": false") != NULL)
    {
      pointer_reset_all;   
      return 0;
    }
  }
    
  pointer_reset_all; 
  return 1;
  
  #undef pointer_reset_all
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
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  if (send_http_request(data,"127.0.0.1","/json_rpc",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_block_count\"}",RECEIVE_DATA_TIMEOUT_SETTINGS,"get current block height",MESSAGE_SETTINGS) <= 0)
  {  
    pointer_reset(data);   
    return 0;
  }
  
  if (parse_json_data(data,"count",result) == 0)
  {
    pointer_reset(data); 
    return 0;
  }
    
  pointer_reset(data); 
  return 1;
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
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  if (send_http_request(data,"127.0.0.1","/json_rpc",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_last_block_header\"}",RECEIVE_DATA_TIMEOUT_SETTINGS,"get previous block hash",MESSAGE_SETTINGS) <= 0)
  {  
    pointer_reset(data);   
    return 0;
  }
  
  if (parse_json_data(data,"hash",result) == 0)
  {
    pointer_reset(data); 
    return 0;
  }
      
  pointer_reset(data); 
  return 1;
}