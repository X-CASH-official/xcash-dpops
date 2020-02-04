#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "variables.h"
#include "define_macros_test.h"

#include "count_database_functions.h"
#include "read_database_functions.h"
#include "network_daemon_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "string_functions.h"
#include "VRF_functions.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: sign_data
Description: Signs data with your XCA address, for sending data securely
Parameters:
  message - The sign_data
  MESSAGE_SETTINGS - 1 to print the messages, otherwise 0. This is used for the testing flag to not print any success or error messages
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sign_data(char *message, const int HTTP_SETTINGS)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char random_data[RANDOM_STRING_LENGTH+1];
  char data[BUFFER_SIZE];
  char proof[VRF_PROOF_LENGTH+1];
  char beta_string[VRF_BETA_LENGTH+1];
  char* result = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char)); // 50 MB
  char* string = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char)); // 50 MB
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;

  // define macros
  #define pointer_reset_all \
  free(result); \
  result = NULL; \
  free(string); \
  string = NULL;

  #define SIGN_DATA_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"sign_data",9); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (result == NULL || string == NULL)
  {    
    if (result != NULL)
    {
      pointer_reset(result);
    }
    if (string != NULL)
    {
      pointer_reset(string);
    }
    memcpy(error_message.function[error_message.total],"sign_data",9);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,data);
    exit(0);
  } 
  
  memset(proof,0,sizeof(proof));
  memset(beta_string,0,sizeof(beta_string));
  memset(random_data,0,sizeof(random_data));
  memset(data,0,sizeof(data));
  
  // create the random data
  if (random_string(random_data,RANDOM_STRING_LENGTH) == 0)
  {
    SIGN_DATA_ERROR("Could not create the random data");
  }
  
  pthread_rwlock_rdlock(&rwlock);
  // create the message
  memcpy(result,message,strlen(message)-1);
  memcpy(result+strlen(result),"\"public_address\": \"",19);
  memcpy(result+strlen(result),xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(result+strlen(result),"\",\r\n \"previous_block_hash\": \"",29);
  memcpy(result+strlen(result),previous_block_hash,strnlen(previous_block_hash,BUFFER_SIZE));  
  memcpy(result+strlen(result),"\",\r\n \"current_round_part\": \"",28);
  memcpy(result+strlen(result),current_round_part,1);
  memcpy(result+strlen(result),"\",\r\n \"current_round_part_backup_node\": \"",40);
  memcpy(result+strlen(result),current_round_part_backup_node,1);
  memcpy(result+strlen(result),"\",\r\n \"data\": \"",14);
  memcpy(result+strlen(result),random_data,RANDOM_STRING_LENGTH);
  memcpy(result+strlen(result),"\",\r\n}",5);
  pthread_rwlock_unlock(&rwlock);

  // format the message
  if (string_replace(result,MAXIMUM_BUFFER_SIZE,"\"","\\\"") == 0)
  {
    SIGN_DATA_ERROR("Could not create the message");
  }
  
  if (strstr(message,"XCASH_PROOF_OF_STAKE_TEST_DATA_2") != NULL || (strstr(message,"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST") == NULL && strstr(message,"XCASH_PROOF_OF_STAKE_TEST_DATA") == NULL && strstr(message,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF") == NULL && strstr(message,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE") == NULL && strstr(message,"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE") == NULL && strstr(message,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE") == NULL))
  {
    // sign data
    if (VRF_sign_data(beta_string,proof,result) == 0)
    {
      SIGN_DATA_ERROR("Could not sign the message");
    }
    
    pthread_rwlock_rdlock(&rwlock);
    // create the message  
    memcpy(message+strlen(message)-1,"\"public_address\": \"",19);
    memcpy(message+strlen(message),xcash_wallet_public_address,XCASH_WALLET_LENGTH);
    memcpy(message+strlen(message),"\",\r\n \"previous_block_hash\": \"",29);
    memcpy(message+strlen(message),previous_block_hash,strnlen(previous_block_hash,BUFFER_SIZE));  
    memcpy(message+strlen(message),"\",\r\n \"current_round_part\": \"",28);
    memcpy(message+strlen(message),current_round_part,1);
    memcpy(message+strlen(message),"\",\r\n \"current_round_part_backup_node\": \"",40);
    memcpy(message+strlen(message),current_round_part_backup_node,1);
    memcpy(message+strlen(message),"\",\r\n \"data\": \"",14);
    memcpy(message+strlen(message),random_data,RANDOM_STRING_LENGTH);
    memcpy(message+strlen(message),"\",\r\n \"XCASH_DPOPS_signature\": \"",31);
    memcpy(message+strlen(message),proof,VRF_PROOF_LENGTH);
    memcpy(message+strlen(message),beta_string,VRF_BETA_LENGTH);
    memcpy(message+strlen(message),"\",\r\n}",5);
    pthread_rwlock_unlock(&rwlock);
  }
  else
  {
    // sign_data
    memcpy(string,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"sign\",\"params\":{\"data\":\"",60);
    memcpy(string+60,result,strnlen(result,MAXIMUM_BUFFER_SIZE));
    memcpy(string+strlen(string),"\"}}",3);
    memset(result,0,strlen(result));
  
    if (send_http_request(data,"127.0.0.1","/json_rpc",XCASH_WALLET_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,string,RECEIVE_DATA_TIMEOUT_SETTINGS,"sign data",HTTP_SETTINGS) <= 0)
    {  
      SIGN_DATA_ERROR("Could not create the message");
    } 

    if (parse_json_data(data,"signature",result,MAXIMUM_BUFFER_SIZE) == 0)
    {
      SIGN_DATA_ERROR("Could not create the message");
    }

    // check if the returned data is valid
    if (strnlen(result,BUFFER_SIZE) != XCASH_SIGN_DATA_LENGTH && strncmp(result,XCASH_SIGN_DATA_PREFIX,sizeof(XCASH_SIGN_DATA_PREFIX)-1) != 0)
    {
      SIGN_DATA_ERROR("Could not create the message");
    }
  
    pthread_rwlock_rdlock(&rwlock);
    // create the message  
    memcpy(message+strlen(message)-1,"\"public_address\": \"",19);
    memcpy(message+strlen(message),xcash_wallet_public_address,XCASH_WALLET_LENGTH);
    memcpy(message+strlen(message),"\",\r\n \"previous_block_hash\": \"",29);
    memcpy(message+strlen(message),previous_block_hash,strnlen(previous_block_hash,BUFFER_SIZE));  
    memcpy(message+strlen(message),"\",\r\n \"current_round_part\": \"",28);
    memcpy(message+strlen(message),current_round_part,1);
    memcpy(message+strlen(message),"\",\r\n \"current_round_part_backup_node\": \"",40);
    memcpy(message+strlen(message),current_round_part_backup_node,1);
    memcpy(message+strlen(message),"\",\r\n \"data\": \"",14);
    memcpy(message+strlen(message),random_data,RANDOM_STRING_LENGTH);
    memcpy(message+strlen(message),"\",\r\n \"XCASH_DPOPS_signature\": \"",31);
    memcpy(message+strlen(message),result,XCASH_SIGN_DATA_LENGTH);
    memcpy(message+strlen(message),"\",\r\n}",5);
    pthread_rwlock_unlock(&rwlock);
  }

  pointer_reset_all;
  return 1;

  #undef pointer_reset_all  
  #undef SIGN_DATA_ERROR
}


/*
-----------------------------------------------------------------------------------------------------------
Name: verify_data
Description: Verifies signed data, for receiving data securely
Parameters:
  message - The signed data
  HTTP_SETTINGS - The http settings
  VERIFY_CURRENT_ROUND_PART_AND_CURRENT_ROUND_PART_BACKUP_NODE_SETTINGS - 1 to verify the current_round_part and the current_round_part_backup_node, otherwise 0
Return: 0 if the signed data is not verified, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int verify_data(const char* MESSAGE, const int HTTP_SETTINGS, const int VERIFY_CURRENT_ROUND_PART_AND_CURRENT_ROUND_PART_BACKUP_NODE_SETTINGS)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);
  
  // Variables
  char message_settings[BUFFER_SIZE];
  char public_address[BUFFER_SIZE];
  char message_previous_block_hash[BUFFER_SIZE];
  char message_current_round_part[BUFFER_SIZE];
  char message_current_round_part_backup_node[BUFFER_SIZE];
  char XCASH_DPOPS_signature[BUFFER_SIZE];
  char public_key[VRF_PUBLIC_KEY_LENGTH+1];
  char proof[VRF_PROOF_LENGTH+1];
  char beta_string[VRF_BETA_LENGTH+1];
  unsigned char public_key_data[crypto_vrf_PUBLICKEYBYTES+1];
  unsigned char proof_data[crypto_vrf_PROOFBYTES+1];
  unsigned char beta_string_data[crypto_vrf_OUTPUTBYTES+1];
  char* result = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char)); // 50 MB
  char data[BUFFER_SIZE];
  char* string = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char)); // 50 MB
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  size_t message_length;
  size_t count;
  size_t count2;

  // define macros
  #define VERIFY_DATA_ERROR(settings) \
  if (network_functions_test_error_settings == 1) \
  { \
  memcpy(error_message.function[error_message.total],"verify_data",11); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  print_error_message(current_date_and_time,current_UTC_date_and_time,data); \
  } \
  pointer_reset_all; \
  return 0;

  #define pointer_reset_all \
  free(result); \
  result = NULL; \
  free(string); \
  string = NULL;

  // check if the memory needed was allocated on the heap successfully
  if (result == NULL || string == NULL)
  {    
    if (result != NULL)
    {
      pointer_reset(result);
    }
    if (string != NULL)
    {
      pointer_reset(string);
    }
    memcpy(error_message.function[error_message.total],"verify_data",11);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,data);  
    exit(0);
  } 

  memset(message_settings,0,sizeof(message_settings));
  memset(public_address,0,sizeof(public_address));
  memset(message_previous_block_hash,0,sizeof(message_previous_block_hash));
  memset(message_current_round_part,0,sizeof(message_current_round_part));
  memset(message_current_round_part_backup_node,0,sizeof(message_current_round_part_backup_node));
  memset(XCASH_DPOPS_signature,0,sizeof(XCASH_DPOPS_signature));
  memset(public_key,0,sizeof(public_key));
  memset(proof,0,sizeof(proof));
  memset(beta_string,0,sizeof(beta_string));
  memset(public_key_data,0,sizeof(public_key_data));
  memset(proof_data,0,sizeof(proof_data));
  memset(beta_string_data,0,sizeof(beta_string_data));
  memset(data,0,sizeof(data));

  // parse the message
  if (strstr(MESSAGE,"}") != NULL)
  { 
    if (parse_json_data(MESSAGE,"message_settings",message_settings,sizeof(message_settings)) == 0)
    {
      VERIFY_DATA_ERROR("Could not parse the message");
    }
  }
  else
  {
    memcpy(message_settings,MESSAGE,strnlen(MESSAGE,sizeof(message_settings)) - strnlen(strstr(MESSAGE,"|"),sizeof(message_settings)));
  }  

  if (strncmp(message_settings,"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"XCASH_PROOF_OF_STAKE_TEST_DATA",sizeof(message_settings)) == 0 || strncmp(message_settings,"XCASH_PROOF_OF_STAKE_TEST_DATA_2",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE",sizeof(message_settings)) == 0)
  {
    if (strstr(MESSAGE,"}") != NULL)
    { 
      if (parse_json_data(MESSAGE,"public_address",public_address,sizeof(public_address)) == 0 || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",XCASH_DPOPS_signature,sizeof(XCASH_DPOPS_signature)) == 0)
      {
        VERIFY_DATA_ERROR("Could not parse the message");
      }
    }
    else
    {
      if (strncmp(message_settings,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF",sizeof(message_settings)) == 0)
      {
        if (string_count(MESSAGE,"|") == VOTE_PARAMETER_AMOUNT)
        {
          for (count = 0, count2 = 0; count < string_count(MESSAGE,"|"); count++)
          {
            if (count == 3)
            {
              memcpy(public_address,&MESSAGE[count2],strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2);
            }
            if (count == 4)
            {
              memcpy(XCASH_DPOPS_signature,&MESSAGE[count2],strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2);
            }
            count2 = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) + 1;
          }
        }
        else
        {
          VERIFY_DATA_ERROR("Could not parse the message");
        }  
      }
      else if (strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE",sizeof(message_settings)) == 0)
      {
        if (string_count(MESSAGE,"|") == REGISTER_PARAMETER_AMOUNT)
        {
          for (count = 0, count2 = 0; count < string_count(MESSAGE,"|"); count++)
          {
            if (count == 4)
            {
              memcpy(public_address,&MESSAGE[count2],strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2);
            }
            if (count == 5)
            {
              memcpy(XCASH_DPOPS_signature,&MESSAGE[count2],strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2);
            }
            count2 = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) + 1;
          }
        }
        else
        {
          VERIFY_DATA_ERROR("Could not parse the message");
        }  
      }
      else if (strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE",sizeof(message_settings)) == 0)
      {
        if (string_count(MESSAGE,"|") == REMOVE_PARAMETER_AMOUNT)
        {
          for (count = 0, count2 = 0; count < string_count(MESSAGE,"|"); count++)
          {
            if (count == 1)
            {
              memcpy(public_address,&MESSAGE[count2],strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2);
            }
            if (count == 2)
            {
              memcpy(XCASH_DPOPS_signature,&MESSAGE[count2],strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2);
            }
            count2 = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) + 1;
          }
        }
        else
        {
          VERIFY_DATA_ERROR("Could not parse the message");
        }  
      }
      else if (strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE",sizeof(message_settings)) == 0)
      {
        if (string_count(MESSAGE,"|") == UPDATE_PARAMETER_AMOUNT)
        {
          for (count = 0, count2 = 0; count < string_count(MESSAGE,"|"); count++)
          {
            if (count == 3)
            {
              memcpy(public_address,&MESSAGE[count2],strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2);
            }
            if (count == 4)
            {
              memcpy(XCASH_DPOPS_signature,&MESSAGE[count2],strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2);
            }
            count2 = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) + 1;
          }
        }
        else
        {
          VERIFY_DATA_ERROR("Could not parse the message");
        }  
      }  
    }    
  }
  else
  {    
    if (parse_json_data(MESSAGE,"public_address",public_address,sizeof(public_address)) == 0 || parse_json_data(MESSAGE,"previous_block_hash",message_previous_block_hash,sizeof(message_previous_block_hash)) == 0 || parse_json_data(MESSAGE,"current_round_part",message_current_round_part,sizeof(message_current_round_part)) == 0 || parse_json_data(MESSAGE,"current_round_part_backup_node",message_current_round_part_backup_node,sizeof(message_current_round_part_backup_node)) == 0 || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",XCASH_DPOPS_signature,sizeof(XCASH_DPOPS_signature)) == 0)
    {
      VERIFY_DATA_ERROR("Could not parse the message");
    }
  }  

  // check that the block verifier that sent the data is the correct main node
  if (strncmp(message_settings,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND",sizeof(message_settings)) == 0 && strncmp(public_address,main_nodes_list.block_producer_public_address,BUFFER_SIZE) != 0)
  {
    VERIFY_DATA_ERROR("Invalid MAIN_NODES_TO_NODES_PART_4_OF_ROUND message");
  }
  else if (strncmp(message_settings,"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_NETWORK_DATA_NODE_BLOCK_VERIFIERS_CURRENT_TIME",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"XCASH_PROOF_OF_STAKE_TEST_DATA",sizeof(message_settings)) == 0 || strncmp(message_settings,"XCASH_PROOF_OF_STAKE_TEST_DATA_2",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_NODES_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD",sizeof(message_settings)) == 0)
  {
    memset(data,0,sizeof(data));
  } 
  else
  {
    // check if the public address is in the current_block_verifiers_list struct, or the 
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (memcmp(public_address,current_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH) == 0 || memcmp(public_address,network_data_nodes_list.network_data_nodes_public_address[count],XCASH_WALLET_LENGTH) == 0)
      {
        break;
      }
    }
    if (count == BLOCK_VERIFIERS_AMOUNT)
    {
      VERIFY_DATA_ERROR("Invalid message");
    }
    memset(data,0,sizeof(data));
  }

  // verify if the previous block hash is correct
  if (strncmp(message_settings,"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST",sizeof(message_settings)) != 0 && strncmp(message_settings,"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST",sizeof(message_settings)) != 0 && strncmp(message_settings,"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES",sizeof(message_settings)) != 0 && strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",sizeof(message_settings)) != 0 && strncmp(message_settings,"XCASH_PROOF_OF_STAKE_TEST_DATA",sizeof(message_settings)) != 0 && strncmp(message_settings,"XCASH_PROOF_OF_STAKE_TEST_DATA_2",sizeof(message_settings)) != 0 && strncmp(message_settings,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF",sizeof(message_settings)) != 0 && strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE",sizeof(message_settings)) != 0 && strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE",sizeof(message_settings)) != 0 && strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE",sizeof(message_settings)) != 0 && strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE",sizeof(message_settings)) != 0 && strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE",sizeof(message_settings)) != 0 && strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",sizeof(message_settings)) != 0 && strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE",sizeof(message_settings)) != 0 && strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE",sizeof(message_settings)) != 0 && strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE",sizeof(message_settings)) != 0 && strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD",sizeof(message_settings)) != 0 && strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD",sizeof(message_settings)) != 0 && strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD",sizeof(message_settings)) != 0 && strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD",sizeof(message_settings)) != 0 && strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD",sizeof(message_settings)) != 0 && strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD",sizeof(message_settings)) != 0 && strncmp(message_settings,"NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST",sizeof(message_settings)) != 0 && strncmp(message_settings,"NETWORK_DATA_NODE_TO_NODE_SEND_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST",sizeof(message_settings)) != 0 && strncmp(message_settings,"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST",sizeof(message_settings)) != 0 && strncmp(message_settings,"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST",sizeof(message_settings)) != 0)
  {    
    if (strncmp(previous_block_hash,message_previous_block_hash,BUFFER_SIZE) != 0)
    {
      VERIFY_DATA_ERROR("Invalid previous block hash");
    }

    pthread_rwlock_rdlock(&rwlock);
    // verify if the current_round_part_backup_node
    if (VERIFY_CURRENT_ROUND_PART_AND_CURRENT_ROUND_PART_BACKUP_NODE_SETTINGS == 1 && (memcmp(current_round_part,message_current_round_part,1) != 0 || memcmp(current_round_part_backup_node,message_current_round_part_backup_node,1) != 0))
    {
      VERIFY_DATA_ERROR("Invalid current_round_part or current_round_part_backup_node");  
    }
    pthread_rwlock_unlock(&rwlock);
  }
   
  // create the message
  if (strncmp(message_settings,"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"XCASH_PROOF_OF_STAKE_TEST_DATA",sizeof(message_settings)) == 0 || strncmp(message_settings,"XCASH_PROOF_OF_STAKE_TEST_DATA_2",sizeof(message_settings)) != 0 || strncmp(message_settings,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE",sizeof(message_settings)) == 0)
  {
    if (strstr(MESSAGE,"previous_block_hash") == NULL)
    {
      message_length = strlen(MESSAGE) - 94;
      memcpy(result,MESSAGE,message_length);
    }
    else if (strstr(MESSAGE,"\"XCASH_DPOPS_signature\": \"SigV1") == NULL && strstr(MESSAGE,"|SigV1") == NULL)
    {
      message_length = strlen(MESSAGE) - 320;
      memcpy(result,MESSAGE,message_length);
      memcpy(result+message_length,"}",1);  
      if (string_replace(result,MAXIMUM_BUFFER_SIZE,"\"","\\\"") == 0)
      {
        VERIFY_DATA_ERROR("Invalid message");
      }   
    }
    else
    {      
      message_length = strlen(MESSAGE) - 125;
      memcpy(result,MESSAGE,message_length);
      memcpy(result+message_length,"}",1);  
      if (string_replace(result,MAXIMUM_BUFFER_SIZE,"\"","\\\"") == 0)
      {
        VERIFY_DATA_ERROR("Invalid message");
      }   
    }
  }
  else
  {
    if (strstr(MESSAGE,"\"XCASH_DPOPS_signature\": \"SigV1") == NULL && strstr(MESSAGE,"|SigV1") == NULL)
    {
      message_length = strlen(MESSAGE) - 320;
      memcpy(result,MESSAGE,message_length);
      memcpy(result+message_length,"}",1);  
      if (string_replace(result,MAXIMUM_BUFFER_SIZE,"\"","\\\"") == 0)
      {
        VERIFY_DATA_ERROR("Invalid message");
      }   
    }
    else
    {      
      message_length = strlen(MESSAGE) - 125;
      memcpy(result,MESSAGE,message_length);
      memcpy(result+message_length,"}",1);  
      if (string_replace(result,MAXIMUM_BUFFER_SIZE,"\"","\\\"") == 0)
      {
        VERIFY_DATA_ERROR("Invalid message");
      }   
    }
  }

  if (strstr(MESSAGE,"\"XCASH_DPOPS_signature\": \"SigV1") == NULL && strstr(MESSAGE,"|SigV1") == NULL)
  {
    // get the public key, proof and beta string
    memcpy(proof,XCASH_DPOPS_signature,VRF_PROOF_LENGTH);
    memcpy(beta_string,&XCASH_DPOPS_signature[VRF_PROOF_LENGTH],VRF_BETA_LENGTH);

    if (test_settings == 0)
    {
      for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
      {      
        if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],public_address,XCASH_WALLET_LENGTH) == 0)
        {
          memcpy(public_key,current_block_verifiers_list.block_verifiers_public_key[count],VRF_PUBLIC_KEY_LENGTH);
          break;
        }
      }

      // check if the block verifiers public key was found, and if not use the database to try to find it
      if (count == BLOCK_VERIFIERS_AMOUNT)
      {
        // create the message
        memset(data,0,sizeof(data));
        memcpy(data,"{\"public_address\":\"",19);
        memcpy(data+19,public_address,XCASH_WALLET_LENGTH);
        memcpy(data+117,"\"}",2);

        if (count_documents_in_collection(database_name,"delegates",data,1) == 1)
        {
          if (read_document_field_from_collection(database_name,"delegates",data,"public_key",public_key,1) == 0)
          {
            VERIFY_DATA_ERROR("Could not find the public key to verify the message");
          }
        }
        else
        {
          color_print(MESSAGE,"yellow");
          VERIFY_DATA_ERROR("Could not find the public key to verify the message");
        }
      }
    }
    else if (test_settings == 1)
    {
      memcpy(public_key,&secret_key[VRF_PUBLIC_KEY_LENGTH],VRF_PUBLIC_KEY_LENGTH);
    }
    
    

    // convert the public key, proof and beta string to a string
    for (count = 0, count2 = 0; count < VRF_PUBLIC_KEY_LENGTH; count2++, count += 2)
    {
      memset(data,0,sizeof(data));
      memcpy(data,&public_key[count],2);
      public_key_data[count2] = (int)strtol(data, NULL, 16);
    }
    for (count = 0, count2 = 0; count < VRF_PROOF_LENGTH; count2++, count += 2)
    {
      memset(data,0,sizeof(data));
      memcpy(data,&proof[count],2);
      proof_data[count2] = (int)strtol(data, NULL, 16);
    }
    for (count = 0, count2 = 0; count < VRF_BETA_LENGTH; count2++, count += 2)
    {
      memset(data,0,sizeof(data));
      memcpy(data,&beta_string[count],2);
      beta_string_data[count2] = (int)strtol(data, NULL, 16);
    }

    // verify the message
    if (crypto_vrf_verify((unsigned char*)beta_string_data,(const unsigned char*)public_key_data,(const unsigned char*)proof_data,(const unsigned char*)result,(unsigned long long)strlen((const char*)result)) != 0)
    {
      VERIFY_DATA_ERROR("Invalid message");
    }
  }
  else
  {
    // create the message
    message_length = strlen(result);
    memcpy(string,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"verify\",\"params\":{\"data\":\"",62);
    memcpy(string+62,result,message_length);
    memcpy(string+62+message_length,"\",\"address\":\"",13);
    memcpy(string+75+message_length,public_address,XCASH_WALLET_LENGTH);
    memcpy(string+75+message_length+XCASH_WALLET_LENGTH,"\",\"signature\":\"",15);
    memcpy(string+90+message_length+XCASH_WALLET_LENGTH,XCASH_DPOPS_signature,XCASH_SIGN_DATA_LENGTH);
    memcpy(string+90+message_length+XCASH_WALLET_LENGTH+XCASH_SIGN_DATA_LENGTH,"\"}}",3);

    memset(result,0,strnlen(result,BUFFER_SIZE));

    if (send_http_request(result,"127.0.0.1","/json_rpc",XCASH_WALLET_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,string,RECEIVE_DATA_TIMEOUT_SETTINGS,"verify data",HTTP_SETTINGS) <= 0)
    {
      VERIFY_DATA_ERROR("Could not verify the data");
    }
    
    if (parse_json_data(result,"good",data,sizeof(data)) == 0)
    {
      VERIFY_DATA_ERROR("Could not verify the data");
    }

    // check if the returned data is valid
    if (strncmp(data,"true",BUFFER_SIZE) != 0)
    {
       VERIFY_DATA_ERROR("Invalid message");
    }
  }
 
  pointer_reset_all;
  return 1;
  
  #undef VERIFY_DATA_ERROR
  #undef pointer_reset_all
}




/*
-----------------------------------------------------------------------------------------------------------
Name: validate_data
Description: Validates that only certain nodes can request certain messages
Parameters:
  message - The data
Return: 0 if the data is not validated, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int validate_data(const char* MESSAGE)
{  
  // Variables
  char data[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;
  int total_delegates = 0;

  // define macros  
  #define VALIDATE_DATA_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"validate_data",13); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  print_error_message(current_date_and_time,current_UTC_date_and_time,data); \
  return 0;

  memset(data,0,sizeof(data));

  // check if the format is correct for each message
  if (strstr(MESSAGE,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND_CREATE_NEW_BLOCK") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND_CREATE_NEW_BLOCK",sizeof(data)) != 0 || parse_json_data(MESSAGE,"block_blob",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_START_BLOCK") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_START_BLOCK",sizeof(data)) != 0 || parse_json_data(MESSAGE,"database_data",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_bytes_data_hash",data,sizeof(data)) == 0 || strlen(data) != DATA_HASH_LENGTH || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_CREATE_NEW_BLOCK") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_CREATE_NEW_BLOCK",sizeof(data)) != 0 || parse_json_data(MESSAGE,"block_blob",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK",sizeof(data)) != 0 || parse_json_data(MESSAGE,"block_blob_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_PROOF_LENGTH+VRF_BETA_LENGTH || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA",sizeof(data)) != 0 || parse_json_data(MESSAGE,"vrf_secret_key",data,sizeof(data)) == 0 || strlen(data) != VRF_SECRET_KEY_LENGTH || parse_json_data(MESSAGE,"vrf_public_key",data,sizeof(data)) == 0 || strlen(data) != VRF_PUBLIC_KEY_LENGTH || parse_json_data(MESSAGE,"random_data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"NODES_TO_NODES_VOTE_RESULTS") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"NODES_TO_NODES_VOTE_RESULTS",sizeof(data)) != 0 || parse_json_data(MESSAGE,"vote_settings",data,sizeof(data)) == 0 || (memcmp(data,"valid",5) != 0 && memcmp(data,"invalid",7) != 0) || parse_json_data(MESSAGE,"vote_data",data,sizeof(data)) == 0 || strlen(data) != DATA_HASH_LENGTH || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE",sizeof(data)) != 0 || parse_json_data(MESSAGE,"block_blob_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_PROOF_LENGTH+VRF_BETA_LENGTH || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS",sizeof(data)) != 0 || parse_json_data(MESSAGE,"public_address_that_created_the_reserve_proof",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) || parse_json_data(MESSAGE,"reserve_proof",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST",sizeof(data)) != 0)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST") != NULL)
  {   
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST",sizeof(data)) != 0)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"NETWORK_DATA_NODE_TO_NODE_SEND_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"NETWORK_DATA_NODE_TO_NODE_SEND_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST",sizeof(data)) != 0 || parse_json_data(MESSAGE,"previous_block_verifiers_name_list",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"previous_block_verifiers_public_address_list",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"previous_block_verifiers_IP_address_list",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"current_block_verifiers_name_list",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"current_block_verifiers_public_address_list",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"current_block_verifiers_IP_address_list",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"next_block_verifiers_name_list",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"next_block_verifiers_public_address_list",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"next_block_verifiers_IP_address_list",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST",sizeof(data)) != 0 || parse_json_data(MESSAGE,"block_verifiers_public_address_list",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"block_verifiers_IP_address_list",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"BLOCK_VERIFIERS_TO_NETWORK_DATA_NODE_BLOCK_VERIFIERS_CURRENT_TIME") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"BLOCK_VERIFIERS_TO_NETWORK_DATA_NODE_BLOCK_VERIFIERS_CURRENT_TIME",sizeof(data)) != 0 || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_BLOCK_VERIFIERS_CURRENT_TIME") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_BLOCK_VERIFIERS_CURRENT_TIME",sizeof(data)) != 0 || parse_json_data(MESSAGE,"current_time",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATABASE_HASH") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATABASE_HASH",sizeof(data)) != 0 || parse_json_data(MESSAGE,"block_height",data,sizeof(data)) == 0)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"NODE_TO_BLOCK_VERIFIERS_CHECK_IF_CURRENT_BLOCK_VERIFIER") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"NODE_TO_BLOCK_VERIFIERS_CHECK_IF_CURRENT_BLOCK_VERIFIER",sizeof(data)) != 0)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES",sizeof(data)) != 0 || parse_json_data(MESSAGE,"reserve_bytes",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",sizeof(data)) != 0)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"BLOCK_VERIFIERS_TO_NODES_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"BLOCK_VERIFIERS_TO_NODES_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD",sizeof(data)) != 0 || parse_json_data(MESSAGE,"data_hash",data,sizeof(data)) == 0 || strlen(data) != DATA_HASH_LENGTH || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"NETWORK_DATA_NODES_TO_NETWORK_DATA_NODES_DATABASE_SYNC_CHECK") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"NETWORK_DATA_NODES_TO_NETWORK_DATA_NODES_DATABASE_SYNC_CHECK",sizeof(data)) != 0 || parse_json_data(MESSAGE,"data_hash",data,sizeof(data)) == 0 || strlen(data) != DATA_HASH_LENGTH || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",sizeof(data)) != 0 || parse_json_data(MESSAGE,"reserve_bytes_data_hash",data,sizeof(data)) == 0 || strlen(data) != DATA_HASH_LENGTH || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD",sizeof(data)) != 0 || parse_json_data(MESSAGE,"reserve_bytes_database",data,sizeof(data)) == 0 || (memcmp(data,"true",4) != 0 && memcmp(data,"false",5) != 0) || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD",sizeof(data)) != 0 || parse_json_data(MESSAGE,"reserve_bytes_database",data,sizeof(data)) == 0 || (memcmp(data,"true",4) != 0 && memcmp(data,"false",5) != 0) || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE",sizeof(data)) != 0 || parse_json_data(MESSAGE,"file",data,sizeof(data)) == 0 || strstr(data,"reserve_bytes_") == NULL || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD",sizeof(data)) != 0 || parse_json_data(MESSAGE,"reserve_bytes_database",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE",sizeof(data)) != 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash",data,sizeof(data)) == 0 || strlen(data) != DATA_HASH_LENGTH || parse_json_data(MESSAGE,"reserve_proofs_data_hash_1",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_2",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_3",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_4",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_5",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_6",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_7",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_8",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_9",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_10",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_11",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_12",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_13",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_14",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_15",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_16",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_17",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_18",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_19",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_20",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_20",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_21",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_22",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_23",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_24",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_25",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_26",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_27",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_28",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_29",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_30",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_31",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_32",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_33",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_34",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_35",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_36",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_37",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_38",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_39",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_40",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_41",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_42",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_43",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_44",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_45",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_46",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_47",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_48",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_49",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_data_hash_50",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD",sizeof(data)) != 0 || parse_json_data(MESSAGE,"reserve_proofs_database",data,sizeof(data)) == 0 || (memcmp(data,"true",4) != 0 && memcmp(data,"false",5) != 0) || parse_json_data(MESSAGE,"reserve_proofs_database_1",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_2",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_3",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_4",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_5",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_6",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_7",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_8",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_9",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_10",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_11",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_12",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_13",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_14",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_15",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_16",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_17",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_18",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_19",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_20",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_20",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_21",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_22",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_23",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_24",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_25",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_26",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_27",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_28",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_29",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_30",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_31",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_32",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_33",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_34",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_35",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_36",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_37",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_38",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_39",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_40",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_41",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_42",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_43",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_44",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_45",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_46",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_47",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_48",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_49",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"reserve_proofs_database_50",data,sizeof(data)) == 0 || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE",sizeof(data)) != 0 || parse_json_data(MESSAGE,"file",data,sizeof(data)) == 0 || strstr(data,"reserve_proofs_") == NULL || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD",sizeof(data)) != 0 || parse_json_data(MESSAGE,"reserve_proofs_database",data,sizeof(data)) == 0 || (memcmp(data,"true",4) != 0 && memcmp(data,"false",5) != 0) || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE",sizeof(data)) != 0 || parse_json_data(MESSAGE,"data_hash",data,sizeof(data)) == 0 || strlen(data) != DATA_HASH_LENGTH || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD",sizeof(data)) != 0 || parse_json_data(MESSAGE,"delegates_database",data,sizeof(data)) == 0 || (memcmp(data,"true",4) != 0 && memcmp(data,"false",5) != 0) || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE",sizeof(data)) != 0 || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD",sizeof(data)) != 0 || parse_json_data(MESSAGE,"delegates_database",data,sizeof(data)) == 0 || (memcmp(data,"true",4) != 0 && memcmp(data,"false",5) != 0) || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE",sizeof(data)) != 0 || parse_json_data(MESSAGE,"data_hash",data,sizeof(data)) == 0 || strlen(data) != DATA_HASH_LENGTH || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD",sizeof(data)) != 0 || parse_json_data(MESSAGE,"statistics_database",data,sizeof(data)) == 0 || (memcmp(data,"true",4) != 0 && memcmp(data,"false",5) != 0) || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE",sizeof(data)) != 0 || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD",sizeof(data)) != 0 || parse_json_data(MESSAGE,"statistics_database",data,sizeof(data)) == 0 || (memcmp(data,"true",4) != 0 && memcmp(data,"false",5) != 0) || parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 0 || strlen(data) != XCASH_WALLET_LENGTH || memcmp(data,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || parse_json_data(MESSAGE,"previous_block_hash",data,sizeof(data)) == 0 || strlen(data) != BLOCK_HASH_LENGTH || parse_json_data(MESSAGE,"current_round_part",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"current_round_part_backup_node",data,sizeof(data)) == 0 || strlen(data) != 1 || parse_json_data(MESSAGE,"data",data,sizeof(data)) == 0 || strlen(data) != RANDOM_STRING_LENGTH || parse_json_data(MESSAGE,"XCASH_DPOPS_signature",data,sizeof(data)) == 0 || strlen(data) != VRF_BETA_LENGTH+VRF_PROOF_LENGTH)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"XCASH_PROOF_OF_STAKE_TEST_DATA") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"XCASH_PROOF_OF_STAKE_TEST_DATA",sizeof(data)) != 0)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"XCASH_PROOF_OF_STAKE_TEST_DATA_2") != NULL)
  {
    if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 0 || strncmp(data,"XCASH_PROOF_OF_STAKE_TEST_DATA_2",sizeof(data)) != 0)
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (strstr(MESSAGE,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF") != NULL || strstr(MESSAGE,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE") != NULL || strstr(MESSAGE,"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE") != NULL || strstr(MESSAGE,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE") != NULL || strstr(MESSAGE,"GET /delegateswebsitegetstatistics HTTP/") != NULL || strstr(MESSAGE,"GET /getdelegates HTTP/") != NULL || memcmp(MESSAGE,"GET /getdelegatesstatistics?parameter1=",39) != 0 || memcmp(MESSAGE,"GET /getdelegatesinformation?parameter1=",40) != 0 || memcmp(MESSAGE,"GET /getdelegatesstatistics?parameter1=",39) != 0 || memcmp(MESSAGE,"GET /getdelegatesstatistics?parameter1=",39) != 0 || strstr(MESSAGE,"") != NULL || strstr(MESSAGE,"") != NULL || strstr(MESSAGE,"") != NULL || strstr(MESSAGE,"") != NULL || strstr(MESSAGE,"") != NULL || strstr(MESSAGE,"") != NULL || strstr(MESSAGE,"") != NULL || strstr(MESSAGE,"") != NULL)
  {
    return 1;
  }
  else if (delegates_website == 1)
  {
    if (memcmp(MESSAGE,"GET /getdelegatesstatistics?parameter1=",39) == 0)
    {
      if (strlen(&MESSAGE[39]) > XCASH_WALLET_LENGTH)
      {
        VALIDATE_DATA_ERROR("Invalid message");
      }      
    }
    else if (memcmp(MESSAGE,"GET /getdelegatesinformation?parameter1=",40) == 0)
    {
      if (strlen(&MESSAGE[40]) > XCASH_WALLET_LENGTH)
      {
        VALIDATE_DATA_ERROR("Invalid message");
      } 
    }
    else if (memcmp(MESSAGE,"GET /getdelegatesvoterslist?parameter1=",39) == 0)
    {
      if (strlen(&MESSAGE[39]) > XCASH_WALLET_LENGTH)
      {
        VALIDATE_DATA_ERROR("Invalid message");
      } 
    }
    else if (memcmp(MESSAGE,"GET /getroundstatistics?parameter1=",35) == 0)
    {
      for (count = 35; count < (int)strlen(MESSAGE); count++)
      {
        if (memcmp(&MESSAGE[count],"0",1) != 0 && memcmp(&MESSAGE[count],"1",1) != 0 && memcmp(&MESSAGE[count],"2",1) != 0 && memcmp(&MESSAGE[count],"3",1) != 0 && memcmp(&MESSAGE[count],"4",1) != 0 && memcmp(&MESSAGE[count],"5",1) != 0 && memcmp(&MESSAGE[count],"6",1) != 0 && memcmp(&MESSAGE[count],"7",1) != 0 && memcmp(&MESSAGE[count],"8",1) != 0 && memcmp(&MESSAGE[count],"9",1) != 0)
        {
          VALIDATE_DATA_ERROR("Invalid message");
        }
      }
    }
    else
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else if (shared_delegates_website == 1)
  {
    if (memcmp(MESSAGE,"GET /getpublicaddressinformation?public_address=",48) == 0)
    {
      if (strlen(&MESSAGE[48]) != XCASH_WALLET_LENGTH || memcmp(&MESSAGE[48],XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0)
      {
        VALIDATE_DATA_ERROR("Invalid message");
      } 
    }
    else if (memcmp(MESSAGE,"GET /getpublicaddresspaymentinformation?public_address=",55) == 0)
    {
      if (strlen(&MESSAGE[55]) != XCASH_WALLET_LENGTH || memcmp(&MESSAGE[55],XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0)
      {
        VALIDATE_DATA_ERROR("Invalid message");
      }
    }
    else if (memcmp(MESSAGE,"GET /getdelegatesvoterslist?parameter1=",39) == 0)
    {
      if (strlen(&MESSAGE[39]) > XCASH_WALLET_LENGTH)
      {
        VALIDATE_DATA_ERROR("Invalid message");
      } 
    }
    else
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }
  }
  else
  {
    VALIDATE_DATA_ERROR("Invalid message");
  }
  
  // dont check if the message came from a valid user, if the message is from the test wallet, or your own wallet
  if (strstr(MESSAGE,"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST") != NULL || strstr(MESSAGE,"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST") != NULL || strstr(MESSAGE,"NODE_TO_BLOCK_VERIFIERS_CHECK_IF_CURRENT_BLOCK_VERIFIER") != NULL || strstr(MESSAGE,"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATABASE_HASH") != NULL || strstr(MESSAGE,"NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE") != NULL || (parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 1 && memcmp(data,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 1 && memcmp(data,TEST_WALLET,XCASH_WALLET_LENGTH) == 0))
  {
    return 1;
  }
  
  // check if the message came from a valid user
  if (parse_json_data(MESSAGE,"message_settings",data,sizeof(data)) == 1 && strncmp(data,"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST",BUFFER_SIZE) != 0 && strncmp(data,"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST",BUFFER_SIZE) != 0 && strncmp(data,"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATABASE_HASH",BUFFER_SIZE) != 0 && strncmp(data,"NODE_TO_BLOCK_VERIFIERS_CHECK_IF_CURRENT_BLOCK_VERIFIER",BUFFER_SIZE) != 0 && strncmp(data,"XCASH_PROOF_OF_STAKE_TEST_DATA",BUFFER_SIZE) != 0 && strncmp(data,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF",BUFFER_SIZE) != 0 && strncmp(data,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE",BUFFER_SIZE) != 0 && strncmp(data,"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE",BUFFER_SIZE) != 0 && strncmp(data,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE",BUFFER_SIZE) != 0)
  {
    // get the delegate amount
    for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++)
    {
      if (strlen(current_block_verifiers_list.block_verifiers_public_address[count]) != XCASH_WALLET_LENGTH)
      {
        total_delegates = count;
        break;
      }
    }

    if (parse_json_data(MESSAGE,"public_address",data,sizeof(data)) == 1)
    {
      for (count = 0; count < total_delegates; count++)
      {
        if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],data,XCASH_WALLET_LENGTH) == 0)
        {
          break;
        }
      }
      if (count == total_delegates)
      {
        VALIDATE_DATA_ERROR("Invalid message");
      }
    }
    else
    {
      VALIDATE_DATA_ERROR("Invalid message");
    }    
  }

  return 1;
  
  #undef VALIDATE_DATA_ERROR
}