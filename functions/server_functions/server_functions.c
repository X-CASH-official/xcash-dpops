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
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

int start_new_round()
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;
  size_t count2;
  int settings;
  int settings2;

  // define macros
  #define START_NEW_ROUND_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"start_new_round",15); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset(data); \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    memcpy(error_message.function[error_message.total],"start_new_round",15);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  // start a new round
  if (get_current_block_height(current_block_height,0) == 0)
  {
    START_NEW_ROUND_ERROR("Could not get the current block height");
  }
  memset(data,0,strlen(data));
  memcpy(data,"A new round is starting for block ",34);
  memcpy(data+34,current_block_height,strnlen(current_block_height,BUFFER_SIZE));
  print_start_message(data);
  
  // reset the variables 
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memset(VRF_data.block_verifiers_vrf_secret_key_data[count],0,strlen(VRF_data.block_verifiers_vrf_secret_key_data[count])); 
    memset(VRF_data.block_verifiers_vrf_secret_key[count],0,strlen(VRF_data.block_verifiers_vrf_secret_key[count])); 
    memset(VRF_data.block_verifiers_vrf_public_key_data[count],0,strlen(VRF_data.block_verifiers_vrf_public_key_data[count])); 
    memset(VRF_data.block_verifiers_vrf_public_key[count],0,strlen(VRF_data.block_verifiers_vrf_public_key[count])); 
    memset(VRF_data.block_verifiers_random_data[count],0,strlen(VRF_data.block_verifiers_random_data[count])); 
    memset(VRF_data.block_blob_signature[count],0,strlen(VRF_data.block_blob_signature[count])); 
    memset(VRF_data_copy->block_verifiers_vrf_secret_key_data[count],0,strlen(VRF_data_copy->block_verifiers_vrf_secret_key_data[count])); 
    memset(VRF_data_copy->block_verifiers_vrf_secret_key[count],0,strlen(VRF_data_copy->block_verifiers_vrf_secret_key[count])); 
    memset(VRF_data_copy->block_verifiers_vrf_public_key_data[count],0,strlen(VRF_data_copy->block_verifiers_vrf_public_key_data[count])); 
    memset(VRF_data_copy->block_verifiers_vrf_public_key[count],0,strlen(VRF_data_copy->block_verifiers_vrf_public_key[count])); 
    memset(VRF_data_copy->block_verifiers_random_data[count],0,strlen(VRF_data_copy->block_verifiers_random_data[count])); 
    memset(VRF_data_copy->block_blob_signature[count],0,strlen(VRF_data_copy->block_blob_signature[count])); 
    
    memset(invalid_reserve_proofs.block_verifier_public_address[count],0,strlen(invalid_reserve_proofs.block_verifier_public_address[count]));
    memset(invalid_reserve_proofs.public_address[count],0,strlen(invalid_reserve_proofs.public_address[count]));
    memset(invalid_reserve_proofs.reserve_proof[count],0,strlen(invalid_reserve_proofs.reserve_proof[count]));
  }

  // reset the VRF_data struct
  memset(VRF_data.vrf_secret_key_data_round_part_4,0,strlen(VRF_data.vrf_secret_key_data_round_part_4));
  memset(VRF_data.vrf_secret_key_round_part_4,0,strlen(VRF_data.vrf_secret_key_round_part_4));
  memset(VRF_data.vrf_public_key_data_round_part_4,0,strlen(VRF_data.vrf_public_key_data_round_part_4));
  memset(VRF_data.vrf_public_key_round_part_4,0,strlen((const char*)VRF_data.vrf_public_key_round_part_4));
  memset(VRF_data.vrf_alpha_string_data_round_part_4,0,strlen(VRF_data.vrf_alpha_string_data_round_part_4));
  memset(VRF_data.vrf_alpha_string_round_part_4,0,strlen((const char*)VRF_data.vrf_alpha_string_round_part_4));
  memset(VRF_data.vrf_proof_data_round_part_4,0,strlen(VRF_data.vrf_proof_data_round_part_4));
  memset(VRF_data.vrf_proof_round_part_4,0,strlen((const char*)VRF_data.vrf_proof_round_part_4));
  memset(VRF_data.vrf_beta_string_data_round_part_4,0,strlen(VRF_data.vrf_beta_string_data_round_part_4));
  memset(VRF_data.vrf_beta_string_round_part_4,0,strlen((const char*)VRF_data.vrf_beta_string_round_part_4));
  memset(VRF_data.reserve_bytes_data_hash,0,strlen(VRF_data.reserve_bytes_data_hash));
  memset(VRF_data.block_blob,0,strlen(VRF_data.block_blob));
  memset(VRF_data_copy->vrf_secret_key_data_round_part_4,0,strlen(VRF_data_copy->vrf_secret_key_data_round_part_4));
  memset(VRF_data_copy->vrf_secret_key_round_part_4,0,strlen(VRF_data_copy->vrf_secret_key_round_part_4));
  memset(VRF_data_copy->vrf_public_key_data_round_part_4,0,strlen(VRF_data_copy->vrf_public_key_data_round_part_4));
  memset(VRF_data_copy->vrf_public_key_round_part_4,0,strlen((const char*)VRF_data_copy->vrf_public_key_round_part_4));
  memset(VRF_data_copy->vrf_alpha_string_data_round_part_4,0,strlen(VRF_data_copy->vrf_alpha_string_data_round_part_4));
  memset(VRF_data_copy->vrf_alpha_string_round_part_4,0,strlen((const char*)VRF_data_copy->vrf_alpha_string_round_part_4));
  memset(VRF_data_copy->vrf_proof_data_round_part_4,0,strlen(VRF_data_copy->vrf_proof_data_round_part_4));
  memset(VRF_data_copy->vrf_proof_round_part_4,0,strlen((const char*)VRF_data_copy->vrf_proof_round_part_4));
  memset(VRF_data_copy->vrf_beta_string_data_round_part_4,0,strlen(VRF_data_copy->vrf_beta_string_data_round_part_4));
  memset(VRF_data_copy->vrf_beta_string_round_part_4,0,strlen((const char*)VRF_data_copy->vrf_beta_string_round_part_4));
  memset(VRF_data_copy->reserve_bytes_data_hash,0,strlen(VRF_data_copy->reserve_bytes_data_hash));
  memset(VRF_data_copy->block_blob,0,strlen(VRF_data_copy->block_blob));

  /*// check if all of the databases are synced
  if (check_if_databases_are_synced() == 0)
  {
    START_NEW_ROUND_ERROR("Could not check if the database is synced. Your block verifier will now sit out for the remainder of the round");
  }

  // wait for all block verifiers to sync the database
  
  do
  {    
    usleep(200000); 
    get_current_UTC_time; 
  } while (current_UTC_date_and_time->tm_min != 1 && current_UTC_date_and_time->tm_min != 30)
  */

  // check if the current block height - 1 is a X-CASH proof of stake block since this will check to see if these are the first three blocks on the network
  sscanf(current_block_height,"%zu", &count);
  count = count - 1;
  memset(data,0,strnlen(data,BUFFER_SIZE));
  sprintf(data,"%zu",count);
  settings = get_block_settings(data,0);
  sscanf(current_block_height,"%zu", &count);
  count = count - 2;
  memset(data,0,strnlen(data,BUFFER_SIZE));
  sprintf(data,"%zu",count);
  settings2 = get_block_settings(data,0);
  if (settings == 0)
  {    
    START_NEW_ROUND_ERROR("Could not get a previous blocks settings. Your block verifier will now sit out for the remainder of the round");
  }
  else if (settings == 1 && settings2 == 1)
  {
    // this is the first block of the network
    color_print("The current block is the first block on the network, meaning the the main network node will create this block","green");

    // set the main_network_data_node_create_block so the main network data node can create the block
    main_network_data_node_create_block = 1;
    if (start_current_round_start_blocks() == 0)
    {      
      START_NEW_ROUND_ERROR("start_current_round_start_blocks error");
    } 
  }
  else if (settings == 2)
  {
    // this is a X-CASH proof of stake block so this is not the start blocks of the network
    if (settings2 == 2)
    {
      // update all of the databases 
      color_print("Updating the previous rounds data in the databases","green");
      if (update_databases() == 0)
      {  
        START_NEW_ROUND_ERROR("Could not check if the databases are synced. This means that your database is out of sync, and you need to resync your databases");
      }
    }

    // reset the current_round_part and current_round_part_backup_node
    memset(current_round_part,0,strlen(current_round_part));
    memcpy(current_round_part,"1",1);
    memset(current_round_part_backup_node,0,strlen(current_round_part_backup_node));
    memcpy(current_round_part_backup_node,"0",1);

    if (calculate_main_nodes_roles() == 0)
    {
      print_error_message;
      
      // set the main_network_data_node_create_block so the main network data node can create the block
      main_network_data_node_create_block = 1;
      if (data_network_node_create_block() == 0)
      {      
        START_NEW_ROUND_ERROR("start_current_round_start_blocks error");
      } 
    }
    if (start_part_4_of_round() == 0)
    {
      print_error_message;
      
      // set the main_network_data_node_create_block so the main network data node can create the block
      main_network_data_node_create_block = 1;
      if (data_network_node_create_block() == 0)
      {      
        START_NEW_ROUND_ERROR("start_current_round_start_blocks error");
      }  
    }
  }
  pointer_reset(data);
  return 1;

  #undef START_NEW_ROUND_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: start_current_round_start_blocks
Description: Runs the round where the network data node will create the first block of the X-CASH proof of stake block on the network
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

