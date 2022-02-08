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
Name: check_for_valid_start_and_amount_parameters
Description: Checks for a valid start and end parameters
Parameters:
  START - The start parameter
  AMOUNT - The amount parameter
Return: 0 if any of the parameters are invalid, 1 if all of the parameters are valid
-----------------------------------------------------------------------------------------------------------
*/

int check_for_valid_start_and_amount_parameters(const char* START, const char* AMOUNT)
{
  // Constants
  const size_t START_LENGTH = strlen(START);
  const size_t AMOUNT_LENGTH = strlen(AMOUNT);

  // define macros
  #define VALID_DATA "0123456789"

  // Variables
  char data[sizeof(VALID_DATA)];
  size_t count;
  size_t count2;

  memset(data,0,sizeof(data));
  memcpy(data,VALID_DATA,sizeof(VALID_DATA)-1);

  // check if the parameters are a valid length
  if (START_LENGTH > MAXIMUM_NUMBER_SIZE || AMOUNT_LENGTH > MAXIMUM_NUMBER_SIZE || START_LENGTH == 0 || AMOUNT_LENGTH == 0)
  {
    return 0;
  }

  // check if the parameters have any invalid characters
  for (count = 0; count < START_LENGTH; count++)
  {
    for (count2 = 0; count2 < (sizeof(VALID_DATA)-1); count2++)
    {
      if (strncmp(&START[count],&data[count2],sizeof(char)) == 0)
      {
        break;
      }
    }
    if (count2 == (sizeof(VALID_DATA)-1))
    {
      return 0;
    }
  }

  for (count = 0; count < AMOUNT_LENGTH; count++)
  {
    for (count2 = 0; count2 < (sizeof(VALID_DATA)-1); count2++)
    {
      if (strncmp(&AMOUNT[count],&data[count2],sizeof(char)) == 0)
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
  char data2[BUFFER_SIZE];
  char data3[SMALL_BUFFER_SIZE];
  char message[BUFFER_SIZE];
  char total_votes_data[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;
  int count2;
  int counter;
  int total_blocks_found;
  int total_payments;
  int total_voters;
  int current_delegate_rank;
  long long int block_reward_number;
  long long int total_xcash_from_found_blocks;
  struct delegates delegates[MAXIMUM_AMOUNT_OF_DELEGATES];
  struct database_multiple_documents_fields database_multiple_documents_fields;
  int document_count;  
  int delegates_count;

  // define macros
  #define DATABASE_COLLECTION "delegates"

  #define SERVER_RECEIVE_DATA_SOCKET_SHARED_DELEGATES_WEBSITE_GET_STATISTICS_ERROR(MESSAGE) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_shared_delegates_website_get_statistics",66); \
  memcpy(error_message.data[error_message.total],MESSAGE,sizeof(MESSAGE)-1); \
  error_message.total++; \
  memset(message,0,strlen(message)); \
  memcpy(message,"{\"Error\":\"Could not get the delegates statistics\"}",50); \
  POINTER_RESET_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES); \
  POINTER_RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,count2,TOTAL_BLOCKS_FOUND_DATABASE_FIELDS); \
  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),400,"application/json"); \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));
  memset(message,0,sizeof(message));
  memset(total_votes_data,0,sizeof(total_votes_data));

  // get the total blocks found
  if ((document_count = count_all_documents_in_collection(shared_delegates_database_name,"blocks_found")) <= 0)
  {
    total_blocks_found = 0;
    total_xcash_from_found_blocks = 0;
    total_payments = 0; 
  }  
  else
  {
    total_blocks_found = document_count;
  }

  // initialize the delegates struct
  INITIALIZE_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES,"server_receive_data_socket_shared_delegates_website_get_statistics",data,current_date_and_time,current_UTC_date_and_time);

  // initialize the database_multiple_documents_fields struct 
  INITIALIZE_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,document_count,TOTAL_BLOCKS_FOUND_DATABASE_FIELDS,"server_receive_data_socket_shared_delegates_website_get_statistics",data,current_date_and_time,current_UTC_date_and_time);
  
  // organize the delegates
  if ((delegates_count = organize_delegates(delegates)) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_SHARED_DELEGATES_WEBSITE_GET_STATISTICS_ERROR("Could not get the shared delegates statistics");
  }

  // get the current_delegate_rank
  for (count = 0; count < delegates_count; count++)
  {
    if (strncmp(delegates[count].public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0)
    {
      current_delegate_rank = count+1;
      break;
    }
  }

  if (total_blocks_found != 0)
  {
    if (read_multiple_documents_all_fields_from_collection(shared_delegates_database_name,"blocks_found","",&database_multiple_documents_fields,1,document_count,0,"") <= 0)
    {
      SERVER_RECEIVE_DATA_SOCKET_SHARED_DELEGATES_WEBSITE_GET_STATISTICS_ERROR("Could not get the shared delegates statistics");
    }

    // get the total xcash from found blocks
    for (total_xcash_from_found_blocks = 0, count = 0; count < document_count; count++)
    {
      sscanf(database_multiple_documents_fields.value[count][3], "%lld", &block_reward_number);
      total_xcash_from_found_blocks += block_reward_number;
    }

    // add the total payments to the database_document_fields struct 
    if ((total_payments = count_all_documents_in_collection(shared_delegates_database_name,"public_addresses_payments")) < 0)
    {
      total_payments = 0;
    }
  }

  // get the total voters
  memcpy(message,"{\"public_address_voted_for\":\"",29);
  memcpy(message+29,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(message+127,"\"}",2);

  // check how many reserve proofs are for the public address
  for (total_voters = 0, count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
  { 
    memset(data,0,strlen(data));
    memcpy(data,"reserve_proofs_",15);
    snprintf(data+15,MAXIMUM_NUMBER_SIZE,"%d",count);

    if ((counter = count_documents_in_collection(database_name,data,message)) <= 0)
    {
      continue;
    }
    total_voters += counter;
  }
  RESET_ERROR_MESSAGES;

  // get the delegate_name, total votes, block_verifier_online_percentage and block_verifier_total_rounds

  // create the message
  memset(message,0,sizeof(message));
  memcpy(message,"{\"public_address\":\"",19);
  memcpy(message+19,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(message+117,"\"}",2);

  if (read_document_field_from_collection(database_name,DATABASE_COLLECTION,message,"delegate_name",data3) == 0 || read_document_field_from_collection(database_name,DATABASE_COLLECTION,message,"total_vote_count",total_votes_data) == 0 || read_document_field_from_collection(database_name,DATABASE_COLLECTION,message,"block_verifier_online_percentage",data) == 0 || read_document_field_from_collection(database_name,DATABASE_COLLECTION,message,"block_verifier_total_rounds",data2) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_SHARED_DELEGATES_WEBSITE_GET_STATISTICS_ERROR("Could not get the shared delegates statistics");
  }

  memset(message,0,sizeof(message));
  memcpy(message,"{\"public_address\":\"",19);
  memcpy(message+strlen(message),xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(message+strlen(message),"\",\"delegate_name\":\"",19);
  memcpy(message+strlen(message),data3,strnlen(data3,sizeof(data3)));
  memcpy(message+strlen(message),"\",\"current_delegate_rank\":\"",27);
  snprintf(message+strlen(message),sizeof(message),"%d",current_delegate_rank);
  memcpy(message+strlen(message),"\",\"total_votes\":\"",17);
  memcpy(message+strlen(message),total_votes_data,strnlen(total_votes_data,sizeof(message)));
  memcpy(message+strlen(message),"\",\"online_percentage\":\"",23);
  memcpy(message+strlen(message),data,strnlen(data,sizeof(message)));
  memcpy(message+strlen(message),"\",\"block_verifier_total_rounds\":\"",33);
  memcpy(message+strlen(message),data2,strnlen(data2,sizeof(message)));
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

  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),200,"application/json");

  POINTER_RESET_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES);
  POINTER_RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,count2,TOTAL_BLOCKS_FOUND_DATABASE_FIELDS);

  return 1;

  #undef DATABASE_COLLECTION
  #undef SERVER_RECEIVE_DATA_SOCKET_SHARED_DELEGATES_WEBSITE_GET_STATISTICS_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_get_blocks_found
Description: Runs the code when the server receives /getblocksfound
Parameters:
  CLIENT_SOCKET - The socket to send data to
  DATA - The data
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_get_blocks_found(const int CLIENT_SOCKET,const char* DATA)
{
  // Variables
  char buffer[1024];
  char start[MAXIMUM_NUMBER_SIZE+1];
  char amount[MAXIMUM_NUMBER_SIZE+1];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count = 0;
  size_t counter = 0;
  size_t start_number;
  size_t amount_number;
  struct database_multiple_documents_fields database_multiple_documents_fields;
  int document_count = 0;

  // define macros
  #define DATABASE_COLLECTION "blocks_found"

  #define SERVER_RECEIVE_DATA_SOCKET_GET_BLOCKS_FOUND_ERROR(settings,MESSAGE) \
  if (debug_settings == 1) \
  { \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_get_blocks_found",43); \
  memcpy(error_message.data[error_message.total],MESSAGE,sizeof(MESSAGE)-1); \
  error_message.total++; \
  } \
  memset(buffer,0,strlen(buffer)); \
  memcpy(buffer,"{\"Error\":\"Could not get the delegates blocks found\"}",52); \
  send_data(CLIENT_SOCKET,(unsigned char*)buffer,strlen(buffer),400,"application/json"); \
  if ((settings) == 0) \
  { \
    POINTER_RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,TOTAL_BLOCKS_FOUND_DATABASE_FIELDS); \
  } \
  return 0;

  memset(start,0,sizeof(start));
  memset(amount,0,sizeof(amount));

  // get the total blocks found
  if ((document_count = count_all_documents_in_collection(shared_delegates_database_name,DATABASE_COLLECTION)) <= 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_BLOCKS_FOUND_ERROR(1,"The delegate has not found any blocks");
  }

  // make sure their is a start and amount in the parameters
  if (strstr(DATA,"start=") == NULL || strstr(DATA,"&amount=") == NULL)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_BLOCKS_FOUND_ERROR(1,"Invalid parameters");
  }

  // get the start and amount
  memcpy(start,&DATA[26],(strlen(DATA) - strlen(strstr(DATA,"&amount")))-26);
  memcpy(amount,&DATA[strlen(DATA) - strlen(strstr(DATA,"&amount="))+8],(strlen(DATA) - strlen(strstr(DATA," HTTP/")))-(((strlen(DATA) - strlen(strstr(DATA,"&amount="))))+8));

  // if the start is 0 change it to 1
  if (strncmp(start,"0",1) == 0)
  {
    memset(start,0,sizeof(start));
    memcpy(start,"1",1);
  }  

  // if the amount says all change it to the total number it has
  if (strncmp(amount,"all",3) == 0)
  {
    memset(amount,0,sizeof(amount));
    snprintf(amount,MAXIMUM_NUMBER_SIZE,"%d",document_count);
  }  

  // check if the start and amount string is valid
  if (check_for_valid_start_and_amount_parameters(start,amount) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_BLOCKS_FOUND_ERROR(1,"Invalid parameters");
  }

  // convert the start and amount to a number
  sscanf(start,"%zu", &start_number);
  sscanf(amount,"%zu", &amount_number);  

  memset(buffer,0,sizeof(buffer));

  // initialize the database_multiple_documents_fields struct
  INITIALIZE_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,document_count,TOTAL_BLOCKS_FOUND_DATABASE_FIELDS,"server_receive_data_socket_get_blocks_found",buffer,current_date_and_time,current_UTC_date_and_time);

  if (read_multiple_documents_all_fields_from_collection(shared_delegates_database_name,DATABASE_COLLECTION,"",&database_multiple_documents_fields,start_number,amount_number,0,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_BLOCKS_FOUND_ERROR(0,"Could not get the delegates blocks found data");
  }

  // count how many bytes to allocate for the json data
  for (count = 0, counter = BUFFER_SIZE; count < document_count; count++)
  {
    counter += 16; // 16 is for quotes for the items and values
    counter += strlen(database_multiple_documents_fields.item[count][0]);
    counter += strlen(database_multiple_documents_fields.item[count][1]);
    counter += strlen(database_multiple_documents_fields.item[count][2]);
    counter += strlen(database_multiple_documents_fields.item[count][3]);
    counter += strlen(database_multiple_documents_fields.value[count][0]);
    counter += strlen(database_multiple_documents_fields.value[count][1]);
    counter += strlen(database_multiple_documents_fields.value[count][2]);
    counter += strlen(database_multiple_documents_fields.value[count][3]);
  }

  char* message = (char*)calloc(counter,sizeof(char)); 
  
  if (create_json_data_from_database_multiple_documents_array(&database_multiple_documents_fields,message,"") == 0)
  {
    pointer_reset(message);
    SERVER_RECEIVE_DATA_SOCKET_GET_BLOCKS_FOUND_ERROR(0,"Could not create the json data");
  }  
  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),200,"application/json");
  
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
  char data2[SMALL_BUFFER_SIZE];
  char message[SMALL_BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count = 0;
  struct database_document_fields database_data;

  // define macros
  #define DATABASE_COLLECTION "public_addresses"

  #define SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_INFORMATION_ERROR(settings,MESSAGE) \
  if (debug_settings == 1) \
  { \
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_get_public_address_information",57); \
    memcpy(error_message.data[error_message.total],MESSAGE,sizeof(MESSAGE)-1); \
    error_message.total++; \
  } \
  memset(message,0,strlen(message)); \
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
  if (strncmp(data2,"",sizeof(data2)) == 0 || strncmp(data2,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || strnlen(data2,sizeof(data2)) != XCASH_WALLET_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_INFORMATION_ERROR(1,"Invalid parameters");
  }
  
  // check if the data is a public address or a delegate name
  memcpy(message,"{\"",2);
  memcpy(message+2,"public_address\":\"",17);
  memcpy(message+19,data2,XCASH_WALLET_LENGTH);
  memcpy(message+19+XCASH_WALLET_LENGTH,"\"}",2);
  
  // check if there is any data in the database that matches the message
  if (count_documents_in_collection(shared_delegates_database_name,DATABASE_COLLECTION,message) <= 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_INFORMATION_ERROR(1,"The public address could not be found in the database for the delegate");
  }

  // initialize the database_document_fields struct 
  INITIALIZE_DATABASE_DOCUMENT_FIELDS_STRUCT(count,TOTAL_PUBLIC_ADDRESSES_DATABASE_FIELDS,"server_receive_data_socket_get_public_address_information",data2,current_date_and_time,current_UTC_date_and_time);
   
  if (read_document_all_fields_from_collection(shared_delegates_database_name,DATABASE_COLLECTION,message,&database_data) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_INFORMATION_ERROR(0,"Could not get the public addresses data");
  }
  
  memset(message,0,strlen(message));

  // create a json string out of the database array of item and value
  if (create_json_data_from_database_document_array(&database_data,message,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_INFORMATION_ERROR(0,"Could not create the json data");
  }
  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),200,"application/json");
  
  POINTER_RESET_DATABASE_DOCUMENT_FIELDS_STRUCT(count);
  return 1;

  #undef DATABASE_COLLECTION
  #undef SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_INFORMATION_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_get_public_address_payment_information
Description: Runs the code when the server receives /getpublicaddresspaymentinformation
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
  char data3[BUFFER_SIZE];
  char start[MAXIMUM_NUMBER_SIZE+1];
  char amount[MAXIMUM_NUMBER_SIZE+1];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count = 0;
  size_t counter = 0;
  size_t start_number;
  size_t amount_number;
  struct database_multiple_documents_fields database_multiple_documents_fields;
  int document_count = 0;

  // define macros
  #define DATABASE_COLLECTION "public_addresses_payments"

  #define SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_ERROR(settings,MESSAGE) \
  if (debug_settings == 1) \
  { \
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_get_public_address_payment_information",65); \
    memcpy(error_message.data[error_message.total],MESSAGE,sizeof(MESSAGE)-1); \
    error_message.total++; \
  } \
  memset(data2,0,strlen(data2)); \
  memcpy(data2,"{\"Error\":\"Could not get the public addresses information\"}",58); \
  send_data(CLIENT_SOCKET,(unsigned char*)data2,strlen(data2),400,"application/json"); \
  if ((settings) == 0) \
  { \
    POINTER_RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,TOTAL_PUBLIC_ADDRESSES_PAYMENTS_DATABASE_FIELDS); \
  } \
  return 0;

  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));
  memset(start,0,sizeof(start));
  memset(amount,0,sizeof(amount));

  // get the parameter1
  memcpy(data2,&DATA[55],(strnlen(DATA,sizeof(data2)) - strnlen(strstr(DATA," HTTP/"),sizeof(data2)))-55);

  // error check
  if (strncmp(data2,"",BUFFER_SIZE) == 0 || strncmp(data2,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || strnlen(data2,BUFFER_SIZE) != XCASH_WALLET_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_ERROR(1,"Invalid parameters");
  }

  // check if there is any data in the database that matches the message
  memcpy(data3,"{\"",2);
  memcpy(data3+2,"public_address\":\"",17);
  memcpy(data3+19,data2,XCASH_WALLET_LENGTH);
  memcpy(data3+19+XCASH_WALLET_LENGTH,"\"}",2);
  
  if ((document_count = count_documents_in_collection(shared_delegates_database_name,DATABASE_COLLECTION,data3)) <= 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_ERROR(1,"There is no payment data for the public address");
  }

  // make sure their is a start and amount in the parameters
  if (strstr(DATA,"&start=") == NULL || strstr(DATA,"&amount=") == NULL)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_ERROR(1,"Invalid parameters");
  }

  // get the start and amount
  memcpy(start,&DATA[156],(strlen(DATA) - strlen(strstr(DATA,"&amount")))-156);
