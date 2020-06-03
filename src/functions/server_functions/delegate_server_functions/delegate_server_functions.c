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
#include "define_macros_test.h"

#include "blockchain_functions.h"
#include "block_verifiers_server_functions.h"
#include "block_verifiers_synchronize_server_functions.h"
#include "block_verifiers_synchronize_functions.h"
#include "database_functions.h"
#include "shared_delegate_website_thread_server_functions.h"
#include "insert_database_functions.h"
#include "read_database_functions.h"
#include "update_database_functions.h"
#include "delete_database_functions.h"
#include "count_database_functions.h"
#include "delegate_server_functions.h"
#include "network_daemon_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "network_wallet_functions.h"
#include "server_functions.h"
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
Name: block_verifiers_add_reserve_proof_check_if_data_is_valid
Description: Checks to see if the vote data is valid
Parameters:
  MESSAGE - The message
  struct reserve_proof
    block_verifier_public_address - The block verifier that added the reserve proof
    public_address_created_reserve_proof - The public address that created the reserve proof
    public_address_voted_for - The public address that the reserve proof is voted for
    reserve_proof_amount - The reserve proof amount
    reserve_proof - The reserve proof
Return: 0 if an error has occured or if the vote is invalid, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int block_verifiers_add_reserve_proof_check_if_data_is_valid(const char* MESSAGE, struct reserve_proof* reserve_proof)
{
  // Variables
  char data[SMALL_BUFFER_SIZE+BUFFER_SIZE_RESERVE_PROOF];
  char data2[SMALL_BUFFER_SIZE+BUFFER_SIZE_RESERVE_PROOF];
  char data3[SMALL_BUFFER_SIZE+BUFFER_SIZE_RESERVE_PROOF];
  int count;
  int count2;
  size_t reserve_proof_amount;
  size_t data_size;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  // verify the message  
  if (verify_data(MESSAGE,0) == 0 || string_count(MESSAGE,"|") != VOTE_PARAMETER_AMOUNT || check_for_invalid_strings(MESSAGE) == 0)
  { 
    return 0;
  }
    
  // parse the message
  for (count = 0, count2 = 0; count < VOTE_PARAMETER_AMOUNT; count++)
  {
    if (count == 1)
    {
      if ((data_size = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2) >= sizeof(data))
      {
        return 2;
      }
      memcpy(data,&MESSAGE[count2],data_size);
    }
    if (count == 2)
    {
      if ((data_size = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2) >= BUFFER_SIZE_RESERVE_PROOF)
      {
        return 2;
      }
      memcpy(reserve_proof->reserve_proof,&MESSAGE[count2],data_size);
    }
    if (count == 3)
    {
      if ((data_size = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2) != XCASH_WALLET_LENGTH)
      {
        return 2;
      }
      memcpy(reserve_proof->public_address_created_reserve_proof,&MESSAGE[count2],data_size);
    }
    count2 = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) + 1;
  }

  // check if they have voted using the delegates name or delegates public address
  if (strlen(data) != XCASH_WALLET_LENGTH)
  {
    // create the message
    memcpy(data2,"{\"delegate_name\":\"",18);
    memcpy(data2+18,data,strnlen(data,sizeof(data2)));
    memcpy(data2+strlen(data2),"\"}",2);

    // get the delegates public address
    if (read_document_field_from_collection(database_name,"delegates",data2,"public_address",data3) == 0)
    {
      return 3;
    }

    memset(data,0,sizeof(data));
    memcpy(data,data3,XCASH_WALLET_LENGTH);
    memset(data3,0,sizeof(data3));
  }

  memcpy(reserve_proof->public_address_voted_for,data,XCASH_WALLET_LENGTH);

  // check if the delegate has the maximum amount of voters
  if (get_delegates_total_voters_count(reserve_proof->public_address_voted_for) >= MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE)
  {
    return 4;
  }

  // check if the data is valid
  if (strlen(reserve_proof->public_address_voted_for) != XCASH_WALLET_LENGTH || strncmp(reserve_proof->public_address_voted_for,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || strlen(reserve_proof->public_address_created_reserve_proof) != XCASH_WALLET_LENGTH || strncmp(reserve_proof->public_address_created_reserve_proof,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || strlen(reserve_proof->reserve_proof) > BUFFER_SIZE_RESERVE_PROOF)
  {
    return 5;
  }

  // check if the reserve proof is valid and the spent amount is 0
  memset(data2,0,sizeof(data2));
  if (check_reserve_proofs(data2,reserve_proof->public_address_created_reserve_proof,reserve_proof->reserve_proof) == 0)
  {
    return 6;
  }

  // check if the reserve proof is greater than or equal to the minimum reserve proof amount
  sscanf(data2,"%zu", &reserve_proof_amount);
  if (test_settings == 0 && reserve_proof_amount < MINIMUM_AMOUNT_RESERVE_PROOF)
  {
    return 7;
  }

  memset(reserve_proof->reserve_proof_amount,0,sizeof(reserve_proof->reserve_proof_amount));
  memcpy(reserve_proof->reserve_proof_amount,data2,strnlen(data2,sizeof(reserve_proof->reserve_proof_amount)));

  // check if the same reserve proof is already in the database
  memset(data,0,sizeof(data));
  memcpy(data,"{\"reserve_proof\":\"",18);
  memcpy(data+18,reserve_proof->reserve_proof,strnlen(reserve_proof->reserve_proof,sizeof(data)));
  memcpy(data+strlen(data),"\"}",2);

  for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
  {
    memset(data2,0,sizeof(data2));
    memcpy(data2,"reserve_proofs_",15);
    snprintf(data2+15,sizeof(data2)-16,"%d",count);

    // check if the reserve proof is in the database
    if (count_documents_in_collection(database_name,data2,data) > 0)
    {      
      return 8;
    }
  }
  RESET_ERROR_MESSAGES;
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: add_reserve_proof_remove_previous_vote
Description: Checks if the public address that created the reserve proof has any other votes in the database.
If they do it removes the reserve proofs from the database and adjusts the delegates total_vote_count
Paramters:
  PUBLIC_ADDRESS_CREATE_RESERVE_PROOF_DATA - The public address that created the reserve proof data
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int add_reserve_proof_remove_previous_vote(const char* PUBLIC_ADDRESS_CREATE_RESERVE_PROOF_DATA)
{
  // Variables
  char data[SMALL_BUFFER_SIZE+BUFFER_SIZE_RESERVE_PROOF];
  char data2[SMALL_BUFFER_SIZE+BUFFER_SIZE_RESERVE_PROOF];
  char data3[SMALL_BUFFER_SIZE+BUFFER_SIZE_RESERVE_PROOF];
  char message[SMALL_BUFFER_SIZE+BUFFER_SIZE_RESERVE_PROOF];
  char message2[SMALL_BUFFER_SIZE+BUFFER_SIZE_RESERVE_PROOF];
  int count;
  int count2;
  int count3;
  size_t reserve_proof_amount;
  size_t total_vote_count;

  // remove any reserve proofs that were created by the public address and adjust the delegates total_vote_count
  for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
  {
    memset(data,0,sizeof(data));
    memcpy(data,"reserve_proofs_",15);
    snprintf(data+15,sizeof(data)-16,"%d",count);
    count2 = count_documents_in_collection(database_name,data,PUBLIC_ADDRESS_CREATE_RESERVE_PROOF_DATA);
    if (count2 <= 0)
    {
      continue;
    }
    for (count3 = 0; count3 < count2; count3++)
    {
      memset(data2,0,sizeof(data2));
      memset(data2,0,sizeof(data3));

      // get the reserve proof amount, and the public address voted for
      if (read_document_field_from_collection(database_name,data,PUBLIC_ADDRESS_CREATE_RESERVE_PROOF_DATA,"public_address_voted_for",data2) == 0 || read_document_field_from_collection(database_name,data,PUBLIC_ADDRESS_CREATE_RESERVE_PROOF_DATA,"total",data3) == 0)
      {
        return 0;
      }

      // remove the reserve proof
      if (delete_document_from_collection(database_name,data,PUBLIC_ADDRESS_CREATE_RESERVE_PROOF_DATA) == 0)
      {
        return 0;
      }

      // adjust the delegates total vote count
      memset(message,0,sizeof(message));
      memset(message2,0,sizeof(message2));

      // create the message
      memcpy(message,"{\"public_address\":\"",19);
      memcpy(message+19,data2,XCASH_WALLET_LENGTH);
      memcpy(message+strlen(message),"\"}",2);

      if (read_document_field_from_collection(database_name,"delegates",message,"total_vote_count",message2) == 0)
      {
        return 0;
      }

      sscanf(data3,"%zu", &reserve_proof_amount);
      sscanf(message2,"%zu", &total_vote_count);

      // check to make sure this does not underflow
      if ((reserve_proof_amount > 0) && (total_vote_count < 0 + reserve_proof_amount))
      {
        return 0;
      }
      total_vote_count -= reserve_proof_amount;

      // create the message
      memset(data3,0,sizeof(data3));
      snprintf(data3,sizeof(data3)-1,"%zu",total_vote_count);
      memset(message2,0,sizeof(message2));
      memcpy(message2,"{\"total_vote_count\":\"",21);
      memcpy(message2+21,data3,strnlen(data3,sizeof(message2)));
      memcpy(message2+strlen(message2),"\"}",2);

      if (update_document_from_collection(database_name,"delegates",message,message2) == 0)
      {
        return 0;
      }
    }
  }
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_node_to_block_verifiers_add_reserve_proof
Description: Runs the code when the server receives the NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  MESSAGE - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_node_to_block_verifiers_add_reserve_proof(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char message[SMALL_BUFFER_SIZE+BUFFER_SIZE_RESERVE_PROOF];
  char message2[SMALL_BUFFER_SIZE+BUFFER_SIZE_RESERVE_PROOF];
  char data[SMALL_BUFFER_SIZE+BUFFER_SIZE_RESERVE_PROOF];
  char data2[SMALL_BUFFER_SIZE+BUFFER_SIZE_RESERVE_PROOF];
  char data3[SMALL_BUFFER_SIZE+BUFFER_SIZE_RESERVE_PROOF];
  struct reserve_proof reserve_proof;
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  size_t count;
  size_t count2;
  int counter;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR(settings) \
  if (debug_settings == 1) \
  { \
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_node_to_block_verifiers_add_reserve_proof",68); \
    memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
    error_message.total++; \
  } \
  send_data(CLIENT_SOCKET,(unsigned char*)settings,0,0,""); \
  return 0;

  // initialize the reserve_proof struct
  memset(reserve_proof.block_verifier_public_address,0,sizeof(reserve_proof.block_verifier_public_address));
  memset(reserve_proof.public_address_created_reserve_proof,0,sizeof(reserve_proof.public_address_created_reserve_proof));
  memset(reserve_proof.public_address_voted_for,0,sizeof(reserve_proof.public_address_voted_for));
  memset(reserve_proof.reserve_proof_amount,0,sizeof(reserve_proof.reserve_proof_amount));
  memset(reserve_proof.reserve_proof,0,sizeof(reserve_proof.reserve_proof));

  memset(message,0,sizeof(message));
  memset(message2,0,sizeof(message2));
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  // get the current time
  get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);

  // check if it is valid to add a reserve proof to the invalid_reserve_proofs struct
  if (test_settings == 0 && current_UTC_date_and_time.tm_min != 2 && current_UTC_date_and_time.tm_min != 3)
  {
    send_data(CLIENT_SOCKET,(unsigned char*)"Invalid vote time\nValid times are the second and third minute of each hour}",0,0,"");
    return 0;
  }

  // check if the data is valid and parse the message
  if ((counter = block_verifiers_add_reserve_proof_check_if_data_is_valid(MESSAGE,&reserve_proof)) != 1)
  {
    if (counter == 0)
    {
      SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("Could not verify the message}");
    }
    else if (counter == 2)
    {
      SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("Could not parse the message}");
    }
    else if (counter == 3)
    {
      SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("The delegate voted for is invalid}");
    }
    else if (counter == 4)
    {
      SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("The delegate voted for has the maximum voters}");
    }
    else if (counter == 5)
    {
      SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("The data is invalid}");
    }
    else if (counter == 6)
    {
      SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("The reserve proof is invalid}");
    }
    else if (counter == 7)
    {
      SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("The reserve proof is below the minimum amount}");
    }
    else if (counter == 8)
    {
      SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("The reserve proof is already in the database}");
    }
  }

  // create the message
  memcpy(data,"{\"public_address_created_reserve_proof\":\"",41);
  memcpy(data+41,reserve_proof.public_address_created_reserve_proof,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\"}",2);

  // remove any reserve proof created by the public address to cancel there vote, and adjust the delegates balance
  if (add_reserve_proof_remove_previous_vote((const char*)data) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("Could not remove the previous vote}");
  }

  // create the message
  memset(data,0,sizeof(data));
  memcpy(data,"{\"public_address_created_reserve_proof\":\"",41);
  memcpy(data+strlen(data),reserve_proof.public_address_created_reserve_proof,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\",\"public_address_voted_for\":\"",30);
  memcpy(data+strlen(data),reserve_proof.public_address_voted_for,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\",\"total\":\"",11);
  memcpy(data+strlen(data),reserve_proof.reserve_proof_amount,strnlen(reserve_proof.reserve_proof_amount,sizeof(data)));
  memcpy(data+strlen(data),"\",\"reserve_proof\":\"",19);
  memcpy(data+strlen(data),reserve_proof.reserve_proof,strnlen(reserve_proof.reserve_proof,sizeof(data)));
  memcpy(data+strlen(data),"\"}",2);

  // add the reserve proof to the database
  for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
  {
    memset(data3,0,sizeof(data3));
    memcpy(data3,"reserve_proofs_",15);
    snprintf(data3+15,sizeof(data3)-16,"%zu",count);
    if (count_documents_in_collection(database_name,data3,data) < (MAXIMUM_INVALID_RESERVE_PROOFS / TOTAL_RESERVE_PROOFS_DATABASES))
    {
      if (insert_document_into_collection_json(database_name,data3,data) == 1)
      {        
        break;
      }
      else
      {
        SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("The vote could not be added to the database}");
      }
    }
  }
  RESET_ERROR_MESSAGES;

  // get the delegates current total vote count
  memset(data2,0,sizeof(data2));
  memcpy(data2,"{\"public_address\":\"",19);
  memcpy(data2+strlen(data2),reserve_proof.public_address_voted_for,XCASH_WALLET_LENGTH);
  memcpy(data2+strlen(data2),"\"}",2);

  if (read_document_field_from_collection(database_name,"delegates",data2,"total_vote_count",message2) == 0)
  {
    // delete the reserve proof, since it could not update the delegates total_vote_count
    memset(data,0,sizeof(data));
    memcpy(data,"{\"public_address_created_reserve_proof\":\"",41);
    memcpy(data+strlen(data),reserve_proof.public_address_created_reserve_proof,XCASH_WALLET_LENGTH);
    memcpy(data+strlen(data),"\"}",2);
    delete_document_from_collection(database_name,data3,data);
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("Could not get the voted for delegates total votes}");
  }

  sscanf(message2,"%zu", &count);
  sscanf(reserve_proof.reserve_proof_amount,"%zu", &count2);
  count+= count2;
  memset(message2,0,sizeof(message2));
  snprintf(message2,sizeof(message2)-1,"%zu",count);  

  memset(message,0,sizeof(message));
  memcpy(message,"{\"total_vote_count\":\"",21);
  memcpy(message+strlen(message),message2,strnlen(message2,sizeof(message)));
  memcpy(message+strlen(message),"\"}",2);

  // add the total of the reserve proof to the total_vote_count of the delegate
  if (update_document_from_collection(database_name,"delegates",data2,message) == 0)
  {
    // delete the reserve proof, since it could not update the delegates total_vote_count
    memset(data,0,sizeof(data));
    memcpy(data,"{\"public_address_created_reserve_proof\":\"",41);
    memcpy(data+strlen(data),reserve_proof.public_address_created_reserve_proof,XCASH_WALLET_LENGTH);
    memcpy(data+strlen(data),"\"}",2);
    delete_document_from_collection(database_name,data3,data);
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("Could not update the voted for delegates total votes}");
  }
  send_data(CLIENT_SOCKET,(unsigned char*)"The vote was successfully added to the database}",0,0,"");
  return 1;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_nodes_to_block_verifiers_register_delegates
Description: Runs the code when the server receives the NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  MESSAGE - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_nodes_to_block_verifiers_register_delegates(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char data[SMALL_BUFFER_SIZE];
  char delegate_name[BUFFER_SIZE_NETWORK_BLOCK_DATA];
  char delegate_public_address[XCASH_WALLET_LENGTH+1];
  char delegate_public_key[VRF_PUBLIC_KEY_LENGTH+1];
  unsigned char delegate_public_key_data[crypto_vrf_PUBLICKEYBYTES+1];
  char delegates_IP_address[BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH+1];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;
  int count2;
  size_t data_size;

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE_ERROR(settings) \
  if (debug_settings == 1) \
  { \
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_nodes_to_block_verifiers_register_delegates",70); \
    memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
    error_message.total++; \
  } \
  send_data(CLIENT_SOCKET,(unsigned char*)settings,0,0,""); \
  return 0;

  memset(data,0,sizeof(data));
  memset(delegate_name,0,sizeof(delegate_name));
  memset(delegate_public_address,0,sizeof(delegate_public_address));
  memset(delegate_public_key,0,sizeof(delegate_public_key));
  memset(delegate_public_key_data,0,sizeof(delegate_public_key_data));
  memset(delegates_IP_address,0,sizeof(delegates_IP_address));

  // get the current time
  get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);

  // check if it is valid to add a reserve proof to the invalid_reserve_proofs struct
  if (test_settings == 0 && current_UTC_date_and_time.tm_min % BLOCK_TIME != 2 && current_UTC_date_and_time.tm_min % BLOCK_TIME != 3)
  {
    send_data(CLIENT_SOCKET,(unsigned char*)"Invalid vote time\nValid times are after the first two minutes of each round}",0,0,"");
    return 0;
  }

  // check if the maximum amount of delegates has been registered
  if (count_all_documents_in_collection(database_name,DATABASE_COLLECTION) >= MAXIMUM_AMOUNT_OF_DELEGATES)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE_ERROR("The maximum amount of delegates has been registered}");
  }

  // verify the message
  if (verify_data(MESSAGE,0) == 0 || string_count(MESSAGE,"|") != REGISTER_PARAMETER_AMOUNT || check_for_invalid_strings(MESSAGE) == 0)
  { 
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE_ERROR("Could not verify the message}");
  }

  // parse the message
  for (count = 0, count2 = 0; count < REGISTER_PARAMETER_AMOUNT; count++)
  {
    if (count == 1)
    {
      if ((data_size = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2) >= sizeof(delegate_name))
      {
        SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE_ERROR("Invalid message data}");
      }
      memcpy(delegate_name,&MESSAGE[count2],data_size);
    }
    if (count == 2)
    {
      if ((data_size = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2) >= sizeof(delegates_IP_address))
      {
        SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE_ERROR("Invalid message data}");
      }
      memcpy(delegates_IP_address,&MESSAGE[count2],data_size);
    }
    if (count == 3)
    {
      if ((data_size = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2) != VRF_PUBLIC_KEY_LENGTH)
      {
        SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE_ERROR("Invalid message data}");
      }
      memcpy(delegate_public_key,&MESSAGE[count2],data_size);
    }
    if (count == 4)
    {
      if ((data_size = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2) != XCASH_WALLET_LENGTH)
      {
        SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE_ERROR("Invalid message data}");
      }
      memcpy(delegate_public_address,&MESSAGE[count2],data_size);
    }
    count2 = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) + 1;
  }

  // convert the VRF public key string to a VRF public key
  for (count2 = 0, count = 0; count2 < VRF_PUBLIC_KEY_LENGTH; count++, count2 += 2)
  {
    memset(data,0,sizeof(data));
    memcpy(data,&delegate_public_key[count2],2);
    delegate_public_key_data[count] = (int)strtol(data, NULL, 16);
  } 
  
  // check if the data is valid
  if (strlen(delegate_name) > MAXIMUM_BUFFER_SIZE_DELEGATES_NAME || strlen(delegate_name) < MINIMUM_BUFFER_SIZE_DELEGATES_NAME || strlen(delegate_public_address) != XCASH_WALLET_LENGTH || strncmp(delegate_public_address,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || strstr(delegates_IP_address,".") == NULL || strlen(delegates_IP_address) > BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH || strstr(delegates_IP_address,"http://") != NULL || strstr(delegates_IP_address,"https://") != NULL || strstr(delegates_IP_address,"www.") != NULL || strlen(delegate_public_key) != VRF_PUBLIC_KEY_LENGTH || crypto_vrf_is_valid_key((const unsigned char*)delegate_public_key_data) != 1)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE_ERROR("Invalid data}");
  }
  
  // create the message
  memset(data,0,sizeof(data));
  memcpy(data,"{\"public_address\":\"",19);
  memcpy(data+19,delegate_public_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\"}",2);

  // check if the public address is already registered
  if (count_documents_in_collection(database_name,DATABASE_COLLECTION,data) != 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE_ERROR("The delegates public address is already registered}");
  }

  // create the message
  memset(data,0,sizeof(data));
  memcpy(data,"{\"IP_address\":\"",15);
  memcpy(data+15,delegates_IP_address,strnlen(delegates_IP_address,sizeof(data)));
  memcpy(data+strlen(data),"\"}",2); 

  // check if the IP address is already registered
  if (count_documents_in_collection(database_name,DATABASE_COLLECTION,data) != 0)
  {    
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE_ERROR("The delegates IP address is already registered}");
  }

  // create the message
  memset(data,0,sizeof(data));
  memcpy(data,"{\"public_key\":\"",15);
  memcpy(data+15,delegate_public_key,VRF_PUBLIC_KEY_LENGTH);
  memcpy(data+79,"\"}",2); 

  // check if the public key is already registered
  if (count_documents_in_collection(database_name,DATABASE_COLLECTION,data) != 0)
  {    
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE_ERROR("The delegates public key is already registered}");
  }

  // create the message
  memset(data,0,sizeof(data));
  memcpy(data,"{\"delegate_name\":\"",18);
  memcpy(data+18,delegate_name,strnlen(delegate_name,sizeof(data)));
  memcpy(data+strlen(data),"\"}",2); 
 
  // check if the delegate name is already registered
  if (count_documents_in_collection(database_name,DATABASE_COLLECTION,data) != 0)
  {    
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE_ERROR("The delegates name is already registered}");
  }
  
  // create the message
  memset(data,0,sizeof(data));
  memcpy(data,"{\"public_address\":\"",19);
  memcpy(data+strlen(data),delegate_public_address,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\",\"total_vote_count\":\"0\",\"IP_address\":\"",39);
  memcpy(data+strlen(data),delegates_IP_address,strnlen(delegates_IP_address,sizeof(data)));
  memcpy(data+strlen(data),"\",\"delegate_name\":\"",19);
  memcpy(data+strlen(data),delegate_name,strnlen(delegate_name,sizeof(data)));
  memcpy(data+strlen(data),"\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"server_settings\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"public_key\":\"",342);
  memcpy(data+strlen(data),delegate_public_key,VRF_PUBLIC_KEY_LENGTH);
  memcpy(data+strlen(data),"\"}",2);

  // add the delegate to the database
  if (strncmp(delegate_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0 && strncmp(delegate_name,"delegate_name_1",BUFFER_SIZE) == 0 && strncmp(delegate_public_key,NEXT_BLOCK_VERIFIERS_PUBLIC_KEY,VRF_PUBLIC_KEY_LENGTH) == 0)
  {
    if (insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,data) == 0)
    {
      SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE_ERROR("The delegate could not be added to the database}");
    }  
  }
  else
  {
    if (insert_document_into_collection_json(database_name,DATABASE_COLLECTION,data) == 0)
    {
      SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE_ERROR("The delegate could not be added to the database}");
    }  
  }
  
  send_data(CLIENT_SOCKET,(unsigned char*)"Registered the delegate}",0,0,"");
  return 1;

  #undef DATABASE_COLLECTION
  #undef SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: check_for_valid_fee_structure
Description: Checks for a valid fee structure
Parameters:
  MESSAGE - The fee structure
Return: 0 if the string is not valid, 1 if the string is valid
-----------------------------------------------------------------------------------------------------------
*/

int check_for_valid_fee_structure(const char* MESSAGE)
{
  // Variables
  int count;
  int settings;
  int settings2;
  double number;

  for (count = 0, settings = 0, settings2 = 0; count < (int)strlen(MESSAGE); count++)
  {
    if ((strncmp(&MESSAGE[count],"0",1) != 0 && strncmp(&MESSAGE[count],"1",1) != 0 && strncmp(&MESSAGE[count],"2",1) != 0 && strncmp(&MESSAGE[count],"3",1) != 0 && strncmp(&MESSAGE[count],"4",1) != 0 && strncmp(&MESSAGE[count],"5",1) != 0 && strncmp(&MESSAGE[count],"6",1) != 0 && strncmp(&MESSAGE[count],"7",1) != 0 && strncmp(&MESSAGE[count],"8",1) != 0 && strncmp(&MESSAGE[count],"9",1) != 0 && strncmp(&MESSAGE[count],".",1) != 0) || (count == 0 && strncmp(&MESSAGE[count],".",1) == 0))
    {
      return 0;
    }
    if (settings == 1)
    {
      settings2++;
    }
    if (settings2 > 6)
    {
      return 0;
    }
    if (strncmp(&MESSAGE[count],".",1) == 0)
    {
      settings = 1;
    }
  }

  sscanf(MESSAGE,"%lf",&number);
  return number < 0 || number > 100 ? 0 : 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_nodes_to_block_verifiers_update_delegates
Description: Runs the code when the server receives the NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  MESSAGE - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_nodes_to_block_verifiers_update_delegates(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char data[4096];
  char data2[4096];
  char delegate_public_address[XCASH_WALLET_LENGTH+1];
  unsigned char delegate_public_key_data[crypto_vrf_PUBLICKEYBYTES+1];
  char item[BUFFER_SIZE_NETWORK_BLOCK_DATA];
  char value[4096];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;
  int count2;
  size_t data_size;

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE_ERROR(settings) \
  if (debug_settings == 1) \
  { \
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_nodes_to_block_verifiers_update_delegates",68); \
    memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
    error_message.total++; \
  } \
  send_data(CLIENT_SOCKET,(unsigned char*)settings,0,0,""); \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(delegate_public_address,0,sizeof(delegate_public_address));
  memset(delegate_public_key_data,0,sizeof(delegate_public_key_data));
  memset(item,0,sizeof(item));
  memset(value,0,sizeof(value));

  // get the current time
  get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);

  // check if it is valid to add a reserve proof to the invalid_reserve_proofs struct
  if (test_settings == 0 && current_UTC_date_and_time.tm_min % BLOCK_TIME != 2 && current_UTC_date_and_time.tm_min % BLOCK_TIME != 3)
  {
    send_data(CLIENT_SOCKET,(unsigned char*)"Invalid vote time\nValid times are after the first two minutes of each round}",0,0,"");
    return 0;
  }

  // verify the message
  if (verify_data(MESSAGE,0) == 0 || string_count(MESSAGE,"|") != UPDATE_PARAMETER_AMOUNT || check_for_invalid_strings(MESSAGE) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE_ERROR("Could not verify the message}");
  }

  // parse the message
  for (count = 0, count2 = 0; count < UPDATE_PARAMETER_AMOUNT; count++)
  {
    if (count == 1)
    {
      if ((data_size = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2) >= sizeof(item))
      {
        SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE_ERROR("Invalid message data}");
      }
      memcpy(item,&MESSAGE[count2],data_size);
    }
    if (count == 2)
    {
      if ((data_size = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2) >= sizeof(value))
      {
        SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE_ERROR("Invalid message data}");
      }
      memcpy(value,&MESSAGE[count2],data_size);
    }
    if (count == 3)
    {
      if ((data_size = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2) != XCASH_WALLET_LENGTH)
      {
        SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE_ERROR("Invalid message data}");
      }
      memcpy(delegate_public_address,&MESSAGE[count2],data_size);
    }
    count2 = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) + 1;
  }

  // check if the data is valid
  if (strlen(delegate_public_address) != XCASH_WALLET_LENGTH || strncmp(delegate_public_address,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE_ERROR("Invalid data}");
  }

  // check if the item is valid
  if (strncmp(item,"IP_address",BUFFER_SIZE) != 0 && strncmp(item,"about",BUFFER_SIZE) != 0 && strncmp(item,"website",BUFFER_SIZE) != 0 && strncmp(item,"team",BUFFER_SIZE) != 0 && strncmp(item,"pool_mode",BUFFER_SIZE) != 0 && strncmp(item,"fee_structure",BUFFER_SIZE) != 0 && strncmp(item,"server_settings",BUFFER_SIZE) != 0)
  {    
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE_ERROR("Invalid item to update}");
  }

  // check if the value is valid
  if ((strncmp(item,"IP_address",BUFFER_SIZE) == 0 && strlen(value) > 255) || (strncmp(item,"about",BUFFER_SIZE) == 0 && strlen(value) > 1024) || (strncmp(item,"website",BUFFER_SIZE) == 0 && strlen(value) > 255) || (strncmp(item,"team",BUFFER_SIZE) == 0 && strlen(value) > 255) || (strncmp(item,"pool_mode",BUFFER_SIZE) == 0 && strncmp(value,"true",BUFFER_SIZE) != 0 && strncmp(value,"false",BUFFER_SIZE) != 0) || (strncmp(item,"fee_structure",BUFFER_SIZE) == 0 && check_for_valid_fee_structure(value) == 0) || (strncmp(item,"server_settings",BUFFER_SIZE) == 0 && strlen(value) > 1024))
  {    
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE_ERROR("Invalid item value to update}");
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

  // update the delegate in the database
  if (update_document_from_collection(database_name,DATABASE_COLLECTION,data,data2) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE_ERROR("The delegate could not be updated from the database}");
  }

  send_data(CLIENT_SOCKET,(unsigned char*)"Updated the delegates information}",0,0,"");
  return 1;

  #undef DATABASE_COLLECTION
  #undef SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE_ERROR
}