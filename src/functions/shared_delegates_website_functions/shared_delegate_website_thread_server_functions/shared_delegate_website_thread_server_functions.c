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
Name: add_block_to_blocks_found
Description: Adds the block to the blocks_found database
Return: 0 if an error has occured, else the block reward
-----------------------------------------------------------------------------------------------------------
*/

long long int add_block_to_blocks_found(void)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char block_height[BUFFER_SIZE];
  char block_hash[BUFFER_SIZE];  
  char block_reward[BUFFER_SIZE];
  char block_date_and_time[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;
  int count2;
  long long int number;
  long long int block_height_number = 0;
  long long int block_reward_number = 0;
  struct database_multiple_documents_fields database_multiple_documents_fields;
  int document_count = 0;

  // define macros
  #define DATABASE_COLLECTION "blocks_found"

  #define ADD_BLOCK_TO_BLOCKS_FOUND_ERROR(message,settings) \
  memcpy(error_message.function[error_message.total],"add_block_to_blocks_found",25); \
  memcpy(error_message.data[error_message.total],message,sizeof(message)-1); \
  error_message.total++; \
  if ((settings) == 0) \
  { \
    POINTER_RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,count2,TOTAL_BLOCKS_FOUND_DATABASE_FIELDS); \
  } \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(block_height,0,sizeof(block_height));
  memset(block_hash,0,sizeof(block_hash));
  memset(block_reward,0,sizeof(block_reward));
  memset(block_date_and_time,0,sizeof(block_date_and_time));

  document_count = count_all_documents_in_collection(DATABASE_NAME_DELEGATES,DATABASE_COLLECTION,1);

  if (document_count <= 0)
  {
    ADD_BLOCK_TO_BLOCKS_FOUND_ERROR("The database is empty",1);
  }

  // initialize the database_data struct 
  INITIALIZE_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,count2,document_count,TOTAL_BLOCKS_FOUND_DATABASE_FIELDS,"add_block_to_blocks_found",data,current_date_and_time,current_UTC_date_and_time);

  // get the block information of the previous block
  if (get_previous_block_information(block_hash,block_reward,block_date_and_time) == 0)
  {
    ADD_BLOCK_TO_BLOCKS_FOUND_ERROR("Could not get the previous block information.\nCould not check if the block verifier found the last block.",0);
  }

  // convert the block_reward to a number
  sscanf(block_reward, "%lld", &block_reward_number);

  // get the block height of the previous block
  sscanf(current_block_height, "%lld", &number);
  number--;
  sprintf(block_height,"%lld",number);

  // get the lat block height of the last block found to calculate the average
  if (document_count > 0)
  {
    // get the block count
    if (read_multiple_documents_all_fields_from_collection(DATABASE_NAME_DELEGATES,DATABASE_COLLECTION,"",&database_multiple_documents_fields,1,document_count,1,"total_vote_count_number",1) == 0)
    {
      ADD_BLOCK_TO_BLOCKS_FOUND_ERROR("Could not read the blocks_found database.\nCould not check if the block verifier found the last block.",0);
    }

    // loop through all blocks found, and find the last block height
    for (count = 0; count < document_count; count++)
    {
      for (count2 = 0; count2 < TOTAL_BLOCKS_FOUND_DATABASE_FIELDS; count2++)
      {
        if (memcmp(database_multiple_documents_fields.item[count][count2],"block_height",12) == 0)
        {
          sscanf(database_multiple_documents_fields.value[count][count2], "%lld", &number);
          if (number > block_height_number)
          {
            block_height_number = number;
          }
        }
      }
    }
    // get the block height of the previous block
    sscanf(current_block_height, "%lld", &number);
    number--;
    number -= block_height_number;
    sprintf(data2,"%lld",number);
  } 

  // add the blocks data
  memset(data,0,sizeof(data));
  memcpy(data,"{\"block_height\":\"",17);
  memcpy(data+strlen(data),block_height,strnlen(block_height,sizeof(data)));
  memcpy(data+strlen(data),"\",\"block_hash\":\"",16);
  memcpy(data+strlen(data),block_hash,BLOCK_HASH_LENGTH);
  memcpy(data+strlen(data),"\",\"block_date_and_time\":\"",25);
  memcpy(data+strlen(data),block_date_and_time,strnlen(block_date_and_time,sizeof(data)));
  memcpy(data+strlen(data),"\",\"block_reward\":\"",18);
  memcpy(data+strlen(data),block_reward,strnlen(block_reward,sizeof(data)));
  memcpy(data+strlen(data),"\",\"block_count\":\"",17);
  memcpy(data+strlen(data),data2,strnlen(data2,sizeof(data)));
  memcpy(data+strlen(data),"\"}",2);

  if (insert_document_into_collection_json(DATABASE_NAME_DELEGATES,DATABASE_COLLECTION,data,1) == 0)
  {
    ADD_BLOCK_TO_BLOCKS_FOUND_ERROR("Could not add the block to the blocks_found database.\nCould not check if the block verifier found the last block.",0);
  }
  POINTER_RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,count2,TOTAL_BLOCKS_FOUND_DATABASE_FIELDS);
  return block_reward_number;

  #undef DATABASE_COLLECTION
  #undef ADD_BLOCK_TO_BLOCKS_FOUND_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_delegates_total_voters
