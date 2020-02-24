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
#include "define_macros_test.h"

#include "blockchain_functions.h"
#include "block_verifiers_synchronize_functions.h"
#include "read_database_functions.h"
#include "count_database_functions.h"
#include "delegates_website_functions.h"
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
Name: server_receive_data_socket_delegates_website_get_statistics
Description: Runs the code when the server receives /delegateswebsitegetstatistics
Parameters:
  CLIENT_SOCKET - The socket to send data to
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_delegates_website_get_statistics(const int CLIENT_SOCKET)
{
  // Variables
  char data[BUFFER_SIZE];
  char buffer[1024];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  size_t count = 0;
  size_t count2 = 0;
  size_t number = 0;
  size_t block_height = 0;
  double generated_supply;
  struct delegates delegates[MAXIMUM_AMOUNT_OF_DELEGATES];
  struct database_document_fields database_data;
  int document_count = 0;

  // define macros
  #define DATABASE_FIELDS "username|"
  #define DATA "{\"username\":\"XCASH\"}"

  #define SERVER_RECEIVE_DATA_SOCKET_GET_STATISTICS_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_delegates_website_get_statistics",59); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  memset(data,0,strlen(data)); \
  memcpy(data,"{\"Error\":\"Could not get statistics\"}",36); \
  send_data(CLIENT_SOCKET,(unsigned char*)data,strlen(data),400,"application/json"); \
  POINTER_RESET_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES); \
  POINTER_RESET_DATABASE_DOCUMENT_FIELDS_STRUCT(count); \
  return 400;

  memset(buffer,0,sizeof(buffer));
  memset(data,0,sizeof(data));

  // initialize the delegates struct
  INITIALIZE_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES,"server_receive_data_socket_delegates_website_get_statistics",buffer,current_date_and_time,current_UTC_date_and_time);

  // initialize the database_document_fields struct 
  INITIALIZE_DATABASE_DOCUMENT_FIELDS_STRUCT(count,TOTAL_STATISTICS_DATABASE_FIELDS+4,"server_receive_data_socket_delegates_website_get_statistics",buffer,current_date_and_time,current_UTC_date_and_time);

  // check if there is any data in the database that matches the message
  if (count_documents_in_collection(database_name,"statistics",DATA,1) <= 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_STATISTICS_ERROR("Could not read the statistics database collection");
  }

  // organize the delegates
  document_count = organize_delegates(delegates,"delegates");

  if (read_document_all_fields_from_collection(database_name,"statistics",DATA,&database_data,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_STATISTICS_ERROR("Could not read all of the fields of the statistics database collection");
  }

  // get the current block height
  if (test_settings == 0)
  {
    if (get_current_block_height(data) == 0)
    {
      SERVER_RECEIVE_DATA_SOCKET_GET_STATISTICS_ERROR("Could not get the current block height");
    }
  }
  else
  {
    memcpy(data,"521850",6);
  }
  
  memcpy(database_data.item[database_data.count],"current_block_height",20);
  memcpy(database_data.value[database_data.count],data,strnlen(data,BUFFER_SIZE));
  database_data.count++;

  // get the xcash proof of stake round number
  sscanf(data,"%zu", &block_height);
  count = block_height - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT;  
  memset(data,0,strlen(data));
  snprintf(data,sizeof(data)-1,"%zu",count);

  memcpy(database_data.item[database_data.count],"XCASH_DPOPS_round_number",24);
  memcpy(database_data.value[database_data.count],data,strnlen(data,BUFFER_SIZE));
  database_data.count++;

  // get the total votes
  for (count = 0, count2 = 0; (int)count < document_count; count++)
  {
    sscanf(delegates[count].total_vote_count,"%zu", &number);
    count2 += number;
  }

  memset(data,0,strlen(data));
  snprintf(data,sizeof(data)-1,"%zu",count2);

  memcpy(database_data.item[database_data.count],"total_votes",11);
  memcpy(database_data.value[database_data.count],data,strnlen(data,BUFFER_SIZE));
  database_data.count++;

  // get the XCASH_DPOPS_circulating_percentage
  generated_supply = get_generated_supply((const size_t)block_height);
  number = ((size_t)((((double)count2) / ((generated_supply - (XCASH_PREMINE_TOTAL_SUPPLY - XCASH_PREMINE_CIRCULATING_SUPPLY)) * XCASH_WALLET_DECIMAL_PLACES_AMOUNT)) * 100) | 0);

  memset(data,0,strlen(data));
  snprintf(data,sizeof(data)-1,"%zu",number);

  memcpy(database_data.item[database_data.count],"XCASH_DPOPS_circulating_percentage",34);
  memcpy(database_data.value[database_data.count],data,strnlen(data,BUFFER_SIZE));
  database_data.count++;
  
  memset(data,0,strlen(data));

  // create a json string out of the database array of item and value
  if (create_json_data_from_database_document_array(&database_data,data,DATABASE_FIELDS) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_STATISTICS_ERROR("Could not create json data");
  }
  send_data(CLIENT_SOCKET,(unsigned char*)data,strlen(data),200,"application/json");

  POINTER_RESET_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES);
  POINTER_RESET_DATABASE_DOCUMENT_FIELDS_STRUCT(count);
  network_functions_test_settings = 0; 
  return 200;

  #undef DATABASE_FIELDS
  #undef DATA
  #undef SERVER_RECEIVE_DATA_SOCKET_GET_STATISTICS_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_get_delegates
