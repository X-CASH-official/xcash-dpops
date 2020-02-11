#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <netdb.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
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
#include "initialize_and_reset_structs_define_macros.h"
#include "define_macros_test.h"

#include "block_verifiers_functions.h"
#include "block_verifiers_synchronize_functions.h"
#include "block_verifiers_thread_server_functions.h"
#include "database_functions.h"
#include "insert_database_functions.h"
#include "read_database_functions.h"
#include "update_database_functions.h"
#include "delete_database_functions.h"
#include "count_database_functions.h"
#include "define_macro_functions.h"
#include "file_functions.h"
#include "network_daemon_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "network_wallet_functions.h"
#include "organize_functions.h"
#include "server_functions.h"
#include "string_functions.h"
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

void* current_block_height_timer_thread(void* parameters)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  size_t count;
  int settings = 0;
  int block_verifier_settings;
  
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // unused parameters
  (void)parameters;

  /*sync_block_verifiers_minutes(current_date_and_time,current_UTC_date_and_time,0);
  get_current_block_height(current_block_height,0);
  if (start_new_round() == 0)
  {
    print_error_message(current_date_and_time,current_UTC_date_and_time,data);
  }
  else
  {
    fprintf(stderr,"\033[1;32mNetwork Block %s Has Been Created Successfully\033[0m\n",current_block_height);
  }*/

  /*do
  {
    get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);
    sleep(60);
  } while (current_UTC_date_and_time.tm_mday != 28 || current_UTC_date_and_time.tm_hour != 16 || current_UTC_date_and_time.tm_min != 56);*/
  
  // get the current block height and wait until the block height is at the XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT
  get_current_block_height(current_block_height,0);
  get_previous_block_hash(previous_block_hash,0);
  sscanf(current_block_height,"%zu", &count);
  while (count < XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT)
  {
    get_current_block_height(current_block_height,0);
    get_previous_block_hash(previous_block_hash,0);
    sscanf(current_block_height,"%zu", &count);
    sleep(1);
  }

  for (;;)
  {
    get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);
    if ((settings == 0 && current_UTC_date_and_time.tm_min % BLOCK_TIME == 0 && current_UTC_date_and_time.tm_sec == 0) || (settings == 1 && current_UTC_date_and_time.tm_min % BLOCK_TIME == 0))
    {
      if (settings == 0)
      {        
        block_verifier_settings = start_new_round();
        if (block_verifier_settings == 0)
        {
          print_error_message(current_date_and_time,current_UTC_date_and_time,data);
        }
        else if (block_verifier_settings == 1)
        {
          memset(data2,0,sizeof(data2));
          memcpy(data2,"Your delegate is not a block verifier for network block ",56);
          memcpy(data2+56,current_block_height,strnlen(current_block_height,sizeof(data2)));
          color_print(data2,"red");
        }
        else
        {
          memset(data2,0,sizeof(data2));
          memcpy(data2,"Network Block ",14);
          memcpy(data2+14,current_block_height,strnlen(current_block_height,sizeof(data2)));
          memcpy(data2+strlen(data2)," Has Been Created Successfully\n",31);
          color_print(data2,"green");
        }
        settings = 1;
        continue;
      }

      get_current_block_height(current_block_height,0);
      get_previous_block_hash(previous_block_hash,0);
      block_verifier_settings = start_new_round();
      if (block_verifier_settings == 0)
      {
        print_error_message(current_date_and_time,current_UTC_date_and_time,data);
      }
      else if (block_verifier_settings == 1)
      {
        memset(data2,0,sizeof(data2));
        memcpy(data2,"Your delegate is not a block verifier for network block ",56);
        memcpy(data2+56,current_block_height,strnlen(current_block_height,sizeof(data2)));
        color_print(data2,"red");
      }
      else
      {
        memset(data2,0,sizeof(data2));
        memcpy(data2,"Network Block ",14);
        memcpy(data2+14,current_block_height,strnlen(current_block_height,sizeof(data2)));
        memcpy(data2+strlen(data2)," Has Been Created Successfully\n",31);
        color_print(data2,"green");
      }      
    }
    sleep(1);
  }
  pthread_exit((void *)(intptr_t)1);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: check_reserve_proofs_timer_create_message
