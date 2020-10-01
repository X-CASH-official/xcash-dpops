#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h> 
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "define_macros_test.h"
#include "structures.h"
#include "variables.h"

#include "blockchain_functions.h"
#include "block_verifiers_functions.h"
#include "block_verifiers_synchronize_check_functions.h"
#include "block_verifiers_synchronize_server_functions.h"
#include "block_verifiers_synchronize_functions.h"
#include "block_verifiers_thread_server_functions.h"
#include "block_verifiers_update_functions.h"
#include "count_database_functions.h"
#include "insert_database_functions.h"
#include "read_database_functions.h"
#include "update_database_functions.h"
#include "delete_database_functions.h"
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



int start_new_round(void)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  size_t count;
  int count2;

   // threads
  pthread_t thread_id;

  // define macros
  #define START_NEW_ROUND_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"start_new_round",15); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  #define RESET_VARIABLES \
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++) \
  { \
    memset(VRF_data.block_verifiers_vrf_secret_key_data[count],0,strlen(VRF_data.block_verifiers_vrf_secret_key_data[count])); \
    memset(VRF_data.block_verifiers_vrf_secret_key[count],0,strlen((const char*)VRF_data.block_verifiers_vrf_secret_key[count])); \
    memset(VRF_data.block_verifiers_vrf_public_key_data[count],0,strlen(VRF_data.block_verifiers_vrf_public_key_data[count])); \
    memset(VRF_data.block_verifiers_vrf_public_key[count],0,strlen((const char*)VRF_data.block_verifiers_vrf_public_key[count])); \
    memset(VRF_data.block_verifiers_random_data[count],0,strlen(VRF_data.block_verifiers_random_data[count])); \
    memset(VRF_data.block_blob_signature[count],0,strlen(VRF_data.block_blob_signature[count])); \
  } \
  memset(VRF_data.vrf_secret_key_data,0,strlen(VRF_data.vrf_secret_key_data)); \
  memset(VRF_data.vrf_secret_key,0,strlen((const char*)VRF_data.vrf_secret_key)); \
  memset(VRF_data.vrf_public_key_data,0,strlen(VRF_data.vrf_public_key_data)); \
  memset(VRF_data.vrf_public_key,0,strlen((const char*)VRF_data.vrf_public_key)); \
  memset(VRF_data.vrf_alpha_string_data,0,strlen(VRF_data.vrf_alpha_string_data)); \
  memset(VRF_data.vrf_alpha_string,0,strlen((const char*)VRF_data.vrf_alpha_string)); \
  memset(VRF_data.vrf_proof_data,0,strlen(VRF_data.vrf_proof_data)); \
  memset(VRF_data.vrf_proof,0,strlen((const char*)VRF_data.vrf_proof)); \
  memset(VRF_data.vrf_beta_string_data,0,strlen(VRF_data.vrf_beta_string_data)); \
  memset(VRF_data.vrf_beta_string,0,strlen((const char*)VRF_data.vrf_beta_string)); \
  memset(VRF_data.reserve_bytes_data_hash,0,strlen(VRF_data.reserve_bytes_data_hash)); \
  memset(VRF_data.block_blob,0,strlen(VRF_data.block_blob));
  

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // start a new round
  if (get_current_block_height(current_block_height) == 0)
  {
    START_NEW_ROUND_ERROR("Could not get the current block height");
  }
  
  // check if it is running in registration mode only
  if (registration_settings != 1)
  {
    memcpy(data,"A new round is starting for block ",34);
    memcpy(data+34,current_block_height,strnlen(current_block_height,BUFFER_SIZE));
    print_start_message(current_date_and_time,current_UTC_date_and_time,data,data2); 
  }

  // get the current block height
  sscanf(current_block_height,"%zu", &count);

  color_print("getting the delegates online status","blue");

  // get the delegates online status
  if (registration_settings != 1)
  {
    get_delegates_online_status();
  }

  // save a copy of the database
  if (production_settings == 1 && network_data_node_settings == 1)
  {
    count2 = system(database_path_write);
  }
  

  // wait so everyone has got the online status
  sync_block_verifiers_minutes_and_seconds(0,15);

  // reload the initial previous, current and next block verifiers list if its the first block or if you had to restart
  if (count == XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT || sync_previous_current_next_block_verifiers_settings == 1)
  {
    sync_previous_current_next_block_verifiers_settings = 0;
    sync_all_block_verifiers_list(0);
  }

  // check if it is running in registration mode only
  if (registration_settings == 1)
  {
    color_print("Registration mode is on, checking to make sure all network data nodes databases are synced","yellow");
    sync_network_data_nodes_database();

    // update the previous, current and next block verifiers after syncing the database
    if (update_block_verifiers_list() == 0)
    {
      START_NEW_ROUND_ERROR("Could not update the previous, current and next block verifiers list");
    }
    
    // start the reserve proofs timer
    pthread_create(&thread_id, NULL, &check_reserve_proofs_timer_thread, NULL);
    pthread_detach(thread_id);
    sync_block_verifiers_minutes_and_seconds((BLOCK_TIME-1),SUBMIT_NETWORK_BLOCK_TIME_SECONDS); 
    return 2;
  }

  

  if (count == XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT)
  {
    // this is the first block of the network
    color_print("The current block is the first block on the network, meaning that the main network node will create this block","yellow");

    RESET_VARIABLES;
    
    // set the main_network_data_node_create_block so the main network data node can create the block
    main_network_data_node_create_block = 1;
    if (start_current_round_start_blocks() == 0)
    {      
      START_NEW_ROUND_ERROR("start_current_round_start_blocks error");
    } 
  }
  else
  {
    // this is a X-CASH proof of stake block so this is not the start blocks of the network
    if (strncmp(VRF_data.block_blob,"",1) != 0)
    {
      // update all of the databases 
      color_print("Updating the previous rounds data in the databases","blue");
      update_databases();
    }

    RESET_VARIABLES;
    
    // reset the current_round_part and current_round_part_backup_node after the databases have been updated for the previous rounds statistics
    memset(current_round_part,0,sizeof(current_round_part));
    memcpy(current_round_part,"1",sizeof(char));
    memset(current_round_part_backup_node,0,sizeof(current_round_part_backup_node));
    memcpy(current_round_part_backup_node,"0",sizeof(char));

    // check if it is time to remove the inactive delegates
    get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);
    if (current_UTC_date_and_time.tm_hour == 23 && current_UTC_date_and_time.tm_min < BLOCK_TIME)
    {
      color_print("Removing inactive delegates\n","blue");
      //remove_inactive_delegates();
    }

    // all block verifiers will sync and make sure they have the same database
    color_print("\nChecking to make sure all block verifiers databases are synced","blue");
    sync_block_verifiers_database();

    // wait for all block verifiers to sync
    sync_block_verifiers_minutes_and_seconds(1,20);

    // update the previous, current and next block verifiers after syncing the database
    if (update_block_verifiers_list() == 0)
    {
      START_NEW_ROUND_ERROR("Could not update the previous, current and next block verifiers list");
    }

    // check if it is running in registration mode only
    if (registration_settings == 1)
    {
      // start the reserve proofs timer
      pthread_create(&thread_id, NULL, &check_reserve_proofs_timer_thread, NULL);
      pthread_detach(thread_id);
      sync_block_verifiers_minutes_and_seconds((BLOCK_TIME-1),SUBMIT_NETWORK_BLOCK_TIME_SECONDS); 
      return 2;
    }

    // wait for all block verifiers to sync, as this will ensure when we calculate the main node roles we have the same buffer
    sync_block_verifiers_minutes_and_seconds(1,25);

    if (calculate_main_nodes_roles() == 0)
    {
      print_error_message(current_date_and_time,current_UTC_date_and_time,data);
      START_NEW_ROUND_ERROR("Error calculating the next block producer.\nYour block verifier will wait until the next round\n");
    }

    // check if the syncing time is over the start time of the round
    get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);
    if (current_UTC_date_and_time.tm_min % BLOCK_TIME >= 2)
    {
      color_print("Your block verifier took longer to sync and the next round has already started, so your block verifier will sit out for the remainder of the round\n","yellow");
      sync_block_verifiers_minutes_and_seconds((BLOCK_TIME-1),SUBMIT_NETWORK_BLOCK_TIME_SECONDS);
      return 2;
    }
    
    if (block_verifiers_create_block() == 0)
    {
      START_NEW_ROUND_ERROR("Your block verifier will wait until the next round\n");
    }
  }
  count2 = 2;
  return count2;

  #undef START_NEW_ROUND_ERROR
  #undef RESET_VARIABLES
}



