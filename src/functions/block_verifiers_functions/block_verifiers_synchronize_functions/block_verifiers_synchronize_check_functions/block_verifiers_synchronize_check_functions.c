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
#include "block_verifiers_synchronize_check_functions.h"
#include "block_verifiers_thread_server_functions.h"
#include "database_functions.h"
#include "read_database_functions.h"
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
Global Define Macros
-----------------------------------------------------------------------------------------------------------
*/
#define SEND_DATABASE_SYNC_CHECK_MESSAGE(database) \
for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++) \
{ \
  if (memcmp(synced_block_verifiers.synced_block_verifiers_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0) \
  { \
    memset(data,0,strlen(data)); \
    memset(data2,0,strlen(data2)); \
    if (send_and_receive_data_socket(data,synced_block_verifiers.synced_block_verifiers_IP_address[count],SEND_DATA_PORT,message,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 0 || verify_data(data,0) == 0) \
    { \
      memcpy(synced_block_verifiers.vote_settings[count],"connection_timeout",18); \
      synced_block_verifiers.vote_settings_connection_timeout++; \
    } \
    else \
    { \
      if (strstr(data,database) != NULL && parse_json_data(data,database,data2,sizeof(data2)) != 0 && memcmp(data2,"true",4) == 0) \
      { \
        synced_block_verifiers.vote_settings_true++; \
      } \
      else \
      { \
        synced_block_verifiers.vote_settings_false++; \
      } \
      memcpy(synced_block_verifiers.vote_settings[count],data2,strnlen(data2,BUFFER_SIZE)); \
    } \
  } \
}



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
  SETTINGS - 1 to sync from a random block verifier, 2 to sync from a random network data node, 3 to sync from a random network data node and not check the majority
  RESERVE_BYTES_START_SETTINGS - 0 to sync all of the reserve bytes databases, 1 to only sync the current reserve bytes database
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int check_if_databases_are_synced(const int SETTINGS, const int RESERVE_BYTES_START_SETTINGS)
{
  // Variables
  char data[BUFFER_SIZE_NETWORK_BLOCK_DATA];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
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
  if (sync_check_reserve_bytes_database(SETTINGS, RESERVE_BYTES_START_SETTINGS) == 0)
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
Name: sync_check_reserve_proofs_database
Description: Checks if the block verifier needs to sync the reserve proofs database
Paramters:
  settings - 1 to sync from a random block verifier, 2 to sync from a random network data node, 3 to sync from a random network data node and not check the majority
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
  struct tm current_UTC_date_and_time;
  int count;

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
    memcpy(error_message.function[error_message.total],"sync_reserve_proofs_database",28);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,data2);  
    exit(0);
  }

  if (test_settings == 0)
  {
    print_start_message(current_date_and_time,current_UTC_date_and_time,"Checking if the reserve proofs database is synced",data2);
  }

  if (get_synced_block_verifiers() == 0)
  {
    SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR("Could not get the synced block verifiers");
  }

  reset_synced_block_verifiers_vote_settings;

  if (settings == 1)
  {
    // get the database data hash for the reserve proofs database
    if (get_database_data_hash(data,database_name,"reserve_proofs") == 0)
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
      snprintf(message+strlen(message),sizeof(message)-1,"%d",count);
      memcpy(message+strlen(message),"\": \"",4);
      // get the database data hash for the reserve proofs database
      memset(data,0,strlen(data));
      memset(data2,0,strlen(data2));  
      memcpy(data2,"reserve_proofs_",15);  
      snprintf(data2+15,sizeof(data2)-16,"%d",count);
      if (get_database_data_hash(data,database_name,data2) == 0)
      {
        SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR("Could not get the database data hash for the reserve proofs database");
      }
      memcpy(message+strlen(message),data,DATA_HASH_LENGTH);
      memcpy(message+strlen(message),"\",\r\n ",5);
    }
    memcpy(message+strlen(message),"}",1);

    // sign_data
    if (sign_data(message) == 0)
    { 
      SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR("Could not sign_data");
    }

    if (test_settings == 0)
    {
      color_print("Sending all block verifiers a message to check if the reserve proofs database is synced","white");
    }
    
    SEND_DATABASE_SYNC_CHECK_MESSAGE("reserve_proofs_database");

    // get the vote settings of the block verifiers

    // check if a consensus could not be reached and sync from a network data node
    if (synced_block_verifiers.vote_settings_connection_timeout >= BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      if (test_settings == 0)
      {
        color_print("A Consensus could not be reached for trying to sync the reserve proofs database, syncing from a random network data node","red");
      }
      settings = 2;
    }
    else if (synced_block_verifiers.vote_settings_false >= BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      if (test_settings == 0)
      {
        color_print("The reserve proofs database is not synced, syncing from a random block verifier","red");
      }

      // get the data
      if (sync_reserve_proofs_database(settings,"") == 0)
      {
        SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR("Could not sync the reserve proofs database");
      }
    }
  }
  if (settings == 2 || settings == 3)
  {
    if (test_settings == 0)
    {
      color_print("Syncing from a random network data node","white");
    }
    if (sync_reserve_proofs_database(settings,"") == 0)
    {
      SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR("Could not sync the reserve proofs database");
    }
  }

  // check to see if the block verifiers database is now in the majority, and if not directly sync the database from the main network data node
  if (settings != 3)
  {
    sync_check_majority_reserve_proofs_database();
  }

  if (test_settings == 0)
  {
    color_print("The reserve proofs database is synced","green");
  }
  
  pointer_reset(data);
  return 1;
  
  #undef SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_check_majority_reserve_proofs_database