int start_current_round_start_blocks()
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count; 
  size_t count2; 

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; \
  free(data3); \
  data3 = NULL;

  #define START_CURRENT_ROUND_START_BLOCKS_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"start_current_round_start_blocks",32); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL || data3 == NULL)
  {
    if (data == NULL)
    {
      pointer_reset(data);
    }
    if (data2 == NULL)
    {
      pointer_reset(data2);
    }
    if (data3 == NULL)
    {
      pointer_reset(data3);
    }
    memcpy(error_message.function[error_message.total],"start_current_round_start_blocks",32);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  // check if the block verifier is the main network data node
  if (memcmp(xcash_wallet_public_address,network_data_nodes_list.network_data_nodes_public_address[0],XCASH_WALLET_LENGTH) != 0)
  {
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Your block verifier is not the main data network node so your block verifier will sit out for the remainder of the round");
  } 

  color_print("Your block verifier is the main data network node so your block verifier will create the block\nWaiting until 4 minutes and 40 seconds of the round","green");

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

  memcpy(blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,blockchain_data.previous_block_hash_data,64);
  memcpy(VRF_data.vrf_alpha_string_round_part_4,blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,64);

  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memcpy(VRF_data.vrf_alpha_string_round_part_4+strlen(VRF_data.vrf_alpha_string_round_part_4),"0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",RANDOM_STRING_LENGTH);
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
    sprintf(VRF_data.vrf_secret_key_data_round_part_4+count,"%02x",VRF_data.vrf_secret_key_round_part_4[count2] & 0xFF);
  }
  for (count2 = 0, count = 0; count2 < crypto_vrf_PUBLICKEYBYTES; count2++, count += 2)
  {
    sprintf(VRF_data.vrf_public_key_data_round_part_4+count,"%02x",VRF_data.vrf_public_key_round_part_4[count2] & 0xFF);
  }
  for (count2 = 0, count = 0; count2 < strlen((const char*)VRF_data.vrf_alpha_string_round_part_4); count2++, count += 2)
  {
    sprintf(VRF_data.vrf_alpha_string_data_round_part_4+count,"%02x",VRF_data.vrf_alpha_string_round_part_4[count2] & 0xFF);
  }
  for (count2 = 0, count = 0; count2 < crypto_vrf_PROOFBYTES; count2++, count += 2)
  {
    sprintf(VRF_data.vrf_proof_data_round_part_4+count,"%02x",VRF_data.vrf_proof_round_part_4[count2] & 0xFF);
  }
  for (count2 = 0, count = 0; count2 < crypto_vrf_OUTPUTBYTES; count2++, count += 2)
  {
    sprintf(VRF_data.vrf_beta_string_data_round_part_4+count,"%02x",VRF_data.vrf_beta_string_round_part_4[count2] & 0xFF);
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
  for (count = 1; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key_data[count],"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",VRF_SECRET_KEY_LENGTH);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key_data[count],"0000000000000000000000000000000000000000000000000000000000000000",VRF_PUBLIC_KEY_LENGTH);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data_text[count],"0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",RANDOM_STRING_LENGTH);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data[count],"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",RANDOM_STRING_LENGTH*2);
  }

  // add the next block verifiers
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  { 
    if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],"",1) == 0)
    {
      memcpy(blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address[count],"XCA1v18Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3",XCASH_WALLET_LENGTH);
    }
    else
    {
      memcpy(blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address[count],current_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH);
    }
  }
  /*// add the next block verifiers
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  { 
    memcpy(blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address[count],current_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH);
  }*/

  // add 0's for the block validation nodes signature, except for the first block validation node signature
  for (count = 1; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count],GET_BLOCK_TEMPLATE_RESERVED_BYTES,sizeof(GET_BLOCK_TEMPLATE_RESERVED_BYTES)-1);
  }
  
  // convert the blockchain_data to a network_block_string
  memset(data,0,strlen(data));
  if (blockchain_data_to_network_block_string(VRF_data.block_blob) == 0)
  {
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not convert the blockchain_data to a network_block_string");
  }

  // sign the network block string
  memset(data,0,strlen(data));
  if (sign_network_block_string(data,VRF_data.block_blob,0) == 0)
  {
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not sign the network block string");
  }

  // convert the network_block_string to a blockchain_data
  if (network_block_string_to_blockchain_data(VRF_data.block_blob,"0") == 0)
  {
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not sign_data");
  }

  // add the main network data nodes signature to the block
  memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[0],data,XCASH_SIGN_DATA_LENGTH);

  // convert the blockchain_data to a network_block_string
  memset(data,0,strlen(data));
  if (blockchain_data_to_network_block_string(data) == 0)
  {
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not convert the blockchain_data to a network_block_string");
  }

  // get the data hash of the network block string
  memset(data2,0,strlen(data2));
  crypto_hash_sha512((unsigned char*)data2,(const unsigned char*)data,(unsigned long long)strnlen(data,BUFFER_SIZE));

  // convert the SHA512 data hash to a string
  for (count2 = 0, count = 0; count2 < DATA_HASH_LENGTH / 2; count2++, count += 2)
  {
    sprintf(data3+count,"%02x",data2[count2] & 0xFF);
  }

  // update the reserve bytes database
  memset(data2,0,strnlen(data2,BUFFER_SIZE));
  memcpy(data2,"{\"block_height\":\"",17);
  memcpy(data2+17,current_block_height,strnlen(current_block_height,BUFFER_SIZE));
  memcpy(data2+strlen(data2),"\",\"reserve_bytes_data_hash\":\"",29);
  memcpy(data2+strlen(data2),data3,strnlen(data3,BUFFER_SIZE));
  memcpy(data2+strlen(data2),"\",\"reserve_bytes\":\"",19);
  memcpy(data2+strlen(data2),VRF_data.block_blob,strnlen(VRF_data.block_blob,BUFFER_SIZE));
  memcpy(data2+strlen(data2),"\"}",2);

  memset(data,0,strnlen(data,BUFFER_SIZE));
  memcpy(data,"reserve_bytes_",14);
  count2 = ((blockchain_data.block_height - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME) + 1;
  sprintf(data+14,"%zu",count2);

  /*if (insert_document_into_collection_json(DATABASE_NAME,data,data2,0) == 0)
  {
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not add the new block to the database");
  }*/

  // add the data hash to the network block string
  memset(data,0,strnlen(data,BUFFER_SIZE));
  if (add_data_hash_to_network_block_string(VRF_data.block_blob,data) == 0)
  {
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not add the data hash to the network block string");
  }

  while (current_UTC_date_and_time->tm_min == 4 && current_UTC_date_and_time->tm_sec == 58)
  {    
    usleep(200000); 
    get_current_UTC_time; 
  }

  // have the main network data node submit the block to the network
  /*if (submit_block_template(data,0) == 0)
  {
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not add the block to the network");
  }*/

  pointer_reset_all;
  return 1;

  #undef pointer_reset_all
  #undef START_CURRENT_ROUND_START_BLOCKS_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: data_network_node_create_block
Description: Runs the round where the main network data node will create the block and have the block verifiers sign the block. This only runs if the block producer and the 5 backup block producers cant create the block
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

int data_network_node_create_block()
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count; 
  size_t counter;
  size_t count2; 

  // threads
  pthread_t thread_id;

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; \
  free(data3); \
  data3 = NULL;

  #define DATA_NETWORK_NODE_CREATE_BLOCK_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"start_current_round_start_blocks",32); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL || data3 == NULL)
  {
    if (data == NULL)
    {
      pointer_reset(data);
    }
    if (data2 == NULL)
    {
      pointer_reset(data2);
    }
    if (data3 == NULL)
    {
      pointer_reset(data3);
    }
    memcpy(error_message.function[error_message.total],"start_current_round_start_blocks",32);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  pthread_rwlock_wrlock(&rwlock);
  // set the server message
  memset(server_message,0,strlen(server_message));
  memcpy(server_message,"MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_CREATE_NEW_BLOCK",58); 

  // set the current_round_part
  memset(current_round_part,0,strlen(current_round_part));
  memcpy(current_round_part,"1",1);

  // set the current_round_part
  memset(current_round_part_backup_node,0,strlen(current_round_part_backup_node));
  memcpy(current_round_part_backup_node,"5",1);
  pthread_rwlock_unlock(&rwlock);

  // check if the block verifier is the main network data node
  if (memcmp(xcash_wallet_public_address,network_data_nodes_list.network_data_nodes_public_address[0],XCASH_WALLET_LENGTH) != 0)
  {    
    color_print("Your block verifier is the main data network node so your block verifier will create the block\nWaiting until 4 minutes and 40 seconds of the round","green");

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
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,"0",1);
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

    memcpy(blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,blockchain_data.previous_block_hash_data,64);
    memcpy(VRF_data.vrf_alpha_string_round_part_4,blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,64);

    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memcpy(VRF_data.vrf_alpha_string_round_part_4+strlen(VRF_data.vrf_alpha_string_round_part_4),"0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",RANDOM_STRING_LENGTH);
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
      sprintf(VRF_data.vrf_secret_key_data_round_part_4+count,"%02x",VRF_data.vrf_secret_key_round_part_4[count2] & 0xFF);
    }
    for (count2 = 0, count = 0; count2 < crypto_vrf_PUBLICKEYBYTES; count2++, count += 2)
    {
      sprintf(VRF_data.vrf_public_key_data_round_part_4+count,"%02x",VRF_data.vrf_public_key_round_part_4[count2] & 0xFF);
    }
    for (count2 = 0, count = 0; count2 < strlen((const char*)VRF_data.vrf_alpha_string_round_part_4); count2++, count += 2)
    {
      sprintf(VRF_data.vrf_alpha_string_data_round_part_4+count,"%02x",VRF_data.vrf_alpha_string_round_part_4[count2] & 0xFF);
    }
    for (count2 = 0, count = 0; count2 < crypto_vrf_PROOFBYTES; count2++, count += 2)
    {
      sprintf(VRF_data.vrf_proof_data_round_part_4+count,"%02x",VRF_data.vrf_proof_round_part_4[count2] & 0xFF);
    }
    for (count2 = 0, count = 0; count2 < crypto_vrf_OUTPUTBYTES; count2++, count += 2)
    {
      sprintf(VRF_data.vrf_beta_string_data_round_part_4+count,"%02x",VRF_data.vrf_beta_string_round_part_4[count2] & 0xFF);
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
    for (count = 1; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key_data[count],"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",VRF_SECRET_KEY_LENGTH);
      memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key_data[count],"0000000000000000000000000000000000000000000000000000000000000000",VRF_PUBLIC_KEY_LENGTH);
      memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data_text[count],"0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",RANDOM_STRING_LENGTH);
      memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data[count],"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",RANDOM_STRING_LENGTH*2);
    }

    // add the next block verifiers
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    { 
      memcpy(blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address[count],current_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH);
    }

    // convert the blockchain_data to a network_block_string
    if (blockchain_data_to_network_block_string(VRF_data.block_blob) == 0)
    {
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not convert the blockchain_data to a network_block_string");
    }

    // sign the network block string
    memset(data,0,strlen(data));
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
    memset(data3,0,strlen(data3));
    memcpy(data3,"{\r\n \"message_settings\": \"MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_CREATE_NEW_BLOCK\",\r\n \"block_blob\": \"",103);
    memcpy(data3+103,VRF_data.block_blob,strnlen(VRF_data.block_blob,BUFFER_SIZE));
    memcpy(data3+strlen(data3),"\",\r\n}",5);
  
    // sign_data
    if (sign_data(data3,0) == 0)
    { 
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not sign_data");
    }

    // send the network block string to all block verifiers and add the block verifier signature to the blockchain data struct
    for (count = 0, count2 = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
      {    
        memset(data,0,strlen(data)); 
        memset(data2,0,strlen(data2));   
        if (send_and_receive_data_socket(data,current_block_verifiers_list.block_verifiers_IP_address[count],SEND_DATA_PORT,data3,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
        {
          // verify the data
          if (verify_data(data,0,1) == 0)
          {
            DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not verify data");
          }

          // parse the message
          if (parse_json_data(data,"block_blob_signature",data2) == 0 || strlen(data2) != XCASH_SIGN_DATA_LENGTH || memcmp(data2,XCASH_SIGN_DATA_PREFIX,sizeof(XCASH_SIGN_DATA_PREFIX)-1) != 0)
          {
            DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not parse the data");
          }

          // add the block verifiers network block signature to the blockchain data struct
          memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count],data2,XCASH_SIGN_DATA_LENGTH);
          
          count2++;
        }
      }
    }

    // check if at least 67 of the block verifiers created the data
    if (count2 < BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Invalid amount of block verifiers network block signatures");
    }

    // convert the blockchain_data to a network_block_string
    memset(data,0,strlen(data));
    if (blockchain_data_to_network_block_string(data) == 0)
    {
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not convert the blockchain_data to a network_block_string");
    }

    // get the previous network block string
    memset(data,0,strlen(data));
    memset(data2,0,strlen(data2));
    memset(data3,0,strlen(data3));
    sscanf(current_block_height,"%zu", &count);
    if (count < XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT)
    {
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not get the current block height");
    }
    count--;
    sprintf(data3,"%zu",count);
    count2 = ((count - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME) + 1;
    memcpy(data,"{\"block_height\":\"",17);
    memcpy(data+17,data3,strnlen(data3,BUFFER_SIZE));
    memcpy(data+strlen(data),"\"}",2);
    memset(data3,0,strlen(data3));
    memcpy(data3,"reserve_bytes_",14);
    sprintf(data3+14,"%zu",count2);
    if (read_document_field_from_collection(DATABASE_NAME,data3,data,"reserve_bytes",data2,0) == 0)
    {
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not get the previous blocks reserve bytes");
    }

    // verify the block
    memset(data3,0,strlen(data3));
    sprintf(data3,"%zu",count);
    if (count != XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT)
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
    memset(data,0,strlen(data));
    if (blockchain_data_to_network_block_string(data) == 0)
    {
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not convert the blockchain_data to a network_block_string");
    }

    // get the data hash of the network block string
    memset(data2,0,strlen(data2));
    memset(data3,0,strlen(data3));
    crypto_hash_sha512((unsigned char*)data2,(const unsigned char*)data,(unsigned long long)strnlen(data,BUFFER_SIZE));

    // convert the SHA512 data hash to a string
    for (count2 = 0, count = 0; count2 < DATA_HASH_LENGTH / 2; count2++, count += 2)
    {
      sprintf(data3+count,"%02x",data2[count2] & 0xFF);
    }

    // copy the reserve bytes data hash
    memcpy(VRF_data.reserve_bytes_data_hash,data3,DATA_HASH_LENGTH);

    // add the data hash to the network block string
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    memcpy(data2,&data[(strnlen(data,BUFFER_SIZE)) - (strnlen(strstr(data,BLOCKCHAIN_RESERVED_BYTES_START),BUFFER_SIZE) - (sizeof(BLOCKCHAIN_RESERVED_BYTES_START)-1))],((strnlen(data,BUFFER_SIZE)) - (strnlen(strstr(data,BLOCKCHAIN_RESERVED_BYTES_END),BUFFER_SIZE) - (sizeof(BLOCKCHAIN_RESERVED_BYTES_END)-1))) - ((strnlen(data,BUFFER_SIZE)) - (strnlen(strstr(data,BLOCKCHAIN_RESERVED_BYTES_START),BUFFER_SIZE) - (sizeof(BLOCKCHAIN_RESERVED_BYTES_START)-1))) - (sizeof(BLOCKCHAIN_RESERVED_BYTES_END)-1));

    // replace the reserve bytes with the network block string data hash
    if (string_replace(data,data2,data3) == 0)
    {
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not add the network block string data hash");
    }

    // submit the block to the network
    if (submit_block_template(VRF_data.block_blob,0) == 0)
    {
      DATA_NETWORK_NODE_CREATE_BLOCK_ERROR("Could not add the network block string");
    }
  }
  else
  {
    printf("Your block verifier is not the main data network node so your block verifier will wait until the network data node creates the block");
  }
   
  pointer_reset_all;
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

