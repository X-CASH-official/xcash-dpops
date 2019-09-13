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

int start_new_round()
{
  // Variables
  char data[BUFFER_SIZE];
  size_t count;
  size_t count2;
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
    system("screen -XS Daemon quit"); \
    sleep(10); \
    system("screen -dmS Daemon xcashd"); \
  } \
  else if (settings == 1) \
  { \
    system("screen -XS Daemon quit"); \
    sleep(10); \
    system("screen -dmS Daemon xcashd --block-verifier"); \
  } \

  #define RESET_VARIABLES \
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++) \
  { \
    memset(VRF_data.block_verifiers_vrf_secret_key_data[count],0,strlen(VRF_data.block_verifiers_vrf_secret_key_data[count])); \
    memset(VRF_data.block_verifiers_vrf_secret_key[count],0,strlen(VRF_data.block_verifiers_vrf_secret_key[count])); \
    memset(VRF_data.block_verifiers_vrf_public_key_data[count],0,strlen(VRF_data.block_verifiers_vrf_public_key_data[count])); \
    memset(VRF_data.block_verifiers_vrf_public_key[count],0,strlen(VRF_data.block_verifiers_vrf_public_key[count])); \
    memset(VRF_data.block_verifiers_random_data[count],0,strlen(VRF_data.block_verifiers_random_data[count])); \
    memset(VRF_data.block_blob_signature[count],0,strlen(VRF_data.block_blob_signature[count])); \
    memset(invalid_reserve_proofs.block_verifier_public_address[count],0,strlen(invalid_reserve_proofs.block_verifier_public_address[count])); \
    memset(invalid_reserve_proofs.public_address[count],0,strlen(invalid_reserve_proofs.public_address[count])); \
    memset(invalid_reserve_proofs.reserve_proof[count],0,strlen(invalid_reserve_proofs.reserve_proof[count])); \
  } \
  memset(VRF_data.vrf_secret_key_data_round_part_4,0,strlen(VRF_data.vrf_secret_key_data_round_part_4)); \
  memset(VRF_data.vrf_secret_key_round_part_4,0,strlen(VRF_data.vrf_secret_key_round_part_4)); \
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

  // start a new round
  if (get_current_block_height(current_block_height,0) == 0)
  {
    START_NEW_ROUND_ERROR("Could not get the current block height");
  }
  memcpy(data,"A new round is starting for block ",34);
  memcpy(data+34,current_block_height,strnlen(current_block_height,BUFFER_SIZE));
  print_start_message(data);

  // update the previous, current and next block verifiers at the begining of the round, so a restart round does not affect the previous, current and next block verifiers
  settings = update_block_verifiers_list();
  if (settings == 0)
  {
    START_NEW_ROUND_ERROR("Could not update the previous, current and next block verifiers list");
  }
 
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
  pthread_rwlock_unlock(&rwlock);*/

  // check if the current block height - 1 is a X-CASH proof of stake block since this will check to see if these are the first three blocks on the network
  sscanf(current_block_height,"%zu", &count);
  count = count - 1;
  memset(data,0,sizeof(data,BUFFER_SIZE));
  snprintf(data,sizeof(data)-1,"%zu",count);
  settings = get_block_settings(data,0);
  sscanf(current_block_height,"%zu", &count);
  count = count - 2;
  memset(data,0,sizeof(data,BUFFER_SIZE));
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
      print_error_message;
      
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
      print_error_message;
      color_print("Your block verifier will wait until the next round","red");
      printf("\n");
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

int start_current_round_start_blocks()
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  struct send_and_receive_data_socket_thread_parameters send_and_receive_data_socket_thread_parameters[BLOCK_VERIFIERS_AMOUNT];
  size_t count; 
  size_t count2; 

  // threads
  pthread_t thread_id[BLOCK_VERIFIERS_AMOUNT];

  // define macros
  #define DATABASE_COLLECTION "reserve_bytes_1"
  #define START_CURRENT_ROUND_START_BLOCKS_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"start_current_round_start_blocks",32); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;
  
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  // set the main_network_data_node_create_block so the main network data node can create the block
  pthread_rwlock_wrlock(&rwlock);
  main_network_data_node_create_block = 1;
  pthread_rwlock_unlock(&rwlock);

  // wait for all block verifiers to sync the database
  color_print("Waiting for all block verifiers to sync the databases","blue");
  printf("\n");
  sync_block_verifiers_minutes(1);

  // check if the block verifier is the main network data node
  if (memcmp(xcash_wallet_public_address,network_data_nodes_list.network_data_nodes_public_address[0],XCASH_WALLET_LENGTH) != 0)
  {
    color_print("Your block verifier is not the main data network node so your block verifier will sit out for the remainder of the round","yellow");
    printf("\n");
    do
    {
      usleep(200000);
      get_current_UTC_time;
    } while (current_UTC_date_and_time->tm_min % BLOCK_TIME != 4 || current_UTC_date_and_time->tm_sec != 50); 
    return 1;
  } 

  color_print("Your block verifier is the main data network node so your block verifier will create the block","yellow");
  printf("\n");

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
    memcpy(VRF_data.vrf_alpha_string_round_part_4+strlen(VRF_data.vrf_alpha_string_round_part_4),GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING)-1);
  }   

  // convert the vrf alpha string to a string
  for (count2 = 0, count = 0; count2 < (((RANDOM_STRING_LENGTH*2)*BLOCK_VERIFIERS_AMOUNT) + (BLOCK_HASH_LENGTH*2)) / 2; count2++, count += 2)
  {
    snprintf(VRF_data.vrf_alpha_string_data_round_part_4+count,BUFFER_SIZE-1,"%02x",VRF_data.vrf_alpha_string_round_part_4[count2] & 0xFF);
  }

  if (crypto_vrf_prove(VRF_data.vrf_proof_round_part_4,(const unsigned char*)VRF_data.vrf_secret_key_round_part_4,VRF_data.vrf_alpha_string_data_round_part_4,(unsigned long long)strlen((const char*)VRF_data.vrf_alpha_string_data_round_part_4)) != 0)
  {
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not create the vrf proof");
  }
  if (crypto_vrf_proof_to_hash(VRF_data.vrf_beta_string_round_part_4,(const unsigned char*)VRF_data.vrf_proof_round_part_4) != 0)
  {
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not create the vrf beta string");
  }
  if (crypto_vrf_verify(VRF_data.vrf_beta_string_round_part_4,(const unsigned char*)VRF_data.vrf_public_key_round_part_4,(const unsigned char*)VRF_data.vrf_proof_round_part_4,VRF_data.vrf_alpha_string_data_round_part_4,(unsigned long long)strlen((const char*)VRF_data.vrf_alpha_string_data_round_part_4)) != 0)
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
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_4,VRF_data.vrf_alpha_string_round_part_4,strnlen(VRF_data.vrf_alpha_string_round_part_4,BUFFER_SIZE));
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
  memset(data,0,sizeof(data,BUFFER_SIZE));
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
  if (get_path(data3,0) == 0 || write_file(VRF_data.reserve_bytes_data_hash,data3) == 0)
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
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    sleep(BLOCK_VERIFIERS_SETTINGS);
    if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
    {    
      memset(send_and_receive_data_socket_thread_parameters[count].HOST,0,sizeof(send_and_receive_data_socket_thread_parameters[count].HOST));
      memset(send_and_receive_data_socket_thread_parameters[count].DATA,0,sizeof(send_and_receive_data_socket_thread_parameters[count].DATA));
      memcpy(send_and_receive_data_socket_thread_parameters[count].HOST,current_block_verifiers_list.block_verifiers_IP_address[count],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count],sizeof(send_and_receive_data_socket_thread_parameters[count].HOST)));
      memcpy(send_and_receive_data_socket_thread_parameters[count].DATA,data3,strnlen(data3,sizeof(send_and_receive_data_socket_thread_parameters[count].DATA)));
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
  printf("\n");
  do
  {
    usleep(200000);
    get_current_UTC_time;
  } while (current_UTC_date_and_time->tm_min % BLOCK_TIME != 4 || current_UTC_date_and_time->tm_sec != 50);

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

int data_network_node_create_block()
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  struct send_and_receive_data_socket_thread_parameters send_and_receive_data_socket_thread_parameters[BLOCK_VERIFIERS_AMOUNT];
  size_t count; 
  size_t counter;
  size_t count2; 

  // threads
  pthread_t thread_id[BLOCK_VERIFIERS_AMOUNT];

  // define macros
  #define DATA_NETWORK_NODE_CREATE_BLOCK_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"start_current_round_start_blocks",32); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  memcpy(data,"The block producer and all backup block producers have not been able to create the network block\nThe main network data node will now create the network block",157);
  print_start_message(data);

  // set the main_network_data_node_create_block so the main network data node can create the block
  pthread_rwlock_wrlock(&rwlock);
  main_network_data_node_create_block = 1;
  pthread_rwlock_unlock(&rwlock);

  // wait for the block verifiers to process the votes
  sync_block_verifiers_minutes(4);

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
    printf("\n");

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
      memcpy(VRF_data.vrf_alpha_string_round_part_4+strlen(VRF_data.vrf_alpha_string_round_part_4),GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING)-1);
    }   

    // convert the vrf alpha string to a string
    for (count2 = 0, count = 0; count2 < (((RANDOM_STRING_LENGTH*2)*BLOCK_VERIFIERS_AMOUNT) + (BLOCK_HASH_LENGTH*2)) / 2; count2++, count += 2)
    {
      snprintf(VRF_data.vrf_alpha_string_data_round_part_4+count,BUFFER_SIZE-1,"%02x",VRF_data.vrf_alpha_string_round_part_4[count2] & 0xFF);
    }

    if (crypto_vrf_prove(VRF_data.vrf_proof_round_part_4,(const unsigned char*)VRF_data.vrf_secret_key_round_part_4,VRF_data.vrf_alpha_string_data_round_part_4,(unsigned long long)strlen((const char*)VRF_data.vrf_alpha_string_data_round_part_4)) != 0)
    {
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not create the vrf proof");
    }
    if (crypto_vrf_proof_to_hash(VRF_data.vrf_beta_string_round_part_4,(const unsigned char*)VRF_data.vrf_proof_round_part_4) != 0)
    {
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not create the vrf beta string");
    }
    if (crypto_vrf_verify(VRF_data.vrf_beta_string_round_part_4,(const unsigned char*)VRF_data.vrf_public_key_round_part_4,(const unsigned char*)VRF_data.vrf_proof_round_part_4,VRF_data.vrf_alpha_string_data_round_part_4,(unsigned long long)strlen((const char*)VRF_data.vrf_alpha_string_data_round_part_4)) != 0)
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
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_4,VRF_data.vrf_alpha_string_round_part_4,strnlen(VRF_data.vrf_alpha_string_round_part_4,BUFFER_SIZE));
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
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      sleep(BLOCK_VERIFIERS_SETTINGS);
      if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
      {  
        memset(send_and_receive_data_socket_thread_parameters[count].HOST,0,sizeof(send_and_receive_data_socket_thread_parameters[count].HOST));
        memset(send_and_receive_data_socket_thread_parameters[count].DATA,0,sizeof(send_and_receive_data_socket_thread_parameters[count].DATA));
        memcpy(send_and_receive_data_socket_thread_parameters[count].HOST,current_block_verifiers_list.block_verifiers_IP_address[count],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count],sizeof(send_and_receive_data_socket_thread_parameters[count].HOST)));
        memcpy(send_and_receive_data_socket_thread_parameters[count].DATA,data3,strnlen(data3,sizeof(send_and_receive_data_socket_thread_parameters[count].DATA)));
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
    snprintf(data3+14,sizeof(data3)-1,"%zu",count2);
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
    snprintf(data3+14,sizeof(data3)-1,"%zu",count2);
    if (insert_document_into_collection_json(DATABASE_NAME,data3,data2,0) == 0)
    {
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not add the new block to the database");
    }

    memset(data3,0,sizeof(data3));

    // create the verify_block file for the X-CASH Daemon
    if (get_path(data3,0) == 0 || write_file(VRF_data.reserve_bytes_data_hash,data3) == 0)
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
        memset(send_and_receive_data_socket_thread_parameters[count].DATA,0,sizeof(send_and_receive_data_socket_thread_parameters[count].DATA));
        memcpy(send_and_receive_data_socket_thread_parameters[count].HOST,current_block_verifiers_list.block_verifiers_IP_address[count],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count],sizeof(send_and_receive_data_socket_thread_parameters[count].HOST)));
        memcpy(send_and_receive_data_socket_thread_parameters[count].DATA,data3,strnlen(data3,sizeof(send_and_receive_data_socket_thread_parameters[count].DATA)));
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
    printf("\n");
    // wait for the block verifiers to process the votes
    do
    {
      usleep(200000);
      get_current_UTC_time;
    } while (current_UTC_date_and_time->tm_min % BLOCK_TIME != 4 || current_UTC_date_and_time->tm_sec != 50); 

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
      snprintf(data3+14,sizeof(data3)-1,"%zu",count2);
      delete_document_from_collection(DATABASE_NAME,data3,data2,0);
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not submit the block to the network");
    }
  }
  else
  {
    printf("Your block verifier is not the main data network node so your block verifier will wait until the network data node creates the block\n\n");

    // wait for the block verifiers to process the votes
    do
    {
      usleep(200000);
      get_current_UTC_time;
   } while (current_UTC_date_and_time->tm_min % BLOCK_TIME != 4 || current_UTC_date_and_time->tm_sec != 50); 
  }
  
  return 1;
  
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

