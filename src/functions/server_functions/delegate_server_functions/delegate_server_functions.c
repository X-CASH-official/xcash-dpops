#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <arpa/nameser.h>
#include <resolv.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "initialize_and_reset_structs_define_macros.h"
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
    count2 = (int)(strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) + 1);
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
  if (check_reserve_proofs(data2,reserve_proof->public_address_created_reserve_proof,reserve_proof->reserve_proof) != 1)
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
    snprintf(data2+15,MAXIMUM_NUMBER_SIZE,"%d",count);

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
    snprintf(data+15,MAXIMUM_NUMBER_SIZE,"%d",count);
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
-----------------------------------------------------------------------------------------------------------
*/

void server_receive_data_socket_node_to_block_verifiers_add_reserve_proof(const int CLIENT_SOCKET, const char* MESSAGE)
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
  return;

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
  if (test_settings == 0 && current_UTC_date_and_time.tm_min != 3)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("Invalid vote time\nValid times are the second and third minute of each hour}");
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

  // dont allow votes for network data nodes
  for (count = 0; count < NETWORK_DATA_NODES_AMOUNT; count++)
  {
    if (strncmp(network_data_nodes_list.network_data_nodes_public_address[count],reserve_proof.public_address_voted_for,BUFFER_SIZE) == 0)
    {
      SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("Can not vote for a network data node}");
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
    snprintf(data3+15,MAXIMUM_NUMBER_SIZE,"%zu",count);
    if (count_all_documents_in_collection(database_name,data3) < (MAXIMUM_INVALID_RESERVE_PROOFS / TOTAL_RESERVE_PROOFS_DATABASES))
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
  return;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: check_for_valid_delegate_name
Description: Checks for a valid delegate name
Parameters:
  DELEGATE_NAME - The delegate name
Return: 0 if the delegate name is not valid, 1 if the delegate name is valid
-----------------------------------------------------------------------------------------------------------
*/

int check_for_valid_delegate_name(const char* DELEGATE_NAME)
{
  // Constants
  const size_t DELEGATE_NAME_LENGTH = strlen(DELEGATE_NAME);

  // define macros
  #define VALID_DATA "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789._-"

  // Variables
  char data[sizeof(VALID_DATA)];
  size_t count;
  size_t count2;

  memset(data,0,sizeof(data));
  memcpy(data,VALID_DATA,sizeof(VALID_DATA)-1);

  // check if the delegate name is a valid length
  if (DELEGATE_NAME_LENGTH > MAXIMUM_BUFFER_SIZE_DELEGATES_NAME || DELEGATE_NAME_LENGTH < MINIMUM_BUFFER_SIZE_DELEGATES_NAME)
  {
    return 0;
  }

  // check if the delegate name has any invalid characters
  for (count = 0; count < DELEGATE_NAME_LENGTH; count++)
  {
    for (count2 = 0; count2 < (sizeof(VALID_DATA)-1); count2++)
    {
      if (strncmp(&DELEGATE_NAME[count],&data[count2],sizeof(char)) == 0)
      {
        break;
      }
    }
    if (count2 == (sizeof(VALID_DATA)-1))
    {
      return 0;
    }
  }
  return 1;

  #undef VALID_DATA
}



/*
-----------------------------------------------------------------------------------------------------------
Name: check_for_valid_ip_address
Description: Checks for a valid IP address
Parameters:
  HOST - The IP address or the domain name
Return: 0 if the IP address is not valid, 1 if the IP address is valid
-----------------------------------------------------------------------------------------------------------
*/

int check_for_valid_ip_address(const char* HOST)
{
  // define macros
  #define IP_ADDRESS_RANGE_AMOUNT 15
  #define IP_ADDRESS_SEGMENTS_TOTAL 4

  // Constants
  const int IP_ADDRESS_RANGE_START[IP_ADDRESS_RANGE_AMOUNT][IP_ADDRESS_SEGMENTS_TOTAL] = {{0,0,0,0},{10,0,0,0},{100,64,0,0},{127,0,0,0},{169,254,0,0},{172,16,0,0},{192,0,0,0},{192,0,2,0},{192,88,99,0},{192,168,0,0},{198,18,0,0},{198,51,100,0},{203,0,113,0},{224,0,0,0},{240,0,0,0}};
  const int IP_ADDRESS_RANGE_END[IP_ADDRESS_RANGE_AMOUNT][IP_ADDRESS_SEGMENTS_TOTAL] = {{0,255,255,255},{10,255,255,255},{100,127,255,255},{127,255,255,255},{169,254,255,255},{172,31,255,255},{192,0,0,255},{192,0,2,255},{192,88,99,255},{192,168,255,255},{198,19,255,255},{198,51,100,255},{203,0,113,255},{239,255,255,255},{255,255,255,254}};

  // Variables
  char ip_address[BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH];
  unsigned int ip_address_data[IP_ADDRESS_SEGMENTS_TOTAL];
  char* search_item;
  char* search_item_data;
  struct addrinfo serv_addr;
  struct addrinfo* settings = NULL;
  char* data;
  void* ptr;
  int count;
  unsigned int count2; 
  unsigned int ip_address_start;
  unsigned int ip_address_end;

  memset(ip_address,0,sizeof(ip_address));

  // check if the IP address is a domain name
  if (check_if_IP_address_or_hostname(HOST) == 1)
  {
    // the host is an IP address
    memcpy(ip_address,HOST,strnlen(HOST,sizeof(ip_address)));
  }
  else
  {
    // the host is a domain name
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.ai_flags = AI_NUMERICSERV;
    serv_addr.ai_family = AF_INET;
    serv_addr.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(HOST, NULL, &serv_addr, &settings) != 0)
    {
      return 0;
    }
  
    inet_ntop(settings->ai_family,settings->ai_addr->sa_data,ip_address,sizeof(ip_address));    
    ptr = &((struct sockaddr_in *)settings->ai_addr)->sin_addr;
    inet_ntop(settings->ai_family,ptr,ip_address,sizeof(ip_address));

    freeaddrinfo(settings);
  }
  
  // check if the host is valid
  if (strstr(ip_address,".") == NULL || strlen(ip_address) > BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH || strlen(HOST) > BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH || string_count(ip_address,".") < 1 || string_count(HOST,".") < 1 || strstr(ip_address,"http://") != NULL || strstr(ip_address,"https://") != NULL || strstr(ip_address,"www.") != NULL)
  {
    return 0;
  }

  // convert the ip address to an int array
  for (count = 0, search_item = strtok_r(ip_address, ".",&search_item_data); search_item != NULL; count++, search_item = strtok_r(NULL, ".",&search_item_data))
  {
    ip_address_data[count] = strtol(search_item,&data,10);
  }

  // check the first invalid ip address since it does not have a range
  if (ip_address_data[0] == 255 && ip_address_data[1] == 255 && ip_address_data[2] == 255 && ip_address_data[3] == 255)
  {
    return 0;
  }
  
  // check if the host is in an invalid IP address range
  for (count = 0; count < IP_ADDRESS_RANGE_AMOUNT; count++)
  {
    ip_address_start = (IP_ADDRESS_RANGE_START[count][0] << 24 | IP_ADDRESS_RANGE_START[count][1] << 16 | IP_ADDRESS_RANGE_START[count][2] << 8 | IP_ADDRESS_RANGE_START[count][3]);
    ip_address_end = (IP_ADDRESS_RANGE_END[count][0] << 24 | IP_ADDRESS_RANGE_END[count][1] << 16 | IP_ADDRESS_RANGE_END[count][2] << 8 | IP_ADDRESS_RANGE_END[count][3]);

    for (count2 = ip_address_start; count2 <= ip_address_end; count2++)
    {
      if (ip_address_data[0] == ((count2 & 0xFF000000)>>24) && ip_address_data[1] == ((count2 & 0x00FF0000)>>16) && ip_address_data[2] == ((count2 & 0x0000FF00)>>8) && ip_address_data[3] == ((count2 & 0x000000FF)))
      {
        return 0;
      }
    }
  }
  return 1;

  #undef IP_ADDRESS_RANGE_AMOUNT
  #undef IP_ADDRESS_SEGMENTS_TOTAL
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_nodes_to_block_verifiers_register_delegates
Description: Runs the code when the server receives the NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  MESSAGE - The message
-----------------------------------------------------------------------------------------------------------
*/

void server_receive_data_socket_nodes_to_block_verifiers_register_delegates(const int CLIENT_SOCKET, const char* MESSAGE)
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
  return;

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
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE_ERROR("Invalid vote time\nValid times are after the first two minutes of each round}");
  }

  // check if the maximum amount of delegates has been registered
  if (count_all_documents_in_collection(database_name,DATABASE_COLLECTION) >= (MAXIMUM_AMOUNT_OF_DELEGATES - 1))
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
    count2 = (int)(strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) + 1);
  }

  // convert the VRF public key string to a VRF public key
  for (count2 = 0, count = 0; count2 < VRF_PUBLIC_KEY_LENGTH; count++, count2 += 2)
  {
    memset(data,0,sizeof(data));
    memcpy(data,&delegate_public_key[count2],2);
    delegate_public_key_data[count] = (unsigned char)strtol(data, NULL, 16);
  } 
  
  // check if the data is valid
  if (check_for_valid_delegate_name(delegate_name) == 0 || strlen(delegate_public_address) != XCASH_WALLET_LENGTH || strncmp(delegate_public_address,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0 || check_for_valid_ip_address(delegates_IP_address) == 0 || strlen(delegate_public_key) != VRF_PUBLIC_KEY_LENGTH || crypto_vrf_is_valid_key((const unsigned char*)delegate_public_key_data) != 1)
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
  memcpy(data+strlen(data),"\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"shared_delegate_status\":\"solo\",\"delegate_fee\":\"\",\"server_specs\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"public_key\":\"",350);
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
  return;

  #undef DATABASE_COLLECTION
  #undef SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: check_for_valid_delegate_fee
Description: Checks for a valid fee structure
Parameters:
  MESSAGE - The fee structure
Return: 0 if the string is not valid, 1 if the string is valid
-----------------------------------------------------------------------------------------------------------
*/

int check_for_valid_delegate_fee(const char* MESSAGE)
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
-----------------------------------------------------------------------------------------------------------
*/

void server_receive_data_socket_nodes_to_block_verifiers_update_delegates(const int CLIENT_SOCKET, const char* MESSAGE)
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
  return;

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
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE_ERROR("Invalid vote time\nValid times are after the first two minutes of each round}");
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
    count2 = (int)(strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) + 1);
  }

  // check if the data is valid
  if (strlen(delegate_public_address) != XCASH_WALLET_LENGTH || strncmp(delegate_public_address,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE_ERROR("Invalid data}");
  }

  // check if the item is valid
  if (strncmp(item,"IP_address",BUFFER_SIZE) != 0 && strncmp(item,"about",BUFFER_SIZE) != 0 && strncmp(item,"website",BUFFER_SIZE) != 0 && strncmp(item,"team",BUFFER_SIZE) != 0 && strncmp(item,"shared_delegate_status",BUFFER_SIZE) != 0 && strncmp(item,"delegate_fee",BUFFER_SIZE) != 0 && strncmp(item,"server_specs",BUFFER_SIZE) != 0)
  {    
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE_ERROR("Invalid item to update}");
  }

  // check if the value is valid
  if ((strncmp(item,"IP_address",BUFFER_SIZE) == 0 && check_for_valid_ip_address(value) == 0) || (strncmp(item,"about",BUFFER_SIZE) == 0 && strlen(value) > 255) || (strncmp(item,"website",BUFFER_SIZE) == 0 && strlen(value) > 255) || (strncmp(item,"team",BUFFER_SIZE) == 0 && strlen(value) > 255) || (strncmp(item,"shared_delegate_status",BUFFER_SIZE) == 0 && strncmp(value,"solo",BUFFER_SIZE) != 0 && strncmp(value,"shared",BUFFER_SIZE) != 0 && strncmp(value,"group",BUFFER_SIZE) != 0) || (strncmp(item,"delegate_fee",BUFFER_SIZE) == 0 && check_for_valid_delegate_fee(value) == 0) || (strncmp(item,"server_specs",BUFFER_SIZE) == 0 && strlen(value) > 255))
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
  return;

  #undef DATABASE_COLLECTION
  #undef SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_nodes_to_block_verifiers_recover_delegates
Description: Runs the code when the server receives the NODES_TO_BLOCK_VERIFIERS_RECOVER_DELEGATE message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  MESSAGE - The message
-----------------------------------------------------------------------------------------------------------
*/

void server_receive_data_socket_nodes_to_block_verifiers_recover_delegates(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  char host[SMALL_BUFFER_SIZE];
  char delegate_public_address[XCASH_WALLET_LENGTH+1];
  char delegate_recover_public_address[XCASH_WALLET_LENGTH+1];
  char delegate_recover_public_key[VRF_PUBLIC_KEY_LENGTH+1];
  unsigned char delegate_recover_public_key_data[crypto_vrf_PUBLICKEYBYTES+1];
  struct database_document_fields database_data;
  char* message;
  unsigned char dns_data[BUFFER_SIZE];
  struct __res_state dnsstate;
  ns_msg dns_message;
  ns_rr dns_record;
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;
  int count2;
  int dns_result;
  size_t data_size;

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RECOVER_DELEGATE_ERROR(settings) \
  color_print(settings,"yellow"); \
  if (debug_settings == 1) \
  { \
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_nodes_to_block_verifiers_recover_delegates",69); \
    memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
    error_message.total++; \
  } \
  send_data(CLIENT_SOCKET,(unsigned char*)settings,0,0,""); \
  return;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));
  memset(host,0,sizeof(host));
  memset(delegate_public_address,0,sizeof(delegate_public_address));
  memset(delegate_recover_public_address,0,sizeof(delegate_recover_public_address));
  memset(delegate_recover_public_key,0,sizeof(delegate_recover_public_key));
  memset(delegate_recover_public_key_data,0,sizeof(delegate_recover_public_key_data));

  // get the current time
  get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);

  // check if it is valid to add a reserve proof to the invalid_reserve_proofs struct
  if (test_settings == 0 && current_UTC_date_and_time.tm_min % BLOCK_TIME != 2 && current_UTC_date_and_time.tm_min % BLOCK_TIME != 3)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RECOVER_DELEGATE_ERROR("Invalid vote time\nValid times are after the first two minutes of each round}");
  }

  // verify the message
  if (string_count(MESSAGE,"|") != RECOVER_PARAMETER_AMOUNT || check_for_invalid_strings(MESSAGE) == 0)
  {   
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RECOVER_DELEGATE_ERROR("Could not verify the message}");
  }

  // parse the message
  for (count = 0, count2 = 0; count < RECOVER_PARAMETER_AMOUNT; count++)
  {
    if (count == 1)
    {
      if ((data_size = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2) >= sizeof(host))
      {
        SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RECOVER_DELEGATE_ERROR("Invalid message data}");
      }
      memcpy(host,&MESSAGE[count2],data_size);
    }
    count2 = (int)(strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) + 1);
  }

  // check if its an ip address
  if (check_if_IP_address_or_hostname(host) == 1)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RECOVER_DELEGATE_ERROR("Can not recover the delegate since it has been registered with an IP address and not a domain name}");
  }

  // make sure the domain is already registered in the database
  memcpy(data,"{\"IP_address\":\"",15);
  memcpy(data+strlen(data),host,strnlen(host,sizeof(data)));
  memcpy(data+strlen(data),"\"}",2);

  if (count_documents_in_collection(database_name,DATABASE_COLLECTION,data) != 1)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RECOVER_DELEGATE_ERROR("The domain is not registered in the database}");
  }

  // initialize the DNS data
  if (res_ninit(&dnsstate) == -1)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RECOVER_DELEGATE_ERROR("The delegate could not be recovered}");
  }

  // set the retry time and max retry attempts
  dnsstate.retrans = CONNECTION_TIMEOUT_SETTINGS;
  dnsstate.retry = 2;
  
  // read the TXT records from the domain
  if ((dns_result = res_nquery(&dnsstate, host, ns_c_in, ns_t_txt, (unsigned char*)&dns_data, sizeof(dns_data))) == -1)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RECOVER_DELEGATE_ERROR("Can not read the TXT records for the domain}");
  }

  ns_initparse(dns_data, dns_result, &dns_message);
  dns_result = ns_msg_count(dns_message, ns_s_an);
  
  // loop through each TXT record and look for a valid XCASH DPOPS TXT record
  for (count = 0; count < dns_result; count++)
  {
    ns_parserr(&dns_message, ns_s_an, count, &dns_record);
    ns_sprintrr(&dns_message, &dns_record, NULL, NULL, data2, sizeof(data2));

    // check if it is a XCASH DPOPS TXT record
    if ((message = strstr(data2,TXT_RECORD_START_STRING)) != NULL)
    {
      message += (sizeof(TXT_RECORD_START_STRING)-1);
      if (strstr(message,XCASH_WALLET_PREFIX) && strlen(message) >= XCASH_WALLET_LENGTH)
      {
        memset(delegate_recover_public_address,0,sizeof(delegate_recover_public_address));
        memcpy(delegate_recover_public_address,message,XCASH_WALLET_LENGTH);
      }
      else if (strlen(message) >= VRF_PUBLIC_KEY_LENGTH)
      {
        memset(delegate_recover_public_key,0,sizeof(delegate_recover_public_key));
        memcpy(delegate_recover_public_key,message,VRF_PUBLIC_KEY_LENGTH);
      }
    }
  }
  

  // check if the public address is valid
  if (strlen(delegate_recover_public_address) == XCASH_WALLET_LENGTH && strncmp(delegate_recover_public_address,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RECOVER_DELEGATE_ERROR("The public address is invalid}");
  }

  // check if the public key is valid
  if (strlen(delegate_recover_public_key) == VRF_PUBLIC_KEY_LENGTH)
  {
    // convert the VRF public key string to a VRF public key
    for (count2 = 0, count = 0; count2 < VRF_PUBLIC_KEY_LENGTH; count++, count2 += 2)
    {
      memset(data2,0,sizeof(data2));
      memcpy(data2,&delegate_recover_public_key[count2],2);
      delegate_recover_public_key_data[count] = (unsigned char)strtol(data2, NULL, 16);
    }
    if (crypto_vrf_is_valid_key((const unsigned char*)delegate_recover_public_key_data) != 1)
    {
      SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RECOVER_DELEGATE_ERROR("The public key is invalid}");
    }
  }

  // check to see if the delegate had at least one XCASH DPOPS TXT record
  if (strlen(delegate_recover_public_address) != XCASH_WALLET_LENGTH && strlen(delegate_recover_public_key) != VRF_PUBLIC_KEY_LENGTH)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RECOVER_DELEGATE_ERROR("The domain name does not have a valid XCASH DPOPS TXT record}");
  }

  // copy the document, change the values and insert the new document into the collection

  // initialize the database_document_fields struct 
  INITIALIZE_DATABASE_DOCUMENT_FIELDS_STRUCT(count,TOTAL_DELEGATES_DATABASE_FIELDS,"server_receive_data_socket_nodes_to_block_verifiers_recover_delegates",data2,current_date_and_time,current_UTC_date_and_time);
    
  // get the delegates data
  if (read_document_all_fields_from_collection(database_name,DATABASE_COLLECTION,data,&database_data) == 0)
  {
    POINTER_RESET_DATABASE_DOCUMENT_FIELDS_STRUCT(count);
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RECOVER_DELEGATE_ERROR("Could not get the delegates information");
  }

  // update the delegates data
  for (count = 0; count < (int)database_data.count; count++)
  {
    if (strncmp(database_data.item[count],"public_address",BUFFER_SIZE) == 0)
    {
      // get the current delegates public address
      memcpy(delegate_public_address,database_data.value[count],XCASH_WALLET_LENGTH);
    }
    if (strncmp(database_data.item[count],"public_address",BUFFER_SIZE) == 0 && strlen(delegate_recover_public_address) == XCASH_WALLET_LENGTH)
    {
      // make sure the new data is not the same as the old data
      if (strncmp(database_data.value[count],delegate_recover_public_address,BUFFER_SIZE) == 0)
      {
        SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RECOVER_DELEGATE_ERROR("The data has already been updated");
      }
      memset(database_data.value[count],0,strlen(database_data.value[count]));
      memcpy(database_data.value[count],delegate_recover_public_address,XCASH_WALLET_LENGTH);
    }
    else if (strncmp(database_data.item[count],"public_key",BUFFER_SIZE) == 0 && strlen(delegate_recover_public_key) == VRF_PUBLIC_KEY_LENGTH)
    {
      // make sure the new data is not the same as the old data
      if (strncmp(database_data.value[count],delegate_recover_public_key,BUFFER_SIZE) == 0)
      {
        SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RECOVER_DELEGATE_ERROR("The data has already been updated");
      }
      memset(database_data.value[count],0,strlen(database_data.value[count]));
      memcpy(database_data.value[count],delegate_recover_public_key,VRF_PUBLIC_KEY_LENGTH);
    }
  }
  
  memset(data2,0,sizeof(data2));

  // create a json string out of the delegates database data
  if (create_json_data_from_database_document_array(&database_data,data2,"") == 0)
  {
    POINTER_RESET_DATABASE_DOCUMENT_FIELDS_STRUCT(count);
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RECOVER_DELEGATE_ERROR("Could not create json data");
  }

  // remove the old document
  if (delete_document_from_collection(database_name,DATABASE_COLLECTION,data) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RECOVER_DELEGATE_ERROR("Could not remove the existing delegates data");
  }

  // add the document to the database
  if (insert_document_into_collection_json(database_name,DATABASE_COLLECTION,data2) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RECOVER_DELEGATE_ERROR("Could not add the delegates data to the database");
  }

  // update the reserve proofs for the document
  if (strlen(delegate_recover_public_address) == XCASH_WALLET_LENGTH)
  {
    memset(data,0,sizeof(data));
    memcpy(data,"{\"public_address_voted_for\":\"",29);
    memcpy(data+strlen(data),delegate_public_address,XCASH_WALLET_LENGTH);
    memcpy(data+strlen(data),"\"}",2);

    memset(data3,0,sizeof(data3));
    memcpy(data3,"{\"public_address_voted_for\":\"",29);
    memcpy(data3+strlen(data3),delegate_recover_public_address,XCASH_WALLET_LENGTH);
    memcpy(data3+strlen(data3),"\"}",2);

    for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
    { 
      memset(data2,0,sizeof(data2));
      memcpy(data2,"reserve_proofs_",15);
      snprintf(data2+15,MAXIMUM_NUMBER_SIZE,"%d",count);
      update_multiple_documents_from_collection(database_name,data2,data,data3);
    }

    RESET_ERROR_MESSAGES;
  }

  send_data(CLIENT_SOCKET,(unsigned char*)"The delegate has been recovered successfully}",0,0,"");
  return;

  #undef DATABASE_COLLECTION
  #undef SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RECOVER_DELEGATE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_nodes_to_network_data_nodes_check_vote_status