Description: Creates the message to send to all of the block verifiers
Paramters:
  message - The message to send to the block verifiers
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int check_reserve_proofs_timer_create_message(char *block_verifiers_message)
{
  // Variables
  char data[BUFFER_SIZE];
  char* reserve_proofs[invalid_reserve_proofs.count];
  int count;
  int count2;
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  crypto_hash_sha512_state state;

  #define RESET_RESERVE_PROOFS_ARRAY \
  for (count = 0; count < invalid_reserve_proofs.count; count++) \
  { \
    pointer_reset(reserve_proofs[count]); \
  }

  // initialize the reserve_proofs array 
  for (count = 0; count < invalid_reserve_proofs.count; count++)
  {
    reserve_proofs[count] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));

    if (reserve_proofs[count] == NULL)
    {
      memcpy(error_message.function[error_message.total],"check_reserve_proofs_timer_thread",33);
      memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
      error_message.total++;
      print_error_message(current_date_and_time,current_UTC_date_and_time,data);  
      exit(0);
    }
  }

  // wait for any block verifiers sending messages, or any block verifiers waiting to process a reserve proof
  sync_block_verifiers_seconds(current_date_and_time,current_UTC_date_and_time,30);

  // set the database to not accept any new data
  database_settings = 0;

  // copy all of the reserve proofs to the reserve_proofs array
  for (count = 0; count < invalid_reserve_proofs.count; count++)
  {
    memcpy(reserve_proofs[count],invalid_reserve_proofs.reserve_proof[count],strnlen(invalid_reserve_proofs.reserve_proof[count],BUFFER_SIZE_RESERVE_PROOF));
  }

  // organize the invalid reserve proofs struct so the data hash is the same
  qsort(reserve_proofs,sizeof(reserve_proofs)/sizeof(reserve_proofs[0]),sizeof(reserve_proofs[0]),organize_invalid_reserve_proofs_settings);

  // get the data hash of the invalid_reserve_proofs struct
  memset(data,0,sizeof(data));
  memset(block_verifiers_message,0,strlen(block_verifiers_message));
  crypto_hash_sha512_init(&state);
  if (invalid_reserve_proofs.count > 0)
  {
    for (count = 0; count < invalid_reserve_proofs.count; count++)
    {
      crypto_hash_sha512_update(&state,(const unsigned char*)reserve_proofs[count],(unsigned long long)strnlen(reserve_proofs[count],BUFFER_SIZE_RESERVE_PROOF));
    }
  }
  else
  {
    crypto_hash_sha512_update(&state,(const unsigned char*)"XCASH",5);
  }      
  crypto_hash_sha512_final(&state, (unsigned char*)data);

  // convert the SHA512 data hash to a string
  for (count2 = 0, count = 0; count2 < DATA_HASH_LENGTH / 2; count2++, count += 2)
  {
    snprintf(block_verifiers_message+count,sizeof(block_verifiers_message)-1,"%02x",data[count2] & 0xFF);
  }

  // reset the current_round_part_vote_data.vote_results_valid struct
  memset(current_round_part_vote_data.current_vote_results,0,sizeof(current_round_part_vote_data.current_vote_results));
  current_round_part_vote_data.vote_results_valid = 1;
  current_round_part_vote_data.vote_results_invalid = 0;

  memcpy(current_round_part_vote_data.current_vote_results,block_verifiers_message,DATA_HASH_LENGTH);

  // create the message
  memset(block_verifiers_message,0,strlen(block_verifiers_message));
  memcpy(block_verifiers_message,"{\r\n \"message_settings\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n \"vote_settings\": \"valid\",\r\n \"vote_data\": \"",99);  
  memcpy(block_verifiers_message+strlen(block_verifiers_message),current_round_part_vote_data.current_vote_results,DATA_HASH_LENGTH);
  memcpy(block_verifiers_message+strlen(block_verifiers_message),"\",\r\n}",5); 

  // sign_data
  if (sign_data(block_verifiers_message,0) == 0)
  { 
    RESET_RESERVE_PROOFS_ARRAY
    return 0;
  }
  RESET_RESERVE_PROOFS_ARRAY
  return 1;

  #undef RESET_RESERVE_PROOFS_ARRAY
}



