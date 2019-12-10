#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include <sys/sysinfo.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "initialize_and_reset_structs_define_macros.h"
#include "structures.h"
#include "variables.h"

#include "blockchain_functions.h"
#include "block_verifiers_synchronize_server_functions.h"
#include "block_verifiers_thread_server_functions.h"
#include "database_functions.h"
#include "read_database_functions.h"
#include "update_database_functions.h"
#include "count_database_functions.h"
#include "file_functions.h"
#include "network_daemon_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "network_wallet_functions.h"
#include "server_functions.h"
#include "organize_functions.h"
#include "string_functions.h"
#include "thread_functions.h"
#include "convert.h"
#include "vrf.h"
#include "crypto_vrf.h"
#include "VRF_functions.h"
#include "sha512EL.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/


/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_node_to_network_data_nodes_get_previous_current_next_block_verifiers_list
Description: Runs the code when the server receives the NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST message
Parameters:
  CLIENT_SOCKET - The socket to send data to
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_node_to_network_data_nodes_get_previous_current_next_block_verifiers_list(const int CLIENT_SOCKET)
{
  // Variables
  char data[BUFFER_SIZE];
  size_t count2;
  int total_delegates = 0;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_node_to_network_data_nodes_get_previous_current_next_block_verifiers_list",100); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  #define COPY_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA(settings,block_verifiers_data) \
  memcpy(data+strlen(data),"\",\r\n \"",6); \
  memcpy(data+strlen(data),(settings),sizeof((settings))-1); \
  memcpy(data+strlen(data),"\": \"",4); \
  for (count2 = 0; (int)count2 < total_delegates; count2++) \
  { \
    memcpy(data+strlen(data),(block_verifiers_data)[count2],strnlen((block_verifiers_data)[count2],sizeof(data))); \
    memcpy(data+strlen(data),"|",1); \
  }

  memset(data,0,sizeof(data));

  // get the delegate amount
  for (count2 = 0; (int)count2 < BLOCK_VERIFIERS_TOTAL_AMOUNT; count2++)
  {
    if (strlen(current_block_verifiers_list.block_verifiers_public_address[count2]) != XCASH_WALLET_LENGTH)
    {
      total_delegates = count2;
      break;
    }
  } 

  // create the message
  memcpy(data,"{\r\n \"message_settings\": \"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST",98);
  COPY_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("previous_block_verifiers_name_list",previous_block_verifiers_list.block_verifiers_name);
  COPY_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("previous_block_verifiers_public_address_list",previous_block_verifiers_list.block_verifiers_public_address);
  COPY_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("previous_block_verifiers_public_key_list",previous_block_verifiers_list.block_verifiers_public_key);
  COPY_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("previous_block_verifiers_IP_address_list",previous_block_verifiers_list.block_verifiers_IP_address);
  COPY_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("current_block_verifiers_name_list",current_block_verifiers_list.block_verifiers_name);
  COPY_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("current_block_verifiers_public_address_list",current_block_verifiers_list.block_verifiers_public_address);
  COPY_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("current_block_verifiers_public_key_list",current_block_verifiers_list.block_verifiers_public_key);
  COPY_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("current_block_verifiers_IP_address_list",current_block_verifiers_list.block_verifiers_IP_address);
  COPY_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("next_block_verifiers_name_list",next_block_verifiers_list.block_verifiers_name);
  COPY_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("next_block_verifiers_public_address_list",next_block_verifiers_list.block_verifiers_public_address);
  COPY_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("next_block_verifiers_public_key_list",next_block_verifiers_list.block_verifiers_public_key);
  COPY_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("next_block_verifiers_IP_address_list",next_block_verifiers_list.block_verifiers_IP_address);
  memcpy(data+strlen(data),"\",\r\n}",5);
  
  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,(unsigned char*)data,0,1,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_ERROR("Could not send the NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST message to the block verifier");
  }
  return 1;

  #undef SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_ERROR
  #undef COPY_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_node_to_network_data_nodes_get_current_block_verifiers_list