/*
-----------------------------------------------------------------------------------------------------------
Name: start_blocks_create_vrf_data
Description: Creates the VRF data for the start block
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int start_blocks_create_vrf_data(void)
{
  // Variables
  size_t count;
  size_t count2;

  // define macros
  #define START_BLOCKS_CREATE_VRF_DATA_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"start_blocks_create_vrf_data",28); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  if (create_random_VRF_keys(VRF_data.vrf_public_key,VRF_data.vrf_secret_key) == 1 && crypto_vrf_is_valid_key((const unsigned char*)VRF_data.vrf_public_key) != 1)
  {
    START_BLOCKS_CREATE_VRF_DATA_ERROR("Could not create the vrf_public_key or vrf_secret_key");
  }

  memset(blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,0,strlen(blockchain_data.blockchain_reserve_bytes.previous_block_hash_data));
  memset(blockchain_data.previous_block_hash_data,0,strlen(blockchain_data.previous_block_hash_data));
  memset(VRF_data.vrf_alpha_string,0,strlen((char*)VRF_data.vrf_alpha_string));    
  memcpy(blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,previous_block_hash,BLOCK_HASH_LENGTH);
  memcpy(blockchain_data.previous_block_hash_data,previous_block_hash,BLOCK_HASH_LENGTH);
  memcpy(VRF_data.vrf_alpha_string,previous_block_hash,BLOCK_HASH_LENGTH);
  blockchain_data.previous_block_hash_data_length = BLOCK_HASH_LENGTH;
  blockchain_data.blockchain_reserve_bytes.previous_block_hash_data_length = BLOCK_HASH_LENGTH;

  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memcpy(VRF_data.vrf_alpha_string+strlen((const char*)VRF_data.vrf_alpha_string),GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING)-1);
  }   

  // convert the vrf alpha string to a string
  for (count2 = 0, count = 0; count2 < (((RANDOM_STRING_LENGTH*2)*BLOCK_VERIFIERS_AMOUNT) + (BLOCK_HASH_LENGTH*2)) / 2; count2++, count += 2)
  {
    snprintf(VRF_data.vrf_alpha_string_data+count,BUFFER_SIZE-1,"%02x",VRF_data.vrf_alpha_string[count2] & 0xFF);
  }

  if (crypto_vrf_prove(VRF_data.vrf_proof,(const unsigned char*)VRF_data.vrf_secret_key,(const unsigned char*)VRF_data.vrf_alpha_string_data,(unsigned long long)strlen((const char*)VRF_data.vrf_alpha_string_data)) != 0)
  {
    START_BLOCKS_CREATE_VRF_DATA_ERROR("Could not create the vrf proof");
  }
  if (crypto_vrf_proof_to_hash(VRF_data.vrf_beta_string,(const unsigned char*)VRF_data.vrf_proof) != 0)
  {
    START_BLOCKS_CREATE_VRF_DATA_ERROR("Could not create the vrf beta string");
  }
  if (crypto_vrf_verify(VRF_data.vrf_beta_string,(const unsigned char*)VRF_data.vrf_public_key,(const unsigned char*)VRF_data.vrf_proof,(const unsigned char*)VRF_data.vrf_alpha_string_data,(unsigned long long)strlen((const char*)VRF_data.vrf_alpha_string_data)) != 0)
  {
    START_BLOCKS_CREATE_VRF_DATA_ERROR("Could not create the VRF data");
  }

  // convert all of the VRF data to a string
  for (count2 = 0, count = 0; count2 < crypto_vrf_SECRETKEYBYTES; count2++, count += 2)
  {
    snprintf(VRF_data.vrf_secret_key_data+count,BUFFER_SIZE_NETWORK_BLOCK_DATA-1,"%02x",VRF_data.vrf_secret_key[count2] & 0xFF);
  }
  for (count2 = 0, count = 0; count2 < crypto_vrf_PUBLICKEYBYTES; count2++, count += 2)
  {
    snprintf(VRF_data.vrf_public_key_data+count,BUFFER_SIZE_NETWORK_BLOCK_DATA-1,"%02x",VRF_data.vrf_public_key[count2] & 0xFF);
  }
  for (count2 = 0, count = 0; count2 < crypto_vrf_PROOFBYTES; count2++, count += 2)
  {
    snprintf(VRF_data.vrf_proof_data+count,BUFFER_SIZE_NETWORK_BLOCK_DATA-1,"%02x",VRF_data.vrf_proof[count2] & 0xFF);
  }
  for (count2 = 0, count = 0; count2 < crypto_vrf_OUTPUTBYTES; count2++, count += 2)
  {
    snprintf(VRF_data.vrf_beta_string_data+count,BUFFER_SIZE_NETWORK_BLOCK_DATA-1,"%02x",VRF_data.vrf_beta_string[count2] & 0xFF);
  }  

  // add all of the VRF data to the blockchain_data struct
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_secret_key,VRF_data.vrf_secret_key,crypto_vrf_SECRETKEYBYTES);
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_secret_key_data,VRF_data.vrf_secret_key_data,VRF_SECRET_KEY_LENGTH);
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_public_key,VRF_data.vrf_public_key,crypto_vrf_PUBLICKEYBYTES);
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data,VRF_data.vrf_public_key_data,VRF_PUBLIC_KEY_LENGTH);
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string,VRF_data.vrf_alpha_string,strnlen((const char*)VRF_data.vrf_alpha_string,BUFFER_SIZE));
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data,VRF_data.vrf_alpha_string_data,strnlen(VRF_data.vrf_alpha_string_data,BUFFER_SIZE));
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_proof,VRF_data.vrf_proof,crypto_vrf_PROOFBYTES);
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_proof_data,VRF_data.vrf_proof_data,VRF_PROOF_LENGTH);
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_beta_string,VRF_data.vrf_beta_string,crypto_vrf_OUTPUTBYTES);
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data,VRF_data.vrf_beta_string_data,VRF_BETA_LENGTH);

  memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key[0],blockchain_data.blockchain_reserve_bytes.vrf_secret_key,crypto_vrf_SECRETKEYBYTES);
  memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key_data[0],blockchain_data.blockchain_reserve_bytes.vrf_secret_key_data,VRF_SECRET_KEY_LENGTH);
  memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key[0],blockchain_data.blockchain_reserve_bytes.vrf_public_key,crypto_vrf_PUBLICKEYBYTES);
  memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key_data[0],blockchain_data.blockchain_reserve_bytes.vrf_public_key_data,VRF_PUBLIC_KEY_LENGTH);
  memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data[0],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING)-1);
  
  for (count = 1; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key_data[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_SECRET_KEY_DATA,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_SECRET_KEY_DATA)-1);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key_data[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_PUBLIC_KEY_DATA,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_PUBLIC_KEY_DATA)-1);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING)-1);
  }
  return 1;

  #undef START_BLOCKS_CREATE_VRF_DATA_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: start_blocks_create_data
Description: Creates the data for the start block
Parameters:
  message - The data
  network_block_string - The network_block_string
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int start_blocks_create_data(char* message, char* network_block_string)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  size_t count;

  // define macros
  #define DATABASE_COLLECTION "reserve_bytes_1"
  #define START_BLOCKS_CREATE_DATA_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"start_blocks_create_data",24); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  // get a block template
  if (get_block_template(data) == 0)
  {
    START_BLOCKS_CREATE_DATA_ERROR("Could not get a block template");
  }

  // convert the network_block_string to blockchain_data
  if (network_block_string_to_blockchain_data((const char*)data,"0",BLOCK_VERIFIERS_AMOUNT) == 0)
  {
    START_BLOCKS_CREATE_DATA_ERROR("Could not convert the network_block_string to blockchain_data");
  }

  // change the nonce to the CONSENSUS_NODE_NETWORK_BLOCK_NONCE
  memcpy(blockchain_data.nonce_data,CONSENSUS_NODE_NETWORK_BLOCK_NONCE,sizeof(CONSENSUS_NODE_NETWORK_BLOCK_NONCE)-1);

  // add the delegates data to the network_block_string
  memset(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name,BUFFER_SIZE));
  memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name,"network_data_node_1",19);
  memset(blockchain_data.blockchain_reserve_bytes.block_producer_public_address,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_public_address,BUFFER_SIZE));
  memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_public_address,network_data_nodes_list.network_data_nodes_public_address[0],XCASH_WALLET_LENGTH);
  memset(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,BUFFER_SIZE));
  memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,"0",sizeof(char));
  memset(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names,BUFFER_SIZE));
  memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names,"network_data_node_1,network_data_node_1,network_data_node_1,network_data_node_1,network_data_node_1",99);

  // add the VRF data
  if (start_blocks_create_vrf_data() == 0)
  {
    START_BLOCKS_CREATE_DATA_ERROR("Could not add the VRF data");
  }
  
  // add the next block verifiers and add 0`s for the block_validation_node_signature
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  { 
    memcpy(blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address[count],next_block_verifiers_list.block_verifiers_public_key[count],VRF_PUBLIC_KEY_LENGTH);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE_DATA,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE_DATA)-1);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE)-1);
  }
  
  // convert the blockchain_data to a network_block_string
  memset(data,0,sizeof(data));
  if (blockchain_data_to_network_block_string(data,BLOCK_VERIFIERS_AMOUNT) == 0)
  {
    START_BLOCKS_CREATE_DATA_ERROR("Could not convert the blockchain_data to a network_block_string");
  }

  // sign the network block string
  if (sign_network_block_string(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[0],data) == 0)
  {
    START_BLOCKS_CREATE_DATA_ERROR("Could not sign the network block string");
  }

  // convert the blockchain_data to a network_block_string
  memset(VRF_data.block_blob,0,strlen(VRF_data.block_blob));
  if (blockchain_data_to_network_block_string(VRF_data.block_blob,BLOCK_VERIFIERS_AMOUNT) == 0)
  {
    START_BLOCKS_CREATE_DATA_ERROR("Could not convert the blockchain_data to a network_block_string");
  }

  // add the data hash to the network block string
  memset(network_block_string,0,strlen(network_block_string));
  if (add_data_hash_to_network_block_string(VRF_data.block_blob,network_block_string) == 0)
  {
    START_BLOCKS_CREATE_DATA_ERROR("Could not add the network block string data hash");
  }

  // update the reserve bytes database
  memset(data2,0,sizeof(data2));
  memcpy(data2,"{\"block_height\":\"",17);
  memcpy(data2+17,current_block_height,strnlen(current_block_height,sizeof(data2)));
  memcpy(data2+strlen(data2),"\",\"reserve_bytes_data_hash\":\"",29);
  memcpy(data2+strlen(data2),VRF_data.reserve_bytes_data_hash,DATA_HASH_LENGTH);
  memcpy(data2+strlen(data2),"\",\"reserve_bytes\":\"",19);
  memcpy(data2+strlen(data2),VRF_data.block_blob,strnlen(VRF_data.block_blob,sizeof(data2)));
  memcpy(data2+strlen(data2),"\"}",2);

  // add the network block string to the database
  if (insert_document_into_collection_json(database_name,DATABASE_COLLECTION,data2) == 0)
  {
    START_BLOCKS_CREATE_DATA_ERROR("Could not add the new block to the database");
  }

  // create the message
  memset(message,0,strlen(message));
  memcpy(message,"{\r\n \"message_settings\": \"MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_START_BLOCK\",\r\n \"database_data\": \"",101);
  memcpy(message+101,data2,strnlen(data2,BUFFER_SIZE));
  memcpy(message+strlen(message),"\",\r\n \"reserve_bytes_data_hash\": \"",33);
  memcpy(message+strlen(message),VRF_data.reserve_bytes_data_hash,DATA_HASH_LENGTH);
  memcpy(message+strlen(message),"\",\r\n}",5);
  
  // sign_data
  if (sign_data(message) == 0)
  { 
    START_BLOCKS_CREATE_DATA_ERROR("Could not sign_data");
  }

  // clear the VRF_data.block_blob so at the start of the next round, the main network data node does not try to update the databases
  memset(VRF_data.block_blob,0,strlen(VRF_data.block_blob));
  return 1;

  #undef DATABASE_COLLECTION
  #undef START_BLOCKS_CREATE_DATA_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: start_current_round_start_blocks
Description: Runs the round where the network data node will create the first block of the X-CASH proof of stake block on the network
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int start_current_round_start_blocks(void)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  
  // define macros
  #define DATABASE_COLLECTION "reserve_bytes_1"

  #define START_CURRENT_ROUND_START_BLOCKS_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"start_current_round_start_blocks",32); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;
  
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // set the main_network_data_node_create_block so the main network data node can create the block
  main_network_data_node_create_block = 1;

  // check if the block verifier is the main network data node
  if (strncmp(network_data_nodes_list.network_data_nodes_public_address[0],xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
  {
    color_print("Your block verifier is not the main data network node so your block verifier will sit out for the remainder of the round\n","yellow");
    sync_block_verifiers_minutes_and_seconds((BLOCK_TIME-1),SUBMIT_NETWORK_BLOCK_TIME_SECONDS);
    return 1;
  } 

  color_print("Your block verifier is the main data network node so your block verifier will create the block\n","yellow");

  // wait until the non network data nodes have synced the previous current and next block verifiers list
  sleep(30);
  
  // create the data
  if (start_blocks_create_data(data,data2) == 0)
  {
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not create the start blocks data");
  }

  // send the database data to all block verifiers
  sleep(BLOCK_VERIFIERS_SETTINGS);
  block_verifiers_send_data_socket((const char*)data);

  color_print("Waiting for the block producer to submit the block to the network\n","blue");
  sync_block_verifiers_minutes_and_seconds((BLOCK_TIME-1),SUBMIT_NETWORK_BLOCK_TIME_SECONDS);

  // have the main network data node submit the block to the network  
  submit_block_template(data2);
  
  return 1;

  #undef DATABASE_COLLECTION
  #undef START_CURRENT_ROUND_START_BLOCKS_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: block_verifiers_create_VRF_secret_key_and_VRF_public_key
Description: The block verifiers will create a VRF secret key and a VRF public key
Parameters:
  message - The message to send to the block verifiers
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int block_verifiers_create_VRF_secret_key_and_VRF_public_key(char* message)
{
  // Variables
  char data[BUFFER_SIZE];
  size_t count;
  size_t counter;

  // define macros
  #define BLOCK_VERIFIERS_CREATE_VRF_SECRET_KEY_AND_VRF_PUBLIC_KEY_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"block_verifiers_create_VRF_secret_key_and_VRF_public_key",56); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0; 

  memset(data,0,sizeof(data));
  
  // create a random VRF public key and secret key
  if (create_random_VRF_keys(VRF_data.vrf_public_key,VRF_data.vrf_secret_key) != 1 || crypto_vrf_is_valid_key((const unsigned char*)VRF_data.vrf_public_key) != 1)
  {
    BLOCK_VERIFIERS_CREATE_VRF_SECRET_KEY_AND_VRF_PUBLIC_KEY_ERROR("Could not create the VRF secret key or VRF public key for the VRF data");
  }  

  // convert the VRF secret key to hexadecimal
  for (count = 0, counter = 0; count < crypto_vrf_SECRETKEYBYTES; count++, counter += 2)
  {
    snprintf(VRF_data.vrf_secret_key_data+counter,BUFFER_SIZE_NETWORK_BLOCK_DATA-1,"%02x",VRF_data.vrf_secret_key[count] & 0xFF);
  }

  // convert the VRF public key to hexadecimal
  for (count = 0, counter = 0; count < crypto_vrf_PUBLICKEYBYTES; count++, counter += 2)
  {
    snprintf(VRF_data.vrf_public_key_data+counter,BUFFER_SIZE_NETWORK_BLOCK_DATA-1,"%02x",VRF_data.vrf_public_key[count] & 0xFF);
  } 

  // create the message
  memset(message,0,strlen(message));
  memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA\",\r\n \"vrf_secret_key\": \"",92);
  memcpy(message+92,VRF_data.vrf_secret_key_data,VRF_SECRET_KEY_LENGTH);
  memcpy(message+220,"\",\r\n \"vrf_public_key\": \"",24);
  memcpy(message+244,VRF_data.vrf_public_key_data,VRF_PUBLIC_KEY_LENGTH);
  memcpy(message+308,"\",\r\n \"random_data\": \"",21);
  
  // create random data to use in the alpha string of the VRF data
  if (random_string(data,RANDOM_STRING_LENGTH) == 0)
  {
    BLOCK_VERIFIERS_CREATE_VRF_SECRET_KEY_AND_VRF_PUBLIC_KEY_ERROR("Could not create random data for the VRF data");
  }

  memcpy(message+329,data,RANDOM_STRING_LENGTH);
  memcpy(message+429,"\",\r\n}",5);

  // add the VRF data to the block verifiers VRF data copy
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    if (strncmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0)
    {        
      memcpy(VRF_data.block_verifiers_vrf_secret_key[count],VRF_data.vrf_secret_key,crypto_vrf_SECRETKEYBYTES);
      memcpy(VRF_data.block_verifiers_vrf_secret_key_data[count],VRF_data.vrf_secret_key_data,VRF_SECRET_KEY_LENGTH);
      memcpy(VRF_data.block_verifiers_vrf_public_key[count],VRF_data.vrf_public_key,crypto_vrf_PUBLICKEYBYTES);
      memcpy(VRF_data.block_verifiers_vrf_public_key_data[count],VRF_data.vrf_public_key_data,VRF_PUBLIC_KEY_LENGTH);
      memcpy(VRF_data.block_verifiers_random_data[count],data,RANDOM_STRING_LENGTH);
    }
  } 
  return 1;

  #undef BLOCK_VERIFIERS_CREATE_VRF_SECRET_KEY_AND_VRF_PUBLIC_KEY_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: block_verifiers_create_VRF_data
Description: The block verifiers will create all of the VRF data
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int block_verifiers_create_VRF_data(void)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  size_t count;
  size_t count2;
  size_t counter;

  // define macros
  #define BLOCK_VERIFIERS_CREATE_VRF_DATA_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"block_verifiers_create_VRF_data",31); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0; 

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // create the VRF alpha string using all of the random data from the block verifiers
  memset(VRF_data.vrf_alpha_string,0,strlen((const char*)VRF_data.vrf_alpha_string));
  memcpy(VRF_data.vrf_alpha_string,previous_block_hash,BLOCK_HASH_LENGTH);
    
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    if (strlen((const char*)VRF_data.block_verifiers_vrf_secret_key[count]) == crypto_vrf_SECRETKEYBYTES && strlen((const char*)VRF_data.block_verifiers_vrf_public_key[count]) == crypto_vrf_PUBLICKEYBYTES && strlen(VRF_data.block_verifiers_random_data[count]) == RANDOM_STRING_LENGTH)
    {
      memcpy(VRF_data.vrf_alpha_string+strlen((const char*)VRF_data.vrf_alpha_string),VRF_data.block_verifiers_random_data[count],RANDOM_STRING_LENGTH);
    }
    else
    {
      memcpy(VRF_data.vrf_alpha_string+strlen((const char*)VRF_data.vrf_alpha_string),GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING)-1);
    }
  }

  // convert the vrf alpha string to a string
  for (count2 = 0, count = 0; count2 < (((RANDOM_STRING_LENGTH*2)*BLOCK_VERIFIERS_AMOUNT) + (BLOCK_HASH_LENGTH*2)) / 2; count2++, count += 2)
  {
    snprintf(VRF_data.vrf_alpha_string_data+count,BUFFER_SIZE-1,"%02x",VRF_data.vrf_alpha_string[count2] & 0xFF);
  }

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  crypto_hash_sha512((unsigned char*)data,(const unsigned char*)VRF_data.vrf_alpha_string_data,strlen(VRF_data.vrf_alpha_string_data));

  // convert the SHA512 data hash to a string
  for (count2 = 0, count = 0; count2 < DATA_HASH_LENGTH / 2; count2++, count += 2)
  {
    snprintf(data2+count,sizeof(data2)-1,"%02x",data[count2] & 0xFF);
  }

  // check what block verifiers vrf secret key and vrf public key to use
  for (count = 0; count < DATA_HASH_LENGTH; count += 2)
  {
    memset(data,0,sizeof(data));
    memcpy(data,&data2[count],2);
    counter = (int)strtol(data, NULL, 16); 
   
    /* if it is not in the range of 01 - FA or it has already been calculated then skip the byte
       This number needs to be evenly divisible by how many maximum block verifiers there will be
       This is so block verifiers in specific spots do not have more of a chance to be the block producer than others
       The goal is to use as many bytes as possible, since the more unused bytes, the more chance that it will run out of bytes when selecting the block producer
    */
    if (counter != 0 && counter <= 250)
    {
      counter = counter % BLOCK_VERIFIERS_AMOUNT;

      // check if the block verifier created the data
      if (strncmp(VRF_data.block_verifiers_vrf_secret_key_data[counter],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_SECRET_KEY_DATA,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_SECRET_KEY_DATA)-1) != 0 && strncmp(VRF_data.block_verifiers_vrf_public_key_data[counter],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_PUBLIC_KEY_DATA,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_PUBLIC_KEY_DATA)-1) != 0 && strncmp(VRF_data.block_verifiers_random_data[counter],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING)-1) != 0)
      {
        break;
      }
    }
  }

  // create all of the VRF data
  memcpy(VRF_data.vrf_secret_key_data,VRF_data.block_verifiers_vrf_secret_key_data[counter],VRF_SECRET_KEY_LENGTH);
  memcpy(VRF_data.vrf_secret_key,VRF_data.block_verifiers_vrf_secret_key[counter],crypto_vrf_SECRETKEYBYTES);
  memcpy(VRF_data.vrf_public_key_data,VRF_data.block_verifiers_vrf_public_key_data[counter],VRF_PUBLIC_KEY_LENGTH);
  memcpy(VRF_data.vrf_public_key,VRF_data.block_verifiers_vrf_public_key[counter],crypto_vrf_PUBLICKEYBYTES);

  if (crypto_vrf_prove(VRF_data.vrf_proof,(const unsigned char*)VRF_data.vrf_secret_key,(const unsigned char*)VRF_data.vrf_alpha_string_data,(unsigned long long)strlen((const char*)VRF_data.vrf_alpha_string_data)) != 0)
  {
    BLOCK_VERIFIERS_CREATE_VRF_DATA_ERROR("Could not create the vrf proof");
  }
  if (crypto_vrf_proof_to_hash(VRF_data.vrf_beta_string,(const unsigned char*)VRF_data.vrf_proof) != 0)
  {
    BLOCK_VERIFIERS_CREATE_VRF_DATA_ERROR("Could not create the vrf beta string");
  }
  if (crypto_vrf_verify(VRF_data.vrf_beta_string,(const unsigned char*)VRF_data.vrf_public_key,(const unsigned char*)VRF_data.vrf_proof,(const unsigned char*)VRF_data.vrf_alpha_string_data,(unsigned long long)strlen((const char*)VRF_data.vrf_alpha_string_data)) != 0)
  {
    BLOCK_VERIFIERS_CREATE_VRF_DATA_ERROR("Could not create the VRF data");
  }

  // convert the vrf proof and vrf beta string to a string
  for (counter = 0, count = 0; counter < crypto_vrf_PROOFBYTES; counter++, count += 2)
  {
    snprintf(VRF_data.vrf_proof_data+count,BUFFER_SIZE_NETWORK_BLOCK_DATA-1,"%02x",VRF_data.vrf_proof[counter] & 0xFF);
  }
  for (counter = 0, count = 0; counter < crypto_vrf_OUTPUTBYTES; counter++, count += 2)
  {
    snprintf(VRF_data.vrf_beta_string_data+count,BUFFER_SIZE_NETWORK_BLOCK_DATA-1,"%02x",VRF_data.vrf_beta_string[counter] & 0xFF);
  }
  return 1;

  #undef BLOCK_VERIFIERS_CREATE_VRF_DATA_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: block_verifiers_create_block_signature
