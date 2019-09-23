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
#include "structures.h"
#include "variables.h"

#include "blockchain_functions.h"
#include "database_functions.h"
#include "file_functions.h"
#include "network_daemon_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "network_wallet_functions.h"
#include "server_functions.h"
#include "block_verifiers_synchronize_functions.h"
#include "delegates_website_and_shared_delegates_website_functions.h"
#include "string_functions.h"
#include "thread_functions.h"
#include "thread_server_functions.h"
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
Name: check_if_databases_are_synced
Description: Checks if the databases are synced, and if not syncs the databases
Paramters:
  SETTINGS - 1 to sync from a random block verifier, 2 to sync from a random network data node
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int check_if_databases_are_synced(const int SETTINGS)
{
  // Variables
  char data[BUFFER_SIZE];
  size_t count;

  // define macros
  #define CHECK_IF_DATABASES_ARE_SYNCED_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"check_if_databases_are_synced",29); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));

  print_start_message("Checking if databases are synced");

  // get the previous block height
  sscanf(current_block_height, "%zu", &count);
  count--;
  snprintf(data,sizeof(data)-1,"%zu",count);

  // check if your reserve proofs database is synced
  if (sync_check_reserve_proofs_database(SETTINGS) == 0)
  {
    CHECK_IF_DATABASES_ARE_SYNCED_ERROR("Could not check if the reserve proofs database is updated. This means you might need to sync the reserve proofs database.");
  }

  // check if your reserve bytes database is synced
  if (sync_check_reserve_bytes_database(SETTINGS) == 0)
  {    
    CHECK_IF_DATABASES_ARE_SYNCED_ERROR("Could not check if the reserve bytes database is updated. This means you might need to sync the reserve bytes database.");
  }

  // check if your delegates database is synced
  if (sync_check_delegates_database(SETTINGS) == 0)
  {
    CHECK_IF_DATABASES_ARE_SYNCED_ERROR("Could not check if the delegates database is updated. This means you might need to sync the delegates database.");
  }

  // check if your statistics database is synced
  if (sync_check_statistics_database(SETTINGS) == 0)
  {
    CHECK_IF_DATABASES_ARE_SYNCED_ERROR("Could not check if the statistics database is updated. This means you might need to sync the statistics database.");
  }
  return 1;

  #undef CHECK_IF_DATABASES_ARE_SYNCED_ERROR
}



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

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_node_to_network_data_nodes_get_previous_current_next_block_verifiers_list",100); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));

  // create the message
  memcpy(data,"{\r\n \"message_settings\": \"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST\",\r\n \"previous_block_verifiers_name_list\": \"",142);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),previous_block_verifiers_list.block_verifiers_name[count2],strnlen(previous_block_verifiers_list.block_verifiers_name[count2],sizeof(data)));
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n \"previous_block_verifiers_public_address_list\": \"",54);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),previous_block_verifiers_list.block_verifiers_public_address[count2],strnlen(previous_block_verifiers_list.block_verifiers_public_address[count2],sizeof(data)));
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n \"previous_block_verifiers_IP_address_list\": \"",50);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),previous_block_verifiers_list.block_verifiers_IP_address[count2],strnlen(previous_block_verifiers_list.block_verifiers_IP_address[count2],sizeof(data)));
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n \"current_block_verifiers_name_list\": \"",43);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),current_block_verifiers_list.block_verifiers_name[count2],strnlen(current_block_verifiers_list.block_verifiers_name[count2],sizeof(data)));
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n \"current_block_verifiers_public_address_list\": \"",54);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),current_block_verifiers_list.block_verifiers_public_address[count2],strnlen(current_block_verifiers_list.block_verifiers_public_address[count2],sizeof(data)));
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n \"current_block_verifiers_IP_address_list\": \"",50);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),current_block_verifiers_list.block_verifiers_IP_address[count2],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count2],sizeof(data)));
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n \"next_block_verifiers_name_list\": \"",43);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),next_block_verifiers_list.block_verifiers_name[count2],strnlen(next_block_verifiers_list.block_verifiers_name[count2],sizeof(data)));
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n \"next_block_verifiers_public_address_list\": \"",54);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),next_block_verifiers_list.block_verifiers_public_address[count2],strnlen(next_block_verifiers_list.block_verifiers_public_address[count2],sizeof(data)));
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n \"next_block_verifiers_IP_address_list\": \"",50);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),next_block_verifiers_list.block_verifiers_IP_address[count2],strnlen(next_block_verifiers_list.block_verifiers_IP_address[count2],sizeof(data)));
    memcpy(data+strlen(data),"|",1);
  }
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
  size_t count2;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_node_to_network_data_nodes_get_current_block_verifiers_list",86); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;
  
  memset(data,0,sizeof(data));

  // create the message
  memcpy(data,"{\r\n \"message_settings\": \"NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST\",\r\n \"block_verifiers_public_address_list\": \"",129);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),current_block_verifiers_list.block_verifiers_public_address[count2],XCASH_WALLET_LENGTH);
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n \"block_verifiers_IP_address_list\": \"",41);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),current_block_verifiers_list.block_verifiers_IP_address[count2],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count2],sizeof(data)));
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
  if (get_database_data_hash(data2,DATABASE_NAME,DATABASE_COLLECTION,0) == 0)
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
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char message[BUFFER_SIZE];
  char message2[BUFFER_SIZE];
  size_t count;
  size_t count2;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_node_to_block_verifiers_get_reserve_bytes",68); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  send_data(CLIENT_SOCKET,(unsigned char*)"Could not get the network blocks reserve bytes}",0,0,""); \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));
  memset(message2,0,sizeof(message2));
  
  if (parse_json_data(MESSAGE,"block_height",data,sizeof(data)) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_ERROR("Could not create the message");
  }

  // create the message
  memcpy(data2,"{\"block_height\": \"",18);
  memcpy(data2+18,data,strnlen(data,sizeof(data2)));
  memcpy(data2+strlen(data2),"\"}",2);

  sscanf(data,"%zu", &count);
  if (count < XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT-1)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_ERROR("Could not get the current block height");
  }
  count2 = ((count - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME) + 1;
  memset(data,0,sizeof(data));
  memcpy(data,"reserve_bytes_",14);
  snprintf(data+14,sizeof(data)-15,"%zu",count2);

  // get the data hash
  if (read_document_field_from_collection(DATABASE_NAME,data,data2,"reserve_bytes",message,0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_ERROR("Could not get the previous blocks reserve bytes");
  }

  // create the message
  memcpy(message2,"BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES|",43);
  memcpy(message2+43,message,strnlen(message,sizeof(message2)));
  memcpy(message2+strlen(message2),"|",1);
  
  // sign_data
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memcpy(data,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"sign\",\"params\":{\"data\":\"",60);
  memcpy(data+60,message2,strnlen(message2,sizeof(data)));
  memcpy(data+strlen(data),"\"}}",3);

  if (send_http_request(data2,"127.0.0.1","/json_rpc",XCASH_WALLET_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,data,RECEIVE_DATA_TIMEOUT_SETTINGS,"sign data",0) <= 0)
  {  
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_ERROR("Could not create the message");
  } 

  memset(data,0,sizeof(data));

  if (parse_json_data(data2,"signature",data,sizeof(data)) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_ERROR("Could not create the message");
  }

  // check if the returned data is valid
  if (strlen(data) != XCASH_SIGN_DATA_LENGTH && strncmp(data,XCASH_SIGN_DATA_PREFIX,sizeof(XCASH_SIGN_DATA_PREFIX)-1) != 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_ERROR("Could not create the message");
  }

  memcpy(message2+strlen(message2),xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(message2+strlen(message2),"|",1);
  memcpy(message2+strlen(message2),data,XCASH_SIGN_DATA_LENGTH);
  memcpy(message2+strlen(message2),"|}",2);

  // send the data
  if (send_data(CLIENT_SOCKET,(unsigned char*)message2,0,0,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_ERROR("Could not send the BLOCK_VERIFIERS_TO_NODES_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD message to the node");
  }
  return 1;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_ERROR
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

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not verify the message");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"reserve_proofs_data_hash",data,sizeof(data)) == 0 || strlen(data) != DATA_HASH_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not parse the message");
  }

  // get the database data hash for the reserve proofs database
  memset(data2,0,strlen(data2));
  if (get_database_data_hash(data2,DATABASE_NAME,"reserve_proofs",0) == 0)
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
      if (get_database_data_hash(reserve_proofs_database,DATABASE_NAME,data2,0) == 0)
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
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not send the NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST message to the block verifier");
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
  if (get_database_data_hash(data2,DATABASE_NAME,data,0) == 0)
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
    print_error_message;  
    exit(0);
  }

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not verify the message");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"file",data,MAXIMUM_BUFFER_SIZE) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not parse the message");
  }

  // get the database data for the reserve proofs database
  memset(data2,0,strlen(data2));
  if (get_database_data(data2,DATABASE_NAME,data,0) == 0)
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
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_all_update",106); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not verify the message");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"data_hash",data,sizeof(data)) == 0 || strlen(data) != DATA_HASH_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not parse the message");
  }

  // get the database data hash for the reserve bytes database
  memset(data2,0,sizeof(data2));
  if (get_database_data_hash(data2,DATABASE_NAME,"reserve_bytes",0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not get the database data hash for the reserve bytes database");
  }

  // create the message
  if (memcmp(data,data2,DATA_HASH_LENGTH) == 0)
  {
    memset(data,0,sizeof(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"reserve_bytes_database\": \"true\",\r\n}",147);
  }
  else
  {
    memset(data,0,sizeof(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"reserve_bytes_database\": \"false\",\r\n}",148);
  }
  
  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,(unsigned char*)data,0,1,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not send the NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST message to the block verifier");
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

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not verify the message");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"file",data,sizeof(data)) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not parse the message");
  }

  // get the database data hash for the reserve bytes database
  memset(data2,0,sizeof(data2));
  if (get_database_data_hash(data2,DATABASE_NAME,data,0) == 0)
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
    print_error_message;  
    exit(0);
  }

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not verify the message");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"file",data,MAXIMUM_BUFFER_SIZE) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not parse the message");
  }

  // get the database data for the reserve bytes database
  memset(data2,0,strlen(data2));
  if (get_database_data(data2,DATABASE_NAME,data,0) == 0)
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

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not verify the message");
  }

  // get the database data hash for the delegates database
  memset(data2,0,strlen(data2));
  if (get_database_data_hash(data2,DATABASE_NAME,DATABASE_COLLECTION,0) == 0)
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

