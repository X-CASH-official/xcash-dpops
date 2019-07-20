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

int start_new_round()
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;
  int settings;

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
  memset(data,0,strlen(data));
  memcpy(data,"A new round is starting for block ",34);
  memcpy(data,current_block_height,strnlen(current_block_height,BUFFER_SIZE));
  color_print(data,"green");

  // update all of the databases 
  color_print("Updating the previous rounds data in the databases","green");
  if (update_databases() == 0)
  {    
    START_NEW_ROUND_ERROR("Could not check if the databases are synced. This means that your database is out of sync, and you need to resync your databases");
  }

  // check to make sure all of the databases are synced
  color_print("Checking if databases are synced","green");
  if (check_if_databases_are_synced() == 0)
  {    
    START_NEW_ROUND_ERROR("Could not check if the databases are synced. This means that your database is out of sync, and you need to resync your databases");
  }  
  
  // reset the variables
  memset(current_round_part,0,strlen(current_round_part));
  memcpy(current_round_part,"1",1);
  memset(current_round_part_backup_node,0,strlen(current_round_part_backup_node));
  memcpy(current_round_part_backup_node,"0",1);

  memset(current_round_part_backup_node_data.current_round_part_4_backup_node,0,strlen(current_round_part_backup_node_data.current_round_part_4_backup_node));
  
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memset(VRF_data.block_verifiers_vrf_secret_key_data[count],0,strlen(VRF_data.block_verifiers_vrf_secret_key_data[count])); 
    memset(VRF_data.block_verifiers_vrf_secret_key[count],0,strlen(VRF_data.block_verifiers_vrf_secret_key[count])); 
    memset(VRF_data.block_verifiers_vrf_public_key_data[count],0,strlen(VRF_data.block_verifiers_vrf_public_key_data[count])); 
    memset(VRF_data.block_verifiers_vrf_public_key[count],0,strlen(VRF_data.block_verifiers_vrf_public_key[count])); 
    memset(VRF_data.block_verifiers_random_data[count],0,strlen(VRF_data.block_verifiers_random_data[count])); 
    memset(VRF_data.block_blob_signature[count],0,strlen(VRF_data.block_blob_signature[count])); 
    
    memset(invalid_reserve_proofs.block_verifier_public_address[count],0,strlen(invalid_reserve_proofs.block_verifier_public_address[count]));
    memset(invalid_reserve_proofs.public_address[count],0,strlen(invalid_reserve_proofs.public_address[count]));
    memset(invalid_reserve_proofs.reserve_proof[count],0,strlen(invalid_reserve_proofs.reserve_proof[count]));
  }

  // reset the VRF_data struct
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

  // check if the current block height - 1 is a X-CASH proof of stake block since this will check to see if these are the first three blocks on the network
  sscanf(current_block_height,"%zu", &count);
  count = count - 1;
  memset(data,0,strnlen(data,BUFFER_SIZE));
  sprintf(data,"%zu",count);
  settings = get_block_settings(data,0);
  if (settings == 0)
  {    
    START_NEW_ROUND_ERROR("Could not get a previous blocks settings. Your block verifier will now sit out for the remainder of the round");
  }
  else if (settings == 1)
  {
    // this is a proof of work block, so this is the start blocks of the network
    color_print("The current block is on of the first three blocks on the network, meaning the data network node will create this block","green");

    // set the main_network_data_node_create_block so the main network data node can create the block
    main_network_data_node_create_block = 1;
    if (start_current_round_start_blocks() == 0)
    {      
      START_NEW_ROUND_ERROR("start_current_round_start_blocks error");
    } 
    if (start_part_4_of_round(1) == 0)
    {      
      START_NEW_ROUND_ERROR("start_part_4_of_round error");
    }  
  }
  else if (settings == 2)
  {
    // this is a X-CASH proof of stake block so this is not the start blocks of the network
    if (calculate_main_nodes_roles() == 0)
    {
      print_error_message;
      // set the main_network_data_node_create_block so the main network data node can create the block
      main_network_data_node_create_block = 1;
      if (start_current_round_start_blocks() == 0)
      {
        START_NEW_ROUND_ERROR("start_current_round_start_blocks error");
      } 
      if (start_part_4_of_round(1) == 0)
      {
        START_NEW_ROUND_ERROR("start_part_4_of_round error");
      }   
    }
    if (start_part_4_of_round(0) == 0)
    {
      print_error_message;
      // set the main_network_data_node_create_block so the main network data node can create the block
      main_network_data_node_create_block = 1;
      if (start_current_round_start_blocks() == 0)
      {
        START_NEW_ROUND_ERROR("start_current_round_start_blocks error");
      } 
      if (start_part_4_of_round(1) == 0)
      {
        START_NEW_ROUND_ERROR("start_part_4_of_round error");
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
Description: Runs the round where the network data node will create the block since its the first three X-CASH proof of stake blocks on the network
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

int start_current_round_start_blocks()
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count; 
  size_t count2; 
  unsigned char vrf_secret_key[crypto_vrf_SECRETKEYBYTES];

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  #define START_CURRENT_ROUND_START_BLOCKS_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"start_current_round_start_blocks",32); \
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
    memcpy(error_message.function[error_message.total],"start_current_round_start_blocks",32);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  for (;;)
  {
    usleep(200000);    
    if (current_UTC_date_and_time->tm_min == 4 && current_UTC_date_and_time->tm_sec == 40)
    {
      break;
    }
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
  memset(blockchain_data.nonce_data,0,strnlen(blockchain_data.nonce_data,9));
  memcpy(blockchain_data.nonce_data,CONSENSUS_NODE_NETWORK_BLOCK_NONCE,8);

  // create the VRF data
  memset(vrf_secret_key,0,crypto_vrf_SECRETKEYBYTES);
  if (create_random_VRF_keys(VRF_data.vrf_public_key_round_part_4,vrf_secret_key) == 1 && crypto_vrf_is_valid_key((const unsigned char*)VRF_data.vrf_public_key_round_part_4) != 1)
  {
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not create the vrf_public_key_round_part_4 or vrf_secret_key_round_part_4");
  }
  if (get_previous_block_hash(data,0) == 0)
  {
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not get the previous block hash");
  }  
  memcpy(VRF_data.vrf_alpha_string_round_part_4,data,64);
  memcpy(VRF_data.vrf_alpha_string_round_part_4+64,"0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",10000); 
  if (crypto_vrf_prove(VRF_data.vrf_proof_round_part_4,(const unsigned char*)vrf_secret_key,VRF_data.vrf_alpha_string_round_part_4,strlen((const char*)VRF_data.vrf_alpha_string_round_part_4)) != 0)
  {
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not create the vrf_proof_round_part_4");
  }
  if (crypto_vrf_proof_to_hash(VRF_data.vrf_beta_string_round_part_4,(const unsigned char*)VRF_data.vrf_proof_round_part_4) != 0)
  {
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not create the beta_string_round_part_4");
  }
  if (crypto_vrf_verify(VRF_data.vrf_beta_string_round_part_4,(const unsigned char*)VRF_data.vrf_public_key_round_part_4,(const unsigned char*)VRF_data.vrf_proof_round_part_4,VRF_data.vrf_alpha_string_round_part_4,crypto_vrf_PUBLICKEYBYTES) != 0)
  {
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not create the VRF data for round part 2");
  }

  // convert all of the VRF data to a string
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
  blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length_round_part_4 = VRF_PUBLIC_KEY_LENGTH;
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_4,VRF_data.vrf_public_key_data_round_part_4,VRF_PUBLIC_KEY_LENGTH);
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_4,VRF_data.vrf_public_key_round_part_4,crypto_vrf_PUBLICKEYBYTES);
  blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length_round_part_4 = strnlen(VRF_data.vrf_alpha_string_data_round_part_4,BUFFER_SIZE_NETWORK_BLOCK_DATA);
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_4,VRF_data.vrf_alpha_string_data_round_part_4,blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length_round_part_4);
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_4,VRF_data.vrf_alpha_string_round_part_4,strnlen((const char*)VRF_data.vrf_alpha_string_round_part_4,BUFFER_SIZE_NETWORK_BLOCK_DATA));
  blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length_round_part_4 = VRF_PROOF_LENGTH;
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_4,VRF_data.vrf_proof_data_round_part_4,VRF_PROOF_LENGTH);
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_4,VRF_data.vrf_proof_round_part_4,crypto_vrf_PROOFBYTES);
  blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length_round_part_4 = VRF_BETA_LENGTH;
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_4,VRF_data.vrf_beta_string_data_round_part_4,VRF_BETA_LENGTH);
  memcpy(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_4,VRF_data.vrf_beta_string_round_part_4,crypto_vrf_OUTPUTBYTES); 

  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key_data[count],"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",VRF_SECRET_KEY_LENGTH);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key[count],"0000000000000000000000000000000000000000000000000000000000000000",crypto_vrf_SECRETKEYBYTES);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key_data[count],"0000000000000000000000000000000000000000000000000000000000000000",VRF_PUBLIC_KEY_LENGTH);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key[count],"00000000000000000000000000000000",crypto_vrf_PUBLICKEYBYTES);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data[count],"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",200);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data_text[count],"0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",RANDOM_STRING_LENGTH);
  }

  // add the block_validation_node_signature_data_length to the blockchain_data
  blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data_length = BLOCK_VALIDATION_NODE_SIGNED_BLOCK_LENGTH;

  // add 0's for the block validation nodes signature
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count],GET_BLOCK_TEMPLATE_RESERVED_BYTES,BLOCK_VALIDATION_NODE_SIGNED_BLOCK_LENGTH);
  }

  // convert the blockchain_data to a network_block_string
  memset(data,0,strlen(data));
  if (blockchain_data_to_network_block_string(VRF_data.block_blob) == 0)
  {
    START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not convert the blockchain_data to a network_block_string");
  }

    // create the message
    memcpy(data,"{\r\n \"message_settings\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND_CREATE_NEW_BLOCK\",\r\n \"block_blob\": \"",97);
    memcpy(data+97,VRF_data.block_blob,strnlen(VRF_data.block_blob,BUFFER_SIZE));
    memcpy(data+strlen(data),"\"}",2);

    // sign_data
    if (sign_data(data,0) == 0)
    { 
      START_CURRENT_ROUND_START_BLOCKS_ERROR("Could not sign_data");
    }

    // set the next server message since the block verifiers will send the data to each other
    memset(server_message,0,strnlen(server_message,BUFFER_SIZE));
    memcpy(server_message,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND_CREATE_NEW_BLOCK",52); 

    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
      {
        send_data_socket(current_block_verifiers_list.block_verifiers_IP_address[count],SEND_DATA_PORT,data,"",0);
      }
    }
  
  pointer_reset(data);
  return 1;

  #undef pointer_reset_all
  #undef START_CURRENT_ROUND_START_BLOCKS_ERROR
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