/*
-----------------------------------------------------------------------------------------------------------
Name: check_reserve_proofs_timer_get_database_data
Description: Gets the reserve proof data from the database as the block verifier has to use its own data to make sure its valid, since the reserve proof is already invalid
Paramters:
  CURRENT_RESERVE_PROOF_COUNT - The current_reserve_proof_count
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int check_reserve_proofs_timer_get_database_data(const int CURRENT_RESERVE_PROOF_COUNT)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  int count;
  size_t block_verifiers_total_vote_count;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  // get the data for the reserve proof from your own database, since you cant know if the data given was valid since the reserve proof is already invalid
  for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
  {
    memset(data,0,sizeof(data));
    memcpy(data,"reserve_proofs_",15);
    snprintf(data+15,sizeof(data)-16,"%d",count);

    // check if the reserve proof is in the database
    if (count_all_documents_in_collection(database_name,data,1) > 0)
    {
      memset(data2,0,sizeof(data2));
      memset(data3,0,sizeof(data3));
      memcpy(data3,"{\"reserve_proof\":\"",18);
      memcpy(data3+18,invalid_reserve_proofs.reserve_proof[CURRENT_RESERVE_PROOF_COUNT],strnlen(invalid_reserve_proofs.reserve_proof[CURRENT_RESERVE_PROOF_COUNT],sizeof(data3)));
      memcpy(data3+strlen(data3),"\"}",2); 

      memset(invalid_reserve_proofs.public_address_created_reserve_proof[CURRENT_RESERVE_PROOF_COUNT],0,strlen(invalid_reserve_proofs.public_address_created_reserve_proof[CURRENT_RESERVE_PROOF_COUNT]));
      memset(invalid_reserve_proofs.public_address_voted_for[CURRENT_RESERVE_PROOF_COUNT],0,strlen(invalid_reserve_proofs.public_address_voted_for[CURRENT_RESERVE_PROOF_COUNT]));
      invalid_reserve_proofs.reserve_proof_amount[CURRENT_RESERVE_PROOF_COUNT] = 0;      
          
      // get the data from the database for the reserve proof. If the data is not in the database then skip that reserve proof when updating the database
      memset(data2,0,sizeof(data2));
      if (read_document_field_from_collection(database_name,data,data3,"public_address_created_reserve_proof",data2,1) == 1)
      {
        memcpy(invalid_reserve_proofs.public_address_created_reserve_proof[CURRENT_RESERVE_PROOF_COUNT],data2,strnlen(data2,XCASH_WALLET_LENGTH));
      }
      else
      {
        return 0;
      }

      memset(data2,0,sizeof(data2));
      if (read_document_field_from_collection(database_name,data,data3,"public_address_voted_for",data2,1) == 1)
      {
        memcpy(invalid_reserve_proofs.public_address_voted_for[CURRENT_RESERVE_PROOF_COUNT],data2,strnlen(data2,XCASH_WALLET_LENGTH));
      }
      else
      {
        return 0;
      }

      memset(data2,0,sizeof(data2));
      if (read_document_field_from_collection(database_name,data,data3,"total",data2,1) == 1)
      {
        sscanf(data2,"%zu", &block_verifiers_total_vote_count);
        invalid_reserve_proofs.reserve_proof_amount[CURRENT_RESERVE_PROOF_COUNT] = block_verifiers_total_vote_count;
      }
      else
      {
        return 0;
      }

      break;
    }                  
  }

  // check if the reserve proof was found in the database and if not skip that reserve proof when updating the database
  if (count > TOTAL_RESERVE_PROOFS_DATABASES)
  {
    return 0;
  } 
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: check_reserve_proofs_timer_update_delegates_total_vote_count
Description: Updates the delegates total vote count
Paramters:
  CURRENT_RESERVE_PROOF_COUNT - The current_reserve_proof_count
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int check_reserve_proofs_timer_update_delegates_total_vote_count(const int CURRENT_RESERVE_PROOF_COUNT)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  size_t block_verifiers_total_vote_count;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  memcpy(data3,"{\"public_address\":\"",19);
  memcpy(data3+strlen(data3),invalid_reserve_proofs.public_address_voted_for[CURRENT_RESERVE_PROOF_COUNT],XCASH_WALLET_LENGTH);
  memcpy(data3+strlen(data3),"\"}",2);

  if (read_document_field_from_collection(database_name,"delegates",data3,"total_vote_count",data,1) == 0)
  {
    return 0;
  }

  sscanf(data,"%zu", &block_verifiers_total_vote_count);
  // check to make sure this does not underflow
  if ((invalid_reserve_proofs.reserve_proof_amount[CURRENT_RESERVE_PROOF_COUNT] > 0) && (block_verifiers_total_vote_count < 0 + invalid_reserve_proofs.reserve_proof_amount[CURRENT_RESERVE_PROOF_COUNT]))
  {
    return 0;
  }
  block_verifiers_total_vote_count-= invalid_reserve_proofs.reserve_proof_amount[CURRENT_RESERVE_PROOF_COUNT];
  memset(data,0,sizeof(data));
  snprintf(data,sizeof(data)-1,"%zu",block_verifiers_total_vote_count);  

  memcpy(data2,"{\"total_vote_count\":\"",21);
  memcpy(data2+strlen(data2),data,strnlen(data,sizeof(data2)));
  memcpy(data2+strlen(data2),"\"}",2);

  if (update_document_from_collection(database_name,"delegates",data3,data2,1) == 0)
  {
    return 0;
  }
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: check_reserve_proofs_timer_update_delegates_score
Description: Updates the delegates score
Paramters:
  CURRENT_RESERVE_PROOF_COUNT - The current_reserve_proof_count
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int check_reserve_proofs_timer_update_delegates_score(const int CURRENT_RESERVE_PROOF_COUNT)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  size_t block_verifiers_score;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  // create the message
  memset(data,0,sizeof(data));
  memcpy(data,"{\"public_address\":\"",19);
  memcpy(data+19,invalid_reserve_proofs.block_verifier_public_address[CURRENT_RESERVE_PROOF_COUNT],strnlen(invalid_reserve_proofs.block_verifier_public_address[CURRENT_RESERVE_PROOF_COUNT],sizeof(data)));
  memcpy(data+strlen(data),"\"}",2);

  // get the block verifiers score
  if (read_document_field_from_collection(database_name,"delegates",data,"block_verifier_score",data2,1) == 0)
  {
    return 0;
  }
  sscanf(data2, "%zu", &block_verifiers_score);
  block_verifiers_score++;

  memset(data2,0,sizeof(data2));
  memcpy(data2,"{\"block_verifier_score\":\"",25);
  snprintf(data2+25,sizeof(data2)-26,"%zu",block_verifiers_score);
  memcpy(data2+strlen(data2),"\"}",2);

  if (update_document_from_collection(database_name,"delegates",data,data2,1) == 0)
  {
    return 0;
  }
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: check_reserve_proofs_timer_delete_reserve_proof
Description: Gets the reserve proof data from the database as the block verifier has to use its own data to make sure its valid, since the reserve proof is already invalid
Paramters:
  CURRENT_RESERVE_PROOF_COUNT - The current_reserve_proof_count
-----------------------------------------------------------------------------------------------------------
*/