int start_part_4_of_round()
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3 = (char*)calloc(BUFFER_SIZE,sizeof(char));  
  size_t count = 0;
  size_t count2;
  size_t counter;
  int block_producer_backup_settings[BLOCK_PRODUCERS_BACKUP_AMOUNT];

  // threads
  pthread_t thread_id;

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; \
  free(data3); \
  data3 = NULL;

  #define START_PART_4_OF_ROUND_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"start_part_4_of_round",21); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  #define SEND_DATA_SOCKET_THREAD(message) \
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++) \
  { \
    struct send_data_socket_thread_parameters* send_data_socket_thread_parameters = malloc(sizeof(struct send_data_socket_thread_parameters)); \
    memset(send_data_socket_thread_parameters->HOST,0,strlen(send_data_socket_thread_parameters->HOST)); \
    memset(send_data_socket_thread_parameters->DATA,0,strlen(send_data_socket_thread_parameters->DATA)); \
    memcpy(send_data_socket_thread_parameters->HOST,current_block_verifiers_list.block_verifiers_IP_address[count],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count],BUFFER_SIZE)); \
    memcpy(send_data_socket_thread_parameters->DATA,message,strnlen(message,BUFFER_SIZE)); \
    if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0) \
    { \
      pthread_create(&thread_id, NULL, &send_data_socket_thread,(void *)send_data_socket_thread_parameters); \
      pthread_detach(thread_id); \
    } \
    usleep(10000000 / BLOCK_VERIFIERS_AMOUNT); \
  }

  #define RESTART_ROUND \
  memset(data,0,strlen(data)); \
  memset(data2,0,strlen(data2)); \
  memset(data3,0,strlen(data3)); \
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
  memset(VRF_data_copy->vrf_secret_key_data_round_part_4,0,strlen(VRF_data_copy->vrf_secret_key_data_round_part_4)); \
  memset(VRF_data_copy->vrf_secret_key_round_part_4,0,strlen(VRF_data_copy->vrf_secret_key_round_part_4)); \
  memset(VRF_data_copy->vrf_public_key_data_round_part_4,0,strlen(VRF_data_copy->vrf_public_key_data_round_part_4)); \
  memset(VRF_data_copy->vrf_public_key_round_part_4,0,strlen(VRF_data_copy->vrf_public_key_round_part_4)); \
  memset(VRF_data_copy->vrf_alpha_string_data_round_part_4,0,strlen(VRF_data_copy->vrf_alpha_string_data_round_part_4)); \
  memset(VRF_data_copy->vrf_alpha_string_round_part_4,0,strlen(VRF_data_copy->vrf_alpha_string_round_part_4)); \
  memset(VRF_data_copy->vrf_proof_data_round_part_4,0,strlen(VRF_data_copy->vrf_proof_data_round_part_4)); \
  memset(VRF_data_copy->vrf_proof_round_part_4,0,strlen(VRF_data_copy->vrf_proof_round_part_4)); \
  memset(VRF_data_copy->vrf_beta_string_data_round_part_4,0,strlen(VRF_data_copy->vrf_beta_string_data_round_part_4)); \
  memset(VRF_data_copy->vrf_beta_string_round_part_4,0,strlen(VRF_data_copy->vrf_beta_string_round_part_4)); \
  memset(VRF_data_copy->block_blob,0,strlen(VRF_data_copy->block_blob)); \
  memset(VRF_data_copy->reserve_bytes_data_hash,0,strlen(VRF_data_copy->reserve_bytes_data_hash)); \
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++) \
  { \
    memset(VRF_data.block_verifiers_vrf_secret_key_data[count],0,strlen(VRF_data.block_verifiers_vrf_secret_key_data[count])); \
    memset(VRF_data.block_verifiers_vrf_secret_key[count],0,strlen(VRF_data.block_verifiers_vrf_secret_key[count])); \
    memset(VRF_data.block_verifiers_vrf_public_key_data[count],0,strlen(VRF_data.block_verifiers_vrf_public_key_data[count])); \
    memset(VRF_data.block_verifiers_vrf_public_key[count],0,strlen(VRF_data.block_verifiers_vrf_public_key[count])); \
    memset(VRF_data.block_verifiers_random_data[count],0,strlen(VRF_data.block_verifiers_random_data[count])); \
    memset(VRF_data.block_blob_signature[count],0,strlen(VRF_data.block_blob_signature[count])); \
    memset(VRF_data_copy->block_verifiers_vrf_secret_key_data[count],0,strlen(VRF_data_copy->block_verifiers_vrf_secret_key_data[count])); \
    memset(VRF_data_copy->block_verifiers_vrf_secret_key[count],0,strlen(VRF_data_copy->block_verifiers_vrf_secret_key[count])); \
    memset(VRF_data_copy->block_verifiers_vrf_public_key_data[count],0,strlen(VRF_data_copy->block_verifiers_vrf_public_key_data[count])); \
    memset(VRF_data_copy->block_verifiers_vrf_public_key[count],0,strlen(VRF_data_copy->block_verifiers_vrf_public_key[count])); \
    memset(VRF_data_copy->block_verifiers_random_data[count],0,strlen(VRF_data_copy->block_verifiers_random_data[count])); \
    memset(VRF_data_copy->block_blob_signature[count],0,strlen(VRF_data_copy->block_blob_signature[count])); \
  } \
  pthread_rwlock_wrlock(&rwlock); \
  if (memcmp(current_round_part_backup_node,"0",1) == 0) \
  { \
    memset(current_round_part,0,strlen(current_round_part)); \
    memcpy(current_round_part,"1",1); \
    memset(current_round_part_backup_node,0,strlen(current_round_part_backup_node)); \
    memcpy(current_round_part_backup_node,"1",1); \
  } \
  else if (memcmp(current_round_part_backup_node,"1",1) == 0) \
  { \
    memset(current_round_part,0,strlen(current_round_part)); \
    memcpy(current_round_part,"1",1); \
    memset(current_round_part_backup_node,0,strlen(current_round_part_backup_node)); \
    memcpy(current_round_part_backup_node,"2",1); \
  } \
  else if (memcmp(current_round_part_backup_node,"2",1) == 0) \
  { \
    memset(current_round_part,0,strlen(current_round_part)); \
    memcpy(current_round_part,"1",1); \
    memset(current_round_part_backup_node,0,strlen(current_round_part_backup_node)); \
    memcpy(current_round_part_backup_node,"3",1); \
  } \
  else if (memcmp(current_round_part_backup_node,"3",1) == 0) \
  { \
    memset(current_round_part,0,strlen(current_round_part)); \
    memcpy(current_round_part,"1",1); \
    memset(current_round_part_backup_node,0,strlen(current_round_part_backup_node)); \
    memcpy(current_round_part_backup_node,"4",1); \
  } \
  else if (memcmp(current_round_part_backup_node,"4",1) == 0) \
  { \
    memset(current_round_part,0,strlen(current_round_part)); \
    memcpy(current_round_part,"1",1); \
    memset(current_round_part_backup_node,0,strlen(current_round_part_backup_node)); \
    memcpy(current_round_part_backup_node,"5",1); \
  } \
  else if (memcmp(current_round_part_backup_node,"5",1) == 0) \
  { \
    data_network_node_create_block(); \
  } \
  pthread_rwlock_unlock(&rwlock); \
  goto start;  

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
    memcpy(error_message.function[error_message.total],"start_part_4_of_round",21);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  start:

    pthread_rwlock_wrlock(&rwlock);
    // set the server message
    memset(server_message,0,strlen(server_message));
    memcpy(server_message,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA",43); 

    // set the current_round_part
    memset(current_round_part,0,strlen(current_round_part));
    memcpy(current_round_part,"1",1);
    pthread_rwlock_unlock(&rwlock);
  
    // create a random VRF public key and secret key
    if (create_random_VRF_keys((unsigned char*)VRF_data.vrf_public_key_round_part_4,(unsigned char*)VRF_data.vrf_secret_key_round_part_4) != 1 || crypto_vrf_is_valid_key((const unsigned char*)VRF_data.vrf_public_key_round_part_4) != 1)
    {
      START_PART_4_OF_ROUND_ERROR("Could not create the VRF secret key or VRF public key for the VRF data");
    }  

    // convert the VRF secret key to hexadecimal
    for (count = 0, counter = 0; count < crypto_vrf_SECRETKEYBYTES; count++, counter += 2)
    {
      sprintf(VRF_data.vrf_secret_key_data_round_part_4+counter,"%02x",VRF_data.vrf_secret_key_round_part_4[count] & 0xFF);
    }

    // convert the VRF public key to hexadecimal
    for (count = 0, counter = 0; count < crypto_vrf_PUBLICKEYBYTES; count++, counter += 2)
    {
      sprintf(VRF_data.vrf_public_key_data_round_part_4+counter,"%02x",VRF_data.vrf_public_key_round_part_4[count] & 0xFF);
    } 

    // create the message
    memset(data3,0,strlen(data3));
    memcpy(data3,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA\",\r\n \"vrf_secret_key\": \"",92);
    memcpy(data3+92,VRF_data.vrf_secret_key_data_round_part_4,VRF_SECRET_KEY_LENGTH);
    memcpy(data3+220,"\",\r\n \"vrf_public_key\": \"",24);
    memcpy(data3+244,VRF_data.vrf_public_key_data_round_part_4,VRF_PUBLIC_KEY_LENGTH);
    memcpy(data3+308,"\",\r\n \"random_data\": \"",21);
  
    // create random data to use in the alpha string of the VRF data
    memset(data,0,strlen(data));
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
        memcpy(VRF_data_copy->block_verifiers_vrf_secret_key[count],VRF_data.vrf_secret_key_round_part_4,crypto_vrf_SECRETKEYBYTES);
        memcpy(VRF_data_copy->block_verifiers_vrf_secret_key_data[count],VRF_data.vrf_secret_key_data_round_part_4,VRF_SECRET_KEY_LENGTH);
        memcpy(VRF_data_copy->block_verifiers_vrf_public_key[count],VRF_data.vrf_public_key_round_part_4,crypto_vrf_PUBLICKEYBYTES);
        memcpy(VRF_data_copy->block_verifiers_vrf_public_key_data[count],VRF_data.vrf_public_key_data_round_part_4,VRF_PUBLIC_KEY_LENGTH);
        memcpy(VRF_data_copy->block_verifiers_random_data[count],data,RANDOM_STRING_LENGTH);
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
    sleep(10);

    // process the data
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memcpy(VRF_data.block_verifiers_vrf_secret_key_data[count],VRF_data_copy->block_verifiers_vrf_secret_key_data[count],VRF_SECRET_KEY_LENGTH);
      memcpy(VRF_data.block_verifiers_vrf_secret_key[count],VRF_data_copy->block_verifiers_vrf_secret_key[count],crypto_vrf_SECRETKEYBYTES);
      memcpy(VRF_data.block_verifiers_vrf_public_key_data[count],VRF_data_copy->block_verifiers_vrf_public_key_data[count],VRF_PUBLIC_KEY_LENGTH);
      memcpy(VRF_data.block_verifiers_vrf_public_key[count],VRF_data_copy->block_verifiers_vrf_public_key[count],crypto_vrf_PUBLICKEYBYTES);
      memcpy(VRF_data.block_verifiers_random_data[count],VRF_data_copy->block_verifiers_random_data[count],RANDOM_STRING_LENGTH);
    }

    // check if at least 67 of the block verifiers created the data
    for (count = 0, counter = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (strlen(VRF_data.block_verifiers_vrf_secret_key[count]) == crypto_vrf_SECRETKEYBYTES && strlen(VRF_data.block_verifiers_vrf_public_key[count]) == crypto_vrf_PUBLICKEYBYTES && strlen(VRF_data.block_verifiers_random_data[count]) == RANDOM_STRING_LENGTH)
      {
        counter++;
      }
    }
    if (counter < BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      RESTART_ROUND;
    }

  

    // at this point all block verifiers should have the all of the other block verifiers secret key, public key and random data

    pthread_rwlock_wrlock(&rwlock);
    // set the server message
    memset(server_message,0,strlen(server_message));
    memcpy(server_message,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND_CREATE_NEW_BLOCK",52); 

    // set the current_round_part
    memset(current_round_part,0,strlen(current_round_part));
    memcpy(current_round_part,"2",1);
    pthread_rwlock_unlock(&rwlock);

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
        memcpy(VRF_data.vrf_alpha_string_round_part_4+strlen(VRF_data.vrf_alpha_string_round_part_4),"0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",RANDOM_STRING_LENGTH);
      }
    }

    // convert the vrf alpha string to a string
    for (count2 = 0, count = 0; count2 < (((RANDOM_STRING_LENGTH*2)*BLOCK_VERIFIERS_AMOUNT) + 128) / 2; count2++, count += 2)
    {
      sprintf(VRF_data.vrf_alpha_string_data_round_part_4+count,"%02x",VRF_data.vrf_alpha_string_round_part_4[count2] & 0xFF);
    }

    memset(data,0,strlen(data));
    memset(data2,0,strlen(data2));
    crypto_hash_sha512((unsigned char*)data,(const unsigned char*)VRF_data.vrf_alpha_string_data_round_part_4,strlen(VRF_data.vrf_alpha_string_data_round_part_4));

    // convert the SHA512 data hash to a string
    for (count2 = 0, count = 0; count2 < DATA_HASH_LENGTH / 2; count2++, count += 2)
    {
      sprintf(data2+count,"%02x",data[count2] & 0xFF);
    }

    // check what block verifiers vrf secret key and vrf public key to use
    for (count = 0; count < DATA_HASH_LENGTH; count += 2)
    {
      memset(data,0,strlen(data));
      memcpy(data,&data2[count],2);
      counter = (int)strtol(data, NULL, 16);  
   
      // if it is not in the range of 01 - C8 then skip the byte
      if (counter != 0 && counter <= 200)
      {
        counter = counter % BLOCK_VERIFIERS_AMOUNT;
        break;
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
      sprintf(VRF_data.vrf_proof_data_round_part_4+count,"%02x",VRF_data.vrf_proof_round_part_4[counter] & 0xFF);
    }
    for (counter = 0, count = 0; counter < crypto_vrf_OUTPUTBYTES; counter++, count += 2)
    {
      sprintf(VRF_data.vrf_beta_string_data_round_part_4+count,"%02x",VRF_data.vrf_beta_string_round_part_4[counter] & 0xFF);
    }

    memset(VRF_data.block_blob,0,strlen(VRF_data.block_blob));
    memset(VRF_data_copy->block_blob,0,strlen(VRF_data_copy->block_blob));

    // create the block template and send it to all block verifiers if the block verifier is the block producer
    if ((memcmp(current_round_part_backup_node,"0",1) == 0 && memcmp(main_nodes_list.block_producer_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"1",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_1_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"2",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_2_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"3",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_3_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"4",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_4_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"5",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_5_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0))
    {
      memset(VRF_data_copy->block_blob,0,strlen(VRF_data_copy->block_blob));
      if (get_block_template(VRF_data_copy->block_blob,0) == 0)
      {
        START_PART_4_OF_ROUND_ERROR("Could not get a block template");
      }  

      // create the message
      memset(data,0,strlen(data));
      memcpy(data,"{\r\n \"message_settings\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND_CREATE_NEW_BLOCK\",\r\n \"block_blob\": \"",97);
      memcpy(data+97,VRF_data_copy->block_blob,strnlen(VRF_data_copy->block_blob,BUFFER_SIZE));
      memcpy(data+strlen(data),"\",\r\n}",5);

      // sign_data
      if (sign_data(data,0) == 0)
      { 
        START_PART_4_OF_ROUND_ERROR("Could not sign_data");
      }

      // send the message to all block verifiers
      SEND_DATA_SOCKET_THREAD(data);

      // wait for the block verifiers to process main node data
      sleep(10);
    }
    else
    {
      // wait for the block verifiers to process main node data
      sleep(20);
    }



    // at this point all block verifiers should have the same VRF data and the network block

    pthread_rwlock_wrlock(&rwlock);
    // set the server message
    memset(server_message,0,strlen(server_message));
    memcpy(server_message,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE",55); 

    // set the current_round_part
    memset(current_round_part,0,strlen(current_round_part));
    memcpy(current_round_part,"3",1);
    pthread_rwlock_unlock(&rwlock);

    memcpy(VRF_data.block_blob,VRF_data_copy->block_blob,strnlen(VRF_data_copy->block_blob,BUFFER_SIZE));

    // check if the network block string was created from the correct block verifier
    if (memcmp(VRF_data.block_blob,"",1) == 0)
    {
      START_PART_4_OF_ROUND_ERROR("Could not receive the network block string from the block producer");
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

      memcpy(blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address[count],current_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH);
      memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count],GET_BLOCK_TEMPLATE_RESERVED_BYTES,sizeof(GET_BLOCK_TEMPLATE_RESERVED_BYTES)-1);

      for (counter = 0, count2 = 0; counter < RANDOM_STRING_LENGTH; counter++, count2 += 2)
      {
        sprintf(blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data[count]+count2,"%02x",VRF_data.block_verifiers_random_data[count][counter] & 0xFF);
      }
    }

    memcpy(blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,blockchain_data.previous_block_hash_data,64);

    // convert the blockchain_data to a network_block_string
    memset(data,0,strlen(data));
    if (blockchain_data_to_network_block_string(VRF_data.block_blob) == 0)
    {
      START_PART_4_OF_ROUND_ERROR("Could not convert the blockchain_data to a network_block_string");
    }

    // sign the network block string
    memset(data,0,strlen(data));
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
        memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count],data,XCASH_SIGN_DATA_LENGTH);
      }
    }

    // create the message
    memset(data3,0,strlen(data3));
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
    sleep(10);



    // at this point all block verifiers should have the same VRF data, network block string and all block verifiers signed data

    pthread_rwlock_wrlock(&rwlock);
    // set the server message
    memset(server_message,0,strlen(server_message));
    memcpy(server_message,"NODES_TO_NODES_VOTE_RESULTS",27); 

    // set the current_round_part
    memset(current_round_part,0,strlen(current_round_part));
    memcpy(current_round_part,"4",1);
    pthread_rwlock_unlock(&rwlock);

    // process the data and add the block verifiers signatures to the block
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
      {
        memcpy(VRF_data.block_blob_signature[count],VRF_data_copy->block_blob_signature[count],XCASH_SIGN_DATA_LENGTH);
        memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count],VRF_data.block_blob_signature[count],XCASH_SIGN_DATA_LENGTH);
      }
    }

    // convert the blockchain_data to a network_block_string
    memset(data,0,strlen(data));
    if (blockchain_data_to_network_block_string(data) == 0)
    {
      START_PART_4_OF_ROUND_ERROR("Could not convert the blockchain_data to a network_block_string");
    }

    // get the previous network block string
    memset(data,0,strlen(data));
    memset(data2,0,strlen(data2));
    memset(data3,0,strlen(data3));
    sscanf(current_block_height,"%zu", &count);
    if (count < XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT)
    {
      START_PART_4_OF_ROUND_ERROR("Could not get the current block height");
    }
    count--;
    sprintf(data3,"%zu",count);
    count2 = ((count - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME) + 1;
    memcpy(data,"{\"block_height\":\"",17);
    memcpy(data+17,data3,strnlen(data3,BUFFER_SIZE));
    memcpy(data+strlen(data),"\"}",2);
    memset(data3,0,strlen(data3));
    memcpy(data3,"reserve_bytes_",14);
    sprintf(data3+14,"%zu",count2);
    if (read_document_field_from_collection(DATABASE_NAME,data3,data,"reserve_bytes",data2,0) == 0)
    {
      START_PART_4_OF_ROUND_ERROR("Could not get the previous blocks reserve bytes");
    }

    // verify the block
    memset(data3,0,strlen(data3));
    sprintf(data3,"%zu",count);
    if (count != XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT)
    {    
      if (verify_network_block_data(1,1,1,data3,data2) == 0)
      {
        START_PART_4_OF_ROUND_ERROR("The MAIN_NODES_TO_NODES_PART_4_OF_ROUND message is invalid");
      }
    }
    else
    {
      if (verify_network_block_data(0,1,1,data3,data2) == 0)
      {
        START_PART_4_OF_ROUND_ERROR("The MAIN_NODES_TO_NODES_PART_4_OF_ROUND message is invalid");
      }
    }

    // convert the blockchain_data to a network_block_string
    memset(data,0,strlen(data));
    if (blockchain_data_to_network_block_string(data) == 0)
    {
      START_PART_4_OF_ROUND_ERROR("Could not convert the blockchain_data to a network_block_string");
    }

    // get the data hash of the network block string
    memset(data2,0,strlen(data2));
    memset(data3,0,strlen(data3));
    crypto_hash_sha512((unsigned char*)data2,(const unsigned char*)data,(unsigned long long)strnlen(data,BUFFER_SIZE));

    // convert the SHA512 data hash to a string
    for (count2 = 0, count = 0; count2 < DATA_HASH_LENGTH / 2; count2++, count += 2)
    {
      sprintf(data3+count,"%02x",data2[count2] & 0xFF);
    }

    memset(current_round_part_vote_data->current_vote_results,0,strlen(current_round_part_vote_data->current_vote_results));
    memcpy(current_round_part_vote_data->current_vote_results,data3,DATA_HASH_LENGTH);

    // create the message
    memset(data3,0,strlen(data3));
    memcpy(data3,"{\r\n \"message_settings\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n \"vote_settings\": \"valid\",\r\n \"vote_data\": \"",99);  
    memcpy(data3+strlen(data3),current_round_part_vote_data->current_vote_results,DATA_HASH_LENGTH);
    memcpy(data3+strlen(data3),"\",\r\n}",5); 

    // sign_data
    if (sign_data(data3,0) == 0)
    { 
      START_PART_4_OF_ROUND_ERROR("Could not sign_data");
    }

    // reset the current_round_part_vote_data->vote_results_valid struct
    current_round_part_vote_data->vote_results_valid = 1;
    current_round_part_vote_data->vote_results_invalid = 0;

    while (current_UTC_date_and_time->tm_min != 32 && current_UTC_date_and_time->tm_min != 0)
    {    
      usleep(200000); 
      get_current_UTC_time; 
    }

    // send the message to all block verifiers
    SEND_DATA_SOCKET_THREAD(data3);

    // wait for the block verifiers to process the votes
    sleep(10);

    // process the vote results
    if (current_round_part_vote_data->vote_results_valid < BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      RESTART_ROUND;
    }



    // at this point all block verifiers have the same network block string with all of the VRF data

    // copy the reserve bytes data hash
    memcpy(VRF_data.reserve_bytes_data_hash,current_round_part_vote_data->current_vote_results,DATA_HASH_LENGTH);

    // add the data hash to the network block string
    memset(data,0,strnlen(data,BUFFER_SIZE));
    memcpy(data,&VRF_data.block_blob[(strnlen(VRF_data.block_blob,BUFFER_SIZE)) - (strnlen(strstr(VRF_data.block_blob,BLOCKCHAIN_RESERVED_BYTES_START),BUFFER_SIZE) - (sizeof(BLOCKCHAIN_RESERVED_BYTES_START)-1))],((strnlen(VRF_data.block_blob,BUFFER_SIZE)) - (strnlen(strstr(VRF_data.block_blob,BLOCKCHAIN_RESERVED_BYTES_END),BUFFER_SIZE) - (sizeof(BLOCKCHAIN_RESERVED_BYTES_END)-1))) - ((strnlen(VRF_data.block_blob,BUFFER_SIZE)) - (strnlen(strstr(VRF_data.block_blob,BLOCKCHAIN_RESERVED_BYTES_START),BUFFER_SIZE) - (sizeof(BLOCKCHAIN_RESERVED_BYTES_START)-1))) - (sizeof(BLOCKCHAIN_RESERVED_BYTES_END)-1));

    // replace the reserve bytes with the network block string data hash
    if (string_replace(VRF_data.block_blob,data,VRF_data.reserve_bytes_data_hash) == 0)
    {
      START_PART_4_OF_ROUND_ERROR("Could not add the network block string data hash");
    }color_print(VRF_data.block_blob,"yellow");

    // have the block producer submit the block to the network
    if (main_network_data_node_create_block == 1)
    {
      submit_block_template(VRF_data.block_blob,0);
    }
    else
    {
      if (memcmp(current_round_part_backup_node,"0",1) == 0 && memcmp(main_nodes_list.block_producer_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0)
      {
        submit_block_template(VRF_data.block_blob,0);
      }
      else if (memcmp(current_round_part_backup_node,"1",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_1_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0)
      {
        submit_block_template(VRF_data.block_blob,0);
      }
      else if (memcmp(current_round_part_backup_node,"2",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_2_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0)
      {
        submit_block_template(VRF_data.block_blob,0);
      }
      else if (memcmp(current_round_part_backup_node,"3",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_3_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0)
      {
        submit_block_template(VRF_data.block_blob,0);
      }
      else if (memcmp(current_round_part_backup_node,"4",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_4_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0)
      {
        submit_block_template(VRF_data.block_blob,0);
      }
      else if (memcmp(current_round_part_backup_node,"5",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_5_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0)
      {
        submit_block_template(VRF_data.block_blob,0);
      }
    }    
    sleep(2);
    if (memcmp(data3,current_block_height,strnlen(current_block_height,BUFFER_SIZE)) == 0)
    {
      // the block was not submitted to the network. Loop through each network data node until it is submitted to the network
      for (count = 0; count < NETWORK_DATA_NODES_AMOUNT; count++)
      {
        if (network_data_node_settings == 1 && memcmp(network_data_nodes_list.network_data_nodes_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0)
        {
          submit_block_template(VRF_data.block_blob,0);
          if (memcmp(data3,current_block_height,strnlen(current_block_height,BUFFER_SIZE)) != 0)
          {
            break;
          }
        }
        sleep(2);
      }    
    }

    pointer_reset_all;
    return 1;

    #undef pointer_reset_all
    #undef START_PART_4_OF_ROUND_ERROR
    #undef SEND_DATA_SOCKET_THREAD
    #undef RESTART_ROUND
}



/*
-----------------------------------------------------------------------------------------------------------
Name: update_block_verifiers_list
Description: Updates the block verifiers list struct
-----------------------------------------------------------------------------------------------------------
*/

int update_block_verifiers_list()
{
  // Variables
  struct database_multiple_documents_fields database_multiple_documents_fields;
  size_t count;
  size_t count2;

  // define macros
  #define DATABASE_COLLECTION "delegates"

  // reset the previous_block_verifiers_list struct
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memset(previous_block_verifiers_list.block_verifiers_name[count],0,strlen(previous_block_verifiers_list.block_verifiers_name[count]));
    memset(previous_block_verifiers_list.block_verifiers_public_address[count],0,strlen(previous_block_verifiers_list.block_verifiers_public_address[count]));
    memset(previous_block_verifiers_list.block_verifiers_IP_address[count],0,strlen(previous_block_verifiers_list.block_verifiers_IP_address[count]));
  }

  // copy the current_block_verifiers_list to the previous_block_verifiers_list
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memcpy(previous_block_verifiers_list.block_verifiers_name[count],current_block_verifiers_list.block_verifiers_name[count],strnlen(current_block_verifiers_list.block_verifiers_name[count],BLOCK_VERIFIERS_NAME_TOTAL_LENGTH+1));
    memcpy(previous_block_verifiers_list.block_verifiers_public_address[count],current_block_verifiers_list.block_verifiers_public_address[count],strnlen(current_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH+1));
    memcpy(previous_block_verifiers_list.block_verifiers_IP_address[count],current_block_verifiers_list.block_verifiers_IP_address[count],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count],BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
  }

  // reset the current_block_verifiers_list struct
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memset(current_block_verifiers_list.block_verifiers_name[count],0,strlen(current_block_verifiers_list.block_verifiers_name[count]));
    memset(current_block_verifiers_list.block_verifiers_public_address[count],0,strlen(current_block_verifiers_list.block_verifiers_public_address[count]));
    memset(current_block_verifiers_list.block_verifiers_IP_address[count],0,strlen(current_block_verifiers_list.block_verifiers_IP_address[count]));
  }  

  // copy the next_block_verifiers_list to the current_block_verifiers_list
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memcpy(current_block_verifiers_list.block_verifiers_name[count],next_block_verifiers_list.block_verifiers_name[count],strnlen(next_block_verifiers_list.block_verifiers_name[count],BLOCK_VERIFIERS_NAME_TOTAL_LENGTH+1));
    memcpy(current_block_verifiers_list.block_verifiers_public_address[count],next_block_verifiers_list.block_verifiers_public_address[count],strnlen(next_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH+1));
    memcpy(current_block_verifiers_list.block_verifiers_IP_address[count],next_block_verifiers_list.block_verifiers_IP_address[count],strnlen(next_block_verifiers_list.block_verifiers_IP_address[count],BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
  }

  // reset the next_block_verifiers_list struct
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memset(next_block_verifiers_list.block_verifiers_name[count],0,strlen(next_block_verifiers_list.block_verifiers_name[count]));
    memset(next_block_verifiers_list.block_verifiers_public_address[count],0,strlen(next_block_verifiers_list.block_verifiers_public_address[count]));
    memset(next_block_verifiers_list.block_verifiers_IP_address[count],0,strlen(next_block_verifiers_list.block_verifiers_IP_address[count]));
  }

  // initialize the database_multiple_documents_fields struct 
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    for (count2 = 0; count2 < 17; count2++)
    {
      database_multiple_documents_fields.item[count][count2] = (char*)calloc(BUFFER_SIZE,sizeof(char));
      database_multiple_documents_fields.value[count][count2] = (char*)calloc(BUFFER_SIZE,sizeof(char));
    }    

    if (database_multiple_documents_fields.item[count][count2] == NULL || database_multiple_documents_fields.value[count][count2] == NULL)
    {
      memcpy(error_message.function[error_message.total],"update_block_verifiers_list",27);
      memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
      error_message.total++;
      print_error_message;  
      exit(0);
    }
  } 
  database_multiple_documents_fields.document_count = 0;
  database_multiple_documents_fields.database_fields_count = 0;

  // get the top 100 delegates by total votes
  if (read_multiple_documents_all_fields_from_collection(DATABASE_NAME,DATABASE_COLLECTION,"",&database_multiple_documents_fields,1,BLOCK_VERIFIERS_AMOUNT,1,"total_vote_count",0) == 0)
  {
    memcpy(error_message.function[error_message.total],"update_block_verifiers_list",27);
    memcpy(error_message.data[error_message.total],"Could not get the top 100 delegates for the next round. This means that you will not be able to particpate in the next round",163);
    error_message.total++;
    return 0;
  }

  // copy the database_multiple_documents_fields to the next_block_verifiers_list
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    for (count2 = 0; count2 < 17; count2++)
    {
      if (memcmp(database_multiple_documents_fields.item[count][count2],"delegate_name",13) == 0)
      {
        memcpy(next_block_verifiers_list.block_verifiers_name[count],database_multiple_documents_fields.value[count][count2],strnlen(database_multiple_documents_fields.value[count][count2],BLOCK_VERIFIERS_NAME_TOTAL_LENGTH+1));
      }
      if (memcmp(database_multiple_documents_fields.item[count][count2],"public_address",14) == 0)
      {
        memcpy(next_block_verifiers_list.block_verifiers_public_address[count],database_multiple_documents_fields.value[count][count2],strnlen(database_multiple_documents_fields.value[count][count2],BLOCK_VERIFIERS_NAME_TOTAL_LENGTH+1));
      }
      if (memcmp(database_multiple_documents_fields.item[count][count2],"IP_address",10) == 0)
      {
        memcpy(next_block_verifiers_list.block_verifiers_IP_address[count],database_multiple_documents_fields.value[count][count2],strnlen(database_multiple_documents_fields.value[count][count2],BLOCK_VERIFIERS_NAME_TOTAL_LENGTH+1));
      }
    }
  }

  // reset the database_multiple_documents_fields
  for (count = 0; count < 150; count++)
  {
    for (count2 = 0; count2 < 17; count2++)
    {
      pointer_reset(database_multiple_documents_fields.item[count][count2]);
      pointer_reset(database_multiple_documents_fields.value[count][count2]);
    }
  }  
  return 1;

  #undef DATABASE_COLLECTION
}



/*
-----------------------------------------------------------------------------------------------------------
Name: update_databases
Description: Updates the databases
-----------------------------------------------------------------------------------------------------------
*/

int update_databases()
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;
  size_t count2;

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; \
  free(data3); \
  data3 = NULL;

  #define UPDATE_DATABASE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"update_databases",16); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL)
  {
    if (data == NULL)
    {
      pointer_reset(data);
    }
    if (data2 == NULL)
    {
      pointer_reset(data2);
    }
    memcpy(error_message.function[error_message.total],"update_databases",16);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  // get the previous block height
  sscanf(current_block_height, "%zu", &count);
  if (count < XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT)
  {
    UPDATE_DATABASE_ERROR("Could not get the current block height");
  }
  count--;
  sprintf(data,"%zu",count);

  // update the databases from the last rounds statistics
  // create the message
  memcpy(data2,"{\"block_height\":\"",17);
  memcpy(data2+strlen(data2),data,strnlen(data,BUFFER_SIZE));
  memcpy(data2+strlen(data2),"\",\"reserve_bytes_data_hash\":\"",29);
  memcpy(data2+strlen(data2),VRF_data.reserve_bytes_data_hash,DATA_HASH_LENGTH);
  memcpy(data2+strlen(data2),"\",\"reserve_bytes\":\"",19);
  memcpy(data2+strlen(data2),VRF_data.block_blob,strnlen(VRF_data.block_blob,BUFFER_SIZE));
  memcpy(data2+strlen(data2),"\"}",2);

  memcpy(data3,"reserve_bytes_",14);
  count2 = ((count - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME) + 1;
  sprintf(data3+14,"%zu",count2);

  if (insert_document_into_collection_json(DATABASE_NAME,data3,data2,0) == 0)
  {
    UPDATE_DATABASE_ERROR("Could not add the new block to the database");
  }

  if (add_block_verifiers_round_statistics((const char*)data) == 0)
  {
    UPDATE_DATABASE_ERROR("Could not update the block verifiers round statistics");
  }

  if (add_round_statistics() == 0)
  {
    UPDATE_DATABASE_ERROR("Could not update the round statistics");
  }

  pointer_reset_all;  
  return 1;

  #undef pointer_reset_all
  #undef UPDATE_DATABASE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: add_block_verifiers_round_statistics
Description: Adds the block verifier statistics to the database after adding the block to the network
Parameters:
  BLOCK_HEIGHT - The block height
-----------------------------------------------------------------------------------------------------------
*/

int add_block_verifiers_round_statistics(const char* BLOCK_HEIGHT)
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;
  size_t count2;
  size_t number;

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; \
  free(message); \
  message = NULL;
  
  #define ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"add_block_verifiers_round_statistics",36); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

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
    memcpy(error_message.function[error_message.total],"add_block_verifiers_round_statistics",36);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
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
    if (read_document_field_from_collection(DATABASE_NAME,DATABASE_COLLECTION,message,"block_verifier_total_rounds",data,0) == 0)
    {
      ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not read the block_verifier_total_rounds from the database");
    }
    count2 = strnlen(data,BUFFER_SIZE);
    sscanf(data, "%zu", &number);
    number++;
    memset(data,0,strnlen(data,BUFFER_SIZE));
    memcpy(data,"{\"block_verifier_total_rounds\":\"",32);
    sprintf(data+32,"%zu",number); 
    memcpy(data+32+count2,"\"}",2);
    if (update_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,message,data,0) == 0)
    {
      ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not update the block_verifier_total_rounds in the database");
    }

    // add one to the block_verifier_online_total_rounds for every block verifier that is currently online
    if (send_data_socket((const char*)previous_block_verifiers_list.block_verifiers_IP_address[count],SEND_DATA_PORT,"") == 1)
    {
      memset(data,0,strnlen(data,BUFFER_SIZE));
      if (read_document_field_from_collection(DATABASE_NAME,DATABASE_COLLECTION,message,"block_verifier_online_total_rounds",data,0) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not read the block_verifier_online_total_rounds from the database");
      }
      count2 = strnlen(data,BUFFER_SIZE);
      sscanf(data, "%zu", &number);
      number++;
      memset(data,0,strnlen(data,BUFFER_SIZE));
      memcpy(data,"{\"block_verifier_online_total_rounds\":\"",39);
      sprintf(data+39,"%zu",number); 
      memcpy(data+39+count2,"\"}",2);
      if (update_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,message,data,0) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not update the block_verifier_online_total_rounds in the database");
      }
    }

    // add one to the block_producer_total_rounds and the current block height to the block_producer_block_heights if the public address is the block producer
    if ((memcmp(current_round_part_backup_node,"0",1) == 0 && memcmp(previous_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"1",1) == 0 && memcmp(previous_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_1_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"2",1) == 0 && memcmp(previous_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_2_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"3",1) == 0 && memcmp(previous_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_3_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"4",1) == 0 && memcmp(previous_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_4_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"5",1) == 0 && memcmp(previous_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_5_public_address,XCASH_WALLET_LENGTH) == 0))
    {
      memset(data,0,strnlen(data,BUFFER_SIZE));
      if (read_document_field_from_collection(DATABASE_NAME,DATABASE_COLLECTION,message,"block_producer_total_rounds",data,0) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not read the block_producer_total_rounds from the database");
      }
      count2 = strnlen(data,BUFFER_SIZE);
      sscanf(data, "%zu", &number);
      number++;
      memset(data,0,strnlen(data,BUFFER_SIZE));
      memcpy(data,"{\"block_producer_total_rounds\":\"",39);
      sprintf(data+39,"%zu",number); 
      memcpy(data+39+count2,"\"}",2);
      if (update_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,message,data,0) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not update the block_producer_total_rounds in the database");
      }

      memset(data,0,strnlen(data,BUFFER_SIZE));
      if (read_document_field_from_collection(DATABASE_NAME,DATABASE_COLLECTION,message,"block_producer_block_heights",data,0) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not read the block_producer_block_heights from the database");
      }      
      memcpy(data,",",1);
      memcpy(data+1,BLOCK_HEIGHT,strnlen(BLOCK_HEIGHT,BUFFER_SIZE));
      memset(data2,0,strnlen(data2,BUFFER_SIZE));
      memcpy(data2,"{\"block_producer_block_heights\":\"",33);
      memcpy(data2+33,data,strnlen(data,BUFFER_SIZE));
      memcpy(data2+33+strnlen(data,BUFFER_SIZE),"\"}",2);
      if (update_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,message,data2,0) == 0)
      {
        ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR("Could not update the block_producer_block_heights in the database");
      }
    }
  }
  return 1;

  #undef DATABASE_COLLECTION
  #undef pointer_reset_all
  #undef ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_ERROR
}




