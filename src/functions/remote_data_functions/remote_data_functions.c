#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <arpa/nameser.h>
#include <resolv.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "initialize_and_reset_structs_define_macros.h"
#include "structures.h"
#include "variables.h"
#include "define_macros_test.h"

#include "blockchain_functions.h"
#include "block_verifiers_functions.h"
#include "block_verifiers_server_functions.h"
#include "block_verifiers_synchronize_server_functions.h"
#include "block_verifiers_synchronize_functions.h"
#include "database_functions.h"
#include "shared_delegate_website_thread_server_functions.h"
#include "insert_database_functions.h"
#include "read_database_functions.h"
#include "update_database_functions.h"
#include "delete_database_functions.h"
#include "count_database_functions.h"
#include "organize_functions.h"
#include "remote_data_functions.h"
#include "network_daemon_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "network_wallet_functions.h"
#include "server_functions.h"
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
Name: get_previous_block_producer
Description: Get the previous block producer
-----------------------------------------------------------------------------------------------------------
*/

int get_previous_block_producer(char *previous_block_producer)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  char* message;
  size_t count;
  size_t count2;
  
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));
  memset(previous_block_producer,0,strlen(previous_block_producer));  

  // check if it is a valid block height
  sscanf(current_block_height, "%zu", &count);
  count--;

  if (test_settings == 0)
  {
    if (count <= XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT)
    {
      return 0;
    }

    // create the message
    memcpy(data2,"{\"block_height\":\"",17);
    snprintf(data2+17,MAXIMUM_NUMBER_SIZE,"%zu",count);
    memcpy(data2+strlen(data2),"\"}",2);

    // get the current reserve_bytes database
    get_reserve_bytes_database(count,1);
    memcpy(data3,"reserve_bytes_",14);
    snprintf(data3+14,MAXIMUM_NUMBER_SIZE,"%zu",count);

    // get the previous blocks reserve bytes
    if (read_document_field_from_collection(database_name,data3,data2,"reserve_bytes",data) == 0)
    {    
      return 0;
    }
  }
  else if (test_settings == 1)
  {
    memcpy(data,NETWORK_BLOCK,sizeof(NETWORK_BLOCK)-1);
  }
  
  if ((message = strstr(data,BLOCKCHAIN_DATA_SEGMENT_STRING)) == NULL)
  {
    return 0;
  }

  memset(data3,0,sizeof(data3));

  memcpy(data3,&message[sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1],XCASH_WALLET_LENGTH*2);

  // convert the hexadecimal string to a string
  for (count = 0, count2 = 0; count < XCASH_WALLET_LENGTH*2; count2++, count += 2)
  {
    memset(data2,0,sizeof(data2));
    memcpy(data2,&data3[count],2);
    previous_block_producer[count2] = (char)strtol(data2, NULL, 16);
  }
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: validate_name
Description: Checks if the amount is valid
Parameters:
  MESSAGE - The amount
Return: 0 if an error has occured or invalid, 1 if valid
-----------------------------------------------------------------------------------------------------------
*/

int validate_amount(const char* MESSAGE)
{
  // Constants
  const size_t MESSAGE_LENGTH = strlen(MESSAGE);

  // define macros
  #define VALID_DATA "0123456789"

  // Variables
  char data[sizeof(VALID_DATA)];
  size_t count;
  size_t count2;

  memset(data,0,sizeof(data));
  memcpy(data,VALID_DATA,sizeof(VALID_DATA)-1);

  if (MESSAGE_LENGTH < 1 || MESSAGE_LENGTH > MAXIMUM_NUMBER_SIZE)
  {
    return 0;
  }

  // check if the amount is over the maximum supply
  sscanf(MESSAGE,"%zu", &count);

  if (count > XCASH_TOTAL_SUPPLY)
  {
    return 0;
  }
  
  // check if the delegate name has any invalid characters
  for (count = 0; count < MESSAGE_LENGTH; count++)
  {
    for (count2 = 0; count2 < (sizeof(VALID_DATA)-1); count2++)
    {
      if (strncmp(&MESSAGE[count],&data[count2],sizeof(char)) == 0)
      {
        break;
      }
    }
    if (count2 == (sizeof(VALID_DATA)-1))
    {
      return 0;
    }
  }
  return 1;

  #undef VALID_DATA
}



/*
-----------------------------------------------------------------------------------------------------------
Name: validate_name
Description: Checks if the name is valid
Parameters:
  MESSAGE - The name
Return: 0 if an error has occured or invalid, 1 if valid
-----------------------------------------------------------------------------------------------------------
*/

int validate_name(const char* MESSAGE)
{
  // Constants
  const size_t MESSAGE_LENGTH = strlen(MESSAGE);

  // define macros
  #define VALID_DATA "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

  // Variables
  char data[sizeof(VALID_DATA)];
  size_t count;
  size_t count2;

  memset(data,0,sizeof(data));
  memcpy(data,VALID_DATA,sizeof(VALID_DATA)-1);

  if (MESSAGE_LENGTH < 1 || MESSAGE_LENGTH > REMOTE_DATA_NAME_MAXIMUM_LENGTH)
  {
    return 0;
  }
  
  // check if the delegate name has any invalid characters
  for (count = 0; count < MESSAGE_LENGTH; count++)
  {
    for (count2 = 0; count2 < (sizeof(VALID_DATA)-1); count2++)
    {
      if (strncmp(&MESSAGE[count],&data[count2],sizeof(char)) == 0)
      {
        break;
      }
    }
    if (count2 == (sizeof(VALID_DATA)-1))
    {
      return 0;
    }
  }
  return 1;

  #undef VALID_DATA
}



/*
-----------------------------------------------------------------------------------------------------------
Name: validate_name_settings
Description: Checks if the name is valid with the timestamp and tx_hash
Parameters:
  MESSAGE - The name
Return: 0 if an error has occured or invalid, 1 if valid
-----------------------------------------------------------------------------------------------------------
*/

int validate_name_settings(const char* MESSAGE)
{
  // Variables
  char data[SMALL_BUFFER_SIZE];
  char data2[SMALL_BUFFER_SIZE];
  char data3[SMALL_BUFFER_SIZE];
  int name_settings_length;
  long int registration_length;

  // define macros
  #define DATABASE_COLLECTION "remote_data"

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  // check if its just the name or the name and the settings
  name_settings_length = strstr(MESSAGE,".xcash") != NULL ? 6 : strstr(MESSAGE,".sxcash") != NULL ? 7 : strstr(MESSAGE,".pxcash") != NULL ? 7 : strstr(MESSAGE,".website") != NULL ? 8 : 0;

  // create the message
  memcpy(data,"{\"name\":\"",9);
  memcpy(data+strlen(data),MESSAGE,strlen(MESSAGE)-name_settings_length);
  memcpy(data+strlen(data),"\"}",2);

  if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,data,"timestamp",data2) == 1 && read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,data,"tx_hash",data3) == 1)
  {
    // check if the name has a valid registration and a valid tx_hash
    sscanf(data2,"%ld", &registration_length);
    if ((registration_length + REMOTE_DATA_REGISTRATION_LENGTH) < time(NULL) || strlen(data3) != TRANSACTION_HASH_LENGTH)
    {
      return 0;
    }
  }
  else
  {
    return 0;
  }
  return 1;

  #undef DATABASE_COLLECTION
}



/*
-----------------------------------------------------------------------------------------------------------
Name: document_field_contains_string_from_collection
Description: Checks if the database field contains the string in the database collection
Parameters:
  DATABASE - The database name
  COLLECTION - The collection name
  FIELD_NAME - The field of the document data to read
  DATA - The data to search the field for
Return: -1 if an error has occured, 0 if false, 1 if true
-----------------------------------------------------------------------------------------------------------
*/

int document_field_contains_string_from_collection(const char* DATABASE, const char* COLLECTION, const char* FIELD_NAME, const char* DATA)
{
  // Variables
  char data[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count = 0;
  int count2 = 0;
  size_t counter = 0;
  struct database_multiple_documents_fields database_multiple_documents_fields;
  int document_count = 0;
  int total_database_fields = 0;
  int settings = 0;

  // define macros
  #define DOCUMENT_FIELD_CONTAINS_STRING_FROM_COLLECTION_ERROR(settings,MESSAGE) \
  if (debug_settings == 1) \
  { \
  memcpy(error_message.function[error_message.total],"document_field_contains_string_from_collection",46); \
  memcpy(error_message.data[error_message.total],MESSAGE,sizeof(MESSAGE)-1); \
  error_message.total++; \
  } \
  if ((settings) == 0) \
  { \
    POINTER_RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,total_database_fields); \
  } \
  return -1;

  // get the total database fields
  if (strstr(COLLECTION,"reserve_bytes") != NULL)
  {
    total_database_fields = TOTAL_RESERVE_BYTES_DATABASE_FIELDS;
  }
  else if (strstr(COLLECTION,"reserve_proofs") != NULL)
  {
    total_database_fields = TOTAL_RESERVE_PROOFS_DATABASE_FIELDS;
  }
  else if (strstr(COLLECTION,"statistics") != NULL)
  {
    total_database_fields = TOTAL_STATISTICS_DATABASE_FIELDS;
  }
  else if (strstr(COLLECTION,"public_addresses_payments") != NULL)
  {
    total_database_fields = TOTAL_PUBLIC_ADDRESSES_PAYMENTS_DATABASE_FIELDS;
  }
  else if (strstr(COLLECTION,"public_addresses") != NULL)
  {
    total_database_fields = TOTAL_PUBLIC_ADDRESSES_DATABASE_FIELDS;
  }
  else if (strstr(COLLECTION,"blocks_found") != NULL)
  {
    total_database_fields = TOTAL_BLOCKS_FOUND_DATABASE_FIELDS;
  }
  else if (strstr(COLLECTION,"remote_data_delegates") != NULL)
  {
    total_database_fields = TOTAL_REMOTE_DATA_DELEGATES_DATABASE_FIELDS;
  }
  else if (strstr(COLLECTION,"remote_data") != NULL)
  {
    total_database_fields = TOTAL_REMOTE_DATA_DATABASE_FIELDS;
  }
  else if (strstr(COLLECTION,"delegates") != NULL)
  {
    total_database_fields = TOTAL_DELEGATES_DATABASE_FIELDS;
  }

  // get the total delegates
  if ((document_count = count_all_documents_in_collection(DATABASE,COLLECTION)) <= 0)
  {
    return -1;
  }

  memset(data,0,sizeof(data));

  // initialize the database_multiple_documents_fields struct
  INITIALIZE_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,count2,document_count,total_database_fields,"document_field_contains_string_from_collection",data,current_date_and_time,current_UTC_date_and_time);

  if (read_multiple_documents_all_fields_from_collection(DATABASE,COLLECTION,"",&database_multiple_documents_fields,0,document_count,0,"") == 0)
  {
    DOCUMENT_FIELD_CONTAINS_STRING_FROM_COLLECTION_ERROR(0,"Could not get the remote data delegates");
  }

  // get the item settings
  for (count = 0; count < total_database_fields; count++)
  {
    if (strncmp(database_multiple_documents_fields.item[0][count],FIELD_NAME,BUFFER_SIZE) == 0)
    {
      break;
    }
  }

  if (count == total_database_fields)
  {
    return -1;
  }

  for (count2 = 0; count2 < document_count; count2++)
  {
    if (strstr(database_multiple_documents_fields.value[count2][count],DATA) != NULL)
    {
      settings = 1;
      break;
    }
  }

  POINTER_RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,total_database_fields);

  return settings;
  
  #undef DOCUMENT_FIELD_CONTAINS_STRING_FROM_COLLECTION_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: remote_data_update_delegates_statistics
Description: Updates the remote data delegates statistics
Parameters:
  DELEGATE_NAME - The name of the delegate to update
  DELEGATE_AMOUNT - The amount
Return: 1 if successful, otherwise 0
-----------------------------------------------------------------------------------------------------------
*/

int remote_data_update_delegates_statistics(const char* DELEGATE_NAME,const char* DELEGATE_AMOUNT)
{
  // Variables
  char data[SMALL_BUFFER_SIZE];
  char data2[SMALL_BUFFER_SIZE];
  char data3[SMALL_BUFFER_SIZE];
  size_t count = 0;
  size_t count2 = 0;

  // define macros
  #define DATABASE_COLLECTION "remote_data_delegates"
  #define REMOTE_DATA_UPDATE_DELEGATES_STATISTICS_ERROR \
  if (debug_settings == 1) \
  { \
  memcpy(error_message.function[error_message.total],"remote_data_update_delegates_statistics",39); \
  memcpy(error_message.data[error_message.total],"Could not update the delegates statistics for the remote data",61); \
  error_message.total++; \
  } \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  // create the message
  memcpy(data,"{\"name\":\"",9);
  memcpy(data+strlen(data),DELEGATE_NAME,strlen(DELEGATE_NAME));
  memcpy(data+strlen(data),"\"}",2);

  // update the total_registered_renewed_amount
  if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,data,"total_registered_renewed_amount",data2) == 0)
  {
    REMOTE_DATA_UPDATE_DELEGATES_STATISTICS_ERROR;
  }
  
  sscanf(data2, "%zu", &count);
  count++;

  memcpy(data3,"{\"total_registered_renewed_amount\":\"",36);
  snprintf(data3+strlen(data3),MAXIMUM_NUMBER_SIZE,"%zu",count);
  memcpy(data3+strlen(data3),"\"}",2);

  if (update_document_from_collection(remote_data_database_name,DATABASE_COLLECTION,data,data3) == 0)
  {
    REMOTE_DATA_UPDATE_DELEGATES_STATISTICS_ERROR;
  }

  // update the total_amount
  memset(data2,0,sizeof(data2));
  if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,data,"total_amount",data2) == 0)
  {
    REMOTE_DATA_UPDATE_DELEGATES_STATISTICS_ERROR;
  }
  
  sscanf(data2, "%zu", &count);
  sscanf(DELEGATE_AMOUNT, "%zu", &count2);
  count += count2;

  memset(data3,0,sizeof(data3));
  memcpy(data3,"{\"total_amount\":\"",17);
  snprintf(data3+strlen(data3),MAXIMUM_NUMBER_SIZE,"%zu",count);
  memcpy(data3+strlen(data3),"\"}",2);

  if (update_document_from_collection(remote_data_database_name,DATABASE_COLLECTION,data,data3) == 0)
  {
    REMOTE_DATA_UPDATE_DELEGATES_STATISTICS_ERROR;
  }

  return 1;

  #undef DATABASE_COLLECTION
  #undef REMOTE_DATA_UPDATE_DELEGATES_STATISTICS_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: remote_data_validate_tx_hash
Description: Validates the tx_hash for the remote data purchase
Parameters:
  public_address - The public address purchasing the name
  delegates_public_address - The public address of the delegate
  delegates_amount - The delegates amount
Return: 0 if an error has occured or invalid, 1 if valid
-----------------------------------------------------------------------------------------------------------
*/

int remote_data_validate_tx_hash(const char* tx_hash,const char* public_address,const char* delegates_public_address,const char* delegates_amount)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char* message2;
  char message[BUFFER_SIZE];
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  char delegates_public_address_data[(XCASH_WALLET_LENGTH*2)+1];
  char public_address_data[(XCASH_WALLET_LENGTH*2)+1];
  char tx_private_key[TRANSACTION_HASH_LENGTH+1];
  size_t count;
  size_t count2;

  // define macros
  #define DATABASE_COLLECTION "remote_data"

  memset(message,0,sizeof(message));
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));
  memset(tx_private_key,0,sizeof(tx_private_key));
  memset(delegates_public_address_data,0,sizeof(delegates_public_address_data));
  memset(public_address_data,0,sizeof(public_address_data));

  // convert the public address to a string
  for (count2 = 0, count = 0; count2 < XCASH_WALLET_LENGTH; count2++, count += 2)
  {
    snprintf(public_address_data+count,MAXIMUM_NUMBER_SIZE,"%02x",public_address[count2] & 0xFF);
  }

  // convert the delegates public address to a string
  for (count2 = 0, count = 0; count2 < XCASH_WALLET_LENGTH; count2++, count += 2)
  {
    snprintf(delegates_public_address_data+count,MAXIMUM_NUMBER_SIZE,"%02x",delegates_public_address[count2] & 0xFF);
  }

  // check if the tx_hash is already in the database
  memcpy(data,"{\"tx_hash\":\"",12);
  memcpy(data+strlen(data),tx_hash,TRANSACTION_HASH_LENGTH);
  memcpy(data+strlen(data),"\"}",2);
  if (count_documents_in_collection(remote_data_database_name,DATABASE_COLLECTION,data) > 0)
  {
    return 0;
  }

  // get the transaction details
  memset(data,0,sizeof(data));
  memcpy(message,"{\"txs_hashes\":[\"",16);
  memcpy(message+strlen(message),tx_hash,TRANSACTION_HASH_LENGTH);
  memcpy(message+strlen(message),"\"]}",3);

  if (send_http_request(data,XCASH_DPOPS_delegates_IP_address,"/get_transactions",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,message,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) <= 0 || parse_json_data(data,"txs_as_hex",data2, BUFFER_SIZE) == 0)
  {
    return 0;
  }

  // verify the transaction is a public transaction and get the tx_private_key of the transaction
  if ((message2 = strstr(data2,PUBLIC_TRANSACTION_VERIFICATION)) == NULL)
  {
    return 0;
  }
  
  memcpy(tx_private_key,&message2[sizeof(PUBLIC_TRANSACTION_VERIFICATION)-1],TRANSACTION_HASH_LENGTH);

  // verfiy the transaction came from the address
  message2 = strstr(data2,"7c584341") + 10;
  if (strstr(message2,public_address_data) == NULL)
  {
    return 0;
  }

  // verify the transaction went to the delegate
  message2 = strstr(data2,"7c584341") + 2;
  if (memcmp(message2,delegates_public_address_data,XCASH_WALLET_LENGTH*2) != 0)
  {
    return 0;
  }

  // verify the transaction is for the correct amount
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));
  memcpy(message,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"check_tx_key\",\"params\":{\"txid\":\"",68);
  memcpy(message+strlen(message),tx_hash,TRANSACTION_HASH_LENGTH);
  memcpy(message+strlen(message),"\",\"tx_key\":\"",12);
  memcpy(message+strlen(message),tx_private_key,TRANSACTION_HASH_LENGTH);
  memcpy(message+strlen(message),"\",\"address\":\"",13);
  memcpy(message+strlen(message),delegates_public_address,XCASH_WALLET_LENGTH);
  memcpy(message+strlen(message),"\"}}",3);

  if (send_http_request(data,XCASH_DPOPS_delegates_IP_address,"/json_rpc",XCASH_WALLET_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,message,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) <= 0 || parse_json_data(data,"received",data2, BUFFER_SIZE) == 0)
  {
    return 0;
  }

  sscanf(delegates_amount, "%zu", &count);
  sscanf(data2, "%zu", &count2);

  return (count2 / XCASH_WALLET_DECIMAL_PLACES_AMOUNT) >= count ? 1 : 0;

  #undef DATABASE_COLLECTION
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_remote_data_get_statistics
Description: Runs the code when the server receives /remotedatagetstatistics
Parameters:
  CLIENT_SOCKET - The socket to send data to
  DATA - The data
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_remote_data_get_statistics(const int CLIENT_SOCKET)
{
  // Variables
  char message[SMALL_BUFFER_SIZE];
  char buffer[1024];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count = 0;
  size_t counter = 0;
  size_t total_registered_renewed_amount = 0;
  size_t total_amount = 0;
  struct database_multiple_documents_fields database_multiple_documents_fields;
  int document_count = 0;

  // define macros
  #define DATABASE_COLLECTION "remote_data_delegates"
  #define REMOTE_DATA_GET_STATISTICS_ERROR(settings,MESSAGE) \
  if (debug_settings == 1) \
  { \
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_remote_data_get_statistics",53); \
    memcpy(error_message.data[error_message.total],MESSAGE,sizeof(MESSAGE)-1); \
    error_message.total++; \
  } \
  memset(message,0,strlen(message)); \
  memcpy(message,"{\"Error\":\"Could not get the remote data statistics\"}",52); \
  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),400,"application/json"); \
  if ((settings) == 0) \
  { \
    POINTER_RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,TOTAL_REMOTE_DATA_DELEGATES_DATABASE_FIELDS); \
  } \
  return 0;
  
  memset(message,0,sizeof(message));

  // get the total delegates
  if ((document_count = count_all_documents_in_collection(remote_data_database_name,DATABASE_COLLECTION)) <= 0)
  {
    REMOTE_DATA_GET_STATISTICS_ERROR(1,"Could not get the remote data delegates");
  }

  memset(buffer,0,sizeof(buffer));

  // initialize the database_multiple_documents_fields struct
  INITIALIZE_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,document_count,TOTAL_REMOTE_DATA_DELEGATES_DATABASE_FIELDS,"server_receive_data_socket_remote_data_get_delegates_information",buffer,current_date_and_time,current_UTC_date_and_time);

  if (read_multiple_documents_all_fields_from_collection(remote_data_database_name,DATABASE_COLLECTION,"",&database_multiple_documents_fields,0,document_count,0,"") == 0)
  {
    REMOTE_DATA_GET_STATISTICS_ERROR(0,"Could not get the remote data delegates");
  }

  for (count = 0; count < document_count; count++)
  {
    sscanf(database_multiple_documents_fields.value[count][3],"%zu", &counter);
    total_registered_renewed_amount += counter;
    sscanf(database_multiple_documents_fields.value[count][4],"%zu", &counter);
    total_amount += counter;
  }

  // create the message
  memcpy(message,"{\"total_registered_renewed_amount\":\"",36);
  snprintf(message+strlen(message),MAXIMUM_NUMBER_SIZE,"%zu",total_registered_renewed_amount);
  memcpy(message+strlen(message),"\",\"total_amount\":\"",18);
  snprintf(message+strlen(message),MAXIMUM_NUMBER_SIZE,"%zu",total_amount);
  memcpy(message+strlen(message),"\"}",2);
  
  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),200,"application/json");
  
  POINTER_RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,TOTAL_REMOTE_DATA_DELEGATES_DATABASE_FIELDS);
  return 1;

  #undef DATABASE_COLLECTION
  #undef REMOTE_DATA_GET_STATISTICS_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_nodes_to_network_data_nodes_remote_data_get_address_settings