void check_reserve_proofs_timer_delete_reserve_proof(const int CURRENT_RESERVE_PROOF_COUNT)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  int count;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  // get the data for the reserve proof from your own database, since you cant know if the data given was valid since the reserve proof is already invalid
  for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
  {
    memset(data,0,sizeof(data));
    memcpy(data,"reserve_proofs_",15);
    snprintf(data+15,sizeof(data)-16,"%d",count);

    memset(data2,0,sizeof(data2));
    memcpy(data2,"{\"reserve_proof\":\"",18);
    memcpy(data2+18,invalid_reserve_proofs.reserve_proof[CURRENT_RESERVE_PROOF_COUNT],strnlen(invalid_reserve_proofs.reserve_proof[CURRENT_RESERVE_PROOF_COUNT],sizeof(data2)));
    memcpy(data2+strlen(data2),"\"}",2);
    if (count_documents_in_collection(database_name,data,data2,1) > 0)
    {
      delete_document_from_collection(database_name,data,data2,1);
    }
  }
  return;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: check_reserve_proofs_timer_update_database
Description: Updates the database
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int check_reserve_proofs_timer_update_database(void)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  int count;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  for (count = 0; count < invalid_reserve_proofs.count; count++)
  {
    memset(data,0,sizeof(data));
    memset(data2,0,sizeof(data2));
    memset(data3,0,sizeof(data3));

    // get the data for the reserve proof from your own database, since you cant know if the data given was valid since the reserve proof is already invalid
    if (check_reserve_proofs_timer_get_database_data(count) == 0)
    {
      continue;
    }

    // update all of the delegates total_vote_count
    if (check_reserve_proofs_timer_update_delegates_total_vote_count(count) == 0)
    {
      continue;
    }

    // update all of the delegates scores
    if (check_reserve_proofs_timer_update_delegates_score(count) == 0)
    {
      continue;
    }

    // delete the reserve proof from the database
    check_reserve_proofs_timer_delete_reserve_proof(count);
  }
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: select_random_unique_reserve_proof
Description: Selects a random reserve proof and checks if it is unique to the current invalid reserve proofs list
Paramters:
  struct reserve_proof
    block_verifier_public_address - The block verifier that added the reserve proof
    public_address_created_reserve_proof - The public address that created the reserve proof
    public_address_voted_for - The public address that the reserve proof is voted for
    reserve_proof_amount - The reserve proof amount
    reserve_proof - The reserve proof
Return: 0 if the reserve proof is not unique, 1 if it is unique and needs to be checked if it is invalid
-----------------------------------------------------------------------------------------------------------
*/

int select_random_unique_reserve_proof(struct reserve_proof* reserve_proof)
{
  // Variables
  char data[BUFFER_SIZE];
  int count;
  int count2;
  struct database_multiple_documents_fields database_multiple_documents_fields;
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;

  #define pointer_reset_database_array \
  for (count = 0; count < TOTAL_RESERVE_PROOFS_DATABASE_FIELDS; count++) \
  { \
    pointer_reset(database_multiple_documents_fields.item[0][count]); \
    pointer_reset(database_multiple_documents_fields.value[0][count]); \
  }

  memset(data,0,sizeof(data));

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
      print_error_message(current_date_and_time,current_UTC_date_and_time,data);  
      exit(0);
    }
  } 
  database_multiple_documents_fields.document_count = 0;
  database_multiple_documents_fields.database_fields_count = 0;

  // reset the reserve_proof struct
  memset(reserve_proof->block_verifier_public_address,0,sizeof(reserve_proof->block_verifier_public_address));
  memset(reserve_proof->public_address_created_reserve_proof,0,sizeof(reserve_proof->public_address_created_reserve_proof));
  memset(reserve_proof->public_address_voted_for,0,sizeof(reserve_proof->public_address_voted_for));
  memset(reserve_proof->reserve_proof_amount,0,sizeof(reserve_proof->reserve_proof_amount));
  memset(reserve_proof->reserve_proof,0,sizeof(reserve_proof->reserve_proof));

  // select a random reserve proofs collection
  do
  {
    memset(data,0,sizeof(data));
    memcpy(data,"reserve_proofs_",15);
    snprintf(data+15,sizeof(data)-16,"%d",((rand() % (TOTAL_RESERVE_PROOFS_DATABASES - 1 + 1)) + 1));
    RESET_ERROR_MESSAGES; 
  } while ((count = count_all_documents_in_collection(database_name,data,1)) <= 0);

  // select a random document in the collection
  count = (rand() % count) + 1;

  // get a random document from the collection
  if (read_multiple_documents_all_fields_from_collection(database_name,data,"",&database_multiple_documents_fields,count,1,0,"",1) == 1)
  {
    // check if the reserve proof is unique 
    for (count2 = 0; count2 <= invalid_reserve_proofs.count; count2++)
    {
      if (strncmp(invalid_reserve_proofs.reserve_proof[count2],database_multiple_documents_fields.value[0][3],BUFFER_SIZE_RESERVE_PROOF) == 0)
      {
        pointer_reset_database_array;
        return 0;
      }
    }
  }

  // the reserve proof is unique copy the data to the votes struct
  memcpy(reserve_proof->block_verifier_public_address,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(reserve_proof->public_address_created_reserve_proof,database_multiple_documents_fields.value[0][0],strnlen(database_multiple_documents_fields.value[0][0],sizeof(reserve_proof->public_address_created_reserve_proof)));
  memcpy(reserve_proof->public_address_voted_for,database_multiple_documents_fields.value[0][1],strnlen(database_multiple_documents_fields.value[0][1],sizeof(reserve_proof->public_address_voted_for)));
  memcpy(reserve_proof->reserve_proof_amount,database_multiple_documents_fields.value[0][2],strnlen(database_multiple_documents_fields.value[0][2],sizeof(reserve_proof->reserve_proof_amount)));
  memcpy(reserve_proof->reserve_proof,database_multiple_documents_fields.value[0][3],strnlen(database_multiple_documents_fields.value[0][3],sizeof(reserve_proof->reserve_proof)));

  pointer_reset_database_array;
  return 1;

  #undef pointer_reset_database_array
}