Description: The block verifiers will create the block signature
Parameters:
  message - The message to send to the block verifiers
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int block_verifiers_create_block_signature(char* message)
{
  // Variables
  char data[BUFFER_SIZE];
  size_t count;
  size_t count2;
  size_t counter;
  int block_producer_backup_settings[BLOCK_PRODUCERS_BACKUP_AMOUNT] = {0,0,0,0,0};

  // define macros
  #define BLOCK_VERIFIERS_CREATE_BLOCK_SIGNATURE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"block_verifiers_create_block_signature",38); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0; 

  memset(data,0,sizeof(data));

  // convert the network block string to a blockchain data
  if (network_block_string_to_blockchain_data(VRF_data.block_blob,"0",BLOCK_VERIFIERS_AMOUNT) == 0)
  {
    BLOCK_VERIFIERS_CREATE_BLOCK_SIGNATURE_ERROR("Could not convert the network block string to a blockchain data");
  }

  // change the network block nonce to the block producer network block nonce
  memcpy(blockchain_data.nonce_data,BLOCK_PRODUCER_NETWORK_BLOCK_NONCE,sizeof(BLOCK_PRODUCER_NETWORK_BLOCK_NONCE)-1);

  // get the current block producer
  if (strncmp(current_round_part_backup_node,"0",1) == 0)
  {
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (strncmp(current_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_public_address,BUFFER_SIZE) == 0)
      {
        memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name,current_block_verifiers_list.block_verifiers_name[count],strnlen(current_block_verifiers_list.block_verifiers_name[count],BUFFER_SIZE)); 
        memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_public_address,current_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH);
        break;
      }
    }
  }
  else if (strncmp(current_round_part_backup_node,"1",1) == 0)
  {
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (strncmp(current_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_1_public_address,BUFFER_SIZE) == 0)
      {
        memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name,current_block_verifiers_list.block_verifiers_name[count],strnlen(current_block_verifiers_list.block_verifiers_name[count],BUFFER_SIZE)); 
        memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_public_address,current_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH);
        break;
      }
    }
  }
  else if (strncmp(current_round_part_backup_node,"2",1) == 0)
  {
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (strncmp(current_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_2_public_address,BUFFER_SIZE) == 0)
      {
        memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name,current_block_verifiers_list.block_verifiers_name[count],strnlen(current_block_verifiers_list.block_verifiers_name[count],BUFFER_SIZE)); 
        memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_public_address,current_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH);
        break;
      }
    }
  }
  else if (strncmp(current_round_part_backup_node,"3",1) == 0)
  {
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (strncmp(current_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_3_public_address,BUFFER_SIZE) == 0)
      {
        memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name,current_block_verifiers_list.block_verifiers_name[count],strnlen(current_block_verifiers_list.block_verifiers_name[count],BUFFER_SIZE)); 
        memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_public_address,current_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH);
        break;
      }
    }
  }
  else if (strncmp(current_round_part_backup_node,"4",1) == 0)
  {
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (strncmp(current_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_4_public_address,BUFFER_SIZE) == 0)
      {
        memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name,current_block_verifiers_list.block_verifiers_name[count],strnlen(current_block_verifiers_list.block_verifiers_name[count],BUFFER_SIZE)); 
        memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_public_address,current_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH);
        break;
      }
    }
  }
  else if (strncmp(current_round_part_backup_node,"5",1) == 0)
  {
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (strncmp(current_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_5_public_address,BUFFER_SIZE) == 0)
      {
        memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name,current_block_verifiers_list.block_verifiers_name[count],strnlen(current_block_verifiers_list.block_verifiers_name[count],BUFFER_SIZE)); 
        memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_public_address,current_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH);
        break;
      }
    }
  }

  // add all of the VRF data to the blockchain_data struct  
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {    
    if (strncmp(main_nodes_list.block_producer_backup_block_verifier_1_public_address,current_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH) == 0)
    {
      block_producer_backup_settings[0] = (int)count;
    }
    if (strncmp(main_nodes_list.block_producer_backup_block_verifier_2_public_address,current_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH) == 0)
    {
      block_producer_backup_settings[1] = (int)count;
    }
    if (strncmp(main_nodes_list.block_producer_backup_block_verifier_3_public_address,current_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH) == 0)
    {
      block_producer_backup_settings[2] = (int)count;
    }
    if (strncmp(main_nodes_list.block_producer_backup_block_verifier_4_public_address,current_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH) == 0)
    {
      block_producer_backup_settings[3] = (int)count;
    }
    if (strncmp(main_nodes_list.block_producer_backup_block_verifier_5_public_address,current_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH) == 0)
    {
      block_producer_backup_settings[4] = (int)count;
    }
  }

  memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,current_round_part_backup_node,sizeof(char));
    
  memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names+strlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names),current_block_verifiers_list.block_verifiers_name[block_producer_backup_settings[0]],strnlen(current_block_verifiers_list.block_verifiers_name[block_producer_backup_settings[0]],BUFFER_SIZE));
  memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names+strlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names),",",sizeof(char));
  memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names+strlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names),current_block_verifiers_list.block_verifiers_name[block_producer_backup_settings[1]],strnlen(current_block_verifiers_list.block_verifiers_name[block_producer_backup_settings[1]],BUFFER_SIZE));
  memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names+strlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names),",",sizeof(char));
  memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names+strlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names),current_block_verifiers_list.block_verifiers_name[block_producer_backup_settings[2]],strnlen(current_block_verifiers_list.block_verifiers_name[block_producer_backup_settings[2]],BUFFER_SIZE));
  memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names+strlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names),",",sizeof(char));
  memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names+strlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names),current_block_verifiers_list.block_verifiers_name[block_producer_backup_settings[3]],strnlen(current_block_verifiers_list.block_verifiers_name[block_producer_backup_settings[3]],BUFFER_SIZE));
  memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names+strlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names),",",sizeof(char));
  memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names+strlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names),current_block_verifiers_list.block_verifiers_name[block_producer_backup_settings[4]],strnlen(current_block_verifiers_list.block_verifiers_name[block_producer_backup_settings[4]],BUFFER_SIZE));

  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_secret_key,VRF_data.vrf_secret_key,crypto_vrf_SECRETKEYBYTES);
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_secret_key_data,VRF_data.vrf_secret_key_data,VRF_SECRET_KEY_LENGTH);
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_public_key,VRF_data.vrf_public_key,crypto_vrf_PUBLICKEYBYTES);
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data,VRF_data.vrf_public_key_data,VRF_PUBLIC_KEY_LENGTH);
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string,VRF_data.vrf_alpha_string,strnlen((const char*)VRF_data.vrf_alpha_string,BUFFER_SIZE));
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data,VRF_data.vrf_alpha_string_data,strnlen(VRF_data.vrf_alpha_string_data,BUFFER_SIZE));
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_proof,VRF_data.vrf_proof,crypto_vrf_PROOFBYTES);
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_proof_data,VRF_data.vrf_proof_data,VRF_PROOF_LENGTH);
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_beta_string,VRF_data.vrf_beta_string,crypto_vrf_OUTPUTBYTES);
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data,VRF_data.vrf_beta_string_data,VRF_BETA_LENGTH);

  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key[count],VRF_data.block_verifiers_vrf_secret_key[count],crypto_vrf_SECRETKEYBYTES);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key[count],VRF_data.block_verifiers_vrf_public_key[count],crypto_vrf_PUBLICKEYBYTES);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key_data[count],VRF_data.block_verifiers_vrf_secret_key_data[count],VRF_SECRET_KEY_LENGTH);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key_data[count],VRF_data.block_verifiers_vrf_public_key_data[count],VRF_PUBLIC_KEY_LENGTH);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data_text[count],VRF_data.block_verifiers_random_data[count],RANDOM_STRING_LENGTH);

    memcpy(blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address[count],next_block_verifiers_list.block_verifiers_public_key[count],VRF_PUBLIC_KEY_LENGTH);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE_DATA,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE_DATA)-1);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE)-1);
            
    for (counter = 0, count2 = 0; counter < RANDOM_STRING_LENGTH; counter++, count2 += 2)
    {
      snprintf(blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data[count]+count2,RANDOM_STRING_LENGTH,"%02x",VRF_data.block_verifiers_random_data[count][counter] & 0xFF);
    }
  }

  memcpy(blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,blockchain_data.previous_block_hash_data,BLOCK_HASH_LENGTH);

  // convert the blockchain_data to a network_block_string
  if (blockchain_data_to_network_block_string(VRF_data.block_blob,BLOCK_VERIFIERS_AMOUNT) == 0)
  {
    BLOCK_VERIFIERS_CREATE_BLOCK_SIGNATURE_ERROR("Could not convert the blockchain_data to a network_block_string");
  }

  // sign the network block string
  memset(data,0,sizeof(data));
  if (sign_network_block_string(data,VRF_data.block_blob) == 0)
  {
    BLOCK_VERIFIERS_CREATE_BLOCK_SIGNATURE_ERROR("Could not sign the network block string");
  }

  // add the block verifier signature to the VRF data and the blockchain_data struct
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    if (strncmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0)
    {
      memcpy(VRF_data.block_blob_signature[count],data,strnlen(data,BUFFER_SIZE));
    }
  }

  // create the message
  memset(message,0,strlen(message));
  memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE\",\r\n \"block_blob_signature\": \"",110);
  memcpy(message+110,data,strnlen(data,BUFFER_SIZE));
  memcpy(message+strlen(message),"\",\r\n}",5);
  return 1;

  #undef BLOCK_VERIFIERS_CREATE_BLOCK_SIGNATURE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: block_verifiers_create_vote_results