Description: Runs the code when the server receives the NODE_TO_NETWORK_DATA_NODES_CHECK_VOTE_STATUS message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  MESSAGE - The message
-----------------------------------------------------------------------------------------------------------
*/

void server_receive_data_socket_nodes_to_network_data_nodes_check_vote_status(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  char data[SMALL_BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  char message[BUFFER_SIZE];
  char total_vote_count[SMALL_BUFFER_SIZE];
  char public_address_voted_for[SMALL_BUFFER_SIZE];
  int count;
  int count2;
  size_t data_size;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_CHECK_VOTE_STATUS_ERROR(settings) \
  if (debug_settings == 1) \
  { \
    memcpy(error_message.function[error_message.total],"server_receive_data_socket_nodes_to_network_data_nodes_check_vote_status",72); \
    memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
    error_message.total++; \
  } \
  memset(message,0,strlen(message)); \
  memcpy(message,"Error: Could not get the vote status}",37); \
  send_data(CLIENT_SOCKET,(unsigned char*)message,0,0,""); \
  return;
  
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));
  memset(message,0,sizeof(message));
  memset(total_vote_count,0,sizeof(total_vote_count));
  memset(public_address_voted_for,0,sizeof(public_address_voted_for));

  // parse the message
  for (count = 0, count2 = 0; count < RECOVER_PARAMETER_AMOUNT; count++)
  {
    if (count == 1)
    {
      if ((data_size = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2) != XCASH_WALLET_LENGTH)
      {
        SERVER_RECEIVE_DATA_SOCKET_CHECK_VOTE_STATUS_ERROR("Invalid message data}");
      }
      memcpy(data2,&MESSAGE[count2],data_size);
    }
    count2 = (int)(strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) + 1);
  }

  // create the message
  memcpy(message,"{\"public_address_created_reserve_proof\":\"",41);
  memcpy(message+strlen(message),data2,XCASH_WALLET_LENGTH);
  memcpy(message+strlen(message),"\"}",2);

  for (count = 0; count < TOTAL_RESERVE_PROOFS_DATABASES; count++)
  {
    memset(data3,0,sizeof(data3));
    memcpy(data3,"reserve_proofs_",15);
    snprintf(data3+15,MAXIMUM_NUMBER_SIZE,"%d",count);

    if (count_documents_in_collection(database_name,data3,message) == 1)
    {
      // get the delegate public address and vote amount
      if (read_document_field_from_collection(database_name,data3,message,"public_address_voted_for",public_address_voted_for) == 0 || read_document_field_from_collection(database_name,data3,message,"total",total_vote_count) == 0)
      {
        SERVER_RECEIVE_DATA_SOCKET_CHECK_VOTE_STATUS_ERROR("Could not get the voters data");
      }
      break;
    }
  }

  if (count == TOTAL_RESERVE_PROOFS_DATABASES)
  {
    memset(message,0,sizeof(message));
    memcpy(message,"Error: The public address does not have an active vote}",55);
    send_data(CLIENT_SOCKET,(unsigned char*)message,0,0,"");
    return;
  }

  // get the delegate name
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  memcpy(data3,"{\"public_address\":\"",19);
  memcpy(data3+strlen(data3),public_address_voted_for,XCASH_WALLET_LENGTH);
  memcpy(data3+strlen(data3),"\"}",2);

  if (read_document_field_from_collection(database_name,"delegates",data3,"delegate_name",data2) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_CHECK_VOTE_STATUS_ERROR("Could not get the voters data");
  }

  memset(message,0,sizeof(message));
  memcpy(message,"delegate_name: ",15);
  memcpy(message+strlen(message),data2,strnlen(data2,sizeof(message)));
  memcpy(message+strlen(message),", total: ",9);
  memcpy(message+strlen(message),total_vote_count,strnlen(total_vote_count,sizeof(message)));
  memcpy(message+strlen(message),"}",1);

  send_data(CLIENT_SOCKET,(unsigned char*)message,0,0,"");
  return;
  
  #undef SERVER_RECEIVE_DATA_SOCKET_CHECK_VOTE_STATUS_ERROR
}