Description: Checks if the block verifiers reserve proofs database is in the majority and if not syncs the reserve proofs database from the main network data node
-----------------------------------------------------------------------------------------------------------
*/

void sync_check_majority_reserve_proofs_database(void)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char data2[BUFFER_SIZE]; 
  char message[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;

  // define macros  
  #define SYNC_CHECK_MAJORITY_RESERVE_PROOFS_DATABASE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"sync_check_reserve_proofs_database",34); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  pointer_reset(data); \
  return;

  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    memcpy(error_message.function[error_message.total],"sync_reserve_proofs_database",28);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,data2);  
    exit(0);
  }
  
  if (test_settings == 0)
  {
    color_print("Checking if the database is in the majority","yellow");
  }

  if (get_synced_block_verifiers() == 0)
  {
    SYNC_CHECK_MAJORITY_RESERVE_PROOFS_DATABASE_ERROR("Could not get the synced block verifiers");
  }

  reset_synced_block_verifiers_vote_settings;
 
  // get the database data hash for the reserve proofs database
  if (get_database_data_hash(data,database_name,"reserve_proofs") == 0)
  {
    SYNC_CHECK_MAJORITY_RESERVE_PROOFS_DATABASE_ERROR("Could not get the database data hash for the reserve proofs database");
  }

  // create the message
  memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE\",\r\n \"reserve_proofs_data_hash\": \"",139);
  memcpy(message+strlen(message),data,DATA_HASH_LENGTH);
  memcpy(message+strlen(message),"\",\r\n ",5);

  for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
  {
    memcpy(message+strlen(message),"\"reserve_proofs_data_hash_",26);
    snprintf(message+strlen(message),sizeof(message)-1,"%d",count);
    memcpy(message+strlen(message),"\": \"",4);
    // get the database data hash for the reserve proofs database
    memset(data,0,strlen(data));
    memset(data2,0,strlen(data2));  
    memcpy(data2,"reserve_proofs_",15);  
    snprintf(data2+15,sizeof(data2)-16,"%d",count);
    if (get_database_data_hash(data,database_name,data2) == 0)
    {
      SYNC_CHECK_MAJORITY_RESERVE_PROOFS_DATABASE_ERROR("Could not get the database data hash for the reserve proofs database");
    }
    memcpy(message+strlen(message),data,DATA_HASH_LENGTH);
    memcpy(message+strlen(message),"\",\r\n ",5);
  }
  memcpy(message+strlen(message),"}",1);

  // sign_data
  if (sign_data(message) == 0)
  { 
    SYNC_CHECK_MAJORITY_RESERVE_PROOFS_DATABASE_ERROR("Could not sign_data");
  }
    
  SEND_DATABASE_SYNC_CHECK_MESSAGE("reserve_proofs_database");

  // get the vote settings of the block verifiers

  // check if the block verifiers database is in the majority
  if (synced_block_verifiers.vote_settings_true < BLOCK_VERIFIERS_VALID_AMOUNT)
  {
    if (test_settings == 0)
    {
      color_print("The database is not in the majority, syncing from a random network data node","red");
    }
    get_random_network_data_node(count);
    sync_reserve_proofs_database(count+3,"");    
  }
  
  pointer_reset(data);
  return;
  
  #undef SYNC_CHECK_MAJORITY_RESERVE_PROOFS_DATABASE_ERROR  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_check_reserve_bytes_database
