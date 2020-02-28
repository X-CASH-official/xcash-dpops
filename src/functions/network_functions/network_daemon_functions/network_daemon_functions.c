#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "read_database_functions.h"
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
Name: check_if_blockchain_is_fully_synced
Description: Gets if the blockchain is fully synced
Return: 1 if fully synced, 0 if not or an error
-----------------------------------------------------------------------------------------------------------
*/

int check_if_blockchain_is_fully_synced(void)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char message[MAXIMUM_NUMBER_SIZE];
  char data[SMALL_BUFFER_SIZE];
  size_t count;
  size_t count2;

  // define macros
  #define MESSAGE "{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_info\"}"
  #define CHECK_IF_BLOCKCHAIN_IS_FULLY_SYNCED_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"check_if_blockchain_is_fully_synced",35); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;
  
  memset(message,0,sizeof(message));
  memset(data,0,sizeof(data));

  if (send_http_request(data,XCASH_DPOPS_delegates_IP_address,"/json_rpc",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,MESSAGE,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) <= 0 || parse_json_data(data,"target_height",message,BUFFER_SIZE) == 0)
  {  
    return 0;
  }

  sscanf(current_block_height,"%zu",&count);
  sscanf(message,"%zu",&count2);
  return count < count2 ? 0 : 1;
  
  #undef MESSAGE
  #undef CHECK_IF_BLOCKCHAIN_IS_FULLY_SYNCED_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_block_template
