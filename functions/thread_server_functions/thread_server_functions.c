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
Name: total_connection_time_thread
Description: Closes the forked process after a certain connection timeout
Parameters:
  parameters - A pointer to the total_connection_time_thread_parameters struct
  struct total_connection_time_thread_parameters
    process_id - The process id of the forked process
    client_address - The client's IP address for the forked process
    port - The client's connected port for the forked process
    data_received - 1 if data was received in the timeout time, otherwise 0
    message_settings - 1 to print the messages, otherwise 0. This is used for the testing flag to not print any success or error messages
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

void* total_connection_time_thread(void* parameters)
{ 
  // Variables
  char* string = (char*)calloc(BUFFER_SIZE,sizeof(char));
  struct total_connection_time_thread_parameters* data = parameters;
  int client_address_length = strnlen(data->client_address,BUFFER_SIZE);
  int data_port_length = strnlen(data->port,BUFFER_SIZE);

  // check if the memory needed was allocated on the heap successfully
  if (string == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  sleep(TOTAL_CONNECTION_TIME_SETTINGS);    
  if (data->message_settings == 1)
  {
    printf("Total connection time has been reached for %s on port %s\n", data->client_address,data->port); 
    if (data->data_received == 1)
    {
      memcpy(string,"Client at ",10);
      memcpy(string+10,data->client_address,client_address_length);
      memcpy(string+10+client_address_length," on port ",9);
      memcpy(string+19+client_address_length,data->port,data_port_length);
      memcpy(string+19+client_address_length+data_port_length," has sent data in the timeout time",34);
      color_print(string,"green");
    }
    else
    {
      memcpy(string,"Client at ",10);
      memcpy(string+10,data->client_address,client_address_length);
      memcpy(string+10+client_address_length," on port ",9);
      memcpy(string+19+client_address_length,data->port,data_port_length);
      memcpy(string+19+client_address_length+data_port_length," did not send any data in the timeout time",42);
      color_print(string,"red"); 
    }
  }
  pointer_reset(string);
  // close the client connection
  kill((intptr_t)data->process_id, SIGKILL); 
  pthread_exit((void *)(intptr_t)1);
}



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
  time_t current_date_and_time;
  struct tm* current_UTC_date_and_time; 
  size_t count;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  for (;;)
  {
    start:
    // pause 200 milliseconds and then check the time. If it is a possible block time check if their is a new block
    usleep(200000);
    time(&current_date_and_time);
    current_UTC_date_and_time = gmtime(&current_date_and_time);
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

          start_new_round();
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
  time_t current_date_and_time;
  struct tm* current_UTC_date_and_time; 
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
    color_print("Could not allocate the memory needed on the heap","red");
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
      color_print("Could not allocate the memory needed on the heap","red");
      exit(0);
    }
  } 
  database_multiple_documents_fields.document_count = 0;
  database_multiple_documents_fields.database_fields_count = 0;



  for (;;)
  {
    time(&current_date_and_time);
    current_UTC_date_and_time = gmtime(&current_date_and_time);
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
      count += 23;
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
        color_print("Could not sign_data. This means the reserve proofs database might be unsynced, and you might have to sync the database.\nFunction: check_reserve_proofs_timer_thread\nSend Message: BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS","red");
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
      for (count = 1; count <= 50; count++)
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
          color_print("Could not update a block verifiers score. This means the delegates database might be unsynced, and you might have to sync the database.\nFunction: check_reserve_proofs_timer_thread\nSend Message: BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS","red");
        }
        sscanf(data2, "%zu", &block_verifiers_score);
        block_verifiers_score++;

        memset(data,0,strlen(data));
        memcpy(data,"{\"block_verifiers_score\":\"",26);
        sprintf(data2+26,"%zu",block_verifiers_score);
        memcpy(data+strlen(data),"\"}",2);

        if (update_document_from_collection(DATABASE_NAME,"delegates",message,data,0) == 0)
        {
          color_print("Could not update a block verifiers score. This means the delegates database might be unsynced, and you might have to sync the database.\nFunction: check_reserve_proofs_timer_thread\nSend Message: BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS","red");
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
  time_t current_date_and_time;
  struct tm* current_UTC_date_and_time; 
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
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // initialize the database_multiple_documents_fields struct 
  for (count = 0; count < 150; count++)
  {
    for (count2 = 0; count2 < 23; count2++)
    {
      database_multiple_documents_fields.item[count][count2] = (char*)calloc(BUFFER_SIZE,sizeof(char));
      database_multiple_documents_fields.value[count][count2] = (char*)calloc(BUFFER_SIZE,sizeof(char));
    }    

    if (database_multiple_documents_fields.item[count][count2] == NULL || database_multiple_documents_fields.value[count][count2] == NULL)
    {
      color_print("Could not allocate the memory needed on the heap","red");
      exit(0);
    }
  } 
  database_multiple_documents_fields.document_count = 0;
  database_multiple_documents_fields.database_fields_count = 0;



  for (;;)
  {
    time(&current_date_and_time);
    current_UTC_date_and_time = gmtime(&current_date_and_time);
    if (current_UTC_date_and_time->tm_min % 5 == 0)
    {
      // get the top 150 delegates by total votes
      if (read_multiple_documents_all_fields_from_collection(DATABASE_NAME,"delegates","",&database_multiple_documents_fields,1,150,1,"total_vote_count",0) == 0)
      {
        color_print("Could not get the top 150 delegates to check their online status. This means the delegates database might be unsynced, and you might have to sync the database.\nFunction: check_delegates_online_status_timer_thread","red");
      }

      // check the online status of the top 150 delegates
      for (count = 0; count < 150; count++)
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
         }   
         if (update_document_from_collection(DATABASE_NAME,"delegates",message,data,0) == 0)
         {
           color_print("Could not update a delegates online online status. This means the delegates database might be unsynced, and you might have to sync the database.\nFunction: check_delegates_online_status_timer_thread","red");
         }     
       }

      // reset the database_multiple_documents_fields
      for (count = 0; count < 150; count++)
      {
        for (count2 = 0; count2 < 23; count2++)
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



/*
-----------------------------------------------------------------------------------------------------------
Name: mainnode_timeout_thread
Description: Closes the forked process after a certain connection timeout. This is so the node knows if it should send a message to the consensus node that the main node never sent any data, and if a round change should occur
Parameters:
  parameters - A pointer to the mainnode_timeout_thread_parameters struct
  struct mainnode_timeout_thread_parameters
    process_id - The process id of the forked process
    data_received - 1 if the node has received data from the main node, otherwise 0
    main_nodes_public_address - The public address of the main node for that part of the round
    current_round_part - The current round part (1-4).
    current_round_part_backup_node - The current main node in the current round part (0-5)
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

void* mainnode_timeout_thread(void* parameters)
{
  // Variables
  char* string = (char*)calloc(BUFFER_SIZE,sizeof(char));
  struct mainnode_timeout_thread_parameters* data = parameters;
  size_t main_node_length = strnlen(data->main_node,BUFFER_SIZE);

  // check if the memory needed was allocated on the heap successfully
  if (string == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  sleep(TOTAL_CONNECTION_TIME_SETTINGS_MAIN_NODE_TIMEOUT);  
  printf("Total connection time for the main node for current round part %s and current round part backup node %s has been reached",current_round_part,current_round_part_backup_node);
  if (data->data_received == 1)
  {
    memcpy(string,"Received data from main node, ",30);
    memcpy(string+30,data->main_node,main_node_length);
    memcpy(string+30+main_node_length," backup node number ",20);
    memcpy(string+50+main_node_length,data->current_round_part_backup_node,1);
    memcpy(string+51+main_node_length," in current round part ",23);
    memcpy(string+74+main_node_length,data->current_round_part,1);
    color_print(string,"green");     
  }
  else
  {
    memcpy(string,"main node, ",11);
    memcpy(string+11,data->main_node,main_node_length);
    memcpy(string+11+main_node_length," backup node number ",20);
    memcpy(string+31+main_node_length,data->current_round_part_backup_node,1);
    memcpy(string+32+main_node_length," in current round part ",23);
    memcpy(string+55+main_node_length,data->current_round_part,1);
    memcpy(string+56+main_node_length," did not send any data before the timeout\nSending the NODES_TO_CONSENSUS_NODE_MAIN_NODE_SOCKET_TIMEOUT_ROUND_CHANGE message to the consensus node",145);
    color_print(string,"red");  

    // set the next server message since a backup node will have to be selected
    memset(server_message,0,strnlen(server_message,BUFFER_SIZE));
    memcpy(server_message,"CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS",48); 

    
  }
  pointer_reset(string);

  // close the client connection, since the consensus node is still connected
  kill((intptr_t)data->process_id, SIGTERM);

  // reset the mainnode_timeout_thread_parameters
  data->process_id = 0;
  data->data_received = 0;
  data->main_node = "";
  data->current_round_part = "";
  data->current_round_part_backup_node = "";

  pthread_exit((void *)(intptr_t)1);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: node_to_node_message_timeout_thread
Description: Closes the forked process after a certain connection timeout. This is so the block verifiers have waited a certain amount of time, and will now count all of the votes
Parameters:
  parameters - A pointer to the node_to_node_timeout_thread_parameters struct
  struct node_to_node_timeout_thread_parameters
    process_id - The process id of the forked process
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

void* node_to_node_message_timeout_thread(void* parameters)
{
  // Variables
  char* string = (char*)calloc(BUFFER_SIZE,sizeof(char));
  struct node_to_node_timeout_thread_parameters* data = parameters;

  // check if the memory needed was allocated on the heap successfully
  if (string == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  sleep(TOTAL_CONNECTION_TIME_SETTINGS_MAIN_NODE_TIMEOUT);  
  printf("Total connection time for all block verifiers to send data to all other block verifiers for current round part %s and current round part backup node %s has been reached",current_round_part,current_round_part_backup_node); 
  
  // create the message
  memcpy(string,"{\r\n \"message_settings\": \"NODES_TO_CONSENSUS_NODE_VOTE_RESULTS\",\r\n  \"vote_result\": \"",84);

  // verify the VRF data
  if (crypto_vrf_verify((unsigned char*)current_round_part_consensus_node_data.vrf_beta_string,(const unsigned char*)current_round_part_consensus_node_data.vrf_public_key,(const unsigned char*)current_round_part_consensus_node_data.vrf_proof,(const unsigned char*)current_round_part_consensus_node_data.vrf_alpha_string,(unsigned long long)strnlen(current_round_part_consensus_node_data.vrf_alpha_string,BUFFER_SIZE)) == 0)
  {
    memcpy(string+84,"TRUE",4);
  }
  else
  {
    memcpy(string+84,"FALSE",5);
  }
  memcpy(string+strnlen(string,BUFFER_SIZE),"\",\r\n  \"vrf_public_key\": \"",25);
  memcpy(string+strnlen(string,BUFFER_SIZE),current_round_part_consensus_node_data.vrf_public_key,VRF_PUBLIC_KEY_LENGTH);
  memcpy(string+strnlen(string,BUFFER_SIZE),"\",\r\n  \"vrf_alpha_string\": \"",27);
  memcpy(string+strnlen(string,BUFFER_SIZE),current_round_part_consensus_node_data.vrf_alpha_string,strnlen(current_round_part_consensus_node_data.vrf_alpha_string,BUFFER_SIZE));
  memcpy(string+strnlen(string,BUFFER_SIZE),"\",\r\n  \"vrf_proof\": \"",20);
  memcpy(string+strnlen(string,BUFFER_SIZE),current_round_part_consensus_node_data.vrf_proof,VRF_PROOF_LENGTH);
  memcpy(string+strnlen(string,BUFFER_SIZE),"\",\r\n  \"vrf_beta_string\": \"",26);
  memcpy(string+strnlen(string,BUFFER_SIZE),current_round_part_consensus_node_data.vrf_beta_string,VRF_BETA_LENGTH);
  memcpy(string+strnlen(string,BUFFER_SIZE),"\",\r\n  \"block_blob\": \"",21);
  memcpy(string+strnlen(string,BUFFER_SIZE),current_round_part_consensus_node_data.block_blob,strnlen(current_round_part_consensus_node_data.block_blob,BUFFER_SIZE));
  memcpy(string+strnlen(string,BUFFER_SIZE),"\"}",2);

  // set the next server message since a backup node will have to be selected
  memset(server_message,0,strnlen(server_message,BUFFER_SIZE));
  memcpy(server_message,"CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS",48); 
  
  pointer_reset(string);

  // close the client connection, since the consensus node is still connected
  kill((intptr_t)data->process_id, SIGTERM);

  // reset the node_to_node_timeout_thread_parameters
  data->process_id = 0;

  pthread_exit((void *)(intptr_t)1);
}