Description: Checks if the block verifier needs to sync the reserve bytes database
Paramters:
  settings - 1 to sync from a random block verifier, 2 to sync from a random network data node, 3 to sync from a random network data node and not check the majority
  RESERVE_BYTES_START_SETTINGS - 0 to sync all of the reserve bytes databases, 1 to only sync the current reserve bytes database, 2 to only check if the current reserve bytes database needs to be synced
Return: 0 if an error has occured, 1 if successfull, 2 to indicate the reserve bytes needs to be synced
-----------------------------------------------------------------------------------------------------------
*/

int sync_check_reserve_bytes_database(int settings, const int RESERVE_BYTES_START_SETTINGS)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char data2[BUFFER_SIZE]; 
  char message[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
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
    memcpy(error_message.function[error_message.total],"sync_reserve_bytes_database",27);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,data2);  
    exit(0);
  }

  if (test_settings == 0)
  {
    print_start_message(current_date_and_time,current_UTC_date_and_time,"Checking if the reserve bytes database is synced",data2);
  }

  if (get_synced_block_verifiers() == 0)
  {
    SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR("Could not get the synced block verifiers");
  }

  reset_synced_block_verifiers_vote_settings;  

  if (settings == 1)
  {
    // get the current reserve bytes database
    get_reserve_bytes_database(current_reserve_bytes_database,0);

    // get the database data hash for the reserve bytes database
    if (get_database_data_hash(data,database_name,"reserve_bytes") == 0)
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
      if (get_database_data_hash(data,database_name,data2) == 0)
      {
        SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR("Could not get the database data hash for the reserve bytes database");
      }
      memcpy(message+strlen(message),data,DATA_HASH_LENGTH);
      memcpy(message+strlen(message),"\",\r\n ",5);
    }
    memcpy(message+strlen(message),"}",1);

    // sign_data
    if (sign_data(message) == 0)
    { 
      SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR("Could not sign_data");
    }

    if (test_settings == 0)
    {
      color_print("Sending all block verifiers a message to check if the reserve bytes database is synced","white");
    }
    
    SEND_DATABASE_SYNC_CHECK_MESSAGE("reserve_bytes_database");

    // get the vote settings of the block verifiers

    // check if a consensus could not be reached and sync from a network data node
    if (synced_block_verifiers.vote_settings_connection_timeout >= BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      if (test_settings == 0)
      {
        color_print("A Consensus could not be reached for trying to sync the reserve bytes database, syncing from a random network data node","red");
      }
      if (RESERVE_BYTES_START_SETTINGS == 2)
      {
        return 2;
      }
      settings = 2;
    }
    else if (synced_block_verifiers.vote_settings_false >= BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      if (RESERVE_BYTES_START_SETTINGS == 2)
      {
        return 2;
      }
      color_print("The reserve bytes database is not synced, syncing from a random block verifier","red");

      // get the data
      if (sync_reserve_bytes_database(settings, RESERVE_BYTES_START_SETTINGS,"") == 0)
      {
        SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR("Could not sync the reserve bytes database");
      }
    }
  }
  if (settings == 2 || settings == 3)
  {
    if (test_settings == 0)
    {
      color_print("Syncing from a random network data node","white");
    }
    if (sync_reserve_bytes_database(settings, RESERVE_BYTES_START_SETTINGS,"") == 0)
    {
      SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR("Could not sync the reserve bytes database");
    }
  }

  // check to see if the block verifiers database is now in the majority, and if not directly sync the database from the main network data node
  if ((settings == 1 && RESERVE_BYTES_START_SETTINGS != 2) || (settings == 2))
  {
    sync_check_majority_reserve_bytes_database(RESERVE_BYTES_START_SETTINGS);
  }

  if (test_settings == 0)
  {
    color_print("The reserve bytes database is synced","green");
  }
  
  pointer_reset(data);
  return 1;
  
  #undef SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_check_majority_reserve_bytes_database