Description: Runs the code when the server receives /getdelegates
Parameters:
  CLIENT_SOCKET - The socket to send data to
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_get_delegates(const int CLIENT_SOCKET)
{
  // Variables
  char* message = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char buffer[1024];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  struct delegates delegates[MAXIMUM_AMOUNT_OF_DELEGATES];
  int count = 0;

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define DATABASE_FIELDS "public_address|IP_address|public_key|about|website|team|server_settings|block_verifier_online_total_rounds|block_producer_total_rounds|block_producer_block_heights|"

  #define SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_get_delegates",40); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  memset(message,0,strlen(message)); \
  memcpy(message,"{\"Error\":\"Could not get the delegates information\"}",51); \
  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),400,"application/json"); \
  pointer_reset(message); \
  POINTER_RESET_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES); \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (message == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  memset(buffer,0,sizeof(buffer));

  // initialize the delegates struct
  INITIALIZE_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES,"server_receive_data_socket_get_delegates",buffer,current_date_and_time,current_UTC_date_and_time);
  
  // organize the delegates
  if (organize_delegates(delegates,DATABASE_COLLECTION) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_ERROR("Could not organize the delegates");
  }
  
  if (create_json_data_from_delegates_array(delegates,message,DATABASE_FIELDS) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_ERROR("Could not create json data");
  }
  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),200,"application/json");

  pointer_reset(message);
  POINTER_RESET_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES);
  return 1;

  #undef DATABASE_COLLECTION
  #undef DATABASE_FIELDS
  #undef SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_get_delegates_statistics
