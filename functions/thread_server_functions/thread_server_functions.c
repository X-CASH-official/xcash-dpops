#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <netdb.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "database_functions.h"
#include "define_macro_functions.h"
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
Name: current_block_height_timer_thread
Description: Gets the current block height and determines if a new round has started
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

void* current_block_height_timer_thread()
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  size_t count;
  int settings = 0;
  int block_verifier_settings;
  
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  /*sync_block_verifiers_minutes(0);
  get_current_block_height(current_block_height,0);
  if (start_new_round() == 0)
  {
    print_error_message;
  }
  else
  {
    printf("\033[1;32mNetwork Block %s Has Been Created Successfully\033[0m\n",current_block_height);
  }*/

  for (;;)
  {
    // pause 200 milliseconds and then check the time. If it is a possible block time check if their is a new block
    usleep(200000);
    get_current_UTC_time;
    if ((settings == 0 && current_UTC_date_and_time->tm_min % 5 == 0 && current_UTC_date_and_time->tm_sec == 0) || (settings == 1 && current_UTC_date_and_time->tm_min % 5 == 0))
    {
      if (settings == 0)
      {
        get_current_block_height(current_block_height,0);
        block_verifier_settings = start_new_round();
        if (block_verifier_settings == 0)
        {
          print_error_message;
        }
        else if (block_verifier_settings == 1)
        {
          printf("\033[1;31mYour delegate is not a block verifier for network block %s\033[0m\n",current_block_height);
        }
        else
        {
          printf("\033[1;32mNetwork Block %s Has Been Created Successfully\033[0m\n",current_block_height);
        }
        settings = 1;
        continue;
      }
      // try for the next 5 seconds and if not then a new block is not going to be added to the network
      for (count = 0; count < 5; count++)
      {
        get_current_block_height(data,0);
        if (memcmp(data,current_block_height,strlen(current_block_height)) != 0)
        {      
          // replace the current_block_height variable
          memset(current_block_height,0,strlen(current_block_height));
          memcpy(current_block_height,data,strnlen(data,BUFFER_SIZE));

          block_verifier_settings = start_new_round();
          if (block_verifier_settings == 0)
          {
            print_error_message;
          }
          else if (block_verifier_settings == 1)
          {
            printf("\033[1;31mYour delegate is not a block verifier for network block %s\033[0m\n",current_block_height);
          }
          else
          {
            printf("\033[1;32mNetwork Block %s Has Been Created Successfully\033[0m\n",current_block_height);
          }         
        }
        sleep(1);
      }
    }
  }
  pthread_exit((void *)(intptr_t)1);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: compare
Description: compare function for sorting
-----------------------------------------------------------------------------------------------------------
*/