Description: Runs the code when the server receives the NODE_TO_NETWORK_DATA_NODES_REMOTE_DATA_GET_ADDRESS_SETTINGS message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  MESSAGE - The message
-----------------------------------------------------------------------------------------------------------
*/

void server_receive_data_socket_nodes_to_network_data_nodes_remote_data_get_address_settings(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char public_address[XCASH_WALLET_LENGTH+1];
  char message[SMALL_BUFFER_SIZE];
  int count;
  int count2;
  size_t data_size;

  // define macros
  #define DATABASE_COLLECTION "remote_data"
  #define REMOTE_DATA_GET_ADDRESS_SETTINGS_ERROR \
  if (debug_settings == 1) \
  { \
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_nodes_to_network_data_nodes_remote_data_get_address_settings",87); \
    memcpy(error_message.data[error_message.total],"Error: Could not get the address settings",41); \
    error_message.total++; \
  } \
  memset(message,0,strlen(message)); \
  memcpy(message,"Error: Could not get the address settings}",42); \
  send_data(CLIENT_SOCKET,(unsigned char*)message,0,0,""); \
  return;

  #define REMOTE_DATA_GET_ADDRESS_SETTINGS_STATUS(settings) \
  memset(message,0,strlen(message)); \
  memcpy(message,settings,strlen(settings)); \
  memcpy(message+strlen(message),"}",1); \
  send_data(CLIENT_SOCKET,(unsigned char*)message,0,0,""); \
  return;
  
  memset(public_address,0,sizeof(public_address));
  memset(message,0,sizeof(message));

  // parse the message
  for (count = 0, count2 = 0; count < REMOTE_DATA_GET_ADDRESS_SETTINGS_PARAMETER_AMOUNT; count++)
  {
    if (count == 1)
    {
      if ((data_size = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2) != XCASH_WALLET_LENGTH)
      {
        REMOTE_DATA_GET_ADDRESS_SETTINGS_ERROR;
      }
      memcpy(public_address,&MESSAGE[count2],data_size);
    }
    count2 = (int)(strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) + 1);
  }
  
  // the public address can only be in address, saddress, paddress, saddress_list or paddress_list. If not their its address

  // create the message
  memcpy(message,"{\"saddress\":\"",13);
  memcpy(message+strlen(message),public_address,XCASH_WALLET_LENGTH);
  memcpy(message+strlen(message),"\"}",2);

  if (count_documents_in_collection(remote_data_database_name,DATABASE_COLLECTION,message) > 0)
  {    
    REMOTE_DATA_GET_ADDRESS_SETTINGS_STATUS("saddress");
  }

  // create the message
  memset(message,0,sizeof(message));
  memcpy(message,"{\"paddress\":\"",13);
  memcpy(message+strlen(message),public_address,XCASH_WALLET_LENGTH);
  memcpy(message+strlen(message),"\"}",2);

  if (count_documents_in_collection(remote_data_database_name,DATABASE_COLLECTION,message) > 0)
  {    
    REMOTE_DATA_GET_ADDRESS_SETTINGS_STATUS("paddress");
  }

  // create the message
  memset(message,0,sizeof(message));
  memcpy(message,public_address,XCASH_WALLET_LENGTH);
  memcpy(message+strlen(message),"|",1);

  if (document_field_contains_string_from_collection(remote_data_database_name,DATABASE_COLLECTION,"saddress_list",message) == 1)
  {   
    REMOTE_DATA_GET_ADDRESS_SETTINGS_STATUS("saddress");
  }

  if (document_field_contains_string_from_collection(remote_data_database_name,DATABASE_COLLECTION,"paddress_list",message) == 1)
  {   
    REMOTE_DATA_GET_ADDRESS_SETTINGS_STATUS("paddress");
  }

  // the public address is either address or not in the database. either way return address
  REMOTE_DATA_GET_ADDRESS_SETTINGS_STATUS("address");

  #undef DATABASE_COLLECTION
  #undef REMOTE_DATA_GET_ADDRESS_SETTINGS_ERROR
  #undef REMOTE_DATA_GET_ADDRESS_SETTINGS_STATUS
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_remote_data_get_address_settings
Description: Runs the code when the server receives /remotedatagetaddresssettings
Parameters:
  CLIENT_SOCKET - The socket to send data to
  DATA - The data
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_remote_data_get_address_settings(const int CLIENT_SOCKET, const char* DATA)
{
  // Variables
  char public_address[BUFFER_SIZE];
  char message[SMALL_BUFFER_SIZE];

  // define macros
  #define DATABASE_COLLECTION "remote_data"
  #define REMOTE_DATA_GET_ADDRESS_SETTINGS_ERROR(settings) \
  if (debug_settings == 1) \
  { \
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_remote_data_get_address_settings",59); \
    memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
    error_message.total++; \
  } \
  memset(message,0,strlen(message)); \
  memcpy(message,"{\"Error\":\"Could not get the address settings\"}",46); \
  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),400,"application/json"); \
  return 0;

  #define REMOTE_DATA_GET_ADDRESS_SETTINGS_STATUS(settings) \
  memset(message,0,strlen(message)); \
  memcpy(message,"{\"address_settings\":\"",21); \
  memcpy(message+strlen(message),settings,strlen(settings)); \
  memcpy(message+strlen(message),"\"}",2); \
  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),200,"application/json"); \
  return 1;
  
  memset(public_address,0,sizeof(public_address));
  memset(message,0,sizeof(message));

  // get the parameter1
  memcpy(public_address,&DATA[45],(strnlen(DATA,sizeof(public_address)) - strnlen(strstr(DATA," HTTP/"),sizeof(public_address)))-45);

  // error check
  if (strncmp(public_address,"",BUFFER_SIZE) == 0 || strlen(public_address) != XCASH_WALLET_LENGTH)
  {
    REMOTE_DATA_GET_ADDRESS_SETTINGS_ERROR("Invalid parameters");
  } 

  // the public address can only be in address, saddress, paddress, saddress_list or paddress_list. If not their its address

  // create the message
  memcpy(message,"{\"saddress\":\"",13);
  memcpy(message+strlen(message),public_address,XCASH_WALLET_LENGTH);
  memcpy(message+strlen(message),"\"}",2);

  if (count_documents_in_collection(remote_data_database_name,DATABASE_COLLECTION,message) > 0)
  {    
    REMOTE_DATA_GET_ADDRESS_SETTINGS_STATUS("saddress");
  }

  // create the message
  memset(message,0,sizeof(message));
  memcpy(message,"{\"paddress\":\"",13);
  memcpy(message+strlen(message),public_address,XCASH_WALLET_LENGTH);
  memcpy(message+strlen(message),"\"}",2);

  if (count_documents_in_collection(remote_data_database_name,DATABASE_COLLECTION,message) > 0)
  {    
    REMOTE_DATA_GET_ADDRESS_SETTINGS_STATUS("paddress");
  }

  // create the message
  memset(message,0,sizeof(message));
  memcpy(message,public_address,XCASH_WALLET_LENGTH);
  memcpy(message+strlen(message),"|",1);

  if (document_field_contains_string_from_collection(remote_data_database_name,DATABASE_COLLECTION,"saddress_list",message) == 1)
  {   
    REMOTE_DATA_GET_ADDRESS_SETTINGS_STATUS("saddress");
  }

  if (document_field_contains_string_from_collection(remote_data_database_name,DATABASE_COLLECTION,"paddress_list",message) == 1)
  {   
    REMOTE_DATA_GET_ADDRESS_SETTINGS_STATUS("paddress");
  }

  // the public address is either address or not in the database. either way return address
  REMOTE_DATA_GET_ADDRESS_SETTINGS_STATUS("address");

  #undef DATABASE_COLLECTION
  #undef REMOTE_DATA_GET_ADDRESS_SETTINGS_ERROR
  #undef REMOTE_DATA_GET_ADDRESS_SETTINGS_STATUS
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_nodes_to_network_data_nodes_remote_data_get_address_from_name
Description: Runs the code when the server receives the NODE_TO_NETWORK_DATA_NODES_REMOTE_DATA_GET_ADDRESS_FROM_NAME message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  MESSAGE - The message
-----------------------------------------------------------------------------------------------------------
*/

void server_receive_data_socket_nodes_to_network_data_nodes_remote_data_get_address_from_name(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char name[REMOTE_DATA_NAME_MAXIMUM_LENGTH+1];
  char message[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  int count;
  int count2;
  int name_settings_length;
  size_t data_size;

  // define macros
  #define DATABASE_COLLECTION "remote_data"
  #define REMOTE_DATA_GET_ADDRESS_FROM_NAME_ERROR \
  if (debug_settings == 1) \
  { \
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_nodes_to_network_data_nodes_remote_data_get_address_from_name",88); \
    memcpy(error_message.data[error_message.total],"Error: Could not get the address settings",41); \
    error_message.total++; \
  } \
  memset(message,0,strlen(message)); \
  memcpy(message,"Error: Could not get the address settings}",42); \
  send_data(CLIENT_SOCKET,(unsigned char*)message,0,0,""); \
  return;

  #define REMOTE_DATA_GET_ADDRESS_FROM_NAME_STATUS(settings) \
  memset(message,0,strlen(message)); \
  memcpy(message,settings,strlen(settings)); \
  memcpy(message+strlen(message),"}",1); \
  send_data(CLIENT_SOCKET,(unsigned char*)message,0,0,""); \
  return;
  
  memset(name,0,sizeof(name));
  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));

  // parse the message
  for (count = 0, count2 = 0; count < REMOTE_DATA_GET_ADDRESS_FROM_NAME_PARAMETER_AMOUNT; count++)
  {
    if (count == 1)
    {
      if ((data_size = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2) > REMOTE_DATA_NAME_MAXIMUM_LENGTH)
      {
        REMOTE_DATA_GET_ADDRESS_FROM_NAME_ERROR;
      }
      memcpy(name,&MESSAGE[count2],data_size);
    }
    count2 = (int)(strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) + 1);
  }

  // validate the name settings
  name_settings_length = strstr(name,".xcash") != NULL ? 6 : strstr(name,".sxcash") != NULL ? 7 : strstr(name,".pxcash") != NULL ? 7 : strstr(name,".website") != NULL ? 8 : 0;
  if (name_settings_length == 0)
  {
    REMOTE_DATA_GET_ADDRESS_FROM_NAME_ERROR;
  }
  
  // check if the tx_hash is empty or the name is expired
  if (validate_name_settings(name) == 0)
  {
    REMOTE_DATA_GET_ADDRESS_FROM_NAME_ERROR;
  }

  // create the message
  memcpy(message,"{\"name\":\"",9);
  memcpy(message+strlen(message),name,strlen(name)-name_settings_length);
  memcpy(message+strlen(message),"\"}",2);

  // get the remote data
  memset(data2,0,sizeof(data2));
  if (strstr(name,".xcash") != NULL)
  {
    if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,message,"address",data2) == 1)
    {
      REMOTE_DATA_GET_ADDRESS_FROM_NAME_STATUS(data2);
    }
  }
  else if (strstr(name,".sxcash") != NULL)
  {
    if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,message,"saddress",data2) == 1)
    {
      REMOTE_DATA_GET_ADDRESS_FROM_NAME_STATUS(data2);
    }
  }
  else if (strstr(name,".pxcash") != NULL)
  {
    if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,message,"paddress",data2) == 1)
    {
      REMOTE_DATA_GET_ADDRESS_FROM_NAME_STATUS(data2);
    }
  }
  else if (strstr(name,".website") != NULL)
  {
    if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,message,"website",data2) == 1)
    {
      REMOTE_DATA_GET_ADDRESS_FROM_NAME_STATUS(data2);
    }
  }
  
  REMOTE_DATA_GET_ADDRESS_FROM_NAME_ERROR;

  #undef DATABASE_COLLECTION
  #undef REMOTE_DATA_GET_ADDRESS_FROM_NAME_ERROR
  #undef REMOTE_DATA_GET_ADDRESS_FROM_NAME_STATUS
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_remote_data_get_address_from_name
Description: Runs the code when the server receives /remotedatagetaddressfromname
Parameters:
  CLIENT_SOCKET - The socket to send data to
  DATA - The data
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_remote_data_get_address_from_name(const int CLIENT_SOCKET, const char* DATA)
{
  // Variables
  char name[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char message[BUFFER_SIZE];
  int name_settings_length;

  // define macros
  #define DATABASE_COLLECTION "remote_data"
  #define REMOTE_DATA_GET_ADDRESS_FROM_NAME_ERROR \
  if (debug_settings == 1) \
  { \
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_remote_data_get_address_from_name",60); \
    memcpy(error_message.data[error_message.total],"Invalid parameters",18); \
    error_message.total++; \
  } \
  memset(message,0,strlen(message)); \
  memcpy(message,"{\"Error\":\"Could not get the remote_data\"}",41); \
  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),400,"application/json"); \
  return 0;

  #define REMOTE_DATA_GET_ADDRESS_FROM_NAME_STATUS(settings) \
  memset(message,0,strlen(message)); \
  memcpy(message,"{\"remote_data\":\"",16); \
  memcpy(message+strlen(message),settings,strlen(settings)); \
  memcpy(message+strlen(message),"\"}",2); \
  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),200,"application/json"); \
  return 1;
  
  memset(name,0,sizeof(name));
  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));

  // get the parameter1
  memcpy(name,&DATA[45],(strnlen(DATA,sizeof(name)) - strnlen(strstr(DATA," HTTP/"),sizeof(name)))-45);

  // error check
  if (strncmp(name,"",BUFFER_SIZE) == 0 || strlen(name) > REMOTE_DATA_NAME_MAXIMUM_LENGTH)
  {
    REMOTE_DATA_GET_ADDRESS_FROM_NAME_ERROR;
  }

  // validate the name settings
  name_settings_length = strstr(name,".xcash") != NULL ? 6 : strstr(name,".sxcash") != NULL ? 7 : strstr(name,".pxcash") != NULL ? 7 : strstr(name,".website") != NULL ? 8 : 0;
  if (name_settings_length == 0)
  {
    REMOTE_DATA_GET_ADDRESS_FROM_NAME_ERROR;
  }

  // check if the tx_hash is empty or the name is expired
  if (validate_name_settings(name) == 0)
  {
    REMOTE_DATA_GET_ADDRESS_FROM_NAME_ERROR;
  }

  // create the message
  memcpy(message,"{\"name\":\"",9);
  memcpy(message+strlen(message),name,strlen(name)-name_settings_length);
  memcpy(message+strlen(message),"\"}",2);

  // get the remote data
  memset(data2,0,sizeof(data2));
  if (strstr(name,".xcash") != NULL)
  {
    if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,message,"address",data2) == 1)
    {
      REMOTE_DATA_GET_ADDRESS_FROM_NAME_STATUS(data2);
    }
  }
  else if (strstr(name,".sxcash") != NULL)
  {
    if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,message,"saddress",data2) == 1)
    {
      REMOTE_DATA_GET_ADDRESS_FROM_NAME_STATUS(data2);
    }
  }
  else if (strstr(name,".pxcash") != NULL)
  {
    if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,message,"paddress",data2) == 1)
    {
      REMOTE_DATA_GET_ADDRESS_FROM_NAME_STATUS(data2);
    }
  }
  else if (strstr(name,".website") != NULL)
  {
    if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,message,"website",data2) == 1)
    {
      REMOTE_DATA_GET_ADDRESS_FROM_NAME_STATUS(data2);
    }
  }
  
  REMOTE_DATA_GET_ADDRESS_FROM_NAME_ERROR;

  #undef DATABASE_COLLECTION
  #undef REMOTE_DATA_GET_ADDRESS_FROM_NAME_ERROR
  #undef REMOTE_DATA_GET_ADDRESS_FROM_NAME_STATUS
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_nodes_to_network_data_nodes_remote_data_get_information_from_name
Description: Runs the code when the server receives the NODE_TO_NETWORK_DATA_NODES_REMOTE_DATA_GET_INFORMATION_FROM_NAME message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  MESSAGE - The message
-----------------------------------------------------------------------------------------------------------
*/

