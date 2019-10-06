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

#include "blockchain_functions.h"
#include "block_verifiers_functions.h"
#include "block_verifiers_synchronize_server_functions.h"
#include "block_verifiers_synchronize_functions.h"
#include "database_functions.h"
#include "file_functions.h"
#include "network_daemon_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "network_wallet_functions.h"
#include "delegates_website_and_shared_delegates_website_functions.h"
#include "organize_functions.h"
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
Name: start_new_round
Description: Checks if the round is a start block round or not
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int start_new_round(void)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm* current_UTC_date_and_time;
  size_t count;
  int settings;
  int settings2;

  // define macros
  #define START_NEW_ROUND_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"start_new_round",15); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  #define RESTART_XCASH_DAEMON(settings) \
  if (settings == 0) \
  { \
    system("systemctl stop XCASH_Daemon_Block_Verifier"); \
    sleep(10); \
    system("systemctl start XCASH_Daemon"); \
  } \
  else if (settings == 1) \
  { \
    system("systemctl stop XCASH_Daemon"); \
    sleep(10); \
    system("systemctl start XCASH_Daemon_Block_Verifier"); \
  } \

  #define RESET_VARIABLES \
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++) \
  { \
    memset(VRF_data.block_verifiers_vrf_secret_key_data[count],0,strlen(VRF_data.block_verifiers_vrf_secret_key_data[count])); \
    memset(VRF_data.block_verifiers_vrf_secret_key[count],0,strlen((const char*)VRF_data.block_verifiers_vrf_secret_key[count])); \
    memset(VRF_data.block_verifiers_vrf_public_key_data[count],0,strlen(VRF_data.block_verifiers_vrf_public_key_data[count])); \
    memset(VRF_data.block_verifiers_vrf_public_key[count],0,strlen((const char*)VRF_data.block_verifiers_vrf_public_key[count])); \
    memset(VRF_data.block_verifiers_random_data[count],0,strlen(VRF_data.block_verifiers_random_data[count])); \
    memset(VRF_data.block_blob_signature[count],0,strlen(VRF_data.block_blob_signature[count])); \
    memset(invalid_reserve_proofs.block_verifier_public_address[count],0,strlen(invalid_reserve_proofs.block_verifier_public_address[count])); \
    memset(invalid_reserve_proofs.public_address[count],0,strlen(invalid_reserve_proofs.public_address[count])); \
    memset(invalid_reserve_proofs.reserve_proof[count],0,strlen(invalid_reserve_proofs.reserve_proof[count])); \
  } \
  memset(VRF_data.vrf_secret_key_data_round_part_4,0,strlen(VRF_data.vrf_secret_key_data_round_part_4)); \
  memset(VRF_data.vrf_secret_key_round_part_4,0,strlen((const char*)VRF_data.vrf_secret_key_round_part_4)); \
  memset(VRF_data.vrf_public_key_data_round_part_4,0,strlen(VRF_data.vrf_public_key_data_round_part_4)); \
  memset(VRF_data.vrf_public_key_round_part_4,0,strlen((const char*)VRF_data.vrf_public_key_round_part_4)); \
  memset(VRF_data.vrf_alpha_string_data_round_part_4,0,strlen(VRF_data.vrf_alpha_string_data_round_part_4)); \
  memset(VRF_data.vrf_alpha_string_round_part_4,0,strlen((const char*)VRF_data.vrf_alpha_string_round_part_4)); \
  memset(VRF_data.vrf_proof_data_round_part_4,0,strlen(VRF_data.vrf_proof_data_round_part_4)); \
  memset(VRF_data.vrf_proof_round_part_4,0,strlen((const char*)VRF_data.vrf_proof_round_part_4)); \
  memset(VRF_data.vrf_beta_string_data_round_part_4,0,strlen(VRF_data.vrf_beta_string_data_round_part_4)); \
  memset(VRF_data.vrf_beta_string_round_part_4,0,strlen((const char*)VRF_data.vrf_beta_string_round_part_4)); \
  memset(VRF_data.reserve_bytes_data_hash,0,strlen(VRF_data.reserve_bytes_data_hash)); \
  memset(VRF_data.block_blob,0,strlen(VRF_data.block_blob));

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // start a new round
  if (get_current_block_height(current_block_height,0) == 0)
  {
    START_NEW_ROUND_ERROR("Could not get the current block height");
  }
  memcpy(data,"A new round is starting for block ",34);
  memcpy(data+34,current_block_height,strnlen(current_block_height,BUFFER_SIZE));
  print_start_message(current_date_and_time,current_UTC_date_and_time,data,data2);

  // get the delegates online status
  get_delegates_online_status();

  /*// check if all of the databases are synced
  if (check_if_databases_are_synced(settings) == 0)
  {
    START_NEW_ROUND_ERROR("Could not check if the database is synced. Your block verifier will now sit out for the remainder of the round");
  }
  */

  /*// check if the block verifier is a current block verifier
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0)
    {
      break;
    }
  }

  pthread_rwlock_rdlock(&rwlock);
  if (count == BLOCK_VERIFIERS_AMOUNT)
  {
    // check to see if the X-CASH daemon needs to be restarted
    if (current_block_verifier_settings == 1)
    {
      current_block_verifier_settings = 0;
      RESTART_XCASH_DAEMON(0);
    }
    return 1;
  }
  else
  {
    // check to see if the X-CASH daemon needs to be restarted
    if (current_block_verifier_settings == 0)
    {
      current_block_verifier_settings = 1;
      RESTART_XCASH_DAEMON(1);
    }
  }  
  pthread_rwlock_unlock(&rwlock);

  if (count == BLOCK_VERIFIERS_AMOUNT)
  {
    START_NEW_ROUND_ERROR("Your delegate is not a block verifier. Your delegate will now sit out for the remainder of the round");
  }*/


  // update the previous, current and next block verifiers at the begining of the round, so a restart round does not affect the previous, current and next block verifiers
  settings = update_block_verifiers_list();
  if (settings == 0)
  {
    START_NEW_ROUND_ERROR("Could not update the previous, current and next block verifiers list");
  }

  // check if the current block height - 1 is a X-CASH proof of stake block since this will check to see if these are the first three blocks on the network
  sscanf(current_block_height,"%zu", &count);
  count = count - 1;
  memset(data,0,sizeof(data));
  snprintf(data,sizeof(data)-1,"%zu",count);
  settings = get_block_settings(data,0);
  sscanf(current_block_height,"%zu", &count);
  count = count - 2;
  memset(data,0,sizeof(data));
  snprintf(data,sizeof(data)-1,"%zu",count);
  settings2 = get_block_settings(data,0);
  if (settings == 0)
  {    
    START_NEW_ROUND_ERROR("Could not get a previous blocks settings. Your block verifier will now sit out for the remainder of the round");
  }
  else if (settings == 1 && settings2 == 1)
  {
    // this is the first block of the network
    color_print("The current block is the first block on the network, meaning the the main network node will create this block","yellow");

    RESET_VARIABLES;

    pthread_rwlock_wrlock(&rwlock);
    // set the main_network_data_node_create_block so the main network data node can create the block
    main_network_data_node_create_block = 1;
    pthread_rwlock_unlock(&rwlock);
    if (start_current_round_start_blocks() == 0)
    {      
      START_NEW_ROUND_ERROR("start_current_round_start_blocks error");
    } 
  }
  else if (settings == 2)
  {
    // this is a X-CASH proof of stake block so this is not the start blocks of the network
    if (settings2 == 2 && memcmp(VRF_data.block_blob,"",1) != 0)
    {
      // update all of the databases 
      color_print("Updating the previous rounds data in the databases","blue");
      if (update_databases() == 0)
      {  
        START_NEW_ROUND_ERROR("Could not update the databases for the previous round");
      }
    }

    RESET_VARIABLES

    pthread_rwlock_wrlock(&rwlock);
    // reset the current_round_part and current_round_part_backup_node
    memset(current_round_part,0,sizeof(current_round_part));
    memcpy(current_round_part,"1",1);
    memset(current_round_part_backup_node,0,sizeof(current_round_part_backup_node));
    memcpy(current_round_part_backup_node,"0",1);
    pthread_rwlock_unlock(&rwlock);

    if (calculate_main_nodes_roles() == 0)
    {
      print_error_message(current_date_and_time,current_UTC_date_and_time,data);
      
      pthread_rwlock_wrlock(&rwlock);
      // set the main_network_data_node_create_block so the main network data node can create the block
      main_network_data_node_create_block = 1;
      pthread_rwlock_unlock(&rwlock);
      if (data_network_node_create_block() == 0)
      {      
        START_NEW_ROUND_ERROR("data_network_node_create_block error");
      } 
    }
    if (start_part_4_of_round() == 0)
    {
      print_error_message(current_date_and_time,current_UTC_date_and_time,data);
      color_print("Your block verifier will wait until the next round","red");
      fprintf(stderr,"\n");
    }
  }
  return 2;

  #undef START_NEW_ROUND_ERROR
  #undef RESTART_XCASH_DAEMON
  #undef RESET_VARIABLES
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
  char data3[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm* current_UTC_date_and_time;
  struct send_and_receive_data_socket_thread_parameters send_and_receive_data_socket_thread_parameters[BLOCK_VERIFIERS_AMOUNT];
  size_t count; 
  size_t count2; 

  // threads
  pthread_t thread_id[BLOCK_VERIFIERS_AMOUNT];

  // define macros
  #define DATABASE_COLLECTION "reserve_bytes_1"

  #define pointer_reset_all \
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++) \
  { \
    free(send_and_receive_data_socket_thread_parameters[count].DATA); \
    send_and_receive_data_socket_thread_parameters[count].DATA = NULL; \
  }

  #define START_CURRENT_ROUND_START_BLOCKS_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"start_current_round_start_blocks",32); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // initialize the send_and_receive_data_socket_thread_parameters struct
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    send_and_receive_data_socket_thread_parameters[count].DATA = (char*)calloc(BUFFER_SIZE,sizeof(char));

    // check if the memory needed was allocated on the heap successfully
    if (send_and_receive_data_socket_thread_parameters[count].DATA == NULL)
    {
      memcpy(error_message.function[error_message.total],"start_current_round_start_blocks",32);
      memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
      error_message.total++;
      print_error_message(current_date_and_time,current_UTC_date_and_time,data);  
      exit(0);
    }
  }
  
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  // set the main_network_data_node_create_block so the main network data node can create the block
  pthread_rwlock_wrlock(&rwlock);
  main_network_data_node_create_block = 1;
  pthread_rwlock_unlock(&rwlock);

  // wait for all block verifiers to sync the database
  color_print("Waiting for all block verifiers to sync the databases","blue");
  fprintf(stderr,"\n");
  sync_block_verifiers_minutes(current_date_and_time,current_UTC_date_and_time,1);

  // check if the block verifier is the main network data node
  if (memcmp(xcash_wallet_public_address,network_data_nodes_list.network_data_nodes_public_address[0],XCASH_WALLET_LENGTH) != 0)
  {
    color_print("Your block verifier is not the main data network node so your block verifier will sit out for the remainder of the round","yellow");
    fprintf(stderr,"\n");
    sync_block_verifiers_minutes_and_seconds(current_date_and_time,current_UTC_date_and_time,4,50);
    return 1;
  } 

  color_print("Your block verifier is the main data network node so your block verifier will create the block","yellow");
  fprintf(stderr,"\n");

  // get a block template
  if (get_block_template(data,0) == 0)
  {
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not get a block template");
  }

  // convert the network_block_string to blockchain_data
  if (network_block_string_to_blockchain_data((const char*)data,"0") == 0)
  {
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not convert the network_block_string to blockchain_data");
  }

  // change the nonce to the CONSENSUS_NODE_NETWORK_BLOCK_NONCE
  memcpy(blockchain_data.nonce_data,CONSENSUS_NODE_NETWORK_BLOCK_NONCE,8);

  // add the delegates data to the network_block_string
  memset(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name,BUFFER_SIZE));
  memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name,"network_data_node_1",19);
  memset(blockchain_data.blockchain_reserve_bytes.block_producer_public_address,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_public_address,BUFFER_SIZE));
  memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_public_address,NETWORK_DATA_NODE_1_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH);
  memset(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,BUFFER_SIZE));
  memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,"0",1);
  memset(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names,BUFFER_SIZE));
  memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names,"network_data_node_1,network_data_node_1,network_data_node_1,network_data_node_1,network_data_node_1",99);

  // create the VRF data
  if (create_random_VRF_keys(VRF_data.vrf_public_key_round_part_4,VRF_data.vrf_secret_key_round_part_4) == 1 && crypto_vrf_is_valid_key((const unsigned char*)VRF_data.vrf_public_key_round_part_4) != 1)
  {
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not create the vrf_public_key_round_part_4 or vrf_secret_key_round_part_4");
  }
  if (get_previous_block_hash(blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,0) == 0)
  {
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not get the previous block hash");
  }  

  memcpy(blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,blockchain_data.previous_block_hash_data,BLOCK_HASH_LENGTH);
  memcpy(VRF_data.vrf_alpha_string_round_part_4,blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,BLOCK_HASH_LENGTH);

  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memcpy(VRF_data.vrf_alpha_string_round_part_4+strlen((const char*)VRF_data.vrf_alpha_string_round_part_4),GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING)-1);
  }   

  // convert the vrf alpha string to a string
  for (count2 = 0, count = 0; count2 < (((RANDOM_STRING_LENGTH*2)*BLOCK_VERIFIERS_AMOUNT) + (BLOCK_HASH_LENGTH*2)) / 2; count2++, count += 2)
  {
    snprintf(VRF_data.vrf_alpha_string_data_round_part_4+count,BUFFER_SIZE-1,"%02x",VRF_data.vrf_alpha_string_round_part_4[count2] & 0xFF);
  }

  if (crypto_vrf_prove(VRF_data.vrf_proof_round_part_4,(const unsigned char*)VRF_data.vrf_secret_key_round_part_4,(const unsigned char*)VRF_data.vrf_alpha_string_data_round_part_4,(unsigned long long)strlen((const char*)VRF_data.vrf_alpha_string_data_round_part_4)) != 0)
  {
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not create the vrf proof");
  }
  if (crypto_vrf_proof_to_hash(VRF_data.vrf_beta_string_round_part_4,(const unsigned char*)VRF_data.vrf_proof_round_part_4) != 0)
  {
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not create the vrf beta string");
  }
  if (crypto_vrf_verify(VRF_data.vrf_beta_string_round_part_4,(const unsigned char*)VRF_data.vrf_public_key_round_part_4,(const unsigned char*)VRF_data.vrf_proof_round_part_4,(const unsigned char*)VRF_data.vrf_alpha_string_data_round_part_4,(unsigned long long)strlen((const char*)VRF_data.vrf_alpha_string_data_round_part_4)) != 0)
  {
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not create the VRF data");
  }

  // convert all of the VRF data to a string
  for (count2 = 0, count = 0; count2 < crypto_vrf_SECRETKEYBYTES; count2++, count += 2)
  {
    snprintf(VRF_data.vrf_secret_key_data_round_part_4+count,BUFFER_SIZE_NETWORK_BLOCK_DATA-1,"%02x",VRF_data.vrf_secret_key_round_part_4[count2] & 0xFF);
  }
  for (count2 = 0, count = 0; count2 < crypto_vrf_PUBLICKEYBYTES; count2++, count += 2)
  {
    snprintf(VRF_data.vrf_public_key_data_round_part_4+count,BUFFER_SIZE_NETWORK_BLOCK_DATA-1,"%02x",VRF_data.vrf_public_key_round_part_4[count2] & 0xFF);
  }
  for (count2 = 0, count = 0; count2 < crypto_vrf_PROOFBYTES; count2++, count += 2)
  {
    snprintf(VRF_data.vrf_proof_data_round_part_4+count,BUFFER_SIZE_NETWORK_BLOCK_DATA-1,"%02x",VRF_data.vrf_proof_round_part_4[count2] & 0xFF);
  }
  for (count2 = 0, count = 0; count2 < crypto_vrf_OUTPUTBYTES; count2++, count += 2)
  {
    snprintf(VRF_data.vrf_beta_string_data_round_part_4+count,BUFFER_SIZE_NETWORK_BLOCK_DATA-1,"%02x",VRF_data.vrf_beta_string_round_part_4[count2] & 0xFF);
  }  

  // add all of the VRF data to the blockchain_data struct
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_secret_key_round_part_4,VRF_data.vrf_secret_key_round_part_4,crypto_vrf_SECRETKEYBYTES);
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_secret_key_data_round_part_4,VRF_data.vrf_secret_key_data_round_part_4,VRF_SECRET_KEY_LENGTH);
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_4,VRF_data.vrf_public_key_round_part_4,crypto_vrf_PUBLICKEYBYTES);
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_4,VRF_data.vrf_public_key_data_round_part_4,VRF_PUBLIC_KEY_LENGTH);
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_4,VRF_data.vrf_alpha_string_round_part_4,strnlen((const char*)VRF_data.vrf_alpha_string_round_part_4,BUFFER_SIZE));
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_4,VRF_data.vrf_alpha_string_data_round_part_4,strnlen(VRF_data.vrf_alpha_string_data_round_part_4,BUFFER_SIZE));
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_4,VRF_data.vrf_proof_round_part_4,crypto_vrf_PROOFBYTES);
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_4,VRF_data.vrf_proof_data_round_part_4,VRF_PROOF_LENGTH);
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_4,VRF_data.vrf_beta_string_round_part_4,crypto_vrf_OUTPUTBYTES);
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_4,VRF_data.vrf_beta_string_data_round_part_4,VRF_BETA_LENGTH);

  memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key[0],blockchain_data.blockchain_reserve_bytes.vrf_secret_key_round_part_4,crypto_vrf_SECRETKEYBYTES);
  memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key_data[0],blockchain_data.blockchain_reserve_bytes.vrf_secret_key_data_round_part_4,VRF_SECRET_KEY_LENGTH);
  memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key[0],blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_4,crypto_vrf_PUBLICKEYBYTES);
  memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key_data[0],blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_4,VRF_PUBLIC_KEY_LENGTH);
  memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data[0],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING)-1);
  
  for (count = 1; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key_data[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_SECRET_KEY_DATA,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_SECRET_KEY_DATA)-1);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key_data[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_PUBLIC_KEY_DATA,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_PUBLIC_KEY_DATA)-1);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING)-1);
  }

  // add the next block verifiers
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  { 
    memcpy(blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address[count],next_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH);
  }

  // add 0's for the block validation nodes signature, except for the first block validation node signature
  for (count = 1; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE_DATA,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE_DATA)-1);
  }
  
  // convert the blockchain_data to a network_block_string
  memset(data,0,sizeof(data));
  if (blockchain_data_to_network_block_string(data) == 0)
  {
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not convert the blockchain_data to a network_block_string");
  }

  // sign the network block string
  if (sign_network_block_string(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[0],data,0) == 0)
  {
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not sign the network block string");
  }

  // convert the blockchain_data to a network_block_string
  memset(VRF_data.block_blob,0,strlen(VRF_data.block_blob));
  if (blockchain_data_to_network_block_string(VRF_data.block_blob) == 0)
  {
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not convert the blockchain_data to a network_block_string");
  }

  // add the data hash to the network block string
  memset(data,0,sizeof(data));
  if (add_data_hash_to_network_block_string(VRF_data.block_blob,data) == 0)
  {
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not add the network block string data hash");
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

  pthread_rwlock_rdlock(&rwlock);
  while(database_settings != 1)
  {
    sleep(1);
  }
  pthread_rwlock_unlock(&rwlock);

  // add the network block string to the database
  if (insert_document_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,data2,0) == 0)
  {
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not add the new block to the database");
  }

  // create the verify_block file for the X-CASH Daemon
  if (write_file(VRF_data.reserve_bytes_data_hash,verify_block_file) == 0)
  {
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not create the verify_block file");
  }

  // create the message
  memset(data3,0,sizeof(data3));
  memcpy(data3,"{\r\n \"message_settings\": \"MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_START_BLOCK\",\r\n \"database_data\": \"",101);
  memcpy(data3+101,data2,strnlen(data2,sizeof(data3)));
  memcpy(data3+strlen(data3),"\",\r\n \"reserve_bytes_data_hash\": \"",33);
  memcpy(data3+strlen(data3),VRF_data.reserve_bytes_data_hash,DATA_HASH_LENGTH);
  memcpy(data3+strlen(data3),"\",\r\n}",5);
  
  // sign_data
  if (sign_data(data3,0) == 0)
  { 
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not sign_data");
  }

  // send the database data to all block verifiers
  sleep(BLOCK_VERIFIERS_SETTINGS);
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
    {    
      memset(send_and_receive_data_socket_thread_parameters[count].HOST,0,sizeof(send_and_receive_data_socket_thread_parameters[count].HOST));
      memset(send_and_receive_data_socket_thread_parameters[count].DATA,0,strlen(send_and_receive_data_socket_thread_parameters[count].DATA));
      memcpy(send_and_receive_data_socket_thread_parameters[count].HOST,current_block_verifiers_list.block_verifiers_IP_address[count],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count],sizeof(send_and_receive_data_socket_thread_parameters[count].HOST)));
      memcpy(send_and_receive_data_socket_thread_parameters[count].DATA,data3,strnlen(data3,BUFFER_SIZE));
      send_and_receive_data_socket_thread_parameters[count].COUNT = count;
      pthread_create(&thread_id[count], NULL, &send_and_receive_data_socket_thread,&send_and_receive_data_socket_thread_parameters[count]);
      pthread_detach(thread_id[count]);
    }
    if (count % (BLOCK_VERIFIERS_AMOUNT / 4) == 0 && count != 0 && count != BLOCK_VERIFIERS_AMOUNT)
    {
      usleep(500000);
    }
  }

  color_print("Waiting for the block producer to submit the block to the network","blue");
  fprintf(stderr,"\n");
  sync_block_verifiers_minutes_and_seconds(current_date_and_time,current_UTC_date_and_time,4,50);

  // have the main network data node submit the block to the network  
  if (submit_block_template(data,0) == 0)
  {
    memset(data2,0,sizeof(data2));
    memcpy(data2,"{\"block_height\":\"",17);
    memcpy(data2+17,current_block_height,strnlen(current_block_height,sizeof(data2)));
    memcpy(data2+strlen(data2),"\"}",2);
    delete_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,data2,0);
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not submit the block to the network");
  }
  
  return 1;

  #undef pointer_reset_all  
  #undef DATABASE_COLLECTION
  #undef START_CURRENT_ROUND_START_BLOCKS_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: data_network_node_create_block