int start_part_4_of_round(const int SETTINGS)
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  int count = 0;
  int count2;
  int counter;

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
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++) \
  { \
    memset(VRF_data.block_verifiers_vrf_secret_key_data[count],0,strlen(VRF_data.block_verifiers_vrf_secret_key_data[count])); \
    memset(VRF_data.block_verifiers_vrf_secret_key[count],0,strlen(VRF_data.block_verifiers_vrf_secret_key[count])); \
    memset(VRF_data.block_verifiers_vrf_public_key_data[count],0,strlen(VRF_data.block_verifiers_vrf_public_key_data[count])); \
    memset(VRF_data.block_verifiers_vrf_public_key[count],0,strlen(VRF_data.block_verifiers_vrf_public_key[count])); \
    memset(VRF_data.block_verifiers_random_data[count],0,strlen(VRF_data.block_verifiers_random_data[count])); \
    memset(VRF_data.block_blob_signature[count],0,strlen(VRF_data.block_blob_signature[count])); \
  } \
  if (memcmp(current_round_part_backup_node,"0",1) == 0) \
  { \
    memset(current_round_part_backup_node,0,strlen(current_round_part_backup_node)); \
    memcpy(current_round_part_backup_node,"1",1); \
  } \
  else if (memcmp(current_round_part_backup_node,"1",1) == 0) \
  { \
    memset(current_round_part_backup_node,0,strlen(current_round_part_backup_node)); \
    memcpy(current_round_part_backup_node,"2",1); \
  } \
  else if (memcmp(current_round_part_backup_node,"2",1) == 0) \
  { \
    memset(current_round_part_backup_node,0,strlen(current_round_part_backup_node)); \
    memcpy(current_round_part_backup_node,"3",1); \
  } \
  else if (memcmp(current_round_part_backup_node,"3",1) == 0) \
  { \
    memset(current_round_part_backup_node,0,strlen(current_round_part_backup_node)); \
    memcpy(current_round_part_backup_node,"4",1); \
  } \
  else if (memcmp(current_round_part_backup_node,"4",1) == 0) \
  { \
    memset(current_round_part_backup_node,0,strlen(current_round_part_backup_node)); \
    memcpy(current_round_part_backup_node,"5",1); \
  } \
  else if (memcmp(current_round_part_backup_node,"5",1) == 0) \
  { \
    if (memcmp(current_round_part,"4",1) != 0) \
    { \
      color_print("Waiting until 4 minutes and 40 seconds of the round to synchronize all block verifiers","green"); \
      for (;;) \
      { \
        usleep(200000); \
        get_current_UTC_time; \
        if (current_UTC_date_and_time->tm_min % 5 == 4 && current_UTC_date_and_time->tm_sec == 40) \
        { \
          break; \
        } \
      } \
    } \
    main_network_data_node_create_block = 1; \
    memset(current_round_part,0,strlen(current_round_part)); \
    memcpy(current_round_part,"4",1); \
    memset(current_round_part_backup_node,0,strlen(current_round_part_backup_node)); \
    memcpy(current_round_part_backup_node,"0",1); \
  } \
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

  if (SETTINGS == 0)
  {

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

    // add the block verifiers signed data to the VRF data
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0)
      {
        memcpy(VRF_data.block_verifiers_random_data[count],data,RANDOM_STRING_LENGTH);
      }
    } 

    memcpy(data3+329,data,RANDOM_STRING_LENGTH);
    memcpy(data3+429,"\",\r\n}",5);

    // sign_data
    if (sign_data(data3,0) == 0)
    { 
      START_PART_4_OF_ROUND_ERROR("Could not sign_data");
    }

    // set the server message
    memset(server_message,0,strnlen(server_message,BUFFER_SIZE));
    memcpy(server_message,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA",43); 

    // send the message to all block verifiers
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
      {
        send_data_socket((const char*)current_block_verifiers_list.block_verifiers_IP_address[count],SEND_DATA_PORT,(const char*)data3,"",0);
      }
    }

    // wait for the block verifiers to process the votes
    sleep(10);

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
  
    // create the VRF alpha string using all of the random data from the block verifiers
    memset(data,0,strlen(data));
    if (get_previous_block_hash(data,0) == 0)
    {
      START_PART_4_OF_ROUND_ERROR("Could not get the previous block hash");
    }

    memcpy(VRF_data.vrf_alpha_string_data_round_part_4,data,64);
    memset(data,0,strlen(data));
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (strlen(VRF_data.block_verifiers_vrf_secret_key[count]) == crypto_vrf_SECRETKEYBYTES && strlen(VRF_data.block_verifiers_vrf_public_key[count]) == crypto_vrf_PUBLICKEYBYTES && strlen(VRF_data.block_verifiers_random_data[count]) == RANDOM_STRING_LENGTH)
      {
        memcpy(VRF_data.vrf_alpha_string_data_round_part_4+strlen(VRF_data.vrf_alpha_string_data_round_part_4),VRF_data.block_verifiers_random_data[count],RANDOM_STRING_LENGTH);
      }
    }  

    crypto_hash_sha512((unsigned char*)data,(const unsigned char*)VRF_data.vrf_alpha_string_data_round_part_4,strlen(VRF_data.vrf_alpha_string_data_round_part_4));
    // convert the SHA512 data hash to a string
    memset(data2,0,strlen(data2));
    for (counter = 0, count = 0; counter < 64; counter++, count += 2)
    {
      sprintf(data2+count,"%02x",data[counter] & 0xFF);
    }

    // create the message
    memset(data3,0,strlen(data3));
    memcpy(data3,"{\r\n \"message_settings\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n \"vote_settings\": \"valid\",\r\n \"vote_data\": \"",99);
    memcpy(data3+99,data2,DATA_HASH_LENGTH);
    memcpy(data3+227,"\",\r\n}",5);

    // sign_data
    if (sign_data(data3,0) == 0)
    { 
      START_PART_4_OF_ROUND_ERROR("Could not sign_data");
    }

    // set the next server message since the block verifiers will send the data to each other
    memset(server_message,0,strnlen(server_message,BUFFER_SIZE));
    memcpy(server_message,"NODES_TO_NODES_VOTE_RESULTS",27); 

    // send the message to all block verifiers
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
      {
        send_data_socket((const char*)current_block_verifiers_list.block_verifiers_IP_address[count],SEND_DATA_PORT,(const char*)data3,"",0);
      }
    }

    // wait for the block verifiers to process the votes
    sleep(10);

    // process the vote results
    if (current_round_part_vote_data.vote_results_valid < BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      RESTART_ROUND;
    } 



    // at this point all block verifiers have the same vrf alpha string

    // check what block verifiers vrf secret key and vrf public key to use
    for (count = 0; count < DATA_HASH_LENGTH; count += 2)
    {
      memset(data,0,strlen(data));
      memcpy(data,&data2[count],2);
      counter = (int)strtol(data, NULL, 16);  
   
      // if it is not in the range of 01 - C8 then skip the byte
      if (counter != 0 && counter <= 200)
      {
        counter = counter % 100;
        break;
      }
    }

    // convert the vrf alpha string to a string
    for (counter = 0, count = 0; counter < 10064; counter++, count += 2)
    {
      sprintf(VRF_data.vrf_alpha_string_data_round_part_4+count,"%02x",VRF_data.vrf_alpha_string_round_part_4[counter] & 0xFF);
    }
  
    memset(data,0,strlen(data));
    crypto_hash_sha512((unsigned char*)data,(const unsigned char*)VRF_data.block_verifiers_vrf_public_key[counter],crypto_vrf_PUBLICKEYBYTES);

    // convert the SHA512 data hash to a string
    memset(data2,0,strlen(data2));
    for (counter = 0, count = 0; counter < 64; counter++, count += 2)
    {
      sprintf(data2+count,"%02x",data[counter] & 0xFF);
    }

    // create the message
    memset(data3,0,strlen(data3));
    memcpy(data3,"{\r\n \"message_settings\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n \"vote_settings\": \"valid\",\r\n \"vote_data\": \"",99);
    memcpy(data3+99,data2,DATA_HASH_LENGTH);
    memcpy(data3+227,"\",\r\n}",5);

    // sign_data
    if (sign_data(data3,0) == 0)
    { 
      START_PART_4_OF_ROUND_ERROR("Could not sign_data");
    }

    // set the next server message since the block verifiers will send the data to each other
    memset(server_message,0,strnlen(server_message,BUFFER_SIZE));
    memcpy(server_message,"NODES_TO_NODES_VOTE_RESULTS",27); 

    // send the message to all block verifiers
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
      {
        send_data_socket((const char*)current_block_verifiers_list.block_verifiers_IP_address[count],SEND_DATA_PORT,(const char*)data3,"",0);
      }
    }

    // wait for the block verifiers to process the votes
    sleep(10);

    // process the vote results
    if (current_round_part_vote_data.vote_results_valid < BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      RESTART_ROUND;
    } 



    // at this point all block verifiers have the same VRF data

    // create all of the VRF data
    memset(VRF_data.vrf_secret_key_data_round_part_4,0,strlen(VRF_data.vrf_secret_key_data_round_part_4));
    memset(VRF_data.vrf_secret_key_round_part_4,0,strlen(VRF_data.vrf_secret_key_round_part_4));
    memset(VRF_data.vrf_public_key_data_round_part_4,0,strlen(VRF_data.vrf_public_key_data_round_part_4));
    memset(VRF_data.vrf_public_key_round_part_4,0,strlen(VRF_data.vrf_public_key_round_part_4));

    memcpy(VRF_data.vrf_secret_key_data_round_part_4,VRF_data.block_verifiers_vrf_secret_key_data[counter],VRF_SECRET_KEY_LENGTH);
    memcpy(VRF_data.vrf_secret_key_round_part_4,VRF_data.block_verifiers_vrf_secret_key[counter],crypto_vrf_SECRETKEYBYTES);
    memcpy(VRF_data.vrf_public_key_data_round_part_4,VRF_data.block_verifiers_vrf_public_key_data[counter],VRF_PUBLIC_KEY_LENGTH);
    memcpy(VRF_data.vrf_public_key_round_part_4,VRF_data.block_verifiers_vrf_public_key[counter],crypto_vrf_PUBLICKEYBYTES);

    if (crypto_vrf_prove(VRF_data.vrf_proof_round_part_4,(const unsigned char*)VRF_data.vrf_secret_key_round_part_4,VRF_data.vrf_alpha_string_round_part_4,strlen((const char*)VRF_data.vrf_alpha_string_round_part_4)) != 0)
    {
      START_PART_4_OF_ROUND_ERROR("Could not create the vrf proof");
    }
    if (crypto_vrf_proof_to_hash(VRF_data.vrf_beta_string_round_part_4,(const unsigned char*)VRF_data.vrf_proof_round_part_4) != 0)
    {
      START_PART_4_OF_ROUND_ERROR("Could not create the vrf beta string");
    }
    if (crypto_vrf_verify(VRF_data.vrf_beta_string_round_part_4,(const unsigned char*)VRF_data.vrf_public_key_round_part_4,(const unsigned char*)VRF_data.vrf_proof_round_part_4,VRF_data.vrf_alpha_string_round_part_4,crypto_vrf_PUBLICKEYBYTES) != 0)
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

    // add all of the VRF data to the blockchain_data struct
    blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_length_round_part_4 = VRF_PUBLIC_KEY_LENGTH;
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_4,VRF_data.vrf_public_key_data_round_part_4,VRF_PUBLIC_KEY_LENGTH);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_4,VRF_data.vrf_public_key_round_part_4,crypto_vrf_PUBLICKEYBYTES);
    blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length_round_part_4 = strnlen(VRF_data.vrf_alpha_string_data_round_part_4,BUFFER_SIZE_NETWORK_BLOCK_DATA);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_4,VRF_data.vrf_alpha_string_data_round_part_4,blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_length_round_part_4);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_4,VRF_data.vrf_alpha_string_round_part_4,strnlen((const char*)VRF_data.vrf_alpha_string_round_part_4,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    blockchain_data.blockchain_reserve_bytes.vrf_proof_data_length_round_part_4 = VRF_PROOF_LENGTH;
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_4,VRF_data.vrf_proof_data_round_part_4,VRF_PROOF_LENGTH);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_4,VRF_data.vrf_proof_round_part_4,crypto_vrf_PROOFBYTES);
    blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_length_round_part_4 = VRF_BETA_LENGTH;
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_4,VRF_data.vrf_beta_string_data_round_part_4,VRF_BETA_LENGTH);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_4,VRF_data.vrf_beta_string_round_part_4,crypto_vrf_OUTPUTBYTES); 

    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key_data[count],VRF_data.block_verifiers_vrf_secret_key_data[count],VRF_SECRET_KEY_LENGTH);
      memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key[count],VRF_data.block_verifiers_vrf_secret_key[count],crypto_vrf_SECRETKEYBYTES);
      memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key_data[count],VRF_data.block_verifiers_vrf_public_key_data[count],VRF_PUBLIC_KEY_LENGTH);
      memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key[count],VRF_data.block_verifiers_vrf_public_key[count],crypto_vrf_PUBLICKEYBYTES);
      memcpy(blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data_text[count],VRF_data.block_verifiers_random_data[count],RANDOM_STRING_LENGTH);

      for (counter = 0, count2 = 0; counter < RANDOM_STRING_LENGTH; counter++, count2 += 2)
      {
        sprintf(blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data[count]+count2,"%02x",VRF_data.block_verifiers_random_data[count][counter] & 0xFF);
      }
    }

    // set the next server message since the block verifiers will send the data to each other
    memset(server_message,0,strnlen(server_message,BUFFER_SIZE));
    memcpy(server_message,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND_CREATE_NEW_BLOCK",52); 

    // create the block template and send it to all block verifiers if the block verifier is the block producer
    if ((memcmp(current_round_part_backup_node,"0",1) == 0 && memcmp(main_nodes_list.block_producer_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"1",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_1_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"2",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_2_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"3",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_3_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"4",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_4_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node,"5",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_5_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0))
    {
      if (get_block_template(VRF_data.block_blob,0) == 0)
      {
        START_PART_4_OF_ROUND_ERROR("Could not get a block template");
      }  
  
      // create the message
      memcpy(data,"{\r\n \"message_settings\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND_CREATE_NEW_BLOCK\",\r\n \"block_blob\": \"",97);
      memcpy(data+97,VRF_data.block_blob,strnlen(VRF_data.block_blob,BUFFER_SIZE));
      memcpy(data+strlen(data),"\"}",2);

      // sign_data
      if (sign_data(data,0) == 0)
      { 
        START_PART_4_OF_ROUND_ERROR("Could not sign_data");
      }    

      for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
      {
        if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
        {
          send_data_socket(current_block_verifiers_list.block_verifiers_IP_address[count],SEND_DATA_PORT,data3,"",0);
        }
      }
    }
  }

  // wait for the block verifiers to process main node data
  sleep(10);



  // at this point all block verifiers and the main node should have the same main node data

  // check if the main node data is valid
  if (memcmp(VRF_data.block_blob,"",1) == 0)
  {
    RESTART_ROUND;
  }

  // check if all of the block verifiers have the same main node data

  // create the message
  memcpy(data3,"{\r\n \"message_settings\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n \"vote_settings\": \"valid\",\r\n \"vote_data\": \"",99);  
  memcpy(data3+strnlen(data3,BUFFER_SIZE),current_round_part_vote_data.current_vote_results,DATA_HASH_LENGTH);
  memcpy(data3+strnlen(data3,BUFFER_SIZE),"\",\r\n}",5); 

  // sign_data
  if (sign_data(data3,0) == 0)
  { 
    START_PART_4_OF_ROUND_ERROR("Could not sign_data");
  }

  // send the message to all block verifiers
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
    {
      send_data_socket((const char*)current_block_verifiers_list.block_verifiers_IP_address[count],SEND_DATA_PORT,(const char*)data3,"",0);
    }
  }

  // wait for the block verifiers to process the votes
  sleep(10);

  // process the vote results
  if (current_round_part_vote_data.vote_results_valid < BLOCK_VERIFIERS_VALID_AMOUNT)
  {
    RESTART_ROUND;
  }



  // at this point is has been voted on that all block verifiers have the same main node data and the data is valid

  // sign the network block string
  memset(data,0,strlen(data));
  if (sign_network_block_string(data,VRF_data.block_blob,0) == 0)
  {
    START_PART_4_OF_ROUND_ERROR("Could not sign the network block string");
  }

  // add the block verifiers signed data to the VRF data
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0)
    {
      memcpy(VRF_data.block_blob_signature[count],data,XCASH_SIGN_DATA_LENGTH);
    }
  } 

  // create the message
  memset(data3,0,strlen(data3));
  memcpy(data3,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE\",\r\n \"block_blob_signature\": \"",110);
  memcpy(data3+110,data,XCASH_SIGN_DATA_LENGTH);
  memcpy(data3+203,"\",\r\n }",8);

  // sign_data
  if (sign_data(data3,0) == 0)
  { 
    START_PART_4_OF_ROUND_ERROR("Could not sign_data");
  }

  // set the server message
  memset(server_message,0,strnlen(server_message,BUFFER_SIZE));
  memcpy(server_message,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE",27); 

  // send the message to all block verifiers
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
    {
      send_data_socket((const char*)current_block_verifiers_list.block_verifiers_IP_address[count],SEND_DATA_PORT,(const char*)data3,"",0);
    }
  }

  // wait for the block verifiers to process the votes
  sleep(10);

  // check if at least 67 of the block verifiers signed the data
  for (count = 0, counter = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    if (memcmp(VRF_data.block_blob_signature[count],"SigV1",5) == 0)
    {
      counter++;
    }
  }
  if (counter < BLOCK_VERIFIERS_VALID_AMOUNT)
  {
    RESTART_ROUND;
  }



  // at this point all block verifiers have signed the block and have all of the other block verifiers signed data

  // update the block verifiers list as this block will be added to the network
  if (update_block_verifiers_list() == 0)
  {
    START_PART_4_OF_ROUND_ERROR("Could not sign_data");
  }

  // convert the network_block_string to a blockchain_data
  if (network_block_string_to_blockchain_data(VRF_data.block_blob,"0") == 0)
  {
    START_PART_4_OF_ROUND_ERROR("Could not sign_data");
  }

  // add all of the next_block_verifiers_list public addresses and block_blob_signature to the network block string
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memset(blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address[count],0,strlen(blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address[0]));
    memcpy(blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address[count],next_block_verifiers_list.block_verifiers_public_address[count],strnlen(next_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH+1));
    memset(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count],0,strlen(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[0]));
    memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count],VRF_data.block_blob_signature[count],strnlen(VRF_data.block_blob_signature[count],BUFFER_SIZE));
  }

  // convert the blockchain_data to a network_block_string
  memset(data3,0,strlen(data3));
  if (blockchain_data_to_network_block_string(data3) == 0)
  {
    START_PART_4_OF_ROUND_ERROR("Could not convert the blockchain_data to a network_block_string");
  }

  // copy the network block string with the current block verifiers signed data and the next block verifiers public addresses to the VRF_data
  memset(VRF_data.block_blob,0,strlen(VRF_data.block_blob));
  memcpy(VRF_data.block_blob,data3,strnlen(data3,BUFFER_SIZE));

  // reset the current_round_part_vote_data.vote_results_valid struct
  memset(current_round_part_vote_data.current_vote_results,0,strlen(current_round_part_vote_data.current_vote_results));
  current_round_part_vote_data.vote_results_valid = 0;
  current_round_part_vote_data.vote_results_invalid = 0;

  // get the data hash of the network block string
  memset(data,0,strlen(data));
  crypto_hash_sha512((unsigned char*)data,(const unsigned char*)data3,(unsigned long long)strnlen(data3,BUFFER_SIZE));

  // convert the SHA512 data hash to a string
  memset(current_round_part_vote_data.current_vote_results,0,strlen(current_round_part_vote_data.current_vote_results));
  for (counter = 0, count = 0; counter < 64; counter++, count += 2)
  {
    sprintf(current_round_part_vote_data.current_vote_results+count,"%02x",data[counter] & 0xFF);
  }

  // create the message
  memset(data3,0,strlen(data3));
  memcpy(data3,"{\r\n \"message_settings\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n \"vote_settings\": \"valid\",\r\n \"vote_data\": \"",99);
  memcpy(data3+99,current_round_part_vote_data.current_vote_results,DATA_HASH_LENGTH);
  memcpy(data3+227,"\",\r\n}",5);

  // sign_data
  if (sign_data(data3,0) == 0)
  { 
    START_PART_4_OF_ROUND_ERROR("Could not sign_data");
  }

  // send the message to all block verifiers
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
    {
      send_data_socket((const char*)current_block_verifiers_list.block_verifiers_IP_address[count],SEND_DATA_PORT,(const char*)data3,"",0);
    }
  }

  // wait for the block verifiers to process the votes
  sleep(10);

  // process the vote results
  if (current_round_part_vote_data.vote_results_valid < BLOCK_VERIFIERS_VALID_AMOUNT)
  {
    RESTART_ROUND;
  } 



  // at this point the all block verifiers have voted on the block with the current block verifiers signed data and the next block verifiers public addresses to the VRF_data

  // save the current_round_part_backup_node
  memcpy(current_round_part_backup_node_data.current_round_part_4_backup_node,current_round_part_backup_node,1);

  // copy the reserve bytes data hash
  memcpy(VRF_data.reserve_bytes_data_hash,current_round_part_vote_data.current_vote_results,DATA_HASH_LENGTH);

  // copy the current block height to calculate when the block has been submitted to the network
  memset(data3,0,strlen(data3));
  memcpy(data3,current_block_height,strnlen(current_block_height,BUFFER_SIZE));

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
  #define UPDATE_DATABASE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"update_databases",16); \
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
  count2 = count / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME; 
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
  
  // check if your reserve bytes database is synced
  if (sync_check_reserve_bytes_database((const char*)data) == 0)
  {
    UPDATE_DATABASE_ERROR("Could not check if the reserve bytes database is updated. This means you might need to sync the reserve bytes database.");
  }
  // wait for the other block verifiers to sync the databse
  sleep(10);

  // check if your delegates database is synced
  if (sync_check_delegates_database() == 0)
  {
    UPDATE_DATABASE_ERROR("Could not check if the delegates database is updated. This means you might need to sync the delegates database.");
  }
  // wait for the other block verifiers to sync the databse
  sleep(10);

  // check if your statistics database is synced
  if (sync_check_statistics_database() == 0)
  {
    UPDATE_DATABASE_ERROR("Could not check if the statistics database is updated. This means you might need to sync the statistics database.");
  }
  // wait for the other block verifiers to sync the databse
  sleep(10);

  pointer_reset_all;  
  return 1;

  #undef pointer_reset_all
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
    if (send_data_socket((const char*)previous_block_verifiers_list.block_verifiers_IP_address[count],SEND_DATA_PORT,"","checking if the block verifier is online",0) == 1)
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
    if ((memcmp(current_round_part_backup_node_data.current_round_part_4_backup_node,"0",1) == 0 && memcmp(previous_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node_data.current_round_part_4_backup_node,"1",1) == 0 && memcmp(previous_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_1_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node_data.current_round_part_4_backup_node,"2",1) == 0 && memcmp(previous_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_2_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node_data.current_round_part_4_backup_node,"3",1) == 0 && memcmp(previous_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_3_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node_data.current_round_part_4_backup_node,"4",1) == 0 && memcmp(previous_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_4_public_address,XCASH_WALLET_LENGTH) == 0) || (memcmp(current_round_part_backup_node_data.current_round_part_4_backup_node,"5",1) == 0 && memcmp(previous_block_verifiers_list.block_verifiers_public_address[count],main_nodes_list.block_producer_backup_block_verifier_5_public_address,XCASH_WALLET_LENGTH) == 0))
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
  if (sync_check_reserve_bytes_database((const char*)data) == 0)
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
  count--;
  sprintf(data2,"%zu",count);

  // calculate the database to get the reserve byte data
  count2 = count / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME;
  memcpy(data,"reserve_bytes_",14);
  sprintf(data,"%d",count2);

  // create the message
  memcpy(data3,"{\"block_height\":\"",17);
  memcpy(data3,data2,strnlen(data2,BUFFER_SIZE));
  memcpy(data3,"\"}",2);

  // get the reserve byte data
  memset(data2,0,strlen(data2));
  if (read_document_field_from_collection(DATABASE_NAME,data,data3,"reserve_bytes",data2,0) == 0)
  {
    CALCULATE_MAIN_NODES_ROLES("Could not get the previous blocks reserve bytes");
  }

  // get the vrf_beta_string_data_round_part_3
  memset(data3,0,strlen(data3));
  if (parse_reserve_bytes_data(data3,(const char*)data2,34,VRF_BETA_LENGTH) == 0)
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
      count2 = count2 % 100;
      if (main_nodes_count == 0)
      {
        // calculate the block_producer
        memset(main_nodes_list.block_producer_public_address,0,strnlen(main_nodes_list.block_producer_public_address,XCASH_WALLET_LENGTH));
        memset(main_nodes_list.block_producer_IP_address,0,strnlen(main_nodes_list.block_producer_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
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
    }
    if (((count + 2) == VRF_BETA_LENGTH) && (main_nodes_count != 6))
    {
      color_print("The main nodes calculation process has run out of bytes to read","red");

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
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // define macros
  #define pointer_reset_all \
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
  if (parse_json_data(MESSAGE,"data_hash",data) == 0)
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
    memset(data,0,strlen(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"reserve_proofs_database\": \"true\"}",146);
  }
  else
  {
    memset(data,0,strlen(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"reserve_proofs_database\": \"false\"}",147);
  }
  
  // sign_data
  if (sign_data(data,0) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_ERROR("Could not sign data");
  }

  // send the data
  if (send_data(CLIENT_SOCKET,data,1) == 0)
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
  if (parse_json_data(MESSAGE,"data_hash",data) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not parse the message");
  }

  // create the message
  if (memcmp(data,data2,DATA_HASH_LENGTH) == 0)
  {
    memset(data,0,strlen(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"reserve_proofs_database\": \"true\"}",146);
  }
  else
  {
    memset(data,0,strlen(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"reserve_proofs_database\": \"false\"}",147);
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
  if (parse_json_data(MESSAGE,"data_hash",data) == 0)
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
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"reserve_bytes_database\": \"true\"}",146);
  }
  else
  {
    memset(data,0,strlen(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"reserve_bytes_database\": \"false\"}",147);
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
  if (parse_json_data(MESSAGE,"data_hash",data) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not parse the message");
  }

  // create the message
  if (memcmp(data,data2,DATA_HASH_LENGTH) == 0)
  {
    memset(data,0,strlen(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"reserve_bytes_database\": \"true\"}",146);
  }
  else
  {
    memset(data,0,strlen(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"reserve_bytes_database\": \"false\"}",147);
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
  if (parse_json_data(MESSAGE,"data_hash",data) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not parse the message");
  }

  // create the message
  if (memcmp(data,data2,DATA_HASH_LENGTH) == 0)
  {
    memset(data,0,strlen(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"delegates_database\": \"true\"}",146);
  }
  else
  {
    memset(data,0,strlen(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"delegates_database\": \"false\"}",147);
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
  if (parse_json_data(MESSAGE,"data_hash",data) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE_ERROR("Could not parse the message");
  }

  // create the message
  if (memcmp(data,data2,DATA_HASH_LENGTH) == 0)
  {
    memset(data,0,strlen(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"statistics_database\": \"true\"}",146);
  }
  else
  {
    memset(data,0,strlen(data));
    memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"statistics_database\": \"false\"}",147);
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
  if (parse_json_data(MESSAGE,"delegates_public_address",delegates_public_address) == 0 || parse_json_data(MESSAGE,"public_address",public_address) == 0 || parse_json_data(MESSAGE,"reserve_proof",reserve_proof) == 0)
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

  for (count = 1; count <= 50; count++)
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
    for (count = 1; count <= 50; count++)
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
  for (count = 1; count <= 50; count++)
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
  if (parse_json_data(MESSAGE,"public_address",block_verifiers_public_address) == 0 || parse_json_data(MESSAGE,"public_address_that_created_the_reserve_proof",data) == 0 || parse_json_data(MESSAGE,"reserve_proof",data2) == 0)
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
  if (parse_json_data(MESSAGE,"delegate_name",delegate_name) == 0 || parse_json_data(MESSAGE,"public_address",delegate_public_address) == 0 || parse_json_data(MESSAGE,"delegates_IP_address",delegates_IP_address) == 0)
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
  if (parse_json_data(MESSAGE,"public_address",delegate_public_address) == 0)
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
  if (parse_json_data(MESSAGE,"public_address",delegate_public_address) == 0 || parse_json_data(MESSAGE,"item",item) == 0 || parse_json_data(MESSAGE,"value",value) == 0)
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
  data2 = NULL; \
  free(data3); \
  data3 = NULL; \
  free(message); \
  message = NULL;  

  #define SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_main_node_to_node_message_part_4",59); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  memset(VRF_data.block_blob,0,strlen(VRF_data.block_blob));

  // verify the data
  if (verify_data(MESSAGE,0,1) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR("Could not verify data");
  }

  // parse the message
  if (parse_json_data(MESSAGE,"block_blob",data) == 0 || parse_json_data(MESSAGE,"public_address",data2) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR("Could not parse the data");
  }

  // check if the public_address is the correct main node
  if ((main_network_data_node_create_block == 1 && memcmp(network_data_nodes_list.network_data_nodes_public_address[0],data2,XCASH_WALLET_LENGTH) == 0 && verify_network_block_data(0,1,1,"0","") == 1) || (main_network_data_node_create_block == 0 && memcmp(current_round_part_backup_node,"0",1) == 0 && memcmp(main_nodes_list.block_producer_public_address,data2,XCASH_WALLET_LENGTH) == 0 && verify_network_block_data(0,1,1,"0","") == 1) || (main_network_data_node_create_block == 0 && memcmp(current_round_part_backup_node,"1",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_1_public_address,data2,XCASH_WALLET_LENGTH) == 0 && verify_network_block_data(0,1,1,"0","") == 1) || (main_network_data_node_create_block == 0 && memcmp(current_round_part_backup_node,"2",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_2_public_address,data2,XCASH_WALLET_LENGTH) == 0 && verify_network_block_data(0,1,1,"0","") == 1) || (main_network_data_node_create_block == 0 && memcmp(current_round_part_backup_node,"3",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_3_public_address,data2,XCASH_WALLET_LENGTH) == 0 && verify_network_block_data(0,1,1,"0","") == 1) || (main_network_data_node_create_block == 0 && memcmp(current_round_part_backup_node,"4",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_4_public_address,data2,XCASH_WALLET_LENGTH) == 0 && verify_network_block_data(0,1,1,"0","") == 1) || (main_network_data_node_create_block == 0 && memcmp(current_round_part_backup_node,"5",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_5_public_address,data2,XCASH_WALLET_LENGTH) == 0 && verify_network_block_data(0,1,1,"0","") == 1))
  {    
    // get the previous network block string
    sscanf(current_block_height,"%zu", &count);
    count--;
    sprintf(data3,"%zu",count);
    count2 = count / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME;
    memcpy(message,"{\"block_height\":\"",17);
    memcpy(message+17,data3,strnlen(data3,BUFFER_SIZE));
    memcpy(message+strlen(message),"\"}",2);
    memset(data3,0,strlen(data3));
    memcpy(data3,"reserve_bytes_",14);
    sprintf(data3+14,"%zu",count2);
    memset(data2,0,strlen(data2));
    if (read_document_field_from_collection(DATABASE_NAME,data3,message,"reserve_bytes",data2,0) == 0)
    {
      SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR("Could not get the previous blocks reserve bytes");
    }
    // verify the block
    if (network_block_string_to_blockchain_data(data,"0") == 0 || verify_network_block_data(1,1,1,"0",data2) == 0)
    {
      SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_ERROR("The MAIN_NODES_TO_NODES_PART_4_OF_ROUND message is invalid");
    }
    memcpy(VRF_data.block_blob,data,strnlen(data,BUFFER_SIZE));

    // SHA2-512 hash the received message
    memset(current_round_part_vote_data.current_vote_results,0,strlen(current_round_part_vote_data.current_vote_results));
    memset(data,0,strlen(data));
    crypto_hash_sha512((unsigned char*)data,(const unsigned char*)MESSAGE,(unsigned long long)strnlen(MESSAGE,BUFFER_SIZE));

    // convert the SHA512 data hash to a string
    for (count2 = 0, count = 0; count2 < 64; count2++, count += 2)
    {
      sprintf(current_round_part_vote_data.current_vote_results+count,"%02x",data[count2] & 0xFF);
    }
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
  if (parse_json_data(MESSAGE,"block_blob",data) == 0 || parse_json_data(MESSAGE,"public_address",data2) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_CREATE_NEW_BLOCK_ERROR("Could not parse the data");
  }

  // check if the public_address is the correct main node
  if ((main_network_data_node_create_block == 1 && memcmp(network_data_nodes_list.network_data_nodes_public_address[0],data2,XCASH_WALLET_LENGTH) == 0 && verify_network_block_data(0,1,1,"0","") == 1) || (main_network_data_node_create_block == 0 && memcmp(current_round_part_backup_node,"0",1) == 0 && memcmp(main_nodes_list.block_producer_public_address,data2,XCASH_WALLET_LENGTH) == 0 && verify_network_block_data(0,1,1,"0","") == 1) || (main_network_data_node_create_block == 0 && memcmp(current_round_part_backup_node,"1",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_1_public_address,data2,XCASH_WALLET_LENGTH) == 0 && verify_network_block_data(0,1,1,"0","") == 1) || (main_network_data_node_create_block == 0 && memcmp(current_round_part_backup_node,"2",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_2_public_address,data2,XCASH_WALLET_LENGTH) == 0 && verify_network_block_data(0,1,1,"0","") == 1) || (main_network_data_node_create_block == 0 && memcmp(current_round_part_backup_node,"3",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_3_public_address,data2,XCASH_WALLET_LENGTH) == 0 && verify_network_block_data(0,1,1,"0","") == 1) || (main_network_data_node_create_block == 0 && memcmp(current_round_part_backup_node,"4",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_4_public_address,data2,XCASH_WALLET_LENGTH) == 0 && verify_network_block_data(0,1,1,"0","") == 1) || (main_network_data_node_create_block == 0 && memcmp(current_round_part_backup_node,"5",1) == 0 && memcmp(main_nodes_list.block_producer_backup_block_verifier_5_public_address,data2,XCASH_WALLET_LENGTH) == 0 && verify_network_block_data(0,1,1,"0","") == 1))
  {    
    // get the previous network block string
    sscanf(current_block_height,"%zu", &count);
    count--;
    sprintf(data3,"%zu",count);
    count2 = count / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME;
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
    memset(current_round_part_vote_data.current_vote_results,0,strlen(current_round_part_vote_data.current_vote_results));
    memset(data,0,strlen(data));
    crypto_hash_sha512((unsigned char*)data,(const unsigned char*)MESSAGE,(unsigned long long)strnlen(MESSAGE,BUFFER_SIZE));

    // convert the SHA512 data hash to a string
    for (count2 = 0, count = 0; count2 < 64; count2++, count += 2)
    {
      sprintf(current_round_part_vote_data.current_vote_results+count,"%02x",data[count2] & 0xFF);
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
  if (parse_json_data(MESSAGE,"vote_settings",data) == 0 || parse_json_data(MESSAGE,"vote_data",data2) == 0)
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
  free(data); \
  data = NULL;

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"server_receive_data_socket_block_verifiers_to_block_verifiers_vrf_data",70); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (public_address == NULL || vrf_secret_key_data == NULL || vrf_public_key_data == NULL || data == NULL)
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
  if (parse_json_data(MESSAGE,"public_address",public_address) == 0 || parse_json_data(MESSAGE,"vrf_secret_key",vrf_secret_key_data) == 0 || parse_json_data(MESSAGE,"vrf_public_key",vrf_public_key_data) == 0 || parse_json_data(MESSAGE,"random_data",data) == 0)
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
      memcpy(VRF_data.block_verifiers_vrf_secret_key_data[count],vrf_secret_key_data,VRF_SECRET_KEY_LENGTH);
      memcpy(VRF_data.block_verifiers_vrf_secret_key[count],vrf_secret_key,crypto_vrf_SECRETKEYBYTES);
      memcpy(VRF_data.block_verifiers_vrf_public_key_data[count],vrf_public_key_data,VRF_PUBLIC_KEY_LENGTH);
      memcpy(VRF_data.block_verifiers_vrf_public_key[count],vrf_public_key,crypto_vrf_PUBLICKEYBYTES);
      memcpy(VRF_data.block_verifiers_random_data[count],data,RANDOM_STRING_LENGTH);
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
  if (parse_json_data(MESSAGE,"block_blob_signature",data) == 0 || parse_json_data(MESSAGE,"public_address",data2) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE_ERROR("Could not parse the data");
  }

  // process the vote data
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],data2,XCASH_WALLET_LENGTH) == 0)
    {
      memcpy(VRF_data.block_blob_signature[count],data,XCASH_SIGN_DATA_LENGTH);
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
  char* string = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  int len;
  int receive_data_result; 
  struct sockaddr_in addr, cl_addr; 

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

  if (MESSAGE_SETTINGS == 1)
  {
    print_start_message("Creating the server");
  }

  // set the main process to ignore if forked processes return a value or not, since the timeout for the total connection time is run on a different thread
  signal(SIGCHLD, SIG_IGN);  

  // check if the memory needed was allocated on the heap successfully
  if (string == NULL)
  {
    memcpy(error_message.function[error_message.total],"create_server",13);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
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
      memcpy(error_message.function[error_message.total],"create_server",13);
      memcpy(error_message.data[error_message.total],"Error creating socket",21);
      error_message.total++;
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
      memcpy(error_message.function[error_message.total],"create_server",13);
      memcpy(error_message.data[error_message.total],"Error setting socket options",28);
      error_message.total++;
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
     memcpy(error_message.function[error_message.total],"create_server",13);
     memcpy(error_message.data[error_message.total],string,strlen(string));
     error_message.total++;
   }
   SERVER_ERROR(0);
  } 
  if (MESSAGE_SETTINGS == 1)
  {
    memset(string,0,strnlen(string,BUFFER_SIZE));
    memcpy(string,"Connected to port ",18);
    memcpy(string+18,buffer2,strnlen(buffer2,BUFFER_SIZE));
    color_print(string,"green");

    printf("Waiting for a connection...\n\n");
  }

  // set the maximum simultaneous connections
  if (listen(SOCKET, MAXIMUM_CONNECTIONS) != 0)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      memcpy(error_message.function[error_message.total],"create_server",13);
      memcpy(error_message.data[error_message.total],"Error creating the server",28);
      error_message.total++;
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

         if (MESSAGE_SETTINGS == 1)
         { 
           // get the current time
           get_current_UTC_time;

           memset(string,0,strlen(string));
           memcpy(string,"Received ",9);
           memcpy(string+9,&buffer[25],strlen(buffer) - strlen(strstr(buffer,"\",\r\n")) - 25);
           memcpy(string+strlen(string)," from ",6);
           memcpy(string+strlen(string),client_address,CLIENT_ADDRESS_LENGTH);
           memcpy(string+strlen(string)," on port ",9);
           memcpy(string+strlen(string),buffer2,BUFFER2_LENGTH);
           memcpy(string+strlen(string),"\n",1);
           memcpy(string+strlen(string),asctime(current_UTC_date_and_time),strlen(asctime(current_UTC_date_and_time)));
           color_print(string,"green");
         }

         // check if a certain type of message has been received 
         if (strstr(buffer,"\"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\"") != NULL && strncmp(server_message,"XCASH_PROOF_OF_STAKE_TEST_DATA",BUFFER_SIZE) == 0)
         {
           server_received_data_xcash_proof_of_stake_test_data(CLIENT_SOCKET,(const char*)buffer);
           close(SOCKET);
           pointer_reset_all;
           _exit(0);
         }
         else if (strstr(buffer,"\"message_settings\": \"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST\"") != NULL && network_data_node_settings == 1)
         {
           printf("Received NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST from %s on %s",client_address, buffer2);
           server_receive_data_socket_node_to_network_data_nodes_get_previous_current_next_block_verifiers_list(CLIENT_SOCKET);
           close(SOCKET);
           pointer_reset_all; 
           _exit(0);
         } 
         else if (strstr(buffer,"\"message_settings\": \"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST\"") != NULL && network_data_node_settings == 1)
         {
           printf("Received NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST from %s on %s",client_address, buffer2);
           server_receive_data_socket_node_to_network_data_nodes_get_current_block_verifiers_list(CLIENT_SOCKET);
           close(SOCKET);
           pointer_reset_all; 
           _exit(0);
         } 
         else if (strstr(buffer,"\"message_settings\": \"NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE\"") != NULL)
         {
           server_receive_data_socket_nodes_to_block_verifiers_reserve_bytes_database_sync_check_all_update(CLIENT_SOCKET);
           close(SOCKET);
           pointer_reset_all; 
           _exit(0);
         }
         else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE\"") != NULL)
         {
           server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_all_update(CLIENT_SOCKET,(const char*)buffer);
           close(SOCKET);
           pointer_reset_all; 
           _exit(0);
         } 
         else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE\"") != NULL)
         {
           server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_update(CLIENT_SOCKET,(const char*)buffer);
           close(SOCKET);
           pointer_reset_all; 
           _exit(0);
         }  
         else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE\"") != NULL)
         {
           server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_download_file_update(CLIENT_SOCKET,(const char*)buffer);
           close(SOCKET);
           pointer_reset_all; 
           _exit(0);
         }  
         else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE\"") != NULL)
         {
           server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_all_update(CLIENT_SOCKET,(const char*)buffer);
           close(SOCKET);
           pointer_reset_all; 
           _exit(0);
         }
         else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE\"") != NULL)
         {
           server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_update(CLIENT_SOCKET,(const char*)buffer);
           close(SOCKET);
           pointer_reset_all; 
           _exit(0);
         }
         else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE\"") != NULL)
         {
           server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_download_file_update(CLIENT_SOCKET,(const char*)buffer);
           close(SOCKET);
           pointer_reset_all; 
           _exit(0);
         }
         else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE\"") != NULL)
         {
           server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_sync_check_update(CLIENT_SOCKET,(const char*)buffer);
           close(SOCKET);
           pointer_reset_all; 
           _exit(0);
         }
         else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE\"") != NULL)
         {
           server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_download_file_update(CLIENT_SOCKET,(const char*)buffer);
           close(SOCKET);
           pointer_reset_all; 
           _exit(0);
         }
         else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE\"") != NULL)
         {
           server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_sync_check_update(CLIENT_SOCKET,(const char*)buffer);
           close(SOCKET);
           pointer_reset_all; 
           _exit(0);
         }
         else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE\"") != NULL)
         {
           server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_download_file_update(CLIENT_SOCKET,(const char*)buffer);
           close(SOCKET);
           pointer_reset_all; 
           _exit(0);
         }
         else if (strstr(buffer,"\"message_settings\": \"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF\"") != NULL)
         {
           server_receive_data_socket_node_to_block_verifiers_add_reserve_proof(CLIENT_SOCKET,(const char*)buffer);
           close(SOCKET);
           pointer_reset_all; 
           _exit(0);
         } 
         else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS\"") != NULL && current_UTC_date_and_time->tm_min % 4 == 0 && current_UTC_date_and_time->tm_sec < 5)
         {
           server_receive_data_socket_block_verifiers_to_block_verifiers_invalid_reserve_proofs((const char*)buffer);
           close(SOCKET);
           pointer_reset_all; 
           _exit(0);
         }  
         else if (strstr(buffer,"\"message_settings\": \"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE\"") != NULL)
         {
           server_receive_data_socket_nodes_to_block_verifiers_register_delegates(CLIENT_SOCKET,(const char*)buffer);
           close(SOCKET);
           pointer_reset_all; 
           _exit(0);
         }            
         else if (strstr(buffer,"\"message_settings\": \"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE\"") != NULL)
         {
           server_receive_data_socket_nodes_to_block_verifiers_remove_delegates(CLIENT_SOCKET,(const char*)buffer);
           close(SOCKET);
           pointer_reset_all; 
           _exit(0);
         } 
         else if (strstr(buffer,"\"message_settings\": \"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE\"") != NULL)
         {
           server_receive_data_socket_nodes_to_block_verifiers_update_delegates(CLIENT_SOCKET,(const char*)buffer);
           close(SOCKET);
           pointer_reset_all; 
           _exit(0);
         } 
         else if (strstr(buffer,"\"message_settings\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND\"") != NULL && memcmp(current_round_part,"4",1) == 0 && memcmp(server_message,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND",35) == 0)
         {
           server_receive_data_socket_main_node_to_node_message_part_4((const char*)buffer);
           close(SOCKET);
           pointer_reset_all; 
           _exit(0);
         } 
         else if (strstr(buffer,"\"message_settings\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND_CREATE_NEW_BLOCK\"") != NULL && memcmp(current_round_part,"4",1) == 0 && memcmp(server_message,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND_CREATE_NEW_BLOCK",52) == 0)
         {
           server_receive_data_socket_main_node_to_node_message_part_4((const char*)buffer);
           close(SOCKET);
           pointer_reset_all; 
           _exit(0);
         } 
         else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA\"") != NULL && memcmp(current_round_part,"4",1) == 0 && memcmp(server_message,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA",46) == 0)
         {
           server_receive_data_socket_block_verifiers_to_block_verifiers_vrf_data((const char*)buffer);
           close(SOCKET);
           pointer_reset_all; 
           _exit(0);
         }  
         else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE\"") != NULL && memcmp(current_round_part,"4",1) == 0 && memcmp(server_message,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE",55) == 0)
         {
           server_receive_data_socket_block_verifiers_to_block_verifiers_block_blob_signature((const char*)buffer);
           close(SOCKET);
           pointer_reset_all; 
           _exit(0);
         }  
         if (strstr(buffer,"\"message_settings\": \"NODES_TO_NODES_VOTE_RESULTS\"") != NULL && strstr(server_message,"NODES_TO_NODES_VOTE_RESULTS") != NULL)
         {
           server_receive_data_socket_node_to_node((const char*)buffer);
           close(SOCKET);
           pointer_reset_all; 
           _exit(0);
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