Description: Checks if the block verifiers reserve bytes database is in the majority and if not syncs the reserve bytes database from the main network data node
Paramters:
  RESERVE_BYTES_START_SETTINGS - 0 to sync all of the reserve bytes databases, 1 to only sync the current reserve bytes database
-----------------------------------------------------------------------------------------------------------
*/

void sync_check_majority_reserve_bytes_database(const int RESERVE_BYTES_START_SETTINGS)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char data2[BUFFER_SIZE]; 
  char message[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  size_t count;
  size_t current_reserve_bytes_database;

  // define macros  
  #define SYNC_CHECK_MAJORITY_RESERVE_BYTES_DATABASE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"sync_check_reserve_bytes_database",33); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  pointer_reset(data); \
  return;
  
  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    memcpy(error_message.function[error_message.total],"sync_reserve_bytes_database",27);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,data2);  
    exit(0);
  }

  if (test_settings == 0)
  {
    color_print("Checking if the database is in the majority","yellow");
  }

  if (get_synced_block_verifiers() == 0)
  {
    SYNC_CHECK_MAJORITY_RESERVE_BYTES_DATABASE_ERROR("Could not get the synced block verifiers");
  }

  reset_synced_block_verifiers_vote_settings;

  // get the current reserve bytes database
  get_reserve_bytes_database(current_reserve_bytes_database,0);

  // get the database data hash for the reserve bytes database
  if (get_database_data_hash(data,database_name,"reserve_bytes") == 0)
  {
    SYNC_CHECK_MAJORITY_RESERVE_BYTES_DATABASE_ERROR("Could not get the database data hash for the reserve bytes database");
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
    if (get_database_data_hash(data,database_name,data2) == 0)
    {
      SYNC_CHECK_MAJORITY_RESERVE_BYTES_DATABASE_ERROR("Could not get the database data hash for the reserve bytes database");
    }
    memcpy(message+strlen(message),data,DATA_HASH_LENGTH);
    memcpy(message+strlen(message),"\",\r\n ",5);
  }
  memcpy(message+strlen(message),"}",1);

  // sign_data
  if (sign_data(message) == 0)
  { 
    SYNC_CHECK_MAJORITY_RESERVE_BYTES_DATABASE_ERROR("Could not sign_data");
  }

  SEND_DATABASE_SYNC_CHECK_MESSAGE("reserve_bytes_database");

  // get the vote settings of the block verifiers

  // check if the block verifiers database is in the majority
  if (synced_block_verifiers.vote_settings_true < BLOCK_VERIFIERS_VALID_AMOUNT)
  {
    if (test_settings == 0)
    {
      color_print("The database is not in the majority, syncing from a random network data node","red");
    }
    get_random_network_data_node(count);
    sync_reserve_bytes_database(count+3,RESERVE_BYTES_START_SETTINGS,""); 
  }
  
  pointer_reset(data);
  return;
  
  #undef SYNC_CHECK_MAJORITY_RESERVE_BYTES_DATABASE_ERROR  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_check_delegates_database