/*
-----------------------------------------------------------------------------------------------------------
Name: send_invalid_reserve_proof_to_block_verifiers
Description: Sends the invalid reserve proof to all of the block verifiers
Paramters:
  struct reserve_proof
    block_verifier_public_address - The block verifier that added the reserve proof
    public_address_created_reserve_proof - The public address that created the reserve proof
    public_address_voted_for - The public address that the reserve proof is voted for
    reserve_proof_amount - The reserve proof amount
    reserve_proof - The reserve proof
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int send_invalid_reserve_proof_to_block_verifiers(struct reserve_proof* reserve_proof)
{
  // Variables
  char data[BUFFER_SIZE];

  memset(data,0,sizeof(data));

  if (test_settings == 0)
  {
    color_print("Found an invalid reserve proof","yellow");
  }

  // add the reserve proof to the invalid_reserve_proofs struct
  if (test_settings == 0)
  {
    pthread_rwlock_wrlock(&rwlock_reserve_proofs);
    memcpy(invalid_reserve_proofs.block_verifier_public_address[invalid_reserve_proofs.count],xcash_wallet_public_address,XCASH_WALLET_LENGTH);
    memcpy(invalid_reserve_proofs.reserve_proof[invalid_reserve_proofs.count],reserve_proof->reserve_proof,strnlen(reserve_proof->reserve_proof,sizeof(reserve_proof->reserve_proof)));
    invalid_reserve_proofs.count++;
    pthread_rwlock_unlock(&rwlock_reserve_proofs);
  }

  // send the reserve proof to all block verifiers
  // create the message
  memset(data,0,sizeof(data));
  memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS\",\r\n \"public_address_that_created_the_reserve_proof\": \"",137);
  memcpy(data+strlen(data),reserve_proof->public_address_created_reserve_proof,strnlen(reserve_proof->public_address_created_reserve_proof,sizeof(data)));
  memcpy(data+strlen(data),"\",\r\n \"reserve_proof\": \"",23);
  memcpy(data+strlen(data),reserve_proof->reserve_proof,strnlen(reserve_proof->reserve_proof,sizeof(data)));
  memcpy(data+strlen(data),"\",\r\n}",5);

  // sign_data
  if (sign_data(data,0) == 0)
  { 
    return 0;
  }

  // send the message to all block verifiers
  if (block_verifiers_send_data_socket((const char*)data) == 0)
  {
    return 0;
  }
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: check_reserve_proofs_timer_thread
Description: checks all of the reserve proofs in the database and removes them if they are invalid
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

void* check_reserve_proofs_timer_thread(void* parameters)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;
  struct reserve_proof reserve_proof;
  
  // unused parameters
  (void)parameters;

  // define macros
  #define RESET_INVALID_RESERVE_PROOFS_DATA \
  for (count = 0; count <= invalid_reserve_proofs.count; count++) \
  { \
    memset(invalid_reserve_proofs.block_verifier_public_address[count],0,strlen(invalid_reserve_proofs.block_verifier_public_address[count])); \
    memset(invalid_reserve_proofs.public_address_created_reserve_proof[count],0,strlen(invalid_reserve_proofs.public_address_created_reserve_proof[count])); \
    memset(invalid_reserve_proofs.public_address_voted_for[count],0,strlen(invalid_reserve_proofs.public_address_voted_for[count])); \
    memset(invalid_reserve_proofs.reserve_proof[count],0,strlen(invalid_reserve_proofs.reserve_proof[count])); \
  } \
  invalid_reserve_proofs.count = 0; \
  database_settings = 1; \
  pthread_cond_broadcast(&thread_settings_lock);

  // initialize the reserve_proof struct
  memset(reserve_proof.block_verifier_public_address,0,sizeof(reserve_proof.block_verifier_public_address));
  memset(reserve_proof.public_address_created_reserve_proof,0,sizeof(reserve_proof.public_address_created_reserve_proof));
  memset(reserve_proof.public_address_voted_for,0,sizeof(reserve_proof.public_address_voted_for));
  memset(reserve_proof.reserve_proof_amount,0,sizeof(reserve_proof.reserve_proof_amount));
  memset(reserve_proof.reserve_proof,0,sizeof(reserve_proof.reserve_proof));

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  for (;;)
  {
    get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);
    if (current_UTC_date_and_time.tm_min % BLOCK_TIME == 4 && current_UTC_date_and_time.tm_sec == 25 && invalid_reserve_proofs.count > 0)
    {
      // create the data to send to the block verifiers
      if (check_reserve_proofs_timer_create_message(data2) == 0)
      {
        RESET_INVALID_RESERVE_PROOFS_DATA;
        continue;
      }

      // send the message to all block verifiers
      if (block_verifiers_send_data_socket((const char*)data2) == 0)
      {
        RESET_INVALID_RESERVE_PROOFS_DATA;
        continue;
      }

      // wait for the block verifiers to process the votes
      sync_block_verifiers_seconds(current_date_and_time,current_UTC_date_and_time,40);

      // process the vote results
      if (current_round_part_vote_data.vote_results_valid < BLOCK_VERIFIERS_VALID_AMOUNT)
      {
        RESET_INVALID_RESERVE_PROOFS_DATA;
        continue;
      }

      // update the database
      if (check_reserve_proofs_timer_update_database() == 0)
      {
        RESET_INVALID_RESERVE_PROOFS_DATA;
        continue;
      }
      
      // set the database to accept new data
      database_settings = 1;

      // reset any thread that was waiting for the database
      pthread_cond_broadcast(&thread_settings_lock);
      
      // reset the invalid_reserve_proofs and the block_verifiers_invalid_reserve_proofs
      RESET_INVALID_RESERVE_PROOFS_DATA;
    }



    // check if the reserve proof is valid, or if its valid but its returning a different amount then the amount in the database. This would mean a user changed their database to increase the total
    memset(data,0,sizeof(data));
    if (select_random_unique_reserve_proof(&reserve_proof) == 1 && (check_reserve_proofs(data,reserve_proof.public_address_created_reserve_proof,reserve_proof.reserve_proof,0) == 0 || memcmp(data,reserve_proof.reserve_proof_amount,strlen(data)) != 0))
    {    
      if (send_invalid_reserve_proof_to_block_verifiers(&reserve_proof) == 0)
      {
        RESET_INVALID_RESERVE_PROOFS_DATA;
        continue;
      }
    }
    usleep(500000);
  }
  pthread_exit((void *)(intptr_t)1);
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
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count = 0;
  int sent;
  int bytes;
  int client_socket;
  struct addrinfo serv_addr;
  struct addrinfo* settings = NULL;
  struct pollfd socket_file_descriptors;
  int socket_settings;
  socklen_t socket_option_settings = sizeof(int);

  // define macros
  #define SEND_AND_RECEIVE_DATA_SOCKET_THREAD_ERROR(message_settings) \
  if ((message_settings) == 0) \
  { \
    close(client_socket); \
  } \
  freeaddrinfo(settings); \
  pthread_exit((void *)(intptr_t)0);


  memset(buffer,0,sizeof(buffer));
  memset(buffer2,0,sizeof(buffer2));
  memset(str,0,sizeof(str));
  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));

  // convert the port to a string  
  snprintf(buffer2,sizeof(buffer2)-1,"%d",SEND_DATA_PORT); 

  // set up the addrinfo
  memset(&serv_addr, 0, sizeof(serv_addr));
  if (string_count(data->HOST,".") == 3)
  {
    /* the host is an IP address
    AI_NUMERICSERV = Specifies that getaddrinfo is provided a numerical port
    AI_NUMERICHOST = The host is already an IP address, and this will have getaddrinfo not lookup the hostname
    AF_INET = IPV4 support
    SOCK_STREAM = TCP protocol
    */
    serv_addr.ai_flags = AI_NUMERICSERV | AI_NUMERICHOST;
    serv_addr.ai_family = AF_INET;
    serv_addr.ai_socktype = SOCK_STREAM;
  }
  else
  {
    /* the host is a domain name
    AI_NUMERICSERV = Specifies that getaddrinfo is provided a numerical port
    AF_INET = IPV4 support
    SOCK_STREAM = TCP protocol
    */
    serv_addr.ai_flags = AI_NUMERICSERV;
    serv_addr.ai_family = AF_INET;
    serv_addr.ai_socktype = SOCK_STREAM;
  }
  
  // convert the hostname if used, to an IP address
  memset(str,0,sizeof(str));
  memcpy(str,data->HOST,strnlen(data->HOST,sizeof(str)));
  string_replace(str,sizeof(str),"http://","");
  string_replace(str,sizeof(str),"https://","");
  string_replace(str,sizeof(str),"www.","");
  if (getaddrinfo(str, buffer2, &serv_addr, &settings) != 0)
  {     
    SEND_AND_RECEIVE_DATA_SOCKET_THREAD_ERROR(1);
  }

  /* Create the socket  
  AF_INET = IPV4 support
  SOCK_STREAM = TCP protocol
  SOCK_NONBLOCK = Set the socket to non blocking mode, so it will use the timeout settings when connecting
  */
  client_socket = socket(settings->ai_family, settings->ai_socktype | SOCK_NONBLOCK, settings->ai_protocol);
  if (client_socket == -1)
  { 
    SEND_AND_RECEIVE_DATA_SOCKET_THREAD_ERROR(1);
  }

  /* Set the socket options for sending and receiving data
  SOL_SOCKET = socket level
  SO_RCVTIMEO = allow the socket on receiving data, to use the timeout settings
  */
  if (setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO,(struct timeval *)&SOCKET_TIMEOUT, sizeof(struct timeval)) != 0)
  { 
    SEND_AND_RECEIVE_DATA_SOCKET_THREAD_ERROR(0);
  } 

  /* set the first poll structure to our socket
  POLLOUT - set it to POLLOUT since the socket is non blocking and it can write data to the socket
  */
  socket_file_descriptors.fd = client_socket;
  socket_file_descriptors.events = POLLOUT;

  // connect to the socket
  if (connect(client_socket,settings->ai_addr, settings->ai_addrlen) != 0)
  {    
    count = poll(&socket_file_descriptors,1,SOCKET_CONNECTION_TIMEOUT_SETTINGS);  
    if ((count != 1) || (count == 1 && getsockopt(client_socket,SOL_SOCKET,SO_ERROR,&socket_settings,&socket_option_settings) == 0 && socket_settings != 0))
    {
      SEND_AND_RECEIVE_DATA_SOCKET_THREAD_ERROR(0);
    }
  }

  // get the current socket settings
  socket_settings = fcntl(client_socket, F_GETFL, NULL);
  if (socket_settings == -1)
  {
    SEND_AND_RECEIVE_DATA_SOCKET_THREAD_ERROR(0);
  }

  // set the socket to blocking mode
  socket_settings &= (~O_NONBLOCK);
  if (fcntl(client_socket, F_SETFL, socket_settings) == -1)
  {
    SEND_AND_RECEIVE_DATA_SOCKET_THREAD_ERROR(0);
  }

  // get the current time
  get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);

  // send the message  
  if (test_settings == 0)
  {
    memcpy(message,data->DATA,strnlen(data->DATA,BUFFER_SIZE));
    memcpy(message+strlen(message),SOCKET_END_STRING,sizeof(SOCKET_END_STRING)-1);
    memcpy(str,"Sending ",8);
    memcpy(str+8,&message[25],strlen(message) - strlen(strstr(message,"\",\r\n")) - 25);
    memcpy(str+strlen(str),"\n",1);
    memcpy(str+strlen(str),data->HOST,HOST_LENGTH);
    memcpy(str+strlen(str)," on port ",9);
    memcpy(str+strlen(str),buffer2,strnlen(buffer2,BUFFER_SIZE));
    memcpy(str+strlen(str),"\n",1);
    memset(data2,0,sizeof(data2));
    strftime(data2,sizeof(data2),"%a %d %b %Y %H:%M:%S UTC\n",&current_UTC_date_and_time);
    memcpy(str+strlen(str),data2,strnlen(data2,sizeof(str)));
    color_print(str,"green");
    memset(str,0,sizeof(str));
  }
 
  const int TOTAL = strnlen(message,BUFFER_SIZE);

  for (sent = 0, bytes = 0; sent < TOTAL; sent+= bytes)
  {
    bytes = send(client_socket,message+sent,TOTAL-sent,MSG_NOSIGNAL);
    if (bytes < 0)
    { 
      SEND_AND_RECEIVE_DATA_SOCKET_THREAD_ERROR(0);
    }
    else if (bytes == 0)  
    {
      break;
    }
  }

  // receive the data
  memset(message,0,sizeof(message));
  for (;;)
  { 
    memset(&buffer, 0, sizeof(buffer));
    // check the size of the data that were about to receive. If it is over BUFFER_SIZE then dont accept it, since it will cause a buffer overflow
    if (((int)recvfrom(client_socket, buffer, BUFFER_SIZE, MSG_DONTWAIT | MSG_PEEK, NULL, NULL) >= (int)sizeof(buffer) - (int)strnlen(message,sizeof(buffer)) && (int)strnlen(message,sizeof(buffer)) > 0) || ((int)recvfrom(client_socket, buffer, BUFFER_SIZE, MSG_DONTWAIT | MSG_PEEK, NULL, NULL) >= (int)sizeof(buffer) && (int)strnlen(message,sizeof(buffer)) == 0))
    {
      SEND_AND_RECEIVE_DATA_SOCKET_THREAD_ERROR(0);
    }    
    // read the socket to see if there is any data, use MSG_DONTWAIT so we dont block the program if there is no data
    recvfrom(client_socket, buffer, BUFFER_SIZE, MSG_DONTWAIT, NULL, NULL);  
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
      SEND_AND_RECEIVE_DATA_SOCKET_THREAD_ERROR(0);
    }
    usleep(200000);   
  }

  // verify the data
  if (verify_data(data2,0,1) == 0)
  {
    SEND_AND_RECEIVE_DATA_SOCKET_THREAD_ERROR(0);
  }

  // parse the message
  memset(message,0,sizeof(message));
  memset(VRF_data.block_blob_signature[data->COUNT],0,strnlen(VRF_data.block_blob_signature[data->COUNT],BUFFER_SIZE));
  memset(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[data->COUNT],0,strnlen(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[data->COUNT],BUFFER_SIZE));
  if (parse_json_data(data2,"block_blob_signature",message,sizeof(message)) == 1 && strlen(message) == VRF_PROOF_LENGTH+VRF_BETA_LENGTH)
  {
    memcpy(VRF_data.block_blob_signature[data->COUNT],message,VRF_PROOF_LENGTH+VRF_BETA_LENGTH);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[data->COUNT],message,VRF_PROOF_LENGTH+VRF_BETA_LENGTH);
  }
  else
  {  
    memcpy(VRF_data.block_blob_signature[data->COUNT],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE)-1);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[data->COUNT],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE)-1);
  }
  close(client_socket);
  freeaddrinfo(settings);
  pthread_exit((void *)(intptr_t)1);

  #undef SEND_AND_RECEIVE_DATA_SOCKET_THREAD_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: remove_inactive_delegates