int compare(const void* STRING1, const void* STRING2)
{
  return strcmp(STRING1, STRING2);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: check_reserve_proofs_timer_thread
Description: Randomly selects a reserve proof from the database and will check if it is valid
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

void* check_reserve_proofs_timer_thread()
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char* message = (char*)calloc(524288000,sizeof(char)); // 500 MB
  char* reserve_proofs[MAXIMUM_INVALID_RESERERVE_PROOFS];
  int count;
  int count2;
  size_t block_verifiers_score;
  int settings;
  struct send_data_socket_thread_parameters send_data_socket_thread_parameters[BLOCK_VERIFIERS_AMOUNT];
  struct database_multiple_documents_fields database_multiple_documents_fields;

  // threads
  pthread_t thread_id[BLOCK_VERIFIERS_AMOUNT];

  // define macros
  #define CHECK_RESERVE_PROOFS_TIMER_THREAD_ERROR(message) \
  memcpy(error_message.function[error_message.total],"check_reserve_proofs_timer_thread",33); \
  memcpy(error_message.data[error_message.total],message,sizeof(message)-1); \
  error_message.total++; \
  print_error_message;

  #define SEND_DATA_SOCKET_THREAD(message) \
  sleep(BLOCK_VERIFIERS_SETTINGS); \
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++) \
  { \
    if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0) \
    { \
      memset(send_data_socket_thread_parameters[count].HOST,0,sizeof(send_data_socket_thread_parameters[count].HOST)); \
      memset(send_data_socket_thread_parameters[count].DATA,0,sizeof(send_data_socket_thread_parameters[count].DATA)); \
      memcpy(send_data_socket_thread_parameters[count].HOST,current_block_verifiers_list.block_verifiers_IP_address[count],strnlen(current_block_verifiers_list.block_verifiers_IP_address[count],BUFFER_SIZE)); \
      memcpy(send_data_socket_thread_parameters[count].DATA,message,strnlen(message,BUFFER_SIZE)); \
      pthread_create(&thread_id[count], NULL, &send_data_socket_thread,&send_data_socket_thread_parameters[count]); \
      pthread_detach(thread_id[count]); \
    } \
    if (count % (BLOCK_VERIFIERS_AMOUNT / 4) == 0 && count != 0 && count != BLOCK_VERIFIERS_AMOUNT) \
    { \
       usleep(500000); \
    } \
  }

  // initialize the database_multiple_documents_fields struct 
  for (count = 0; count < TOTAL_RESERVE_PROOFS_DATABASE_FIELDS; count++)
  {
    database_multiple_documents_fields.item[0][count] = (char*)calloc(BUFFER_SIZE,sizeof(char));
    database_multiple_documents_fields.value[0][count] = (char*)calloc(BUFFER_SIZE,sizeof(char));

    if (database_multiple_documents_fields.item[0][count] == NULL || database_multiple_documents_fields.value[0][count] == NULL)
    {
      memcpy(error_message.function[error_message.total],"check_reserve_proofs_timer_thread",33);
      memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
      error_message.total++;
      print_error_message;  
      exit(0);
    }
  } 
  database_multiple_documents_fields.document_count = 0;
  database_multiple_documents_fields.database_fields_count = 0;

  // initialize the reserve_proofs array 
  for (count = 0; count < MAXIMUM_INVALID_RESERERVE_PROOFS; count++)
  {
    reserve_proofs[count] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));

    if (reserve_proofs[count] == NULL)
    {
      memcpy(error_message.function[error_message.total],"check_reserve_proofs_timer_thread",33);
      memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
      error_message.total++;
      print_error_message;  
      exit(0);
    }
  } 



  for (;;)
  {
    get_current_UTC_time;
    if (current_UTC_date_and_time->tm_min % 5 == 4 && current_UTC_date_and_time->tm_sec == 25)
    {
      // wait for any block verifiers sending messages, or any block verifiers waiting to process a reserve proof
      sync_block_verifiers_seconds(30);

      // copy all of the reserve proofs to the reserve_proofs array
      for (count = 0; count < MAXIMUM_INVALID_RESERERVE_PROOFS; count++)
      {
        memcpy(reserve_proofs[count],invalid_reserve_proofs.reserve_proof[count],strnlen(invalid_reserve_proofs.reserve_proof[count],BUFFER_SIZE_RESERVE_PROOF));
      }

      // sort the invalid reserve proofs struct so the data hash is the same
      qsort(reserve_proofs,sizeof(reserve_proofs)/sizeof(reserve_proofs[0]),sizeof(reserve_proofs[0]),compare);

      // get the data hash of the invalid_reserve_proofs struct
      memset(message,0,strlen(message));
      memset(data,0,sizeof(data));
      memset(data2,0,sizeof(data2));
      if (invalid_reserve_proofs.count > 0)
      {
        for (count = 0; count < MAXIMUM_INVALID_RESERERVE_PROOFS; count++)
        {
          if (memcmp(reserve_proofs[count],"",1) != 0)
          {
            memcpy(message+strlen(message),reserve_proofs[count],strnlen(reserve_proofs[count],BUFFER_SIZE_RESERVE_PROOF));
          }          
        }
      }
      else
      {
        memcpy(message,"XCASH",5);
      }
      
      crypto_hash_sha512((unsigned char*)data,(const unsigned char*)message,(unsigned long long)strnlen(message,524288000));

      // convert the SHA512 data hash to a string
      for (count2 = 0, count = 0; count2 < DATA_HASH_LENGTH / 2; count2++, count += 2)
      {
        snprintf(data2+count,sizeof(data2)-1,"%02x",data[count2] & 0xFF);
      }

      // reset the current_round_part_vote_data.vote_results_valid struct
      memset(current_round_part_vote_data.current_vote_results,0,sizeof(current_round_part_vote_data.current_vote_results));
      current_round_part_vote_data.vote_results_valid = 1;
      current_round_part_vote_data.vote_results_invalid = 0;

      memcpy(current_round_part_vote_data.current_vote_results,data2,DATA_HASH_LENGTH);

      // create the message
      memset(data2,0,sizeof(data2));
      memcpy(data2,"{\r\n \"message_settings\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n \"vote_settings\": \"valid\",\r\n \"vote_data\": \"",99);  
      memcpy(data2+strlen(data2),current_round_part_vote_data.current_vote_results,DATA_HASH_LENGTH);
      memcpy(data2+strlen(data2),"\",\r\n}",5); 

      // sign_data
      if (sign_data(data2,0) == 0)
      { 
        // reset the invalid_reserve_proofs and the block_verifiers_invalid_reserve_proofs
        for (count = 0; count < MAXIMUM_INVALID_RESERERVE_PROOFS; count++)
        {
          memset(invalid_reserve_proofs.block_verifier_public_address[count],0,strlen(invalid_reserve_proofs.block_verifier_public_address[count]));
          memset(invalid_reserve_proofs.public_address[count],0,strlen(invalid_reserve_proofs.public_address[count]));
          memset(invalid_reserve_proofs.reserve_proof[count],0,strlen(invalid_reserve_proofs.reserve_proof[count]));      
        }
        invalid_reserve_proofs.count = 0;
        continue;
      }

      // send the message to all block verifiers
      SEND_DATA_SOCKET_THREAD(data2);

      // wait for the block verifiers to process the votes
      sync_block_verifiers_seconds(40);

      // process the vote results
      if (current_round_part_vote_data.vote_results_valid < BLOCK_VERIFIERS_VALID_AMOUNT)
      {
        // reset the invalid_reserve_proofs and the block_verifiers_invalid_reserve_proofs
        for (count = 0; count < MAXIMUM_INVALID_RESERERVE_PROOFS; count++)
        {
          memset(invalid_reserve_proofs.block_verifier_public_address[count],0,strlen(invalid_reserve_proofs.block_verifier_public_address[count]));
          memset(invalid_reserve_proofs.public_address[count],0,strlen(invalid_reserve_proofs.public_address[count]));
          memset(invalid_reserve_proofs.reserve_proof[count],0,strlen(invalid_reserve_proofs.reserve_proof[count]));      
        }
        invalid_reserve_proofs.count = 0;
        continue;
      }

      // delete all of the reserve proofs in the database
      for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
      {
        memset(data,0,sizeof(data));
        memcpy(data,"reserve_proofs_",15);
        snprintf(data+15,sizeof(data)-1,"%d",count);
        for (count2 = 0; count2 < invalid_reserve_proofs.count; count2++)
        {
          memset(data2,0,sizeof(data2));
          memcpy(data2,"{\"reserve_proof\":\"",18);
          memcpy(data2+18,invalid_reserve_proofs.reserve_proof[count2],strnlen(invalid_reserve_proofs.reserve_proof[count2],sizeof(data2)));
          memcpy(data2+strlen(data2),"\"}",2);
          delete_document_from_collection(DATABASE_NAME,data,data2,0);
        }       
      }
      
      // update all of the block verifiers score
      for (count2 = 0; count2 < invalid_reserve_proofs.count; count2++)
      {
        // create the message
        memset(data,0,sizeof(data));
        memcpy(data,"{\"public_address\":\"",18);
        memcpy(data+18,invalid_reserve_proofs.block_verifier_public_address[count2],strnlen(invalid_reserve_proofs.block_verifier_public_address[count2],sizeof(data)));
        memcpy(data+strlen(data),"\"}",2);

        // get the block verifiers score
        memset(data2,0,sizeof(data2));
        if (read_document_field_from_collection(DATABASE_NAME,"delegates",data,"block_verifiers_score",data2,0) == 0)
        {
          CHECK_RESERVE_PROOFS_TIMER_THREAD_ERROR("Could not update a block verifiers score. This means the delegates database might be unsynced, and you might have to sync the database");
        }
        sscanf(data2, "%zu", &block_verifiers_score);
        block_verifiers_score++;

        memset(data2,0,sizeof(data2));
        memcpy(data2,"{\"block_verifiers_score\":\"",26);
        snprintf(data2+26,sizeof(data2)-1,"%zu",block_verifiers_score);
        memcpy(data2+strlen(data2),"\"}",2);

        pthread_rwlock_rdlock(&rwlock_reserve_proofs);
        while(database_settings != 1)
        {
          sleep(1);
        }
        pthread_rwlock_unlock(&rwlock_reserve_proofs);
        if (update_document_from_collection(DATABASE_NAME,"delegates",data,data2,0) == 0)
        {
          CHECK_RESERVE_PROOFS_TIMER_THREAD_ERROR("Could not update a block verifiers score. This means the delegates database might be unsynced, and you might have to sync the database");
        }
      }
      
      // reset the invalid_reserve_proofs and the block_verifiers_invalid_reserve_proofs
      for (count = 0; count < MAXIMUM_INVALID_RESERERVE_PROOFS; count++)
      {
        memset(invalid_reserve_proofs.block_verifier_public_address[count],0,strlen(invalid_reserve_proofs.block_verifier_public_address[count]));
        memset(invalid_reserve_proofs.public_address[count],0,strlen(invalid_reserve_proofs.public_address[count]));
        memset(invalid_reserve_proofs.reserve_proof[count],0,strlen(invalid_reserve_proofs.reserve_proof[count]));      
      }
      invalid_reserve_proofs.count = 0;
    }



    // reset the database_multiple_documents_fields
    for (count = 0; count < TOTAL_RESERVE_PROOFS_DATABASE_FIELDS; count++)
    {
      memset(database_multiple_documents_fields.item[0][count],0,strlen(database_multiple_documents_fields.item[0][count]));
      memset(database_multiple_documents_fields.value[0][count],0,strlen(database_multiple_documents_fields.value[0][count]));
    }

    // select a random reserve proofs collection
    memset(data,0,sizeof(data));
    memcpy(data,"reserve_proofs_",15);
    snprintf(data+15,sizeof(data)-1,"%d",((rand() % (TOTAL_RESERVE_PROOFS_DATABASES - 1 + 1)) + 1)); 

    // select a random document in the collection
    count = count_all_documents_in_collection(DATABASE_NAME,data,0);
    if (count > 0)
    {
      count = (rand() % count) + 1;
    }
    else
    {
      continue;
    }   

    // get a random document from the collection
    if (read_multiple_documents_all_fields_from_collection(DATABASE_NAME,data,"",&database_multiple_documents_fields,count,1,0,"",0) == 1)
    {
      // check if the reserve proof is valid
      memset(data,0,sizeof(data));
      if (check_reserve_proofs(data,database_multiple_documents_fields.value[0][0],database_multiple_documents_fields.value[0][3],0) == 0)
      {
        // add the reserve proof to the invalid_reserve_proofs struct
        pthread_rwlock_wrlock(&rwlock);
        memcpy(invalid_reserve_proofs.block_verifier_public_address[invalid_reserve_proofs.count],xcash_wallet_public_address,XCASH_WALLET_LENGTH);
        memcpy(invalid_reserve_proofs.public_address[invalid_reserve_proofs.count],database_multiple_documents_fields.value[0][0],XCASH_WALLET_LENGTH);
        memcpy(invalid_reserve_proofs.reserve_proof[invalid_reserve_proofs.count],database_multiple_documents_fields.value[0][3],strnlen(database_multiple_documents_fields.value[0][3],BUFFER_SIZE_RESERVE_PROOF));
        invalid_reserve_proofs.count++;
        pthread_rwlock_unlock(&rwlock);

        // send the reserve proof to all block verifiers
        // create the message
        memset(data,0,sizeof(data));
        memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS\",\r\n \"public_address_that_created_the_reserve_proof\": \"",137);
        memcpy(data+strlen(data),invalid_reserve_proofs.public_address[invalid_reserve_proofs.count-1],strnlen(invalid_reserve_proofs.public_address[invalid_reserve_proofs.count-1],sizeof(data)));
        memcpy(data+strlen(data),"\",\r\n \"reserve_proof\": \"",23);
        memcpy(data+strlen(data),invalid_reserve_proofs.reserve_proof[invalid_reserve_proofs.count-1],strnlen(invalid_reserve_proofs.reserve_proof[invalid_reserve_proofs.count-1],sizeof(data)));
        memcpy(data+strlen(data),"\",\r\n}",5);

        // sign_data
        if (sign_data(data,0) == 0)
        { 
          CHECK_RESERVE_PROOFS_TIMER_THREAD_ERROR("Could not sign_data. This means the reserve proofs database might be unsynced, and you might have to sync the database.");
        }

        // send the message to all block verifiers
        SEND_DATA_SOCKET_THREAD(data);
      }
    }      
  }
  pointer_reset(message);
  pthread_exit((void *)(intptr_t)1);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: check_delegates_online_status_timer_thread