Description: Checks if the block verifier needs to sync the delegates database
Paramters:
  settings - 1 to sync from a random block verifier, 2 to sync from a random network data node, 3 to sync from a random network data node and not check the majority
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_check_delegates_database(int settings)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char data2[BUFFER_SIZE_NETWORK_BLOCK_DATA]; 
  char message[BUFFER_SIZE_NETWORK_BLOCK_DATA];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;

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
    memcpy(error_message.function[error_message.total],"sync_check_delegates_database",29);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,data2);  
    exit(0);
  }

  if (test_settings == 0)
  {
    print_start_message(current_date_and_time,current_UTC_date_and_time,"Checking if the delegates database is synced",data2);
  }

  if (get_synced_block_verifiers() == 0)
  {
    SYNC_CHECK_DELEGATES_DATABASE_ERROR("Could not get the synced block verifiers");
  }

  reset_synced_block_verifiers_vote_settings;

  if (settings == 1)
  {
    // get the database data hash for the delegates database
    if (get_database_data_hash(data,database_name,DATABASE_COLLECTION) == 0)
    {
      SYNC_CHECK_DELEGATES_DATABASE_ERROR("Could not get the database data hash for the delegates database");
    }

    // create the message
    memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"data_hash\": \"",115);
    memcpy(message+115,data,DATA_HASH_LENGTH);
    memcpy(message+243,"\",\r\n}",5);

    // sign_data
    if (sign_data(message) == 0)
    { 
      SYNC_CHECK_DELEGATES_DATABASE_ERROR("Could not sign_data");
    }

    if (test_settings == 0)
    {
      color_print("Sending all block verifiers a message to check if the delegates database is synced","white"); 
    }

    SEND_DATABASE_SYNC_CHECK_MESSAGE("delegates_database");

    // get the vote settings of the block verifiers

    // check if a consensus could not be reached and sync from a network data node
    if (synced_block_verifiers.vote_settings_connection_timeout >= BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      if (test_settings == 0)
      {
        color_print("A Consensus could not be reached for trying to sync the delegates database, syncing from a random network data node","red");
      }
      settings = 2;
    }
    else if (synced_block_verifiers.vote_settings_false >= BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      if (test_settings == 0)
      {
        color_print("The delegates database is not synced, syncing from a random block verifier","red");
      }

      // get the data
      if (sync_delegates_database(settings,"") == 0)
      {
        SYNC_CHECK_DELEGATES_DATABASE_ERROR("Could not sync the delegates database database");
      }
    }
  }
  if (settings == 2 || settings == 3)
  {
    if (test_settings == 0)
    {
      color_print("Syncing from a random network data node","white");
    }
    if (sync_delegates_database(settings,"") == 0)
    {
      SYNC_CHECK_DELEGATES_DATABASE_ERROR("Could not sync the delegates database database");
    }
  }

  // check to see if the block verifiers database is now in the majority, and if not directly sync the database from the main network data node
  if (settings != 3)
  {
    sync_check_majority_delegates_database();
  }

  if (test_settings == 0)
  {
    color_print("The delegates database is synced","green");
  }
  
  pointer_reset(data);
  return 1;

  #undef DATABASE_COLLECTION  
  #undef SYNC_CHECK_DELEGATES_DATABASE_ERROR  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_check_majority_delegates_database
Description: Checks if the block verifiers delegates database is in the majority and if not syncs the delegates database from the main network data node
-----------------------------------------------------------------------------------------------------------
*/