Description: Runs the code when the server receives the NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST message
Parameters:
  CLIENT_SOCKET - The socket to send data to
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_node_to_network_data_nodes_get_current_block_verifiers_list(const int CLIENT_SOCKET)
{
  // Variables
  char data[BUFFER_SIZE];
  int total_delegates = 0;
  size_t count;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_node_to_network_data_nodes_get_current_block_verifiers_list",86); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;
  
  memset(data,0,sizeof(data));

  // get the delegate amount
  for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++)
  {
    if (strlen(current_block_verifiers_list.block_verifiers_public_address[count]) != XCASH_WALLET_LENGTH)
    {
      total_delegates = count;
      break;
    }
  }

  // create the message
  memcpy(data,"{\r\n \"message_settings\": \"NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST\",\r\n \"block_verifiers_public_address_list\": \"",129);
  for (count = 0; (int)count < total_delegates; count++)
  {
    memcpy(data+strlen(data),current_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH);
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n \"block_verifiers_public_key_list\": \"",41);
  for (count = 0; (int)count < total_delegates; count++)
  {
    memcpy(data+strlen(data),current_block_verifiers_list.block_verifiers_public_key[count],VRF_PUBLIC_KEY_LENGTH);
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n \"block_verifiers_IP_address_list\": \"",41);
  for (count = 0; (int)count < total_delegates; count++)
  {
    memcpy(data+strlen(data),current_block_verifiers_list.block_verifiers_IP_address[count],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count],sizeof(data)));
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n}",5);
  
  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,(unsigned char*)data,0,1,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST_ERROR("Could not send the NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST message to the block verifier");
  }
  return 1;

  #undef SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_network_data_nodes_to_network_data_nodes_database_sync_check
Description: Runs the code when the server receives the NETWORK_DATA_NODES_TO_NETWORK_DATA_NODES_DATABASE_SYNC_CHECK message
Parameters:
  MESSAGE - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_network_data_nodes_to_network_data_nodes_database_sync_check(const char* MESSAGE)
{
  // Variables
  int count;
  char public_address[BUFFER_SIZE];

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_NETWORK_DATA_NODES_TO_NETWORK_DATA_NODES_DATABASE_SYNC_CHECK_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_network_data_nodes_to_network_data_nodes_database_sync_check",87); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(public_address,0,sizeof(public_address));

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_NETWORK_DATA_NODES_TO_NETWORK_DATA_NODES_DATABASE_SYNC_CHECK_ERROR("Could not verify the message");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"public_address",public_address,sizeof(public_address)) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NETWORK_DATA_NODES_TO_NETWORK_DATA_NODES_DATABASE_SYNC_CHECK_ERROR("Could not parse the message");
  }

  for (count = 0; count < NETWORK_DATA_NODES_AMOUNT; count++)
  {
    if (memcmp(network_data_nodes_list.network_data_nodes_public_address[count],public_address,XCASH_WALLET_LENGTH) == 0)
    {      
      memset(public_address,0,sizeof(public_address));
      parse_json_data(MESSAGE,"data_hash",public_address,DATA_HASH_LENGTH);
      if (strlen(public_address) == DATA_HASH_LENGTH)
      {
        memcpy(network_data_nodes_database_data[count],public_address,DATA_HASH_LENGTH);
      }      
      break;
    }
  }
  return 1;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_NETWORK_DATA_NODES_TO_NETWORK_DATA_NODES_DATABASE_SYNC_CHECK_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_node_to_network_data_nodes_get_current_block_verifiers_list