int server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_download_file_update(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));

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
    print_error_message;  
    exit(0);
  }

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not verify the message");
  }

  // get the database data for the reserve bytes database
  memset(data2,0,strlen(data2));
  if (get_database_data(data2,DATABASE_NAME,DATABASE_COLLECTION,0) == 0)
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

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not verify the message");
  }

  // get the database data hash for the statistics database
  memset(data2,0,sizeof(data2));
  if (get_database_data_hash(data2,DATABASE_NAME,DATABASE_COLLECTION,0) == 0)
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

int server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_download_file_update(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));

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
    print_error_message;  
    exit(0);
  }

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not verify the message");
  }

  // get the database data for the reserve bytes database
  memset(data2,0,strlen(data2));
  if (get_database_data(data2,DATABASE_NAME,DATABASE_COLLECTION,0) == 0)
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

  return 1;

  #undef DATABASE_COLLECTION
  #undef pointer_reset_all
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_all_block_verifiers_list
Description: Sync the previous, current and next block verifiers from a network data node. This function is only run at startup, since after that the database is used to get the block verifiers list
-----------------------------------------------------------------------------------------------------------
*/

int sync_all_block_verifiers_list()
{
  // Variables
  struct database_multiple_documents_fields database_multiple_documents_fields;
  char message[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char* data3 = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  size_t count;
  size_t count2;

  // define macros
  #define SYNC_ALL_BLOCK_VERIFIERS_LIST_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"sync_all_block_verifiers_list",29); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(message,0,sizeof(message));
  memset(data2,0,sizeof(data2));

  // reset the previous current and next block verifiers list
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memset(previous_block_verifiers_list.block_verifiers_name[count],0,sizeof(previous_block_verifiers_list.block_verifiers_name[count]));
    memset(previous_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_public_address[count]));
    memset(previous_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_IP_address[count]));
    memset(current_block_verifiers_list.block_verifiers_name[count],0,sizeof(current_block_verifiers_list.block_verifiers_name[count]));
    memset(current_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_public_address[count]));
    memset(current_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_IP_address[count]));
    memset(next_block_verifiers_list.block_verifiers_name[count],0,sizeof(next_block_verifiers_list.block_verifiers_name[count]));
    memset(next_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_public_address[count]));
    memset(next_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_IP_address[count]));
  }

  if (network_data_node_settings == 0)
  {
    print_start_message("Syncing the previous, current and next block verifiers directly from a network data node, to be able to update the database");

    // create the message
    memcpy(message,"{\r\n \"message_settings\": \"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST\",\r\n}",103);

    start:
    fprintf(stderr,"Connecting to a random network data node to get a list of previous, current and next block verifiers\n");

    // send the message to a random network data node
    do
    {
      count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT);
    } while (strncmp(network_data_nodes_list.network_data_nodes_IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0);
  
    // get the current time
    get_current_UTC_time;
    
    memcpy(data3,"Connecting to network data node ",32);
    memcpy(data3+32,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],MAXIMUM_BUFFER_SIZE));
    memcpy(data3+strlen(data3)," and sending NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST\n",87);
    memcpy(data3+strlen(data3),asctime(current_UTC_date_and_time),strlen(asctime(current_UTC_date_and_time)));
    fprintf(stderr,"%s\n",data3);
    memset(data3,0,strlen(data3));

    if (send_and_receive_data_socket(data3,network_data_nodes_list.network_data_nodes_IP_address[count],SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
    {
      memcpy(data2,"Could not receive data from network data node ",46);
      memcpy(data2+46,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],MAXIMUM_BUFFER_SIZE));
      color_print(data2,"red");
      memset(data2,0,sizeof(data2));
      fprintf(stderr,"Connecting to a different network data node\n\n");
      goto start;
    }

    if (verify_data(data3,0,0) == 0)
    {
      SYNC_ALL_BLOCK_VERIFIERS_LIST_ERROR("Could not verify data");
    }
 
    // parse the message
    memset(data2,0,sizeof(data2));
    if (parse_json_data(data3,"previous_block_verifiers_name_list",data2,sizeof(data2)) == 0)
    {
      SYNC_ALL_BLOCK_VERIFIERS_LIST_ERROR("Could not parse the message");
    } 

    // parse the previous_block_verifiers_name_list
    for (count = 0, count2 = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memcpy(previous_block_verifiers_list.block_verifiers_name[count],&data2[count2],strnlen(data2,sizeof(data2)) - strnlen(strstr(data2+count2,"|"),sizeof(data2)) - count2);
      count2 = strnlen(data2,sizeof(data2)) - strnlen(strstr(data2+count2,"|"),sizeof(data2)) + 1;
    }

    memset(data2,0,sizeof(data2));
    if (parse_json_data(data3,"previous_block_verifiers_public_address_list",data2,sizeof(data2)) == 0)
    {
      SYNC_ALL_BLOCK_VERIFIERS_LIST_ERROR("Could not parse the message");
    } 

    // parse the previous_block_verifiers_public_address_list
    for (count = 0, count2 = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memcpy(previous_block_verifiers_list.block_verifiers_public_address[count],&data2[count2],strnlen(data2,sizeof(data2)) - strnlen(strstr(data2+count2,"|"),sizeof(data2)) - count2);
      count2 = strnlen(data2,sizeof(data2)) - strnlen(strstr(data2+count2,"|"),sizeof(data2)) + 1;
    }

    memset(data2,0,sizeof(data2));
    if (parse_json_data(data3,"previous_block_verifiers_IP_address_list",data2,sizeof(data2)) == 0)
    {
      SYNC_ALL_BLOCK_VERIFIERS_LIST_ERROR("Could not parse the message");
    } 

    // parse the previous_block_verifiers_IP_address_list
    for (count = 0, count2 = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memcpy(previous_block_verifiers_list.block_verifiers_IP_address[count],&data2[count2],strnlen(data2,sizeof(data2)) - strnlen(strstr(data2+count2,"|"),sizeof(data2)) - count2);
      count2 = strnlen(data2,sizeof(data2)) - strnlen(strstr(data2+count2,"|"),sizeof(data2)) + 1;
    }

    // parse the message
    memset(data2,0,sizeof(data2));
    if (parse_json_data(data3,"current_block_verifiers_name_list",data2,sizeof(data2)) == 0)
    {
      SYNC_ALL_BLOCK_VERIFIERS_LIST_ERROR("Could not parse the message");
    } 

    // parse the current_block_verifiers_name_list
    for (count = 0, count2 = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memcpy(current_block_verifiers_list.block_verifiers_name[count],&data2[count2],strnlen(data2,sizeof(data2)) - strnlen(strstr(data2+count2,"|"),sizeof(data2)) - count2);
      count2 = strnlen(data2,sizeof(data2)) - strnlen(strstr(data2+count2,"|"),sizeof(data2)) + 1;
    }

    memset(data2,0,sizeof(data2));
    if (parse_json_data(data3,"current_block_verifiers_public_address_list",data2,sizeof(data2)) == 0)
    {
      SYNC_ALL_BLOCK_VERIFIERS_LIST_ERROR("Could not parse the message");
    } 

    // parse the current_block_verifiers_public_address_list
    for (count = 0, count2 = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memcpy(current_block_verifiers_list.block_verifiers_public_address[count],&data2[count2],strnlen(data2,sizeof(data2)) - strnlen(strstr(data2+count2,"|"),sizeof(data2)) - count2);
      count2 = strnlen(data2,sizeof(data2)) - strnlen(strstr(data2+count2,"|"),sizeof(data2)) + 1;
    }

    memset(data2,0,sizeof(data2));
    if (parse_json_data(data3,"current_block_verifiers_IP_address_list",data2,sizeof(data2)) == 0)
    {
      SYNC_ALL_BLOCK_VERIFIERS_LIST_ERROR("Could not parse the message");
    } 

    // parse the current_block_verifiers_IP_address_list
    for (count = 0, count2 = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memcpy(current_block_verifiers_list.block_verifiers_IP_address[count],&data2[count2],strnlen(data2,sizeof(data2)) - strnlen(strstr(data2+count2,"|"),sizeof(data2)) - count2);
      count2 = strnlen(data2,sizeof(data2)) - strnlen(strstr(data2+count2,"|"),sizeof(data2)) + 1;
    }

    // parse the message
    memset(data2,0,sizeof(data2));
    if (parse_json_data(data3,"next_block_verifiers_name_list",data2,sizeof(data2)) == 0)
    {
      SYNC_ALL_BLOCK_VERIFIERS_LIST_ERROR("Could not parse the message");
    } 

    // parse the next_block_verifiers_name_list
    for (count = 0, count2 = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memcpy(next_block_verifiers_list.block_verifiers_name[count],&data2[count2],strnlen(data2,sizeof(data2)) - strnlen(strstr(data2+count2,"|"),sizeof(data2)) - count2);
      count2 = strnlen(data2,sizeof(data2)) - strnlen(strstr(data2+count2,"|"),sizeof(data2)) + 1;
    }

    memset(data2,0,sizeof(data2));
    if (parse_json_data(data3,"next_block_verifiers_public_address_list",data2,sizeof(data2)) == 0)
    {
      SYNC_ALL_BLOCK_VERIFIERS_LIST_ERROR("Could not parse the message");
    } 

    // parse the next_block_verifiers_public_address_list
    for (count = 0, count2 = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memcpy(next_block_verifiers_list.block_verifiers_public_address[count],&data2[count2],strnlen(data2,sizeof(data2)) - strnlen(strstr(data2+count2,"|"),sizeof(data2)) - count2);
      count2 = strnlen(data2,sizeof(data2)) - strnlen(strstr(data2+count2,"|"),sizeof(data2)) + 1;
    }

    memset(data2,0,sizeof(data2));
    if (parse_json_data(data3,"next_block_verifiers_IP_address_list",data2,sizeof(data2)) == 0)
    {
      SYNC_ALL_BLOCK_VERIFIERS_LIST_ERROR("Could not parse the message");
    } 

    // parse the next_block_verifiers_IP_address_list
    for (count = 0, count2 = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memcpy(next_block_verifiers_list.block_verifiers_IP_address[count],&data2[count2],strnlen(data2,sizeof(data2)) - strnlen(strstr(data2+count2,"|"),sizeof(data2)) - count2);
      count2 = strnlen(data2,sizeof(data2)) - strnlen(strstr(data2+count2,"|"),sizeof(data2)) + 1;
    }

    color_print("The previous, current and next block verifiers have been synced from a network data node","green");
  }
  else
  {
    print_start_message("Loading the previous, current and next block verifiers from the database");

    // initialize the database_multiple_documents_fields struct 
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      for (count2 = 0; count2 < 19; count2++)
      {
        database_multiple_documents_fields.item[count][count2] = (char*)calloc(BUFFER_SIZE,sizeof(char));
        database_multiple_documents_fields.value[count][count2] = (char*)calloc(BUFFER_SIZE,sizeof(char));

        if (database_multiple_documents_fields.item[count][count2] == NULL || database_multiple_documents_fields.value[count][count2] == NULL)
        {
          memcpy(error_message.function[error_message.total],"get_block_template",18);
          memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
          error_message.total++;
          print_error_message;  
          exit(0);
        }
      }       
    } 
    database_multiple_documents_fields.document_count = 0;
    database_multiple_documents_fields.database_fields_count = 0;

    // get the top 100 delegates by total votes
    if (read_multiple_documents_all_fields_from_collection(DATABASE_NAME,"delegates","",&database_multiple_documents_fields,1,BLOCK_VERIFIERS_AMOUNT,1,"total_vote_count",0) == 0)
    {
      SYNC_ALL_BLOCK_VERIFIERS_LIST_ERROR("Could not get the top 100 delegates for the next round. This means that you will not be able to particpate in the next round");
    }

    // copy the database_multiple_documents_fields to the next, current and previous block verifiers list
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      for (count2 = 0; count2 < 19; count2++)
      {
        if (memcmp(database_multiple_documents_fields.item[count][count2],"delegate_name",13) == 0)
        {
          memcpy(previous_block_verifiers_list.block_verifiers_name[count],database_multiple_documents_fields.value[count][count2],strnlen(database_multiple_documents_fields.value[count][count2],sizeof(previous_block_verifiers_list.block_verifiers_name[count])));
          memcpy(current_block_verifiers_list.block_verifiers_name[count],database_multiple_documents_fields.value[count][count2],strnlen(database_multiple_documents_fields.value[count][count2],sizeof(current_block_verifiers_list.block_verifiers_name[count])));
          memcpy(next_block_verifiers_list.block_verifiers_name[count],database_multiple_documents_fields.value[count][count2],strnlen(database_multiple_documents_fields.value[count][count2],sizeof(next_block_verifiers_list.block_verifiers_name[count])));
        }
        if (memcmp(database_multiple_documents_fields.item[count][count2],"public_address",14) == 0)
        {
          memcpy(previous_block_verifiers_list.block_verifiers_public_address[count],database_multiple_documents_fields.value[count][count2],strnlen(database_multiple_documents_fields.value[count][count2],sizeof(previous_block_verifiers_list.block_verifiers_public_address[count])));
          memcpy(current_block_verifiers_list.block_verifiers_public_address[count],database_multiple_documents_fields.value[count][count2],strnlen(database_multiple_documents_fields.value[count][count2],sizeof(current_block_verifiers_list.block_verifiers_public_address[count])));
          memcpy(next_block_verifiers_list.block_verifiers_public_address[count],database_multiple_documents_fields.value[count][count2],strnlen(database_multiple_documents_fields.value[count][count2],sizeof(next_block_verifiers_list.block_verifiers_public_address[count])));
        }
        if (memcmp(database_multiple_documents_fields.item[count][count2],"IP_address",10) == 0)
        {
          memcpy(previous_block_verifiers_list.block_verifiers_IP_address[count],database_multiple_documents_fields.value[count][count2],strnlen(database_multiple_documents_fields.value[count][count2],sizeof(previous_block_verifiers_list.block_verifiers_IP_address[count])));
          memcpy(current_block_verifiers_list.block_verifiers_IP_address[count],database_multiple_documents_fields.value[count][count2],strnlen(database_multiple_documents_fields.value[count][count2],sizeof(current_block_verifiers_list.block_verifiers_IP_address[count])));
          memcpy(next_block_verifiers_list.block_verifiers_IP_address[count],database_multiple_documents_fields.value[count][count2],strnlen(database_multiple_documents_fields.value[count][count2],sizeof(next_block_verifiers_list.block_verifiers_IP_address[count])));
        }
      }
    }

    // reset the database_multiple_documents_fields
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      for (count2 = 0; count2 < 19; count2++)
      {
        pointer_reset(database_multiple_documents_fields.item[count][count2]);
        pointer_reset(database_multiple_documents_fields.value[count][count2]);
      }
    }
    color_print("The previous, current and next block verifiers have been loaded from the database","green");
  }
  return 1;
  
  #undef SYNC_ALL_BLOCK_VERIFIERS_LIST_ERROR  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_synced_block_verifiers