Description: Runs the code when the server receives /getdelegatesstatistics
Parameters:
  CLIENT_SOCKET - The socket to send data to
  DATA - The data
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_get_delegates_statistics(const int CLIENT_SOCKET, const char* DATA)
{
  // Variables
  char message[BUFFER_SIZE];
  char buffer[1024];
  char data2[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count = 0;
  int count2 = 0;
  struct database_document_fields database_data;
  struct delegates delegates[MAXIMUM_AMOUNT_OF_DELEGATES];
  int document_count = 0;

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define DATABASE_FIELDS "IP_address|public_key|"

  #define SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_STATISTICS_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_get_delegates_statistics",51); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  memset(message,0,strlen(message)); \
  memcpy(message,"{\"Error\":\"Could not get the delegates statistics\"}",50); \
  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),400,"application/json"); \
  POINTER_RESET_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES); \
  POINTER_RESET_DATABASE_DOCUMENT_FIELDS_STRUCT(count); \
  return 0;

  memset(buffer,0,sizeof(buffer));
  memset(message,0,sizeof(message));

  // initialize the delegates struct
  INITIALIZE_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES,"server_receive_data_socket_get_delegates_statistics",buffer,current_date_and_time,current_UTC_date_and_time);

  // initialize the database_document_fields struct 
  INITIALIZE_DATABASE_DOCUMENT_FIELDS_STRUCT(count,TOTAL_DELEGATES_DATABASE_FIELDS+1,"server_receive_data_socket_get_delegates_statistics",buffer,current_date_and_time,current_UTC_date_and_time);

  memset(data2,0,sizeof(data2));

  // get the parameter1
  memcpy(data2,&DATA[39],(strnlen(DATA,sizeof(data2)) - strnlen(strstr(DATA," HTTP/"),sizeof(data2)))-39);

  // error check
  if (strncmp(data2,"",BUFFER_SIZE) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_STATISTICS_ERROR("Invalid parameters");
  } 

  const size_t DATA_LENGTH = strnlen(data2,BUFFER_SIZE);

  // check if the data is a public address or a delegate name
  memcpy(message,"{\"",2);
  if (memcmp(data2,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) == 0 && DATA_LENGTH == XCASH_WALLET_LENGTH)
  {
    memcpy(message+2,"public_address\":\"",17);
    memcpy(message+19,data2,DATA_LENGTH);
    memcpy(message+19+DATA_LENGTH,"\"}",2);
  }
  else
  {
    memcpy(message+2,"delegate_name\":\"",16);
    memcpy(message+18,data2,DATA_LENGTH);
    memcpy(message+18+DATA_LENGTH,"\"}",2);
  }

  // check if there is any data in the database that matches the message
  if (count_documents_in_collection(database_name,DATABASE_COLLECTION,message,1) <= 0 || read_document_all_fields_from_collection(database_name,DATABASE_COLLECTION,message,&database_data,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_STATISTICS_ERROR("Could not get the data from the delegates database collection");
  }

  // organize the delegates
  document_count = organize_delegates(delegates,DATABASE_COLLECTION);
  if (document_count == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_STATISTICS_ERROR("Could not organize the delegates");
  }

  // get the delegates rank
  for (count = 0, count2 = 0; (int)count < document_count; count++)
  {
    if (strncmp(delegates[count].public_address,data2,XCASH_WALLET_LENGTH) == 0 || strncmp(delegates[count].delegate_name,data2,BUFFER_SIZE) == 0)
    {
      count2++;
      break;
    }
    count2++;
  }

  memset(data2,0,strlen(data2));
  snprintf(data2, sizeof(data2), "%d", count2);
  memcpy(database_data.item[TOTAL_DELEGATES_DATABASE_FIELDS],"current_delegate_rank",21);
  memcpy(database_data.value[TOTAL_DELEGATES_DATABASE_FIELDS],data2,strnlen(data2,BUFFER_SIZE));
  database_data.count++;  

  memset(message,0,strlen(message)); 

  // create a json string out of the database array of item and value
  if (create_json_data_from_database_document_array(&database_data,message,DATABASE_FIELDS) == 0)
  {    
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_STATISTICS_ERROR("Could not create json data");
  }
  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),200,"application/json");
  POINTER_RESET_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES);
  POINTER_RESET_DATABASE_DOCUMENT_FIELDS_STRUCT(count);
  return 1;

  #undef DATABASE_COLLECTION
  #undef DATABASE_FIELDS
  #undef SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_STATISTICS_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_get_delegates_information