void server_receive_data_socket_nodes_to_network_data_nodes_remote_data_get_information_from_name(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char name[REMOTE_DATA_NAME_MAXIMUM_LENGTH+1];
  char message[BUFFER_SIZE];
  char message2[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  int count;
  int count2;
  size_t data_size;

  // define macros
  #define DATABASE_COLLECTION "remote_data"
  #define REMOTE_DATA_GET_INFORMATION_FROM_NAME_ERROR \
  if (debug_settings == 1) \
  { \
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_nodes_to_network_data_nodes_remote_data_get_information_from_name",92); \
    memcpy(error_message.data[error_message.total],"Error: Could not get the address settings",41); \
    error_message.total++; \
  } \
  memset(message,0,strlen(message)); \
  memcpy(message,"Error: Could not get the information}",37); \
  send_data(CLIENT_SOCKET,(unsigned char*)message,0,0,""); \
  return;
  
  memset(name,0,sizeof(name));
  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));
  memset(message2,0,sizeof(message2));

  // parse the message
  for (count = 0, count2 = 0; count < REMOTE_DATA_GET_ADDRESS_FROM_NAME_PARAMETER_AMOUNT; count++)
  {
    if (count == 1)
    {
      if ((data_size = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2) > REMOTE_DATA_NAME_MAXIMUM_LENGTH)
      {
        REMOTE_DATA_GET_INFORMATION_FROM_NAME_ERROR;
      }
      memcpy(name,&MESSAGE[count2],data_size);
    }
    count2 = (int)(strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) + 1);
  }

  // check if the tx_hash is empty or the name is expired
  if (validate_name_settings(name) == 0)
  {
    REMOTE_DATA_GET_INFORMATION_FROM_NAME_ERROR;
  }

  // create the message
  memset(message2,0,sizeof(message2));  
  memcpy(message2,"{\"name\":\"",9);
  memcpy(message2+strlen(message2),name,strlen(name));
  memcpy(message2+strlen(message2),"\"}",2);

  // get the remote data
  memset(message,0,sizeof(message));  
  memcpy(message,"Information for ",16);
  memcpy(message+strlen(message),name,strlen(name));
  memcpy(message+strlen(message),":|address: ",11);
  memset(data2,0,sizeof(data2));
  if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,message2,"address",data2) == 0)
  {
    REMOTE_DATA_GET_INFORMATION_FROM_NAME_ERROR;
  }
  memcpy(message+strlen(message),data2,strlen(data2));
  memcpy(message+strlen(message),"|private only address (saddress): ",34);
  memset(data2,0,sizeof(data2));
  if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,message2,"saddress",data2) == 0)
  {
    REMOTE_DATA_GET_INFORMATION_FROM_NAME_ERROR;
  }
  memcpy(message+strlen(message),data2,strlen(data2));
  memcpy(message+strlen(message),"|public only address (paddress): ",33);
  memset(data2,0,sizeof(data2));
  if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,message2,"paddress",data2) == 0)
  {
    REMOTE_DATA_GET_INFORMATION_FROM_NAME_ERROR;
  }
  memcpy(message+strlen(message),data2,strlen(data2));
  memcpy(message+strlen(message),"|website: ",10);
  memset(data2,0,sizeof(data2));
  if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,message2,"website",data2) == 0)
  {
    REMOTE_DATA_GET_INFORMATION_FROM_NAME_ERROR;
  }
  memcpy(message+strlen(message),data2,strlen(data2));
  memcpy(message+strlen(message),"|timestamp: ",12);
  memset(data2,0,sizeof(data2));
  if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,message2,"timestamp",data2) == 0)
  {
    REMOTE_DATA_GET_INFORMATION_FROM_NAME_ERROR;
  }
  memcpy(message+strlen(message),data2,strlen(data2));
  memcpy(message+strlen(message),"|register delegate: ",20);
  memset(data2,0,sizeof(data2));
  if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,message2,"reserve_delegate_address",data2) == 0)
  {
    REMOTE_DATA_GET_INFORMATION_FROM_NAME_ERROR;
  }
  memcpy(message+strlen(message),data2,strlen(data2));
  memcpy(message+strlen(message),"|register amount: ",18);
  memset(data2,0,sizeof(data2));
  if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,message2,"reserve_delegate_amount",data2) == 0)
  {
    REMOTE_DATA_GET_INFORMATION_FROM_NAME_ERROR;
  }
  memcpy(message+strlen(message),data2,strlen(data2));
  memcpy(message+strlen(message),"|tx_hash: ",10);
  memset(data2,0,sizeof(data2));
  if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,message2,"tx_hash",data2) == 0)
  {
    REMOTE_DATA_GET_INFORMATION_FROM_NAME_ERROR;
  }
  memcpy(message+strlen(message),data2,strlen(data2));
  memcpy(message+strlen(message),"|}",2);

  send_data(CLIENT_SOCKET,(unsigned char*)message,0,0,"");
  return;

  #undef DATABASE_COLLECTION
  #undef REMOTE_DATA_GET_INFORMATION_FROM_NAME_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_remote_data_get_information_from_name
Description: Runs the code when the server receives /remotedatagetinformationfromname
Parameters:
  CLIENT_SOCKET - The socket to send data to
  DATA - The data
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_remote_data_get_information_from_name(const int CLIENT_SOCKET, const char* DATA)
{
  // Variables
  char name[BUFFER_SIZE];
  char message[BUFFER_SIZE];
  char message2[BUFFER_SIZE];
  char data2[BUFFER_SIZE];

  // define macros
  #define DATABASE_COLLECTION "remote_data"
  #define REMOTE_DATA_GET_INFORMATION_FROM_NAME_ERROR \
  if (debug_settings == 1) \
  { \
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_nodes_to_network_data_nodes_remote_data_get_information_from_name",92); \
    memcpy(error_message.data[error_message.total],"Error: Could not get the address settings",41); \
    error_message.total++; \
  } \
  memset(message,0,strlen(message)); \
  memcpy(message,"{\"Error\":\"Could not get the information\"}",41); \
  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),400,"application/json"); \
  return 0;
  
  memset(name,0,sizeof(name));
  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));
  memset(message2,0,sizeof(message2));

  // get the parameter1
  memcpy(name,&DATA[49],(strnlen(DATA,sizeof(name)) - strnlen(strstr(DATA," HTTP/"),sizeof(name)))-49);

  // error check
  if (strncmp(name,"",BUFFER_SIZE) == 0 || strlen(name) > REMOTE_DATA_NAME_MAXIMUM_LENGTH)
  {
    REMOTE_DATA_GET_INFORMATION_FROM_NAME_ERROR;
  } 

  // check if the tx_hash is empty or the name is expired
  if (validate_name_settings(name) == 0)
  {
    REMOTE_DATA_GET_INFORMATION_FROM_NAME_ERROR;
  }

  // create the message
  memset(message2,0,sizeof(message2));  
  memcpy(message2,"{\"name\":\"",9);
  memcpy(message2+strlen(message2),name,strlen(name));
  memcpy(message2+strlen(message2),"\"}",2);

  // get the remote data
  memset(message,0,sizeof(message));  
  memcpy(message,"{\"name\":\"",9);
  memcpy(message+strlen(message),name,strlen(name));
  memcpy(message+strlen(message),"\",\"address\":\"",13);
  memset(data2,0,sizeof(data2));
  if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,message2,"address",data2) == 0)
  {
    REMOTE_DATA_GET_INFORMATION_FROM_NAME_ERROR;
  }
  memcpy(message+strlen(message),data2,strlen(data2));
  memcpy(message+strlen(message),"\",\"saddress\":\"",14);
  memset(data2,0,sizeof(data2));
  if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,message2,"saddress",data2) == 0)
  {
    REMOTE_DATA_GET_INFORMATION_FROM_NAME_ERROR;
  }
  memcpy(message+strlen(message),data2,strlen(data2));
  memcpy(message+strlen(message),"\",\"paddress\":\"",14);
  memset(data2,0,sizeof(data2));
  if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,message2,"paddress",data2) == 0)
  {
    REMOTE_DATA_GET_INFORMATION_FROM_NAME_ERROR;
  }
  memcpy(message+strlen(message),data2,strlen(data2));
  memcpy(message+strlen(message),"\",\"website\":\"",13);
  memset(data2,0,sizeof(data2));
  if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,message2,"website",data2) == 0)
  {
    REMOTE_DATA_GET_INFORMATION_FROM_NAME_ERROR;
  }
  memcpy(message+strlen(message),data2,strlen(data2));
  memcpy(message+strlen(message),"\",\"timestamp\":\"",15);
  memset(data2,0,sizeof(data2));
  if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,message2,"timestamp",data2) == 0)
  {
    REMOTE_DATA_GET_INFORMATION_FROM_NAME_ERROR;
  }
  memcpy(message+strlen(message),data2,strlen(data2));
  memcpy(message+strlen(message),"\",\"reserve_delegate_address\":\"",30);
  memset(data2,0,sizeof(data2));
  if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,message2,"reserve_delegate_address",data2) == 0)
  {
    REMOTE_DATA_GET_INFORMATION_FROM_NAME_ERROR;
  }
  memcpy(message+strlen(message),data2,strlen(data2));
  memcpy(message+strlen(message),"\",\"reserve_delegate_amount\":\"",29);
  memset(data2,0,sizeof(data2));
  if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,message2,"reserve_delegate_amount",data2) == 0)
  {
    REMOTE_DATA_GET_INFORMATION_FROM_NAME_ERROR;
  }
  memcpy(message+strlen(message),data2,strlen(data2));
  memcpy(message+strlen(message),"\",\"tx_hash\":\"",13);
  memset(data2,0,sizeof(data2));
  if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,message2,"tx_hash",data2) == 0)
  {
    REMOTE_DATA_GET_INFORMATION_FROM_NAME_ERROR;
  }
  memcpy(message+strlen(message),data2,strlen(data2));
  memcpy(message+strlen(message),"\"}",2);

  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),200,"application/json");
  return 1;

  #undef DATABASE_COLLECTION
  #undef REMOTE_DATA_GET_INFORMATION_FROM_NAME_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_remote_data_nodes_to_block_verifiers_update_remote_data
Description: Runs the code when the server receives the NODES_TO_BLOCK_VERIFIERS_UPDATE_REMOTE_DATA message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  MESSAGE - The message
-----------------------------------------------------------------------------------------------------------
*/

void server_receive_data_socket_remote_data_nodes_to_block_verifiers_update_remote_data(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char data[4096];
  char data2[4096];
  char delegate_public_address[XCASH_WALLET_LENGTH+1];
  char item[100];
  char value[100];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;
  int count2;
  size_t data_size;

  // define macros
  #define DATABASE_COLLECTION "remote_data"
  #define REMOTE_DATA_UPDATE_DATA_ERROR \
  if (debug_settings == 1) \
  { \
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_remote_data_nodes_to_block_verifiers_update_remote_data",82); \
    memcpy(error_message.data[error_message.total],"Could not update the remote data",32); \
    error_message.total++; \
  } \
  send_data(CLIENT_SOCKET,(unsigned char*)"Could not update the remote data}",0,0,""); \
  return;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(delegate_public_address,0,sizeof(delegate_public_address));
  memset(item,0,sizeof(item));
  memset(value,0,sizeof(value));

  // get the current time
  get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);

  // check if it is valid data interval
  if (test_settings == 0 && current_UTC_date_and_time.tm_min % 60 >= 0 && current_UTC_date_and_time.tm_min % 60 < 10)
  {
    REMOTE_DATA_UPDATE_DATA_ERROR;
  }

  // verify the message
  if (verify_data(MESSAGE,0) == 0 || string_count(MESSAGE,"|") != REMOTE_DATA_UPDATE_DATA_PARAMETER_AMOUNT || check_for_invalid_strings(MESSAGE) == 0)
  {  
    REMOTE_DATA_UPDATE_DATA_ERROR;
  }

  // parse the message
  for (count = 0, count2 = 0; count < REMOTE_DATA_UPDATE_DATA_PARAMETER_AMOUNT; count++)
  {
    if (count == 1)
    {
      if ((data_size = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2) >= sizeof(item))
      {
        REMOTE_DATA_UPDATE_DATA_ERROR;
      }
      memcpy(item,&MESSAGE[count2],data_size);
    }
    if (count == 2)
    {
      if ((data_size = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2) >= sizeof(value))
      {
        REMOTE_DATA_UPDATE_DATA_ERROR;
      }
      memcpy(value,&MESSAGE[count2],data_size);
    }
    if (count == 3)
    {
      if ((data_size = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2) != XCASH_WALLET_LENGTH)
      {
        REMOTE_DATA_UPDATE_DATA_ERROR;
      }
      memcpy(delegate_public_address,&MESSAGE[count2],data_size);
    }
    count2 = (int)(strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) + 1);
  }

  // check if the data is valid
  if (strlen(delegate_public_address) != XCASH_WALLET_LENGTH || strncmp(delegate_public_address,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0)
  {color_print("2","yellow");
    REMOTE_DATA_UPDATE_DATA_ERROR;
  }

  // check if the item is valid
  if (strncmp(item,"website",BUFFER_SIZE) != 0 && strncmp(item,"smart_contract_hash",BUFFER_SIZE) != 0)
  {    
    REMOTE_DATA_UPDATE_DATA_ERROR;
  }

  // create the message
  memcpy(data,"{\"address\":\"",12);
  memcpy(data+strlen(data),delegate_public_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\"}",2);

  if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,data,"name",data2) == 0 || validate_name_settings(data2) == 0)
  {
    REMOTE_DATA_UPDATE_DATA_ERROR;
  }

  memset(data2,0,sizeof(data2));
  memcpy(data2,"{\"",2);
  memcpy(data2+2,item,strnlen(item,sizeof(item)));
  memcpy(data2+strlen(data2),"\":\"",3);
  memcpy(data2+strlen(data2),value,strnlen(value,sizeof(value)));
  memcpy(data2+strlen(data2),"\"}",2);

  // update the delegate in the database
  if (update_document_from_collection(remote_data_database_name,DATABASE_COLLECTION,data,data2) == 0)
  {
    REMOTE_DATA_UPDATE_DATA_ERROR;
  }

  send_data(CLIENT_SOCKET,(unsigned char*)"Updated the remote data}",0,0,"");
  return;

  #undef DATABASE_COLLECTION
  #undef REMOTE_DATA_UPDATE_DATA_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_remote_data_nodes_to_block_verifiers_save_name
Description: Runs the code when the server receives the NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_SAVE_NAME message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  MESSAGE - The message
-----------------------------------------------------------------------------------------------------------
*/