Description: Gets the synced block verifiers for syncing the database
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int get_synced_block_verifiers()
{
  // Variables
  char data[BUFFER_SIZE];
  char* data2 = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  size_t count;
  size_t count2;

  // define macros
  #define GET_SYNCED_BLOCK_VERIFIERS_DATA "{\r\n \"message_settings\": \"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST\",\r\n}"
  
  #define GET_SYNCED_BLOCK_VERIFIERS_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"get_synced_block_verifiers",26); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  // reset the block_verifiers_IP_addresses 
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memset(synced_block_verifiers.synced_block_verifiers_IP_address[count],0,sizeof(synced_block_verifiers.synced_block_verifiers_IP_address[count]));
    memset(synced_block_verifiers.vote_settings[count],0,sizeof(synced_block_verifiers.vote_settings[count]));
  }
  synced_block_verifiers.vote_settings_true = 0;
  synced_block_verifiers.vote_settings_false = 0;

  start:
  fprintf(stderr,"Connecting to a random network data node to get a list of current block verifiers\n");

  memset(data,0,sizeof(data));
  memset(data2,0,strlen(data2));

  // send the message to a random network data node
  do
  {
    count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT);
  } while (strncmp(network_data_nodes_list.network_data_nodes_IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0);
  
  // get the current time
  get_current_UTC_time;

  memset(data,0,sizeof(data));
  memcpy(data,"Connecting to network data node ",32);
  memcpy(data+32,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
  memcpy(data+strlen(data)," and sending NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST\n",73);
  memcpy(data+strlen(data),asctime(current_UTC_date_and_time),strlen(asctime(current_UTC_date_and_time)));
  fprintf(stderr,"%s\n",data);
  memset(data,0,sizeof(data));

  if (send_and_receive_data_socket(data2,network_data_nodes_list.network_data_nodes_IP_address[count],SEND_DATA_PORT,GET_SYNCED_BLOCK_VERIFIERS_DATA,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
  {
    memcpy(data,"Could not receive data from network data node ",46);
    memcpy(data+46,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
    color_print(data,"red");
    memset(data,0,sizeof(data));
    fprintf(stderr,"Connecting to a different network data node\n\n");
    goto start;
  }

  if (verify_data(data2,0,0) == 0)
  {
    GET_SYNCED_BLOCK_VERIFIERS_ERROR("Could not verify data");
  }

  // parse the message
  memset(data,0,sizeof(data));
  if (parse_json_data(data2,"block_verifiers_public_address_list",data,sizeof(data)) == 0)
  {
    GET_SYNCED_BLOCK_VERIFIERS_ERROR("Could not parse the message");
  } 

  // parse the block verifiers public addresses
  for (count = 0, count2 = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memcpy(synced_block_verifiers.synced_block_verifiers_public_address[count],&data[count2],strnlen(data,sizeof(data)) - strnlen(strstr(data+count2,"|"),sizeof(data)) - count2);
    count2 = strnlen(data,sizeof(data)) - strnlen(strstr(data+count2,"|"),sizeof(data)) + 1;
  }

  // parse the message
  memset(data,0,sizeof(data));
  if (parse_json_data(data2,"block_verifiers_IP_address_list",data,sizeof(data)) == 0)
  {
    GET_SYNCED_BLOCK_VERIFIERS_ERROR("Could not parse the message");
  } 

  // parse the block verifiers IP addresses
  for (count = 0, count2 = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memcpy(synced_block_verifiers.synced_block_verifiers_IP_address[count],&data[count2],strnlen(data,sizeof(data)) - strnlen(strstr(data+count2,"|"),sizeof(data)) - count2);
    count2 = strnlen(data,sizeof(data)) - strnlen(strstr(data+count2,"|"),sizeof(data)) + 1;
  }
  return 1;

  #undef GET_SYNCED_BLOCK_VERIFIERS_DATA
  #undef GET_SYNCED_BLOCK_VERIFIERS_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_check_reserve_proofs_database
Description: Checks if the block verifier needs to sync the reserve proofs database
Paramters:
  SETTINGS - 1 to sync from a random block verifier, 2 to sync from a random network data node
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_check_reserve_proofs_database(const int SETTINGS)
{
  // Variables
  char message[BUFFER_SIZE];
  char reserve_proofs_database[BUFFER_SIZE]; 
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char data2[BUFFER_SIZE]; 
  size_t count;

  // define macros  
  #define SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"sync_check_reserve_proofs_database",34); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  pointer_reset(data); \
  return 0;

  memset(message,0,sizeof(message));
  memset(reserve_proofs_database,0,sizeof(reserve_proofs_database));
  memset(data2,0,sizeof(data2));

  print_start_message("Checking if the reserve proofs database is synced");

  if (get_synced_block_verifiers() == 0)
  {
    SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR("Could not get the synced block verifiers");
  }

  // get the database data hash for the reserve proofs database
  if (get_database_data_hash(data,DATABASE_NAME,"reserve_proofs",0) == 0)
  {
    SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR("Could not get the database data hash for the reserve proofs database");
  }

  // create the message
  memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE\",\r\n \"reserve_proofs_data_hash\": \"",139);
  memcpy(message+strlen(message),data,DATA_HASH_LENGTH);
  memcpy(message+strlen(message),"\",\r\n ",5);

  for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
  {
    memcpy(message+strlen(message),"\"reserve_proofs_data_hash_",26);
    snprintf(message+strlen(message),sizeof(message)-1,"%zu",count);
    memcpy(message+strlen(message),"\": \"",4);
    // get the database data hash for the reserve proofs database
    memset(data,0,strlen(data));
    memset(data2,0,strlen(data2));  
    memcpy(data2,"reserve_proofs_",15);  
    snprintf(data2+15,sizeof(data2)-16,"%zu",count);
    if (get_database_data_hash(data,DATABASE_NAME,data2,0) == 0)
    {
      SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR("Could not get the database data hash for the reserve proofs database");
    }
    memcpy(message+strlen(message),data,DATA_HASH_LENGTH);
    memcpy(message+strlen(message),"\",\r\n ",5);
  }
  memcpy(message+strlen(message),"}",1);

  // sign_data
  if (sign_data(message,0) == 0)
  { 
    SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR("Could not sign_data");
  }

  fprintf(stderr,"Sending all block verifiers a message to check if the reserve proofs database is synced\n"); 

  if (SETTINGS == 1)
  {
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memset(data,0,strlen(data));
      memset(data2,0,sizeof(data2));
      if (send_and_receive_data_socket(data,synced_block_verifiers.synced_block_verifiers_IP_address[count],SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0 || verify_data(data,0,0) == 0)
      {
        memcpy(synced_block_verifiers.vote_settings[count],"connection_timeout",18);
        synced_block_verifiers.vote_settings_connection_timeout++;
      }
      else
      {
        if (strncmp(synced_block_verifiers.synced_block_verifiers_IP_address[count],network_data_nodes_list.network_data_nodes_IP_address[count],BUFFER_SIZE) == 0)
        {
          memcpy(reserve_proofs_database,data,strnlen(data,BUFFER_SIZE));
        } 
        parse_json_data(data,"reserve_proofs_database",data2,sizeof(data2));
        memcpy(synced_block_verifiers.vote_settings[count],data2,strnlen(data2,BUFFER_SIZE));
        if (memcmp(data2,"true",4) == 0)
        {
          synced_block_verifiers.vote_settings_true++;
        }
        else if (memcmp(data2,"false",5) == 0)
        {
          synced_block_verifiers.vote_settings_false++;
        }
      }   
    }

    if (synced_block_verifiers.vote_settings_false >= BLOCK_VERIFIERS_VALID_AMOUNT || synced_block_verifiers.vote_settings_connection_timeout >= BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      color_print("The reserve proofs database is not synced","red");
      if (sync_reserve_proofs_database(reserve_proofs_database, SETTINGS) == 0)
      {
        SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR("Could not sync the reserve proofs database");
      }
    }
  }
  else if (SETTINGS == 2)
  {
    for (count = 0; count < NETWORK_DATA_NODES_AMOUNT; count++)
    {
      memset(data,0,strlen(data));
      memset(data2,0,sizeof(data2));
      if (send_and_receive_data_socket(data,network_data_nodes_list.network_data_nodes_IP_address[count],SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0 || verify_data(data,0,0) == 0)
      {
        memcpy(synced_block_verifiers.vote_settings[count],"connection_timeout",18);
        synced_block_verifiers.vote_settings_connection_timeout++;
      }
      else
      {
        if (strncmp(synced_block_verifiers.synced_block_verifiers_IP_address[count],network_data_nodes_list.network_data_nodes_IP_address[count],BUFFER_SIZE) == 0)
        {
          memcpy(reserve_proofs_database,data,strnlen(data,BUFFER_SIZE));
        } 
        parse_json_data(data,"reserve_proofs_database",data2,sizeof(data2));
        memcpy(synced_block_verifiers.vote_settings[count],data2,strnlen(data2,BUFFER_SIZE));
        if (memcmp(data2,"true",4) == 0)
        {
          synced_block_verifiers.vote_settings_true++;
        }
        else if (memcmp(data2,"false",5) == 0)
        {
          synced_block_verifiers.vote_settings_false++;
        }
      }   
    }

    if (synced_block_verifiers.vote_settings_false > 0 || synced_block_verifiers.vote_settings_connection_timeout > 0)
    {
      color_print("The reserve proofs database is not synced","red");
      if (sync_reserve_proofs_database(reserve_proofs_database, SETTINGS) == 0)
      {
        SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR("Could not sync the reserve proofs database");
      }
    }
  }

  color_print("The reserve proofs database is synced","green");
  
  pointer_reset(data);
  return 1;
  
  #undef SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_reserve_proofs_database
Description: Syncs the reserve proofs database
Paramters:
  RESERVE_PROOFS_DATABASE - The RESERVE_PROOFS_DATABASE data from a random network data node
  SETTINGS - 1 to sync from a random block verifier, 2 to sync from a random network data node
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_reserve_proofs_database(const char* RESERVE_PROOFS_DATABASE, const int SETTINGS)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  size_t count = 0;
  size_t count2;
  size_t counter;
  int settings;

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL;
  
  #define SYNC_RESERVE_PROOFS_DATABASE_ERROR(message,settings) \
  if (settings == 0) \
  { \
    memcpy(error_message.function[error_message.total],"sync_reserve_proofs_database",28); \
    memcpy(error_message.data[error_message.total],message,strnlen(message,sizeof(error_message.data[error_message.total]))); \
    error_message.total++; \
    pointer_reset_all; \
    return 0; \
  } \
  else \
  { \
    memset(data,0,strlen(data)); \
    memcpy(data+strlen(data),message,strnlen(message,BUFFER_SIZE)); \
    memcpy(data+strlen(data),synced_block_verifiers.synced_block_verifiers_IP_address[count],strnlen(synced_block_verifiers.synced_block_verifiers_IP_address[count],BUFFER_SIZE)); \
    memcpy(data+strlen(data),"\nConnecting to another block verifier",37); \
    color_print(data,"red"); \
    goto start; \
  } 

  memset(data,0,strlen(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    pointer_reset(data);
    memcpy(error_message.function[error_message.total],"sync_reserve_proofs_database",28);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  start:

    /* select a random block verifier from the majority vote settings to sync the database from, making sure not to select your own block verifier node
       select a random network data node to sync from if
       there was a lot of connection_timeouts, to where a majority vote could not be calculated,
       there were more than BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT new block verifiers
    */
    if (synced_block_verifiers.vote_settings_connection_timeout < BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT && SETTINGS == 1)
    {
      count = (int)(rand() % BLOCK_VERIFIERS_AMOUNT-1);
      if (memcmp(synced_block_verifiers.vote_settings[count],"true",4) != 0 || strncmp(synced_block_verifiers.synced_block_verifiers_IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0)
      {
        goto start;
      }
    }
    else if (SETTINGS == 1)
    {
      count = (int)(rand() % BLOCK_VERIFIERS_AMOUNT-1);
      for (counter = 0, settings = 0; counter < NETWORK_DATA_NODES_AMOUNT; counter++)
      {
        if (strncmp(synced_block_verifiers.synced_block_verifiers_IP_address[count],network_data_nodes_list.network_data_nodes_IP_address[counter],BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0 && strncmp(synced_block_verifiers.synced_block_verifiers_IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) != 0)
        {
          settings = 1;
        }
      }
      if (settings != 1)    
      {
        goto start;
      }
    }
    else if (SETTINGS == 2)
    {
      count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT-1);
      if (strncmp(network_data_nodes_list.network_data_nodes_IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0)
      {
        goto start;
      }
    }

  for (count2 = 1; count2 <= TOTAL_RESERVE_PROOFS_DATABASES; count2++)
  {
    memset(data,0,strlen(data));
    memcpy(data,"Connecting to block verifier ",29);
    memcpy(data+strlen(data),synced_block_verifiers.synced_block_verifiers_IP_address[count],strnlen(synced_block_verifiers.synced_block_verifiers_IP_address[count],BUFFER_SIZE));
    memcpy(data+strlen(data)," to sync reserve_proofs_",24);
    snprintf(data+strlen(data),MAXIMUM_BUFFER_SIZE-1,"%zu",count2);
    fprintf(stderr,"%s\n",data);

    // parse the RESERVE_PROOFS_DATABASE
    memset(data,0,strlen(data));
    memset(data2,0,sizeof(data2));
    memcpy(data2,"reserve_proofs_database_",24);
    snprintf(data2+24,sizeof(data2)-25,"%zu",count2);

    if (parse_json_data(RESERVE_PROOFS_DATABASE,data2,data,MAXIMUM_BUFFER_SIZE) == 0)
    {
      SYNC_RESERVE_PROOFS_DATABASE_ERROR("Could not receive data from ",1);
    }
   
    if (memcmp(data,"false",5) == 0)
    {
      // sync the database
      memset(data,0,strlen(data));
      memcpy(data,"reserve_proofs_",15);
      snprintf(data+strlen(data),MAXIMUM_BUFFER_SIZE-1,"%zu",count2);
      memcpy(data+strlen(data)," is not synced, downloading it from ",36);
      memcpy(data+strlen(data),synced_block_verifiers.synced_block_verifiers_IP_address[count],36);
      color_print(data,"red");

      // create the message
      memset(data2,0,sizeof(data2));
      memcpy(data2,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"file\": \"reserve_proofs_",133);
      snprintf(data2+133,sizeof(data2)-134,"%zu",count2);
      memcpy(data2+strlen(data2),"\",\r\n}",5);

      // sign_data
      if (sign_data(data2,0) == 0)
      { 
        SYNC_RESERVE_PROOFS_DATABASE_ERROR("Could not sign_data",0);
      }
     
      memset(data,0,strlen(data));
      if (send_and_receive_data_socket(data,synced_block_verifiers.synced_block_verifiers_IP_address[count],SEND_DATA_PORT,data2,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
      {
        SYNC_RESERVE_PROOFS_DATABASE_ERROR("Could not receive data from ",1);
      }

      if (verify_data(data,0,0) == 0)
      {
        SYNC_RESERVE_PROOFS_DATABASE_ERROR("Could not verify data from ",1);
      }

      // parse the message
      memset(data3,0,sizeof(data3));
      if (parse_json_data(data,"reserve_proofs_database",data3,sizeof(data3)) == 0 || memcmp(data3,"",1) == 0)
      {
        SYNC_RESERVE_PROOFS_DATABASE_ERROR("Could not receive data from ",1);
      }

      // add the data to the database
      memset(data2,0,sizeof(data2));
      memcpy(data2,"reserve_proofs_",15);
      snprintf(data2+15,sizeof(data2)-16,"%zu",count2);

      // delete the collection from the database
      delete_collection_from_database(DATABASE_NAME,data2,0);

      // add the data to the database
      memset(data,0,strlen(data));
      memcpy(data,data3,strlen(data3)-2);
      insert_multiple_documents_into_collection_json(DATABASE_NAME,data2,data,MAXIMUM_BUFFER_SIZE,0);

      memset(data,0,strlen(data));
      memcpy(data,"reserve_proofs_",15);
      snprintf(data+strlen(data),MAXIMUM_BUFFER_SIZE-1,"%zu",count2);
      memcpy(data+strlen(data)," has been synced successfully\n",31);
      color_print(data,"green");
    }
    else
    {
      memset(data,0,strlen(data));
      memcpy(data,"reserve_proofs_",15);
      snprintf(data+strlen(data),MAXIMUM_BUFFER_SIZE-1,"%zu",count2);
      memcpy(data+strlen(data)," is already synced\n",19);
      color_print(data,"green");
    }
  }

  pointer_reset_all;
  return 1;

  #undef pointer_reset_all
  #undef SYNC_RESERVE_PROOFS_DATABASE_ERROR   
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_check_reserve_bytes_database
Description: Checks if the block verifier needs to sync the reserve bytes database
Paramters:
  SETTINGS - 1 to sync from a random block verifier, 2 to sync from a random network data node
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_check_reserve_bytes_database(const int SETTINGS)
{
  // Variables
  char message[BUFFER_SIZE];
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char data2[BUFFER_SIZE];
  size_t count;

  // define macros
  #define SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"sync_check_reserve_bytes_database",33); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(message,0,sizeof(message));
  memset(data2,0,sizeof(data2));

  print_start_message("Checking if the reserve bytes database is synced");

  if (get_synced_block_verifiers() == 0)
  {
    SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR("Could not get the synced block verifiers");
  }

  // get the database data hash for the reserve bytes database
  if (get_database_data_hash(data,DATABASE_NAME,"reserve_bytes",0) == 0)
  {
    SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR("Could not get the database data hash for the reserve bytes database");
  }

  // create the message
  memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE\",\r\n \"data_hash\": \"",123);
  memcpy(message+123,data,DATA_HASH_LENGTH);
  memcpy(message+251,"\",\r\n}",5);

  // sign_data
  if (sign_data(message,0) == 0)
  { 
    SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR("Could not sign_data");
  }

  fprintf(stderr,"Sending all block verifiers a message to check if the reserve bytes database is synced\n"); 

  if (SETTINGS == 1)
  {
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memset(data,0,strlen(data));
      memset(data2,0,sizeof(data2));
      if (send_and_receive_data_socket(data,synced_block_verifiers.synced_block_verifiers_IP_address[count],SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0 || verify_data(data,0,0) == 0)
      {
        memcpy(synced_block_verifiers.vote_settings[count],"connection_timeout",18);
        synced_block_verifiers.vote_settings_connection_timeout++;
      }
      else
      {
        parse_json_data(data,"reserve_bytes_database",data2,sizeof(data2));
        memcpy(synced_block_verifiers.vote_settings[count],data2,strnlen(data2,BUFFER_SIZE));
        if (memcmp(data2,"true",4) == 0)
        {
          synced_block_verifiers.vote_settings_true++;
        }
        else if (memcmp(data2,"false",5) == 0)
        {
          synced_block_verifiers.vote_settings_false++;
        }
      }   
    }

    if (synced_block_verifiers.vote_settings_false >= BLOCK_VERIFIERS_VALID_AMOUNT || synced_block_verifiers.vote_settings_connection_timeout >= BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      color_print("The reserve bytes database is not synced","red");
      if (sync_reserve_bytes_database(SETTINGS) == 0)
      {
        SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR("Could not sync the reserve bytes database");
      }
    }
  }
  else if (SETTINGS == 2)
  {
    for (count = 0; count < NETWORK_DATA_NODES_AMOUNT; count++)
    {
      memset(data,0,strlen(data));
      memset(data2,0,sizeof(data2));
      if (send_and_receive_data_socket(data,network_data_nodes_list.network_data_nodes_IP_address[count],SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0 || verify_data(data,0,0) == 0)
      {
        memcpy(synced_block_verifiers.vote_settings[count],"connection_timeout",18);
        synced_block_verifiers.vote_settings_connection_timeout++;
      }
      else
      {
        parse_json_data(data,"reserve_bytes_database",data2,sizeof(data2));
        memcpy(synced_block_verifiers.vote_settings[count],data2,strnlen(data2,BUFFER_SIZE));
        if (memcmp(data2,"true",4) == 0)
        {
          synced_block_verifiers.vote_settings_true++;
        }
        else if (memcmp(data2,"false",5) == 0)
        {
          synced_block_verifiers.vote_settings_false++;
        }
      }   
    }

    if (synced_block_verifiers.vote_settings_false > 0 || synced_block_verifiers.vote_settings_connection_timeout > 0)
    {
      color_print("The reserve bytes database is not synced","red");
      if (sync_reserve_bytes_database(SETTINGS) == 0)
      {
        SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR("Could not sync the reserve bytes database");
      }
    }
  }

  color_print("The reserve bytes database is synced","green");
  
  return 1;
  
  #undef SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_reserve_bytes_database
Description: Syncs the reserve bytes database
Paramters:
  SETTINGS - 1 to sync from a random block verifier, 2 to sync from a random network data node
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_reserve_bytes_database(const int SETTINGS)
{
  // Variables
  char message[BUFFER_SIZE];
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  size_t count = 0;
  size_t count2;
  int counter;
  size_t number;
  int settings;
  int settings2 = 0;

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL;
  
  #define SYNC_RESERVE_BYTES_DATABASE_ERROR(message,settings) \
  if (settings == 0) \
  { \
    memcpy(error_message.function[error_message.total],"sync_reserve_bytes_database",27); \
    memcpy(error_message.data[error_message.total],message,strnlen(message,sizeof(error_message.data[error_message.total]))); \
    error_message.total++; \
    pointer_reset_all; \
    return 0; \
  } \
  else \
  { \
    memset(data,0,strlen(data)); \
    memcpy(data+strlen(data),message,strnlen(message,BUFFER_SIZE)); \
    memcpy(data+strlen(data),synced_block_verifiers.synced_block_verifiers_IP_address[count],strnlen(synced_block_verifiers.synced_block_verifiers_IP_address[count],BUFFER_SIZE)); \
    memcpy(data+strlen(data),"\nConnecting to another block verifier",37); \
    color_print(data,"red"); \
    goto start; \
  }  

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    pointer_reset(data);
    memcpy(error_message.function[error_message.total],"sync_reserve_bytes_database",27);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }
  
  memset(message,0,sizeof(message));
  memset(data,0,strlen(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  // get the current reserve bytes database
  sscanf(current_block_height,"%zu", &number);
  number--;
  if (number < XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT-1)
  {
    SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not get the current block height",0);
  }
  number = ((number - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME) + 1;

  // get what reserve bytes database count it has and start at that count to sync
  for (count2 = 1; count2 <= number; count2++)
  {
    memset(data2,0,sizeof(data2));
    memcpy(data2+strlen(data2),"reserve_bytes_",14);
    snprintf(data2+strlen(data2),sizeof(data2)-1,"%zu",count2);    
    if (count_all_documents_in_collection(DATABASE_NAME,data2,0) <= 0)
    {
      break;
    }
  }

  for (; count2 <= number; count2++)
  {
    start:

    /* select a random block verifier from the majority vote settings to sync the database from, making sure not to select your own block verifier node
       select a random network data node to sync from if
       there was a lot of connection_timeouts, to where a majority vote could not be calculated,
       there were more than BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT new block verifiers
    */
    if (synced_block_verifiers.vote_settings_connection_timeout < BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT && SETTINGS == 1)
    {
      count = (int)(rand() % BLOCK_VERIFIERS_AMOUNT-1);
      if (memcmp(synced_block_verifiers.vote_settings[count],"true",4) != 0 || strncmp(synced_block_verifiers.synced_block_verifiers_IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0)
      {
        goto start;
      }
    }
    else if (SETTINGS == 1)
    {
      count = (int)(rand() % BLOCK_VERIFIERS_AMOUNT-1);
      for (counter = 0, settings = 0; counter < NETWORK_DATA_NODES_AMOUNT; counter++)
      {
        if (strncmp(synced_block_verifiers.synced_block_verifiers_IP_address[count],network_data_nodes_list.network_data_nodes_IP_address[counter],BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0 && strncmp(synced_block_verifiers.synced_block_verifiers_IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) != 0)
        {
          settings = 1;
        }
      }
      if (settings != 1)    
      {
        goto start;
      }
    }
    else if (SETTINGS == 2)
    {
      count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT-1);
      if (strncmp(network_data_nodes_list.network_data_nodes_IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0)
      {
        goto start;
      }
    }

    memset(data,0,strlen(data));
    memcpy(data,"Connecting to block verifier ",29);
    memcpy(data+strlen(data),synced_block_verifiers.synced_block_verifiers_IP_address[count],strnlen(synced_block_verifiers.synced_block_verifiers_IP_address[count],BUFFER_SIZE));
    memcpy(data+strlen(data)," to sync reserve_bytes_",23);
    snprintf(data+strlen(data),MAXIMUM_BUFFER_SIZE,"%zu",count2);
    color_print(data,"green");

    // get the database data hash
    memset(data,0,strlen(data));
    memset(data2,0,sizeof(data2));
    memcpy(data2+strlen(data2),"reserve_bytes_",14);
    snprintf(data2+strlen(data2),sizeof(data2)-1,"%zu",count2);
    if (get_database_data_hash(data,DATABASE_NAME,data2,0) == 0)
    {
      SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not get the database data hash for the reserve bytes database",0);
    }

    // create the message
    memset(data2,0,sizeof(data2));
    memcpy(data2,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"file\": \"reserve_bytes_",128);
    snprintf(data2+128,sizeof(data2)-129,"%zu",count2);
    memcpy(data2+strlen(data2),"\",\r\n \"data_hash\": \"",19);
    memcpy(data2+strlen(data2),data,DATA_HASH_LENGTH);
    memcpy(data2+strlen(data2),"\",\r\n}",5);

    // sign_data
    if (sign_data(data2,0) == 0)
    { 
      SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not sign_data",0);
    }
     
    memset(data,0,strlen(data));
    if (send_and_receive_data_socket(data,synced_block_verifiers.synced_block_verifiers_IP_address[count],SEND_DATA_PORT,data2,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
    {
      SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not receive data from ",1);
    }

    if (verify_data(data,0,0) == 0)
    {
      SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not verify data from ",1);
    }

    // parse the message
    memset(data3,0,sizeof(data3));
    if (parse_json_data(data,"reserve_bytes_database",data3,sizeof(data3)) == 0)
    {
      SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not receive data from ",1);
    }

    if (memcmp(data3,"false",5) == 0)
    {
      // sync the database
      memset(data,0,strlen(data));
      memcpy(data,"reserve_bytes_",14);
      snprintf(data+strlen(data),MAXIMUM_BUFFER_SIZE-1,"%zu",count2);
      memcpy(data+strlen(data)," is not synced, downloading it from ",36);
      memcpy(data+strlen(data),synced_block_verifiers.synced_block_verifiers_IP_address[count],36);
      color_print(data,"red");

      // create the message
      memset(data2,0,sizeof(data2));
      memcpy(data2,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"file\": \"reserve_bytes_",131);
      snprintf(data2+131,sizeof(data2)-132,"%zu",count2);
      memcpy(data2+strlen(data2),"\",\r\n}",5);

      // sign_data
      if (sign_data(data2,0) == 0)
      { 
        SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not sign_data",0);
      }

      memset(data,0,strlen(data));
      if (send_and_receive_data_socket(data,synced_block_verifiers.synced_block_verifiers_IP_address[count],SEND_DATA_PORT,data2,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
      {
        SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not receive data from ",1);
      }

      if (verify_data(data,0,0) == 0)
      {
        SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not verify data from ",1);
      }

      // parse the message
      memset(data3,0,sizeof(data3));
      if (parse_json_data(data,"reserve_bytes_database",data3,sizeof(data3)) == 0 || memcmp(data3,"",1) == 0)
      {
        SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not receive data from ",1);
      }

      // add the data to the database
      memset(data2,0,sizeof(data2));
      memcpy(data2,"reserve_bytes_",14);
      snprintf(data2+14,sizeof(data2)-15,"%zu",count2);

      // delete the collection from the database
      delete_collection_from_database(DATABASE_NAME,data2,0);

      // add the data to the database
      memset(data,0,strlen(data));
      memcpy(data,data3,strlen(data3)-2);
      insert_multiple_documents_into_collection_json(DATABASE_NAME,data2,data,MAXIMUM_BUFFER_SIZE,0);

      memset(data,0,strlen(data));
      memcpy(data,"reserve_bytes_",14);
      snprintf(data+strlen(data),MAXIMUM_BUFFER_SIZE-1,"%zu",count2);
      memcpy(data+strlen(data)," has been synced successfully\n",31);
      color_print(data,"green");
    }
    else
    {
      memset(data,0,strlen(data));
      memcpy(data,"reserve_bytes_",14);
      snprintf(data+strlen(data),MAXIMUM_BUFFER_SIZE-1,"%zu",count2);
      memcpy(data+strlen(data)," is already synced\n",19);
      color_print(data,"green");
    }
  }



  // check that the reserve bytes database is synced and if not sync all of the reserve bytes databases
  if (settings2 == 0)
  {
    if (get_synced_block_verifiers() == 0)
    {
      SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not get the synced block verifiers",0);
    }

    // get the database data hash for the reserve bytes database
    memset(data,0,strlen(data));
    if (get_database_data_hash(data,DATABASE_NAME,"reserve_bytes",0) == 0)
    {
      SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not get the database data hash for the reserve bytes database",0);
    }

    // create the message
    memset(message,0,sizeof(message));
    memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE\",\r\n \"data_hash\": \"",123);
    memcpy(message+123,data,DATA_HASH_LENGTH);
    memcpy(message+strlen(message),"\",\r\n}",5);

    // sign_data
    if (sign_data(message,0) == 0)
    { 
      SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not sign_data",0);
    }

    fprintf(stderr,"Sending all block verifiers a message to check if the reserve bytes database is synced\n"); 
 
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memset(data,0,strlen(data));
      memset(data2,0,sizeof(data2));
      if (send_and_receive_data_socket(data,synced_block_verifiers.synced_block_verifiers_IP_address[count],SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0 || verify_data(data,0,0) == 0)
      {
        memcpy(synced_block_verifiers.vote_settings[count],"connection_timeout",18);
        synced_block_verifiers.vote_settings_connection_timeout++;
      }
      else
      {
        parse_json_data(data,"reserve_bytes_database",data2,sizeof(data2));
        memcpy(synced_block_verifiers.vote_settings[count],data2,strnlen(data2,sizeof(synced_block_verifiers.vote_settings[count])));
        if (memcmp(data2,"true",4) == 0)
        {
          synced_block_verifiers.vote_settings_true++;
        }
        else if (memcmp(data2,"false",5) == 0)
        {
          synced_block_verifiers.vote_settings_false++;
        }
      }   
    }

    if (synced_block_verifiers.vote_settings_false >= BLOCK_VERIFIERS_VALID_AMOUNT || synced_block_verifiers.vote_settings_connection_timeout >= BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      color_print("The reserve bytes database is not synced\nSyncing all of the reserve bytes databases","red");
      count2 = 1;
      settings2 = 1;
      goto start;
    }
  }

  pointer_reset_all;
  return 1;

  #undef pointer_reset_all
  #undef SYNC_RESERVE_bytes_DATABASE_ERROR   
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_check_delegates_database
Description: Checks if the block verifier needs to sync the delegates database
Paramters:
  SETTINGS - 1 to sync from a random block verifier, 2 to sync from a random network data node
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_check_delegates_database(const int SETTINGS)
{
  // Variables
  char message[BUFFER_SIZE];
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char data2[BUFFER_SIZE];
  size_t count;

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define SYNC_CHECK_DELEGATES_DATABASE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"sync_check_delegates_database",29); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(message,0,sizeof(message));
  memset(data2,0,sizeof(data2));

  print_start_message("Checking if the delegates database is synced");

  if (get_synced_block_verifiers() == 0)
  {
    SYNC_CHECK_DELEGATES_DATABASE_ERROR("Could not get the synced block verifiers");
  }

  // get the database data hash for the reserve proofs database
  if (get_database_data_hash(data,DATABASE_NAME,DATABASE_COLLECTION,0) == 0)
  {
    SYNC_CHECK_DELEGATES_DATABASE_ERROR("Could not get the database data hash for the delegates database");
  }

  // create the message
  memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"data_hash\": \"",115);
  memcpy(message+115,data,DATA_HASH_LENGTH);
  memcpy(message+243,"\",\r\n}",5);

  // sign_data
  if (sign_data(message,0) == 0)
  { 
    SYNC_CHECK_DELEGATES_DATABASE_ERROR("Could not sign_data");
  }

  fprintf(stderr,"Sending all block verifiers a message to check if the delegates database is synced\n"); 

  if (SETTINGS == 1)
  {
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memset(data,0,strlen(data));
      memset(data2,0,sizeof(data2));
      if (send_and_receive_data_socket(data,synced_block_verifiers.synced_block_verifiers_IP_address[count],SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0 || verify_data(data,0,0) == 0)
      {
        memcpy(synced_block_verifiers.vote_settings[count],"connection_timeout",18);
        synced_block_verifiers.vote_settings_connection_timeout++;
      }
      else
      {
        parse_json_data(data,"delegates_database",data2,sizeof(data2));
        memcpy(synced_block_verifiers.vote_settings[count],data2,strnlen(data2,BUFFER_SIZE));
        if (memcmp(data2,"true",4) == 0)
        {
          synced_block_verifiers.vote_settings_true++;
        }
        else if (memcmp(data2,"false",5) == 0)
        {
          synced_block_verifiers.vote_settings_false++;
        }
      }   
    }

    if (synced_block_verifiers.vote_settings_false >= BLOCK_VERIFIERS_VALID_AMOUNT || synced_block_verifiers.vote_settings_connection_timeout >= BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      color_print("The delegates database is not synced","red");
      if (sync_delegates_database(SETTINGS) == 0)
      {
        SYNC_CHECK_DELEGATES_DATABASE_ERROR("Could not sync the delegates database database");
      }
    }
  }
  else if (SETTINGS == 2)
  {
    for (count = 0; count < NETWORK_DATA_NODES_AMOUNT; count++)
    {
      memset(data,0,strlen(data));
      memset(data2,0,sizeof(data2));
      if (send_and_receive_data_socket(data,network_data_nodes_list.network_data_nodes_IP_address[count],SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0 || verify_data(data,0,0) == 0)
      {
        memcpy(synced_block_verifiers.vote_settings[count],"connection_timeout",18);
        synced_block_verifiers.vote_settings_connection_timeout++;
      }
      else
      {
        parse_json_data(data,"delegates_database",data2,sizeof(data2));
        memcpy(synced_block_verifiers.vote_settings[count],data2,strnlen(data2,BUFFER_SIZE));
        if (memcmp(data2,"true",4) == 0)
        {
          synced_block_verifiers.vote_settings_true++;
        }
        else if (memcmp(data2,"false",5) == 0)
        {
          synced_block_verifiers.vote_settings_false++;
        }
      }   
    }

    if (synced_block_verifiers.vote_settings_false > 0 || synced_block_verifiers.vote_settings_connection_timeout > 0)
    {
      color_print("The delegates database is not synced","red");
      if (sync_delegates_database(SETTINGS) == 0)
      {
        SYNC_CHECK_DELEGATES_DATABASE_ERROR("Could not sync the delegates database database");
      }
    }
  }

  color_print("The delegates database is synced","green");
  
  return 1;

  #undef DATABASE_COLLECTION
  #undef SYNC_CHECK_DELEGATES_DATABASE_ERROR  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_delegates_database
Description: Syncs the reserve proofs database
Paramters:
  SETTINGS - 1 to sync from a random block verifier, 2 to sync from a random network data node
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_delegates_database(const int SETTINGS)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char data2[BUFFER_SIZE];
  size_t count = 0;
  size_t count2;
  int settings;

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define pointer_reset_all \
  free(data); \
  data = NULL;

  #define SYNC_DELEGATES_DATABASE_ERROR(message,settings) \
  if (settings == 0) \
  { \
    memcpy(error_message.function[error_message.total],"sync_delegates_database",23); \
    memcpy(error_message.data[error_message.total],message,strnlen(message,sizeof(error_message.data[error_message.total]))); \
    error_message.total++; \
    pointer_reset_all; \
    return 0; \
  } \
  else \
  { \
    memset(data,0,strlen(data)); \
    memcpy(data+strlen(data),message,strnlen(message,BUFFER_SIZE)); \
    memcpy(data+strlen(data),synced_block_verifiers.synced_block_verifiers_IP_address[count],strnlen(synced_block_verifiers.synced_block_verifiers_IP_address[count],BUFFER_SIZE)); \
    memcpy(data+strlen(data),"\nConnecting to another block verifier",37); \
    color_print(data,"red"); \
    goto start; \
  }  

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    pointer_reset(data);
    memcpy(error_message.function[error_message.total],"sync_delegates_database",23);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  memset(data,0,strlen(data));
  memset(data2,0,sizeof(data2));
  
  start:

  /* select a random block verifier from the majority vote settings to sync the database from, making sure not to select your own block verifier node
     select a random network data node to sync from if
     there was a lot of connection_timeouts, to where a majority vote could not be calculated,
     there were more than BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT new block verifiers
  */
  if (synced_block_verifiers.vote_settings_connection_timeout < BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT && SETTINGS == 1)
  {
    count = (int)(rand() % BLOCK_VERIFIERS_AMOUNT-1);
    if (memcmp(synced_block_verifiers.vote_settings[count],"true",4) != 0 || strncmp(synced_block_verifiers.synced_block_verifiers_IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0)
    {
      goto start;
    }
  }
  else if (SETTINGS == 1)
  {
    count = (int)(rand() % BLOCK_VERIFIERS_AMOUNT-1);
    for (count2 = 0, settings = 0; count2 < NETWORK_DATA_NODES_AMOUNT; count2++)
    {
      if (strncmp(synced_block_verifiers.synced_block_verifiers_IP_address[count],network_data_nodes_list.network_data_nodes_IP_address[count2],BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0 && strncmp(synced_block_verifiers.synced_block_verifiers_IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) != 0)
      {
        settings = 1;
      }
    }
    if (settings != 1)    
    {
      goto start;
    }
  }
  else if (SETTINGS == 2)
  {
    count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT-1);
    if (strncmp(network_data_nodes_list.network_data_nodes_IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0)
    {
      goto start;
    }
  }

  memset(data,0,strlen(data));
  memcpy(data,"Connecting to block verifier ",29);
  memcpy(data+strlen(data),synced_block_verifiers.synced_block_verifiers_IP_address[count],strnlen(synced_block_verifiers.synced_block_verifiers_IP_address[count],BUFFER_SIZE));
  memcpy(data+strlen(data)," to sync the delegates database",31);
  color_print(data,"green");

  // get the database data hash
  if (get_database_data_hash(data,DATABASE_NAME,DATABASE_COLLECTION,0) == 0)
  {
    SYNC_DELEGATES_DATABASE_ERROR("Could not get the database data hash for the delegates database",0);
  }

  // create the message
  memset(data2,0,sizeof(data2));
  memcpy(data2,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"file\": \"",118);
  snprintf(data2+118,sizeof(data2)-119,"%zu",count);
  memcpy(data2+strlen(data2),"\",\r\n}",5);

  // sign_data
  if (sign_data(data2,0) == 0)
  { 
    SYNC_DELEGATES_DATABASE_ERROR("Could not sign_data",0);
  }
     
  memset(data,0,strlen(data));
  if (send_and_receive_data_socket(data,synced_block_verifiers.synced_block_verifiers_IP_address[count],SEND_DATA_PORT,data2,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
  {
    SYNC_DELEGATES_DATABASE_ERROR("Could not receive data from ",1);
  }

  if (verify_data(data,0,0) == 0)
  {
    SYNC_DELEGATES_DATABASE_ERROR("Could not verify data from ",1);
  }

  // parse the message
  memset(data2,0,sizeof(data2));
  if (parse_json_data(data,"delegates_database",data2,sizeof(data2)) == 0 || memcmp(data2,"",1) == 0)
  {
    SYNC_DELEGATES_DATABASE_ERROR("Could not receive data from ",1);
  }

  // delete the collection from the database
  delete_collection_from_database(DATABASE_NAME,DATABASE_COLLECTION,0);

  // add the data to the database
  memset(data,0,strlen(data));
  memcpy(data,data2,strlen(data2)-2);
  insert_multiple_documents_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,data,10485760,0);

  pointer_reset_all;
  return 1;

  #undef DATABASE_COLLECTION
  #undef pointer_reset_all
  #undef SYNC_DELEGATES_DATABASE_ERROR   
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_check_statistics_database
Description: Checks if the block verifier needs to sync the statistics database
Paramters:
  SETTINGS - 1 to sync from a random block verifier, 2 to sync from a random network data node
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_check_statistics_database(const int SETTINGS)
{
  // Variables
  char message[BUFFER_SIZE];
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char data2[BUFFER_SIZE];
  size_t count;

  // define macros
  #define DATABASE_COLLECTION "statistics"
  #define SYNC_CHECK_STATISTICS_DATABASE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"sync_check_statistics_database",30); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(message,0,sizeof(message));
  memset(data2,0,sizeof(data2));

  print_start_message("Checking if the statistics database is synced");

  if (get_synced_block_verifiers() == 0)
  {
    SYNC_CHECK_STATISTICS_DATABASE_ERROR("Could not get the synced block verifiers");
  }

  // get the database data hash for the reserve proofs database
  if (get_database_data_hash(data,DATABASE_NAME,DATABASE_COLLECTION,0) == 0)
  {
    SYNC_CHECK_STATISTICS_DATABASE_ERROR("Could not get the database data hash for the delegates database");
  }

  // create the message
  memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"data_hash\": \"",116);
  memcpy(message+116,data,DATA_HASH_LENGTH);
  memcpy(message+244,"\",\r\n}",5);

  // sign_data
  if (sign_data(message,0) == 0)
  { 
    SYNC_CHECK_STATISTICS_DATABASE_ERROR("Could not sign_data");
  }

  fprintf(stderr,"Sending all block verifiers a message to check if the statistics database is synced\n"); 

  if (SETTINGS == 1)
  {
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memset(data,0,strlen(data));
      memset(data2,0,sizeof(data2));
      if (send_and_receive_data_socket(data,synced_block_verifiers.synced_block_verifiers_IP_address[count],SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0 || verify_data(data,0,0) == 0)
      {
        memcpy(synced_block_verifiers.vote_settings[count],"connection_timeout",18);
        synced_block_verifiers.vote_settings_connection_timeout++;
      }
      else
      {
        parse_json_data(data,"statistics_database",data2,sizeof(data2));
        memcpy(synced_block_verifiers.vote_settings[count],data2,strnlen(data2,BUFFER_SIZE));
        if (memcmp(data2,"true",4) == 0)
        {
          synced_block_verifiers.vote_settings_true++;
        }
        else if (memcmp(data2,"false",5) == 0)
        {
          synced_block_verifiers.vote_settings_false++;
        }
      }   
    }

    if (synced_block_verifiers.vote_settings_false >= BLOCK_VERIFIERS_VALID_AMOUNT || synced_block_verifiers.vote_settings_connection_timeout >= BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      color_print("The statistics database is not synced","red");
      if (sync_statistics_database(SETTINGS) == 0)
      {
        SYNC_CHECK_STATISTICS_DATABASE_ERROR("Could not sync the statistics database database");
      }
    }
  }
  else if (SETTINGS == 2)
  {
    for (count = 0; count < NETWORK_DATA_NODES_AMOUNT; count++)
    {
      memset(data,0,strlen(data));
      memset(data2,0,sizeof(data2));
      if (send_and_receive_data_socket(data,network_data_nodes_list.network_data_nodes_IP_address[count],SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0 || verify_data(data,0,0) == 0)
      {
        memcpy(synced_block_verifiers.vote_settings[count],"connection_timeout",18);
        synced_block_verifiers.vote_settings_connection_timeout++;
      }
      else
      {
        parse_json_data(data,"statistics_database",data2,sizeof(data2));
        memcpy(synced_block_verifiers.vote_settings[count],data2,strnlen(data2,BUFFER_SIZE));
        if (memcmp(data2,"true",4) == 0)
        {
          synced_block_verifiers.vote_settings_true++;
        }
        else if (memcmp(data2,"false",5) == 0)
        {
          synced_block_verifiers.vote_settings_false++;
        }
      }   
    }

    if (synced_block_verifiers.vote_settings_false > 0 || synced_block_verifiers.vote_settings_connection_timeout > 0)
    {
      color_print("The statistics database is not synced","red");
      if (sync_statistics_database(SETTINGS) == 0)
      {
        SYNC_CHECK_STATISTICS_DATABASE_ERROR("Could not sync the statistics database database");
      }
    }
  }

  color_print("The statistics database is synced","green");
  
  return 1;

  #undef DATABASE_COLLECTION
  #undef SYNC_CHECK_STATISTICS_DATABASE_ERROR  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_statistics_database
Description: Syncs the reserve proofs database
Paramters:
  SETTINGS - 1 to sync from a random block verifier, 2 to sync from a random network data node
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_statistics_database(const int SETTINGS)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char data2[BUFFER_SIZE];
  size_t count = 0;
  size_t count2;
  int settings;

  // define macros
  #define DATABASE_COLLECTION "statistics"
  #define pointer_reset_all \
  free(data); \
  data = NULL;

  #define SYNC_STATISTICS_DATABASE_ERROR(message,settings) \
  if (settings == 0) \
  { \
    memcpy(error_message.function[error_message.total],"sync_statistics_database",24); \
    memcpy(error_message.data[error_message.total],message,sizeof(message)-1); \
    error_message.total++; \
    pointer_reset_all; \
    return 0; \
  } \
  else \
  { \
    memset(data,0,strlen(data)); \
    memcpy(data+strlen(data),message,sizeof(message)-1); \
    memcpy(data+strlen(data),synced_block_verifiers.synced_block_verifiers_IP_address[count],strnlen(synced_block_verifiers.synced_block_verifiers_IP_address[count],BUFFER_SIZE)); \
    memcpy(data+strlen(data),"\nConnecting to another block verifier",37); \
    color_print(data,"red"); \
    goto start; \
  }  

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    pointer_reset(data);
    memcpy(error_message.function[error_message.total],"sync_statistics_database",24);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  memset(data,0,strlen(data));
  memset(data2,0,sizeof(data2));
  
  start:

  /* select a random block verifier from the majority vote settings to sync the database from, making sure not to select your own block verifier node
     select a random network data node to sync from if
     there was a lot of connection_timeouts, to where a majority vote could not be calculated,
     there were more than BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT new block verifiers
  */
  if (synced_block_verifiers.vote_settings_connection_timeout < BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT && SETTINGS == 1)
  {
    count = (int)(rand() % BLOCK_VERIFIERS_AMOUNT-1);
    if (memcmp(synced_block_verifiers.vote_settings[count],"true",4) != 0 || strncmp(synced_block_verifiers.synced_block_verifiers_IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0)
    {
      goto start;
    }
  }
  else if (SETTINGS == 1)
  {
    count = (int)(rand() % BLOCK_VERIFIERS_AMOUNT-1);
    for (count2 = 0, settings = 0; count2 < NETWORK_DATA_NODES_AMOUNT; count2++)
    {
      if (strncmp(synced_block_verifiers.synced_block_verifiers_IP_address[count],network_data_nodes_list.network_data_nodes_IP_address[count2],BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0 && strncmp(synced_block_verifiers.synced_block_verifiers_IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) != 0)
      {
        settings = 1;
      }
    }
    if (settings != 1)    
    {
      goto start;
    }
  }
  else if (SETTINGS == 2)
  {
    count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT-1);
    if (strncmp(network_data_nodes_list.network_data_nodes_IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0)
    {
      goto start;
    }
  }

  memset(data,0,strlen(data));
  memcpy(data,"Connecting to block verifier ",29);
  memcpy(data+strlen(data),synced_block_verifiers.synced_block_verifiers_IP_address[count],strnlen(synced_block_verifiers.synced_block_verifiers_IP_address[count],BUFFER_SIZE));
  memcpy(data+strlen(data)," to sync the statistics database",32);
  color_print(data,"green");

  // get the database data hash
  if (get_database_data_hash(data,DATABASE_NAME,DATABASE_COLLECTION,0) == 0)
  {
    SYNC_STATISTICS_DATABASE_ERROR("Could not get the database data hash for the statistics database",0);
  }

  // create the message
  memset(data2,0,sizeof(data2));
  memcpy(data2,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"file\": \"",118);
  snprintf(data2+118,sizeof(data2)-119,"%zu",count);
  memcpy(data2+strlen(data2),"\",\r\n}",5);

  // sign_data
  if (sign_data(data2,0) == 0)
  { 
    SYNC_STATISTICS_DATABASE_ERROR("Could not sign_data",0);
  }
     
  memset(data,0,strlen(data));
  if (send_and_receive_data_socket(data,synced_block_verifiers.synced_block_verifiers_IP_address[count],SEND_DATA_PORT,data2,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
  {
    SYNC_STATISTICS_DATABASE_ERROR("Could not receive data from ",1);
  }

  if (verify_data(data,0,0) == 0)
  {
    SYNC_STATISTICS_DATABASE_ERROR("Could not verify data from ",1);
  }

  // parse the message
  memset(data2,0,sizeof(data2));
  if (parse_json_data(data,"statistics_database",data2,sizeof(data2)) == 0 || memcmp(data2,"",1) == 0)
  {
    SYNC_STATISTICS_DATABASE_ERROR("Could not receive data from ",1);
  }

  // delete the collection from the database
  delete_collection_from_database(DATABASE_NAME,DATABASE_COLLECTION,0);

  // add the data to the database
  memset(data,0,strlen(data));
  memcpy(data,data2,strlen(data2)-2);
  insert_multiple_documents_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,data,10485760,0);

  pointer_reset_all;
  return 1;

  #undef DATABASE_COLLECTION
  #undef pointer_reset_all
  #undef SYNC_STATISTICS_DATABASE_ERROR   
}