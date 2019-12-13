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
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"
#include "initialize_and_reset_structs_define_macros.h"

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
  size_t count;
  size_t count2;
  int settings = 0;
  int total_delegates = 0;

  // define macros
  #define UPDATE_BLOCK_VERIFIERS_LIST_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"update_block_verifiers_list",27); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  database_settings = 1; \
  pthread_cond_broadcast(&thread_settings_lock); \
  POINTER_RESET_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES); \
  return 0;

  // set the database to not accept any new data
  database_settings = 0;

  // reset the previous_block_verifiers_list struct
  for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++)
  {
    memset(previous_block_verifiers_list.block_verifiers_name[count],0,sizeof(previous_block_verifiers_list.block_verifiers_name[count]));
    memset(previous_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_public_address[count]));
    memset(previous_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(previous_block_verifiers_list.block_verifiers_public_key[count]));
    memset(previous_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_IP_address[count]));
  }

  // get the delegate amount
  for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++)
  {
    if (strlen(current_block_verifiers_list.block_verifiers_public_address[count]) != XCASH_WALLET_LENGTH)
    {
      total_delegates = count;
      break;
    }
  }

  // copy the current_block_verifiers_list to the previous_block_verifiers_list
  for (count = 0; (int)count < total_delegates; count++)
  {
    memcpy(previous_block_verifiers_list.block_verifiers_name[count],current_block_verifiers_list.block_verifiers_name[count],strnlen(current_block_verifiers_list.block_verifiers_name[count],sizeof(previous_block_verifiers_list.block_verifiers_name[count])));
    memcpy(previous_block_verifiers_list.block_verifiers_public_address[count],current_block_verifiers_list.block_verifiers_public_address[count],strnlen(current_block_verifiers_list.block_verifiers_public_address[count],sizeof(previous_block_verifiers_list.block_verifiers_public_address[count])));
    memcpy(previous_block_verifiers_list.block_verifiers_public_key[count],current_block_verifiers_list.block_verifiers_public_key[count],strnlen(current_block_verifiers_list.block_verifiers_public_key[count],sizeof(previous_block_verifiers_list.block_verifiers_public_key[count])));
    memcpy(previous_block_verifiers_list.block_verifiers_IP_address[count],current_block_verifiers_list.block_verifiers_IP_address[count],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count],sizeof(previous_block_verifiers_list.block_verifiers_IP_address[count])));
  }

  // reset the current_block_verifiers_list struct
  for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++)
  {
    memset(current_block_verifiers_list.block_verifiers_name[count],0,sizeof(current_block_verifiers_list.block_verifiers_name[count]));
    memset(current_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_public_address[count]));
    memset(current_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(current_block_verifiers_list.block_verifiers_public_key[count]));
    memset(current_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_IP_address[count]));
  } 

  // get the delegate amount
  for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++)
  {
    if (strlen(next_block_verifiers_list.block_verifiers_public_address[count]) != XCASH_WALLET_LENGTH)
    {
      total_delegates = count;
      break;
    }
  } 

  // copy the next_block_verifiers_list to the current_block_verifiers_list
  for (count = 0; (int)count < total_delegates; count++)
  {
    memcpy(current_block_verifiers_list.block_verifiers_name[count],next_block_verifiers_list.block_verifiers_name[count],strnlen(next_block_verifiers_list.block_verifiers_name[count],sizeof(current_block_verifiers_list.block_verifiers_name[count])));
    memcpy(current_block_verifiers_list.block_verifiers_public_address[count],next_block_verifiers_list.block_verifiers_public_address[count],strnlen(next_block_verifiers_list.block_verifiers_public_address[count],sizeof(current_block_verifiers_list.block_verifiers_public_address[count])));
    memcpy(current_block_verifiers_list.block_verifiers_public_key[count],next_block_verifiers_list.block_verifiers_public_key[count],strnlen(next_block_verifiers_list.block_verifiers_public_key[count],sizeof(current_block_verifiers_list.block_verifiers_public_key[count])));
    memcpy(current_block_verifiers_list.block_verifiers_IP_address[count],next_block_verifiers_list.block_verifiers_IP_address[count],strnlen(next_block_verifiers_list.block_verifiers_IP_address[count],sizeof(current_block_verifiers_list.block_verifiers_IP_address[count])));
  }

  // reset the next_block_verifiers_list struct
  for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++)
  {
    memset(next_block_verifiers_list.block_verifiers_name[count],0,sizeof(next_block_verifiers_list.block_verifiers_name[count]));
    memset(next_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_public_address[count]));
    memset(next_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(next_block_verifiers_list.block_verifiers_public_key[count]));
    memset(next_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_IP_address[count]));
  }

  // initialize the delegates struct
  INITIALIZE_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES,"update_block_verifiers_list",data,current_date_and_time,current_UTC_date_and_time);

  // organize the delegates
  total_delegates = organize_delegates(delegates);
  if (total_delegates == 0)
  {
    POINTER_RESET_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES);
    UPDATE_BLOCK_VERIFIERS_LIST_ERROR("Could not organize the delegates");
  }
  else if (total_delegates > BLOCK_VERIFIERS_TOTAL_AMOUNT)
  {
    total_delegates = BLOCK_VERIFIERS_TOTAL_AMOUNT;
  }

  // copy the database_multiple_documents_fields to the next_block_verifiers_list
  for (count = 0; (int)count < total_delegates; count++)
  {
    memcpy(next_block_verifiers_list.block_verifiers_name[count],delegates[count].delegate_name,strnlen(delegates[count].delegate_name,sizeof(next_block_verifiers_list.block_verifiers_name[count])));
    memcpy(next_block_verifiers_list.block_verifiers_public_address[count],delegates[count].public_address,strnlen(delegates[count].public_address,sizeof(next_block_verifiers_list.block_verifiers_public_address[count])));
    memcpy(next_block_verifiers_list.block_verifiers_public_key[count],delegates[count].public_key,strnlen(delegates[count].public_key,sizeof(next_block_verifiers_list.block_verifiers_public_key[count])));
    memcpy(next_block_verifiers_list.block_verifiers_IP_address[count],delegates[count].IP_address,strnlen(delegates[count].IP_address,sizeof(next_block_verifiers_list.block_verifiers_IP_address[count])));
  }

  // check if more than BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT are new block verifiers
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
    {
      if (memcmp(previous_block_verifiers_list.block_verifiers_public_address[count],current_block_verifiers_list.block_verifiers_public_address[count2],XCASH_WALLET_LENGTH) == 0)
      {
        settings++;
        break;
      }
    }
  }

  if (settings > (BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT))
  {
    settings = 1;
  }
  else
  {
    settings = 2;
  } 

  // set the database to accept data
  database_settings = 1;

  // reset any thread that was waiting for the database
  pthread_cond_broadcast(&thread_settings_lock);

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
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  size_t count;

  // define macros
  #define UPDATE_DATABASE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"update_databases",16); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));
  
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
  size_t count;
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
    memcpy(message+19,previous_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH);
    memcpy(message+19+XCASH_WALLET_LENGTH,"\"}",2);

    // add one to the block_verifier_total_rounds for every block verifier
    memset(data,0,sizeof(data));
    if (read_document_field_from_collection(DATABASE_NAME,DATABASE_COLLECTION,message,"block_verifier_total_rounds",data,1) == 0)
    {
      color_print(message,"yellow");
      ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not read the block_verifier_total_rounds from the database");
    }
    sscanf(data, "%zu", &block_verifier_total_rounds);
    block_verifier_total_rounds++;
    memset(data,0,sizeof(data));
    memcpy(data,"{\"block_verifier_total_rounds\":\"",32);
    snprintf(data+32,sizeof(data)-33,"%zu",block_verifier_total_rounds); 
    memcpy(data+strlen(data),"\"}",2);
    sync_database_threads;
    if (update_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,message,data,1) == 0)
    {
      ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not update the block_verifier_total_rounds in the database");
    }

    // add one to the block_verifier_online_total_rounds
    memset(data,0,sizeof(data));
    if (read_document_field_from_collection(DATABASE_NAME,DATABASE_COLLECTION,message,"block_verifier_online_total_rounds",data,1) == 0)
    {
      ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not read the block_verifier_online_total_rounds from the database");
    }
    sscanf(data, "%zu", &block_verifier_online_total_rounds);
    block_verifier_online_total_rounds++;
    memset(data,0,sizeof(data));
    memcpy(data,"{\"block_verifier_online_total_rounds\":\"",39);
    snprintf(data+39,sizeof(data)-40,"%zu",block_verifier_online_total_rounds); 
    memcpy(data+strlen(data),"\"}",2);
    sync_database_threads;
    if (update_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,message,data,1) == 0)
    {
      ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not update the block_verifier_online_total_rounds in the database");
    }

    // calculate the block_verifier_online_percentage
    number = ((size_t)((((double)block_verifier_online_total_rounds) / ((double)block_verifier_total_rounds)) * 100) | 0);

    memset(data,0,sizeof(data));
    memcpy(data,"{\"block_verifier_online_percentage\":\"",37);
    snprintf(data+37,sizeof(data)-38,"%zu",number); 
    memcpy(data+strlen(data),"\"}",2);
    sync_database_threads;
    if (update_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,message,data,1) == 0)
    {
      ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not update the block_verifier_online_total_rounds in the database");
    }

    
    // add one to the block_producer_total_rounds and the current block height to the block_producer_block_heights if the public address is the block producer
    if ((memcmp(current_round_part_backup_node,"0",1) == 0 && memcmp(previous_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"1",1) == 0 && memcmp(previous_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_1_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"2",1) == 0 && memcmp(previous_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_2_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"3",1) == 0 && memcmp(previous_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_3_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"4",1) == 0 && memcmp(previous_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_4_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"5",1) == 0 && memcmp(previous_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_5_public_address,XCASH_WALLET_LENGTH) == 0))
    {
      memset(data,0,sizeof(data));
      if (read_document_field_from_collection(DATABASE_NAME,DATABASE_COLLECTION,message,"block_producer_total_rounds",data,1) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not read the block_producer_total_rounds from the database");
      }
      sscanf(data, "%zu", &number);
      number++;
      memset(data,0,sizeof(data));
      memcpy(data,"{\"block_producer_total_rounds\":\"",32);
      snprintf(data+32,sizeof(data)-33,"%zu",number); 
      memcpy(data+strlen(data),"\"}",2);
      sync_database_threads;
      if (update_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,message,data,1) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not update the block_producer_total_rounds in the database");
      }

      memset(data,0,sizeof(data));
      if (read_document_field_from_collection(DATABASE_NAME,DATABASE_COLLECTION,message,"block_producer_block_heights",data,1) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not read the block_producer_block_heights from the database");
      }      
      memcpy(data+strlen(data),"|",1);
      memcpy(data+strlen(data),BLOCK_HEIGHT,strnlen(BLOCK_HEIGHT,sizeof(data)));
      memset(data2,0,sizeof(data2));
      memcpy(data2,"{\"block_producer_block_heights\":\"",33);
      memcpy(data2+33,data,strnlen(data,sizeof(data2)));
      memcpy(data2+strlen(data2),"\"}",2);
      sync_database_threads;
      if (update_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,message,data2,1) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not update the block_producer_block_heights in the database");
      }
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
  char message1[BUFFER_SIZE];
  char message2[BUFFER_SIZE];
  char message3[BUFFER_SIZE];
  char message4[BUFFER_SIZE];
  char message5[BUFFER_SIZE];
  char message6[BUFFER_SIZE];
  size_t block_verifier_total_rounds_count = 0;
  size_t block_verifier_total_rounds_count2 = 0;
  size_t most_block_producer_total_rounds_count = 0;
  size_t most_block_producer_total_rounds_count2 = 0;
  double total = 0;
  double total2;
  double total3;
  mongoc_collection_t* collection;
  mongoc_cursor_t* document_settings;
  bson_t* document = NULL;  
  char* message;
  char* message_copy1;
  char* message_copy2;

  // define macros
  #define MESSAGE "{\"username\":\"XCASH\"}"
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
  
  // set the collection
  collection = mongoc_client_get_collection(database_client, DATABASE_NAME,"delegates");
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
    sscanf(data3, "%lf", &total2);
    memset(data2,0,sizeof(data2));
    memset(data3,0,sizeof(data3));

    if (block_verifier_total_rounds_count2 > block_verifier_total_rounds_count)
    {
      block_verifier_total_rounds_count = block_verifier_total_rounds_count2;
      memset(block_verifier_total_rounds_delegates_name,0,sizeof(block_verifier_total_rounds_delegates_name));
      memcpy(block_verifier_total_rounds_delegates_name,delegates_name,strnlen(delegates_name,sizeof(block_verifier_total_rounds_delegates_name)));
    }
    
    // get the block_verifier_online_total_rounds
    memcpy(data2,", \"block_verifier_online_total_rounds\" : \"",42);
    message_copy1 = strstr(data,data2) + 42;
    message_copy2 = strstr(message_copy1,"\"");
    memcpy(data3,message_copy1,message_copy2 - message_copy1);
    sscanf(data3, "%lf", &total3);
    memset(data2,0,sizeof(data2));
    memset(data3,0,sizeof(data3));

    if ((int)total2 != 0 && total3 / total2 > total)
    {
      total = total3 / total2;
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
  snprintf(message4+42,sizeof(message4)-43,"%.2lf",total);
  memcpy(message4+strlen(message4),"\"}",2);

  memcpy(message5,"{\"most_block_producer_total_rounds_delegate_name\":\"",51);
  memcpy(message5+51,most_block_producer_total_rounds_delegate_name,strnlen(most_block_producer_total_rounds_delegate_name,sizeof(message5)));
  memcpy(message5+strlen(message5),"\"}",2);

  memcpy(message6,"{\"most_block_producer_total_rounds\":\"",37);
  snprintf(message6+37,sizeof(message6)-38,"%zu",most_block_producer_total_rounds_count);
  memcpy(message6+strlen(message6),"\"}",2);

  sync_database_threads;

  // update the database
  if (update_document_from_collection(DATABASE_NAME,"statistics",MESSAGE,message1,1) == 0 || update_document_from_collection(DATABASE_NAME,"statistics",MESSAGE,message2,1) == 0 || update_document_from_collection(DATABASE_NAME,"statistics",MESSAGE,message3,1) == 0 || update_document_from_collection(DATABASE_NAME,"statistics",MESSAGE,message4,1) == 0 || update_document_from_collection(DATABASE_NAME,"statistics",MESSAGE,message5,1) == 0 || update_document_from_collection(DATABASE_NAME,"statistics",MESSAGE,message6,1) == 0)
  {
    ADD_ROUND_STATISTICS_ERROR("Could not update the round statistics in the database");
  }
  return 1;

  #undef MESSAGE
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
  int random_network_data_node;
  int main_nodes_count;
  int number[DATA_HASH_LENGTH/2];
  int settings;

  // define macros
  #define CALCULATE_MAIN_NODES_ROLES(settings) \
  memcpy(error_message.function[error_message.total],"calculate_main_nodes_roles",26); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  #define CALCULATE_BLOCK_PRODUCER_DATA(block_producer_settings,current_block_verifiers_settings) \
  memset(block_producer_settings,0,sizeof(block_producer_settings)); \
  memcpy(block_producer_settings,(current_block_verifiers_settings)[count2],strnlen((current_block_verifiers_settings)[count2],sizeof(block_producer_settings)));

  #define CALCULATE_MAIN_NODES_LIST_DATA(main_nodes_settings,network_data_nodes_list_settings) \
  random_network_data_node = (int)(rand() % NETWORK_DATA_NODES_AMOUNT); \
  memset(main_nodes_settings,0,sizeof(main_nodes_settings)); \
  memcpy(main_nodes_settings,(network_data_nodes_list_settings)[random_network_data_node],strnlen((network_data_nodes_list_settings)[random_network_data_node],sizeof(main_nodes_settings)));

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));
  
  sscanf(current_block_height,"%zu", &count);
  if (count < XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT-1)
  {
    CALCULATE_MAIN_NODES_ROLES("Could not get the current block height");
  }
  count--;
  snprintf(data2,sizeof(data2)-1,"%zu",count);

  // calculate the database to get the reserve byte data
  count2 = ((count - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME) + 1;
  memcpy(data,"reserve_bytes_",14);
  snprintf(data+14,sizeof(data)-15,"%d",count2);

  // create the message
  memcpy(data3,"{\"block_height\":\"",17);
  memcpy(data3+17,data2,strnlen(data2,sizeof(data3)));
  memcpy(data3+strlen(data3),"\"}",2);

  // get the reserve byte data
  memset(data2,0,sizeof(data2));
  if (read_document_field_from_collection(DATABASE_NAME,data,data3,"reserve_bytes",data2,1) == 0)
  {
    CALCULATE_MAIN_NODES_ROLES("Could not get the previous blocks reserve bytes");
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
   
    for (counter = 0, settings = 0; counter < count3; counter++)
    {
      // check if this number has already been calculated
      if (number[counter] == count2 % 100)
      {
        settings = 1;
      }
    }
    // if it is not in the range of 01 - C8 or it has already been calculated then skip the byte
    if (count2 != 0 && count2 <= 200 && settings == 0)
    {
      count2 = count2 % BLOCK_VERIFIERS_AMOUNT;
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
      color_print("The main nodes calculation process has run out of bytes to read.\nA random network data node will be the block producer","yellow");

      // select a random network data node
      if (strncmp(main_nodes_list.block_producer_public_address,"",1) == 0)
      {
        CALCULATE_MAIN_NODES_LIST_DATA(main_nodes_list.block_producer_public_address,network_data_nodes_list.network_data_nodes_public_address);
        CALCULATE_MAIN_NODES_LIST_DATA(main_nodes_list.block_producer_IP_address,network_data_nodes_list.network_data_nodes_IP_address);
      } 
      if (strncmp(main_nodes_list.block_producer_backup_block_verifier_1_public_address,"",1) == 0)
      {
        CALCULATE_MAIN_NODES_LIST_DATA(main_nodes_list.block_producer_backup_block_verifier_1_public_address,network_data_nodes_list.network_data_nodes_public_address);
        CALCULATE_MAIN_NODES_LIST_DATA(main_nodes_list.block_producer_backup_block_verifier_1_IP_address,network_data_nodes_list.network_data_nodes_IP_address);
      } 
      if (strncmp(main_nodes_list.block_producer_backup_block_verifier_2_public_address,"",1) == 0)
      {
        CALCULATE_MAIN_NODES_LIST_DATA(main_nodes_list.block_producer_backup_block_verifier_2_public_address,network_data_nodes_list.network_data_nodes_public_address);
        CALCULATE_MAIN_NODES_LIST_DATA(main_nodes_list.block_producer_backup_block_verifier_2_IP_address,network_data_nodes_list.network_data_nodes_IP_address);
      } 
      if (strncmp(main_nodes_list.block_producer_backup_block_verifier_3_public_address,"",1) == 0)
      {
        CALCULATE_MAIN_NODES_LIST_DATA(main_nodes_list.block_producer_backup_block_verifier_3_public_address,network_data_nodes_list.network_data_nodes_public_address);
        CALCULATE_MAIN_NODES_LIST_DATA(main_nodes_list.block_producer_backup_block_verifier_3_IP_address,network_data_nodes_list.network_data_nodes_IP_address);
      } 
      if (strncmp(main_nodes_list.block_producer_backup_block_verifier_4_public_address,"",1) == 0)
      {
        CALCULATE_MAIN_NODES_LIST_DATA(main_nodes_list.block_producer_backup_block_verifier_4_public_address,network_data_nodes_list.network_data_nodes_public_address);
        CALCULATE_MAIN_NODES_LIST_DATA(main_nodes_list.block_producer_backup_block_verifier_4_IP_address,network_data_nodes_list.network_data_nodes_IP_address);
      }   
      if (strncmp(main_nodes_list.block_producer_backup_block_verifier_5_public_address,"",1) == 0)
      {
        CALCULATE_MAIN_NODES_LIST_DATA(main_nodes_list.block_producer_backup_block_verifier_5_public_address,network_data_nodes_list.network_data_nodes_public_address);
        CALCULATE_MAIN_NODES_LIST_DATA(main_nodes_list.block_producer_backup_block_verifier_5_IP_address,network_data_nodes_list.network_data_nodes_IP_address);
      } 
    }
  }
  return 1;

  #undef CALCULATE_MAIN_NODES_ROLES
  #undef CALCULATE_BLOCK_PRODUCER_DATA
  #undef CALCULATE_MAIN_NODES_LIST_DATA
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_delegates_online_status
Description: Get all of the delegates online status
Return: 0 if an error has occured, otherwise the amount of online delegates
-----------------------------------------------------------------------------------------------------------
*/

int get_delegates_online_status(void)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  struct delegates delegates[MAXIMUM_AMOUNT_OF_DELEGATES];
  struct delegates_online_status delegates_online_status[MAXIMUM_AMOUNT_OF_DELEGATES];
  int epoll_fd_copy;
  struct epoll_event events[MAXIMUM_AMOUNT_OF_DELEGATES];
  int count;
  int count2;
  int number;
  int total_delegates = 0;
  int total_delegates_online = 0;

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define GET_DELEGATES_ONLINE_STATUS_ERROR(message) \
  memcpy(error_message.function[error_message.total],"get_delegates_online_status",27); \
  memcpy(error_message.data[error_message.total],message,strnlen(message,BUFFER_SIZE)); \
  error_message.total++; \
  POINTER_RESET_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES); \
  POINTER_RESET_DELEGATES_ONLINE_STATUS_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES); \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  
  // initialize the delegates struct
  INITIALIZE_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES,"get_delegates_online_status",data,current_date_and_time,current_UTC_date_and_time);

  // initialize the delegates_online_status struct
  INITIALIZE_DELEGATES_ONLINE_STATUS_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES,"get_delegates_online_status",data,current_date_and_time,current_UTC_date_and_time);

  // organize the delegates
  total_delegates = organize_delegates(delegates);
  
  // create the epoll file descriptor
  epoll_fd_copy = epoll_create1(0);
  if (epoll_fd_copy < 0)
  {
    GET_DELEGATES_ONLINE_STATUS_ERROR("Error creating the epoll file descriptor");
  }

  // create the delegates_online_status struct for each delegate
  for (count = 0; count < total_delegates; count++)
  {
    // Variables
    struct addrinfo serv_addr;
    struct addrinfo* settings = NULL;

    memcpy(delegates_online_status[count].public_address,delegates[count].public_address,XCASH_WALLET_LENGTH);

    if (memcmp(delegates_online_status[count].public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0)
    {
      delegates_online_status[count].settings = 1;
      total_delegates_online++;
      continue;
    }

    // convert the port to a string  
    snprintf(data2,sizeof(data2)-1,"%d",SEND_DATA_PORT); 

    // set up the addrinfo
    memset(&serv_addr, 0, sizeof(serv_addr));
    if (string_count(delegates[count].IP_address,".") == 3)
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
    memset(data,0,sizeof(data));
    memcpy(data,delegates[count].IP_address,strnlen(delegates[count].IP_address,sizeof(data)));
    string_replace(data,sizeof(data),"http://","");
    string_replace(data,sizeof(data),"https://","");
    string_replace(data,sizeof(data),"www.","");
    if (getaddrinfo(data, data2, &serv_addr, &settings) != 0)
    {  
      freeaddrinfo(settings);
      continue;
    }

    /* Create the socket  
    AF_INET = IPV4 support
    SOCK_STREAM = TCP protocol
    SOCK_NONBLOCK = Set the socket to non blocking mode, so it will use the timeout settings when connecting
    */
    delegates_online_status[count].socket = socket(settings->ai_family, settings->ai_socktype | SOCK_NONBLOCK, settings->ai_protocol);
    if (delegates_online_status[count].socket == -1)
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
    events[count].data.fd = delegates_online_status[count].socket;

    // add the delegates socket to the epoll file descriptor
    epoll_ctl(epoll_fd_copy, EPOLL_CTL_ADD, delegates_online_status[count].socket, &events[count]);

    // connect to the delegate
    connect(delegates_online_status[count].socket,settings->ai_addr, settings->ai_addrlen);

    freeaddrinfo(settings);
  }

  sleep(TOTAL_CONNECTION_TIME_SETTINGS+1);

  // get the total amount of sockets that are ready
  number = epoll_wait(epoll_fd_copy, events, MAXIMUM_AMOUNT_OF_DELEGATES, 1);

  for (count = 0; count < number; count++)
  {
    // check that the socket is connected
    if (events[count].events & EPOLLIN || events[count].events & EPOLLOUT)
    {
      // set the settings of the delegate to 1
      for (count2 = 0; count2 < MAXIMUM_AMOUNT_OF_DELEGATES; count2++)
      {
        if (events[count].data.fd == delegates_online_status[count2].socket)
        {
          delegates_online_status[count2].settings = 1;
          total_delegates_online++;
        }
      }
    }
  }

  // Update the delegates online status
  for (count = 0; count < MAXIMUM_AMOUNT_OF_DELEGATES; count++)
  {
    if (memcmp(delegates_online_status[count].public_address,"",1) != 0)
    {
      // create the message
      memset(data2,0,sizeof(data2));
      memcpy(data2,"{\"public_address\":\"",19);
      memcpy(data2+19,delegates_online_status[count].public_address,XCASH_WALLET_LENGTH);
      memcpy(data2+117,"\"}",2);

      if (delegates_online_status[count].settings == 1)
      {
        memset(data,0,sizeof(data));
        memcpy(data,"{\"online_status\":\"true\"}",24);
      }
      else
      {
        memset(data,0,sizeof(data));
        memcpy(data,"{\"online_status\":\"false\"}",25);
      }   

      sync_database_threads;
      update_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,data2,data,1);  
    }
  }

  // remove the sockets from the epoll file descriptor and close all of the sockets
  for (count = 0; count < MAXIMUM_AMOUNT_OF_DELEGATES; count++)
  {
    epoll_ctl(epoll_fd_copy, EPOLL_CTL_DEL, delegates_online_status[count].socket, &events[count]);
    close(delegates_online_status[count].socket);
  }
  POINTER_RESET_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES);
  POINTER_RESET_DELEGATES_ONLINE_STATUS_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES);
  return total_delegates_online;

  #undef DATABASE_COLLECTION
  #undef GET_DELEGATES_ONLINE_STATUS_ERROR
}