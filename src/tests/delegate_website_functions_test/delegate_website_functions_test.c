#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "define_macros_test.h"

#include "insert_database_functions.h"
#include "delete_database_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "string_functions.h"
#include "variables_test.h"
#include "delegate_website_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: delegate_website_functions_test
Description: Test the delegate website functions
Return: The number of passed delegate_website_functions test
-----------------------------------------------------------------------------------------------------------
*/

int delegate_website_functions_test(void)
{
  // Variables
  char data[BUFFER_SIZE];
  int count;

  // define macros
  #define DELEGATE_WEBSITE_FUNCTIONS_TOTAL_TEST 18
  #define DELEGATE_WEBSITE_GET_STATISTICS_TEST_DATA "{\"most_total_rounds_delegate_name\":\"delegate_name_5\",\"most_total_rounds\":\"5\",\"best_block_verifier_online_percentage_delegate_name\":\"delegate_name_2\",\"best_block_verifier_online_percentage\":\"100\",\"most_block_producer_total_rounds_delegate_name\":\"delegate_name_4\",\"most_block_producer_total_rounds\":\"2\",\"current_block_height\":\"521850\",\"XCASH_DPOPS_round_number\":\"5\",\"total_votes\":\"380000000\",\"XCASH_DPOPS_circulating_percentage\":\"0\"}"
  #define DELEGATE_WEBSITE_GET_DELEGATES_TEST_DATA "[{\"total_vote_count\":\"200000000\",\"delegate_name\":\"delegate_name_5\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"5\",\"block_verifier_online_percentage\":\"0\"},{\"total_vote_count\":\"100000000\",\"delegate_name\":\"delegate_name_4\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"4\",\"block_verifier_online_percentage\":\"25\"},{\"total_vote_count\":\"50000000\",\"delegate_name\":\"delegate_name_3\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"3\",\"block_verifier_online_percentage\":\"33\"},{\"total_vote_count\":\"20000000\",\"delegate_name\":\"delegate_name_2\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"2\",\"block_verifier_online_percentage\":\"100\"},{\"total_vote_count\":\"10000000\",\"delegate_name\":\"delegate_name_1\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"1\",\"block_verifier_online_percentage\":\"0\"}]"
  #define DELEGATE_WEBSITE_GET_DELEGATES_STATISTICS_TEST_DATA "{\"public_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"total_vote_count\":\"10000000\",\"delegate_name\":\"delegate_name_1\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"server_settings\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"1\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"current_delegate_rank\":\"5\"}"
  #define DELEGATE_WEBSITE_GET_DELEGATES_INFORMATION_TEST_DATA "{\"public_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"server_settings\":\"\"}"
  #define DELEGATE_WEBSITE_GET_DELEGATES_VOTERS_LIST_TEST_DATA "[{\"public_address_created_reserve_proof\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"public_address_voted_for\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"total\":\"120000000\",\"reserve_proof\":\"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti\"},{\"public_address_created_reserve_proof\":\"XCA1v18Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3\",\"public_address_voted_for\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"total\":\"200000000\",\"reserve_proof\":\"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti\"}]"
  #define DELEGATE_WEBSITE_GET_ROUND_STATISTICS_TEST_DATA "{\"reserve_bytes\":\"DATA\"}"

  memset(database_name,0,sizeof(database_name));
  memcpy(database_name,DATABASE_NAME_TEST,sizeof(DATABASE_NAME_TEST)-1);
  delegates_website = 1;

  // reset the variables
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memset(data,0,sizeof(data));
  count_test = 0;
  network_functions_test_settings = 1;
  network_functions_test_error_settings = 2;

  delete_database(database_name,0);
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"total_vote_count\":\"10000000\",\"IP_address\":\"192.168.1.201\",\"delegate_name\":\"delegate_name_1\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"server_settings\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"1\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}",1);
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v\",\"total_vote_count\":\"20000000\",\"IP_address\":\"192.168.1.201\",\"delegate_name\":\"delegate_name_2\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"server_settings\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"2\",\"block_verifier_online_total_rounds\":\"2\",\"block_verifier_online_percentage\":\"100\",\"block_producer_total_rounds\":\"1\",\"block_producer_block_heights\":\"\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}",1);
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"XCA1qbzqCoX9WGsqjYgKMGH2eFjkvBRmsb2HKiumrKbjMVDLhukUShsgjFbT6osQBA3TaL9hD7wr8dW2Zq5QzRyU1mnQAQwTpJ\",\"total_vote_count\":\"50000000\",\"IP_address\":\"192.168.1.201\",\"delegate_name\":\"delegate_name_3\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"server_settings\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"3\",\"block_verifier_online_total_rounds\":\"1\",\"block_verifier_online_percentage\":\"33\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}",1);
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"XCA1VXWDkYm2u56cbe6b3VajEe4qFLGhVgxeNcgnA3ht1gWvrnZTYr8PJtpkWzagR7c76dDdeyFgoeKZj55W78bp4T8L8n6wLW\",\"total_vote_count\":\"100000000\",\"IP_address\":\"192.168.1.201\",\"delegate_name\":\"delegate_name_4\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"server_settings\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"4\",\"block_verifier_online_total_rounds\":\"1\",\"block_verifier_online_percentage\":\"25\",\"block_producer_total_rounds\":\"2\",\"block_producer_block_heights\":\"\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}",1);
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"XCA1dpsUrcgZvnZYLjXUbcbxSSJqhNWyphoqURumeeKoE8BXQeKqep8QQSk2fd65at2yspnT3z7KVRTU3MFmVS8u7K89wrKVxu\",\"total_vote_count\":\"200000000\",\"IP_address\":\"192.168.1.201\",\"delegate_name\":\"delegate_name_5\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"server_settings\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"5\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}",1);
  insert_document_into_collection_json(database_name,"statistics","{\"username\":\"XCASH\",\"most_total_rounds_delegate_name\":\"delegate_name_5\",\"most_total_rounds\":\"5\",\"best_block_verifier_online_percentage_delegate_name\":\"delegate_name_2\",\"best_block_verifier_online_percentage\":\"100\",\"most_block_producer_total_rounds_delegate_name\":\"delegate_name_4\",\"most_block_producer_total_rounds\":\"2\"}",1);
  insert_document_into_collection_json(database_name,"reserve_proofs_1","{\"public_address_created_reserve_proof\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"public_address_voted_for\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"total\":\"120000000\",\"reserve_proof\":\"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti\"}",0);
  insert_document_into_collection_json(database_name,"reserve_proofs_1","{\"public_address_created_reserve_proof\":\"" TEST_WALLET "\",\"public_address_voted_for\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"total\":\"200000000\",\"reserve_proof\":\"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti\"}",0);
  insert_document_into_collection_json(database_name,"reserve_bytes_1",RESERVE_BYTES_TEST_DATA,0);
  RESET_ERROR_MESSAGES;
 
  // write the start test message
  fprintf(stderr,"\033[1;34m%s\ndelegate website functions test - Total test: %d\n%s\n\n\033[0m",TEST_OUTLINE,DELEGATE_WEBSITE_FUNCTIONS_TOTAL_TEST,TEST_OUTLINE);
 
  // run the test
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,"127.0.0.1",SEND_DATA_PORT,SERVER_RECEIVE_DATA_SOCKET_DELEGATES_WEBSITE_GET_STATISTICS_TEST_DATA,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS,"",0) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,DELEGATE_WEBSITE_GET_STATISTICS_TEST_DATA,sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for server_receive_data_socket_delegates_website_get_statistics","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for server_receive_data_socket_delegates_website_get_statistics","red");
  }
  RESET_ERROR_MESSAGES;

  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,"127.0.0.1",SEND_DATA_PORT,SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_TEST_DATA,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS,"",0) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,DELEGATE_WEBSITE_GET_DELEGATES_TEST_DATA,sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for server_receive_data_socket_get_delegates","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for server_receive_data_socket_get_delegates","red");
  }
  RESET_ERROR_MESSAGES;

  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,"127.0.0.1",SEND_DATA_PORT,SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_STATISTICS_TEST_DATA,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS,"",0) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,DELEGATE_WEBSITE_GET_DELEGATES_STATISTICS_TEST_DATA,sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for server_receive_data_socket_get_delegates_statistics","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for server_receive_data_socket_get_delegates_statistics","red");
  }
  RESET_ERROR_MESSAGES;

  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,"127.0.0.1",SEND_DATA_PORT,SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_INFORMATION_TEST_DATA,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS,"",0) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,DELEGATE_WEBSITE_GET_DELEGATES_INFORMATION_TEST_DATA,sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for server_receive_data_socket_get_delegates_information","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for server_receive_data_socket_get_delegates_information","red");
  }
  RESET_ERROR_MESSAGES;

  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,"127.0.0.1",SEND_DATA_PORT,SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_VOTERS_LIST_TEST_DATA,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS,"",0) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,DELEGATE_WEBSITE_GET_DELEGATES_VOTERS_LIST_TEST_DATA,sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for server_receive_data_socket_get_delegates_voters_list","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for server_receive_data_socket_get_delegates_voters_list","red");
  }
  RESET_ERROR_MESSAGES;

  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,"127.0.0.1",SEND_DATA_PORT,SERVER_RECEIVE_DATA_SOCKET_GET_ROUND_STATISTICS_TEST_DATA,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS,"",0) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,DELEGATE_WEBSITE_GET_ROUND_STATISTICS_TEST_DATA,sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for server_receive_data_socket_get_round_statistics","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for server_receive_data_socket_get_round_statistics","red");
  }
  RESET_ERROR_MESSAGES;
  delete_database(DATABASE_NAME_TEST,0);

  

  // check all errors for each test
  delete_database(database_name,0);
  RESET_ERROR_MESSAGES;
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));

  // server_receive_data_socket_delegates_website_get_statistics
  CHECK_WEBSITE_SERVER_FUNCTIONS_ERROR(SERVER_RECEIVE_DATA_SOCKET_DELEGATES_WEBSITE_GET_STATISTICS_TEST_DATA,"server_receive_data_socket_delegates_website_get_statistics","Could not read the statistics database collection");  
  insert_document_into_collection_json(database_name,"statistics",DATABASE_COLLECTION_STATISTICS_DATA,0);
  RESET_ERROR_MESSAGES;

  // server_receive_data_socket_get_delegates
  CHECK_WEBSITE_SERVER_FUNCTIONS_ERROR(SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_TEST_DATA,"server_receive_data_socket_get_delegates","Could not organize the delegates");  
  RESET_ERROR_MESSAGES;

  // server_receive_data_socket_get_delegates_statistics
  CHECK_WEBSITE_SERVER_FUNCTIONS_ERROR("GET /getdelegatesstatistics?parameter1= HTTP/","server_receive_data_socket_get_delegates_statistics","Invalid parameters");  
  RESET_ERROR_MESSAGES;
  
  CHECK_WEBSITE_SERVER_FUNCTIONS_ERROR(SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_STATISTICS_TEST_DATA,"server_receive_data_socket_get_delegates_statistics","Could not get the data from the delegates database collection");  
  RESET_ERROR_MESSAGES;

  // server_receive_data_socket_get_delegates_information
  CHECK_WEBSITE_SERVER_FUNCTIONS_ERROR("GET /getdelegatesinformation?parameter1= HTTP/","server_receive_data_socket_get_delegates_information","Invalid parameters");  
  RESET_ERROR_MESSAGES;
  
  CHECK_WEBSITE_SERVER_FUNCTIONS_ERROR(SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_INFORMATION_TEST_DATA,"server_receive_data_socket_get_delegates_information","Could not get the delegates information");  
  RESET_ERROR_MESSAGES;

  // server_receive_data_socket_get_delegates_voters_list
  CHECK_WEBSITE_SERVER_FUNCTIONS_ERROR("GET /getdelegatesvoterslist?parameter1= HTTP/","server_receive_data_socket_get_delegates_voters_list","Invalid parameters");  
  RESET_ERROR_MESSAGES;
  
  CHECK_WEBSITE_SERVER_FUNCTIONS_ERROR("GET /getdelegatesvoterslist?parameter1=delegate_name_1 HTTP/","server_receive_data_socket_get_delegates_voters_list","Could not find a registered delegate with the information provided");  
  insert_document_into_collection_json(database_name,"delegates",DELEGATES_TEST_DATA,0);
  RESET_ERROR_MESSAGES;

  CHECK_WEBSITE_SERVER_FUNCTIONS_ERROR(SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_VOTERS_LIST_TEST_DATA,"server_receive_data_socket_get_delegates_voters_list","There are no reserve proofs for the delegate");  
  delete_database(database_name,0);
  RESET_ERROR_MESSAGES;

  // server_receive_data_socket_get_round_statistics
  CHECK_WEBSITE_SERVER_FUNCTIONS_ERROR("GET /getroundstatistics?parameter1= HTTP/ XCA1v18Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3","server_receive_data_socket_get_round_statistics","Invalid parameters");  
  RESET_ERROR_MESSAGES;
  
  CHECK_WEBSITE_SERVER_FUNCTIONS_ERROR("GET /getroundstatistics?parameter1=100 HTTP/ XCA1v18Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3","server_receive_data_socket_get_round_statistics","Invalid block height");  
  RESET_ERROR_MESSAGES;

  CHECK_WEBSITE_SERVER_FUNCTIONS_ERROR(SERVER_RECEIVE_DATA_SOCKET_GET_ROUND_STATISTICS_TEST_DATA,"server_receive_data_socket_get_round_statistics","Could not get the rounds statistics");  
  RESET_ERROR_MESSAGES;

  // write the end test message
  fprintf(stderr,"\033[1;33m\n\n%s\ndelegate website functions test - Passed test: %d, Failed test: %d\n%s\n\n\n\033[0m",TEST_OUTLINE,count_test,DELEGATE_WEBSITE_FUNCTIONS_TOTAL_TEST-count_test,TEST_OUTLINE);
  return count_test;

  #undef DELEGATE_WEBSITE_FUNCTIONS_TOTAL_TEST
  #undef DELEGATE_WEBSITE_GET_STATISTICS_TEST_DATA
  #undef DELEGATE_WEBSITE_GET_DELEGATES_TEST_DATA
  #undef DELEGATE_WEBSITE_GET_DELEGATES_STATISTICS_TEST_DATA
  #undef DELEGATE_WEBSITE_GET_DELEGATES_INFORMATION_TEST_DATA
  #undef DELEGATE_WEBSITE_GET_DELEGATES_VOTERS_LIST_TEST_DATA
  #undef DELEGATE_WEBSITE_GET_ROUND_STATISTICS_TEST_DATA
}