#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <resolv.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"
#include "initialize_and_reset_structs_define_macros.h"
#include "define_macros_test.h"

#include "blockchain_functions.h"
#include "block_verifiers_functions.h"
#include "block_verifiers_synchronize_server_functions.h"
#include "block_verifiers_synchronize_functions.h"
#include "block_verifiers_thread_server_functions.h"
#include "block_verifiers_update_functions.h"
#include "read_database_functions.h"
#include "update_database_functions.h"
#include "file_functions.h"
#include "network_daemon_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "network_wallet_functions.h"
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
Name: get_block_verifiers_from_network_block
Description: Gets the block verifiers from the network block
Parameters:
  TOTAL_DELEGATES - The total delegates
  struct delegates - struct delegates
  BLOCK_HEIGHT - The block height
  SETTINGS - 0 for current block verifiers, 1 for previous block verifiers
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int get_block_verifiers_from_network_block(const int TOTAL_DELEGATES, struct delegates* delegates, const size_t CURRENT_BLOCK_HEIGHT, const int SETTINGS)
{
  // Variables 
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  char message[BUFFER_SIZE];
  char* message_copy1;
  int count;
  int count2;
  int count3;
  size_t block_height;

  // define macros
  #define GET_BLOCK_VERIFIERS_FROM_NETWORK_BLOCK_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"get_block_verifiers_from_network_block",38); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));
  memset(message,0,sizeof(message));

  // get the block height
  block_height = SETTINGS == 0 ? CURRENT_BLOCK_HEIGHT - 1 : CURRENT_BLOCK_HEIGHT - 2;

  // create the message  
  memcpy(data,"{\"block_height\":\"",17);
  snprintf(data+17,sizeof(data)-18,"%zu",block_height);
  memcpy(data+strlen(data),"\"}",2);

  // get the reserve bytes database for the block
  if (block_height-1 == XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT || block_height == XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT)
  {
    count = 1;
  }
  else
  {
    count = ((block_height - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME) + 1;
  }

  // get the blocks reserve bytes
  memcpy(data3,"reserve_bytes_",14);
  snprintf(data3+14,sizeof(data3)-15,"%d",count);
  if (read_document_field_from_collection(database_name,data3,data,"reserve_bytes",message) == 0)
  {
    // restore the database from the backup
    count = system("cd ~ && mongorestore");
    GET_BLOCK_VERIFIERS_FROM_NETWORK_BLOCK_ERROR("Could not get the previous blocks reserve bytes, restoring the database from the backup");
  }
  
  // get the next block verifiers public keys from the previous network blocks reserve bytes
  message_copy1 = message;
  message_copy1 = strstr(message_copy1,BLOCKCHAIN_DATA_SEGMENT_PUBLIC_ADDRESS_STRING_DATA) + (sizeof(BLOCKCHAIN_DATA_SEGMENT_PUBLIC_ADDRESS_STRING_DATA)-1);
  
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {  
    memset(data,0,sizeof(data)); 
    memset(data2,0,sizeof(data2)); 
    memset(data3,0,sizeof(data3)); 
    memcpy(data,message_copy1,VRF_PUBLIC_KEY_LENGTH*2);
    message_copy1 += (VRF_PUBLIC_KEY_LENGTH*2) + (sizeof(BLOCKCHAIN_DATA_SEGMENT_PUBLIC_ADDRESS_STRING_DATA)-1);
              
    // convert the hexadecimal string to a string
    for (count2 = 0, count3 = 0; count2 < VRF_PUBLIC_KEY_LENGTH*2; count3++, count2 += 2)
    {
      memset(data2,0,sizeof(data2));
      memcpy(data2,&data[count2],2);
      data3[count3] = (int)strtol(data2, NULL, 16);
    }
    SETTINGS == 0 ? memcpy(current_block_verifiers_list.block_verifiers_public_key[count],data3,strnlen(data3,sizeof(current_block_verifiers_list.block_verifiers_public_key[count]))) : memcpy(previous_block_verifiers_list.block_verifiers_public_key[count],data3,strnlen(data3,sizeof(previous_block_verifiers_list.block_verifiers_public_key[count])));
  }

  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    for (count2 = 0; count2 < TOTAL_DELEGATES; count2++)
    {
      if (SETTINGS == 0)
      {
        if (strncmp(current_block_verifiers_list.block_verifiers_public_key[count],delegates[count2].public_key,BUFFER_SIZE) == 0)
        {
          memcpy(current_block_verifiers_list.block_verifiers_name[count],delegates[count2].delegate_name,strnlen(delegates[count2].delegate_name,sizeof(current_block_verifiers_list.block_verifiers_name[count])));
          memcpy(current_block_verifiers_list.block_verifiers_IP_address[count],delegates[count2].IP_address,strnlen(delegates[count2].IP_address,sizeof(current_block_verifiers_list.block_verifiers_IP_address[count])));
          memcpy(current_block_verifiers_list.block_verifiers_public_address[count],delegates[count2].public_address,strnlen(delegates[count2].public_address,sizeof(current_block_verifiers_list.block_verifiers_public_address[count])));
        }
      }
      else if (SETTINGS == 1)
      {
        if (strncmp(previous_block_verifiers_list.block_verifiers_public_key[count],delegates[count2].public_key,BUFFER_SIZE) == 0)
        {
          memcpy(previous_block_verifiers_list.block_verifiers_name[count],delegates[count2].delegate_name,strnlen(delegates[count2].delegate_name,sizeof(previous_block_verifiers_list.block_verifiers_name[count])));
          memcpy(previous_block_verifiers_list.block_verifiers_IP_address[count],delegates[count2].IP_address,strnlen(delegates[count2].IP_address,sizeof(previous_block_verifiers_list.block_verifiers_IP_address[count])));
          memcpy(previous_block_verifiers_list.block_verifiers_public_address[count],delegates[count2].public_address,strnlen(delegates[count2].public_address,sizeof(previous_block_verifiers_list.block_verifiers_public_address[count])));
        }
      }
    }
  }

  return 1;
  
  #undef GET_BLOCK_VERIFIERS_FROM_NETWORK_BLOCK_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: update_block_verifiers_list
Description: Updates the block verifiers list struct
Return: 0 if an error has occured, 1 to sync from a random block verifier, 2 to sync from a random network data node
-----------------------------------------------------------------------------------------------------------
*/

int update_block_verifiers_list(void)
{
  // Variables 
  char data[1024];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  struct delegates delegates[MAXIMUM_AMOUNT_OF_DELEGATES];
  int count;
  int count2;
  int settings = 0;
  int total_delegates = 0;
  size_t current_block_height_count;

  // define macros
  #define UPDATE_BLOCK_VERIFIERS_LIST_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"update_block_verifiers_list",27); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  POINTER_RESET_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES); \
  return 0;
  
  // initialize the delegates struct
  INITIALIZE_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES,"update_block_verifiers_list",data,current_date_and_time,current_UTC_date_and_time);

  // organize the delegates
  if ((total_delegates = organize_delegates(delegates)) == 0)
  {
    POINTER_RESET_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES);
    UPDATE_BLOCK_VERIFIERS_LIST_ERROR("Could not organize the delegates");
  }
  else if (total_delegates > BLOCK_VERIFIERS_TOTAL_AMOUNT)
  {
    total_delegates = BLOCK_VERIFIERS_TOTAL_AMOUNT;
  }

  // get the current block height
  sscanf(current_block_height,"%zu", &current_block_height_count);

  /*
  load the next block verifiers from the delegates database in order of vote count
  load the current block verifiers from the public addresses in the previous block
  load the previous block verifiers from the public addresses in the previous block before the previous block
  */

  if (current_block_height_count == XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT)
  {
    // load all of block verifiers from the current delegate database since there are no previous blocks
    for (count = 0; count < total_delegates; count++)
    {
      memset(previous_block_verifiers_list.block_verifiers_name[count],0,sizeof(previous_block_verifiers_list.block_verifiers_name[count]));
      memset(previous_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_public_address[count]));
      memset(previous_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(previous_block_verifiers_list.block_verifiers_public_key[count]));
      memset(previous_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_IP_address[count]));
      memset(current_block_verifiers_list.block_verifiers_name[count],0,sizeof(current_block_verifiers_list.block_verifiers_name[count]));
      memset(current_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_public_address[count]));
      memset(current_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(current_block_verifiers_list.block_verifiers_public_key[count]));
      memset(current_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_IP_address[count]));
      memset(next_block_verifiers_list.block_verifiers_name[count],0,sizeof(next_block_verifiers_list.block_verifiers_name[count]));
      memset(next_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_public_address[count]));
      memset(next_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(next_block_verifiers_list.block_verifiers_public_key[count]));
      memset(next_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_IP_address[count]));

      memcpy(previous_block_verifiers_list.block_verifiers_name[count],delegates[count].delegate_name,strnlen(delegates[count].delegate_name,sizeof(previous_block_verifiers_list.block_verifiers_name[count])));
      memcpy(previous_block_verifiers_list.block_verifiers_public_address[count],delegates[count].public_address,strnlen(delegates[count].public_address,sizeof(previous_block_verifiers_list.block_verifiers_public_address[count])));
      memcpy(previous_block_verifiers_list.block_verifiers_public_key[count],delegates[count].public_key,strnlen(delegates[count].public_key,sizeof(previous_block_verifiers_list.block_verifiers_public_key[count])));
      memcpy(previous_block_verifiers_list.block_verifiers_IP_address[count],delegates[count].IP_address,strnlen(delegates[count].IP_address,sizeof(previous_block_verifiers_list.block_verifiers_IP_address[count])));
      memcpy(current_block_verifiers_list.block_verifiers_name[count],delegates[count].delegate_name,strnlen(delegates[count].delegate_name,sizeof(current_block_verifiers_list.block_verifiers_name[count])));
      memcpy(current_block_verifiers_list.block_verifiers_public_address[count],delegates[count].public_address,strnlen(delegates[count].public_address,sizeof(current_block_verifiers_list.block_verifiers_public_address[count])));
      memcpy(current_block_verifiers_list.block_verifiers_public_key[count],delegates[count].public_key,strnlen(delegates[count].public_key,sizeof(current_block_verifiers_list.block_verifiers_public_key[count])));
      memcpy(current_block_verifiers_list.block_verifiers_IP_address[count],delegates[count].IP_address,strnlen(delegates[count].IP_address,sizeof(current_block_verifiers_list.block_verifiers_IP_address[count])));
      memcpy(next_block_verifiers_list.block_verifiers_name[count],delegates[count].delegate_name,strnlen(delegates[count].delegate_name,sizeof(next_block_verifiers_list.block_verifiers_name[count])));
      memcpy(next_block_verifiers_list.block_verifiers_public_address[count],delegates[count].public_address,strnlen(delegates[count].public_address,sizeof(next_block_verifiers_list.block_verifiers_public_address[count])));
      memcpy(next_block_verifiers_list.block_verifiers_public_key[count],delegates[count].public_key,strnlen(delegates[count].public_key,sizeof(next_block_verifiers_list.block_verifiers_public_key[count])));
      memcpy(next_block_verifiers_list.block_verifiers_IP_address[count],delegates[count].IP_address,strnlen(delegates[count].IP_address,sizeof(next_block_verifiers_list.block_verifiers_IP_address[count])));
    }    
  }
  else if (current_block_height_count == XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT+1)
  {
    // get the next block verifiers from the database by vote count
    for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++)
    {
      memset(next_block_verifiers_list.block_verifiers_name[count],0,sizeof(next_block_verifiers_list.block_verifiers_name[count]));
      memset(next_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_public_address[count]));
      memset(next_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(next_block_verifiers_list.block_verifiers_public_key[count]));
      memset(next_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_IP_address[count]));

      memcpy(next_block_verifiers_list.block_verifiers_name[count],delegates[count].delegate_name,strnlen(delegates[count].delegate_name,sizeof(next_block_verifiers_list.block_verifiers_name[count])));
      memcpy(next_block_verifiers_list.block_verifiers_public_address[count],delegates[count].public_address,strnlen(delegates[count].public_address,sizeof(next_block_verifiers_list.block_verifiers_public_address[count])));
      memcpy(next_block_verifiers_list.block_verifiers_public_key[count],delegates[count].public_key,strnlen(delegates[count].public_key,sizeof(next_block_verifiers_list.block_verifiers_public_key[count])));
      memcpy(next_block_verifiers_list.block_verifiers_IP_address[count],delegates[count].IP_address,strnlen(delegates[count].IP_address,sizeof(next_block_verifiers_list.block_verifiers_IP_address[count])));
    }

    // copy the current block verifiers to the previous block verifiers
    for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++)
    {
      memset(previous_block_verifiers_list.block_verifiers_name[count],0,sizeof(previous_block_verifiers_list.block_verifiers_name[count]));
      memset(previous_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_public_address[count]));
      memset(previous_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(previous_block_verifiers_list.block_verifiers_public_key[count]));
      memset(previous_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_IP_address[count]));

      memcpy(previous_block_verifiers_list.block_verifiers_name[count],current_block_verifiers_list.block_verifiers_name[count],strnlen(current_block_verifiers_list.block_verifiers_name[count],sizeof(previous_block_verifiers_list.block_verifiers_name[count])));
      memcpy(previous_block_verifiers_list.block_verifiers_public_address[count],current_block_verifiers_list.block_verifiers_public_address[count],strnlen(current_block_verifiers_list.block_verifiers_public_address[count],sizeof(previous_block_verifiers_list.block_verifiers_public_address[count])));
      memcpy(previous_block_verifiers_list.block_verifiers_public_key[count],current_block_verifiers_list.block_verifiers_public_key[count],strnlen(current_block_verifiers_list.block_verifiers_public_key[count],sizeof(previous_block_verifiers_list.block_verifiers_public_key[count])));
      memcpy(previous_block_verifiers_list.block_verifiers_IP_address[count],current_block_verifiers_list.block_verifiers_IP_address[count],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count],sizeof(previous_block_verifiers_list.block_verifiers_IP_address[count])));
    }

    // load the current block verifiers from the previous block
    for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++)
    {
      memset(current_block_verifiers_list.block_verifiers_name[count],0,sizeof(current_block_verifiers_list.block_verifiers_name[count]));
      memset(current_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_public_address[count]));
      memset(current_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(current_block_verifiers_list.block_verifiers_public_key[count]));
      memset(current_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_IP_address[count]));
    }

    if (get_block_verifiers_from_network_block(total_delegates,delegates,current_block_height_count,0) == 0)
    {
      POINTER_RESET_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES);
      UPDATE_BLOCK_VERIFIERS_LIST_ERROR("Could not organize the delegates");
    }
  }
  else
  {
    // get the next block verifiers from the database by vote count
    for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++)
    {
      memset(next_block_verifiers_list.block_verifiers_name[count],0,sizeof(next_block_verifiers_list.block_verifiers_name[count]));
      memset(next_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_public_address[count]));
      memset(next_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(next_block_verifiers_list.block_verifiers_public_key[count]));
      memset(next_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_IP_address[count]));

      memcpy(next_block_verifiers_list.block_verifiers_name[count],delegates[count].delegate_name,strnlen(delegates[count].delegate_name,sizeof(next_block_verifiers_list.block_verifiers_name[count])));
      memcpy(next_block_verifiers_list.block_verifiers_public_address[count],delegates[count].public_address,strnlen(delegates[count].public_address,sizeof(next_block_verifiers_list.block_verifiers_public_address[count])));
      memcpy(next_block_verifiers_list.block_verifiers_public_key[count],delegates[count].public_key,strnlen(delegates[count].public_key,sizeof(next_block_verifiers_list.block_verifiers_public_key[count])));
      memcpy(next_block_verifiers_list.block_verifiers_IP_address[count],delegates[count].IP_address,strnlen(delegates[count].IP_address,sizeof(next_block_verifiers_list.block_verifiers_IP_address[count])));
    }

    // load the current block verifiers from the previous block
    for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++)
    {
      memset(current_block_verifiers_list.block_verifiers_name[count],0,sizeof(current_block_verifiers_list.block_verifiers_name[count]));
      memset(current_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_public_address[count]));
      memset(current_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(current_block_verifiers_list.block_verifiers_public_key[count]));
      memset(current_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_IP_address[count]));
    }

    if (get_block_verifiers_from_network_block(total_delegates,delegates,current_block_height_count,0) == 0)
    {
      POINTER_RESET_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES);
      UPDATE_BLOCK_VERIFIERS_LIST_ERROR("Could not organize the delegates");
    }

    // load the previous block verifiers from the previous block before the previous block
    for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++)
    {
      memset(previous_block_verifiers_list.block_verifiers_name[count],0,sizeof(previous_block_verifiers_list.block_verifiers_name[count]));
      memset(previous_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_public_address[count]));
      memset(previous_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(previous_block_verifiers_list.block_verifiers_public_key[count]));
      memset(previous_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_IP_address[count]));
    }

    if (get_block_verifiers_from_network_block(total_delegates,delegates,current_block_height_count,1) == 0)
    {
      POINTER_RESET_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES);
      UPDATE_BLOCK_VERIFIERS_LIST_ERROR("Could not organize the delegates");
    }
  }

  // check if more than BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT are new block verifiers
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
    {
      if (strncmp(previous_block_verifiers_list.block_verifiers_public_address[count],current_block_verifiers_list.block_verifiers_public_address[count2],XCASH_WALLET_LENGTH) == 0)
      {
        settings++;
        break;
      }
    }
  }
  settings = settings > (BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT) ? 1 : 2;

  POINTER_RESET_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES);

  return settings;
  
  #undef UPDATE_BLOCK_VERIFIERS_LIST_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: update_databases