Description: Gets the delegates total voters
Parameters:
  block_reward - The block reward
Return: 0 if an error has occured, otherwise the total votes from all of the voters for the delegate
-----------------------------------------------------------------------------------------------------------
*/

int get_delegates_total_voters(struct voters* voters)
{
  // Variables
  char data[1024];
  char data2[1024];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;
  int count2;
  int counter;
  int total_votes;
  long long int number;
  struct database_multiple_documents_fields database_multiple_documents_fields;

  #define GET_DELEGATES_TOTAL_VOTERS_ERROR(message) \
  memcpy(error_message.function[error_message.total],"get_delegates_total_voters",26); \
  memcpy(error_message.data[error_message.total],message,sizeof(message)-1); \
  error_message.total++; \
  RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,count2,TOTAL_RESERVE_PROOFS_DATABASE_FIELDS); \
  return 0;

  // initialize the database_multiple_documents_fields struct 
  INITIALIZE_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,count2,MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE,TOTAL_RESERVE_PROOFS_DATABASE_FIELDS,"get_delegates_total_voters",data,current_date_and_time,current_UTC_date_and_time);

  memset(data,0,sizeof(data));
  memcpy(data,"{\"public_address_voted_for\":\"",29);
  memcpy(data+29,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(data+127,"\"}",2);

  // get the count of how many public addresses voted for the delegate
  for (total_votes = 0, counter = 1; counter <= TOTAL_RESERVE_PROOFS_DATABASES; counter++)
  { 
    memset(data2,0,strlen(data2));
    memcpy(data2,"reserve_proofs_",15);
    snprintf(data2+15,sizeof(data2)-16,"%d",counter);
    count2 = count_documents_in_collection(DATABASE_NAME,data2,data,1);

    if (count2 > 0)
    {
      if (read_multiple_documents_all_fields_from_collection(DATABASE_NAME,data2,data,&database_multiple_documents_fields,1,count2,0,"",1) == 0)
      {
        GET_DELEGATES_TOTAL_VOTERS_ERROR("Could not read the reserve proofs database.\nCould not calculate the block reward for each delegate");
      }

      // copy the data to the voters struct
      for (count = 0; count < count2; count++)
      {
        memcpy(voters[count].public_address,database_multiple_documents_fields.value[count][0],XCASH_WALLET_LENGTH);
        memcpy(voters[count].total_vote_count,database_multiple_documents_fields.value[count][2],strnlen(database_multiple_documents_fields.value[count][2],100));
        sscanf(voters[count].total_vote_count, "%lld", &number);
        voters[count].total_votes = number;
        total_votes += number;
      }

      // reset the database_multiple_documents_fields struct
      for (count = 0; count < MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE; count++)
      {
        for (count2 = 0; count2 < TOTAL_RESERVE_PROOFS_DATABASE_FIELDS; count2++)
        {
          memset(database_multiple_documents_fields.item[count][count2],0,strlen(database_multiple_documents_fields.item[count][count2]));
          memset(database_multiple_documents_fields.value[count][count2],0,strlen(database_multiple_documents_fields.item[count][count2]));
        }
      }
      database_multiple_documents_fields.document_count = 0;
      database_multiple_documents_fields.database_fields_count = 0;
    }
  }
  RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,count2,TOTAL_RESERVE_PROOFS_DATABASE_FIELDS);
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
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;
  long long int number;
  long long int total_votes;
  int total_voters;
  long long int current_delegates_block_reward;
  struct voters voters[MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE];

  // define macros
  #define CALCULATE_BLOCK_REWARD_FOR_EACH_DELEGATE_ERROR(message) \
  memcpy(error_message.function[error_message.total],"calculate_block_reward_for_each_delegate",40); \
  memcpy(error_message.data[error_message.total],message,sizeof(message)-1); \
  error_message.total++; \
  POINTER_RESET_VOTERS_STRUCT(count,MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE); \
  return 0;

  // initialize the delegates struct
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
  total_voters = get_delegates_total_voters_count(xcash_wallet_public_address);
  if (total_voters < 1)
  {
    CALCULATE_BLOCK_REWARD_FOR_EACH_DELEGATE_ERROR("The delegate does not have enough voters to distribute the block rewards with yet");
  }
  
  // get the total votes for the delegate
  total_votes = get_delegates_total_voters(voters);

  // calculate the block reward for all of the voters
  for (count = 0; count < total_voters; count++)
  {
    current_delegates_block_reward = (long long int)((((double)voters[count].total_votes / (double)total_votes) * (double)block_reward) * ((100 - fee)/100)) | 0;
    
    // create the message
    memset(data2,0,sizeof(data2));
    memcpy(data2,"{\"public_address\":\"",19);
    memcpy(data2+19,voters[count].public_address,XCASH_WALLET_LENGTH);
    memcpy(data2+117,"\"}",2);
    memset(data,0,sizeof(data));

    // check if the public address is in the database, if not add it
    if (count_documents_in_collection(DATABASE_NAME_DELEGATES,"public_addresses",data2,0) <= 0)
    {
      memset(data3,0,sizeof(data3));
      memcpy(data3,"{\"public_address\":\"",19);
      memcpy(data3+19,voters[count].public_address,XCASH_WALLET_LENGTH);
      memcpy(data3+strlen(data3),"\",\"current_total\":\"0\",\"total\":\"0\",\"inactivity_count\":\"0\"}",57);

      if (insert_document_into_collection_json(DATABASE_NAME_DELEGATES,"public_addresses",data3,1) == 0)
      {
        CALCULATE_BLOCK_REWARD_FOR_EACH_DELEGATE_ERROR("Could not calculate block reward for all of the public address that voted for the delegate.");
      }
    }
    
    if (read_document_field_from_collection(DATABASE_NAME_DELEGATES,"public_addresses",data2,"current_total",data,1) == 0)
    {
      CALCULATE_BLOCK_REWARD_FOR_EACH_DELEGATE_ERROR("Could not calculate block reward for all of the public address that voted for the delegate.");
    }
    
    sscanf(data, "%lld", &number);
    number += current_delegates_block_reward;
    memset(data,0,sizeof(data));
    memset(data3,0,sizeof(data3));
    sprintf(data3,"%lld",number);
    memcpy(data,"{\"current_total\":\"",18);
    memcpy(data+18,data3,strnlen(data3,sizeof(data)));
    memcpy(data+strlen(data),"\"}",2);
    
    if (update_document_from_collection(DATABASE_NAME_DELEGATES,"public_addresses",data2,data,1) == 0)
    {
      CALCULATE_BLOCK_REWARD_FOR_EACH_DELEGATE_ERROR("Could not calculate block reward for all of the public address that voted for the delegate.");
    } 
  }
  POINTER_RESET_VOTERS_STRUCT(count,MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE);
  return 1;

  #undef CALCULATE_BLOCK_REWARD_FOR_EACH_DELEGATE_ERROR
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
  continue;

  for (;;)
  {   
    get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);
    if (current_UTC_date_and_time.tm_min % BLOCK_TIME == 0 && current_UTC_date_and_time.tm_sec == 10)
    {
      // check if you found the previous block in the network
      if (check_found_block() == 2)
      {
        block_reward_number = add_block_to_blocks_found();
        if (block_reward_number == 0)
        {
          BLOCK_HEIGHT_TIMER_THREAD_ERROR("Could not add the previous block that the block verifier found to the database");
        }

        if (calculate_block_reward_for_each_delegate(block_reward_number) == 0)
        {
          BLOCK_HEIGHT_TIMER_THREAD_ERROR("Could not calculate the block reward for each delegate");
        }
      }
    }
    usleep(200000);
  }
  pthread_exit((void *)(intptr_t)1);

  #undef BLOCK_HEIGHT_TIMER_THREAD_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: payment_timer_thread