void sync_check_majority_delegates_database(void)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char data2[BUFFER_SIZE_NETWORK_BLOCK_DATA]; 
  char message[BUFFER_SIZE_NETWORK_BLOCK_DATA];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;

  // define macros 
  #define DATABASE_COLLECTION "delegates"
  #define SYNC_CHECK_MAJORITY_DELEGATES_DATABASE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"sync_check_delegates_database",29); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  pointer_reset(data); \
  return;

  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    memcpy(error_message.function[error_message.total],"sync_check_delegates_database",29);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,data2);  
    exit(0);
  }

  if (test_settings == 0)
  {
    color_print("Checking if the database is in the majority","yellow");
  }

  if (get_synced_block_verifiers() == 0)
  {
    SYNC_CHECK_MAJORITY_DELEGATES_DATABASE_ERROR("Could not get the synced block verifiers");
  }

  reset_synced_block_verifiers_vote_settings;

  // get the database data hash for the reserve proofs database
  if (get_database_data_hash(data,database_name,DATABASE_COLLECTION) == 0)
  {
    SYNC_CHECK_MAJORITY_DELEGATES_DATABASE_ERROR("Could not get the database data hash for the delegates database");
  }

  // create the message
  memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"data_hash\": \"",115);
  memcpy(message+115,data,DATA_HASH_LENGTH);
  memcpy(message+243,"\",\r\n}",5);

  // sign_data
  if (sign_data(message) == 0)
  { 
    SYNC_CHECK_MAJORITY_DELEGATES_DATABASE_ERROR("Could not sign_data");
  }

  SEND_DATABASE_SYNC_CHECK_MESSAGE("delegates_database");

  // get the vote settings of the block verifiers

  // check if the block verifiers database is in the majority
  if (synced_block_verifiers.vote_settings_true < BLOCK_VERIFIERS_VALID_AMOUNT)
  {
    if (test_settings == 0)
    {
      color_print("The database is not in the majority, syncing from a random network data node","red");
    }
    get_random_network_data_node(count);
    sync_delegates_database(count+3,"");   
  }
  
  pointer_reset(data);
  return;

  #undef DATABASE_COLLECTION  
  #undef SYNC_CHECK_MAJORITY_DELEGATES_DATABASE_ERROR  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_check_statistics_database
Description: Checks if the block verifier needs to sync the statistics database
Paramters:
  settings - 1 to sync from a random block verifier, 2 to sync from a random network data node, 3 to sync from a random network data node and not check the majority
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_check_statistics_database(int settings)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char data2[BUFFER_SIZE_NETWORK_BLOCK_DATA]; 
  char message[BUFFER_SIZE_NETWORK_BLOCK_DATA];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;

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
    memcpy(error_message.function[error_message.total],"sync_check_statistics_database",30);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,data2);  
    exit(0);
  }

  if (test_settings == 0)
  {
    print_start_message(current_date_and_time,current_UTC_date_and_time,"Checking if the statistics database is synced",data2);
  }

  if (get_synced_block_verifiers() == 0)
  {
    SYNC_CHECK_STATISTICS_DATABASE_ERROR("Could not get the synced block verifiers");
  }

  reset_synced_block_verifiers_vote_settings;

  if (settings == 1)
  {
    // get the database data hash for the reserve proofs database
    if (get_database_data_hash(data,database_name,DATABASE_COLLECTION) == 0)
    {
      SYNC_CHECK_STATISTICS_DATABASE_ERROR("Could not get the database data hash for the statistics database");
    }

    // create the message
    memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"data_hash\": \"",116);
    memcpy(message+116,data,DATA_HASH_LENGTH);
    memcpy(message+244,"\",\r\n}",5);

    // sign_data
    if (sign_data(message) == 0)
    { 
      SYNC_CHECK_STATISTICS_DATABASE_ERROR("Could not sign_data");
    }

    if (test_settings == 0)
    {
      color_print("Sending all block verifiers a message to check if the statistics database is synced","white"); 
    }

    SEND_DATABASE_SYNC_CHECK_MESSAGE("statistics_database");

    // get the vote settings of the block verifiers

    // check if a consensus could not be reached and sync from a network data node
    if (synced_block_verifiers.vote_settings_connection_timeout >= BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      if (test_settings == 0)
      {
        color_print("A Consensus could not be reached for trying to sync the statistics database, syncing from a random network data node","red");
      }
      settings = 2;
    }
    else if (synced_block_verifiers.vote_settings_false >= BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      if (test_settings == 0)
      {
        color_print("The statistics database is not synced, syncing from a random block verifier","red");
      }

      // get the data
      if (sync_statistics_database(settings,"") == 0)
      {
        SYNC_CHECK_STATISTICS_DATABASE_ERROR("Could not sync the statistics database database");
      }
    }
  }
  if (settings == 2 || settings == 3)
  {
    if (test_settings == 0)
    {
      color_print("Syncing from a random network data node","white");
    }
    if (sync_statistics_database(settings,"") == 0)
    {
      SYNC_CHECK_STATISTICS_DATABASE_ERROR("Could not sync the statistics database database");
    }
  }

  // check to see if the block verifiers database is now in the majority, and if not directly sync the database from the main network data node
  if (settings != 3)
  {
    sync_check_majority_statistics_database();
  }

  if (test_settings == 0)
  {
    color_print("The statistics database is synced","green");
  }
  
  pointer_reset(data);
  return 1;

  #undef DATABASE_COLLECTION  
  #undef SYNC_CHECK_STATISTICS_DATABASE_ERROR  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_check_majority_statistics_database
