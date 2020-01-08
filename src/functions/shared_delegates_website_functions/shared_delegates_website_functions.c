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
#include "initialize_and_reset_structs_define_macros.h"

#include "blockchain_functions.h"
#include "block_verifiers_synchronize_functions.h"
#include "read_database_functions.h"
#include "count_database_functions.h"
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
  struct tm current_UTC_date_and_time;
  size_t count;
  size_t count2;
  size_t counter;
  size_t number;
  int total_blocks_found;
  int total_payments;
  int total_voters;
  int total_votes;
  int current_delegate_rank;
  long long int block_reward_number;
  long long int total_xcash_from_found_blocks;
  struct delegates delegates[MAXIMUM_AMOUNT_OF_DELEGATES];
  struct database_multiple_documents_fields database_multiple_documents_fields;
  struct database_multiple_documents_fields database_multiple_documents_fields2;
  int document_count;  
  int delegates_count;

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define SERVER_RECEIVE_DATA_SOCKET_SHARED_DELEGATES_WEBSITE_GET_STATISTICS_ERROR(settings) \
  network_functions_test_settings = 1; \
  memset(message,0,strnlen(message,MAXIMUM_BUFFER_SIZE)); \
  memcpy(message,"{\"Error\":\"Could not get the delegates statistics\"}",50); \
  if ((settings) == 0) \
  { \
    pointer_reset_database_array; \
  } \
  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),400,"application/json"); \
  return 0;

  #define pointer_reset_database_array \
  for (count = 0; count < MAXIMUM_AMOUNT_OF_DELEGATES; count++) \
  { \
    pointer_reset(delegates[count].public_address); \
    pointer_reset(delegates[count].total_vote_count); \
    pointer_reset(delegates[count].IP_address); \
    pointer_reset(delegates[count].delegate_name); \
    pointer_reset(delegates[count].about); \
    pointer_reset(delegates[count].website); \
    pointer_reset(delegates[count].team); \
    pointer_reset(delegates[count].pool_mode); \
    pointer_reset(delegates[count].fee_structure); \
    pointer_reset(delegates[count].server_settings); \
    pointer_reset(delegates[count].block_verifier_score); \
    pointer_reset(delegates[count].online_status); \
    pointer_reset(delegates[count].block_verifier_total_rounds); \
    pointer_reset(delegates[count].block_verifier_online_total_rounds); \
    pointer_reset(delegates[count].block_verifier_online_percentage); \
    pointer_reset(delegates[count].block_producer_total_rounds); \
    pointer_reset(delegates[count].block_producer_block_heights); \
  } \
  for (count = 0; (int)count < document_count; count++) \
  { \
    for (counter = 0; counter < TOTAL_BLOCKS_FOUND_DATABASE_FIELDS; counter++) \
    { \
      pointer_reset(database_multiple_documents_fields.item[count][counter]); \
      pointer_reset(database_multiple_documents_fields.value[count][counter]); \
    } \
  } \
  for (count = 0; count < MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE; count++) \
  { \
    for (counter = 0; counter < TOTAL_RESERVE_PROOFS_DATABASE_FIELDS; counter++) \
    { \
      pointer_reset(database_multiple_documents_fields2.item[count][counter]); \
      pointer_reset(database_multiple_documents_fields2.value[count][counter]); \
    } \
  }

  memset(data,0,sizeof(data));
  memset(message,0,sizeof(message));

  // get the total blocks found
  document_count = count_all_documents_in_collection(shared_delegates_database_name,"blocks_found",1);
  total_blocks_found = document_count;
  if (document_count <= 0)
  {
    total_blocks_found = 0;
    total_xcash_from_found_blocks = 0;
    total_payments = 0; 
  }

  // initialize the delegates struct
  INITIALIZE_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES,"server_receive_data_socket_shared_delegates_website_get_statistics",data,current_date_and_time,current_UTC_date_and_time);

  // initialize the database_multiple_documents_fields struct 
  if (total_blocks_found != 0)
  {
    INITIALIZE_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,document_count,TOTAL_BLOCKS_FOUND_DATABASE_FIELDS,"server_receive_data_socket_shared_delegates_website_get_statistics",data,current_date_and_time,current_UTC_date_and_time);
  }
  // initialize the database_multiple_documents_fields struct 
  for (count = 0; count < MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE; count++)
  {
    for (counter = 0; counter < TOTAL_RESERVE_PROOFS_DATABASE_FIELDS; counter++)
    {
      database_multiple_documents_fields2.item[count][counter] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));
      database_multiple_documents_fields2.value[count][counter] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));

      if (database_multiple_documents_fields2.item[count][counter] == NULL || database_multiple_documents_fields2.value[count][counter] == NULL)
      {
        memcpy(error_message.function[error_message.total],"server_receive_data_socket_shared_delegates_website_get_statistics",66);
        memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
        error_message.total++;
        print_error_message(current_date_and_time,current_UTC_date_and_time,data);  
        exit(0);
      }
    }
  }
  database_multiple_documents_fields2.document_count = 0;
  database_multiple_documents_fields2.database_fields_count = 0;

  // organize the delegates
  delegates_count = organize_delegates(delegates,DATABASE_COLLECTION);

  // get the current_delegate_rank
  for (count = 0; (int)count < delegates_count; count++)
  {
    if (memcmp(delegates[count].public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0)
    {
      current_delegate_rank = count+1;
      break;
    }
  }

  if (total_blocks_found != 0)
  {
    if (read_multiple_documents_all_fields_from_collection(shared_delegates_database_name,"blocks_found","",&database_multiple_documents_fields,1,document_count,0,"",1) == 0)
    {
      SERVER_RECEIVE_DATA_SOCKET_SHARED_DELEGATES_WEBSITE_GET_STATISTICS_ERROR(0);
    }

    // get the total xcash from found blocks
    for (total_xcash_from_found_blocks = 0, count = 0; (int)count < document_count; count++)
    {
      sscanf(database_multiple_documents_fields.value[count][3], "%lld", &block_reward_number);
      total_xcash_from_found_blocks += block_reward_number;
    }

    // add the total payments to the database_document_fields struct 
    total_payments = count_all_documents_in_collection(shared_delegates_database_name,"public_addresses_payments",1);
  }

  // get the total voters and total votes
  memcpy(message,"{\"public_address_voted_for\":\"",29);
  memcpy(message+29,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(message+127,"\"}",2);

  // check how many reserve proofs are for the public address
  for (total_voters = 0, total_votes = 0, count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
  { 
    memset(data,0,strlen(data));
    memcpy(data,"reserve_proofs_",15);
    snprintf(data+15,sizeof(data)-16,"%zu",count);

    counter = count_documents_in_collection(database_name,data,message,1);
    total_voters += counter;

    if (read_multiple_documents_all_fields_from_collection(database_name,data,message,&database_multiple_documents_fields2,1,counter,0,"",1) == 1)
    {
      for (count2 = 0; count2 < counter; count2++)
      {
        sscanf(database_multiple_documents_fields2.value[count2][2], "%zu", &number);
        total_votes += number;
      }
    }

    // reset the database_multiple_documents_fields struct
    for (count2 = 0; count2 < counter; count2++)
    {
      for (number = 0; number < TOTAL_RESERVE_PROOFS_DATABASE_FIELDS; number++)
      {
        memset(database_multiple_documents_fields2.item[count2][number],0,strlen(database_multiple_documents_fields2.item[count2][number]));
        memset(database_multiple_documents_fields2.value[count2][number],0,strlen(database_multiple_documents_fields2.value[count2][number]));
      }
    }
  }

  // get the block_verifier_online_percentage
  memset(data,0,sizeof(data));
  memset(message,0,sizeof(message));
  memcpy(message,"{\"public_address\":\"",19);
  memcpy(message+19,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(message+117,"\"}",2);

  if (read_document_field_from_collection(database_name,"delegates",message,"block_verifier_online_percentage",data,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_SHARED_DELEGATES_WEBSITE_GET_STATISTICS_ERROR(0);
  } 
  
  memset(message,0,sizeof(message));

  memcpy(message,"{\"public_address\":\"",19);
  memcpy(message+strlen(message),xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(message+strlen(message),"\",\"current_delegate_rank\":\"",27);
  snprintf(message+strlen(message),sizeof(message),"%d",current_delegate_rank);
  memcpy(message+strlen(message),"\",\"total_votes\":\"",17);
  snprintf(message+strlen(message),sizeof(message),"%d",total_votes);
  memcpy(message+strlen(message),"\",\"online_percentage\":\"",23);
  memcpy(message+strlen(message),data,strnlen(data,sizeof(message)));
  memcpy(message+strlen(message),"\",\"total_blocks_found\":\"",24);
  snprintf(message+strlen(message),sizeof(message),"%d",total_blocks_found);
  memcpy(message+strlen(message),"\",\"total_xcash_from_blocks_found\":\"",35);
  snprintf(message+strlen(message),sizeof(message),"%lld",total_xcash_from_found_blocks);
  memcpy(message+strlen(message),"\",\"total_payments\":\"",20);
  snprintf(message+strlen(message),sizeof(message),"%d",total_payments);
  memcpy(message+strlen(message),"\",\"total_voters\":\"",18);
  snprintf(message+strlen(message),sizeof(message),"%d",total_voters);
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

  #undef DATABASE_COLLECTION
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
  struct tm current_UTC_date_and_time;
  int count = 0;
  int counter = 0;
  struct database_multiple_documents_fields database_multiple_documents_fields;
  int document_count = 0;

  // define macros
  #define DATABASE_COLLECTION "blocks_found"
  #define TOTAL_BLOCKS_FOUND_DATABASE_FIELDS 5

  #define SERVER_RECEIVE_DATA_SOCKET_GET_BLOCKS_FOUND_ERROR(settings) \
  network_functions_test_settings = 1; \
  memset(message,0,strnlen(message,MAXIMUM_BUFFER_SIZE)); \
  memcpy(message,"{\"Error\":\"Could not get the delegates blocks found\"}",52); \
  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),400,"application/json"); \
  pointer_reset(message); \
  if ((settings) == 0) \
  { \
    POINTER_RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,TOTAL_BLOCKS_FOUND_DATABASE_FIELDS); \
  } \
  return 0;

  // get the total blocks found
  document_count = count_all_documents_in_collection(shared_delegates_database_name,DATABASE_COLLECTION,1);

  if (document_count <= 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_BLOCKS_FOUND_ERROR(1);
  }

  memset(buffer,0,sizeof(buffer));

  // initialize the database_multiple_documents_fields struct
  INITIALIZE_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,document_count,TOTAL_BLOCKS_FOUND_DATABASE_FIELDS,"server_receive_data_socket_get_blocks_found",buffer,current_date_and_time,current_UTC_date_and_time);

  if (read_multiple_documents_all_fields_from_collection(shared_delegates_database_name,DATABASE_COLLECTION,"",&database_multiple_documents_fields,1,document_count,0,"",1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_BLOCKS_FOUND_ERROR(0);
  }
  
  if (create_json_data_from_database_multiple_documents_array(&database_multiple_documents_fields,message,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_BLOCKS_FOUND_ERROR(0);
  } 
  
  if (send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),200,"application/json") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_BLOCKS_FOUND_ERROR(0);
  }
  
  pointer_reset(message);
  POINTER_RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,TOTAL_BLOCKS_FOUND_DATABASE_FIELDS);
  return 1;

  #undef DATABASE_COLLECTION
  #undef SERVER_RECEIVE_DATA_SOCKET_GET_BLOCKS_FOUND_ERROR
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
  struct tm current_UTC_date_and_time;
  int count = 0;
  struct database_document_fields database_data;

  // define macros
  #define DATABASE_COLLECTION "public_addresses"
  #define TOTAL_PUBLIC_ADDRESSES_DATABASE_FIELDS 4

  #define SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_INFORMATION_ERROR(settings) \
  network_functions_test_settings = 1; \
  memset(message,0,strnlen(message,MAXIMUM_BUFFER_SIZE)); \
  memcpy(message,"{\"Error\":\"Could not get the public addresses information\"}",58); \
  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),400,"application/json"); \
  if ((settings) == 0) \
  { \
    POINTER_RESET_DATABASE_DOCUMENT_FIELDS_STRUCT(count); \
  } \
  return 0;

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
  if (count_documents_in_collection(shared_delegates_database_name,DATABASE_COLLECTION,message,0) <= 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_INFORMATION_ERROR(1);
  }

  // initialize the database_document_fields struct 
  INITIALIZE_DATABASE_DOCUMENT_FIELDS_STRUCT(count,TOTAL_PUBLIC_ADDRESSES_DATABASE_FIELDS,"server_receive_data_socket_get_public_address_information",data2,current_date_and_time,current_UTC_date_and_time);
   
  if (read_document_all_fields_from_collection(shared_delegates_database_name,DATABASE_COLLECTION,message,&database_data,1) == 0)
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
  
  POINTER_RESET_DATABASE_DOCUMENT_FIELDS_STRUCT(count);
  return 1;

  #undef DATABASE_COLLECTION
  #undef TOTAL_PUBLIC_ADDRESSES_DATABASE_FIELDS
  #undef SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_INFORMATION_ERROR
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
  struct tm current_UTC_date_and_time;
  int count = 0;
  int counter = 0;
  struct database_multiple_documents_fields database_multiple_documents_fields;
  int document_count = 0;

  // define macros
  #define DATABASE_COLLECTION "public_addresses_payments"
  #define TOTAL_PUBLIC_ADDRESSES_PAYMENTS_DATABASE_FIELDS 5

  #define SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_ERROR(settings) \
  network_functions_test_settings = 1; \
  memset(message,0,strnlen(message,MAXIMUM_BUFFER_SIZE)); \
  memcpy(message,"{\"Error\":\"Could not get the public addresses information\"}",58); \
  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),400,"application/json"); \
  pointer_reset(message); \
  if ((settings) == 0) \
  { \
    POINTER_RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,TOTAL_PUBLIC_ADDRESSES_PAYMENTS_DATABASE_FIELDS); \
  } \
  return 0;

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
  document_count = count_documents_in_collection(shared_delegates_database_name,DATABASE_COLLECTION,message,1);
  if (document_count <= 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_ERROR(1);
  }
  
  // initialize the database_multiple_documents_fields struct
  INITIALIZE_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,document_count,TOTAL_PUBLIC_ADDRESSES_PAYMENTS_DATABASE_FIELDS,"server_receive_data_socket_get_public_address_payment_information",data2,current_date_and_time,current_UTC_date_and_time);
  
  if (read_multiple_documents_all_fields_from_collection(shared_delegates_database_name,DATABASE_COLLECTION,"",&database_multiple_documents_fields,1,document_count,0,"",1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_ERROR(0);
  }
  
  memset(message,0,strlen(message));

  // create a json string out of the database array of item and value
  if (create_json_data_from_database_multiple_documents_array(&database_multiple_documents_fields,message,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_ERROR(0);
  }

  if (send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),200,"application/json") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_ERROR(0);
  }
  
  POINTER_RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,TOTAL_PUBLIC_ADDRESSES_PAYMENTS_DATABASE_FIELDS);
  pointer_reset(message);
  return 1;

  #undef DATABASE_COLLECTION
  #undef TOTAL_PUBLIC_ADDRESSES_PAYMENTS_DATABASE_FIELDS
  #undef SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_ERROR
}