Description: Removes any inactive delegates from the database
Paramters:
  total_delegates - The total delegates
  total_inactive_delegates - The total inactive delegates
-----------------------------------------------------------------------------------------------------------
*/

void remove_inactive_delegates(int *total_delegates, int *total_inactive_delegates)
{
  // Variables
  char data[BUFFER_SIZE];
  struct delegates delegates[MAXIMUM_AMOUNT_OF_DELEGATES];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;
  long long int total_votes;

  // define macros
  #define DATABASE_COLLECTION "delegates"

  memset(data,0,sizeof(data));

  // initialize the delegates struct
  INITIALIZE_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES,"remove_inactive_delegates_timer_thread",data,current_date_and_time,current_UTC_date_and_time);
  
  memset(data,0,sizeof(data));

  // organize the delegates
  *total_delegates = organize_delegates(delegates,DATABASE_COLLECTION);

  for (count = 0, *total_inactive_delegates = 0; count < *total_delegates; count++)
  {
    sscanf(delegates[count].total_vote_count, "%lld", &total_votes);

    // check if each delegate has not mined a block, and their vote count is under the MINIMUM_AMOUNT_REGISTER_DELEGATE
    if (memcmp(delegates[count].block_producer_block_heights,"",1) == 0 && total_votes < MINIMUM_AMOUNT_REGISTER_DELEGATE)
    {
      // remove the delegate from the database
      memset(data,0,sizeof(data));
      memcpy(data,"{\"public_address\":\"",19);
      memcpy(data+strlen(data),delegates[count].public_address,XCASH_WALLET_LENGTH);
      memcpy(data+strlen(data),"\"}",2);
      if (delete_document_from_collection(database_name,DATABASE_COLLECTION,data,1) == 1)
      {
        (*total_inactive_delegates)++;
      }
    }
  }

  POINTER_RESET_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES);
  return;

  #undef DATABASE_COLLECTION
}



