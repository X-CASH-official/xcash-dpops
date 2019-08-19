#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
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
#include "vrf.h"
#include "crypto_vrf.h"
#include "VRF_functions.h"

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
  
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  sync_block_verifiers_minutes(0);
  get_current_block_height(current_block_height,0);
  if (start_new_round() == 0)
  {
    print_error_message;
  }
  else
  {
    printf("\033[1;32mNetwork Block %s Has Been Created Successfully\033[0m\n",current_block_height);
  } 
  /*for (;;)
  {
    start:
    // pause 200 milliseconds and then check the time. If it is a possible block time check if their is a new block
    usleep(200000);
    get_current_UTC_time;
    if (current_UTC_date_and_time->tm_min % 5 == 0)
    {
      // try for the next 5 seconds and if not then a new block is not going to be added to the network
      for (count = 0; count < 5; count++)
      {
        get_current_block_height(data,0);
        if (memcmp(data,current_block_height,strlen(current_block_height)) != 0)
        {      
          // replace the current_block_height variable
          memset(current_block_height,0,strlen(current_block_height));
          memcpy(current_block_height,data,strnlen(data,BUFFER_SIZE));

          memcpy(data2,"Network Block ",14);
          memcpy(data2+14,data,strnlen(data,BUFFER_SIZE));
          print_start_message(data2);

          if (start_new_round() == 0)
          {
            print_error_message;
          }
          else
          {
            printf("\033[1;32mNetwork Block %s Has Been Created Successfully\033[0m\n",data);
          }          
          goto start;
        }
        sleep(1);
      }
    }
  }*/
  pthread_exit((void *)(intptr_t)1);
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
  char* message = (char*)calloc(15728640,sizeof(char)); // 15 MB
  int count;
  int count2;
  size_t block_verifiers_score;
  int settings;
  struct database_multiple_documents_fields database_multiple_documents_fields;

  // check if the memory needed was allocated on the heap successfully
  if (message == NULL)
  {
    pointer_reset(message);
    memcpy(error_message.function[error_message.total],"check_reserve_proofs_timer_thread",33);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  pthread_rwlock_wrlock(&rwlock);
  reserve_proofs_settings = 1;
  main_network_data_node_create_block = 0;
  pthread_rwlock_unlock(&rwlock);

  // initialize the database_multiple_documents_fields struct 
  for (count = 0; count < 4; count++)
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



  for (;;)
  {
    get_current_UTC_time;
    if (current_UTC_date_and_time->tm_min % 4 == 0 && current_UTC_date_and_time->tm_sec < 5)
    {
      // send the invalid_reserve_proofs struct to all block verifiers
      // create the message
      memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS\",\r\n \"public_address_that_created_the_reserve_proof\": \"",137);
      count = 137;
      for (count2 = 0; count2 < invalid_reserve_proofs.count; count2++)
      {
        memcpy(message+count,invalid_reserve_proofs.public_address[count],strnlen(invalid_reserve_proofs.public_address[count],XCASH_WALLET_LENGTH));
        count += strnlen(invalid_reserve_proofs.public_address[count],XCASH_WALLET_LENGTH);
        memcpy(message+count,"|",1);
        count += 1;
      }
      memcpy(message+count-1,"\",\r\n \"reserve_proof\": \"",23);
      count += 17;
      for (count2 = 0; count2 < invalid_reserve_proofs.count; count2++)
      {
        memcpy(message+count,invalid_reserve_proofs.reserve_proof[count],strnlen(invalid_reserve_proofs.reserve_proof[count],BUFFER_SIZE_RESERVE_PROOF));
        count += strnlen(invalid_reserve_proofs.reserve_proof[count],BUFFER_SIZE_RESERVE_PROOF);
        memcpy(message+count,"|",1);
        count += 1;
      }
      memcpy(message+count-1,"\",\r\n}",5);

      // sign_data
      memset(data,0,sizeof(data));
      if (sign_data(message,0) == 0)
      { 
        memcpy(error_message.function[error_message.total],"check_reserve_proofs_timer_thread",33);
        memcpy(error_message.data[error_message.total],"Could not sign_data. This means the reserve proofs database might be unsynced, and you might have to sync the database.",119);
        error_message.total++;
        print_error_message;
      }

      // send the message to all block verifiers
      for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
      {
        if (memcmp(current_block_verifiers_list.block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
        {
          send_data_socket(current_block_verifiers_list.block_verifiers_IP_address[count],SEND_DATA_PORT,message);
        }
      }
      
      // wait for all of the other block verifiers to send you their invalid_reserve_proofs
      sleep(10);

      pthread_rwlock_wrlock(&rwlock);
      // set the reserve_proofs_settings so their are no reserve proofs added to the database
      reserve_proofs_settings = 0;

      // at this point the block verifier will have added all other reserve proofs to the invalid_reserve_proofs struct, so now we need to delete all of the reserve proofs in the collections
      for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
      {
        memset(data,0,sizeof(data));
        memcpy(data,"reserve_proofs_",15);
        sprintf(data+15,"%d",count);
        for (count2 = 0; count2 < invalid_reserve_proofs.count; count2++)
        {
          memset(message,0,strlen(message));
          memcpy(message,"{\"reserve_proof\":\"",18);
          memcpy(message+18,invalid_reserve_proofs.reserve_proof[count],strnlen(invalid_reserve_proofs.reserve_proof[count],BUFFER_SIZE_RESERVE_PROOF));
          memcpy(message+18+strnlen(invalid_reserve_proofs.reserve_proof[count],BUFFER_SIZE_RESERVE_PROOF),"\"}",2);
          delete_document_from_collection(DATABASE_NAME,data,"",0);
        }       
      }

      // set the reserve_proofs_settings so reserve proofs can be added to the database
      reserve_proofs_settings = 1;
      pthread_rwlock_unlock(&rwlock);

      // update all of the block verifiers score
      for (count2 = 0; count2 < invalid_reserve_proofs.count; count2++)
      {
        // create the message
        memset(message,0,strlen(message));
        memcpy(message,"{\"public_address\":\"",18);
        memcpy(message+18,invalid_reserve_proofs.block_verifier_public_address[count2],strnlen(invalid_reserve_proofs.block_verifier_public_address[count2],XCASH_WALLET_LENGTH));
        memcpy(message+18+strnlen(invalid_reserve_proofs.block_verifier_public_address[count2],XCASH_WALLET_LENGTH),"\"}",2);

        // get the block verifiers score
        memset(data2,0,sizeof(data2));
        if (read_document_field_from_collection(DATABASE_NAME,"delegates",message,"block_verifiers_score",data2,0) == 0)
        {
          memcpy(error_message.function[error_message.total],"check_reserve_proofs_timer_thread",33);
          memcpy(error_message.data[error_message.total],"Could not update a block verifiers score. This means the delegates database might be unsynced, and you might have to sync the database.",135);
          error_message.total++;
        }
        sscanf(data2, "%zu", &block_verifiers_score);
        block_verifiers_score++;

        memset(data,0,sizeof(data));
        memcpy(data,"{\"block_verifiers_score\":\"",26);
        sprintf(data2+26,"%zu",block_verifiers_score);
        memcpy(data+strlen(data),"\"}",2);

        if (update_document_from_collection(DATABASE_NAME,"delegates",message,data,0) == 0)
        {
          memcpy(error_message.function[error_message.total],"check_reserve_proofs_timer_thread",33);
          memcpy(error_message.data[error_message.total],"Could not update a block verifiers score. This means the delegates database might be unsynced, and you might have to sync the database.",135);
          error_message.total++;
          print_error_message;
        }
      }
      
      // reset the invalid_reserve_proofs and the block_verifiers_invalid_reserve_proofs
      for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
      {
        memset(invalid_reserve_proofs.block_verifier_public_address[count],0,strlen(invalid_reserve_proofs.block_verifier_public_address[count]));
        memset(invalid_reserve_proofs.public_address[count],0,strlen(invalid_reserve_proofs.public_address[count]));
        memset(invalid_reserve_proofs.reserve_proof[count],0,strlen(invalid_reserve_proofs.reserve_proof[count]));      
      }
      invalid_reserve_proofs.count = 0;
    }



    // reset the database_multiple_documents_fields
    for (count = 0; count < 4; count++)
    {
      memset(database_multiple_documents_fields.item[0][count],0,strlen(database_multiple_documents_fields.item[0][count]));
      memset(database_multiple_documents_fields.value[0][count],0,strlen(database_multiple_documents_fields.value[0][count]));
    }

    // select a random reserve proofs collection
    memset(data,0,sizeof(data));
    memcpy(data,"reserve_proofs_",15);
    sprintf(data+15,"%d",((rand() % (TOTAL_RESERVE_PROOFS_DATABASES - 1 + 1)) + 1)); 

    // select a random document in the collection
    count = count_all_documents_in_collection(DATABASE_NAME,data,0);
    if (count > 0)
    {
      count = ((rand() % (count_all_documents_in_collection(DATABASE_NAME,data,0) - 1 + 1)) + 1);
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
        // the proof is invalid, check if it is a unique reserve proof
        for (count = 0, settings = 1; count < invalid_reserve_proofs.count; count++)
        {
          if (strncmp(invalid_reserve_proofs.reserve_proof[count],database_multiple_documents_fields.value[0][3],BUFFER_SIZE_RESERVE_PROOF) == 0)
          {
            settings = 0;
          }
        }

        if (settings != 0)
        {
          // add the reserve proof to the invalid_reserve_proofs struct
          memcpy(invalid_reserve_proofs.block_verifier_public_address[invalid_reserve_proofs.count],xcash_wallet_public_address,XCASH_WALLET_LENGTH);
          memcpy(invalid_reserve_proofs.public_address[invalid_reserve_proofs.count],database_multiple_documents_fields.value[0][0],XCASH_WALLET_LENGTH);
          memcpy(invalid_reserve_proofs.reserve_proof[invalid_reserve_proofs.count],database_multiple_documents_fields.value[0][3],strnlen(database_multiple_documents_fields.value[0][3],BUFFER_SIZE_RESERVE_PROOF));
          invalid_reserve_proofs.count++;
        }        
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
  for (count = 0; count < 150; count++)
  {
    for (count2 = 0; count2 < 19; count2++)
    {
      database_multiple_documents_fields.item[count][count2] = (char*)calloc(100,sizeof(char));
      database_multiple_documents_fields.value[count][count2] = (char*)calloc(100,sizeof(char));
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
    if (current_UTC_date_and_time->tm_min % 5 == 0)
    {
      // get the top 150 delegates by total votes
      if (read_multiple_documents_all_fields_from_collection(DATABASE_NAME,"delegates","",&database_multiple_documents_fields,1,150,1,"total_vote_count",0) == 0)
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

         if (get_delegate_online_status(database_multiple_documents_fields.value[count][2]) == 1)
         {
           memset(data,0,sizeof(data));
           memcpy(data,"{\"online_status\":\"true\"}",24);
         }
         else
         {
           memset(data,0,sizeof(data));
           memcpy(data,"{\"online_status\":\"false\"}",25);
           print_error_message;
         }   
         if (update_document_from_collection(DATABASE_NAME,"delegates",message,data,0) == 0)
         {
           memcpy(error_message.function[error_message.total],"check_delegates_online_status_timer_thread",42);
           memcpy(error_message.data[error_message.total],"Could not update a delegates online online status. This means the delegates database might be unsynced, and you might have to sync the database.",144);
           error_message.total++;
           print_error_message;
         }     
       }

      // reset the database_multiple_documents_fields
      for (count = 0; count < 150; count++)
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
    sleep(60);
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
  const struct hostent* HOST_NAME = gethostbyname(data->HOST); 
  if (HOST_NAME == NULL)
  {
    SEND_DATA_SOCKET_ERROR;
  }
    
  // convert the port to a string  
  sprintf(buffer2,"%d",SEND_DATA_PORT); 
   
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
  memcpy(message+strlen(message),SOCKET_END_STRING,sizeof(SOCKET_END_STRING)-1);
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
  const struct hostent* HOST_NAME = gethostbyname(data->HOST); 
  if (HOST_NAME == NULL)
  {
    SEND_AND_RECEIVE_DATA_SOCKET_ERROR;
  }
    
  // convert the port to a string  
  sprintf(buffer2,"%d",SEND_DATA_PORT); 
   
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
    if (recvfrom(SOCKET, buffer, BUFFER_SIZE, MSG_DONTWAIT | MSG_PEEK, NULL, NULL) >= BUFFER_SIZE)
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
  if (parse_json_data(data2,"block_blob_signature",message) == 1 && strlen(message) == XCASH_SIGN_DATA_LENGTH && memcmp(message,XCASH_SIGN_DATA_PREFIX,sizeof(XCASH_SIGN_DATA_PREFIX)-1) == 0)
  {
    memcpy(VRF_data.block_blob_signature[data->COUNT],message,XCASH_SIGN_DATA_LENGTH);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[data->COUNT],message,XCASH_SIGN_DATA_LENGTH);
  }
  else
  {  
    memcpy(VRF_data.block_blob_signature[data->COUNT],"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",XCASH_SIGN_DATA_LENGTH);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[data->COUNT],"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",XCASH_SIGN_DATA_LENGTH);
  }
  pthread_exit((void *)(intptr_t)1);
  
  #undef SEND_AND_RECEIVE_DATA_SOCKET_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: socket_thread
Description: socket_thread
Parameters:
  parameters - A pointer to the socket_thread_parameters struct
  struct socket_thread_parameters
    client_socket - The client socket
    client_address - The client address
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

void* socket_thread(void* parameters)
{ 
  // Variables
  struct socket_thread_parameters* data = (struct socket_thread_parameters*)parameters;
  int CLIENT_SOCKET = data->client_socket;
  char buffer[BUFFER_SIZE];
  char buffer2[BUFFER_SIZE];
  char message[BUFFER_SIZE];
  char client_address[BUFFER_SIZE]; 
  int len;
  int receive_data_result; 
  struct sockaddr_in addr, cl_addr; 

  memset(buffer,0,sizeof(buffer));
  memset(buffer2,0,sizeof(buffer2));
  memset(message,0,sizeof(message));
  memset(client_address,0,sizeof(client_address));

  memcpy(client_address,data->client_address,strnlen(data->client_address,BUFFER_SIZE)); 

  // convert the port to a string
  sprintf(buffer2,"%d",SEND_DATA_PORT); 

  // receive the data
  memset(buffer, 0, sizeof(buffer)); 
  receive_data_result = receive_data(CLIENT_SOCKET,buffer,SOCKET_END_STRING,1,TOTAL_CONNECTION_TIME_SETTINGS);
  if (receive_data_result < 2)
  {
    close(CLIENT_SOCKET);
    pointer_reset(data);
    pthread_exit((void *)(intptr_t)0);
  }  

  // get the current time
  get_current_UTC_time;
  
  memcpy(message,"Received ",9);
  memcpy(message+9,&buffer[25],strlen(buffer) - strlen(strstr(buffer,"\",\r\n")) - 25);
  memcpy(message+strlen(message)," from ",6);
  memcpy(message+strlen(message),data->client_address,strnlen(client_address,BUFFER_SIZE));
  memcpy(message+strlen(message)," on port ",9);
  memcpy(message+strlen(message),buffer2,strnlen(buffer2,BUFFER_SIZE));
  memcpy(message+strlen(message),"\n",1);
  memcpy(message+strlen(message),asctime(current_UTC_date_and_time),strnlen(asctime(current_UTC_date_and_time),BUFFER_SIZE));
  color_print(message,"green");

 // check if a certain type of message has been received 
 if (strstr(buffer,"\"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\"") != NULL)
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
 else if (strstr(buffer,"\"message_settings\": \"MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_CREATE_NEW_BLOCK\"") != NULL)
 {  
   server_receive_data_socket_main_network_data_node_to_block_verifier_create_new_block(CLIENT_SOCKET,(const char*)buffer);
 } 
 else if (strstr(buffer,"\"message_settings\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND_CREATE_NEW_BLOCK\"") != NULL && current_UTC_date_and_time->tm_sec % 30 >= 5 && current_UTC_date_and_time->tm_sec % 30 < 10)
 {
   server_receive_data_socket_main_node_to_node_message_part_4((const char*)buffer);
 }         
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA\"") != NULL && current_UTC_date_and_time->tm_sec % 30 < 5)
 {
   server_receive_data_socket_block_verifiers_to_block_verifiers_vrf_data((const char*)buffer);
 }  
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE\"") != NULL && current_UTC_date_and_time->tm_sec % 30 >= 10 && current_UTC_date_and_time->tm_sec % 30 < 15)
 {
   server_receive_data_socket_block_verifiers_to_block_verifiers_block_blob_signature((const char*)buffer);
 }  
 else if (strstr(buffer,"\"message_settings\": \"NODES_TO_NODES_VOTE_RESULTS\"") != NULL && current_UTC_date_and_time->tm_sec % 30 >= 20 && current_UTC_date_and_time->tm_sec % 30 < 25)
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
close(CLIENT_SOCKET);
pointer_reset(data);
pthread_exit((void *)(intptr_t)1);
}