void server_receive_data_socket_remote_data_nodes_to_block_verifiers_save_name(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char data[BUFFER_SIZE];
  char public_address[XCASH_WALLET_LENGTH+1];
  char block_producer_delegate_address[XCASH_WALLET_LENGTH+1];
  char block_producer_delegate_amount[MAXIMUM_NUMBER_SIZE+1];
  char name[100];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;
  int count2;
  size_t data_size;

  // define macros
  #define DATABASE_COLLECTION "remote_data"
  #define REMOTE_DATA_SAVE_NAME_ERROR \
  if (debug_settings == 1) \
  { \
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_remote_data_nodes_to_block_verifiers_save_name",73); \
    memcpy(error_message.data[error_message.total],"Could not save the name",23); \
    error_message.total++; \
  } \
  send_data(CLIENT_SOCKET,(unsigned char*)"Could not save the name}",0,0,""); \
  return;

  memset(data,0,sizeof(data));
  memset(public_address,0,sizeof(public_address));
  memset(block_producer_delegate_address,0,sizeof(block_producer_delegate_address));
  memset(block_producer_delegate_amount,0,sizeof(block_producer_delegate_amount));
  memset(name,0,sizeof(name));

  // get the current time
  get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);

  // check if it is valid data interval
  if (test_settings == 0 && current_UTC_date_and_time.tm_min % BLOCK_TIME != 1 && current_UTC_date_and_time.tm_min % BLOCK_TIME != 2 && current_UTC_date_and_time.tm_min % BLOCK_TIME != 3)
  {
    REMOTE_DATA_SAVE_NAME_ERROR;
  }

  // verify the message
  if (verify_data(MESSAGE,0) == 0 || string_count(MESSAGE,"|") != REMOTE_DATA_SAVE_NAME_PARAMETER_AMOUNT || check_for_invalid_strings(MESSAGE) == 0)
  {
    REMOTE_DATA_SAVE_NAME_ERROR;
  }

  // parse the message
  for (count = 0, count2 = 0; count < REMOTE_DATA_SAVE_NAME_PARAMETER_AMOUNT; count++)
  {
    if (count == 1)
    {
      if ((data_size = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2) >= REMOTE_DATA_NAME_MAXIMUM_LENGTH)
      {
        REMOTE_DATA_SAVE_NAME_ERROR;
      }
      memcpy(name,&MESSAGE[count2],data_size);
    }
    if (count == 2)
    {
      if ((data_size = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2) != XCASH_WALLET_LENGTH)
      {
        REMOTE_DATA_SAVE_NAME_ERROR;
      }
      memcpy(public_address,&MESSAGE[count2],data_size);
    }
    count2 = (int)(strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) + 1);
  }

  // check if the data is valid
  if (strlen(public_address) != XCASH_WALLET_LENGTH || strncmp(public_address,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || validate_name(name) == 0)
  {
    REMOTE_DATA_SAVE_NAME_ERROR;
  }

  // check if the name is already in the database
  memcpy(data,"{\"name\":\"",9);
  memcpy(data+strlen(data),name,strlen(name));
  memcpy(data+strlen(data),"\"}",2);

  if (count_documents_in_collection(remote_data_database_name,DATABASE_COLLECTION,data) > 0)
  {
    REMOTE_DATA_SAVE_NAME_ERROR;
  }

  // check if the address is already in the database for another name on the address, saddress or paddress
  memset(data,0,sizeof(data));
  memcpy(data,"{\"address\":\"",12);
  memcpy(data+strlen(data),public_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\"}",2);

  if (count_documents_in_collection(remote_data_database_name,DATABASE_COLLECTION,data) > 0)
  {
    REMOTE_DATA_SAVE_NAME_ERROR;
  }

  memset(data,0,sizeof(data));
  memcpy(data,"{\"saddress\":\"",13);
  memcpy(data+strlen(data),public_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\"}",2);

  if (count_documents_in_collection(remote_data_database_name,DATABASE_COLLECTION,data) > 0)
  {
    REMOTE_DATA_SAVE_NAME_ERROR;
  }

  memset(data,0,sizeof(data));
  memcpy(data,"{\"paddress\":\"",13);
  memcpy(data+strlen(data),public_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\"}",2);

  if (count_documents_in_collection(remote_data_database_name,DATABASE_COLLECTION,data) > 0)
  {
    REMOTE_DATA_SAVE_NAME_ERROR;
  }

  // get the current block producer and the amount
  if (get_previous_block_producer(block_producer_delegate_address) == 0)
  {
    REMOTE_DATA_SAVE_NAME_ERROR;
  }

  memset(data,0,sizeof(data));
  memcpy(data,"{\"public_address\":\"",19);
  memcpy(data+strlen(data),block_producer_delegate_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\"}",2);
  if (read_document_field_from_collection(remote_data_database_name,"remote_data_delegates",data,"amount",block_producer_delegate_amount) == 0)
  {
    REMOTE_DATA_SAVE_NAME_ERROR;
  }

  // save the name
  memset(data,0,sizeof(data));
  memcpy(data,"{\"name\":\"",9);
  memcpy(data+strlen(data),name,strlen(name));
  memcpy(data+strlen(data),"\",\"address\":\"",13);
  memcpy(data+strlen(data),public_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\",\"saddress\":\"\",\"paddress\":\"\",\"saddress_list\":\"\",\"paddress_list\":\"\",\"website\":\"\",\"smart_contract_hash\":\"\",\"timestamp\":\"",119);
  snprintf(data+strlen(data),MAXIMUM_NUMBER_SIZE,"%ld",time(NULL));
  memcpy(data+strlen(data),"\",\"reserve_delegate_address\":\"",30);
  memcpy(data+strlen(data),block_producer_delegate_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\",\"reserve_delegate_amount\":\"",29);
  memcpy(data+strlen(data),block_producer_delegate_amount,strlen(block_producer_delegate_amount));
  memcpy(data+strlen(data),"\",\"tx_hash\":\"\"}",15);
  
  if (insert_document_into_collection_json(remote_data_database_name,DATABASE_COLLECTION,data) == 0)
  {
    REMOTE_DATA_SAVE_NAME_ERROR;
  }

  send_data(CLIENT_SOCKET,(unsigned char*)"Saved the name}",0,0,"");
  return;

  #undef DATABASE_COLLECTION
  #undef REMOTE_DATA_SAVE_NAME_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_remote_data_nodes_to_block_verifiers_purchase_name
Description: Runs the code when the server receives the NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_PURCHASE_NAME message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  MESSAGE - The message
-----------------------------------------------------------------------------------------------------------
*/

void server_receive_data_socket_remote_data_nodes_to_block_verifiers_purchase_name(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char data[SMALL_BUFFER_SIZE];
  char data2[SMALL_BUFFER_SIZE];
  char public_address[SMALL_BUFFER_SIZE];
  char saddress[SMALL_BUFFER_SIZE];
  char paddress[SMALL_BUFFER_SIZE];
  char tx_hash[SMALL_BUFFER_SIZE];
  char name[SMALL_BUFFER_SIZE];
  char delegates_public_address[SMALL_BUFFER_SIZE];
  char delegates_amount[MAXIMUM_NUMBER_SIZE+1]; 
  char delegate_name[XCASH_WALLET_LENGTH+1]; 
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;
  int count2;
  long int registration_length;
  size_t data_size;

  // define macros
  #define DATABASE_COLLECTION "remote_data"
  #define REMOTE_DATA_PURCHASE_NAME_ERROR \
  if (debug_settings == 1) \
  { \
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_remote_data_nodes_to_block_verifiers_purchase_name",77); \
    memcpy(error_message.data[error_message.total],"Could not purchase the name",27); \
    error_message.total++; \
  } \
  send_data(CLIENT_SOCKET,(unsigned char*)"Could not purchase the name}",0,0,""); \
  return;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(public_address,0,sizeof(public_address));
  memset(saddress,0,sizeof(saddress));
  memset(paddress,0,sizeof(paddress));
  memset(tx_hash,0,sizeof(tx_hash));
  memset(name,0,sizeof(name));
  memset(delegates_public_address,0,sizeof(delegates_public_address));
  memset(delegates_amount,0,sizeof(delegates_amount));
  memset(delegate_name,0,sizeof(delegate_name));

  // get the current time
  get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);

  // check if it is valid data interval
  if (test_settings == 0 && current_UTC_date_and_time.tm_min % 60 >= 0 && current_UTC_date_and_time.tm_min % 60 < 10)
  {
    REMOTE_DATA_PURCHASE_NAME_ERROR;
  }

  // verify the message
  if (verify_data(MESSAGE,0) == 0 || string_count(MESSAGE,"|") != REMOTE_DATA_PURCHASE_NAME_PARAMETER_AMOUNT || check_for_invalid_strings(MESSAGE) == 0)
  {
    REMOTE_DATA_PURCHASE_NAME_ERROR;
  }

  // parse the message
  for (count = 0, count2 = 0; count < REMOTE_DATA_PURCHASE_NAME_PARAMETER_AMOUNT; count++)
  {
    if (count == 1)
    {
      if ((data_size = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2) != XCASH_WALLET_LENGTH)
      {
        REMOTE_DATA_PURCHASE_NAME_ERROR;
      }
      memcpy(saddress,&MESSAGE[count2],data_size);
    }
    if (count == 2)
    {
      if ((data_size = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2) != XCASH_WALLET_LENGTH)
      {
        REMOTE_DATA_PURCHASE_NAME_ERROR;
      }
      memcpy(paddress,&MESSAGE[count2],data_size);
    }
    if (count == 3)
    {
      if ((data_size = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2) != TRANSACTION_HASH_LENGTH)
      {
        REMOTE_DATA_PURCHASE_NAME_ERROR;
      }
      memcpy(tx_hash,&MESSAGE[count2],data_size);
    }
    if (count == 4)
    {
      if ((data_size = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2) != XCASH_WALLET_LENGTH)
      {
        REMOTE_DATA_PURCHASE_NAME_ERROR;
      }
      memcpy(public_address,&MESSAGE[count2],data_size);
    }
    count2 = (int)(strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) + 1);
  }

  // check if the data is valid
  if (strlen(public_address) != XCASH_WALLET_LENGTH || strncmp(public_address,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || strlen(saddress) != XCASH_WALLET_LENGTH || strncmp(saddress,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || strlen(paddress) != XCASH_WALLET_LENGTH || strncmp(paddress,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || strlen(tx_hash) != TRANSACTION_HASH_LENGTH)
  {
    REMOTE_DATA_PURCHASE_NAME_ERROR;
  }

  // check that the address,saddress and paddress are different
  if (strncmp(public_address,saddress,BUFFER_SIZE) == 0 || strncmp(public_address,paddress,BUFFER_SIZE) == 0 || strncmp(saddress,paddress,BUFFER_SIZE) == 0)
  {
    REMOTE_DATA_PURCHASE_NAME_ERROR;
  } 

  // check if the saddress is already in the database for another name on the address, saddress or paddress
  memset(data,0,sizeof(data));
  memcpy(data,"{\"address\":\"",12);
  memcpy(data+strlen(data),saddress,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\"}",2);

  if (count_documents_in_collection(remote_data_database_name,DATABASE_COLLECTION,data) > 0)
  {
    REMOTE_DATA_PURCHASE_NAME_ERROR;
  }

  memset(data,0,sizeof(data));
  memcpy(data,"{\"saddress\":\"",13);
  memcpy(data+strlen(data),saddress,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\"}",2);

  if (count_documents_in_collection(remote_data_database_name,DATABASE_COLLECTION,data) > 0)
  {
    REMOTE_DATA_PURCHASE_NAME_ERROR;
  }

  memset(data,0,sizeof(data));
  memcpy(data,"{\"paddress\":\"",13);
  memcpy(data+strlen(data),saddress,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\"}",2);

  if (count_documents_in_collection(remote_data_database_name,DATABASE_COLLECTION,data) > 0)
  {
    REMOTE_DATA_PURCHASE_NAME_ERROR;
  }

  // check if the paddress is already in the database for another name on the address, saddress or paddress
  memset(data,0,sizeof(data));
  memcpy(data,"{\"address\":\"",12);
  memcpy(data+strlen(data),paddress,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\"}",2);

  if (count_documents_in_collection(remote_data_database_name,DATABASE_COLLECTION,data) > 0)
  {
    REMOTE_DATA_PURCHASE_NAME_ERROR;
  }

  memset(data,0,sizeof(data));
  memcpy(data,"{\"saddress\":\"",13);
  memcpy(data+strlen(data),paddress,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\"}",2);

  if (count_documents_in_collection(remote_data_database_name,DATABASE_COLLECTION,data) > 0)
  {
    REMOTE_DATA_PURCHASE_NAME_ERROR;
  }

  memset(data,0,sizeof(data));
  memcpy(data,"{\"paddress\":\"",13);
  memcpy(data+strlen(data),paddress,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\"}",2);

  if (count_documents_in_collection(remote_data_database_name,DATABASE_COLLECTION,data) > 0)
  {
    REMOTE_DATA_PURCHASE_NAME_ERROR;
  }

  // check if the timestamp is valid
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memcpy(data,"{\"address\":\"",12);
  memcpy(data+strlen(data),public_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\"}",2);
  if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,data,"timestamp",data2) == 0)
  {
    REMOTE_DATA_PURCHASE_NAME_ERROR;
  }

  sscanf(data2,"%ld", &registration_length);
  if (test_settings == 0 && (registration_length + REMOTE_DATA_SAVE_NAME_LENGTH) < time(NULL))
  {
    REMOTE_DATA_PURCHASE_NAME_ERROR;
  }

  // get the delegates data
  memset(data,0,sizeof(data));
  memcpy(data,"{\"address\":\"",12);
  memcpy(data+strlen(data),public_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\"}",2);
  if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,data,"reserve_delegate_address",delegates_public_address) == 0 || read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,data,"reserve_delegate_amount",delegates_amount) == 0 || read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,data,"name",name) == 0)
  {
    REMOTE_DATA_PURCHASE_NAME_ERROR;
  }

  // check if the tx_hash is valid
  if (test_settings == 0 && remote_data_validate_tx_hash(tx_hash,public_address,delegates_public_address,delegates_amount) == 0)
  {
    REMOTE_DATA_PURCHASE_NAME_ERROR;
  }

  // purchase the name (add the saddress,paddress and tx_hash and update the timestamp)

  // get the delegate name for the logs
  memset(data2,0,sizeof(data2));
  memcpy(data2,"{\"public_address\":\"",19);
  memcpy(data2+strlen(data2),delegates_public_address,XCASH_WALLET_LENGTH);
  memcpy(data2+strlen(data2),"\"}",2);

  if (read_document_field_from_collection(database_name,"delegates",data2,"delegate_name",delegate_name) == 0)
  {
    REMOTE_DATA_PURCHASE_NAME_ERROR;
  }

  memset(data2,0,sizeof(data2));
  memcpy(data2,"{\"saddress\":\"",13);
  memcpy(data2+strlen(data2),saddress,XCASH_WALLET_LENGTH);
  memcpy(data2+strlen(data2),"\"}",2);

  // update the delegate in the database
  if (update_document_from_collection(remote_data_database_name,DATABASE_COLLECTION,data,data2) == 0)
  {
    REMOTE_DATA_PURCHASE_NAME_ERROR;
  }

  memset(data2,0,sizeof(data2));
  memcpy(data2,"{\"paddress\":\"",13);
  memcpy(data2+strlen(data2),paddress,XCASH_WALLET_LENGTH);
  memcpy(data2+strlen(data2),"\"}",2);

  // update the delegate in the database
  if (update_document_from_collection(remote_data_database_name,DATABASE_COLLECTION,data,data2) == 0)
  {
    REMOTE_DATA_PURCHASE_NAME_ERROR;
  }

  memset(data2,0,sizeof(data2));
  memcpy(data2,"{\"tx_hash\":\"",12);
  memcpy(data2+strlen(data2),tx_hash,TRANSACTION_HASH_LENGTH);
  memcpy(data2+strlen(data2),"\"}",2);

  // update the delegate in the database
  if (update_document_from_collection(remote_data_database_name,DATABASE_COLLECTION,data,data2) == 0)
  {
    REMOTE_DATA_PURCHASE_NAME_ERROR;
  }
  
  memset(data2,0,sizeof(data2));
  memcpy(data2,"{\"timestamp\":\"",14);
  snprintf(data2+strlen(data2),MAXIMUM_NUMBER_SIZE,"%ld",time(NULL) + REMOTE_DATA_REGISTRATION_LENGTH);
  memcpy(data2+strlen(data2),"\"}",2);

  // update the delegate in the database
  if (update_document_from_collection(remote_data_database_name,DATABASE_COLLECTION,data,data2) == 0)
  {
    REMOTE_DATA_PURCHASE_NAME_ERROR;
  }

  // update the delegates statistics in the database
  if (remote_data_update_delegates_statistics(delegate_name,delegates_amount) == 0)
  {
    REMOTE_DATA_PURCHASE_NAME_ERROR;
  }

  // log the data that the purchase was made to the network
  memset(data2,0,sizeof(data2));
  memcpy(data2,"Remote Data Protocol: Added ",28);
  memcpy(data2+strlen(data2),name,strlen(name));
  memcpy(data2+strlen(data2)," using delegate ",16);
  memcpy(data2+strlen(data2),delegate_name,strlen(delegate_name));
  memcpy(data2+strlen(data2)," for amount ",12);
  memcpy(data2+strlen(data2),delegates_amount,strlen(delegates_amount));
  color_print(data2,"yellow");
  send_data(CLIENT_SOCKET,(unsigned char*)"Purchased the name}",0,0,"");
  return;

  #undef DATABASE_COLLECTION
  #undef REMOTE_DATA_PURCHASE_NAME_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_remote_data_get_delegates_information
Description: Runs the code when the server receives /remotedatagetdelegatesinformation
Parameters:
  CLIENT_SOCKET - The socket to send data to
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_remote_data_get_delegates_information(const int CLIENT_SOCKET)
{
  // Variables
  char buffer[1024];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count = 0;
  size_t counter = 0;
  struct database_multiple_documents_fields database_multiple_documents_fields;
  int document_count = 0;

  // define macros
  #define DATABASE_COLLECTION "remote_data_delegates"

  #define REMOTE_DATA_GET_DELEGATES_INFORMATION_ERROR(settings,MESSAGE) \
  if (debug_settings == 1) \
  { \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_remote_data_get_delegates_information",64); \
  memcpy(error_message.data[error_message.total],MESSAGE,sizeof(MESSAGE)-1); \
  error_message.total++; \
  } \
  memset(buffer,0,strlen(buffer)); \
  memcpy(buffer,"{\"Error\":\"Could not get the remote data delegates\"}",51); \
  send_data(CLIENT_SOCKET,(unsigned char*)buffer,strlen(buffer),400,"application/json"); \
  if ((settings) == 0) \
  { \
    POINTER_RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,TOTAL_REMOTE_DATA_DELEGATES_DATABASE_FIELDS); \
  } \
  return 0;

  // get the total delegates
  if ((document_count = count_all_documents_in_collection(remote_data_database_name,DATABASE_COLLECTION)) <= 0)
  {
    REMOTE_DATA_GET_DELEGATES_INFORMATION_ERROR(1,"Could not get the remote data delegates");
  }

  memset(buffer,0,sizeof(buffer));

  // initialize the database_multiple_documents_fields struct
  INITIALIZE_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,document_count,TOTAL_REMOTE_DATA_DELEGATES_DATABASE_FIELDS,"server_receive_data_socket_remote_data_get_delegates_information",buffer,current_date_and_time,current_UTC_date_and_time);

  if (read_multiple_documents_all_fields_from_collection(remote_data_database_name,DATABASE_COLLECTION,"",&database_multiple_documents_fields,0,document_count,0,"") == 0)
  {
    REMOTE_DATA_GET_DELEGATES_INFORMATION_ERROR(0,"Could not get the remote data delegates");
  }

  // count how many bytes to allocate for the json data
  for (count = 0, counter = BUFFER_SIZE; count < document_count; count++)
  {
    counter += 16; // 16 is for quotes for the items and values
    counter += strlen(database_multiple_documents_fields.item[count][0]);
    counter += strlen(database_multiple_documents_fields.item[count][1]);
    counter += strlen(database_multiple_documents_fields.item[count][2]);
    counter += strlen(database_multiple_documents_fields.item[count][3]);
    counter += strlen(database_multiple_documents_fields.item[count][4]);
    counter += strlen(database_multiple_documents_fields.value[count][0]);
    counter += strlen(database_multiple_documents_fields.value[count][1]);
    counter += strlen(database_multiple_documents_fields.value[count][2]);
    counter += strlen(database_multiple_documents_fields.value[count][3]);
    counter += strlen(database_multiple_documents_fields.value[count][4]);
  }

  char* message = (char*)calloc(counter,sizeof(char)); 
  
  if (create_json_data_from_database_multiple_documents_array(&database_multiple_documents_fields,message,"") == 0)
  {
    pointer_reset(message);
    REMOTE_DATA_GET_DELEGATES_INFORMATION_ERROR(0,"Could not create the json data");
  }  
  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),200,"application/json");
  
  pointer_reset(message);
  POINTER_RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,TOTAL_REMOTE_DATA_DELEGATES_DATABASE_FIELDS);
  return 1;

  #undef DATABASE_COLLECTION
  #undef REMOTE_DATA_GET_DELEGATES_INFORMATION_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_remote_data_get_block_producer_information
Description: Runs the code when the server receives /remotedatagetblockproducerinformation
Parameters:
  CLIENT_SOCKET - The socket to send data to
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_remote_data_get_block_producer_information(const int CLIENT_SOCKET)
{
  // Variables
  char public_address[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char message[BUFFER_SIZE];

  // define macros
  #define DATABASE_COLLECTION "remote_data_delegates"
  #define REMOTE_DATA_GET_BLOCK_PRODUCER_INFORMATION_ERROR \
  if (debug_settings == 1) \
  { \
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_remote_data_get_block_producer_information",69); \
    memcpy(error_message.data[error_message.total],"Invalid parameters",18); \
    error_message.total++; \
  } \
  memset(message,0,strlen(message)); \
  memcpy(message,"{\"Error\":\"Could not get the block producer\"}",44); \
  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),400,"application/json"); \
  return 0;

  memset(public_address,0,sizeof(public_address));
  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));

  // get the current block producer
  if (get_previous_block_producer(public_address) == 0)
  {
    REMOTE_DATA_GET_BLOCK_PRODUCER_INFORMATION_ERROR;
  }

  // create the message
  memcpy(message,"{\"public_address\":\"",19);
  memcpy(message+strlen(message),public_address,XCASH_WALLET_LENGTH);
  memcpy(message+strlen(message),"\"}",2);

  // get the amount
  if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,message,"amount",data2) == 0)
  {
    REMOTE_DATA_GET_BLOCK_PRODUCER_INFORMATION_ERROR;
  }

  // create the message
  memset(message,0,sizeof(message));
  memcpy(message,"{\"public_address\":\"",19);
  memcpy(message+strlen(message),public_address,XCASH_WALLET_LENGTH);
  memcpy(message+strlen(message),"\",\"amount\":\"",12);
  memcpy(message+strlen(message),data2,strlen(data2));
  memcpy(message+strlen(message),"\"}",2);

  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),200,"application/json");
  
  REMOTE_DATA_GET_BLOCK_PRODUCER_INFORMATION_ERROR;

  #undef DATABASE_COLLECTION
  #undef REMOTE_DATA_GET_BLOCK_PRODUCER_INFORMATION_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_remote_data_nodes_to_block_verifiers_delegates_set_amount
Description: Runs the code when the server receives the NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_DELEGATES_SET_AMOUNT message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  MESSAGE - The message
-----------------------------------------------------------------------------------------------------------
*/

