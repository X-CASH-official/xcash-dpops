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
#include "block_verifiers_server_functions.h"
#include "block_verifiers_synchronize_server_functions.h"
#include "block_verifiers_synchronize_functions.h"
#include "database_functions.h"
#include "delegate_server_functions.h"
#include "delegates_website_and_shared_delegates_website_functions.h"
#include "file_functions.h"
#include "network_daemon_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "network_wallet_functions.h"
#include "server_functions.h"
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
  char message[BUFFER_SIZE];
  char message2[BUFFER_SIZE];
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
  send_data(CLIENT_SOCKET,(unsigned char*)"Could not add the reserve proof to the database}",0,0,""); \
  return 0;

  memset(delegates_public_address,0,sizeof(delegates_public_address));
  memset(public_address,0,sizeof(public_address));
  memset(reserve_proof,0,sizeof(reserve_proof));
  memset(message,0,sizeof(message));
  memset(message2,0,sizeof(message2));
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  // check if it is valid to add a reserve proof to the invalid_reserve_proofs struct
  if (current_UTC_date_and_time->tm_min % BLOCK_TIME == 4 && current_UTC_date_and_time->tm_sec > 25 && current_UTC_date_and_time->tm_sec < 50)
  {
    send_data(CLIENT_SOCKET,(unsigned char*)"The block verifiers are currently deleting invalid reserve proofs from the database.\n\nPlease wait a few seconds",0,0,"");
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
      memcpy(data,&MESSAGE[count2],strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2);
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

  // check if they have voted using the delegates name or delegates public address
  if (strlen(data) != XCASH_WALLET_LENGTH)
  {
    // create the message
    memcpy(data2,"{\"delegate_name\":\"",18);
    memcpy(data2+18,data,strnlen(data,sizeof(data2)));
    memcpy(data2+strlen(data2),"\"}",2);

    // get the delegates public address
    if (read_document_field_from_collection(DATABASE_NAME,"delegates",data2,"public_address",data3,0) == 0)
    {
      SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("Invalid data");
    }

    memset(data,0,sizeof(data));
    memcpy(data,data3,XCASH_WALLET_LENGTH);
    memset(data3,0,sizeof(data3));
  }

  memcpy(delegates_public_address,data,XCASH_WALLET_LENGTH);

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
    snprintf(data2+15,sizeof(data2)-16,"%zu",count);

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

  // check if the reserve proof is greater than or equal to the minimum reserve proof amount
  sscanf(data2,"%zu", &count);
  if (count < MINIMUM_AMOUNT_RESERVE_PROOF)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("The reserve proof is not greater than or equal to the minimum amount");
  }

  // remove any reserve proofs that were created by the public address
  if (settings == 1)
  {
    for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
    {
      memset(data,0,sizeof(data));
      memcpy(data,"reserve_proofs_",15);
      snprintf(data+15,sizeof(data)-16,"%zu",count);
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
    memset(data3,0,sizeof(data3));
    memcpy(data3,"reserve_proofs_",15);
    snprintf(data3+15,sizeof(data3)-16,"%zu",count);
    if (count_documents_in_collection(DATABASE_NAME,data3,data,0) < MAXIMUM_INVALID_RESERVE_PROOFS / TOTAL_RESERVE_PROOFS_DATABASES)
    {
      if (insert_document_into_collection_json(DATABASE_NAME,data3,data,0) == 1)
      {        
        break;
      }
      else
      {
        SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("The vote could not be added to the database");
      }         
    }
  }

  // update the delegates total_vote_count
  memset(data3,0,sizeof(data3));
  memcpy(data3,"{\"public_address\":\"",19);
  memcpy(data3+strlen(data3),delegates_public_address,XCASH_WALLET_LENGTH);
  memcpy(data3+strlen(data3),"\"}",2);

  if (read_document_field_from_collection(DATABASE_NAME,"delegates",data3,"total_vote_count",message2,0) == 0)
  {
    // delete the reserve proof, since it could not update the delegates total_vote_count
    memset(data,0,sizeof(data));
    memcpy(data,"{\"public_address_created_reserve_proof\":\"",41);
    memcpy(data+strlen(data),public_address,XCASH_WALLET_LENGTH);
    memcpy(data+strlen(data),"\"}",2);
    delete_document_from_collection(DATABASE_NAME,data2,data,0);
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("The vote could not be added to the database");
  }

  sscanf(message2,"%zu", &count);
  sscanf(data2,"%zu", &count2);
  count+= count2;
  memset(message2,0,sizeof(message2));
  snprintf(message2,sizeof(message2)-1,"%zu",count);  

  memset(message,0,sizeof(message));
  memcpy(message,"{\"total_vote_count\":\"",21);
  memcpy(message+strlen(message),message2,strnlen(message2,sizeof(message)));
  memcpy(message+strlen(message),"\"}",2);

  // add the total of the reserve proof to the total_vote_count of the delegate
  if (update_document_from_collection(DATABASE_NAME,"delegates",data3,message,0) == 0)
  {
    // delete the reserve proof, since it could not update the delegates total_vote_count
    memset(data,0,sizeof(data));
    memcpy(data,"{\"public_address_created_reserve_proof\":\"",41);
    memcpy(data+strlen(data),public_address,XCASH_WALLET_LENGTH);
    memcpy(data+strlen(data),"\"}",2);
    delete_document_from_collection(DATABASE_NAME,data2,data,0);
    SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF_ERROR("The vote could not be added to the database");
  }
  pthread_rwlock_unlock(&rwlock);
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
  send_data(CLIENT_SOCKET,(unsigned char*)"Could not register the delegate}",0,0,""); \
  return 0;

  memset(data,0,sizeof(data));
  memset(delegate_name,0,sizeof(delegate_name));
  memset(delegate_public_address,0,sizeof(delegate_public_address));
  memset(delegates_IP_address,0,sizeof(delegates_IP_address));

  // check if the maximum amount of delegates has been registered
  if (count_all_documents_in_collection(DATABASE_NAME,DATABASE_COLLECTION,0) >= MAXIMUM_AMOUNT_OF_DELEGATES)
  {
    SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE_ERROR("The maximum amount of delegates has been registered");
  }

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

  send_data(CLIENT_SOCKET,(unsigned char*)"Registered the delegate}",0,0,"");
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
  send_data(CLIENT_SOCKET,(unsigned char*)"Could not remove the delegate}",0,0,""); \
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

  send_data(CLIENT_SOCKET,(unsigned char*)"Removed the delegate}",0,0,"");
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
  send_data(CLIENT_SOCKET,(unsigned char*)"Could not update the delegates information}",0,0,""); \
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

  send_data(CLIENT_SOCKET,(unsigned char*)"Updated the delegates information}",0,0,"");
  return 1;

  #undef DATABASE_COLLECTION
  #undef SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE_ERROR
}