Description: Runs the round where the main network data node will create the block and have the block verifiers sign the block. This only runs if the block producer and the 5 backup block producers cant create the block
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int data_network_node_create_block(void)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm* current_UTC_date_and_time;
  struct send_and_receive_data_socket_thread_parameters send_and_receive_data_socket_thread_parameters[BLOCK_VERIFIERS_AMOUNT];
  size_t count; 
  size_t count2; 

  // threads
  pthread_t thread_id[BLOCK_VERIFIERS_AMOUNT];

  // define macros
  #define pointer_reset_all \
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++) \
  { \
    free(send_and_receive_data_socket_thread_parameters[count].DATA); \
    send_and_receive_data_socket_thread_parameters[count].DATA = NULL; \
  }

  #define DATA_NETWORK_NODE_CREATE_BLOCK_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"start_current_round_start_blocks",32); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // initialize the send_and_receive_data_socket_thread_parameters struct
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    send_and_receive_data_socket_thread_parameters[count].DATA = (char*)calloc(BUFFER_SIZE,sizeof(char));

    // check if the memory needed was allocated on the heap successfully
    if (send_and_receive_data_socket_thread_parameters[count].DATA == NULL)
    {
      memcpy(error_message.function[error_message.total],"data_network_node_create_block",30);
      memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
      error_message.total++;
      print_error_message(current_date_and_time,current_UTC_date_and_time,data);  
      exit(0);
    }
  }

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  memcpy(data,"The block producer and all backup block producers have not been able to create the network block\nThe main network data node will now create the network block",157);
  print_start_message(current_date_and_time,current_UTC_date_and_time,data,data2);

  // set the main_network_data_node_create_block so the main network data node can create the block
  pthread_rwlock_wrlock(&rwlock);
  main_network_data_node_create_block = 1;
  pthread_rwlock_unlock(&rwlock);

  // wait for the block verifiers to process the votes
  sync_block_verifiers_minutes(current_date_and_time,current_UTC_date_and_time,4);

  pthread_rwlock_wrlock(&rwlock);
  // set the current_round_part
  memset(current_round_part,0,sizeof(current_round_part));
  memcpy(current_round_part,"1",1);

  // set the current_round_part_backup_node
  memset(current_round_part_backup_node,0,sizeof(current_round_part_backup_node));
  memcpy(current_round_part_backup_node,"2",1);
  pthread_rwlock_unlock(&rwlock);

  // check if the block verifier is the main network data node
  if (memcmp(network_data_nodes_list.network_data_nodes_public_address[0],xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0)
  {    
    color_print("Your block verifier is the main data network node so your block verifier will create the block","yellow");
    fprintf(stderr,"\n");

    // get a block template
    if (get_block_template(data,0) == 0)
    {
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not get a block template");
    }

    // convert the network_block_string to blockchain_data
    if (network_block_string_to_blockchain_data((const char*)data,"0") == 0)
    {
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not convert the network_block_string to blockchain_data");
    }

    // change the nonce to the CONSENSUS_NODE_NETWORK_BLOCK_NONCE
    memcpy(blockchain_data.nonce_data,CONSENSUS_NODE_NETWORK_BLOCK_NONCE,8);

    // add the delegates data to the network_block_string
    memset(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name,BUFFER_SIZE));
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name,"network_data_node_1",19);
    memset(blockchain_data.blockchain_reserve_bytes.block_producer_public_address,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_public_address,BUFFER_SIZE));
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_public_address,NETWORK_DATA_NODE_1_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH);
    memset(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,BUFFER_SIZE));
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,"2",1);
    memset(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names,BUFFER_SIZE));
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names,"network_data_node_1,network_data_node_1,network_data_node_1,network_data_node_1,network_data_node_1",99);

    // create the VRF data
    if (create_random_VRF_keys(VRF_data.vrf_public_key_round_part_4,VRF_data.vrf_secret_key_round_part_4) == 1 && crypto_vrf_is_valid_key((const unsigned char*)VRF_data.vrf_public_key_round_part_4) != 1)
    {
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not create the vrf_public_key_round_part_4 or vrf_secret_key_round_part_4");
    }
    if (get_previous_block_hash(blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,0) == 0)
    {
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not get the previous block hash");
    }  

    memcpy(blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,blockchain_data.previous_block_hash_data,BLOCK_HASH_LENGTH);
    memcpy(VRF_data.vrf_alpha_string_round_part_4,blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,BLOCK_HASH_LENGTH);

    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memcpy(VRF_data.vrf_alpha_string_round_part_4+strlen((const char*)VRF_data.vrf_alpha_string_round_part_4),GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING)-1);
    }   

    // convert the vrf alpha string to a string
    for (count2 = 0, count = 0; count2 < (((RANDOM_STRING_LENGTH*2)*BLOCK_VERIFIERS_AMOUNT) + (BLOCK_HASH_LENGTH*2)) / 2; count2++, count += 2)
    {
      snprintf(VRF_data.vrf_alpha_string_data_round_part_4+count,BUFFER_SIZE-1,"%02x",VRF_data.vrf_alpha_string_round_part_4[count2] & 0xFF);
    }

    if (crypto_vrf_prove(VRF_data.vrf_proof_round_part_4,(const unsigned char*)VRF_data.vrf_secret_key_round_part_4,(const unsigned char*)VRF_data.vrf_alpha_string_data_round_part_4,(unsigned long long)strlen((const char*)VRF_data.vrf_alpha_string_data_round_part_4)) != 0)
    {
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not create the vrf proof");
    }
    if (crypto_vrf_proof_to_hash(VRF_data.vrf_beta_string_round_part_4,(const unsigned char*)VRF_data.vrf_proof_round_part_4) != 0)
    {
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not create the vrf beta string");
    }
    if (crypto_vrf_verify(VRF_data.vrf_beta_string_round_part_4,(const unsigned char*)VRF_data.vrf_public_key_round_part_4,(const unsigned char*)VRF_data.vrf_proof_round_part_4,(const unsigned char*)VRF_data.vrf_alpha_string_data_round_part_4,(unsigned long long)strlen((const char*)VRF_data.vrf_alpha_string_data_round_part_4)) != 0)
    {
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not create the VRF data");
    }

    // convert all of the VRF data to a string
    for (count2 = 0, count = 0; count2 < crypto_vrf_SECRETKEYBYTES; count2++, count += 2)
    {
      snprintf(VRF_data.vrf_secret_key_data_round_part_4+count,BUFFER_SIZE_NETWORK_BLOCK_DATA-1,"%02x",VRF_data.vrf_secret_key_round_part_4[count2] & 0xFF);
    }
    for (count2 = 0, count = 0; count2 < crypto_vrf_PUBLICKEYBYTES; count2++, count += 2)
    {
      snprintf(VRF_data.vrf_public_key_data_round_part_4+count,BUFFER_SIZE_NETWORK_BLOCK_DATA-1,"%02x",VRF_data.vrf_public_key_round_part_4[count2] & 0xFF);
    }
    for (count2 = 0, count = 0; count2 < crypto_vrf_PROOFBYTES; count2++, count += 2)
    {
      snprintf(VRF_data.vrf_proof_data_round_part_4+count,BUFFER_SIZE_NETWORK_BLOCK_DATA-1,"%02x",VRF_data.vrf_proof_round_part_4[count2] & 0xFF);
    }
    for (count2 = 0, count = 0; count2 < crypto_vrf_OUTPUTBYTES; count2++, count += 2)
    {
      snprintf(VRF_data.vrf_beta_string_data_round_part_4+count,BUFFER_SIZE_NETWORK_BLOCK_DATA-1,"%02x",VRF_data.vrf_beta_string_round_part_4[count2] & 0xFF);
    }  

    // add all of the VRF data to the blockchain_data struct
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_secret_key_round_part_4,VRF_data.vrf_secret_key_round_part_4,crypto_vrf_SECRETKEYBYTES);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_secret_key_data_round_part_4,VRF_data.vrf_secret_key_data_round_part_4,VRF_SECRET_KEY_LENGTH);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_4,VRF_data.vrf_public_key_round_part_4,crypto_vrf_PUBLICKEYBYTES);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_4,VRF_data.vrf_public_key_data_round_part_4,VRF_PUBLIC_KEY_LENGTH);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_4,VRF_data.vrf_alpha_string_round_part_4,strnlen((const char*)VRF_data.vrf_alpha_string_round_part_4,BUFFER_SIZE));
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_4,VRF_data.vrf_alpha_string_data_round_part_4,strnlen(VRF_data.vrf_alpha_string_data_round_part_4,BUFFER_SIZE));
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_4,VRF_data.vrf_proof_round_part_4,crypto_vrf_PROOFBYTES);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_4,VRF_data.vrf_proof_data_round_part_4,VRF_PROOF_LENGTH);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_4,VRF_data.vrf_beta_string_round_part_4,crypto_vrf_OUTPUTBYTES);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_4,VRF_data.vrf_beta_string_data_round_part_4,VRF_BETA_LENGTH);

    memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key[0],blockchain_data.blockchain_reserve_bytes.vrf_secret_key_round_part_4,crypto_vrf_SECRETKEYBYTES);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key_data[0],blockchain_data.blockchain_reserve_bytes.vrf_secret_key_data_round_part_4,VRF_SECRET_KEY_LENGTH);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key[0],blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_4,crypto_vrf_PUBLICKEYBYTES);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key_data[0],blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_4,VRF_PUBLIC_KEY_LENGTH);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data[0],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING)-1);
    for (count = 1; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key_data[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_SECRET_KEY_DATA,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_SECRET_KEY_DATA)-1);
      memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key_data[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_PUBLIC_KEY_DATA,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_PUBLIC_KEY_DATA)-1);
      memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING)-1);
    }

    // add the next block verifiers
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    { 
      memcpy(blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address[count],next_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH);
    }

    // convert the blockchain_data to a network_block_string
    if (blockchain_data_to_network_block_string(VRF_data.block_blob) == 0)
    {
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not convert the blockchain_data to a network_block_string");
    }

    // sign the network block string
    memset(data,0,sizeof(data));
    if (sign_network_block_string(data,VRF_data.block_blob,0) == 0)
    {
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not sign the network block string");
    }

    // add the block verifier signature to the VRF data and the blockchain_data struct
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0)
      {
        memcpy(VRF_data.block_blob_signature[count],data,XCASH_SIGN_DATA_LENGTH);
        memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count],data,XCASH_SIGN_DATA_LENGTH);
      }
    }

    // create the message
    memset(data3,0,sizeof(data3));
    memcpy(data3,"{\r\n \"message_settings\": \"MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_CREATE_NEW_BLOCK\",\r\n \"block_blob\": \"",103);
    memcpy(data3+103,VRF_data.block_blob,strnlen(VRF_data.block_blob,BUFFER_SIZE));
    memcpy(data3+strlen(data3),"\",\r\n}",5);
  
    // sign_data
    if (sign_data(data3,0) == 0)
    { 
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not sign_data");
    }

    // send the network block string to all block verifiers and add the block verifier signature to the blockchain data struct
    sleep(BLOCK_VERIFIERS_SETTINGS);
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {      
      if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
      {  
        memset(send_and_receive_data_socket_thread_parameters[count].HOST,0,sizeof(send_and_receive_data_socket_thread_parameters[count].HOST));
        memset(send_and_receive_data_socket_thread_parameters[count].DATA,0,strlen(send_and_receive_data_socket_thread_parameters[count].DATA));
        memcpy(send_and_receive_data_socket_thread_parameters[count].HOST,current_block_verifiers_list.block_verifiers_IP_address[count],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count],sizeof(send_and_receive_data_socket_thread_parameters[count].HOST)));
        memcpy(send_and_receive_data_socket_thread_parameters[count].DATA,data3,strnlen(data3,BUFFER_SIZE));
        send_and_receive_data_socket_thread_parameters[count].COUNT = count;
        pthread_create(&thread_id[count], NULL, &send_and_receive_data_socket_thread,&send_and_receive_data_socket_thread_parameters[count]);
        pthread_detach(thread_id[count]);
      }
      if (count % (BLOCK_VERIFIERS_AMOUNT / 4) == 0 && count != 0 && count != BLOCK_VERIFIERS_AMOUNT)
      {
        usleep(500000);
      }
    }

    sleep(10);

    for (count = 0, count2 = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (memcmp(VRF_data.block_blob_signature[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE)-1) != 0)
      {
        count2++;
      }
    }

    // check if at least 67 of the block verifiers created the data
    if (count2 < BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Invalid amount of block verifiers network block signatures");
    }

    // convert the blockchain_data to a network_block_string
    memset(data,0,sizeof(data));
    if (blockchain_data_to_network_block_string(data) == 0)
    {
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not convert the blockchain_data to a network_block_string");
    }

    // get the previous network block string
    memset(data,0,sizeof(data));
    memset(data2,0,sizeof(data2));
    memset(data3,0,sizeof(data3));
    sscanf(current_block_height,"%zu", &count);
    if (count < XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT-1)
    {
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not get the current block height");
    }
    count--;
    snprintf(data3,sizeof(data3)-1,"%zu",count);
    count2 = ((count - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME) + 1;
    memcpy(data,"{\"block_height\":\"",17);
    memcpy(data+17,data3,strnlen(data3,BUFFER_SIZE));
    memcpy(data+strlen(data),"\"}",2);
    memset(data3,0,strlen(data3));
    memcpy(data3,"reserve_bytes_",14);
    snprintf(data3+14,sizeof(data3)-15,"%zu",count2);
    if (read_document_field_from_collection(DATABASE_NAME,data3,data,"reserve_bytes",data2,0) == 0)
    {
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not get the previous blocks reserve bytes");
    }

    // verify the block
    memset(data3,0,sizeof(data3));
    snprintf(data3,sizeof(data3)-1,"%zu",count);
    if (count+1 != XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT)
    {    
      if (verify_network_block_data(1,1,1,data3,data2) == 0)
      {
        DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("The MAIN_NODES_TO_NODES_PART_4_OF_ROUND message is invalid");
      }
    }
    else
    {
      if (verify_network_block_data(0,1,1,data3,data2) == 0)
      {
        DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("The MAIN_NODES_TO_NODES_PART_4_OF_ROUND message is invalid");
      }
    }

    // convert the blockchain_data to a network_block_string
    memset(data,0,sizeof(data));
    if (blockchain_data_to_network_block_string(data) == 0)
    {
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not convert the blockchain_data to a network_block_string");
    }

    // add the data hash to the network block string
    memset(data,0,sizeof(data));
    if (add_data_hash_to_network_block_string(VRF_data.block_blob,data) == 0)
    {
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not add the network block string data hash");
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

    pthread_rwlock_rdlock(&rwlock);
    while(database_settings != 1)
    {
      sleep(1);
    }
    pthread_rwlock_unlock(&rwlock);

    // add the network block string to the database
    sscanf(current_block_height, "%zu", &count);
    memset(data3,0,sizeof(data3));
    memcpy(data3,"reserve_bytes_",14);
    count2 = ((count - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME) + 1;
    snprintf(data3+14,sizeof(data3)-15,"%zu",count2);
    if (insert_document_into_collection_json(DATABASE_NAME,data3,data2,0) == 0)
    {
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not add the new block to the database");
    }

    memset(data3,0,sizeof(data3));

    // create the verify_block file for the X-CASH Daemon
    if (write_file(VRF_data.reserve_bytes_data_hash,verify_block_file) == 0)
    {
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not create the verify_block file");
    }
    sleep(BLOCK_VERIFIERS_SETTINGS);

    // create the message
    memset(data3,0,sizeof(data3));
    memcpy(data3,"{\r\n \"message_settings\": \"MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_START_BLOCK\",\r\n \"database_data\": \"",101);
    memcpy(data3+101,data2,strnlen(data2,sizeof(data3)));
    memcpy(data3+strlen(data3),"\",\r\n \"reserve_bytes_data_hash\": \"",33);
    memcpy(data3+strlen(data3),VRF_data.reserve_bytes_data_hash,DATA_HASH_LENGTH);
    memcpy(data3+strlen(data3),"\",\r\n}",5);
  
    // sign_data
    if (sign_data(data3,0) == 0)
    { 
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not sign_data");
    }

    // send the database data to all block verifiers
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
      {    
        memset(send_and_receive_data_socket_thread_parameters[count].HOST,0,sizeof(send_and_receive_data_socket_thread_parameters[count].HOST));
        memset(send_and_receive_data_socket_thread_parameters[count].DATA,0,strlen(send_and_receive_data_socket_thread_parameters[count].DATA));
        memcpy(send_and_receive_data_socket_thread_parameters[count].HOST,current_block_verifiers_list.block_verifiers_IP_address[count],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count],sizeof(send_and_receive_data_socket_thread_parameters[count].HOST)));
        memcpy(send_and_receive_data_socket_thread_parameters[count].DATA,data3,strnlen(data3,BUFFER_SIZE));
        send_and_receive_data_socket_thread_parameters[count].COUNT = count;
        pthread_create(&thread_id[count], NULL, &send_and_receive_data_socket_thread,&send_and_receive_data_socket_thread_parameters[count]);
        pthread_detach(thread_id[count]);
      }
      if (count % (BLOCK_VERIFIERS_AMOUNT / 4) == 0 && count != 0 && count != BLOCK_VERIFIERS_AMOUNT)
      {
        usleep(500000);
      }
    }

    color_print("Waiting for the block producer to submit the block to the network","blue");
    fprintf(stderr,"\n");
    // wait for the block verifiers to process the votes
    sync_block_verifiers_minutes_and_seconds(current_date_and_time,current_UTC_date_and_time,4,50);

    // submit the block to the network
    if (submit_block_template(data,0) == 0)
    {
      memset(data2,0,sizeof(data2));
      memcpy(data2,"{\"block_height\":\"",17);
      memcpy(data2+17,current_block_height,strnlen(current_block_height,sizeof(data2)));
      memcpy(data2+strlen(data2),"\"}",2);
      sscanf(current_block_height, "%zu", &count);
      memset(data3,0,sizeof(data3));
      memcpy(data3,"reserve_bytes_",14);
      count2 = ((count - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME) + 1;
      snprintf(data3+14,sizeof(data3)-15,"%zu",count2);
      delete_document_from_collection(DATABASE_NAME,data3,data2,0);
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not submit the block to the network");
    }
  }
  else
  {
    fprintf(stderr,"Your block verifier is not the main data network node so your block verifier will wait until the network data node creates the block\n\n");

    // wait for the block verifiers to process the votes
    sync_block_verifiers_minutes_and_seconds(current_date_and_time,current_UTC_date_and_time,4,50);
  }
  
  return 1;

  #undef pointer_reset_all  
  #undef DATA_NETWORK_NODE_CREATE_BLOCK_ERROR
  #undef SEND_DATA_SOCKET_THREAD
}