Description: Updates the databases
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int update_databases(void)
{
  // Variables
  char data[BUFFER_SIZE_NETWORK_BLOCK_DATA];
  size_t count;

  // define macros
  #define UPDATE_DATABASE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"update_databases",16); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));
  
  // get the previous block height
  sscanf(current_block_height, "%zu", &count);
  if (count < XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT-1)
  {
    UPDATE_DATABASE_ERROR("Could not get the current block height");
  }
  count--;
  snprintf(data,sizeof(data)-1,"%zu",count); 

  if (add_block_verifiers_round_statistics((const char*)data) == 0)
  {
    UPDATE_DATABASE_ERROR("Could not update the block verifiers round statistics");
  }

  if (add_round_statistics() == 0)
  {
    UPDATE_DATABASE_ERROR("Could not update the round statistics");
  }
  return 1;
  
  #undef UPDATE_DATABASE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: add_block_verifiers_round_statistics
Description: Adds the block verifier statistics to the database after adding the block to the network
Parameters:
  BLOCK_HEIGHT - The block height
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int add_block_verifiers_round_statistics(const char* BLOCK_HEIGHT)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char message[BUFFER_SIZE];
  int count;
  size_t number;
  size_t block_verifier_total_rounds;
  size_t block_verifier_online_total_rounds;

  // define macros
  #define DATABASE_COLLECTION "delegates"
  
  #define ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"add_block_verifiers_round_statistics",36); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));

  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    // create the message
    memset(message,0,sizeof(message));
    memcpy(message,"{\"public_address\":\"",19);
    memcpy(message+19,current_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH);
    memcpy(message+19+XCASH_WALLET_LENGTH,"\"}",2);

    // add one to the block_verifier_total_rounds for every block verifier
    memset(data,0,sizeof(data));
    if (read_document_field_from_collection(database_name,DATABASE_COLLECTION,message,"block_verifier_total_rounds",data) == 0)
    {
      ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not read the block_verifier_total_rounds from the database");
    }
    sscanf(data, "%zu", &block_verifier_total_rounds);
    block_verifier_total_rounds++;
    memset(data,0,sizeof(data));
    memcpy(data,"{\"block_verifier_total_rounds\":\"",32);
    snprintf(data+32,sizeof(data)-33,"%zu",block_verifier_total_rounds); 
    memcpy(data+strlen(data),"\"}",2);
    if (update_document_from_collection(database_name,DATABASE_COLLECTION,message,data) == 0)
    {
      ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not update the block_verifier_total_rounds in the database");
    }

    // for the block verifiers that were online add one to the block_verifier_online_total_rounds
    memset(data,0,sizeof(data));
    memset(data2,0,sizeof(data2));
    if (read_document_field_from_collection(database_name,DATABASE_COLLECTION,message,"block_verifier_online_total_rounds",data) == 0)
    {
      ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not read the block_verifier_online_total_rounds from the database");
    }
    sscanf(data, "%zu", &block_verifier_online_total_rounds);

    if (read_document_field_from_collection(database_name,DATABASE_COLLECTION,message,"online_status",data2) == 1 && strncmp(data2,"true",BUFFER_SIZE) == 0)
    { 
      block_verifier_online_total_rounds++;
      memset(data,0,sizeof(data));
      memcpy(data,"{\"block_verifier_online_total_rounds\":\"",39);
      snprintf(data+39,sizeof(data)-40,"%zu",block_verifier_online_total_rounds); 
      memcpy(data+strlen(data),"\"}",2);
      if (update_document_from_collection(database_name,DATABASE_COLLECTION,message,data) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not update the block_verifier_online_total_rounds in the database");
      }
    }

    // calculate the block_verifier_online_percentage
    number = ((size_t)((((double)block_verifier_online_total_rounds) / ((double)block_verifier_total_rounds)) * 100) | 0);

    memset(data,0,sizeof(data));
    memcpy(data,"{\"block_verifier_online_percentage\":\"",37);
    snprintf(data+37,sizeof(data)-38,"%zu",number); 
    memcpy(data+strlen(data),"\"}",2);
    if (update_document_from_collection(database_name,DATABASE_COLLECTION,message,data) == 0)
    {
      ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not update the block_verifier_online_total_rounds in the database");
    }

    
    // add one to the block_producer_total_rounds and the current block height to the block_producer_block_heights if the public address is the block producer (only if it is not a replayed round)
    if (replayed_round_settings == 0 && main_network_data_node_create_block == 0 && ((strncmp(current_round_part_backup_node,"0",1) == 0 && strncmp(current_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_public_address,XCASH_WALLET_LENGTH) == 0) || (strncmp(current_round_part_backup_node,"1",1) == 0 && strncmp(current_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_1_public_address,XCASH_WALLET_LENGTH) == 0) || (strncmp(current_round_part_backup_node,"2",1) == 0 && strncmp(current_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_2_public_address,XCASH_WALLET_LENGTH) == 0) || (strncmp(current_round_part_backup_node,"3",1) == 0 && strncmp(current_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_3_public_address,XCASH_WALLET_LENGTH) == 0) || (strncmp(current_round_part_backup_node,"4",1) == 0 && strncmp(current_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_4_public_address,XCASH_WALLET_LENGTH) == 0) || (strncmp(current_round_part_backup_node,"5",1) == 0 && strncmp(current_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_5_public_address,XCASH_WALLET_LENGTH) == 0)))
    {
      memset(data,0,sizeof(data));
      if (read_document_field_from_collection(database_name,DATABASE_COLLECTION,message,"block_producer_total_rounds",data) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not read the block_producer_total_rounds from the database");
      }
      sscanf(data, "%zu", &number);
      number++;
      memset(data,0,sizeof(data));
      memcpy(data,"{\"block_producer_total_rounds\":\"",32);
      snprintf(data+32,sizeof(data)-33,"%zu",number); 
      memcpy(data+strlen(data),"\"}",2);
      if (update_document_from_collection(database_name,DATABASE_COLLECTION,message,data) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not update the block_producer_total_rounds in the database");
      }

      memset(data,0,sizeof(data));
      if (read_document_field_from_collection(database_name,DATABASE_COLLECTION,message,"block_producer_block_heights",data) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not read the block_producer_block_heights from the database");
      }      
      memcpy(data+strlen(data),"|",sizeof(char));
      memcpy(data+strlen(data),BLOCK_HEIGHT,strnlen(BLOCK_HEIGHT,sizeof(data)));
      memset(data2,0,sizeof(data2));
      memcpy(data2,"{\"block_producer_block_heights\":\"",33);
      memcpy(data2+33,data,strnlen(data,sizeof(data2)));
      memcpy(data2+strlen(data2),"\"}",2);
      if (update_document_from_collection(database_name,DATABASE_COLLECTION,message,data2) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not update the block_producer_block_heights in the database");
      }
    }

    // only run once if the test are running
    if (test_settings == 1)
    {
      break;
    }
  }

  // update the backup network data node if it had to create the block as the backup block producer (only if it is not a replayed round)
  if (replayed_round_settings == 0 && main_network_data_node_create_block == 1)
  {
    // create the message
    memset(message,0,sizeof(message));
    memcpy(message,"{\"public_address\":\"",19);
    memcpy(message+strlen(message),network_data_nodes_list.network_data_nodes_public_address[backup_network_data_node_settings],XCASH_WALLET_LENGTH);
    memcpy(message+strlen(message),"\"}",2);

    memset(data,0,sizeof(data));
    if (read_document_field_from_collection(database_name,DATABASE_COLLECTION,message,"block_producer_total_rounds",data) == 0)
    {
      ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not read the block_producer_total_rounds from the database");
    }
    sscanf(data, "%zu", &number);
    number++;
    memset(data,0,sizeof(data));
    memcpy(data,"{\"block_producer_total_rounds\":\"",32);
    snprintf(data+32,sizeof(data)-33,"%zu",number); 
    memcpy(data+strlen(data),"\"}",2);
    if (update_document_from_collection(database_name,DATABASE_COLLECTION,message,data) == 0)
    {
      ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not update the block_producer_total_rounds in the database");
    }

    memset(data,0,sizeof(data));
    if (read_document_field_from_collection(database_name,DATABASE_COLLECTION,message,"block_producer_block_heights",data) == 0)
    {
      ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not read the block_producer_block_heights from the database");
    }      
    memcpy(data+strlen(data),"|",sizeof(char));
    memcpy(data+strlen(data),BLOCK_HEIGHT,strnlen(BLOCK_HEIGHT,sizeof(data)));
    memset(data2,0,sizeof(data2));
    memcpy(data2,"{\"block_producer_block_heights\":\"",33);
    memcpy(data2+33,data,strnlen(data,sizeof(data2)));
    memcpy(data2+strlen(data2),"\"}",2);
    if (update_document_from_collection(database_name,DATABASE_COLLECTION,message,data2) == 0)
    {
      ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not update the block_producer_block_heights in the database");
    }
  }
  return 1;

  #undef DATABASE_COLLECTION
  #undef ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR
}