Description: Runs the code when the server receives /getdelegatesinformation
Parameters:
  CLIENT_SOCKET - The socket to send data to
  DATA - The data
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_get_delegates_information(const int CLIENT_SOCKET, const char* DATA)
{
  // Variables
  char data2[BUFFER_SIZE];
  char message[BUFFER_SIZE];
  char buffer[1024];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count = 0;
  struct database_document_fields database_data;

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define DATABASE_FIELDS "total_vote_count|IP_address|delegate_name|public_key|block_verifier_score|online_status|block_verifier_total_rounds|block_verifier_online_total_rounds|block_verifier_online_percentage|block_producer_total_rounds|VRF_node_public_and_secret_key_total_rounds|VRF_node_random_data_total_rounds|VRF_node_next_main_nodes_total_rounds|block_producer_block_heights|VRF_node_public_and_secret_key_block_heights|VRF_node_random_data_block_heights|VRF_node_next_main_nodes_block_heights|"

  #define SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_INFORMATION_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_get_delegates_information",52); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  memset(message,0,strlen(message)); \
  memcpy(message,"{\"Error\":\"Could not get the delegates information\"}",51); \
  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),400,"application/json"); \
  POINTER_RESET_DATABASE_DOCUMENT_FIELDS_STRUCT(count); \
  return 0;

  memset(buffer,0,sizeof(buffer));
  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));

  // initialize the database_document_fields struct 
  INITIALIZE_DATABASE_DOCUMENT_FIELDS_STRUCT(count,TOTAL_DELEGATES_DATABASE_FIELDS,"server_receive_data_socket_get_delegates_information",buffer,current_date_and_time,current_UTC_date_and_time);

  // get the parameter1
  memcpy(data2,&DATA[40],(strnlen(DATA,sizeof(data2)) - strnlen(strstr(DATA," HTTP/"),sizeof(data2)))-40);

  // error check
  if (strncmp(data2,"",BUFFER_SIZE) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_INFORMATION_ERROR("Invalid parameters");
  } 

  const size_t DATA_LENGTH = strnlen(data2,BUFFER_SIZE);
  
  // check if the data is a public address or a delegate name
  memcpy(message,"{\"",2);
  if (memcmp(data2,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) == 0 && DATA_LENGTH == XCASH_WALLET_LENGTH)
  {
    memcpy(message+2,"public_address\":\"",17);
    memcpy(message+19,data2,DATA_LENGTH);
    memcpy(message+19+DATA_LENGTH,"\"}",2);
  }
  else
  {
    memcpy(message+2,"delegate_name\":\"",16);
    memcpy(message+18,data2,DATA_LENGTH);
    memcpy(message+18+DATA_LENGTH,"\"}",2);
  }
  
  // check if there is any data in the database that matches the message
  if (count_documents_in_collection(database_name,DATABASE_COLLECTION,message,1) <= 0 || read_document_all_fields_from_collection(database_name,DATABASE_COLLECTION,message,&database_data,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_INFORMATION_ERROR("Could not get the delegates information");
  }
  
  memset(message,0,strlen(message));

  // create a json string out of the database array of item and value
  if (create_json_data_from_database_document_array(&database_data,message,DATABASE_FIELDS) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_INFORMATION_ERROR("Could not create json data");
  }
  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),200,"application/json");
  
  POINTER_RESET_DATABASE_DOCUMENT_FIELDS_STRUCT(count);
  return 1;

  #undef DATABASE_COLLECTION
  #undef DATABASE_FIELDS
  #undef SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_INFORMATION_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_get_delegates_voters_list