Description: The block verifiers will create the vote results
Parameters:
  message - The message to send to the block verifiers
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int block_verifiers_create_vote_results(char* message)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  size_t count;
  size_t count2;

  // define macros
  #define BLOCK_VERIFIERS_CREATE_VOTE_RESULTS_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"block_verifiers_create_vote_results",35); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0; 

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  // verify the block
  if (verify_network_block_data(1,1,"0",BLOCK_VERIFIERS_AMOUNT) == 0)
  {
    BLOCK_VERIFIERS_CREATE_VOTE_RESULTS_ERROR("The MAIN_NODES_TO_NODES_PART_4_OF_ROUND message is invalid");
  }

  // convert the blockchain_data to a network_block_string
  memset(data,0,sizeof(data));	
  if (blockchain_data_to_network_block_string(data,BLOCK_VERIFIERS_AMOUNT) == 0)	
  {		 
    BLOCK_VERIFIERS_CREATE_VOTE_RESULTS_ERROR("Could not convert the blockchain_data to a network_block_string");	
  }
  
  memset(VRF_data.block_blob,0,strlen(VRF_data.block_blob));
  memcpy(VRF_data.block_blob,data,strnlen(data,BUFFER_SIZE));

  // get the data hash of the network block string
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));
  crypto_hash_sha512((unsigned char*)data2,(const unsigned char*)data,(unsigned long long)strnlen(data,BUFFER_SIZE));

  // convert the SHA512 data hash to a string
  for (count2 = 0, count = 0; count2 < DATA_HASH_LENGTH / 2; count2++, count += 2)
  {
    snprintf(data3+count,MAXIMUM_NUMBER_SIZE,"%02x",data2[count2] & 0xFF);
  }

  // reset the current_round_part_vote_data.vote_results_valid struct
  memset(current_round_part_vote_data.current_vote_results,0,sizeof(current_round_part_vote_data.current_vote_results));
  current_round_part_vote_data.vote_results_valid = 1;
  current_round_part_vote_data.vote_results_invalid = 0;

  memcpy(current_round_part_vote_data.current_vote_results,data3,DATA_HASH_LENGTH);

  // create the message
  memset(message,0,strlen(message));
  memcpy(message,"{\r\n \"message_settings\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n \"vote_settings\": \"valid\",\r\n \"vote_data\": \"",99);  
  memcpy(message+strlen(message),current_round_part_vote_data.current_vote_results,DATA_HASH_LENGTH);
  memcpy(message+strlen(message),"\",\r\n}",5); 
  return 1;

  #undef BLOCK_VERIFIERS_CREATE_VOTE_RESULTS_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: block_verifiers_create_block_and_update_database
