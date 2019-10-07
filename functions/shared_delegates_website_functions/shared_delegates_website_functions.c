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
#include "block_verifiers_synchronize_functions.h"
#include "database_functions.h"
#include "file_functions.h"
#include "network_daemon_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "network_wallet_functions.h"
#include "server_functions.h"
#include "shared_delegates_website_functions.h"
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
Name: server_receive_data_socket_shared_delegates_website_get_statistics
Description: Runs the code when the server receives /shareddelegateswebsitegetstatistics
Parameters:
  CLIENT_SOCKET - The socket to send data to
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_shared_delegates_website_get_statistics(const int CLIENT_SOCKET)
{
  // Variables
  char data[BUFFER_SIZE];
  char message[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm* current_UTC_date_and_time;
  int count = 0;
  int counter = 0;
  int total_blocks_found;
  int total_payments;
  int total_votes;
  long long int block_reward_number;
  long long int total_xcash = 0;
  struct database_multiple_documents_fields database_multiple_documents_fields;
  int document_count = 0;  

  // define macros
  #define TOTAL_BLOCKS_FOUND_DATABASE_FIELDS 5

  #define SERVER_RECEIVE_DATA_SOCKET_SHARED_DELEGATES_WEBSITE_GET_STATISTICS_ERROR(settings) \
  memset(message,0,strnlen(message,MAXIMUM_BUFFER_SIZE)); \
  memcpy(message,"{\"Error\":\"Could not get the delegates statistics\"}",50); \
  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),400,"application/json"); \
  if (settings == 0) \
  { \
    pointer_reset_database_array; \
  } \
  return 0;

  #define pointer_reset_database_array \
  for (count = 0; count < document_count; count++) \
  { \
    for (counter = 0; counter < TOTAL_BLOCKS_FOUND_DATABASE_FIELDS; counter++) \
    { \
      pointer_reset(database_multiple_documents_fields.item[count][counter]); \
      pointer_reset(database_multiple_documents_fields.value[count][counter]); \
    } \
  }

  memset(data,0,sizeof(data));
  memset(message,0,sizeof(message));

  // get the total blocks found
  document_count = count_all_documents_in_collection(DATABASE_NAME_DELEGATES,"blocks_found",0);
  total_blocks_found = document_count;

  // initialize the database_multiple_documents_fields struct 
  for (count = 0; count < document_count; count++)
  {
    for (counter = 0; counter < TOTAL_BLOCKS_FOUND_DATABASE_FIELDS; counter++)
    {
      database_multiple_documents_fields.item[count][counter] = (char*)calloc(100,sizeof(char));
      database_multiple_documents_fields.value[count][counter] = (char*)calloc(100,sizeof(char));

      if (database_multiple_documents_fields.item[count][counter] == NULL || database_multiple_documents_fields.value[count][counter] == NULL)
      {
        memcpy(error_message.function[error_message.total],"server_receive_data_socket_shared_delegates_website_get_statistics",66);
        memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
        error_message.total++;
        print_error_message(current_date_and_time,current_UTC_date_and_time,data);  
        exit(0);
      }
    }
  }
  database_multiple_documents_fields.document_count = 0;
  database_multiple_documents_fields.database_fields_count = 0;

  if (read_multiple_documents_all_fields_from_collection(DATABASE_NAME_DELEGATES,"blocks_found","",&database_multiple_documents_fields,1,document_count,0,"",0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_SHARED_DELEGATES_WEBSITE_GET_STATISTICS_ERROR(0);
  }

  // get the total xcash
  for (count = 0; count < document_count; count++)
  {
    sscanf(database_multiple_documents_fields.value[count][3], "%lld", &block_reward_number);
    total_xcash += block_reward_number;
  }

  // add the total payments to the database_document_fields struct 
  total_payments = count_all_documents_in_collection(DATABASE_NAME_DELEGATES,"public_addresses_payments",0); 

  // get the total vote count
  memcpy(message,"{\"public_address_voted_for\":\"",29);
  memcpy(message+29,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(message+127,"\"}",2);

  // check how many reserve proofs are for the public address
  for (total_votes = 0, count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
  { 
    memset(data,0,strlen(data));
    memcpy(data,"reserve_proofs_",15);
    snprintf(data+15,sizeof(data)-16,"%d",count);

    total_votes += count_documents_in_collection(DATABASE_NAME_DELEGATES,data,message,0);
  }

  memset(message,0,sizeof(message));

  memcpy(message,"{\"public_address\":\"",19);
  memcpy(message+strlen(message),xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(message+strlen(message),"\",\"total_blocks_found\":\"",24);
  snprintf(message+strlen(message),sizeof(message),"%d",total_blocks_found);
  memcpy(message+strlen(message),"\",\"total_xcash\":\"",17);
  snprintf(message+strlen(message),sizeof(message),"%lld",total_xcash);
  memcpy(message+strlen(message),"\",\"total_payments\":\"",20);
  snprintf(message+strlen(message),sizeof(message),"%d",total_payments);
  memcpy(message+strlen(message),"\",\"total_vote_count\":\"",22);
  snprintf(message+strlen(message),sizeof(message),"%d",total_votes);
  memcpy(message+strlen(message),"\",\"fee\":\"",9);
  snprintf(message+strlen(message),sizeof(message),"%lf",fee);
  memcpy(message+strlen(message),"\",\"minimum_amount\":\"",20);
  snprintf(message+strlen(message),sizeof(message),"%lld",minimum_amount);
  memcpy(message+strlen(message),"\"}",2);

  if (send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),200,"application/json") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_SHARED_DELEGATES_WEBSITE_GET_STATISTICS_ERROR(0);
  }

  pointer_reset_database_array; 
  return 1;

  #undef TOTAL_BLOCKS_FOUND_DATABASE_FIELDS
  #undef SERVER_RECEIVE_DATA_SOCKET_SHARED_DELEGATES_WEBSITE_GET_STATISTICS_ERROR
  #undef pointer_reset_database_array
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_get_blocks_found
Description: Runs the code when the server receives /getblocksfound
Parameters:
  CLIENT_SOCKET - The socket to send data to
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_get_blocks_found(const int CLIENT_SOCKET)
{
  // Variables
  char* message = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char)); 
  char buffer[1024]; 
  time_t current_date_and_time;
  struct tm* current_UTC_date_and_time;
  int count = 0;
  int counter = 0;
  struct database_multiple_documents_fields database_data;
  int document_count = 0;

  // define macros
  #define DATABASE_COLLECTION "blocks_found"
  #define TOTAL_BLOCKS_FOUND_DATABASE_FIELDS 5

  #define SERVER_RECEIVE_DATA_SOCKET_GET_BLOCKS_FOUND_ERROR(settings) \
  memset(message,0,strnlen(message,MAXIMUM_BUFFER_SIZE)); \
  memcpy(message,"{\"Error\":\"Could not get the delegates blocks found\"}",52); \
  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),400,"application/json"); \
  pointer_reset(message); \
  if (settings == 0) \
  { \
    pointer_reset_database_array; \
  } \
  return 0;

  #define pointer_reset_database_array \
  for (count = 0; count < document_count; count++) \
  { \
    for (counter = 0; counter < TOTAL_BLOCKS_FOUND_DATABASE_FIELDS; counter++) \
    { \
      pointer_reset(database_data.item[count][counter]); \
      pointer_reset(database_data.value[count][counter]); \
    } \
  }

  // get the total blocks found
  document_count = count_all_documents_in_collection(DATABASE_NAME_DELEGATES,DATABASE_COLLECTION,0);

  // initialize the database_data struct 
  for (count = 0; count < document_count; count++)
  {
    for (counter = 0; counter < TOTAL_BLOCKS_FOUND_DATABASE_FIELDS; counter++)
    {
      database_data.item[count][counter] = (char*)calloc(100,sizeof(char));
      database_data.value[count][counter] = (char*)calloc(100,sizeof(char));

      if (database_data.item[count][counter] == NULL || database_data.value[count][counter] == NULL)
      {
        memcpy(error_message.function[error_message.total],"server_receive_data_socket_get_blocks_found",43);
        memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
        error_message.total++;
        print_error_message(current_date_and_time,current_UTC_date_and_time,buffer);  
        exit(0);
      }
    }
  }
  database_data.document_count = 0;
  database_data.database_fields_count = 0;

  if (read_multiple_documents_all_fields_from_collection(DATABASE_NAME_DELEGATES,DATABASE_COLLECTION,"",&database_data,1,document_count,0,"",0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_BLOCKS_FOUND_ERROR(0);
  }
  
  if (create_json_data_from_database_multiple_documents_array(&database_data,message,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_BLOCKS_FOUND_ERROR(0);
  } 
  
  if (send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),200,"application/json") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_BLOCKS_FOUND_ERROR(0);
  }
  
  pointer_reset(message);
  pointer_reset_database_array; 
  return 1;

  #undef DATABASE_COLLECTION
  #undef SERVER_RECEIVE_DATA_SOCKET_GET_BLOCKS_FOUND_ERROR
  #undef pointer_reset_database_array
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_get_public_address_information
Description: Runs the code when the server receives /getpublicaddressinformation
Parameters:
  CLIENT_SOCKET - The socket to send data to
  DATA - The data
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_get_public_address_information(const int CLIENT_SOCKET, const char* DATA)
{
  // Variables
  char data2[BUFFER_SIZE];
  char message[BUFFER_SIZE]; 
  time_t current_date_and_time;
  struct tm* current_UTC_date_and_time;
  int count = 0;
  struct database_document_fields database_data;

  // define macros
  #define DATABASE_COLLECTION "public_addresses"
  #define TOTAL_PUBLIC_ADDRESSES_DATABASE_FIELDS 4

  #define SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_INFORMATION_ERROR(settings) \
  memset(message,0,strnlen(message,MAXIMUM_BUFFER_SIZE)); \
  memcpy(message,"{\"Error\":\"Could not get the public addresses information\"}",58); \
  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),400,"application/json"); \
  if (settings == 0) \
  { \
    pointer_reset_database_array; \
  } \
  return 0;
  
  #define pointer_reset_database_array \
  for (count = 0; count < TOTAL_PUBLIC_ADDRESSES_DATABASE_FIELDS; count++) \
  { \
    pointer_reset(database_data.item[count]); \
    pointer_reset(database_data.value[count]); \
  }

  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));

  // get the parameter1
  memcpy(data2,&DATA[48],(strnlen(DATA,sizeof(data2)) - strnlen(strstr(DATA," HTTP/"),sizeof(data2)))-48);

  // error check
  if (strncmp(data2,"",BUFFER_SIZE) == 0 || memcmp(data2,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || strnlen(data2,BUFFER_SIZE) != XCASH_WALLET_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_INFORMATION_ERROR(1);
  } 
  
  // check if the data is a public address or a delegate name
  memcpy(message,"{\"",2);
  memcpy(message+2,"public_address\":\"",17);
  memcpy(message+19,data2,XCASH_WALLET_LENGTH);
  memcpy(message+19+XCASH_WALLET_LENGTH,"\"}",2);
  
  // check if there is any data in the database that matches the message
  if (count_documents_in_collection(DATABASE_NAME_DELEGATES,DATABASE_COLLECTION,message,0) <= 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_INFORMATION_ERROR(1);
  }
  
  // initialize the database_document_fields struct 
  for (count = 0; count < TOTAL_PUBLIC_ADDRESSES_DATABASE_FIELDS; count++)
  {
    database_data.item[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));
    database_data.value[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));

    if (database_data.item[count] == NULL || database_data.value[count] == NULL)
    {
      memcpy(error_message.function[error_message.total],"server_receive_data_socket_get_public_address_information",57);
      memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
      error_message.total++;
      print_error_message(current_date_and_time,current_UTC_date_and_time,data2);  
      exit(0);
    }
  }
  database_data.count = 0;
  
  if (read_document_all_fields_from_collection(DATABASE_NAME_DELEGATES,DATABASE_COLLECTION,message,&database_data,0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_INFORMATION_ERROR(0);
  }
  
  memset(message,0,strlen(message));

  // create a json string out of the database array of item and value
  if (create_json_data_from_database_document_array(&database_data,message,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_INFORMATION_ERROR(0);
  }

  if (send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),200,"application/json") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_INFORMATION_ERROR(0);
  }
  
  pointer_reset_database_array;
  return 1;

  #undef DATABASE_COLLECTION
  #undef TOTAL_PUBLIC_ADDRESSES_DATABASE_FIELDS
  #undef SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_INFORMATION_ERROR
  #undef pointer_reset_database_array
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_get_public_address_payment_information
Description: Runs the code when the server receives /getpublicaddressinformation
Parameters:
  CLIENT_SOCKET - The socket to send data to
  DATA - The data
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_get_public_address_payment_information(const int CLIENT_SOCKET, const char* DATA)
{
  // Variables
  char data2[BUFFER_SIZE];
  char* message = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));  
  time_t current_date_and_time;
  struct tm* current_UTC_date_and_time;
  int count = 0;
  int counter = 0;
  struct database_multiple_documents_fields database_data;
  int document_count = 0;

  // define macros
  #define DATABASE_COLLECTION "public_addresses_payments"
  #define TOTAL_PUBLIC_ADDRESSES_PAYMENTS_DATABASE_FIELDS 5

  #define SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_ERROR(settings) \
  memset(message,0,strnlen(message,MAXIMUM_BUFFER_SIZE)); \
  memcpy(message,"{\"Error\":\"Could not get the public addresses information\"}",58); \
  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),400,"application/json"); \
  pointer_reset(message); \
  if (settings == 0) \
  { \
    pointer_reset_database_array; \
  } \
  return 0;
  
  #define pointer_reset_database_array \
  for (count = 0; count < document_count; count++) \
  { \
    for (counter = 0; counter < TOTAL_PUBLIC_ADDRESSES_PAYMENTS_DATABASE_FIELDS; counter++) \
    { \
      pointer_reset(database_data.item[count][counter]); \
      pointer_reset(database_data.value[count][counter]); \
    } \
  }

  // check if the memory needed was allocated on the heap successfully
  if (message == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  memset(data2,0,sizeof(data2));

  // get the parameter1
  memcpy(data2,&DATA[55],(strnlen(DATA,sizeof(data2)) - strnlen(strstr(DATA," HTTP/"),sizeof(data2)))-55);

  // error check
  if (strncmp(data2,"",BUFFER_SIZE) == 0 || memcmp(data2,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || strnlen(data2,BUFFER_SIZE) != XCASH_WALLET_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_ERROR(1);
  }
  
  // check if the data is a public address or a delegate name
  memcpy(message,"{\"",2);
  memcpy(message+2,"public_address\":\"",17);
  memcpy(message+19,data2,XCASH_WALLET_LENGTH);
  memcpy(message+19+XCASH_WALLET_LENGTH,"\"}",2);
  
  // check if there is any data in the database that matches the message
  document_count = count_documents_in_collection(DATABASE_NAME_DELEGATES,DATABASE_COLLECTION,message,0);
  if (document_count <= 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_ERROR(1);
  }
  
  // initialize the database_data struct 
  for (count = 0; count < document_count; count++)
  {
    for (counter = 0; counter < TOTAL_PUBLIC_ADDRESSES_PAYMENTS_DATABASE_FIELDS; counter++)
    {
      database_data.item[count][counter] = (char*)calloc(100,sizeof(char));
      database_data.value[count][counter] = (char*)calloc(100,sizeof(char));

      if (database_data.item[count][counter] == NULL || database_data.value[count][counter] == NULL)
      {
        memcpy(error_message.function[error_message.total],"server_receive_data_socket_get_public_address_payment_information",65);
        memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
        error_message.total++;
        print_error_message(current_date_and_time,current_UTC_date_and_time,data2);  
        exit(0);
      }
    }
  }
  database_data.document_count = 0;
  database_data.database_fields_count = 0;
  
  if (read_multiple_documents_all_fields_from_collection(DATABASE_NAME_DELEGATES,DATABASE_COLLECTION,"",&database_data,1,document_count,0,"",0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_ERROR(0);
  }
  
  memset(message,0,strlen(message));

  // create a json string out of the database array of item and value
  if (create_json_data_from_database_multiple_documents_array(&database_data,message,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_ERROR(0);
  }

  if (send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),200,"application/json") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_ERROR(0);
  }
  
  pointer_reset_database_array;
  pointer_reset(message);
  return 1;

  #undef DATABASE_COLLECTION
  #undef TOTAL_PUBLIC_ADDRESSES_PAYMENTS_DATABASE_FIELDS
  #undef SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_ERROR
  #undef pointer_reset_database_array
}