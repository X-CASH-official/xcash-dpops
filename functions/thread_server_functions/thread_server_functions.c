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
#include "network_daemon_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "network_wallet_functions.h"
#include "server_functions.h"
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
    memcpy(error_message.function[error_message.total],"current_block_height_timer_thread",33);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  for (;;)
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
  }
  pointer_reset(data);
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
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message = (char*)calloc(15728640,sizeof(char)); // 15 MB
  int count;
  int count2;
  size_t block_verifiers_score;
  int settings;
  struct database_multiple_documents_fields database_multiple_documents_fields;

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; \
  free(message); \
  message = NULL;

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
    memcpy(error_message.function[error_message.total],"check_reserve_proofs_timer_thread",33);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  reserve_proofs_settings = 1;
  main_network_data_node_create_block = 0;

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
      memset(data,0,strlen(data));
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
          send_data_socket(current_block_verifiers_list.block_verifiers_IP_address[count],SEND_DATA_PORT,message,"",0);
        }
      }
      
      // wait for all of the other block verifiers to send you their invalid_reserve_proofs
      sleep(10);

      // set the reserve_proofs_settings so their are no reserve proofs added to the database
      reserve_proofs_settings = 0;

      // at this point the block verifier will have added all other reserve proofs to the invalid_reserve_proofs struct, so now we need to delete all of the reserve proofs in the collections
      for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
      {
        memset(data,0,strlen(data));
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

      // update all of the block verifiers score
      for (count2 = 0; count2 < invalid_reserve_proofs.count; count2++)
      {
        // create the message
        memset(message,0,strlen(message));
        memcpy(message,"{\"public_address\":\"",18);
        memcpy(message+18,invalid_reserve_proofs.block_verifier_public_address[count2],strnlen(invalid_reserve_proofs.block_verifier_public_address[count2],XCASH_WALLET_LENGTH));
        memcpy(message+18+strnlen(invalid_reserve_proofs.block_verifier_public_address[count2],XCASH_WALLET_LENGTH),"\"}",2);

        // get the block verifiers score
        memset(data2,0,strlen(data2));
        if (read_document_field_from_collection(DATABASE_NAME,"delegates",message,"block_verifiers_score",data2,0) == 0)
        {
          memcpy(error_message.function[error_message.total],"check_reserve_proofs_timer_thread",33);
          memcpy(error_message.data[error_message.total],"Could not update a block verifiers score. This means the delegates database might be unsynced, and you might have to sync the database.",135);
          error_message.total++;
        }
        sscanf(data2, "%zu", &block_verifiers_score);
        block_verifiers_score++;

        memset(data,0,strlen(data));
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
      for (count = 0; count < 100; count++)
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
    memset(data,0,strlen(data));
    memcpy(data,"reserve_proofs_",15);
    sprintf(data+15,"%d",((rand() % (50 - 1 + 1)) + 1)); 

    // select a random document in the collection
    count = ((rand() % (count_all_documents_in_collection(DATABASE_NAME,data,0) - 1 + 1)) + 1);

    // get a random document from the collection
    if (read_multiple_documents_all_fields_from_collection(DATABASE_NAME,data,"",&database_multiple_documents_fields,count,1,0,"",0) == 1)
    {
      // check if the reserve proof is valid
      memset(data,0,strlen(data));
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
  pointer_reset_all;
  pthread_exit((void *)(intptr_t)1);

  #undef pointer_reset_all
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
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;
  size_t count2;
  struct database_multiple_documents_fields database_multiple_documents_fields;

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(message); \
  message = NULL;

  // check if the memory needed was allocated on the heap successfully
  if (message == NULL || data == NULL)
  {
    if (message != NULL)
    {
      pointer_reset(message);
    }
    if (data != NULL)
    {
      pointer_reset(data);
    }
    memcpy(error_message.function[error_message.total],"check_delegates_online_status_timer_thread",42);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

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
         memset(message,0,strnlen(message,BUFFER_SIZE));
         memcpy(message,"{\"public_address\":\"",19);
         memcpy(message+19,database_multiple_documents_fields.value[count][0],XCASH_WALLET_LENGTH);
         memcpy(message+19+XCASH_WALLET_LENGTH,"\"}",2);

         if (get_delegate_online_status(database_multiple_documents_fields.value[count][2]) == 1)
         {
           memset(data,0,strnlen(data,BUFFER_SIZE));
           memcpy(data,"{\"online_status\":\"true\"}",24);
         }
         else
         {
           memset(data,0,strnlen(data,BUFFER_SIZE));
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
    sleep(1);
  }
  pointer_reset_all;
  pthread_exit((void *)(intptr_t)1);

  #undef pointer_reset_all
}