Description: Runs the code when the server receives /getdelegatesvoterslist
Parameters:
  CLIENT_SOCKET - The socket to send data to
  DATA - The data
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_get_delegates_voters_list(const int CLIENT_SOCKET, const char* DATA)
{
  // Variables
  char data2[BUFFER_SIZE];
  char* message = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  char buffer[1024];
  int count = 0;
  int count2 = 0;
  int counter = 0;
  struct votes votes[MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE];
  struct database_multiple_documents_fields database_multiple_documents_fields;
  int document_count = 0;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_VOTERS_LIST_ERROR(settings,MESSAGE) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_get_delegates_voters_list",52); \
  memcpy(error_message.data[error_message.total],MESSAGE,sizeof(MESSAGE)-1); \
  error_message.total++; \
  memset(message,0,strlen(message)); \
  memcpy(message,"{\"Error\":\"Could not get the delegates voters list\"}",51); \
  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),400,"application/json"); \
  pointer_reset(message); \
  if ((settings) == 0) \
  { \
    POINTER_RESET_VOTES_STRUCT(count,MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE); \
    POINTER_RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,TOTAL_RESERVE_PROOFS_DATABASE_FIELDS); \
  } \
  return 0;
  
  // check if the memory needed was allocated on the heap successfully
  if (message == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  memset(buffer,0,sizeof(buffer));
  memset(data2,0,sizeof(data2));

  // get the parameter1
  memcpy(data2,&DATA[39],(strnlen(DATA,sizeof(data2)) - strnlen(strstr(DATA," HTTP/"),sizeof(data2)))-39);

  // error check
  if (strncmp(data2,"",BUFFER_SIZE) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_VOTERS_LIST_ERROR(1,"Invalid parameters");
  } 

  const size_t DATA_LENGTH = strnlen(data2,BUFFER_SIZE);

  // get the delegates public address
  if (memcmp(data2,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || DATA_LENGTH != XCASH_WALLET_LENGTH)
  {
    memcpy(message,"{\"delegate_name\":\"",18);
    memcpy(message+18,data2,DATA_LENGTH);
    memcpy(message+18+DATA_LENGTH,"\"}",2);
    memset(data2,0,strlen(data2));

    if (read_document_field_from_collection(database_name,"delegates",message,"public_address",data2,1) == 0)
    {
      SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_VOTERS_LIST_ERROR(1,"Could not find a registered delegate with the information provided");
    }
  }

  // create the message
  memset(message,0,strlen(message));
  memcpy(message,"{\"public_address_voted_for\":\"",29);
  memcpy(message+29,data2,XCASH_WALLET_LENGTH);
  memcpy(message+127,"\"}",2);

  // check how many reserve proofs are for the public address
  for (count = 1, document_count = 0; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
  { 
    memset(data2,0,strlen(data2));
    memcpy(data2,"reserve_proofs_",15);
    snprintf(data2+15,sizeof(data2)-16,"%d",count);
    counter = count_documents_in_collection(database_name,data2,message,1);
    if (counter == -1)
    {
      continue;
    }
    document_count += counter;
  }
  if (document_count == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_VOTERS_LIST_ERROR(1,"There are no reserve proofs for the delegate");
  }

  // initialize the delegates struct
  INITIALIZE_VOTES_STRUCT(count,MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE,"server_receive_data_socket_get_delegates_voters_list",buffer,current_date_and_time,current_UTC_date_and_time);

  // initialize the database_multiple_documents_fields struct
  INITIALIZE_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,document_count,TOTAL_RESERVE_PROOFS_DATABASE_FIELDS,"server_receive_data_socket_get_delegates_voters_list",buffer,current_date_and_time,current_UTC_date_and_time);

  // get all of the reserve proofs for the public address
  for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
  { 
    memset(data2,0,strlen(data2));
    memcpy(data2,"reserve_proofs_",15);
    snprintf(data2+15,sizeof(data2)-16,"%d",count);

    counter = count_documents_in_collection(database_name,data2,message,1);
    if (counter > 0)
    {      
      if (read_multiple_documents_all_fields_from_collection(database_name,data2,message,&database_multiple_documents_fields,1,counter,0,"",1) == 0)
      {
        SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_VOTERS_LIST_ERROR(0,"Could not get the delegates voters list");
      }

      // copy the database_multiple_documents_fields struct to the votes struct
      for (counter = 0; counter < (int)database_multiple_documents_fields.document_count; counter++)
      {
        memcpy(votes[counter].public_address_created_reserve_proof,database_multiple_documents_fields.value[counter][0],strnlen(database_multiple_documents_fields.value[counter][0],100));
        memcpy(votes[counter].public_address_voted_for,database_multiple_documents_fields.value[counter][1],strnlen(database_multiple_documents_fields.value[counter][1],100));
        memcpy(votes[counter].total,database_multiple_documents_fields.value[counter][2],strnlen(database_multiple_documents_fields.value[counter][2],100));
        memcpy(votes[counter].reserve_proof,database_multiple_documents_fields.value[counter][3],strnlen(database_multiple_documents_fields.value[counter][3],BUFFER_SIZE_RESERVE_PROOF+1));
      }

      // reset the database_multiple_documents_fields struct
      RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(counter,count2,TOTAL_RESERVE_PROOFS_DATABASE_FIELDS);
    }
  }
  RESET_ERROR_MESSAGES;

  memset(message,0,strlen(message)); 

  if (create_json_data_from_votes_array(votes,message,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_VOTERS_LIST_ERROR(0,"Could not create json data");
  }
  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),200,"application/json");

  POINTER_RESET_VOTES_STRUCT(count,MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE);
  POINTER_RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,TOTAL_RESERVE_PROOFS_DATABASE_FIELDS);
  pointer_reset(message);
  return 1;

  #undef SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_VOTERS_LIST_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_get_round_statistics