Description: Checks the top 150 delegates every round to update their online status
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

void* check_delegates_online_status_timer_thread()
{
  // Variables
  char message[BUFFER_SIZE];
  char data[BUFFER_SIZE];
  size_t count;
  size_t count2;
  struct database_multiple_documents_fields database_multiple_documents_fields;
  
  memset(message,0,sizeof(message));
  memset(data,0,sizeof(data));

  // initialize the database_multiple_documents_fields struct 
  for (count = 0; count < BLOCK_VERIFIERS_SELECTED_AMOUNT; count++)
  {
    for (count2 = 0; count2 < 19; count2++)
    {
      database_multiple_documents_fields.item[count][count2] = (char*)calloc(BUFFER_SIZE,sizeof(char));
      database_multiple_documents_fields.value[count][count2] = (char*)calloc(BUFFER_SIZE,sizeof(char));
      if (database_multiple_documents_fields.item[count][count2] == NULL || database_multiple_documents_fields.value[count][count2] == NULL)
      {
        memcpy(error_message.function[error_message.total],"check_delegates_online_status_timer_thread",42);
        memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
        error_message.total++;
        print_error_message;  
        exit(0);
      }
    } 
  } 
  database_multiple_documents_fields.document_count = 0;
  database_multiple_documents_fields.database_fields_count = 0;



  for (;;)
  {    
    get_current_UTC_time;
    if (current_UTC_date_and_time->tm_min % 5 == 1 && current_UTC_date_and_time->tm_sec == 0)
    {
      // get the top 150 delegates by total votes
      if (read_multiple_documents_all_fields_from_collection(DATABASE_NAME,"delegates","",&database_multiple_documents_fields,1,BLOCK_VERIFIERS_SELECTED_AMOUNT,1,"total_vote_count",0) == 0)
      {
        memcpy(error_message.function[error_message.total],"check_delegates_online_status_timer_thread",42);
        memcpy(error_message.data[error_message.total],"Could not get the top 150 delegates to check their online status. This means the delegates database might be unsynced, and you might have to sync the database.",159);
        error_message.total++;
        print_error_message;
      }     

      // check the online status of the top 150 delegates
      for (count = 0; count < database_multiple_documents_fields.document_count; count++)
      {
         // create the message
         memset(message,0,sizeof(message));
         memcpy(message,"{\"public_address\":\"",19);
         memcpy(message+19,database_multiple_documents_fields.value[count][0],XCASH_WALLET_LENGTH);
         memcpy(message+19+XCASH_WALLET_LENGTH,"\"}",2);

         if (get_delegate_online_status(database_multiple_documents_fields.value[count][3]) == 1)
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
         if (update_document_from_collection(DATABASE_NAME,"delegates",message,data,0) == 0)
         {
           memcpy(error_message.function[error_message.total],"check_delegates_online_status_timer_thread",42);
           memcpy(error_message.data[error_message.total],"Could not update a delegates online status. This means the delegates database might be unsynced, and you might have to sync the database",136);
           error_message.total++;
           print_error_message;
         }     
       }

      // reset the database_multiple_documents_fields
      for (count = 0; count < BLOCK_VERIFIERS_SELECTED_AMOUNT; count++)
      {
        for (count2 = 0; count2 < 19; count2++)
        {
          memset(database_multiple_documents_fields.item[count][count2],0,strlen(database_multiple_documents_fields.item[count][count2]));
          memset(database_multiple_documents_fields.value[count][count2],0,strlen(database_multiple_documents_fields.value[count][count2]));
        }
      }
      database_multiple_documents_fields.document_count = 0;
      database_multiple_documents_fields.database_fields_count = 0;
    }
    usleep(200000);
  }
  pthread_exit((void *)(intptr_t)1);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: send_data_socket_thread
Description: Send a message through a socket on a separate thread
Parameters:
  parameters - A pointer to the send_data_socket_thread_parameters struct
  struct send_data_socket_thread_parameters
    HOST - The host to send the message to  
    DATA - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

void* send_data_socket_thread(void* parameters)
{   
  // Variables  
  struct send_data_socket_thread_parameters* data = (struct send_data_socket_thread_parameters*)parameters;
  size_t HOST_LENGTH = strnlen(data->HOST,BUFFER_SIZE);
  struct timeval SOCKET_TIMEOUT = {SOCKET_CONNECTION_TIMEOUT_SETTINGS, 0};   
  char buffer2[BUFFER_SIZE];
  char str[BUFFER_SIZE];
  char message[BUFFER_SIZE];
  struct sockaddr_in serv_addr;
  struct pollfd socket_file_descriptors;
  int socket_settings;
  socklen_t socket_option_settings = sizeof(socket_settings);

  // define macros
  #define SEND_DATA_SOCKET_ERROR \
  close(SOCKET); \
  pthread_exit((void *)(intptr_t)0); 

  memset(buffer2,0,sizeof(buffer2));
  memset(str,0,sizeof(str));
  memset(message,0,sizeof(message));

  /* Create the socket  
  AF_INET = IPV4 support
  SOCK_STREAM = TCP protocol
  SOCK_NONBLOCK = Set the socket to non blocking mode, so it will use the timeout settings when connecting
  */
  const int SOCKET = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  if (SOCKET == -1)
  { 
    SEND_DATA_SOCKET_ERROR;
  }

  /* Set the socket options for sending and receiving data
  SOL_SOCKET = socket level
  SO_RCVTIMEO = allow the socket on receiving data, to use the timeout settings
  */
  if (setsockopt(SOCKET, SOL_SOCKET, SO_RCVTIMEO,(struct timeval *)&SOCKET_TIMEOUT, sizeof(struct timeval)) != 0)
  { 
    SEND_DATA_SOCKET_ERROR;
  } 

  // convert the hostname if used, to an IP address
  memset(str,0,sizeof(str));
  memcpy(str,data->HOST,strnlen(data->HOST,sizeof(str)));
  string_replace(str,sizeof(str),"http://","");
  string_replace(str,sizeof(str),"https://","");
  string_replace(str,sizeof(str),"www.",""); 
  const struct hostent* HOST_NAME = gethostbyname(str); 
  if (HOST_NAME == NULL)
  {
    SEND_DATA_SOCKET_ERROR;
  }
    
  // convert the port to a string  
  snprintf(buffer2,sizeof(buffer2)-1,"%d",SEND_DATA_PORT); 
   
  const size_t BUFFER2_LENGTH = strnlen(buffer2,BUFFER_SIZE);
  
  /* setup the connection
  AF_INET = IPV4
  use htons to convert the port from host byte order to network byte order short
  */
  memset(&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr*)HOST_NAME->h_addr_list[0])));
  serv_addr.sin_port = htons(SEND_DATA_PORT);

  /* set the first poll structure to our socket
  POLLOUT - set it to POLLOUT since the socket is non blocking and it can write data to the socket
  */
  socket_file_descriptors.fd = SOCKET;
  socket_file_descriptors.events = POLLOUT;

  // connect to the socket
  if (connect(SOCKET,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) != 0)
  {    
    if (poll(&socket_file_descriptors,1,TOTAL_CONNECTION_TIME_SETTINGS) == 1 && getsockopt(SOCKET,SOL_SOCKET,SO_ERROR,&socket_settings,&socket_option_settings) == 0)
    {   
      if (socket_settings != 0)
      {  
        SEND_DATA_SOCKET_ERROR;
      } 
    }
  }

  // get the current socket settings
  socket_settings = fcntl(SOCKET, F_GETFL, NULL);
  if (socket_settings == -1)
  {
    SEND_DATA_SOCKET_ERROR;
  }

  // set the socket to blocking mode
  socket_settings &= (~O_NONBLOCK);
  if (fcntl(SOCKET, F_SETFL, socket_settings) == -1)
  {
    SEND_DATA_SOCKET_ERROR;
  }

  // send the message   
  memcpy(message,data->DATA,strnlen(data->DATA,BUFFER_SIZE));
  memcpy(message+strlen(message),SOCKET_END_STRING,sizeof(SOCKET_END_STRING));
  memcpy(str,"Sending ",8);
  memcpy(str+8,&message[25],strlen(message) - strlen(strstr(message,"\",\r\n")) - 25);
  memcpy(str+strlen(str)," to ",4);
  memcpy(str+strlen(str),data->HOST,HOST_LENGTH);
  memcpy(str+strlen(str)," on port ",9);
  memcpy(str+strlen(str),buffer2,strnlen(buffer2,BUFFER_SIZE));
  memcpy(str+strlen(str),"\n",1);
  memcpy(str+strlen(str),asctime(current_UTC_date_and_time),strnlen(asctime(current_UTC_date_and_time),BUFFER_SIZE));
  color_print(str,"green");
  memset(str,0,sizeof(str));

  const int TOTAL = strnlen(message,BUFFER_SIZE);
  int sent = 0;
  int bytes = 0;
  do {
    bytes = write(SOCKET,message+sent,TOTAL-sent);
    if (bytes < 0)
    { 
      SEND_DATA_SOCKET_ERROR;
    }
    else if (bytes == 0)  
    {
      break;
    }
    sent+=bytes;
    } while (sent < TOTAL);
    close(SOCKET);
    
  pthread_exit((void *)(intptr_t)1);

  #undef SEND_DATA_SOCKET_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: send_and_receive_data_socket_thread