Description: Sends all of the delegates payments at UTC 00:00
-----------------------------------------------------------------------------------------------------------
*/

void* payment_timer_thread(void* parameters)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  char public_address[XCASH_WALLET_LENGTH+1];
  char current_total[BUFFER_SIZE];
  char total[BUFFER_SIZE];
  char inactivity_count[BUFFER_SIZE];
  char payment_tx_hash[BUFFER_SIZE];
  char payment_tx_key[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  size_t count;
  size_t counter; 
  long long int number;
  long long int amount_of_payments;
  long long int updated_total;
  long long int total_amount;
  struct database_multiple_documents_fields database_multiple_documents_fields;
  int document_count;

  // unused parameters
  (void)parameters;

  // define macros
  #define PAYMENT_TIMER_THREAD_ERROR(message,settings) \
  memcpy(error_message.function[error_message.total],"payment_timer_thread",20); \
  memcpy(error_message.data[error_message.total],message,sizeof(message)-1); \
  error_message.total++; \
  if ((settings) == 0) \
  { \
    RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,TOTAL_PUBLIC_ADDRESSES_DATABASE_FIELDS); \
  } \
  continue;

  for (;;)
  {
    // check if it is UTC 00:00
    get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);
    if (current_UTC_date_and_time.tm_hour == 0 && current_UTC_date_and_time.tm_min == 0)
    {
      color_print("It is UTC 00:00\nSending the daily payments","green");

      memset(data,0,sizeof(data));
      memset(data2,0,sizeof(data2));
      memset(data3,0,sizeof(data3));

      // reset the amount of payments and total amount
      amount_of_payments = 0;
      total_amount = 0;

      document_count = count_all_documents_in_collection(DATABASE_NAME_DELEGATES,"public_addresses",1);
      if (document_count <= 0)
      {
        PAYMENT_TIMER_THREAD_ERROR("The database is empty",1);
      }

      // initialize the database_multiple_documents_fields struct 
      INITIALIZE_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,document_count,TOTAL_RESERVE_PROOFS_DATABASE_FIELDS,"payment_timer_thread",data,current_date_and_time,current_UTC_date_and_time);

      if (read_multiple_documents_all_fields_from_collection(DATABASE_NAME_DELEGATES,"public_addresses","",&database_multiple_documents_fields,1,document_count,0,"",1) == 0)
      {
        PAYMENT_TIMER_THREAD_ERROR("Could not read the public addresses database.\nCould not send any payments to the delegates",0);
      }

      // loop through each delegate
      for (count = 0; count < database_multiple_documents_fields.document_count; count++)
      {
        for (counter = 0; counter < TOTAL_RESERVE_PROOFS_DATABASE_FIELDS; counter++)
        {
          if (memcmp(database_multiple_documents_fields.item[count][counter],"public_address",14) == 0)
          {
            memset(public_address,0,sizeof(public_address));
            memcpy(public_address,database_multiple_documents_fields.value[count][counter],XCASH_WALLET_LENGTH);
          }
          else if (memcmp(database_multiple_documents_fields.item[count][counter],"current_total",13) == 0)
          {
            memset(current_total,0,sizeof(current_total));
            memcpy(current_total,database_multiple_documents_fields.value[count][counter],strnlen(database_multiple_documents_fields.value[count][counter],sizeof(current_total)));
          }
          else if (memcmp(database_multiple_documents_fields.item[count][counter],"total",5) == 0)
          {
            memset(total,0,sizeof(total));
            memcpy(total,database_multiple_documents_fields.value[count][counter],strnlen(database_multiple_documents_fields.value[count][counter],sizeof(total)));
          }
          else if (memcmp(database_multiple_documents_fields.item[count][counter],"inactivity_count",16) == 0)
          {
            memset(inactivity_count,0,sizeof(inactivity_count));
            memcpy(inactivity_count,database_multiple_documents_fields.value[count][counter],strnlen(database_multiple_documents_fields.value[count][counter],sizeof(inactivity_count)));
          }
        }

        // check if the current_total is over the minimum amount
        sscanf(current_total, "%lld", &number);
        if (number >= (minimum_amount * XCASH_WALLET_DECIMAL_PLACES_AMOUNT))
        {
          // send the payment
          memset(data,0,sizeof(data));
          memset(data2,0,sizeof(data2));
          memset(data3,0,sizeof(data3));
          memset(payment_tx_hash,0,sizeof(payment_tx_hash));
          memset(payment_tx_key,0,sizeof(payment_tx_key));
          
          if (send_payment(public_address, current_total, payment_tx_hash, payment_tx_key) == 0)
          {
            PAYMENT_TIMER_THREAD_ERROR("Could not send a payment.\nCould not send payments.",0);
          }

          // create the message
          memcpy(data,"{\"public_address\":\"",19);
          memcpy(data+19,public_address,XCASH_WALLET_LENGTH);
          memcpy(data+strlen(data),"\"}",2);
          
          // reset the current total in the public_addresses collection
          memcpy(data2,"{\"current_total\":\"0\"}",21);
          if (update_document_from_collection(DATABASE_NAME_DELEGATES,"public_addresses",data,data2,1) == 0)
          {
            PAYMENT_TIMER_THREAD_ERROR("Could not reset the current_total for a payment in the database.\nCould not send payments.",0);
          } 

          // add the current total to the total in the public_addresses collection
          sscanf(total, "%lld", &updated_total);
          updated_total += number;
          sprintf(data3,"%lld",updated_total);
          memset(data2,0,sizeof(data2));
          memcpy(data2,"{\"total\":\"",10);
          memcpy(data2+10,data3,strnlen(data3,sizeof(data2)));
          memcpy(data2+strlen(data2),"\"}",2);

          if (update_document_from_collection(DATABASE_NAME_DELEGATES,"public_addresses",data,data2,1) == 0)
          {
            PAYMENT_TIMER_THREAD_ERROR("Could not add the current_total to the total for a payment in the database.\nCould not send payments.",0);
          } 

          // get the current date and time
          memset(data3,0,sizeof(data3));
          sprintf(data3,"%lld",(long long int)time(NULL));

          // add the payment to the public_addresses_payments in the collection
          memset(data2,0,sizeof(data2));
          memcpy(data2,"{\"public_address\":\"",19);
          memcpy(data2+19,public_address,XCASH_WALLET_LENGTH);
          memcpy(data2+strlen(data2),"\",\"total\":\"",11);
          memcpy(data2+strlen(data2),current_total,strnlen(current_total,sizeof(data2)));
          memcpy(data2+strlen(data2),"\",\"date_and_time\":\"",19);
          memcpy(data2+strlen(data2),data3,strnlen(data3,sizeof(data2)));
          memcpy(data2+strlen(data2),"\",\"tx_hash\":\"",13);
          memcpy(data2+strlen(data2),payment_tx_hash,strnlen(payment_tx_hash,sizeof(data2)));
          memcpy(data2+strlen(data2),"\",\"tx_key\":\"",12);
          memcpy(data2+strlen(data2),payment_tx_key,strnlen(payment_tx_key,sizeof(data2)));
          memcpy(data2+strlen(data2),"\"}",2);

          if (insert_document_into_collection_json(DATABASE_NAME_DELEGATES,"public_addresses_payments",data2,1) == 0)
          {
            PAYMENT_TIMER_THREAD_ERROR("Could not add the payment to the database.\nCould not send payments.",0);
          }
          amount_of_payments++;
          total_amount += number; 
        }

        /*
        set the inactivity_count
        if the address has a 0 current_total before sending any payments the inactivity_count ++
        if any address has not a 0 current_total and has an inactivity_count > 0 then reset it to 0
        if any address has an inactivity_count of 30 and a current_total of 0 before sending a payment, remove this document from the collection
        */

        memset(data2,0,sizeof(data2));
        memset(data3,0,sizeof(data3));
                 
        if (read_document_field_from_collection(DATABASE_NAME_DELEGATES,"public_addresses",data,"inactivity_count",data3,1) == 0)
        {
          PAYMENT_TIMER_THREAD_ERROR("Could not read the inactivity_count from the database.\nCould not send payments.",0);
        }

        if (strncmp(current_total,"0",BUFFER_SIZE) == 0 && strncmp(data3,VOTER_INACTIVITY_COUNT,BUFFER_SIZE) != 0)
        {
          // convert the total_xcash to a number
          sscanf(data3, "%lld", &number);
          number++;

          // update the amount_of_payments in the database
          memset(data3,0,sizeof(data3));
          sprintf(data3,"%lld",number);
          memcpy(data2,"{\"inactivity_count\":\"",21);
          memcpy(data2+21,data3,strnlen(data3,sizeof(data2)));
          memcpy(data2+strlen(data2),"\"}",2);

          if (update_document_from_collection(DATABASE_NAME_DELEGATES,"public_addresses",data,data2,1) == 0)
          {
            PAYMENT_TIMER_THREAD_ERROR("Could not update the inactivity_count for the database.\nCould not send payments.",0);
          }
        }
        else if (strncmp(current_total,"0",BUFFER_SIZE) != 0)
        {
          // set the inactivity_count to 0
          memcpy(data2,"{\"inactivity_count\":\"0\"}",24);
          if (update_document_from_collection(DATABASE_NAME_DELEGATES,"public_addresses",data,data2,1) == 0)
          {
            PAYMENT_TIMER_THREAD_ERROR("Could not update the inactivity_count for the database.\nCould not send payments.",0);
          }
        }
        else if (strncmp(current_total,"0",BUFFER_SIZE) == 0 && strncmp(data3,VOTER_INACTIVITY_COUNT,BUFFER_SIZE) == 0)
        {
          // remove the document from the database
          if (delete_document_from_collection(DATABASE_NAME,"public_addresses",data,1) == 0)
          {
            PAYMENT_TIMER_THREAD_ERROR("Could not remove a document from the database.\nCould not send payments.",0);
          }                   
        }
      }
      memset(data,0,sizeof(data));  
      print_start_message(current_date_and_time,current_UTC_date_and_time,"Delegates payments",data);      
      memcpy(data,"Amount of payments: ",20);
      snprintf(data+strlen(data),sizeof(data)-1,"%lld",amount_of_payments);
      memcpy(data,"\nTotal amount: ",15);
      snprintf(data+strlen(data),sizeof(data)-1,"%lld",total_amount);
      memcpy(data,"\n",1);
      color_print(data,"green");
    }
    sleep(10);
  }
  RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,counter,TOTAL_PUBLIC_ADDRESSES_DATABASE_FIELDS);
  pthread_exit((void *)(intptr_t)1);
  
  #undef BLOCK_HEIGHT_TIMER_THREAD_ERROR
}