memcpy(amount,&DATA[strlen(DATA) - strlen(strstr(DATA,"&amount="))+8],(strlen(DATA) - strlen(strstr(DATA," HTTP/")))-(((strlen(DATA) - strlen(strstr(DATA,"&amount="))))+8));

  // if the start is 0 change it to 1
  if (strncmp(start,"0",1) == 0)
  {
    memset(start,0,sizeof(start));
    memcpy(start,"1",1);
  }  

  // if the amount says all change it to the total number it has
  if (strncmp(amount,"all",3) == 0)
  {
    memset(amount,0,sizeof(amount));
    snprintf(amount,MAXIMUM_NUMBER_SIZE,"%d",document_count);
  }  

  // check if the start and amount string is valid
  if (check_for_valid_start_and_amount_parameters(start,amount) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_ERROR(1,"Invalid parameters");
  }

  // convert the start and amount to a number
  sscanf(start,"%zu", &start_number);
  sscanf(amount,"%zu", &amount_number);  
  
  // initialize the database_multiple_documents_fields struct
  INITIALIZE_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,document_count,TOTAL_PUBLIC_ADDRESSES_PAYMENTS_DATABASE_FIELDS,"server_receive_data_socket_get_public_address_payment_information",data2,current_date_and_time,current_UTC_date_and_time);
  
  if (read_multiple_documents_all_fields_from_collection(shared_delegates_database_name,DATABASE_COLLECTION,data3,&database_multiple_documents_fields,start_number,amount_number,0,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_ERROR(0,"Could not get the payment data for the public address");
  }

  // count how many bytes to allocate for the json data
  for (count = 0, counter = BUFFER_SIZE; count < document_count; count++)
  {
    counter += 100000;
    counter += strlen(database_multiple_documents_fields.item[count][0]);
    counter += strlen(database_multiple_documents_fields.item[count][1]);
    counter += strlen(database_multiple_documents_fields.item[count][2]);
    counter += strlen(database_multiple_documents_fields.item[count][3]);
    counter += strlen(database_multiple_documents_fields.item[count][4]);
    counter += strlen(database_multiple_documents_fields.item[count][5]);
    counter += strlen(database_multiple_documents_fields.item[count][6]);
    counter += strlen(database_multiple_documents_fields.value[count][0]);
    counter += strlen(database_multiple_documents_fields.value[count][1]);
    counter += strlen(database_multiple_documents_fields.value[count][2]);
    counter += strlen(database_multiple_documents_fields.value[count][3]);
    counter += strlen(database_multiple_documents_fields.value[count][4]);
    counter += strlen(database_multiple_documents_fields.value[count][5]);
    counter += strlen(database_multiple_documents_fields.value[count][6]);
  }
  
  char* message = (char*)calloc(counter,sizeof(char)); 

  // create a json string out of the database array of item and value
  if (create_json_data_from_database_multiple_documents_array(&database_multiple_documents_fields,message,"") == 0)
  {
    pointer_reset(message);
    SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_ERROR(0,"Could not create the json data");
  }

  send_data(CLIENT_SOCKET,(unsigned char*)message,strlen(message),200,"application/json");
  
  POINTER_RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,TOTAL_PUBLIC_ADDRESSES_PAYMENTS_DATABASE_FIELDS);
  pointer_reset(message);
  return 1;

  #undef DATABASE_COLLECTION
  #undef SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_ERROR
}