/*
-----------------------------------------------------------------------------------------------------------
Name: add_round_statistics
Description: Adds the round statistics to the database after adding the block to the network
-----------------------------------------------------------------------------------------------------------
*/

int add_round_statistics()
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
  #define DATABASE_COLLECTION "statistics"
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
  message6 = NULL;

  #define ADD_ROUND_STATISTICS_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"add_round_statistics",20); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset(data); \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL || data3 == NULL || settings == NULL || delegates_name == NULL || block_verifier_total_rounds_delegates_name == NULL || best_block_verifier_online_percentage_delegate_name == NULL || most_block_producer_total_rounds_delegate_name == NULL || message1 == NULL || message2 == NULL || message3 == NULL || message4 == NULL || message5 == NULL || message6 == NULL)
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
    memcpy(error_message.function[error_message.total],"add_round_statistics",20);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }  
  
  // set the collection
  collection = mongoc_client_get_collection(database_client, DATABASE_NAME,"delegates");
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

  // update the database
  if (update_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,MESSAGE,message1,0) == 0 || update_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,MESSAGE,message2,0) == 0 || update_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,MESSAGE,message3,0) == 0 || update_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,MESSAGE,message4,0) == 0 || update_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,MESSAGE,message5,0) == 0 || update_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,MESSAGE,message6,0) == 0)
  {
    ADD_ROUND_STATISTICS_ERROR("Could not update the round statistics in the database");
  }

  return 1;

  #undef DATABASE_COLLECTION
  #undef MESSAGE
  #undef pointer_reset_all
  #undef ADD_ROUND_STATISTICS_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: check_if_databases_are_synced
