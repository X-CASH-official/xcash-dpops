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
#include "block_verifiers_thread_server_functions.h"
#include "database_functions.h"
#include "insert_database_functions.h"
#include "delete_database_functions.h"
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
  time_t current_date_and_time;
  struct tm* current_UTC_date_and_time;
  size_t count;

  // define macros
  #define CHECK_IF_DATABASES_ARE_SYNCED_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"check_if_databases_are_synced",29); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));

  print_start_message(current_date_and_time,current_UTC_date_and_time,"Checking if databases are synced",data);

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
Name: sync_all_block_verifiers_list
Description: Sync the previous, current and next block verifiers from a network data node. This function is only run at startup, since after that the database is used to get the block verifiers list
-----------------------------------------------------------------------------------------------------------
*/

int sync_all_block_verifiers_list(void)
{
  // Variables
  struct delegates delegates[MAXIMUM_AMOUNT_OF_DELEGATES];
  char message[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm* current_UTC_date_and_time;
  char* data3 = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  size_t count;
  size_t count2;

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define PARSE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA(settings,block_verifiers_data) \
  memset(data2,0,sizeof(data2)); \
  if (parse_json_data(data3,settings,data2,sizeof(data2)) == 0) \
  { \
    SYNC_ALL_BLOCK_VERIFIERS_LIST_ERROR("Could not parse the message"); \
  } \
  for (count = 0, count2 = 0; count < BLOCK_VERIFIERS_AMOUNT; count++) \
  { \
    memcpy(block_verifiers_data[count],&data2[count2],strnlen(data2,sizeof(data2)) - strnlen(strstr(data2+count2,"|"),sizeof(data2)) - count2); \
    count2 = strnlen(data2,sizeof(data2)) - strnlen(strstr(data2+count2,"|"),sizeof(data2)) + 1; \
  }

  #define SYNC_ALL_BLOCK_VERIFIERS_LIST_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"sync_all_block_verifiers_list",29); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(message,0,sizeof(message));
  memset(data2,0,sizeof(data2));
  memset(data3,0,strlen(data3));

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
    print_start_message(current_date_and_time,current_UTC_date_and_time,"Syncing the previous, current and next block verifiers directly from a network data node, to be able to update the database",data2);

    // create the message
    memcpy(message,"{\r\n \"message_settings\": \"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST\",\r\n}",103);

    start:
    fprintf(stderr,"Connecting to a random network data node to get a list of previous, current and next block verifiers\n");
    
    memset(data2,0,sizeof(data2));
    memset(data3,0,strlen(data3));

    // send the message to a random network data node
    do
    {
      count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT);
    } while (memcmp(network_data_nodes_list.network_data_nodes_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0);
  
    // get the current time
    get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);   
    
    memcpy(data3,"Connecting to network data node ",32);
    memcpy(data3+32,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],MAXIMUM_BUFFER_SIZE));
    memcpy(data3+strlen(data3)," and sending NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST\n",87);
    strftime(data2,sizeof(data2),"%a %d %b %Y %H:%M:%S UTC\n",current_UTC_date_and_time);
    memcpy(data3+strlen(data3),data2,strnlen(data2,MAXIMUM_BUFFER_SIZE));
    fprintf(stderr,"%s\n",data3);
    memset(data2,0,sizeof(data2));
    memset(data3,0,strlen(data3));

    if (send_and_receive_data_socket(data3,network_data_nodes_list.network_data_nodes_IP_address[count],SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
    {
      memset(data2,0,sizeof(data2));
      memcpy(data2,"Could not receive data from network data node ",46);
      memcpy(data2+46,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],MAXIMUM_BUFFER_SIZE));
      color_print(data2,"red");
      fprintf(stderr,"Connecting to a different network data node\n\n");
      goto start;
    }

    if (verify_data(data3,0,0) == 0)
    {
      SYNC_ALL_BLOCK_VERIFIERS_LIST_ERROR("Could not verify data");
    }
    
    PARSE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("previous_block_verifiers_name_list",previous_block_verifiers_list.block_verifiers_name);
    PARSE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("previous_block_verifiers_public_address_list",previous_block_verifiers_list.block_verifiers_public_address);
    PARSE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("previous_block_verifiers_IP_address_list",previous_block_verifiers_list.block_verifiers_IP_address);
    PARSE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("current_block_verifiers_name_list",current_block_verifiers_list.block_verifiers_name);
    PARSE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("current_block_verifiers_public_address_list",current_block_verifiers_list.block_verifiers_public_address);
    PARSE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("current_block_verifiers_IP_address_list",current_block_verifiers_list.block_verifiers_IP_address);
    PARSE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("next_block_verifiers_name_list",next_block_verifiers_list.block_verifiers_name);
    PARSE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("next_block_verifiers_public_address_list",next_block_verifiers_list.block_verifiers_public_address);
    PARSE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("next_block_verifiers_IP_address_list",next_block_verifiers_list.block_verifiers_IP_address);

    color_print("The previous, current and next block verifiers have been synced from a network data node","green");
  }
  else
  {
    print_start_message(current_date_and_time,current_UTC_date_and_time,"Loading the previous, current and next block verifiers from the database",data2);

    // initialize the delegates struct
    INITIALIZE_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES,"sync_all_block_verifiers_list",data2);

    // organize the delegates
    count2 = organize_delegates(delegates);
    if (count2 == 0)
    {
      POINTER_RESET_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES);
      SYNC_ALL_BLOCK_VERIFIERS_LIST_ERROR("Could not organize the delegates");
    }
    else if (count2 > BLOCK_VERIFIERS_TOTAL_AMOUNT)
    {
      count2 = BLOCK_VERIFIERS_TOTAL_AMOUNT;
    }

    // copy the database_multiple_documents_fields to the next, current and previous block verifiers list
    for (count = 0; count < count2; count++)
    {
       memcpy(previous_block_verifiers_list.block_verifiers_name[count],delegates[count].delegate_name,strnlen(delegates[count].delegate_name,sizeof(previous_block_verifiers_list.block_verifiers_name[count])));
       memcpy(previous_block_verifiers_list.block_verifiers_public_address[count],delegates[count].public_address,strnlen(delegates[count].public_address,sizeof(previous_block_verifiers_list.block_verifiers_public_address[count])));
       memcpy(previous_block_verifiers_list.block_verifiers_IP_address[count],delegates[count].IP_address,strnlen(delegates[count].IP_address,sizeof(previous_block_verifiers_list.block_verifiers_IP_address[count])));
       memcpy(current_block_verifiers_list.block_verifiers_name[count],delegates[count].delegate_name,strnlen(delegates[count].delegate_name,sizeof(current_block_verifiers_list.block_verifiers_name[count])));
       memcpy(current_block_verifiers_list.block_verifiers_public_address[count],delegates[count].public_address,strnlen(delegates[count].public_address,sizeof(current_block_verifiers_list.block_verifiers_public_address[count])));
       memcpy(current_block_verifiers_list.block_verifiers_IP_address[count],delegates[count].IP_address,strnlen(delegates[count].IP_address,sizeof(current_block_verifiers_list.block_verifiers_IP_address[count])));
       memcpy(next_block_verifiers_list.block_verifiers_name[count],delegates[count].delegate_name,strnlen(delegates[count].delegate_name,sizeof(next_block_verifiers_list.block_verifiers_name[count])));
       memcpy(next_block_verifiers_list.block_verifiers_public_address[count],delegates[count].public_address,strnlen(delegates[count].public_address,sizeof(next_block_verifiers_list.block_verifiers_public_address[count])));
       memcpy(next_block_verifiers_list.block_verifiers_IP_address[count],delegates[count].IP_address,strnlen(delegates[count].IP_address,sizeof(next_block_verifiers_list.block_verifiers_IP_address[count])));
    }

    POINTER_RESET_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES);
    color_print("The previous, current and next block verifiers have been loaded from the database","green");
  }
  pointer_reset(data3);
  return 1;

  #undef DATABASE_COLLECTION  
  #undef PARSE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA
  #undef SYNC_ALL_BLOCK_VERIFIERS_LIST_ERROR  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_synced_block_verifiers