void server_receive_data_socket_remote_data_nodes_to_block_verifiers_delegates_set_amount(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char data[4096];
  char data2[4096];
  char delegate_public_address[XCASH_WALLET_LENGTH+1];
  char amount[MAXIMUM_NUMBER_SIZE+1];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;
  int count2;
  size_t data_size;

  // define macros
  #define DATABASE_COLLECTION "remote_data_delegates"
  #define REMOTE_DATA_SET_AMOUNT_ERROR \
  if (debug_settings == 1) \
  { \
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_remote_data_nodes_to_block_verifiers_delegates_set_amount",84); \
    memcpy(error_message.data[error_message.total],"Could not set the amount",24); \
    error_message.total++; \
  } \
  send_data(CLIENT_SOCKET,(unsigned char*)"Could not set the amount}",0,0,""); \
  return;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(delegate_public_address,0,sizeof(delegate_public_address));
  memset(amount,0,sizeof(amount));

  // get the current time
  get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);

  // check if it is valid data interval
  if (test_settings == 0 && current_UTC_date_and_time.tm_min % 60 >= 0 && current_UTC_date_and_time.tm_min % 60 < 10)
  {
    REMOTE_DATA_SET_AMOUNT_ERROR;
  }

  // verify the message
  if (verify_data(MESSAGE,0) == 0 || string_count(MESSAGE,"|") != REMOTE_DATA_SET_AMOUNT_PARAMETER_AMOUNT || check_for_invalid_strings(MESSAGE) == 0)
  {   
    REMOTE_DATA_SET_AMOUNT_ERROR;
  }

  // parse the message
  for (count = 0, count2 = 0; count < REMOTE_DATA_SET_AMOUNT_PARAMETER_AMOUNT; count++)
  {
    if (count == 1)
    {
      if ((data_size = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2) >= sizeof(amount))
      {
        REMOTE_DATA_SET_AMOUNT_ERROR;
      }
      memcpy(amount,&MESSAGE[count2],data_size);
    }
    if (count == 2)
    {
      if ((data_size = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2) != XCASH_WALLET_LENGTH)
      {
        REMOTE_DATA_SET_AMOUNT_ERROR;
      }
      memcpy(delegate_public_address,&MESSAGE[count2],data_size);
    }
    count2 = (int)(strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) + 1);
  }

  // check if the amount is valid
  if (validate_amount(amount) == 0)
  {
    REMOTE_DATA_SET_AMOUNT_ERROR;
  }

  // create the message
  memcpy(data,"{\"public_address\":\"",19);
  memcpy(data+strlen(data),delegate_public_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\"}",2);

  memset(data2,0,sizeof(data2));
  memcpy(data2,"{\"amount\":\"",11);
  memcpy(data2+strlen(data2),amount,strlen(amount));
  memcpy(data2+strlen(data2),"\"}",2);

  // update the delegate in the database
  if (update_document_from_collection(remote_data_database_name,DATABASE_COLLECTION,data,data2) == 0)
  {
    REMOTE_DATA_SET_AMOUNT_ERROR;
  }

  send_data(CLIENT_SOCKET,(unsigned char*)"Set the amount}",0,0,"");
  return;

  #undef DATABASE_COLLECTION
  #undef REMOTE_DATA_SET_AMOUNT_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_remote_data_nodes_to_block_verifiers_renewal_start
Description: Runs the code when the server receives the NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_RENEWAL_START message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  MESSAGE - The message
-----------------------------------------------------------------------------------------------------------
*/

void server_receive_data_socket_remote_data_nodes_to_block_verifiers_renewal_start(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char data[SMALL_BUFFER_SIZE];
  char data2[SMALL_BUFFER_SIZE];
  char public_address[XCASH_WALLET_LENGTH+1];
  char block_producer_delegate_address[XCASH_WALLET_LENGTH+1];
  char block_producer_delegate_amount[MAXIMUM_NUMBER_SIZE+1];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;
  int count2;
  size_t data_size;

  // define macros
  #define DATABASE_COLLECTION "remote_data"
  #define REMOTE_DATA_RENEWAL_START_ERROR \
  if (debug_settings == 1) \
  { \
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_remote_data_nodes_to_block_verifiers_renewal_start",77); \
    memcpy(error_message.data[error_message.total],"Could not start to renew the name",23); \
    error_message.total++; \
  } \
  send_data(CLIENT_SOCKET,(unsigned char*)"Could not start to renew the name}",0,0,""); \
  return;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(public_address,0,sizeof(public_address));
  memset(block_producer_delegate_address,0,sizeof(block_producer_delegate_address));
  memset(block_producer_delegate_amount,0,sizeof(block_producer_delegate_amount));

  // get the current time
  get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);

  // check if it is valid data interval
  if (test_settings == 0 && current_UTC_date_and_time.tm_min % BLOCK_TIME != 1 && current_UTC_date_and_time.tm_min % BLOCK_TIME != 2 && current_UTC_date_and_time.tm_min % BLOCK_TIME != 3)
  {
    REMOTE_DATA_RENEWAL_START_ERROR;
  }

  // verify the message
  if (verify_data(MESSAGE,0) == 0 || string_count(MESSAGE,"|") != REMOTE_DATA_RENEWAL_START_PARAMETER_AMOUNT || check_for_invalid_strings(MESSAGE) == 0)
  {   
    REMOTE_DATA_RENEWAL_START_ERROR;
  }

  // parse the message
  for (count = 0, count2 = 0; count < REMOTE_DATA_RENEWAL_START_PARAMETER_AMOUNT; count++)
  {
    if (count == 1)
    {
      if ((data_size = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2) != XCASH_WALLET_LENGTH)
      {
        REMOTE_DATA_RENEWAL_START_ERROR;
      }
      memcpy(public_address,&MESSAGE[count2],data_size);
    }
    count2 = (int)(strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) + 1);
  }

  // check if the data is valid
  if (strlen(public_address) != XCASH_WALLET_LENGTH || strncmp(public_address,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0)
  {
    REMOTE_DATA_RENEWAL_START_ERROR;
  }

  // get the current block producer and the amount
  if (get_previous_block_producer(block_producer_delegate_address) == 0)
  {
    REMOTE_DATA_RENEWAL_START_ERROR;
  }

  memset(data,0,sizeof(data));
  memcpy(data,"{\"public_address\":\"",19);
  memcpy(data+strlen(data),block_producer_delegate_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\"}",2);
  if (read_document_field_from_collection(remote_data_database_name,"remote_data_delegates",data,"amount",block_producer_delegate_amount) == 0)
  {
    REMOTE_DATA_RENEWAL_START_ERROR;
  }

  // start the renewal process
  memcpy(data2,"{\"address\":\"",12);
  memcpy(data2+strlen(data2),public_address,XCASH_WALLET_LENGTH);
  memcpy(data2+strlen(data2),"\"}",2);

  // update the timestamp
  memset(data,0,sizeof(data));
  memcpy(data,"{\"timestamp\":\"",14);
  snprintf(data+strlen(data),MAXIMUM_NUMBER_SIZE,"%ld",time(NULL) + REMOTE_DATA_REGISTRATION_LENGTH);
  memcpy(data+strlen(data),"\"}",2);

  if (update_document_from_collection(remote_data_database_name,DATABASE_COLLECTION,data2,data) == 0)
  {
    REMOTE_DATA_RENEWAL_START_ERROR;
  }

  // update the reserve_delegate_address
  memset(data,0,sizeof(data));
  memcpy(data,"{\"reserve_delegate_address\":\"",29);
  memcpy(data+strlen(data),block_producer_delegate_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\"}",2);

  if (update_document_from_collection(remote_data_database_name,DATABASE_COLLECTION,data2,data) == 0)
  {
    REMOTE_DATA_RENEWAL_START_ERROR;
  }

  // update the reserve_delegate_amount
  memset(data,0,sizeof(data));
  memcpy(data,"{\"reserve_delegate_amount\":\"",28);
  memcpy(data+strlen(data),block_producer_delegate_amount,strlen(block_producer_delegate_amount));
  memcpy(data+strlen(data),"\"}",2);

  if (update_document_from_collection(remote_data_database_name,DATABASE_COLLECTION,data2,data) == 0)
  {
    REMOTE_DATA_RENEWAL_START_ERROR;
  }

  // update the tx_hash
  memset(data,0,sizeof(data));
  memcpy(data,"{\"tx_hash\":\"\"}",14);

  if (update_document_from_collection(remote_data_database_name,DATABASE_COLLECTION,data2,data) == 0)
  {
    REMOTE_DATA_RENEWAL_START_ERROR;
  }

  send_data(CLIENT_SOCKET,(unsigned char*)"Started to renew the name}",0,0,"");
  return;

  #undef DATABASE_COLLECTION
  #undef REMOTE_DATA_RENEWAL_START_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_remote_data_nodes_to_block_verifiers_renewal_end
Description: Runs the code when the server receives the NODES_TO_BLOCK_VERIFIERS_REMOTE_DATA_RENEWAL_END message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  MESSAGE - The message
-----------------------------------------------------------------------------------------------------------
*/

void server_receive_data_socket_remote_data_nodes_to_block_verifiers_renewal_end(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char data[SMALL_BUFFER_SIZE];
  char data2[SMALL_BUFFER_SIZE];
  char public_address[XCASH_WALLET_LENGTH+1];
  char tx_hash[TRANSACTION_HASH_LENGTH+1];
  char delegates_public_address[XCASH_WALLET_LENGTH+1];
  char delegates_amount[MAXIMUM_NUMBER_SIZE+1]; 
  char name[SMALL_BUFFER_SIZE];
  char delegate_name[XCASH_WALLET_LENGTH+1]; 
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;
  int count2;
  long int registration_length;
  size_t data_size;

  // define macros
  #define DATABASE_COLLECTION "remote_data"
  #define REMOTE_DATA_RENEWAL_END_ERROR \
  if (debug_settings == 1) \
  { \
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_remote_data_nodes_to_block_verifiers_renewal_end",75); \
    memcpy(error_message.data[error_message.total],"Could not renew the name",24); \
    error_message.total++; \
  } \
  send_data(CLIENT_SOCKET,(unsigned char*)"Could not renew the name}",0,0,""); \
  return;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(public_address,0,sizeof(public_address));
  memset(tx_hash,0,sizeof(tx_hash));
  memset(delegates_public_address,0,sizeof(delegates_public_address));
  memset(delegates_amount,0,sizeof(delegates_amount));
  memset(name,0,sizeof(name));
  memset(delegate_name,0,sizeof(delegate_name));

  // get the current time
  get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);

  // check if it is valid data interval
  if (test_settings == 0 && current_UTC_date_and_time.tm_min % 60 >= 0 && current_UTC_date_and_time.tm_min % 60 < 10)
  {
    REMOTE_DATA_RENEWAL_END_ERROR;
  }

  // verify the message
  if (verify_data(MESSAGE,0) == 0 || string_count(MESSAGE,"|") != REMOTE_DATA_RENEWAL_END_PARAMETER_AMOUNT || check_for_invalid_strings(MESSAGE) == 0)
  {   
    REMOTE_DATA_RENEWAL_END_ERROR;
  }

  // parse the message
  for (count = 0, count2 = 0; count < REMOTE_DATA_RENEWAL_END_PARAMETER_AMOUNT; count++)
  {
    if (count == 1)
    {
      if ((data_size = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2) != TRANSACTION_HASH_LENGTH)
      {
        REMOTE_DATA_RENEWAL_END_ERROR;
      }
      memcpy(tx_hash,&MESSAGE[count2],data_size);
    }
    if (count == 2)
    {
      if ((data_size = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2) != XCASH_WALLET_LENGTH)
      {
        REMOTE_DATA_RENEWAL_END_ERROR;
      }
      memcpy(public_address,&MESSAGE[count2],data_size);
    }
    count2 = (int)(strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) + 1);
  }

  // check if the data is valid
  if (strlen(public_address) != XCASH_WALLET_LENGTH || strncmp(public_address,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || strlen(tx_hash) != TRANSACTION_HASH_LENGTH)
  {
    REMOTE_DATA_RENEWAL_END_ERROR;
  }

  // check if the timestamp is valid
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memcpy(data,"{\"address\":\"",12);
  memcpy(data+strlen(data),public_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\"}",2);
  if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,data,"timestamp",data2) == 0)
  {
    REMOTE_DATA_RENEWAL_END_ERROR;
  }

  sscanf(data2,"%ld", &registration_length);
  if (test_settings == 0 && (registration_length + REMOTE_DATA_SAVE_NAME_LENGTH) < time(NULL))
  {
    REMOTE_DATA_RENEWAL_END_ERROR;
  }

  // get the delegates data
  memset(data,0,sizeof(data));
  memcpy(data,"{\"address\":\"",12);
  memcpy(data+strlen(data),public_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\"}",2);
  if (read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,data,"reserve_delegate_address",delegates_public_address) == 0 || read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,data,"reserve_delegate_amount",delegates_amount) == 0 || read_document_field_from_collection(remote_data_database_name,DATABASE_COLLECTION,data,"name",name) == 0)
  {
    REMOTE_DATA_RENEWAL_END_ERROR;
  }

  // check if the tx_hash is valid
  if (test_settings == 0 && remote_data_validate_tx_hash(tx_hash,public_address,delegates_public_address,delegates_amount) == 0)
  {
    REMOTE_DATA_RENEWAL_END_ERROR;
  }

  // renew the name (set the new tx_hash and update the timestamp)

  // get the delegate name for the logs
  memset(data2,0,sizeof(data2));
  memcpy(data2,"{\"public_address\":\"",19);
  memcpy(data2+strlen(data2),delegates_public_address,XCASH_WALLET_LENGTH);
  memcpy(data2+strlen(data2),"\"}",2);

  if (read_document_field_from_collection(database_name,"delegates",data2,"delegate_name",delegate_name) == 0)
  {
    REMOTE_DATA_RENEWAL_END_ERROR;
  }

  memset(data2,0,sizeof(data2));
  memcpy(data2,"{\"tx_hash\":\"",12);
  memcpy(data2+strlen(data2),tx_hash,TRANSACTION_HASH_LENGTH);
  memcpy(data2+strlen(data2),"\"}",2);

  // update the delegate in the database
  if (update_document_from_collection(remote_data_database_name,DATABASE_COLLECTION,data,data2) == 0)
  {
    REMOTE_DATA_RENEWAL_END_ERROR;
  }

  memset(data2,0,sizeof(data2));
  memcpy(data2,"{\"timestamp\":\"",14);
  snprintf(data2+strlen(data2),MAXIMUM_NUMBER_SIZE,"%ld",time(NULL) + REMOTE_DATA_REGISTRATION_LENGTH);
  memcpy(data2+strlen(data2),"\"}",2);

  // update the delegate in the database
  if (update_document_from_collection(remote_data_database_name,DATABASE_COLLECTION,data,data2) == 0)
  {
    REMOTE_DATA_RENEWAL_END_ERROR;
  }

  // update the delegates statistics in the database
  if (remote_data_update_delegates_statistics(delegate_name,delegates_amount) == 0)
  {
    REMOTE_DATA_RENEWAL_END_ERROR;
  }

  // log the data that the purchase was made to the network
  memset(data2,0,sizeof(data2));
  memcpy(data2,"Remote Data Protocol: Renewed ",30);
  memcpy(data2+strlen(data2),name,strlen(name));
  memcpy(data2+strlen(data2)," using delegate ",16);
  memcpy(data2+strlen(data2),delegate_name,strlen(delegate_name));
  memcpy(data2+strlen(data2)," for amount ",12);
  memcpy(data2+strlen(data2),delegates_amount,strlen(delegates_amount));
  color_print(data2,"yellow");

  send_data(CLIENT_SOCKET,(unsigned char*)"Renewed the name}",0,0,"");
  return;

  #undef DATABASE_COLLECTION
  #undef REMOTE_DATA_RENEWAL_END_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: add_delegates_to_remote_data_delegates
Description: Adds delegates to the remote_data_delegates
-----------------------------------------------------------------------------------------------------------
*/

void add_delegates_to_remote_data_delegates(void)
{
  // Variables
  char data[SMALL_BUFFER_SIZE];
  struct delegates delegates[MAXIMUM_AMOUNT_OF_DELEGATES];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;
  int count2;
  int total_delegates;

  // define macros
  #define DATABASE_COLLECTION "remote_data_delegates"

  memset(data,0,sizeof(data));

  // initialize the delegates struct
  INITIALIZE_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES,"add_delegates_to_remote_data_delegates",data,current_date_and_time,current_UTC_date_and_time);

  // organize the delegates  
  if ((total_delegates = organize_delegates(delegates)) == 0)
  {
    POINTER_RESET_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES);
    return;
  }

  for (count = 0; count < total_delegates; count++)
  {
    // check to make sure it is not a network data node
    for (count2 = 0; count2 < NETWORK_DATA_NODES_AMOUNT; count2++)
    {
      if (strncmp(network_data_nodes_list.network_data_nodes_public_address[count2],delegates[count].public_address,XCASH_WALLET_LENGTH) == 0)
      {
        goto start;
      }
    }

    // check to make sure it is not an official delegate
    if (strncmp(OFFICIAL_SHARED_DELEGATE_PUBLIC_ADDRESS_PRODUCTION,delegates[count].public_address,XCASH_WALLET_LENGTH) == 0)
    {
      goto start;
    }

    // add the delegate to the database
    memset(data,0,strlen(data));
    memcpy(data,"{\"name\":\"",9);
    memcpy(data+strlen(data),delegates[count].delegate_name,strlen(delegates[count].delegate_name));
    memcpy(data+strlen(data),"\",\"public_address\":\"",20);
    memcpy(data+strlen(data),delegates[count].public_address,XCASH_WALLET_LENGTH);
    memcpy(data+strlen(data),"\",\"amount\":\"" REMOTE_DATA_DELEGATES_DEFAULT_AMOUNT "\",\"total_registered_renewed_amount\":\"0\",\"total_amount\":\"0\"}",81);

    insert_document_into_collection_json(remote_data_database_name,DATABASE_COLLECTION,data);

    start:
    RESET_ERROR_MESSAGES;
  }
  
  POINTER_RESET_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES);
  return;

  #undef DATABASE_COLLECTION
}



/*
-----------------------------------------------------------------------------------------------------------
Name: check_for_expired_names
Description: Checks for expired names
-----------------------------------------------------------------------------------------------------------
*/