Description: Runs the code when the server receives the NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_nodes_to_block_verifiers_reserve_bytes_database_sync_check_all_update(const int CLIENT_SOCKET)
{
   // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char message[BUFFER_SIZE];

  // define macros
  #define DATABASE_COLLECTION "reserve_bytes"
  #define SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_nodes_to_block_verifiers_reserve_bytes_database_sync_check_all_update",96); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  send_data(CLIENT_SOCKET,(unsigned char*)"Could not get the reserve bytes data hash}",0,0,""); \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));

  // get the database data hash for the reserve bytes database
  if (get_database_data_hash(data2,DATABASE_NAME,DATABASE_COLLECTION) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not get the database data hash for the reserve bytes database");
  }

  // create the message
  memcpy(data,"BLOCK_VERIFIERS_TO_NODES_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD|",72);
  memcpy(data+72,data2,DATA_HASH_LENGTH);
  memcpy(data+200,"|",1);
  
  // sign_data
  memset(data2,0,sizeof(data2));
  memcpy(message,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"sign\",\"params\":{\"data\":\"",60);
  memcpy(message+60,data,strnlen(data,sizeof(message)));
  memcpy(message+strlen(message),"\"}}",3);

  if (send_http_request(data2,"127.0.0.1","/json_rpc",XCASH_WALLET_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,message,RECEIVE_DATA_TIMEOUT_SETTINGS,"sign data",0) <= 0)
  {  
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not create the message");
  } 

  memset(message,0,sizeof(message));

  if (parse_json_data(data2,"signature",message,sizeof(message)) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not create the message");
  }

  // check if the returned data is valid
  if (strlen(message) != XCASH_SIGN_DATA_LENGTH && strncmp(message,XCASH_SIGN_DATA_PREFIX,sizeof(XCASH_SIGN_DATA_PREFIX)-1) != 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not create the message");
  }

  memcpy(data+strlen(data),xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"|",1);
  memcpy(data+strlen(data),message,XCASH_SIGN_DATA_LENGTH);
  memcpy(data+strlen(data),"|}",2);

  // send the data
  if (send_data(CLIENT_SOCKET,(unsigned char*)data,0,0,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not send the BLOCK_VERIFIERS_TO_NODES_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD message to the node");
  }
  return 1;

  #undef DATABASE_COLLECTION
  #undef SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_node_to_block_verifiers_get_reserve_bytes
Description: Runs the code when the server receives the NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES message
Parameters:
  CLIENT_SOCKET - The socket to send data to
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_node_to_block_verifiers_get_reserve_bytes(const int CLIENT_SOCKET, const char* MESSAGE)
{  
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char message[BUFFER_SIZE];
  char* message2 = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  size_t count;
  size_t count2;
  size_t current_block_height_reserve_bytes;
  size_t reserve_bytes_blocks_amount;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_node_to_block_verifiers_get_reserve_bytes",68); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  pointer_reset(message2); \
  send_data(CLIENT_SOCKET,(unsigned char*)"Could not get the network blocks reserve bytes}",0,0,""); \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (message2 == NULL)
  {
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_node_to_block_verifiers_get_reserve_bytes",68);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,data2);  
    exit(0);
  }

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));
  
  if (parse_json_data(MESSAGE,"block_height",data,sizeof(data)) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_ERROR("Could not create the message");
  }

  // calculate how many blocks to send
  sscanf(current_block_height,"%zu",&count2);
  sscanf(data,"%zu",&current_block_height_reserve_bytes);
  if (current_block_height_reserve_bytes < XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_ERROR("Invalid block height");
  }
  reserve_bytes_blocks_amount = (count2 - current_block_height_reserve_bytes) + 1;
  if (reserve_bytes_blocks_amount > BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME)
  {
    reserve_bytes_blocks_amount = BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME;
  }

  // create the message
  memcpy(message2,"BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES|",43);

  for (count = 0; count < reserve_bytes_blocks_amount; count++, current_block_height_reserve_bytes++)
  {
    // create the message
    memset(data,0,sizeof(data));
    memset(data2,0,sizeof(data2));
    memset(message,0,sizeof(message));
    memcpy(data2,"{\"block_height\": \"",18);
    snprintf(data2+18,sizeof(data2)-19,"%zu",current_block_height_reserve_bytes);
    memcpy(data2+strlen(data2),"\"}",2);
  
    count2 = ((current_block_height_reserve_bytes - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME) + 1;
    memcpy(data,"reserve_bytes_",14);
    snprintf(data+14,sizeof(data)-15,"%zu",count2);

    // get the data hash
    if (read_document_field_from_collection(DATABASE_NAME,data,data2,"reserve_bytes",message,1) == 0)
    {
      SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_ERROR("Could not get the previous blocks reserve bytes");
    }
  
    memcpy(message2+strlen(message2),message,strnlen(message,MAXIMUM_BUFFER_SIZE));
    memcpy(message2+strlen(message2),"|",1);
  }

  memcpy(message2+strlen(message2),"}",1);
  
  // send the data
  if (send_data(CLIENT_SOCKET,(unsigned char*)message2,0,0,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_ERROR("Could not send the BLOCK_VERIFIERS_TO_NODES_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD message to the node");
  }
  pointer_reset(message2);
  return 1;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_node_to_block_verifiers_get_reserve_bytes_data_hash
Description: Runs the code when the server receives the NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATA_HASH message
Parameters:
  CLIENT_SOCKET - The socket to send data to
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_node_to_block_verifiers_get_reserve_bytes_data_hash(const int CLIENT_SOCKET, const char* MESSAGE)
{  
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char message[BUFFER_SIZE];
  size_t count;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATA_HASH_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_node_to_block_verifiers_get_reserve_bytes_data_hash",78); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  send_data(CLIENT_SOCKET,(unsigned char*)"Could not get the network blocks reserve bytes data hash}",0,0,""); \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));
  
  if (parse_json_data(MESSAGE,"block_height",data,sizeof(data)) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATA_HASH_ERROR("Could not create the message");
  }

  // get the blocks data hash
  memcpy(data2,"{\"block_height\": \"",18);
  memcpy(data2+18,data,strnlen(data,sizeof(data2)));
  memcpy(data2+strlen(data2),"\"}",2);

  get_reserve_bytes_database(count,0);
  memset(data,0,sizeof(data));
  memcpy(data,"reserve_bytes_",14);
  snprintf(data+14,sizeof(data)-15,"%zu",count);

  if (read_document_field_from_collection(DATABASE_NAME,data,data2,"reserve_bytes_data_hash",message,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATA_HASH_ERROR("Could not get the network blocks reserve bytes data hash");
  }

  // create the message
  memset(data,0,sizeof(data));
  memcpy(data,"BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES_DATA_HASH|",53);
  memcpy(data+53,message,DATA_HASH_LENGTH);
  memcpy(data+strlen(data),"|}",2);

  // send the data
  if (send_data(CLIENT_SOCKET,(unsigned char*)data,0,0,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATA_HASH_ERROR("Could not send the network blocks reserve bytes data hash");
  }
  return 1;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATA_HASH_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_node_to_block_verifiers_check_if_current_block_verifier
Description: Runs the code when the server receives the NODE_TO_BLOCK_VERIFIERS_CHECK_IF_CURRENT_BLOCK_VERIFIER message
Parameters:
  CLIENT_SOCKET - The socket to send data to
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_node_to_block_verifiers_check_if_current_block_verifier(const int CLIENT_SOCKET)
{  
  // Variables
  char data[BUFFER_SIZE];
  int count;

  memset(data,0,sizeof(data));
  memcpy(data,"0}",2);

  for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++)
  {
    if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0)
    {
      memset(data,0,sizeof(data));
      memcpy(data,"1}",2);
      break;
    }    
  }

  send_data(CLIENT_SOCKET,(unsigned char*)data,0,0,"");
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_all_update
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_all_update(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char message[BUFFER_SIZE];
  char reserve_proofs_database[BUFFER_SIZE];
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  size_t count;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_all_update",107); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(message,0,sizeof(message));
  memset(reserve_proofs_database,0,sizeof(reserve_proofs_database));
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // parse the message
  if (parse_json_data(MESSAGE,"reserve_proofs_data_hash",data,sizeof(data)) == 0 || strlen(data) != DATA_HASH_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not parse the message");
  }

  // get the database data hash for the reserve proofs database
  memset(data2,0,strlen(data2));
  if (get_database_data_hash(data2,DATABASE_NAME,"reserve_proofs") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not get the database data hash for the reserve proofs database");
  }

  // create the message
  if (memcmp(data,data2,DATA_HASH_LENGTH) == 0)
  {
    memset(message,0,strlen(message));
    memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"reserve_proofs_database\": \"true\",\r\n \"reserve_proofs_database_1\": \"true\",\r\n \"reserve_proofs_database_2\": \"true\",\r\n \"reserve_proofs_database_3\": \"true\",\r\n \"reserve_proofs_database_4\": \"true\",\r\n \"reserve_proofs_database_5\": \"true\",\r\n \"reserve_proofs_database_6\": \"true\",\r\n \"reserve_proofs_database_7\": \"true\",\r\n \"reserve_proofs_database_8\": \"true\",\r\n \"reserve_proofs_database_9\": \"true\",\r\n \"reserve_proofs_database_10\": \"true\",\r\n \"reserve_proofs_database_11\": \"true\",\r\n \"reserve_proofs_database_12\": \"true\",\r\n \"reserve_proofs_database_13\": \"true\",\r\n \"reserve_proofs_database_14\": \"true\",\r\n \"reserve_proofs_database_15\": \"true\",\r\n \"reserve_proofs_database_16\": \"true\",\r\n \"reserve_proofs_database_17\": \"true\",\r\n \"reserve_proofs_database_18\": \"true\",\r\n \"reserve_proofs_database_19\": \"true\",\r\n \"reserve_proofs_database_20\": \"true\",\r\n \"reserve_proofs_database_21\": \"true\",\r\n \"reserve_proofs_database_22\": \"true\",\r\n \"reserve_proofs_database_23\": \"true\",\r\n \"reserve_proofs_database_24\": \"true\",\r\n \"reserve_proofs_database_25\": \"true\",\r\n \"reserve_proofs_database_26\": \"true\",\r\n \"reserve_proofs_database_27\": \"true\",\r\n \"reserve_proofs_database_28\": \"true\",\r\n \"reserve_proofs_database_29\": \"true\",\r\n \"reserve_proofs_database_30\": \"true\",\r\n \"reserve_proofs_database_31\": \"true\",\r\n \"reserve_proofs_database_32\": \"true\",\r\n \"reserve_proofs_database_33\": \"true\",\r\n \"reserve_proofs_database_34\": \"true\",\r\n \"reserve_proofs_database_35\": \"true\",\r\n \"reserve_proofs_database_36\": \"true\",\r\n \"reserve_proofs_database_37\": \"true\",\r\n \"reserve_proofs_database_38\": \"true\",\r\n \"reserve_proofs_database_39\": \"true\",\r\n \"reserve_proofs_database_40\": \"true\",\r\n \"reserve_proofs_database_41\": \"true\",\r\n \"reserve_proofs_database_42\": \"true\",\r\n \"reserve_proofs_database_43\": \"true\",\r\n \"reserve_proofs_database_44\": \"true\",\r\n \"reserve_proofs_database_45\": \"true\",\r\n \"reserve_proofs_database_46\": \"true\",\r\n \"reserve_proofs_database_47\": \"true\",\r\n \"reserve_proofs_database_48\": \"true\",\r\n \"reserve_proofs_database_49\": \"true\",\r\n \"reserve_proofs_database_50\": \"true\",\r\n}",2140);
  }
  else
  {
    memset(message,0,sizeof(message));
    memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"reserve_proofs_database\": \"false\",\r\n ",150);
    for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
    {
      memcpy(message+strlen(message),"\"reserve_proofs_database_",25);
      snprintf(message+strlen(message),sizeof(message)-1,"%zu",count);
      memcpy(message+strlen(message),"\": \"",4);      
      memset(data2,0,strlen(data2));  
      memcpy(data2,"reserve_proofs_data_hash_",25);  
      snprintf(data2+25,sizeof(data2)-26,"%zu",count); 
      // parse the message
      if (parse_json_data(MESSAGE,data2,data,sizeof(data)) == 0 || strlen(data) != DATA_HASH_LENGTH)
      {
        SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not parse the message");
      }
      // get the database data hash for the reserve proofs database
      memset(data2,0,sizeof(data2));  
      memcpy(data2,"reserve_proofs_",15);  
      snprintf(data2+15,sizeof(data2)-16,"%zu",count);
      if (get_database_data_hash(reserve_proofs_database,DATABASE_NAME,data2) == 0)
      {
        SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not get the database data hash for the reserve proofs database");
      }
      if (memcmp(reserve_proofs_database,data,DATA_HASH_LENGTH) == 0)
      {
        memcpy(message+strlen(message),"true",4);
      }
      else
      {
        memcpy(message+strlen(message),"false",5);
      }
      memcpy(message+strlen(message),"\",\r\n",4);
    }
    memcpy(message+strlen(message),"}",1);
  }
  
  // sign_data
  if (sign_data(message,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,(unsigned char*)message,0,1,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not send the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD message to the block verifier");
  }
  return 1;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_update
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_update(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];

  // define macros
  #define DATABASE_MESSAGE_SYNCED_TRUE "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"reserve_proofs_database\": \"true\",\r\n}"
  #define DATABASE_MESSAGE_SYNCED_FALSE "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"reserve_proofs_database\": \"false\",\r\n}"
  
  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_update",103); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not verify the message");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"file",data,sizeof(data)) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not parse the message");
  }

  // get the database data hash for the reserve proofs database
  memset(data2,0,sizeof(data2));
  if (get_database_data_hash(data2,DATABASE_NAME,data) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not get the database data hash for the reserve proofs database");
  }

  // parse the message
  memset(data,0,sizeof(data));
  if (parse_json_data(MESSAGE,"data_hash",data,sizeof(data)) == 0 || strlen(data) != DATA_HASH_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not parse the message");
  }

  // create the message
  if (strncmp(data,data2,DATA_HASH_LENGTH) == 0)
  {
    memset(data,0,strlen(data));
    memcpy(data,DATABASE_MESSAGE_SYNCED_TRUE,sizeof(DATABASE_MESSAGE_SYNCED_TRUE)-1);
  }
  else
  {
    memset(data,0,strlen(data));
    memcpy(data,DATABASE_MESSAGE_SYNCED_FALSE,sizeof(DATABASE_MESSAGE_SYNCED_FALSE)-1);
  }
  
  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,(unsigned char*)data,0,1,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not send the NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST message to the block verifier");
  }

  return 1;

  #undef DATABASE_MESSAGE_SYNCED_TRUE
  #undef DATABASE_MESSAGE_SYNCED_FALSE
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_download_file_update
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_download_file_update(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char buffer[1024];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_download_file_update",106); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_download_file_update",106);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,buffer);  
    exit(0);
  }

  // parse the message
  if (parse_json_data(MESSAGE,"file",data,MAXIMUM_BUFFER_SIZE) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not parse the message");
  }

  // get the database data for the reserve proofs database
  memset(data2,0,strlen(data2));
  if (get_database_data(data2,DATABASE_NAME,data,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not get the database data hash for the reserve proofs database");
  }

  // create the message
  memset(data,0,strlen(data));
  memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"reserve_proofs_database\": \"",139);
  memcpy(data+139,data2,strnlen(data2,MAXIMUM_BUFFER_SIZE));
  memcpy(data+strlen(data),"\",\r\n}",5);

  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,(unsigned char*)data,0,1,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not send the NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST message to the block verifier");
  }

  pointer_reset_all;
  return 1;

  #undef pointer_reset_all
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_all_update
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_all_update(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char message[BUFFER_SIZE];
  char reserve_bytes_database[BUFFER_SIZE];
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  size_t count;
  size_t current_reserve_bytes_database;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_all_update",106); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(message,0,sizeof(message));
  memset(reserve_bytes_database,0,sizeof(reserve_bytes_database));
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // parse the message
  if (parse_json_data(MESSAGE,"reserve_bytes_data_hash",data,sizeof(data)) == 0 || strlen(data) != DATA_HASH_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not parse the message");
  }

  // get the current reserve bytes database
  get_reserve_bytes_database(current_reserve_bytes_database,0);

  // get the database data hash for the reserve bytes database
  memset(data2,0,strlen(data2));
  if (get_database_data_hash(data2,DATABASE_NAME,"reserve_bytes") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not get the database data hash for the reserve proofs database");
  }

  // create the message
  if (memcmp(data,data2,DATA_HASH_LENGTH) == 0)
  {
    memset(message,0,strlen(message));
    memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"reserve_bytes_database\": \"true\",\r\n ",147);
  }
  else
  {
    memset(message,0,sizeof(message));
    memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"reserve_bytes_database\": \"false\",\r\n ",148);
  }
  
  for (count = 1; count <= current_reserve_bytes_database; count++)
  {
    memcpy(message+strlen(message),"\"reserve_bytes_database_",24);
    snprintf(message+strlen(message),sizeof(message)-1,"%zu",count);
    memcpy(message+strlen(message),"\": \"",4);      
    memset(data2,0,strlen(data2));  
    memcpy(data2,"reserve_bytes_data_hash_",24);  
    snprintf(data2+24,sizeof(data2)-25,"%zu",count); 
    // parse the message
    if (parse_json_data(MESSAGE,data2,data,sizeof(data)) == 0 || strlen(data) != DATA_HASH_LENGTH)
    {
      SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not parse the message");
    }
    // get the database data hash for the reserve proofs database
    memset(data2,0,sizeof(data2));  
    memcpy(data2,"reserve_bytes_",14);  
    snprintf(data2+14,sizeof(data2)-15,"%zu",count);
    if (get_database_data_hash(reserve_bytes_database,DATABASE_NAME,data2) == 0)
    {
      SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not get the database data hash for the reserve proofs database");
    }
    if (memcmp(reserve_bytes_database,data,DATA_HASH_LENGTH) == 0)
    {
      memcpy(message+strlen(message),"true",4);
    }
    else
    {
      memcpy(message+strlen(message),"false",5);
    }
    memcpy(message+strlen(message),"\",\r\n",4);
  }
  memcpy(message+strlen(message),"}",1);
  
  // sign_data
  if (sign_data(message,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,(unsigned char*)message,0,1,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not send the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD message to the block verifier");
  }
  return 1;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_update
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_update(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];

  // define macros
  #define DATABASE_MESSAGE_SYNCED_TRUE "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"reserve_bytes_database\": \"true\",\r\n}"
  #define DATABASE_MESSAGE_SYNCED_FALSE "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"reserve_bytes_database\": \"false\",\r\n}"
  
  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_update",102); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // parse the message
  if (parse_json_data(MESSAGE,"file",data,sizeof(data)) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not parse the message");
  }

  // get the database data hash for the reserve bytes database
  memset(data2,0,sizeof(data2));
  if (get_database_data_hash(data2,DATABASE_NAME,data) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not get the database data hash for the reserve bytes database");
  }

  // parse the message
  memset(data,0,sizeof(data));
  if (parse_json_data(MESSAGE,"data_hash",data,sizeof(data)) == 0 || strlen(data) != DATA_HASH_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not parse the message");
  }

  // create the message
  if (memcmp(data,data2,DATA_HASH_LENGTH) == 0)
  {
    memset(data,0,sizeof(data));
    memcpy(data,DATABASE_MESSAGE_SYNCED_TRUE,strnlen(DATABASE_MESSAGE_SYNCED_TRUE,BUFFER_SIZE));
  }
  else
  {
    memset(data,0,sizeof(data));
    memcpy(data,DATABASE_MESSAGE_SYNCED_FALSE,strnlen(DATABASE_MESSAGE_SYNCED_FALSE,BUFFER_SIZE));
  }
  
  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,(unsigned char*)data,0,1,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not send the NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST message to the block verifier");
  }

  return 1;

  #undef DATABASE_MESSAGE_SYNCED_TRUE
  #undef DATABASE_MESSAGE_SYNCED_FALSE
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_download_file_update
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_download_file_update(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char buffer[1024];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_download_file_update",105); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_download_file_update",105);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,buffer);  
    exit(0);
  }

  // parse the message
  if (parse_json_data(MESSAGE,"file",data,MAXIMUM_BUFFER_SIZE) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not parse the message");
  }

  // get the database data for the reserve bytes database
  memset(data2,0,strlen(data2));
  if (get_database_data(data2,DATABASE_NAME,data,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not get the database data hash for the reserve bytes database");
  }

  // create the message
  memset(data,0,strlen(data));
  memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"reserve_bytes_database\": \"",137);
  memcpy(data+137,data2,strnlen(data2,MAXIMUM_BUFFER_SIZE));
  memcpy(data+strlen(data),"\",\r\n}",5);

  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,(unsigned char*)data,0,1,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not send the NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST message to the block verifier");
  }

  pointer_reset_all;
  return 1;

  #undef pointer_reset_all
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_sync_check_update
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_sync_check_update(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_sync_check_update",98); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // get the database data hash for the delegates database
  memset(data2,0,strlen(data2));
  if (get_database_data_hash(data2,DATABASE_NAME,DATABASE_COLLECTION) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not get the database data hash for the reserve bytes database");
  }

  // parse the message
  memset(data,0,sizeof(data));
  if (parse_json_data(MESSAGE,"data_hash",data,sizeof(data)) == 0 || strlen(data) != DATA_HASH_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not parse the message");
  }

  // create the message
  if (memcmp(data,data2,DATA_HASH_LENGTH) == 0)
  {
    memset(data,0,sizeof(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"delegates_database\": \"true\",\r\n}",135);
  }
  else
  {
    memset(data,0,sizeof(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"delegates_database\": \"false\",\r\n}",136);
  }
  
  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,(unsigned char*)data,0,1,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not send the NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST message to the block verifier");
  }

  return 1;

  #undef DATABASE_COLLECTION
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_download_file_update
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_download_file_update(const int CLIENT_SOCKET)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char buffer[1024];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_download_file_update",101); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  memset(buffer,0,sizeof(buffer));

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_download_file_update",101);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,buffer);  
    exit(0);
  }

  // get the database data for the reserve bytes database
  memset(data2,0,strlen(data2));
  if (get_database_data(data2,DATABASE_NAME,DATABASE_COLLECTION,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not get the database data hash for the reserve bytes database");
  }

  // create the message
  memset(data,0,strlen(data));
  memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"delegates_database\": \"",129);
  memcpy(data+129,data2,strnlen(data2,MAXIMUM_BUFFER_SIZE));
  memcpy(data+strlen(data),"\",\r\n}",5);

  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,(unsigned char*)data,0,1,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not send the NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST message to the block verifier");
  }

  pointer_reset_all;
  return 1;

  #undef DATABASE_COLLECTION
  #undef pointer_reset_all
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_sync_check_update
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_sync_check_update(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];

  // define macros
  #define DATABASE_COLLECTION "statistics"
  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_sync_check_update",99); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // get the database data hash for the statistics database
  memset(data2,0,sizeof(data2));
  if (get_database_data_hash(data2,DATABASE_NAME,DATABASE_COLLECTION) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not get the database data hash for the reserve bytes database");
  }

  // parse the message
  memset(data,0,sizeof(data));
  if (parse_json_data(MESSAGE,"data_hash",data,sizeof(data)) == 0 || strlen(data) != DATA_HASH_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not parse the message");
  }

  // create the message
  if (memcmp(data,data2,DATA_HASH_LENGTH) == 0)
  {
    memset(data,0,sizeof(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"statistics_database\": \"true\",\r\n}",137);
  }
  else
  {
    memset(data,0,sizeof(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"statistics_database\": \"false\",\r\n}",138);
  }
  
  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,(unsigned char*)data,0,1,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not send the NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST message to the block verifier");
  }

  return 1;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_download_file_update
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_download_file_update(const int CLIENT_SOCKET)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char buffer[1024];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;

  // define macros
  #define DATABASE_COLLECTION "statistics"
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_download_file_update",102); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_download_file_update",102);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,buffer);  
    exit(0);
  }

  // get the database data for the reserve bytes database
  memset(data2,0,strlen(data2));
  if (get_database_data(data2,DATABASE_NAME,DATABASE_COLLECTION,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not get the database data hash for the reserve bytes database");
  }

  // create the message
  memset(data,0,strlen(data));
  memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"statistics_database\": \"",131);
  memcpy(data+131,data2,strnlen(data2,MAXIMUM_BUFFER_SIZE));
  memcpy(data+strlen(data),"\",\r\n}",5);

  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,(unsigned char*)data,0,1,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not send the NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST message to the block verifier");
  }

  pointer_reset_all;
  return 1;

  #undef DATABASE_COLLECTION
  #undef pointer_reset_all
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR
}