int start_part_4_of_round()
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  size_t count = 0;
  size_t count2;
  size_t counter;
  struct send_data_socket_thread_parameters send_data_socket_thread_parameters[BLOCK_VERIFIERS_AMOUNT];
  int block_producer_backup_settings[BLOCK_PRODUCERS_BACKUP_AMOUNT];

  // threads
  pthread_t thread_id[BLOCK_VERIFIERS_AMOUNT];

  // define macros
  #define START_PART_4_OF_ROUND_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"start_part_4_of_round",21); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;  

  #define SEND_DATA_SOCKET_THREAD(message) \
  sleep(BLOCK_VERIFIERS_SETTINGS); \
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++) \
  { \
    if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0) \
    { \
      memset(send_data_socket_thread_parameters[count].HOST,0,sizeof(send_data_socket_thread_parameters[count].HOST)); \
      memset(send_data_socket_thread_parameters[count].DATA,0,sizeof(send_data_socket_thread_parameters[count].DATA)); \
      memcpy(send_data_socket_thread_parameters[count].HOST,current_block_verifiers_list.block_verifiers_IP_address[count],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count],sizeof(send_data_socket_thread_parameters[count].HOST))); \
      memcpy(send_data_socket_thread_parameters[count].DATA,message,strnlen(message,sizeof(send_data_socket_thread_parameters[count].DATA))); \
      pthread_create(&thread_id[count], NULL, &send_data_socket_thread,&send_data_socket_thread_parameters[count]); \
      pthread_detach(thread_id[count]); \
    } \
    if (count % (BLOCK_VERIFIERS_AMOUNT / 4) == 0 && count != 0 && count != BLOCK_VERIFIERS_AMOUNT) \
    { \
       usleep(500000); \
    } \
  }

  #define RESTART_ROUND(message) \
  printf("\n"); \
  color_print(message,"red"); \
  memset(data,0,sizeof(data)); \
  memset(data,0,sizeof(data)); \
  memset(data2,0,sizeof(data2)); \
  memset(data3,0,sizeof(data3)); \
  memset(VRF_data.vrf_secret_key_data_round_part_4,0,strlen(VRF_data.vrf_secret_key_data_round_part_4)); \
  memset(VRF_data.vrf_secret_key_round_part_4,0,strlen(VRF_data.vrf_secret_key_round_part_4)); \
  memset(VRF_data.vrf_public_key_data_round_part_4,0,strlen(VRF_data.vrf_public_key_data_round_part_4)); \
  memset(VRF_data.vrf_public_key_round_part_4,0,strlen(VRF_data.vrf_public_key_round_part_4)); \
  memset(VRF_data.vrf_alpha_string_data_round_part_4,0,strlen(VRF_data.vrf_alpha_string_data_round_part_4)); \
  memset(VRF_data.vrf_alpha_string_round_part_4,0,strlen(VRF_data.vrf_alpha_string_round_part_4)); \
  memset(VRF_data.vrf_proof_data_round_part_4,0,strlen(VRF_data.vrf_proof_data_round_part_4)); \
  memset(VRF_data.vrf_proof_round_part_4,0,strlen(VRF_data.vrf_proof_round_part_4)); \
  memset(VRF_data.vrf_beta_string_data_round_part_4,0,strlen(VRF_data.vrf_beta_string_data_round_part_4)); \
  memset(VRF_data.vrf_beta_string_round_part_4,0,strlen(VRF_data.vrf_beta_string_round_part_4)); \
  memset(VRF_data.block_blob,0,strlen(VRF_data.block_blob)); \
  memset(VRF_data.reserve_bytes_data_hash,0,strlen(VRF_data.reserve_bytes_data_hash)); \
  memset(current_round_part_vote_data.current_vote_results,0,sizeof(current_round_part_vote_data.current_vote_results)); \
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++) \
  { \
    memset(VRF_data.block_verifiers_vrf_secret_key_data[count],0,strlen(VRF_data.block_verifiers_vrf_secret_key_data[count])); \
    memset(VRF_data.block_verifiers_vrf_secret_key[count],0,strlen(VRF_data.block_verifiers_vrf_secret_key[count])); \
    memset(VRF_data.block_verifiers_vrf_public_key_data[count],0,strlen(VRF_data.block_verifiers_vrf_public_key_data[count])); \
    memset(VRF_data.block_verifiers_vrf_public_key[count],0,strlen(VRF_data.block_verifiers_vrf_public_key[count])); \
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
    print_start_message(data); \
    printf("\n"); \
  } \
  else if (memcmp(current_round_part_backup_node,"1",1) == 0) \
  { \
    memset(current_round_part,0,sizeof(current_round_part)); \
    memcpy(current_round_part,"1",1); \
    memset(current_round_part_backup_node,0,sizeof(current_round_part_backup_node)); \
    memcpy(current_round_part_backup_node,"2",1); \
    memcpy(data,"Restarting the round with backup block producer 2 for block ",60); \
    memcpy(data+60,current_block_height,strnlen(current_block_height,BUFFER_SIZE)); \
    print_start_message(data); \
    printf("\n"); \
  } \
  else if (memcmp(current_round_part_backup_node,"2",1) == 0) \
  { \
    data_network_node_create_block(); \
  } \
  pthread_rwlock_unlock(&rwlock); \
  sync_block_verifiers_seconds(0); \
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
  printf("\n");
  sync_block_verifiers_minutes(1);

  start:

    if ((memcmp(current_round_part_backup_node,"0",1) == 0 && memcmp(main_nodes_list.block_producer_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"1",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_1_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"2",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_2_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"3",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_3_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"4",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_4_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"5",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_5_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0))
    {
      color_print("Your block verifier is the block producer","yellow");
      printf("\n");
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
    SEND_DATA_SOCKET_THREAD(data3);

    // wait for the block verifiers to process the votes
    sync_block_verifiers_seconds(10);

    // process the data
    for (count = 0, count2 = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (strlen(VRF_data.block_verifiers_vrf_secret_key_data[count]) == VRF_SECRET_KEY_LENGTH && strlen(VRF_data.block_verifiers_vrf_public_key_data[count]) == VRF_PUBLIC_KEY_LENGTH && strlen(VRF_data.block_verifiers_random_data[count]) == RANDOM_STRING_LENGTH)
      {
        memcpy(VRF_data.block_verifiers_vrf_secret_key_data[count],VRF_data.block_verifiers_vrf_secret_key_data[count],VRF_SECRET_KEY_LENGTH);
        memcpy(VRF_data.block_verifiers_vrf_secret_key[count],VRF_data.block_verifiers_vrf_secret_key[count],crypto_vrf_SECRETKEYBYTES);
        memcpy(VRF_data.block_verifiers_vrf_public_key_data[count],VRF_data.block_verifiers_vrf_public_key_data[count],VRF_PUBLIC_KEY_LENGTH);
        memcpy(VRF_data.block_verifiers_vrf_public_key[count],VRF_data.block_verifiers_vrf_public_key[count],crypto_vrf_PUBLICKEYBYTES);
        memcpy(VRF_data.block_verifiers_random_data[count],VRF_data.block_verifiers_random_data[count],RANDOM_STRING_LENGTH);
        count2++;
      }
      else
      {
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
    memset(VRF_data.vrf_alpha_string_round_part_4,0,strlen(VRF_data.vrf_alpha_string_round_part_4));
    if (get_previous_block_hash(VRF_data.vrf_alpha_string_round_part_4,0) == 0)
    {
      START_PART_4_OF_ROUND_ERROR("Could not get the previous block hash");
    }
    
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (strlen(VRF_data.block_verifiers_vrf_secret_key[count]) == crypto_vrf_SECRETKEYBYTES && strlen(VRF_data.block_verifiers_vrf_public_key[count]) == crypto_vrf_PUBLICKEYBYTES && strlen(VRF_data.block_verifiers_random_data[count]) == RANDOM_STRING_LENGTH)
      {
        memcpy(VRF_data.vrf_alpha_string_round_part_4+strlen(VRF_data.vrf_alpha_string_round_part_4),VRF_data.block_verifiers_random_data[count],RANDOM_STRING_LENGTH);
      }
      else
      {
        memcpy(VRF_data.vrf_alpha_string_round_part_4+strlen(VRF_data.vrf_alpha_string_round_part_4),GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING)-1);
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

    if (crypto_vrf_prove(VRF_data.vrf_proof_round_part_4,(const unsigned char*)VRF_data.vrf_secret_key_round_part_4,VRF_data.vrf_alpha_string_data_round_part_4,(unsigned long long)strlen((const char*)VRF_data.vrf_alpha_string_data_round_part_4)) != 0)
    {
      START_PART_4_OF_ROUND_ERROR("Could not create the vrf proof");
    }
    if (crypto_vrf_proof_to_hash(VRF_data.vrf_beta_string_round_part_4,(const unsigned char*)VRF_data.vrf_proof_round_part_4) != 0)
    {
      START_PART_4_OF_ROUND_ERROR("Could not create the vrf beta string");
    }
    if (crypto_vrf_verify(VRF_data.vrf_beta_string_round_part_4,(const unsigned char*)VRF_data.vrf_public_key_round_part_4,(const unsigned char*)VRF_data.vrf_proof_round_part_4,VRF_data.vrf_alpha_string_data_round_part_4,(unsigned long long)strlen((const char*)VRF_data.vrf_alpha_string_data_round_part_4)) != 0)
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
      SEND_DATA_SOCKET_THREAD(data);
    }
    
    // wait for the block verifiers to process the votes
    sync_block_verifiers_seconds(30);



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
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_4,VRF_data.vrf_alpha_string_round_part_4,strnlen(VRF_data.vrf_alpha_string_round_part_4,BUFFER_SIZE));
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
    SEND_DATA_SOCKET_THREAD(data3);

    // wait for the block verifiers to process the votes
    sync_block_verifiers_seconds(40);



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
    snprintf(data3+14,sizeof(data3)-1,"%zu",count2);
    if (read_document_field_from_collection(DATABASE_NAME,data3,data,"reserve_bytes",data2,0) == 0)
    {
      START_PART_4_OF_ROUND_ERROR("Could not get the previous blocks reserve bytes");
    }

    // verify the block
    memset(data3,0,sizeof(data3));
    snprintf(data3,sizeof(data3)-1,"%zu",count);
    if (count+1 != XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT)
    {
      if (verify_network_block_data(1,1,1,"0",data2) == 0)
      {
        START_PART_4_OF_ROUND_ERROR("The MAIN_NODES_TO_NODES_PART_4_OF_ROUND message is invalid");
      }
    }
    else
    {
      if (verify_network_block_data(0,1,1,"0",data2) == 0)
      {
        START_PART_4_OF_ROUND_ERROR("The MAIN_NODES_TO_NODES_PART_4_OF_ROUND message is invalid");
      }
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
    sync_block_verifiers_seconds(45);

    // send the message to all block verifiers
    SEND_DATA_SOCKET_THREAD(data3);

    // wait for the block verifiers to process the votes
    sync_block_verifiers_seconds(55);

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
    snprintf(data3+14,sizeof(data3)-1,"%zu",count2);
    if (insert_document_into_collection_json(DATABASE_NAME,data3,data2,0) == 0)
    {
      START_PART_4_OF_ROUND_ERROR("Could not add the new block to the database");
    }

    memset(data3,0,sizeof(data3));

    // create the verify_block file for the X-CASH Daemon
    if (get_path(data3,0) == 0 || write_file(VRF_data.reserve_bytes_data_hash,data3) == 0)
    {
      START_PART_4_OF_ROUND_ERROR("Could not create the verify_block file");
    }
    sleep(BLOCK_VERIFIERS_SETTINGS);

    // wait for the block verifiers to process the votes
    color_print("Waiting for the block producer to submit the block to the network","blue");
    printf("\n");
    do
    {
      usleep(200000);
      get_current_UTC_time;
    } while (current_UTC_date_and_time->tm_min % BLOCK_TIME != 4 || current_UTC_date_and_time->tm_sec != 50);

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
        snprintf(data3+14,sizeof(data3)-1,"%zu",count2);
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
              snprintf(data3+14,sizeof(data3)-1,"%zu",count2);
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
    #undef SEND_DATA_SOCKET_THREAD
    #undef RESTART_ROUND
}



/*
-----------------------------------------------------------------------------------------------------------
Name: update_block_verifiers_list
Description: Updates the block verifiers list struct
Return: 0 if an error has occured, 1 to sync from a random block verifier, 2 to sync from a random network data node
-----------------------------------------------------------------------------------------------------------
*/

int update_block_verifiers_list()
{
  // Variables
  struct database_multiple_documents_fields database_multiple_documents_fields;
  size_t count;
  size_t count2;
  int settings = 0;

  // define macros
  #define DATABASE_COLLECTION "delegates"

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

  // initialize the database_multiple_documents_fields struct 
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    for (count2 = 0; count2 < TOTAL_DELEGATES_DATABASE_FIELDS; count2++)
    {
      database_multiple_documents_fields.item[count][count2] = (char*)calloc(BUFFER_SIZE,sizeof(char));
      database_multiple_documents_fields.value[count][count2] = (char*)calloc(BUFFER_SIZE,sizeof(char));

      if (database_multiple_documents_fields.item[count][count2] == NULL || database_multiple_documents_fields.value[count][count2] == NULL)
      {
        memcpy(error_message.function[error_message.total],"update_block_verifiers_list",27);
        memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
        error_message.total++;
        print_error_message;  
        exit(0);
      }
    }     
  } 
  database_multiple_documents_fields.document_count = 0;
  database_multiple_documents_fields.database_fields_count = 0;

  // get the top 150 delegates by total votes  
  if (read_multiple_documents_all_fields_from_collection(DATABASE_NAME,DATABASE_COLLECTION,"",&database_multiple_documents_fields,1,BLOCK_VERIFIERS_SELECTED_AMOUNT,1,"total_vote_count",0) == 0)
  {
    memcpy(error_message.function[error_message.total],"update_block_verifiers_list",27);
    memcpy(error_message.data[error_message.total],"Could not get the top 100 delegates for the next round. This means that you will not be able to particpate in the next round",163);
    error_message.total++;
    return 0;
  }

  // copy the database_multiple_documents_fields to the next_block_verifiers_list
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    if (strncmp(database_multiple_documents_fields.value[count][12],"true",BUFFER_SIZE) == 0)
    {
       memcpy(next_block_verifiers_list.block_verifiers_name[count],database_multiple_documents_fields.value[count][4],strnlen(database_multiple_documents_fields.value[count][4],sizeof(next_block_verifiers_list.block_verifiers_name[count])));
       memcpy(next_block_verifiers_list.block_verifiers_public_address[count],database_multiple_documents_fields.value[count][0],strnlen(database_multiple_documents_fields.value[count][0],sizeof(next_block_verifiers_list.block_verifiers_public_address[count])));
       memcpy(next_block_verifiers_list.block_verifiers_IP_address[count],database_multiple_documents_fields.value[count][3],strnlen(database_multiple_documents_fields.value[count][3],sizeof(next_block_verifiers_list.block_verifiers_IP_address[count])));
    } 
  }

  // reset the database_multiple_documents_fields
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    for (count2 = 0; count2 < TOTAL_DELEGATES_DATABASE_FIELDS; count2++)
    {
      pointer_reset(database_multiple_documents_fields.item[count][count2]);
      pointer_reset(database_multiple_documents_fields.value[count][count2]);
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

  return settings;

  #undef DATABASE_COLLECTION
}



/*
-----------------------------------------------------------------------------------------------------------
Name: update_databases
Description: Updates the databases
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int update_databases()
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  size_t count;
  size_t count2;

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
  size_t count2;
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
    snprintf(data+32,sizeof(data)-1,"%zu",number); 
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
    snprintf(data+39,sizeof(data)-1,"%zu",number); 
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
      snprintf(data+32,sizeof(data)-1,"%zu",number); 
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

int add_round_statistics()
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
  snprintf(message2+22,sizeof(message2)-1,"%zu",block_verifier_total_rounds_count);
  memcpy(message2+strlen(message2),"\"}",2);

  memcpy(message3,"{\"best_block_verifier_online_percentage_delegate_name\":\"",56);
  memcpy(message3+56,best_block_verifier_online_percentage_delegate_name,strnlen(best_block_verifier_online_percentage_delegate_name,sizeof(message3)));
  memcpy(message3+strlen(message3),"\"}",2);

  memcpy(message4,"{\"best_block_verifier_online_percentage\":\"",42);
  snprintf(message4+42,sizeof(message4)-1,"%.2lf",total);
  memcpy(message4+strlen(message4),"\"}",2);

  memcpy(message5,"{\"most_block_producer_total_rounds_delegate_name\":\"",51);
  memcpy(message5+51,most_block_producer_total_rounds_delegate_name,strnlen(most_block_producer_total_rounds_delegate_name,sizeof(message5)));
  memcpy(message5+strlen(message5),"\"}",2);

  memcpy(message6,"{\"most_block_producer_total_rounds\":\"",37);
  snprintf(message6+37,sizeof(message6)-1,"%zu",most_block_producer_total_rounds_count);
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
  size_t count;

  // define macros
  #define CHECK_IF_DATABASES_ARE_SYNCED_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"check_if_databases_are_synced",29); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));

  print_start_message("Checking if databases are synced");

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
Name: calculate_main_nodes_roles
Description: Calculates the main nodes roles for the round
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int calculate_main_nodes_roles()
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
  snprintf(data+14,sizeof(data)-1,"%d",count2);

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
        memset(main_nodes_list.block_producer_backup_block_verifier_1_public_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_1_public_address,XCASH_WALLET_LENGTH));
        memset(main_nodes_list.block_producer_backup_block_verifier_1_IP_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_1_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_1_public_address,current_block_verifiers_list.block_verifiers_public_address[count2],strnlen(current_block_verifiers_list.block_verifiers_public_address[count2],sizeof(main_nodes_list.block_producer_backup_block_verifier_1_public_address)));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_1_IP_address,current_block_verifiers_list.block_verifiers_IP_address[count2],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count2],sizeof(main_nodes_list.block_producer_backup_block_verifier_1_IP_address)));
      }
      if (main_nodes_count == 2)
      {
        // calculate the block_producer_backup_block_verifier_2
        memset(main_nodes_list.block_producer_backup_block_verifier_2_public_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_2_public_address,XCASH_WALLET_LENGTH));
        memset(main_nodes_list.block_producer_backup_block_verifier_2_IP_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_2_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_2_public_address,current_block_verifiers_list.block_verifiers_public_address[count2],strnlen(current_block_verifiers_list.block_verifiers_public_address[count2],sizeof(main_nodes_list.block_producer_backup_block_verifier_2_public_address)));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_2_IP_address,current_block_verifiers_list.block_verifiers_IP_address[count2],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count2],sizeof(main_nodes_list.block_producer_backup_block_verifier_2_IP_address)));
      }
      if (main_nodes_count == 3)
      {
        // calculate the block_producer_backup_block_verifier_3
        memset(main_nodes_list.block_producer_backup_block_verifier_3_public_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_3_public_address,XCASH_WALLET_LENGTH));
        memset(main_nodes_list.block_producer_backup_block_verifier_3_IP_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_3_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_3_public_address,current_block_verifiers_list.block_verifiers_public_address[count2],strnlen(current_block_verifiers_list.block_verifiers_public_address[count2],sizeof(main_nodes_list.block_producer_backup_block_verifier_3_public_address)));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_3_IP_address,current_block_verifiers_list.block_verifiers_IP_address[count2],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count2],sizeof(main_nodes_list.block_producer_backup_block_verifier_3_IP_address)));
      }
      if (main_nodes_count == 4)
      {
        // calculate the block_producer_backup_block_verifier_4
        memset(main_nodes_list.block_producer_backup_block_verifier_4_public_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_4_public_address,XCASH_WALLET_LENGTH));
        memset(main_nodes_list.block_producer_backup_block_verifier_4_IP_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_4_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_4_public_address,current_block_verifiers_list.block_verifiers_public_address[count2],strnlen(current_block_verifiers_list.block_verifiers_public_address[count2],sizeof(main_nodes_list.block_producer_backup_block_verifier_4_public_address)));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_4_IP_address,current_block_verifiers_list.block_verifiers_IP_address[count2],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count2],sizeof(main_nodes_list.block_producer_backup_block_verifier_4_IP_address)));
      }
      if (main_nodes_count == 5)
      {
        // calculate the block_producer_backup_block_verifier_5
        memset(main_nodes_list.block_producer_backup_block_verifier_5_public_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_5_public_address,XCASH_WALLET_LENGTH));
        memset(main_nodes_list.block_producer_backup_block_verifier_5_IP_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_5_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
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
        memset(main_nodes_list.block_producer_public_address,0,sizeof(main_nodes_list.block_producer_public_address,XCASH_WALLET_LENGTH));
        memset(main_nodes_list.block_producer_IP_address,0,sizeof(main_nodes_list.block_producer_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
        memcpy(main_nodes_list.block_producer_public_address,network_data_nodes_list.network_data_nodes_public_address[count],strnlen(network_data_nodes_list.network_data_nodes_public_address[count],sizeof(main_nodes_list.block_producer_public_address)));
        memcpy(main_nodes_list.block_producer_IP_address,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],sizeof(main_nodes_list.block_producer_IP_address)));
      } 
      if (strncmp(main_nodes_list.block_producer_backup_block_verifier_1_public_address,"",1) == 0)
      {
        count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT);
        memset(main_nodes_list.block_producer_backup_block_verifier_1_public_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_1_public_address,XCASH_WALLET_LENGTH));
        memset(main_nodes_list.block_producer_backup_block_verifier_1_IP_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_1_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_1_public_address,network_data_nodes_list.network_data_nodes_public_address[count],strnlen(network_data_nodes_list.network_data_nodes_public_address[count],sizeof(main_nodes_list.block_producer_backup_block_verifier_1_public_address)));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_1_IP_address,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],sizeof(main_nodes_list.block_producer_backup_block_verifier_1_IP_address)));
      } 
      if (strncmp(main_nodes_list.block_producer_backup_block_verifier_2_public_address,"",1) == 0)
      {
        count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT);
        memset(main_nodes_list.block_producer_backup_block_verifier_2_public_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_2_public_address,XCASH_WALLET_LENGTH));
        memset(main_nodes_list.block_producer_backup_block_verifier_2_IP_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_2_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_2_public_address,network_data_nodes_list.network_data_nodes_public_address[count],strnlen(network_data_nodes_list.network_data_nodes_public_address[count],sizeof(main_nodes_list.block_producer_backup_block_verifier_2_public_address)));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_2_IP_address,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],sizeof(main_nodes_list.block_producer_backup_block_verifier_2_IP_address)));
      } 
      if (strncmp(main_nodes_list.block_producer_backup_block_verifier_3_public_address,"",1) == 0)
      {
        count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT);
        memset(main_nodes_list.block_producer_backup_block_verifier_3_public_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_3_public_address,XCASH_WALLET_LENGTH));
        memset(main_nodes_list.block_producer_backup_block_verifier_3_IP_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_3_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_3_public_address,network_data_nodes_list.network_data_nodes_public_address[count],strnlen(network_data_nodes_list.network_data_nodes_public_address[count],sizeof(main_nodes_list.block_producer_backup_block_verifier_3_public_address)));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_3_IP_address,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],sizeof(main_nodes_list.block_producer_backup_block_verifier_3_IP_address)));
      } 
      if (strncmp(main_nodes_list.block_producer_backup_block_verifier_4_public_address,"",1) == 0)
      {
        count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT);
        memset(main_nodes_list.block_producer_backup_block_verifier_4_public_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_4_public_address,XCASH_WALLET_LENGTH));
        memset(main_nodes_list.block_producer_backup_block_verifier_4_IP_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_4_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_4_public_address,network_data_nodes_list.network_data_nodes_public_address[count],strnlen(network_data_nodes_list.network_data_nodes_public_address[count],sizeof(main_nodes_list.block_producer_backup_block_verifier_4_public_address)));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_4_IP_address,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],sizeof(main_nodes_list.block_producer_backup_block_verifier_4_IP_address)));
      }   
      if (strncmp(main_nodes_list.block_producer_backup_block_verifier_5_public_address,"",1) == 0)
      {
        count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT);
        memset(main_nodes_list.block_producer_backup_block_verifier_5_public_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_5_public_address,XCASH_WALLET_LENGTH));
        memset(main_nodes_list.block_producer_backup_block_verifier_5_IP_address,0,sizeof(main_nodes_list.block_producer_backup_block_verifier_5_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
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
Name: server_received_data_xcash_proof_of_stake_test_data
Description: Runs the code when the server receives the xcash_proof_of_stake_test_data message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_received_data_xcash_proof_of_stake_test_data(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // verify the message
  if (verify_data(MESSAGE,0,1) == 0)
  {   
    return 0;
  }
  else
  {
    if (send_data(CLIENT_SOCKET,(char*)MESSAGE,0,1,"") == 1)
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

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_GET_FILES_ERROR \
  memcpy(data,"{\"Error\":\"Could not get the file\"}",34); \
  send_data(CLIENT_SOCKET,data,strlen(data),400,"application/json"); \
  pointer_reset(data); \
  return 0;

  memset(data2,0,sizeof(data2));

  // get the file
  memcpy(data2,&MESSAGE[(strlen(MESSAGE) - strlen(strstr(MESSAGE,"GET /")))+5],(strlen(MESSAGE) - strlen(strstr(MESSAGE," HTTP/"))) - ((strlen(MESSAGE) - strlen(strstr(MESSAGE,"GET /")))+5));
  file_size = read_file(data,data2);
  if (file_size == 0)
  {
    file_size = read_file(data,"index.html");
    if (file_size == 0)
    {
      SERVER_RECEIVE_DATA_SOCKET_GET_FILES_ERROR;
    }
  }

  memset(data2,0,sizeof(data2));

  if (strstr(MESSAGE,".html") != NULL)
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
      SERVER_RECEIVE_DATA_SOCKET_GET_STATISTICS_ERROR(1);
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
        SERVER_RECEIVE_DATA_SOCKET_GET_STATISTICS_ERROR(1);
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
  sprintf(data,"%zu",count);

  memcpy(database_data.item[database_data.count],"xcash_proof_of_stake_round_number",33);
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
  sprintf(data,"%zu",count2);

  memcpy(database_data.item[database_data.count],"total_votes",11);
  memcpy(database_data.value[database_data.count],data,strnlen(data,BUFFER_SIZE));
  database_data.count++;

  // get the xcash_proof_of_stake_circulating_percentage
  generated_supply = 190734.9 + XCASH_PREMINE_TOTAL_SUPPLY;
  for (counter = 2; counter < current_block_height; counter++)
  { 
    generated_supply += (XCASH_TOTAL_SUPPLY - generated_supply) / 524288;
  }  

  number = ((size_t)((((double)count2) / (generated_supply - (XCASH_PREMINE_TOTAL_SUPPLY - XCASH_PREMINE_CIRCULATING_SUPPLY))) * 100) | 0);

  memset(data,0,strlen(data));
  sprintf(data,"%zu",number);

  memcpy(database_data.item[database_data.count],"xcash_proof_of_stake_circulating_percentage",43);
  memcpy(database_data.value[database_data.count],data,strnlen(data,BUFFER_SIZE));
  database_data.count++;
  
  memset(data,0,strnlen(data,BUFFER_SIZE));

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
        SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_ERROR(1);
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
  #define DATABASE_FIELDS ""

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

  // get the parameter1
  memcpy(data2,&DATA[28],(strnlen(DATA,sizeof(data2)) - strnlen(strstr(DATA," HTTP/"),sizeof(data2)))-28);

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
      SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_STATISTICS_ERROR(1);
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
        SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_STATISTICS_ERROR(1);
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
  sprintf(data2, "%zu", counter);
  memcpy(database_data.item[18],"current_delegate_rank",21);
  memcpy(database_data.value[18],data2,strnlen(data2,BUFFER_SIZE));
  database_data.count++;

  memset(message,0,strnlen(message,BUFFER_SIZE)); 

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
Name: server_receive_data_socket_node_to_network_data_nodes_get_previous_current_next_block_verifiers_list
Description: Runs the code when the server receives the NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST message
Parameters:
  CLIENT_SOCKET - The socket to send data to
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_node_to_network_data_nodes_get_previous_current_next_block_verifiers_list(const int CLIENT_SOCKET)
{
  // Variables
  char data[BUFFER_SIZE];
  size_t count;
  size_t count2;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_node_to_network_data_nodes_get_previous_current_next_block_verifiers_list",100); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));

  // create the message
  memcpy(data,"{\r\n \"message_settings\": \"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST\",\r\n \"previous_block_verifiers_name_list\": \"",142);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),previous_block_verifiers_list.block_verifiers_name[count2],strnlen(previous_block_verifiers_list.block_verifiers_name[count2],sizeof(data)));
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n \"previous_block_verifiers_public_address_list\": \"",54);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),previous_block_verifiers_list.block_verifiers_public_address[count2],strnlen(previous_block_verifiers_list.block_verifiers_public_address[count2],sizeof(data)));
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n \"previous_block_verifiers_IP_address_list\": \"",50);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),previous_block_verifiers_list.block_verifiers_IP_address[count2],strnlen(previous_block_verifiers_list.block_verifiers_IP_address[count2],sizeof(data)));
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n \"current_block_verifiers_name_list\": \"",43);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),current_block_verifiers_list.block_verifiers_name[count2],strnlen(current_block_verifiers_list.block_verifiers_name[count2],sizeof(data)));
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n \"current_block_verifiers_public_address_list\": \"",54);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),current_block_verifiers_list.block_verifiers_public_address[count2],strnlen(current_block_verifiers_list.block_verifiers_public_address[count2],sizeof(data)));
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n \"current_block_verifiers_IP_address_list\": \"",50);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),current_block_verifiers_list.block_verifiers_IP_address[count2],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count2],sizeof(data)));
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n \"next_block_verifiers_name_list\": \"",43);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),next_block_verifiers_list.block_verifiers_name[count2],strnlen(next_block_verifiers_list.block_verifiers_name[count2],sizeof(data)));
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n \"next_block_verifiers_public_address_list\": \"",54);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),next_block_verifiers_list.block_verifiers_public_address[count2],strnlen(next_block_verifiers_list.block_verifiers_public_address[count2],sizeof(data)));
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n \"next_block_verifiers_IP_address_list\": \"",50);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),next_block_verifiers_list.block_verifiers_IP_address[count2],strnlen(next_block_verifiers_list.block_verifiers_IP_address[count2],sizeof(data)));
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n}",5);
  
  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,data,0,1,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_ERROR("Could not send the NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST message to the block verifier");
  }
  return 1;

  #undef SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_node_to_network_data_nodes_get_current_block_verifiers_list
Description: Runs the code when the server receives the NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST message
Parameters:
  CLIENT_SOCKET - The socket to send data to
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_node_to_network_data_nodes_get_current_block_verifiers_list(const int CLIENT_SOCKET)
{
  // Variables
  char data[BUFFER_SIZE];
  size_t count;
  size_t count2;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_node_to_network_data_nodes_get_current_block_verifiers_list",86); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;
  
  memset(data,0,sizeof(data));

  // create the message
  memcpy(data,"{\r\n \"message_settings\": \"NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST\",\r\n \"block_verifiers_public_address_list\": \"",129);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),current_block_verifiers_list.block_verifiers_public_address[count2],XCASH_WALLET_LENGTH);
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n \"block_verifiers_IP_address_list\": \"",41);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),current_block_verifiers_list.block_verifiers_IP_address[count2],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count2],sizeof(data)));
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n}",5);
  
  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,data,0,1,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST_ERROR("Could not send the NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST message to the block verifier");
  }
  return 1;

  #undef SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_node_to_network_data_nodes_get_current_block_verifiers_list
Description: Runs the code when the server receives the NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_nodes_to_block_verifiers_reserve_bytes_database_sync_check_all_update(const int CLIENT_SOCKET)
{
   // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char message[BUFFER_SIZE];
  size_t count;
  size_t count2;

  // define macros
  #define DATABASE_COLLECTION "reserve_bytes"
  #define SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_nodes_to_block_verifiers_reserve_bytes_database_sync_check_all_update",96); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  send_data(CLIENT_SOCKET,"Could not get the reserve bytes data hash}",0,0,""); \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));

  // get the database data hash for the reserve bytes database
  if (get_database_data_hash(data2,DATABASE_NAME,DATABASE_COLLECTION,0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not get the database data hash for the reserve bytes database");
  }

  // create the message
  memcpy(data,"BLOCK_VERIFIERS_TO_NODES_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD|",72);
  memcpy(data+72,data2,DATA_HASH_LENGTH);
  memcpy(data+200,"|",1);
  
  // sign_data
  memset(data2,0,sizeof(data2));
  memcpy(message,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"sign\",\"params\":{\"data\":\"",60);
  memcpy(message+60,data,strnlen(data,sizeof(message)));
  memcpy(message+strlen(message),"\"}}",3);

  if (send_http_request(data2,"127.0.0.1","/json_rpc",XCASH_WALLET_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,message,RECEIVE_DATA_TIMEOUT_SETTINGS,"sign data",0) <= 0)
  {  
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not create the message");
  } 

  memset(message,0,sizeof(message));

  if (parse_json_data(data2,"signature",message,sizeof(message)) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not create the message");
  }

  // check if the returned data is valid
  if (strlen(message) != XCASH_SIGN_DATA_LENGTH && strncmp(message,XCASH_SIGN_DATA_PREFIX,sizeof(XCASH_SIGN_DATA_PREFIX)-1) != 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not create the message");
  }

  memcpy(data+strlen(data),xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"|",1);
  memcpy(data+strlen(data),message,XCASH_SIGN_DATA_LENGTH);
  memcpy(data+strlen(data),"|}",2);

  // send the data
  if (send_data(CLIENT_SOCKET,data,0,0,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not send the BLOCK_VERIFIERS_TO_NODES_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD message to the node");
  }
  return 1;

  #undef DATABASE_COLLECTION
  #undef SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_node_to_block_verifiers_get_reserve_bytes
Description: Runs the code when the server receives the NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES message
Parameters:
  CLIENT_SOCKET - The socket to send data to
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_node_to_block_verifiers_get_reserve_bytes(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char message[BUFFER_SIZE];
  char message2[BUFFER_SIZE];
  size_t count;
  size_t count2;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_node_to_block_verifiers_get_reserve_bytes",68); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  send_data(CLIENT_SOCKET,"Could not get the network blocks reserve bytes}",0,0,""); \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));
  memset(message2,0,sizeof(message2));
  
  if (parse_json_data(MESSAGE,"block_height",data,sizeof(data)) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_ERROR("Could not create the message");
  }

  // create the message
  memcpy(data2,"{\"block_height\": \"",18);
  memcpy(data2+18,data,strnlen(data,sizeof(data2)));
  memcpy(data2+strlen(data2),"\"}",2);

  sscanf(data,"%zu", &count);
  if (count < XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT-1)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_ERROR("Could not get the current block height");
  }
  count2 = ((count - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME) + 1;
  memset(data,0,sizeof(data));
  memcpy(data,"reserve_bytes_",14);
  snprintf(data+14,sizeof(data)-1,"%zu",count2);

  // get the data hash
  if (read_document_field_from_collection(DATABASE_NAME,data,data2,"reserve_bytes",message,0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_ERROR("Could not get the previous blocks reserve bytes");
  }

  // create the message
  memcpy(message2,"BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES|",43);
  memcpy(message2+43,message,strnlen(message,sizeof(message2)));
  memcpy(message2+strlen(message2),"|",1);
  
  // sign_data
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memcpy(data,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"sign\",\"params\":{\"data\":\"",60);
  memcpy(data+60,message2,strnlen(message2,sizeof(data)));
  memcpy(data+strlen(data),"\"}}",3);

  if (send_http_request(data2,"127.0.0.1","/json_rpc",XCASH_WALLET_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,data,RECEIVE_DATA_TIMEOUT_SETTINGS,"sign data",0) <= 0)
  {  
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_ERROR("Could not create the message");
  } 

  memset(data,0,sizeof(data));

  if (parse_json_data(data2,"signature",data,sizeof(data)) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_ERROR("Could not create the message");
  }

  // check if the returned data is valid
  if (strlen(data) != XCASH_SIGN_DATA_LENGTH && strncmp(data,XCASH_SIGN_DATA_PREFIX,sizeof(XCASH_SIGN_DATA_PREFIX)-1) != 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_ERROR("Could not create the message");
  }

  memcpy(message2+strlen(message2),xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(message2+strlen(message2),"|",1);
  memcpy(message2+strlen(message2),data,XCASH_SIGN_DATA_LENGTH);
  memcpy(message2+strlen(message2),"|}",2);

  // send the data
  if (send_data(CLIENT_SOCKET,message2,0,0,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_ERROR("Could not send the BLOCK_VERIFIERS_TO_NODES_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD message to the node");
  }
  return 1;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_all_update
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_all_update(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char message[BUFFER_SIZE];
  char reserve_proofs_database[BUFFER_SIZE];
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  size_t count;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_all_update",107); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(message,0,sizeof(message));
  memset(reserve_proofs_database,0,sizeof(reserve_proofs_database));
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not verify the message");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"reserve_proofs_data_hash",data,sizeof(data)) == 0 || strlen(data) != DATA_HASH_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not parse the message");
  }

  // get the database data hash for the reserve proofs database
  memset(data2,0,strlen(data2));
  if (get_database_data_hash(data2,DATABASE_NAME,"reserve_proofs",0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not get the database data hash for the reserve proofs database");
  }

  // create the message
  if (memcmp(data,data2,DATA_HASH_LENGTH) == 0)
  {
    memset(message,0,strlen(message));
    memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"reserve_proofs_database\": \"true\",\r\n \"reserve_proofs_database_1\": \"true\",\r\n \"reserve_proofs_database_2\": \"true\",\r\n \"reserve_proofs_database_3\": \"true\",\r\n \"reserve_proofs_database_4\": \"true\",\r\n \"reserve_proofs_database_5\": \"true\",\r\n \"reserve_proofs_database_6\": \"true\",\r\n \"reserve_proofs_database_7\": \"true\",\r\n \"reserve_proofs_database_8\": \"true\",\r\n \"reserve_proofs_database_9\": \"true\",\r\n \"reserve_proofs_database_10\": \"true\",\r\n \"reserve_proofs_database_11\": \"true\",\r\n \"reserve_proofs_database_12\": \"true\",\r\n \"reserve_proofs_database_13\": \"true\",\r\n \"reserve_proofs_database_14\": \"true\",\r\n \"reserve_proofs_database_15\": \"true\",\r\n \"reserve_proofs_database_16\": \"true\",\r\n \"reserve_proofs_database_17\": \"true\",\r\n \"reserve_proofs_database_18\": \"true\",\r\n \"reserve_proofs_database_19\": \"true\",\r\n \"reserve_proofs_database_20\": \"true\",\r\n \"reserve_proofs_database_21\": \"true\",\r\n \"reserve_proofs_database_22\": \"true\",\r\n \"reserve_proofs_database_23\": \"true\",\r\n \"reserve_proofs_database_24\": \"true\",\r\n \"reserve_proofs_database_25\": \"true\",\r\n \"reserve_proofs_database_26\": \"true\",\r\n \"reserve_proofs_database_27\": \"true\",\r\n \"reserve_proofs_database_28\": \"true\",\r\n \"reserve_proofs_database_29\": \"true\",\r\n \"reserve_proofs_database_30\": \"true\",\r\n \"reserve_proofs_database_31\": \"true\",\r\n \"reserve_proofs_database_32\": \"true\",\r\n \"reserve_proofs_database_33\": \"true\",\r\n \"reserve_proofs_database_34\": \"true\",\r\n \"reserve_proofs_database_35\": \"true\",\r\n \"reserve_proofs_database_36\": \"true\",\r\n \"reserve_proofs_database_37\": \"true\",\r\n \"reserve_proofs_database_38\": \"true\",\r\n \"reserve_proofs_database_39\": \"true\",\r\n \"reserve_proofs_database_40\": \"true\",\r\n \"reserve_proofs_database_41\": \"true\",\r\n \"reserve_proofs_database_42\": \"true\",\r\n \"reserve_proofs_database_43\": \"true\",\r\n \"reserve_proofs_database_44\": \"true\",\r\n \"reserve_proofs_database_45\": \"true\",\r\n \"reserve_proofs_database_46\": \"true\",\r\n \"reserve_proofs_database_47\": \"true\",\r\n \"reserve_proofs_database_48\": \"true\",\r\n \"reserve_proofs_database_49\": \"true\",\r\n \"reserve_proofs_database_50\": \"true\",\r\n}",2140);
  }
  else
  {
    memset(message,0,sizeof(message));
    memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"reserve_proofs_database\": \"false\",\r\n ",150);
    for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
    {
      memcpy(message+strlen(message),"\"reserve_proofs_database_",25);
      snprintf(message+strlen(message),sizeof(message)-1,"%zu",count);
      memcpy(message+strlen(message),"\": \"",4);      
      memset(data2,0,strlen(data2));  
      memcpy(data2,"reserve_proofs_data_hash_",25);  
      snprintf(data2+25,sizeof(data2)-1,"%zu",count); 
      // parse the message
      if (parse_json_data(MESSAGE,data2,data,sizeof(data)) == 0 || strlen(data) != DATA_HASH_LENGTH)
      {
        SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not parse the message");
      }
      // get the database data hash for the reserve proofs database
      memset(data2,0,sizeof(data2));  
      memcpy(data2,"reserve_proofs_",15);  
      snprintf(data2+15,sizeof(data2)-1,"%zu",count);
      if (get_database_data_hash(reserve_proofs_database,DATABASE_NAME,data2,0) == 0)
      {
        SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not get the database data hash for the reserve proofs database");
      }
      if (memcmp(reserve_proofs_database,data,DATA_HASH_LENGTH) == 0)
      {
        memcpy(message+strlen(message),"true",4);
      }
      else
      {
        memcpy(message+strlen(message),"false",5);
      }
      memcpy(message+strlen(message),"\",\r\n",4);
    }
    memcpy(message+strlen(message),"}",1);
  }
  
  // sign_data
  if (sign_data(message,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,message,0,1,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not send the NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST message to the block verifier");
  }
  return 1;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_update
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_update(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];

  // define macros
  #define DATABASE_MESSAGE_SYNCED_TRUE "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"reserve_proofs_database\": \"true\",\r\n}"
  #define DATABASE_MESSAGE_SYNCED_FALSE "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"reserve_proofs_database\": \"false\",\r\n}"
  
  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_update",103); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not verify the message");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"file",data,sizeof(data)) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not parse the message");
  }

  // get the database data hash for the reserve proofs database
  memset(data2,0,sizeof(data2));
  if (get_database_data_hash(data2,DATABASE_NAME,data,0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not get the database data hash for the reserve proofs database");
  }

  // parse the message
  memset(data,0,sizeof(data));
  if (parse_json_data(MESSAGE,"data_hash",data,sizeof(data)) == 0 || strlen(data) != DATA_HASH_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not parse the message");
  }

  // create the message
  if (strncmp(data,data2,DATA_HASH_LENGTH) == 0)
  {
    memset(data,0,strlen(data));
    memcpy(data,DATABASE_MESSAGE_SYNCED_TRUE,sizeof(DATABASE_MESSAGE_SYNCED_TRUE,BUFFER_SIZE)-1);
  }
  else
  {
    memset(data,0,strlen(data));
    memcpy(data,DATABASE_MESSAGE_SYNCED_FALSE,sizeof(DATABASE_MESSAGE_SYNCED_FALSE,BUFFER_SIZE)-1);
  }
  
  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,data,0,1,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not send the NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST message to the block verifier");
  }

  return 1;

  #undef DATABASE_MESSAGE_SYNCED_TRUE
  #undef DATABASE_MESSAGE_SYNCED_FALSE
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_download_file_update
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_download_file_update(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_download_file_update",106); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_download_file_update",106);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not verify the message");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"file",data,MAXIMUM_BUFFER_SIZE) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not parse the message");
  }

  // get the database data for the reserve proofs database
  memset(data2,0,strlen(data2));
  if (get_database_data(data2,DATABASE_NAME,data,0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not get the database data hash for the reserve proofs database");
  }

  // create the message
  memset(data,0,strlen(data));
  memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"reserve_proofs_database\": \"",139);
  memcpy(data+139,data2,strnlen(data2,MAXIMUM_BUFFER_SIZE));
  memcpy(data+strlen(data),"\",\r\n}",5);

  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,data,0,1,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not send the NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST message to the block verifier");
  }

  return 1;

  #undef pointer_reset_all
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_all_update
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_all_update(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_all_update",106); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not verify the message");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"data_hash",data,sizeof(data)) == 0 || strlen(data) != DATA_HASH_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not parse the message");
  }

  // get the database data hash for the reserve bytes database
  memset(data2,0,sizeof(data2));
  if (get_database_data_hash(data2,DATABASE_NAME,"reserve_bytes",0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not get the database data hash for the reserve bytes database");
  }

  // create the message
  if (memcmp(data,data2,DATA_HASH_LENGTH) == 0)
  {
    memset(data,0,sizeof(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"reserve_bytes_database\": \"true\",\r\n}",147);
  }
  else
  {
    memset(data,0,sizeof(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"reserve_bytes_database\": \"false\",\r\n}",148);
  }
  
  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,data,0,1,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not send the NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST message to the block verifier");
  }

  return 1;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_update
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_update(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];

  // define macros
  #define DATABASE_MESSAGE_SYNCED_TRUE "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"reserve_bytes_database\": \"true\",\r\n}"
  #define DATABASE_MESSAGE_SYNCED_FALSE "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"reserve_bytes_database\": \"false\",\r\n}"
  
  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_update",102); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not verify the message");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"file",data,sizeof(data)) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not parse the message");
  }

  // get the database data hash for the reserve bytes database
  memset(data2,0,sizeof(data2));
  if (get_database_data_hash(data2,DATABASE_NAME,data,0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not get the database data hash for the reserve bytes database");
  }

  // parse the message
  memset(data,0,sizeof(data));
  if (parse_json_data(MESSAGE,"data_hash",data,sizeof(data)) == 0 || strlen(data) != DATA_HASH_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not parse the message");
  }

  // create the message
  if (memcmp(data,data2,DATA_HASH_LENGTH) == 0)
  {
    memset(data,0,sizeof(data));
    memcpy(data,DATABASE_MESSAGE_SYNCED_TRUE,strnlen(DATABASE_MESSAGE_SYNCED_TRUE,BUFFER_SIZE));
  }
  else
  {
    memset(data,0,sizeof(data));
    memcpy(data,DATABASE_MESSAGE_SYNCED_FALSE,strnlen(DATABASE_MESSAGE_SYNCED_FALSE,BUFFER_SIZE));
  }
  
  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,data,0,1,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not send the NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST message to the block verifier");
  }

  return 1;

  #undef DATABASE_MESSAGE_SYNCED_TRUE
  #undef DATABASE_MESSAGE_SYNCED_FALSE
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_download_file_update
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_download_file_update(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_download_file_update",105); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_download_file_update",105);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not verify the message");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"file",data,MAXIMUM_BUFFER_SIZE) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not parse the message");
  }

  // get the database data for the reserve bytes database
  memset(data2,0,strlen(data2));
  if (get_database_data(data2,DATABASE_NAME,data,0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not get the database data hash for the reserve bytes database");
  }

  // create the message
  memset(data,0,strlen(data));
  memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"reserve_bytes_database\": \"",137);
  memcpy(data+137,data2,strnlen(data2,MAXIMUM_BUFFER_SIZE));
  memcpy(data+strlen(data),"\",\r\n}",5);

  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,data,0,1,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not send the NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST message to the block verifier");
  }

  return 1;

  #undef pointer_reset_all
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_sync_check_update
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_sync_check_update(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_sync_check_update",98); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not verify the message");
  }

  // get the database data hash for the delegates database
  memset(data2,0,strlen(data2));
  if (get_database_data_hash(data2,DATABASE_NAME,DATABASE_COLLECTION,0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not get the database data hash for the reserve bytes database");
  }

  // parse the message
  memset(data,0,sizeof(data));
  if (parse_json_data(MESSAGE,"data_hash",data,sizeof(data)) == 0 || strlen(data) != DATA_HASH_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not parse the message");
  }

  // create the message
  if (memcmp(data,data2,DATA_HASH_LENGTH) == 0)
  {
    memset(data,0,sizeof(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"delegates_database\": \"true\",\r\n}",135);
  }
  else
  {
    memset(data,0,sizeof(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"delegates_database\": \"false\",\r\n}",136);
  }
  
  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,data,0,1,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not send the NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST message to the block verifier");
  }

  return 1;

  #undef DATABASE_COLLECTION
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_download_file_update
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_download_file_update(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_download_file_update",101); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_download_file_update",101);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not verify the message");
  }

  // get the database data for the reserve bytes database
  memset(data2,0,strlen(data2));
  if (get_database_data(data2,DATABASE_NAME,DATABASE_COLLECTION,0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not get the database data hash for the reserve bytes database");
  }

  // create the message
  memset(data,0,strlen(data));
  memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"delegates_database\": \"",129);
  memcpy(data+129,data2,strnlen(data2,MAXIMUM_BUFFER_SIZE));
  memcpy(data+strlen(data),"\",\r\n}",5);

  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,data,0,1,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not send the NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST message to the block verifier");
  }

  return 1;

  #undef DATABASE_COLLECTION
  #undef pointer_reset_all
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_sync_check_update
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_sync_check_update(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];

  // define macros
  #define DATABASE_COLLECTION "statistics"
  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_sync_check_update",99); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not verify the message");
  }

  // get the database data hash for the statistics database
  memset(data2,0,sizeof(data2));
  if (get_database_data_hash(data2,DATABASE_NAME,DATABASE_COLLECTION,0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not get the database data hash for the reserve bytes database");
  }

  // parse the message
  memset(data,0,sizeof(data));
  if (parse_json_data(MESSAGE,"data_hash",data,sizeof(data)) == 0 || strlen(data) != DATA_HASH_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not parse the message");
  }

  // create the message
  if (memcmp(data,data2,DATA_HASH_LENGTH) == 0)
  {
    memset(data,0,sizeof(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"statistics_database\": \"true\",\r\n}",137);
  }
  else
  {
    memset(data,0,sizeof(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"statistics_database\": \"false\",\r\n}",138);
  }
  
  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,data,0,1,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not send the NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST message to the block verifier");
  }

  return 1;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_download_file_update
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_download_file_update(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));

  // define macros
  #define DATABASE_COLLECTION "statistics"
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_download_file_update",102); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_download_file_update",102);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not verify the message");
  }

  // get the database data for the reserve bytes database
  memset(data2,0,strlen(data2));
  if (get_database_data(data2,DATABASE_NAME,DATABASE_COLLECTION,0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not get the database data hash for the reserve bytes database");
  }

  // create the message
  memset(data,0,strlen(data));
  memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"statistics_database\": \"",131);
  memcpy(data+131,data2,strnlen(data2,MAXIMUM_BUFFER_SIZE));
  memcpy(data+strlen(data),"\",\r\n}",5);

  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,data,0,1,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not send the NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST message to the block verifier");
  }

  return 1;

  #undef DATABASE_COLLECTION
  #undef pointer_reset_all
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_node_to_block_verifiers_add_reserve_proof
Description: Runs the code when the server receives the NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_node_to_block_verifiers_add_reserve_proof(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char delegates_public_address[BUFFER_SIZE];
  char public_address[BUFFER_SIZE];
  char reserve_proof[BUFFER_SIZE];
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  size_t count;
  size_t count2;
  int settings = 0;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_node_to_block_verifiers_add_reserve_proof",68); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  send_data(CLIENT_SOCKET,"Could not add the reserve proof to the database}",0,0,""); \
  return 0;

  memset(delegates_public_address,0,sizeof(delegates_public_address));
  memset(public_address,0,sizeof(public_address));
  memset(reserve_proof,0,sizeof(reserve_proof));
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  // check if it is valid to add a reserve proof to the invalid_reserve_proofs struct
  if (current_UTC_date_and_time->tm_min % BLOCK_TIME == 4 && current_UTC_date_and_time->tm_sec > 25 && current_UTC_date_and_time->tm_sec < 50)
  {
    send_data(CLIENT_SOCKET,"The block verifiers are currently deleting invalid reserve proofs from the database.\n\nPlease wait a few seconds",0,0,"");
    return 0;
  }

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("Could not verify the message");
  }

  // parse the message
  for (count = 0, count2 = 0; count < 5; count++)
  {
    if (count == 1)
    {
      memcpy(delegates_public_address,&MESSAGE[count2],strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2);
    }
    if (count == 2)
    {
      memcpy(reserve_proof,&MESSAGE[count2],strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2);
    }
    if (count == 3)
    {
      memcpy(public_address,&MESSAGE[count2],strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2);
    }
    count2 = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) + 1;
  }

  // check if the data is valid
  if (strlen(delegates_public_address) != XCASH_WALLET_LENGTH || memcmp(delegates_public_address,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || strlen(public_address) != XCASH_WALLET_LENGTH || memcmp(public_address,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || strlen(reserve_proof) > BUFFER_SIZE_RESERVE_PROOF)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("Invalid data");
  }

  // create the message
  memcpy(data,"{\"reserve_proof\":\"",18);
  memcpy(data+18,reserve_proof,strnlen(reserve_proof,sizeof(data)));
  memcpy(data+strlen(data),"\"}",2);

  memcpy(data3,"{\"public_address_created_reserve_proof\":\"",41);
  memcpy(data3+41,public_address,strnlen(public_address,sizeof(data3)));
  memcpy(data3+strlen(data3),"\"}",2);

  for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
  {
    memset(data2,0,sizeof(data2));
    memcpy(data2,"reserve_proofs_",15);
    snprintf(data2+15,sizeof(data2)-1,"%zu",count);

    // check if the reserve proof is in the database
    if (count_documents_in_collection(DATABASE_NAME,data2,data,0) > 0)
    {      
      SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("The reserve proof is already in the database");
    }
    // check if another proof from the public address is already in the database
    if (count_documents_in_collection(DATABASE_NAME,data2,data3,0) > 0)
    {      
      settings = 1;
    }
  }

  // check if the reserve proof is valid and the spent amount is 0
  memset(data2,0,sizeof(data2));
  if (check_reserve_proofs(data2,public_address,reserve_proof,0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("The reserve proof is invalid");
  }  

  // remove any reserve proofs that were created by the public address
  if (settings == 1)
  {
    for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
    {
      memset(data,0,sizeof(data));
      memcpy(data,"reserve_proofs_",15);
      snprintf(data+15,sizeof(data)-1,"%zu",count);
      while (count_documents_in_collection(DATABASE_NAME,data,data3,0) > 0)
      {
        pthread_rwlock_rdlock(&rwlock);
        while(database_settings != 1)
        {
          sleep(1);
        }
        pthread_rwlock_unlock(&rwlock);
        if (delete_document_from_collection(DATABASE_NAME,data,data3,0) == 0)
        {
          SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("The previous reserve proof could not be cancelled for this public address");
        }
      }
    }
  }  

  // create the message
  memset(data,0,sizeof(data));
  memcpy(data,"{\"public_address_created_reserve_proof\":\"",41);
  memcpy(data+strlen(data),public_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\",\"public_address_voted_for\":\"",30);
  memcpy(data+strlen(data),delegates_public_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\",\"total\":\"",11);
  memcpy(data+strlen(data),data2,strnlen(data2,sizeof(data)));
  memcpy(data+strlen(data),"\",\"reserve_proof\":\"",19);
  memcpy(data+strlen(data),reserve_proof,strnlen(reserve_proof,sizeof(data)));
  memcpy(data+strlen(data),"\"}",2);

  pthread_rwlock_wrlock(&rwlock);
  while(database_settings != 1)
  {
    sleep(1);
  }
  // add the reserve proof to the database
  for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
  {
    memset(data2,0,sizeof(data2));
    memcpy(data2,"reserve_proofs_",15);
    snprintf(data2+15,sizeof(data2)-1,"%zu",count);
    if (count_documents_in_collection(DATABASE_NAME,data2,data,0) < MAXIMUM_INVALID_RESERERVE_PROOFS / TOTAL_RESERVE_PROOFS_DATABASES)
    {
      if (insert_document_into_collection_json(DATABASE_NAME,data2,data,0) == 1)
      {        
        break;
      }
      else
      {
        SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("The vote could not be added to the database");
      }         
    }
  }
  pthread_rwlock_unlock(&rwlock);
  send_data(CLIENT_SOCKET,"The vote was successfully added to the database}",0,0,"");
  return 1;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_block_verifiers_invalid_reserve_proofs
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_block_verifiers_to_block_verifiers_invalid_reserve_proofs(const char* MESSAGE)
{
  // Variables
  char block_verifiers_public_address[XCASH_WALLET_LENGTH+1];
  char public_address[XCASH_WALLET_LENGTH+1];
  char reserve_proof[MAXIMUM_INVALID_RESERERVE_PROOFS];  
  char data3[BUFFER_SIZE];
  // since were going to be changing where data and data2 are referencing, we need to create a copy to pointer_reset
  size_t count;
  size_t count2;
  size_t count3;
  int settings;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_invalid_reserve_proofs",84); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(block_verifiers_public_address,0,sizeof(block_verifiers_public_address));
  memset(public_address,0,sizeof(public_address));
  memset(reserve_proof,0,sizeof(reserve_proof));
  memset(data3,0,sizeof(data3));

  // check if it is valid to add a reserve proof to the invalid_reserve_proofs struct
  if (current_UTC_date_and_time->tm_min % BLOCK_TIME == 4 && current_UTC_date_and_time->tm_sec > 25 && current_UTC_date_and_time->tm_sec < 50)
  {
    return 1;
  }

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS_ERROR("Could not verify the message");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"public_address",block_verifiers_public_address,sizeof(block_verifiers_public_address)) == 0 || strlen(block_verifiers_public_address) != XCASH_WALLET_LENGTH || memcmp(block_verifiers_public_address,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"public_address_that_created_the_reserve_proof",public_address,sizeof(public_address)) == 0 || strlen(public_address) != XCASH_WALLET_LENGTH || memcmp(public_address,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"reserve_proof",reserve_proof,sizeof(reserve_proof)) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS_ERROR("Could not parse the message");
  }

  // check if the reserve proof is unique 
  pthread_rwlock_wrlock(&rwlock_reserve_proofs);
  for (count3 = 0, settings = 1; count3 <= invalid_reserve_proofs.count; count3++)
  {
    if (strncmp(invalid_reserve_proofs.reserve_proof[count3],reserve_proof,sizeof(reserve_proof)) == 0)
    {
      settings = 0;
    }
  }
  
  if (settings == 1)
  {
    // check if the reserve proof is valid
    memset(data3,0,sizeof(data3));
    if (check_reserve_proofs(data3,public_address,reserve_proof,0) == 0)
    {
      // add the reserve proof to the invalid_reserve_proofs struct
      memcpy(invalid_reserve_proofs.block_verifier_public_address[invalid_reserve_proofs.count],block_verifiers_public_address,strnlen(block_verifiers_public_address,XCASH_WALLET_LENGTH));
      memcpy(invalid_reserve_proofs.public_address[invalid_reserve_proofs.count],public_address,strnlen(public_address,XCASH_WALLET_LENGTH));
      memcpy(invalid_reserve_proofs.reserve_proof[invalid_reserve_proofs.count],reserve_proof,strnlen(reserve_proof,BUFFER_SIZE_RESERVE_PROOF));
      invalid_reserve_proofs.count++;
    }
  } 
  pthread_rwlock_unlock(&rwlock_reserve_proofs);
  return 1;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_nodes_to_block_verifiers_register_delegates
Description: Runs the code when the server receives the NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_nodes_to_block_verifiers_register_delegates(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char data[BUFFER_SIZE];
  char delegate_name[BUFFER_SIZE];
  char delegate_public_address[XCASH_WALLET_LENGTH+1];
  char delegates_IP_address[BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH+1];
  size_t count;
  size_t count2;

  // define macros
  #define DATABASE_COLLECTION "delegates"

  #define SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_nodes_to_block_verifiers_register_delegates",70); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  send_data(CLIENT_SOCKET,"Could not register the delegate}",0,0,""); \
  return 0;

  memset(data,0,sizeof(data));
  memset(delegate_name,0,sizeof(delegate_name));
  memset(delegate_public_address,0,sizeof(delegate_public_address));
  memset(delegates_IP_address,0,sizeof(delegates_IP_address));

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE_ERROR("Could not verify the message");
  }

  // parse the message
  for (count = 0, count2 = 0; count < 5; count++)
  {
    if (count == 1)
    {
      memcpy(delegate_name,&MESSAGE[count2],strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2);
    }
    if (count == 2)
    {
      memcpy(delegates_IP_address,&MESSAGE[count2],strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2);
    }
    if (count == 3)
    {
      memcpy(delegate_public_address,&MESSAGE[count2],strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2);
    }
    count2 = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) + 1;
  }
  
  // check if the data is valid
  if (strlen(delegate_name) > 30 || strlen(delegate_public_address) != XCASH_WALLET_LENGTH || memcmp(delegate_public_address,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || strstr(delegates_IP_address,".") == NULL)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE_ERROR("Invalid data");
  }

  // create the message
  memcpy(data,"{\"public_address\":\"",19);
  memcpy(data+19,delegate_public_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\"}",2);

  // check if the public address is already registered
  if (count_documents_in_collection(DATABASE_NAME,DATABASE_COLLECTION,data,0) > 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE_ERROR("The delegates public address is already registered");
  }

  // create the message
  memcpy(data,"{\"IP_address\":\"",15);
  memcpy(data+15,delegates_IP_address,strnlen(delegates_IP_address,sizeof(data)));
  memcpy(data+strlen(data),"\"}",2); 

  // check if the IP address is already registered
  if (count_documents_in_collection(DATABASE_NAME,DATABASE_COLLECTION,data,0) > 0)
  {    
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE_ERROR("The delegates IP address is already registered");
  }
  
  // create the message
  memset(data,0,sizeof(data));
  memcpy(data,"{\"public_address\":\"",19);
  memcpy(data+strlen(data),delegate_public_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\",\"delegate_name\":\"",19);
  memcpy(data+strlen(data),delegate_name,strnlen(delegate_name,sizeof(data)));
  memcpy(data+strlen(data),"\",\"IP_address\":\"",16);
  memcpy(data+strlen(data),delegates_IP_address,strnlen(delegates_IP_address,sizeof(data)));
  memcpy(data+strlen(data),"\"}",2);

  // add the delegate to the database
  pthread_rwlock_rdlock(&rwlock);
  while(database_settings != 1)
  {
    sleep(1);
  }
  pthread_rwlock_unlock(&rwlock);
  if (insert_document_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,data,0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE_ERROR("The delegate could not be added to the database");
  }  

  send_data(CLIENT_SOCKET,"Registered the delegate}",0,0,"");
  return 1;

  #undef DATABASE_COLLECTION
  #undef SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_nodes_to_block_verifiers_remove_delegates
Description: Runs the code when the server receives the NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_nodes_to_block_verifiers_remove_delegates(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char data[BUFFER_SIZE];
  char delegate_public_address[XCASH_WALLET_LENGTH+1];
  size_t count;
  size_t count2;

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_nodes_to_block_verifiers_remove_delegates",68); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  send_data(CLIENT_SOCKET,"Could not remove the delegate}",0,0,""); \
  return 0;

  memset(data,0,sizeof(data));
  memset(delegate_public_address,0,sizeof(delegate_public_address));

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE_ERROR("Could not verify the message");
  }

  // parse the message
  for (count = 0, count2 = 0; count < 3; count++)
  {
    if (count == 1)
    {
      memcpy(delegate_public_address,&MESSAGE[count2],strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2);
    }
    count2 = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) + 1;
  }

  // check if the data is valid
  if (strlen(delegate_public_address) != XCASH_WALLET_LENGTH || memcmp(delegate_public_address,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE_ERROR("Invalid data");
  }

  // create the message
  memcpy(data,"{\"public_address\":\"",19);
  memcpy(data+19,delegate_public_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\"}",2);

  // check if the delegate is already removed
  if (count_documents_in_collection(DATABASE_NAME,DATABASE_COLLECTION,data,0) <= 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE_ERROR("The delegate is already removed");
  }

  // remove the delegate from the database
  pthread_rwlock_rdlock(&rwlock);
  while(database_settings != 1)
  {
    sleep(1);
  }
  pthread_rwlock_unlock(&rwlock);
  if (delete_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,data,0) == 0)
  {    
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE_ERROR("The delegate could not be removed from the database");
  }

  send_data(CLIENT_SOCKET,"Removed the delegate}",0,0,"");
  return 1;

  #undef DATABASE_COLLECTION
  #undef SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_nodes_to_block_verifiers_update_delegates
Description: Runs the code when the server receives the NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_nodes_to_block_verifiers_update_delegates(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char delegate_public_address[XCASH_WALLET_LENGTH+1];
  char item[BUFFER_SIZE];
  char value[BUFFER_SIZE];
  size_t count;
  size_t count2;

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_nodes_to_block_verifiers_update_delegates",68); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  send_data(CLIENT_SOCKET,"Could not update the delegates information}",0,0,""); \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(delegate_public_address,0,sizeof(delegate_public_address));
  memset(item,0,sizeof(item));
  memset(value,0,sizeof(value));

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE_ERROR("Could not verify the message");
  }

  // parse the message
  for (count = 0, count2 = 0; count < 5; count++)
  {
    if (count == 1)
    {
      memcpy(item,&MESSAGE[count2],strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2);
    }
    if (count == 2)
    {
      memcpy(value,&MESSAGE[count2],strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2);
    }
    if (count == 3)
    {
      memcpy(delegate_public_address,&MESSAGE[count2],strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2);
    }
    count2 = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) + 1;
  }

  // check if the data is valid
  if (strlen(delegate_public_address) != XCASH_WALLET_LENGTH || memcmp(delegate_public_address,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE_ERROR("Invalid data");
  }

  // error check
  if (memcmp(item,"IP_address",10) != 0 && memcmp(item,"about",5) != 0 && memcmp(item,"website",7) != 0 && memcmp(item,"team",4) != 0 && memcmp(item,"pool_mode",9) != 0 && memcmp(item,"fee_structure",13) != 0 && memcmp(item,"server_settings",15) != 0)
  {    
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE_ERROR("Invalid item to update");
  }

  // create the message
  memcpy(data,"{\"public_address\":\"",19);
  memcpy(data+19,delegate_public_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\"}",2);

  memcpy(data2,"{\"",2);
  memcpy(data2+2,item,strnlen(item,sizeof(data2)));
  memcpy(data2+strlen(data2),"\":\"",3);
  memcpy(data2+strlen(data2),value,strnlen(value,sizeof(data2)));
  memcpy(data2+strlen(data2),"\"}",2);

  pthread_rwlock_rdlock(&rwlock);
  while(database_settings != 1)
  {
    sleep(1);
  }
  pthread_rwlock_unlock(&rwlock);

  // update the delegate in the database
  if (update_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,data,data2,0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE_ERROR("The delegate could not be updated from the database");
  }

  send_data(CLIENT_SOCKET,"Updated the delegates information}",0,0,"");
  return 1;

  #undef DATABASE_COLLECTION
  #undef SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_network_data_nodes_block_verifiers_current_time
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_NETWORK_DATA_NODE_BLOCK_VERIFIERS_CURRENT_TIME message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_block_verifiers_to_network_data_nodes_block_verifiers_current_time(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char data[BUFFER_SIZE];

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_NETWORK_DATA_NODE_BLOCK_VERIFIERS_CURRENT_TIME_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_network_data_nodes_block_verifiers_current_time",93); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  send_data(CLIENT_SOCKET,"Could not update the delegates information}",0,0,""); \
  return 0;

  memset(data,0,sizeof(data));

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_NETWORK_DATA_NODE_BLOCK_VERIFIERS_CURRENT_TIME_ERROR("Could not verify the message");
  }
  
  // create the message
  memset(data,0,strlen(data));
  memcpy(data,"{\r\n \"message_settings\": \"NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_BLOCK_VERIFIERS_CURRENT_TIME\",\r\n \"current_time\": \"",112);
  snprintf(data+112,sizeof(data)-1,"%ld",time(0));
  memcpy(data+strlen(data),"\",\r\n}",5);

  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_NETWORK_DATA_NODE_BLOCK_VERIFIERS_CURRENT_TIME_ERROR("Could not sign_data");
  }
  
  // send the network block signature to the main network data node
  if (send_data(CLIENT_SOCKET,data,0,1,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_NETWORK_DATA_NODE_BLOCK_VERIFIERS_CURRENT_TIME_ERROR("Could not send the data to the block verifier");
  }
  return 1;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_NETWORK_DATA_NODE_BLOCK_VERIFIERS_CURRENT_TIME_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_main_network_data_node_to_block_verifier_start_block
Description: Runs the code when the server receives the MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_START_BLOCK message
Parameters:
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_main_network_data_node_to_block_verifier_start_block(const char* MESSAGE)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];

  // define macros
  #define DATABASE_COLLECTION "reserve_bytes_1"

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_START_BLOCK(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_main_network_data_node_to_block_verifier_create_new_block",84); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_START_BLOCK("Could not verify the message");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"database_data",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_bytes_data_hash",data2,sizeof(data2)) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_START_BLOCK("Could not parse the data");
  }

  // add the data to the database
  memcpy(data3,data,strlen(data)-2);
  if (insert_document_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,data3,0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_START_BLOCK("Could not add the start block to the database");
  }

  memset(data3,0,sizeof(data3));

  // create the verify_block file for the X-CASH Daemon
  if (get_path(data3,0) == 0 || write_file(data2,data3) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_START_BLOCK("Could not create the verify_block file");
  }

  return 1;

  #undef DATABASE_COLLECTION
  #undef SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_START_BLOCK
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_main_network_data_node_to_block_verifier_create_new_block
Description: Runs the code when the server receives the MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_CREATE_NEW_BLOCK message
Parameters:
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_main_network_data_node_to_block_verifier_create_new_block(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_CREATE_NEW_BLOCK(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_main_network_data_node_to_block_verifier_create_new_block",84); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // verify the data
  if (verify_data(MESSAGE,0,1) == 0 || memcmp(current_round_part_backup_node,"2",1) != 0 || main_network_data_node_create_block != 1)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_CREATE_NEW_BLOCK("Could not verify data");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"block_blob",data,sizeof(data)) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_CREATE_NEW_BLOCK("Could not parse the data");
  }

  // sign the network block string
  if (sign_network_block_string(data2,data,0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_CREATE_NEW_BLOCK("Could not sign the network block string");
  }

  // create the message
  memset(data,0,strlen(data));
  memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK\",\r\n \"block_blob_signature\": \"",113);
  memcpy(data+113,data2,XCASH_SIGN_DATA_LENGTH);
  memcpy(data+206,"\",\r\n}",5);

  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_CREATE_NEW_BLOCK("Could not sign_data");
  }
  
  // send the network block signature to the main network data node
  if (send_data(CLIENT_SOCKET,data,0,1,"") == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_CREATE_NEW_BLOCK("Could not send the data to the main network data node");
  }
  return 1;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_CREATE_NEW_BLOCK
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_main_node_to_node_message_part_4
Description: Runs the code when the server receives the MAIN_NODES_TO_NODES_PART_4_OF_ROUND message
Parameters:
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_main_node_to_node_message_part_4(const char* MESSAGE)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  int count;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_main_node_to_node_message_part_4",59); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  memset(VRF_data.block_blob,0,sizeof(VRF_data.block_blob));

  // verify the data
  if (verify_data(MESSAGE,0,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR("Could not verify data");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"block_blob",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"public_address",data2,sizeof(data2)) == 0 || strlen(data2) != XCASH_WALLET_LENGTH || memcmp(data2,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR("Could not parse the data");
  }

  // check if the public_address is the correct main node
  pthread_rwlock_rdlock(&rwlock);
  count = main_network_data_node_create_block;
  pthread_rwlock_unlock(&rwlock);

  if ((count == 1 && memcmp(network_data_nodes_list.network_data_nodes_public_address[0],data2,XCASH_WALLET_LENGTH) == 0) || (count == 0 && memcmp(current_round_part_backup_node,"0",1) == 0 && memcmp(main_nodes_list.block_producer_public_address,data2,XCASH_WALLET_LENGTH) == 0) || (count == 0 && memcmp(current_round_part_backup_node,"1",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_1_public_address,data2,XCASH_WALLET_LENGTH) == 0) || (count == 0 && memcmp(current_round_part_backup_node,"2",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_2_public_address,data2,XCASH_WALLET_LENGTH) == 0) || (count == 0 && memcmp(current_round_part_backup_node,"3",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_3_public_address,data2,XCASH_WALLET_LENGTH) == 0) || (count == 0 && memcmp(current_round_part_backup_node,"4",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_4_public_address,data2,XCASH_WALLET_LENGTH) == 0) || (count == 0 && memcmp(current_round_part_backup_node,"5",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_5_public_address,data2,XCASH_WALLET_LENGTH) == 0))
  { 
    if (memcmp(VRF_data.block_blob,"",1) == 0)
    {
      memcpy(VRF_data.block_blob,data,strnlen(data,BUFFER_SIZE));
    }
    else
    {
      SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR("Invalid main node");
    }    
  }
  else
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR("Invalid main node");
  }
  return 1;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_main_node_to_node_message_part_4_create_new_block
Description: Runs the code when the server receives the MAIN_NODES_TO_NODES_PART_4_OF_ROUND message
Parameters:
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_main_node_to_node_message_part_4_create_new_block(const char* MESSAGE)
{
  // Variables
  char message[BUFFER_SIZE];
  char data[BUFFER_SIZE]; 
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE]; 
  size_t count;
  size_t count2;  

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_CREATE_NEW_BLOCK_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_main_node_to_node_message_part_4_create_new_block",76); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(message,0,sizeof(message));
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  memset(VRF_data.block_blob,0,strlen(VRF_data.block_blob));

  // verify the data
  if (verify_data(MESSAGE,0,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_CREATE_NEW_BLOCK_ERROR("Could not verify data");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"block_blob",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"public_address",data2,sizeof(data2)) == 0 || strlen(data2) != XCASH_WALLET_LENGTH || memcmp(data2,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_CREATE_NEW_BLOCK_ERROR("Could not parse the data");
  }

  // check if the public_address is the correct main node
  pthread_rwlock_rdlock(&rwlock);
  count = main_network_data_node_create_block;
  pthread_rwlock_unlock(&rwlock);

  if ((count == 1 && memcmp(network_data_nodes_list.network_data_nodes_public_address[0],data2,XCASH_WALLET_LENGTH) == 0 && verify_network_block_data(0,1,1,"0","") == 1) || (count == 0 && memcmp(current_round_part_backup_node,"0",1) == 0 && memcmp(main_nodes_list.block_producer_public_address,data2,XCASH_WALLET_LENGTH) == 0 && verify_network_block_data(0,1,1,"0","") == 1) || (count == 0 && memcmp(current_round_part_backup_node,"1",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_1_public_address,data2,XCASH_WALLET_LENGTH) == 0 && verify_network_block_data(0,1,1,"0","") == 1) || (count == 0 && memcmp(current_round_part_backup_node,"2",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_2_public_address,data2,XCASH_WALLET_LENGTH) == 0 && verify_network_block_data(0,1,1,"0","") == 1) || (count == 0 && memcmp(current_round_part_backup_node,"3",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_3_public_address,data2,XCASH_WALLET_LENGTH) == 0 && verify_network_block_data(0,1,1,"0","") == 1) || (count == 0 && memcmp(current_round_part_backup_node,"4",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_4_public_address,data2,XCASH_WALLET_LENGTH) == 0 && verify_network_block_data(0,1,1,"0","") == 1) || (count == 0 && memcmp(current_round_part_backup_node,"5",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_5_public_address,data2,XCASH_WALLET_LENGTH) == 0 && verify_network_block_data(0,1,1,"0","") == 1))
  {    
    // get the previous network block string
    sscanf(current_block_height,"%zu", &count);
    if (count < XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT-1)
    {
      SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_CREATE_NEW_BLOCK_ERROR("Could not get the current block height");
    }
    count--;
    snprintf(data3,sizeof(data3)-1,"%zu",count);
    count2 = ((count - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME) + 1;
    memcpy(message,"{\"block_height\":\"",17);
    memcpy(message+17,data3,strnlen(data3,sizeof(message)));
    memcpy(message+strlen(message),"\"}",2);
    memset(data3,0,sizeof(data3));
    memcpy(data3,"reserve_bytes_",14);
    snprintf(data3+14,sizeof(data3)-1,"%zu",count2);
    memset(data2,0,sizeof(data2));
    if (read_document_field_from_collection(DATABASE_NAME,data3,message,"reserve_bytes",data2,0) == 0)
    {
      SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_CREATE_NEW_BLOCK_ERROR("Could not get the previous blocks reserve bytes");
    }
    // verify the block
    if (network_block_string_to_blockchain_data(data,"0") == 0 || verify_network_block_data(1,1,1,"0",data2) == 0)
    {
      SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_CREATE_NEW_BLOCK_ERROR("The MAIN_NODES_TO_NODES_PART_4_OF_ROUND message is invalid");
    }
    memcpy(VRF_data.block_blob,data,strnlen(data,BUFFER_SIZE));

    // SHA2-512 hash the received message
    memset(current_round_part_vote_data.current_vote_results,0,sizeof(current_round_part_vote_data.current_vote_results));
    memset(data,0,sizeof(data));
    crypto_hash_sha512((unsigned char*)data,(const unsigned char*)MESSAGE,(unsigned long long)strnlen(MESSAGE,BUFFER_SIZE));

    // convert the SHA512 data hash to a string
    for (count2 = 0, count = 0; count2 < DATA_HASH_LENGTH / 2; count2++, count += 2)
    {
      snprintf(current_round_part_vote_data.current_vote_results+count,DATA_HASH_LENGTH,"%02x",data[count2] & 0xFF);
    }
  }
  return 1;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_CREATE_NEW_BLOCK_ERROR
}




/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_node_to_node
Description: Runs the code when the server receives the NODES_TO_NODES_VOTE_RESULTS message
Parameters:
  MESSAGE - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_node_to_node(const char* MESSAGE)
{
  // Variables
  char data[BUFFER_SIZE]; 
  char data2[BUFFER_SIZE];

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NODE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_node_to_node",39); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // verify the data
  if (verify_data(MESSAGE,0,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NODE_ERROR("Could not verify data");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"vote_settings",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"vote_data",data2,sizeof(data2)) == 0 || strlen(data2) != DATA_HASH_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NODE_ERROR("Could not parse the data");
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
  return 1;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NODE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_block_verifiers_vrf_data
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA message
Parameters:
  MESSAGE - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_block_verifiers_to_block_verifiers_vrf_data(const char* MESSAGE)
{
  // Variables  
  char public_address[BUFFER_SIZE];
  char vrf_secret_key_data[BUFFER_SIZE];
  char vrf_public_key_data[BUFFER_SIZE];
  char random_data[BUFFER_SIZE];
  char data[BUFFER_SIZE];
  unsigned char vrf_public_key[crypto_vrf_PUBLICKEYBYTES];
  unsigned char vrf_secret_key[crypto_vrf_SECRETKEYBYTES];
  size_t count;
  size_t counter;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_vrf_data",70); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(public_address,0,sizeof(public_address));
  memset(vrf_secret_key_data,0,sizeof(vrf_secret_key_data));
  memset(vrf_public_key_data,0,sizeof(vrf_public_key_data));
  memset(random_data,0,sizeof(random_data));
  memset(data,0,sizeof(data));  
  memset(vrf_public_key,0,sizeof(vrf_public_key));
  memset(vrf_secret_key,0,sizeof(vrf_secret_key));

  // verify the data
  if (verify_data(MESSAGE,0,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA_ERROR("Could not verify data");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"public_address",public_address,sizeof(public_address)) == 0 || strlen(public_address) != XCASH_WALLET_LENGTH || memcmp(public_address,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"vrf_secret_key",vrf_secret_key_data,sizeof(vrf_secret_key_data)) == 0 || strlen(vrf_secret_key_data) != VRF_SECRET_KEY_LENGTH || parse_json_data(MESSAGE,"vrf_public_key",vrf_public_key_data,sizeof(vrf_public_key_data)) == 0 || strlen(vrf_public_key_data) != VRF_PUBLIC_KEY_LENGTH || parse_json_data(MESSAGE,"random_data",random_data,sizeof(random_data)) == 0 || strlen(random_data) != RANDOM_STRING_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA_ERROR("Could not parse the data");
  }

  // convert the VRF secret key string to a VRF secret key
  for (counter = 0, count = 0; counter < VRF_SECRET_KEY_LENGTH; count++, counter += 2)
  {
    memset(data,0,strnlen(data,BUFFER_SIZE));
    memcpy(data,&vrf_secret_key_data[counter],2);
    vrf_secret_key[count] = (int)strtol(data, NULL, 16);
  } 

  // convert the VRF public key string to a VRF public key
  for (counter = 0, count = 0; counter < VRF_PUBLIC_KEY_LENGTH; count++, counter += 2)
  {
    memset(data,0,strnlen(data,BUFFER_SIZE));
    memcpy(data,&vrf_public_key_data[counter],2);
    vrf_public_key[count] = (int)strtol(data, NULL, 16);
  } 

  // process the vote data
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],public_address,XCASH_WALLET_LENGTH) == 0 && memcmp(VRF_data.block_verifiers_vrf_secret_key_data[count],"",1) == 0 && memcmp(VRF_data.block_verifiers_vrf_secret_key[count],"",1) == 0 && memcmp(VRF_data.block_verifiers_vrf_public_key_data[count],"",1) == 0 && memcmp(VRF_data.block_verifiers_vrf_public_key[count],"",1) == 0 && memcmp(VRF_data.block_verifiers_random_data[count],"",1) == 0)
    {
      memcpy(VRF_data.block_verifiers_vrf_secret_key_data[count],vrf_secret_key_data,VRF_SECRET_KEY_LENGTH);
      memcpy(VRF_data.block_verifiers_vrf_secret_key[count],vrf_secret_key,crypto_vrf_SECRETKEYBYTES);
      memcpy(VRF_data.block_verifiers_vrf_public_key_data[count],vrf_public_key_data,VRF_PUBLIC_KEY_LENGTH);
      memcpy(VRF_data.block_verifiers_vrf_public_key[count],vrf_public_key,crypto_vrf_PUBLICKEYBYTES);
      memcpy(VRF_data.block_verifiers_random_data[count],random_data,RANDOM_STRING_LENGTH);
    }
  }  
  return 1;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_block_verifiers_to_block_verifiers_block_blob_signature
Description: Runs the code when the server receives the BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE message
Parameters:
  MESSAGE - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_block_verifiers_to_block_verifiers_block_blob_signature(const char* MESSAGE)
{
  // Variables
  char data[BUFFER_SIZE]; 
  char data2[BUFFER_SIZE];
  size_t count;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_block_blob_signature",82); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // verify the data
  if (verify_data(MESSAGE,0,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE_ERROR("Could not verify data");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"block_blob_signature",data,sizeof(data)) == 0 || strlen(data) != XCASH_SIGN_DATA_LENGTH || memcmp(data,XCASH_SIGN_DATA_PREFIX,sizeof(XCASH_SIGN_DATA_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"public_address",data2,sizeof(data2)) == 0 || strlen(data2) != XCASH_WALLET_LENGTH || memcmp(data2,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE_ERROR("Could not parse the data");
  }

  // process the vote data
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],data2,XCASH_WALLET_LENGTH) == 0 && memcmp(VRF_data.block_blob_signature[count],"",1) == 0)
    {
      memcpy(VRF_data.block_blob_signature[count],data,XCASH_SIGN_DATA_LENGTH);
    }
  } 
  return 1;

  #undef SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE_ERROR
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
  const int THREADS = get_nprocs();

  // Variables
  char buffer[BUFFER_SIZE];
  int len;
  int receive_data_result; 
  struct sockaddr_in addr, cl_addr; 
  struct socket_thread_parameters* socket_thread_parameters;
  struct epoll_event events;
  size_t count;
  int settings;

  // threads
  pthread_t thread_id;

  // define macros
  #define SOCKET_FILE_DESCRIPTORS_LENGTH 1

  #define SERVER_ERROR(message) \
  memcpy(error_message.function[error_message.total],"create_server",13); \
  memcpy(error_message.data[error_message.total],message,strnlen(message,BUFFER_SIZE)); \
  error_message.total++; \
  exit(0);  

  // set the main process to ignore if forked processes return a value or not, since the timeout for the total connection time is run on a different thread
  signal(SIGCHLD, SIG_IGN);
  
  memset(buffer,0,sizeof(buffer));

  if (MESSAGE_SETTINGS == 1)
  {
    print_start_message("Creating the server");
  }
  
  /* Create the socket  
  AF_INET = IPV4 support
  SOCK_STREAM = TCP protocol
  */
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1)
  {
    SERVER_ERROR("Error creating socket");
  }

  /* Set the socket options
  SOL_SOCKET = socket level
  SO_REUSEADDR = allows for reuse of the same address, so one can shutdown and restart the program without errors
  SO_REUSEPORT = allows for reuse of the same port, so one can shutdown and restart the program without errors
  */
  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &SOCKET_OPTION,sizeof(int)) != 0)
  {
    SERVER_ERROR("Error setting socket options");
  } 

  if (MESSAGE_SETTINGS == 1)
  {   
    color_print("Socket created","green");
  }
 
  // convert the port to a string
  snprintf(buffer,sizeof(buffer)-1,"%d",SEND_DATA_PORT);  
 
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
  if (bind(server_socket, (struct sockaddr *) &addr, sizeof(addr)) != 0)
  {    
    memcpy(error_message.function[error_message.total],"create_server",13);
    memcpy(error_message.data[error_message.total],"Error connecting to port ",25);
    memcpy(error_message.data[error_message.total]+25,buffer,strnlen(buffer,BUFFER_SIZE));
    error_message.total++;
    print_error_message;
    exit(0);
  } 

  // set the socket to non blocking, to avoid the server stopping when a client diconnects
  settings = fcntl(server_socket, F_GETFL);
  fcntl(server_socket, F_SETFL, settings | O_NONBLOCK);

  // set the maximum simultaneous connections
  if (listen(server_socket, MAXIMUM_CONNECTIONS) != 0)
  {
    SERVER_ERROR("Error creating the server");
  }

  if (MESSAGE_SETTINGS == 1)
  {
    printf("\033[1;32mConnected to port %s\033[0m\n",buffer);
    printf("Waiting for a connection...\n\n");
  }

  /* create the epoll file descriptor
  EPOLL_CLOEXEC = close access to the epoll_fd when the process or fork closes
  */
  epoll_fd = epoll_create1(0);

  if (epoll_fd < 0)
  {
    SERVER_ERROR("Error creating the server");
  }

  /* create the epoll_event struct
  EPOLLIN = signal when the file secriptor is ready to read
  EPOLLET = use edge triggered mode, this will only signal that a file descriptor is ready when that file descriptor changes states
  */  
  events.events = EPOLLIN | EPOLLET;
  events.data.fd = server_socket;

  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_socket, &events) < 0)
  {
    SERVER_ERROR("Error creating the server");
  }

  for (count = 0; count < THREADS-1; count++)
  {
    if (pthread_create(&server_threads[count], NULL, socket_receive_data_thread, NULL) < 0)
    {
      SERVER_ERROR("Error creating the server");
    }
  }

  // run the socket_receive_data_thread on the mian thread
  socket_receive_data_thread(NULL);

  return 1;

  #undef SOCKET_FILE_DESCRIPTORS_LENGTH
  #undef SERVER_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: new_socket_thread