/*
-----------------------------------------------------------------------------------------------------------
Name: add_round_statistics
Description: Adds the round statistics to the database after adding the block to the network
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int add_round_statistics(void)
{
  // Constants
  const bson_t* current_document;

  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  char delegates_name[BUFFER_SIZE];
  char block_verifier_total_rounds_delegates_name[BUFFER_SIZE];
  char best_block_verifier_online_percentage_delegate_name[BUFFER_SIZE];
  char most_block_producer_total_rounds_delegate_name[BUFFER_SIZE];
  char message1[SMALL_BUFFER_SIZE];
  char message2[SMALL_BUFFER_SIZE];
  char message3[SMALL_BUFFER_SIZE];
  char message4[SMALL_BUFFER_SIZE];
  char message5[SMALL_BUFFER_SIZE];
  char message6[SMALL_BUFFER_SIZE];
  size_t block_verifier_total_rounds_count = 0;
  size_t block_verifier_total_rounds_count2 = 0;
  int block_verifier_online_percentage_count = 0;
  int block_verifier_online_percentage_count2 = 0;
  size_t most_block_producer_total_rounds_count = 0;
  size_t most_block_producer_total_rounds_count2 = 0;
  mongoc_client_t* database_client_thread = NULL;
  mongoc_collection_t* collection = NULL;
  mongoc_cursor_t* document_settings = NULL;
  bson_t* document = NULL;  
  char* message;
  char* message_copy1;
  char* message_copy2;

  // define macros
  #define MESSAGE "{\"username\":\"XCASH\"}"

  #define database_reset \
  bson_destroy(document); \
  mongoc_cursor_destroy(document_settings); \
  mongoc_collection_destroy(collection); \
  mongoc_client_pool_push(database_client_thread_pool, database_client_thread);

  #define ADD_ROUND_STATISTICS_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"add_round_statistics",20); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(message1,0,sizeof(message1));
  memset(message2,0,sizeof(message2));
  memset(message3,0,sizeof(message3));
  memset(message4,0,sizeof(message4));
  memset(message5,0,sizeof(message5));
  memset(message6,0,sizeof(message6));

  // get a temporary connection
  if (!(database_client_thread = mongoc_client_pool_pop(database_client_thread_pool)))
  {
    ADD_ROUND_STATISTICS_ERROR("Could not update the round statistics in the database");
  }

  // set the collection
  collection = mongoc_client_get_collection(database_client_thread, database_name,"delegates");

  document = bson_new(); 
  document_settings = mongoc_collection_find_with_opts(collection, document, NULL, NULL);
  while (mongoc_cursor_next(document_settings, &current_document))
  {
    // reset the variables
    memset(data,0,sizeof(data));
    memset(data2,0,sizeof(data2));
    memset(data3,0,sizeof(data3));
    memset(delegates_name,0,sizeof(delegates_name));

    message = bson_as_canonical_extended_json(current_document, NULL);
    memcpy(data,message,strnlen(message,sizeof(data)));
    bson_free(message);

    // get the delegate_name
    memcpy(data2,", \"delegate_name\" : \"",21);
    message_copy1 = strstr(data,data2) + 21;
    message_copy2 = strstr(message_copy1,"\"");
    memcpy(delegates_name,message_copy1,message_copy2 - message_copy1);
    memset(data2,0,sizeof(data2));

    // get the block_verifier_total_rounds
    memcpy(data2,", \"block_verifier_total_rounds\" : \"",35);
    message_copy1 = strstr(data,data2) + 35;
    message_copy2 = strstr(message_copy1,"\"");
    memcpy(data3,message_copy1,message_copy2 - message_copy1);
    sscanf(data3, "%zu", &block_verifier_total_rounds_count2);
    memset(data2,0,sizeof(data2));
    memset(data3,0,sizeof(data3));

    if (block_verifier_total_rounds_count2 > block_verifier_total_rounds_count)
    {
      block_verifier_total_rounds_count = block_verifier_total_rounds_count2;
      memset(block_verifier_total_rounds_delegates_name,0,sizeof(block_verifier_total_rounds_delegates_name));
      memcpy(block_verifier_total_rounds_delegates_name,delegates_name,strnlen(delegates_name,sizeof(block_verifier_total_rounds_delegates_name)));
    }
    
    // get the block_verifier_online_percentage
    memcpy(data2,", \"block_verifier_online_percentage\" : \"",40);
    message_copy1 = strstr(data,data2) + 40;
    message_copy2 = strstr(message_copy1,"\"");
    memcpy(data3,message_copy1,message_copy2 - message_copy1);
    sscanf(data3, "%d", &block_verifier_online_percentage_count2);
    memset(data2,0,sizeof(data2));
    memset(data3,0,sizeof(data3));

    if (block_verifier_online_percentage_count2 > block_verifier_online_percentage_count)
    {
      block_verifier_online_percentage_count = block_verifier_online_percentage_count2;
      memset(best_block_verifier_online_percentage_delegate_name,0,sizeof(best_block_verifier_online_percentage_delegate_name));
      memcpy(best_block_verifier_online_percentage_delegate_name,delegates_name,strnlen(delegates_name,sizeof(best_block_verifier_online_percentage_delegate_name)));
    }
 
    // get the block_producer_total_rounds
    memcpy(data2,", \"block_producer_total_rounds\" : \"",35);
    message_copy1 = strstr(data,data2) + 35;
    message_copy2 = strstr(message_copy1,"\"");
    memcpy(data3,message_copy1,message_copy2 - message_copy1);
    sscanf(data3, "%zu", &most_block_producer_total_rounds_count2);
    memset(data,0,sizeof(data));
    memset(data2,0,sizeof(data2));
    memset(data3,0,sizeof(data3));

    if (most_block_producer_total_rounds_count2 > most_block_producer_total_rounds_count)
    {
      most_block_producer_total_rounds_count = most_block_producer_total_rounds_count2;
      memset(most_block_producer_total_rounds_delegate_name,0,sizeof(most_block_producer_total_rounds_delegate_name));
      memcpy(most_block_producer_total_rounds_delegate_name,delegates_name,strnlen(delegates_name,sizeof(most_block_producer_total_rounds_delegate_name)));
    }
  }

  database_reset;
  sleep(1);

  // create the message
  memcpy(message1,"{\"most_total_rounds_delegate_name\":\"",36);
  memcpy(message1+36,block_verifier_total_rounds_delegates_name,strnlen(block_verifier_total_rounds_delegates_name,sizeof(message1)));
  memcpy(message1+strlen(message1),"\"}",2);

  memcpy(message2,"{\"most_total_rounds\":\"",22);
  snprintf(message2+22,sizeof(message2)-23,"%zu",block_verifier_total_rounds_count);
  memcpy(message2+strlen(message2),"\"}",2);

  memcpy(message3,"{\"best_block_verifier_online_percentage_delegate_name\":\"",56);
  memcpy(message3+56,best_block_verifier_online_percentage_delegate_name,strnlen(best_block_verifier_online_percentage_delegate_name,sizeof(message3)));
  memcpy(message3+strlen(message3),"\"}",2);

  memcpy(message4,"{\"best_block_verifier_online_percentage\":\"",42);
  snprintf(message4+42,sizeof(message4)-43,"%d",block_verifier_online_percentage_count);
  memcpy(message4+strlen(message4),"\"}",2);

  memcpy(message5,"{\"most_block_producer_total_rounds_delegate_name\":\"",51);
  memcpy(message5+51,most_block_producer_total_rounds_delegate_name,strnlen(most_block_producer_total_rounds_delegate_name,sizeof(message5)));
  memcpy(message5+strlen(message5),"\"}",2);

  memcpy(message6,"{\"most_block_producer_total_rounds\":\"",37);
  snprintf(message6+37,sizeof(message6)-38,"%zu",most_block_producer_total_rounds_count);
  memcpy(message6+strlen(message6),"\"}",2);

  // update the database
  if (update_document_from_collection(database_name,"statistics",MESSAGE,message1) == 0 || update_document_from_collection(database_name,"statistics",MESSAGE,message2) == 0 || update_document_from_collection(database_name,"statistics",MESSAGE,message3) == 0 || update_document_from_collection(database_name,"statistics",MESSAGE,message4) == 0 || update_document_from_collection(database_name,"statistics",MESSAGE,message5) == 0 || update_document_from_collection(database_name,"statistics",MESSAGE,message6) == 0)
  {
    ADD_ROUND_STATISTICS_ERROR("Could not update the round statistics in the database");
  }
  return 1;

  #undef MESSAGE
  #undef database_reset
  #undef ADD_ROUND_STATISTICS_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: calculate_main_nodes_roles
Description: Calculates the main nodes roles for the round
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int calculate_main_nodes_roles(void)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  size_t count;
  int count2;
  int count3;
  int counter;
  int main_nodes_count;
  int number[DATA_HASH_LENGTH/2];
  int settings;
  int total_block_verifiers;

  // define macros
  #define CALCULATE_MAIN_NODES_ROLES(settings) \
  memcpy(error_message.function[error_message.total],"calculate_main_nodes_roles",26); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  #define CALCULATE_BLOCK_PRODUCER_DATA(block_producer_settings,current_block_verifiers_settings) \
  memset(block_producer_settings,0,sizeof(block_producer_settings)); \
  memcpy(block_producer_settings,(current_block_verifiers_settings)[count2],strnlen((current_block_verifiers_settings)[count2],sizeof(block_producer_settings)));

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  total_block_verifiers = test_settings == 0 ? BLOCK_VERIFIERS_AMOUNT : BLOCK_VERIFIERS_TOTAL_AMOUNT;
  if (test_settings == 0)
  {
    sscanf(current_block_height,"%zu", &count);
    if (count < XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT-1)
    {
      CALCULATE_MAIN_NODES_ROLES("Could not get the current block height");
    }
    count--;
    snprintf(data2,sizeof(data2)-1,"%zu",count);
  }  

  // calculate the database to get the reserve byte data
  get_reserve_bytes_database(count,1);
  count2 = count;
  memcpy(data,"reserve_bytes_",14);
  snprintf(data+14,sizeof(data)-15,"%d",count2);

  // create the message
  memcpy(data3,"{\"block_height\":\"",17);
  memcpy(data3+17,data2,strnlen(data2,sizeof(data3)));
  memcpy(data3+strlen(data3),"\"}",2);

  // get the reserve byte data
  memset(data2,0,sizeof(data2));
  if (test_settings == 0)
  {
    if (read_document_field_from_collection(database_name,data,data3,"reserve_bytes",data2) == 0)
    {
      CALCULATE_MAIN_NODES_ROLES("Could not get the previous blocks reserve bytes");
    }
  }
  else
  {
    memcpy(data2,NETWORK_BLOCK,sizeof(NETWORK_BLOCK)-1);
  }  

  // get the vrf_beta_string_data_round_part_3
  memset(data3,0,sizeof(data3));
  if (parse_reserve_bytes_data(data3,(const char*)data2,8) == 0)
  {
    CALCULATE_MAIN_NODES_ROLES("Could not get the previous blocks reserve bytes");
  }

  for (count = 0, count3 = 0, main_nodes_count = 0; count < VRF_BETA_LENGTH || main_nodes_count == 6; count += 2)
  {
    memset(data,0,sizeof(data));
    memcpy(data,&data3[count],2);
    count2 = (int)strtol(data, NULL, 16); 

    // set the backup network data node setting
    if (count == 0)
    {
      backup_network_data_node_settings = count2 % NETWORK_DATA_NODES_AMOUNT;
    } 
   
    for (counter = 0, settings = 0; counter < count3; counter++)
    {
      // check if this number has already been calculated
      if (number[counter] == count2 % total_block_verifiers)
      {
        settings = 1;
      }
    }
    // if it is not in the range of 01 - C8 or it has already been calculated then skip the byte
    if (count2 != 0 && count2 <= 200 && settings == 0)
    {
      count2 = count2 % total_block_verifiers;
      if (main_nodes_count == 0)
      {
        // calculate the block_producer
        CALCULATE_BLOCK_PRODUCER_DATA(main_nodes_list.block_producer_public_address,current_block_verifiers_list.block_verifiers_public_address);
        CALCULATE_BLOCK_PRODUCER_DATA(main_nodes_list.block_producer_IP_address,current_block_verifiers_list.block_verifiers_IP_address);
      }
      if (main_nodes_count == 1)
      {
        // calculate the block_producer_backup_block_verifier_1
        CALCULATE_BLOCK_PRODUCER_DATA(main_nodes_list.block_producer_backup_block_verifier_1_public_address,current_block_verifiers_list.block_verifiers_public_address);
        CALCULATE_BLOCK_PRODUCER_DATA(main_nodes_list.block_producer_backup_block_verifier_1_IP_address,current_block_verifiers_list.block_verifiers_IP_address);
      }
      if (main_nodes_count == 2)
      {
        // calculate the block_producer_backup_block_verifier_2
        CALCULATE_BLOCK_PRODUCER_DATA(main_nodes_list.block_producer_backup_block_verifier_2_public_address,current_block_verifiers_list.block_verifiers_public_address);
        CALCULATE_BLOCK_PRODUCER_DATA(main_nodes_list.block_producer_backup_block_verifier_2_IP_address,current_block_verifiers_list.block_verifiers_IP_address);
      }
      if (main_nodes_count == 3)
      {
        // calculate the block_producer_backup_block_verifier_3
        CALCULATE_BLOCK_PRODUCER_DATA(main_nodes_list.block_producer_backup_block_verifier_3_public_address,current_block_verifiers_list.block_verifiers_public_address);
        CALCULATE_BLOCK_PRODUCER_DATA(main_nodes_list.block_producer_backup_block_verifier_3_IP_address,current_block_verifiers_list.block_verifiers_IP_address);
      }
      if (main_nodes_count == 4)
      {
        // calculate the block_producer_backup_block_verifier_4
        CALCULATE_BLOCK_PRODUCER_DATA(main_nodes_list.block_producer_backup_block_verifier_4_public_address,current_block_verifiers_list.block_verifiers_public_address);
        CALCULATE_BLOCK_PRODUCER_DATA(main_nodes_list.block_producer_backup_block_verifier_4_IP_address,current_block_verifiers_list.block_verifiers_IP_address);
      }
      if (main_nodes_count == 5)
      {
        // calculate the block_producer_backup_block_verifier_5
        CALCULATE_BLOCK_PRODUCER_DATA(main_nodes_list.block_producer_backup_block_verifier_5_public_address,current_block_verifiers_list.block_verifiers_public_address);
        CALCULATE_BLOCK_PRODUCER_DATA(main_nodes_list.block_producer_backup_block_verifier_5_IP_address,current_block_verifiers_list.block_verifiers_IP_address);
      }    
      // add the number to the number array
      number[count3] = count2;
      count3++;
      main_nodes_count++;
      if (main_nodes_count == BLOCK_PRODUCERS_BACKUP_AMOUNT+1)
      {
        break;
      }
    }
    if (((count + 2) == VRF_BETA_LENGTH) && (main_nodes_count != BLOCK_PRODUCERS_BACKUP_AMOUNT+1))
    {
      color_print("The main nodes calculation process has run out of bytes to read.\nnetwork data nodes will be the block producer and backup block producers","yellow");

      memset(main_nodes_list.block_producer_public_address,0,strlen(main_nodes_list.block_producer_public_address));
      memset(main_nodes_list.block_producer_IP_address,0,strlen(main_nodes_list.block_producer_IP_address));
      memset(main_nodes_list.block_producer_backup_block_verifier_1_public_address,0,strlen(main_nodes_list.block_producer_backup_block_verifier_1_public_address));
      memset(main_nodes_list.block_producer_backup_block_verifier_1_IP_address,0,strlen(main_nodes_list.block_producer_backup_block_verifier_1_IP_address));
      memset(main_nodes_list.block_producer_backup_block_verifier_2_public_address,0,strlen(main_nodes_list.block_producer_backup_block_verifier_2_public_address));
      memset(main_nodes_list.block_producer_backup_block_verifier_2_IP_address,0,strlen(main_nodes_list.block_producer_backup_block_verifier_2_IP_address));
      memset(main_nodes_list.block_producer_backup_block_verifier_3_public_address,0,strlen(main_nodes_list.block_producer_backup_block_verifier_3_public_address));
      memset(main_nodes_list.block_producer_backup_block_verifier_3_IP_address,0,strlen(main_nodes_list.block_producer_backup_block_verifier_3_IP_address));
      memset(main_nodes_list.block_producer_backup_block_verifier_4_public_address,0,strlen(main_nodes_list.block_producer_backup_block_verifier_4_public_address));
      memset(main_nodes_list.block_producer_backup_block_verifier_4_IP_address,0,strlen(main_nodes_list.block_producer_backup_block_verifier_4_IP_address));
      memset(main_nodes_list.block_producer_backup_block_verifier_5_public_address,0,strlen(main_nodes_list.block_producer_backup_block_verifier_5_public_address));
      memset(main_nodes_list.block_producer_backup_block_verifier_5_IP_address,0,strlen(main_nodes_list.block_producer_backup_block_verifier_5_IP_address));

      memcpy(main_nodes_list.block_producer_public_address,network_data_nodes_list.network_data_nodes_public_address[0],strnlen(network_data_nodes_list.network_data_nodes_public_address[0],sizeof(main_nodes_list.block_producer_public_address)));
      memcpy(main_nodes_list.block_producer_IP_address,network_data_nodes_list.network_data_nodes_IP_address[0],strnlen(network_data_nodes_list.network_data_nodes_IP_address[0],sizeof(main_nodes_list.block_producer_IP_address)));
      memcpy(main_nodes_list.block_producer_backup_block_verifier_1_public_address,network_data_nodes_list.network_data_nodes_public_address[1],strnlen(network_data_nodes_list.network_data_nodes_public_address[1],sizeof(main_nodes_list.block_producer_backup_block_verifier_1_public_address)));
      memcpy(main_nodes_list.block_producer_backup_block_verifier_1_IP_address,network_data_nodes_list.network_data_nodes_IP_address[1],strnlen(network_data_nodes_list.network_data_nodes_IP_address[1],sizeof(main_nodes_list.block_producer_backup_block_verifier_1_IP_address)));
      memcpy(main_nodes_list.block_producer_backup_block_verifier_2_public_address,network_data_nodes_list.network_data_nodes_public_address[2],strnlen(network_data_nodes_list.network_data_nodes_public_address[2],sizeof(main_nodes_list.block_producer_backup_block_verifier_2_public_address)));
      memcpy(main_nodes_list.block_producer_backup_block_verifier_2_IP_address,network_data_nodes_list.network_data_nodes_IP_address[2],strnlen(network_data_nodes_list.network_data_nodes_IP_address[2],sizeof(main_nodes_list.block_producer_backup_block_verifier_2_IP_address)));
      memcpy(main_nodes_list.block_producer_backup_block_verifier_3_public_address,network_data_nodes_list.network_data_nodes_public_address[3],strnlen(network_data_nodes_list.network_data_nodes_public_address[3],sizeof(main_nodes_list.block_producer_backup_block_verifier_3_public_address)));
      memcpy(main_nodes_list.block_producer_backup_block_verifier_3_IP_address,network_data_nodes_list.network_data_nodes_IP_address[3],strnlen(network_data_nodes_list.network_data_nodes_IP_address[3],sizeof(main_nodes_list.block_producer_backup_block_verifier_3_IP_address)));
      memcpy(main_nodes_list.block_producer_backup_block_verifier_4_public_address,network_data_nodes_list.network_data_nodes_public_address[4],strnlen(network_data_nodes_list.network_data_nodes_public_address[4],sizeof(main_nodes_list.block_producer_backup_block_verifier_4_public_address)));
      memcpy(main_nodes_list.block_producer_backup_block_verifier_4_IP_address,network_data_nodes_list.network_data_nodes_IP_address[4],strnlen(network_data_nodes_list.network_data_nodes_IP_address[4],sizeof(main_nodes_list.block_producer_backup_block_verifier_4_IP_address)));
      memcpy(main_nodes_list.block_producer_backup_block_verifier_5_public_address,network_data_nodes_list.network_data_nodes_public_address[0],strnlen(network_data_nodes_list.network_data_nodes_public_address[0],sizeof(main_nodes_list.block_producer_backup_block_verifier_5_public_address)));
      memcpy(main_nodes_list.block_producer_backup_block_verifier_5_IP_address,network_data_nodes_list.network_data_nodes_IP_address[0],strnlen(network_data_nodes_list.network_data_nodes_IP_address[0],sizeof(main_nodes_list.block_producer_backup_block_verifier_5_IP_address)));
          
      return 1;
    }
  }
  return 1;

  #undef CALCULATE_MAIN_NODES_ROLES
  #undef CALCULATE_BLOCK_PRODUCER_DATA
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_delegates_online_status
Description: Get all of the delegates online status. They are only online if they are running the xcash-dpops program
Return: 0 if an error has occured, otherwise the amount of online delegates
-----------------------------------------------------------------------------------------------------------
*/

