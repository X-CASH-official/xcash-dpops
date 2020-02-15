#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"
#include "define_macros_test.h"

#include "block_verifiers_functions.h"
#include "blockchain_functions.h"
#include "insert_database_functions.h"
#include "read_database_functions.h"
#include "delete_database_functions.h"
#include "count_database_functions.h"
#include "block_verifiers_thread_server_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "network_daemon_functions.h"
#include "network_wallet_functions.h"
#include "VRF_functions.h"

#include "variables_test.h"
#include "optimizations_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: vrf_data_verify_timer
Description: Runs the VRF_data_verify function using multiple threads
-----------------------------------------------------------------------------------------------------------
*/

void* vrf_data_verify_timer(void* parameters)
{
  struct verify_network_block_data_vrf_data_verify_thread_parameters* verify_network_block_data_vrf_data_verify_thread_parameters = (struct verify_network_block_data_vrf_data_verify_thread_parameters*)parameters;
  int count;
  int count2;
  int total_block_verifiers_thread = (verify_network_block_data_vrf_data_verify_thread_parameters->block_verifier_total / verify_network_block_data_vrf_data_verify_thread_parameters->total_amount_of_threads) * (verify_network_block_data_vrf_data_verify_thread_parameters->start + 1);
  int total_block_verifiers = verify_network_block_data_vrf_data_verify_thread_parameters->block_verifier_total;
  for (count = (verify_network_block_data_vrf_data_verify_thread_parameters->block_verifier_total / verify_network_block_data_vrf_data_verify_thread_parameters->total_amount_of_threads) * verify_network_block_data_vrf_data_verify_thread_parameters->start; count < total_block_verifiers_thread; count++)
  { 
    for (count2 = 0; (int)count2 < total_block_verifiers; count2++)
    {
      VRF_data_verify(NEXT_BLOCK_VERIFIERS_PUBLIC_KEY,BLOCK_VALIDATION_NODE_SIGNATURE,NETWORK_BLOCK);
    }
  }
  pthread_exit((void *)(intptr_t)1);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: optimizations_functions_test
Description: Test the optimizations functions
Return: The number of passed optimizations_functions test
-----------------------------------------------------------------------------------------------------------
*/

int optimizations_functions_test(void)
{
  // Variables
  struct reserve_proof reserve_proof;
  struct send_and_receive_data_socket_thread_parameters send_and_receive_data_socket_thread_parameters[BLOCK_VERIFIERS_TOTAL_AMOUNT];
  struct verify_network_block_data_vrf_data_verify_thread_parameters verify_network_block_data_vrf_data_verify_thread_parameters[5];
  time_t start;
  int count;
  long int total;
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;

  // threads
  pthread_t thread_id[BLOCK_VERIFIERS_TOTAL_AMOUNT];  
  
  // define macros
  #define MESSAGE "{\r\n \"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\",\r\n}"
  #define VALIDATE_RESERVE_PROOFS_WALLET "XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP"
  #define VALIDATE_RESERVE_PROOFS_RESERVE_PROOF "ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti"
  #define MAXIMUM_TIME_BLOCK_VERIFIERS_SEND_DATA_SOCKET START_TIME_NETWORK_BLOCK_PART_2
  #define MAXIMUM_TIME_SEND_AND_RECEIVE_DATA_SOCKET_THREAD START_TIME_NETWORK_BLOCK_PART_2
  #define MINIMUM_VALIDATED_RESERVE_PROOF_AMOUNT MAXIMUM_INVALID_RESERVE_PROOFS / BLOCK_VERIFIERS_TOTAL_AMOUNT
  #define MAXIMUM_TIME_VALIDATED_RESERVE_PROOFS 265
  #define MAXIMUM_TIME_RESERVE_PROOF_UPDATE_DATABASE 10
  #define MAXIMUM_TIME_VRF_DATA_VERIFY SEND_DATA_TIME_NETWORK_BLOCK_PART_4 - START_TIME_NETWORK_BLOCK_PART_4
  #define MAXIMUM_TIME_VRF_DATA_VERIFY_MULTITHREADED SEND_DATA_TIME_NETWORK_BLOCK_PART_4 - START_TIME_NETWORK_BLOCK_PART_4
  #define MAXIMUM_TIME_NETWORK_BLOCK_PART_1 START_TIME_NETWORK_BLOCK_PART_2
  #define MAXIMUM_TIME_NETWORK_BLOCK_PART_2 START_TIME_NETWORK_BLOCK_PART_3 - START_TIME_NETWORK_BLOCK_PART_2
  #define MAXIMUM_TIME_NETWORK_BLOCK_PART_3 START_TIME_NETWORK_BLOCK_PART_4 - START_TIME_NETWORK_BLOCK_PART_3
  #define MAXIMUM_TIME_NETWORK_BLOCK_PART_4 SEND_DATA_TIME_NETWORK_BLOCK_PART_4 - START_TIME_NETWORK_BLOCK_PART_4
  #define MAXIMUM_TIME_NETWORK_BLOCK_PART_5 START_TIME_NETWORK_BLOCK_PART_5 - SEND_DATA_TIME_NETWORK_BLOCK_PART_4

  #define RESET_INVALID_RESERVE_PROOFS_DATA \
  for (count = 0; count < invalid_reserve_proofs.count; count++) \
  { \
    pointer_reset(invalid_reserve_proofs.block_verifier_public_address[count]); \
    pointer_reset(invalid_reserve_proofs.public_address_created_reserve_proof[count]); \
    pointer_reset(invalid_reserve_proofs.public_address_voted_for[count]); \
    pointer_reset(invalid_reserve_proofs.reserve_proof[count]); \
  } \
  invalid_reserve_proofs.count = 0;

  #define RESET_NETWORK_BLOCK_ROUND \
  memset(VRF_data.vrf_secret_key_data_round_part_4,0,strlen(VRF_data.vrf_secret_key_data_round_part_4)); \
  memset(VRF_data.vrf_secret_key_round_part_4,0,strlen((const char*)VRF_data.vrf_secret_key_round_part_4)); \
  memset(VRF_data.vrf_public_key_data_round_part_4,0,strlen(VRF_data.vrf_public_key_data_round_part_4)); \
  memset(VRF_data.vrf_public_key_round_part_4,0,strlen((const char*)VRF_data.vrf_public_key_round_part_4)); \
  memset(VRF_data.vrf_alpha_string_data_round_part_4,0,strlen(VRF_data.vrf_alpha_string_data_round_part_4)); \
  memset(VRF_data.vrf_alpha_string_round_part_4,0,strlen((const char*)VRF_data.vrf_alpha_string_round_part_4)); \
  memset(VRF_data.vrf_proof_data_round_part_4,0,strlen(VRF_data.vrf_proof_data_round_part_4)); \
  memset(VRF_data.vrf_proof_round_part_4,0,strlen((const char*)VRF_data.vrf_proof_round_part_4)); \
  memset(VRF_data.vrf_beta_string_data_round_part_4,0,strlen(VRF_data.vrf_beta_string_data_round_part_4)); \
  memset(VRF_data.vrf_beta_string_round_part_4,0,strlen((const char*)VRF_data.vrf_beta_string_round_part_4)); \
  memset(VRF_data.block_blob,0,strlen(VRF_data.block_blob)); \
  memset(VRF_data.reserve_bytes_data_hash,0,strlen(VRF_data.reserve_bytes_data_hash)); \
  memset(current_round_part_vote_data.current_vote_results,0,sizeof(current_round_part_vote_data.current_vote_results)); \
  for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++) \
  { \
    memset(VRF_data.block_verifiers_vrf_secret_key_data[count],0,strlen(VRF_data.block_verifiers_vrf_secret_key_data[count])); \
    memset(VRF_data.block_verifiers_vrf_secret_key[count],0,strlen((const char*)VRF_data.block_verifiers_vrf_secret_key[count])); \
    memset(VRF_data.block_verifiers_vrf_public_key_data[count],0,strlen(VRF_data.block_verifiers_vrf_public_key_data[count])); \
    memset(VRF_data.block_verifiers_vrf_public_key[count],0,strlen((const char*)VRF_data.block_verifiers_vrf_public_key[count])); \
    memset(VRF_data.block_verifiers_random_data[count],0,strlen(VRF_data.block_verifiers_random_data[count])); \
    memset(VRF_data.block_blob_signature[count],0,strlen(VRF_data.block_blob_signature[count])); \
  } \
  
  // reset the variables
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  count_test = 0;
  network_functions_test_error_settings = 2;
  delete_database(database_name,0);
  RESET_ERROR_MESSAGES;

  // initialize the send_and_receive_data_socket_thread_parameters struct
  for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++)
  {
    send_and_receive_data_socket_thread_parameters[count].DATA = (char*)calloc(BUFFER_SIZE,sizeof(char));

    // check if the memory needed was allocated on the heap successfully
    if (send_and_receive_data_socket_thread_parameters[count].DATA == NULL)
    {
      memcpy(error_message.function[error_message.total],"data_network_node_create_block",30);
      memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
      error_message.total++;
      print_error_message(current_date_and_time,current_UTC_date_and_time,data_test);  
      exit(0);
    }
  }
 
  // write the start test message
  fprintf(stderr,"\033[1;34m%s\noptimizations functions test - Total test: %d\n%s\n\n\033[0m",TEST_OUTLINE,OPTIMIZATIONS_TOTAL_TEST,TEST_OUTLINE);
 
  // run the test
  
  // test sending 100 messages to the block verifiers
  delete_database(database_name,0);
  INITIALIZE_DATABASE_DATA(2);
  RESET_INVALID_RESERVE_PROOFS_DATA;
  // initalize the previous current and next block verifiers list
  for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++)
  {
    memset(previous_block_verifiers_list.block_verifiers_name[count],0,sizeof(previous_block_verifiers_list.block_verifiers_name[count]));
    memset(previous_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_public_address[count]));
    memset(previous_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(previous_block_verifiers_list.block_verifiers_public_key[count]));
    memset(previous_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_IP_address[count]));
    memset(current_block_verifiers_list.block_verifiers_name[count],0,sizeof(current_block_verifiers_list.block_verifiers_name[count]));
    memset(current_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_public_address[count]));
    memset(current_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(current_block_verifiers_list.block_verifiers_public_key[count]));
    memset(current_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_IP_address[count]));
    memset(next_block_verifiers_list.block_verifiers_name[count],0,sizeof(next_block_verifiers_list.block_verifiers_name[count]));
    memset(next_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_public_address[count]));
    memset(next_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(next_block_verifiers_list.block_verifiers_public_key[count]));
    memset(next_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_IP_address[count]));

    memcpy(previous_block_verifiers_list.block_verifiers_name[count],"delegate_1",10);
    memcpy(previous_block_verifiers_list.block_verifiers_public_address[count],TEST_WALLET,XCASH_WALLET_LENGTH);
    memcpy(previous_block_verifiers_list.block_verifiers_IP_address[count],"127.0.0.1",9);
    memcpy(previous_block_verifiers_list.block_verifiers_public_key[count],NEXT_BLOCK_VERIFIERS_PUBLIC_KEY,VRF_PUBLIC_KEY_LENGTH);
    memcpy(current_block_verifiers_list.block_verifiers_name[count],"delegate_1",10);
    memcpy(current_block_verifiers_list.block_verifiers_public_address[count],TEST_WALLET,XCASH_WALLET_LENGTH);
    memcpy(current_block_verifiers_list.block_verifiers_IP_address[count],"127.0.0.1",9);
    memcpy(current_block_verifiers_list.block_verifiers_public_key[count],NEXT_BLOCK_VERIFIERS_PUBLIC_KEY,VRF_PUBLIC_KEY_LENGTH);
    memcpy(next_block_verifiers_list.block_verifiers_name[count],"delegate_1",10);
    memcpy(next_block_verifiers_list.block_verifiers_public_address[count],TEST_WALLET,XCASH_WALLET_LENGTH);
    memcpy(next_block_verifiers_list.block_verifiers_IP_address[count],"127.0.0.1",9);
    memcpy(next_block_verifiers_list.block_verifiers_public_key[count],NEXT_BLOCK_VERIFIERS_PUBLIC_KEY,VRF_PUBLIC_KEY_LENGTH);
  }
  
  // initialize the reserve_proof struct
  memset(reserve_proof.block_verifier_public_address,0,sizeof(reserve_proof.block_verifier_public_address));
  memset(reserve_proof.public_address_created_reserve_proof,0,sizeof(reserve_proof.public_address_created_reserve_proof));
  memset(reserve_proof.public_address_voted_for,0,sizeof(reserve_proof.public_address_voted_for));
  memset(reserve_proof.reserve_proof_amount,0,sizeof(reserve_proof.reserve_proof_amount));
  memset(reserve_proof.reserve_proof,0,sizeof(reserve_proof.reserve_proof));
  memcpy(reserve_proof.block_verifier_public_address,"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP",XCASH_WALLET_LENGTH);
  memcpy(reserve_proof.public_address_created_reserve_proof,"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP",XCASH_WALLET_LENGTH);
  memcpy(reserve_proof.public_address_voted_for,"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP",XCASH_WALLET_LENGTH);
  memcpy(reserve_proof.reserve_proof_amount,"120000000",9);
  memcpy(reserve_proof.reserve_proof,"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5j1ey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed41JUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti",537);
  start = time(NULL);
  count = send_invalid_reserve_proof_to_block_verifiers(&reserve_proof);
  total = time(NULL) - start;
  if (count == 1 && total <= MAXIMUM_TIME_BLOCK_VERIFIERS_SEND_DATA_SOCKET)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for sending a message to all block verifiers using block_verifiers_send_data_socket took %ld seconds out of %d seconds\033[0m\n",total,MAXIMUM_TIME_BLOCK_VERIFIERS_SEND_DATA_SOCKET);
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for sending a message to all block verifiers using block_verifiers_send_data_socket took %ld seconds out of %d seconds\033[0m\n",total,MAXIMUM_TIME_BLOCK_VERIFIERS_SEND_DATA_SOCKET);
  }



  start = time(NULL);
  memset(data_test,0,sizeof(data_test));
  memcpy(data_test,"{\r\n \"message_settings\": \"MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_CREATE_NEW_BLOCK\",\r\n \"block_blob\": \"" NETWORK_BLOCK "\",\r\n}",166417);
  sign_data(data_test);
  for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++)
  {      
    memset(send_and_receive_data_socket_thread_parameters[count].HOST,0,sizeof(send_and_receive_data_socket_thread_parameters[count].HOST));
    memset(send_and_receive_data_socket_thread_parameters[count].DATA,0,strlen(send_and_receive_data_socket_thread_parameters[count].DATA));
    memcpy(send_and_receive_data_socket_thread_parameters[count].HOST,"127.0.0.1",9);
    memcpy(send_and_receive_data_socket_thread_parameters[count].DATA,data_test,strnlen(data_test,BUFFER_SIZE));
    send_and_receive_data_socket_thread_parameters[count].COUNT = count;
    pthread_create(&thread_id[count], NULL, &send_and_receive_data_socket_thread,&send_and_receive_data_socket_thread_parameters[count]);
    if (count % (BLOCK_VERIFIERS_TOTAL_AMOUNT / 4) == 0 && count != 0 && count != BLOCK_VERIFIERS_TOTAL_AMOUNT)
    {
      nanosleep((const struct timespec[]){{0, 500000000L}}, NULL);
    }
  }
  for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++)
  {
    pthread_join(thread_id[count],NULL);
  }
  total = time(NULL) - start;
  if (total <= MAXIMUM_TIME_SEND_AND_RECEIVE_DATA_SOCKET_THREAD)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for sending a message to all block verifiers using send_and_receive_data_socket_thread took %ld seconds out of %d seconds\033[0m\n",total,MAXIMUM_TIME_SEND_AND_RECEIVE_DATA_SOCKET_THREAD);
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for sending a message to all block verifiers using send_and_receive_data_socket_thread took %ld seconds out of %d seconds\033[0m\n",total,MAXIMUM_TIME_SEND_AND_RECEIVE_DATA_SOCKET_THREAD);
  }



  delete_database(database_name,0);
  INITIALIZE_DATABASE_DATA(2);
  RESET_INVALID_RESERVE_PROOFS_DATA;
  for (count = 0; count < 20; count++)
  {
    invalid_reserve_proofs.block_verifier_public_address[count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    invalid_reserve_proofs.public_address_created_reserve_proof[count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    invalid_reserve_proofs.public_address_voted_for[count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    invalid_reserve_proofs.reserve_proof[count] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));
  }
  // initialize the reserve_proof struct
  memset(reserve_proof.block_verifier_public_address,0,sizeof(reserve_proof.block_verifier_public_address));
  memset(reserve_proof.public_address_created_reserve_proof,0,sizeof(reserve_proof.public_address_created_reserve_proof));
  memset(reserve_proof.public_address_voted_for,0,sizeof(reserve_proof.public_address_voted_for));
  memset(reserve_proof.reserve_proof_amount,0,sizeof(reserve_proof.reserve_proof_amount));
  memset(reserve_proof.reserve_proof,0,sizeof(reserve_proof.reserve_proof));
  memcpy(invalid_reserve_proofs.reserve_proof[0],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti",537);
  memcpy(invalid_reserve_proofs.reserve_proof[1],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NbzvSR1wszWsvK6FWUS2EG26WsxJ6KVqAKTTFQUAhw6BQVrX11BwfgJdtgRtKXepc1Nzjb6dWs8fCH5Q5LPZf3NY8pLrdYfsYvA1bRTtbfJSJcifMX22g5wukytaX43TUPA2CvacUXxPUnFmSJwy1DAUMnLGDUuW3fAcMLfTYZGNzK5ifErHJ2DC55jDrffSjACL5EZQk5V5MXeLHzjEtA9LKXrX9ktQAfqNcXiP1oca9htAdkDYHDGEocGktiGtMth79oBDiKfDxPaMPTqfB6nMQxPj3Q8WsU3Won1vE9ggXTK7j8R64miuoQGQT6kEnhww1wtHAKWYBGC1BvHiDLCguhHFEeCJH3bbafpKouBQ26XvFWjcgTnv7RF9qPMn3BCNLUYtbn8o7o1eejV84mYYZ4F71T2D6tc71hE9FnHpaK4FUmNQ5mY8CW3vYBG1iG6GmwqW9buHKVHx6fG2GqrvR",537);
  memcpy(invalid_reserve_proofs.reserve_proof[2],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NN9oPHNJwcKf646XJeeJ4UrJwKiFZvvzdRE6vAEEyHvnM3wBu1Bviq5DBTZP8XRyAJy4AXz3mQFGbzES6gC8uirABFecuKCacyLmxSZAMQ6yoSasRdXNURj94Npd9ocW5XUexv5xoHzDRG8JJ5jM1D9nzuuBV4gZrxwcgTbKuk5zddBUZdhdVGW5An8H4iLMF4PoPgk9fTKAKV6rV19aXXaSRJo5CmmAC37YASx8ppgh5BoDLSiNCfiLjSLpQjhQdPVPAuYGdQzxHMvigxTqXyQS8kCGQHGM52zaxxxK98s4XjJ7aU6vDHUoj35XyvxD5LiCYz5ESCtRcoS1Bw2XJkWvGu8vVtogUV66ejAAoY9KzaYrF1CjGbYPZtg2gYWVSwNCMS5peXHk6mwDHSP1tmSoLFEzFVX9CiAKPXQcUussue6w527pGZAYnqUvLBZQauFrcd2FXxeLx4rrwXj4SnZh5",537);
  memcpy(invalid_reserve_proofs.reserve_proof[3],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NMgDQ1mMESB9TVcANL57CGPgJbx3RKRnKsw3i3SAHi1uLqELo1Bv8A4RqdwV8o2XHnzwNLg9mrduoVL4oHE89ka6ZmhvxNhYnetNnkDsT5fmixsA1mGQ6irZgEVpo7SZjeJvH3T1G8hioRe2KpLP1DAVQ1j1HHW3NCoW3huZppKeepRBFpLoibxbiNsLmX8HDPEq6UyYYPFEKcK36NgvXJeiSUwPBwmhvfRkM1sqmaSZ2rFBWBRvKiB6tCfLUocfZA8Vkn1ReqazADd7ahjrdGz3cWoE3u5s9RZCqMTG1jrCDvDdRaxask64jP1L8AaCYAmGTgJNv6t3Zr81BwzDvQkxbicZLVkaJw9qPYmxidbZeDQ6XYhg8ASJRHW4XZVVEjFHyHQTjy4XerbfthvENw2t95ZEE1aPaiGgFh6CoDdA3Zh2G8YJguX9nU8iW2zVvYindPVk8UVraiqVGD66qPaDR",537);
  memcpy(invalid_reserve_proofs.reserve_proof[4],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NQCgyHa86iohwBQtUYqsFEBxwTwEhZdB8BUmi3MBkt9p5hsMH113UoGhwWp554GA1LgZWkNavEg5gsKk4yFUG2Wqci5qKNL4ah3qw3j7KPoh2jNB4AwaYZLVuwLuRqHYudFoDeqHS7i9ZXZ9GN47113kbZbNgye8PyvjvPXgtTR9URHxFg3QtcZuz8xBvwR7NmJVw4zT8wJMZ9AeviMe7MbNLv13os7Em9ySGc6h85hmZhFrNMZqKuQ9Zg6EUNh7MZRNXc6v1J2Ka54NrMFQizzGMyhxNjYdy2H2swTSFamg1CHAvuaZNqwvXdNxtij7kwZh6sWfoX47Tm21Ak9cK2Rf2piFfgnp7P5pPaAWEJac1W8vjiNHSLRB1k7G6Xr6SJGdxTLGcDKBc9JJNebRxrpovRcYFgc9zuZ7jePevRUYRzipb863gVatEDdFN1NCT12VfjqcdTDSy1f7kJm366MBRe",538);
  memcpy(invalid_reserve_proofs.reserve_proof[5],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NFBQCR6tvtf9VZmpe3yEp9HUrLVVhUsh3od6tePf2WxjPs39y113Ujth3vguaVREosd4KssCExayJGoYtXb1eskQmUe1DJhCcDBbXbDLP5R4fMx1v7UX9bELM62vUCSt3ftkSFHaTF3F2m6zpGBR113kWUgAttUT6TtpxBRkmR1z7tKEnMdpQDF95DDZfcSQZnwXX7ixVa85W9zQT9Z4HvQZvA6LA517GZhtUfxT4fXJXvdj8tHSNRjgaxEgCCn2WkehJQEbyCvAgiCkU3gjpdnKApZAVamRcYKYtDSPZcpRd9ZiZmjg6QXvsJ7xBi1DniHKR4RV7CM7dQk1Ak9jsFDJ6sbshVEYsSrnEX5q6WWzHkMaEgRd2TSkrcGckvqKGigMJSEUrp3eFBNfkHRpXT4atrkCERjhcPQr5BuLmgs6VFNvPd4qy2xTEDz4hcYwB9R17iTEUKjMq2q52x8A5z6VJm",538);
  memcpy(invalid_reserve_proofs.reserve_proof[6],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NiQQdQLR2dvXxdVDY14ohv8YeG5awfLre3LWKuCAZ3czqBwx3113UeKJM4r2iuFfeuospz5Nz5sGSfjw62DCMUHLLbSx98WPQfaY558zBimfMZPtQj8Wi64LP8JWcVF9objHHGgAJN5AXzodUwgT113kWqHCDfoe11QW54cQtkiadCAQGFbZnKqRdm7XjYxxZWc8MUTq64oiJKN2HoPzu8S4TWZd3hQsxSfX3Vo4BZp3gSbuFRVhZ75T3xNUZ86DV1Zw8Tpa1WXaaiQxNEbA1thKJtfXfU24mgRM4BwX4NgGKmLASAYhsh94J2UfMGPo37N6LsyDTDKYK7e1Ak9ZgQzL2z9EtCHG1H5gW72yLndfRdveHC7SSLqqPU9U9G5LRTYausWfwgGdfTESiQSHouWUDWmLjR7m8AKZYKpZ2yD1SQUVQb8xc68aDHkJEQyLRvQsmrLgGcBB23S9GxJ5GnHaax",538);
  memcpy(invalid_reserve_proofs.reserve_proof[7],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NvFXVMBgRDLfFzrbKcb6etZhNnxWCJWAadHixQZSbEErPTMiP113UVpSvB68MLPJeRtGGV84Kco2pidEHp1Kr8FHYQQqtXnigdNRovZSewSM4eCuxCGi5PZaZ6wTDv3ssnvYve2G2BAjZtEMhTuq113kYtFuZUFC9NSbGN62ar8gb6me7Sn3aRrTHinhKpFY147LqMv7a2B6JKh6KHCvaUExzfsJ1XWQBTg3xecEq9VqD53egXuNrbyf7iKr7rAFyrZBL3ZFzmrikM4L8aCzdbDWNQcezSSpN4BAq6U6SiCDGcBJFCkgPzZU79kGyWPRtSrcbdvuf2wiYyE1Ak9UeDvLdJ5EmQ52g84TDDVkiVKSiwn6JAC1xSHNd2UCxPBXHwRAYgCoEmUHUoNNRQLUNzNqN1Qp1wenqxMeMzzJZqqSxHfiaCj1CSjs7MxVBTe57KNcGEpyVcmSV2pSHCd6aDvQT6",538);
  memcpy(invalid_reserve_proofs.reserve_proof[8],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118Nd66C58NdU8sZef7KoNS6jfjv1aYqoWc9Z3XwU7eUuACjAMv71Bx7BFRCms13QKzz3FqaJNLFfuyzPkMzfM19qNowqouBYT6qyrLBZTYTF2ihnBXmwraH4uFUNe87QG6qRXKPun1RPmk3qjWYuWw1D8cSKyqCpNWaSeA9PFmPzAeqiP5Vx77eZZdKuhdhwbK2B2zhi4zMCkAkJjaTQxvJG2sZre7UgYa1b5YEDxa9SAQJuVMsvQaDEG6PiH9xjjDeGab1L7UeRqKoEhTqRSugaRFV7yQQj2MyCXPHTvJpNczsMHd9kaCyHP3CTxTij16PSWmaCrn5p22FNp1BxBmFkNPdbGRmGUCcCYR5LLHG1BzRtL46dd19eYohphTLcAzUBHgikdJamxWnyySHWdnnG5qCrfE3cDj1MvQNz76mSUxn1Ybj1bUPE7C69hR4aBfJPHVi3THE2fmqZXTTUo4kPc1N",537);
  memcpy(invalid_reserve_proofs.reserve_proof[9],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118N4SC1cC2znqTJ9ES3dvN6TekYCXHZt1UwJSanbaWQQ4WsDk3Z113UnETjPu4A4sHDuLWjXzfXPJ99wLk2NEbU7rXTdozkagMLht3PDpi2wzCM6DL79qDCtJfLJ6a1sDuhZ6yMeCbh8Szen1TJ3WX113kNXVTKsWfsFfAeKwki6aZAUxC325H21Z2QUm95eo4bGmqN48PdYeJMsuXkTP2pRbV35L3sVnx29i2SbFtpWgSbE3WGDDeC5xUrZr4e3nuRZE2D6qSDnxixPes7koSKypdV6f4DweXemoNHx1R42K8q5cfyML7MRhyW2ancaVZ8MyCdb2kNtQxTMi1Ak9orv3GhaVS1B72pFWfKQQ7gMXyePgjhc5PLaQuHmmgjPZ8TLBktMdiio3Kd3ZuNGLuitar2aNUJiZXP5UiRx7BMa28ck6cvYRgV9HBqdGs13wUDw6DnhWR7oLvpdaLRYY26vftFH",538);
  memcpy(invalid_reserve_proofs.reserve_proof[10],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NTiwbZLCstwWzpKy8J6J8M1uq9qf5PbWha7C68N8V7qshnzUP113UoSQuy78bpD5CeY2H3ViztSqDBFmmMiNu6uCQGkbu4NrZhZk2T7y7T1ixLCAxYfKRtd2pAAbuAWeBVCNzvmKB8YaneMgZ3mH113kXRg67xZRBtVcawf1CTYHCVuhpur8F6nAGxwFeNoRAdr6rnGwa1SUVdqrXypfmJJyzpPUGTSriAJqtRSBKJqAgQDPz4aV99aLMHHKWAv3eKaGAvBZ3obk2GuVBnHeUUEHAihQ5ECndKS4dw8YxMogdY9o8PvrhZgt2gnX4YrUiF5VbGBcBUsxbag1Ak9oZfTH2d1WeyMntcv1Mb28Nzc73UvMbck3VyBedN57gV1LS8WNV1SeHAFBiGgeBcuveGee9VZYQvGRfiWzkJf9JfYWoQ4dyxh13BrXpp3rF1nWv3UXxMrJJX8SRtQy5zrLAywENZ",538);
  memcpy(invalid_reserve_proofs.reserve_proof[11],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NhgFwT234BAHaZpNVUgpw57BNNxQk4GmvZQCve2PnN1Cnoyk31BvkeGg866DJ1KMrD6gP1KfDoHgApSvV7Rs14czdVEndFSz3oqzE3zcXp9UgYViWVqJXPqgdXrWzuAqaVmeJy2jZ1N7NNrDF1jm1DAGmageWVxbugKdf1rHUS61fXPvdiEYsaPFvwujnKtyeooG9tG2tJBAGFs4BSBNe98YWxM1Vkcnf3foGzt8zCZwSRre71EZcytGKVjRbdJL4dYW41EKc7ZH8McVQyUCWigufp3vKjUd1BLGW7E3HbxQW3dj9udGYxpR8cET8SfUEKtxGHuKjL2miue1Bv2avJpeKiUWhPf1q8nPgFSSGMj2mSRBPkxJvDrpCMJcE4Z4ozGKSqZVakiA8hLgF38ha9XeUQadShuqzRkr1fSF1cDgcYaBiKGy3TtzmN9rS9KFvTVjyrydeSW318ohHya5PD3Jd",537);
  memcpy(invalid_reserve_proofs.reserve_proof[12],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NVaFAFVWQJsLQfgXVhi5RZysdBxHLQ3m3MbsgXbEMMCSXdbs5113UZv51HEQYhfKa8SNmPJ8hg7pVtrM8JhqSW2JoiYpZW24YNZKUipJfGQAToYjufPKf6EpMBabbA2c3u4q9dGfUNVAsarYXPnw113kQgGTutsFwxBTLCN1o3EkFgJAkqJmaTM2qqNfZKAX1m5KTzmPo5uYVSBa8VWxtuaPhYeoXwKNaFQT7aJtbsw4gPRDu2AUuMW684KdxcKrS4EjWL5RcJkPUcwSQYXCw9WN8Duh6jXyKwABdGeoXUmP8oLWtygKFkVqiHS1Jj26E24e6XEoP1SbRrg1Ak9TsSgBKPeXjZ5pJPmRtPifbCthUwmp2dpMPjJyveQ6U9F5Nspek9NJTy56k2syJhrCLv9rCipsPphLSbBcQEudZzkbhPRy3d722YvS1Qy2gDq33mwk5JrZXnHcqQNKQPJC5nWbHG",538);
  memcpy(invalid_reserve_proofs.reserve_proof[13],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NR5pyLUHTrzNp1jgcXYceCmXcyKGhQUb7o7j8SejmtK1BGWum1BvPsj8HAZHApAujd6nyjYhhxLuD2EuzSJ2T6XzqWNVbgrQA2Zab2PniccUJr4UuJmKMgium6CtC3TyAbxWv87n5eivRSPneB9q1D94z76oVBZZk8JyrCEBnpNNcAHfLGdn5Vohw9pS8mr8a7xrGp8t5HdZBac8mrqQNHEekyPUTAG5BiJWy8EEMSZgc8ieK6jcSCdVE3yoYZrx7fNd6MZPFruzGNqF9DHQJpCSPw9z4NeMp5Pf8rJmfo5eFsgy7TrtsV8zgAQrQksWKYyw4LLKnVFjLPi1BwmixzbUnggnHQxM9py6sSq6yUjuD5mr9rg1KcyfRZQ2XpyyEwoeEV9Jd26yKih3ZgQuLTsoywV2LJWJHa89UA1ixYGjHkaokUdkaXr4Wfa9DGdMY9Q6YoZ42dfz4XjGmmF6fpQz5",537);
  memcpy(invalid_reserve_proofs.reserve_proof[14],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NKGRYSRkBmumeAXu2t5zoYgUchPaUMVgvFKuF6UJft6DRLZpb1BwBtebvdiJMsPF5nVBoL88pLEbBMzP5hLBc9Ti71MMagy7C5cVpQL1dtmahXza6TN8rLwwPUgSvQ1TnmPqutXbMefzs2jX5yYT1D9RL1SQC8NCYyin4vnY51Dpnnimf35QXbTQheoNs8bANFAGawGUBw8QP845pHBm7kAGE8Uc9TPSVLLx5m16XPPY72dCvxUwQk2Bz8MugKntd5MkyNkxWy3B7anuWCtJwoxTM8ZXwsSuiCi9M2r5mw9PMcAJQXmqtWVXiS9eMht73XXqN8Mch6kQFmi1BvvkDw9MCPdp3Yzx8ajmyjGyEf17oHvaidRdAH7XUvqLnpRBukky4kHWgu9i8Up3Xhw6nwuwPxRuhB6w7qv3991j3XzGeAucXTDgkTPjvuw7bQKVF6WrSXmbJteEcFpWA2M1GsjiQ",537);
  memcpy(invalid_reserve_proofs.reserve_proof[15],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118P1c3vZLJ76utEqs2Fdymmd18aU1hRKnUQGD8YP8fUWo7a6wHq1BvvV5zo9y5GyCPTR4YqVdepMoRanbseEMe4egDDLBJo1x91AM5iE2gH2JLrTcaTJn7ZWMmnnCsq99Kb4RAXLo7RbQBE2djmQzT1D8TYMgeppLdFBpBesfHDP1c79KuD7UQmMQyywnRgHpgRRgSDaZY3z47QozqqzjkQy7AHWcVZW2HvVwup4vfgffx93EGVzc8Fx2YWwghcLi6yJMME1v9S64AtgUHtnkZfCwTUoGxDyQvuDyHVPiQahsp8qUYxcWzQB24ajJ94rBi3MeJHz1hL3mLoNG1BvSiJJb2DSBNiZKUA4Ev9LCodHHqsjuBQ3zGpwhomvtUtTVK4TMApUhKdHKUKFmHPWZYtqdHaxPxRiz6v6yemQ45nJy6tq4J4PjVUUZQdje8UHmwnYtk86CnSi9RVEJJgAJ6WWtZd",537);
  memcpy(invalid_reserve_proofs.reserve_proof[16],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118PFMZP3mEiqLZBnwhY2ZdzmUzBEV8vb7kyAEXm3BnyzniyALcX113Udjpi3MfZYZFmtzXmHk1WiQpjAeuKKBNKzpNvwP3u3S6uFynVresVDmV2Jq9UgVgsSSzAq25Tr8n6KfcMPkkETqpxPdUkzcf113kqvgMLLNKrWqB4HJbigcmuBzBwBYppDxZ1c4bpcqCWGm2TctNVFVgsQKRXjZhpGCnDc7hKfYgj8LMpwsTjQjfQQoC9T38kDjbpbh5C8yCSie6shRWRdE5pMkerFxuwx96XJDvrLPweJa6LZYVUJ6W4ijXGp8SwrVjUPVTJNFsjt8a8GuGKedTW3A1Ak9VCEj9MaETmewJvoJB57URT7xzBowJ9JJwj29xxGme21D63pPuZzjMjZp69M83dYz5BEPDyA1bjTtHXS5GyB6aarGEdkvfGG4ZUk4fPWzY8hKe1mrNcy8iVRp7VmWuUCuRrvCyRD",538);
  memcpy(invalid_reserve_proofs.reserve_proof[17],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118Nm6ZbNk26cUKoQLHRgLfxMK6cXaEKUjdxDzEYaJau7VvrxrFh1Bx45Z5yqBsRoFVXij2BNbL6oWiVKcmbDdA52UsoKrotgV97TJS866NNgw13GNUPgdbHqKzrndWyjYQMbNcCtmFoAjqDZ2nUki31D9NG4WBeuNWMJdiLmQA5FiTUNnpBPPsMDmKfat8G7UsTwVwC8rgFRDQyKJd33g2hKSj6hKpsYxfeC7pc4HUfuhgC3MoWADBjdvSr3o2WSzb6NXt5ftYvEJDW8pv3DZ9RtEsQSWd9p2hfHTU1PL7YVVREXW4euAnYQvK2JhctTF46xhxh1ktmfpNBYU1BvJ7a6UZsGFpMavkrgxdWUjYXmZZmuHXfc2xYBUezqh491iaSKtbGjiNu9xESvzaaYba5BmKLsabguQXdFy5o84BqizPMitUACiAjdivwEYwLJVifqaEwJmDXQo1VPMDKFE1eB5CX",537);
  memcpy(invalid_reserve_proofs.reserve_proof[18],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NMQFZBxMsfrNuRXPfV2qqnmSxWXSFAuyJN2qdchG33bKVx3H1113UsPsoy5DagVBEMSUZj2CArH8f9rtmFarbECZ1QvpBidEarNp3323aanBpEeEDNGZjSGg2nXAuAa6BtkjRJ9x5bXMosKHkZ9y113kUKHdK9SCeP4g4WRmsAcHgLU2wTTto17u8NhDuZDjKgHRrT4Xyq3XZNEPcxcbKJFvueNZiTYD4WfJzt6KMzqcFyXfbRQrhqQZcWr7ZLzDkTJTFKLU2UdCsPdDBJDhfQRyfTGDDTs5nSM4iPQZ8axKPtNuYSkftFD2aMgGSrb2CVp4QvjZW6jkAXz1Ak9qn6fZss9WGsqjYgKMGH2eFjkvBRmsb2HKiumrKbjMVDLKjYPoetboiTxojeKPjX529qqAqpq2QsTGymT1prw37QPu7uqzKCDzfaLpAtY567R6AZ92aMi9UeivoDHHAiqSUP5oG5",538);
  memcpy(invalid_reserve_proofs.reserve_proof[19],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NbeQ6htDtJPees7GH3NHXKcrtJJ8VbqFWJFVLw8Dfrq28UFw51BvtKbU16YYjNJAeqAYsSVHg92toMYk4yY7HYUbZ65vgfn8wXbLELBXTKYQezErCAUBjwTDEGNdMk1QZF4tcLEmVUz4me8MaXts1D8ouHVqJQRAHv6KDHkMU143k5KrZbbkEMhijGDi9oGHRbUm2W4pLWMgzzZRPEQ7bP9eTRPHi2G2aa11pujWY4WaEPX12FyYbG2LJv18dvtejyiPTgXLVxPvRTNeP3aY7GwRRetePkxqJtJWXuXjp37yeeP4GJPaxeDzZhzzQ4T8JugtEHfCj8vs8j21Bx63K8SDTU3nQg7Ns6AoZg9rL2UCgA8W2FC7UmEb2ws85XJ8WfrTqzNpqoiweBGKgSG3wfbpzgMMKKz4obsEeTHZSfsKzwGRgMiEqkbTRSxdCPFi2tDWQdrWdvmaWm81iK42MSfud",537);
  invalid_reserve_proofs.count = 20;

  start = time(NULL);
  count = 0;
  while (time(NULL)-start < MAXIMUM_TIME_VALIDATED_RESERVE_PROOFS)
  {
    memset(data_test,0,sizeof(data_test));
    select_random_unique_reserve_proof(&reserve_proof);
    check_reserve_proofs(data_test,VALIDATE_RESERVE_PROOFS_WALLET,VALIDATE_RESERVE_PROOFS_RESERVE_PROOF);
    count++;
  }
  if (count >= MINIMUM_VALIDATED_RESERVE_PROOF_AMOUNT)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for validating reserve proofs. Validated %d reserve proofs (need at least %d) in %d seconds\033[0m\n",count,MINIMUM_VALIDATED_RESERVE_PROOF_AMOUNT,MAXIMUM_TIME_VALIDATED_RESERVE_PROOFS);
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for validating reserve proofs took %ld seconds out of %d seconds\033[0m\n",total,MINIMUM_VALIDATED_RESERVE_PROOF_AMOUNT);
  }


  delete_database(database_name,0);
  INITIALIZE_DATABASE_DATA(2);
  RESET_INVALID_RESERVE_PROOFS_DATA;
  for (count = 0; count < MAXIMUM_INVALID_RESERVE_PROOFS * 0.10; count++)
  {
    invalid_reserve_proofs.block_verifier_public_address[count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    invalid_reserve_proofs.public_address_created_reserve_proof[count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    invalid_reserve_proofs.public_address_voted_for[count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    invalid_reserve_proofs.reserve_proof[count] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));
    memcpy(invalid_reserve_proofs.block_verifier_public_address[count],"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP",XCASH_WALLET_LENGTH);
    memcpy(invalid_reserve_proofs.public_address_voted_for[count],"XCA1jh9Nw2XbshVEYsSrnEX5q6WWzHkMaEgRd2TSkrcGckvqoqxoQZejnv31SZPgVjL9uVRvX4NYsPbCLjMdT4m58kmgCnN63n",XCASH_WALLET_LENGTH);
    memcpy(invalid_reserve_proofs.public_address_created_reserve_proof[count],"XCA1oPZcuxH1WeyMntcv1Mb28Nzc73UvMbck3VyBedN57gV1c9ikU6tBALSjCwigMs1XCDtTYND7vfELU31PQEBh6zs5MLxYek",XCASH_WALLET_LENGTH);
    invalid_reserve_proofs.reserve_proof_amount[count] = 1000000000;
    memcpy(invalid_reserve_proofs.reserve_proof[count],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NTiwbZLCstwWzpKy8J6J8M1uq9qf5PbWha7C68N8V7qshnzUP113UoSQuy78bpD5CeY2H3ViztSqDBFmmMiNu6uCQGkbu4NrZhZk2T7y7T1ixLCAxYfKRtd2pAAbuAWeBVCNzvmKB8YaneMgZ3mH113kXRg67xZRBtVcawf1CTYHCVuhpur8F6nAGxwFeNoRAdr6rnGwa1SUVdqrXypfmJJyzpPUGTSriAJqtRSBKJqAgQDPz4aV99aLMHHKWAv3eKaGAvBZ3obk2GuVBnHeUUEHAihQ5ECndKS4dw8YxMogdY9o8PvrhZgt2gnX4YrUiF5VbGBcBUsxbag1Ak9oZfTH2d1WeyMntcv1Mb28Nzc73UvMbck3VyBedN57gV1LS8WNV1SeHAFBiGgeBcuveGee9VZYQvGRfiWzkJf9JfYWoQ4dyxh13BrXpp3rF1nWv3UXxMrJJX8SRtQy5zrLAywENZ",538);
  }
  invalid_reserve_proofs.count = MAXIMUM_INVALID_RESERVE_PROOFS * 0.10;
  start = time(NULL);
  check_reserve_proofs_timer_update_database();
  total = time(NULL) - start;
  if (total <= MAXIMUM_TIME_RESERVE_PROOF_UPDATE_DATABASE)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for check_reserve_proofs_timer_update_database took %ld seconds out of %d seconds\033[0m\n",total,MAXIMUM_TIME_RESERVE_PROOF_UPDATE_DATABASE);
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for check_reserve_proofs_timer_update_database took %ld seconds out of %d seconds\033[0m\n",total,MAXIMUM_TIME_RESERVE_PROOF_UPDATE_DATABASE);
  }
  RESET_INVALID_RESERVE_PROOFS_DATA;


  start = time(NULL);
  for (count = 0; count < 5; count++)
  {
    verify_network_block_data_vrf_data_verify_thread_parameters[count].total_amount_of_threads = 5;
    verify_network_block_data_vrf_data_verify_thread_parameters[count].block_verifier_total = 100;
    verify_network_block_data_vrf_data_verify_thread_parameters[count].start = count;
    memset(verify_network_block_data_vrf_data_verify_thread_parameters[count].network_block_string,0,sizeof(verify_network_block_data_vrf_data_verify_thread_parameters[count].network_block_string));
    memcpy(verify_network_block_data_vrf_data_verify_thread_parameters[count].network_block_string,NETWORK_BLOCK,sizeof(NETWORK_BLOCK)-1);
    pthread_create(&thread_id[count], NULL, &vrf_data_verify_timer,&verify_network_block_data_vrf_data_verify_thread_parameters[count]);
  }
  for (count = 0; count < 5; count++)
  {
    pthread_join(thread_id[count],NULL);
  }
  total = time(NULL) - start;
  if (total <= MAXIMUM_TIME_VRF_DATA_VERIFY_MULTITHREADED)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for VRF_data_verify using 5 threads took %ld seconds out of %d seconds\033[0m\n",total,MAXIMUM_TIME_VRF_DATA_VERIFY_MULTITHREADED);
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for VRF_data_verify using 5 threads took %ld seconds out of %d seconds\033[0m\n",total,MAXIMUM_TIME_VRF_DATA_VERIFY_MULTITHREADED);
  }


  start = time(NULL);
  for (count = 0; count < 4; count++)
  {
    verify_network_block_data_vrf_data_verify_thread_parameters[count].total_amount_of_threads = 4;
    verify_network_block_data_vrf_data_verify_thread_parameters[count].block_verifier_total = 100;
    verify_network_block_data_vrf_data_verify_thread_parameters[count].start = count;
    memset(verify_network_block_data_vrf_data_verify_thread_parameters[count].network_block_string,0,sizeof(verify_network_block_data_vrf_data_verify_thread_parameters[count].network_block_string));
    memcpy(verify_network_block_data_vrf_data_verify_thread_parameters[count].network_block_string,NETWORK_BLOCK,sizeof(NETWORK_BLOCK)-1);
    pthread_create(&thread_id[count], NULL, &vrf_data_verify_timer,&verify_network_block_data_vrf_data_verify_thread_parameters[count]);
  }
  for (count = 0; count < 4; count++)
  {
    pthread_join(thread_id[count],NULL);
  }
  total = time(NULL) - start;
  if (total <= MAXIMUM_TIME_VRF_DATA_VERIFY_MULTITHREADED)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for VRF_data_verify using 4 threads took %ld seconds out of %d seconds\033[0m\n",total,MAXIMUM_TIME_VRF_DATA_VERIFY_MULTITHREADED);
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for VRF_data_verify using 4 threads took %ld seconds out of %d seconds\033[0m\n",total,MAXIMUM_TIME_VRF_DATA_VERIFY_MULTITHREADED);
  }


  start = time(NULL);
  for (count = 0; count < 2; count++)
  {
    verify_network_block_data_vrf_data_verify_thread_parameters[count].total_amount_of_threads = 2;
    verify_network_block_data_vrf_data_verify_thread_parameters[count].block_verifier_total = 100;
    verify_network_block_data_vrf_data_verify_thread_parameters[count].start = count;
    memset(verify_network_block_data_vrf_data_verify_thread_parameters[count].network_block_string,0,sizeof(verify_network_block_data_vrf_data_verify_thread_parameters[count].network_block_string));
    memcpy(verify_network_block_data_vrf_data_verify_thread_parameters[count].network_block_string,NETWORK_BLOCK,sizeof(NETWORK_BLOCK)-1);
    pthread_create(&thread_id[count], NULL, &vrf_data_verify_timer,&verify_network_block_data_vrf_data_verify_thread_parameters[count]);
  }
  for (count = 0; count < 2; count++)
  {
    pthread_join(thread_id[count],NULL);
  }
  total = time(NULL) - start;
  if (total <= 100)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for VRF_data_verify using 2 threads took %ld seconds out of %d seconds\033[0m\n",total,100);
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for VRF_data_verify using 2 threads took %ld seconds out of %d seconds\033[0m\n",total,100);
  }


  start = time(NULL);
  for (count = 0; count < 1; count++)
  {
    verify_network_block_data_vrf_data_verify_thread_parameters[count].total_amount_of_threads = 1;
    verify_network_block_data_vrf_data_verify_thread_parameters[count].block_verifier_total = 100;
    verify_network_block_data_vrf_data_verify_thread_parameters[count].start = count;
    memset(verify_network_block_data_vrf_data_verify_thread_parameters[count].network_block_string,0,sizeof(verify_network_block_data_vrf_data_verify_thread_parameters[count].network_block_string));
    memcpy(verify_network_block_data_vrf_data_verify_thread_parameters[count].network_block_string,NETWORK_BLOCK,sizeof(NETWORK_BLOCK)-1);
    pthread_create(&thread_id[count], NULL, &vrf_data_verify_timer,&verify_network_block_data_vrf_data_verify_thread_parameters[count]);
  }
  for (count = 0; count < 1; count++)
  {
    pthread_join(thread_id[count],NULL);
  }
  total = time(NULL) - start;
  if (total <= 100)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for VRF_data_verify using 1 thread took %ld seconds out of %d seconds\033[0m\n",total,100);
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for VRF_data_verify using 1 thread took %ld seconds out of %d seconds\033[0m\n",total,100);
  }

  
  RESET_NETWORK_BLOCK_ROUND;
  start = time(NULL);

  memset(data_test,0,sizeof(data_test));
  block_verifiers_create_VRF_secret_key_and_VRF_public_key(data_test);
  sign_data(data_test);
  block_verifiers_send_data_socket((const char*)data_test);

  total = time(NULL) - start;
  if (total < MAXIMUM_TIME_NETWORK_BLOCK_PART_1)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for Part 1 of the network block round took %ld seconds out of %d seconds\033[0m\n",total,MAXIMUM_TIME_NETWORK_BLOCK_PART_1);
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for Part 1 of the network block round took %ld seconds out of %d seconds\033[0m\n",total,MAXIMUM_TIME_NETWORK_BLOCK_PART_1);
  }


  RESET_NETWORK_BLOCK_ROUND;
  start = time(NULL);

  memset(data_test,0,sizeof(data_test));
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memset(VRF_data.block_verifiers_vrf_secret_key_data[count],0,strlen(VRF_data.block_verifiers_vrf_secret_key_data[count]));
    memset(VRF_data.block_verifiers_vrf_secret_key[count],0,strlen((char*)VRF_data.block_verifiers_vrf_secret_key[count]));
    memset(VRF_data.block_verifiers_vrf_public_key_data[count],0,strlen(VRF_data.block_verifiers_vrf_public_key_data[count]));
    memset(VRF_data.block_verifiers_vrf_public_key[count],0,strlen((char*)VRF_data.block_verifiers_vrf_public_key[count]));
    memset(VRF_data.block_verifiers_random_data[count],0,strlen(VRF_data.block_verifiers_random_data[count]));
    memcpy(VRF_data.block_verifiers_vrf_secret_key_data[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_SECRET_KEY_DATA,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_SECRET_KEY_DATA)-1);
    memcpy(VRF_data.block_verifiers_vrf_secret_key[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_SECRET_KEY,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_SECRET_KEY)-1);
    memcpy(VRF_data.block_verifiers_vrf_public_key_data[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_PUBLIC_KEY_DATA,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_PUBLIC_KEY_DATA)-1);
    memcpy(VRF_data.block_verifiers_vrf_public_key[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_PUBLIC_KEY,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_PUBLIC_KEY)-1);
    memcpy(VRF_data.block_verifiers_random_data[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING)-1); 
  }
  block_verifiers_create_VRF_data();
  memset(VRF_data.block_blob,0,strlen(VRF_data.block_blob));
  get_block_template(VRF_data.block_blob);
  memset(data_test,0,sizeof(data_test));
  memcpy(data_test,"{\r\n \"message_settings\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND_CREATE_NEW_BLOCK\",\r\n \"block_blob\": \"",97);
  memcpy(data_test+97,VRF_data.block_blob,strnlen(VRF_data.block_blob,BUFFER_SIZE));
  memcpy(data_test+strlen(data_test),"\",\r\n}",5);
  sign_data(data_test);
  block_verifiers_send_data_socket((const char*)data_test);

  total = time(NULL) - start;
  if (total < MAXIMUM_TIME_NETWORK_BLOCK_PART_2)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for Part 2 of the network block round took %ld seconds out of %d seconds\033[0m\n",total,MAXIMUM_TIME_NETWORK_BLOCK_PART_2);
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for Part 2 of the network block round took %ld seconds out of %d seconds\033[0m\n",total,MAXIMUM_TIME_NETWORK_BLOCK_PART_2);
  }


  RESET_NETWORK_BLOCK_ROUND;
  start = time(NULL);

  memset(data_test,0,sizeof(data_test));
  memset(VRF_data.block_blob,0,strlen(VRF_data.block_blob));
  memcpy(VRF_data.block_blob,NETWORK_BLOCK,sizeof(NETWORK_BLOCK)-1);
  block_verifiers_create_block_signature(data_test);
  sign_data(data_test);

  total = time(NULL) - start;
  if (total < MAXIMUM_TIME_NETWORK_BLOCK_PART_3)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for Part 3 of the network block round took %ld seconds out of %d seconds\033[0m\n",total,MAXIMUM_TIME_NETWORK_BLOCK_PART_3);
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for Part 3 of the network block round took %ld seconds out of %d seconds\033[0m\n",total,MAXIMUM_TIME_NETWORK_BLOCK_PART_3);
  }


  RESET_NETWORK_BLOCK_ROUND;
  start = time(NULL);

  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    if (strlen(VRF_data.block_blob_signature[count]) == VRF_PROOF_LENGTH+VRF_BETA_LENGTH)
    {
      memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count],VRF_data.block_blob_signature[count],VRF_PROOF_LENGTH+VRF_BETA_LENGTH);
    }
    else
    {
      memcpy(VRF_data.block_blob_signature[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE)-1);
      memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count],GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE,sizeof(GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE)-1);
    }
  }
  memset(data_test,0,strlen(data_test));
  network_block_string_to_blockchain_data(NETWORK_BLOCK,NETWORK_BLOCK_HEIGHT,BLOCK_VERIFIERS_TOTAL_AMOUNT);
  verify_network_block_data(0,0,0,NETWORK_BLOCK_HEIGHT,NETWORK_BLOCK,BLOCK_VERIFIERS_TOTAL_AMOUNT);
  RESET_BLOCKCHAIN_DATA;
  memset(data_test,0,strlen(data_test));
  CREATE_BLOCK_DATA;
  blockchain_data_to_network_block_string(data_test,BLOCK_VERIFIERS_TOTAL_AMOUNT);
  sign_data(data_test);
  for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT * BLOCK_VERIFIERS_TOTAL_AMOUNT; count++)
  {
    VRF_data_verify(NEXT_BLOCK_VERIFIERS_PUBLIC_KEY,BLOCK_VALIDATION_NODE_SIGNATURE,NETWORK_BLOCK);
  }
  total = time(NULL) - start;
  if (total < MAXIMUM_TIME_NETWORK_BLOCK_PART_4)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for Part 4 of the network block round took %ld seconds out of %d seconds\033[0m\n",total,MAXIMUM_TIME_NETWORK_BLOCK_PART_4);
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for Part 4 of the network block round took %ld seconds out of %d seconds\033[0m\n",total,MAXIMUM_TIME_NETWORK_BLOCK_PART_4);
  }


  RESET_NETWORK_BLOCK_ROUND;
  start = time(NULL);

  total = time(NULL) - start;
  if (total < MAXIMUM_TIME_NETWORK_BLOCK_PART_5)
  {
    fprintf(stderr,"\033[1;32mPASSED! Test for Part 5 of the network block round took %ld seconds out of %d seconds\033[0m\n",total,MAXIMUM_TIME_NETWORK_BLOCK_PART_5);
    count_test++;
  }
  else
  {
    fprintf(stderr,"\033[1;31mFAILED! Test for Part 5 of the network block round took %ld seconds out of %d seconds\033[0m\n",total,MAXIMUM_TIME_NETWORK_BLOCK_PART_5);
  }



  for (count = 0; count < NETWORK_DATA_NODES_AMOUNT; count++)
  {
    memset(network_data_nodes_list.network_data_nodes_public_address[count],0,sizeof(network_data_nodes_list.network_data_nodes_public_address[count]));
    memset(network_data_nodes_list.network_data_nodes_IP_address[count],0,sizeof(network_data_nodes_list.network_data_nodes_IP_address[count]));
  }
  INITIALIZE_NETWORK_DATA_NODES;

  // write the end test message
  fprintf(stderr,"\033[1;33m\n\n%s\noptimizations functions test - Passed test: %d, Failed test: %d\n%s\n\n\n\033[0m",TEST_OUTLINE,count_test,OPTIMIZATIONS_TOTAL_TEST-count_test,TEST_OUTLINE);
  return count_test;
  
  #undef MESSAGE
  #undef VALIDATE_RESERVE_PROOFS_WALLET
  #undef VALIDATE_RESERVE_PROOFS_RESERVE_PROOF
  #undef MAXIMUM_TIME_BLOCK_VERIFIERS_SEND_DATA_SOCKET
  #undef MAXIMUM_TIME_SEND_AND_RECEIVE_DATA_SOCKET_THREAD
  #undef MINIMUM_VALIDATED_RESERVE_PROOF_AMOUNT
  #undef MAXIMUM_TIME_VALIDATED_RESERVE_PROOFS
  #undef MAXIMUM_TIME_RESERVE_PROOF_UPDATE_DATABASE
  #undef MAXIMUM_TIME_VRF_DATA_VERIFY
  #undef MAXIMUM_TIME_VRF_DATA_VERIFY_MULTITHREADED
  #undef MAXIMUM_TIME_NETWORK_BLOCK_PART_1
  #undef MAXIMUM_TIME_NETWORK_BLOCK_PART_2
  #undef MAXIMUM_TIME_NETWORK_BLOCK_PART_3
  #undef MAXIMUM_TIME_NETWORK_BLOCK_PART_4
  #undef MAXIMUM_TIME_NETWORK_BLOCK_PART_5
  #undef RESET_INVALID_RESERVE_PROOFS_DATA
  #undef RESET_NETWORK_BLOCK_ROUND
}