Description: Gets the synced block verifiers for syncing the database
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int get_synced_block_verifiers(void)
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
  } while (memcmp(network_data_nodes_list.network_data_nodes_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0);
  
  // get the current time
  get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);
  
  memcpy(data,"Connecting to network data node ",32);
  memcpy(data+32,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
  memcpy(data+strlen(data)," and sending NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST\n",73);
  strftime(data2,MAXIMUM_BUFFER_SIZE,"%a %d %b %Y %H:%M:%S UTC\n",current_UTC_date_and_time);
  memcpy(data+strlen(data),data2,strnlen(data2,sizeof(data)));
  fprintf(stderr,"%s\n",data);
  memset(data,0,sizeof(data));
  memset(data2,0,strlen(data2));

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
  pointer_reset(data2);
  return 1;

  #undef GET_SYNCED_BLOCK_VERIFIERS_DATA
  #undef GET_SYNCED_BLOCK_VERIFIERS_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_check_reserve_proofs_database
Description: Checks if the block verifier needs to sync the reserve proofs database
Paramters:
  settings - 1 to sync from a random block verifier, 2 to sync from a random network data node
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_check_reserve_proofs_database(int settings)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char data2[BUFFER_SIZE]; 
  char message[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm* current_UTC_date_and_time;
  size_t count;

  // define macros  
  #define SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"sync_check_reserve_proofs_database",34); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  pointer_reset(data); \
  return 0;

  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    pointer_reset(data);
    memcpy(error_message.function[error_message.total],"sync_reserve_proofs_database",28);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,data2);  
    exit(0);
  }

  print_start_message(current_date_and_time,current_UTC_date_and_time,"Checking if the reserve proofs database is synced",data2);

  if (get_synced_block_verifiers() == 0)
  {
    SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR("Could not get the synced block verifiers");
  }

  if (settings == 1)
  {
     // get the database data hash for the reserve proofs database
    if (get_database_data_hash(data,DATABASE_NAME,"reserve_proofs") == 0)
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
      if (get_database_data_hash(data,DATABASE_NAME,data2) == 0)
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

    // get the vote settings of the block verifiers

    // check if a consensus could not be reached and sync from a network data node
    if (synced_block_verifiers.vote_settings_connection_timeout >= BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      color_print("A Consensus could not be reached for trying to sync the reserve proofs database, syncing from a random network data node","red");
      settings = 2;
    }
    else if (synced_block_verifiers.vote_settings_false >= BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      color_print("The reserve proofs database is not synced, syncing from a random block verifier","red");

      // get the data
      if (sync_reserve_proofs_database(settings) == 0)
      {
        SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR("Could not sync the reserve proofs database");
      }
    }
  }
  if (settings == 2)
  {
    fprintf(stderr,"Syncing from a random network data node\n");
    if (sync_reserve_proofs_database(settings) == 0)
    {
      SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR("Could not sync the reserve proofs database");
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
  settings - 1 to sync from a random block verifier, 2 to sync from a random network data node
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_reserve_proofs_database(int settings)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  char database_data[BUFFER_SIZE];
  char block_verifiers_ip_address[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm* current_UTC_date_and_time;
  size_t count = 0;
  size_t count2;
  
  // define macros
  #define SYNC_RESERVE_PROOFS_DATABASE_ERROR(message,settings) \
  if (settings == 0) \
  { \
    memcpy(error_message.function[error_message.total],"sync_reserve_proofs_database",28); \
    memcpy(error_message.data[error_message.total],message,strnlen(message,sizeof(error_message.data[error_message.total]))); \
    error_message.total++; \
    pointer_reset(data); \
    return 0; \
  } \
  else \
  { \
    memset(data,0,strlen(data)); \
    memcpy(data+strlen(data),message,strnlen(message,BUFFER_SIZE)); \
    memcpy(data+strlen(data),block_verifiers_ip_address,strnlen(block_verifiers_ip_address,MAXIMUM_BUFFER_SIZE)); \
    memcpy(data+strlen(data),"\nConnecting to another block verifier",37); \
    color_print(data,"red"); \
    goto start; \
  } 

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    pointer_reset(data);
    memcpy(error_message.function[error_message.total],"sync_reserve_proofs_database",28);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,data2);  
    exit(0);
  }

  start:

  memset(data,0,strlen(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));
  memset(database_data,0,sizeof(database_data));
  memset(block_verifiers_ip_address,0,sizeof(block_verifiers_ip_address));

    /* select a random block verifier from the majority vote settings to sync the database from, making sure not to select your own block verifier node
       select a random network data node to sync from if there was a lot of connection_timeouts, to where a majority vote could not be calculated, there were more than BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT new block verifiers
    */

    memset(block_verifiers_ip_address,0,sizeof(block_verifiers_ip_address));
    
    if (settings == 1)
    {
      do
      {
        count = (int)(rand() % BLOCK_VERIFIERS_AMOUNT);
      } while (memcmp(synced_block_verifiers.vote_settings[count],"true",4) != 0 || memcmp(synced_block_verifiers.synced_block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0);
      memcpy(block_verifiers_ip_address,synced_block_verifiers.synced_block_verifiers_IP_address[count],strnlen(synced_block_verifiers.synced_block_verifiers_IP_address[count],sizeof(block_verifiers_ip_address)));
    }
    else if (settings == 2)
    {
      do
      {
        count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT);
      } while (memcmp(network_data_nodes_list.network_data_nodes_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0);
      memcpy(block_verifiers_ip_address,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],sizeof(block_verifiers_ip_address)));
    }



  // get the database data hash for the reserve proofs database
  fprintf(stderr,"Getting the database data from %s\n\n",block_verifiers_ip_address);
  if (get_database_data_hash(data,DATABASE_NAME,"reserve_proofs") == 0)
  {
    SYNC_RESERVE_PROOFS_DATABASE_ERROR("Could not get the database data hash for the reserve proofs database from ",1);
  }

  // create the message
  memcpy(data3,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE\",\r\n \"reserve_proofs_data_hash\": \"",139);
  memcpy(data3+strlen(data3),data,DATA_HASH_LENGTH);
  memcpy(data3+strlen(data3),"\",\r\n ",5);

  for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
  {
    memcpy(data3+strlen(data3),"\"reserve_proofs_data_hash_",26);
    snprintf(data3+strlen(data3),sizeof(data3)-1,"%zu",count);
    memcpy(data3+strlen(data3),"\": \"",4);
    // get the database data hash for the reserve proofs database
    memset(data,0,strlen(data));
    memset(data2,0,strlen(data2));  
    memcpy(data2,"reserve_proofs_",15);  
    snprintf(data2+15,sizeof(data2)-16,"%zu",count);
    if (get_database_data_hash(data,DATABASE_NAME,data2) == 0)
    {
      SYNC_RESERVE_PROOFS_DATABASE_ERROR("Could not get the database data hash for the reserve proofs database from ",1);
    }
    memcpy(data3+strlen(data3),data,DATA_HASH_LENGTH);
    memcpy(data3+strlen(data3),"\",\r\n ",5);
  }
  memcpy(data3+strlen(data3),"}",1);

  // sign_data
  if (sign_data(data3,0) == 0)
  { 
    SYNC_RESERVE_PROOFS_DATABASE_ERROR("Could not sign_data",0);
  }

  // connect to the block verifier and get the database data
  if (send_and_receive_data_socket(database_data,block_verifiers_ip_address,SEND_DATA_PORT,data3,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
  {
    SYNC_RESERVE_PROOFS_DATABASE_ERROR("Could not receive data from ",1);
  }

  if (verify_data(database_data,0,0) == 0)
  {
    SYNC_RESERVE_PROOFS_DATABASE_ERROR("Could not verify data from ",1);
  }



  for (count2 = 1; count2 <= TOTAL_RESERVE_PROOFS_DATABASES; count2++)
  {
    memset(data,0,strlen(data));
    memcpy(data,"Checking if reserve_proofs_",27);
    snprintf(data+strlen(data),MAXIMUM_BUFFER_SIZE-1,"%zu",count2);
    memcpy(data+strlen(data)," is synced",10);
    fprintf(stderr,"%s\n",data);

    // parse the database_data
    memset(data,0,strlen(data));
    memset(data2,0,sizeof(data2));
    memset(data3,0,sizeof(data3));
    memcpy(data2,"reserve_proofs_database_",24);
    snprintf(data2+24,sizeof(data2)-25,"%zu",count2);

    if (parse_json_data(database_data,data2,data,MAXIMUM_BUFFER_SIZE) == 0)
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
      memcpy(data+strlen(data),block_verifiers_ip_address,strnlen(block_verifiers_ip_address,MAXIMUM_BUFFER_SIZE));
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
      if (send_and_receive_data_socket(data,block_verifiers_ip_address,SEND_DATA_PORT,data2,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
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

  pointer_reset(data);
  return 1;
  
  #undef SYNC_RESERVE_PROOFS_DATABASE_ERROR   
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_check_reserve_bytes_database
Description: Checks if the block verifier needs to sync the reserve bytes database
Paramters:
  settings - 1 to sync from a random block verifier, 2 to sync from a random network data node
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_check_reserve_bytes_database(int settings)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char data2[BUFFER_SIZE]; 
  char message[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm* current_UTC_date_and_time;
  size_t count;
  size_t current_reserve_bytes_database;

  // define macros  
  #define SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"sync_check_reserve_bytes_database",33); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  pointer_reset(data); \
  return 0;
  
  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    pointer_reset(data);
    memcpy(error_message.function[error_message.total],"sync_reserve_bytes_database",27);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,data2);  
    exit(0);
  }

  print_start_message(current_date_and_time,current_UTC_date_and_time,"Checking if the reserve bytes database is synced",data2);

  if (get_synced_block_verifiers() == 0)
  {
    SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR("Could not get the synced block verifiers");
  }

  

  if (settings == 1)
  {
    // get the current reserve bytes database
    get_reserve_bytes_database(current_reserve_bytes_database,0);

    // get the database data hash for the reserve bytes database
    if (get_database_data_hash(data,DATABASE_NAME,"reserve_bytes") == 0)
    {
      SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR("Could not get the database data hash for the reserve bytes database");
    }

    // create the message
    memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE\",\r\n \"reserve_bytes_data_hash\": \"",137);
    memcpy(message+strlen(message),data,DATA_HASH_LENGTH);
    memcpy(message+strlen(message),"\",\r\n ",5);

    for (count = 1; count <= current_reserve_bytes_database; count++)
    {
      memcpy(message+strlen(message),"\"reserve_bytes_data_hash_",25);
      snprintf(message+strlen(message),sizeof(message)-1,"%zu",count);
      memcpy(message+strlen(message),"\": \"",4);
      // get the database data hash for the reserve bytes database
      memset(data,0,strlen(data));
      memset(data2,0,strlen(data2));  
      memcpy(data2,"reserve_bytes_",14);  
      snprintf(data2+14,sizeof(data2)-15,"%zu",count);
      if (get_database_data_hash(data,DATABASE_NAME,data2) == 0)
      {
        SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR("Could not get the database data hash for the reserve bytes database");
      }
      memcpy(message+strlen(message),data,DATA_HASH_LENGTH);
      memcpy(message+strlen(message),"\",\r\n ",5);
    }
    memcpy(message+strlen(message),"}",1);

    // sign_data
    if (sign_data(message,0) == 0)
    { 
      SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR("Could not sign_data");
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

    // get the vote settings of the block verifiers

    // check if a consensus could not be reached and sync from a network data node
    if (synced_block_verifiers.vote_settings_connection_timeout >= BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      color_print("A Consensus could not be reached for trying to sync the reserve bytes database, syncing from a random network data node","red");
      settings = 2;
    }
    else if (synced_block_verifiers.vote_settings_false >= BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      color_print("The reserve bytes database is not synced, syncing from a random block verifier","red");

      // get the data
      if (sync_reserve_bytes_database(settings) == 0)
      {
        SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR("Could not sync the reserve bytes database");
      }
    }
  }
  if (settings == 2)
  {
    fprintf(stderr,"Syncing from a random network data node\n");
    if (sync_reserve_bytes_database(settings) == 0)
    {
      SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR("Could not sync the reserve bytes database");
    }
  }

  color_print("The reserve bytes database is synced","green");
  
  pointer_reset(data);
  return 1;
  
  #undef SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_reserve_bytes_database
Description: Syncs the reserve bytes database
Paramters:
  settings - 1 to sync from a random block verifier, 2 to sync from a random network data node
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_reserve_bytes_database(int settings)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char data2[BUFFER_SIZE];
  char* data3 = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char database_data[BUFFER_SIZE];
  char block_verifiers_ip_address[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm* current_UTC_date_and_time;
  size_t count = 0;
  size_t count2;
  size_t current_reserve_bytes_database;
  
  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data3); \
  data3 = NULL; \

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
    memcpy(data+strlen(data),block_verifiers_ip_address,strnlen(block_verifiers_ip_address,MAXIMUM_BUFFER_SIZE)); \
    memcpy(data+strlen(data),"\nConnecting to another block verifier",37); \
    color_print(data,"red"); \
    goto start; \
  } 

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data3 == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }  
    if (data3 != NULL)
    {
      pointer_reset(data3);
    }  
    memcpy(error_message.function[error_message.total],"sync_reserve_bytes_database",27);
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

    /* select a random block verifier from the majority vote settings to sync the database from, making sure not to select your own block verifier node
       select a random network data node to sync from if there was a lot of connection_timeouts, to where a majority vote could not be calculated, there were more than BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT new block verifiers
    */

    memset(block_verifiers_ip_address,0,sizeof(block_verifiers_ip_address));
    
    if (settings == 1)
    {
      do
      {
        count = (int)(rand() % BLOCK_VERIFIERS_AMOUNT);
      } while (memcmp(synced_block_verifiers.vote_settings[count],"true",4) != 0 || memcmp(synced_block_verifiers.synced_block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0);
      memcpy(block_verifiers_ip_address,synced_block_verifiers.synced_block_verifiers_IP_address[count],strnlen(synced_block_verifiers.synced_block_verifiers_IP_address[count],sizeof(block_verifiers_ip_address)));
    }
    else if (settings == 2)
    {
      do
      {
        count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT);
      } while (memcmp(network_data_nodes_list.network_data_nodes_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0);
      memcpy(block_verifiers_ip_address,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],sizeof(block_verifiers_ip_address)));
    }



  // get the current reserve bytes database
  get_reserve_bytes_database(current_reserve_bytes_database,0);
  
  // get the database data hash for the reserve bytes database
  fprintf(stderr,"Getting the database data from %s\n\n",block_verifiers_ip_address);
  if (get_database_data_hash(data,DATABASE_NAME,"reserve_bytes") == 0)
  {
    SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not get the database data hash for the reserve bytes database from ",1);
  }

  // create the message
  memcpy(data3,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE\",\r\n \"reserve_bytes_data_hash\": \"",137);
  memcpy(data3+strlen(data3),data,DATA_HASH_LENGTH);
  memcpy(data3+strlen(data3),"\",\r\n ",5);

  for (count = 1; count <= current_reserve_bytes_database; count++)
  {
    memcpy(data3+strlen(data3),"\"reserve_bytes_data_hash_",25);
    snprintf(data3+strlen(data3),sizeof(data3)-1,"%zu",count);
    memcpy(data3+strlen(data3),"\": \"",4);
    // get the database data hash for the reserve bytes database
    memset(data,0,strlen(data));
    memset(data2,0,strlen(data2));  
    memcpy(data2,"reserve_bytes_",14);  
    snprintf(data2+14,sizeof(data2)-15,"%zu",count);
    if (get_database_data_hash(data,DATABASE_NAME,data2) == 0)
    {
      SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not get the database data hash for the reserve bytes database from ",1);
    }
    memcpy(data3+strlen(data3),data,DATA_HASH_LENGTH);
    memcpy(data3+strlen(data3),"\",\r\n ",5);
  }
  memcpy(data3+strlen(data3),"}",1);

  // sign_data
  if (sign_data(data3,0) == 0)
  { 
    SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not sign_data",0);
  }

  // connect to the block verifier and get the database data
  if (send_and_receive_data_socket(database_data,block_verifiers_ip_address,SEND_DATA_PORT,data3,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
  {
    SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not receive data from ",1);
  }

  if (verify_data(database_data,0,0) == 0)
  {
    SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not verify data from ",1);
  }



  for (count2 = 1; count2 <= current_reserve_bytes_database; count2++)
  {
    memset(data,0,strlen(data));
    memcpy(data,"Checking if reserve_bytes_",26);
    snprintf(data+strlen(data),MAXIMUM_BUFFER_SIZE-1,"%zu",count2);
    memcpy(data+strlen(data)," is synced",10);
    fprintf(stderr,"%s\n",data);

    // parse the database_data
    memset(data,0,strlen(data));
    memset(data2,0,sizeof(data2));
    memset(data3,0,strlen(data3));
    memcpy(data2,"reserve_bytes_database_",23);
    snprintf(data2+23,sizeof(data2)-24,"%zu",count2);

    if (parse_json_data(database_data,data2,data,MAXIMUM_BUFFER_SIZE) == 0)
    {
      SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not receive data from ",1);
    }
   
    if (memcmp(data,"false",5) == 0)
    {
      // sync the database
      memset(data,0,strlen(data));
      memcpy(data,"reserve_bytes_",14);
      snprintf(data+strlen(data),MAXIMUM_BUFFER_SIZE-1,"%zu",count2);
      memcpy(data+strlen(data)," is not synced, downloading it from ",36);
      memcpy(data+strlen(data),block_verifiers_ip_address,strnlen(block_verifiers_ip_address,MAXIMUM_BUFFER_SIZE));
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
      if (send_and_receive_data_socket(data,block_verifiers_ip_address,SEND_DATA_PORT,data2,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
      {
        SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not receive data from ",1);
      }

      if (verify_data(data,0,0) == 0)
      {
        SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not verify data from ",1);
      }

      // parse the message
      memset(data3,0,strlen(data3));
      if (parse_json_data(data,"reserve_bytes_database",data3,MAXIMUM_BUFFER_SIZE) == 0 || memcmp(data3,"",1) == 0)
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

  pointer_reset_all;
  return 1;

  #undef pointer_reset_all  
  #undef SYNC_RESERVE_BYTES_DATABASE_ERROR   
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_check_delegates_database
Description: Checks if the block verifier needs to sync the delegates database
Paramters:
  settings - 1 to sync from a random block verifier, 2 to sync from a random network data node
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_check_delegates_database(int settings)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char data2[BUFFER_SIZE]; 
  char message[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm* current_UTC_date_and_time;
  size_t count;

  // define macros 
  #define DATABASE_COLLECTION "delegates"
  #define SYNC_CHECK_DELEGATES_DATABASE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"sync_check_delegates_database",29); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  pointer_reset(data); \
  return 0;

  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    pointer_reset(data);
    memcpy(error_message.function[error_message.total],"sync_check_delegates_database",29);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,data2);  
    exit(0);
  }

  print_start_message(current_date_and_time,current_UTC_date_and_time,"Checking if the delegates database is synced",data2);

  if (get_synced_block_verifiers() == 0)
  {
    SYNC_CHECK_DELEGATES_DATABASE_ERROR("Could not get the synced block verifiers");
  }

  if (settings == 1)
  {
    // get the database data hash for the reserve proofs database
    if (get_database_data_hash(data,DATABASE_NAME,DATABASE_COLLECTION) == 0)
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

    fprintf(stderr,"Sending all block verifiers a message to check if the reserve proofs database is synced\n"); 

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

    // get the vote settings of the block verifiers

    // check if a consensus could not be reached and sync from a network data node
    if (synced_block_verifiers.vote_settings_connection_timeout >= BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      color_print("A Consensus could not be reached for trying to sync the delegates database, syncing from a random network data node","red");
      settings = 2;
    }
    else if (synced_block_verifiers.vote_settings_false >= BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      color_print("The delegates database is not synced, syncing from a random block verifier","red");

      // get the data
      if (sync_delegates_database(settings) == 0)
      {
        SYNC_CHECK_DELEGATES_DATABASE_ERROR("Could not sync the delegates database database");
      }
    }
  }
  if (settings == 2)
  {
    fprintf(stderr,"Syncing from a random network data node\n");
    if (sync_delegates_database(settings) == 0)
    {
      SYNC_CHECK_DELEGATES_DATABASE_ERROR("Could not sync the delegates database database");
    }
  }

  color_print("The delegates database is synced","green");
  
  pointer_reset(data);
  return 1;

  #undef DATABASE_COLLECTION  
  #undef SYNC_CHECK_DELEGATES_DATABASE_ERROR  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_delegates_database
Description: Syncs the delegates database
Paramters:
  settings - 1 to sync from a random block verifier, 2 to sync from a random network data node
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_delegates_database(int settings)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  char database_data[BUFFER_SIZE];
  char block_verifiers_ip_address[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm* current_UTC_date_and_time;
  size_t count = 0;
  
  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define SYNC_DELEGATES_DATABASE_ERROR(message,settings) \
  if (settings == 0) \
  { \
    memcpy(error_message.function[error_message.total],"sync_delegates_database",23); \
    memcpy(error_message.data[error_message.total],message,strnlen(message,sizeof(error_message.data[error_message.total]))); \
    error_message.total++; \
    pointer_reset(data); \
    return 0; \
  } \
  else \
  { \
    memset(data,0,strlen(data)); \
    memcpy(data+strlen(data),message,strnlen(message,BUFFER_SIZE)); \
    memcpy(data+strlen(data),block_verifiers_ip_address,strnlen(block_verifiers_ip_address,MAXIMUM_BUFFER_SIZE)); \
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
    print_error_message(current_date_and_time,current_UTC_date_and_time,data2);  
    exit(0);
  }

  start:

  memset(data,0,strlen(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));
  memset(database_data,0,sizeof(database_data));
  memset(block_verifiers_ip_address,0,sizeof(block_verifiers_ip_address));

    /* select a random block verifier from the majority vote settings to sync the database from, making sure not to select your own block verifier node
       select a random network data node to sync from if there was a lot of connection_timeouts, to where a majority vote could not be calculated, there were more than BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT new block verifiers
    */

    memset(block_verifiers_ip_address,0,sizeof(block_verifiers_ip_address));
    
    if (settings == 1)
    {
      do
      {
        count = (int)(rand() % BLOCK_VERIFIERS_AMOUNT);
      } while (memcmp(synced_block_verifiers.vote_settings[count],"true",4) != 0 || memcmp(synced_block_verifiers.synced_block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0);
      memcpy(block_verifiers_ip_address,synced_block_verifiers.synced_block_verifiers_IP_address[count],strnlen(synced_block_verifiers.synced_block_verifiers_IP_address[count],sizeof(block_verifiers_ip_address)));
    }
    else if (settings == 2)
    {
      do
      {
        count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT);
      } while (memcmp(network_data_nodes_list.network_data_nodes_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0);
      memcpy(block_verifiers_ip_address,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],sizeof(block_verifiers_ip_address)));
    }



  // get the database data hash for the delegates database
  fprintf(stderr,"Getting the database data from %s\n\n",block_verifiers_ip_address);
  if (get_database_data_hash(data,DATABASE_NAME,DATABASE_COLLECTION) == 0)
  {
    SYNC_DELEGATES_DATABASE_ERROR("Could not get the database data hash for the delegates database from ",1);
  }

  // create the message
  memset(data2,0,sizeof(data2));
  memcpy(data2,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"file\": \"",113);
  snprintf(data2+113,sizeof(data2)-114,"%zu",count);
  memcpy(data2+strlen(data2),"\",\r\n}",5);

  // sign_data
  if (sign_data(data2,0) == 0)
  { 
    SYNC_DELEGATES_DATABASE_ERROR("Could not sign_data",0);
  }
     
  memset(data,0,strlen(data));
  if (send_and_receive_data_socket(data,block_verifiers_ip_address,SEND_DATA_PORT,data2,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
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
  insert_multiple_documents_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,data,MAXIMUM_BUFFER_SIZE,0);

  pointer_reset(data);
  return 1;

  #undef DATABASE_COLLECTION  
  #undef SYNC_DELEGATES_DATABASE_ERROR   
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_check_statistics_database
Description: Checks if the block verifier needs to sync the statistics database
Paramters:
  settings - 1 to sync from a random block verifier, 2 to sync from a random network data node
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_check_statistics_database(int settings)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char data2[BUFFER_SIZE]; 
  char message[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm* current_UTC_date_and_time;
  size_t count;

  // define macros 
  #define DATABASE_COLLECTION "statistics"
  #define SYNC_CHECK_STATISTICS_DATABASE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"sync_check_statistics_database",30); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  pointer_reset(data); \
  return 0;

  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    pointer_reset(data);
    memcpy(error_message.function[error_message.total],"sync_check_statistics_database",30);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,data2);  
    exit(0);
  }

  print_start_message(current_date_and_time,current_UTC_date_and_time,"Checking if the statistics database is synced",data2);

  if (get_synced_block_verifiers() == 0)
  {
    SYNC_CHECK_STATISTICS_DATABASE_ERROR("Could not get the synced block verifiers");
  }

  if (settings == 1)
  {
    // get the database data hash for the reserve proofs database
    if (get_database_data_hash(data,DATABASE_NAME,DATABASE_COLLECTION) == 0)
    {
      SYNC_CHECK_STATISTICS_DATABASE_ERROR("Could not get the database data hash for the statistics database");
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

    fprintf(stderr,"Sending all block verifiers a message to check if the reserve proofs database is synced\n"); 

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

    // get the vote settings of the block verifiers

    // check if a consensus could not be reached and sync from a network data node
    if (synced_block_verifiers.vote_settings_connection_timeout >= BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      color_print("A Consensus could not be reached for trying to sync the statistics database, syncing from a random network data node","red");
      settings = 2;
    }
    else if (synced_block_verifiers.vote_settings_false >= BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      color_print("The statistics database is not synced, syncing from a random block verifier","red");

      // get the data
      if (sync_statistics_database(settings) == 0)
      {
        SYNC_CHECK_STATISTICS_DATABASE_ERROR("Could not sync the statistics database database");
      }
    }
  }
  if (settings == 2)
  {
    fprintf(stderr,"Syncing from a random network data node\n");
    if (sync_statistics_database(settings) == 0)
    {
      SYNC_CHECK_STATISTICS_DATABASE_ERROR("Could not sync the statistics database database");
    }
  }

  color_print("The statistics database is synced","green");
  
  pointer_reset(data);
  return 1;

  #undef DATABASE_COLLECTION  
  #undef SYNC_CHECK_STATISTICS_DATABASE_ERROR  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_statistics_database
Description: Syncs the statistics database
Paramters:
  settings - 1 to sync from a random block verifier, 2 to sync from a random network data node
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_statistics_database(int settings)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  char database_data[BUFFER_SIZE];
  char block_verifiers_ip_address[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm* current_UTC_date_and_time;
  size_t count = 0;
  
  // define macros
  #define DATABASE_COLLECTION "statistics"
  #define SYNC_STATISTICS_DATABASE_ERROR(message,settings) \
  if (settings == 0) \
  { \
    memcpy(error_message.function[error_message.total],"sync_statistics_database",24); \
    memcpy(error_message.data[error_message.total],message,strnlen(message,sizeof(error_message.data[error_message.total]))); \
    error_message.total++; \
    pointer_reset(data); \
    return 0; \
  } \
  else \
  { \
    memset(data,0,strlen(data)); \
    memcpy(data+strlen(data),message,strnlen(message,BUFFER_SIZE)); \
    memcpy(data+strlen(data),block_verifiers_ip_address,strnlen(block_verifiers_ip_address,MAXIMUM_BUFFER_SIZE)); \
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
    print_error_message(current_date_and_time,current_UTC_date_and_time,data2);
    exit(0);
  }

  start:

  memset(data,0,strlen(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));
  memset(database_data,0,sizeof(database_data));
  memset(block_verifiers_ip_address,0,sizeof(block_verifiers_ip_address));

    /* select a random block verifier from the majority vote settings to sync the database from, making sure not to select your own block verifier node
       select a random network data node to sync from if there was a lot of connection_timeouts, to where a majority vote could not be calculated, there were more than BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT new block verifiers
    */

    memset(block_verifiers_ip_address,0,sizeof(block_verifiers_ip_address));
    
    if (settings == 1)
    {
      do
      {
        count = (int)(rand() % BLOCK_VERIFIERS_AMOUNT);
      } while (memcmp(synced_block_verifiers.vote_settings[count],"true",4) != 0 || memcmp(synced_block_verifiers.synced_block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0);
      memcpy(block_verifiers_ip_address,synced_block_verifiers.synced_block_verifiers_IP_address[count],strnlen(synced_block_verifiers.synced_block_verifiers_IP_address[count],sizeof(block_verifiers_ip_address)));
    }
    else if (settings == 2)
    {
      do
      {
        count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT);
      } while (memcmp(network_data_nodes_list.network_data_nodes_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0);
      memcpy(block_verifiers_ip_address,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],sizeof(block_verifiers_ip_address)));
    }



  // get the database data hash for the statistics database
  fprintf(stderr,"Getting the database data from %s\n\n",block_verifiers_ip_address);
  if (get_database_data_hash(data,DATABASE_NAME,DATABASE_COLLECTION) == 0)
  {
    SYNC_STATISTICS_DATABASE_ERROR("Could not get the database data hash for the statistics database from ",1);
  }

  // create the message
  memset(data2,0,sizeof(data2));
  memcpy(data2,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"file\": \"",114);
  snprintf(data2+114,sizeof(data2)-115,"%zu",count);
  memcpy(data2+strlen(data2),"\",\r\n}",5);

  // sign_data
  if (sign_data(data2,0) == 0)
  { 
    SYNC_STATISTICS_DATABASE_ERROR("Could not sign_data",0);
  }
     
  memset(data,0,strlen(data));
  if (send_and_receive_data_socket(data,block_verifiers_ip_address,SEND_DATA_PORT,data2,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
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
  insert_multiple_documents_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,data,MAXIMUM_BUFFER_SIZE,0);

  pointer_reset(data);
  return 1;

  #undef DATABASE_COLLECTION  
  #undef SYNC_STATISTICS_DATABASE_ERROR   
}