Description: new socket thread
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

int new_socket_thread()
{
  // Variables
  int client_socket;
  struct sockaddr_in addr;
  socklen_t addrlen = sizeof(addr);
  struct epoll_event events;

  start:

  client_socket = accept(server_socket, (struct sockaddr *) &addr, &addrlen);
  if (client_socket < 0 && errno == EWOULDBLOCK)
  {
    sleep(1);
    goto start;
  }
  else if (client_socket < 0 && errno != EWOULDBLOCK)
  {
    return 0;
  }
  
  /* create the epoll_event struct
  EPOLLIN = signal when the file secriptor is ready to read
  EPOLLET = use edge triggered mode, this will only signal that a file descriptor is ready when that file descriptor changes states
  */
  events.events = EPOLLIN | EPOLLET;
  events.data.fd = client_socket;

  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket, &events) < 0)
  {
    close(client_socket);
    return 0;
  }
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: socket_thread
Description: socket thread
Parameters:
  client_socket - The client socket
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int socket_thread(int client_socket)
{
  // Variables
  char* buffer = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char buffer2[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char message[BUFFER_SIZE];
  char client_address[BUFFER_SIZE]; 
  int len;
  int receive_data_result; 
  struct in_addr ip_address_data;
  struct sockaddr_in addr;
  socklen_t addrlength = sizeof(addr);
  
  memset(buffer2,0,sizeof(buffer2));
  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));
  memset(client_address,0,sizeof(client_address));

  // convert the port to a string
  snprintf(buffer2,sizeof(buffer2)-1,"%d",SEND_DATA_PORT); 

  // receive the data
  receive_data_result = receive_data(client_socket,buffer,SOCKET_END_STRING,1,TOTAL_CONNECTION_TIME_SETTINGS);
  if (receive_data_result < 2)
  {
    pointer_reset(buffer);
    return 0;
  } 

  // check if the message length is correct for the type of message
  if (strstr,buffer,"GET / HTTP/" != NULL || strstr,buffer,"POST / HTTP/" != NULL)
  {
    memcpy(data2,"HTTP ",5);
    memcpy(data2+5,buffer,strnlen(buffer,sizeof(data2)) - strnlen(strstr(buffer," HTTP/"),sizeof(data2)));
  }
  else if (strnlen(buffer,MAXIMUM_BUFFER_SIZE) > 25 && strstr(buffer,"}") != NULL)
  {
    memcpy(data2,&buffer[25],strlen(buffer) - strlen(strstr(buffer,"\",\r\n")) - 25);
    if ((strncmp(data2,"XCASH_PROOF_OF_STAKE_TEST_DATA",BUFFER_SIZE) == 0 || strncmp(data2,"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST",BUFFER_SIZE) == 0 || strncmp(data2,"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST",BUFFER_SIZE) == 0 || strncmp(data2,"NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",BUFFER_SIZE) == 0 || strncmp(data2,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE",BUFFER_SIZE) == 0 || strncmp(data2,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) == 0 || strncmp(data2,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",BUFFER_SIZE) == 0 || strncmp(data2,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) == 0 || strncmp(data2,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) == 0 || strncmp(data2,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) == 0 || strncmp(data2,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF",BUFFER_SIZE) == 0 || strncmp(data2,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS",BUFFER_SIZE) == 0 || strncmp(data2,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE",BUFFER_SIZE) == 0 || strncmp(data2,"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE",BUFFER_SIZE) == 0 || strncmp(data2,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE",BUFFER_SIZE) == 0 || strncmp(data2,"MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_CREATE_NEW_BLOCK",BUFFER_SIZE) == 0 || strncmp(data2,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND_CREATE_NEW_BLOCK",BUFFER_SIZE) == 0 || strncmp(data2,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA",BUFFER_SIZE) == 0 || strncmp(data2,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE",BUFFER_SIZE) == 0 || strncmp(data2,"NODES_TO_NODES_VOTE_RESULTS",BUFFER_SIZE) == 0) && (strnlen(buffer,MAXIMUM_BUFFER_SIZE) == MAXIMUM_BUFFER_SIZE))
    {
      pointer_reset(buffer);
      return 0;
    }
  }
  else if (strstr(buffer,"|") != NULL)
  {
    memcpy(data2,buffer,strnlen(buffer,sizeof(data2)) - strnlen(strstr(buffer,"|"),sizeof(data2)));
    if (strncmp(data2,"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST",BUFFER_SIZE) != 0 && strncmp(data2,"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST",BUFFER_SIZE) != 0 && strncmp(data2,"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES",BUFFER_SIZE) != 0 && strncmp(data2,"NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",BUFFER_SIZE) != 0 && strncmp(data2,"XCASH_PROOF_OF_STAKE_TEST_DATA",BUFFER_SIZE) != 0 && strncmp(data2,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF",BUFFER_SIZE) != 0 && strncmp(data2,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE",BUFFER_SIZE) != 0 && strncmp(data2,"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE",BUFFER_SIZE) != 0 && strncmp(data2,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE",BUFFER_SIZE) != 0)
    {
      pointer_reset(buffer);
      return 0;
    }
  }
  else
  {
    pointer_reset(buffer);
    return 0;
  }

  if (getpeername(client_socket, (struct sockaddr *) &addr, &addrlength) < 0 || inet_ntop(AF_INET, &addr.sin_addr, client_address, sizeof(client_address)) == NULL)
  {
    pointer_reset(buffer);
    return 0;
  }
  
  // check if the IP address has a reverse DNS associated with it 
  if (inet_aton(client_address, &ip_address_data) == 0)
  {
    pointer_reset(buffer);
    return 0;
  }

  const struct hostent* HOST_NAME = gethostbyaddr((const void *)&ip_address_data, sizeof(ip_address_data), AF_INET);
  if (HOST_NAME != NULL)
  {
    memset(client_address,0,sizeof(client_address));
    memcpy(client_address,HOST_NAME->h_name,strnlen(HOST_NAME->h_name,sizeof(client_address)));
  }
  
  // get the current time
  get_current_UTC_time;
  
  memcpy(message,"Received ",9);
  memcpy(message+9,data2,strnlen(data2,sizeof(message)));
  memcpy(message+strlen(message),"\n",1);
  memcpy(message+strlen(message),client_address,strnlen(client_address,sizeof(message)));
  memcpy(message+strlen(message)," on port ",9);
  memcpy(message+strlen(message),buffer2,strnlen(buffer2,sizeof(message)));
  memcpy(message+strlen(message),"\n",1);
  memset(data2,0,sizeof(data2));
  strftime(data2,sizeof(data2),"%a %d %b %Y %H:%M:%S UTC\n",current_UTC_date_and_time);
  memcpy(message+strlen(message),data2,strnlen(data2,sizeof(message)));
  color_print(message,"green");

 // check if a certain type of message has been received 
 if (strstr(buffer,"\"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\"") != NULL)
 {
   server_received_data_xcash_proof_of_stake_test_data(client_socket,(const char*)buffer);
 }
 else if (strstr(buffer,"GET /") != NULL && (strstr(buffer,".html") != NULL || strstr(buffer,".js") != NULL || strstr(buffer,".css") != NULL || strstr(buffer,".png") != NULL || strstr(buffer,".jpg") != NULL || strstr(buffer,".jpeg") != NULL))
 {
   server_receive_data_socket_get_files(client_socket,(const char*)buffer);
 } 
 else if (strstr(buffer,"GET /delegateswebsitegetstatistics HTTP/") != NULL)
 {
   server_receive_data_socket_delegates_website_get_statistics(client_socket);
 } 
 else if (strstr(buffer,"GET /getdelegates HTTP/") != NULL)
 {
   server_receive_data_socket_get_delegates(client_socket);
 } 
 else if (strstr(buffer,"GET /getdelegatesstatistics?") != NULL)
 {
   server_receive_data_socket_get_delegates_statistics(client_socket,(const char*)buffer);
 } 
 else if (strstr(buffer,"\"message_settings\": \"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST\"") != NULL && network_data_node_settings == 1)
 {
   server_receive_data_socket_node_to_network_data_nodes_get_previous_current_next_block_verifiers_list(client_socket);
 } 
 else if (strstr(buffer,"\"message_settings\": \"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST\"") != NULL && network_data_node_settings == 1)
 {
   server_receive_data_socket_node_to_network_data_nodes_get_current_block_verifiers_list(client_socket);
 } 
 else if (strstr(buffer,"\"message_settings\": \"NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE\"") != NULL)
 {
   server_receive_data_socket_nodes_to_block_verifiers_reserve_bytes_database_sync_check_all_update(client_socket);
 }
 else if (strstr(buffer,"\"message_settings\": \"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES\"") != NULL)
 {
   server_receive_data_socket_node_to_block_verifiers_get_reserve_bytes(client_socket,(const char*)buffer);
 }
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE\"") != NULL)
 {
   server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_all_update(client_socket,(const char*)buffer);
 } 
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE\"") != NULL)
 {
   server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_update(client_socket,(const char*)buffer);
 }  
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE\"") != NULL)
 {
   server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_download_file_update(client_socket,(const char*)buffer);
 }  
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE\"") != NULL)
 {
   server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_all_update(client_socket,(const char*)buffer);
 }
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE\"") != NULL)
 {
   server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_update(client_socket,(const char*)buffer);
 }
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE\"") != NULL)
 {
   server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_download_file_update(client_socket,(const char*)buffer);
 }
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE\"") != NULL)
 {
   server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_sync_check_update(client_socket,(const char*)buffer);
 }
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE\"") != NULL)
 {
   server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_download_file_update(client_socket,(const char*)buffer);
 }
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE\"") != NULL)
 {
   server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_sync_check_update(client_socket,(const char*)buffer);
 }
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE\"") != NULL)
 {
   server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_download_file_update(client_socket,(const char*)buffer);
 }
 else if (strstr(buffer,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF") != NULL)
 {
   server_receive_data_socket_node_to_block_verifiers_add_reserve_proof(client_socket,(const char*)buffer);
 } 
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS\"") != NULL)
 {
   server_receive_data_socket_block_verifiers_to_block_verifiers_invalid_reserve_proofs((const char*)buffer);
 }  
 else if (strstr(buffer,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE") != NULL)
 {
   server_receive_data_socket_nodes_to_block_verifiers_register_delegates(client_socket,(const char*)buffer);
 }            
 else if (strstr(buffer,"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE") != NULL)
 {
   server_receive_data_socket_nodes_to_block_verifiers_remove_delegates(client_socket,(const char*)buffer);
 } 
 else if (strstr(buffer,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE") != NULL)
 {
   server_receive_data_socket_nodes_to_block_verifiers_update_delegates(client_socket,(const char*)buffer);
 } 
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_NETWORK_DATA_NODE_BLOCK_VERIFIERS_CURRENT_TIME\"") != NULL)
 {
   server_receive_data_socket_block_verifiers_to_network_data_nodes_block_verifiers_current_time(client_socket,(const char*)buffer);
 } 
 else if (strstr(buffer,"\"message_settings\": \"MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_START_BLOCK\"") != NULL && main_network_data_node_create_block == 1)
 {  
   server_receive_data_socket_main_network_data_node_to_block_verifier_start_block((const char*)buffer);
 } 
 else if (strstr(buffer,"\"message_settings\": \"MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_CREATE_NEW_BLOCK\"") != NULL)
 {  
   server_receive_data_socket_main_network_data_node_to_block_verifier_create_new_block(client_socket,(const char*)buffer);
 } 
 else if (strstr(buffer,"\"message_settings\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND_CREATE_NEW_BLOCK\"") != NULL && current_UTC_date_and_time->tm_sec >= 10 && current_UTC_date_and_time->tm_sec < 30)
 {
   server_receive_data_socket_main_node_to_node_message_part_4((const char*)buffer);
 }         
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA\"") != NULL && current_UTC_date_and_time->tm_sec < 10)
 {
   server_receive_data_socket_block_verifiers_to_block_verifiers_vrf_data((const char*)buffer);
 }  
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE\"") != NULL && current_UTC_date_and_time->tm_sec >= 30 && current_UTC_date_and_time->tm_sec < 40)
 {
   server_receive_data_socket_block_verifiers_to_block_verifiers_block_blob_signature((const char*)buffer);
 }  
 else if (strstr(buffer,"\"message_settings\": \"NODES_TO_NODES_VOTE_RESULTS\"") != NULL && ((current_UTC_date_and_time->tm_sec >= 45 && current_UTC_date_and_time->tm_sec < 55) || (current_UTC_date_and_time->tm_min % BLOCK_TIME == 4 && current_UTC_date_and_time->tm_sec >= 30 && current_UTC_date_and_time->tm_sec < 40)))
 {
   server_receive_data_socket_node_to_node((const char*)buffer);
 }
 else
 {
   memset(buffer,0,strlen(buffer));
   memcpy(buffer,"{\"Error\":\"Could not get the file\"}",34);
   send_data(client_socket,buffer,strlen(buffer),400,"application/json");
 }
pointer_reset(buffer);
return 1;
}