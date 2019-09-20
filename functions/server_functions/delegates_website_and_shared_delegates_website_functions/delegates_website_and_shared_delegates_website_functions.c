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
Name: server_receive_data_socket_get_files
Description: gets the files
Parameters:
  CLIENT_SOCKET - The socket to send data to
  MESSAGE - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_get_files(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  unsigned char* data = (unsigned char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(unsigned char));
  char data2[BUFFER_SIZE];
  long file_size;
  int settings = 0;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_GET_FILES_ERROR \
  memcpy(data,"{\"Error\":\"Could not get the file\"}",34); \
  send_data(CLIENT_SOCKET,data,strlen(data),400,"application/json"); \
  pointer_reset(data); \
  return 0;

  memset(data2,0,sizeof(data2));

  // get the file
  if (shared_delegates_website == 1)
  {
    memcpy(data2,"shared_delegates_website/",25);
  }
  if (delegates_website == 1)
  {
    memcpy(data2,"delegates_website/",18);
  }
  memcpy(data2+strlen(data2),&MESSAGE[(strlen(MESSAGE) - strlen(strstr(MESSAGE,"GET /")))+5],(strlen(MESSAGE) - strlen(strstr(MESSAGE," HTTP/"))) - ((strlen(MESSAGE) - strlen(strstr(MESSAGE,"GET /")))+5));
  file_size = read_file(data,data2);
  if (file_size == 0)
  {
    if (shared_delegates_website == 1)
    {
      file_size = read_file(data,"shared_delegates_website/index.html");
    }
    if (delegates_website == 1)
    {
      file_size = read_file(data,"delegates_website/index.html");
    }    
    settings = 1;
    if (file_size == 0)
    {
      SERVER_RECEIVE_DATA_SOCKET_GET_FILES_ERROR;
    }
  }

  memset(data2,0,sizeof(data2));

  if (strstr(MESSAGE,".html") != NULL || settings == 1)
  {
    memcpy(data2,"text/html",9);
  }
  else if (strstr(MESSAGE,".js") != NULL)
  {
    memcpy(data2,"application/javascript",22);
  }
  else if (strstr(MESSAGE,".css") != NULL)
  {
    memcpy(data2,"text/css",8);
  }
  else if (strstr(MESSAGE,".png") != NULL)
  {
    memcpy(data2,"image/png",9);
  }
  else if (strstr(MESSAGE,".jpg") != NULL || strstr(MESSAGE,".jpeg") != NULL)
  {
    memcpy(data2,"image/jpeg",10);
  }
  else
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_FILES_ERROR;
  }
  

  if (send_data(CLIENT_SOCKET,data,(const long)file_size,200,data2) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_FILES_ERROR;
  }

  pointer_reset(data);
  return 1;

  #undef SERVER_RECEIVE_DATA_SOCKET_GET_FILES_ERROR
}



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
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char)); 
  size_t count = 0;
  size_t count2 = 0;
  size_t counter = 0;
  size_t number = 0;
  size_t current_block_height = 0;
  double generated_supply;
  struct database_document_fields database_data;
  struct database_multiple_documents_fields database_multiple_documents_fields;
  int document_count = 0;

  // define macros
  #define DATABASE_FIELDS "username|"
  #define DATA "{\"username\":\"XCASH\"}"

  #define SERVER_RECEIVE_DATA_SOCKET_GET_STATISTICS_ERROR(settings) \
  memset(data,0,strnlen(data,MAXIMUM_BUFFER_SIZE)); \
  memcpy(data,"{\"Error\":\"Could not get statistics\"}",36); \
  send_data(CLIENT_SOCKET,data,strlen(data),400,"application/json"); \
  pointer_reset(data); \
  if (settings == 0) \
  { \
    pointer_reset_database_array; \
  } \
  return 400;

  #define pointer_reset_database_array \
  for (count = 0; count < 11; count++) \
  { \
    pointer_reset(database_data.item[count]); \
    pointer_reset(database_data.value[count]); \
  } \
  for (count = 0; count < document_count; count++) \
  { \
    for (counter = 0; counter < TOTAL_DELEGATES_DATABASE_FIELDS; counter++) \
    { \
      pointer_reset(database_multiple_documents_fields.item[count][counter]); \
      pointer_reset(database_multiple_documents_fields.value[count][counter]); \
    } \
  }
  
  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // check if there is any data in the database that matches the message
  if (count_documents_in_collection(DATABASE_NAME,"statistics",DATA,0) <= 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_STATISTICS_ERROR(1);
  }

  // get how many documents are in the database
  document_count = count_all_documents_in_collection(DATABASE_NAME,"delegates",0);
  if (document_count <= 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_STATISTICS_ERROR(1);
  }
  else if (document_count > BLOCK_VERIFIERS_AMOUNT)
  {
    document_count = BLOCK_VERIFIERS_AMOUNT;
  }

  // initialize the database_document_fields struct 
  for (count = 0; count < 11; count++)
  {
    database_data.item[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));
    database_data.value[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));
    if (database_data.item[count] == NULL || database_data.value[count] == NULL)
    {
      memcpy(error_message.function[error_message.total],"server_receive_data_socket_delegates_website_get_statistics",59);
      memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
      error_message.total++;
      print_error_message;  
      exit(0);
    }
  }
  database_data.count = 0;

  // initialize the database_multiple_documents_fields struct 
  for (count = 0; count < document_count; count++)
  {
    for (counter = 0; counter < TOTAL_DELEGATES_DATABASE_FIELDS; counter++)
    {
      database_multiple_documents_fields.item[count][counter] = (char*)calloc(BUFFER_SIZE,sizeof(char));
      database_multiple_documents_fields.value[count][counter] = (char*)calloc(BUFFER_SIZE,sizeof(char));
      if (database_multiple_documents_fields.item[count][counter] == NULL || database_multiple_documents_fields.value[count][counter] == NULL)
      {
        memcpy(error_message.function[error_message.total],"server_receive_data_socket_delegates_website_get_statistics",59);
        memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
        error_message.total++;
        print_error_message;  
        exit(0);
      }
    }
  }
  database_multiple_documents_fields.document_count = 0;
  database_multiple_documents_fields.database_fields_count = 0;

  if (read_multiple_documents_all_fields_from_collection(DATABASE_NAME,"delegates","",&database_multiple_documents_fields,1,document_count,1,"total_vote_count_number",0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_STATISTICS_ERROR(0);
  }

  if (read_document_all_fields_from_collection(DATABASE_NAME,"statistics",DATA,&database_data,0) == 0)
  {
    pointer_reset_database_array;
    SERVER_RECEIVE_DATA_SOCKET_GET_STATISTICS_ERROR(0);
  }

  // get the current block height
  if (get_current_block_height(data,0) == 0)
  {
    pointer_reset_database_array;
    SERVER_RECEIVE_DATA_SOCKET_GET_STATISTICS_ERROR(0);
  }
  memcpy(database_data.item[database_data.count],"current_block_height",20);
  memcpy(database_data.value[database_data.count],data,strnlen(data,BUFFER_SIZE));
  database_data.count++;

  // get the xcash proof of stake round number
  sscanf(data,"%zu", &current_block_height);
  count = current_block_height - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT;  
  memset(data,0,strlen(data));
  snprintf(data,MAXIMUM_BUFFER_SIZE,"%zu",count);

  memcpy(database_data.item[database_data.count],"XCASH_DPOPS_round_number",33);
  memcpy(database_data.value[database_data.count],data,strnlen(data,BUFFER_SIZE));
  database_data.count++;

  // get the total votes
  for (count = 0, count2 = 0; count < document_count; count++)
  {
    for (counter = 0; counter < TOTAL_DELEGATES_DATABASE_FIELDS; counter++)
    {
      if (strncmp(database_multiple_documents_fields.item[count][counter],"total_vote_count",BUFFER_SIZE) == 0)
      {
        sscanf(database_multiple_documents_fields.value[count][counter],"%zu", &number);
        count2 += number;
      }
    }
  }

  memset(data,0,strlen(data));
  snprintf(data,MAXIMUM_BUFFER_SIZE,"%zu",count2);

  memcpy(database_data.item[database_data.count],"total_votes",11);
  memcpy(database_data.value[database_data.count],data,strnlen(data,BUFFER_SIZE));
  database_data.count++;

  // get the XCASH_DPOPS_circulating_percentage
  generated_supply = 190734.9 + XCASH_PREMINE_TOTAL_SUPPLY;
  for (counter = 2; counter < current_block_height; counter++)
  { 
    generated_supply += (XCASH_TOTAL_SUPPLY - generated_supply) / 524288;
  }  

  number = ((size_t)((((double)count2) / (generated_supply - (XCASH_PREMINE_TOTAL_SUPPLY - XCASH_PREMINE_CIRCULATING_SUPPLY))) * 100) | 0);

  memset(data,0,strlen(data));
  snprintf(data,MAXIMUM_BUFFER_SIZE,"%zu",number);

  memcpy(database_data.item[database_data.count],"XCASH_DPOPS_circulating_percentage",43);
  memcpy(database_data.value[database_data.count],data,strnlen(data,BUFFER_SIZE));
  database_data.count++;
  
  memset(data,0,strlen(data));

  // create a json string out of the database array of item and value
  if (create_json_data_from_database_document_array(&database_data,data,DATABASE_FIELDS) == 0)
  {
    pointer_reset_database_array;
    SERVER_RECEIVE_DATA_SOCKET_GET_STATISTICS_ERROR(0);
  }

  if (send_data(CLIENT_SOCKET,data,strlen(data),200,"application/json") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_STATISTICS_ERROR(0);
  }
 
  pointer_reset_database_array;  
  pointer_reset(data);  
  return 200;

  #undef DATABASE_COLLECTION
  #undef DATABASE_FIELDS
  #undef DATA
  #undef SERVER_RECEIVE_DATA_SOCKET_GET_STATISTICS_ERROR
  #undef pointer_reset_database_array
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
  struct database_multiple_documents_fields database_data;
  int document_count = 0;
  size_t count = 0;
  size_t counter = 0;

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define DATABASE_FIELDS "public_address|total_vote_count_number|IP_address|about|website|team|server_settings|block_verifier_online_total_rounds|block_producer_total_rounds|VRF_node_public_and_secret_key_total_rounds|VRF_node_random_data_total_rounds|VRF_node_next_main_nodes_total_rounds|block_producer_block_heights|VRF_node_public_and_private_key_block_heights|VRF_node_random_data_block_heights|VRF_node_next_main_nodes_block_heights|"
 
  #define SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_ERROR(settings) \
  memset(message,0,strnlen(message,MAXIMUM_BUFFER_SIZE)); \
  memcpy(message,"{\"Error\":\"Could not get the delegates information\"}",51); \
  send_data(CLIENT_SOCKET,message,strlen(message),400,"application/json"); \
  pointer_reset(message); \
  if (settings == 0) \
  { \
    pointer_reset_database_array; \
  } \
  return 0;

  #define pointer_reset_database_array \
  for (count = 0; count < document_count; count++) \
  { \
    for (counter = 0; counter < TOTAL_DELEGATES_DATABASE_FIELDS; counter++) \
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

  // get how many documents are in the database
  document_count = count_all_documents_in_collection(DATABASE_NAME,DATABASE_COLLECTION,0);
  if (document_count <= 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_ERROR(1);
  }
  else if (document_count > BLOCK_VERIFIERS_AMOUNT)
  {
    document_count = BLOCK_VERIFIERS_AMOUNT;
  }
  
  // initialize the database_multiple_documents_fields struct 
  for (count = 0; count < document_count; count++)
  {
    for (counter = 0; counter < TOTAL_DELEGATES_DATABASE_FIELDS; counter++)
    {
      database_data.item[count][counter] = (char*)calloc(BUFFER_SIZE,sizeof(char));
      database_data.value[count][counter] = (char*)calloc(BUFFER_SIZE,sizeof(char));
      if (database_data.item[count][counter] == NULL || database_data.value[count][counter] == NULL)
      {
        memcpy(error_message.function[error_message.total],"server_receive_data_socket_get_delegates",40);
        memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
        error_message.total++;
        print_error_message;  
        exit(0);
      }
    }
  }
  database_data.document_count = 0;
  database_data.database_fields_count = 0;

  if (read_multiple_documents_all_fields_from_collection(DATABASE_NAME,DATABASE_COLLECTION,"",&database_data,1,document_count,1,"total_vote_count_number",0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_ERROR(0);
  }
  
  if (create_json_data_from_database_multiple_documents_array(&database_data,message,DATABASE_FIELDS) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_ERROR(0);
  }

  if (send_data(CLIENT_SOCKET,message,strlen(message),200,"application/json") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_ERROR(0);
  }

  pointer_reset(message);
  pointer_reset_database_array;
  return 1;

  #undef DATABASE_COLLECTION
  #undef DATABASE_FIELDS
  #undef SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_ERROR
  #undef pointer_reset_database_array
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_get_delegates_statistics
Description: Runs the code when the server receives /getdelegates
Parameters:
  CLIENT_SOCKET - The socket to send data to
  DATA - The data
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_get_delegates_statistics(const int CLIENT_SOCKET, const char* DATA)
{
  // Variables
  char data2[BUFFER_SIZE];
  char* message = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));  
  size_t count = 0;
  size_t counter = 0;
  struct database_document_fields database_data;
  struct database_multiple_documents_fields database_multiple_documents_fields;
  int document_count = 0;

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define DATABASE_FIELDS "current_delegate_rank|"

  #define SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_STATISTICS_ERROR(settings) \
  memset(message,0,strnlen(message,MAXIMUM_BUFFER_SIZE)); \
  memcpy(message,"{\"Error\":\"Could not get the delegates statistics\"}",50); \
  send_data(CLIENT_SOCKET,message,strlen(message),400,"application/json"); \
  pointer_reset(message); \
  if (settings == 0) \
  { \
    pointer_reset_database_array; \
  } \
  return 0;
  
  #define pointer_reset_database_array \
  for (count = 0; count < TOTAL_DELEGATES_DATABASE_FIELDS+1; count++) \
  { \
    pointer_reset(database_data.item[count]); \
    pointer_reset(database_data.value[count]); \
  } \
  for (count = 0; count < document_count; count++) \
  { \
    for (counter = 0; counter < TOTAL_DELEGATES_DATABASE_FIELDS; counter++) \
    { \
      pointer_reset(database_multiple_documents_fields.item[count][counter]); \
      pointer_reset(database_multiple_documents_fields.value[count][counter]); \
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
  memcpy(data2,&DATA[39],(strnlen(DATA,sizeof(data2)) - strnlen(strstr(DATA," HTTP/"),sizeof(data2)))-39);

  // error check
  if (strncmp(data2,"",BUFFER_SIZE) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_STATISTICS_ERROR(1);
  } 

  const size_t DATA_LENGTH = strnlen(data2,BUFFER_SIZE);

  // check if the data is a public address or a delegate name
  memcpy(message,"{\"",2);
  if (memcmp(data2,XCASH_WALLET_PREFIX,3) == 0 && DATA_LENGTH == XCASH_WALLET_LENGTH)
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
  if (count_documents_in_collection(DATABASE_NAME,DATABASE_COLLECTION,message,0) <= 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_STATISTICS_ERROR(1);
  }
  
  // initialize the database_document_fields struct 
  for (count = 0; count < TOTAL_DELEGATES_DATABASE_FIELDS+1; count++)
  {
    database_data.item[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));
    database_data.value[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));
    if (database_data.item[count] == NULL || database_data.value[count] == NULL)
    {
      memcpy(error_message.function[error_message.total],"server_receive_data_socket_get_delegates_statistics",51);
      memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
      error_message.total++;
      print_error_message;  
      exit(0);
    }
  }
  database_data.count = 0;

  // get how many documents are in the database
  document_count = count_all_documents_in_collection(DATABASE_NAME,DATABASE_COLLECTION,0);
  if (document_count <= 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_STATISTICS_ERROR(1);
  }
  else if (document_count > BLOCK_VERIFIERS_AMOUNT)
  {
    document_count = BLOCK_VERIFIERS_AMOUNT;
  }

  // initialize the database_multiple_documents_fields struct 
  for (count = 0; count < document_count; count++)
  {
    for (counter = 0; counter < TOTAL_DELEGATES_DATABASE_FIELDS+1; counter++)
    {
      database_multiple_documents_fields.item[count][counter] = (char*)calloc(BUFFER_SIZE,sizeof(char));
      database_multiple_documents_fields.value[count][counter] = (char*)calloc(BUFFER_SIZE,sizeof(char));
      
      if (database_multiple_documents_fields.item[count][counter] == NULL || database_multiple_documents_fields.value[count][counter] == NULL)
      {
        memcpy(error_message.function[error_message.total],"server_receive_data_socket_get_delegates_statistics",51);
        memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
        error_message.total++;
        print_error_message;  
        exit(0);
      }
    }
  }
  database_multiple_documents_fields.document_count = 0;
  database_multiple_documents_fields.database_fields_count = 0;
  
  if (read_document_all_fields_from_collection(DATABASE_NAME,DATABASE_COLLECTION,message,&database_data,0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_STATISTICS_ERROR(0);
  }

  if (read_multiple_documents_all_fields_from_collection(DATABASE_NAME,DATABASE_COLLECTION,"",&database_multiple_documents_fields,1,document_count,1,"total_vote_count",0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_STATISTICS_ERROR(0);
  }

  // get the delegates rank
  for (count = 0, counter = 0; count < database_multiple_documents_fields.document_count; count++)
  {
    if (strncmp(database_multiple_documents_fields.item[count][0],data2,XCASH_WALLET_LENGTH) == 0 || strncmp(database_multiple_documents_fields.item[count][4],data2,BUFFER_SIZE) == 0)
    {
      break;
    }
    counter++;
  }

  memset(data2,0,sizeof(data2));
  snprintf(data2, sizeof(data2), "%zu", counter);
  memcpy(database_data.item[18],"current_delegate_rank",21);
  memcpy(database_data.value[18],data2,strnlen(data2,BUFFER_SIZE));
  database_data.count++;

  memset(message,0,strlen(message)); 

  // create a json string out of the database array of item and value
  if (create_json_data_from_database_document_array(&database_data,message,DATABASE_FIELDS) == 0)
  {    
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_STATISTICS_ERROR(0);
  }

  if (send_data(CLIENT_SOCKET,message,strlen(message),200,"application/json") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_STATISTICS_ERROR(0);
  }
  
  pointer_reset_database_array;
  pointer_reset(message);
  return 1;

  #undef DATABASE_COLLECTION
  #undef DATABASE_FIELDS
  #undef GET_DELEGATES_STATISTICS_ERROR
  #undef pointer_reset_database_array
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
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));  
  int count = 0;
  struct database_document_fields database_data;

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define DATABASE_FIELDS "total_vote_count|total_vote_count_number|IP_address|delegate_name|block_verifier_score|online_status|block_verifier_total_rounds|block_verifier_online_total_rounds|block_verifier_online_percentage|block_producer_total_rounds|VRF_node_public_and_secret_key_total_rounds|VRF_node_random_data_total_rounds|VRF_node_next_main_nodes_total_rounds|block_producer_block_heights|VRF_node_public_and_secret_key_block_heights|VRF_node_random_data_block_heights|VRF_node_next_main_nodes_block_heights|"

  #define SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_INFORMATION_ERROR(settings) \
  memset(message,0,strnlen(message,MAXIMUM_BUFFER_SIZE)); \
  memcpy(message,"{\"Error\":\"Could not get the delegates information\"}",51); \
  send_data(CLIENT_SOCKET,message,strlen(message),400,"application/json"); \
  pointer_reset(message); \
  if (settings == 0) \
  { \
    pointer_reset_database_array; \
  } \
  return 0;
  
  #define pointer_reset_database_array \
  for (count = 0; count < TOTAL_DELEGATES_DATABASE_FIELDS; count++) \
  { \
    pointer_reset(database_data.item[count]); \
    pointer_reset(database_data.value[count]); \
  }

  // check if the memory needed was allocated on the heap successfully
  if (message == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  memset(data2,0,sizeof(data2));

  // get the parameter1
  memcpy(data2,&DATA[40],(strnlen(DATA,sizeof(data2)) - strnlen(strstr(DATA," HTTP/"),sizeof(data2)))-40);

  // error check
  if (strncmp(data2,"",BUFFER_SIZE) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_INFORMATION_ERROR(1);
  } 

  const size_t DATA_LENGTH = strnlen(data2,BUFFER_SIZE);
  
  // check if the data is a public address or a delegate name
  memcpy(message,"{\"",2);
  if (memcmp(data2,XCASH_WALLET_PREFIX,3) == 0 && DATA_LENGTH == XCASH_WALLET_LENGTH)
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
  if (count_documents_in_collection(DATABASE_NAME,DATABASE_COLLECTION,message,0) <= 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_INFORMATION_ERROR(1);
  }
  
  // initialize the database_document_fields struct 
  for (count = 0; count < TOTAL_DELEGATES_DATABASE_FIELDS; count++)
  {
    database_data.item[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));
    database_data.value[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));

    if (database_data.item[count] == NULL || database_data.value[count] == NULL)
    {
      memcpy(error_message.function[error_message.total],"server_receive_data_socket_get_delegates_information",52);
      memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
      error_message.total++;
      print_error_message;  
      exit(0);
    }
  }
  database_data.count = 0;
  
  if (read_document_all_fields_from_collection(DATABASE_NAME,DATABASE_COLLECTION,message,&database_data,0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_INFORMATION_ERROR(0);
  }
  
  memset(message,0,strlen(message));

  // create a json string out of the database array of item and value
  if (create_json_data_from_database_document_array(&database_data,message,DATABASE_FIELDS) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_INFORMATION_ERROR(0);
  }

  if (send_data(CLIENT_SOCKET,message,strlen(message),200,"application/json") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_STATISTICS_ERROR(0);
  }
  
  pointer_reset_database_array;
  pointer_reset(message);
  return 1;

  #undef DATABASE_COLLECTION
  #undef DATABASE_FIELDS
  #undef GET_DELEGATES_INFORMATION_ERROR
  #undef pointer_reset_database_array
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
  int count = 0;
  int counter = 0;
  struct database_multiple_documents_fields database_data;
  int document_count = 0;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_VOTERS_LIST_ERROR(settings) \
  memset(message,0,strnlen(message,MAXIMUM_BUFFER_SIZE)); \
  memcpy(message,"{\"Error\":\"Could not get the delegates voters list\"}",51); \
  send_data(CLIENT_SOCKET,message,strlen(message),400,"application/json"); \
  pointer_reset(message); \
  if (settings == 0) \
  { \
    pointer_reset_database_array; \
  } \
  return 0;

  #define pointer_reset_database_array \
  for (count = 0; count < document_count; count++) \
  { \
    for (counter = 0; counter < TOTAL_RESERVE_PROOFS_DATABASE_FIELDS; counter++) \
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
  memcpy(data2,&DATA[39],(strnlen(DATA,sizeof(data2)) - strnlen(strstr(DATA," HTTP/"),sizeof(data2)))-39);

  // error check
  if (strncmp(data2,"",BUFFER_SIZE) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_INFORMATION_ERROR(1);
  } 

  const size_t DATA_LENGTH = strnlen(data2,BUFFER_SIZE);

  // get the delegates public address
  if (memcmp(data2,XCASH_WALLET_PREFIX,3) != 0 || DATA_LENGTH != XCASH_WALLET_LENGTH)
  {
    memcpy(message,"{\"delegate_name\":\"",18);
    memcpy(message+18,data2,DATA_LENGTH);
    memcpy(message+18+DATA_LENGTH,"\"}",2);
    memset(data2,0,sizeof(data2));

    if (read_document_field_from_collection(DATABASE_NAME,"delegates",message,"public_address",data2,0) == 0)
    {
      SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_VOTERS_LIST_ERROR(1);
    }
  }

  // create the message
  memset(message,0,strlen(message));
  memcpy(message,"{\"public_address_voted_for\":\"",29);
  memcpy(message+29,data2,XCASH_WALLET_LENGTH);
  memcpy(message+127,"\"}",2);

  // check how many reserve proofs are for the public address
  for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
  { 
    memset(data2,0,sizeof(data2));
    memcpy(data2,"reserve_proofs_",15);
    snprintf(data2+15,sizeof(data2),"%d",count);

    counter = count_documents_in_collection(DATABASE_NAME,data2,message,0);
    if (counter < 0)
    {
      SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_VOTERS_LIST_ERROR(1);
    }
    document_count += counter;
  }
  if (document_count <= 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_VOTERS_LIST_ERROR(1);
  }

  // initialize the database_multiple_documents_fields struct
  for (count = 0; count < document_count; count++)
  {
    for (counter = 0; counter < TOTAL_RESERVE_PROOFS_DATABASE_FIELDS; counter++)
    {
      database_data.item[count][counter] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));
      database_data.value[count][counter] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));

      if (database_data.item[count][counter] == NULL || database_data.value[count][counter] == NULL)
      {
        memcpy(error_message.function[error_message.total],"server_receive_data_socket_get_delegates_voters_list",52);
        memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
        error_message.total++;
        print_error_message;  
        exit(0);
      }
    }
  }
  database_data.document_count = 0;
  database_data.database_fields_count = 0;

  // get all of the reserve proofs for the public address
  for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
  { 
    memset(data2,0,sizeof(data2));
    memcpy(data2,"reserve_proofs_",15);
    snprintf(data2+15,sizeof(data2),"%d",count);

    counter = count_documents_in_collection(DATABASE_NAME,data2,message,0);
    if (counter > 0)
    {      
      if (read_multiple_documents_all_fields_from_collection(DATABASE_NAME,data2,"",&database_data,1,counter,0,"",0) == 0)
      {
        SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_VOTERS_LIST_ERROR(0);
      }
    }
  }

  memset(message,0,strlen(message)); 

  if (create_json_data_from_database_multiple_documents_array(&database_data,message,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_VOTERS_LIST_ERROR(0);
  }  

  if (send_data(CLIENT_SOCKET,message,strlen(message),200,"application/json") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_VOTERS_LIST_ERROR(0);
  }

  pointer_reset_database_array;
  pointer_reset(message);
  return 1;

  #undef SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_VOTERS_LIST_ERROR
  #undef pointer_reset_database_array
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
  char data3[BUFFER_SIZE];  
  size_t count = 0;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_GET_ROUND_STATISTICS_ERROR \
  memset(message,0,strnlen(message,MAXIMUM_BUFFER_SIZE)); \
  memcpy(message,"{\"Error\":\"Could not get the round statistics\"}",46); \
  send_data(CLIENT_SOCKET,message,strlen(message),400,"application/json"); \
  return 0;

  memset(message,0,sizeof(message));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  // get the parameter1
  memcpy(data2,&DATA[35],(strnlen(DATA,sizeof(data2)) - strnlen(strstr(DATA," HTTP/"),sizeof(data2)))-35);

  // error check
  if (strncmp(data2,"",BUFFER_SIZE) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_ROUND_STATISTICS_ERROR;
  } 

  const size_t DATA_LENGTH = strnlen(data2,BUFFER_SIZE);

  // calculate what reserve_bytes database the reserve bytes data will be in
  sscanf(data2,"%zu", &count);
  if (count < XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_ROUND_STATISTICS_ERROR;
  }

  count = ((count - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME) + 1;
  memcpy(data3,"reserve_bytes_",14);
  snprintf(data3+14,sizeof(data3),"%zu",count);

  // create the message
  memcpy(message,"{\"block_height\":\"",17);
  memcpy(message+17,data2,strnlen(data2,sizeof(message)));
  memcpy(message+strlen(message),"\"}",2);

  // check if there is any data in the database that matches the message
  if (count_documents_in_collection(DATABASE_NAME,data3,message,0) <= 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_ROUND_STATISTICS_ERROR;
  }

  memset(data2,0,sizeof(data2));

  if (read_document_field_from_collection(DATABASE_NAME,data3,message,"reserve_bytes",data2,0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_ROUND_STATISTICS_ERROR;
  }

  // create a json string out of the database array of item and value
  memset(message,0,sizeof(message));
  memcpy(message,"{\"reserve_bytes\":\"",18);
  memcpy(message+18,data2,strnlen(data2,sizeof(message)));
  memcpy(message+strlen(message),"\"}",2);

  if (send_data(CLIENT_SOCKET,message,strlen(message),200,"application/json") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_ROUND_STATISTICS_ERROR;
  }
  
  return 1;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_GET_ROUND_STATISTICS_ERROR
}



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
  send_data(CLIENT_SOCKET,message,strlen(message),400,"application/json"); \
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
        print_error_message;  
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
    snprintf(data+15,sizeof(data),"%d",count);

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

  if (send_data(CLIENT_SOCKET,message,strlen(message),200,"application/json") == 0)
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
  send_data(CLIENT_SOCKET,message,strlen(message),400,"application/json"); \
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
        print_error_message;  
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
  
  if (send_data(CLIENT_SOCKET,message,strlen(message),200,"application/json") == 0)
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
  int count = 0;
  struct database_document_fields database_data;

  // define macros
  #define DATABASE_COLLECTION "public_addresses"
  #define TOTAL_PUBLIC_ADDRESSES_DATABASE_FIELDS 4

  #define SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_INFORMATION_ERROR(settings) \
  memset(message,0,strnlen(message,MAXIMUM_BUFFER_SIZE)); \
  memcpy(message,"{\"Error\":\"Could not get the public addresses information\"}",58); \
  send_data(CLIENT_SOCKET,message,strlen(message),400,"application/json"); \
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
  if (strncmp(data2,"",BUFFER_SIZE) == 0 || memcmp(data2,XCASH_WALLET_PREFIX,3) != 0 || strnlen(data2,BUFFER_SIZE) != XCASH_WALLET_LENGTH)
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
      print_error_message;  
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

  if (send_data(CLIENT_SOCKET,message,strlen(message),200,"application/json") == 0)
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
  send_data(CLIENT_SOCKET,message,strlen(message),400,"application/json"); \
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
  if (strncmp(data2,"",BUFFER_SIZE) == 0 || memcmp(data2,XCASH_WALLET_PREFIX,3) != 0 || strnlen(data2,BUFFER_SIZE) != XCASH_WALLET_LENGTH)
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
        print_error_message;  
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

  if (send_data(CLIENT_SOCKET,message,strlen(message),200,"application/json") == 0)
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