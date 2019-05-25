#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "define_macros_functions.h"
#include "blockchain_functions.h"
#include "database_functions.h"
#include "file_functions.h"
#include "network_daemon_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "network_wallet_functions.h"
#include "server_functions.h"
#include "string_functions.h"
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
Name: start_new_round
Description: Checks if the round is a start block round or not
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

void start_new_round()
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;
  int settings;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // start a new round
  memset(data,0,strlen(data));
  memcpy(data,"A new round is starting for block ",34);
  memcpy(data,current_block_height,strnlen(current_block_height,BUFFER_SIZE));
  color_print(data,"green");

  // reset the variables
  memset(current_round_part,0,strlen(current_round_part));
  memcpy(current_round_part,"1",1);
  memset(current_round_part_backup_node,0,strlen(current_round_part_backup_node));
  memcpy(current_round_part_backup_node,"0",1);

  // check if the current block height - 3 is a X-CASH proof of stake block since this will check to see if these are the first three blocks on the network
  sscanf(current_block_height,"%zu", &count);
  count = count - 3;
  memset(data,0,strnlen(data,BUFFER_SIZE));
  sprintf(data,"%zu",count);
  settings = get_block_settings(data,0);
  if (settings == 0)
  {
    // an error has occured so wait until the next round
    color_print("Could not get a previous blocks settings. Your block verifier will now sit out for the remainder of the round","red");
    return;
  }
  else if (settings == 1)
  {
    // this is a proof of work block, so this is the start blocks of the network
    color_print("The current block is on of the first three blocks on the network, meaning the data network node will create this block","green");
    start_current_round_start_blocks();    
  }
  else if (settings == 2)
  {
    // this is a X-CASH proof of stake block so this is not the start blocks of the network
    start_current_round();
  }
  pointer_reset(data);
  return;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: start_current_round_start_blocks
Description: Runs the round where the network data node will create the block since its the first three X-CASH proof of stake blocks on the network
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

void start_current_round_start_blocks()
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;
  int settings;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // check if the block verifier is a network data node
  if (MAIN_NETWORK_DATA_NODE == 0)
  {
    color_print("Your block verifier is not the main data network node so your block verifier will sit out for the remainder of the round","red");
    return;
  }

  color_print("Your block verifier is the main data network node so your block verifier will create the block","green");

  // create the block with the reserve bytes
  
  pointer_reset(data);
  return;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: start_current_round
Description: Runs the current round
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

void start_current_round()
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  time_t current_date_and_time;
  struct tm* current_UTC_date_and_time; 
  size_t count;
  int settings;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // check if the block verifier is a network data node
  if (NETWORK_DATA_NODE == 1)
  {
    color_print("Your block verifier is a data network node.\nSaving the previous rounds statistics to the database","green");
    data_network_node_save_previous_round_statistics();
    // wait until 0 minutes and 10 seconds of this round
    for (;;)
    {
      usleep(200000);
      time(&current_date_and_time);
      current_UTC_date_and_time = gmtime(&current_date_and_time);
      if (current_UTC_date_and_time->tm_sec == 10)
      {
       goto start;
      }
    }
  }
  else
  {
    color_print("Your block verifier is not the main data network node waiting until 0 minutes and 10 seconds of this round, for the data network node to save the previous rounds statistics to the database","red");
  }

  // wait until 0 minutes and 10 seconds of this round
  color_print("Waiting until 0 minutes and 10 seconds of the round to synchronize all block verifiers","green");
  for (;;)
  {
    usleep(200000);
    time(&current_date_and_time);
    current_UTC_date_and_time = gmtime(&current_date_and_time);
    if (current_UTC_date_and_time->tm_sec == 10)
    {
     goto start;
    }
  }

  start:

  // calculate main nodes roles
  
  pointer_reset(data);
  return;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: data_network_node_save_previous_round_statistics
Description: Save the previous round statistics
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

void data_network_node_save_previous_round_statistics()
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;
  int settings;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // check if the block verifier is the mian network data node
  if (MAIN_NETWORK_DATA_NODE == 0)
  {
    color_print("Your block verifier is not the main data network node so your block verifier will check to see if the main network data node is online","red");
    if (get_delegate_online_status(MAIN_NETWORK_DATA_NODE_IP_ADDRESS) == 0)
    {
      // check again to make sure the main network data node is offline
      sleep(2);
      if (get_delegate_online_status(MAIN_NETWORK_DATA_NODE_IP_ADDRESS) == 0)
      {
        // the main network data node is offline, so save the previous round statistics to the database
        color_print("The main network data node is offline, so your block verifier will save the previous round statistics to the database","green");
        add_block_verifiers_round_statistics();
        add_round_statistics();        
      }
    }
  }
  else
  {
    // save the previous round statistics to the database
    color_print("Your block verifier is the main data network node so your block verifier will save the previous round statistics to the database","green");
    add_block_verifiers_round_statistics();
    add_round_statistics();
  }
  
  pointer_reset(data);
  return;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: add_block_verifiers_round_statistics
Description: Adds the block verifier statistics to the database after adding the block to the network
-----------------------------------------------------------------------------------------------------------
*/

void add_block_verifiers_round_statistics()
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;
  size_t count2;
  size_t number;

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; \
  free(message); \
  message = NULL;
  
  #define ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR(settings) \
  color_print(settings,"red"); \
  pointer_reset_all; \
  return;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL || message == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    if (message != NULL)
    {
      pointer_reset(message);
    }
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    // create the message
    memset(message,0,strnlen(message,BUFFER_SIZE));
    memcpy(message,"{\"public_address\":\"",19);
    memcpy(message+19,previous_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH);
    memcpy(message+19+XCASH_WALLET_LENGTH,"\"}",2);

    // add one to the block_verifier_total_rounds for every block verifier
    memset(data,0,strnlen(data,BUFFER_SIZE));
    if (read_document_field_from_collection(DATABASE_NAME,"delegates",message,"block_verifier_total_rounds",data,0) == 0)
    {
      ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not read the block_verifier_total_rounds from the database\nFunction: add_block_verifiers_round_statistics");
    }
    count2 = strnlen(data,BUFFER_SIZE);
    sscanf(data, "%zu", &number);
    number++;
    memset(data,0,strnlen(data,BUFFER_SIZE));
    memcpy(data,"{\"block_verifier_total_rounds\":\"",32);
    sprintf(data+32,"%zu",number); 
    memcpy(data+32+count2,"\"}",2);
    if (update_document_from_collection(DATABASE_NAME,"delegates",message,data,0) == 0)
    {
      ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not update the block_verifier_total_rounds in the database\nFunction: add_block_verifiers_round_statistics");
    }

    // add one to the block_verifier_online_total_rounds for every block verifier that is currently online
    if (send_data_socket(previous_block_verifiers_list.block_verifiers_IP_address[count],SEND_DATA_PORT,"","checking if the block verifier is online",0) == 1)
    {
      memset(data,0,strnlen(data,BUFFER_SIZE));
      if (read_document_field_from_collection(DATABASE_NAME,"delegates",message,"block_verifier_online_total_rounds",data,0) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not read the block_verifier_online_total_rounds from the database\nFunction: add_block_verifiers_round_statistics");
      }
      count2 = strnlen(data,BUFFER_SIZE);
      sscanf(data, "%zu", &number);
      number++;
      memset(data,0,strnlen(data,BUFFER_SIZE));
      memcpy(data,"{\"block_verifier_online_total_rounds\":\"",39);
      sprintf(data+39,"%zu",number); 
      memcpy(data+39+count2,"\"}",2);
      if (update_document_from_collection(DATABASE_NAME,"delegates",message,data,0) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not update the block_verifier_online_total_rounds in the database\nFunction: add_block_verifiers_round_statistics");
      }
    }

    // add one to the VRF_node_public_and_private_key_total_rounds and the current block height to the VRF_node_public_and_private_key_block_heights if the public address is the block producer
    if (memcmp(previous_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.vrf_node_public_and_secret_key_public_address,XCASH_WALLET_LENGTH) == 0)
    {
      memset(data,0,strnlen(data,BUFFER_SIZE));
      if (read_document_field_from_collection(DATABASE_NAME,"delegates",message,"VRF_node_public_and_private_key_total_rounds",data,0) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not read the VRF_node_public_and_private_key_total_rounds from the database\nFunction: add_block_verifiers_round_statistics");
      }
      count2 = strnlen(data,BUFFER_SIZE);
      sscanf(data, "%zu", &number);
      number++;
      memset(data,0,strnlen(data,BUFFER_SIZE));
      memcpy(data,"{\"VRF_node_public_and_private_key_total_rounds\":\"",49);
      sprintf(data+49,"%zu",number); 
      memcpy(data+49+count2,"\"}",2);
      if (update_document_from_collection(DATABASE_NAME,"delegates",message,data,0) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not update the VRF_node_public_and_private_key_total_rounds in the database\nFunction: add_block_verifiers_round_statistics");
      }

      memset(data,0,strnlen(data,BUFFER_SIZE));
      if (read_document_field_from_collection(DATABASE_NAME,"delegates",message,"VRF_node_public_and_private_key_block_heights",data,0) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not read the VRF_node_public_and_private_key_block_heights from the database\nFunction: add_block_verifiers_round_statistics");
      }      
      memcpy(data,",",1);
      memcpy(data+1,current_block_height,strnlen(current_block_height,BUFFER_SIZE));
      memset(data2,0,strnlen(data2,BUFFER_SIZE));
      memcpy(data2,"{\"VRF_node_public_and_private_key_block_heights\":\"",50);
      memcpy(data2+50,data,strnlen(data,BUFFER_SIZE));
      memcpy(data2+50+strnlen(data,BUFFER_SIZE),"\"}",2);
      if (update_document_from_collection(DATABASE_NAME,"delegates",message,data2,0) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not update the VRF_node_public_and_private_key_block_heights in the database\nFunction: add_block_verifiers_round_statistics");
      }
    }

    // add one to the VRF_node_random_data_total_rounds and the current block height to the block_producer_block_heights if the public address is the block producer
    if (memcmp(previous_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.vrf_node_random_data_public_address,XCASH_WALLET_LENGTH) == 0)
    {
      memset(data,0,strnlen(data,BUFFER_SIZE));
      if (read_document_field_from_collection(DATABASE_NAME,"delegates",message,"VRF_node_random_data_total_rounds",data,0) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not read the VRF_node_random_data_total_rounds from the database\nFunction: add_block_verifiers_round_statistics");
      }
      count2 = strnlen(data,BUFFER_SIZE);
      sscanf(data, "%zu", &number);
      number++;
      memset(data,0,strnlen(data,BUFFER_SIZE));
      memcpy(data,"{\"VRF_node_random_data_total_rounds\":\"",38);
      sprintf(data+38,"%zu",number); 
      memcpy(data+38+count2,"\"}",2);
      if (update_document_from_collection(DATABASE_NAME,"delegates",message,data,0) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not update the VRF_node_random_data_total_rounds in the database\nFunction: add_block_verifiers_round_statistics");
      }

      memset(data,0,strnlen(data,BUFFER_SIZE));
      if (read_document_field_from_collection(DATABASE_NAME,"delegates",message,"VRF_node_random_data_block_heights",data,0) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not read the VRF_node_random_data_block_heights from the database\nFunction: add_block_verifiers_round_statistics");
      }      
      memcpy(data,",",1);
      memcpy(data+1,current_block_height,strnlen(current_block_height,BUFFER_SIZE));
      memset(data2,0,strnlen(data2,BUFFER_SIZE));
      memcpy(data2,"{\"VRF_node_random_data_block_heights\":\"",39);
      memcpy(data2+39,data,strnlen(data,BUFFER_SIZE));
      memcpy(data2+39+strnlen(data,BUFFER_SIZE),"\"}",2);
      if (update_document_from_collection(DATABASE_NAME,"delegates",message,data2,0) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not update the VRF_node_random_data_block_heights in the database\nFunction: add_block_verifiers_round_statistics");
      }
    }

    // add one to the VRF_node_public_and_private_key_total_rounds and the current block height to the VRF_node_public_and_private_key_block_heights if the public address is the block producer
    if (memcmp(previous_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.vrf_node_next_main_nodes_public_address,XCASH_WALLET_LENGTH) == 0)
    {
      memset(data,0,strnlen(data,BUFFER_SIZE));
      if (read_document_field_from_collection(DATABASE_NAME,"delegates",message,"VRF_node_next_main_nodes_total_rounds",data,0) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not read the VRF_node_next_main_nodes_total_rounds from the database\nFunction: add_block_verifiers_round_statistics");
      }
      count2 = strnlen(data,BUFFER_SIZE);
      sscanf(data, "%zu", &number);
      number++;
      memset(data,0,strnlen(data,BUFFER_SIZE));
      memcpy(data,"{\"VRF_node_next_main_nodes_total_rounds\":\"",42);
      sprintf(data+42,"%zu",number); 
      memcpy(data+42+count2,"\"}",2);
      if (update_document_from_collection(DATABASE_NAME,"delegates",message,data,0) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not update the VRF_node_next_main_nodes_total_rounds in the database\nFunction: add_block_verifiers_round_statistics");
      }

      memset(data,0,strnlen(data,BUFFER_SIZE));
      if (read_document_field_from_collection(DATABASE_NAME,"delegates",message,"VRF_node_next_main_nodes_block_heights",data,0) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not read the VRF_node_next_main_nodes_block_heights from the database\nFunction: add_block_verifiers_round_statistics");
      }      
      memcpy(data,",",1);
      memcpy(data+1,current_block_height,strnlen(current_block_height,BUFFER_SIZE));
      memset(data2,0,strnlen(data2,BUFFER_SIZE));
      memcpy(data2,"{\"VRF_node_next_main_nodes_block_heights\":\"",43);
      memcpy(data2+43,data,strnlen(data,BUFFER_SIZE));
      memcpy(data2+43+strnlen(data,BUFFER_SIZE),"\"}",2);
      if (update_document_from_collection(DATABASE_NAME,"delegates",message,data2,0) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not update the VRF_node_next_main_nodes_block_heights in the database\nFunction: add_block_verifiers_round_statistics");
      }
    }

    // add one to the block_producer_total_rounds and the current block height to the block_producer_block_heights if the public address is the block producer
    if (memcmp(previous_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_public_address,XCASH_WALLET_LENGTH) == 0)
    {
      memset(data,0,strnlen(data,BUFFER_SIZE));
      if (read_document_field_from_collection(DATABASE_NAME,"delegates",message,"block_producer_total_rounds",data,0) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not read the block_producer_total_rounds from the database\nFunction: add_block_verifiers_round_statistics");
      }
      count2 = strnlen(data,BUFFER_SIZE);
      sscanf(data, "%zu", &number);
      number++;
      memset(data,0,strnlen(data,BUFFER_SIZE));
      memcpy(data,"{\"block_producer_total_rounds\":\"",39);
      sprintf(data+39,"%zu",number); 
      memcpy(data+39+count2,"\"}",2);
      if (update_document_from_collection(DATABASE_NAME,"delegates",message,data,0) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not update the block_producer_total_rounds in the database\nFunction: add_block_verifiers_round_statistics");
      }

      memset(data,0,strnlen(data,BUFFER_SIZE));
      if (read_document_field_from_collection(DATABASE_NAME,"delegates",message,"block_producer_block_heights",data,0) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not read the block_producer_block_heights from the database\nFunction: add_block_verifiers_round_statistics");
      }      
      memcpy(data,",",1);
      memcpy(data+1,current_block_height,strnlen(current_block_height,BUFFER_SIZE));
      memset(data2,0,strnlen(data2,BUFFER_SIZE));
      memcpy(data2,"{\"block_producer_block_heights\":\"",33);
      memcpy(data2+33,data,strnlen(data,BUFFER_SIZE));
      memcpy(data2+33+strnlen(data,BUFFER_SIZE),"\"}",2);
      if (update_document_from_collection(DATABASE_NAME,"delegates",message,data2,0) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not update the block_producer_block_heights in the database\nFunction: add_block_verifiers_round_statistics");
      }
    }
  }

  return;

  #undef pointer_reset_all
  #undef ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR
}