Description: Gets the block template for creating a new block
Parameters:
  result - The block template
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int get_block_template(char *result)
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
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;
  
  memset(message,0,sizeof(message));
  memset(data,0,sizeof(data));

  // create the message
  memcpy(message,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_block_template\",\"params\":{\"wallet_address\":\"",84);
  memcpy(message+84,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(message+84+XCASH_WALLET_LENGTH,"\",\"reserve_size\":128}",21);

  if (send_http_request(data,XCASH_DPOPS_delegates_IP_address,"/json_rpc",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,message,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) <= 0 || parse_json_data(data,"blocktemplate_blob",result, BUFFER_SIZE) == 0)
  {
    memset(data,0,sizeof(data));
    memset(result,0,strlen(result));
    sleep(INVALID_RESERVE_PROOFS_SETTINGS);

    if (send_http_request(data,XCASH_DPOPS_delegates_IP_address,"/json_rpc",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,message,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) <= 0 || parse_json_data(data,"blocktemplate_blob",result, BUFFER_SIZE) == 0)
    {
      GET_BLOCK_TEMPLATE_ERROR("Could not create the block template");
    }
  }
  return 1;
  
  #undef GET_BLOCK_TEMPLATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: submit_block_template
Description: Adds a network block to the network
Parameters:
  DATA - The block_blob
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int submit_block_template(const char* DATA)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);
  const size_t DATA_LENGTH = strnlen(DATA,BUFFER_SIZE);

  // Variables
  char data[SMALL_BUFFER_SIZE];
  char message[SMALL_BUFFER_SIZE];
  
  memset(data,0,sizeof(data));
  memset(message,0,sizeof(message));

  // create the message
  memcpy(message,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"submit_block\",\"params\":[\"",61);
  memcpy(message+61,DATA,DATA_LENGTH);
  memcpy(message+61+DATA_LENGTH,"\"]}",3);

  send_http_request(data,XCASH_DPOPS_delegates_IP_address,"/json_rpc",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,message,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  return 1;

  #undef SUBMIT_BLOCK_TEMPLATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_block_reserve_byte_data_hash
Description: Gets the blocks reserve byte data hash
Parameters:
  reserve_byte_data_hash - The reserve byte data hash
  BLOCK_HEIGHT - The block height
Return: 0 if an error has occured, 1 if the block is a proof of work block, 2 if the block is a X-CASH proof of stake block
-----------------------------------------------------------------------------------------------------------
*/

int get_block_reserve_byte_data_hash(char *reserve_byte_data_hash, const char* BLOCK_HEIGHT)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char message[BUFFER_SIZE];
  char data[BUFFER_SIZE];

  // define macros
  #define GET_BLOCK_RESERVE_BYTE_DATA_HASH_ERROR(settings) \
  if (network_functions_test_error_settings == 1) \
  { \
    memcpy(error_message.function[error_message.total],"get_block_reserve_byte_data_hash",32); \
    memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
    error_message.total++; \
  } \
  return 0;
  
  memset(message,0,sizeof(message));
  memset(data,0,sizeof(data));
  
  // create the message
  memcpy(message,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_block\",\"params\":{\"height\":\"",67);
  memcpy(message+67,BLOCK_HEIGHT,strnlen(BLOCK_HEIGHT,sizeof(message)));
  memcpy(message+67+strnlen(BLOCK_HEIGHT,sizeof(message)),"\"}}",3);

  if (send_http_request(data,XCASH_DPOPS_delegates_IP_address,"/json_rpc",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,message,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) <= 0)
  {  
    GET_BLOCK_RESERVE_BYTE_DATA_HASH_ERROR("Could not get the blocks reserve bytes data hash");
  }
  memset(message,0,sizeof(message));
  
  if (parse_json_data(data,"blob",message,sizeof(message)) == 0)
  {
    GET_BLOCK_RESERVE_BYTE_DATA_HASH_ERROR("Could not get the blocks reserve bytes data hash");
  }

  if (strstr(data,BLOCKCHAIN_RESERVED_BYTES_START_DATA) == NULL)
  {
    GET_BLOCK_RESERVE_BYTE_DATA_HASH_ERROR("Could not get the blocks reserve bytes data hash");
  }

  memset(reserve_byte_data_hash,0,strlen(reserve_byte_data_hash));
  memcpy(reserve_byte_data_hash,&data[strlen(data) - (strlen(strstr(data,BLOCKCHAIN_RESERVED_BYTES_START_DATA)) - 62)],DATA_HASH_LENGTH);
  
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
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int verify_blockchain_network_transactions(char* transactions[], const size_t AMOUNT_OF_TRANSACTIONS, const int TRANSACTION_SETTINGS)
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
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
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

  if (send_http_request(data,XCASH_DPOPS_delegates_IP_address,"/get_transactions",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,message,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) <= 0)
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
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int get_current_block_height(char *result)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char data[SMALL_BUFFER_SIZE];

  // define macros
  #define GET_CURRENT_BLOCK_HEIGHT_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"get_current_block_height",24); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));

  if (send_http_request(data,XCASH_DPOPS_delegates_IP_address,"/json_rpc",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_block_count\"}",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) <= 0 || parse_json_data(data,"count",result, BUFFER_SIZE) == 0)
  {  
    memset(data,0,sizeof(data));
    memset(result,0,strlen(result));
    sleep(INVALID_RESERVE_PROOFS_SETTINGS);

    if (send_http_request(data,XCASH_DPOPS_delegates_IP_address,"/json_rpc",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_block_count\"}",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) <= 0 || parse_json_data(data,"count",result, BUFFER_SIZE) == 0)
    {
      GET_CURRENT_BLOCK_HEIGHT_ERROR("Could not get the current block height");
    }
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
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int get_previous_block_hash(char *result)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char data[SMALL_BUFFER_SIZE];

  // define macros
  #define GET_PREVIOUS_BLOCK_HASH_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"get_previous_block_hash",23); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));
  memset(result,0,strlen(result));

  if (send_http_request(data,XCASH_DPOPS_delegates_IP_address,"/json_rpc",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_last_block_header\"}",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) <= 0 || parse_json_data(data,"hash",result, BUFFER_SIZE) == 0)
  {
    memset(data,0,sizeof(data));
    memset(result,0,strlen(result));
    sleep(INVALID_RESERVE_PROOFS_SETTINGS);

    if (send_http_request(data,XCASH_DPOPS_delegates_IP_address,"/json_rpc",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_last_block_header\"}",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) <= 0 || parse_json_data(data,"hash",result, BUFFER_SIZE) == 0)
    {
      GET_PREVIOUS_BLOCK_HASH_ERROR("Could not get the previous block hash");
    }
  }
  return 1;

  #undef GET_PREVIOUS_BLOCK_HASH_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_previous_block_information
Description: Gets the information of the previous block of the network
Parameters:
  block_date_and_time - The string where you want the previous block date and time to be saved to
  block_hash - The string where you want the previous block hash to be saved to
  block_reward - The string where you want the previous block reward to be saved to
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int get_previous_block_information(char *block_hash, char *block_reward, char *block_date_and_time)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  size_t block_height;
  
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  
  // get the previous block height
  sscanf(current_block_height, "%zu", &block_height);
  block_height--;
  snprintf(data2,sizeof(data2)-1,"%zu",block_height);

  // create the message
  memcpy(data,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_block\",\"params\":{\"height\":",66);
  memcpy(data+66,data2,strnlen(data2,sizeof(data)));
  memcpy(data+66+strnlen(data2,sizeof(data)),"}}",2);

  // get the previous block information
  memset(data2,0,sizeof(data2));
  if (send_http_request(data2,XCASH_DPOPS_delegates_IP_address,"/json_rpc",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) <= 0 || parse_json_data(data2,"hash",block_hash,BUFFER_SIZE) == 0 || parse_json_data(data2,"reward",block_reward,BUFFER_SIZE) == 0 || parse_json_data(data2,"timestamp",block_date_and_time,BUFFER_SIZE) == 0)
  {
    memset(data2,0,sizeof(data2));
    memset(block_hash,0,strlen(block_hash));
    memset(block_reward,0,strlen(block_reward));
    memset(block_date_and_time,0,strlen(block_date_and_time));
    sleep(INVALID_RESERVE_PROOFS_SETTINGS);

    if (send_http_request(data2,XCASH_DPOPS_delegates_IP_address,"/json_rpc",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) <= 0 || parse_json_data(data2,"hash",block_hash,BUFFER_SIZE) == 0 || parse_json_data(data2,"reward",block_reward,BUFFER_SIZE) == 0 || parse_json_data(data2,"timestamp",block_date_and_time,BUFFER_SIZE) == 0)
    {
      return 0;
    }
  }
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: check_found_block
Description: Checks to see if your wallet address found the previous block on the network
Return: 0 if an error has occured, 1 if you did not find the previous block on the network, 2 if you did find the previous block on the network
-----------------------------------------------------------------------------------------------------------
*/

int check_found_block(void)
{ 
  // Variables
  char data[SMALL_BUFFER_SIZE];
  char data2[SMALL_BUFFER_SIZE];
  char result[BUFFER_SIZE];
  size_t block_height;
  size_t count;
  size_t count2;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(result,0,sizeof(result));

  // get the previous block height
  sscanf(current_block_height, "%zu", &block_height);
  block_height--;
  memset(result,0,sizeof(result));
  snprintf(data2,sizeof(data2)-1,"%zu",block_height);

  // create the message
  memset(data,0,sizeof(data));
  memcpy(data,"{\"block_height\":\"",17);
  memcpy(data+17,data2,strnlen(data2,sizeof(data)));
  memcpy(data+strlen(data),"\"}",2);

  // get the reserve_bytes database for the block
  count = ((block_height - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME) + 1;
  memset(data2,0,sizeof(data2));
  memcpy(data2,"reserve_bytes_",14);
  snprintf(data2+14,sizeof(data2)-15,"%zu",count);

  // get the reserve byte data
  memset(result,0,sizeof(result));
  if (read_document_field_from_collection(database_name,data2,data,"reserve_bytes",result,1) == 0)
  {
    return 0;
  }

  memset(data2,0,sizeof(data2));
  memset(data,0,sizeof(data));

  // convert the public_address to a string
  for (count = 0, count2 = 0; count < XCASH_WALLET_LENGTH; count++, count2 += 2)
  {
    snprintf(data+count2,(XCASH_WALLET_LENGTH*2)-1,"%02x",xcash_wallet_public_address[count] & 0xFF);
  }

  // check if the block verifier was the block producer for the previous block
  return string_count(result,data) == 2 ? 2 : 1;
}