void check_for_expired_names(void)
{
  // Variables
  char buffer[1024];
  char name[SMALL_BUFFER_SIZE];
  char message[BUFFER_SIZE];
  char message2[BUFFER_SIZE];
  char message3[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  size_t count = 0;
  size_t counter = 0;
  struct database_multiple_documents_fields database_multiple_documents_fields;
  int document_count = 0;
  long int timestamp;

  // define macros
  #define DATABASE_COLLECTION "remote_data"

  #define REMOTE_DATA_CHECK_FOR_EXPIRED_NAMES_ERROR(settings,MESSAGE) \
  if (debug_settings == 1) \
  { \
  memcpy(error_message.function[error_message.total],"check_for_expired_names",23); \
  memcpy(error_message.data[error_message.total],MESSAGE,sizeof(MESSAGE)-1); \
  error_message.total++; \
  } \
  if ((settings) == 0) \
  { \
    POINTER_RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,TOTAL_REMOTE_DATA_DATABASE_FIELDS); \
  } \
  return;

  
  memset(message,0,sizeof(message));
  memset(message2,0,sizeof(message2));
  memset(message3,0,sizeof(message3));

  // get the total documents
  if ((document_count = count_all_documents_in_collection(remote_data_database_name,DATABASE_COLLECTION)) <= 0)
  {
    REMOTE_DATA_CHECK_FOR_EXPIRED_NAMES_ERROR(1,"Could not get the remote data");
  }

  memset(buffer,0,sizeof(buffer));

  // initialize the database_multiple_documents_fields struct
  INITIALIZE_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,document_count,TOTAL_REMOTE_DATA_DATABASE_FIELDS,"maintain_slist_and_plist",buffer,current_date_and_time,current_UTC_date_and_time);

  if (read_multiple_documents_all_fields_from_collection(remote_data_database_name,DATABASE_COLLECTION,"",&database_multiple_documents_fields,0,document_count,0,"") == 0)
  {
    REMOTE_DATA_CHECK_FOR_EXPIRED_NAMES_ERROR(0,"Could not get the remote data");
  }

  // loop through all documents, if the name is expired
  for (count = 0; count < database_multiple_documents_fields.document_count; count++)
  {
    // get the timestamp
    sscanf(database_multiple_documents_fields.value[count][8], "%ld", &timestamp);

    // check if this is a name that was saved but never registered
    if (time(NULL) > (timestamp + REMOTE_DATA_REGISTRATION_LENGTH) && strncmp(database_multiple_documents_fields.value[count][2],"",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[count][3],"",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[count][11],"",BUFFER_SIZE) == 0)
    {
      // delete the document
      memset(message,0,sizeof(message));
      memcpy(message,"{\"name\":\"",9);
      memcpy(message+strlen(message),database_multiple_documents_fields.value[count][0],strlen(database_multiple_documents_fields.value[count][0]));
      memcpy(message+strlen(message),"\"}",2);
      delete_document_from_collection(remote_data_database_name,DATABASE_COLLECTION,message);
    }

    // check if this is a name that expired or was in renewal and never got renewed
    if (time(NULL) > (timestamp + REMOTE_DATA_REGISTRATION_LENGTH))
    {
      // edit the document
      memset(name,0,sizeof(name));
      memset(message,0,sizeof(message));
      memset(message2,0,sizeof(message2));
      memset(message3,0,sizeof(message3));

      memcpy(name,"{\"name\":\"",9);
      memcpy(name+strlen(name),database_multiple_documents_fields.value[count][0],strlen(database_multiple_documents_fields.value[count][0]));
      memcpy(name+strlen(name),"\"}",2);

      // move the saddress to the saddress_list
      memcpy(message,database_multiple_documents_fields.value[count][4],strlen(database_multiple_documents_fields.value[count][4]));
      memcpy(message+strlen(message),database_multiple_documents_fields.value[count][2],XCASH_WALLET_LENGTH);
      memcpy(message+strlen(message),"|",1);       
      memcpy(message2,"{\"saddress_list\":\"",18);
      memcpy(message2+strlen(message2),message,strlen(message));
      memcpy(message2+strlen(message2),"\"}",2);
      update_document_from_collection(remote_data_database_name,DATABASE_COLLECTION,name,message2);

      // move the paddress to the paddress_list
      memset(message,0,sizeof(message));
      memset(message2,0,sizeof(message2));
      memcpy(message,database_multiple_documents_fields.value[count][5],strlen(database_multiple_documents_fields.value[count][5]));
      memcpy(message+strlen(message),database_multiple_documents_fields.value[count][3],XCASH_WALLET_LENGTH);
      memcpy(message+strlen(message),"|",1);       
      memcpy(message2,"{\"paddress_list\":\"",18);
      memcpy(message2+strlen(message2),message,strlen(message));
      memcpy(message2+strlen(message2),"\"}",2);
      update_document_from_collection(remote_data_database_name,DATABASE_COLLECTION,name,message2);

      // empty all other fields (set the timestamp in the future so it does not get checked again until someone renews it)
      update_document_from_collection(remote_data_database_name,DATABASE_COLLECTION,name,"{\"address\":\"\"}");
      update_document_from_collection(remote_data_database_name,DATABASE_COLLECTION,name,"{\"saddress\":\"\"}");
      update_document_from_collection(remote_data_database_name,DATABASE_COLLECTION,name,"{\"paddress\":\"\"}");
      update_document_from_collection(remote_data_database_name,DATABASE_COLLECTION,name,"{\"website\":\"\"}");
      update_document_from_collection(remote_data_database_name,DATABASE_COLLECTION,name,"{\"smart_contract_hash\":\"\"}");
      update_document_from_collection(remote_data_database_name,DATABASE_COLLECTION,name,"{\"reserve_delegate_address\":\"\"}");
      update_document_from_collection(remote_data_database_name,DATABASE_COLLECTION,name,"{\"reserve_delegate_amount\":\"\"}");
      update_document_from_collection(remote_data_database_name,DATABASE_COLLECTION,name,"{\"tx_hash\":\"\"}");
      update_document_from_collection(remote_data_database_name,DATABASE_COLLECTION,name,"{\"timestamp\":\"10000000000\"}");
    }
  }
  
  POINTER_RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,TOTAL_REMOTE_DATA_DATABASE_FIELDS);
  return;

  #undef DATABASE_COLLECTION
  #undef REMOTE_DATA_CHECK_FOR_EXPIRED_NAMES_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: maintain_slist_and_plist
Description: Maintains the slist and plist
-----------------------------------------------------------------------------------------------------------
*/

void maintain_slist_and_plist(void)
{
  // Variables
  char buffer[1024];
  char saddress[XCASH_WALLET_LENGTH+1];
  char paddress[XCASH_WALLET_LENGTH+1];
  char message[SMALL_BUFFER_SIZE];
  char message2[BUFFER_SIZE];
  char message3[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  size_t count = 0;
  size_t counter = 0;
  struct database_multiple_documents_fields database_multiple_documents_fields;
  int document_count = 0;

  // define macros
  #define DATABASE_COLLECTION "remote_data"

  #define REMOTE_DATA_MAINTAIN_SLIST_AND_PLIST_ERROR(settings,MESSAGE) \
  if (debug_settings == 1) \
  { \
  memcpy(error_message.function[error_message.total],"maintain_slist_and_plist",24); \
  memcpy(error_message.data[error_message.total],MESSAGE,sizeof(MESSAGE)-1); \
  error_message.total++; \
  } \
  if ((settings) == 0) \
  { \
    POINTER_RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,TOTAL_REMOTE_DATA_DATABASE_FIELDS); \
  } \
  return;

  memset(saddress,0,sizeof(saddress));
  memset(paddress,0,sizeof(paddress));
  memset(message,0,sizeof(message));
  memset(message2,0,sizeof(message2));
  memset(message3,0,sizeof(message3));

  // get the total documents
  if ((document_count = count_all_documents_in_collection(remote_data_database_name,DATABASE_COLLECTION)) <= 0)
  {
    REMOTE_DATA_MAINTAIN_SLIST_AND_PLIST_ERROR(1,"Could not get the remote data");
  }

  memset(buffer,0,sizeof(buffer));

  // initialize the database_multiple_documents_fields struct
  INITIALIZE_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,document_count,TOTAL_REMOTE_DATA_DATABASE_FIELDS,"maintain_slist_and_plist",buffer,current_date_and_time,current_UTC_date_and_time);

  if (read_multiple_documents_all_fields_from_collection(remote_data_database_name,DATABASE_COLLECTION,"",&database_multiple_documents_fields,0,document_count,0,"") == 0)
  {
    REMOTE_DATA_MAINTAIN_SLIST_AND_PLIST_ERROR(0,"Could not get the remote data");
  }

  // loop through all documents, for every saddress read all saddress_list and if its their remove it from the list. Do the same for paddress and paddress_list
  for (count = 0; count < database_multiple_documents_fields.document_count; count++)
  {
    // get the saddress and paddress
    memset(saddress,0,sizeof(saddress));
    memset(paddress,0,sizeof(paddress));
    memcpy(saddress,database_multiple_documents_fields.value[count][2],XCASH_WALLET_LENGTH);
    memcpy(paddress,database_multiple_documents_fields.value[count][3],XCASH_WALLET_LENGTH);

    for (counter = 0; counter < database_multiple_documents_fields.document_count; counter++)
    {
      // check for saddress
      if (strstr(database_multiple_documents_fields.value[counter][4],saddress) != NULL)
      {
        // remove the saddress from the list
        memset(message,0,sizeof(message));
        memcpy(message,saddress,XCASH_WALLET_LENGTH);
        memcpy(message+strlen(message),"|",1);
        memset(message2,0,sizeof(message2));
        memcpy(message2,database_multiple_documents_fields.value[counter][4],strlen(database_multiple_documents_fields.value[counter][4]));
        string_replace(message2,sizeof(message2),message,"");

        memset(message,0,sizeof(message));
        memcpy(message,"{\"name\":\"",9);
        memcpy(message+strlen(message),database_multiple_documents_fields.value[counter][0],strlen(database_multiple_documents_fields.value[counter][0]));
        memcpy(message+strlen(message),"\"}",2);

        memset(message3,0,sizeof(message3));
        memcpy(message3,"{\"saddress_list\":\"",18);
        memcpy(message3+strlen(message3),message2,strlen(message2));
        memcpy(message3+strlen(message3),"\"}",2);

        // update the document
        if (update_document_from_collection(remote_data_database_name,DATABASE_COLLECTION,message,message3) == 0)
        {
          continue;
        }
      }

      // check for paddress
      if (strstr(database_multiple_documents_fields.value[counter][5],paddress) != NULL)
      {
        // remove the paddress from the list
        memset(message,0,sizeof(message));
        memcpy(message,paddress,XCASH_WALLET_LENGTH);
        memcpy(message+strlen(message),"|",1);
        memset(message2,0,sizeof(message2));
        memcpy(message2,database_multiple_documents_fields.value[counter][5],strlen(database_multiple_documents_fields.value[counter][5]));
        string_replace(message2,sizeof(message2),message,"");

        memset(message,0,sizeof(message));
        memcpy(message,"{\"name\":\"",9);
        memcpy(message+strlen(message),database_multiple_documents_fields.value[counter][0],strlen(database_multiple_documents_fields.value[counter][0]));
        memcpy(message+strlen(message),"\"}",2);

        memset(message3,0,sizeof(message3));
        memcpy(message3,"{\"paddress_list\":\"",18);
        memcpy(message3+strlen(message3),message2,strlen(message2));
        memcpy(message3+strlen(message3),"\"}",2);

        // update the document
        if (update_document_from_collection(remote_data_database_name,DATABASE_COLLECTION,message,message3) == 0)
        {
          continue;
        }
      }
    }
  }
  
  POINTER_RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,TOTAL_REMOTE_DATA_DATABASE_FIELDS);
  return;

  #undef DATABASE_COLLECTION
  #undef REMOTE_DATA_MAINTAIN_SLIST_AND_PLIST_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: remote_data_database_synchronization_timer_thread
Description: Runs the timer thread database sync functions for the remote data
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

void* remote_data_database_synchronization_timer_thread(void* parameters)
{
  // Variables
  size_t count;
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;

  // unused parameters
  (void)parameters;

  // check if the start block is valid
  sscanf(current_block_height,"%zu", &count);

  while (count < BLOCK_HEIGHT_REMOTE_DATA_DELEGATES)
  {
    sscanf(current_block_height,"%zu", &count);
    sleep(60);
  }

  for (;;)
  {
    get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);

    if (current_UTC_date_and_time.tm_min == 0)
    {
      sync_delegates_remote_data_databases();
    }
    sleep(60);
  }
  pthread_exit((void *)(intptr_t)1);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: remote_data_maintenance_timer_thread
Description: Runs the timer thread maintenance functions for the remote data
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

void* remote_data_maintenance_timer_thread(void* parameters)
{
  // Variables
  size_t count;
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;

  // unused parameters
  (void)parameters;

  // check if the start block is valid
  sscanf(current_block_height,"%zu", &count);

  while (count < BLOCK_HEIGHT_REMOTE_DATA_DELEGATES)
  {
    sscanf(current_block_height,"%zu", &count);
    sleep(60);
  }


  for (;;)
  {
    get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);

    if (current_UTC_date_and_time.tm_hour == 0 && current_UTC_date_and_time.tm_min == 30)
    {
      maintain_slist_and_plist();
    }
    else if (current_UTC_date_and_time.tm_hour == 1 && current_UTC_date_and_time.tm_min == 30)
    {
      add_delegates_to_remote_data_delegates();
    }
    else if (current_UTC_date_and_time.tm_min == 30 && current_UTC_date_and_time.tm_hour != 0 && current_UTC_date_and_time.tm_hour != 1)
    {
      check_for_expired_names();
    }
    sleep(60);
  }
  pthread_exit((void *)(intptr_t)1);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_delegates_remote_data_databases
Description: Sync all of the remote data database
-----------------------------------------------------------------------------------------------------------
*/