int get_delegates_online_status(void)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  struct delegates delegates[MAXIMUM_AMOUNT_OF_DELEGATES];
  int epoll_fd_copy;
  struct timeval SOCKET_TIMEOUT = {SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS, 0};   
  int total_delegates;
  int total_delegates_online;
  size_t total;
  size_t sent;
  long long int bytes = 1;
  int count;
  int count2;
  int number;

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define MESSAGE "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_ONLINE_STATUS\",\r\n}"
  #define GET_DELEGATES_ONLINE_STATUS_ERROR(message) \
  memcpy(error_message.function[error_message.total],"get_delegates_online_status",27); \
  memcpy(error_message.data[error_message.total],message,strnlen(message,BUFFER_SIZE)); \
  error_message.total++; \
  POINTER_RESET_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES); \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  
  // initialize the delegates struct
  INITIALIZE_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES,"get_delegates_online_status",data,current_date_and_time,current_UTC_date_and_time);

  // organize the delegates
  if ((total_delegates = organize_delegates(delegates)) == 0)
  {
    GET_DELEGATES_ONLINE_STATUS_ERROR("Could not organize the delegates");
  }

  struct epoll_event events[total_delegates];
  struct block_verifiers_send_data_socket block_verifiers_send_data_socket[total_delegates];

  // reset the struct delegates_online_status and struct block_verifiers_send_data_socket
  for (count = 0; count < total_delegates; count++)
  {
    memset(delegates_online_status[count].public_address,0,sizeof(delegates_online_status[count].public_address));
    memcpy(delegates_online_status[count].public_address,delegates[count].public_address,strnlen(delegates[count].public_address,sizeof(delegates_online_status[count].public_address)));
    delegates_online_status[count].settings = 0;

    memset(block_verifiers_send_data_socket[count].IP_address,0,sizeof(block_verifiers_send_data_socket[count].IP_address));
    memcpy(block_verifiers_send_data_socket[count].IP_address,delegates[count].IP_address,strnlen(delegates[count].IP_address,sizeof(block_verifiers_send_data_socket[count].IP_address)));
    block_verifiers_send_data_socket[count].settings = 0;
  }

  // create the message
  memcpy(data,MESSAGE,sizeof(MESSAGE)-1);
  if (sign_data(data) == 0)
  {
    GET_DELEGATES_ONLINE_STATUS_ERROR("Could not sign the message");
  }
  memcpy(data+strlen(data),SOCKET_END_STRING,sizeof(SOCKET_END_STRING)-1);
  total = strnlen(data,BUFFER_SIZE);
  
  // create the epoll file descriptor
  if ((epoll_fd_copy = epoll_create1(0)) < 0)
  {
    GET_DELEGATES_ONLINE_STATUS_ERROR("Error creating the epoll file descriptor");
  }

  // convert the port to a string
  snprintf(data2,sizeof(data2)-1,"%d",SEND_DATA_PORT); 
  
  for (count = 0; count < total_delegates; count++)
  { 
    if (strncmp(delegates[count].public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
    {
      // Variables
      struct addrinfo serv_addr;
      struct addrinfo* settings = NULL;      

      // set up the addrinfo
      memset(&serv_addr, 0, sizeof(serv_addr));
      if (string_count(block_verifiers_send_data_socket[count].IP_address,".") == 3)
      {
        /* the host is an IP address
        AI_NUMERICSERV = Specifies that getaddrinfo is provided a numerical port
        AI_NUMERICHOST = The host is already an IP address, and this will have getaddrinfo not lookup the hostname
        AF_INET = IPV4 support
        SOCK_STREAM = TCP protocol
        */
        serv_addr.ai_flags = AI_NUMERICSERV | AI_NUMERICHOST;
        serv_addr.ai_family = AF_INET;
        serv_addr.ai_socktype = SOCK_STREAM;
      }
      else
      {
        /* the host is a domain name
        AI_NUMERICSERV = Specifies that getaddrinfo is provided a numerical port
        AF_INET = IPV4 support
        SOCK_STREAM = TCP protocol
        */
        serv_addr.ai_flags = AI_NUMERICSERV;
        serv_addr.ai_family = AF_INET;
        serv_addr.ai_socktype = SOCK_STREAM;
      }
  
      // convert the hostname if used, to an IP address
      memset(data3,0,sizeof(data3));
      memcpy(data3,block_verifiers_send_data_socket[count].IP_address,strnlen(block_verifiers_send_data_socket[count].IP_address,sizeof(data3)));
      if (getaddrinfo(data3, data2, &serv_addr, &settings) != 0)
      { 
        freeaddrinfo(settings);
        continue;
      }

      /* Create the socket  
      AF_INET = IPV4 support
      SOCK_STREAM = TCP protocol
      SOCK_NONBLOCK = Non blocking socket, so it will be able to use a custom timeout
      */
      if ((block_verifiers_send_data_socket[count].socket = socket(settings->ai_family, settings->ai_socktype | SOCK_NONBLOCK, settings->ai_protocol)) == -1)
      {
        freeaddrinfo(settings);
        continue;
      }

      /* Set the socket options for sending and receiving data
      SOL_SOCKET = socket level
      SO_SNDTIMEO = allow the socket on sending data, to use the timeout settings
      */
      if (setsockopt(block_verifiers_send_data_socket[count].socket, SOL_SOCKET, SO_SNDTIMEO,(struct timeval *)&SOCKET_TIMEOUT, sizeof(struct timeval)) != 0)
      { 
        freeaddrinfo(settings);
        continue;
      } 

      /* create the epoll_event struct
      EPOLLIN = signal when the file descriptor is ready to read
      EPOLLOUT = signal when the file descriptor is ready to write
      EPOLLONESHOT = set the socket to only signal its ready once, since were using multiple threads
      */  
      events[count].events = EPOLLIN | EPOLLOUT | EPOLLONESHOT;
      events[count].data.fd = block_verifiers_send_data_socket[count].socket;

      // add the delegates socket to the epoll file descriptor
      epoll_ctl(epoll_fd_copy, EPOLL_CTL_ADD, block_verifiers_send_data_socket[count].socket, &events[count]);

      // connect to the delegate
      connect(block_verifiers_send_data_socket[count].socket,settings->ai_addr, settings->ai_addrlen);

      freeaddrinfo(settings);
    }  
  }

  // wait for all of the sockets to connect
  sleep(BLOCK_VERIFIERS_SETTINGS);

  // get the total amount of sockets that are ready
  number = epoll_wait(epoll_fd_copy, events, total_delegates, 0);

  for (count = 0; count < number; count++)
  {
    // check that the socket is connected
    if (events[count].events & EPOLLIN || events[count].events & EPOLLOUT)
    {
      // set the settings of the delegate to 1
      for (count2 = 0; count2 < total_delegates; count2++)
      {
        if (events[count].data.fd == block_verifiers_send_data_socket[count2].socket)
        {
          block_verifiers_send_data_socket[count2].settings = 1;
        }
      }
    }
  }

  // get the current time
  get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);

  for (count = 0; count < total_delegates; count++)
  {
    if (block_verifiers_send_data_socket[count].settings == 1)
    {
      delegates_online_status[count].settings = 1;

      // send the message  
      if (debug_settings == 1 && test_settings == 0)
      {  
        memset(data2,0,sizeof(data2));   
        memcpy(data2,"Sending ",8);
        memcpy(data2+8,&data[25],strlen(data) - strlen(strstr(data,"\",\r\n")) - 25);
        memcpy(data2+strlen(data2),"\n",sizeof(char));
        memcpy(data2+strlen(data2),block_verifiers_send_data_socket[count].IP_address,strnlen(block_verifiers_send_data_socket[count].IP_address,sizeof(data2)));
        memcpy(data2+strlen(data2)," on port ",9);
        memset(data3,0,sizeof(data3));
        snprintf(data3,sizeof(data3)-1,"%d",SEND_DATA_PORT);
        memcpy(data2+strlen(data2),data3,strnlen(data3,sizeof(data2)));
        memcpy(data2+strlen(data2),"\n",sizeof(char));
        memset(data3,0,sizeof(data3));
        strftime(data3,sizeof(data3),"%a %d %b %Y %H:%M:%S UTC\n",&current_UTC_date_and_time);
        memcpy(data2+strlen(data2),data3,strnlen(data3,sizeof(data3)));
        color_print(data2,"green");
      }
      
      for (sent = 0; sent < total; sent+= bytes)
      {
        if ((bytes = send(block_verifiers_send_data_socket[count].socket,data+sent,total-sent,MSG_NOSIGNAL)) == -1 && errno != EAGAIN && errno != EWOULDBLOCK)
        {      
          delegates_online_status[count].settings = 0;     
          break;
        }
      }
    }
    else if (block_verifiers_send_data_socket[count].settings == 0 && strncmp(delegates_online_status[count].public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0)
    {
      delegates_online_status[count].settings = 1;
    }
    else if (block_verifiers_send_data_socket[count].settings == 0 && strncmp(delegates_online_status[count].public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
    {
      delegates_online_status[count].settings = 0;
    }        
  }

  // wait for all of the data to be sent to the connected sockets, and for the block verifiers to process the data
  sleep(10);

  for (count = 0, total_delegates_online = 0; count < total_delegates; count++)
  {
    // update the delegates online status
    memset(data,0,sizeof(data));
    memset(data2,0,sizeof(data2));
    memcpy(data2,"{\"public_address\":\"",19);
    memcpy(data2+19,delegates_online_status[count].public_address,XCASH_WALLET_LENGTH);
    memcpy(data2+117,"\"}",2);

    if (delegates_online_status[count].settings == 1)
    {
      memcpy(data,"{\"online_status\":\"true\"}",24);
      total_delegates_online++;
    }
    else
    {
      memcpy(data,"{\"online_status\":\"false\"}",25);
    }

    update_document_from_collection(database_name,DATABASE_COLLECTION,data2,data);

    // remove all of the sockets from the epoll file descriptor and close all of the sockets
    epoll_ctl(epoll_fd_copy, EPOLL_CTL_DEL, block_verifiers_send_data_socket[count].socket, &events[count]);
    close(block_verifiers_send_data_socket[count].socket);
  }
  POINTER_RESET_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES);
  return total_delegates_online;

  #undef DATABASE_COLLECTION
  #undef MESSAGE
  #undef GET_DELEGATES_ONLINE_STATUS_ERROR
}