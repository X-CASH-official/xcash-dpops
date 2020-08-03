#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "insert_database_functions.h"
#include "read_database_functions.h"
#include "delete_database_functions.h"
#include "count_database_functions.h"
#include "block_verifiers_thread_server_functions.h"
#include "define_macros_test.h"

#include "variables_test.h"
#include "block_verifiers_thread_server_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: block_verifiers_thread_server_functions_test
Description: Test the block_verifiers_thread_server functions
Return: The number of passed block_verifiers_thread_server_functions test
-----------------------------------------------------------------------------------------------------------
*/

int block_verifiers_thread_server_functions_test(void)
{
  // Variables
  struct reserve_proof reserve_proof;
  int count;
  
  // define macros
  #define RESERVE_PROOF_PUBLIC_ADDRESS_TEST "XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP"
  #define RESERVE_PROOF_TEST "ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB31qaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti"
  #define VOTE_DATA_TEST "\"vote_data\": \"59afe2f27423efdc1f5a7acc15fa5b76b6553b36886e9f32b0b7ab3baf5e77a0222a855957e5b9a6d4e174c8bd0b405c0eef42c6be1776e32f0f742aa1c125e5\""
  #define RESET_INVALID_RESERVE_PROOFS_DATA \
  for (count = 0; count < invalid_reserve_proofs.count; count++) \
  { \
    pointer_reset(invalid_reserve_proofs.block_verifier_public_address[count]); \
    pointer_reset(invalid_reserve_proofs.public_address_created_reserve_proof[count]); \
    pointer_reset(invalid_reserve_proofs.public_address_voted_for[count]); \
    pointer_reset(invalid_reserve_proofs.reserve_proof[count]); \
  } \
  invalid_reserve_proofs.count = 0;

  // reset the variables
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  count_test = 0;
  network_functions_test_error_settings = 2;
  delete_database(database_name);
  RESET_ERROR_MESSAGES;
 
  // write the start test message
  fprintf(stderr,"\033[1;34m%s\nblock verifiers thread_server functions test - Total test: %d\n%s\n\n\033[0m",TEST_OUTLINE,BLOCK_VERIFIERS_THREAD_SERVER_FUNCTIONS_TOTAL_TEST,TEST_OUTLINE);
 
  // run the test
  // sync_reserve_proofs_database
  INITIALIZE_NETWORK_DATA_NODES_TEST;
  INITIALIZE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_TEST;
  insert_document_into_collection_json(database_name,"reserve_proofs_1",RESERVE_PROOFS_TEST_DATA);
  insert_document_into_collection_json(database_name,"reserve_bytes_1",RESERVE_BYTES_TEST_DATA);
  insert_document_into_collection_json(database_name,"delegates",DELEGATES_TEST_DATA);
  insert_document_into_collection_json(database_name,"statistics",DATABASE_COLLECTION_STATISTICS_DATA);
  RESET_ERROR_MESSAGES;

  invalid_reserve_proofs.block_verifier_public_address[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_created_reserve_proof[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_voted_for[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.reserve_proof[0] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));
  invalid_reserve_proofs.block_verifier_public_address[1] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_created_reserve_proof[1] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_voted_for[1] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.reserve_proof[1] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));
  memcpy(invalid_reserve_proofs.public_address_voted_for[0],TEST_WALLET,XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.public_address_created_reserve_proof[0],RESERVE_PROOF_PUBLIC_ADDRESS_TEST,XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.reserve_proof[0],RESERVE_PROOF_TEST,sizeof(RESERVE_PROOF_TEST)-1);
  memcpy(invalid_reserve_proofs.public_address_voted_for[1],TEST_WALLET,XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.public_address_created_reserve_proof[1],RESERVE_PROOF_PUBLIC_ADDRESS_TEST,XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.reserve_proof[1],RESERVE_PROOF_TEST,sizeof(RESERVE_PROOF_TEST)-1);
  invalid_reserve_proofs.count = 2;

  // check_reserve_proofs_timer_create_message
  if (check_reserve_proofs_timer_create_message(data_test) == 1 && strstr(data_test,VOTE_DATA_TEST) != NULL)
  {
    color_print("PASSED! Test for check_reserve_proofs_timer_create_message","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for check_reserve_proofs_timer_create_message","red");
  }

  // check_reserve_proofs_timer_get_database_data
  delete_database(database_name);
  INITIALIZE_DATABASE_DATA(3);
  RESET_INVALID_RESERVE_PROOFS_DATA;
  invalid_reserve_proofs.block_verifier_public_address[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_created_reserve_proof[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_voted_for[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.reserve_proof[0] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));
  invalid_reserve_proofs.block_verifier_public_address[1] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_created_reserve_proof[1] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_voted_for[1] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.reserve_proof[1] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));
  memcpy(invalid_reserve_proofs.block_verifier_public_address[0],RESERVE_PROOF_PUBLIC_ADDRESS_TEST,XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.public_address_voted_for[0],TEST_WALLET,XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.public_address_created_reserve_proof[0],TEST_WALLET,XCASH_WALLET_LENGTH);
  invalid_reserve_proofs.reserve_proof_amount[0] = 0;
  memcpy(invalid_reserve_proofs.reserve_proof[0],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr1p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti",537);
  memcpy(invalid_reserve_proofs.reserve_proof[1],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NTiwbZLCstwWzpKy8J6J8M1uq9qf5PbWha7C68N8V7qshnzUP113UoSQuy78bpD5CeY2H3ViztSqDBFmmMiNu6uCQGkbu4NrZhZk2T7y7T1ixLCAxYfKRtd2pAAbuAWeBVCNzvmKB8YaneMgZ3mH113kXRg67xZRBtVcawf1CTYHCVuhpur8F6nAGxwFeNoRAdr6rnGwa1SUVdqrXypfmJJyzpPUGTSriAJqtRSBKJqAgQDPz4aV99aLMHHKWAv3eKaGAvBZ3obk2GuVBnHeUUEHAihQ5ECndKS4dw8YxMogdY9o8PvrhZgt2gnX4YrUiF5VbGBcBUsxbag1Ak9oZfTH2d1WeyMntcv1Mb28Nzc73UvMbck3VyBedN57gV1LS8WNV1SeHAFBiGgeBcuveGee9VZYQvGRfiWzkJf9JfYWoQ4dyxh13BrXpp3rF1nWv3UXxMrJJX8SRtQy5zrLAywENZ",538);
  invalid_reserve_proofs.count = 2;
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (check_reserve_proofs_timer_get_database_data(1) == 1 && strncmp(invalid_reserve_proofs.public_address_created_reserve_proof[1],"XCA1oPZcuxH1WeyMntcv1Mb28Nzc73UvMbck3VyBedN57gV1c9ikU6tBALSjCwigMs1XCDtTYND7vfELU31PQEBh6zs5MLxYek",BUFFER_SIZE) == 0 && strncmp(invalid_reserve_proofs.public_address_voted_for[1],"XCA1jh9Nw2XbshVEYsSrnEX5q6WWzHkMaEgRd2TSkrcGckvqoqxoQZejnv31SZPgVjL9uVRvX4NYsPbCLjMdT4m58kmgCnN63n",BUFFER_SIZE) == 0 && invalid_reserve_proofs.reserve_proof_amount[1] == 1000000000)
  {
    color_print("PASSED! Test for check_reserve_proofs_timer_get_database_data","green");
    count_test++;
  }
  else
  {
    color_print(invalid_reserve_proofs.public_address_created_reserve_proof[1],"yellow");
    color_print(invalid_reserve_proofs.public_address_voted_for[1],"yellow");
    fprintf(stderr,"%zu",invalid_reserve_proofs.reserve_proof_amount[1]);
    color_print("FAILED! Test for check_reserve_proofs_timer_get_database_data","red");
  }

  // check_reserve_proofs_timer_update_delegates_total_vote_count
  delete_database(database_name);
  INITIALIZE_DATABASE_DATA(3);
  RESET_INVALID_RESERVE_PROOFS_DATA;
  invalid_reserve_proofs.block_verifier_public_address[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_created_reserve_proof[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_voted_for[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.reserve_proof[0] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));
  invalid_reserve_proofs.block_verifier_public_address[1] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_created_reserve_proof[1] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_voted_for[1] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.reserve_proof[1] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));
  memcpy(invalid_reserve_proofs.block_verifier_public_address[0],TEST_WALLET_6,XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.public_address_voted_for[0],RESERVE_PROOF_PUBLIC_ADDRESS_TEST,XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.public_address_created_reserve_proof[0],RESERVE_PROOF_PUBLIC_ADDRESS_TEST,XCASH_WALLET_LENGTH);
  invalid_reserve_proofs.reserve_proof_amount[0] = 120000000;
  memcpy(invalid_reserve_proofs.reserve_proof[0],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti",537);
  memcpy(invalid_reserve_proofs.block_verifier_public_address[1],RESERVE_PROOF_PUBLIC_ADDRESS_TEST,XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.public_address_voted_for[1],TEST_WALLET_6,XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.public_address_created_reserve_proof[1],TEST_WALLET_11,XCASH_WALLET_LENGTH);
  invalid_reserve_proofs.reserve_proof_amount[1] = 1000000000;
  memcpy(invalid_reserve_proofs.reserve_proof[1],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NTiwbZLCstwWzpKy8J6J8M1uq9qf5PbWha7C68N8V7qshnzUP113UoSQuy78bpD5CeY2H3ViztSqDBFmmMiNu6uCQGkbu4NrZhZk2T7y7T1ixLCAxYfKRtd2pAAbuAWeBVCNzvmKB8YaneMgZ3mH113kXRg67xZRBtVcawf1CTYHCVuhpur8F6nAGxwFeNoRAdr6rnGwa1SUVdqrXypfmJJyzpPUGTSriAJqtRSBKJqAgQDPz4aV99aLMHHKWAv3eKaGAvBZ3obk2GuVBnHeUUEHAihQ5ECndKS4dw8YxMogdY9o8PvrhZgt2gnX4YrUiF5VbGBcBUsxbag1Ak9oZfTH2d1WeyMntcv1Mb28Nzc73UvMbck3VyBedN57gV1LS8WNV1SeHAFBiGgeBcuveGee9VZYQvGRfiWzkJf9JfYWoQ4dyxh13BrXpp3rF1nWv3UXxMrJJX8SRtQy5zrLAywENZ",538);
  invalid_reserve_proofs.count = 2;
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (check_reserve_proofs_timer_update_delegates_total_vote_count(0) == 1 && read_document_field_from_collection(database_name,"delegates","{\"public_address\":\"" RESERVE_PROOF_PUBLIC_ADDRESS_TEST "\"}","total_vote_count",result_test) == 1 && strncmp(result_test,"0",sizeof(result_test)) == 0 && check_reserve_proofs_timer_update_delegates_total_vote_count(1) == 1 && read_document_field_from_collection(database_name,"delegates","{\"public_address\":\"XCA1jh9Nw2XbshVEYsSrnEX5q6WWzHkMaEgRd2TSkrcGckvqoqxoQZejnv31SZPgVjL9uVRvX4NYsPbCLjMdT4m58kmgCnN63n\"}","total_vote_count",result_test) == 1 && strncmp(result_test,"2000000000",sizeof(result_test)) == 0)
  {
    color_print("PASSED! Test for check_reserve_proofs_timer_update_delegates_total_vote_count","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for check_reserve_proofs_timer_update_delegates_total_vote_count","red");
  }

  // check_reserve_proofs_timer_update_delegates_score
  delete_database(database_name);
  INITIALIZE_DATABASE_DATA(3);
  RESET_INVALID_RESERVE_PROOFS_DATA;
  invalid_reserve_proofs.block_verifier_public_address[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_created_reserve_proof[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_voted_for[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.reserve_proof[0] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));
  memcpy(invalid_reserve_proofs.block_verifier_public_address[0],RESERVE_PROOF_PUBLIC_ADDRESS_TEST,XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.public_address_voted_for[0],TEST_WALLET_6,XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.public_address_created_reserve_proof[0],TEST_WALLET_6,XCASH_WALLET_LENGTH);
  invalid_reserve_proofs.reserve_proof_amount[0] = 0;
  memcpy(invalid_reserve_proofs.reserve_proof[0],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr1p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti",537);
  invalid_reserve_proofs.count = 1;
  memset(data_test,0,sizeof(data_test));
  if (check_reserve_proofs_timer_update_delegates_score(0) == 1 && read_document_field_from_collection(database_name,"delegates","{\"public_address\":\"" RESERVE_PROOF_PUBLIC_ADDRESS_TEST "\"}","block_verifier_score",data_test) == 1 && strncmp(data_test,"1",sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for check_reserve_proofs_timer_update_delegates_score","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for check_reserve_proofs_timer_update_delegates_score","red");
  }

  // select_random_unique_reserve_proof
  delete_database(database_name);
  INITIALIZE_DATABASE_DATA(3);
  RESET_INVALID_RESERVE_PROOFS_DATA;
  // initialize the reserve_proof struct
  memset(reserve_proof.block_verifier_public_address,0,sizeof(reserve_proof.block_verifier_public_address));
  memset(reserve_proof.public_address_created_reserve_proof,0,sizeof(reserve_proof.public_address_created_reserve_proof));
  memset(reserve_proof.public_address_voted_for,0,sizeof(reserve_proof.public_address_voted_for));
  memset(reserve_proof.reserve_proof_amount,0,sizeof(reserve_proof.reserve_proof_amount));
  memset(reserve_proof.reserve_proof,0,sizeof(reserve_proof.reserve_proof));
  if (select_random_unique_reserve_proof(&reserve_proof) == 1)
  {
    color_print("PASSED! Test for select_random_unique_reserve_proof","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for select_random_unique_reserve_proof","red");
  }

  // send_invalid_reserve_proof_to_block_verifiers
  delete_database(database_name);
  INITIALIZE_DATABASE_DATA(3);
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
  }
  memcpy(previous_block_verifiers_list.block_verifiers_name[0],"delegate_1",10);
  memcpy(previous_block_verifiers_list.block_verifiers_public_address[0],TEST_WALLET,XCASH_WALLET_LENGTH);
  memcpy(previous_block_verifiers_list.block_verifiers_IP_address[0],XCASH_DPOPS_delegates_IP_address,9);
  memcpy(previous_block_verifiers_list.block_verifiers_public_key[0],NEXT_BLOCK_VERIFIERS_PUBLIC_KEY,VRF_PUBLIC_KEY_LENGTH);
  memcpy(current_block_verifiers_list.block_verifiers_name[0],"delegate_1",10);
  memcpy(current_block_verifiers_list.block_verifiers_public_address[0],TEST_WALLET,XCASH_WALLET_LENGTH);
  memcpy(current_block_verifiers_list.block_verifiers_IP_address[0],XCASH_DPOPS_delegates_IP_address,9);
  memcpy(current_block_verifiers_list.block_verifiers_public_key[0],NEXT_BLOCK_VERIFIERS_PUBLIC_KEY,VRF_PUBLIC_KEY_LENGTH);
  memcpy(next_block_verifiers_list.block_verifiers_name[0],"delegate_1",10);
  memcpy(next_block_verifiers_list.block_verifiers_public_address[0],TEST_WALLET,XCASH_WALLET_LENGTH);
  memcpy(next_block_verifiers_list.block_verifiers_IP_address[0],XCASH_DPOPS_delegates_IP_address,9);
  memcpy(next_block_verifiers_list.block_verifiers_public_key[0],NEXT_BLOCK_VERIFIERS_PUBLIC_KEY,VRF_PUBLIC_KEY_LENGTH);
  // initialize the reserve_proof struct
  memset(reserve_proof.block_verifier_public_address,0,sizeof(reserve_proof.block_verifier_public_address));
  memset(reserve_proof.public_address_created_reserve_proof,0,sizeof(reserve_proof.public_address_created_reserve_proof));
  memset(reserve_proof.public_address_voted_for,0,sizeof(reserve_proof.public_address_voted_for));
  memset(reserve_proof.reserve_proof_amount,0,sizeof(reserve_proof.reserve_proof_amount));
  memset(reserve_proof.reserve_proof,0,sizeof(reserve_proof.reserve_proof));
  memcpy(reserve_proof.block_verifier_public_address,RESERVE_PROOF_PUBLIC_ADDRESS_TEST,XCASH_WALLET_LENGTH);
  memcpy(reserve_proof.public_address_created_reserve_proof,RESERVE_PROOF_PUBLIC_ADDRESS_TEST,XCASH_WALLET_LENGTH);
  memcpy(reserve_proof.public_address_voted_for,RESERVE_PROOF_PUBLIC_ADDRESS_TEST,XCASH_WALLET_LENGTH);
  memcpy(reserve_proof.reserve_proof_amount,"120000000",9);
  memcpy(reserve_proof.reserve_proof,"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5j1ey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed41JUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti",537);
  if (send_invalid_reserve_proof_to_block_verifiers(&reserve_proof) == 1 && strncmp(invalid_reserve_proofs.reserve_proof[0],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5j1ey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed41JUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti",537) == 0)
  {
    color_print("PASSED! Test for send_invalid_reserve_proof_to_block_verifiers","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for send_invalid_reserve_proof_to_block_verifiers","red");
  }


  // check all errors for each test
  // check_reserve_proofs_timer_get_database_data
  delete_database(database_name);
  INITIALIZE_DATABASE_DATA(3);
  RESET_INVALID_RESERVE_PROOFS_DATA;
  invalid_reserve_proofs.block_verifier_public_address[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_created_reserve_proof[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_voted_for[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.reserve_proof[0] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));
  invalid_reserve_proofs.block_verifier_public_address[1] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_created_reserve_proof[1] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_voted_for[1] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.reserve_proof[1] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));
  memcpy(invalid_reserve_proofs.block_verifier_public_address[0],"XCA1jh9Nw2XbshVEYsSrnEX5q6WWzHkMaEgRd2TSkrcGckvqoqxoQZejnv31SZPgVjL9uVRvX4NYsPbCLjMdT4m58kmgCnN63n",XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.public_address_voted_for[0],TEST_WALLET,XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.public_address_created_reserve_proof[0],TEST_WALLET,XCASH_WALLET_LENGTH);
  invalid_reserve_proofs.reserve_proof_amount[0] = 0;
  memcpy(invalid_reserve_proofs.reserve_proof[0],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr1p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti",537);
  memcpy(invalid_reserve_proofs.reserve_proof[1],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NTiwbZLCstwWzpKy8J6J8M1uq9qf5PbWha7C68N8V7qshnzUP113UoSQuy78bpD5CeY2H3ViztSqDBFmmMiNu6uCQGkbu4NrZhZk2T7y7T1ixLCAxYfKRtd2pAAbuAWeBVCNzvmKB8YaneMgZ3mH113kXRg67xZRBtVcawf1CTYHCVuhpur8F6nAGxwFeNoRAdr6rnGwa1SUVdqrXypfmJJyzpPUGTSriAJqtRSBKJqAgQDPz4aV99aLMHHKWAv3eKaGAvBZ3obk2GuVBnHeUUEHAihQ5ECndKS4dw8YxMogdY9o8PvrhZgt2gnX4YrUiF5VbGBcBUsxbag1Ak9oZfTH2d1WeyMntcv1Mb28Nzc73UvMbck3VyBedN57gV1LS8WNV1SeHAFBiGgeBcuveGee9VZYQvGRfiWzkJf9JfYWoQ4dyxh13BrXpp3rF1nWv3UXxMrJJX8SRtQy5zrLAywENZ",538);
  invalid_reserve_proofs.count = 2;
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (check_reserve_proofs_timer_get_database_data(0) == 0)
  {
    color_print("PASSED! Test for check_reserve_proofs_timer_get_database_data for not finding a reserve proof in the database","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for check_reserve_proofs_timer_get_database_data for not finding a reserve proof in the database","red");
  }

  // check_reserve_proofs_timer_update_delegates_total_vote_count
  delete_database(database_name);
  INITIALIZE_DATABASE_DATA(3);
  RESET_INVALID_RESERVE_PROOFS_DATA;
  invalid_reserve_proofs.block_verifier_public_address[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_created_reserve_proof[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_voted_for[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.reserve_proof[0] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));
  invalid_reserve_proofs.block_verifier_public_address[1] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_created_reserve_proof[1] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_voted_for[1] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.reserve_proof[1] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));
  memcpy(invalid_reserve_proofs.block_verifier_public_address[0],TEST_WALLET_6,XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.public_address_voted_for[0],TEST_WALLET,XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.public_address_created_reserve_proof[0],RESERVE_PROOF_PUBLIC_ADDRESS_TEST,XCASH_WALLET_LENGTH);
  invalid_reserve_proofs.reserve_proof_amount[0] = 120000000;
  memcpy(invalid_reserve_proofs.reserve_proof[0],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti",537);
  memcpy(invalid_reserve_proofs.block_verifier_public_address[1],RESERVE_PROOF_PUBLIC_ADDRESS_TEST,XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.public_address_voted_for[1],TEST_WALLET_6,XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.public_address_created_reserve_proof[1],TEST_WALLET_11,XCASH_WALLET_LENGTH);
  invalid_reserve_proofs.reserve_proof_amount[1] = 1000000000;
  memcpy(invalid_reserve_proofs.reserve_proof[1],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NTiwbZLCstwWzpKy8J6J8M1uq9qf5PbWha7C68N8V7qshnzUP113UoSQuy78bpD5CeY2H3ViztSqDBFmmMiNu6uCQGkbu4NrZhZk2T7y7T1ixLCAxYfKRtd2pAAbuAWeBVCNzvmKB8YaneMgZ3mH113kXRg67xZRBtVcawf1CTYHCVuhpur8F6nAGxwFeNoRAdr6rnGwa1SUVdqrXypfmJJyzpPUGTSriAJqtRSBKJqAgQDPz4aV99aLMHHKWAv3eKaGAvBZ3obk2GuVBnHeUUEHAihQ5ECndKS4dw8YxMogdY9o8PvrhZgt2gnX4YrUiF5VbGBcBUsxbag1Ak9oZfTH2d1WeyMntcv1Mb28Nzc73UvMbck3VyBedN57gV1LS8WNV1SeHAFBiGgeBcuveGee9VZYQvGRfiWzkJf9JfYWoQ4dyxh13BrXpp3rF1nWv3UXxMrJJX8SRtQy5zrLAywENZ",538);
  invalid_reserve_proofs.count = 2;
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (check_reserve_proofs_timer_update_delegates_total_vote_count(0) == 0)
  {
    color_print("PASSED! Test for check_reserve_proofs_timer_update_delegates_total_vote_count checking for invalid data","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for check_reserve_proofs_timer_update_delegates_total_vote_count checking for invalid data","red");
  }

  // check_reserve_proofs_timer_update_delegates_score
  delete_database(database_name);
  INITIALIZE_DATABASE_DATA(3);
  RESET_INVALID_RESERVE_PROOFS_DATA;
  invalid_reserve_proofs.block_verifier_public_address[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_created_reserve_proof[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_voted_for[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.reserve_proof[0] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));
  memcpy(invalid_reserve_proofs.block_verifier_public_address[0],TEST_WALLET,XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.public_address_voted_for[0],"XCA1jh9Nw2XbshVEYsSrnEX5q6WWzHkMaEgRd2TSkrcGckvqoqxoQZejnv31SZPgVjL9uVRvX4NYsPbCLjMdT4m58kmgCnN63n",XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.public_address_created_reserve_proof[0],"XCA1jh9Nw2XbshVEYsSrnEX5q6WWzHkMaEgRd2TSkrcGckvqoqxoQZejnv31SZPgVjL9uVRvX4NYsPbCLjMdT4m58kmgCnN63n",XCASH_WALLET_LENGTH);
  invalid_reserve_proofs.reserve_proof_amount[0] = 0;
  memcpy(invalid_reserve_proofs.reserve_proof[0],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr1p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti",537);
  invalid_reserve_proofs.count = 1;
  memset(data_test,0,sizeof(data_test));
  if (check_reserve_proofs_timer_update_delegates_score(0) == 0)
  {
    color_print("PASSED! Test for check_reserve_proofs_timer_update_delegates_score checking for invalid data","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for check_reserve_proofs_timer_update_delegates_score checking for invalid data","red");
  }
  RESET_ERROR_MESSAGES;

  // check_reserve_proofs_timer_update_delegates_total_vote_count
  delete_database(database_name);
  INITIALIZE_DATABASE_DATA(3);
  RESET_INVALID_RESERVE_PROOFS_DATA;
  invalid_reserve_proofs.block_verifier_public_address[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_created_reserve_proof[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_voted_for[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.reserve_proof[0] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));
  invalid_reserve_proofs.block_verifier_public_address[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_created_reserve_proof[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_voted_for[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.reserve_proof[0] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));
  memcpy(invalid_reserve_proofs.block_verifier_public_address[0],"XCA1jh9Nw2XbshVEYsSrnEX5q6WWzHkMaEgRd2TSkrcGckvqoqxoQZejnv31SZPgVjL9uVRvX4NYsPbCLjMdT4m58kmgCnN63n",XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.public_address_voted_for[0],TEST_WALLET,XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.public_address_created_reserve_proof[0],TEST_WALLET,XCASH_WALLET_LENGTH);
  invalid_reserve_proofs.reserve_proof_amount[0] = 0;
  memcpy(invalid_reserve_proofs.reserve_proof[0],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti",537);
  invalid_reserve_proofs.count = 1;
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (check_reserve_proofs_timer_update_delegates_total_vote_count(0) == 0)
  {
    color_print("PASSED! Test for check_reserve_proofs_timer_update_delegates_total_vote_count checking for invalid data","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for check_reserve_proofs_timer_update_delegates_total_vote_count checking for invalid data","red");
  }

  // select_random_unique_reserve_proof
  delete_database(database_name);
  INITIALIZE_DATABASE_DATA(3);
  RESET_INVALID_RESERVE_PROOFS_DATA;
  // initialize the reserve_proof struct
  for (count = 0; count < 20; count++)
  {
    invalid_reserve_proofs.block_verifier_public_address[count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    invalid_reserve_proofs.public_address_created_reserve_proof[count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    invalid_reserve_proofs.public_address_voted_for[count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    invalid_reserve_proofs.reserve_proof[count] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));
  }
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
  memcpy(invalid_reserve_proofs.reserve_proof[9],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NumvXioCcLhBB4dA2Je5APZkDeWBJn9RcF62tG3i8nPtC7BTu1BvGTPNUP1A7WW9AtQyiPC7jknT8MAouNbVSTWckCZVqKDNn4crJdn941APbnUSZLRcba7TEy7urVBPcVR68AxiBaJSpXZigaWb1DAREhJYnaVVdGLsesKY5YBciAmBwJ7rxRaK3pX3GSDKNkHvooGS5wdFQrpss6X4de5CTUdcVhUbHemyon7Aid2LPRTijbaBkxj4jYtbqXq5sddmkKczgxJYbBC1X4Lo6R7SaHckP8D6pvLFwvWwAfx3fiEoxdLK3DEnF3RhC9E7oioDF23EGitxCX21BwgGfUkddAitmuVNsQmLvf96zPZ6VtK3ZYMMBVASS5APv9ZVSsZTeZjPSGajignddM6JMY9Yey9QanNZHYgySrkiAp93T3ihnk2UJ2xmTKwcDTDV2xVyRxRdE8DNQsuCqbZ2Tjw8H",537);
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
  if (select_random_unique_reserve_proof(&reserve_proof) == 0)
  {
    color_print("PASSED! Test for select_random_unique_reserve_proof checking for not finding an unique reserve proof","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for select_random_unique_reserve_proof checking for not finding an unique reserve proof","red");
  }

  // send_invalid_reserve_proof_to_block_verifiers
  delete_database(database_name);
  INITIALIZE_DATABASE_DATA(3);
  RESET_INVALID_RESERVE_PROOFS_DATA;
  // initialize the reserve_proof struct
  invalid_reserve_proofs.block_verifier_public_address[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_created_reserve_proof[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_voted_for[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.reserve_proof[0] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));
  memset(reserve_proof.block_verifier_public_address,0,sizeof(reserve_proof.block_verifier_public_address));
  memset(reserve_proof.public_address_created_reserve_proof,0,sizeof(reserve_proof.public_address_created_reserve_proof));
  memset(reserve_proof.public_address_voted_for,0,sizeof(reserve_proof.public_address_voted_for));
  memset(reserve_proof.reserve_proof_amount,0,sizeof(reserve_proof.reserve_proof_amount));
  memset(reserve_proof.reserve_proof,0,sizeof(reserve_proof.reserve_proof));
  memcpy(reserve_proof.block_verifier_public_address,RESERVE_PROOF_PUBLIC_ADDRESS_TEST,XCASH_WALLET_LENGTH);
  memcpy(reserve_proof.public_address_created_reserve_proof,RESERVE_PROOF_PUBLIC_ADDRESS_TEST,XCASH_WALLET_LENGTH);
  memcpy(reserve_proof.public_address_voted_for,RESERVE_PROOF_PUBLIC_ADDRESS_TEST,XCASH_WALLET_LENGTH);
  memcpy(reserve_proof.reserve_proof_amount,"120000000",9);
  memcpy(reserve_proof.reserve_proof,"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti",537);
  invalid_reserve_proofs.count = 1;
  if (send_invalid_reserve_proof_to_block_verifiers(&reserve_proof) == 1 && strncmp(invalid_reserve_proofs.reserve_proof[0],"",1) == 0)
  {
    color_print("PASSED! Test for send_invalid_reserve_proof_to_block_verifiers checking for an invalid reserve proof","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for send_invalid_reserve_proof_to_block_verifiers checking for an invalid reserve proof","red");
  }

  RESET_INVALID_RESERVE_PROOFS_DATA;
  for (count = 0; count < NETWORK_DATA_NODES_AMOUNT; count++)
  {
    memset(network_data_nodes_list.network_data_nodes_public_address[count],0,sizeof(network_data_nodes_list.network_data_nodes_public_address[count]));
    memset(network_data_nodes_list.network_data_nodes_IP_address[count],0,sizeof(network_data_nodes_list.network_data_nodes_IP_address[count]));
  }
  INITIALIZE_NETWORK_DATA_NODES;

  // write the end test message
  fprintf(stderr,"\033[1;33m\n\n%s\nblock verifiers thread_server functions test - Passed test: %d, Failed test: %d\n%s\n\n\n\033[0m",TEST_OUTLINE,count_test,BLOCK_VERIFIERS_THREAD_SERVER_FUNCTIONS_TOTAL_TEST-count_test,TEST_OUTLINE);
  return count_test;

  #undef RESERVE_PROOF_PUBLIC_ADDRESS_TEST
  #undef RESERVE_PROOF_TEST
  #undef VOTE_DATA_TEST
  #undef RESET_INVALID_RESERVE_PROOFS_DATA
}