void sync_delegates_remote_data_databases(void)
{
  // Variables
  char data[SMALL_BUFFER_SIZE];
  char database_data_hash_majority[REMOTE_DATA_DATABASE_TOTAL][DATA_HASH_LENGTH+1];
  int count;
  int count3;
  size_t count2;
  int database_count;
  int settings = 0;
  int reset_count = 1;

  // define macros
  #define SYNC_BLOCK_VERIFIERS \
  if (database_count == 0) \
  { \
    color_print("Syncing the remote data database","yellow"); \
    sync_remote_data_database(0,SYNCED_BLOCK_VERIFIER_STRING); \
  } \
  else if (database_count == 1) \
  { \
    color_print("Syncing the remote data delegates database","yellow"); \
    sync_remote_data_delegates_database(0,SYNCED_BLOCK_VERIFIER_STRING); \
  }

  #define SYNC_BLOCK_VERIFIERS_FROM_SPECIFIC_BLOCK_VERIFIER(BLOCK_VERIFIER) \
  if (database_count == 0) \
  { \
    color_print("Syncing the remote data database","yellow"); \
    sync_remote_data_database(0,BLOCK_VERIFIER); \
  } \
  else if (database_count == 1) \
  { \
    color_print("Syncing the remote data delegates database","yellow"); \
    sync_remote_data_delegates_database(0,BLOCK_VERIFIER); \
  }

  #define SYNC_BLOCK_VERIFIERS_FROM_RANDOM_NETWORK_DATA_NODE \
  if (database_count == 0) \
  { \
    color_print("Syncing the remote data database","yellow"); \
    sync_remote_data_database(2,""); \
  } \
  else if (database_count == 1) \
  { \
    color_print("Syncing the remote data delegates database","yellow"); \
    sync_remote_data_delegates_database(2,""); \
  }

  #define MAXIMUM_DATABASE_MAJORITY_SYNC_CONNECTIONS_ATTEMPTS 5

  memset(data,0,sizeof(data));

  // reset the variables
  for (database_count = 0; database_count < REMOTE_DATA_DATABASE_TOTAL; database_count++)
  {
    memset(database_data_hash_majority[database_count],0,sizeof(database_data_hash_majority[database_count]));
    for (count3 = 0; count3 < BLOCK_VERIFIERS_AMOUNT; count3++)
    {
      synced_block_verifiers_nodes[database_count][count3] = -1;
      memset(block_verifiers_sync_database_list.block_verifiers_database_data_hash[database_count][count3],0,sizeof(block_verifiers_sync_database_list.block_verifiers_database_data_hash[database_count][count3]));
    }
  }
  
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {    
    memset(block_verifiers_sync_database_list.block_verifiers_public_address[count],0,sizeof(block_verifiers_sync_database_list.block_verifiers_public_address[count]));
    memset(block_verifiers_sync_database_list.block_verifiers_IP_address[count],0,sizeof(block_verifiers_sync_database_list.block_verifiers_IP_address[count]));    
    memcpy(block_verifiers_sync_database_list.block_verifiers_public_address[count],current_block_verifiers_list.block_verifiers_public_address[count],strnlen(current_block_verifiers_list.block_verifiers_public_address[count],sizeof(block_verifiers_sync_database_list.block_verifiers_public_address[count])));
    memcpy(block_verifiers_sync_database_list.block_verifiers_IP_address[count],current_block_verifiers_list.block_verifiers_IP_address[count],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count],sizeof(block_verifiers_sync_database_list.block_verifiers_IP_address[count])));
    block_verifiers_sync_database_list.block_verifiers_previous_block_settings[count] = 0;
  }

  // wait so all block verifiers start at the same time, this way one is not reseting the variables as another one is sending them data
  sync_block_verifiers_minutes_and_seconds(0,40);

  // get the data and send it to the other block verifiers
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    if (strncmp(block_verifiers_sync_database_list.block_verifiers_public_address[count],xcash_wallet_public_address,BUFFER_SIZE) == 0)
    {
      // set your own block_verifiers_sync_database_list.block_verifiers_previous_block_settings
      block_verifiers_sync_database_list.block_verifiers_previous_block_settings[count] = 1;
      
      // get the database data hash and send it to all other network data nodes
      if (get_database_data_hash(block_verifiers_sync_database_list.block_verifiers_database_data_hash[0][count],database_name,"remote_data") == 0 || get_database_data_hash(block_verifiers_sync_database_list.block_verifiers_database_data_hash[1][count],database_name,"remote_data_delegates") == 0)
      {
        break;
      }

      // create the message
      memcpy(data,"{\r\n \"message_settings\": \"NETWORK_DATA_NODES_TO_NETWORK_DATA_NODES_DATABASE_SYNC_CHECK_REMOTE_DATA\",\r\n \"data_hash_remote_data\": \"",128);
      memcpy(data+strlen(data),block_verifiers_sync_database_list.block_verifiers_database_data_hash[0][count],DATA_HASH_LENGTH);
      memcpy(data+strlen(data),"\",\r\n \"data_hash_remote_data_delegates\": \"",41);
      memcpy(data+strlen(data),block_verifiers_sync_database_list.block_verifiers_database_data_hash[1][count],DATA_HASH_LENGTH);
      memcpy(data+strlen(data),"\",\r\n}",5);

      // sign_data
      if (sign_data(data) == 0)
      { 
        break;
      }

      // send the data to the other network data nodes
      block_verifiers_send_data_socket((const char*)data);
    }
  }

  // wait for all of the block verifiers to process the data
  sync_block_verifiers_minutes_and_seconds(1,15);

  // get the majority database data hash, and every block verifier that is in the majority
  for (database_count = 0; database_count < REMOTE_DATA_DATABASE_TOTAL; database_count++)
  {
    for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
    {
      for (count = 0, count3 = 0; count3 < BLOCK_VERIFIERS_AMOUNT; count3++)
      {
        if (strncmp(block_verifiers_sync_database_list.block_verifiers_database_data_hash[database_count][count2],block_verifiers_sync_database_list.block_verifiers_database_data_hash[database_count][count3],sizeof(block_verifiers_sync_database_list.block_verifiers_database_data_hash[database_count][count3])) == 0)
        {
          count++;
        }
      }
      if (count >= BLOCK_VERIFIERS_VALID_AMOUNT)
      {
        memset(database_data_hash_majority[database_count],0,sizeof(database_data_hash_majority[database_count]));
        memcpy(database_data_hash_majority[database_count],block_verifiers_sync_database_list.block_verifiers_database_data_hash[database_count][count2],DATA_HASH_LENGTH);

        // the majority can only use one data hash, but we dont exit the loop because we want to get every block verifier that is in the majority for syncing
        synced_block_verifiers_nodes[database_count][count2] = 0;
      }
    }
  }

  // check if there is a majority
  for (database_count = 0; database_count < REMOTE_DATA_DATABASE_TOTAL; database_count++, reset_count = 1)
  {
    if (database_count == 0)
    {
      color_print("Checking remote data","green");
    }
    else if (database_count == 1)
    {
      color_print("Checking remote data delegates","green");
    }
    if (strlen(database_data_hash_majority[database_count]) == DATA_HASH_LENGTH)
    {
      // there is a majority, check if the block verifier is in the majority
      for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
      {
        if (strncmp(block_verifiers_sync_database_list.block_verifiers_public_address[count],xcash_wallet_public_address,BUFFER_SIZE) == 0)
        {
          settings = strncmp(block_verifiers_sync_database_list.block_verifiers_database_data_hash[database_count][count],database_data_hash_majority[database_count],BUFFER_SIZE) == 0 ? 1 : 0;
        }
      }

      if (settings == 1)
      {
        // the block verifier is in the majority
        color_print("A majority has been reached and the block verifier is already synced with the majority\n","yellow");
        if (database_count == REMOTE_DATA_DATABASE_TOTAL-1)
        {
          return;
        }
        else
        {
          goto start;
        }
      }
      else
      {
        // the block verifier is not in the majority, sync the database from a block verifier that is in the majority
        color_print("The database is not synced with the majority of block verifiers, syncing the database from a random block verifier that is in the majority","yellow");  
        do
        {
          SYNC_BLOCK_VERIFIERS;

          color_print("Checking to make sure the database received is the majority database, if not sync from another block verifier in the majority\n","blue");

          // check if you received the same database data as the database_data_hash_majority, otherwise sync from a different block verifier
          memset(data,0,sizeof(data));

          if (reset_count >= MAXIMUM_DATABASE_MAJORITY_SYNC_CONNECTIONS_ATTEMPTS)
          {
            color_print("Could not sync the database, maximum connection attempts has been reached\n","red");
            goto start2;
          }
          reset_count++;
        } while ((database_count == 0 && (get_database_data_hash(data,database_name,"remote_data") == 0 || strncmp(data,database_data_hash_majority[database_count],sizeof(database_data_hash_majority[database_count])) != 0)) || (database_count == 1 && (get_database_data_hash(data,database_name,"remote_data_delegates") == 0 || strncmp(data,database_data_hash_majority[database_count],sizeof(database_data_hash_majority[database_count])) != 0)));      
      
        color_print("Successfully synced the database\n","yellow");

        start2:
        if (database_count == REMOTE_DATA_DATABASE_TOTAL-1)
        {
          return;
        }
        else
        {
          goto start;
        }
      }    
    }
    else
    {
      // there is not a majority, sync from a block verifier that has the previous blocks reserve bytes data
      color_print("A majority could not be reached between block verifiers for the database sync. Syncing the database from a block verifier with the previous blocks reserve bytes\n","yellow");
    
      for (count = 0, settings = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
      {
        if (block_verifiers_sync_database_list.block_verifiers_previous_block_settings[count] == 1)
        {
          settings = 1;
          if (strncmp(block_verifiers_sync_database_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
          {
            SYNC_BLOCK_VERIFIERS_FROM_SPECIFIC_BLOCK_VERIFIER(block_verifiers_sync_database_list.block_verifiers_IP_address[count]);
            if (database_count == REMOTE_DATA_DATABASE_TOTAL-1)
            {
              return;
            }  
            else
            {
              goto start;
            }
          }
          else
          {
            color_print("Successfully synced all databases\n","yellow");
            if (database_count == REMOTE_DATA_DATABASE_TOTAL-1)
            {
              return;
            }
            else
            {
              goto start;
            }
          } 
        }        
      }

      // if no block verifiers had the previous blocks reserve bytes, sync from a random network data node
      if (settings == 0)
      {
        SYNC_BLOCK_VERIFIERS_FROM_RANDOM_NETWORK_DATA_NODE;
      }
    }
    start: ;
  }
  color_print("Syncing databases is complete","yellow");
  return;

  #undef SYNC_BLOCK_VERIFIERS
  #undef SYNC_BLOCK_VERIFIERS_FROM_SPECIFIC_BLOCK_VERIFIER
  #undef SYNC_BLOCK_VERIFIERS_FROM_RANDOM_NETWORK_DATA_NODE
  #undef MAXIMUM_DATABASE_MAJORITY_SYNC_CONNECTIONS_ATTEMPTS
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_network_data_nodes_to_network_data_nodes_database_sync_check_remote_data
Description: Runs the code when the server receives the NETWORK_DATA_NODES_TO_NETWORK_DATA_NODES_DATABASE_SYNC_CHECK_REMOTE_DATA message
Parameters:
  MESSAGE - The message
-----------------------------------------------------------------------------------------------------------
*/

void server_receive_data_socket_network_data_nodes_to_network_data_nodes_database_sync_check_remote_data(const char* MESSAGE)
{
  // Variables
  char data_hash[REMOTE_DATA_DATABASE_TOTAL][DATA_HASH_LENGTH+1];
  char public_address[XCASH_WALLET_LENGTH+1];
  char data[SMALL_BUFFER_SIZE];
  int count;
  int count2;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_NETWORK_DATA_NODES_TO_NETWORK_DATA_NODES_DATABASE_SYNC_CHECK_REMOTE_DATA_ERROR(settings) \
  if (debug_settings == 1) \
  { \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_network_data_nodes_to_network_data_nodes_database_sync_check_remote_data",99); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  } \
  return;

  memset(data,0,sizeof(data));
  memset(public_address,0,sizeof(public_address));

  for (count = 0; count < REMOTE_DATA_DATABASE_TOTAL; count++)
  {
    memset(data_hash[count],0,sizeof(data_hash[count]));
  }

  // parse the message
  if (registration_settings == 0)
  {
    if (parse_json_data(MESSAGE,"public_address",public_address,sizeof(public_address)) == 0 || parse_json_data(MESSAGE,"data_hash_remote_data",data_hash[0],DATA_HASH_LENGTH) == 0 || parse_json_data(MESSAGE,"data_hash_remtoe_data_delegates",data_hash[1],DATA_HASH_LENGTH) == 0)
    {
      SERVER_RECEIVE_DATA_SOCKET_NETWORK_DATA_NODES_TO_NETWORK_DATA_NODES_DATABASE_SYNC_CHECK_REMOTE_DATA_ERROR("Could not parse the message");
    }
  }

  if (registration_settings == 0)
  {
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (strncmp(block_verifiers_sync_database_list.block_verifiers_public_address[count],public_address,sizeof(public_address)) == 0)
      {
        for (count2 = 0; count2 < REMOTE_DATA_DATABASE_TOTAL; count2++)
        {
          memset(block_verifiers_sync_database_list.block_verifiers_database_data_hash[count2][count],0,sizeof(block_verifiers_sync_database_list.block_verifiers_database_data_hash[count2][count]));
          memcpy(block_verifiers_sync_database_list.block_verifiers_database_data_hash[count2][count],data_hash[count2],DATA_HASH_LENGTH);
        }
        block_verifiers_sync_database_list.block_verifiers_previous_block_settings[count] = 1;
      }
    }
  }
  else
  {
    for (count = 0; count < NETWORK_DATA_NODES_AMOUNT; count++)
    {
      if (strncmp(network_data_nodes_sync_database_list.network_data_node_public_address[count],public_address,sizeof(public_address)) == 0)
      {
        memset(network_data_nodes_sync_database_list.network_data_nodes_database_data_hash[count],0,sizeof(network_data_nodes_sync_database_list.network_data_nodes_database_data_hash[count]));
        memcpy(network_data_nodes_sync_database_list.network_data_nodes_database_data_hash[count],data_hash[0],DATA_HASH_LENGTH);
        network_data_nodes_sync_database_list.network_data_nodes_previous_block_settings[count] = 1;
      }
    }
  }
  
  return;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_NETWORK_DATA_NODES_TO_NETWORK_DATA_NODES_DATABASE_SYNC_CHECK_REMOTE_DATA_ERROR
}



#define SEND_DATABASE_SYNC_CHECK_MESSAGE_REMOTE_DATA(database) \
for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++) \
{ \
  memset(synced_block_verifiers.vote_settings[count],0,sizeof(synced_block_verifiers.vote_settings[count])); \
  if (strncmp((char*)synced_block_verifiers.synced_block_verifiers_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0) \
  { \
    memset(data,0,strlen(data)); \
    memset(data2,0,strlen(data2)); \
    if (send_and_receive_data_socket(data,sizeof(data),synced_block_verifiers.synced_block_verifiers_IP_address[count],SEND_DATA_PORT,message,CONNECTION_TIMEOUT_SETTINGS) == 0 || verify_data(data,0) == 0) \
    { \
      memcpy(synced_block_verifiers.vote_settings[count],"connection_timeout",18); \
      synced_block_verifiers.vote_settings_connection_timeout++; \
    } \
    else \
    { \
      if (strstr(data,"false") != NULL) \
      { \
        memcpy(synced_block_verifiers.vote_settings[count],"false",5); \
        synced_block_verifiers.vote_settings_false++; \
      } \
      else \
      { \
        memcpy(synced_block_verifiers.vote_settings[count],"true",4); \
        synced_block_verifiers.vote_settings_true++; \
      } \
    } \
  } \
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_check_remote_data_database
Description: Checks if the block verifier needs to sync the remote data database
Paramters:
  settings - 1 to sync from a random block verifier, 2 to sync from a random network data node, 3 to sync from a random network data node and not check the majority, 4 to sync from the main network data node and not check the majority
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_check_remote_data_database(int settings)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE]; 
  char message[SMALL_BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;

  // define macros 
  #define DATABASE_COLLECTION "remote_data"
  #define SYNC_CHECK_REMOTE_DATA_DATABASE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"sync_check_remote_data_database",31); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;
  
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));

  if (test_settings == 0)
  {
    print_start_message(current_date_and_time,current_UTC_date_and_time,"Checking if the remote_data database is synced",data2);
  }

  if (time(NULL) - synced_block_verifiers.last_refresh_time_of_synced_block_verifiers > GET_SYNCED_BLOCK_VERIFIERS_REFRESH_SETTINGS && get_synced_block_verifiers() == 0)
  {
    SYNC_CHECK_REMOTE_DATA_DATABASE_ERROR("Could not get the synced block verifiers");
  }

  reset_synced_block_verifiers_vote_settings;

  if (settings == 1)
  {
    // get the database data hash for the delegates database
    if (get_database_data_hash(data,database_name,DATABASE_COLLECTION) == 0)
    {
      SYNC_CHECK_REMOTE_DATA_DATABASE_ERROR("Could not get the database data hash for the remote_data database");
    }

    // create the message
    memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"data_hash\": \"",117);
    memcpy(message+strlen(message),data,DATA_HASH_LENGTH);
    memcpy(message+strlen(message),"\",\r\n}",5);

    // sign_data
    if (sign_data(message) == 0)
    { 
      SYNC_CHECK_REMOTE_DATA_DATABASE_ERROR("Could not sign_data");
    }

    if (test_settings == 0)
    {
      color_print("Sending all block verifiers a message to check if the remote_data database is synced","white"); 
    }

    SEND_DATABASE_SYNC_CHECK_MESSAGE_REMOTE_DATA("remote_data_database");

    // get the vote settings of the block verifiers

    // check if a consensus could not be reached and sync from a network data node
    if (synced_block_verifiers.vote_settings_connection_timeout >= BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      if (test_settings == 0)
      {
        color_print("A Consensus could not be reached for trying to sync the remote_data database, syncing from a random network data node","red");
      }
      settings = 2;
    }
    else if (synced_block_verifiers.vote_settings_false >= BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      if (test_settings == 0)
      {
        color_print("The remote_data database is not synced, syncing from a random block verifier","red");
      }

      // get the data
      if (sync_remote_data_database(settings,"") == 0)
      {
        SYNC_CHECK_REMOTE_DATA_DATABASE_ERROR("Could not sync the remote_data database database");
      }
    }
  }
  if (settings == 2 || settings == 3)
  {
    if (test_settings == 0)
    {
      color_print("Syncing from a random network data node","white");
    }
    if (sync_remote_data_database(settings,"") == 0)
    {
      SYNC_CHECK_REMOTE_DATA_DATABASE_ERROR("Could not sync the remote_data database database");
    }
  }
  if (settings == 4)
  {
    if (test_settings == 0)
    {
      color_print("Syncing from the main network data node","white");
    }
    if (sync_remote_data_database(settings,network_data_nodes_list.network_data_nodes_IP_address[0]) == 0)
    {
      SYNC_CHECK_REMOTE_DATA_DATABASE_ERROR("Could not sync the remote_data database");
    }
  }


  // check to see if the block verifiers database is now in the majority, and if not directly sync the database from the main network data node
  if (settings != 3)
  {
    /*disable this for now
    sync_check_majority_delegates_database();*/
  }

  if (test_settings == 0)
  {
    color_print("The remote_data database is synced","green");
  }
  return 1;

  #undef DATABASE_COLLECTION  
  #undef SYNC_CHECK_REMOTE_DATA_DATABASE_ERROR  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_check_majority_remote_data_database
Description: Checks if the block verifiers remote data database is in the majority and if not syncs the remote data database from the main network data node
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_check_majority_remote_data_database(void)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE]; 
  char message[SMALL_BUFFER_SIZE];
  int count;

  // define macros 
  #define DATABASE_COLLECTION "remote_data"
  #define SYNC_CHECK_MAJORITY_REMOTE_DATA_DATABASE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"sync_check_majority_remote_data_database",40); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));

  if (test_settings == 0)
  {
    color_print("Checking if the database is in the majority","yellow");
  }

  if (time(NULL) - synced_block_verifiers.last_refresh_time_of_synced_block_verifiers > GET_SYNCED_BLOCK_VERIFIERS_REFRESH_SETTINGS && get_synced_block_verifiers() == 0)
  {
    SYNC_CHECK_MAJORITY_REMOTE_DATA_DATABASE_ERROR("Could not get the synced block verifiers");
  }

  reset_synced_block_verifiers_vote_settings;

  // get the database data hash for the reserve proofs database
  if (get_database_data_hash(data,database_name,DATABASE_COLLECTION) == 0)
  {
    SYNC_CHECK_MAJORITY_REMOTE_DATA_DATABASE_ERROR("Could not get the database data hash for the remote_data database");
  }

  // create the message
  memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"data_hash\": \"",117);
  memcpy(message+strlen(message),data,DATA_HASH_LENGTH);
  memcpy(message+strlen(message),"\",\r\n}",5);

  // sign_data
  if (sign_data(message) == 0)
  { 
    SYNC_CHECK_MAJORITY_REMOTE_DATA_DATABASE_ERROR("Could not sign_data");
  }

  SEND_DATABASE_SYNC_CHECK_MESSAGE_REMOTE_DATA("remote_data_database");

  // get the vote settings of the block verifiers

  // check if the block verifiers database is in the majority
  if (synced_block_verifiers.vote_settings_true < BLOCK_VERIFIERS_VALID_AMOUNT)
  {
    if (test_settings == 0)
    {
      color_print("The database is not in the majority, syncing from a random network data node","red");
    }
    sync_remote_data_database(2,"");   
  }
  return 1;

  #undef DATABASE_COLLECTION  
  #undef SYNC_CHECK_MAJORITY_REMOTE_DATA_DATABASE_ERROR  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_check_remote_data_database
Description: Checks if the block verifier needs to sync the remote data delegates database
Paramters:
  settings - 1 to sync from a random block verifier, 2 to sync from a random network data node, 3 to sync from a random network data node and not check the majority, 4 to sync from the main network data node and not check the majority
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_check_remote_data_delegates_database(int settings)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE]; 
  char message[SMALL_BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;

  // define macros 
  #define DATABASE_COLLECTION "remote_data_delegates"
  #define SYNC_CHECK_REMOTE_DATA_DELEGATES_DATABASE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"sync_check_remote_data_delegates_database",41); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;
  
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));

  if (test_settings == 0)
  {
    print_start_message(current_date_and_time,current_UTC_date_and_time,"Checking if the remote_data_delegates database is synced",data2);
  }

  if (time(NULL) - synced_block_verifiers.last_refresh_time_of_synced_block_verifiers > GET_SYNCED_BLOCK_VERIFIERS_REFRESH_SETTINGS && get_synced_block_verifiers() == 0)
  {
    SYNC_CHECK_REMOTE_DATA_DELEGATES_DATABASE_ERROR("Could not get the synced block verifiers");
  }

  reset_synced_block_verifiers_vote_settings;

  if (settings == 1)
  {
    // get the database data hash for the delegates database
    if (get_database_data_hash(data,database_name,DATABASE_COLLECTION) == 0)
    {
      SYNC_CHECK_REMOTE_DATA_DELEGATES_DATABASE_ERROR("Could not get the database data hash for the remote_data_delegates database");
    }

    // create the message
    memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DELEGATES_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"data_hash\": \"",127);
    memcpy(message+strlen(message),data,DATA_HASH_LENGTH);
    memcpy(message+strlen(message),"\",\r\n}",5);

    // sign_data
    if (sign_data(message) == 0)
    { 
      SYNC_CHECK_REMOTE_DATA_DELEGATES_DATABASE_ERROR("Could not sign_data");
    }

    if (test_settings == 0)
    {
      color_print("Sending all block verifiers a message to check if the remote_data_delegates database is synced","white"); 
    }

    SEND_DATABASE_SYNC_CHECK_MESSAGE_REMOTE_DATA("remote_data_delegates_database");

    // get the vote settings of the block verifiers

    // check if a consensus could not be reached and sync from a network data node
    if (synced_block_verifiers.vote_settings_connection_timeout >= BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      if (test_settings == 0)
      {
        color_print("A Consensus could not be reached for trying to sync the remote_data_delegates database, syncing from a random network data node","red");
      }
      settings = 2;
    }
    else if (synced_block_verifiers.vote_settings_false >= BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      if (test_settings == 0)
      {
        color_print("The remote_data_delegates database is not synced, syncing from a random block verifier","red");
      }

      // get the data
      if (sync_remote_data_delegates_database(settings,"") == 0)
      {
        SYNC_CHECK_REMOTE_DATA_DELEGATES_DATABASE_ERROR("Could not sync the remote_data_delegates database database");
      }
    }
  }
  if (settings == 2 || settings == 3)
  {
    if (test_settings == 0)
    {
      color_print("Syncing from a random network data node","white");
    }
    if (sync_remote_data_delegates_database(settings,"") == 0)
    {
      SYNC_CHECK_REMOTE_DATA_DELEGATES_DATABASE_ERROR("Could not sync the remote_data_delegates database database");
    }
  }
  if (settings == 4)
  {
    if (test_settings == 0)
    {
      color_print("Syncing from the main network data node","white");
    }
    if (sync_remote_data_delegates_database(settings,network_data_nodes_list.network_data_nodes_IP_address[0]) == 0)
    {
      SYNC_CHECK_REMOTE_DATA_DELEGATES_DATABASE_ERROR("Could not sync the remote_data_delegates database");
    }
  }


  // check to see if the block verifiers database is now in the majority, and if not directly sync the database from the main network data node
  if (settings != 3)
  {
    /*disable this for now
    sync_check_majority_delegates_database();*/
  }

  if (test_settings == 0)
  {
    color_print("The remote_data_delegates database is synced","green");
  }
  return 1;

  #undef DATABASE_COLLECTION  
  #undef SYNC_CHECK_REMOTE_DATA_DELEGATES_DATABASE_ERROR  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_check_majority_remote_data_delegates_database