/*
-----------------------------------------------------------------------------------------------------------
Name: add_round_statistics
Description: Adds the round statistics to the database after adding the block to the network
-----------------------------------------------------------------------------------------------------------
*/

void add_round_statistics()
{
  // Constants
  const bson_t* current_document;

  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* settings = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* delegates_name = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* block_verifier_total_rounds_delegates_name = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* best_block_verifier_online_percentage_delegate_name = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* most_block_producer_total_rounds_delegate_name = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* most_VRF_node_public_and_private_key_total_rounds_delegate_name = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* most_VRF_node_random_data_total_rounds_delegate_name = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* most_VRF_next_main_nodes_total_rounds_delegate_name = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message1 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message4 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message5 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message6 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message7 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message8 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message9 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message10 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message11 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message12 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t block_verifier_total_rounds_count = 0;
  size_t block_verifier_total_rounds_count2 = 0;
  size_t most_block_producer_total_rounds_count = 0;
  size_t most_block_producer_total_rounds_count2 = 0;
  size_t most_VRF_node_public_and_private_key_total_rounds_count = 0;
  size_t most_VRF_node_public_and_private_key_total_rounds_count2 = 0;
  size_t most_VRF_node_random_data_total_rounds_count = 0;
  size_t most_VRF_node_random_data_total_rounds_count2 = 0;
  size_t most_VRF_node_next_main_nodes_total_rounds_count = 0;
  size_t most_VRF_node_next_main_nodes_total_rounds_count2 = 0;
  double total;
  double total2;
  double total3;
  mongoc_collection_t* collection;
  mongoc_cursor_t* document_settings;
  bson_t* document = NULL;  
  char* message;
  char* message_copy1;
  char* message_copy2;

  // define macros
  #define MESSAGE "{\"username\":\"xcash\"}"
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; \
  free(data3); \
  data3 = NULL; \
  free(settings); \
  settings = NULL; \
  free(delegates_name); \
  delegates_name = NULL; \
  free(block_verifier_total_rounds_delegates_name); \
  block_verifier_total_rounds_delegates_name = NULL; \
  free(best_block_verifier_online_percentage_delegate_name); \
  best_block_verifier_online_percentage_delegate_name = NULL; \
  free(most_block_producer_total_rounds_delegate_name); \
  most_block_producer_total_rounds_delegate_name = NULL; \
  free(most_VRF_node_public_and_private_key_total_rounds_delegate_name); \
  most_VRF_node_public_and_private_key_total_rounds_delegate_name = NULL; \
  free(most_VRF_node_random_data_total_rounds_delegate_name); \
  most_VRF_node_random_data_total_rounds_delegate_name = NULL; \
  free(most_VRF_next_main_nodes_total_rounds_delegate_name); \
  most_VRF_next_main_nodes_total_rounds_delegate_name = NULL; \
  free(message1); \
  message1 = NULL; \
  free(message2); \
  message2 = NULL; \
  free(message3); \
  message3 = NULL; \
  free(message4); \
  message4 = NULL; \
  free(message5); \
  message5 = NULL; \
  free(message6); \
  message6 = NULL; \
  free(message7); \
  message7 = NULL; \
  free(message8); \
  message8 = NULL; \
  free(message9); \
  message9 = NULL; \
  free(message10); \
  message10 = NULL; \
  free(message11); \
  message11 = NULL; \
  free(message12); \
  message12 = NULL;

  #define ADD_ROUND_STATISTICS_ERROR(settings) \
  color_print(settings,"red"); \
  pointer_reset(data); \
  return;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL || data3 == NULL || settings == NULL || delegates_name == NULL || block_verifier_total_rounds_delegates_name == NULL || best_block_verifier_online_percentage_delegate_name == NULL || most_block_producer_total_rounds_delegate_name == NULL || most_VRF_node_public_and_private_key_total_rounds_delegate_name == NULL || most_VRF_node_random_data_total_rounds_delegate_name == NULL || most_VRF_next_main_nodes_total_rounds_delegate_name == NULL || message1 == NULL || message2 == NULL || message3 == NULL || message4 == NULL || message5 == NULL || message6 == NULL || message7 == NULL || message8 == NULL || message9 == NULL || message10 == NULL || message11 == NULL || message12 == NULL)
  {   
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    if (data3 != NULL)
    {
      pointer_reset(data3);
    }
    if (settings != NULL)
    {
      pointer_reset(settings);
    }
    if (delegates_name != NULL)
    {
      pointer_reset(delegates_name);
    }
    if (block_verifier_total_rounds_delegates_name != NULL)
    {
      pointer_reset(block_verifier_total_rounds_delegates_name);
    }
    if (best_block_verifier_online_percentage_delegate_name != NULL)
    {
      pointer_reset(best_block_verifier_online_percentage_delegate_name);
    }
    if (most_block_producer_total_rounds_delegate_name != NULL)
    {
      pointer_reset(most_block_producer_total_rounds_delegate_name);
    }
    if (most_VRF_node_public_and_private_key_total_rounds_delegate_name != NULL)
    {
      pointer_reset(most_VRF_node_public_and_private_key_total_rounds_delegate_name);
    }
    if (most_VRF_node_random_data_total_rounds_delegate_name != NULL)
    {
      pointer_reset(most_VRF_node_random_data_total_rounds_delegate_name);
    }
    if (most_VRF_next_main_nodes_total_rounds_delegate_name != NULL)
    {
      pointer_reset(most_VRF_next_main_nodes_total_rounds_delegate_name);
    }
    if (message1 != NULL)
    {
      pointer_reset(message1);
    }
    if (message2 != NULL)
    {
      pointer_reset(message2);
    }
    if (message3 != NULL)
    {
      pointer_reset(message3);
    }
    if (message4 != NULL)
    {
      pointer_reset(message4);
    }
    if (message5 != NULL)
    {
      pointer_reset(message5);
    }
    if (message6 != NULL)
    {
      pointer_reset(message6);
    }
    if (message7 != NULL)
    {
      pointer_reset(message7);
    }
    if (message8 != NULL)
    {
      pointer_reset(message8);
    }
    if (message9 != NULL)
    {
      pointer_reset(message9);
    }
    if (message10 != NULL)
    {
      pointer_reset(message10);
    }
    if (message11 != NULL)
    {
      pointer_reset(message11);
    }
    if (message12 != NULL)
    {
      pointer_reset(message12);
    }
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }  
  
  // set the collection
  collection = mongoc_client_get_collection(database_client, DATABASE_NAME, "delegates");
  document = bson_new(); 
  document_settings = mongoc_collection_find_with_opts(collection, document, NULL, NULL);
  while (mongoc_cursor_next(document_settings, &current_document))
  {
    // reset the variables
    memset(data,0,strnlen(data,10485760));
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memset(data3,0,strnlen(data3,BUFFER_SIZE));
    memset(settings,0,strnlen(settings,BUFFER_SIZE));

    message = bson_as_canonical_extended_json(current_document, NULL);
    memcpy(data,message,strnlen(message,BUFFER_SIZE));
    bson_free(message);

    // get the delegate_name
    memcpy(data2,", \"",3);
    memcpy(data2+3,"delegate_name",13);
    memcpy(data2+16,"\" : \"",5);

    message_copy1 = strstr(data,data2) + strnlen(data2,BUFFER_SIZE);
    message_copy2 = strstr(message_copy1,"\"");
    memcpy(delegates_name,message_copy1,message_copy2 - message_copy1);
    memset(data2,0,strnlen(data2,BUFFER_SIZE));

    // get the block_verifier_total_rounds
    memcpy(data2,", \"",3);
    memcpy(data2+3,"block_verifier_total_rounds",27);
    memcpy(data2+30,"\" : \"",5);

    message_copy1 = strstr(data,data2) + strnlen(data2,BUFFER_SIZE);
    message_copy2 = strstr(message_copy1,"\"");
    memcpy(data3,message_copy1,message_copy2 - message_copy1);
    sscanf(data3, "%zu", &block_verifier_total_rounds_count2);
    sscanf(data3, "%lf", &total2);
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memset(data3,0,strnlen(data3,BUFFER_SIZE));

    if (block_verifier_total_rounds_count2 > block_verifier_total_rounds_count)
    {
      block_verifier_total_rounds_count = block_verifier_total_rounds_count2;
      memset(block_verifier_total_rounds_delegates_name,0,strnlen(block_verifier_total_rounds_delegates_name,BUFFER_SIZE));
      memcpy(block_verifier_total_rounds_delegates_name,delegates_name,strnlen(delegates_name,BUFFER_SIZE));
    }

    // get the block_verifier_online_total_rounds
    memcpy(data2,", \"",3);
    memcpy(data2+3,"block_verifier_online_total_rounds",34);
    memcpy(data2+34,"\" : \"",5);

    message_copy1 = strstr(data,data2) + strnlen(data2,BUFFER_SIZE);
    message_copy2 = strstr(message_copy1,"\"");
    memcpy(data3,message_copy1,message_copy2 - message_copy1);
    sscanf(data3, "%lf", &total3);
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memset(data3,0,strnlen(data3,BUFFER_SIZE));

    if (total3 / total2 > total)
    {
      total = total3 / total2;
      memset(best_block_verifier_online_percentage_delegate_name,0,strnlen(best_block_verifier_online_percentage_delegate_name,BUFFER_SIZE));
      memcpy(best_block_verifier_online_percentage_delegate_name,delegates_name,strnlen(delegates_name,BUFFER_SIZE));
    }

    // get the block_producer_total_rounds
    memcpy(data2,", \"",3);
    memcpy(data2+3,"block_producer_total_rounds",27);
    memcpy(data2+30,"\" : \"",5);
    message_copy1 = strstr(data,data2) + strnlen(data2,BUFFER_SIZE);
    message_copy2 = strstr(message_copy1,"\"");
    memcpy(data3,message_copy1,message_copy2 - message_copy1);
    sscanf(data3, "%zu", &most_block_producer_total_rounds_count2);
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memset(data3,0,strnlen(data3,BUFFER_SIZE));

    if (most_block_producer_total_rounds_count2 > most_block_producer_total_rounds_count)
    {
      most_block_producer_total_rounds_count = most_block_producer_total_rounds_count2;
      memset(most_block_producer_total_rounds_delegate_name,0,strnlen(most_block_producer_total_rounds_delegate_name,BUFFER_SIZE));
      memcpy(most_block_producer_total_rounds_delegate_name,delegates_name,strnlen(delegates_name,BUFFER_SIZE));
    }

    // get the VRF_node_public_and_private_key_total_rounds
    memcpy(data2,", \"",3);
    memcpy(data2+3,"VRF_node_public_and_private_key_total_rounds",44);
    memcpy(data2+47,"\" : \"",5);
    message_copy1 = strstr(data,data2) + strnlen(data2,BUFFER_SIZE);
    message_copy2 = strstr(message_copy1,"\"");
    memcpy(data3,message_copy1,message_copy2 - message_copy1);
    sscanf(data3, "%zu", &most_VRF_node_public_and_private_key_total_rounds_count2);
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memset(data3,0,strnlen(data3,BUFFER_SIZE));

    if (most_VRF_node_public_and_private_key_total_rounds_count2 > most_VRF_node_public_and_private_key_total_rounds_count)
    {
      most_VRF_node_public_and_private_key_total_rounds_count = most_VRF_node_public_and_private_key_total_rounds_count2;
      memset(most_VRF_node_public_and_private_key_total_rounds_delegate_name,0,strnlen(most_VRF_node_public_and_private_key_total_rounds_delegate_name,BUFFER_SIZE));
      memcpy(most_VRF_node_public_and_private_key_total_rounds_delegate_name,delegates_name,strnlen(delegates_name,BUFFER_SIZE));
    }

    // get the VRF_node_random_data_total_rounds
    memcpy(data2,", \"",3);
    memcpy(data2+3,"VRF_node_random_data_total_rounds",33);
    memcpy(data2+36,"\" : \"",5);
    message_copy1 = strstr(data,data2) + strnlen(data2,BUFFER_SIZE);
    message_copy2 = strstr(message_copy1,"\"");
    memcpy(data3,message_copy1,message_copy2 - message_copy1);
    sscanf(data3, "%zu", &most_VRF_node_random_data_total_rounds_count2);
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memset(data3,0,strnlen(data3,BUFFER_SIZE));

    if (most_VRF_node_random_data_total_rounds_count2 > most_VRF_node_random_data_total_rounds_count)
    {
      most_VRF_node_random_data_total_rounds_count = most_VRF_node_random_data_total_rounds_count2;
      memset(most_VRF_node_random_data_total_rounds_delegate_name,0,strnlen(most_VRF_node_random_data_total_rounds_delegate_name,BUFFER_SIZE));
      memcpy(most_VRF_node_random_data_total_rounds_delegate_name,delegates_name,strnlen(delegates_name,BUFFER_SIZE));
    }

    // get the VRF_node_next_main_nodes_total_rounds
    memcpy(data2,", \"",3);
    memcpy(data2+3,"VRF_node_next_main_nodes_total_rounds",37);
    memcpy(data2+40,"\" : \"",5);
    message_copy1 = strstr(data,data2) + strnlen(data2,BUFFER_SIZE);
    message_copy2 = strstr(message_copy1,"\"");
    memcpy(data3,message_copy1,message_copy2 - message_copy1);
    sscanf(data3, "%zu", &most_VRF_node_next_main_nodes_total_rounds_count2);
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memset(data3,0,strnlen(data3,BUFFER_SIZE));

    if (most_VRF_node_next_main_nodes_total_rounds_count2 > most_VRF_node_next_main_nodes_total_rounds_count)
    {
      most_VRF_node_next_main_nodes_total_rounds_count = most_VRF_node_next_main_nodes_total_rounds_count2;
      memset(most_VRF_next_main_nodes_total_rounds_delegate_name,0,strnlen(most_VRF_next_main_nodes_total_rounds_delegate_name,BUFFER_SIZE));
      memcpy(most_VRF_next_main_nodes_total_rounds_delegate_name,delegates_name,strnlen(delegates_name,BUFFER_SIZE));
    }
  }

  // create the message
  memcpy(message1,"{\"most_total_rounds_delegate_name\":\"",36);
  memcpy(message1+36,block_verifier_total_rounds_delegates_name,strnlen(block_verifier_total_rounds_delegates_name,BUFFER_SIZE));
  memcpy(message1+36+strnlen(block_verifier_total_rounds_delegates_name,BUFFER_SIZE),"\"}",2);

  memcpy(message2,"{\"most_total_rounds\":\"",25);
  sprintf(message2+25,"%zu",block_verifier_total_rounds_count);
  memcpy(message2+strnlen(message2,BUFFER_SIZE),"\"}",2);

  memcpy(message3,"{\"best_block_verifier_online_percentage_delegate_name\":\"",56);
  memcpy(message3+56,best_block_verifier_online_percentage_delegate_name,strnlen(settings,BUFFER_SIZE));
  memcpy(message3+56+strnlen(best_block_verifier_online_percentage_delegate_name,BUFFER_SIZE),"\"}",2);

  memcpy(message4,"{\"best_block_verifier_online_percentage\":\"",45);
  sprintf(message4+45,"%lf",total);
  memcpy(message4+strnlen(message4,BUFFER_SIZE),"\"}",2);

  memcpy(message5,"{\"most_block_producer_total_rounds_delegate_name\":\"",51);
  memcpy(message5+51,most_block_producer_total_rounds_delegate_name,strnlen(most_block_producer_total_rounds_delegate_name,BUFFER_SIZE));
  memcpy(message5+51+strnlen(most_block_producer_total_rounds_delegate_name,BUFFER_SIZE),"\"}",2);

  memcpy(message6,"{\"most_block_producer_total_rounds\":\"",40);
  sprintf(message6+40,"%zu",most_block_producer_total_rounds_count);
  memcpy(message6+strnlen(message6,BUFFER_SIZE),"\"}",2);

  memcpy(message7,"{\"most_VRF_node_public_and_private_key_total_rounds_delegate_name\":\"",68);
  memcpy(message7+68,most_VRF_node_public_and_private_key_total_rounds_delegate_name,strnlen(most_VRF_node_public_and_private_key_total_rounds_delegate_name,BUFFER_SIZE));
  memcpy(message7+68+strnlen(most_VRF_node_public_and_private_key_total_rounds_delegate_name,BUFFER_SIZE),"\"}",2);

  memcpy(message8,"{\"most_VRF_node_public_and_private_key_total_rounds\":\"",54);
  sprintf(message8+54,"%zu",most_VRF_node_public_and_private_key_total_rounds_count);
  memcpy(message8+strnlen(message6,BUFFER_SIZE),"\"}",2);

  memcpy(message9,"{\"most_VRF_node_random_data_total_rounds_delegate_name\":\"",57);
  memcpy(message9+57,most_VRF_node_random_data_total_rounds_delegate_name,strnlen(most_VRF_node_random_data_total_rounds_delegate_name,BUFFER_SIZE));
  memcpy(message9+57+strnlen(most_VRF_node_random_data_total_rounds_delegate_name,BUFFER_SIZE),"\"}",2);

  memcpy(message10,"{\"most_VRF_node_random_data_total_rounds\":\"",43);
  sprintf(message10+43,"%zu",most_VRF_node_random_data_total_rounds_count);
  memcpy(message10+strnlen(message6,BUFFER_SIZE),"\"}",2);

  memcpy(message11,"{\"most_VRF_node_next_main_nodes_total_rounds_delegate_name\":\"",61);
  memcpy(message11+61,most_VRF_next_main_nodes_total_rounds_delegate_name,strnlen(most_VRF_next_main_nodes_total_rounds_delegate_name,BUFFER_SIZE));
  memcpy(message11+61+strnlen(most_VRF_next_main_nodes_total_rounds_delegate_name,BUFFER_SIZE),"\"}",2);

  memcpy(message12,"{\"most_VRF_node_next_main_nodes_total_rounds\":\"",47);
  sprintf(message12+47,"%zu",most_VRF_node_next_main_nodes_total_rounds_count);
  memcpy(message12+strnlen(message6,BUFFER_SIZE),"\"}",2);

  // update the database
  if (update_document_from_collection(DATABASE_NAME,"statistics",MESSAGE,message1,0) == 0 || update_document_from_collection(DATABASE_NAME,"statistics",MESSAGE,message2,0) == 0 || update_document_from_collection(DATABASE_NAME,"statistics",MESSAGE,message3,0) == 0 || update_document_from_collection(DATABASE_NAME,"statistics",MESSAGE,message4,0) == 0 || update_document_from_collection(DATABASE_NAME,"statistics",MESSAGE,message5,0) == 0 || update_document_from_collection(DATABASE_NAME,"statistics",MESSAGE,message6,0) == 0 || update_document_from_collection(DATABASE_NAME,"statistics",MESSAGE,message7,0) == 0 || update_document_from_collection(DATABASE_NAME,"statistics",MESSAGE,message8,0) == 0 || update_document_from_collection(DATABASE_NAME,"statistics",MESSAGE,message9,0) == 0 || update_document_from_collection(DATABASE_NAME,"statistics",MESSAGE,message10,0) == 0 || update_document_from_collection(DATABASE_NAME,"statistics",MESSAGE,message11,0) == 0 || update_document_from_collection(DATABASE_NAME,"statistics",MESSAGE,message12,0) == 0)
  {
    ADD_ROUND_STATISTICS_ERROR("Could not update the round statistics in the database\nFunction: add_round_statistics");
  }

  return;

  #undef MESSAGE
  #undef pointer_reset_all
  #undef ADD_ROUND_STATISTICS_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_updated_node_list
