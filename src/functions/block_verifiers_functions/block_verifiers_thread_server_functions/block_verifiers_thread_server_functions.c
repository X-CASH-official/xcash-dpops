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

#include "blockchain_functions.h"
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
Name: check_if_replayed_round
Description: Checks if the round is replayed
Return: 0 if an error has occured or the round is not a replayed round, 1 if the round is a replayed round
-----------------------------------------------------------------------------------------------------------
*/

int check_if_replayed_round(void)
{
  // Variables
  char data[SMALL_BUFFER_SIZE];
  char data2[SMALL_BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  size_t count;
  size_t count2;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  // check if its the first block as it cant be a replayed round
  sscanf(current_block_height,"%zu", &count);
  if (count == XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT)
  {
    return 0;
  }

  // get the reserve bytes data from the previous block
  count--;
  count2 = count;

  // calculate the database to get the reserve byte data
  get_reserve_bytes_database(count,0);
  memcpy(data2,"reserve_bytes_",14);
  snprintf(data2+14,MAXIMUM_NUMBER_SIZE,"%zu",count);

  memcpy(data,"{\"block_height\":\"",17);
  snprintf(data+strlen(data),MAXIMUM_NUMBER_SIZE,"%zu",count2);
  memcpy(data+strlen(data),"\"}",2);

  // get the blocks reserve bytes
  if (read_document_field_from_collection(database_name,data2,data,"reserve_bytes",data3) == 0)
  {
    return 0;
  }

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // get the timestamp from the previous blocks reserve bytes
  memcpy(data,&data3[(sizeof(NETWORK_VERSION)-1)],TIMESTAMP_LENGTH);

  // convert the timestamp from varint to the timestamp
  count = varint_decode((size_t)strtol(data, NULL, 16));

  // check if the previous blocks timestamp is longer than 5 minutes
  return ((time(NULL) - (long int)count) > (BLOCK_TIME * 60)) ? 1 : 0;
}



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
  char data[BUFFER_SIZE_NETWORK_BLOCK_DATA];
  char data2[BUFFER_SIZE_NETWORK_BLOCK_DATA];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  size_t count;
  int settings = 0;
  int block_verifier_settings;
  
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // unused parameters
  (void)parameters;

  // wait for the specific start time if the block_height_start_time is on
  if (block_height_start_time.block_height_start_time == 1 && registration_settings == 0)
  {
    start_time:
    color_print("Waiting for the specific start time","yellow");
    do
    {
      get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);
      nanosleep((const struct timespec[]){{0, 200000000L}}, NULL);  

      // check if it is 30 minutes before the start time and restart, this way non network data nodes get an up to date copy of the database
      if (network_data_node_settings == 0 && current_UTC_date_and_time.tm_year == block_height_start_time.block_height_start_time_year && current_UTC_date_and_time.tm_mon == block_height_start_time.block_height_start_time_month && current_UTC_date_and_time.tm_mday == block_height_start_time.block_height_start_time_day && current_UTC_date_and_time.tm_hour == block_height_start_time.block_height_start_time_hour && block_height_start_time.block_height_start_time_minute - current_UTC_date_and_time.tm_min == 30)
      {
        sleep(60);
        color_print("Restarting to get a up to date copy of the database","green");
        exit(0);
      }
    } while (current_UTC_date_and_time.tm_year != block_height_start_time.block_height_start_time_year || current_UTC_date_and_time.tm_mon != block_height_start_time.block_height_start_time_month || current_UTC_date_and_time.tm_mday != block_height_start_time.block_height_start_time_day || current_UTC_date_and_time.tm_hour != block_height_start_time.block_height_start_time_hour || current_UTC_date_and_time.tm_min != block_height_start_time.block_height_start_time_minute);
  }
  
  // get the current block height and wait until the block height is at the XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT
  get_current_block_height(current_block_height);
  get_previous_block_hash(previous_block_hash);
  sscanf(current_block_height,"%zu", &count);

  if (registration_settings == 0)
  {
    while (count < XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT)
    {
      get_current_block_height(current_block_height);
      get_previous_block_hash(previous_block_hash);
      sscanf(current_block_height,"%zu", &count);
      sleep(1);
    }
  }

  for (;;)
  {
    get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);
    if ((settings == 0 && current_UTC_date_and_time.tm_min % BLOCK_TIME == 0 && current_UTC_date_and_time.tm_sec == 0) || (settings == 1 && current_UTC_date_and_time.tm_min % BLOCK_TIME == 0))
    {
      if (settings == 0)
      {
        if ((block_verifier_settings = start_new_round()) == 0)
        {
          print_error_message(current_date_and_time,current_UTC_date_and_time,data);
        }
        else if (block_verifier_settings == 1)
        {
          memset(data,0,strlen(data));
          memcpy(data,"Your delegate is not a block verifier for network block ",56);
          memcpy(data+56,current_block_height,strnlen(current_block_height,sizeof(data)));
          color_print(data,"red");
        }
        else if (block_verifier_settings == 10)
        {
          goto start_time;
        }
        else
        {
          if (registration_settings == 0)
          {
            memset(data,0,strlen(data));
            memcpy(data,"Network Block ",14);
            memcpy(data+14,current_block_height,strnlen(current_block_height,sizeof(data)));
            memcpy(data+strlen(data)," Has Been Created Successfully\n",31);
            color_print(data,"green");
          }
        }
        settings = 1;
        continue;
      }

      get_current_block_height(current_block_height);
      get_previous_block_hash(previous_block_hash);

      // check if this round is a replayed round
      replayed_round_settings = check_if_replayed_round();

      if ((block_verifier_settings = start_new_round()) == 0)
      {
        print_error_message(current_date_and_time,current_UTC_date_and_time,data);
      }
      else if (block_verifier_settings == 1)
      {
        memset(data,0,strlen(data));
        memcpy(data,"Your delegate is not a block verifier for network block ",56);
        memcpy(data+56,current_block_height,strnlen(current_block_height,sizeof(data)));
        color_print(data,"red");
      }
      else if (block_verifier_settings == 10)
      {
        goto start_time;
      }
      else
      {
        if (registration_settings == 0)
        {
          // wait for the reserve proof checking before displaying the message
          sync_block_verifiers_minutes_and_seconds((BLOCK_TIME-1),45);

          memset(data,0,strlen(data));
          memcpy(data,"Network Block ",14);
          memcpy(data+14,current_block_height,strnlen(current_block_height,sizeof(data)));
          memcpy(data+strlen(data)," Has Been Created Successfully\n",31);
          color_print(data,"green");
        }
      }  
    }
    nanosleep((const struct timespec[]){{0, 200000000L}}, NULL);
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
  char data[DATA_HASH_LENGTH+1];
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

  // copy all of the reserve proofs to the reserve_proofs array
  for (count = 0; count < invalid_reserve_proofs.count; count++)
  {
    memcpy(reserve_proofs[count],invalid_reserve_proofs.reserve_proof[count],strnlen(invalid_reserve_proofs.reserve_proof[count],BUFFER_SIZE_RESERVE_PROOF));
  }

  // organize the invalid reserve proofs struct so the data hash is the same
  qsort(reserve_proofs,invalid_reserve_proofs.count,sizeof(char*),organize_invalid_reserve_proofs_settings);

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

  RESET_RESERVE_PROOFS_ARRAY;
  return sign_data(block_verifiers_message);

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
  char data[100];
  char data2[XCASH_WALLET_LENGTH+1];
  char data3[BUFFER_SIZE_RESERVE_PROOF+BUFFER_SIZE_NETWORK_BLOCK_DATA];
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
    snprintf(data+15,MAXIMUM_NUMBER_SIZE,"%d",count);

    // check if the reserve proof is in the database
    if (count_all_documents_in_collection(database_name,data) > 0)
    {
      memset(data2,0,strlen(data2));
      memset(data3,0,strlen(data3));
      memcpy(data3,"{\"reserve_proof\":\"",18);
      memcpy(data3+18,invalid_reserve_proofs.reserve_proof[CURRENT_RESERVE_PROOF_COUNT],strnlen(invalid_reserve_proofs.reserve_proof[CURRENT_RESERVE_PROOF_COUNT],sizeof(data3)));
      memcpy(data3+strlen(data3),"\"}",2); 

      memset(invalid_reserve_proofs.public_address_created_reserve_proof[CURRENT_RESERVE_PROOF_COUNT],0,strlen(invalid_reserve_proofs.public_address_created_reserve_proof[CURRENT_RESERVE_PROOF_COUNT]));
      memset(invalid_reserve_proofs.public_address_voted_for[CURRENT_RESERVE_PROOF_COUNT],0,strlen(invalid_reserve_proofs.public_address_voted_for[CURRENT_RESERVE_PROOF_COUNT]));
      invalid_reserve_proofs.reserve_proof_amount[CURRENT_RESERVE_PROOF_COUNT] = 0;      
          
      // get the data from the database for the reserve proof. If the data is not in the database then skip that reserve proof when updating the database
      memset(data2,0,strlen(data2));
      if (read_document_field_from_collection(database_name,data,data3,"public_address_created_reserve_proof",data2) == 0)
      {
        return 0;
      }
      memcpy(invalid_reserve_proofs.public_address_created_reserve_proof[CURRENT_RESERVE_PROOF_COUNT],data2,strnlen(data2,XCASH_WALLET_LENGTH));

      memset(data2,0,strlen(data2));
      if (read_document_field_from_collection(database_name,data,data3,"public_address_voted_for",data2) == 0)
      {
        return 0;
      }
      memcpy(invalid_reserve_proofs.public_address_voted_for[CURRENT_RESERVE_PROOF_COUNT],data2,strnlen(data2,XCASH_WALLET_LENGTH));

      memset(data2,0,strlen(data2));
      if (read_document_field_from_collection(database_name,data,data3,"total",data2) == 0)
      {
        return 0;
      }
      sscanf(data2,"%zu", &block_verifiers_total_vote_count);
      invalid_reserve_proofs.reserve_proof_amount[CURRENT_RESERVE_PROOF_COUNT] = block_verifiers_total_vote_count;
      break;
    }                  
  }

  // check if the reserve proof was found in the database and if not skip that reserve proof when updating the database
  return count > TOTAL_RESERVE_PROOFS_DATABASES ? 0 : 1;
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
  char data[BUFFER_SIZE_NETWORK_BLOCK_DATA];
  char data2[BUFFER_SIZE_NETWORK_BLOCK_DATA];
  char data3[BUFFER_SIZE_RESERVE_PROOF+BUFFER_SIZE_NETWORK_BLOCK_DATA];
  size_t block_verifiers_total_vote_count;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  memcpy(data3,"{\"public_address\":\"",19);
  memcpy(data3+strlen(data3),invalid_reserve_proofs.public_address_voted_for[CURRENT_RESERVE_PROOF_COUNT],XCASH_WALLET_LENGTH);
  memcpy(data3+strlen(data3),"\"}",2);

  if (read_document_field_from_collection(database_name,"delegates",data3,"total_vote_count",data) == 0)
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
  memset(data,0,strlen(data));
  snprintf(data,sizeof(data)-1,"%zu",block_verifiers_total_vote_count);  

  memcpy(data2,"{\"total_vote_count\":\"",21);
  memcpy(data2+strlen(data2),data,strnlen(data,sizeof(data2)));
  memcpy(data2+strlen(data2),"\"}",2);

  return update_document_from_collection(database_name,"delegates",data3,data2);
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
  char data[BUFFER_SIZE_NETWORK_BLOCK_DATA];
  char data2[BUFFER_SIZE_NETWORK_BLOCK_DATA];
  char data3[BUFFER_SIZE_NETWORK_BLOCK_DATA];
  size_t block_verifiers_score;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  // create the message
  memcpy(data,"{\"public_address\":\"",19);
  memcpy(data+19,invalid_reserve_proofs.block_verifier_public_address[CURRENT_RESERVE_PROOF_COUNT],strnlen(invalid_reserve_proofs.block_verifier_public_address[CURRENT_RESERVE_PROOF_COUNT],sizeof(data)));
  memcpy(data+strlen(data),"\"}",2);

  // get the block verifiers score
  if (read_document_field_from_collection(database_name,"delegates",data,"block_verifier_score",data2) == 0)
  {
    return 0;
  }
  sscanf(data2, "%zu", &block_verifiers_score);
  block_verifiers_score++;

  memset(data2,0,strlen(data2));
  memcpy(data2,"{\"block_verifier_score\":\"",25);
  snprintf(data2+25,sizeof(data2)-26,"%zu",block_verifiers_score);
  memcpy(data2+strlen(data2),"\"}",2);

  return update_document_from_collection(database_name,"delegates",data,data2);
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
  char data[100];
  char data2[BUFFER_SIZE_RESERVE_PROOF+BUFFER_SIZE_NETWORK_BLOCK_DATA];
  int count;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // get the data for the reserve proof from your own database, since you cant know if the data given was valid since the reserve proof is already invalid
  for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
  {
    memset(data,0,strlen(data));
    memcpy(data,"reserve_proofs_",15);
    snprintf(data+15,MAXIMUM_NUMBER_SIZE,"%d",count);

    memset(data2,0,strlen(data2));
    memcpy(data2,"{\"reserve_proof\":\"",18);
    memcpy(data2+18,invalid_reserve_proofs.reserve_proof[CURRENT_RESERVE_PROOF_COUNT],strnlen(invalid_reserve_proofs.reserve_proof[CURRENT_RESERVE_PROOF_COUNT],sizeof(data2)));
    memcpy(data2+strlen(data2),"\"}",2);
    if (count_documents_in_collection(database_name,data,data2) > 0)
    {
      delete_document_from_collection(database_name,data,data2);
    }
  }

  // reset the error messages since there just going to contain The database collection does not exist if not all reserve proof databases are being used
  RESET_ERROR_MESSAGES;

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
  int count;

  for (count = 0; count < invalid_reserve_proofs.count; count++)
  {
    RESET_ERROR_MESSAGES;

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

    /*// update all of the delegates scores
    if (check_reserve_proofs_timer_update_delegates_score(count) == 0)
    {
      continue;
    }*/

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
  char data[SMALL_BUFFER_SIZE];
  int count;
  int count2;
  int counter;
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
    snprintf(data+15,MAXIMUM_NUMBER_SIZE,"%d",((rand() % (TOTAL_RESERVE_PROOFS_DATABASES - 1 + 1)) + 1));
  } while ((count = count_all_documents_in_collection(database_name,data)) <= 0);
  pthread_rwlock_wrlock(&rwlock_reserve_proofs);
  RESET_ERROR_MESSAGES; 
  pthread_rwlock_unlock(&rwlock_reserve_proofs);

  // select a random document in the collection
  count = (rand() % count) + 1;

  // get a random document from the collection
  if (read_multiple_documents_all_fields_from_collection(database_name,data,"",&database_multiple_documents_fields,count,1,0,"") == 1)
  {
    // check if the reserve proof is unique
    pthread_rwlock_wrlock(&rwlock_reserve_proofs);
    counter = invalid_reserve_proofs.count;
    pthread_rwlock_unlock(&rwlock_reserve_proofs);

    for (count2 = 0; count2 < counter; count2++)
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

int send_invalid_reserve_proof_to_block_verifiers(const struct reserve_proof* reserve_proof)
{
  // Variables
  char data[SMALL_BUFFER_SIZE+BUFFER_SIZE_RESERVE_PROOF];

  memset(data,0,sizeof(data));

  // add the reserve proof to the invalid_reserve_proofs struct
  if (test_settings == 0)
  {
    pthread_mutex_lock(&invalid_reserve_proof_lock);
    invalid_reserve_proofs.block_verifier_public_address[invalid_reserve_proofs.count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    invalid_reserve_proofs.public_address_created_reserve_proof[invalid_reserve_proofs.count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    invalid_reserve_proofs.public_address_voted_for[invalid_reserve_proofs.count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    invalid_reserve_proofs.reserve_proof[invalid_reserve_proofs.count] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));
    memcpy(invalid_reserve_proofs.block_verifier_public_address[invalid_reserve_proofs.count],xcash_wallet_public_address,XCASH_WALLET_LENGTH);
    memcpy(invalid_reserve_proofs.reserve_proof[invalid_reserve_proofs.count],reserve_proof->reserve_proof,strnlen(reserve_proof->reserve_proof,sizeof(reserve_proof->reserve_proof)));
    invalid_reserve_proofs.count++;
    pthread_mutex_unlock(&invalid_reserve_proof_lock);
  }

  // send the reserve proof to all block verifiers
  // create the message
  memcpy(data,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS\",\r\n \"public_address_that_created_the_reserve_proof\": \"",137);
  memcpy(data+strlen(data),reserve_proof->public_address_created_reserve_proof,strnlen(reserve_proof->public_address_created_reserve_proof,sizeof(data)));
  memcpy(data+strlen(data),"\",\r\n \"reserve_proof\": \"",23);
  memcpy(data+strlen(data),reserve_proof->reserve_proof,strnlen(reserve_proof->reserve_proof,sizeof(data)));
  memcpy(data+strlen(data),"\",\r\n}",5);

  // sign_data
  if (sign_data(data) == 0)
  { 
    return 0;
  }

  // send the message to all block verifiers
  return block_verifiers_send_data_socket((const char*)data);
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
  char data[SMALL_BUFFER_SIZE];
  char data2[SMALL_BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;
  int settings;
  struct reserve_proof reserve_proof;
  
  // unused parameters
  (void)parameters;

  // define macros
  #define RESET_INVALID_RESERVE_PROOFS_DATA \
  for (count = 0; count < invalid_reserve_proofs.count; count++) \
  { \
    pointer_reset(invalid_reserve_proofs.block_verifier_public_address[count]); \
    pointer_reset(invalid_reserve_proofs.public_address_created_reserve_proof[count]); \
    pointer_reset(invalid_reserve_proofs.public_address_voted_for[count]); \
    pointer_reset(invalid_reserve_proofs.reserve_proof[count]); \
  } \
  invalid_reserve_proofs.count = 0; \
  color_print("Stopping the check reserve proofs timer thread","yellow"); \
  fprintf(stderr,"\n"); \
  pthread_exit((void *)(intptr_t)1);

  // reset the reserve_proof struct
  memset(reserve_proof.block_verifier_public_address,0,sizeof(reserve_proof.block_verifier_public_address));
  memset(reserve_proof.public_address_created_reserve_proof,0,sizeof(reserve_proof.public_address_created_reserve_proof));
  memset(reserve_proof.public_address_voted_for,0,sizeof(reserve_proof.public_address_voted_for));
  memset(reserve_proof.reserve_proof_amount,0,sizeof(reserve_proof.reserve_proof_amount));
  memset(reserve_proof.reserve_proof,0,sizeof(reserve_proof.reserve_proof));

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  
  color_print("Started the check reserve proofs timer thread","green");
  color_print("Part 1 - Randomly select reserve proofs and check if they are valid","yellow");

  // check if there are any reserve proofs in the database
  for (count = 1, settings = 0; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
  {
    memset(data,0,sizeof(data));
    memcpy(data,"reserve_proofs_",15);
    snprintf(data+15,MAXIMUM_NUMBER_SIZE,"%d",count);
    if (count_all_documents_in_collection(database_name,data) > 0)
    {
      settings = 1;
    }
  }

  if (settings == 0)
  {
    color_print("There are no reserve proofs in the database","yellow");
    RESET_INVALID_RESERVE_PROOFS_DATA;
  }

  for (;;)
  {
    get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);
    if (current_UTC_date_and_time.tm_min % BLOCK_TIME == (BLOCK_TIME-1))
    {
      // check if there was any invalid reserve proofs found
      if (invalid_reserve_proofs.count <= 0)
      {
        color_print("No invalid reserve proofs were found","yellow");
        RESET_INVALID_RESERVE_PROOFS_DATA;
      }

      // wait for any block verifiers sending messages, or any block verifiers waiting to process a reserve proof
      sync_block_verifiers_minutes_and_seconds((BLOCK_TIME-1),15);

      /* disable this part for now

      color_print("Part 2 - Send all invalid reserve proofs to all block verifiers","yellow");

      // create the data to send to the block verifiers
      if (check_reserve_proofs_timer_create_message(data2) == 0)
      {
        RESET_INVALID_RESERVE_PROOFS_DATA;
      }

      // send the message to all block verifiers
      if (block_verifiers_send_data_socket((const char*)data2) == 0)
      {
        RESET_INVALID_RESERVE_PROOFS_DATA;
      }

      // wait for the block verifiers to process the votes
      sync_block_verifiers_minutes_and_seconds((BLOCK_TIME-1),START_TIME_SECONDS_INVALID_RESERVE_PROOFS_PART_3);

      color_print("Part 3 - Check if the valid amount of block verifiers had the same invalid reserve proofs","yellow");

      // process the vote results
      if ((registration_settings == 0 && current_round_part_vote_data.vote_results_valid >= BLOCK_VERIFIERS_VALID_AMOUNT) || (registration_settings == 1 && current_round_part_vote_data.vote_results_valid >= NETWORK_DATA_NODES_VALID_AMOUNT))
      {
        registration_settings == 0 ? fprintf(stderr,"\033[1;32m%d / %d block verifiers have the same invalid reserve proofs\033[0m\n\n",current_round_part_vote_data.vote_results_valid,BLOCK_VERIFIERS_VALID_AMOUNT) : fprintf(stderr,"\033[1;32m%d / %d network data nodes have the same invalid reserve proofs\033[0m\n\n",current_round_part_vote_data.vote_results_valid,NETWORK_DATA_NODES_VALID_AMOUNT);
      }
      else
      {
        fprintf(stderr,"\033[1;31m%d / %d block verifiers have the same invalid reserve proofs\033[0m\n\n",current_round_part_vote_data.vote_results_valid,BLOCK_VERIFIERS_VALID_AMOUNT);
        RESET_INVALID_RESERVE_PROOFS_DATA;
      }      

      color_print("Part 4 - Remove the invalid reserve proofs from the database","yellow");*/

      color_print("Part 2 - Remove the invalid reserve proofs from the database","yellow");

      // update the database
      check_reserve_proofs_timer_update_database();

      color_print("Invalid reserve proofs have been removed from the database","yellow");

      // reset the invalid_reserve_proofs and the block_verifiers_invalid_reserve_proofs
      RESET_INVALID_RESERVE_PROOFS_DATA;
    }
    
    // check if the reserve proof is invalid
    memset(data,0,strlen(data));
    if (select_random_unique_reserve_proof(&reserve_proof) == 1 && check_reserve_proofs(data,reserve_proof.public_address_created_reserve_proof,reserve_proof.reserve_proof) == 0)
    {
      send_invalid_reserve_proof_to_block_verifiers(&reserve_proof);
    }
    nanosleep((const struct timespec[]){{0, 200000000L}}, NULL);
  }
}



/*
-----------------------------------------------------------------------------------------------------------
Name: remove_inactive_delegates
Description: Removes any inactive delegates from the database
-----------------------------------------------------------------------------------------------------------
*/

void remove_inactive_delegates(void)
{
  // Variables
  char data[BUFFER_SIZE_NETWORK_BLOCK_DATA];
  struct delegates delegates[MAXIMUM_AMOUNT_OF_DELEGATES];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;
  int count2;
  long long int total_votes;
  int total_delegates;

  // define macros
  #define DATABASE_COLLECTION "delegates"

  memset(data,0,sizeof(data));

  // initialize the delegates struct
  INITIALIZE_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES,"remove_inactive_delegates_timer_thread",data,current_date_and_time,current_UTC_date_and_time);

  // organize the delegates
  
  if ((total_delegates = organize_delegates(delegates)) == 0)
  {
    POINTER_RESET_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES);
    return;
  }

  for (count = 0; count < total_delegates; count++)
  {
    sscanf(delegates[count].total_vote_count, "%lld", &total_votes);

    // check to make sure it is not a network data node
    for (count2 = 0; count2 < NETWORK_DATA_NODES_AMOUNT; count2++)
    {
      if (strncmp(network_data_nodes_list.network_data_nodes_public_address[count2],delegates[count].public_address,XCASH_WALLET_LENGTH) == 0)
      {
        continue;
      }
    }

    // check to make sure it is not an official delegate
    if (strncmp(OFFICIAL_SHARED_DELEGATE_PUBLIC_ADDRESS_PRODUCTION,delegates[count].public_address,XCASH_WALLET_LENGTH) == 0)
    {
      continue;
    }

    // check if each delegate has not mined a block, and their vote count is under the MINIMUM_AMOUNT_REGISTER_DELEGATE
    if (strncmp(delegates[count].block_producer_block_heights,"",1) == 0 && total_votes < MINIMUM_AMOUNT_REGISTER_DELEGATE)
    {
      // remove the delegate from the database
      memset(data,0,strlen(data));
      memcpy(data,"{\"public_address\":\"",19);
      memcpy(data+strlen(data),delegates[count].public_address,XCASH_WALLET_LENGTH);
      memcpy(data+strlen(data),"\"}",2);
      delete_document_from_collection(database_name,DATABASE_COLLECTION,data);
    }
  }
  RESET_ERROR_MESSAGES;
  POINTER_RESET_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES);
  return;

  #undef DATABASE_COLLECTION
}
