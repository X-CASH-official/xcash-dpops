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
#include "define_macros_test.h"
#include "structures.h"
#include "variables.h"
#include "initialize_and_reset_structs_define_macros.h"

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
#include "server_functions.h"
#include "string_functions.h"
#include "shared_delegate_website_thread_server_functions.h"
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
Name: check_if_previous_block_producer
Description: Checks if the delegate is the previous block producer
Return: 0 if an error has occured or if the delegate is not the previous block producer, 1 if the delegate is the previous block producer
-----------------------------------------------------------------------------------------------------------
*/

int check_if_previous_block_producer(void)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  char previous_block_producer[XCASH_WALLET_LENGTH+1];
  char* message;
  size_t count;
  size_t count2;
  
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));
  memset(previous_block_producer,0,sizeof(previous_block_producer));  

  // check if it is a valid block height
  sscanf(current_block_height, "%zu", &count);
  count--;

  if (count <= XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT)
  {
    return 0;
  }

  if (test_settings == 0)
  {
    // create the message
    memcpy(data2,"{\"block_height\":\"",17);
    snprintf(data2+17,MAXIMUM_NUMBER_SIZE,"%zu",count);
    memcpy(data2+strlen(data2),"\"}",2);

    // get the current reserve_bytes database
    get_reserve_bytes_database(count,1);
    memcpy(data3,"reserve_bytes_",14);
    snprintf(data3+14,MAXIMUM_NUMBER_SIZE,"%zu",count);

    // get the previous blocks reserve bytes
    if (read_document_field_from_collection(database_name,data3,data2,"reserve_bytes",data) == 0)
    {    
      return 0;
    }
  }
  else if (test_settings == 1)
  {
    memcpy(data,NETWORK_BLOCK,sizeof(NETWORK_BLOCK)-1);
  }
  
  if ((message = strstr(data,BLOCKCHAIN_DATA_SEGMENT_STRING)) == NULL)
  {
    return 0;
  }

  memset(data3,0,sizeof(data3));

  memcpy(data3,&message[sizeof(BLOCKCHAIN_DATA_SEGMENT_STRING)-1],XCASH_WALLET_LENGTH*2);

  // convert the hexadecimal string to a string
  for (count = 0, count2 = 0; count < XCASH_WALLET_LENGTH*2; count2++, count += 2)
  {
    memset(data2,0,sizeof(data2));
    memcpy(data2,&data3[count],2);
    previous_block_producer[count2] = (char)strtol(data2, NULL, 16);
  }

  // check if the delegate is the previous block producer
  return strncmp(previous_block_producer,xcash_wallet_public_address,sizeof(xcash_wallet_public_address)) == 0 ? 1 : 0;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: add_block_to_blocks_found
Description: Adds the previous block on the network to the blocks_found database
Return: 0 if an error has occured, else the block reward
-----------------------------------------------------------------------------------------------------------
*/

long long int add_block_to_blocks_found(void)
{
  // Variables
  char data[SMALL_BUFFER_SIZE];
  char block_height[100];
  char block_hash[BLOCK_HASH_LENGTH+1];  
  char block_reward[100];
  char block_date_and_time[100];
  long long int number;
  long long int block_reward_number = 0;

  // define macros
  #define DATABASE_COLLECTION "blocks_found"

  #define ADD_BLOCK_TO_BLOCKS_FOUND_ERROR(message) \
  memcpy(error_message.function[error_message.total],"add_block_to_blocks_found",25); \
  memcpy(error_message.data[error_message.total],message,sizeof(message)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));
  memset(block_height,0,sizeof(block_height));
  memset(block_hash,0,sizeof(block_hash));
  memset(block_reward,0,sizeof(block_reward));
  memset(block_date_and_time,0,sizeof(block_date_and_time));

  // get the block information of the previous block
  if (get_previous_block_information(block_hash,block_reward,block_date_and_time) == 0)
  {
    ADD_BLOCK_TO_BLOCKS_FOUND_ERROR("Could not get the previous block information.\nCould not check if the block verifier found the last block.");
  }

  // convert the block_reward to a number
  sscanf(block_reward, "%lld", &block_reward_number);

  // get the block height of the previous block
  sscanf(current_block_height, "%lld", &number);
  number--;
  snprintf(block_height,sizeof(block_height)-1,"%lld",number);

  // check if the block is already in the database
  memset(data,0,sizeof(data));
  memcpy(data,"{\"block_height\":\"",17);
  memcpy(data+strlen(data),block_height,strnlen(block_height,sizeof(block_height)));
  memcpy(data+strlen(data),"\"}",2);

  if (count_documents_in_collection(shared_delegates_database_name,DATABASE_COLLECTION,data) >= 1)
  {
    ADD_BLOCK_TO_BLOCKS_FOUND_ERROR("The block is already in the database.");
  }

  // add the blocks data
  memset(data,0,sizeof(data));
  memcpy(data,"{\"block_height\":\"",17);
  memcpy(data+strlen(data),block_height,strnlen(block_height,sizeof(block_height)));
  memcpy(data+strlen(data),"\",\"block_hash\":\"",16);
  memcpy(data+strlen(data),block_hash,BLOCK_HASH_LENGTH);
  memcpy(data+strlen(data),"\",\"block_date_and_time\":\"",25);
  memcpy(data+strlen(data),block_date_and_time,strnlen(block_date_and_time,sizeof(block_date_and_time)));
  memcpy(data+strlen(data),"\",\"block_reward\":\"",18);
  memcpy(data+strlen(data),block_reward,strnlen(block_reward,sizeof(block_reward)));
  memcpy(data+strlen(data),"\"}",2);

  if (insert_document_into_collection_json(shared_delegates_database_name,DATABASE_COLLECTION,data) == 0)
  {
    ADD_BLOCK_TO_BLOCKS_FOUND_ERROR("Could not add the block to the blocks_found database.\nCould not check if the block verifier found the last block.");
  }
  return block_reward_number;

  #undef DATABASE_COLLECTION
  #undef ADD_BLOCK_TO_BLOCKS_FOUND_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_delegates_total_voters_count