Description: Runs the code when the server receives /getroundstatistics
Parameters:
  CLIENT_SOCKET - The socket to send data to
  DATA - The data
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_get_round_statistics(const int CLIENT_SOCKET, const char* DATA)
{
  // Variables
  char message[BUFFER_SIZE];  
  char data2[BUFFER_SIZE];
  char data3[SMALL_BUFFER_SIZE];  
  size_t count = 0;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_GET_ROUND_STATISTICS_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_get_round_statistics",47); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  memset(message,0,strlen(message)); \
  memcpy(message,"{\"Error\":\"Could not get the round statistics\"}",46); \
  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),400,"application/json"); \
  return 0;

  memset(message,0,sizeof(message));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  // get the parameter1
  memcpy(data2,&DATA[35],(strnlen(DATA,sizeof(data2)) - strnlen(strstr(DATA," HTTP/"),sizeof(data2)))-35);

  // error check
  if (strncmp(data2,"",BUFFER_SIZE) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_ROUND_STATISTICS_ERROR("Invalid parameters");
  } 

  // calculate what reserve_bytes database the reserve bytes data will be in
  sscanf(data2,"%zu", &count);
  if (count < XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_ROUND_STATISTICS_ERROR("Invalid block height");
  }

  count = ((count - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME) + 1;
  memcpy(data3,"reserve_bytes_",14);
  snprintf(data3+14,sizeof(data3)-15,"%zu",count);

  // create the message
  memcpy(message,"{\"block_height\":\"",17);
  memcpy(message+17,data2,strnlen(data2,sizeof(message)));
  memcpy(message+strlen(message),"\"}",2);

  // check if there is any data in the database that matches the message
  if (count_documents_in_collection(database_name,data3,message,1) <= 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_ROUND_STATISTICS_ERROR("Could not get the rounds statistics");
  }

  memset(data2,0,strlen(data2));

  if (read_document_field_from_collection(database_name,data3,message,"reserve_bytes",data2,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_ROUND_STATISTICS_ERROR("Could not get the rounds statistics");
  }

  // create a json string out of the database array of item and value
  memset(message,0,strlen(message));
  memcpy(message,"{\"reserve_bytes\":\"",18);
  memcpy(message+18,data2,strnlen(data2,sizeof(message)));
  memcpy(message+strlen(message),"\"}",2);
  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),200,"application/json");
  
  return 1;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_GET_ROUND_STATISTICS_ERROR
}