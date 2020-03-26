#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "define_macros_test.h"
#include "initialize_and_reset_structs_define_macros.h"

#include "shared_delegate_website_thread_server_functions.h"
#include "insert_database_functions.h"
#include "read_database_functions.h"
#include "delete_database_functions.h"
#include "count_database_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "string_functions.h"
#include "variables_test.h"
#include "shared_delegate_website_thread_server_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: shared_delegate_website_thread_server_functions_test
Description: Test the shared_delegate website functions
Return: The number of passed shared_delegate_website_thread_server_functions test
-----------------------------------------------------------------------------------------------------------
*/

int shared_delegate_website_thread_server_functions_test(void)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  struct voters voters[MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count = 0;
  size_t block_height;
  
  // initialize the voters struct
  INITIALIZE_VOTERS_STRUCT(count,MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE,"shared_delegate_website_thread_server_functions_test",data,current_date_and_time,current_UTC_date_and_time);

  // reset the variables
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memset(data,0,sizeof(data));
  delete_database(database_name);  
  count_test = 0;
  network_functions_test_error_settings = 2;
 
  // write the start test message
  fprintf(stderr,"\033[1;34m%s\nshared_delegate website thread server functions test - Total test: %d\n%s\n\n\033[0m",TEST_OUTLINE,SHARED_DELEGATE_WEBSITE_THREAD_SERVER_FUNCTIONS_TOTAL_TEST,TEST_OUTLINE);
 
  // run the test
  // test add_block_to_blocks_found

  // get the block height of the previous block
  sscanf(current_block_height, "%zu", &block_height);
  block_height--;
  snprintf(result_test,sizeof(result_test)-1,"%zu",block_height);
  memcpy(data_test,"{\"block_height\":\"",17);
  memcpy(data_test+strlen(data_test),result_test,strnlen(result_test,sizeof(data_test)));
  memcpy(data_test+strlen(data_test),"\"}",2);

  if (add_block_to_blocks_found() != 0 && count_documents_in_collection(database_name,"blocks_found",data_test) == 1)
  {
    color_print("PASSED! Test for add_block_to_blocks_found","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for add_block_to_blocks_found","red");
  }
  RESET_ERROR_MESSAGES;

  // test get_delegates_total_voters_count
  delete_database(database_name);
  memset(data_test,0,sizeof(data_test));
  memcpy(data_test,"{\"public_address_created_reserve_proof\":\"" TEST_WALLET_1 "\",\"public_address_voted_for\":\"",169);
  memcpy(data_test+strlen(data_test),xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(data_test+strlen(data_test),"\",\"total\":\"10000000000000\",\"reserve_proof\":\"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti\"}",583);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",data_test);
  memset(data_test,0,sizeof(data_test));
  memcpy(data_test,"{\"public_address_created_reserve_proof\":\"" TEST_WALLET_20 "\",\"public_address_voted_for\":\"",169);
  memcpy(data_test+strlen(data_test),xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(data_test+strlen(data_test),"\",\"total\":\"20000000000000\",\"reserve_proof\":\"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti\"}",583);
  insert_document_into_collection_json(database_name,"reserve_proofs_2",data_test);
  memset(data_test,0,sizeof(data_test));
  memcpy(data_test,"{\"public_address_created_reserve_proof\":\"" TEST_WALLET_19 "\",\"public_address_voted_for\":\"",169);
  memcpy(data_test+strlen(data_test),xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(data_test+strlen(data_test),"\",\"total\":\"15000000000000\",\"reserve_proof\":\"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti\"}",583);
  insert_document_into_collection_json(database_name,"reserve_proofs_3",data_test);
  memset(data_test,0,sizeof(data_test));
  memcpy(data_test,"{\"public_address_created_reserve_proof\":\"" TEST_WALLET_18 "\",\"public_address_voted_for\":\"",169);
  memcpy(data_test+strlen(data_test),xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(data_test+strlen(data_test),"\",\"total\":\"5529141006200\",\"reserve_proof\":\"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti\"}",582);
  insert_document_into_collection_json(database_name,"reserve_proofs_4",data_test);
  memset(data_test,0,sizeof(data_test));
  memcpy(data_test,"{\"public_address_created_reserve_proof\":\"" TEST_WALLET_15 "\",\"public_address_voted_for\":\"",169);
  memcpy(data_test+strlen(data_test),xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(data_test+strlen(data_test),"\",\"total\":\"2000000000000\",\"reserve_proof\":\"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti\"}",582);
  insert_document_into_collection_json(database_name,"reserve_proofs_5",data_test);
  memset(data_test,0,sizeof(data_test));
  memcpy(data_test,"{\"public_address_created_reserve_proof\":\"" TEST_WALLET_1 "\",\"public_address_voted_for\":\"",169);
  memcpy(data_test+strlen(data_test),TEST_WALLET,XCASH_WALLET_LENGTH);
  memcpy(data_test+strlen(data_test),"\",\"total\":\"10000000000000\",\"reserve_proof\":\"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti\"}",583);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",data_test);
  memset(data_test,0,sizeof(data_test));
  memcpy(data_test,"{\"public_address_created_reserve_proof\":\"" TEST_WALLET_20 "\",\"public_address_voted_for\":\"",169);
  memcpy(data_test+strlen(data_test),TEST_WALLET,XCASH_WALLET_LENGTH);
  memcpy(data_test+strlen(data_test),"\",\"total\":\"20000000000000\",\"reserve_proof\":\"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti\"}",583);
  insert_document_into_collection_json(database_name,"reserve_proofs_2",data_test);
  memset(data_test,0,sizeof(data_test));
  memcpy(data_test,"{\"public_address_created_reserve_proof\":\"" TEST_WALLET_19 "\",\"public_address_voted_for\":\"",169);
  memcpy(data_test+strlen(data_test),TEST_WALLET,XCASH_WALLET_LENGTH);
  memcpy(data_test+strlen(data_test),"\",\"total\":\"15000000000000\",\"reserve_proof\":\"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti\"}",583);
  insert_document_into_collection_json(database_name,"reserve_proofs_3",data_test);
  memset(data_test,0,sizeof(data_test));
  memcpy(data_test,"{\"public_address_created_reserve_proof\":\"" TEST_WALLET_18 "\",\"public_address_voted_for\":\"",169);
  memcpy(data_test+strlen(data_test),TEST_WALLET,XCASH_WALLET_LENGTH);
  memcpy(data_test+strlen(data_test),"\",\"total\":\"5529141006200\",\"reserve_proof\":\"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti\"}",582);
  insert_document_into_collection_json(database_name,"reserve_proofs_4",data_test);
  memset(data_test,0,sizeof(data_test));
  memcpy(data_test,"{\"public_address_created_reserve_proof\":\"" TEST_WALLET_15 "\",\"public_address_voted_for\":\"",169);
  memcpy(data_test+strlen(data_test),TEST_WALLET,XCASH_WALLET_LENGTH);
  memcpy(data_test+strlen(data_test),"\",\"total\":\"2000000000000\",\"reserve_proof\":\"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti\"}",582);
  insert_document_into_collection_json(database_name,"reserve_proofs_5",data_test);
  if (get_delegates_total_voters_count(xcash_wallet_public_address) == 5)
  {
    color_print("PASSED! Test for get_delegates_total_voters_count","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for get_delegates_total_voters_count","red");
  }

  // get_delegates_total_voters
  if (get_delegates_total_voters(voters) == 52529141006200)
  {
    color_print("PASSED! Test for get_delegates_total_voters","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for get_delegates_total_voters","red");
  }
  
  // test calculate_block_reward_for_each_delegate
  // add some of the voters already to the database and let some of them be added on the first block reward they receive
  insert_document_into_collection_json(database_name,"public_addresses","{\"public_address\":\"" TEST_WALLET_1 "\",\"current_total\":\"5000000000000\",\"total\":\"0\",\"inactivity_count\":\"10\"}");
  insert_document_into_collection_json(database_name,"public_addresses","{\"public_address\":\"" TEST_WALLET_20 "\",\"current_total\":\"0\",\"total\":\"10000000000000\",\"inactivity_count\":\"0\"}");
  memset(data_test,0,sizeof(data_test));
  memset(result_test,0,sizeof(result_test));
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));  
  if (calculate_block_reward_for_each_delegate(1000000000000) == 1 && read_document_field_from_collection(database_name,"public_addresses","{\"public_address\":\"" TEST_WALLET_1 "\"}","current_total",data_test) == 1 && read_document_field_from_collection(database_name,"public_addresses","{\"public_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\"}","current_total",result_test) == 1 && read_document_field_from_collection(database_name,"public_addresses","{\"public_address\":\"XCA1qbzqCoX9WGsqjYgKMGH2eFjkvBRmsb2HKiumrKbjMVDLhukUShsgjFbT6osQBA3TaL9hD7wr8dW2Zq5QzRyU1mnQAQwTpJ\"}","current_total",data) == 1 && read_document_field_from_collection(database_name,"public_addresses","{\"public_address\":\"XCA1VXWDkYm2u56cbe6b3VajEe4qFLGhVgxeNcgnA3ht1gWvrnZTYr8PJtpkWzagR7c76dDdeyFgoeKZj55W78bp4T8L8n6wLW\"}","current_total",data2) == 1 && read_document_field_from_collection(database_name,"public_addresses","{\"public_address\":\"XCA1dpsUrcgZvnZYLjXUbcbxSSJqhNWyphoqURumeeKoE8BXQeKqep8QQSk2fd65at2yspnT3z7KVRTU3MFmVS8u7K89wrKVxu\"}","current_total",data3) == 1 && strncmp(data_test,"5190370522122",sizeof(data_test)) == 0 && strncmp(result_test,"380741044245",sizeof(result_test)) == 0 && strncmp(data,"285555783183",sizeof(data)) == 0 && strncmp(data2,"105258546023",sizeof(data2)) == 0 && strncmp(data3,"38074104424",sizeof(data3)) == 0)
  {
    color_print("PASSED! Test for calculate_block_reward_for_each_delegate","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for calculate_block_reward_for_each_delegate","red");
  }
  RESET_ERROR_MESSAGES;

  // payment_timer_send_payment_and_update_databases
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"public_addresses","{\"public_address\":\"" TEST_WALLET_1 "\",\"current_total\":\"5000000000000\",\"total\":\"10000000000000\",\"inactivity_count\":\"10\"}");
  insert_document_into_collection_json(database_name,"public_addresses","{\"public_address\":\"" TEST_WALLET_20 "\",\"current_total\":\"0\",\"total\":\"10000000000000\",\"inactivity_count\":\"0\"}");
  memset(data_test,0,sizeof(data_test));
  memset(result_test,0,sizeof(result_test));
  memset(data,0,sizeof(data)); 
  if (payment_timer_send_payment_and_update_databases(TEST_WALLET_1,"5000000000000","10000000000000") == 5000000000000 && read_document_field_from_collection(database_name,"public_addresses","{\"public_address\":\"" TEST_WALLET_1 "\"}","current_total",data_test) == 1 && read_document_field_from_collection(database_name,"public_addresses","{\"public_address\":\"" TEST_WALLET_1 "\"}","total",result_test) == 1 && strncmp(data_test,"0",sizeof(data_test)) == 0 && strncmp(result_test,"15000000000000",sizeof(result_test)) == 0 && read_document_field_from_collection(database_name,"public_addresses_payments","{\"public_address\":\"" TEST_WALLET_1 "\"}","total",data) == 1 && strncmp(data,"5000000000000",sizeof(data)) == 0)
  {
    color_print("PASSED! Test for payment_timer_send_payment_and_update_databases","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for payment_timer_send_payment_and_update_databases","red");
  }
  RESET_ERROR_MESSAGES;

  // payment_timer_update_inactivity_count
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"public_addresses","{\"public_address\":\"" TEST_WALLET_1 "\",\"current_total\":\"0\",\"total\":\"10000000000000\",\"inactivity_count\":\"0\"}");
  memset(data_test,0,sizeof(data_test));
  if (payment_timer_update_inactivity_count(TEST_WALLET_1,"0","0") == 1 && read_document_field_from_collection(database_name,"public_addresses","{\"public_address\":\"" TEST_WALLET_1 "\"}","inactivity_count",data_test) == 1 && strncmp(data_test,"1",sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for payment_timer_update_inactivity_count checking for a current total of 0 and an increment of the inactivity_count","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for payment_timer_update_inactivity_count checking for a current total of 0 and an increment of the inactivity_count","red");
  }
  RESET_ERROR_MESSAGES;

  delete_database(database_name);
  insert_document_into_collection_json(database_name,"public_addresses","{\"public_address\":\"" TEST_WALLET_1 "\",\"current_total\":\"5000000000000\",\"total\":\"10000000000000\",\"inactivity_count\":\"10\"}");
  memset(data_test,0,sizeof(data_test));
  if (payment_timer_update_inactivity_count(TEST_WALLET_1,"5000000000000","10") == 1)
  {
    color_print("PASSED! Test for payment_timer_update_inactivity_count checking for a current total not equal to 0 and a reset of the inactivity_count","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for payment_timer_update_inactivity_count checking for a current total not equal to 0 and a reset of the inactivity_count","red");
  }
  RESET_ERROR_MESSAGES;

  delete_database(database_name);
  insert_document_into_collection_json(database_name,"public_addresses","{\"public_address\":\"" TEST_WALLET_1 "\",\"current_total\":\"0\",\"total\":\"10000000000000\",\"inactivity_count\":\"31\"}");
  memset(data_test,0,sizeof(data_test));
  if (payment_timer_update_inactivity_count(TEST_WALLET_1,"0","31") == 1 && count_all_documents_in_collection(database_name,"public_addresses") == 0)
  {
    color_print("PASSED! Test for payment_timer_update_inactivity_count checking for a current total of 0 and for a maximum inactivity count and removing the user from the database","green");
    count_test++;
  }
  else
  {
    fprintf(stderr,"%d",count_all_documents_in_collection(database_name,"public_addresses"));
    color_print("FAILED! Test for payment_timer_update_inactivity_count checking for a current total of 0 and for a maximum inactivity count and removing the user from the database","red");
  }
  RESET_ERROR_MESSAGES;




  // check all errors for each test
  delete_database(database_name);
  POINTER_RESET_VOTERS_STRUCT(count,MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE);
  RESET_ERROR_MESSAGES;
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));

  if (add_block_to_blocks_found() == 0)
  {
    for (count = 0; count < error_message.total; count++)
    {
      if (strncmp(error_message.data[count],"The database is empty",BUFFER_SIZE) == 0)
      {
        break;
      }
    }
  }  
  if (count != error_message.total)
  {
    color_print("PASSED! Test for add_block_to_blocks_found checking for The database is empty","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for add_block_to_blocks_found checking for The database is empty","red");
  }
  RESET_ERROR_MESSAGES;

  if (calculate_block_reward_for_each_delegate(100) == 0)
  {
    for (count = 0; count < error_message.total; count++)
    {
      if (strncmp(error_message.data[count],"The delegate does not have enough voters to distribute the block rewards with",BUFFER_SIZE) == 0)
      {
        break;
      }
    }
  }  
  if (count != error_message.total)
  {
    color_print("PASSED! Test for calculate_block_reward_for_each_delegate checking for The delegate does not have enough voters to distribute the block rewards with","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for calculate_block_reward_for_each_delegate checking for The delegate does not have enough voters to distribute the block rewards with","red");
  }
  delete_database(database_name);
  RESET_ERROR_MESSAGES;

  POINTER_RESET_VOTERS_STRUCT(count,MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE);

  // write the end test message
  fprintf(stderr,"\033[1;33m\n\n%s\nshared_delegate website thread server functions test - Passed test: %d, Failed test: %d\n%s\n\n\n\033[0m",TEST_OUTLINE,count_test,SHARED_DELEGATE_WEBSITE_THREAD_SERVER_FUNCTIONS_TOTAL_TEST-count_test,TEST_OUTLINE);
  return count_test;
}