Description: Send a message through a socket and receives the message on a separate thread
Parameters:
  parameters - A pointer to the send_and_receive_data_socket_thread_parameters struct
  struct send_and_receive_data_socket_thread_parameters
    HOST - The host to send the message to  
    DATA - The message
    COUNT - The block verifier that received the message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

void* send_and_receive_data_socket_thread(void* parameters)
{   
  // Variables  
  struct send_and_receive_data_socket_thread_parameters* data = (struct send_and_receive_data_socket_thread_parameters*)parameters;
  size_t HOST_LENGTH = strnlen(data->HOST,BUFFER_SIZE);
  struct timeval SOCKET_TIMEOUT = {SOCKET_CONNECTION_TIMEOUT_SETTINGS, 0};  
  char buffer[BUFFER_SIZE]; 
  char buffer2[BUFFER_SIZE];
  char str[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char message[BUFFER_SIZE];
  int count;
  struct sockaddr_in serv_addr;
  struct pollfd socket_file_descriptors;
  int socket_settings;
  socklen_t socket_option_settings = sizeof(socket_settings);

  // define macros
  #define SEND_AND_RECEIVE_DATA_SOCKET_ERROR \
  close(SOCKET); \
  pthread_exit((void *)(intptr_t)0); 

  memset(buffer,0,sizeof(buffer));
  memset(buffer2,0,sizeof(buffer2));
  memset(str,0,sizeof(str));
  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));

  /* Create the socket  
  AF_INET = IPV4 support
  SOCK_STREAM = TCP protocol
  SOCK_NONBLOCK = Set the socket to non blocking mode, so it will use the timeout settings when connecting
  */
  const int SOCKET = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  if (SOCKET == -1)
  { 
    SEND_AND_RECEIVE_DATA_SOCKET_ERROR;
  }

  /* Set the socket options for sending and receiving data
  SOL_SOCKET = socket level
  SO_RCVTIMEO = allow the socket on receiving data, to use the timeout settings
  */
  if (setsockopt(SOCKET, SOL_SOCKET, SO_RCVTIMEO,(struct timeval *)&SOCKET_TIMEOUT, sizeof(struct timeval)) != 0)
  { 
    SEND_AND_RECEIVE_DATA_SOCKET_ERROR;
  } 

  // convert the hostname if used, to an IP address
  memset(str,0,sizeof(str));
  memcpy(str,data->HOST,strnlen(data->HOST,sizeof(str)));
  string_replace(str,sizeof(str),"http://","");
  string_replace(str,sizeof(str),"https://","");
  string_replace(str,sizeof(str),"www.",""); 
  const struct hostent* HOST_NAME = gethostbyname(str); 
  if (HOST_NAME == NULL)
  {
    SEND_AND_RECEIVE_DATA_SOCKET_ERROR;
  }
    
  // convert the port to a string  
  snprintf(buffer2,sizeof(buffer2)-1,"%d",SEND_DATA_PORT); 
   
  const size_t BUFFER2_LENGTH = strnlen(buffer2,BUFFER_SIZE);
  
  /* setup the connection
  AF_INET = IPV4
  use htons to convert the port from host byte order to network byte order short
  */
  memset(&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr*)HOST_NAME->h_addr_list[0])));
  serv_addr.sin_port = htons(SEND_DATA_PORT);

  /* set the first poll structure to our socket
  POLLOUT - set it to POLLOUT since the socket is non blocking and it can write data to the socket
  */
  socket_file_descriptors.fd = SOCKET;
  socket_file_descriptors.events = POLLOUT;

  // connect to the socket
  if (connect(SOCKET,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) != 0)
  {    
    if (poll(&socket_file_descriptors,1,TOTAL_CONNECTION_TIME_SETTINGS) == 1 && getsockopt(SOCKET,SOL_SOCKET,SO_ERROR,&socket_settings,&socket_option_settings) == 0)
    {   
      if (socket_settings != 0)
      {  
        SEND_AND_RECEIVE_DATA_SOCKET_ERROR;
      } 
    }
  }

  // get the current socket settings
  socket_settings = fcntl(SOCKET, F_GETFL, NULL);
  if (socket_settings == -1)
  {
    SEND_AND_RECEIVE_DATA_SOCKET_ERROR;
  }

  // set the socket to blocking mode
  socket_settings &= (~O_NONBLOCK);
  if (fcntl(SOCKET, F_SETFL, socket_settings) == -1)
  {
    SEND_AND_RECEIVE_DATA_SOCKET_ERROR;
  }

  // send the message   
  memcpy(message,data->DATA,strnlen(data->DATA,BUFFER_SIZE));
  memcpy(message+strlen(message),SOCKET_END_STRING,sizeof(SOCKET_END_STRING));
  memcpy(str,"Sending ",8);
  memcpy(str+8,&message[25],strlen(message) - strlen(strstr(message,"\",\r\n")) - 25);
  memcpy(str+strlen(str)," to ",4);
  memcpy(str+strlen(str),data->HOST,HOST_LENGTH);
  memcpy(str+strlen(str)," on port ",9);
  memcpy(str+strlen(str),buffer2,strnlen(buffer2,BUFFER_SIZE));
  memcpy(str+strlen(str),"\n",1);
  memcpy(str+strlen(str),asctime(current_UTC_date_and_time),strnlen(asctime(current_UTC_date_and_time),BUFFER_SIZE));
  color_print(str,"green");
  memset(str,0,sizeof(str));
 
  const int TOTAL = strnlen(message,BUFFER_SIZE);
  int sent = 0;
  int bytes = 0;
  do {
    bytes = write(SOCKET,message+sent,TOTAL-sent);
    if (bytes < 0)
    { 
      SEND_AND_RECEIVE_DATA_SOCKET_ERROR;
    }
    else if (bytes == 0)  
    {
      break;
    }
    sent+=bytes;
    } while (sent < TOTAL);

  // receive the data
  memset(message,0,sizeof(message));
  for (;;)
  { 
    memset(&buffer, 0, sizeof(buffer));
    // check the size of the data that were about to receive. If it is over BUFFER_SIZE then dont accept it, since it will cause a buffer overflow
    if ((recvfrom(SOCKET, buffer, BUFFER_SIZE, MSG_DONTWAIT | MSG_PEEK, NULL, NULL) >= sizeof(buffer) - strnlen(message,sizeof(buffer)) && strnlen(message,sizeof(buffer)) > 0) || (recvfrom(SOCKET, buffer, BUFFER_SIZE, MSG_DONTWAIT | MSG_PEEK, NULL, NULL) >= sizeof(buffer) && strnlen(message,sizeof(buffer)) == 0))
    {
      SEND_AND_RECEIVE_DATA_SOCKET_ERROR;
    }    
    // read the socket to see if there is any data, use MSG_DONTWAIT so we dont block the program if there is no data
    recvfrom(SOCKET, buffer, BUFFER_SIZE, MSG_DONTWAIT, NULL, NULL);  
    if (buffer[0] != '\0' && strstr(buffer,SOCKET_END_STRING) == NULL)
    {
      // there is data, but this is not the final data
      memcpy(message,buffer,strnlen(buffer,BUFFER_SIZE));
    }
    if (buffer[0] != '\0' && strstr(buffer,SOCKET_END_STRING) != NULL)
    {
      // there is data, and this is the final data
      memcpy(message,buffer,strnlen(buffer,BUFFER_SIZE));
      // if the final message has the SOCKET_END_STRING in the message, remove it
      if (strstr(message,SOCKET_END_STRING) != NULL)
      {
        // remove SOCKET_END_STRING from the message
        memset(data2,0,sizeof(data2));
        memcpy(data2,message,strnlen(message,BUFFER_SIZE) - (sizeof(SOCKET_END_STRING)-1));
      }
      break;
    }

    // check for a timeout in receiving data
    count++;
    if (count > (RECEIVE_DATA_TIMEOUT_SETTINGS * 5))
    {
      SEND_AND_RECEIVE_DATA_SOCKET_ERROR;
    }
    usleep(200000);   
  }

  // verify the data
  if (verify_data(data2,0,1) == 0)
  {
    SEND_AND_RECEIVE_DATA_SOCKET_ERROR;
  }

  // parse the message
  memset(message,0,sizeof(message));
  memset(VRF_data.block_blob_signature[data->COUNT],0,strnlen(VRF_data.block_blob_signature[data->COUNT],BUFFER_SIZE));
  memset(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[data->COUNT],0,strnlen(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[data->COUNT],BUFFER_SIZE));
  if (parse_json_data(data2,"block_blob_signature",message,sizeof(message)) == 1 && strlen(message) == XCASH_SIGN_DATA_LENGTH && memcmp(message,XCASH_SIGN_DATA_PREFIX,sizeof(XCASH_SIGN_DATA_PREFIX)-1) == 0)
  {
    memcpy(VRF_data.block_blob_signature[data->COUNT],message,XCASH_SIGN_DATA_LENGTH);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[data->COUNT],message,XCASH_SIGN_DATA_LENGTH);
  }
  else
  {  
    memcpy(VRF_data.block_blob_signature[data->COUNT],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE)-1);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[data->COUNT],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE)-1);
  }
  pthread_exit((void *)(intptr_t)1);
  
  #undef SEND_AND_RECEIVE_DATA_SOCKET_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: socket_receive_data_thread