/*
-----------------------------------------------------------------------------------------------------------
Name: start_part_4_of_round
Description: Runs the start_part_4_of_round code
Parameters:
  settings - 0 to create the VRF data in the round, otherwise 1
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int start_part_4_of_round(void)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm* current_UTC_date_and_time;
  size_t count = 0;
  size_t count2;
  size_t counter;
  int block_producer_backup_settings[BLOCK_PRODUCERS_BACKUP_AMOUNT] = {0,0,0,0,0};

  // define macros
  #define START_PART_4_OF_ROUND_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"start_part_4_of_round",21); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0; 

  #define RESTART_ROUND(message) \
  fprintf(stderr,"\n"); \
  color_print(message,"red"); \
  memset(data,0,sizeof(data)); \
  memset(data2,0,sizeof(data2)); \
  memset(data3,0,sizeof(data3)); \
  memset(VRF_data.vrf_secret_key_data_round_part_4,0,strlen(VRF_data.vrf_secret_key_data_round_part_4)); \
  memset(VRF_data.vrf_secret_key_round_part_4,0,strlen((const char*)VRF_data.vrf_secret_key_round_part_4)); \
  memset(VRF_data.vrf_public_key_data_round_part_4,0,strlen(VRF_data.vrf_public_key_data_round_part_4)); \
  memset(VRF_data.vrf_public_key_round_part_4,0,strlen((const char*)VRF_data.vrf_public_key_round_part_4)); \
  memset(VRF_data.vrf_alpha_string_data_round_part_4,0,strlen(VRF_data.vrf_alpha_string_data_round_part_4)); \
  memset(VRF_data.vrf_alpha_string_round_part_4,0,strlen((const char*)VRF_data.vrf_alpha_string_round_part_4)); \
  memset(VRF_data.vrf_proof_data_round_part_4,0,strlen(VRF_data.vrf_proof_data_round_part_4)); \
  memset(VRF_data.vrf_proof_round_part_4,0,strlen((const char*)VRF_data.vrf_proof_round_part_4)); \
  memset(VRF_data.vrf_beta_string_data_round_part_4,0,strlen(VRF_data.vrf_beta_string_data_round_part_4)); \
  memset(VRF_data.vrf_beta_string_round_part_4,0,strlen((const char*)VRF_data.vrf_beta_string_round_part_4)); \
  memset(VRF_data.block_blob,0,strlen(VRF_data.block_blob)); \
  memset(VRF_data.reserve_bytes_data_hash,0,strlen(VRF_data.reserve_bytes_data_hash)); \
  memset(current_round_part_vote_data.current_vote_results,0,sizeof(current_round_part_vote_data.current_vote_results)); \
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++) \
  { \
    memset(VRF_data.block_verifiers_vrf_secret_key_data[count],0,strlen(VRF_data.block_verifiers_vrf_secret_key_data[count])); \
    memset(VRF_data.block_verifiers_vrf_secret_key[count],0,strlen((const char*)VRF_data.block_verifiers_vrf_secret_key[count])); \
    memset(VRF_data.block_verifiers_vrf_public_key_data[count],0,strlen(VRF_data.block_verifiers_vrf_public_key_data[count])); \
    memset(VRF_data.block_verifiers_vrf_public_key[count],0,strlen((const char*)VRF_data.block_verifiers_vrf_public_key[count])); \
    memset(VRF_data.block_verifiers_random_data[count],0,strlen(VRF_data.block_verifiers_random_data[count])); \
    memset(VRF_data.block_blob_signature[count],0,strlen(VRF_data.block_blob_signature[count])); \
  } \
  pthread_rwlock_wrlock(&rwlock); \
  if (memcmp(current_round_part_backup_node,"0",1) == 0) \
  { \
    memset(current_round_part,0,sizeof(current_round_part)); \
    memcpy(current_round_part,"1",1); \
    memset(current_round_part_backup_node,0,sizeof(current_round_part_backup_node)); \
    memcpy(current_round_part_backup_node,"1",1); \
    memcpy(data,"Restarting the round with backup block producer 1 for block ",60); \
    memcpy(data+60,current_block_height,strnlen(current_block_height,BUFFER_SIZE)); \
    print_start_message(current_date_and_time,current_UTC_date_and_time,data,data2); \
    fprintf(stderr,"\n"); \
  } \
  else if (memcmp(current_round_part_backup_node,"1",1) == 0) \
  { \
    memset(current_round_part,0,sizeof(current_round_part)); \
    memcpy(current_round_part,"1",1); \
    memset(current_round_part_backup_node,0,sizeof(current_round_part_backup_node)); \
    memcpy(current_round_part_backup_node,"2",1); \
    memcpy(data,"Restarting the round with backup block producer 2 for block ",60); \
    memcpy(data+60,current_block_height,strnlen(current_block_height,BUFFER_SIZE)); \
    print_start_message(current_date_and_time,current_UTC_date_and_time,data,data2); \
    fprintf(stderr,"\n"); \
  } \
  else if (memcmp(current_round_part_backup_node,"2",1) == 0) \
  { \
    data_network_node_create_block(); \
  } \
  pthread_rwlock_unlock(&rwlock); \
  sync_block_verifiers_seconds(current_date_and_time,current_UTC_date_and_time,0); \
  goto start; 

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  // set the main_network_data_node_create_block so the main network data node can create the block
  pthread_rwlock_wrlock(&rwlock);
  main_network_data_node_create_block = 0;
  pthread_rwlock_unlock(&rwlock);

  // wait for all block verifiers to sync the database
  color_print("Waiting for all block verifiers to sync the databases","blue");
  fprintf(stderr,"\n");
  sync_block_verifiers_minutes(current_date_and_time,current_UTC_date_and_time,1);

  start:

    if ((memcmp(current_round_part_backup_node,"0",1) == 0 && memcmp(main_nodes_list.block_producer_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"1",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_1_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"2",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_2_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"3",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_3_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"4",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_4_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"5",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_5_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0))
    {
      color_print("Your block verifier is the block producer","yellow");
      fprintf(stderr,"\n");
    }
  
    // create a random VRF public key and secret key
    if (create_random_VRF_keys((unsigned char*)VRF_data.vrf_public_key_round_part_4,(unsigned char*)VRF_data.vrf_secret_key_round_part_4) != 1 || crypto_vrf_is_valid_key((const unsigned char*)VRF_data.vrf_public_key_round_part_4) != 1)
    {
      START_PART_4_OF_ROUND_ERROR("Could not create the VRF secret key or VRF public key for the VRF data");
    }  

    // convert the VRF secret key to hexadecimal
    for (count = 0, counter = 0; count < crypto_vrf_SECRETKEYBYTES; count++, counter += 2)
    {
      snprintf(VRF_data.vrf_secret_key_data_round_part_4+counter,BUFFER_SIZE_NETWORK_BLOCK_DATA-1,"%02x",VRF_data.vrf_secret_key_round_part_4[count] & 0xFF);
    }

    // convert the VRF public key to hexadecimal
    for (count = 0, counter = 0; count < crypto_vrf_PUBLICKEYBYTES; count++, counter += 2)
    {
      snprintf(VRF_data.vrf_public_key_data_round_part_4+counter,BUFFER_SIZE_NETWORK_BLOCK_DATA-1,"%02x",VRF_data.vrf_public_key_round_part_4[count] & 0xFF);
    } 

    // create the message
    memset(data3,0,sizeof(data3));
    memcpy(data3,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA\",\r\n \"vrf_secret_key\": \"",92);
    memcpy(data3+92,VRF_data.vrf_secret_key_data_round_part_4,VRF_SECRET_KEY_LENGTH);
    memcpy(data3+220,"\",\r\n \"vrf_public_key\": \"",24);
    memcpy(data3+244,VRF_data.vrf_public_key_data_round_part_4,VRF_PUBLIC_KEY_LENGTH);
    memcpy(data3+308,"\",\r\n \"random_data\": \"",21);
  
    // create random data to use in the alpha string of the VRF data
    memset(data,0,sizeof(data));
    if (random_string(data,RANDOM_STRING_LENGTH) == 0)
    {
      START_PART_4_OF_ROUND_ERROR("Could not create random data for the VRF data");
    }

    memcpy(data3+329,data,RANDOM_STRING_LENGTH);
    memcpy(data3+429,"\",\r\n}",5);

    // add the VRF data to the block verifiers VRF data copy
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0)
      {        
        memcpy(VRF_data.block_verifiers_vrf_secret_key[count],VRF_data.vrf_secret_key_round_part_4,crypto_vrf_SECRETKEYBYTES);
        memcpy(VRF_data.block_verifiers_vrf_secret_key_data[count],VRF_data.vrf_secret_key_data_round_part_4,VRF_SECRET_KEY_LENGTH);
        memcpy(VRF_data.block_verifiers_vrf_public_key[count],VRF_data.vrf_public_key_round_part_4,crypto_vrf_PUBLICKEYBYTES);
        memcpy(VRF_data.block_verifiers_vrf_public_key_data[count],VRF_data.vrf_public_key_data_round_part_4,VRF_PUBLIC_KEY_LENGTH);
        memcpy(VRF_data.block_verifiers_random_data[count],data,RANDOM_STRING_LENGTH);
      }
    } 

    // sign_data
    if (sign_data(data3,0) == 0)
    { 
      START_PART_4_OF_ROUND_ERROR("Could not sign_data");
    }

    // send the message to all block verifiers
    if (block_verifiers_send_data_socket((const char*)data3) == 0)
    {
      START_PART_4_OF_ROUND_ERROR("Could not send data to the block verifiers");
    }

    // wait for the block verifiers to process the votes
    sync_block_verifiers_seconds(current_date_and_time,current_UTC_date_and_time,10);

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
    if (count2 < BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      RESTART_ROUND("Less than the required amount of block verifiers created the data");
    }

  

    // at this point all block verifiers should have the all of the other block verifiers secret key, public key and random data

    // create the VRF alpha string using all of the random data from the block verifiers
    memset(VRF_data.vrf_alpha_string_round_part_4,0,strlen((const char*)VRF_data.vrf_alpha_string_round_part_4));
    if (get_previous_block_hash((char*)VRF_data.vrf_alpha_string_round_part_4,0) == 0)
    {
      START_PART_4_OF_ROUND_ERROR("Could not get the previous block hash");
    }
    
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (strlen((const char*)VRF_data.block_verifiers_vrf_secret_key[count]) == crypto_vrf_SECRETKEYBYTES && strlen((const char*)VRF_data.block_verifiers_vrf_public_key[count]) == crypto_vrf_PUBLICKEYBYTES && strlen(VRF_data.block_verifiers_random_data[count]) == RANDOM_STRING_LENGTH)
      {
        memcpy(VRF_data.vrf_alpha_string_round_part_4+strlen((const char*)VRF_data.vrf_alpha_string_round_part_4),VRF_data.block_verifiers_random_data[count],RANDOM_STRING_LENGTH);
      }
      else
      {
        memcpy(VRF_data.vrf_alpha_string_round_part_4+strlen((const char*)VRF_data.vrf_alpha_string_round_part_4),GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING)-1);
      }
    }

    // convert the vrf alpha string to a string
    for (count2 = 0, count = 0; count2 < (((RANDOM_STRING_LENGTH*2)*BLOCK_VERIFIERS_AMOUNT) + (BLOCK_HASH_LENGTH*2)) / 2; count2++, count += 2)
    {
      snprintf(VRF_data.vrf_alpha_string_data_round_part_4+count,BUFFER_SIZE-1,"%02x",VRF_data.vrf_alpha_string_round_part_4[count2] & 0xFF);
    }

    memset(data,0,sizeof(data));
    memset(data2,0,sizeof(data2));
    crypto_hash_sha512((unsigned char*)data,(const unsigned char*)VRF_data.vrf_alpha_string_data_round_part_4,strlen(VRF_data.vrf_alpha_string_data_round_part_4));

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
   
      // if it is not in the range of 01 - C8 then skip the byte
      if (counter != 0 && counter <= 200)
      {
        counter = counter % BLOCK_VERIFIERS_AMOUNT;
        // check if the block verifier created the data
        if (memcmp(VRF_data.block_verifiers_vrf_secret_key_data[counter],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_SECRET_KEY_DATA,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_SECRET_KEY_DATA)-1) != 0 && memcmp(VRF_data.block_verifiers_vrf_public_key_data[counter],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_PUBLIC_KEY_DATA,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_PUBLIC_KEY_DATA)-1) != 0 && memcmp(VRF_data.block_verifiers_random_data[counter],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING)-1) != 0)
        {
          break;
        }
      }
    }

    // create all of the VRF data
    memcpy(VRF_data.vrf_secret_key_data_round_part_4,VRF_data.block_verifiers_vrf_secret_key_data[counter],VRF_SECRET_KEY_LENGTH);
    memcpy(VRF_data.vrf_secret_key_round_part_4,VRF_data.block_verifiers_vrf_secret_key[counter],crypto_vrf_SECRETKEYBYTES);
    memcpy(VRF_data.vrf_public_key_data_round_part_4,VRF_data.block_verifiers_vrf_public_key_data[counter],VRF_PUBLIC_KEY_LENGTH);
    memcpy(VRF_data.vrf_public_key_round_part_4,VRF_data.block_verifiers_vrf_public_key[counter],crypto_vrf_PUBLICKEYBYTES);

    if (crypto_vrf_prove(VRF_data.vrf_proof_round_part_4,(const unsigned char*)VRF_data.vrf_secret_key_round_part_4,(const unsigned char*)VRF_data.vrf_alpha_string_data_round_part_4,(unsigned long long)strlen((const char*)VRF_data.vrf_alpha_string_data_round_part_4)) != 0)
    {
      START_PART_4_OF_ROUND_ERROR("Could not create the vrf proof");
    }
    if (crypto_vrf_proof_to_hash(VRF_data.vrf_beta_string_round_part_4,(const unsigned char*)VRF_data.vrf_proof_round_part_4) != 0)
    {
      START_PART_4_OF_ROUND_ERROR("Could not create the vrf beta string");
    }
    if (crypto_vrf_verify(VRF_data.vrf_beta_string_round_part_4,(const unsigned char*)VRF_data.vrf_public_key_round_part_4,(const unsigned char*)VRF_data.vrf_proof_round_part_4,(const unsigned char*)VRF_data.vrf_alpha_string_data_round_part_4,(unsigned long long)strlen((const char*)VRF_data.vrf_alpha_string_data_round_part_4)) != 0)
    {
      START_PART_4_OF_ROUND_ERROR("Could not create the VRF data");
    }

    // convert the vrf proof and vrf beta string to a string
    for (counter = 0, count = 0; counter < crypto_vrf_PROOFBYTES; counter++, count += 2)
    {
      snprintf(VRF_data.vrf_proof_data_round_part_4+count,BUFFER_SIZE_NETWORK_BLOCK_DATA-1,"%02x",VRF_data.vrf_proof_round_part_4[counter] & 0xFF);
    }
    for (counter = 0, count = 0; counter < crypto_vrf_OUTPUTBYTES; counter++, count += 2)
    {
      snprintf(VRF_data.vrf_beta_string_data_round_part_4+count,BUFFER_SIZE_NETWORK_BLOCK_DATA-1,"%02x",VRF_data.vrf_beta_string_round_part_4[counter] & 0xFF);
    }

    memset(VRF_data.block_blob,0,strlen(VRF_data.block_blob));

    // create the block template and send it to all block verifiers if the block verifier is the block producer
    if ((memcmp(current_round_part_backup_node,"0",1) == 0 && memcmp(main_nodes_list.block_producer_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"1",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_1_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"2",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_2_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"3",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_3_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"4",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_4_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"5",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_5_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0))
    {
      if (get_block_template(VRF_data.block_blob,0) == 0)
      {
        START_PART_4_OF_ROUND_ERROR("Could not get a block template");
      }  

      // create the message
      memset(data,0,sizeof(data));
      memcpy(data,"{\r\n \"message_settings\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND_CREATE_NEW_BLOCK\",\r\n \"block_blob\": \"",97);
      memcpy(data+97,VRF_data.block_blob,strnlen(VRF_data.block_blob,BUFFER_SIZE));
      memcpy(data+strlen(data),"\",\r\n}",5);

      // sign_data
      if (sign_data(data,0) == 0)
      { 
        START_PART_4_OF_ROUND_ERROR("Could not sign_data");
      }

      // send the message to all block verifiers
      if (block_verifiers_send_data_socket((const char*)data) == 0)
      {
        START_PART_4_OF_ROUND_ERROR("Could not send data to the block verifiers");
      }
    }
    
    // wait for the block verifiers to process the votes
    sync_block_verifiers_seconds(current_date_and_time,current_UTC_date_and_time,30);



    // at this point all block verifiers should have the same VRF data and the network block

    // check if the network block string was created from the correct block verifier
    if (memcmp(VRF_data.block_blob,"",1) == 0)
    {
      RESTART_ROUND("Could not receive the network block string from the block producer");
    }

    // convert the network block string to a blockchain data
    if (network_block_string_to_blockchain_data(VRF_data.block_blob,"0") == 0)
    {
      START_PART_4_OF_ROUND_ERROR("Could not convert the network block string to a blockchain data");
    }

    // change the network block nonce to the block producer network block nonce
    memcpy(blockchain_data.nonce_data,BLOCK_PRODUCER_NETWORK_BLOCK_NONCE,sizeof(BLOCK_PRODUCER_NETWORK_BLOCK_NONCE)-1);

    // add all of the VRF data to the blockchain_data struct 
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (memcmp(main_nodes_list.block_producer_public_address,current_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH) == 0)
      {
        memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name,current_block_verifiers_list.block_verifiers_name[count],strnlen(current_block_verifiers_list.block_verifiers_name[count],BUFFER_SIZE)); 
        memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_public_address,current_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH);
      }
      if (memcmp(main_nodes_list.block_producer_backup_block_verifier_1_public_address,current_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH) == 0)
      {
        block_producer_backup_settings[0] = count;
      }
      if (memcmp(main_nodes_list.block_producer_backup_block_verifier_2_public_address,current_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH) == 0)
      {
        block_producer_backup_settings[1] = count;
      }
      if (memcmp(main_nodes_list.block_producer_backup_block_verifier_3_public_address,current_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH) == 0)
      {
        block_producer_backup_settings[2] = count;
      }
      if (memcmp(main_nodes_list.block_producer_backup_block_verifier_4_public_address,current_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH) == 0)
      {
        block_producer_backup_settings[3] = count;
      }
      if (memcmp(main_nodes_list.block_producer_backup_block_verifier_5_public_address,current_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH) == 0)
      {
        block_producer_backup_settings[4] = count;
      }
    }

    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,current_round_part_backup_node,1);
    
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names+strlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names),current_block_verifiers_list.block_verifiers_name[block_producer_backup_settings[0]],strnlen(current_block_verifiers_list.block_verifiers_name[block_producer_backup_settings[0]],BUFFER_SIZE));
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names+strlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names),",",1);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names+strlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names),current_block_verifiers_list.block_verifiers_name[block_producer_backup_settings[1]],strnlen(current_block_verifiers_list.block_verifiers_name[block_producer_backup_settings[1]],BUFFER_SIZE));
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names+strlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names),",",1);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names+strlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names),current_block_verifiers_list.block_verifiers_name[block_producer_backup_settings[2]],strnlen(current_block_verifiers_list.block_verifiers_name[block_producer_backup_settings[2]],BUFFER_SIZE));
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names+strlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names),",",1);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names+strlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names),current_block_verifiers_list.block_verifiers_name[block_producer_backup_settings[3]],strnlen(current_block_verifiers_list.block_verifiers_name[block_producer_backup_settings[3]],BUFFER_SIZE));
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names+strlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names),",",1);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names+strlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names),current_block_verifiers_list.block_verifiers_name[block_producer_backup_settings[4]],strnlen(current_block_verifiers_list.block_verifiers_name[block_producer_backup_settings[4]],BUFFER_SIZE));

    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_secret_key_round_part_4,VRF_data.vrf_secret_key_round_part_4,crypto_vrf_SECRETKEYBYTES);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_secret_key_data_round_part_4,VRF_data.vrf_secret_key_data_round_part_4,VRF_SECRET_KEY_LENGTH);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_4,VRF_data.vrf_public_key_round_part_4,crypto_vrf_PUBLICKEYBYTES);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_4,VRF_data.vrf_public_key_data_round_part_4,VRF_PUBLIC_KEY_LENGTH);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_4,VRF_data.vrf_alpha_string_round_part_4,strnlen((const char*)VRF_data.vrf_alpha_string_round_part_4,BUFFER_SIZE));
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_4,VRF_data.vrf_alpha_string_data_round_part_4,strnlen(VRF_data.vrf_alpha_string_data_round_part_4,BUFFER_SIZE));
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_4,VRF_data.vrf_proof_round_part_4,crypto_vrf_PROOFBYTES);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_4,VRF_data.vrf_proof_data_round_part_4,VRF_PROOF_LENGTH);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_4,VRF_data.vrf_beta_string_round_part_4,crypto_vrf_OUTPUTBYTES);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_4,VRF_data.vrf_beta_string_data_round_part_4,VRF_BETA_LENGTH);

    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key[count],VRF_data.block_verifiers_vrf_secret_key[count],crypto_vrf_SECRETKEYBYTES);
      memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key[count],VRF_data.block_verifiers_vrf_public_key[count],crypto_vrf_PUBLICKEYBYTES);
      memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key_data[count],VRF_data.block_verifiers_vrf_secret_key_data[count],VRF_SECRET_KEY_LENGTH);
      memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key_data[count],VRF_data.block_verifiers_vrf_public_key_data[count],VRF_PUBLIC_KEY_LENGTH);
      memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data_text[count],VRF_data.block_verifiers_random_data[count],RANDOM_STRING_LENGTH);

      memcpy(blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address[count],next_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH);
      memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE_DATA,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE_DATA)-1);
            
      for (counter = 0, count2 = 0; counter < RANDOM_STRING_LENGTH; counter++, count2 += 2)
      {
        snprintf(blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data[count]+count2,RANDOM_STRING_LENGTH,"%02x",VRF_data.block_verifiers_random_data[count][counter] & 0xFF);
      }
    }

    memcpy(blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,blockchain_data.previous_block_hash_data,BLOCK_HASH_LENGTH);

    // convert the blockchain_data to a network_block_string
    memset(data,0,sizeof(data));
    if (blockchain_data_to_network_block_string(VRF_data.block_blob) == 0)
    {
      START_PART_4_OF_ROUND_ERROR("Could not convert the blockchain_data to a network_block_string");
    }

    // sign the network block string
    memset(data,0,sizeof(data));
    if (sign_network_block_string(data,VRF_data.block_blob,0) == 0)
    {
      START_PART_4_OF_ROUND_ERROR("Could not sign the network block string");
    }

    // add the block verifier signature to the VRF data and the blockchain_data struct
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0)
      {
        memcpy(VRF_data.block_blob_signature[count],data,XCASH_SIGN_DATA_LENGTH);
      }
    }

    // create the message
    memset(data3,0,sizeof(data3));
    memcpy(data3,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE\",\r\n \"block_blob_signature\": \"",110);
    memcpy(data3+110,data,XCASH_SIGN_DATA_LENGTH);
    memcpy(data3+203,"\",\r\n}",5);
  
    // sign_data
    if (sign_data(data3,0) == 0)
    { 
      START_PART_4_OF_ROUND_ERROR("Could not sign_data");
    }

    // send the message to all block verifiers
    if (block_verifiers_send_data_socket((const char*)data3) == 0)
    {
      START_PART_4_OF_ROUND_ERROR("Could not send data to the block verifiers");
    }

    // wait for the block verifiers to process the votes
    sync_block_verifiers_seconds(current_date_and_time,current_UTC_date_and_time,40);



    // at this point all block verifiers should have the same VRF data, network block string and all block verifiers signed data

    // process the data and add the block verifiers signatures to the block
    for (count = 0, count2 = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (strlen(VRF_data.block_blob_signature[count]) == XCASH_SIGN_DATA_LENGTH && memcmp(VRF_data.block_blob_signature[count],XCASH_SIGN_DATA_PREFIX,sizeof(XCASH_SIGN_DATA_PREFIX)-1) == 0)
      {
        memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count],VRF_data.block_blob_signature[count],XCASH_SIGN_DATA_LENGTH);
        count2++;
      }
      else
      {
        memcpy(VRF_data.block_blob_signature[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE)-1);
        memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE)-1);
      }
    }

    // check if the network block string has at least 67 of the block verifiers network block signature
    if (count2 < BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      RESTART_ROUND("The network block string has less than the required amount of block verifiers network block signature");
    }

    // get the previous network block string
    memset(data,0,sizeof(data));
    memset(data2,0,sizeof(data2));
    memset(data3,0,sizeof(data3));
    sscanf(current_block_height,"%zu", &count);
    if (count < XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT-1)
    {
      START_PART_4_OF_ROUND_ERROR("Could not get the current block height");
    }
    count--;
    snprintf(data3,sizeof(data3)-1,"%zu",count);
    count2 = ((count - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME) + 1;
    memcpy(data,"{\"block_height\":\"",17);
    memcpy(data+17,data3,strnlen(data3,sizeof(data)));
    memcpy(data+strlen(data),"\"}",2);
    memset(data3,0,strlen(data3));
    memcpy(data3,"reserve_bytes_",14);
    snprintf(data3+14,sizeof(data3)-15,"%zu",count2);
    if (read_document_field_from_collection(DATABASE_NAME,data3,data,"reserve_bytes",data2,0) == 0)
    {
      START_PART_4_OF_ROUND_ERROR("Could not get the previous blocks reserve bytes");
    }

    // verify the block
    if (verify_network_block_data(1,1,1,"0",data2) == 0)
    {
      START_PART_4_OF_ROUND_ERROR("The MAIN_NODES_TO_NODES_PART_4_OF_ROUND message is invalid");
    }

    // convert the blockchain_data to a network_block_string
	  memset(data,0,sizeof(data));	
	  if (blockchain_data_to_network_block_string(data) == 0)	
	  {		 
	    START_PART_4_OF_ROUND_ERROR("Could not convert the blockchain_data to a network_block_string");	
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
      snprintf(data3+count,sizeof(data3)-1,"%02x",data2[count2] & 0xFF);
    }

    // reset the current_round_part_vote_data.vote_results_valid struct
    memset(current_round_part_vote_data.current_vote_results,0,sizeof(current_round_part_vote_data.current_vote_results));
    current_round_part_vote_data.vote_results_valid = 1;
    current_round_part_vote_data.vote_results_invalid = 0;

    memcpy(current_round_part_vote_data.current_vote_results,data3,DATA_HASH_LENGTH);

    // create the message
    memset(data3,0,sizeof(data3));
    memcpy(data3,"{\r\n \"message_settings\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n \"vote_settings\": \"valid\",\r\n \"vote_data\": \"",99);  
    memcpy(data3+strlen(data3),current_round_part_vote_data.current_vote_results,DATA_HASH_LENGTH);
    memcpy(data3+strlen(data3),"\",\r\n}",5); 

    // sign_data
    if (sign_data(data3,0) == 0)
    { 
      START_PART_4_OF_ROUND_ERROR("Could not sign_data");
    }    

    // wait for the block verifiers to process the votes
    sync_block_verifiers_seconds(current_date_and_time,current_UTC_date_and_time,45);

    // send the message to all block verifiers
    if (block_verifiers_send_data_socket((const char*)data3) == 0)
    {
      START_PART_4_OF_ROUND_ERROR("Could not send data to the block verifiers");
    }

    // wait for the block verifiers to process the votes
    sync_block_verifiers_seconds(current_date_and_time,current_UTC_date_and_time,55);

    // process the vote results
    if (current_round_part_vote_data.vote_results_valid < BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      RESTART_ROUND("Invalid network block string data hash");
    }



    // at this point all block verifiers have the same network block string with all of the VRF data

    // add the data hash to the network block string
    memset(data,0,sizeof(data));
    if (add_data_hash_to_network_block_string(VRF_data.block_blob,data) == 0)
    {
      START_PART_4_OF_ROUND_ERROR("Could not add the network block string data hash");
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

    pthread_rwlock_rdlock(&rwlock);
    while(database_settings != 1)
    {
      sleep(1);
    }
    pthread_rwlock_unlock(&rwlock);

    // add the network block string to the database
    sscanf(current_block_height, "%zu", &count);
    memset(data3,0,sizeof(data3));
    memcpy(data3,"reserve_bytes_",14);
    count2 = ((count - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME) + 1;
    snprintf(data3+14,sizeof(data3)-15,"%zu",count2);
    if (insert_document_into_collection_json(DATABASE_NAME,data3,data2,0) == 0)
    {
      START_PART_4_OF_ROUND_ERROR("Could not add the new block to the database");
    }

    memset(data3,0,sizeof(data3));

    // create the verify_block file for the X-CASH Daemon
    if (write_file(VRF_data.reserve_bytes_data_hash,verify_block_file) == 0)
    {
      START_PART_4_OF_ROUND_ERROR("Could not create the verify_block file");
    }
    sleep(BLOCK_VERIFIERS_SETTINGS);

    // wait for the block verifiers to process the votes
    color_print("Waiting for the block producer to submit the block to the network","blue");
    fprintf(stderr,"\n");
    sync_block_verifiers_minutes_and_seconds(current_date_and_time,current_UTC_date_and_time,4,50);

    // have the block producer submit the block to the network
    if ((memcmp(current_round_part_backup_node,"0",1) == 0 && memcmp(main_nodes_list.block_producer_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"1",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_1_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"2",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_2_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"3",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_3_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"4",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_4_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"5",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_5_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0))
    {
      if (submit_block_template(data,0) == 0)
      {
        memset(data2,0,sizeof(data2));
        memcpy(data2,"{\"block_height\":\"",17);
        memcpy(data2+17,current_block_height,strnlen(current_block_height,sizeof(data2)));
        memcpy(data2+strlen(data2),"\"}",2);
        sscanf(current_block_height, "%zu", &count);
        memset(data3,0,sizeof(data3));
        memcpy(data3,"reserve_bytes_",14);
        count2 = ((count - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME) + 1;
        snprintf(data3+14,sizeof(data3)-15,"%zu",count2);
        delete_document_from_collection(DATABASE_NAME,data3,data2,0);
        START_PART_4_OF_ROUND_ERROR("Could not submit the block to the network");
      }
    }
    sleep(BLOCK_VERIFIERS_SETTINGS);

    if (network_data_node_settings == 1)
    {
      memset(data3,0,sizeof(data3));
      if (get_current_block_height(data3,0) == 0)
      {
        START_PART_4_OF_ROUND_ERROR("Could not get the current block height");
      }

      if (strncmp(data3,current_block_height,sizeof(data3)) == 0)
      {
        // the block was not submitted to the network. Loop through each network data node until it is submitted to the network
        for (count = 0; count < NETWORK_DATA_NODES_AMOUNT; count++)
        {
          if (memcmp(network_data_nodes_list.network_data_nodes_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0)
          {
            if (submit_block_template(data,0) == 0)
            {
              memset(data2,0,sizeof(data2));
              memcpy(data2,"{\"block_height\":\"",17);
              memcpy(data2+17,current_block_height,strnlen(current_block_height,sizeof(data2)));
              memcpy(data2+strlen(data2),"\"}",2);
              sscanf(current_block_height, "%zu", &count);
              memset(data3,0,sizeof(data3));
              memcpy(data3,"reserve_bytes_",14);
              count2 = ((count - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME) + 1;
              snprintf(data3+14,sizeof(data3)-15,"%zu",count2);
              delete_document_from_collection(DATABASE_NAME,data3,data2,0);
              START_PART_4_OF_ROUND_ERROR("Could not submit the block to the network");
            }

            if (get_current_block_height(data3,0) == 0)
            {
              START_PART_4_OF_ROUND_ERROR("Could not get the current block height");
            }

            if (memcmp(data3,current_block_height,strnlen(current_block_height,BUFFER_SIZE)) != 0)
            {
              break;
            }
          }
          sleep(BLOCK_VERIFIERS_SETTINGS);
        } 
      }   
    }
    return 1;
    
    #undef START_PART_4_OF_ROUND_ERROR
    #undef RESTART_ROUND
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
  struct tm* current_UTC_date_and_time;
  struct delegates delegates[MAXIMUM_AMOUNT_OF_DELEGATES];
  size_t count;
  size_t count2;
  int settings = 0;

  // define macros
  #define UPDATE_BLOCK_VERIFIERS_LIST_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"update_block_verifiers_list",27); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  // reset the previous_block_verifiers_list struct
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memset(previous_block_verifiers_list.block_verifiers_name[count],0,sizeof(previous_block_verifiers_list.block_verifiers_name[count]));
    memset(previous_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_public_address[count]));
    memset(previous_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_IP_address[count]));
  }

  // copy the current_block_verifiers_list to the previous_block_verifiers_list
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memcpy(previous_block_verifiers_list.block_verifiers_name[count],current_block_verifiers_list.block_verifiers_name[count],strnlen(current_block_verifiers_list.block_verifiers_name[count],sizeof(previous_block_verifiers_list.block_verifiers_name[count])));
    memcpy(previous_block_verifiers_list.block_verifiers_public_address[count],current_block_verifiers_list.block_verifiers_public_address[count],strnlen(current_block_verifiers_list.block_verifiers_public_address[count],sizeof(previous_block_verifiers_list.block_verifiers_public_address[count])));
    memcpy(previous_block_verifiers_list.block_verifiers_IP_address[count],current_block_verifiers_list.block_verifiers_IP_address[count],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count],sizeof(previous_block_verifiers_list.block_verifiers_IP_address[count])));
  }

  // reset the current_block_verifiers_list struct
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memset(current_block_verifiers_list.block_verifiers_name[count],0,sizeof(current_block_verifiers_list.block_verifiers_name[count]));
    memset(current_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_public_address[count]));
    memset(current_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_IP_address[count]));
  }  

  // copy the next_block_verifiers_list to the current_block_verifiers_list
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memcpy(current_block_verifiers_list.block_verifiers_name[count],next_block_verifiers_list.block_verifiers_name[count],strnlen(next_block_verifiers_list.block_verifiers_name[count],sizeof(current_block_verifiers_list.block_verifiers_name[count])));
    memcpy(current_block_verifiers_list.block_verifiers_public_address[count],next_block_verifiers_list.block_verifiers_public_address[count],strnlen(next_block_verifiers_list.block_verifiers_public_address[count],sizeof(current_block_verifiers_list.block_verifiers_public_address[count])));
    memcpy(current_block_verifiers_list.block_verifiers_IP_address[count],next_block_verifiers_list.block_verifiers_IP_address[count],strnlen(next_block_verifiers_list.block_verifiers_IP_address[count],sizeof(current_block_verifiers_list.block_verifiers_IP_address[count])));
  }

  // reset the next_block_verifiers_list struct
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memset(next_block_verifiers_list.block_verifiers_name[count],0,sizeof(next_block_verifiers_list.block_verifiers_name[count]));
    memset(next_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_public_address[count]));
    memset(next_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_IP_address[count]));
  }

  // initialize the delegates struct
  for (count = 0; count < MAXIMUM_AMOUNT_OF_DELEGATES; count++)
  {
    delegates[count].public_address = (char*)calloc(100,sizeof(char));
    delegates[count].total_vote_count = (char*)calloc(100,sizeof(char));
    delegates[count].IP_address = (char*)calloc(100,sizeof(char));
    delegates[count].delegate_name = (char*)calloc(100,sizeof(char));
    delegates[count].about = (char*)calloc(1025,sizeof(char));
    delegates[count].website = (char*)calloc(100,sizeof(char));
    delegates[count].team = (char*)calloc(100,sizeof(char));
    delegates[count].pool_mode = (char*)calloc(100,sizeof(char));
    delegates[count].fee_structure = (char*)calloc(100,sizeof(char));
    delegates[count].server_settings = (char*)calloc(100,sizeof(char));
    delegates[count].block_verifier_score = (char*)calloc(100,sizeof(char));
    delegates[count].online_status = (char*)calloc(100,sizeof(char));
    delegates[count].block_verifier_total_rounds = (char*)calloc(100,sizeof(char));
    delegates[count].block_verifier_online_total_rounds = (char*)calloc(100,sizeof(char));
    delegates[count].block_verifier_online_percentage = (char*)calloc(100,sizeof(char));
    delegates[count].block_producer_total_rounds = (char*)calloc(100,sizeof(char));
    delegates[count].block_producer_block_heights = (char*)calloc(50000,sizeof(char));

    if (delegates[count].public_address == NULL || delegates[count].total_vote_count == NULL || delegates[count].IP_address == NULL || delegates[count].delegate_name == NULL || delegates[count].about == NULL || delegates[count].website == NULL || delegates[count].team == NULL || delegates[count].pool_mode == NULL || delegates[count].fee_structure == NULL || delegates[count].server_settings == NULL || delegates[count].block_verifier_score == NULL || delegates[count].online_status == NULL || delegates[count].block_verifier_total_rounds == NULL || delegates[count].block_verifier_online_total_rounds == NULL || delegates[count].block_verifier_online_percentage == NULL || delegates[count].block_producer_total_rounds == NULL || delegates[count].block_producer_block_heights == NULL)
    {
      memcpy(error_message.function[error_message.total],"update_block_verifiers_list",27);
      memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
      error_message.total++;
      print_error_message(current_date_and_time,current_UTC_date_and_time,data);  
      exit(0);
    }
  }

   // organize the delegates
   if (organize_delegates(delegates) < BLOCK_VERIFIERS_AMOUNT)
   {
     UPDATE_BLOCK_VERIFIERS_LIST_ERROR("Could not organize the delegates");
   }

  // copy the database_multiple_documents_fields to the next_block_verifiers_list
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    if (strncmp(delegates[count].online_status,"true",BUFFER_SIZE) == 0)
    {
       memcpy(next_block_verifiers_list.block_verifiers_name[count],delegates[count].delegate_name,strnlen(delegates[count].delegate_name,sizeof(next_block_verifiers_list.block_verifiers_name[count])));
       memcpy(next_block_verifiers_list.block_verifiers_public_address[count],delegates[count].public_address,strnlen(delegates[count].public_address,sizeof(next_block_verifiers_list.block_verifiers_public_address[count])));
       memcpy(next_block_verifiers_list.block_verifiers_IP_address[count],delegates[count].IP_address,strnlen(delegates[count].IP_address,sizeof(next_block_verifiers_list.block_verifiers_IP_address[count])));
    } 
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

  // reset the delegates struct
  for (count = 0; count < MAXIMUM_AMOUNT_OF_DELEGATES; count++)
  {
    pointer_reset(delegates[count].public_address);
    pointer_reset(delegates[count].total_vote_count);
    pointer_reset(delegates[count].IP_address);
    pointer_reset(delegates[count].delegate_name);
    pointer_reset(delegates[count].about);
    pointer_reset(delegates[count].website);
    pointer_reset(delegates[count].team);
    pointer_reset(delegates[count].pool_mode);
    pointer_reset(delegates[count].fee_structure);
    pointer_reset(delegates[count].server_settings);
    pointer_reset(delegates[count].block_verifier_score);
    pointer_reset(delegates[count].online_status);
    pointer_reset(delegates[count].block_verifier_total_rounds);
    pointer_reset(delegates[count].block_verifier_online_total_rounds);
    pointer_reset(delegates[count].block_verifier_online_percentage);
    pointer_reset(delegates[count].block_producer_total_rounds);
    pointer_reset(delegates[count].block_producer_block_heights);
  }

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
    if (read_document_field_from_collection(DATABASE_NAME,DATABASE_COLLECTION,message,"block_verifier_total_rounds",data,0) == 0)
    {
      ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not read the block_verifier_total_rounds from the database");
    }
    sscanf(data, "%zu", &number);
    number++;
    memset(data,0,sizeof(data));
    memcpy(data,"{\"block_verifier_total_rounds\":\"",32);
    snprintf(data+32,sizeof(data)-33,"%zu",number); 
    memcpy(data+strlen(data),"\"}",2);
    pthread_rwlock_rdlock(&rwlock);
    while(database_settings != 1)
    {
      sleep(1);
    }
    pthread_rwlock_unlock(&rwlock);
    if (update_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,message,data,0) == 0)
    {
      ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not update the block_verifier_total_rounds in the database");
    }

    // add one to the block_verifier_online_total_rounds
    memset(data,0,sizeof(data));
    if (read_document_field_from_collection(DATABASE_NAME,DATABASE_COLLECTION,message,"block_verifier_online_total_rounds",data,0) == 0)
    {
      ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not read the block_verifier_online_total_rounds from the database");
    }
    sscanf(data, "%zu", &number);
    number++;
    memset(data,0,sizeof(data));
    memcpy(data,"{\"block_verifier_online_total_rounds\":\"",39);
    snprintf(data+39,sizeof(data)-40,"%zu",number); 
    memcpy(data+strlen(data),"\"}",2);
    pthread_rwlock_rdlock(&rwlock);
    while(database_settings != 1)
    {
      sleep(1);
    }
    pthread_rwlock_unlock(&rwlock);
    if (update_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,message,data,0) == 0)
    {
      ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not update the block_verifier_online_total_rounds in the database");
    }
    
    // add one to the block_producer_total_rounds and the current block height to the block_producer_block_heights if the public address is the block producer
    if ((memcmp(current_round_part_backup_node,"0",1) == 0 && memcmp(previous_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"1",1) == 0 && memcmp(previous_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_1_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"2",1) == 0 && memcmp(previous_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_2_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"3",1) == 0 && memcmp(previous_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_3_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"4",1) == 0 && memcmp(previous_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_4_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"5",1) == 0 && memcmp(previous_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_5_public_address,XCASH_WALLET_LENGTH) == 0))
    {
      memset(data,0,sizeof(data));
      if (read_document_field_from_collection(DATABASE_NAME,DATABASE_COLLECTION,message,"block_producer_total_rounds",data,0) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not read the block_producer_total_rounds from the database");
      }
      sscanf(data, "%zu", &number);
      number++;
      memset(data,0,sizeof(data));
      memcpy(data,"{\"block_producer_total_rounds\":\"",32);
      snprintf(data+32,sizeof(data)-33,"%zu",number); 
      memcpy(data+strlen(data),"\"}",2);
      pthread_rwlock_rdlock(&rwlock);
      while(database_settings != 1)
      {
        sleep(1);
      }
      pthread_rwlock_unlock(&rwlock);
      if (update_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,message,data,0) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not update the block_producer_total_rounds in the database");
      }

      memset(data,0,sizeof(data));
      if (read_document_field_from_collection(DATABASE_NAME,DATABASE_COLLECTION,message,"block_producer_block_heights",data,0) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not read the block_producer_block_heights from the database");
      }      
      memcpy(data+strlen(data),"|",1);
      memcpy(data+strlen(data),BLOCK_HEIGHT,strnlen(BLOCK_HEIGHT,sizeof(data)));
      memset(data2,0,sizeof(data2));
      memcpy(data2,"{\"block_producer_block_heights\":\"",33);
      memcpy(data2+33,data,strnlen(data,sizeof(data2)));
      memcpy(data2+strlen(data2),"\"}",2);
      pthread_rwlock_rdlock(&rwlock);
      while(database_settings != 1)
      {
        sleep(1);
      }
      pthread_rwlock_unlock(&rwlock);
      if (update_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,message,data2,0) == 0)
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

    if (total2 != 0)
    {
      if (total3 / total2 > total)
      {
        total = total3 / total2;
        memset(best_block_verifier_online_percentage_delegate_name,0,sizeof(best_block_verifier_online_percentage_delegate_name));
        memcpy(best_block_verifier_online_percentage_delegate_name,delegates_name,strnlen(delegates_name,sizeof(best_block_verifier_online_percentage_delegate_name)));
      }
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

  pthread_rwlock_rdlock(&rwlock);
  while(database_settings != 1)
  {
    sleep(1);
  }
  pthread_rwlock_unlock(&rwlock);

  // update the database
  if (update_document_from_collection(DATABASE_NAME,"statistics",MESSAGE,message1,0) == 0 || update_document_from_collection(DATABASE_NAME,"statistics",MESSAGE,message2,0) == 0 || update_document_from_collection(DATABASE_NAME,"statistics",MESSAGE,message3,0) == 0 || update_document_from_collection(DATABASE_NAME,"statistics",MESSAGE,message4,0) == 0 || update_document_from_collection(DATABASE_NAME,"statistics",MESSAGE,message5,0) == 0 || update_document_from_collection(DATABASE_NAME,"statistics",MESSAGE,message6,0) == 0)
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
  int main_nodes_count;
  int number[DATA_HASH_LENGTH/2];
  int settings = 1;

  // define macros
  #define CALCULATE_MAIN_NODES_ROLES(settings) \
  memcpy(error_message.function[error_message.total],"calculate_main_nodes_roles",26); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

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
  if (read_document_field_from_collection(DATABASE_NAME,data,data3,"reserve_bytes",data2,0) == 0)
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
        memset(main_nodes_list.block_producer_public_address,0,sizeof(main_nodes_list.block_producer_public_address));
        memset(main_nodes_list.block_producer_IP_address,0,sizeof(main_nodes_list.block_producer_IP_address));
        memcpy(main_nodes_list.block_producer_public_address,current_block_verifiers_list.block_verifiers_public_address[count2],strnlen(current_block_verifiers_list.block_verifiers_public_address[count2],sizeof(main_nodes_list.block_producer_public_address)));
        memcpy(main_nodes_list.block_producer_IP_address,current_block_verifiers_list.block_verifiers_IP_address[count2],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count2],sizeof(main_nodes_list.block_producer_IP_address)));
      }
      if (main_nodes_count == 1)
      {
        // calculate the block_producer_backup_block_verifier_1
        memset(main_nodes_list.block_producer_backup_block_verifier_1_public_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_1_public_address));
        memset(main_nodes_list.block_producer_backup_block_verifier_1_IP_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_1_IP_address));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_1_public_address,current_block_verifiers_list.block_verifiers_public_address[count2],strnlen(current_block_verifiers_list.block_verifiers_public_address[count2],sizeof(main_nodes_list.block_producer_backup_block_verifier_1_public_address)));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_1_IP_address,current_block_verifiers_list.block_verifiers_IP_address[count2],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count2],sizeof(main_nodes_list.block_producer_backup_block_verifier_1_IP_address)));
      }
      if (main_nodes_count == 2)
      {
        // calculate the block_producer_backup_block_verifier_2
        memset(main_nodes_list.block_producer_backup_block_verifier_2_public_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_2_public_address));
        memset(main_nodes_list.block_producer_backup_block_verifier_2_IP_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_2_IP_address));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_2_public_address,current_block_verifiers_list.block_verifiers_public_address[count2],strnlen(current_block_verifiers_list.block_verifiers_public_address[count2],sizeof(main_nodes_list.block_producer_backup_block_verifier_2_public_address)));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_2_IP_address,current_block_verifiers_list.block_verifiers_IP_address[count2],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count2],sizeof(main_nodes_list.block_producer_backup_block_verifier_2_IP_address)));
      }
      if (main_nodes_count == 3)
      {
        // calculate the block_producer_backup_block_verifier_3
        memset(main_nodes_list.block_producer_backup_block_verifier_3_public_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_3_public_address));
        memset(main_nodes_list.block_producer_backup_block_verifier_3_IP_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_3_IP_address));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_3_public_address,current_block_verifiers_list.block_verifiers_public_address[count2],strnlen(current_block_verifiers_list.block_verifiers_public_address[count2],sizeof(main_nodes_list.block_producer_backup_block_verifier_3_public_address)));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_3_IP_address,current_block_verifiers_list.block_verifiers_IP_address[count2],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count2],sizeof(main_nodes_list.block_producer_backup_block_verifier_3_IP_address)));
      }
      if (main_nodes_count == 4)
      {
        // calculate the block_producer_backup_block_verifier_4
        memset(main_nodes_list.block_producer_backup_block_verifier_4_public_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_4_public_address));
        memset(main_nodes_list.block_producer_backup_block_verifier_4_IP_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_4_IP_address));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_4_public_address,current_block_verifiers_list.block_verifiers_public_address[count2],strnlen(current_block_verifiers_list.block_verifiers_public_address[count2],sizeof(main_nodes_list.block_producer_backup_block_verifier_4_public_address)));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_4_IP_address,current_block_verifiers_list.block_verifiers_IP_address[count2],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count2],sizeof(main_nodes_list.block_producer_backup_block_verifier_4_IP_address)));
      }
      if (main_nodes_count == 5)
      {
        // calculate the block_producer_backup_block_verifier_5
        memset(main_nodes_list.block_producer_backup_block_verifier_5_public_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_5_public_address));
        memset(main_nodes_list.block_producer_backup_block_verifier_5_IP_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_5_IP_address));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_5_public_address,current_block_verifiers_list.block_verifiers_public_address[count2],strnlen(current_block_verifiers_list.block_verifiers_public_address[count2],sizeof(main_nodes_list.block_producer_backup_block_verifier_5_public_address)));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_5_IP_address,current_block_verifiers_list.block_verifiers_IP_address[count2],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count2],sizeof(main_nodes_list.block_producer_backup_block_verifier_5_IP_address)));
      }    
      // add the number to the number array
      number[count3] = count2;
      count3++;
      main_nodes_count++;
      if (main_nodes_count == 6)
      {
        break;
      }
    }
    if (((count + 2) == VRF_BETA_LENGTH) && (main_nodes_count != 6))
    {
      color_print("The main nodes calculation process has run out of bytes to read.\nA random network data node will be the block producer","yellow");

      // select a random network data node
      if (strncmp(main_nodes_list.block_producer_public_address,"",1) == 0)
      {
        count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT);
        memset(main_nodes_list.block_producer_public_address,0,sizeof(main_nodes_list.block_producer_public_address));
        memset(main_nodes_list.block_producer_IP_address,0,sizeof(main_nodes_list.block_producer_IP_address));
        memcpy(main_nodes_list.block_producer_public_address,network_data_nodes_list.network_data_nodes_public_address[count],strnlen(network_data_nodes_list.network_data_nodes_public_address[count],sizeof(main_nodes_list.block_producer_public_address)));
        memcpy(main_nodes_list.block_producer_IP_address,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],sizeof(main_nodes_list.block_producer_IP_address)));
      } 
      if (strncmp(main_nodes_list.block_producer_backup_block_verifier_1_public_address,"",1) == 0)
      {
        count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT);
        memset(main_nodes_list.block_producer_backup_block_verifier_1_public_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_1_public_address));
        memset(main_nodes_list.block_producer_backup_block_verifier_1_IP_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_1_IP_address));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_1_public_address,network_data_nodes_list.network_data_nodes_public_address[count],strnlen(network_data_nodes_list.network_data_nodes_public_address[count],sizeof(main_nodes_list.block_producer_backup_block_verifier_1_public_address)));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_1_IP_address,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],sizeof(main_nodes_list.block_producer_backup_block_verifier_1_IP_address)));
      } 
      if (strncmp(main_nodes_list.block_producer_backup_block_verifier_2_public_address,"",1) == 0)
      {
        count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT);
        memset(main_nodes_list.block_producer_backup_block_verifier_2_public_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_2_public_address));
        memset(main_nodes_list.block_producer_backup_block_verifier_2_IP_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_2_IP_address));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_2_public_address,network_data_nodes_list.network_data_nodes_public_address[count],strnlen(network_data_nodes_list.network_data_nodes_public_address[count],sizeof(main_nodes_list.block_producer_backup_block_verifier_2_public_address)));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_2_IP_address,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],sizeof(main_nodes_list.block_producer_backup_block_verifier_2_IP_address)));
      } 
      if (strncmp(main_nodes_list.block_producer_backup_block_verifier_3_public_address,"",1) == 0)
      {
        count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT);
        memset(main_nodes_list.block_producer_backup_block_verifier_3_public_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_3_public_address));
        memset(main_nodes_list.block_producer_backup_block_verifier_3_IP_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_3_IP_address));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_3_public_address,network_data_nodes_list.network_data_nodes_public_address[count],strnlen(network_data_nodes_list.network_data_nodes_public_address[count],sizeof(main_nodes_list.block_producer_backup_block_verifier_3_public_address)));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_3_IP_address,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],sizeof(main_nodes_list.block_producer_backup_block_verifier_3_IP_address)));
      } 
      if (strncmp(main_nodes_list.block_producer_backup_block_verifier_4_public_address,"",1) == 0)
      {
        count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT);
        memset(main_nodes_list.block_producer_backup_block_verifier_4_public_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_4_public_address));
        memset(main_nodes_list.block_producer_backup_block_verifier_4_IP_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_4_IP_address));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_4_public_address,network_data_nodes_list.network_data_nodes_public_address[count],strnlen(network_data_nodes_list.network_data_nodes_public_address[count],sizeof(main_nodes_list.block_producer_backup_block_verifier_4_public_address)));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_4_IP_address,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],sizeof(main_nodes_list.block_producer_backup_block_verifier_4_IP_address)));
      }   
      if (strncmp(main_nodes_list.block_producer_backup_block_verifier_5_public_address,"",1) == 0)
      {
        count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT);
        memset(main_nodes_list.block_producer_backup_block_verifier_5_public_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_5_public_address));
        memset(main_nodes_list.block_producer_backup_block_verifier_5_IP_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_5_IP_address));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_5_public_address,network_data_nodes_list.network_data_nodes_public_address[count],strnlen(network_data_nodes_list.network_data_nodes_public_address[count],sizeof(main_nodes_list.block_producer_backup_block_verifier_5_public_address)));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_5_IP_address,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],sizeof(main_nodes_list.block_producer_backup_block_verifier_5_IP_address)));
      } 
    }
  }
  return 1;

  #undef CALCULATE_MAIN_NODES_ROLES
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_delegates_online_status
Description: Get all of the delegates online status
Return: 0 if an error has occured, otherwise the amount of online delegates
-----------------------------------------------------------------------------------------------------------
*/