/*
-----------------------------------------------------------------------------------------------------------
Name: remove_inactive_delegates_timer_thread
Description: runs the remove_inactive_delegates function at UTC 00:00
-----------------------------------------------------------------------------------------------------------
*/

void* remove_inactive_delegates_timer_thread(void* parameters)
{
  // Variables
  char data[BUFFER_SIZE];
  int total_delegates = 0;
  int total_inactive_delegates = 0;
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;  

  // unused parameters
  (void)parameters;

  for (;;)
  {
    // check if it is UTC 23:58
    get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);
    if (current_UTC_date_and_time.tm_hour == 23 && current_UTC_date_and_time.tm_min == 58)
    {
      color_print("It is UTC 23:58\nRemoving all inactive delegates from the database","yellow");

      remove_inactive_delegates(&total_delegates,&total_inactive_delegates);

      memset(data,0,sizeof(data));  
      print_start_message(current_date_and_time,current_UTC_date_and_time,"Inactive Delegates",data);      
      memcpy(data,"Amount of delegates: ",21);
      snprintf(data+strlen(data),sizeof(data)-1,"%d",total_delegates);
      memcpy(data,"\nAmount of inactive delegates removed: ",39);
      snprintf(data+strlen(data),sizeof(data)-1,"%d",total_inactive_delegates);
      memcpy(data,"\n",1);
      color_print(data,"yellow");
    }
    sleep(10);
  }
  pthread_exit((void *)(intptr_t)1);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_network_data_nodes_database_timer_thread