Description: Checks if the block verifiers statistics database is in the majority and if not syncs the statistics database from the main network data node
-----------------------------------------------------------------------------------------------------------
*/

void sync_check_majority_statistics_database(void)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char data2[BUFFER_SIZE_NETWORK_BLOCK_DATA]; 
  char message[BUFFER_SIZE_NETWORK_BLOCK_DATA];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;

  // define macros 
  #define DATABASE_COLLECTION "statistics"
  #define SYNC_CHECK_MAJORITY_STATISTICS_DATABASE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"sync_check_statistics_database",30); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  pointer_reset(data); \
  return;

  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    memcpy(error_message.function[error_message.total],"sync_check_statistics_database",30);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,data2);  
    exit(0);
  }

  if (test_settings == 0)
  { 
    color_print("Checking if the database is in the majority","yellow");
  }

  if (get_synced_block_verifiers() == 0)
  {
    SYNC_CHECK_MAJORITY_STATISTICS_DATABASE_ERROR("Could not get the synced block verifiers");
  }

  reset_synced_block_verifiers_vote_settings;
  
  // get the database data hash for the reserve proofs database
  if (get_database_data_hash(data,database_name,DATABASE_COLLECTION) == 0)
  {
    SYNC_CHECK_MAJORITY_STATISTICS_DATABASE_ERROR("Could not get the database data hash for the statistics database");
  }

  // create the message
  memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"data_hash\": \"",116);
  memcpy(message+116,data,DATA_HASH_LENGTH);
  memcpy(message+244,"\",\r\n}",5);

  // sign_data
  if (sign_data(message) == 0)
  { 
    SYNC_CHECK_MAJORITY_STATISTICS_DATABASE_ERROR("Could not sign_data");
  }

  SEND_DATABASE_SYNC_CHECK_MESSAGE("statistics_database");

  // get the vote settings of the block verifiers

  // check if the block verifiers database is in the majority
  if (synced_block_verifiers.vote_settings_true < BLOCK_VERIFIERS_VALID_AMOUNT)
  {
    if (test_settings == 0)
    {
      color_print("The database is not in the majority, syncing from a random network data node","red");
    }
    get_random_network_data_node(count);
    sync_statistics_database(count+3,"");     
  }
  
  pointer_reset(data);
  return;

  #undef DATABASE_COLLECTION  
  #undef SYNC_CHECK_MAJORITY_STATISTICS_DATABASE_ERROR  
}

#undef SEND_DATABASE_SYNC_CHECK_MESSAGE