Description: socket receive data thread
-----------------------------------------------------------------------------------------------------------
*/

void* socket_receive_data_thread(void* parameters)
{
  // Constants
  const int CONNECTIONS_PER_THREAD = MAXIMUM_CONNECTIONS / get_nprocs();
  
  // Variables
  struct epoll_event* events = calloc(CONNECTIONS_PER_THREAD, sizeof(struct epoll_event));
  int count;
  int count2;

  // define macros
  #define SOCKET_RECEIVE_DATA_THREAD_ERROR(message) \
  memcpy(error_message.function[error_message.total],"socket_receive_data_thread",26); \
  memcpy(error_message.data[error_message.total],message,strnlen(message,BUFFER_SIZE)); \
  error_message.total++; \
  pthread_exit((void *)(intptr_t)1);

  if (events == NULL)
  {
    SOCKET_RECEIVE_DATA_THREAD_ERROR("Could not allocate the memory needed on the heap");
  }

  /* get the events that have a ready signal
  set the timeout settings to -1 to wait until any file descriptor is ready
  */ 
 for (;;)
 { 
  while ((count = epoll_wait(epoll_fd, events, CONNECTIONS_PER_THREAD, 100)) > 0)
  {
    for (count2 = 0; count2 < count; count2++)
    {
      if (events[count2].data.fd == server_socket)
      {
        // a file descriptor is ready for a new socket connection
        new_socket_thread();
      }
      else
      {
        // a file descriptor is ready for a current socket connection
       socket_thread(events[count2].data.fd);
       close(events[count2].data.fd);
      }
    }
  }
}
pointer_reset(events);
pthread_exit((void *)(intptr_t)1);
}