Description: Makes sure that all of the network data nodes have the same synced database every BLOCK_TIME interval
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

void* sync_network_data_nodes_database_timer_thread(void* parameters)
{
  // Variables
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;

  // unused parameters
  (void)parameters;

  for (;;)
  {
    usleep(200000);
    get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);
    if (current_UTC_date_and_time.tm_min % BLOCK_TIME == 0 && current_UTC_date_and_time.tm_sec == 0)
    {
      sync_network_data_nodes_database();
      sleep(1);
    }
  }
  pthread_exit((void *)(intptr_t)1);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_all_block_verifiers_list_timer_thread
Description: Updates the block verifiers list every BLOCK_TIME interval
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

void* sync_all_block_verifiers_list_timer_thread(void* parameters)
{
  // Variables
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;

  // unused parameters
  (void)parameters;

  for (;;)
  {
    usleep(200000);
    get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);
    if (current_UTC_date_and_time.tm_min % BLOCK_TIME == 2 && current_UTC_date_and_time.tm_sec == 0)
    {
      // if the block verifier is not a network data node then it needs to run the timer after the networm data nodes, since it loads the block verifiers list from the network data nodes
      if (network_data_node_settings == 0)
      {
        sleep(60);
      }
      sync_all_block_verifiers_list();
      sleep(1);
    }
  }
  pthread_exit((void *)(intptr_t)1);
}