Description: The block verifiers will create the vote results
Parameters:
  message - The message to send to the block verifiers
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int block_verifiers_create_block_and_update_database(void)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  size_t count;

  // threads
  pthread_t thread_id;

  // define macros
  #define BLOCK_VERIFIERS_CREATE_BLOCK_TIMEOUT_SETTINGS 5 // The time to wait to check if the block was created
  #define BLOCK_VERIFIERS_CREATE_BLOCK_AND_UPDATE_DATABASES_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"block_verifiers_create_block_and_update_database",48); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0; 

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  // add the data hash to the network block string
  if (add_data_hash_to_network_block_string(VRF_data.block_blob,data) == 0)
  {
    BLOCK_VERIFIERS_CREATE_BLOCK_AND_UPDATE_DATABASES_ERROR("Could not add the network block string data hash");
  }
    
  // update the reserve bytes database
  memset(data2,0,sizeof(data2));
  memcpy(data2,"{\"block_height\":\"",17);
  memcpy(data2+17,current_block_height,strnlen(current_block_height,sizeof(data2)));
  memcpy(data2+strlen(data2),"\",\"reserve_bytes_data_hash\":\"",29);
  memcpy(data2+strlen(data2),VRF_data.reserve_bytes_data_hash,DATA_HASH_LENGTH);
  memcpy(data2+strlen(data2),"\",\"reserve_bytes\":\"",19);
  memcpy(data2+strlen(data2),VRF_data.block_blob,strnlen(VRF_data.block_blob,sizeof(data2)));
  memcpy(data2+strlen(data2),"\"}",2);

  // add the network block string to the database
  get_reserve_bytes_database(count,0);
  memcpy(data3,"reserve_bytes_",14);
  snprintf(data3+14,sizeof(data3)-15,"%zu",count);
  if (insert_document_into_collection_json(database_name,data3,data2) == 0)
  {
    BLOCK_VERIFIERS_CREATE_BLOCK_AND_UPDATE_DATABASES_ERROR("Could not add the new block to the database");
  }
  
  // start the reserve proofs timer
  if (strncmp(current_round_part_backup_node,"0",1) == 0)
  {
    pthread_create(&thread_id, NULL, &check_reserve_proofs_timer_thread, NULL);
    pthread_detach(thread_id);
  }

  sync_block_verifiers_minutes_and_seconds((BLOCK_TIME-1),SUBMIT_NETWORK_BLOCK_TIME_SECONDS);  

  // let the block producer try to submit the block first, then loop through all of the network data nodes to make sure it was submitted
  if ((strncmp(current_round_part_backup_node,"0",1) == 0 && strncmp(main_nodes_list.block_producer_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (strncmp(current_round_part_backup_node,"1",1) == 0 && strncmp(main_nodes_list.block_producer_backup_block_verifier_1_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (strncmp(current_round_part_backup_node,"2",1) == 0 && strncmp(main_nodes_list.block_producer_backup_block_verifier_2_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (strncmp(current_round_part_backup_node,"3",1) == 0 && strncmp(main_nodes_list.block_producer_backup_block_verifier_3_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (strncmp(current_round_part_backup_node,"4",1) == 0 && strncmp(main_nodes_list.block_producer_backup_block_verifier_4_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (strncmp(current_round_part_backup_node,"5",1) == 0 && strncmp(main_nodes_list.block_producer_backup_block_verifier_5_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0))
  {
    submit_block_template(data);
  }
  sleep(BLOCK_VERIFIERS_SETTINGS);

  for (count = 0; count < NETWORK_DATA_NODES_AMOUNT; count++)
  {
    if (strncmp(network_data_nodes_list.network_data_nodes_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0)
    {
      submit_block_template(data);
    }
  }
  return 1;

  #undef BLOCK_VERIFIERS_CREATE_BLOCK_TIMEOUT_SETTINGS
  #undef BLOCK_VERIFIERS_CREATE_BLOCK_AND_UPDATE_DATABASES_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: print_block_producer
Description: Prints the block producers name
-----------------------------------------------------------------------------------------------------------
*/

void print_block_producer(void)
{
  // Variables
  int count;

  if (strncmp(current_round_part_backup_node,"0",1) == 0)
  {
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (strncmp(current_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_public_address,BUFFER_SIZE) == 0)
      {
        fprintf(stderr,"\033[1;36m%s is the block producer\033[0m\n",current_block_verifiers_list.block_verifiers_name[count]);
        break;
      }
    }
  }
  else if (strncmp(current_round_part_backup_node,"1",1) == 0)
  {
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (strncmp(current_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_1_public_address,BUFFER_SIZE) == 0)
      {
        fprintf(stderr,"\033[1;36m%s is the block producer\033[0m\n",current_block_verifiers_list.block_verifiers_name[count]);
        break;
      }
    }
  }
  else if (strncmp(current_round_part_backup_node,"2",1) == 0)
  {
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (strncmp(current_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_2_public_address,BUFFER_SIZE) == 0)
      {
        fprintf(stderr,"\033[1;36m%s is the block producer\033[0m\n",current_block_verifiers_list.block_verifiers_name[count]);
        break;
      }
    }
  }
  else if (strncmp(current_round_part_backup_node,"3",1) == 0)
  {
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (strncmp(current_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_3_public_address,BUFFER_SIZE) == 0)
      {
        fprintf(stderr,"\033[1;36m%s is the block producer\033[0m\n",current_block_verifiers_list.block_verifiers_name[count]);
        break;
      }
    }
  }
  else if (strncmp(current_round_part_backup_node,"4",1) == 0)
  {
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (strncmp(current_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_4_public_address,BUFFER_SIZE) == 0)
      {
        fprintf(stderr,"\033[1;36m%s is the block producer\033[0m\n",current_block_verifiers_list.block_verifiers_name[count]);
        break;
      }
    }
  }
  else if (strncmp(current_round_part_backup_node,"5",1) == 0)
  {
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (strncmp(current_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_5_public_address,BUFFER_SIZE) == 0)
      {
        fprintf(stderr,"\033[1;36m%s is the block producer\033[0m\n",current_block_verifiers_list.block_verifiers_name[count]);
        break;
      }
    }
  }
  return;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: block_verifiers_create_block
Description: Runs the round where the block verifiers will create the block
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int block_verifiers_create_block(void)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  size_t count;
  size_t count2;

  // define macros
  #define RESTART_ROUND(message) \
  color_print("\n","white"); \
  color_print(message,"red"); \
  memset(data,0,sizeof(data)); \
  memset(data2,0,sizeof(data2)); \
  if (error_message_count != 0) \
  { \
    print_error_message(current_date_and_time,current_UTC_date_and_time,data); \
    memset(data,0,sizeof(data)); \
  } \
  memset(VRF_data.vrf_secret_key_data,0,strlen(VRF_data.vrf_secret_key_data)); \
  memset(VRF_data.vrf_secret_key,0,strlen((const char*)VRF_data.vrf_secret_key)); \
  memset(VRF_data.vrf_public_key_data,0,strlen(VRF_data.vrf_public_key_data)); \
  memset(VRF_data.vrf_public_key,0,strlen((const char*)VRF_data.vrf_public_key)); \
  memset(VRF_data.vrf_alpha_string_data,0,strlen(VRF_data.vrf_alpha_string_data)); \
  memset(VRF_data.vrf_alpha_string,0,strlen((const char*)VRF_data.vrf_alpha_string)); \
  memset(VRF_data.vrf_proof_data,0,strlen(VRF_data.vrf_proof_data)); \
  memset(VRF_data.vrf_proof,0,strlen((const char*)VRF_data.vrf_proof)); \
  memset(VRF_data.vrf_beta_string_data,0,strlen(VRF_data.vrf_beta_string_data)); \
  memset(VRF_data.vrf_beta_string,0,strlen((const char*)VRF_data.vrf_beta_string)); \
  memset(VRF_data.block_blob,0,strlen(VRF_data.block_blob)); \
  memset(VRF_data.reserve_bytes_data_hash,0,strlen(VRF_data.reserve_bytes_data_hash)); \
  memset(current_round_part_vote_data.current_vote_results,0,sizeof(current_round_part_vote_data.current_vote_results)); \
  for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++) \
  { \
    memset(VRF_data.block_verifiers_vrf_secret_key_data[count],0,strlen(VRF_data.block_verifiers_vrf_secret_key_data[count])); \
    memset(VRF_data.block_verifiers_vrf_secret_key[count],0,strlen((const char*)VRF_data.block_verifiers_vrf_secret_key[count])); \
    memset(VRF_data.block_verifiers_vrf_public_key_data[count],0,strlen(VRF_data.block_verifiers_vrf_public_key_data[count])); \
    memset(VRF_data.block_verifiers_vrf_public_key[count],0,strlen((const char*)VRF_data.block_verifiers_vrf_public_key[count])); \
    memset(VRF_data.block_verifiers_random_data[count],0,strlen(VRF_data.block_verifiers_random_data[count])); \
    memset(VRF_data.block_blob_signature[count],0,strlen(VRF_data.block_blob_signature[count])); \
  } \
  if (strncmp(current_round_part_backup_node,"0",1) == 0) \
  { \
    memset(current_round_part,0,sizeof(current_round_part)); \
    memcpy(current_round_part,"1",sizeof(char)); \
    memset(current_round_part_backup_node,0,sizeof(current_round_part_backup_node)); \
    memcpy(current_round_part_backup_node,"1",sizeof(char)); \
    memcpy(data,"Restarting the round with backup block producer 1 for block ",60); \
    memcpy(data+60,current_block_height,strnlen(current_block_height,BUFFER_SIZE)); \
    print_start_message(current_date_and_time,current_UTC_date_and_time,data,data2); \
    color_print("\n","white"); \
  } \
  else if (strncmp(current_round_part_backup_node,"1",1) == 0) \
  { \
    color_print("both the block producer and backup block producer failed, waiting for the next round to begin","red"); \
    return 0; \
  } \
  sync_block_verifiers_minutes_and_seconds(2,50); \
  goto start; 

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // set the main_network_data_node_create_block so the main network data node can create the block
  main_network_data_node_create_block = 0;
  
  // wait for all block verifiers to sync
  sync_block_verifiers_minutes_and_seconds(1,30);

  if (get_previous_block_hash(previous_block_hash) == 0)
  {
    RESTART_ROUND("Could not get the previous block hash");
  }

  start:

    print_block_producer();
    
    color_print("Part 1 - Create and send VRF data to all block verifiers","yellow");

    // create a random VRF public key and secret key
    if (block_verifiers_create_VRF_secret_key_and_VRF_public_key(data) == 0)
    {
      RESTART_ROUND("Could not create a VRF secret key and a VRF public key");
    }

    // sign_data
    if (sign_data(data) == 0)
    { 
      RESTART_ROUND("Could not sign_data");
    }

    // send the message to all block verifiers
    if (block_verifiers_send_data_socket((const char*)data) == 0)
    {
      RESTART_ROUND("Could not send data to the block verifiers");
    }

    // wait for the block verifiers to process the votes
    strncmp(current_round_part_backup_node,"0",1) == 0 ? sync_block_verifiers_minutes_and_seconds(1,45) : sync_block_verifiers_minutes_and_seconds(3,5);

    // process the data
    for (count = 0, count2 = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (strlen(VRF_data.block_verifiers_vrf_secret_key_data[count]) == VRF_SECRET_KEY_LENGTH && strlen(VRF_data.block_verifiers_vrf_public_key_data[count]) == VRF_PUBLIC_KEY_LENGTH && strlen(VRF_data.block_verifiers_random_data[count]) == RANDOM_STRING_LENGTH)
      {
        count2++;
      }
      else
      {
        memset(VRF_data.block_verifiers_vrf_secret_key_data[count],0,strlen(VRF_data.block_verifiers_vrf_secret_key_data[count]));
        memset(VRF_data.block_verifiers_vrf_secret_key[count],0,strlen((char*)VRF_data.block_verifiers_vrf_secret_key[count]));
        memset(VRF_data.block_verifiers_vrf_public_key_data[count],0,strlen(VRF_data.block_verifiers_vrf_public_key_data[count]));
        memset(VRF_data.block_verifiers_vrf_public_key[count],0,strlen((char*)VRF_data.block_verifiers_vrf_public_key[count]));
        memset(VRF_data.block_verifiers_random_data[count],0,strlen(VRF_data.block_verifiers_random_data[count]));
        memcpy(VRF_data.block_verifiers_vrf_secret_key_data[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_SECRET_KEY_DATA,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_SECRET_KEY_DATA)-1);
        memcpy(VRF_data.block_verifiers_vrf_secret_key[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_SECRET_KEY,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_SECRET_KEY)-1);
        memcpy(VRF_data.block_verifiers_vrf_public_key_data[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_PUBLIC_KEY_DATA,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_PUBLIC_KEY_DATA)-1);
        memcpy(VRF_data.block_verifiers_vrf_public_key[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_PUBLIC_KEY,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_PUBLIC_KEY)-1);
        memcpy(VRF_data.block_verifiers_random_data[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING)-1);
      }
    }

    // check if at least 67 of the block verifiers created the data
    if (count2 >= BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      fprintf(stderr,"\033[1;32m%zu / %d block verifiers created valid VRF data\033[0m\n\n",count2,BLOCK_VERIFIERS_VALID_AMOUNT);      
    }
    else if (count2 == 1)
    {
      // restart the delegate, as it could only verify its own message and not anyone elses message
      color_print("Restarting, could not process any other block verifiers data","red");
      exit(0);
    }
    else
    {
      fprintf(stderr,"\033[1;31m%zu / %d block verifiers created valid VRF data\033[0m\n\n",count2,BLOCK_VERIFIERS_VALID_AMOUNT);
      RESTART_ROUND("An invalid amount of block verifiers created valid VRF data");     
    }

  

    // at this point all block verifiers should have the all of the other block verifiers secret key, public key and random data
    fprintf(stderr,"\033[1;33mPart 2 - Select VRF data and block producer creates the block and sends it to all block verifiers\033[0m\n\n");

    // create all of the VRF data
    if (block_verifiers_create_VRF_data() == 0)
    {
      RESTART_ROUND("Could not create the VRF data");
    }

    memset(VRF_data.block_blob,0,strlen(VRF_data.block_blob));

    // create the block template and send it to all block verifiers if the block verifier is the block producer
    if ((strncmp(current_round_part_backup_node,"0",1) == 0 && strncmp(main_nodes_list.block_producer_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (strncmp(current_round_part_backup_node,"1",1) == 0 && strncmp(main_nodes_list.block_producer_backup_block_verifier_1_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (strncmp(current_round_part_backup_node,"2",1) == 0 && strncmp(main_nodes_list.block_producer_backup_block_verifier_2_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (strncmp(current_round_part_backup_node,"3",1) == 0 && strncmp(main_nodes_list.block_producer_backup_block_verifier_3_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (strncmp(current_round_part_backup_node,"4",1) == 0 && strncmp(main_nodes_list.block_producer_backup_block_verifier_4_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (strncmp(current_round_part_backup_node,"5",1) == 0 && strncmp(main_nodes_list.block_producer_backup_block_verifier_5_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0))
    {
      if (get_block_template(VRF_data.block_blob) == 0)
      {
        RESTART_ROUND("Could not get a block template");
      }  

      // create the message
      memset(data,0,sizeof(data));
      memcpy(data,"{\r\n \"message_settings\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND_CREATE_NEW_BLOCK\",\r\n \"block_blob\": \"",97);
      memcpy(data+97,VRF_data.block_blob,strnlen(VRF_data.block_blob,BUFFER_SIZE));
      memcpy(data+strlen(data),"\",\r\n}",5);

      // sign_data
      if (sign_data(data) == 0)
      { 
        RESTART_ROUND("Could not sign_data");
      }

      // send the message to all block verifiers
      if (block_verifiers_send_data_socket((const char*)data) == 0)
      {
        RESTART_ROUND("Could not send data to the block verifiers");
      }
    }
    
    // wait for the block verifiers to process the votes
    strncmp(current_round_part_backup_node,"0",1) == 0 ? sync_block_verifiers_minutes_and_seconds(2,5) : sync_block_verifiers_minutes_and_seconds(3,25);



    // at this point all block verifiers should have the same VRF data and the network block
    fprintf(stderr,"\033[1;33mPart 3 - Create reserve bytes data for the block using the selected VRF data and sign the block\033[0m\n\n");

    // check if the network block string was created from the correct block verifier
    if (strncmp(VRF_data.block_blob,"",1) == 0)
    {
      RESTART_ROUND("Could not receive the network block string from the block producer");
    }

    // create the block verifiers block signature
    if (block_verifiers_create_block_signature(data) == 0 || sign_data(data) == 0)
    {
      RESTART_ROUND("Could not sign_data");
    }

    // send the message to all block verifiers
    if (block_verifiers_send_data_socket((const char*)data) == 0)
    {
      RESTART_ROUND("Could not send data to the block verifiers");
    }

    // wait for the block verifiers to process the votes
    strncmp(current_round_part_backup_node,"0",1) == 0 ? sync_block_verifiers_minutes_and_seconds(2,20) : sync_block_verifiers_minutes_and_seconds(3,40);



    // at this point all block verifiers should have the same VRF data, network block string and all block verifiers signed data

    color_print("Part 4 - Verify that a valid amount of block verifiers have signed the block and have the same created data and block","yellow");

    // process the data and add the block verifiers signatures to the block
    for (count = 0, count2 = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (strlen(VRF_data.block_blob_signature[count]) == VRF_PROOF_LENGTH+VRF_BETA_LENGTH)
      {
        memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count],VRF_data.block_blob_signature[count],VRF_PROOF_LENGTH+VRF_BETA_LENGTH);
        count2++;
      }
      else
      {
        memcpy(VRF_data.block_blob_signature[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE)-1);
        memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE)-1);
      }
    }

    // check if the network block string has at least 67 of the block verifiers network block signature
    if (count2 >= BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      fprintf(stderr,"\033[1;32m%zu / %d block verifiers have signed the block\033[0m\n",count2,BLOCK_VERIFIERS_VALID_AMOUNT);     
    }
    else if (count2 == 1)
    {
      // restart the delegate, as it could only verify its own message and not anyone elses message
      color_print("Restarting, could not process any other block verifiers data","red");
      exit(0);
    }
    else
    {
      fprintf(stderr,"\033[1;31m%zu / %d block verifiers have signed the block\033[0m\n",count2,BLOCK_VERIFIERS_VALID_AMOUNT);
      RESTART_ROUND("An invalid amount of block verifiers have signed the block");
    }

    // create the vote results
    if (block_verifiers_create_vote_results(data) == 0)
    {
      RESTART_ROUND("Could not create the vote results");
    }

    // sign_data
    if (sign_data(data) == 0)
    { 
      RESTART_ROUND("Could not sign_data");
    }    

    // wait for the block verifiers to process the votes
    strncmp(current_round_part_backup_node,"0",1) == 0 ? sync_block_verifiers_minutes_and_seconds(2,30) : sync_block_verifiers_minutes_and_seconds(3,50);

    // send the message to all block verifiers
    if (block_verifiers_send_data_socket((const char*)data) == 0)
    {
      RESTART_ROUND("Could not send data to the block verifiers");
    }

    // wait for the block verifiers to process the votes
    strncmp(current_round_part_backup_node,"0",1) == 0 ? sync_block_verifiers_minutes_and_seconds(2,45) : sync_block_verifiers_minutes_and_seconds((BLOCK_TIME-1),5);

    // process the vote results
    if (current_round_part_vote_data.vote_results_valid >= BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      fprintf(stderr,"\033[1;32m%d / %d block verifiers have the same created data and block\033[0m\n\n",current_round_part_vote_data.vote_results_valid,BLOCK_VERIFIERS_VALID_AMOUNT);    
    }
    else if (current_round_part_vote_data.vote_results_valid == 1)
    {
      // restart the delegate, as it could only verify its own message and not anyone elses message
      color_print("Restarting, could not process any other block verifiers data","red");
      exit(0);
    }
    else
    {
      fprintf(stderr,"\033[1;31m%d / %d block verifiers have the same created data and block\033[0m\n\n",current_round_part_vote_data.vote_results_valid,BLOCK_VERIFIERS_VALID_AMOUNT);
      RESTART_ROUND("An invalid amount of block verifiers have the same created data and block");
    }



    // at this point all block verifiers have the same network block string with all of the VRF data

    color_print("Part 5 - Wait for block producer to submit block and update databases","yellow");

    // update the database and submit the block to the network
    if (block_verifiers_create_block_and_update_database() == 0)
    {
      return 0;
    }    
    return 1;
    
    #undef RESTART_ROUND
}

/*
-----------------------------------------------------------------------------------------------------------
Name: sync_block_verifiers_minutes_and_seconds
Description: Syncs the block verifiers to a specific minute and second
Parameters:
  minutes - The minutes
  seconds - The seconds
-----------------------------------------------------------------------------------------------------------
*/

void sync_block_verifiers_minutes_and_seconds(const int MINUTES, const int SECONDS)
{
  // Variables
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;

  do
  {
    nanosleep((const struct timespec[]){{0, 200000000L}}, NULL);
    time(&current_date_and_time);
    gmtime_r(&current_date_and_time,&current_UTC_date_and_time);
  } while (current_UTC_date_and_time.tm_min % BLOCK_TIME != MINUTES || current_UTC_date_and_time.tm_sec != SECONDS);

  return;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_network_data_nodes_online_status
Description: Get all of the network data nodes online status
Return: 0 if no network data nodes are online, 1 if there is at least one network data node online
-----------------------------------------------------------------------------------------------------------
*/

int get_network_data_nodes_online_status(void)
{
  // Variables
  char data[SMALL_BUFFER_SIZE];
  char data2[SMALL_BUFFER_SIZE];
  int count;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  
  // create the message
  memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_NETWORK_DATA_NODE_BLOCK_VERIFIERS_CURRENT_TIME\",\r\n}",95);

  // sign_data
  if (sign_data(data) == 0)
  { 
    return 0;
  }

  for (count = 0; count < NETWORK_DATA_NODES_AMOUNT; count++)
  {
    if (send_and_receive_data_socket(data2,sizeof(data2),network_data_nodes_list.network_data_nodes_IP_address[count],SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1)
    {
      return 1;
    }
  }
  return 0;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: block_verifiers_send_data_socket
Description: Sends the message to all of the block verifiers
Parameters:
  MESSAGE - The message that the block verifier will send to the other block verifiers
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int block_verifiers_send_data_socket(const char* MESSAGE)
{
  // Constants
  const int TOTAL_BLOCK_VERIFIERS = test_settings == 0 ? BLOCK_VERIFIERS_AMOUNT : BLOCK_VERIFIERS_TOTAL_AMOUNT; 

  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int epoll_fd_copy;
  struct epoll_event events[TOTAL_BLOCK_VERIFIERS];
  struct timeval SOCKET_TIMEOUT = {SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS, 0};   
  struct block_verifiers_send_data_socket block_verifiers_send_data_socket[TOTAL_BLOCK_VERIFIERS];
  size_t total;
  size_t sent;
  long long int bytes = 1;
  int count;
  int count2;
  int number;

  // define macros
  #define BLOCK_VERIFIERS_SEND_DATA_SOCKET(message) \
  memcpy(error_message.function[error_message.total],"block_verifiers_send_data_socket",32); \
  memcpy(error_message.data[error_message.total],message,strnlen(message,BUFFER_SIZE)); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  // create the message
  memcpy(data,MESSAGE,strnlen(MESSAGE,sizeof(data)));
  memcpy(data+strlen(data),SOCKET_END_STRING,sizeof(SOCKET_END_STRING)-1);
  total = strnlen(data,BUFFER_SIZE);
  
  // create the epoll file descriptor
  if ((epoll_fd_copy = epoll_create1(0)) == -1)
  {
    BLOCK_VERIFIERS_SEND_DATA_SOCKET("Error creating the epoll file descriptor");
  }

  // convert the port to a string
  snprintf(data2,sizeof(data2)-1,"%d",SEND_DATA_PORT);
  
  for (count = 0; count < TOTAL_BLOCK_VERIFIERS; count++)
  {    
    if (strncmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
    {
      // Variables
      struct addrinfo serv_addr;
      struct addrinfo* settings = NULL;

      // initialize the block_verifiers_send_data_socket struct
      memset(block_verifiers_send_data_socket[count].IP_address,0,sizeof(block_verifiers_send_data_socket[count].IP_address));
      memcpy(block_verifiers_send_data_socket[count].IP_address,current_block_verifiers_list.block_verifiers_IP_address[count],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count],sizeof(block_verifiers_send_data_socket[count].IP_address)));
      block_verifiers_send_data_socket[count].settings = 0;

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
      if (setsockopt(block_verifiers_send_data_socket[count].socket, SOL_SOCKET, SO_SNDTIMEO,&SOCKET_TIMEOUT, sizeof(struct timeval)) != 0)
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

  // wait for all of the sockets to connect. Wait for 3 seconds instead of 1 since servers time can be synced up to 3 seconds
  sleep(BLOCK_VERIFIERS_SETTINGS);

  // get the total amount of sockets that are ready
  number = epoll_wait(epoll_fd_copy, events, TOTAL_BLOCK_VERIFIERS, 0);

  for (count = 0; count < number; count++)
  {
    // check that the socket is connected
    if (events[count].events & EPOLLIN || events[count].events & EPOLLOUT)
    {
      // set the settings of the delegate to 1
      for (count2 = 0; count2 < TOTAL_BLOCK_VERIFIERS; count2++)
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

  for (count = 0; count < TOTAL_BLOCK_VERIFIERS; count++)
  {
    if (block_verifiers_send_data_socket[count].settings == 1)
    {
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
      
      for (sent = 0; sent < total; sent += bytes == -1 ? 0 : bytes)
      {
        if ((bytes = send(block_verifiers_send_data_socket[count].socket,data+sent,total-sent,MSG_NOSIGNAL)) == -1 && errno != EAGAIN && errno != EWOULDBLOCK)
        {           
          break;
        }
      }
    }    
  }

  // wait for all of the data to be sent to the connected sockets
  sleep(8);

  // remove all of the sockets from the epoll file descriptor and close all of the sockets
  for (count = 0; count < TOTAL_BLOCK_VERIFIERS; count++)
  {
    epoll_ctl(epoll_fd_copy, EPOLL_CTL_DEL, block_verifiers_send_data_socket[count].socket, &events[count]);
    close(block_verifiers_send_data_socket[count].socket);
  }
  return 1;
  
  #undef BLOCK_VERIFIERS_SEND_DATA_SOCKET
}