Description: Counts all the delegates voters
Parameters:
  DELEGATES_PUBLIC_ADDRESS - The delegates public address
Return: 0 if an error has occured, otherwise the amount of voters for the delegate
-----------------------------------------------------------------------------------------------------------
*/

int get_delegates_total_voters_count(const char* DELEGATES_PUBLIC_ADDRESS)
{
  // Variables
  char data[1024];
  char data2[1024];
  int public_address_count;
  int count;
  int counter;

  memset(data,0,sizeof(data));
  memcpy(data,"{\"public_address_voted_for\":\"",29);
  memcpy(data+29,DELEGATES_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\"}",2);

  // get the count of how many public addresses voted for the delegate
  for (public_address_count = 0, count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
  { 
    memset(data2,0,strlen(data2));
    memcpy(data2,"reserve_proofs_",15);
    snprintf(data2+15,MAXIMUM_NUMBER_SIZE,"%d",count);
    if ((counter = count_documents_in_collection(database_name,data2,data)) != -1)
    {
      public_address_count += counter;
    }
  }
  return public_address_count;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_delegates_total_voters
Description: Gets the delegates total voters
Parameters:
  struct voters - The voters
Return: -1 if an error has occured, otherwise the total votes from all of the voters for the delegate
-----------------------------------------------------------------------------------------------------------
*/

long long int get_delegates_total_voters(struct voters* voters)
{
  // Variables
  char data[1024];
  char data2[1024];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;
  int count2;
  int counter;
  int private_group_count;
  int voters_count;
  long long int total_votes = 0;
  long long int number;
  struct database_multiple_documents_fields database_multiple_documents_fields;

  #define GET_DELEGATES_TOTAL_VOTERS_ERROR(message) \
  memcpy(error_message.function[error_message.total],"get_delegates_total_voters",26); \
  memcpy(error_message.data[error_message.total],message,sizeof(message)-1); \
  error_message.total++; \
  RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,count2,TOTAL_RESERVE_PROOFS_DATABASE_FIELDS); \
  return -1;

  // initialize the database_multiple_documents_fields struct 
  INITIALIZE_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,count2,MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE,TOTAL_RESERVE_PROOFS_DATABASE_FIELDS,"get_delegates_total_voters",data,current_date_and_time,current_UTC_date_and_time);

  memset(data,0,sizeof(data));
  memcpy(data,"{\"public_address_voted_for\":\"",29);
  memcpy(data+29,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(data+127,"\"}",2);

  // get the count of how many public addresses voted for the delegate
  for (total_votes = 0, voters_count = 0, counter = 1; counter <= TOTAL_RESERVE_PROOFS_DATABASES; counter++)
  { 
    memset(data2,0,strlen(data2));
    memcpy(data2,"reserve_proofs_",15);
    snprintf(data2+15,MAXIMUM_NUMBER_SIZE,"%d",counter);

    if ((count2 = count_documents_in_collection(database_name,data2,data)) > 0)
    {
      if (read_multiple_documents_all_fields_from_collection(database_name,data2,data,&database_multiple_documents_fields,1,count2,0,"") == 0)
      {
        GET_DELEGATES_TOTAL_VOTERS_ERROR("Could not read the reserve proofs database.\nCould not calculate the block reward for each delegate");
      }

      // copy the data to the voters struct
      for (count = 0; count < count2; count++, voters_count++)
      {
        memcpy(voters[voters_count].public_address,database_multiple_documents_fields.value[count][0],XCASH_WALLET_LENGTH);

        // if the private group is on and the public address is not in the private group, set the total vote count to 0, and this will also not add to the overall total vote count of all of the votes
        if (private_group.private_group_settings == 1)
        {
          for (private_group_count = 0; private_group_count < MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE_PRIVATE_GROUP; private_group_count++)
          {
            if (strncmp(voters[voters_count].public_address,private_group.private_group_voting_public_address[private_group_count],BUFFER_SIZE) == 0)
            {
              break;
            }
          }
        }
        private_group.private_group_settings == 1 && private_group_count == MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE_PRIVATE_GROUP ? memcpy(voters[voters_count].total_vote_count,"0",1) : memcpy(voters[voters_count].total_vote_count,database_multiple_documents_fields.value[count][2],strnlen(database_multiple_documents_fields.value[count][2],100));
        
        sscanf(voters[voters_count].total_vote_count, "%lld", &number);
        voters[voters_count].total_votes = number;
        total_votes += number;
      }

      // reset the database_multiple_documents_fields struct
      for (count = 0; count < MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE; count++)
      {
        for (count2 = 0; count2 < TOTAL_RESERVE_PROOFS_DATABASE_FIELDS; count2++)
        {
          memset(database_multiple_documents_fields.item[count][count2],0,strlen(database_multiple_documents_fields.item[count][count2]));
          memset(database_multiple_documents_fields.value[count][count2],0,strlen(database_multiple_documents_fields.value[count][count2]));
        }
      }
      database_multiple_documents_fields.document_count = 0;
      database_multiple_documents_fields.database_fields_count = 0;
    }
  }
  database_multiple_documents_fields.document_count = MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE;
  POINTER_RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,count2,TOTAL_RESERVE_PROOFS_DATABASE_FIELDS);
  return total_votes;
  
  #undef GET_DELEGATES_TOTAL_VOTERS_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: calculate_block_reward_for_each_delegate
Description: Calculates the block reward for each delegate
Parameters:
  block_reward - The block reward
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int calculate_block_reward_for_each_delegate(long long int block_reward)
{
  // Variables
  char data[SMALL_BUFFER_SIZE];
  char data2[SMALL_BUFFER_SIZE];
  char data3[SMALL_BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;
  long long int number;
  long long int total_votes;
  int total_voters;
  long long int current_delegates_block_reward;
  struct voters voters[MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE];

  // define macros
  #define DATABASE_COLLECTION "public_addresses"
  #define CALCULATE_BLOCK_REWARD_FOR_EACH_DELEGATE_ERROR(message) \
  memcpy(error_message.function[error_message.total],"calculate_block_reward_for_each_delegate",40); \
  memcpy(error_message.data[error_message.total],message,sizeof(message)-1); \
  error_message.total++; \
  POINTER_RESET_VOTERS_STRUCT(count,MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE); \
  return 0;

  // initialize the voters struct
  INITIALIZE_VOTERS_STRUCT(count,MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE,"calculate_block_reward_for_each_delegate",data,current_date_and_time,current_UTC_date_and_time);

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  // create the message
  memset(data,0,strlen(data));
  memcpy(data,"{\"public_address_voted_for\":\"",29);
  memcpy(data+29,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(data+127,"\"}",2); 

  // check if the delegate has the maximum amount of voters
  if ((total_voters = get_delegates_total_voters_count(xcash_wallet_public_address)) < 1)
  {
    CALCULATE_BLOCK_REWARD_FOR_EACH_DELEGATE_ERROR("The delegate does not have enough voters to distribute the block rewards with");
  }
  
  // get the total votes for the delegate and a list of all of the voters
  if ((total_votes = get_delegates_total_voters(voters)) == -1)
  {
    total_votes = 0;
  }

  // calculate the block reward for all of the voters
  for (count = 0; count < total_voters; count++)
  {
    // if the total_votes is 0 skip the delegate
    if (voters[count].total_votes == 0)
    {
      continue;
    }

    // create the message
    memset(data2,0,sizeof(data2));
    memcpy(data2,"{\"public_address\":\"",19);
    memcpy(data2+19,voters[count].public_address,XCASH_WALLET_LENGTH);
    memcpy(data2+117,"\"}",2);
    memset(data,0,sizeof(data));

    // check if the public address is in the database, if not add it
    if (count_documents_in_collection(shared_delegates_database_name,DATABASE_COLLECTION,data2) <= 0)
    {
      memset(data3,0,sizeof(data3));
      memcpy(data3,"{\"public_address\":\"",19);
      memcpy(data3+19,voters[count].public_address,XCASH_WALLET_LENGTH);
      memcpy(data3+strlen(data3),"\",\"current_total\":\"0\",\"total\":\"0\",\"inactivity_count\":\"0\"}",57);

      if (insert_document_into_collection_json(shared_delegates_database_name,DATABASE_COLLECTION,data3) == 0)
      {
        CALCULATE_BLOCK_REWARD_FOR_EACH_DELEGATE_ERROR("Could not calculate block reward for all of the public address that voted for the delegate.");
      }
    }

    // calculate the current delegates block reward
    current_delegates_block_reward = (long long int)((((double)voters[count].total_votes / (double)total_votes) * (double)block_reward) * ((100 - fee)/100)) | 0;
    
    if (read_document_field_from_collection(shared_delegates_database_name,DATABASE_COLLECTION,data2,"current_total",data) == 0)
    {
      CALCULATE_BLOCK_REWARD_FOR_EACH_DELEGATE_ERROR("Could not calculate block reward for all of the public address that voted for the delegate.");
    }
    
    sscanf(data, "%lld", &number);
    number += current_delegates_block_reward;
    memset(data,0,sizeof(data));
    memset(data3,0,sizeof(data3));
    snprintf(data3,sizeof(data3)-1,"%lld",number);
    memcpy(data,"{\"current_total\":\"",18);
    memcpy(data+18,data3,strnlen(data3,sizeof(data)));
    memcpy(data+strlen(data),"\"}",2);
    
    if (update_document_from_collection(shared_delegates_database_name,DATABASE_COLLECTION,data2,data) == 0)
    {
      CALCULATE_BLOCK_REWARD_FOR_EACH_DELEGATE_ERROR("Could not calculate block reward for all of the public address that voted for the delegate.");
    } 
  }
  POINTER_RESET_VOTERS_STRUCT(count,MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE);
  return 1;

  #undef CALCULATE_BLOCK_REWARD_FOR_EACH_DELEGATE_ERROR
  #undef DATABASE_COLLECTION
}



/*
-----------------------------------------------------------------------------------------------------------
Name: block_height_timer_thread
Description: Checks if the block verifier has found a block
-----------------------------------------------------------------------------------------------------------
*/

void* block_height_timer_thread(void* parameters)
{
  // Variables
  char data[1024];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  long long int block_reward_number;

  // unused parameters
  (void)parameters;

  // define macros
  #define BLOCK_HEIGHT_TIMER_THREAD_ERROR(message) \
  memcpy(error_message.function[error_message.total],"block_height_timer_thread",25); \
  memcpy(error_message.data[error_message.total],message,sizeof(message)-1); \
  error_message.total++; \
  print_error_message(current_date_and_time,current_UTC_date_and_time,data); \
  sleep(1); \
  continue;

  for (;;)
  {   
    get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);

    // check if you found the previous block in the network
    if (current_UTC_date_and_time.tm_min % BLOCK_TIME == 2 && current_UTC_date_and_time.tm_sec == 0 && check_if_previous_block_producer() == 1)
    {
      // load the private group configuration
      if (private_group.private_group_settings == 1 && load_private_group_configuration() == 0)
      {
        color_print("The private group file could not be loaded","yellow");
        exit(0);
      } 
      
      // make sure the round is not replayed
      if (replayed_round_settings == 1)
      {
        sleep(1);
        continue;
      }

      if ((block_reward_number = add_block_to_blocks_found()) == 0)
      {
        BLOCK_HEIGHT_TIMER_THREAD_ERROR("Could not add the previous block that the block verifier found to the database");
      }

      if (calculate_block_reward_for_each_delegate(block_reward_number) == 0)
      {
        BLOCK_HEIGHT_TIMER_THREAD_ERROR("Could not calculate the block reward for each delegate");
      }
      sleep(1);
    }
    nanosleep((const struct timespec[]){{0, 200000000L}}, NULL);
  }
  pthread_exit((void *)(intptr_t)1);

  #undef BLOCK_HEIGHT_TIMER_THREAD_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: payment_timer_send_payment_and_update_databases
Description: Sends the payment if over the minimum amount and updates the databases
Parameters:
  PUBLIC_ADDRESS - The public address to send the trasnaction to
  CURRENT_TOTAL - The current total
  TOTAL - The total
  TX_HASH - The transaction hash of the payment
  TX_KEY - The transaction key of the payment
Return: 0 if an error has occured, otherwise the transaction total
-----------------------------------------------------------------------------------------------------------
*/

long long int payment_timer_send_payment_and_update_databases(const char* PUBLIC_ADDRESS,const char* CURRENT_TOTAL,const char* TOTAL,const char* TX_HASH,const char* TX_KEY)
{
  // Variables
  char data[10000];
  char data2[10000];
  char data3[10000];
  int private_group_count;
  long long int number;
  long long int updated_total; 

  // send the payment
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  // create the message
  memcpy(data,"{\"public_address\":\"",19);
  memcpy(data+19,PUBLIC_ADDRESS,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\"}",2);
          
  // reset the current total in the public_addresses collection
  memcpy(data2,"{\"current_total\":\"0\"}",21);
  if (update_document_from_collection(shared_delegates_database_name,"public_addresses",data,data2) == 0)
  {
    return 0;
  } 

  // add the current total to the total in the public_addresses collection
  sscanf(TOTAL, "%lld", &updated_total);
  sscanf(CURRENT_TOTAL,"%lld",&number);
  updated_total += number;
  snprintf(data3,sizeof(data3)-1,"%lld",updated_total);
  memset(data2,0,sizeof(data2));
  memcpy(data2,"{\"total\":\"",10);
  memcpy(data2+10,data3,strnlen(data3,sizeof(data2)));
  memcpy(data2+strlen(data2),"\"}",2);

  if (update_document_from_collection(shared_delegates_database_name,"public_addresses",data,data2) == 0)
  {
    return 0;
  }

  // if the voter is the delegates public address, dont add a payment to the database since it does not send payments to itself
  if (strncmp(PUBLIC_ADDRESS,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0)
  {
    sscanf(CURRENT_TOTAL, "%lld", &number);
    return number;
  }

  // get the current date and time
  memset(data3,0,sizeof(data3));
  snprintf(data3,sizeof(data3)-1,"%lld",(long long int)time(NULL));

  // check if the private group is on, and if so send it to the payment address instead of the voter address
  if (private_group.private_group_settings == 1)
  {
    for (private_group_count = 0; private_group_count < MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE_PRIVATE_GROUP; private_group_count++)
    {
      if (strncmp(PUBLIC_ADDRESS,private_group.private_group_voting_public_address[private_group_count],BUFFER_SIZE) == 0)
      {
        break;
      }
    }
  }

  // add the payment to the public_addresses_payments in the collection
  memset(data2,0,sizeof(data2));
  memcpy(data2,"{\"public_address\":\"",19);
  memcpy(data2+19,PUBLIC_ADDRESS,XCASH_WALLET_LENGTH);
  memcpy(data2+strlen(data2),"\",\"payment_name\":\"",18);
  
  if (private_group.private_group_settings == 1)
  {
    memcpy(data2+strlen(data2),private_group.private_group_name[private_group_count],strnlen(private_group.private_group_name[private_group_count],sizeof(data2)));
  }
  
  memcpy(data2+strlen(data2),"\",\"payment_address\":\"",21);
  private_group.private_group_settings == 1 ? memcpy(data2+strlen(data2),private_group.private_group_payment_public_address[private_group_count],XCASH_WALLET_LENGTH) : memcpy(data2+strlen(data2),PUBLIC_ADDRESS,XCASH_WALLET_LENGTH);
  
  memcpy(data2+strlen(data2),"\",\"total\":\"",11);
  memcpy(data2+strlen(data2),CURRENT_TOTAL,strnlen(CURRENT_TOTAL,sizeof(data2)));
  memcpy(data2+strlen(data2),"\",\"date_and_time\":\"",19);
  memcpy(data2+strlen(data2),data3,strnlen(data3,sizeof(data2)));
  memcpy(data2+strlen(data2),"\",\"tx_hash\":\"",13);
  memcpy(data2+strlen(data2),TX_HASH,TRANSACTION_HASH_LENGTH);
  memcpy(data2+strlen(data2),"\",\"tx_key\":\"",12);
  memcpy(data2+strlen(data2),TX_KEY,TRANSACTION_HASH_LENGTH);
  memcpy(data2+strlen(data2),"\"}",2);

  if (insert_document_into_collection_json(shared_delegates_database_name,"public_addresses_payments",data2) == 0)
  {
    return 0;
  }
  sscanf(CURRENT_TOTAL, "%lld", &number);
  return number;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: payment_timer_update_inactivity_count
Description: Updates the inactivity count
Parameters:
  PUBLIC_ADDRESS - The public address to send the trasnaction to
  CURRENT_TOTAL - The current total
  INACTIVITY_COUNT - The inactivity count
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int payment_timer_update_inactivity_count(const char* PUBLIC_ADDRESS,const char* CURRENT_TOTAL,const char* INACTIVITY_COUNT)
{
  // Variables
  char data[SMALL_BUFFER_SIZE];
  char data2[SMALL_BUFFER_SIZE];
  char data3[SMALL_BUFFER_SIZE];
  long long int inactivity_count;
  long long int maximum_inactivity_count;

  // define macros
  #define DATABASE_COLLECTION "public_addresses"

  /*
  set the inactivity_count
  if the address has a 0 current_total before sending any payments the inactivity_count ++
  if any address has not a 0 current_total and has an inactivity_count > 0 then reset it to 0
  if any address has an inactivity_count of the maximum inactivity count (default is 30) and a current_total of 0 before sending a payment, remove this document from the collection
  */

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  // create the message
  memcpy(data,"{\"public_address\":\"",19);
  memcpy(data+19,PUBLIC_ADDRESS,XCASH_WALLET_LENGTH);
  memcpy(data+strlen(data),"\"}",2);

  sscanf(INACTIVITY_COUNT, "%lld", &inactivity_count);
  sscanf(voter_inactivity_count, "%lld", &maximum_inactivity_count);                

  if (strncmp(CURRENT_TOTAL,"0",BUFFER_SIZE) == 0 && inactivity_count <= maximum_inactivity_count)
  {
    // update the amount_of_payments in the database
    memset(data3,0,sizeof(data3));
    snprintf(data3,sizeof(data3)-1,"%lld",inactivity_count + 1);
    memcpy(data2,"{\"inactivity_count\":\"",21);
    memcpy(data2+21,data3,strnlen(data3,sizeof(data2)));
    memcpy(data2+strlen(data2),"\"}",2);

    if (update_document_from_collection(shared_delegates_database_name,DATABASE_COLLECTION,data,data2) == 0)
    {
      return 0;
    }
  }
  else if (strncmp(CURRENT_TOTAL,"0",BUFFER_SIZE) != 0)
  {
    // set the inactivity_count to 0
    memcpy(data2,"{\"inactivity_count\":\"0\"}",24);
    if (update_document_from_collection(shared_delegates_database_name,DATABASE_COLLECTION,data,data2) == 0)
    {
      return 0;
    }
  }
  else if (strncmp(CURRENT_TOTAL,"0",BUFFER_SIZE) == 0 && inactivity_count > maximum_inactivity_count)
  {
    // remove the document from the database
    if (delete_document_from_collection(shared_delegates_database_name,DATABASE_COLLECTION,data) == 0)
    {
      return 0;
    }                   
  }
  return 1;

  #undef DATABASE_COLLECTION
}



/*
-----------------------------------------------------------------------------------------------------------
Name: load_private_group_configuration
Description: Loads the private group configuration from the configuration file
Return: 0 if an error has occured, otherwise 1
-----------------------------------------------------------------------------------------------------------
*/

int load_private_group_configuration(void)
{
  // Variables
  char data[BUFFER_SIZE];
  char data_read[SMALL_BUFFER_SIZE];
  char data_read_2[SMALL_BUFFER_SIZE];
  char* search_item;
  char* search_item_data;
  char* search_item_2;
  char* search_item_data_2;
  int count;
  int count2;

  memset(data,0,sizeof(data));

  // reset all of the private group members
  for (count = 0; count < MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE_PRIVATE_GROUP; count++)
  {
    memset(private_group.private_group_name[count],0,sizeof(private_group.private_group_name[count]));
    memset(private_group.private_group_voting_public_address[count],0,sizeof(private_group.private_group_voting_public_address[count]));
    memset(private_group.private_group_payment_public_address[count],0,sizeof(private_group.private_group_payment_public_address[count]));
  }

  // read the entire file into a string
  if (read_file((unsigned char*)data,private_group.private_group_file) == 0)
  {
    return 0;
  }

  // parse the private group data
  for (count = 0, search_item = strtok_r(data, "\n",&search_item_data); search_item != NULL; count++, search_item = strtok_r(NULL, "\n",&search_item_data))
  {
    memset(data_read,0,sizeof(data_read));
    memcpy(data_read,search_item,strnlen(search_item,sizeof(data_read)));

    // check if this line is not data
    if (string_count(data_read,"|") != 2)
    {
      // subtract one from the count, so it stays the same on the next loop
      count--;
      continue;
    }

    for (count2 = 0, search_item_2 = strtok_r(data_read, "|",&search_item_data_2); search_item_2 != NULL; count2++, search_item_2 = strtok_r(NULL, "|",&search_item_data_2))
    {
      memset(data_read_2,0,sizeof(data_read_2));
      memcpy(data_read_2,search_item_2,strnlen(search_item_2,sizeof(data_read_2)));

      // load the data and check that each voting address is only a XCA address, and each payment address is not an integrated address
      if (count2 == 0)
      {
        memcpy(private_group.private_group_name[count],data_read_2,strnlen(data_read_2,sizeof(private_group.private_group_name[count])));
      }
      else if (count2 == 1)
      {
        if (strncmp(data_read_2,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0)
        {
          color_print("Voting address can only be XCA addresses","red");
          return 0;
        }
        memcpy(private_group.private_group_voting_public_address[count],data_read_2,XCASH_WALLET_LENGTH);
      }
      else
      {
        if (strncmp(data_read_2,"XCB",sizeof(XCASH_WALLET_PREFIX)-1) == 0)
        {
          color_print("Payment address can not be a XCB addresses","red");
          return 0;
        }
        memcpy(private_group.private_group_payment_public_address[count],data_read_2,XCASH_WALLET_LENGTH);
      }
    }
  }
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: payment_timer_thread
Description: Sends all of the delegates payments once a day at a random time, or a set time if using the --shared_delegate_payment_time_hour and --shared_delegate_payment_time_minute paramters
-----------------------------------------------------------------------------------------------------------
*/

void* payment_timer_thread(void* parameters)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[SMALL_BUFFER_SIZE];
  char data3[SMALL_BUFFER_SIZE];
  char tx_hash[TRANSACTION_HASH_LENGTH+1];
  char tx_key[TRANSACTION_HASH_LENGTH+1];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;
  int counter; 
  int private_group_count = 0;
  long long int number;
  long long int amount_of_payments;
  long long int total_amount;
  double total;
  struct database_multiple_documents_fields database_multiple_documents_fields;
  int document_count;
  char* transaction_list_data[MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE];
  char transaction_list_data_tx_hash[MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE][TRANSACTION_HASH_LENGTH+1];
  char transaction_list_data_tx_key[MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE][TRANSACTION_HASH_LENGTH+1];
  int transaction_list_data_count;
  int maximum_unspents_per_transaction = MAXIMUM_UNSPENTS_PER_TRANSACTION;

  // unused parameters
  (void)parameters;

  // define macros
  #define TRANSACTION_LIST_BUFFER 5000
  #define DATABASE_COLLECTION "public_addresses"
  #define PAYMENT_TIMER_THREAD_ERROR(message) \
  color_print(message,"red"); \
  RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,TOTAL_PUBLIC_ADDRESSES_DATABASE_FIELDS); \
  sleep(60); \
  goto start;

  // initialize the data
  for (count = 0; count < MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE; count++)
  {
    if ((transaction_list_data[count] = (char*)calloc(TRANSACTION_LIST_BUFFER,sizeof(char))) == NULL)
    {
      exit(0);
    } 
  }

  for (;;)
  {
    start:
    // check if it is time to send the payments
    get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);
    if (current_UTC_date_and_time.tm_min == (BLOCK_TIME-1))
    {
      // wait to make sure the checking of reserve proofs have stoped
      sleep(5);
      color_print("Sending the hourly payments","yellow");

      memset(data,0,sizeof(data));
      memset(data2,0,sizeof(data2));
      memset(data3,0,sizeof(data3));
      
      if ((document_count = count_all_documents_in_collection(shared_delegates_database_name,DATABASE_COLLECTION)) <= 0)
      {
        color_print("The database is empty so there are no payments to send","yellow");
        sleep(60);
        goto start;
      }

      // check if the wallet has above MINIMUM_AMOUNT_SHARED_DELEGATE_SEND_PAYMENT_AMOUNT to avoid errors
      if ((total_amount = get_total_amount()) < MINIMUM_AMOUNT_SHARED_DELEGATE_SEND_PAYMENT_AMOUNT)
      {
        memset(data,0,sizeof(data));
        memcpy(data,"The wallet is under the minimum amount (",40);
        snprintf(data+strlen(data),MAXIMUM_NUMBER_SIZE,"%ld",MINIMUM_AMOUNT_SHARED_DELEGATE_SEND_PAYMENT_AMOUNT / XCASH_WALLET_DECIMAL_PLACES_AMOUNT);
        memcpy(data+strlen(data),") to be able to send payments.\nPlease add at least ",51);
        snprintf(data+strlen(data),MAXIMUM_NUMBER_SIZE,"%lld",(MINIMUM_AMOUNT_SHARED_DELEGATE_SEND_PAYMENT_AMOUNT - total_amount) / XCASH_WALLET_DECIMAL_PLACES_AMOUNT);
        memcpy(data+strlen(data)," to the wallet, before voter payments can be sent out",53);
        color_print(data,"red");
        sleep(60);
        goto start;
      }

      // reset the amount of payments and total amount
      amount_of_payments = 0;
      total_amount = 0;

      // initialize the database_multiple_documents_fields struct 
      INITIALIZE_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,document_count,TOTAL_RESERVE_PROOFS_DATABASE_FIELDS,"payment_timer_thread",data,current_date_and_time,current_UTC_date_and_time);

      if (read_multiple_documents_all_fields_from_collection(shared_delegates_database_name,DATABASE_COLLECTION,"",&database_multiple_documents_fields,1,document_count,0,"") == 0)
      {
        PAYMENT_TIMER_THREAD_ERROR("Could not read the public addresses database.\nCould not send any payments to the delegates\nThe database is kept the same and no action is needed");
      }

      /* loop through each delegate and build each tx list
         start with the maximum unspents per transaction, and lessen by 1 if there is a failure to send any tx or it returns more than 1 tx hash
      */

      for (;;)
      {
        start2:
        // reset the transaction_list_data
        for (count = 0; count < MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE; count++)
        {
          memset(transaction_list_data[count],0,TRANSACTION_LIST_BUFFER);
          memcpy(transaction_list_data[count],"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"transfer_split\",\"params\":{\"destinations\":[",78);
        }

        for (count = 0, counter = 1, transaction_list_data_count = 0; count < (int)database_multiple_documents_fields.document_count; count++)
        {
          // check if the current_total is over the minimum amount
          sscanf(database_multiple_documents_fields.value[count][1], "%lld", &number);
          if (number >= (minimum_amount * XCASH_WALLET_DECIMAL_PLACES_AMOUNT))
          {           
            // check if the private group is on, and if so send it to the payment address instead of the voter address
            if (private_group.private_group_settings == 1)
            {
              for (private_group_count = 0; private_group_count < MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE_PRIVATE_GROUP; private_group_count++)
              {
                if (strncmp(database_multiple_documents_fields.value[count][0],private_group.private_group_voting_public_address[private_group_count],BUFFER_SIZE) == 0)
                {
                  break;
                }
              }
              if (private_group_count == MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE_PRIVATE_GROUP)
              {
                continue;
              }
            }            

            memcpy(transaction_list_data[transaction_list_data_count]+strlen(transaction_list_data[transaction_list_data_count]),"{\"amount\":",10);
            memcpy(transaction_list_data[transaction_list_data_count]+strlen(transaction_list_data[transaction_list_data_count]),database_multiple_documents_fields.value[count][1],strnlen(database_multiple_documents_fields.value[count][1],TRANSACTION_LIST_BUFFER));
            memcpy(transaction_list_data[transaction_list_data_count]+strlen(transaction_list_data[transaction_list_data_count]),",\"address\":\"",12);             
            private_group.private_group_settings == 1 ? memcpy(transaction_list_data[transaction_list_data_count]+strlen(transaction_list_data[transaction_list_data_count]),private_group.private_group_payment_public_address[private_group_count],XCASH_WALLET_LENGTH) : memcpy(transaction_list_data[transaction_list_data_count]+strlen(transaction_list_data[transaction_list_data_count]),database_multiple_documents_fields.value[count][0],XCASH_WALLET_LENGTH);          
            memcpy(transaction_list_data[transaction_list_data_count]+strlen(transaction_list_data[transaction_list_data_count]),"\"}",2);
            memcpy(transaction_list_data[transaction_list_data_count]+strlen(transaction_list_data[transaction_list_data_count]),",",1);
            counter++;            

            // check if we have the maximum unspents
            if (counter > maximum_unspents_per_transaction)
            {
              memcpy(transaction_list_data[transaction_list_data_count]+strlen(transaction_list_data[transaction_list_data_count])-1,"],\"priority\":0,\"ring_size\":21,\"get_tx_keys\":true}}",50);
              transaction_list_data_count++;
              counter = 1;
            }
          }
        }

        // get how many transaction list there are
        for (count = 0, transaction_list_data_count = 0; count < MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE; count++)
        {
          if (strstr(transaction_list_data[count],XCASH_WALLET_PREFIX) != NULL)
          {
            transaction_list_data_count++;
          }
        }    

        // make sure their is at least 1 transaction list
        if (transaction_list_data_count <= 0)
        {
          PAYMENT_TIMER_THREAD_ERROR("Their is no transaction list.\nno action is needed");
        }
        
        // check if we need to add the end string to the last transaction list
        if (strstr(transaction_list_data[transaction_list_data_count-1],"get_tx_keys") == NULL)
        {
          memcpy(transaction_list_data[transaction_list_data_count-1]+strlen(transaction_list_data[transaction_list_data_count-1])-1,"],\"priority\":0,\"ring_size\":21,\"get_tx_keys\":true}}",50);
        }

        // check each transaction list to make sure it is able to be sent
        for (count = 0; count < transaction_list_data_count; count++)
        {
          memset(tx_hash,0,sizeof(tx_hash));
          memset(tx_key,0,sizeof(tx_key));     

          if (send_payment(transaction_list_data[count],tx_hash,tx_key,0) == 0 || strlen(tx_hash) != TRANSACTION_HASH_LENGTH || strlen(tx_key) != TRANSACTION_HASH_LENGTH)
          {
            maximum_unspents_per_transaction--;
            if (maximum_unspents_per_transaction == 0)
            {
              PAYMENT_TIMER_THREAD_ERROR("Sending transaction error.\nCould not send any payments to the delegates\nThe database is kept the same and no action is needed");
            }
            goto start2;
          }
        }
        break;
      }

      // all of the transaction list should work, send the payments and copy what tx hash and tx key is for what transaction list
      for (count = 0; count < transaction_list_data_count; count++)
      {
        memset(transaction_list_data_tx_hash[count],0,sizeof(transaction_list_data_tx_hash[count]));
        memset(transaction_list_data_tx_key[count],0,sizeof(transaction_list_data_tx_key[count]));
        
        if (send_payment(transaction_list_data[count],transaction_list_data_tx_hash[count],transaction_list_data_tx_key[count],1) == 0 || strlen(transaction_list_data_tx_hash[count]) != TRANSACTION_HASH_LENGTH || strlen(transaction_list_data_tx_key[count]) != TRANSACTION_HASH_LENGTH)
        {
          color_print("Failed to send a transaction list\nManually send each amount to each public address in this transaction list listed below (or try to resend this transaction list manually), as the database has been adjusted!","red");
          color_print(transaction_list_data[count],"red");
        }
      }

      // all payments have been sent, update the databases
      for (count = 0; count < (int)database_multiple_documents_fields.document_count; count++)
      {
        // check if the current_total is over the minimum amount
        sscanf(database_multiple_documents_fields.value[count][1], "%lld", &number);
        if (number >= (minimum_amount * XCASH_WALLET_DECIMAL_PLACES_AMOUNT))
        {
           // check if the private group is on, and if so send it to the payment address instead of the voter address
            if (private_group.private_group_settings == 1)
            {
              for (private_group_count = 0; private_group_count < MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE_PRIVATE_GROUP; private_group_count++)
              {
                if (strncmp(database_multiple_documents_fields.value[count][0],private_group.private_group_voting_public_address[private_group_count],BUFFER_SIZE) == 0)
                {
                  break;
                }
              }
              if (private_group_count == MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE_PRIVATE_GROUP)
              {
                continue;
              }
            }      

          // update the database for the public address
          for (counter = 0; counter < transaction_list_data_count; counter++)
          {
            if ((private_group.private_group_settings == 0 && strstr(transaction_list_data[counter],database_multiple_documents_fields.value[count][0]) != NULL) || (private_group.private_group_settings == 1 && strstr(transaction_list_data[counter],private_group.private_group_payment_public_address[private_group_count]) != NULL))
            { 
              if (payment_timer_send_payment_and_update_databases(database_multiple_documents_fields.value[count][0],database_multiple_documents_fields.value[count][1],database_multiple_documents_fields.value[count][2],transaction_list_data_tx_hash[counter],transaction_list_data_tx_key[counter]) == 0)
              {
                color_print("Failed to update the database for a delegate\nManually update the following data (run the commands in the terminal), as the delegate has already been paid.\nFirst open the mongodb terminal by typing \"mongo\"\n Next use the shared delegates database by typing \"use XCASH_PROOF_OF_STAKE_DELEGATES\"\nUse the following command in the mongodb terminal to update values in the database:\ndb.collection.update({\"field_to_find_delegate\":\"value\"},{$set:{\"field_to_update_for_delegate\":\"value\"}}","red");
                memset(data,0,sizeof(data));
                memcpy(data,"#1 In the database collection public_addresses, substract ",58);
                memcpy(data+strlen(data),database_multiple_documents_fields.value[count][1],strlen(database_multiple_documents_fields.value[count][1]));
                memcpy(data+strlen(data)," from the current_total field, for delegate ",44);
                memcpy(data+strlen(data),database_multiple_documents_fields.value[count][0],strlen(database_multiple_documents_fields.value[count][0]));
                memcpy(data+strlen(data),"\n\n#2 In the database collection public_addresses, add ",54);
                memcpy(data+strlen(data),database_multiple_documents_fields.value[count][1],strlen(database_multiple_documents_fields.value[count][1]));
                memcpy(data+strlen(data)," to the total field, for delegate ",34);
                memcpy(data+strlen(data),database_multiple_documents_fields.value[count][0],strlen(database_multiple_documents_fields.value[count][0]));
                memcpy(data+strlen(data),"\n\n#3 Run the following command to add the document to the database collection\ndb.public_addresses_payments.insertOne({\"public_address\":\"",136);
                memcpy(data+strlen(data),database_multiple_documents_fields.value[count][0],strlen(database_multiple_documents_fields.value[count][0]));

                memcpy(data+strlen(data),"\",\"payment_name\":\"",18);
                if (private_group.private_group_settings == 1)
                {
                  memcpy(data+strlen(data),private_group.private_group_name[private_group_count],strnlen(private_group.private_group_name[private_group_count],sizeof(data)));
                }

                memcpy(data+strlen(data),"\",\"payment_address\":\"",21);
                private_group.private_group_settings == 1 ? memcpy(data+strlen(data),private_group.private_group_payment_public_address[private_group_count],strnlen(private_group.private_group_payment_public_address[private_group_count],sizeof(data))) : memcpy(data+strlen(data),database_multiple_documents_fields.value[count][0],strlen(database_multiple_documents_fields.value[count][0]));

                memcpy(data+strlen(data),"\",\"total\":\"",11);
                memcpy(data+strlen(data),database_multiple_documents_fields.value[count][1],strlen(database_multiple_documents_fields.value[count][1]));
                memcpy(data+strlen(data),"\",\"date_and_time\":\"",19);
                snprintf(data+strlen(data),MAXIMUM_NUMBER_SIZE,"%lld",(long long int)time(NULL));
                memcpy(data+strlen(data),"\",\"tx_hash\":\"",13);
                memcpy(data+strlen(data),transaction_list_data_tx_hash[counter],TRANSACTION_LENGTH);
                memcpy(data+strlen(data),"\",\"tx_key\":\"",12);
                memcpy(data+strlen(data),transaction_list_data_tx_key[counter],TRANSACTION_LENGTH);
                memcpy(data+strlen(data),"\"})",2);
                color_print(data,"yellow");           
              }
              else
              {
                amount_of_payments++;
                total_amount += number;
                break;
              }
            }
          }          
        }
        if (payment_timer_update_inactivity_count(database_multiple_documents_fields.value[count][0],database_multiple_documents_fields.value[count][1],database_multiple_documents_fields.value[count][3]) == 0)
        {
          PAYMENT_TIMER_THREAD_ERROR("Could not update the inactivity count for the shared delegates");
        }
      }

      // format the total amount
      total = (double)total_amount / XCASH_WALLET_DECIMAL_PLACES_AMOUNT;   

      RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,TOTAL_PUBLIC_ADDRESSES_DATABASE_FIELDS);
      memset(data,0,sizeof(data));  
      print_start_message(current_date_and_time,current_UTC_date_and_time,"Delegates Payments",data); 
      memset(data,0,sizeof(data));    
      memcpy(data,"Amount of delegates paid: ",26);
      snprintf(data+strlen(data),MAXIMUM_NUMBER_SIZE,"%lld",amount_of_payments);
      memcpy(data+strlen(data),"\nTotal amount: ",15);
      snprintf(data+strlen(data),sizeof(data)-1,"%lf",total);
      memcpy(data+strlen(data),"\nAmount of payments: ",21);
      snprintf(data+strlen(data),MAXIMUM_NUMBER_SIZE,"%d",transaction_list_data_count);
      memcpy(data+strlen(data),"\n",sizeof(char));
      color_print(data,"yellow");
      sleep(60);
    }
    nanosleep((const struct timespec[]){{0, 200000000L}}, NULL);
  }

  // reset the data
  for (count = 0; count < MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE; count++)
  {
    pointer_reset(transaction_list_data[count]);
  }

  pthread_exit((void *)(intptr_t)1);
  
  #undef TRANSACTION_LIST_BUFFER
  #undef DATABASE_COLLECTION
  #undef PAYMENT_TIMER_THREAD_ERROR
}
