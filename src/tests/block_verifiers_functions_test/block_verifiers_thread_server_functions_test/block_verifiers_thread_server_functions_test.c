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
  int count;
  
  // define macros
  #define BLOCK_VERIFIERS_THREAD_SERVER_FUNCTIONS_TOTAL_TEST 8
  #define RESERVE_PROOF_PUBLIC_ADDRESS_TEST "XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP"
  #define RESERVE_PROOF_TEST "ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB31qaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti"
  #define VOTE_DATA_TEST "\"vote_data\": \"59afe2f27423efdc1f5a7acc15fa5b76b6553b36886e9f32b0b7ab3baf5e77a0222a855957e5b9a6d4e174c8bd0b405c0eef42c6be1776e32f0f742aa1c125e5\""
  #define RESET_INVALID_RESERVE_PROOFS \
  for (count = 0; count <= invalid_reserve_proofs.count; count++) \
  { \
    memset(invalid_reserve_proofs.public_address_voted_for[count],0,strlen(invalid_reserve_proofs.public_address_voted_for[count])); \
    memset(invalid_reserve_proofs.public_address_created_reserve_proof[count],0,strlen(invalid_reserve_proofs.public_address_created_reserve_proof[count])); \
    memset(invalid_reserve_proofs.reserve_proof[count],0,strlen(invalid_reserve_proofs.reserve_proof[count])); \
  } \
  invalid_reserve_proofs.count = 0; \

  // reset the variables
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  count_test = 0;
  network_functions_test_error_settings = 2;
  delete_database(database_name,0);
  RESET_ERROR_MESSAGES;
 
  // write the start test message
  fprintf(stderr,"\033[1;34m%s\nblock verifiers thread_server functions test - Total test: %d\n%s\n\n\033[0m",TEST_OUTLINE,BLOCK_VERIFIERS_THREAD_SERVER_FUNCTIONS_TOTAL_TEST,TEST_OUTLINE);
 
  // run the test
  // sync_reserve_proofs_database
  INITIALIZE_NETWORK_DATA_NODES_TEST;
  INITIALIZE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_TEST;
  insert_document_into_collection_json(database_name,"reserve_proofs_1",RESERVE_PROOFS_TEST_DATA,0);
  insert_document_into_collection_json(database_name,"reserve_bytes_1","{\"message_settings\": \"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATABASE_HASH\",\"block_height\": \"" XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT_TEST "\"}",0);
  insert_document_into_collection_json(database_name,"delegates",DELEGATES_TEST_DATA,0);
  insert_document_into_collection_json(database_name,"statistics",DATABASE_COLLECTION_STATISTICS_DATA,0);
  RESET_INVALID_RESERVE_PROOFS;
  RESET_ERROR_MESSAGES;

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
  delete_database(database_name,0);
  INITIALIZE_DATABASE_DATA(2);
  RESET_INVALID_RESERVE_PROOFS;
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
    color_print("FAILED! Test for check_reserve_proofs_timer_get_database_data","red");
  }

  // check_reserve_proofs_timer_update_delegates_total_vote_count
  delete_database(database_name,0);
  INITIALIZE_DATABASE_DATA(2);
  RESET_INVALID_RESERVE_PROOFS;
  memcpy(invalid_reserve_proofs.public_address_voted_for[0],"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP",XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.public_address_created_reserve_proof[0],"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP",XCASH_WALLET_LENGTH);
  invalid_reserve_proofs.reserve_proof_amount[0] = 120000000;
  memcpy(invalid_reserve_proofs.reserve_proof[0],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti",537);
  memcpy(invalid_reserve_proofs.public_address_voted_for[1],"XCA1jh9Nw2XbshVEYsSrnEX5q6WWzHkMaEgRd2TSkrcGckvqoqxoQZejnv31SZPgVjL9uVRvX4NYsPbCLjMdT4m58kmgCnN63n",XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.public_address_created_reserve_proof[1],"XCA1oPZcuxH1WeyMntcv1Mb28Nzc73UvMbck3VyBedN57gV1c9ikU6tBALSjCwigMs1XCDtTYND7vfELU31PQEBh6zs5MLxYek",XCASH_WALLET_LENGTH);
  invalid_reserve_proofs.reserve_proof_amount[1] = 1000000000;
  memcpy(invalid_reserve_proofs.reserve_proof[1],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NTiwbZLCstwWzpKy8J6J8M1uq9qf5PbWha7C68N8V7qshnzUP113UoSQuy78bpD5CeY2H3ViztSqDBFmmMiNu6uCQGkbu4NrZhZk2T7y7T1ixLCAxYfKRtd2pAAbuAWeBVCNzvmKB8YaneMgZ3mH113kXRg67xZRBtVcawf1CTYHCVuhpur8F6nAGxwFeNoRAdr6rnGwa1SUVdqrXypfmJJyzpPUGTSriAJqtRSBKJqAgQDPz4aV99aLMHHKWAv3eKaGAvBZ3obk2GuVBnHeUUEHAihQ5ECndKS4dw8YxMogdY9o8PvrhZgt2gnX4YrUiF5VbGBcBUsxbag1Ak9oZfTH2d1WeyMntcv1Mb28Nzc73UvMbck3VyBedN57gV1LS8WNV1SeHAFBiGgeBcuveGee9VZYQvGRfiWzkJf9JfYWoQ4dyxh13BrXpp3rF1nWv3UXxMrJJX8SRtQy5zrLAywENZ",538);
  invalid_reserve_proofs.count = 2;
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (check_reserve_proofs_timer_update_delegates_total_vote_count(0) == 1 && read_document_field_from_collection(database_name,"delegates","{\"public_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\"}","total_vote_count",result_test,1) == 1 && strncmp(result_test,"0",sizeof(result_test)) == 0 && check_reserve_proofs_timer_update_delegates_total_vote_count(1) == 1 && read_document_field_from_collection(database_name,"delegates","{\"public_address\":\"XCA1jh9Nw2XbshVEYsSrnEX5q6WWzHkMaEgRd2TSkrcGckvqoqxoQZejnv31SZPgVjL9uVRvX4NYsPbCLjMdT4m58kmgCnN63n\"}","total_vote_count",result_test,1) == 1 && strncmp(result_test,"2000000000",sizeof(result_test)) == 0)
  {
    color_print("PASSED! Test for check_reserve_proofs_timer_update_delegates_total_vote_count","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for check_reserve_proofs_timer_update_delegates_total_vote_count","red");
  }

  // check_reserve_proofs_timer_update_delegates_score
  delete_database(database_name,0);
  INITIALIZE_DATABASE_DATA(2);
  RESET_INVALID_RESERVE_PROOFS;
  memcpy(invalid_reserve_proofs.block_verifier_public_address[0],"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP",XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.public_address_voted_for[0],"XCA1jh9Nw2XbshVEYsSrnEX5q6WWzHkMaEgRd2TSkrcGckvqoqxoQZejnv31SZPgVjL9uVRvX4NYsPbCLjMdT4m58kmgCnN63n",XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.public_address_created_reserve_proof[0],"XCA1jh9Nw2XbshVEYsSrnEX5q6WWzHkMaEgRd2TSkrcGckvqoqxoQZejnv31SZPgVjL9uVRvX4NYsPbCLjMdT4m58kmgCnN63n",XCASH_WALLET_LENGTH);
  invalid_reserve_proofs.reserve_proof_amount[0] = 0;
  memcpy(invalid_reserve_proofs.reserve_proof[0],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr1p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti",537);
  invalid_reserve_proofs.count = 1;
  memset(data_test,0,sizeof(data_test));
  if (check_reserve_proofs_timer_update_delegates_score(0) == 1 && read_document_field_from_collection(database_name,"delegates","{\"public_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\"}","block_verifier_score",data_test,1) == 1 && strncmp(data_test,"1",sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for check_reserve_proofs_timer_update_delegates_score","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for check_reserve_proofs_timer_update_delegates_score","red");
  }

  // check_reserve_proofs_timer_update_database
  delete_database(database_name,0);
  INITIALIZE_DATABASE_DATA(2);
  RESET_INVALID_RESERVE_PROOFS;
  memcpy(invalid_reserve_proofs.public_address_voted_for[0],"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP",XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.public_address_created_reserve_proof[0],"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP",XCASH_WALLET_LENGTH);
  invalid_reserve_proofs.reserve_proof_amount[0] = 120000000;
  memcpy(invalid_reserve_proofs.reserve_proof[0],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti",537);
  memcpy(invalid_reserve_proofs.public_address_voted_for[1],"XCA1jh9Nw2XbshVEYsSrnEX5q6WWzHkMaEgRd2TSkrcGckvqoqxoQZejnv31SZPgVjL9uVRvX4NYsPbCLjMdT4m58kmgCnN63n",XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.public_address_created_reserve_proof[1],"XCA1oPZcuxH1WeyMntcv1Mb28Nzc73UvMbck3VyBedN57gV1c9ikU6tBALSjCwigMs1XCDtTYND7vfELU31PQEBh6zs5MLxYek",XCASH_WALLET_LENGTH);
  invalid_reserve_proofs.reserve_proof_amount[1] = 1000000000;
  memcpy(invalid_reserve_proofs.reserve_proof[1],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NTiwbZLCstwWzpKy8J6J8M1uq9qf5PbWha7C68N8V7qshnzUP113UoSQuy78bpD5CeY2H3ViztSqDBFmmMiNu6uCQGkbu4NrZhZk2T7y7T1ixLCAxYfKRtd2pAAbuAWeBVCNzvmKB8YaneMgZ3mH113kXRg67xZRBtVcawf1CTYHCVuhpur8F6nAGxwFeNoRAdr6rnGwa1SUVdqrXypfmJJyzpPUGTSriAJqtRSBKJqAgQDPz4aV99aLMHHKWAv3eKaGAvBZ3obk2GuVBnHeUUEHAihQ5ECndKS4dw8YxMogdY9o8PvrhZgt2gnX4YrUiF5VbGBcBUsxbag1Ak9oZfTH2d1WeyMntcv1Mb28Nzc73UvMbck3VyBedN57gV1LS8WNV1SeHAFBiGgeBcuveGee9VZYQvGRfiWzkJf9JfYWoQ4dyxh13BrXpp3rF1nWv3UXxMrJJX8SRtQy5zrLAywENZ",538);
  invalid_reserve_proofs.count = 2;
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  if (check_reserve_proofs_timer_update_database() == 1)
  {
    color_print("PASSED! Test for check_reserve_proofs_timer_update_database","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for check_reserve_proofs_timer_update_database","red");
  }



  // check all errors for each test
  // check_reserve_proofs_timer_get_database_data
  delete_database(database_name,0);
  INITIALIZE_DATABASE_DATA(2);
  RESET_INVALID_RESERVE_PROOFS;
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

  // check_reserve_proofs_timer_update_database
  delete_database(database_name,0);
  INITIALIZE_DATABASE_DATA(2);
  RESET_INVALID_RESERVE_PROOFS;
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


  

  for (count = 0; count < NETWORK_DATA_NODES_AMOUNT; count++)
  {
    memset(network_data_nodes_list.network_data_nodes_public_address[count],0,sizeof(network_data_nodes_list.network_data_nodes_public_address[count]));
    memset(network_data_nodes_list.network_data_nodes_IP_address[count],0,sizeof(network_data_nodes_list.network_data_nodes_IP_address[count]));
  }
  INITIALIZE_NETWORK_DATA_NODES

  // write the end test message
  fprintf(stderr,"\033[1;33m\n\n%s\nblock verifiers thread_server functions test - Passed test: %d, Failed test: %d\n%s\n\n\n\033[0m",TEST_OUTLINE,count_test,BLOCK_VERIFIERS_THREAD_SERVER_FUNCTIONS_TOTAL_TEST-count_test,TEST_OUTLINE);
  return count_test;

  #undef BLOCK_VERIFIERS_THREAD_SERVER_FUNCTIONS_TOTAL_TEST
}