Description: Checks if the block verifiers remote data database is in the majority and if not syncs the remote_data_delegates database from the main network data node
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_check_majority_remote_data_delegates_database(void)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE]; 
  char message[SMALL_BUFFER_SIZE];
  int count;

  // define macros 
  #define DATABASE_COLLECTION "remote_data_delegates"
  #define SYNC_CHECK_MAJORITY_REMOTE_DATA_DELEGATES_DATABASE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"sync_check_majority_remote_data_delegates_database",50); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));

  if (test_settings == 0)
  {
    color_print("Checking if the database is in the majority","yellow");
  }

  if (time(NULL) - synced_block_verifiers.last_refresh_time_of_synced_block_verifiers > GET_SYNCED_BLOCK_VERIFIERS_REFRESH_SETTINGS && get_synced_block_verifiers() == 0)
  {
    SYNC_CHECK_MAJORITY_REMOTE_DATA_DELEGATES_DATABASE_ERROR("Could not get the synced block verifiers");
  }

  reset_synced_block_verifiers_vote_settings;

  // get the database data hash for the remote_data_delegates database
  if (get_database_data_hash(data,database_name,DATABASE_COLLECTION) == 0)
  {
    SYNC_CHECK_MAJORITY_REMOTE_DATA_DELEGATES_DATABASE_ERROR("Could not get the database data hash for the remote_data_delegates database");
  }

  // create the message
  memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DELEGATES_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"data_hash\": \"",127);
  memcpy(message+strlen(message),data,DATA_HASH_LENGTH);
  memcpy(message+strlen(message),"\",\r\n}",5);

  // sign_data
  if (sign_data(message) == 0)
  { 
    SYNC_CHECK_MAJORITY_REMOTE_DATA_DELEGATES_DATABASE_ERROR("Could not sign_data");
  }

  SEND_DATABASE_SYNC_CHECK_MESSAGE_REMOTE_DATA("remote_data_delegates_database");

  // get the vote settings of the block verifiers

  // check if the block verifiers database is in the majority
  if (synced_block_verifiers.vote_settings_true < BLOCK_VERIFIERS_VALID_AMOUNT)
  {
    if (test_settings == 0)
    {
      color_print("The database is not in the majority, syncing from a random network data node","red");
    }
    sync_remote_data_delegates_database(2,"");   
  }
  return 1;

  #undef DATABASE_COLLECTION  
  #undef SYNC_CHECK_MAJORITY_REMOTE_DATA_DELEGATES_DATABASE_ERROR  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_block_verifiers_remote_data_database_sync_check_update
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DATABASE_SYNC_CHECK_UPDATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  MESSAGE - The message
-----------------------------------------------------------------------------------------------------------
*/

void server_receive_data_socket_block_verifiers_to_block_verifiers_remote_data_database_sync_check_update(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char data[SMALL_BUFFER_SIZE];
  char data2[DATA_HASH_LENGTH+1];

  // define macros
  #define DATABASE_COLLECTION "remote_data"
  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DATABASE_SYNC_CHECK_UPDATE_ERROR(settings) \
  if (debug_settings == 1) \
  { \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_remote_data_database_sync_check_update",100); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  } \
  ERROR_DATA_MESSAGE;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // get the database data hash for the delegates database
  if (get_database_data_hash(data2,database_name,DATABASE_COLLECTION) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not get the database data hash for the remote data database");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"data_hash",data,sizeof(data)) == 0 || strlen(data) != DATA_HASH_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not parse the message");
  }

  // create the message
  if (strncmp(data,data2,DATA_HASH_LENGTH) == 0)
  {
    memset(data,0,strlen(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"remote_data_database\": \"true\",\r\n}",139);
  }
  else
  {
    memset(data,0,strlen(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"remote_data_database\": \"false\",\r\n}",140);
  }
  
  // sign_data
  if (sign_data(data) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  send_data(CLIENT_SOCKET,(unsigned char*)data,0,1,"");

  return;

  #undef DATABASE_COLLECTION
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DATABASE_SYNC_CHECK_UPDATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_block_verifiers_remote_data_database_download_file_update
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DATABASE_DOWNLOAD_FILE_UPDATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
-----------------------------------------------------------------------------------------------------------
*/

void server_receive_data_socket_block_verifiers_to_block_verifiers_remote_data_database_download_file_update(const int CLIENT_SOCKET)
{
  // define macros
  #define DATABASE_COLLECTION "remote_data"
  
  // Constants
  const size_t DATABASE_COLLECTION_SIZE = get_database_collection_size(remote_data_database_name,DATABASE_COLLECTION);

  if (DATABASE_COLLECTION_SIZE == 0)
  {
    ERROR_DATA_MESSAGE;
  }

  char* data;
  char* data2;

  if (time(NULL) > TIME_SF_V_1_0_5_PART_1)
  {
    data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
    data2 = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  }
  else
  {
    data = (char*)calloc(DATABASE_COLLECTION_SIZE+SMALL_BUFFER_SIZE,sizeof(char));
    data2 = (char*)calloc(DATABASE_COLLECTION_SIZE+SMALL_BUFFER_SIZE,sizeof(char));    
  }

  // Variables
  char buffer[1024];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR(settings) \
  if (debug_settings == 1) \
  { \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_remote_data_database_download_file_update",103); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  } \
  pointer_reset_all; \
  ERROR_DATA_MESSAGE;

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
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_remote_data_database_download_file_update",103);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,buffer);  
    exit(0);
  }

  // get the database data for the reserve bytes database
  if (get_database_data(data2,database_name,DATABASE_COLLECTION) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not get the database data hash for the remote_data database");
  }
  
  // create the message
  memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"remote_data_database\": \"",133);
  memcpy(data+133,data2,strnlen(data2,MAXIMUM_BUFFER_SIZE));
  memcpy(data+strlen(data),"\",\r\n}",5);

  // send the data
  send_data(CLIENT_SOCKET,(unsigned char*)data,0,1,"");

  pointer_reset_all;
  return;

  #undef DATABASE_COLLECTION
  #undef pointer_reset_all
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_block_verifiers_remote_data_delegates_database_sync_check_update
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_remote_data_delegates_DELEGATES_DATABASE_SYNC_CHECK_UPDATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  MESSAGE - The message
-----------------------------------------------------------------------------------------------------------
*/

void server_receive_data_socket_block_verifiers_to_block_verifiers_remote_data_delegates_database_sync_check_update(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char data[SMALL_BUFFER_SIZE];
  char data2[DATA_HASH_LENGTH+1];

  // define macros
  #define DATABASE_COLLECTION "remote_data_delegates"
  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_ERROR(settings) \
  if (debug_settings == 1) \
  { \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_remote_data_delegates_database_sync_check_update",110); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  } \
  ERROR_DATA_MESSAGE;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // get the database data hash for the delegates database
  if (get_database_data_hash(data2,database_name,DATABASE_COLLECTION) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not get the database data hash for the remote data database");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"data_hash",data,sizeof(data)) == 0 || strlen(data) != DATA_HASH_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not parse the message");
  }

  // create the message
  if (strncmp(data,data2,DATA_HASH_LENGTH) == 0)
  {
    memset(data,0,strlen(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"remote_data_delegates_database\": \"true\",\r\n}",159);
  }
  else
  {
    memset(data,0,strlen(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"remote_data_delegates_database\": \"false\",\r\n}",160);
  }
  
  // sign_data
  if (sign_data(data) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  send_data(CLIENT_SOCKET,(unsigned char*)data,0,1,"");

  return;

  #undef DATABASE_COLLECTION
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_block_verifiers_remote_data_delegates_database_download_file_update
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_remote_data_delegates_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
-----------------------------------------------------------------------------------------------------------
*/

void server_receive_data_socket_block_verifiers_to_block_verifiers_remote_data_delegates_database_download_file_update(const int CLIENT_SOCKET)
{
  // define macros
  #define DATABASE_COLLECTION "remote_data_delegates"
  
  // Constants
  const size_t DATABASE_COLLECTION_SIZE = get_database_collection_size(remote_data_database_name,DATABASE_COLLECTION);

  if (DATABASE_COLLECTION_SIZE == 0)
  {
    ERROR_DATA_MESSAGE;
  }

  char* data;
  char* data2;

  if (time(NULL) > TIME_SF_V_1_0_5_PART_1)
  {
    data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
    data2 = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  }
  else
  {
    data = (char*)calloc(DATABASE_COLLECTION_SIZE+SMALL_BUFFER_SIZE,sizeof(char));
    data2 = (char*)calloc(DATABASE_COLLECTION_SIZE+SMALL_BUFFER_SIZE,sizeof(char));    
  }

  // Variables
  char buffer[1024];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR(settings) \
  if (debug_settings == 1) \
  { \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_remote_data_delegates_database_download_file_update",113); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  } \
  pointer_reset_all; \
  ERROR_DATA_MESSAGE;

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
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_remote_data_delegates_database_download_file_update",113);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,buffer);  
    exit(0);
  }

  // get the database data for the reserve bytes database
  if (get_database_data(data2,database_name,DATABASE_COLLECTION) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not get the database data hash for the remote_data_delegates database");
  }

  // create the message
  memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"remote_data_delegates_database\": \"",153);
  memcpy(data+153,data2,strnlen(data2,MAXIMUM_BUFFER_SIZE));
  memcpy(data+strlen(data),"\",\r\n}",5);

  // send the data
  send_data(CLIENT_SOCKET,(unsigned char*)data,0,1,"");

  pointer_reset_all;
  return;

  #undef DATABASE_COLLECTION
  #undef pointer_reset_all
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_remote_data_database
Description: Syncs the remote_data database
Paramters:
  settings - 1 to sync from a random block verifier, 2 to sync from a random network data node, otherwise the index of the network data node to sync from + 3
  DELEGATES_IP_ADDRESS - The specific delegates IP address, if you are syncing directly from a delegate, otherwise an empty string
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_remote_data_database(int settings, const char* DELEGATES_IP_ADDRESS)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char data2[BUFFER_SIZE];
  char database_data[BUFFER_SIZE];
  char block_verifiers_ip_address[BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int reset_count = 1;
  
  // define macros
  #define DATABASE_COLLECTION "remote_data"
  #define MESSAGE "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n}"
  #define SYNC_REMOTE_DATA_DATABASE_ERROR(message) \
  memset(data,0,strlen(data)); \
  memcpy(data+strlen(data),message,strnlen(message,BUFFER_SIZE)); \
  memcpy(data+strlen(data),block_verifiers_ip_address,strnlen(block_verifiers_ip_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH)); \
  memcpy(data+strlen(data),"\nConnecting to another block verifier",37); \
  color_print(data,"red"); \
  if (reset_count >= MAXIMUM_DATABASE_SYNC_CONNECTIONS_ATTEMPTS) \
  { \
    pointer_reset(data); \
    exit(0); \
  } \
  else \
  { \
    reset_count++; \
    goto start; \
  }

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    pointer_reset(data);
    memcpy(error_message.function[error_message.total],"sync_remote_data_database",25);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,data2);  
    exit(0);
  }

  start:

  memset(data,0,strlen(data));
  memset(data2,0,sizeof(data2));
  memset(database_data,0,sizeof(database_data));
  memset(block_verifiers_ip_address,0,sizeof(block_verifiers_ip_address));

  // get a block verifier to sync the database from
  get_block_verifier_for_syncing_database(settings,DELEGATES_IP_ADDRESS,block_verifiers_ip_address,2);

  // get the database data hash for the delegates database
  if (test_settings == 0)
  {
    memset(data2,0,sizeof(data2));
    memcpy(data2,"Getting the database data from ",31);
    memcpy(data2+31,block_verifiers_ip_address,strnlen(block_verifiers_ip_address,sizeof(block_verifiers_ip_address)));
    memcpy(data2+strlen(data2),"\n",sizeof(char));
    color_print(data2,"white");
  }
  if (get_database_data_hash(data,database_name,DATABASE_COLLECTION) == 0)
  {
    SYNC_REMOTE_DATA_DATABASE_ERROR("Could not get the database data hash for the remote_data database from ");
  }

  // create the message
  memset(data2,0,strlen(data2));
  memcpy(data2,MESSAGE,sizeof(MESSAGE)-1);

  // sign_data
  if (sign_data(data2) == 0)
  { 
    SYNC_REMOTE_DATA_DATABASE_ERROR("Could not sign_data");
  }     
  memset(data,0,strlen(data));

  // allow the database data to be received over the socket
  database_data_socket_settings = 1;

  if (send_and_receive_data_socket(data,MAXIMUM_BUFFER_SIZE,block_verifiers_ip_address,SEND_DATA_PORT,data2,DATABASE_SYNCING_TIMEOUT_SETTINGS) == 0)
  {
    database_data_socket_settings = 0;
    SYNC_REMOTE_DATA_DATABASE_ERROR("Could not receive data from ");
  }
  database_data_socket_settings = 0;

  // parse the message
  memset(data2,0,strlen(data2));
  if (parse_json_data(data,"remote_data_database",data2,sizeof(data2)) == 0)
  {
    SYNC_REMOTE_DATA_DATABASE_ERROR("Could not receive data from ");
  }

  // delete the collection from the database
  delete_collection_from_database(remote_data_database_name,DATABASE_COLLECTION);

  // if the database is empty dont add any data
  if (strncmp(data2,DATABASE_EMPTY_STRING,BUFFER_SIZE) != 0)
  {
    // add the data to the database
    //data2[strlen(data2)-2] = 0;
    // insert_multiple_documents_into_collection_json(remote_data_database_name,DATABASE_COLLECTION,data2,sizeof(data2));
    memset(data,0,strlen(data));
    memcpy(data,data2,strlen(data2)-2);
    insert_multiple_documents_into_collection_json(remote_data_database_name,DATABASE_COLLECTION,data,MAXIMUM_BUFFER_SIZE);
  }  

  pointer_reset(data);
  return 1;

  #undef DATABASE_COLLECTION
  #undef MESSAGE
  #undef SYNC_REMOTE_DATA_DATABASE_ERROR   
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_remote_data_delegates_database
Description: Syncs the remote_data_delegates database
Paramters:
  settings - 1 to sync from a random block verifier, 2 to sync from a random network data node, otherwise the index of the network data node to sync from + 3
  DELEGATES_IP_ADDRESS - The specific delegates IP address, if you are syncing directly from a delegate, otherwise an empty string
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_remote_data_delegates_database(int settings, const char* DELEGATES_IP_ADDRESS)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char data2[BUFFER_SIZE];
  char database_data[BUFFER_SIZE];
  char block_verifiers_ip_address[BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int reset_count = 1;
  
  // define macros
  #define DATABASE_COLLECTION "remote_data_delegates"
  #define MESSAGE "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_REMOTE_DATA_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n}"
  #define SYNC_REMOTE_DATA_DELEGATES_DATABASE_ERROR(message) \
  memset(data,0,strlen(data)); \
  memcpy(data+strlen(data),message,strnlen(message,BUFFER_SIZE)); \
  memcpy(data+strlen(data),block_verifiers_ip_address,strnlen(block_verifiers_ip_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH)); \
  memcpy(data+strlen(data),"\nConnecting to another block verifier",37); \
  color_print(data,"red"); \
  if (reset_count >= MAXIMUM_DATABASE_SYNC_CONNECTIONS_ATTEMPTS) \
  { \
    pointer_reset(data); \
    exit(0); \
  } \
  else \
  { \
    reset_count++; \
    goto start; \
  }

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    pointer_reset(data);
    memcpy(error_message.function[error_message.total],"sync_remote_data_delegates_database",35);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,data2);  
    exit(0);
  }

  start:

  memset(data,0,strlen(data));
  memset(data2,0,sizeof(data2));
  memset(database_data,0,sizeof(database_data));
  memset(block_verifiers_ip_address,0,sizeof(block_verifiers_ip_address));

  // get a block verifier to sync the database from
  get_block_verifier_for_syncing_database(settings,DELEGATES_IP_ADDRESS,block_verifiers_ip_address,2);

  // get the database data hash for the delegates database
  if (test_settings == 0)
  {
    memset(data2,0,sizeof(data2));
    memcpy(data2,"Getting the database data from ",31);
    memcpy(data2+31,block_verifiers_ip_address,strnlen(block_verifiers_ip_address,sizeof(block_verifiers_ip_address)));
    memcpy(data2+strlen(data2),"\n",sizeof(char));
    color_print(data2,"white");
  }
  if (get_database_data_hash(data,database_name,DATABASE_COLLECTION) == 0)
  {
    SYNC_REMOTE_DATA_DELEGATES_DATABASE_ERROR("Could not get the database data hash for the remote_data_delegates database from ");
  }

  // create the message
  memset(data2,0,strlen(data2));
  memcpy(data2,MESSAGE,sizeof(MESSAGE)-1);

  // sign_data
  if (sign_data(data2) == 0)
  { 
    SYNC_REMOTE_DATA_DELEGATES_DATABASE_ERROR("Could not sign_data");
  }     
  memset(data,0,strlen(data));

  // allow the database data to be received over the socket
  database_data_socket_settings = 1;

  if (send_and_receive_data_socket(data,MAXIMUM_BUFFER_SIZE,block_verifiers_ip_address,SEND_DATA_PORT,data2,DATABASE_SYNCING_TIMEOUT_SETTINGS) == 0)
  {
    database_data_socket_settings = 0;
    SYNC_REMOTE_DATA_DELEGATES_DATABASE_ERROR("Could not receive data from ");
  }
  database_data_socket_settings = 0;

  // parse the message
  memset(data2,0,strlen(data2));
  if (parse_json_data(data,"remote_data_delegates_database",data2,sizeof(data2)) == 0)
  {
    SYNC_REMOTE_DATA_DELEGATES_DATABASE_ERROR("Could not receive data from ");
  }

  // delete the collection from the database
  delete_collection_from_database(remote_data_database_name,DATABASE_COLLECTION);

  // if the database is empty dont add any data
  if (strncmp(data2,DATABASE_EMPTY_STRING,BUFFER_SIZE) != 0)
  {
    // add the data to the database
    //data2[strlen(data2)-2] = 0;
    // insert_multiple_documents_into_collection_json(remote_data_database_name,DATABASE_COLLECTION,data2,sizeof(data2));
    memset(data,0,strlen(data));
    memcpy(data,data2,strlen(data2)-2);
    insert_multiple_documents_into_collection_json(remote_data_database_name,DATABASE_COLLECTION,data,MAXIMUM_BUFFER_SIZE);
  }  

  pointer_reset(data);
  return 1;

  #undef DATABASE_COLLECTION
  #undef MESSAGE
  #undef SYNC_REMOTE_DATA_DELEGATES_DATABASE_ERROR   
}
