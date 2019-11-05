#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "variables.h"

#include "network_daemon_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "string_functions.h"

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
  unsigned char proof_data[crypto_vrf_PROOFBYTES+1];
  unsigned char beta_string_data[crypto_vrf_OUTPUTBYTES+1];
  char* result = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char)); // 50 MB
  char* string = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char)); // 50 MB
  size_t count;
  size_t count2;
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
  memset(proof_data,0,sizeof(proof_data));
  memset(beta_string_data,0,sizeof(beta_string_data));  
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
  memcpy(result+strlen(result)," \"public_address\": \"",20);
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
  
  if (strstr(message,"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST") == NULL && strstr(message,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF") == NULL && strstr(message,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE") == NULL && strstr(message,"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE") == NULL && strstr(message,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE") == NULL)
  {
    // sign data
    if (crypto_vrf_prove((unsigned char*)proof_data,(const unsigned char*)secret_key_data,(const unsigned char*)result,(unsigned long long)strlen((const char*)result)) != 0 || crypto_vrf_proof_to_hash((unsigned char*)beta_string_data,(const unsigned char*)proof_data) != 0)
    {
      SIGN_DATA_ERROR("Could not create the message");
    }

    // convert the data to a string
    for (count2 = 0, count = 0; count2 < crypto_vrf_PROOFBYTES; count2++, count += 2)
    {
      snprintf(proof+count,BUFFER_SIZE_NETWORK_BLOCK_DATA-1,"%02x",proof_data[count2] & 0xFF);
    }
    for (count2 = 0, count = 0; count2 < crypto_vrf_OUTPUTBYTES; count2++, count += 2)
    {
      snprintf(beta_string+count,BUFFER_SIZE_NETWORK_BLOCK_DATA-1,"%02x",beta_string_data[count2] & 0xFF);
    } 

     //fprintf(stderr,"message = \n%s\n\nbeta = \n%s\n\n proof = \n%s\n\n",result,proof,beta_string);

    pthread_rwlock_rdlock(&rwlock);
    // create the message  
    memcpy(message+strlen(message)-1," \"public_address\": \"",20);
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
    memcpy(message+strlen(message)-1," \"public_address\": \"",20);
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
  int settings = 0;

  // define macros
  #define VERIFY_DATA_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"verify_data",11); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  print_error_message(current_date_and_time,current_UTC_date_and_time,data); \
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

  if (strncmp(message_settings,"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"XCASH_PROOF_OF_STAKE_TEST_DATA",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE",sizeof(message_settings)) == 0)
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
        if (string_count(MESSAGE,"|") == 5)
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
        if (string_count(MESSAGE,"|") == 5)
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
      else if (strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE",sizeof(message_settings)) == 0)
      {
        if (string_count(MESSAGE,"|") == 3)
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
        if (string_count(MESSAGE,"|") == 5)
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
  else if (strncmp(message_settings,"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_NETWORK_DATA_NODE_BLOCK_VERIFIERS_CURRENT_TIME",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"XCASH_PROOF_OF_STAKE_TEST_DATA",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_NODES_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD",sizeof(message_settings)) == 0)
  {
    memset(data,0,sizeof(data));
  } 
  else
  {
    // check if the public address is in the current_block_verifiers_list struct
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (memcmp(public_address,current_block_verifiers_list.block_verifiers_public_address[count],XCASH_WALLET_LENGTH) == 0)
      {
        settings = 1;
      }
    }
    if (settings == 0)
    {
      VERIFY_DATA_ERROR("Invalid message1");
    }
    memset(data,0,sizeof(data));
  }

  // verify if the previous block hash is correct
  if (strncmp(message_settings,"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST",sizeof(message_settings)) != 0 && strncmp(message_settings,"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST",sizeof(message_settings)) != 0 && strncmp(message_settings,"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES",sizeof(message_settings)) != 0 && strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",sizeof(message_settings)) != 0 && strncmp(message_settings,"XCASH_PROOF_OF_STAKE_TEST_DATA",sizeof(message_settings)) != 0 && strncmp(message_settings,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF",sizeof(message_settings)) != 0 && strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE",sizeof(message_settings)) != 0 && strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE",sizeof(message_settings)) != 0 && strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE",sizeof(message_settings)) != 0)
  {    
    if (strncmp(previous_block_hash,message_previous_block_hash,BUFFER_SIZE) != 0)
    {
      VERIFY_DATA_ERROR("Invalid previous block hash");
    }

    pthread_rwlock_rdlock(&rwlock);
    // verify if the current_round_part_backup_node
    if (VERIFY_CURRENT_ROUND_PART_AND_CURRENT_ROUND_PART_BACKUP_NODE_SETTINGS == 1)
    {
      if (memcmp(current_round_part,message_current_round_part,1) != 0 || memcmp(current_round_part_backup_node,message_current_round_part_backup_node,1) != 0)
      {
        VERIFY_DATA_ERROR("Invalid current_round_part or current_round_part_backup_node");
      }
    }
    pthread_rwlock_unlock(&rwlock);
  }
   
  // create the message
  if (strncmp(message_settings,"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"XCASH_PROOF_OF_STAKE_TEST_DATA",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE",sizeof(message_settings)) == 0 || strncmp(message_settings,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE",sizeof(message_settings)) == 0)
  {
    if (strstr(MESSAGE,"previous_block_hash") == NULL)
    {
      message_length = strlen(MESSAGE) - 94;
      memcpy(result,MESSAGE,message_length);
    }
    else if (strstr(MESSAGE,"\"XCASH_DPOPS_signature\": \"SigV1") == NULL)
    {
      message_length = strlen(MESSAGE) - 320;
      memcpy(result,MESSAGE,message_length);
      memcpy(result+message_length,"}",1);  
      if (string_replace(result,MAXIMUM_BUFFER_SIZE,"\"","\\\"") == 0)
      {
        VERIFY_DATA_ERROR("Invalid message2");
      }   
    }
    else
    {      
      message_length = strlen(MESSAGE) - 125;
      memcpy(result,MESSAGE,message_length);
      memcpy(result+message_length,"}",1);  
      if (string_replace(result,MAXIMUM_BUFFER_SIZE,"\"","\\\"") == 0)
      {
        VERIFY_DATA_ERROR("Invalid message3");
      }   
    }
  }
  else
  {
    if (strstr(MESSAGE,"\"XCASH_DPOPS_signature\": \"SigV1") == NULL)
    {
      message_length = strlen(MESSAGE) - 320;
      memcpy(result,MESSAGE,message_length);
      memcpy(result+message_length,"}",1);  
      if (string_replace(result,MAXIMUM_BUFFER_SIZE,"\"","\\\"") == 0)
      {
        VERIFY_DATA_ERROR("Invalid message4");
      }   
    }
    else
    {      
      message_length = strlen(MESSAGE) - 125;
      memcpy(result,MESSAGE,message_length);
      memcpy(result+message_length,"}",1);  
      if (string_replace(result,MAXIMUM_BUFFER_SIZE,"\"","\\\"") == 0)
      {
        VERIFY_DATA_ERROR("Invalid message5");
      }   
    }
  }

  if (strstr(MESSAGE,"\"XCASH_DPOPS_signature\": \"SigV1") == NULL)
  {
    // get the public key, proof and beta string
    memcpy(proof,XCASH_DPOPS_signature,VRF_PROOF_LENGTH);
    memcpy(beta_string,&XCASH_DPOPS_signature[VRF_PROOF_LENGTH],VRF_BETA_LENGTH);
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],public_address,XCASH_WALLET_LENGTH) == 0)
      {
        memcpy(public_key,current_block_verifiers_list.block_verifiers_public_key,VRF_PUBLIC_KEY_LENGTH);
      }
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

    //fprintf(stderr,"message = \n%s\n\npublic key = \n%s\n\n beta = \n%s\n\n proof = \n%s\n\n",result,public_key,proof,beta_string);

    // verify the message
    if (crypto_vrf_verify((unsigned char*)beta_string_data,(const unsigned char*)public_key_data,(const unsigned char*)proof_data,(const unsigned char*)result,(unsigned long long)strlen((const char*)result)) != 0)
    {
      VERIFY_DATA_ERROR("Invalid message6");
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
       VERIFY_DATA_ERROR("Invalid message7");
    }
  }
 
  pointer_reset_all;
  return 1;
  
  #undef VERIFY_DATA_ERROR
  #undef pointer_reset_all
}