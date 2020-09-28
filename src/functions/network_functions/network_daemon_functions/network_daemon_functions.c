#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "block_verifiers_functions.h"
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
  // Variables
  char data[SMALL_BUFFER_SIZE];
  int count;
  int count2;
  size_t block_height;
  
  memset(data,0,sizeof(data));

  // reset the block_verifiers_current_block_height
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    block_verifiers_current_block_height[count2] = 0;
  }

  sscanf(current_block_height,"%zu",&block_height);

  // get the current block height from each block verifier
  memcpy(data,"{\r\n \"message_settings\": \"GET_CURRENT_BLOCK_HEIGHT\",\r\n}",54);

  // send the message to all block verifiers. This function will wait until all messages have arrived
  block_verifiers_send_data_socket((const char*)data);

  // check if the blockchain is fully synced
  for (count = 0, count2 = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    if (block_height < block_verifiers_current_block_height[count])
    {
      count2++;
    }
  }

  return count2 >= BLOCK_VERIFIERS_VALID_AMOUNT ? 0 : 1;
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