Description: Gets the updated node list, so it will know what nodes to accept data from
Parameters:
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int get_updated_node_list()
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;
  size_t count2;

   // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; \
  free(message); \
  message = NULL; \
  free(message2); \
  message2 = NULL; \
  free(message3); \
  message3 = NULL;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL || message == NULL || message2 == NULL || message3 == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    if (message != NULL)
    {
      pointer_reset(message);
    }
    if (message2 != NULL)
    {
      pointer_reset(message2);
    }
    if (message3 != NULL)
    {
      pointer_reset(message3);
    }
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  #define GET_UPDATED_NODE_LIST_ERROR(settings) \
  color_print(settings,"red"); \
  pointer_reset_all; \
  return 0;

  
  return 1;

  #undef pointer_reset_all
  #undef GET_UPDATED_NODE_LIST_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_received_data_xcash_proof_of_stake_test_data
Description: Runs the code when the server receives the xcash_proof_of_stake_test_data message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_received_data_xcash_proof_of_stake_test_data(const int CLIENT_SOCKET, char* MESSAGE)
{
  // verify the message
  if (verify_data(MESSAGE,0,1,1) == 0)
  {   
    return 0;
  }
  else
  {
    if (send_data(CLIENT_SOCKET,MESSAGE,1) == 1)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_received_data_xcash_proof_of_stake_test_data
Description: Runs the code when the server receives the xcash_proof_of_stake_test_data message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_block_verifiers_to_block_verifiers_invalid_reserve_proofs(const char* MESSAGE)
{
  // Variables
  char* block_verifiers_public_address = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  char* public_address = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  char* reserve_proof = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));
  char* data = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  char* data2 = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  char* data3 = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  // since were going to be changing where data and data2 are referencing, we need to create a copy to pointer_reset
  char* datacopy = data; 
  char* datacopy2 = data2; 
  size_t count;
  size_t count2 = string_count(MESSAGE,"|") + 1;
  size_t count3;
  int settings;

  // define macros
  #define pointer_reset_all \
  free(block_verifiers_public_address); \
  block_verifiers_public_address = NULL; \
  free(public_address); \
  public_address = NULL; \
  free(reserve_proof); \
  reserve_proof = NULL; \
  free(datacopy); \
  datacopy = NULL; \
  free(datacopy2); \
  datacopy2 = NULL; \
  free(data3); \
  data3 = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS_ERROR(settings) \
  color_print(settings,"red"); \
  pointer_reset_all; \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (block_verifiers_public_address = NULL || public_address == NULL || reserve_proof == NULL || data == NULL || data2 == NULL || data3 == NULL)
  {
    if (block_verifiers_public_address != NULL)
    {
      pointer_reset(block_verifiers_public_address);
    }
    if (public_address != NULL)
    {
      pointer_reset(public_address);
    }
    if (reserve_proof != NULL)
    {
      pointer_reset(reserve_proof);
    }
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    if (data3 != NULL)
    {
      pointer_reset(data3);
    }
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // verify the message
  if (verify_data(MESSAGE,0,0,0) == 0)
  {   
    return 0;
  }

  // parse the message
  if (parse_json_data(MESSAGE,"public_address",block_verifiers_public_address) == 0 || parse_json_data(MESSAGE,"public_address_that_created_the_reserve_proof",data) == 0 || parse_json_data(MESSAGE,"reserve_proof",data2) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS_ERROR("Could not parse the message\nFunction: server_receive_data_socket_block_verifiers_to_block_verifiers_invalid_reserve_proofs\nReceived Message: BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS");
  }

  // loop through all of the reserve proofs and only check reserve proofs that are not already in the invalid reserve proofs struct
  for (count = 0; count < count2; count++)
  {
    // get the public address
    memset(public_address,0,strlen(public_address));
    memcpy(public_address,data,strnlen(strstr(data,"|"),XCASH_WALLET_LENGTH));
    data = strstr(data,"|") + 1;

    // get the reserve proof
    memset(reserve_proof,0,strlen(reserve_proof));
    memcpy(reserve_proof,data2,strnlen(strstr(data2,"|"),BUFFER_SIZE_RESERVE_PROOF));
    data2 = strstr(data2,"|") + 1;

    // check if the reserve proof is unique and formated correctly
    if (memcmp(reserve_proof,"ReserveProofV1",14) == 0)
    {
      for (count3 = 0, settings = 1; count3 < invalid_reserve_proofs.count; count3++)
      {
        if (strncmp(invalid_reserve_proofs.reserve_proof[count3],reserve_proof,BUFFER_SIZE_RESERVE_PROOF) == 0)
        {
          settings = 0;
        }
      }
      if (settings == 1)
      {
        // check if the reserve proof is valid
        memset(data3,0,strlen(data3));
        if (check_reserve_proofs(data3,public_address,reserve_proof,0) == 0)
        {
          // add the reserve proof to the invalid_reserve_proofs struct
          memcpy(invalid_reserve_proofs.block_verifier_public_address[invalid_reserve_proofs.count],block_verifiers_public_address,strnlen(block_verifiers_public_address,XCASH_WALLET_LENGTH));
          memcpy(invalid_reserve_proofs.public_address[invalid_reserve_proofs.count],public_address,strnlen(public_address,XCASH_WALLET_LENGTH));
          memcpy(invalid_reserve_proofs.reserve_proof[invalid_reserve_proofs.count],reserve_proof,strnlen(reserve_proof,BUFFER_SIZE_RESERVE_PROOF));
          invalid_reserve_proofs.count++;
        }
      }
    } 
  }

  return 1;

  #undef pointer_reset_all
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS_ERROR
}


/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_consensus_node_to_node
Description: Runs the code when the server receives the CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS message
Parameters:
  parameters - A mainnode_timeout_thread_parameters struct
    pid_t process_id - Holds the forked process ID that the client is connected to
    int data_received - 1 if the node has received data from the main node, otherwise 0
    char* main_node - The main node (VRF_PUBLIC_AND_SECRET_KEY, VRF_RANDOM_DATA, BLOCK_PRODUCER)
    char* current_round_part - The current round part (1-4).
    char* current_round_part_backup_node - The current main node in the current round part (0-5)
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_consensus_node_to_node(struct mainnode_timeout_thread_parameters* parameters, char* message)
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // threads
  pthread_t thread_id;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_CONSENSUS_NODE_TO_NODE_ERROR(settings) \
  color_print(settings,"red"); \
  pointer_reset(data); \
  return 0;

  // verify the data
  if (verify_data(message,0,0,0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_CONSENSUS_NODE_TO_NODE_ERROR("Could not verify data\nFunction: server_receive_data_socket_consensus_node_to_node\nReceived Message: CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS");
  }

  // parse the message
  memset(current_round_part,0,strnlen(current_round_part,BUFFER_SIZE));
  memset(current_round_part_backup_node,0,strnlen(current_round_part_backup_node,BUFFER_SIZE));
  if (parse_json_data(message,"current_round_part",current_round_part) == 0 || parse_json_data(message,"current_round_part_backup_node",current_round_part_backup_node) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_CONSENSUS_NODE_TO_NODE_ERROR("Could not parse main_nodes_public_address\nFunction: server_receive_data_socket_consensus_node_to_node\nReceived Message: CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS");
  }
  memset(data,0,strnlen(data,BUFFER_SIZE));

  // create a timeout from the time the consensus node lets us know who the main node is for this part of the round, to the time the main node sends us data.
  if (strncmp(current_round_part,"1",BUFFER_SIZE) == 0 || strncmp(current_round_part,"3",BUFFER_SIZE) == 0)
  {
    memcpy(data,"VRF_PUBLIC_AND_SECRET_KEY",25);
  }
  else if (strncmp(current_round_part,"2",BUFFER_SIZE) == 0)
  {
    memcpy(data,"VRF_RANDOM_DATA",15);
  }
  else if (strncmp(current_round_part,"4",BUFFER_SIZE) == 0)
  {
    memcpy(data,"BLOCK_PRODUCER",14);
  }

  // create a mainnode_timeout_thread_parameters struct since this function will use the mainnode_timeout_thread
  parameters->data_received = 0;
  parameters->main_node = data;
  parameters->current_round_part = current_round_part;
  parameters->current_round_part_backup_node = current_round_part_backup_node;
 
  // create a timeout for this connection, since we need to limit the amount of time a client has to send data from once it connected
  if (pthread_create(&thread_id, NULL, &mainnode_timeout_thread, (void *)parameters) != 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_CONSENSUS_NODE_TO_NODE_ERROR("Could not create the timeout thread\nFunction: server_receive_data_socket_consensus_node_to_node\nReceived Message: CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS");
  }
  // set the thread to dettach once completed, since we do not need to use anything it will return
  if (pthread_detach(thread_id) != 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_CONSENSUS_NODE_TO_NODE_ERROR("Could not start the timeout thread in detach mode\nFunction: server_receive_data_socket_consensus_node_to_node\nReceived Message: CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS");
  }

  // set the next server message
  if (strncmp(current_round_part,"1",BUFFER_SIZE) == 0)
  {
    memset(server_message,0,strnlen(server_message,BUFFER_SIZE));
    memcpy(server_message,"MAIN_NODES_TO_NODES_PART_1_OF_ROUND",35);
  }
  else if (strncmp(current_round_part,"2",BUFFER_SIZE) == 0)
  {
    memset(server_message,0,strnlen(server_message,BUFFER_SIZE));
    memcpy(server_message,"MAIN_NODES_TO_NODES_PART_2_OF_ROUND",35);
  }
  else if (strncmp(current_round_part,"3",BUFFER_SIZE) == 0)
  {
    memset(server_message,0,strnlen(server_message,BUFFER_SIZE));
    memcpy(server_message,"MAIN_NODES_TO_NODES_PART_3_OF_ROUND",35);
  }
  else if (strncmp(current_round_part,"4",BUFFER_SIZE) == 0)
  {
    memset(server_message,0,strnlen(server_message,BUFFER_SIZE));
    memcpy(server_message,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND",35);
  }

  pointer_reset(data);
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_main_node_to_node_message_part_1
Description: Runs the code when the server receives the MAIN_NODES_TO_NODES_PART_1_OF_ROUND message
Parameters:
  mainnode_timeout_thread_parameters - A mainnode_timeout_thread_parameters struct
    pid_t process_id - Holds the forked process ID that the client is connected to
    int data_received - 1 if the node has received data from the main node, otherwise 0
    char* main_node - The main node (VRF_PUBLIC_AND_SECRET_KEY, VRF_RANDOM_DATA, BLOCK_PRODUCER)
    char* current_round_part - The current round part (1-4).
    char* current_round_part_backup_node - The current main node in the current round part (0-5)
  node_to_node_timeout_thread_parameters - A node_to_node_timeout_thread_parameters struct
    pid_t process_id - Holds the forked process ID that the client is connected to
  current_round_part_consensus_node_data - A current_round_part_consensus_node_data struct
    char* vrf_public_key - Holds the forked process ID that the client is connected to
    char* vrf_alpha_string - 1 if the node has received data from the main node, otherwise 0
    char* vrf_proof - The main node (VRF_PUBLIC_AND_SECRET_KEY, VRF_RANDOM_DATA, BLOCK_PRODUCER)
    char* vrf_beta_string - The current round part (1-4).
    char* block_blob - The current main node in the current round part (0-5)
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_main_node_to_node_message_part_1(struct mainnode_timeout_thread_parameters* mainnode_timeout_thread_parameters, struct node_to_node_timeout_thread_parameters* node_to_node_timeout_thread_parameters, char* message)
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  int count = 0;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL || data3 == NULL || message == NULL || message2 == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    if (data3 != NULL)
    {
      pointer_reset(data3);
    }
    if (message2 != NULL)
    {
      pointer_reset(message2);
    }
    if (message3 != NULL)
    {
      pointer_reset(message3);
    }
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // threads
  pthread_t thread_id;

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; \
  free(data3); \
  data3 = NULL; \
  free(message2); \
  message2 = NULL; \
  free(message3); \
  message3 = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_1_ERROR(settings) \
  color_print(settings,"red"); \
  pointer_reset_all; \
  return 0;

  // since the block verifier has received data from the main node, we need to stop the mainnode_timeout_thread
  mainnode_timeout_thread_parameters->data_received = 1;

  // set the next server message since the block verifiers will send the data to each other
  memset(server_message,0,strnlen(server_message,BUFFER_SIZE));
  memcpy(server_message,"NODES_TO_NODES_VOTE_RESULTS",27); 

  // verify the data
  if (verify_data(message,0,1,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_1_ERROR("Could not verify data\nFunction: mainnode_to_node_message_part_1\nReceived Message: MAIN_NODES_TO_NODES_PART_1_OF_ROUND\nSend Message: NODES_TO_NODES_VOTE_RESULTS");
  }

  // parse the message
  memset(vrf_public_key_part_1,0,strnlen(vrf_public_key_part_1,BUFFER_SIZE));
  if (parse_json_data(message,"vrf_public_key",vrf_public_key_part_1) == 0 || parse_json_data(message,"vrf_alpha_string",data) == 0 || parse_json_data(message,"vrf_proof",data2) == 0 || parse_json_data(message,"vrf_beta_string",data3) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_1_ERROR("Could not parse the data\nFunction: mainnode_to_node_message_part_1\nReceived Message: MAIN_NODES_TO_NODES_PART_1_OF_ROUND\nSend Message: NODES_TO_NODES_VOTE_RESULTS");
  }

  // create the message
  memcpy(message2,"{\r\n \"message_settings\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n \"vote_settings\": \"",75);

  // verify the VRF data
  if (crypto_vrf_verify((unsigned char*)data3,(const unsigned char*)vrf_public_key_part_1,(const unsigned char*)data2,(const unsigned char*)data,(unsigned long long)strnlen(data,BUFFER_SIZE)) == 0)
  {
    memcpy(message2+75,"valid",5);
  }
  else
  {
    memcpy(message2+75,"invalid",7);
  }
  memcpy(message2+strnlen(message2,BUFFER_SIZE),"\",\r\n \"vote_data\": \"",19);

  // SHA2-512 hash all of the VRF data
  memcpy(message3,vrf_public_key_part_1,strnlen(vrf_public_key_part_1,BUFFER_SIZE));
  memcpy(message3+strnlen(message3,BUFFER_SIZE),data,strnlen(data,BUFFER_SIZE));
  memcpy(message3+strnlen(message3,BUFFER_SIZE),data2,strnlen(data2,BUFFER_SIZE));
  memcpy(message3+strnlen(message3,BUFFER_SIZE),data3,strnlen(data3,BUFFER_SIZE));
  crypto_hash_sha512((unsigned char*)current_round_part_vote_data.current_vote_results,(const unsigned char*)message3,(unsigned long long)strnlen(message3,BUFFER_SIZE));
  memcpy(message2+strnlen(message,BUFFER_SIZE),current_round_part_vote_data.current_vote_results,DATA_HASH_LENGTH);
  
  memcpy(message2+strnlen(message2,BUFFER_SIZE),"\",\r\n}",5); 

  // save all of the VRF data to the current_round_part_consensus_node_data struct
  memcpy(current_round_part_consensus_node_data.vrf_public_key,vrf_public_key_part_1,strnlen(vrf_public_key_part_1,BUFFER_SIZE));
  memcpy(current_round_part_consensus_node_data.vrf_alpha_string,data,strnlen(data,BUFFER_SIZE));
  memcpy(current_round_part_consensus_node_data.vrf_proof,data2,strnlen(data2,BUFFER_SIZE));
  memcpy(current_round_part_consensus_node_data.vrf_beta_string,data3,strnlen(data3,BUFFER_SIZE));

  // sign_data
  if (sign_data(message2,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_1_ERROR("Could not sign_data\nFunction: mainnode_to_node_message_part_1\nReceived Message: MAIN_NODES_TO_NODES_PART_1_OF_ROUND\nSend Message: NODES_TO_NODES_VOTE_RESULTS");
  }

  // send the message to all block verifiers
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
    {
      send_data_socket(current_block_verifiers_list.block_verifiers_IP_address[count],SEND_DATA_PORT,message2,"sending NODES_TO_NODES_VOTE_RESULTS to the block verifiers",0);
    }
  }

  // start the node_to_node_message_timeout
  if (pthread_create(&thread_id, NULL, &node_to_node_message_timeout_thread, (void *)node_to_node_timeout_thread_parameters) != 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_1_ERROR("Could not create the timeout thread\nFunction: mainnode_to_node_message_part_1\nReceived Message: MAIN_NODES_TO_NODES_PART_1_OF_ROUND\nSend Message: NODES_TO_NODES_VOTE_RESULTS");
  }
  // set the thread to dettach once completed, since we do not need to use anything it will return
  if (pthread_detach(thread_id) != 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_1_ERROR("Could not start the timeout thread in detach mode\nFunction: mainnode_to_node_message_part_1\nReceived Message: MAIN_NODES_TO_NODES_PART_1_OF_ROUND\nSend Message: NODES_TO_NODES_VOTE_RESULTS");
  }

  pointer_reset(data);
  return 1;

  #undef pointer_reset_all
  #undef SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_1_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_main_node_to_node_message_part_2
Description: Runs the code when the server receives the MAIN_NODES_TO_NODES_PART_2_OF_ROUND message
Parameters:
  mainnode_timeout_thread_parameters - A mainnode_timeout_thread_parameters struct
    pid_t process_id - Holds the forked process ID that the client is connected to
    int data_received - 1 if the node has received data from the main node, otherwise 0
    char* main_node - The main node (VRF_PUBLIC_AND_SECRET_KEY, VRF_RANDOM_DATA, BLOCK_PRODUCER)
    char* current_round_part - The current round part (1-4).
    char* current_round_part_backup_node - The current main node in the current round part (0-5)
  node_to_node_timeout_thread_parameters - A node_to_node_timeout_thread_parameters struct
    pid_t process_id - Holds the forked process ID that the client is connected to
  current_round_part_consensus_node_data - A current_round_part_consensus_node_data struct
    char* vrf_public_key - Holds the forked process ID that the client is connected to
    char* vrf_alpha_string - 1 if the node has received data from the main node, otherwise 0
    char* vrf_proof - The main node (VRF_PUBLIC_AND_SECRET_KEY, VRF_RANDOM_DATA, BLOCK_PRODUCER)
    char* vrf_beta_string - The current round part (1-4).
    char* block_blob - The current main node in the current round part (0-5)
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_main_node_to_node_message_part_2(struct mainnode_timeout_thread_parameters* mainnode_timeout_thread_parameters, struct node_to_node_timeout_thread_parameters* node_to_node_timeout_thread_parameters, char* message)
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  int count = 0;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL || data3 == NULL || message == NULL || message2 == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    if (data3 != NULL)
    {
      pointer_reset(data3);
    }
    if (message2 != NULL)
    {
      pointer_reset(message2);
    }
    if (message3 != NULL)
    {
      pointer_reset(message3);
    }
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // threads
  pthread_t thread_id;

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; \
  free(data3); \
  data3 = NULL; \
  free(message2); \
  message2 = NULL; \
  free(message3); \
  message3 = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_2_ERROR(settings) \
  color_print(settings,"red"); \
  pointer_reset_all; \
  return 0;

  // since the block verifier has received data from the main node, we need to stop the mainnode_timeout_thread
  mainnode_timeout_thread_parameters->data_received = 1;

  // set the next server message since the block verifiers will send the data to each other
  memset(server_message,0,strnlen(server_message,BUFFER_SIZE));
  memcpy(server_message,"NODES_TO_NODES_VOTE_RESULTS",27); 

  // verify the data
  if (verify_data(message,0,1,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_2_ERROR("Could not verify data\nFunction: mainnode_to_node_message_part_1\nReceived Message: MAIN_NODES_TO_NODES_PART_2_OF_ROUND\nSend Message: NODES_TO_NODES_VOTE_RESULTS");
  }

  // parse the message
  memset(vrf_alpha_string_part_2,0,strnlen(vrf_alpha_string_part_2,BUFFER_SIZE));
  if (parse_json_data(message,"vrf_public_key",data) == 0 || parse_json_data(message,"vrf_alpha_string",vrf_alpha_string_part_2) == 0 || parse_json_data(message,"vrf_proof",data2) == 0 || parse_json_data(message,"vrf_beta_string",data3) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_2_ERROR("Could not parse the data\nFunction: mainnode_to_node_message_part_1\nReceived Message: MAIN_NODES_TO_NODES_PART_2_OF_ROUND\nSend Message: NODES_TO_NODES_VOTE_RESULTS");
  }

  // create the message
  memcpy(message2,"{\r\n \"message_settings\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n \"vote_settings\": \"",75);

  // verify the VRF data
  if (crypto_vrf_verify((unsigned char*)data3,(const unsigned char*)data,(const unsigned char*)data2,(const unsigned char*)vrf_alpha_string_part_2,(unsigned long long)strnlen(vrf_alpha_string_part_2,BUFFER_SIZE)) == 0)
  {
    memcpy(message2+75,"valid",5);
  }
  else
  {
    memcpy(message2+75,"invalid",7);
  }
  memcpy(message2+strnlen(message2,BUFFER_SIZE),"\",\r\n \"vote_data\": \"",19);

  // SHA2-512 hash all of the VRF data
  memcpy(message3,data,strnlen(data,BUFFER_SIZE));
  memcpy(message3+strnlen(message3,BUFFER_SIZE),vrf_alpha_string_part_2,strnlen(vrf_alpha_string_part_2,BUFFER_SIZE));
  memcpy(message3+strnlen(message3,BUFFER_SIZE),data2,strnlen(data2,BUFFER_SIZE));
  memcpy(message3+strnlen(message3,BUFFER_SIZE),data3,strnlen(data3,BUFFER_SIZE));
  crypto_hash_sha512((unsigned char*)current_round_part_vote_data.current_vote_results,(const unsigned char*)message3,(unsigned long long)strnlen(message3,BUFFER_SIZE));
  memcpy(message2+strnlen(message,BUFFER_SIZE),current_round_part_vote_data.current_vote_results,DATA_HASH_LENGTH);
  
  memcpy(message2+strnlen(message2,BUFFER_SIZE),"\",\r\n}",5); 

  // save all of the VRF data to the current_round_part_consensus_node_data struct
  memcpy(current_round_part_consensus_node_data.vrf_public_key,data,strnlen(data,BUFFER_SIZE));
  memcpy(current_round_part_consensus_node_data.vrf_alpha_string,vrf_alpha_string_part_2,strnlen(vrf_alpha_string_part_2,BUFFER_SIZE));
  memcpy(current_round_part_consensus_node_data.vrf_proof,data2,strnlen(data2,BUFFER_SIZE));
  memcpy(current_round_part_consensus_node_data.vrf_beta_string,data3,strnlen(data3,BUFFER_SIZE));

  // sign_data
  if (sign_data(message2,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_2_ERROR("Could not sign_data\nFunction: mainnode_to_node_message_part_1\nReceived Message: MAIN_NODES_TO_NODES_PART_2_OF_ROUND\nSend Message: NODES_TO_NODES_VOTE_RESULTS");
  }

  // send the message to all block verifiers
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
    {
      send_data_socket(current_block_verifiers_list.block_verifiers_IP_address[count],SEND_DATA_PORT,message2,"sending NODES_TO_NODES_VOTE_RESULTS to the block verifiers",0);
    }
  }

  // start the node_to_node_message_timeout
  if (pthread_create(&thread_id, NULL, &node_to_node_message_timeout_thread, (void *)node_to_node_timeout_thread_parameters) != 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_2_ERROR("Could not create the timeout thread\nFunction: mainnode_to_node_message_part_1\nReceived Message: MAIN_NODES_TO_NODES_PART_2_OF_ROUND\nSend Message: NODES_TO_NODES_VOTE_RESULTS");
  }
  // set the thread to dettach once completed, since we do not need to use anything it will return
  if (pthread_detach(thread_id) != 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_2_ERROR("Could not start the timeout thread in detach mode\nFunction: mainnode_to_node_message_part_1\nReceived Message: MAIN_NODES_TO_NODES_PART_2_OF_ROUND\nSend Message: NODES_TO_NODES_VOTE_RESULTS");
  }

  pointer_reset(data);
  return 1;

  #undef pointer_reset_all
  #undef SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_2_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_main_node_to_node_message_part_3
Description: Runs the code when the server receives the MAIN_NODES_TO_NODES_PART_3_OF_ROUND message
Parameters:
  mainnode_timeout_thread_parameters - A mainnode_timeout_thread_parameters struct
    pid_t process_id - Holds the forked process ID that the client is connected to
    int data_received - 1 if the node has received data from the main node, otherwise 0
    char* main_node - The main node (VRF_PUBLIC_AND_SECRET_KEY, VRF_RANDOM_DATA, BLOCK_PRODUCER)
    char* current_round_part - The current round part (1-4).
    char* current_round_part_backup_node - The current main node in the current round part (0-5)
  node_to_node_timeout_thread_parameters - A node_to_node_timeout_thread_parameters struct
    pid_t process_id - Holds the forked process ID that the client is connected to
  current_round_part_consensus_node_data - A current_round_part_consensus_node_data struct
    char* vrf_public_key - Holds the forked process ID that the client is connected to
    char* vrf_alpha_string - 1 if the node has received data from the main node, otherwise 0
    char* vrf_proof - The main node (VRF_PUBLIC_AND_SECRET_KEY, VRF_RANDOM_DATA, BLOCK_PRODUCER)
    char* vrf_beta_string - The current round part (1-4).
    char* block_blob - The current main node in the current round part (0-5)
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_main_node_to_node_message_part_3(struct mainnode_timeout_thread_parameters* mainnode_timeout_thread_parameters, struct node_to_node_timeout_thread_parameters* node_to_node_timeout_thread_parameters, char* message)
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data4 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  int count = 0;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL || data3 == NULL || message == NULL || message2 == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    if (data3 != NULL)
    {
      pointer_reset(data3);
    }
    if (data4 != NULL)
    {
      pointer_reset(data4);
    }
    if (message2 != NULL)
    {
      pointer_reset(message2);
    }
    if (message3 != NULL)
    {
      pointer_reset(message3);
    }
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // threads
  pthread_t thread_id;

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; \
  free(data3); \
  data3 = NULL; \
  free(data4); \
  data4 = NULL; \
  free(message2); \
  message2 = NULL; \
  free(message3); \
  message3 = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_3_ERROR(settings) \
  color_print(settings,"red"); \
  pointer_reset_all; \
  return 0;

  // since the block verifier has received data from the main node, we need to stop the mainnode_timeout_thread
  mainnode_timeout_thread_parameters->data_received = 1;

  // set the next server message since the block verifiers will send the data to each other
  memset(server_message,0,strnlen(server_message,BUFFER_SIZE));
  memcpy(server_message,"NODES_TO_NODES_VOTE_RESULTS",27); 

  // verify the data
  if (verify_data(message,0,1,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_3_ERROR("Could not verify data\nFunction: mainnode_to_node_message_part_1\nReceived Message: MAIN_NODES_TO_NODES_PART_3_OF_ROUND\nSend Message: NODES_TO_NODES_VOTE_RESULTS");
  }

  // parse the message
  if (parse_json_data(message,"vrf_public_key",data) == 0 || parse_json_data(message,"vrf_alpha_string",data2) == 0 || parse_json_data(message,"vrf_proof",data3) == 0 || parse_json_data(message,"vrf_beta_string",data4) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_3_ERROR("Could not parse the data\nFunction: mainnode_to_node_message_part_1\nReceived Message: MAIN_NODES_TO_NODES_PART_3_OF_ROUND\nSend Message: NODES_TO_NODES_VOTE_RESULTS");
  }

  // create the message
  memcpy(message2,"{\r\n \"message_settings\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n \"vote_settings\": \"",75);

  // verify the VRF data and check that the vrf_public_key_part_1 and vrf_alpha_string_part_2 match the current vrf_public_key and vrf_alpha_string
  if (crypto_vrf_verify((unsigned char*)data4,(const unsigned char*)data,(const unsigned char*)data3,(const unsigned char*)data2,(unsigned long long)strnlen(data2,BUFFER_SIZE)) == 0 || memcmp(current_round_part_consensus_node_data.vrf_public_key,data,strnlen(data,BUFFER_SIZE)) != 0 || memcmp(current_round_part_consensus_node_data.vrf_alpha_string,data2,strnlen(data2,BUFFER_SIZE)) != 0)
  {
    memcpy(message2+75,"valid",5);
  }
  else
  {
    memcpy(message2+75,"invalid",7);
  }
  memcpy(message2+strnlen(message2,BUFFER_SIZE),"\",\r\n \"vote_data\": \"",19);

  // SHA2-512 hash all of the VRF data
  memcpy(message3,data,strnlen(data,BUFFER_SIZE));
  memcpy(message3+strnlen(message3,BUFFER_SIZE),data2,strnlen(data2,BUFFER_SIZE));
  memcpy(message3+strnlen(message3,BUFFER_SIZE),data3,strnlen(data3,BUFFER_SIZE));
  memcpy(message3+strnlen(message3,BUFFER_SIZE),data4,strnlen(data4,BUFFER_SIZE));
  crypto_hash_sha512((unsigned char*)current_round_part_vote_data.current_vote_results,(const unsigned char*)message3,(unsigned long long)strnlen(message3,BUFFER_SIZE));
  memcpy(message2+strnlen(message,BUFFER_SIZE),current_round_part_vote_data.current_vote_results,DATA_HASH_LENGTH);
  
  memcpy(message2+strnlen(message2,BUFFER_SIZE),"\",\r\n}",5); 

  // save all of the VRF data to the current_round_part_consensus_node_data struct
  memcpy(current_round_part_consensus_node_data.vrf_public_key,data,strnlen(data,BUFFER_SIZE));
  memcpy(current_round_part_consensus_node_data.vrf_alpha_string,vrf_alpha_string_part_2,strnlen(vrf_alpha_string_part_2,BUFFER_SIZE));
  memcpy(current_round_part_consensus_node_data.vrf_proof,data2,strnlen(data2,BUFFER_SIZE));
  memcpy(current_round_part_consensus_node_data.vrf_beta_string,data3,strnlen(data3,BUFFER_SIZE));

  // sign_data
  if (sign_data(message2,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_3_ERROR("Could not sign_data\nFunction: mainnode_to_node_message_part_1\nReceived Message: MAIN_NODES_TO_NODES_PART_3_OF_ROUND\nSend Message: NODES_TO_NODES_VOTE_RESULTS");
  }

  // send the message to all block verifiers
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
    {
      send_data_socket(current_block_verifiers_list.block_verifiers_IP_address[count],SEND_DATA_PORT,message2,"sending NODES_TO_NODES_VOTE_RESULTS to the block verifiers",0);
    }
  }

  // start the node_to_node_message_timeout
  if (pthread_create(&thread_id, NULL, &node_to_node_message_timeout_thread, (void *)node_to_node_timeout_thread_parameters) != 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_3_ERROR("Could not create the timeout thread\nFunction: mainnode_to_node_message_part_1\nReceived Message: MAIN_NODES_TO_NODES_PART_3_OF_ROUND\nSend Message: NODES_TO_NODES_VOTE_RESULTS");
  }
  // set the thread to dettach once completed, since we do not need to use anything it will return
  if (pthread_detach(thread_id) != 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_3_ERROR("Could not start the timeout thread in detach mode\nFunction: mainnode_to_node_message_part_1\nReceived Message: MAIN_NODES_TO_NODES_PART_3_OF_ROUND\nSend Message: NODES_TO_NODES_VOTE_RESULTS");
  }

  pointer_reset(data);
  return 1;

  #undef pointer_reset_all
  #undef SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_3_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_main_node_to_node_message_part_4
Description: Runs the code when the server receives the MAIN_NODES_TO_NODES_PART_4_OF_ROUND message
Parameters:
  mainnode_timeout_thread_parameters - A mainnode_timeout_thread_parameters struct
    pid_t process_id - Holds the forked process ID that the client is connected to
    int data_received - 1 if the node has received data from the main node, otherwise 0
    char* main_node - The main node (VRF_PUBLIC_AND_SECRET_KEY, VRF_RANDOM_DATA, BLOCK_PRODUCER)
    char* current_round_part - The current round part (1-4).
    char* current_round_part_backup_node - The current main node in the current round part (0-5)
  node_to_node_timeout_thread_parameters - A node_to_node_timeout_thread_parameters struct
    pid_t process_id - Holds the forked process ID that the client is connected to
  current_round_part_consensus_node_data - A current_round_part_consensus_node_data struct
    char* vrf_public_key - Holds the forked process ID that the client is connected to
    char* vrf_alpha_string - 1 if the node has received data from the main node, otherwise 0
    char* vrf_proof - The main node (VRF_PUBLIC_AND_SECRET_KEY, VRF_RANDOM_DATA, BLOCK_PRODUCER)
    char* vrf_beta_string - The current round part (1-4).
    char* block_blob - The current main node in the current round part (0-5)
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_main_node_to_node_message_part_4(struct mainnode_timeout_thread_parameters* mainnode_timeout_thread_parameters, struct node_to_node_timeout_thread_parameters* node_to_node_timeout_thread_parameters, char* message)
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  int count = 0;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL || data3 == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    if (data3 != NULL)
    {
      pointer_reset(data3);
    }
    if (message2 != NULL)
    {
      pointer_reset(message2);
    }
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // threads
  pthread_t thread_id;

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; \
  free(data3); \
  data3 = NULL; \
  free(message2); \
  message2 = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR(settings) \
  color_print(settings,"red"); \
  pointer_reset_all; \
  return 0;

  // since the block verifier has received data from the main node, we need to stop the mainnode_timeout_thread
  mainnode_timeout_thread_parameters->data_received = 1;

  // set the next server message since the block verifiers will send the data to each other
  memset(server_message,0,strnlen(server_message,BUFFER_SIZE));
  memcpy(server_message,"NODES_TO_NODES_VOTE_RESULTS",27); 

  // verify the data
  if (verify_data(message,0,1,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR("Could not verify data\nFunction: mainnode_to_node_message_part_1\nReceived Message: MAIN_NODES_TO_NODES_PART_4_OF_ROUND\nSend Message: NODES_TO_NODES_VOTE_RESULTS");
  }

  // parse the message
  if (parse_json_data(message,"block_blob",data) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR("Could not parse the data\nFunction: mainnode_to_node_message_part_1\nReceived Message: MAIN_NODES_TO_NODES_PART_4_OF_ROUND\nSend Message: NODES_TO_NODES_VOTE_RESULTS");
  }

  // create the message
  memcpy(message2,"{\r\n \"message_settings\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n \"vote_settings\": \"",75);

  // convert the network_block_string to blockchain_data
  if (network_block_string_to_blockchain_data(data,"0") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR("Could not convert the network_block_string to blockchain_data\nFunction: mainnode_to_node_message_part_1\nReceived Message: MAIN_NODES_TO_NODES_PART_4_OF_ROUND\nSend Message: NODES_TO_NODES_VOTE_RESULTS");
  }

  // verify the network block
  if (verify_network_block_data(0,1,1,"0","") == 1)
  {
    memcpy(message2+75,"valid",5);
  }
  else
  {
    memcpy(message2+75,"invalid",7);
  }
  memcpy(message2+strnlen(message2,BUFFER_SIZE),"\",\r\n \"vote_data\": \"",19);

  // SHA2-512 hash the network block
  crypto_hash_sha512((unsigned char*)current_round_part_vote_data.current_vote_results,(const unsigned char*)data,(unsigned long long)strnlen(data,BUFFER_SIZE));
  memcpy(message2+strnlen(message,BUFFER_SIZE),current_round_part_vote_data.current_vote_results,DATA_HASH_LENGTH);
  
  memcpy(message2+strnlen(message2,BUFFER_SIZE),"\",\r\n}",5); 

  // save the netowrk block to the current_round_part_consensus_node_data struct
  memcpy(current_round_part_consensus_node_data.block_blob,data,strnlen(data,BUFFER_SIZE));

  // sign_data
  if (sign_data(message2,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR("Could not sign_data\nFunction: mainnode_to_node_message_part_1\nReceived Message: MAIN_NODES_TO_NODES_PART_4_OF_ROUND\nSend Message: NODES_TO_NODES_VOTE_RESULTS");
  }

  // send the message to all block verifiers
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
    {
      send_data_socket(current_block_verifiers_list.block_verifiers_IP_address[count],SEND_DATA_PORT,message2,"sending NODES_TO_NODES_VOTE_RESULTS to the block verifiers",0);
    }
  }

  // start the node_to_node_message_timeout
  if (pthread_create(&thread_id, NULL, &node_to_node_message_timeout_thread, (void *)node_to_node_timeout_thread_parameters) != 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR("Could not create the timeout thread\nFunction: mainnode_to_node_message_part_1\nReceived Message: MAIN_NODES_TO_NODES_PART_4_OF_ROUND\nSend Message: NODES_TO_NODES_VOTE_RESULTS");
  }
  // set the thread to dettach once completed, since we do not need to use anything it will return
  if (pthread_detach(thread_id) != 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR("Could not start the timeout thread in detach mode\nFunction: mainnode_to_node_message_part_1\nReceived Message: MAIN_NODES_TO_NODES_PART_4_OF_ROUND\nSend Message: NODES_TO_NODES_VOTE_RESULTS");
  }

  pointer_reset(data);
  return 1;

  #undef pointer_reset_all
  #undef SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_node_to_node
Description: Runs the code when the server receives the NODES_TO_NODES_VOTE_RESULTS message
Parameters:
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_node_to_node(char* message)
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3 = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL || data3 == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    if (data3 != NULL)
    {
      pointer_reset(data3);
    }
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; \
  free(data3); \
  data3 = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NODE_ERROR(settings) \
  color_print(settings,"red"); \
  pointer_reset_all; \
  return 0;

  // verify the data
  if (verify_data(message,0,1,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NODE_ERROR("Could not verify data\nFunction: server_receive_data_socket_node_to_node\nReceived Message: NODES_TO_NODES_VOTE_RESULTS");
  }

  // parse the message
  if (parse_json_data(message,"vote_settings",data) == 0 || parse_json_data(message,"vote_data",data2) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NODE_ERROR("Could not parse the data\nFunction: server_receive_data_socket_node_to_node\nReceived Message: NODES_TO_NODES_VOTE_RESULTS");
  }

  // process the vote data
  if (memcmp(data,"valid",5) == 0 && memcmp(data2,current_round_part_vote_data.current_vote_results,DATA_HASH_LENGTH) == 0)
  {
    current_round_part_vote_data.vote_results_valid++;
  }
  else
  {
    current_round_part_vote_data.vote_results_invalid++;
  }

  pointer_reset_all;
  return 1;

  #undef pointer_reset_all
  #undef SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NODE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_consensus_node_to_main_node_message_start_part_of_round
Description: Runs the code when the server receives the CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND message
Parameters:
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_consensus_node_to_main_node_message_start_part_of_round(char* message)
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* vrf_public_key = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* vrf_secret_key = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* vrf_alpha_string = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* vrf_proof = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* vrf_beta_string = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message_copy1;
  int count = 0;
  int counter = 0;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL || data3 == NULL || vrf_public_key == NULL || vrf_secret_key == NULL || vrf_alpha_string == NULL || vrf_proof == NULL || vrf_beta_string == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    if (data3 != NULL)
    {
      pointer_reset(data3);
    }
    if (vrf_public_key != NULL)
    {
      pointer_reset(vrf_public_key);
    }
    if (vrf_secret_key != NULL)
    {
      pointer_reset(vrf_secret_key);
    }
    if (vrf_alpha_string != NULL)
    {
      pointer_reset(vrf_alpha_string);
    }
    if (vrf_proof != NULL)
    {
      pointer_reset(vrf_proof);
    }
    if (vrf_beta_string != NULL)
    {
      pointer_reset(vrf_beta_string);
    }
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // define macros
  #define MINIMUM 1
  #define MAXIMUM 255
  
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; \
  free(data3); \
  data3 = NULL; \
  free(vrf_public_key); \
  vrf_public_key = NULL; \
  free(vrf_secret_key); \
  vrf_secret_key = NULL; \
  free(vrf_alpha_string); \
  vrf_alpha_string = NULL; \
  free(vrf_proof); \
  vrf_proof = NULL; \
  free(vrf_beta_string); \
  vrf_beta_string = NULL; \

  #define SERVER_RECEIVE_DATA_SOCKET_CONSENSUS_NODE_TO_MAIN_NODE_MESSAGE_START_PART_OF_ROUND(settings) \
  color_print(settings,"red"); \
  pointer_reset_all; \
  return 0;

  // verify the data
  if (verify_data(message,0,0,0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_CONSENSUS_NODE_TO_MAIN_NODE_MESSAGE_START_PART_OF_ROUND("Could not verify data\nFunction: server_receive_data_socket_consensus_node_to_main_node_message_start_part_of_round\nReceived Message: CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND\nSend Message: MAIN_NODES_TO_NODES_PART_1_OF_ROUND|MAIN_NODES_TO_NODES_PART_2_OF_ROUND|MAIN_NODES_TO_NODES_PART_3_OF_ROUND|MAIN_NODES_TO_NODES_PART_4_OF_ROUND");
  }

  // parse the message
  memset(current_round_part,0,strnlen(current_round_part,BUFFER_SIZE));
  memset(current_round_part_backup_node,0,strnlen(current_round_part_backup_node,BUFFER_SIZE));
  if (parse_json_data(message,"message",current_round_part) == 0 || parse_json_data(message,"VRF_block_blob",data2) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_CONSENSUS_NODE_TO_MAIN_NODE_MESSAGE_START_PART_OF_ROUND("Could not parse main_nodes_public_address\nFunction: server_receive_data_socket_consensus_node_to_main_node_message_start_part_of_round\nReceived Message: CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND\nSend Message: MAIN_NODES_TO_NODES_PART_1_OF_ROUND|MAIN_NODES_TO_NODES_PART_2_OF_ROUND|MAIN_NODES_TO_NODES_PART_3_OF_ROUND|MAIN_NODES_TO_NODES_PART_4_OF_ROUND");
  }

  // create the VRF data for this part of the round
  if (memcmp(current_round_part,"1",1) == 0)
  {
    // The VRF private and secret key will create the public and secret key
    if (create_random_VRF_keys((unsigned char*)vrf_public_key,(unsigned char*)vrf_secret_key) == 0 || crypto_vrf_is_valid_key((const unsigned char*)vrf_public_key) == 0)
    {
      SERVER_RECEIVE_DATA_SOCKET_CONSENSUS_NODE_TO_MAIN_NODE_MESSAGE_START_PART_OF_ROUND("Could not create the VRF public and secret key\nFunction: server_receive_data_socket_consensus_node_to_main_node_message_start_part_of_round\nReceived Message: CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND\nSend Message: MAIN_NODES_TO_NODES_PART_1_OF_ROUND|MAIN_NODES_TO_NODES_PART_2_OF_ROUND|MAIN_NODES_TO_NODES_PART_3_OF_ROUND|MAIN_NODES_TO_NODES_PART_4_OF_ROUND");
    }

    // create the alpha string
    memcpy(vrf_alpha_string,vrf_public_key,strnlen(vrf_public_key,BUFFER_SIZE));

    // create the proof
    if (crypto_vrf_prove((unsigned char*)vrf_proof,(const unsigned char*)vrf_secret_key,(const unsigned char*)vrf_alpha_string,(unsigned long long)strlen(vrf_alpha_string)) == 1)
    {
      SERVER_RECEIVE_DATA_SOCKET_CONSENSUS_NODE_TO_MAIN_NODE_MESSAGE_START_PART_OF_ROUND("Could not create the VRF proof\nFunction: server_receive_data_socket_consensus_node_to_main_node_message_start_part_of_round\nReceived Message: CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND\nSend Message: MAIN_NODES_TO_NODES_PART_1_OF_ROUND|MAIN_NODES_TO_NODES_PART_2_OF_ROUND|MAIN_NODES_TO_NODES_PART_3_OF_ROUND|MAIN_NODES_TO_NODES_PART_4_OF_ROUND");
    }

    // create the beta string
    if (crypto_vrf_proof_to_hash((unsigned char*)vrf_beta_string,(const unsigned char*)vrf_proof) == 1)
    {
      SERVER_RECEIVE_DATA_SOCKET_CONSENSUS_NODE_TO_MAIN_NODE_MESSAGE_START_PART_OF_ROUND("Could not create the VRF beta string, or verify the VRF proof\nFunction: server_receive_data_socket_consensus_node_to_main_node_message_start_part_of_round\nReceived Message: CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND\nSend Message: MAIN_NODES_TO_NODES_PART_1_OF_ROUND|MAIN_NODES_TO_NODES_PART_2_OF_ROUND|MAIN_NODES_TO_NODES_PART_3_OF_ROUND|MAIN_NODES_TO_NODES_PART_4_OF_ROUND");
    }

    // save the VRF public key and secret key
    memset(vrf_public_key_part_1,0,strlen(vrf_public_key_part_1));
    memset(vrf_secret_key_part_1,0,strlen(vrf_secret_key_part_1));
    memcpy(vrf_public_key_part_1,vrf_public_key,strnlen(vrf_public_key,BUFFER_SIZE));
    memcpy(vrf_secret_key_part_1,vrf_secret_key,strnlen(vrf_secret_key,BUFFER_SIZE));

    // create the message
    memcpy(data3,"{\r\n \"message_settings\": \"MAIN_NODES_TO_NODES_PART_1_OF_ROUND\",\r\n \"vrf_public_key\": \"",84);
    memcpy(data3,vrf_public_key,strnlen(vrf_public_key,BUFFER_SIZE));
    memcpy(data3,"\",\r\n \"vrf_alpha_string\": \"",26);
    memcpy(data3,vrf_alpha_string,strnlen(vrf_alpha_string,BUFFER_SIZE));
    memcpy(data3,"\",\r\n \"vrf_proof\": \"",26);
    memcpy(data3,vrf_proof,strnlen(vrf_proof,BUFFER_SIZE));
    memcpy(data3,"\",\r\n \"vrf_beta_string\": \"",26);
    memcpy(data3,vrf_beta_string,strnlen(vrf_beta_string,BUFFER_SIZE));
    memcpy(data3,"\",\r\n}",5);
  }
  else if (memcmp(current_round_part,"2",1) == 0)
  {
    // The VRF private and secret key will create the public and secret key
    if (create_random_VRF_keys((unsigned char*)vrf_public_key,(unsigned char*)vrf_secret_key) == 0 || crypto_vrf_is_valid_key((const unsigned char*)vrf_public_key) == 0)
    {
      SERVER_RECEIVE_DATA_SOCKET_CONSENSUS_NODE_TO_MAIN_NODE_MESSAGE_START_PART_OF_ROUND("Could not create the VRF public and secret key\nFunction: server_receive_data_socket_consensus_node_to_main_node_message_start_part_of_round\nReceived Message: CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND\nSend Message: MAIN_NODES_TO_NODES_PART_1_OF_ROUND|MAIN_NODES_TO_NODES_PART_2_OF_ROUND|MAIN_NODES_TO_NODES_PART_3_OF_ROUND|MAIN_NODES_TO_NODES_PART_4_OF_ROUND");
    }

    // create the alpha string
    memset(data,0,strnlen(data,BUFFER_SIZE));
    if (get_previous_block_hash(data,0) == 0)
    {
      SERVER_RECEIVE_DATA_SOCKET_CONSENSUS_NODE_TO_MAIN_NODE_MESSAGE_START_PART_OF_ROUND("Could not get the previous block hash\nFunction: server_receive_data_socket_consensus_node_to_main_node_message_start_part_of_round\nReceived Message: CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND\nSend Message: MAIN_NODES_TO_NODES_PART_1_OF_ROUND|MAIN_NODES_TO_NODES_PART_2_OF_ROUND|MAIN_NODES_TO_NODES_PART_3_OF_ROUND|MAIN_NODES_TO_NODES_PART_4_OF_ROUND");
    }
    memcpy(vrf_alpha_string,data,strnlen(data,BUFFER_SIZE));
    // create the 100 character random string
    for (count = 0, counter = 0; count < 50; count++, counter +=2)
    {
      sprintf(vrf_alpha_string+counter,"%02x",((rand() % (MAXIMUM - MINIMUM + 1)) + MINIMUM) & 0xFF);
    }
    memset(vrf_alpha_string_part_2,0,strlen(vrf_alpha_string_part_2));
    memcpy(vrf_alpha_string_part_2,vrf_alpha_string,strnlen(vrf_alpha_string,BUFFER_SIZE));

    // create the proof
    if (crypto_vrf_prove((unsigned char*)vrf_proof,(const unsigned char*)vrf_secret_key,(const unsigned char*)vrf_alpha_string,(unsigned long long)strlen(vrf_alpha_string)) == 1)
    {
      SERVER_RECEIVE_DATA_SOCKET_CONSENSUS_NODE_TO_MAIN_NODE_MESSAGE_START_PART_OF_ROUND("Could not create the VRF proof\nFunction: server_receive_data_socket_consensus_node_to_main_node_message_start_part_of_round\nReceived Message: CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND\nSend Message: MAIN_NODES_TO_NODES_PART_1_OF_ROUND|MAIN_NODES_TO_NODES_PART_2_OF_ROUND|MAIN_NODES_TO_NODES_PART_3_OF_ROUND|MAIN_NODES_TO_NODES_PART_4_OF_ROUND");
    }

    // create the beta string
    if (crypto_vrf_proof_to_hash((unsigned char*)vrf_beta_string,(const unsigned char*)vrf_proof) == 1)
    {
      SERVER_RECEIVE_DATA_SOCKET_CONSENSUS_NODE_TO_MAIN_NODE_MESSAGE_START_PART_OF_ROUND("Could not create the VRF beta string, or verify the VRF proof\nFunction: server_receive_data_socket_consensus_node_to_main_node_message_start_part_of_round\nReceived Message: CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND\nSend Message: MAIN_NODES_TO_NODES_PART_1_OF_ROUND|MAIN_NODES_TO_NODES_PART_2_OF_ROUND|MAIN_NODES_TO_NODES_PART_3_OF_ROUND|MAIN_NODES_TO_NODES_PART_4_OF_ROUND");
    }

    // create the message
    memcpy(data3,"{\r\n \"message_settings\": \"MAIN_NODES_TO_NODES_PART_2_OF_ROUND\",\r\n \"vrf_public_key\": \"",84);
    memcpy(data3,vrf_public_key,strnlen(vrf_public_key,BUFFER_SIZE));
    memcpy(data3,"\",\r\n \"vrf_alpha_string\": \"",26);
    memcpy(data3,vrf_alpha_string,strnlen(vrf_alpha_string,BUFFER_SIZE));
    memcpy(data3,"\",\r\n \"vrf_proof\": \"",26);
    memcpy(data3,vrf_proof,strnlen(vrf_proof,BUFFER_SIZE));
    memcpy(data3,"\",\r\n \"vrf_beta_string\": \"",26);
    memcpy(data3,vrf_beta_string,strnlen(vrf_beta_string,BUFFER_SIZE));
    memcpy(data3,"\",\r\n}",5);
  }
  else if (memcmp(current_round_part,"3",1) == 0)
  {
    // create the proof
    if (crypto_vrf_prove((unsigned char*)vrf_proof,(const unsigned char*)vrf_secret_key_part_1,(const unsigned char*)vrf_alpha_string_part_2,(unsigned long long)strlen(vrf_alpha_string_part_2)) == 1)
    {
      SERVER_RECEIVE_DATA_SOCKET_CONSENSUS_NODE_TO_MAIN_NODE_MESSAGE_START_PART_OF_ROUND("Could not create the VRF proof\nFunction: server_receive_data_socket_consensus_node_to_main_node_message_start_part_of_round\nReceived Message: CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND\nSend Message: MAIN_NODES_TO_NODES_PART_1_OF_ROUND|MAIN_NODES_TO_NODES_PART_2_OF_ROUND|MAIN_NODES_TO_NODES_PART_3_OF_ROUND|MAIN_NODES_TO_NODES_PART_4_OF_ROUND");
    }

    // create the beta string
    if (crypto_vrf_proof_to_hash((unsigned char*)vrf_beta_string,(const unsigned char*)vrf_proof) == 1)
    {
      SERVER_RECEIVE_DATA_SOCKET_CONSENSUS_NODE_TO_MAIN_NODE_MESSAGE_START_PART_OF_ROUND("Could not create the VRF beta string, or verify the VRF proof\nFunction: server_receive_data_socket_consensus_node_to_main_node_message_start_part_of_round\nReceived Message: CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND\nSend Message: MAIN_NODES_TO_NODES_PART_1_OF_ROUND|MAIN_NODES_TO_NODES_PART_2_OF_ROUND|MAIN_NODES_TO_NODES_PART_3_OF_ROUND|MAIN_NODES_TO_NODES_PART_4_OF_ROUND");
    }

    // create the message
    memcpy(data3,"{\r\n \"message_settings\": \"MAIN_NODES_TO_NODES_PART_3_OF_ROUND\",\r\n \"vrf_public_key\": \"",84);
    memcpy(data3,vrf_public_key_part_1,strnlen(vrf_public_key_part_1,BUFFER_SIZE));
    memcpy(data3,"\",\r\n \"vrf_alpha_string\": \"",26);
    memcpy(data3,vrf_alpha_string_part_2,strnlen(vrf_alpha_string_part_2,BUFFER_SIZE));
    memcpy(data3,"\",\r\n \"vrf_proof\": \"",26);
    memcpy(data3,vrf_proof,strnlen(vrf_proof,BUFFER_SIZE));
    memcpy(data3,"\",\r\n \"vrf_beta_string\": \"",26);
    memcpy(data3,vrf_beta_string,strnlen(vrf_beta_string,BUFFER_SIZE));
    memcpy(data3,"\",\r\n}",5);
  }
  else if (memcmp(current_round_part,"4",1) == 0)
  {
    // get the block template
    memset(data,0,strnlen(data,BUFFER_SIZE));
    if (get_block_template(data,0) == 0)
    {
      SERVER_RECEIVE_DATA_SOCKET_CONSENSUS_NODE_TO_MAIN_NODE_MESSAGE_START_PART_OF_ROUND("Could not get the block template\nFunction: server_receive_data_socket_consensus_node_to_main_node_message_start_part_of_round\nReceived Message: CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND\nSend Message: MAIN_NODES_TO_NODES_PART_1_OF_ROUND|MAIN_NODES_TO_NODES_PART_2_OF_ROUND|MAIN_NODES_TO_NODES_PART_3_OF_ROUND|MAIN_NODES_TO_NODES_PART_4_OF_ROUND");
    }

    // replace the reserve bytes with the received reserve bytes
    message_copy1 = strstr(data,RESERVE_BYTE_START_STRING);
    count = strnlen(data,BUFFER_SIZE) - strnlen(message_copy1,BUFFER_SIZE);
    memcpy(data+count,data2,strnlen(data2,BUFFER_SIZE));

    // create the message
    memcpy(data3,"{\r\n \"message_settings\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND\",\r\n \"block_blob\": \"",80);
    memcpy(data3,data,strnlen(data,BUFFER_SIZE));
    memcpy(data3,"\",\r\n}",5);
  }

  // sign_data
  if (sign_data(data3,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_CONSENSUS_NODE_TO_MAIN_NODE_MESSAGE_START_PART_OF_ROUND("Could not sign_data\nFunction: server_receive_data_socket_consensus_node_to_main_node_message_start_part_of_round\nReceived Message: CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND\nSend Message: MAIN_NODES_TO_NODES_PART_1_OF_ROUND|MAIN_NODES_TO_NODES_PART_2_OF_ROUND|MAIN_NODES_TO_NODES_PART_3_OF_ROUND|MAIN_NODES_TO_NODES_PART_4_OF_ROUND");
  }

  // send the message to all block verifiers
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
    {
      send_data_socket(current_block_verifiers_list.block_verifiers_IP_address[count],SEND_DATA_PORT,data,"",0);
    }
  }

  // set the next server message since the block verifiers will send the data to each other
  memset(server_message,0,strnlen(server_message,BUFFER_SIZE));
  memcpy(server_message,"CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS|CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND",96);

  pointer_reset(data);
  return 1;

  #undef pointer_reset_all
  #undef SERVER_RECEIVE_DATA_SOCKET_CONSENSUS_NODE_TO_MAIN_NODE_MESSAGE_START_PART_OF_ROUND
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_consensus_node_to_node_and_main_node_restart
Description: Runs the code when the server receives the CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEW_PART_OF_ROUND, CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEXT_ROUND, CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_ROUND_CHANGE, CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CREATE_NEW_BLOCK_MESSAGE or CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_RECALCULATING_VOTES message
Parameters:
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_consensus_node_to_node_and_main_node_restart(char* message)
{
  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_CONSENSUS_NODE_TO_NODE_AND_MAIN_NODE_MESSAGE_RESTART(settings) \
  color_print(settings,"red"); \
  return 0;

  // verify the data
  if (verify_data(message,0,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_CONSENSUS_NODE_TO_NODE_AND_MAIN_NODE_MESSAGE_RESTART("Could not verify data\nFunction: server_receive_data_socket_consensus_node_to_node_and_main_node_restart");
  }

  // set the current_round_part, current_round_part_backup_node and server message, this way the node will start at the begining of a round
  memset(current_round_part,0,strnlen(current_round_part,BUFFER_SIZE));
  memset(current_round_part_backup_node,0,strnlen(current_round_part_backup_node,BUFFER_SIZE));
  memcpy(current_round_part,"1",1);
  memcpy(current_round_part_backup_node,"0",1);
  memset(server_message,0,strnlen(server_message,BUFFER_SIZE));
  memcpy(server_message,"CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS|CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND",96);
  
  return 1;

  #undef SERVER_RECEIVE_DATA_SOCKET_CONSENSUS_NODE_TO_NODE_AND_MAIN_NODE_MESSAGE_RESTART
}



/*
-----------------------------------------------------------------------------------------------------------
Name: create_server
Description: Creates the server
Parameters:
   MESSAGE_SETTINGS - 1 to print the messages, otherwise 0. This is used for the testing flag to not print any success or error messages
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int create_server(const int MESSAGE_SETTINGS)
{
  // Constants
  const int SOCKET_OPTION = 1; 

  // Variables
  char buffer[BUFFER_SIZE];
  char buffer2[BUFFER_SIZE];
  char client_address[BUFFER_SIZE];  
  char* string = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  int len;
  int receive_data_result; 
  time_t current_date_and_time;
  struct tm* current_UTC_date_and_time; 
  struct sockaddr_in addr, cl_addr; 
  struct mainnode_timeout_thread_parameters mainnode_timeout_thread_parameters;
  struct node_to_node_timeout_thread_parameters node_to_node_timeout_thread_parameters;

  // define macros
  #define SOCKET_FILE_DESCRIPTORS_LENGTH 1

  #define pointer_reset_all \
  free(string); \
  string = NULL;

  /* Reset the node so it is ready for the next round.
  close the client socket
  reset the variables for the forked process
  reset the current_round_part to 1 and current_round_part_backup_node to 0
  reset the server_message to CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND|CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS
  exit the forked process
  this way the node will sit out the current round, and start the next round.
  */
  #define SERVER_ERROR(settings) \
  close(SOCKET); \
  pointer_reset_all; \
  memset(current_round_part,0,strnlen(current_round_part,BUFFER_SIZE)); \
  memset(current_round_part_backup_node,0,strnlen(current_round_part_backup_node,BUFFER_SIZE)); \
  memcpy(current_round_part,"1",1); \
  memcpy(current_round_part_backup_node,"0",1); \
  memset(server_message,0,strnlen(server_message,BUFFER_SIZE)); \
  memcpy(server_message,"CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND|CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS",96); \
  if (settings == 0) \
  { \
    return 0; \
  } \
  else \
  { \
    _exit(0); \
  }  
  
  // threads
  pthread_t thread_id;

  // set the main process to ignore if forked processes return a value or not, since the timeout for the total connection time is run on a different thread
  signal(SIGCHLD, SIG_IGN);  

  // check if the memory needed was allocated on the heap successfully
  if (string == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }
    
  /* Create the socket  
  AF_INET = IPV4 support
  SOCK_STREAM = TCP protocol
  */
  const int SOCKET = socket(AF_INET, SOCK_STREAM, 0);
  if (SOCKET == -1)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      color_print("Error creating socket","red");
    }    
    pointer_reset_all;
    return 0;
  }

  /* Set the socket options
  SOL_SOCKET = socket level
  SO_REUSEADDR = allows for reuse of the same address, so one can shutdown and restart the program without errros
  SO_REUSEPORT = allows for reuse of the same port, so one can shutdown and restart the program without errros
  */
  if (setsockopt(SOCKET, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &SOCKET_OPTION,sizeof(int)) != 0)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      color_print("Error setting socket options","red"); 
    }
    SERVER_ERROR(0);
  } 
  if (MESSAGE_SETTINGS == 1)
  {
    color_print("Socket created","green");
  }
 
  // convert the port to a string
  sprintf(buffer2,"%d",SEND_DATA_PORT);  
 
  memset(&addr, 0, sizeof(addr));
  /* setup the connection
  AF_INET = IPV4
  INADDR_ANY = connect to 0.0.0.0
  use htons to convert the port from host byte order to network byte order short
  */
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(SEND_DATA_PORT);
 
  // connect to 0.0.0.0
  if (bind(SOCKET, (struct sockaddr *) &addr, sizeof(addr)) != 0)
  {
   if (MESSAGE_SETTINGS == 1)
   {
     memset(string,0,strnlen(string,BUFFER_SIZE));
     memcpy(string,"Error connecting to port ",25);
     memcpy(string+25,buffer2,strnlen(buffer2,BUFFER_SIZE));
     color_print(string,"red"); 
   }
   SERVER_ERROR(0);
  } 
  if (MESSAGE_SETTINGS == 1)
  {
    memset(string,0,strnlen(string,BUFFER_SIZE));
    memcpy(string,"Connected to port ",18);
    memcpy(string+18,buffer2,strnlen(buffer2,BUFFER_SIZE));
    color_print(string,"green");

    printf("Waiting for a connection...\n");
  }

  // set the maximum simultaneous connections
  if (listen(SOCKET, MAXIMUM_CONNECTIONS) != 0)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      color_print("Error creating the server","red"); 
    }
    SERVER_ERROR(0);
  }

  for (;;)
  {
    len = sizeof(cl_addr);
    const int CLIENT_SOCKET = accept(SOCKET, (struct sockaddr *) &cl_addr, (socklen_t*)&len);
    inet_ntop(AF_INET, &(cl_addr.sin_addr), client_address, BUFFER_SIZE);
    if (client_address == NULL)
    {
      if (MESSAGE_SETTINGS == 1)
      {
        color_print("Error accepting connection","red"); 
      }
      continue;
    }
    const size_t CLIENT_ADDRESS_LENGTH = strnlen(client_address,BUFFER_SIZE);
    const size_t BUFFER2_LENGTH = strnlen(buffer2,BUFFER_SIZE);
  
    if (CLIENT_SOCKET == -1)
    {
      if (MESSAGE_SETTINGS == 1)
      {
        memset(string,0,strnlen(string,BUFFER_SIZE));
        memcpy(string,"Error accepting connection from ",32);
        memcpy(string+32,inet_ntoa(cl_addr.sin_addr),CLIENT_ADDRESS_LENGTH);
        memcpy(string+32+CLIENT_ADDRESS_LENGTH," on ",4);
        memcpy(string+36+CLIENT_ADDRESS_LENGTH,buffer2,strnlen(buffer2,BUFFER_SIZE));
        color_print(string,"red"); 
      }
      continue;
    }
    if (MESSAGE_SETTINGS == 1)
    {
      memset(string,0,strnlen(string,BUFFER_SIZE));
      memcpy(string,"Connection accepted from ",25);
      memcpy(string+25,inet_ntoa(cl_addr.sin_addr),CLIENT_ADDRESS_LENGTH);
      memcpy(string+25+CLIENT_ADDRESS_LENGTH," on ",4);
      memcpy(string+29+CLIENT_ADDRESS_LENGTH,buffer2,strnlen(buffer2,BUFFER_SIZE));
      color_print(string,"green"); 
    }

   

    if (fork() == 0)
    {     
      // create a struct for the parameters
      struct total_connection_time_thread_parameters parameters = {
        getpid(),
        client_address,
        buffer2,
        0,
        (int)MESSAGE_SETTINGS
      };
          // create a timeout for this connection, since we need to limit the amount of time a client has to send data from once it connected
         if (pthread_create(&thread_id, NULL, &total_connection_time_thread, (void *)&parameters) != 0)
         {
           // close the forked process
           SERVER_ERROR(1);
         }
         // set the thread to dettach once completed, since we do not need to use anything it will return.
         if (pthread_detach(thread_id) != 0)
         {
           // close the forked process
           SERVER_ERROR(1);
         }
      

       // close the main socket, since the socket is now copied to the forked process
       close(SOCKET); 

       for (;;)
       {         
         // receive the data
         memset(buffer, 0, BUFFER_SIZE); 
         receive_data_result = receive_data(CLIENT_SOCKET,buffer,SOCKET_END_STRING,0,TOTAL_CONNECTION_TIME_SETTINGS);
         if (receive_data_result < 2)
         {
           if (MESSAGE_SETTINGS == 1)
           {
             memset(string,0,strnlen(string,BUFFER_SIZE));
             memcpy(string,"Error receiving data from ",26);
             memcpy(string+26,client_address,CLIENT_ADDRESS_LENGTH);
             memcpy(string+26+CLIENT_ADDRESS_LENGTH," on port ",9);
             memcpy(string+35+CLIENT_ADDRESS_LENGTH,buffer2,BUFFER2_LENGTH);
             if (receive_data_result == 1)
             {
               memcpy(string+35+CLIENT_ADDRESS_LENGTH+BUFFER2_LENGTH,", because of a timeout issue",28);
             }
             else if (receive_data_result == 0)
             {
               memcpy(string+35+CLIENT_ADDRESS_LENGTH+BUFFER2_LENGTH,", because of a potential buffer overflow issue",46);
             }
             color_print(string,"red"); 
           }
           // close the forked process, since the client had an error sending data       
           SERVER_ERROR(1);
         }
         else if (receive_data_result == 2)
         {
          // update the parameters, since we have received data from the client
          parameters.data_received = 1;
         }   

         // get the current time
         time(&current_date_and_time);
         current_UTC_date_and_time = gmtime(&current_date_and_time);

         // check if a certain type of message has been received         
         if (strstr(buffer,"\"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\"") != NULL && strncmp(server_message,"XCASH_PROOF_OF_STAKE_TEST_DATA",BUFFER_SIZE) == 0)
         {
           server_received_data_xcash_proof_of_stake_test_data(CLIENT_SOCKET,buffer);
           close(SOCKET);
           pointer_reset_all;
         }
         else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS\"") != NULL && current_UTC_date_and_time->tm_min % 4 == 0 && current_UTC_date_and_time->tm_sec < 5)
         {
           server_receive_data_socket_block_verifiers_to_block_verifiers_invalid_reserve_proofs((const char*)buffer);
           close(SOCKET);
           pointer_reset_all; 
         } 
         else if (strstr(buffer,"\"message_settings\": \"MAIN_NODES_TO_NODES_PART_1_OF_ROUND\"") != NULL && strstr(server_message,"MAIN_NODES_TO_NODES_PART_1_OF_ROUND") != NULL)
         {
           // only close the forked process on the timeout in the node_to_node_timeout_thread
           // create a node_to_node_timeout_thread_parameters struct since this function will use the node_to_node_timeout_thread
           node_to_node_timeout_thread_parameters.process_id = getpid();
           if (server_receive_data_socket_main_node_to_node_message_part_1(&mainnode_timeout_thread_parameters,&node_to_node_timeout_thread_parameters,buffer) == 0)
           {
             SERVER_ERROR(1);
           }
         } 
         else if (strstr(buffer,"\"message_settings\": \"MAIN_NODES_TO_NODES_PART_2_OF_ROUND\"") != NULL && strstr(server_message,"MAIN_NODES_TO_NODES_PART_2_OF_ROUND") != NULL)
         {
           // only close the forked process on the timeout in the node_to_node_timeout_thread
           // create a node_to_node_timeout_thread_parameters struct since this function will use the node_to_node_timeout_thread
           node_to_node_timeout_thread_parameters.process_id = getpid();
           if (server_receive_data_socket_main_node_to_node_message_part_2(&mainnode_timeout_thread_parameters,&node_to_node_timeout_thread_parameters,buffer) == 0)
           {
             SERVER_ERROR(1);
           }
         } 
         else if (strstr(buffer,"\"message_settings\": \"MAIN_NODES_TO_NODES_PART_3_OF_ROUND\"") != NULL && strstr(server_message,"MAIN_NODES_TO_NODES_PART_3_OF_ROUND") != NULL)
         {
           // only close the forked process on the timeout in the node_to_node_timeout_thread
           // create a node_to_node_timeout_thread_parameters struct since this function will use the node_to_node_timeout_thread
           node_to_node_timeout_thread_parameters.process_id = getpid();
           if (server_receive_data_socket_main_node_to_node_message_part_3(&mainnode_timeout_thread_parameters,&node_to_node_timeout_thread_parameters,buffer) == 0)
           {
             SERVER_ERROR(1);
           }
         } 
         else if (strstr(buffer,"\"message_settings\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND\"") != NULL && strstr(server_message,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND") != NULL)
         {
           // only close the forked process on the timeout in the node_to_node_timeout_thread
           // create a node_to_node_timeout_thread_parameters struct since this function will use the node_to_node_timeout_thread
           node_to_node_timeout_thread_parameters.process_id = getpid();
           if (server_receive_data_socket_main_node_to_node_message_part_4(&mainnode_timeout_thread_parameters,&node_to_node_timeout_thread_parameters,buffer) == 0)
           {
             SERVER_ERROR(1);
           }
         } 
         if (strstr(buffer,"\"message_settings\": \"NODES_TO_NODES_VOTE_RESULTS\"") != NULL && strstr(server_message,"NODES_TO_NODES_VOTE_RESULTS") != NULL)
         {
           // close the forked process when done
           server_receive_data_socket_node_to_node(buffer);
           SERVER_ERROR(1);
         }
         else if (strstr(buffer,"\"message_settings\": \"CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND\"") != NULL && strstr(server_message,"CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND") != NULL)
         {
           // close the forked process when done
           server_receive_data_socket_consensus_node_to_main_node_message_start_part_of_round(buffer);
           SERVER_ERROR(1);
         } 
         else if (strstr(buffer,"\"message_settings\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEW_PART_OF_ROUND\"") != NULL || strstr(buffer,"\"message_settings\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEXT_ROUND\"") != NULL || strstr(buffer,"\"message_settings\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_ROUND_CHANGE\"") != NULL || strstr(buffer,"\"message_settings\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CREATE_NEW_BLOCK_MESSAGE\"") != NULL || strstr(buffer,"\"message_settings\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_RECALCULATING_VOTES\"") != NULL)
         {
           if (server_receive_data_socket_consensus_node_to_node_and_main_node_restart(buffer) == 0)
           {
             SERVER_ERROR(1);
           }
           // close the server
           break;           
         }
         else
         {
           printf("Received %s from %s on port %s\r\n",buffer,client_address,buffer2);
           // send the message 
           if (send_data(CLIENT_SOCKET,buffer,1) == 1)
           {
             printf("Sent %s to %s on port %s\r\n",buffer,client_address,buffer2);
           } 
           else
           {
             memset(string,0,strnlen(string,BUFFER_SIZE));
             memcpy(string,"Error sending data to ",22);
             memcpy(string+22,client_address,CLIENT_ADDRESS_LENGTH);
             memcpy(string+22+CLIENT_ADDRESS_LENGTH," on port ",9);
             memcpy(string+31+CLIENT_ADDRESS_LENGTH,buffer2,BUFFER2_LENGTH);
             color_print(string,"red"); 
             continue;
           } 
         }

         
       
       }
     }   
     else
     {
       // if the process did not fork, close the client socket
       close(CLIENT_SOCKET);
     } 
   }
   return 1;

   #undef pointer_reset_all
   #undef SERVER_ERROR
}