Description: Checks if the databases are synced, and if not syncs the databases
-----------------------------------------------------------------------------------------------------------
*/

int check_if_databases_are_synced()
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;

  // define macros
  #define CHECK_IF_DATABASES_ARE_SYNCED_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"check_if_databases_are_synced",29); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset(data); \
  return 0;

  print_start_message("Checking if databases are synced");

  // get the previous block height
  sscanf(current_block_height, "%zu", &count);
  count--;
  sprintf(data,"%zu",count);

  // check if your reserve proofs database is synced
  if (sync_check_reserve_proofs_database() == 0)
  {
    CHECK_IF_DATABASES_ARE_SYNCED_ERROR("Could not check if the reserve proofs database is updated. This means you might need to sync the reserve proofs database.");
  }
  // wait for the other block verifiers to sync the databse
  sleep(10);

  // check if your reserve bytes database is synced
  if (sync_check_reserve_bytes_database() == 0)
  {    
    CHECK_IF_DATABASES_ARE_SYNCED_ERROR("Could not check if the reserve bytes database is updated. This means you might need to sync the reserve bytes database.");
  }
  // wait for the other block verifiers to sync the databse
  sleep(10);

  // check if your delegates database is synced
  if (sync_check_delegates_database() == 0)
  {
    CHECK_IF_DATABASES_ARE_SYNCED_ERROR("Could not check if the delegates database is updated. This means you might need to sync the delegates database.");
  }
  // wait for the other block verifiers to sync the databse
  sleep(10);

  // check if your statistics database is synced
  if (sync_check_statistics_database() == 0)
  {
    CHECK_IF_DATABASES_ARE_SYNCED_ERROR("Could not check if the statistics database is updated. This means you might need to sync the statistics database.");
  }
  // wait for the other block verifiers to sync the databse
  sleep(10);

  pointer_reset(data);
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
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;
  int count2;
  int count3;
  int counter;
  int main_nodes_count;
  int number[64];
  int settings = 1;

  // define macros
  #define CALCULATE_MAIN_NODES_ROLES(settings) \
  memcpy(error_message.function[error_message.total],"calculate_main_nodes_roles",26); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; \
  free(data3); \
  data3 = NULL;

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
    memcpy(error_message.function[error_message.total],"calculate_main_nodes_roles",26);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }
  
  sscanf(current_block_height,"%zu", &count);
  if (count < XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT)
  {
    CALCULATE_MAIN_NODES_ROLES("Could not get the current block height");
  }
  count--;
  sprintf(data2,"%zu",count);

  // calculate the database to get the reserve byte data
  count2 = ((count - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME) + 1;
  memcpy(data,"reserve_bytes_",14);
  sprintf(data+14,"%d",count2);

  // create the message
  memcpy(data3,"{\"block_height\":\"",17);
  memcpy(data3+17,data2,strnlen(data2,BUFFER_SIZE));
  memcpy(data3+strlen(data3),"\"}",2);

  // get the reserve byte data
  memset(data2,0,strlen(data2));
  if (read_document_field_from_collection(DATABASE_NAME,data,data3,"reserve_bytes",data2,0) == 0)
  {
    CALCULATE_MAIN_NODES_ROLES("Could not get the previous blocks reserve bytes");
  }

  // get the vrf_beta_string_data_round_part_3
  memset(data3,0,strlen(data3));
  if (parse_reserve_bytes_data(data3,(const char*)data2,8) == 0)
  {
    CALCULATE_MAIN_NODES_ROLES("Could not get the previous blocks reserve bytes");
  }

  for (count = 0, count3 = 0, main_nodes_count = 0; count < VRF_BETA_LENGTH || main_nodes_count == 6; count += 2)
  {
    memset(data,0,strlen(data));
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
        memset(main_nodes_list.block_producer_public_address,0,strlen(main_nodes_list.block_producer_public_address));
        memset(main_nodes_list.block_producer_IP_address,0,strlen(main_nodes_list.block_producer_IP_address));
        memcpy(main_nodes_list.block_producer_public_address,current_block_verifiers_list.block_verifiers_public_address[count2],strnlen(current_block_verifiers_list.block_verifiers_public_address[count2],XCASH_WALLET_LENGTH));
        memcpy(main_nodes_list.block_producer_IP_address,current_block_verifiers_list.block_verifiers_IP_address[count2],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count2],BUFFER_SIZE));
      }
      if (main_nodes_count == 1)
      {
        // calculate the block_producer_backup_block_verifier_1
        memset(main_nodes_list.block_producer_backup_block_verifier_1_public_address,0,strnlen(main_nodes_list.block_producer_backup_block_verifier_1_public_address,XCASH_WALLET_LENGTH));
        memset(main_nodes_list.block_producer_backup_block_verifier_1_IP_address,0,strnlen(main_nodes_list.block_producer_backup_block_verifier_1_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_1_public_address,current_block_verifiers_list.block_verifiers_public_address[count2],strnlen(current_block_verifiers_list.block_verifiers_public_address[count2],XCASH_WALLET_LENGTH));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_1_IP_address,current_block_verifiers_list.block_verifiers_IP_address[count2],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count2],BUFFER_SIZE));
      }
      if (main_nodes_count == 2)
      {
        // calculate the block_producer_backup_block_verifier_2
        memset(main_nodes_list.block_producer_backup_block_verifier_2_public_address,0,strnlen(main_nodes_list.block_producer_backup_block_verifier_2_public_address,XCASH_WALLET_LENGTH));
        memset(main_nodes_list.block_producer_backup_block_verifier_2_IP_address,0,strnlen(main_nodes_list.block_producer_backup_block_verifier_2_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_2_public_address,current_block_verifiers_list.block_verifiers_public_address[count2],strnlen(current_block_verifiers_list.block_verifiers_public_address[count2],XCASH_WALLET_LENGTH));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_2_IP_address,current_block_verifiers_list.block_verifiers_IP_address[count2],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count2],BUFFER_SIZE));
      }
      if (main_nodes_count == 3)
      {
        // calculate the block_producer_backup_block_verifier_3
        memset(main_nodes_list.block_producer_backup_block_verifier_3_public_address,0,strnlen(main_nodes_list.block_producer_backup_block_verifier_3_public_address,XCASH_WALLET_LENGTH));
        memset(main_nodes_list.block_producer_backup_block_verifier_3_IP_address,0,strnlen(main_nodes_list.block_producer_backup_block_verifier_3_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_3_public_address,current_block_verifiers_list.block_verifiers_public_address[count2],strnlen(current_block_verifiers_list.block_verifiers_public_address[count2],XCASH_WALLET_LENGTH));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_3_IP_address,current_block_verifiers_list.block_verifiers_IP_address[count2],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count2],BUFFER_SIZE));
      }
      if (main_nodes_count == 4)
      {
        // calculate the block_producer_backup_block_verifier_4
        memset(main_nodes_list.block_producer_backup_block_verifier_4_public_address,0,strnlen(main_nodes_list.block_producer_backup_block_verifier_4_public_address,XCASH_WALLET_LENGTH));
        memset(main_nodes_list.block_producer_backup_block_verifier_4_IP_address,0,strnlen(main_nodes_list.block_producer_backup_block_verifier_4_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_4_public_address,current_block_verifiers_list.block_verifiers_public_address[count2],strnlen(current_block_verifiers_list.block_verifiers_public_address[count2],XCASH_WALLET_LENGTH));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_4_IP_address,current_block_verifiers_list.block_verifiers_IP_address[count2],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count2],BUFFER_SIZE));
      }
      if (main_nodes_count == 5)
      {
        // calculate the block_producer_backup_block_verifier_5
        memset(main_nodes_list.block_producer_backup_block_verifier_5_public_address,0,strnlen(main_nodes_list.block_producer_backup_block_verifier_5_public_address,XCASH_WALLET_LENGTH));
        memset(main_nodes_list.block_producer_backup_block_verifier_5_IP_address,0,strnlen(main_nodes_list.block_producer_backup_block_verifier_5_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_5_public_address,current_block_verifiers_list.block_verifiers_public_address[count2],strnlen(current_block_verifiers_list.block_verifiers_public_address[count2],XCASH_WALLET_LENGTH));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_5_IP_address,current_block_verifiers_list.block_verifiers_IP_address[count2],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count2],BUFFER_SIZE));
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
      color_print("The main nodes calculation process has run out of bytes to read.\nA random network data node will be the block producer","red");

      // select a random network data node
      if (strncmp(main_nodes_list.block_producer_public_address,"",1) == 0)
      {
        count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT);
        memset(main_nodes_list.block_producer_public_address,0,strnlen(main_nodes_list.block_producer_public_address,XCASH_WALLET_LENGTH));
        memset(main_nodes_list.block_producer_IP_address,0,strnlen(main_nodes_list.block_producer_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
        memcpy(main_nodes_list.block_producer_public_address,network_data_nodes_list.network_data_nodes_public_address[count],strnlen(network_data_nodes_list.network_data_nodes_public_address[count],XCASH_WALLET_LENGTH));
        memcpy(main_nodes_list.block_producer_IP_address,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],BUFFER_SIZE));
      } 
      if (strncmp(main_nodes_list.block_producer_backup_block_verifier_1_public_address,"",1) == 0)
      {
        count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT);
        memset(main_nodes_list.block_producer_backup_block_verifier_1_public_address,0,strnlen(main_nodes_list.block_producer_backup_block_verifier_1_public_address,XCASH_WALLET_LENGTH));
        memset(main_nodes_list.block_producer_backup_block_verifier_1_IP_address,0,strnlen(main_nodes_list.block_producer_backup_block_verifier_1_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_1_public_address,network_data_nodes_list.network_data_nodes_public_address[count],strnlen(network_data_nodes_list.network_data_nodes_public_address[count],XCASH_WALLET_LENGTH));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_1_IP_address,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],BUFFER_SIZE));
      } 
      if (strncmp(main_nodes_list.block_producer_backup_block_verifier_2_public_address,"",1) == 0)
      {
        count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT);
        memset(main_nodes_list.block_producer_backup_block_verifier_2_public_address,0,strnlen(main_nodes_list.block_producer_backup_block_verifier_2_public_address,XCASH_WALLET_LENGTH));
        memset(main_nodes_list.block_producer_backup_block_verifier_2_IP_address,0,strnlen(main_nodes_list.block_producer_backup_block_verifier_2_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_2_public_address,network_data_nodes_list.network_data_nodes_public_address[count],strnlen(network_data_nodes_list.network_data_nodes_public_address[count],XCASH_WALLET_LENGTH));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_2_IP_address,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],BUFFER_SIZE));
      } 
      if (strncmp(main_nodes_list.block_producer_backup_block_verifier_3_public_address,"",1) == 0)
      {
        count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT);
        memset(main_nodes_list.block_producer_backup_block_verifier_3_public_address,0,strnlen(main_nodes_list.block_producer_backup_block_verifier_3_public_address,XCASH_WALLET_LENGTH));
        memset(main_nodes_list.block_producer_backup_block_verifier_3_IP_address,0,strnlen(main_nodes_list.block_producer_backup_block_verifier_3_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_3_public_address,network_data_nodes_list.network_data_nodes_public_address[count],strnlen(network_data_nodes_list.network_data_nodes_public_address[count],XCASH_WALLET_LENGTH));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_3_IP_address,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],BUFFER_SIZE));
      } 
      if (strncmp(main_nodes_list.block_producer_backup_block_verifier_4_public_address,"",1) == 0)
      {
        count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT);
        memset(main_nodes_list.block_producer_backup_block_verifier_4_public_address,0,strnlen(main_nodes_list.block_producer_backup_block_verifier_4_public_address,XCASH_WALLET_LENGTH));
        memset(main_nodes_list.block_producer_backup_block_verifier_4_IP_address,0,strnlen(main_nodes_list.block_producer_backup_block_verifier_4_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_4_public_address,network_data_nodes_list.network_data_nodes_public_address[count],strnlen(network_data_nodes_list.network_data_nodes_public_address[count],XCASH_WALLET_LENGTH));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_4_IP_address,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],BUFFER_SIZE));
      }   
      if (strncmp(main_nodes_list.block_producer_backup_block_verifier_5_public_address,"",1) == 0)
      {
        count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT);
        memset(main_nodes_list.block_producer_backup_block_verifier_5_public_address,0,strnlen(main_nodes_list.block_producer_backup_block_verifier_5_public_address,XCASH_WALLET_LENGTH));
        memset(main_nodes_list.block_producer_backup_block_verifier_5_IP_address,0,strnlen(main_nodes_list.block_producer_backup_block_verifier_5_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_5_public_address,network_data_nodes_list.network_data_nodes_public_address[count],strnlen(network_data_nodes_list.network_data_nodes_public_address[count],XCASH_WALLET_LENGTH));
        memcpy(main_nodes_list.block_producer_backup_block_verifier_5_IP_address,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],BUFFER_SIZE));
      } 
    }
  }
  return 1;

  #undef CALCULATE_MAIN_NODES_ROLES
  #undef pointer_reset_all
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
    if (send_data(CLIENT_SOCKET,(char*)MESSAGE,1) == 1)
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
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;
  size_t count2;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_node_to_network_data_nodes_get_previous_current_next_block_verifiers_list",100); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset(data); \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_node_to_network_data_nodes_get_previous_current_next_block_verifiers_list",100);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  // create the message
  memcpy(data,"{\r\n \"message_settings\": \"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST\",\r\n \"previous_block_verifiers_name_list\": \"",142);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),previous_block_verifiers_list.block_verifiers_name[count2],strnlen(previous_block_verifiers_list.block_verifiers_name[count2],BUFFER_SIZE));
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n \"previous_block_verifiers_public_address_list\": \"",54);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),previous_block_verifiers_list.block_verifiers_public_address[count2],strnlen(previous_block_verifiers_list.block_verifiers_public_address[count2],BUFFER_SIZE));
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n \"previous_block_verifiers_IP_address_list\": \"",50);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),previous_block_verifiers_list.block_verifiers_IP_address[count2],strnlen(previous_block_verifiers_list.block_verifiers_IP_address[count2],BUFFER_SIZE));
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n \"current_block_verifiers_name_list\": \"",43);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),current_block_verifiers_list.block_verifiers_name[count2],strnlen(current_block_verifiers_list.block_verifiers_name[count2],BUFFER_SIZE));
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n \"current_block_verifiers_public_address_list\": \"",54);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),current_block_verifiers_list.block_verifiers_public_address[count2],strnlen(current_block_verifiers_list.block_verifiers_public_address[count2],BUFFER_SIZE));
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n \"current_block_verifiers_IP_address_list\": \"",50);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),current_block_verifiers_list.block_verifiers_IP_address[count2],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count2],BUFFER_SIZE));
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n \"next_block_verifiers_name_list\": \"",43);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),next_block_verifiers_list.block_verifiers_name[count2],strnlen(next_block_verifiers_list.block_verifiers_name[count2],BUFFER_SIZE));
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n \"next_block_verifiers_public_address_list\": \"",54);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),next_block_verifiers_list.block_verifiers_public_address[count2],strnlen(next_block_verifiers_list.block_verifiers_public_address[count2],BUFFER_SIZE));
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n \"next_block_verifiers_IP_address_list\": \"",50);
  for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
  {
    memcpy(data+strlen(data),next_block_verifiers_list.block_verifiers_IP_address[count2],strnlen(next_block_verifiers_list.block_verifiers_IP_address[count2],BUFFER_SIZE));
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n}",5);
  
  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,data,1) == 0)
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
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;
  size_t count2;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_node_to_network_data_nodes_get_current_block_verifiers_list",86); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset(data); \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_node_to_network_data_nodes_get_current_block_verifiers_list",86);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

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
    memcpy(data+strlen(data),current_block_verifiers_list.block_verifiers_IP_address[count2],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count2],BUFFER_SIZE));
    memcpy(data+strlen(data),"|",1);
  }
  memcpy(data+strlen(data),"\",\r\n}",5);
  
  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,data,1) == 0)
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
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;
  size_t count2;

  // define macros
  #define DATABASE_COLLECTION "reserve_bytes"
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_nodes_to_block_verifiers_reserve_bytes_database_sync_check_all_update",96); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
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
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_nodes_to_block_verifiers_reserve_bytes_database_sync_check_all_update",96);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  // get the database data hash for the reserve bytes database
  if (get_database_data_hash(data2,DATABASE_NAME,DATABASE_COLLECTION,0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not get the database data hash for the reserve bytes database");
  }

  // create the message
  memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_NODES_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"data_hash\": \"",115);
  memcpy(data+115,data2,DATA_HASH_LENGTH);
  memcpy(data+243,"\",\r\n}",5);
  
  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,data,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not send the BLOCK_VERIFIERS_TO_NODES_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD message to the node");
  }

  pointer_reset_all;
  return 1;

  #undef pointer_reset_all
  #undef DATABASE_COLLECTION
  #undef SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR
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
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* reserve_proofs_database = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;

  // define macros
  #define pointer_reset_all \
  free(message); \
  message = NULL; \
  free(reserve_proofs_database); \
  reserve_proofs_database = NULL; \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_all_update",107); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (message == NULL || reserve_proofs_database == NULL || data == NULL || data2 == NULL)
  {
    if (message != NULL)
    {
      pointer_reset(message);
    }
    if (reserve_proofs_database != NULL)
    {
      pointer_reset(reserve_proofs_database);
    }
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_all_update",107);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not verify the message");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"reserve_proofs_data_hash",data) == 0 || strlen(data) != DATA_HASH_LENGTH)
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
    memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"reserve_proofs_database\": \"true\",\r\n \"reserve_proofs_database_1\": \"true\",\r\n \"reserve_proofs_database_2\": \"true\",\r\n \"reserve_proofs_database_3\": \"true\",\r\n \"reserve_proofs_database_4\": \"true\",\r\n \"reserve_proofs_database_5\": \"true\",\r\n \"reserve_proofs_database_6\": \"true\",\r\n \"reserve_proofs_database_7\": \"true\",\r\n \"reserve_proofs_database_8\": \"true\",\r\n \"reserve_proofs_database_9\": \"true\",\r\n \"reserve_proofs_database_10\": \"true\",\r\n \"reserve_proofs_database_11\": \"true\",\r\n \"reserve_proofs_database_12\": \"true\",\r\n \"reserve_proofs_database_13\": \"true\",\r\n \"reserve_proofs_database_14\": \"true\",\r\n \"reserve_proofs_database_15\": \"true\",\r\n \"reserve_proofs_database_16\": \"true\",\r\n \"reserve_proofs_database_17\": \"true\",\r\n \"reserve_proofs_database_18\": \"true\",\r\n \"reserve_proofs_database_19\": \"true\",\r\n \"reserve_proofs_database_20\": \"true\",\r\n \"reserve_proofs_database_21\": \"true\",\r\n \"reserve_proofs_database_22\": \"true\",\r\n \"reserve_proofs_database_23\": \"true\",\r\n \"reserve_proofs_database_24\": \"true\",\r\n \"reserve_proofs_database_25\": \"true\",\r\n \"reserve_proofs_database_26\": \"true\",\r\n \"reserve_proofs_database_27\": \"true\",\r\n \"reserve_proofs_database_28\": \"true\",\r\n \"reserve_proofs_database_29\": \"true\",\r\n \"reserve_proofs_database_30\": \"true\",\r\n \"reserve_proofs_database_31\": \"true\",\r\n \"reserve_proofs_database_32\": \"true\",\r\n \"reserve_proofs_database_33\": \"true\",\r\n \"reserve_proofs_database_34\": \"true\",\r\n \"reserve_proofs_database_35\": \"true\",\r\n \"reserve_proofs_database_36\": \"true\",\r\n \"reserve_proofs_database_37\": \"true\",\r\n \"reserve_proofs_database_38\": \"true\",\r\n \"reserve_proofs_database_39\": \"true\",\r\n \"reserve_proofs_database_40\": \"true\",\r\n \"reserve_proofs_database_41\": \"true\",\r\n \"reserve_proofs_database_42\": \"true\",\r\n \"reserve_proofs_database_43\": \"true\",\r\n \"reserve_proofs_database_44\": \"true\",\r\n \"reserve_proofs_database_45\": \"true\",\r\n \"reserve_proofs_database_46\": \"true\",\r\n \"reserve_proofs_database_47\": \"true\",\r\n \"reserve_proofs_database_48\": \"true\",\r\n \"reserve_proofs_database_49\": \"true\",\r\n \"reserve_proofs_database_50\": \"true\",\r\n}",strnlen("{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"reserve_proofs_database\": \"true\",\r\n \"reserve_proofs_database_1\": \"true\",\r\n \"reserve_proofs_database_2\": \"true\",\r\n \"reserve_proofs_database_3\": \"true\",\r\n \"reserve_proofs_database_4\": \"true\",\r\n \"reserve_proofs_database_5\": \"true\",\r\n \"reserve_proofs_database_6\": \"true\",\r\n \"reserve_proofs_database_7\": \"true\",\r\n \"reserve_proofs_database_8\": \"true\",\r\n \"reserve_proofs_database_9\": \"true\",\r\n \"reserve_proofs_database_10\": \"true\",\r\n \"reserve_proofs_database_11\": \"true\",\r\n \"reserve_proofs_database_12\": \"true\",\r\n \"reserve_proofs_database_13\": \"true\",\r\n \"reserve_proofs_database_14\": \"true\",\r\n \"reserve_proofs_database_15\": \"true\",\r\n \"reserve_proofs_database_16\": \"true\",\r\n \"reserve_proofs_database_17\": \"true\",\r\n \"reserve_proofs_database_18\": \"true\",\r\n \"reserve_proofs_database_19\": \"true\",\r\n \"reserve_proofs_database_20\": \"true\",\r\n \"reserve_proofs_database_21\": \"true\",\r\n \"reserve_proofs_database_22\": \"true\",\r\n \"reserve_proofs_database_23\": \"true\",\r\n \"reserve_proofs_database_24\": \"true\",\r\n \"reserve_proofs_database_25\": \"true\",\r\n \"reserve_proofs_database_26\": \"true\",\r\n \"reserve_proofs_database_27\": \"true\",\r\n \"reserve_proofs_database_28\": \"true\",\r\n \"reserve_proofs_database_29\": \"true\",\r\n \"reserve_proofs_database_30\": \"true\",\r\n \"reserve_proofs_database_31\": \"true\",\r\n \"reserve_proofs_database_32\": \"true\",\r\n \"reserve_proofs_database_33\": \"true\",\r\n \"reserve_proofs_database_34\": \"true\",\r\n \"reserve_proofs_database_35\": \"true\",\r\n \"reserve_proofs_database_36\": \"true\",\r\n \"reserve_proofs_database_37\": \"true\",\r\n \"reserve_proofs_database_38\": \"true\",\r\n \"reserve_proofs_database_39\": \"true\",\r\n \"reserve_proofs_database_40\": \"true\",\r\n \"reserve_proofs_database_41\": \"true\",\r\n \"reserve_proofs_database_42\": \"true\",\r\n \"reserve_proofs_database_43\": \"true\",\r\n \"reserve_proofs_database_44\": \"true\",\r\n \"reserve_proofs_database_45\": \"true\",\r\n \"reserve_proofs_database_46\": \"true\",\r\n \"reserve_proofs_database_47\": \"true\",\r\n \"reserve_proofs_database_48\": \"true\",\r\n \"reserve_proofs_database_49\": \"true\",\r\n \"reserve_proofs_database_50\": \"true\",\r\n}",BUFFER_SIZE));
  }
  else
  {
    memset(message,0,strlen(message));
    memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"reserve_proofs_database\": \"false\",\r\n ",150);
    for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
    {
      memcpy(message+strlen(message),"\"reserve_proofs_database_",25);
      sprintf(message+strlen(message),"%zu",count);
      memcpy(message+strlen(message),"\": \"",4);      
      memset(data2,0,strlen(data2));  
      memcpy(data2,"reserve_proofs_data_hash_",25);  
      sprintf(data2+25,"%zu",count); 
      // parse the message
      if (parse_json_data(MESSAGE,data2,data) == 0 || strlen(data) != DATA_HASH_LENGTH)
      {
        SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not parse the message");
      }
      // get the database data hash for the reserve proofs database
      memset(data2,0,strlen(data2));  
      memcpy(data2,"reserve_proofs_",15);  
      sprintf(data2+15,"%zu",count);
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
  if (send_data(CLIENT_SOCKET,message,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not send the NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST message to the block verifier");
  }

  return 1;

  #undef pointer_reset_all
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
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // define macros
  #define DATABASE_MESSAGE_SYNCED_TRUE "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"reserve_proofs_database\": \"true\",\r\n}"
  #define DATABASE_MESSAGE_SYNCED_FALSE "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"reserve_proofs_database\": \"false\",\r\n}"
  
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_update",103); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
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
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_update",103);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not verify the message");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"file",data) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not parse the message");
  }

  // get the database data hash for the reserve proofs database
  memset(data2,0,strlen(data2));
  if (get_database_data_hash(data2,DATABASE_NAME,data,0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not get the database data hash for the reserve proofs database");
  }

  // parse the message
  memset(data,0,strlen(data));
  if (parse_json_data(MESSAGE,"data_hash",data) == 0 || strlen(data) != DATA_HASH_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not parse the message");
  }

  // create the message
  if (strncmp(data,data2,DATA_HASH_LENGTH) == 0)
  {
    memset(data,0,strlen(data));
    memcpy(data,DATABASE_MESSAGE_SYNCED_TRUE,strnlen(DATABASE_MESSAGE_SYNCED_TRUE,BUFFER_SIZE));
  }
  else
  {
    memset(data,0,strlen(data));
    memcpy(data,DATABASE_MESSAGE_SYNCED_FALSE,strnlen(DATABASE_MESSAGE_SYNCED_FALSE,BUFFER_SIZE));
  }
  
  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,data,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not send the NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST message to the block verifier");
  }

  return 1;

  #undef pointer_reset_all
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
  char* data = (char*)calloc(52428800,sizeof(char)); // 50 MB
  char* data2 = (char*)calloc(52428800,sizeof(char)); // 50 MB

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_download_file_update",106); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
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
  if (parse_json_data(MESSAGE,"file",data) == 0)
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
  memcpy(data+139,data2,strnlen(data2,52428800));
  memcpy(data+strlen(data),"\",\r\n}",5);

  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,data,1) == 0)
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
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_all_update",106); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
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
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_all_update",106);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not verify the message");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"data_hash",data) == 0 || strlen(data) != DATA_HASH_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not parse the message");
  }

  // get the database data hash for the reserve bytes database
  memset(data2,0,strlen(data2));
  if (get_database_data_hash(data2,DATABASE_NAME,"reserve_bytes",0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not get the database data hash for the reserve bytes database");
  }

  // create the message
  if (memcmp(data,data2,DATA_HASH_LENGTH) == 0)
  {
    memset(data,0,strlen(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"reserve_bytes_database\": \"true\",\r\n}",149);
  }
  else
  {
    memset(data,0,strlen(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"reserve_bytes_database\": \"false\",\r\n}",150);
  }
  
  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,data,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not send the NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST message to the block verifier");
  }

  return 1;

  #undef pointer_reset_all
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
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // define macros
  #define DATABASE_MESSAGE_SYNCED_TRUE "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"reserve_bytes_database\": \"true\",\r\n}"
  #define DATABASE_MESSAGE_SYNCED_FALSE "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"reserve_bytes_database\": \"false\",\r\n}"
  
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_update",102); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
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
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_update",102);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not verify the message");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"file",data) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not parse the message");
  }

  // get the database data hash for the reserve bytes database
  memset(data2,0,strlen(data2));
  if (get_database_data_hash(data2,DATABASE_NAME,data,0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not get the database data hash for the reserve bytes database");
  }

  // parse the message
  memset(data,0,strlen(data));
  if (parse_json_data(MESSAGE,"data_hash",data) == 0 || strlen(data) != DATA_HASH_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not parse the message");
  }

  // create the message
  if (memcmp(data,data2,DATA_HASH_LENGTH) == 0)
  {
    memset(data,0,strlen(data));
    memcpy(data,DATABASE_MESSAGE_SYNCED_TRUE,strnlen(DATABASE_MESSAGE_SYNCED_TRUE,BUFFER_SIZE));
  }
  else
  {
    memset(data,0,strlen(data));
    memcpy(data,DATABASE_MESSAGE_SYNCED_FALSE,strnlen(DATABASE_MESSAGE_SYNCED_FALSE,BUFFER_SIZE));
  }
  
  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,data,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not send the NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST message to the block verifier");
  }

  return 1;

  #undef DATABASE_MESSAGE_SYNCED_TRUE
  #undef DATABASE_MESSAGE_SYNCED_FALSE
  #undef pointer_reset_all
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
  char* data = (char*)calloc(52428800,sizeof(char)); // 50 MB
  char* data2 = (char*)calloc(52428800,sizeof(char)); // 50 MB

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_download_file_update",105); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
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
  if (parse_json_data(MESSAGE,"file",data) == 0)
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
  memcpy(data+137,data2,strnlen(data2,52428800));
  memcpy(data+strlen(data),"\",\r\n}",5);

  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,data,1) == 0)
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
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_sync_check_update",98); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
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
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_sync_check_update",105);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not verify the message");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"file",data) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not parse the message");
  }

  // get the database data hash for the reserve bytes database
  memset(data2,0,strlen(data2));
  if (get_database_data_hash(data2,DATABASE_NAME,data,0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not get the database data hash for the reserve bytes database");
  }

  // parse the message
  memset(data,0,strlen(data));
  if (parse_json_data(MESSAGE,"data_hash",data) == 0 || strlen(data) != DATA_HASH_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not parse the message");
  }

  // create the message
  if (memcmp(data,data2,DATA_HASH_LENGTH) == 0)
  {
    memset(data,0,strlen(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"delegates_database\": \"true\",\r\n}",149);
  }
  else
  {
    memset(data,0,strlen(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"delegates_database\": \"false\",\r\n}",150);
  }
  
  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,data,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not send the NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST message to the block verifier");
  }

  return 1;

  #undef pointer_reset_all
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
  char* data = (char*)calloc(52428800,sizeof(char)); // 50 MB
  char* data2 = (char*)calloc(52428800,sizeof(char)); // 50 MB

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_download_file_update",101); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
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
  memcpy(data+129,data2,strnlen(data2,52428800));
  memcpy(data+strlen(data),"\",\r\n}",5);

  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,data,1) == 0)
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
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_sync_check_update",99); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
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
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_sync_check_update",99);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not verify the message");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"file",data) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not parse the message");
  }

  // get the database data hash for the reserve bytes database
  memset(data2,0,strlen(data2));
  if (get_database_data_hash(data2,DATABASE_NAME,data,0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not get the database data hash for the reserve bytes database");
  }

  // parse the message
  memset(data,0,strlen(data));
  if (parse_json_data(MESSAGE,"data_hash",data) == 0 || strlen(data) != DATA_HASH_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not parse the message");
  }

  // create the message
  if (memcmp(data,data2,DATA_HASH_LENGTH) == 0)
  {
    memset(data,0,strlen(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"statistics_database\": \"true\",\r\n}",149);
  }
  else
  {
    memset(data,0,strlen(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"statistics_database\": \"false\",\r\n}",150);
  }
  
  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,data,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not send the NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST message to the block verifier");
  }

  return 1;

  #undef pointer_reset_all
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
  char* data = (char*)calloc(52428800,sizeof(char)); // 50 MB
  char* data2 = (char*)calloc(52428800,sizeof(char)); // 50 MB

  // define macros
  #define DATABASE_COLLECTION "statistics"
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_download_file_update",102); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
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
  memcpy(data+131,data2,strnlen(data2,52428800));
  memcpy(data+strlen(data),"\",\r\n}",5);

  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,data,1) == 0)
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
  char* delegates_public_address = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  char* public_address = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  char* reserve_proof = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;
  int settings = 0;

  // define macros
  #define pointer_reset_all \
  free(delegates_public_address); \
  delegates_public_address = NULL; \
  free(public_address); \
  public_address = NULL; \
  free(reserve_proof); \
  reserve_proof = NULL; \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; \
  free(data3); \
  data3 = NULL; \

  #define SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_node_to_block_verifiers_add_reserve_proof",68); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (delegates_public_address == NULL || public_address == NULL || reserve_proof == NULL || data == NULL || data2 == NULL)
  {
    if (delegates_public_address != NULL)
    {
      pointer_reset(delegates_public_address);
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
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_node_to_block_verifiers_add_reserve_proof",68);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    send_data(CLIENT_SOCKET,"Could not verify the message",1);
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("Could not verify the message");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"delegates_public_address",delegates_public_address) == 0 || strlen(delegates_public_address) != XCASH_WALLET_LENGTH || memcmp(delegates_public_address,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"public_address",public_address) == 0 || strlen(public_address) != XCASH_WALLET_LENGTH || memcmp(public_address,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"reserve_proof",reserve_proof) == 0)
  {
    send_data(CLIENT_SOCKET,"Could not parse the message",1);
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("Could not parse the message");
  }

  // create the message
  memcpy(data,"{\"reserve_proof\":\"",18);
  memcpy(data+18,reserve_proof,strnlen(reserve_proof,BUFFER_SIZE_RESERVE_PROOF));
  memcpy(data+strlen(data),"\"}",2);

  memcpy(data3,"{\"public_address_created_reserve_proof\":\"",41);
  memcpy(data3+41,reserve_proof,strnlen(reserve_proof,BUFFER_SIZE_RESERVE_PROOF));
  memcpy(data3+strlen(data3),"\"}",2);

  for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
  {
    memset(data2,0,strlen(data2));
    memcpy(data2,"reserve_proofs_",15);
    sprintf(data2+15,"%zu",count);

    // check if the reserve proof is in the database
    if (count_documents_in_collection(DATABASE_NAME,data2,data,0) != 0)
    {
      send_data(CLIENT_SOCKET,"The reserve proof is already in the database",1);
      SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("The reserve proof is already in the database");
    }
    // check if another proof from the public address is already in the database
    if (count_documents_in_collection(DATABASE_NAME,data2,data3,0) != 0)
    {      
      settings = 1;
    }
  }

  // check if the reserve proof is valid and the spent amount is 0
  memset(data2,0,strlen(data2));
  if (check_reserve_proofs(data2,public_address,reserve_proof,0) == 0)
  {
    send_data(CLIENT_SOCKET,"The reserve proof is invalid",1);
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("The reserve proof is invalid");
  }  

  // remove any reserve proofs that were created by the public address
  if (settings == 1)
  {
    for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
    {
      memset(data2,0,strlen(data2));
      memcpy(data2,"reserve_proofs_",15);
      sprintf(data2+15,"%zu",count);
      if (count_documents_in_collection(DATABASE_NAME,data2,data3,0) > 0)
      {
        if (delete_document_from_collection(DATABASE_NAME,data2,data3,0) == 0)
        {
          send_data(CLIENT_SOCKET,"The previous reserve proof could not be cancelled for this public address. Please wait a few minutes and try again.",1);
          SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("The previous reserve proof could not be cancelled for this public address");
        }
      }
    }
  }
  

  // create the message
  memset(data,0,strlen(data));
  memcpy(data,"{\"public_address_created_reserve_proof\":\"",41);
  memcpy(data+strlen(data),public_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\",\"public_address_voted_for\":\"",30);
  memcpy(data+strlen(data),delegates_public_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\",\"total\":\"",11);
  memcpy(data+strlen(data),data2,strnlen(data2,BUFFER_SIZE));
  memcpy(data+strlen(data),"\",\"reserve_proof\":\"",19);
  memcpy(data+strlen(data),data2,strnlen(data2,BUFFER_SIZE_RESERVE_PROOF));
  memcpy(data+strlen(data),"\"}",2);

  // add the reserve proof to the database
  for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
  {
    memset(data2,0,strlen(data2));
    memcpy(data2,"reserve_proofs_",15);
    sprintf(data+15,"%zu",count);
    if (count_documents_in_collection(DATABASE_NAME,data2,data,0) < 1000)
    {
      // check the reserve_proofs_settings
      if (reserve_proofs_settings == 1)
      {
        if (insert_document_into_collection_json(DATABASE_NAME,data,data2,0) == 1)
        {
          send_data(CLIENT_SOCKET,"The vote was successfully added to the database",1);
          break;
        }
        else
        {
          send_data(CLIENT_SOCKET,"The vote could not be added to the database",1);
          SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("The vote could not be added to the database");
        }
      }
      else
      {
        send_data(CLIENT_SOCKET,"The block verifiers are currently deleting invalid reserve proofs from the database.\n\nPlease wait a few seconds",1);
        SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("The block verifiers are currently deleting invalid reserve proofs from the database.\n\nPlease wait a few seconds");
      }      
    }
  }

  return 1;

  #undef pointer_reset_all
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
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_invalid_reserve_proofs",84); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (block_verifiers_public_address == NULL || public_address == NULL || reserve_proof == NULL || data == NULL || data2 == NULL || data3 == NULL)
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
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_invalid_reserve_proofs",84);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS_ERROR("Could not verify the message");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"public_address",block_verifiers_public_address) == 0 || strlen(block_verifiers_public_address) != XCASH_WALLET_LENGTH || memcmp(block_verifiers_public_address,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"public_address_that_created_the_reserve_proof",data) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"reserve_proof",data2) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS_ERROR("Could not parse the message");
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
      for (count3 = 0, settings = 1; (int)count3 < invalid_reserve_proofs.count; count3++)
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
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* delegate_name = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* delegate_public_address = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* delegates_IP_address = (char*)calloc(BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH,sizeof(char));

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(delegate_name); \
  delegate_name = NULL; \
  free(delegate_public_address); \
  delegate_public_address = NULL; \
  free(delegates_IP_address); \
  delegates_IP_address = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_nodes_to_block_verifiers_register_delegates",70); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || delegate_name == NULL || delegate_public_address == NULL || delegates_IP_address == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (delegate_name != NULL)
    {
      pointer_reset(delegate_name);
    }
    if (delegate_public_address != NULL)
    {
      pointer_reset(delegate_public_address);
    }
    if (delegates_IP_address != NULL)
    {
      pointer_reset(delegates_IP_address);
    }
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_nodes_to_block_verifiers_register_delegates",70);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE_ERROR("Could not verify the message");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"delegate_name",delegate_name) == 0 || parse_json_data(MESSAGE,"public_address",delegate_public_address) == 0 || strlen(delegate_public_address) != XCASH_WALLET_LENGTH || memcmp(delegate_public_address,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"delegates_IP_address",delegates_IP_address) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE_ERROR("Could not parse the message");
  }

  // create the message
  memcpy(data,"{\"public_address\":\"",19);
  memcpy(data+19,delegate_public_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\"}",2);

  // check if the delegate is already registered
  if (count_documents_in_collection(DATABASE_NAME,DATABASE_COLLECTION,data,0) != 0)
  {
    send_data(CLIENT_SOCKET,"false",1);
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE_ERROR("The delegate is already registered");
  }

  // create the message
  memset(data,0,strlen(data));
  memcpy(data,"{\"public_address\":\"",19);
  memcpy(data+strlen(data),delegate_public_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\",\"delegate_name\":\"",30);
  memcpy(data+strlen(data),delegate_name,strnlen(delegate_name,BUFFER_SIZE));
  memcpy(data+strlen(data),"\",\"IP_address\":\"",11);
  memcpy(data+strlen(data),delegates_IP_address,strnlen(delegates_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
  memcpy(data+strlen(data),"\"}",2);

  // add the delegate to the database
  if (insert_document_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,data,0) == 0)
  {
    send_data(CLIENT_SOCKET,"false",1);
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE_ERROR("The delegate could not be added to the database");
  }

  send_data(CLIENT_SOCKET,"true",1);
  return 1;

  #undef DATABASE_COLLECTION
  #undef pointer_reset_all
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
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* delegate_public_address = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(delegate_public_address); \
  delegate_public_address = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_nodes_to_block_verifiers_remove_delegates",68); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || delegate_public_address == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (delegate_public_address != NULL)
    {
      pointer_reset(delegate_public_address);
    }
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_nodes_to_block_verifiers_remove_delegates",68);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE_ERROR("Could not verify the message");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"public_address",delegate_public_address) == 0 || strlen(delegate_public_address) != XCASH_WALLET_LENGTH || memcmp(delegate_public_address,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE_ERROR("Could not parse the message");
  }

  // create the message
  memcpy(data,"{\"public_address\":\"",19);
  memcpy(data+19,delegate_public_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\"}",2);

  // check if the delegate is already removed
  if (count_documents_in_collection(DATABASE_NAME,DATABASE_COLLECTION,data,0) <= 0)
  {
    send_data(CLIENT_SOCKET,"false",1);
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE_ERROR("The delegate is already removed");
  }

  // remove the delegate from the database
  if (delete_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,data,0) == 0)
  {
    send_data(CLIENT_SOCKET,"false",1);
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE_ERROR("The delegate could not be removed from the database");
  }

  send_data(CLIENT_SOCKET,"true",1);
  return 1;

  #undef DATABASE_COLLECTION
  #undef pointer_reset_all
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
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* delegate_public_address = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* item = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* value = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; \
  free(delegate_public_address); \
  delegate_public_address = NULL; \
  free(item); \
  item = NULL; \
  free(value); \
  value = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_nodes_to_block_verifiers_update_delegates",68); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL || delegate_public_address == NULL || item == NULL || value == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    if (delegate_public_address != NULL)
    {
      pointer_reset(delegate_public_address);
    }
    if (item != NULL)
    {
      pointer_reset(item);
    }
    if (value != NULL)
    {
      pointer_reset(value);
    }
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_nodes_to_block_verifiers_update_delegates",68);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  // verify the message
  if (verify_data(MESSAGE,0,0) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE_ERROR("Could not verify the message");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"public_address",delegate_public_address) == 0 || strlen(delegate_public_address) != XCASH_WALLET_LENGTH || memcmp(delegate_public_address,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"item",item) == 0 || parse_json_data(MESSAGE,"value",value) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE_ERROR("Could not parse the message");
  }

  // error check
  if (memcmp(item,"IP_address",10) != 0 && memcmp(item,"about",5) != 0 && memcmp(item,"website",7) != 0 && memcmp(item,"team",4) != 0 && memcmp(item,"pool_mode",9) != 0 && memcmp(item,"fee_structure",13) != 0 && memcmp(item,"server_settings",15) != 0)
  {
    send_data(CLIENT_SOCKET,"false",1);
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE_ERROR("Invalid item to update");
  }

  // create the message
  memcpy(data,"{\"public_address\":\"",19);
  memcpy(data+19,delegate_public_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\"}",2);

  memcpy(data2,"{\"",2);
  memcpy(data2+2,item,strnlen(item,BUFFER_SIZE));
  memcpy(data+strlen(data),"\":\"",3);
  memcpy(data2+strlen(data),value,strnlen(value,BUFFER_SIZE));
  memcpy(data2+strlen(data2),"\"}",2);

  // update the delegate in the database
  if (update_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,data,data2,0) == 0)
  {
    send_data(CLIENT_SOCKET,"false",1);
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE_ERROR("The delegate could not be updated from the database");
  }

  send_data(CLIENT_SOCKET,"true",1);
  return 1;

  #undef DATABASE_COLLECTION
  #undef pointer_reset_all
  #undef SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE_ERROR
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
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));

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
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_main_network_data_node_to_block_verifier_create_new_block",84);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; 

  #define SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_main_network_data_node_to_block_verifier_create_new_block",84); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // verify the data
  if (verify_data(MESSAGE,0,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR("Could not verify data");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"block_blob",data) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR("Could not parse the data");
  }

  // sign the network block string
  if (sign_network_block_string(data2,data,0) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR("Could not sign the network block string");
  }

  // create the message
  memset(data,0,strlen(data));
  memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK\",\r\n \"block_blob_signature\": \"",113);
  memcpy(data+113,data2,XCASH_SIGN_DATA_LENGTH);
  memcpy(data+206,"\",\r\n}",5);

  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR("Could not sign_data");
  }

  // send the network block signature to the main network data node
  if (send_data(CLIENT_SOCKET,data,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR("Could not send the data to the main network data node");
  }

  pointer_reset_all;
  return 1;

  #undef pointer_reset_all
  #undef SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR
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
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));

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
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_main_node_to_node_message_part_4",59);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; 

  #define SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_main_node_to_node_message_part_4",59); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  memset(VRF_data_copy->block_blob,0,strlen(VRF_data_copy->block_blob));

  // verify the data
  if (verify_data(MESSAGE,0,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR("Could not verify data");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"block_blob",data) == 0 || parse_json_data(MESSAGE,"public_address",data2) == 0 || strlen(data2) != XCASH_WALLET_LENGTH || memcmp(data2,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR("Could not parse the data");
  }

  // check if the public_address is the correct main node
  if ((main_network_data_node_create_block == 1 && memcmp(network_data_nodes_list.network_data_nodes_public_address[0],data2,XCASH_WALLET_LENGTH) == 0) || (main_network_data_node_create_block == 0 && memcmp(current_round_part_backup_node,"0",1) == 0 && memcmp(main_nodes_list.block_producer_public_address,data2,XCASH_WALLET_LENGTH) == 0) || (main_network_data_node_create_block == 0 && memcmp(current_round_part_backup_node,"1",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_1_public_address,data2,XCASH_WALLET_LENGTH) == 0) || (main_network_data_node_create_block == 0 && memcmp(current_round_part_backup_node,"2",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_2_public_address,data2,XCASH_WALLET_LENGTH) == 0) || (main_network_data_node_create_block == 0 && memcmp(current_round_part_backup_node,"3",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_3_public_address,data2,XCASH_WALLET_LENGTH) == 0) || (main_network_data_node_create_block == 0 && memcmp(current_round_part_backup_node,"4",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_4_public_address,data2,XCASH_WALLET_LENGTH) == 0) || (main_network_data_node_create_block == 0 && memcmp(current_round_part_backup_node,"5",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_5_public_address,data2,XCASH_WALLET_LENGTH) == 0))
  { 
    memcpy(VRF_data_copy->block_blob,data,strnlen(data,BUFFER_SIZE));
  }
  else
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR("Invalid main node");
  }

  pointer_reset_all;
  return 1;

  #undef pointer_reset_all
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
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;
  size_t count2;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL || data3 == NULL || message == NULL)
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
     if (message != NULL)
    {
      pointer_reset(message);
    }
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_main_node_to_node_message_part_4_create_new_block",76);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; \
  free(data3); \
  data3 = NULL; \
  free(message); \
  message = NULL;  

  #define SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_CREATE_NEW_BLOCK_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_main_node_to_node_message_part_4_create_new_block",76); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  memset(VRF_data.block_blob,0,strlen(VRF_data.block_blob));

  // verify the data
  if (verify_data(MESSAGE,0,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_CREATE_NEW_BLOCK_ERROR("Could not verify data");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"block_blob",data) == 0 || parse_json_data(MESSAGE,"public_address",data2) == 0 || strlen(data2) != XCASH_WALLET_LENGTH || memcmp(data2,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_CREATE_NEW_BLOCK_ERROR("Could not parse the data");
  }

  // check if the public_address is the correct main node
  if ((main_network_data_node_create_block == 1 && memcmp(network_data_nodes_list.network_data_nodes_public_address[0],data2,XCASH_WALLET_LENGTH) == 0 && verify_network_block_data(0,1,1,"0","") == 1) || (main_network_data_node_create_block == 0 && memcmp(current_round_part_backup_node,"0",1) == 0 && memcmp(main_nodes_list.block_producer_public_address,data2,XCASH_WALLET_LENGTH) == 0 && verify_network_block_data(0,1,1,"0","") == 1) || (main_network_data_node_create_block == 0 && memcmp(current_round_part_backup_node,"1",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_1_public_address,data2,XCASH_WALLET_LENGTH) == 0 && verify_network_block_data(0,1,1,"0","") == 1) || (main_network_data_node_create_block == 0 && memcmp(current_round_part_backup_node,"2",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_2_public_address,data2,XCASH_WALLET_LENGTH) == 0 && verify_network_block_data(0,1,1,"0","") == 1) || (main_network_data_node_create_block == 0 && memcmp(current_round_part_backup_node,"3",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_3_public_address,data2,XCASH_WALLET_LENGTH) == 0 && verify_network_block_data(0,1,1,"0","") == 1) || (main_network_data_node_create_block == 0 && memcmp(current_round_part_backup_node,"4",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_4_public_address,data2,XCASH_WALLET_LENGTH) == 0 && verify_network_block_data(0,1,1,"0","") == 1) || (main_network_data_node_create_block == 0 && memcmp(current_round_part_backup_node,"5",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_5_public_address,data2,XCASH_WALLET_LENGTH) == 0 && verify_network_block_data(0,1,1,"0","") == 1))
  {    
    // get the previous network block string
    sscanf(current_block_height,"%zu", &count);
    if (count < XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT)
    {
      SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_CREATE_NEW_BLOCK_ERROR("Could not get the current block height");
    }
    count--;
    sprintf(data3,"%zu",count);
    count2 = ((count - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME) + 1;
    memcpy(message,"{\"block_height\":\"",17);
    memcpy(message+17,data3,strnlen(data3,BUFFER_SIZE));
    memcpy(message+strlen(message),"\"}",2);
    memset(data3,0,strlen(data3));
    memcpy(data3,"reserve_bytes_",14);
    sprintf(data3+14,"%zu",count2);
    memset(data2,0,strlen(data2));
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
    memset(current_round_part_vote_data->current_vote_results,0,strlen(current_round_part_vote_data->current_vote_results));
    memset(data,0,strlen(data));
    crypto_hash_sha512((unsigned char*)data,(const unsigned char*)MESSAGE,(unsigned long long)strnlen(MESSAGE,BUFFER_SIZE));

    // convert the SHA512 data hash to a string
    for (count2 = 0, count = 0; count2 < DATA_HASH_LENGTH / 2; count2++, count += 2)
    {
      sprintf(current_round_part_vote_data->current_vote_results+count,"%02x",data[count2] & 0xFF);
    }
  }

  pointer_reset_all;
  return 1;

  #undef pointer_reset_all
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
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));

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
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_node_to_node",39);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NODE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_node_to_node",39); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // verify the data
  if (verify_data(MESSAGE,0,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NODE_ERROR("Could not verify data");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"vote_settings",data) == 0 || parse_json_data(MESSAGE,"vote_data",data2) == 0 || strlen(data2) != DATA_HASH_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NODE_ERROR("Could not parse the data");
  }

  // process the vote data
  if (memcmp(data,"valid",5) == 0 && memcmp(data2,current_round_part_vote_data->current_vote_results,DATA_HASH_LENGTH) == 0)
  {
    current_round_part_vote_data->vote_results_valid++;
  }
  else
  {
    current_round_part_vote_data->vote_results_invalid++;
  }

  pointer_reset_all;
  return 1;

  #undef pointer_reset_all
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
  char* public_address = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* vrf_secret_key_data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* vrf_public_key_data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* random_data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  unsigned char vrf_public_key[crypto_vrf_PUBLICKEYBYTES];
  unsigned char vrf_secret_key[crypto_vrf_SECRETKEYBYTES];
  size_t count;
  size_t counter;

  // define macros
  #define pointer_reset_all \
  free(public_address); \
  public_address = NULL; \
  free(vrf_secret_key_data); \
  vrf_secret_key_data = NULL; \
  free(vrf_public_key_data); \
  vrf_public_key_data = NULL; \
  free(random_data); \
  random_data = NULL; \
  free(data); \
  data = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_vrf_data",70); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (public_address == NULL || vrf_secret_key_data == NULL || vrf_public_key_data == NULL || random_data == NULL || data == NULL)
  {
    if (public_address != NULL)
    {
      pointer_reset(public_address);
    }
    if (vrf_secret_key_data != NULL)
    {
      pointer_reset(vrf_secret_key_data);
    }
    if (vrf_public_key_data != NULL)
    {
      pointer_reset(vrf_public_key_data);
    }
    if (random_data != NULL)
    {
      pointer_reset(random_data);
    }
    if (data != NULL)
    {
      pointer_reset(data);
    }
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_vrf_data",70);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  // verify the data
  if (verify_data(MESSAGE,0,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA_ERROR("Could not verify data");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"public_address",public_address) == 0 || strlen(public_address) != XCASH_WALLET_LENGTH || memcmp(public_address,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"vrf_secret_key",vrf_secret_key_data) == 0 || strlen(vrf_secret_key_data) != VRF_SECRET_KEY_LENGTH || parse_json_data(MESSAGE,"vrf_public_key",vrf_public_key_data) == 0 || strlen(vrf_public_key_data) != VRF_PUBLIC_KEY_LENGTH || parse_json_data(MESSAGE,"random_data",random_data) == 0 || strlen(random_data) != RANDOM_STRING_LENGTH)
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
    if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],public_address,XCASH_WALLET_LENGTH) == 0)
    {
      memcpy(VRF_data_copy->block_verifiers_vrf_secret_key_data[count],vrf_secret_key_data,VRF_SECRET_KEY_LENGTH);
      memcpy(VRF_data_copy->block_verifiers_vrf_secret_key[count],vrf_secret_key,crypto_vrf_SECRETKEYBYTES);
      memcpy(VRF_data_copy->block_verifiers_vrf_public_key_data[count],vrf_public_key_data,VRF_PUBLIC_KEY_LENGTH);
      memcpy(VRF_data_copy->block_verifiers_vrf_public_key[count],vrf_public_key,crypto_vrf_PUBLICKEYBYTES);
      memcpy(VRF_data_copy->block_verifiers_random_data[count],random_data,RANDOM_STRING_LENGTH);
    }
  }  

  pointer_reset_all;
  return 1;

  #undef pointer_reset_all
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
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;

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
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_block_blob_signature",82);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_block_blob_signature",82); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // verify the data
  if (verify_data(MESSAGE,0,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE_ERROR("Could not verify data");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"block_blob_signature",data) == 0 || strlen(data) != XCASH_SIGN_DATA_LENGTH || memcmp(data,XCASH_SIGN_DATA_PREFIX,sizeof(XCASH_SIGN_DATA_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"public_address",data2) == 0 || strlen(data2) != XCASH_WALLET_LENGTH || memcmp(data2,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE_ERROR("Could not parse the data");
  }

  // process the vote data
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],data2,XCASH_WALLET_LENGTH) == 0)
    {
      memset(VRF_data_copy->block_blob_signature[count],0,strlen(VRF_data_copy->block_blob_signature[count]));
      memcpy(VRF_data_copy->block_blob_signature[count],data,XCASH_SIGN_DATA_LENGTH);
    }
  }  

  pointer_reset_all;
  return 1;

  #undef pointer_reset_all
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

  // Variables
  char buffer[BUFFER_SIZE];
  char buffer2[BUFFER_SIZE];
  char client_address[BUFFER_SIZE]; 
  int len;
  int receive_data_result; 
  struct sockaddr_in addr, cl_addr; 

  // define macros
  #define SOCKET_FILE_DESCRIPTORS_LENGTH 1

  #define SERVER_ERROR(message) \
  memcpy(error_message.function[error_message.total],"create_server",13); \
  memcpy(error_message.data[error_message.total],message,strnlen(message,BUFFER_SIZE)); \
  error_message.total++; \
  return 0;  

  // set the main process to ignore if forked processes return a value or not, since the timeout for the total connection time is run on a different thread
  signal(SIGCHLD, SIG_IGN);

  if (MESSAGE_SETTINGS == 1)
  {
    print_start_message("Creating the server");
  }
  
  /* Create the socket  
  AF_INET = IPV4 support
  SOCK_STREAM = TCP protocol
  */
  const int SOCKET = socket(AF_INET, SOCK_STREAM, 0);
  if (SOCKET == -1)
  {
    SERVER_ERROR("Error creating socket");
  }

  /* Set the socket options
  SOL_SOCKET = socket level
  SO_REUSEADDR = allows for reuse of the same address, so one can shutdown and restart the program without errros
  SO_REUSEPORT = allows for reuse of the same port, so one can shutdown and restart the program without errros
  */
  if (setsockopt(SOCKET, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &SOCKET_OPTION,sizeof(int)) != 0)
  {
    SERVER_ERROR("Error setting socket options");
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
    memcpy(error_message.function[error_message.total],"create_server",13);
    memcpy(error_message.data[error_message.total],"Error connecting to port ",25);
    memcpy(error_message.data[error_message.total]+25,buffer2,strnlen(buffer2,BUFFER_SIZE));
    error_message.total++;
  } 

  // set the maximum simultaneous connections
  if (listen(SOCKET, MAXIMUM_CONNECTIONS) != 0)
  {
    SERVER_ERROR("Error creating the server");
  }

  if (MESSAGE_SETTINGS == 1)
  {
    printf("\033[1;32mConnected to port %s\033[0m\n",buffer2);
    printf("Waiting for a connection...\n\n");
  }

  for (;;)
  {
    len = sizeof(cl_addr);
    const int CLIENT_SOCKET = accept(SOCKET, (struct sockaddr *) &cl_addr, (socklen_t*)&len);
    inet_ntop(AF_INET, &(cl_addr.sin_addr), client_address, BUFFER_SIZE);
    if (client_address == NULL)
    {      
      continue;
    }
    const size_t CLIENT_ADDRESS_LENGTH = strnlen(client_address,BUFFER_SIZE);
    const size_t BUFFER2_LENGTH = strnlen(buffer2,BUFFER_SIZE);
  
    if (CLIENT_SOCKET == -1)
    {      
      continue;
    }
    
    // create a new process for each server connection
    if (fork() == 0)
    { 
       // Variables
       char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));

       // close the main socket, since the socket is now copied to the forked process
       close(SOCKET);

       for (;;)
       {         
         // receive the data
         memset(buffer, 0, BUFFER_SIZE); 
         receive_data_result = receive_data(CLIENT_SOCKET,buffer,SOCKET_END_STRING,1,TOTAL_CONNECTION_TIME_SETTINGS);
         if (receive_data_result < 2)
         {
           // close the forked process, since the client had an error sending data  
           close(SOCKET);
           close(CLIENT_SOCKET);
           pointer_reset(data);
           _exit(0);
         }  

         if (MESSAGE_SETTINGS == 1)
         { 
           // get the current time
           get_current_UTC_time;

           memcpy(data,"Received ",9);
           memcpy(data+9,&buffer[25],strlen(buffer) - strlen(strstr(buffer,"\",\r\n")) - 25);
           memcpy(data+strlen(data)," from ",6);
           memcpy(data+strlen(data),client_address,CLIENT_ADDRESS_LENGTH);
           memcpy(data+strlen(data)," on port ",9);
           memcpy(data+strlen(data),buffer2,BUFFER2_LENGTH);
           memcpy(data+strlen(data),"\n",1);
           memcpy(data+strlen(data),asctime(current_UTC_date_and_time),strlen(asctime(current_UTC_date_and_time)));
           color_print(data,"green");
         }

         pthread_rwlock_rdlock(&rwlock);

         // check if a certain type of message has been received 
         if (strstr(buffer,"\"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\"") != NULL && strncmp(server_message,"XCASH_PROOF_OF_STAKE_TEST_DATA",BUFFER_SIZE) == 0)
         {
           server_received_data_xcash_proof_of_stake_test_data(CLIENT_SOCKET,(const char*)buffer);
         }
         else if (strstr(buffer,"\"message_settings\": \"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST\"") != NULL && network_data_node_settings == 1)
         {
           server_receive_data_socket_node_to_network_data_nodes_get_previous_current_next_block_verifiers_list(CLIENT_SOCKET);
         } 
         else if (strstr(buffer,"\"message_settings\": \"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST\"") != NULL && network_data_node_settings == 1)
         {
           server_receive_data_socket_node_to_network_data_nodes_get_current_block_verifiers_list(CLIENT_SOCKET);
         } 
         else if (strstr(buffer,"\"message_settings\": \"NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE\"") != NULL)
         {
           server_receive_data_socket_nodes_to_block_verifiers_reserve_bytes_database_sync_check_all_update(CLIENT_SOCKET);
         }
         else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE\"") != NULL)
         {
           server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_all_update(CLIENT_SOCKET,(const char*)buffer);
         } 
         else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE\"") != NULL)
         {
           server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_update(CLIENT_SOCKET,(const char*)buffer);
         }  
         else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE\"") != NULL)
         {
           server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_download_file_update(CLIENT_SOCKET,(const char*)buffer);
         }  
         else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE\"") != NULL)
         {
           server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_all_update(CLIENT_SOCKET,(const char*)buffer);
         }
         else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE\"") != NULL)
         {
           server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_update(CLIENT_SOCKET,(const char*)buffer);
         }
         else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE\"") != NULL)
         {
           server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_download_file_update(CLIENT_SOCKET,(const char*)buffer);
         }
         else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE\"") != NULL)
         {
           server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_sync_check_update(CLIENT_SOCKET,(const char*)buffer);
         }
         else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE\"") != NULL)
         {
           server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_download_file_update(CLIENT_SOCKET,(const char*)buffer);
         }
         else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE\"") != NULL)
         {
           server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_sync_check_update(CLIENT_SOCKET,(const char*)buffer);
         }
         else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE\"") != NULL)
         {
           server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_download_file_update(CLIENT_SOCKET,(const char*)buffer);
         }
         else if (strstr(buffer,"\"message_settings\": \"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF\"") != NULL)
         {
           server_receive_data_socket_node_to_block_verifiers_add_reserve_proof(CLIENT_SOCKET,(const char*)buffer);
         } 
         else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS\"") != NULL && current_UTC_date_and_time->tm_min % 4 == 0 && current_UTC_date_and_time->tm_sec < 5)
         {
           server_receive_data_socket_block_verifiers_to_block_verifiers_invalid_reserve_proofs((const char*)buffer);
         }  
         else if (strstr(buffer,"\"message_settings\": \"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE\"") != NULL)
         {
           server_receive_data_socket_nodes_to_block_verifiers_register_delegates(CLIENT_SOCKET,(const char*)buffer);
         }            
         else if (strstr(buffer,"\"message_settings\": \"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE\"") != NULL)
         {
           server_receive_data_socket_nodes_to_block_verifiers_remove_delegates(CLIENT_SOCKET,(const char*)buffer);
         } 
         else if (strstr(buffer,"\"message_settings\": \"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE\"") != NULL)
         {
           server_receive_data_socket_nodes_to_block_verifiers_update_delegates(CLIENT_SOCKET,(const char*)buffer);
         } 
         else if (strstr(buffer,"\"message_settings\": \"MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_CREATE_NEW_BLOCK\"") != NULL && memcmp(server_message,"MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_CREATE_NEW_BLOCK",58) == 0)
         {
           server_receive_data_socket_main_network_data_node_to_block_verifier_create_new_block(CLIENT_SOCKET,(const char*)buffer);
         } 
         else if (strstr(buffer,"\"message_settings\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND_CREATE_NEW_BLOCK\"") != NULL && memcmp(server_message,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND_CREATE_NEW_BLOCK",52) == 0)
         {
           server_receive_data_socket_main_node_to_node_message_part_4((const char*)buffer);
         }         
         else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA\"") != NULL && memcmp(server_message,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA",43) == 0)
         {
           server_receive_data_socket_block_verifiers_to_block_verifiers_vrf_data((const char*)buffer);
         }  
         else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE\"") != NULL && memcmp(server_message,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE",55) == 0)
         {
           server_receive_data_socket_block_verifiers_to_block_verifiers_block_blob_signature((const char*)buffer);
         }  
         else if (strstr(buffer,"\"message_settings\": \"NODES_TO_NODES_VOTE_RESULTS\"") != NULL && memcmp(server_message,"NODES_TO_NODES_VOTE_RESULTS",27) == 0)
         {
           server_receive_data_socket_node_to_node((const char*)buffer);
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
             printf("\033[1;31mError sending data to %s on port %s\033[0m\n",client_address,buffer2); 
           } 
         }

         pthread_rwlock_unlock(&rwlock);
         
         close(SOCKET);
         close(CLIENT_SOCKET);
         pointer_reset(data); 
         _exit(0);
       }
     }  
     // close the client socket
     close(CLIENT_SOCKET);
   }
   return 1;

   #undef SOCKET_FILE_DESCRIPTORS_LENGTH
   #undef SERVER_ERROR
}