int get_delegates_online_status()
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm* current_UTC_date_and_time;
  struct delegates delegates[MAXIMUM_AMOUNT_OF_DELEGATES];
  struct delegates_online_status delegates_online_status[MAXIMUM_AMOUNT_OF_DELEGATES];
  int epoll_fd;
  struct epoll_event events[MAXIMUM_AMOUNT_OF_DELEGATES];
  struct sockaddr_in serv_addr;
  int count;
  int count2;
  int number;
  int total_delegates = 4;
  int total_delegates_online = 0;

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define GET_DELEGATES_ONLINE_STATUS_ERROR(message) \
  memcpy(error_message.function[error_message.total],"get_delegates_online_status",27); \
  memcpy(error_message.data[error_message.total],message,strnlen(message,BUFFER_SIZE)); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  
  // initialize the delegates struct
  for (count = 0; count < MAXIMUM_AMOUNT_OF_DELEGATES; count++)
  {
    delegates[count].public_address = (char*)calloc(100,sizeof(char));
    delegates[count].total_vote_count = (char*)calloc(100,sizeof(char));
    delegates[count].IP_address = (char*)calloc(100,sizeof(char));
    delegates[count].delegate_name = (char*)calloc(100,sizeof(char));
    delegates[count].about = (char*)calloc(1025,sizeof(char));
    delegates[count].website = (char*)calloc(100,sizeof(char));
    delegates[count].team = (char*)calloc(100,sizeof(char));
    delegates[count].pool_mode = (char*)calloc(100,sizeof(char));
    delegates[count].fee_structure = (char*)calloc(100,sizeof(char));
    delegates[count].server_settings = (char*)calloc(100,sizeof(char));
    delegates[count].block_verifier_score = (char*)calloc(100,sizeof(char));
    delegates[count].online_status = (char*)calloc(100,sizeof(char));
    delegates[count].block_verifier_total_rounds = (char*)calloc(100,sizeof(char));
    delegates[count].block_verifier_online_total_rounds = (char*)calloc(100,sizeof(char));
    delegates[count].block_verifier_online_percentage = (char*)calloc(100,sizeof(char));
    delegates[count].block_producer_total_rounds = (char*)calloc(100,sizeof(char));
    delegates[count].block_producer_block_heights = (char*)calloc(50000,sizeof(char));

    if (delegates[count].public_address == NULL || delegates[count].total_vote_count == NULL || delegates[count].IP_address == NULL || delegates[count].delegate_name == NULL || delegates[count].about == NULL || delegates[count].website == NULL || delegates[count].team == NULL || delegates[count].pool_mode == NULL || delegates[count].fee_structure == NULL || delegates[count].server_settings == NULL || delegates[count].block_verifier_score == NULL || delegates[count].online_status == NULL || delegates[count].block_verifier_total_rounds == NULL || delegates[count].block_verifier_online_total_rounds == NULL || delegates[count].block_verifier_online_percentage == NULL || delegates[count].block_producer_total_rounds == NULL || delegates[count].block_producer_block_heights == NULL)
    {
      memcpy(error_message.function[error_message.total],"update_block_verifiers_list",27);
      memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
      error_message.total++;
      print_error_message(current_date_and_time,current_UTC_date_and_time,data);  
      exit(0);
    }
  }

  // initialize the delegates_online_status struct
  for (count = 0; count < MAXIMUM_AMOUNT_OF_DELEGATES; count++)
  {
    delegates_online_status[count].public_address = (char*)calloc(100,sizeof(char));
    delegates_online_status[count].settings = 0;

    if (delegates_online_status[count].public_address == NULL)
    {
      memcpy(error_message.function[error_message.total],"update_block_verifiers_list",27);
      memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
      error_message.total++;
      print_error_message(current_date_and_time,current_UTC_date_and_time,data);  
      exit(0);
    }
  }

  // organize the delegates
  total_delegates = organize_delegates(delegates);
  
  // create the epoll file descriptor
  epoll_fd = epoll_create1(0);
  if (epoll_fd < 0)
  {
    GET_DELEGATES_ONLINE_STATUS_ERROR("Error creating the epoll file descriptor");
  }

  // create the delegates_online_status struct for each delegate
  for (count = 0; count < total_delegates; count++)
  {
    memcpy(delegates_online_status[count].public_address,delegates[count].public_address,XCASH_WALLET_LENGTH);

    if (memcmp(delegates_online_status[count].public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0)
    {
      delegates_online_status[count].settings = 1;
      total_delegates_online++;
      continue;
    }

    /* Create the socket  
    AF_INET = IPV4 support
    SOCK_STREAM = TCP protocol
    SOCK_NONBLOCK = Non blocking socket, so it will be able to use a custom timeout
    */
    delegates_online_status[count].socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (delegates_online_status[count].socket == -1)
    {
      continue;
    }

    // convert the hostname if used, to an IP address
    memset(data,0,sizeof(data));
    memcpy(data,delegates[count].IP_address,strnlen(delegates[count].IP_address,sizeof(data)));
    string_replace(data,sizeof(data),"http://","");
    string_replace(data,sizeof(data),"https://","");
    string_replace(data,sizeof(data),"www.","");
    const struct hostent* HOST_NAME = gethostbyname(data); 
    if (HOST_NAME == NULL)
    {       
      close(delegates_online_status[count].socket);
      continue;
    }

    /* setup the connection
    AF_INET = IPV4
    use htons to convert the port from host byte order to network byte order short
    */
    memset(&serv_addr,0,sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr*)HOST_NAME->h_addr_list[0])));
    serv_addr.sin_port = htons(SEND_DATA_PORT);

    /* create the epoll_event struct
    EPOLLIN = signal when the file descriptor is ready to read
    EPOLLOUT = signal when the file descriptor is ready to write
    */  
    events[count].events = EPOLLIN | EPOLLOUT;
    events[count].data.fd = delegates_online_status[count].socket;

    // add the delegates socket to the epoll file descriptor
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, delegates_online_status[count].socket, &events[count]);

    // connect to the delegate
    connect(delegates_online_status[count].socket,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr_in));
  }

  sleep(TOTAL_CONNECTION_TIME_SETTINGS+1);

  // get the total amount of sockets that are ready
  number = epoll_wait(epoll_fd, events, MAXIMUM_AMOUNT_OF_DELEGATES, 1);

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

    pthread_rwlock_rdlock(&rwlock);
    while(database_settings != 1)
    {
      sleep(1);
    }
    pthread_rwlock_unlock(&rwlock);
    update_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,data2,data,0);  
  }

  // remove the sockets from the epoll file descriptor and close all of the sockets
  for (count = 0; count < total_delegates; count++)
  {
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, delegates_online_status[count].socket, &events[count]);
    close(delegates_online_status[count].socket);
  }
  return total_delegates_online;

  #undef DATABASE_COLLECTION
  #undef GET_DELEGATES_ONLINE_STATUS_ERROR
}