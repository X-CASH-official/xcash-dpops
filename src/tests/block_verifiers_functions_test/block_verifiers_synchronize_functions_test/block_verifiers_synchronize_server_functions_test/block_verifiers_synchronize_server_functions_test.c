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
#include "variables_test.h"
#include "string_functions.h"
#include "block_verifiers_synchronize_server_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: block_verifiers_synchronize_server_functions_test
Description: Test the block_verifiers_synchronize_server functions
Return: The number of passed block_verifiers_synchronize_server_functions test
-----------------------------------------------------------------------------------------------------------
*/

int block_verifiers_synchronize_server_functions_test(void)
{
  // Variables
  int count;

  // define macros
  #define GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_TEST_DATA "{\r\n \"message_settings\": \"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST\",\r\n \"previous_block_verifiers_name_list\": \"delegate_1|delegate_1|\",\r\n \"previous_block_verifiers_public_address_list\": \"XCA1v18Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3|XCA1v18Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3|\",\r\n \"previous_block_verifiers_public_key_list\": \"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a|ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a|\",\r\n \"previous_block_verifiers_IP_address_list\": \"127.0.0.1|127.0.0.1|\",\r\n \"current_block_verifiers_name_list\": \"delegate_1|delegate_1|\",\r\n \"current_block_verifiers_public_address_list\": \"XCA1v18Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3|XCA1v18Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3|\",\r\n \"current_block_verifiers_public_key_list\": \"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a|ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a|\",\r\n \"current_block_verifiers_IP_address_list\": \"127.0.0.1|127.0.0.1|\",\r\n \"next_block_verifiers_name_list\": \"delegate_1|delegate_1|\",\r\n \"next_block_verifiers_public_address_list\": \"XCA1v18Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3|XCA1v18Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3|\",\r\n \"next_block_verifiers_public_key_list\": \"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a|ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a|\",\r\n \"next_block_verifiers_IP_address_list\": \"127.0.0.1|127.0.0.1|\""
  #define GET_CURRENT_BLOCK_VERIFIERS_LIST_TEST_DATA "{\r\n \"message_settings\": \"NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST\",\r\n \"block_verifiers_public_address_list\": \"XCA1v18Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3|XCA1v18Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3|\",\r\n \"block_verifiers_public_key_list\": \"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a|ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a|\",\r\n \"block_verifiers_IP_address_list\": \"127.0.0.1|127.0.0.1|\""
  #define RESERVE_BYTES_SYNC_CHECK_ALL_UPDATE_TEST_DATA "BLOCK_VERIFIERS_TO_NODES_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD|1eba4b3c56c6d25f91b5ce6340c49f6a8d1a69f21b4f02584863aa55b27edd453e6352ab0ae838f5ddbde1c1ed06d0684c00f4bfa8d5b0b9b74b61b909b96daf|"
  #define GET_RESERVE_BYTES_DATABASE_HASH_TEST_DATA "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000|00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001|00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002|00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003|00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000004|}"
  #define GET_RESERVE_BYTES_DATABASE_HASH_LIVE_SYNCING_TEST_DATA "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005|}"
  #define RESERVE_PROOFS_SYNC_CHECK_ALL_UPDATE_TEST_DATA "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"reserve_proofs_database\": \"false\",\r\n \"reserve_proofs_database_1\": \"true\",\r\n\"reserve_proofs_database_2\": \"false\",\r\n\"reserve_proofs_database_3\": \"false\",\r\n\"reserve_proofs_database_4\": \"false\",\r\n\"reserve_proofs_database_5\": \"false\",\r\n\"reserve_proofs_database_6\": \"false\",\r\n\"reserve_proofs_database_7\": \"false\",\r\n\"reserve_proofs_database_8\": \"false\",\r\n\"reserve_proofs_database_9\": \"false\",\r\n\"reserve_proofs_database_10\": \"false\",\r\n\"reserve_proofs_database_11\": \"false\",\r\n\"reserve_proofs_database_12\": \"false\",\r\n\"reserve_proofs_database_13\": \"false\",\r\n\"reserve_proofs_database_14\": \"false\",\r\n\"reserve_proofs_database_15\": \"false\",\r\n\"reserve_proofs_database_16\": \"false\",\r\n\"reserve_proofs_database_17\": \"false\",\r\n\"reserve_proofs_database_18\": \"false\",\r\n\"reserve_proofs_database_19\": \"false\",\r\n\"reserve_proofs_database_20\": \"false\",\r\n\"reserve_proofs_database_21\": \"false\",\r\n\"reserve_proofs_database_22\": \"false\",\r\n\"reserve_proofs_database_23\": \"false\",\r\n\"reserve_proofs_database_24\": \"false\",\r\n\"reserve_proofs_database_25\": \"false\",\r\n\"reserve_proofs_database_26\": \"false\",\r\n\"reserve_proofs_database_27\": \"false\",\r\n\"reserve_proofs_database_28\": \"false\",\r\n\"reserve_proofs_database_29\": \"false\",\r\n\"reserve_proofs_database_30\": \"false\",\r\n\"reserve_proofs_database_31\": \"false\",\r\n\"reserve_proofs_database_32\": \"false\",\r\n\"reserve_proofs_database_33\": \"false\",\r\n\"reserve_proofs_database_34\": \"false\",\r\n\"reserve_proofs_database_35\": \"false\",\r\n\"reserve_proofs_database_36\": \"false\",\r\n\"reserve_proofs_database_37\": \"false\",\r\n\"reserve_proofs_database_38\": \"false\",\r\n\"reserve_proofs_database_39\": \"false\",\r\n\"reserve_proofs_database_40\": \"false\",\r\n\"reserve_proofs_database_41\": \"false\",\r\n\"reserve_proofs_database_42\": \"false\",\r\n\"reserve_proofs_database_43\": \"false\",\r\n\"reserve_proofs_database_44\": \"false\",\r\n\"reserve_proofs_database_45\": \"false\",\r\n\"reserve_proofs_database_46\": \"false\",\r\n\"reserve_proofs_database_47\": \"false\",\r\n\"reserve_proofs_database_48\": \"false\",\r\n\"reserve_proofs_database_49\": \"false\",\r\n\"reserve_proofs_database_50\": \"false\""
  #define RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"reserve_proofs_database\": \"{\"public_address_created_reserve_proof\" : \"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\", \"public_address_voted_for\" : \"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\", \"total\" : \"120000000\", \"reserve_proof\" : \"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti\" }\""
  #define RESERVE_BYTES_SYNC_CHECK_ALL_UPDATE_TEST_DATA_RESULTS "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"reserve_bytes_database\": \"false\",\r\n \"reserve_bytes_database_1\": \"true\""
  #define RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"reserve_bytes_database\": \"{\"block_height\" : \"521850\", \"reserve_bytes_data_hash\" : \"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000\", \"reserve_bytes\" : \"DATA\" }\""
  #define DELEGATES_SYNC_CHECK_ALL_UPDATE_TEST_DATA "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"delegates_database\": \"true\""
  #define DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"delegates_database\": \"{\"public_address\" : \"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\", \"total_vote_count\" : \"120000000\", \"IP_address\" : \"192.168.1.201\", \"delegate_name\" : \"delegate_name_1\", \"about\" : \"\", \"website\" : \"\", \"team\" : \"\", \"pool_mode\" : \"false\", \"fee_structure\" : \"\", \"server_settings\" : \"\", \"block_verifier_score\" : \"0\", \"online_status\" : \"true\", \"block_verifier_total_rounds\" : \"0\", \"block_verifier_online_total_rounds\" : \"0\", \"block_verifier_online_percentage\" : \"0\", \"block_producer_total_rounds\" : \"0\", \"block_producer_block_heights\" : \"\", \"public_key\" : \"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\" }\""
  #define STATISTICS_SYNC_CHECK_ALL_UPDATE_TEST_DATA "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"statistics_database\": \"true\","
  #define STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"statistics_database\": \"{\"username\" : \"XCASH\", \"most_total_rounds_delegate_name\" : \"delegate_name_1\", \"most_total_rounds\" : \"0\", \"best_block_verifier_online_percentage_delegate_name\" : \"delegate_name_1\", \"best_block_verifier_online_percentage\" : \"0\", \"most_block_producer_total_rounds_delegate_name\" : \"delegate_name_1\", \"most_block_producer_total_rounds\" : \"0\" }\""

  // initialize the previous, current and next block verifiers list
  INITIALIZE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_TEST;

  // reset the variables
  insert_document_into_collection_json(database_name,"reserve_bytes_1",RESERVE_BYTES_TEST_DATA,0);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",RESERVE_PROOFS_TEST_DATA,0);
  insert_document_into_collection_json(database_name,"delegates",DELEGATES_TEST_DATA,0);
  insert_document_into_collection_json(database_name,"statistics",DATABASE_COLLECTION_STATISTICS_DATA,0);
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  count_test = 0;
  RESET_ERROR_MESSAGES;
  
 
  // write the start test message
  fprintf(stderr,"\033[1;34m%s\nblock verifiers synchronize server functions test - Total test: %d\n%s\n\n\033[0m",TEST_OUTLINE,BLOCK_VERIFIERS_SYNCHRONIZE_SERVER_FUNCTIONS_TOTAL_TEST,TEST_OUTLINE);
 
  // run the test
  network_functions_test_error_settings = 2;

  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(data_test,SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_TEST_DATA)-1);
  if (sign_data(data_test) == 1 && send_and_receive_data_socket(result_test,XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,data_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && memcmp(result_test,GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_TEST_DATA,sizeof(GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_TEST_DATA)-1) == 0)
  {
    color_print("PASSED! Test for server_receive_data_socket_node_to_network_data_nodes_get_previous_current_next_block_verifiers_list","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for server_receive_data_socket_node_to_network_data_nodes_get_previous_current_next_block_verifiers_list","red");
  }
  RESET_ERROR_MESSAGES;

  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memcpy(data_test,SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST_TEST_DATA)-1);
  if (sign_data(data_test) == 1 && send_and_receive_data_socket(result_test,XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,data_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && memcmp(result_test,GET_CURRENT_BLOCK_VERIFIERS_LIST_TEST_DATA,sizeof(GET_CURRENT_BLOCK_VERIFIERS_LIST_TEST_DATA)-1) == 0)
  {
    color_print("PASSED! Test for server_receive_data_socket_node_to_network_data_nodes_get_current_block_verifiers_list","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for server_receive_data_socket_node_to_network_data_nodes_get_current_block_verifiers_list","red");
  }
  RESET_ERROR_MESSAGES;

  CHECK_SERVER_FUNCTIONS("server_receive_data_socket_network_data_nodes_to_network_data_nodes_database_sync_check",SERVER_RECEIVE_DATA_SOCKET_NETWORK_DATA_NODES_TO_NETWORK_DATA_NODES_DATABASE_SYNC_CHECK_TEST_DATA);
  
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  delete_database(database_name,0);
  insert_document_into_collection_json(database_name,"reserve_bytes_1","{\"block_height\":\"521850\",\"reserve_bytes_data_hash\":\"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000\",\"reserve_bytes\":\"DATA\"}",0);
  RESET_ERROR_MESSAGES;
  memcpy(data_test,SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_TEST_DATA)-1);
  if (sign_data(data_test) == 1 && send_and_receive_data_socket(result_test,XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,data_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && memcmp(result_test,RESERVE_BYTES_SYNC_CHECK_ALL_UPDATE_TEST_DATA,sizeof(RESERVE_BYTES_SYNC_CHECK_ALL_UPDATE_TEST_DATA)-1) == 0)
  {
    color_print("PASSED! Test for server_receive_data_socket_nodes_to_block_verifiers_reserve_bytes_database_sync_check_all_update","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for server_receive_data_socket_nodes_to_block_verifiers_reserve_bytes_database_sync_check_all_update","red");
  }
  RESET_ERROR_MESSAGES;

  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  delete_database(database_name,0);
  network_functions_test_error_settings = 0;
  insert_document_into_collection_json(database_name,"reserve_bytes_1","{\"block_height\":\"521850\",\"reserve_bytes_data_hash\":\"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000\",\"reserve_bytes\":\"DATA\"}",0);
  insert_document_into_collection_json(database_name,"reserve_bytes_1","{\"block_height\":\"521851\",\"reserve_bytes_data_hash\":\"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001\",\"reserve_bytes\":\"DATA\"}",0);
  insert_document_into_collection_json(database_name,"reserve_bytes_1","{\"block_height\":\"521852\",\"reserve_bytes_data_hash\":\"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002\",\"reserve_bytes\":\"DATA\"}",0);
  insert_document_into_collection_json(database_name,"reserve_bytes_1","{\"block_height\":\"521853\",\"reserve_bytes_data_hash\":\"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003\",\"reserve_bytes\":\"DATA\"}",0);
  insert_document_into_collection_json(database_name,"reserve_bytes_1","{\"block_height\":\"521854\",\"reserve_bytes_data_hash\":\"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000004\",\"reserve_bytes\":\"DATA\"}",0);
  insert_document_into_collection_json(database_name,"reserve_bytes_1","{\"block_height\":\"521855\",\"reserve_bytes_data_hash\":\"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005\",\"reserve_bytes\":\"DATA\"}",0);
  RESET_ERROR_MESSAGES;
  memcpy(data_test,"{\r\n \"message_settings\": \"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATABASE_HASH\",\r\n \"block_height\": \"521850\",\r\n}",113);
  if (sign_data(data_test) == 1 && send_and_receive_data_socket(result_test,XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,data_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && memcmp(result_test,GET_RESERVE_BYTES_DATABASE_HASH_TEST_DATA,sizeof(GET_RESERVE_BYTES_DATABASE_HASH_TEST_DATA)-1) == 0)
  {
    color_print("PASSED! Test for server_receive_data_socket_node_to_block_verifiers_get_reserve_bytes_database_hash checking for receiving multiple blocks","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for server_receive_data_socket_node_to_block_verifiers_get_reserve_bytes_database_hash checking for receiving multiple blocks","red");
  }
  RESET_ERROR_MESSAGES;

  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  delete_database(database_name,0);
  network_functions_test_error_settings = 0;
  insert_document_into_collection_json(database_name,"reserve_bytes_1","{\"block_height\":\"521850\",\"reserve_bytes_data_hash\":\"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000\",\"reserve_bytes\":\"DATA\"}",0);
  insert_document_into_collection_json(database_name,"reserve_bytes_1","{\"block_height\":\"521851\",\"reserve_bytes_data_hash\":\"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001\",\"reserve_bytes\":\"DATA\"}",0);
  insert_document_into_collection_json(database_name,"reserve_bytes_1","{\"block_height\":\"521852\",\"reserve_bytes_data_hash\":\"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002\",\"reserve_bytes\":\"DATA\"}",0);
  insert_document_into_collection_json(database_name,"reserve_bytes_1","{\"block_height\":\"521853\",\"reserve_bytes_data_hash\":\"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003\",\"reserve_bytes\":\"DATA\"}",0);
  insert_document_into_collection_json(database_name,"reserve_bytes_1","{\"block_height\":\"521854\",\"reserve_bytes_data_hash\":\"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000004\",\"reserve_bytes\":\"DATA\"}",0);
  insert_document_into_collection_json(database_name,"reserve_bytes_1","{\"block_height\":\"521855\",\"reserve_bytes_data_hash\":\"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005\",\"reserve_bytes\":\"DATA\"}",0);
  RESET_ERROR_MESSAGES;
  memcpy(data_test,"{\r\n \"message_settings\": \"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATABASE_HASH\",\r\n \"block_height\": \"521855\",\r\n}",113);
  if (sign_data(data_test) == 1 && send_and_receive_data_socket(result_test,XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,data_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && memcmp(result_test,GET_RESERVE_BYTES_DATABASE_HASH_LIVE_SYNCING_TEST_DATA,sizeof(GET_RESERVE_BYTES_DATABASE_HASH_LIVE_SYNCING_TEST_DATA)-1) == 0)
  {
    color_print("PASSED! Test for server_receive_data_socket_node_to_block_verifiers_get_reserve_bytes_database_hash checking for live syncing","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for server_receive_data_socket_node_to_block_verifiers_get_reserve_bytes_database_hash checking for live syncing","red");
  }
  RESET_ERROR_MESSAGES;

  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  network_functions_test_error_settings = 2;
  RESET_ERROR_MESSAGES;
  memcpy(data_test,SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_CHECK_IF_CURRENT_BLOCK_VERIFIER_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_CHECK_IF_CURRENT_BLOCK_VERIFIER_TEST_DATA)-1);
  if (sign_data(data_test) == 1 && send_and_receive_data_socket(result_test,XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,data_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && memcmp(result_test,"0}",2) == 0)
  {
    color_print("PASSED! Test for server_receive_data_socket_node_to_block_verifiers_check_if_current_block_verifier","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for server_receive_data_socket_node_to_block_verifiers_check_if_current_block_verifier","red");
  }
  RESET_ERROR_MESSAGES;

  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  delete_database(database_name,0);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_RESERVE_PROOFS_DATA_1,0);
  RESET_ERROR_MESSAGES;
  memcpy(data_test,SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_TEST_DATA)-1);
  if (sign_data(data_test) == 1 && send_and_receive_data_socket(result_test,XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,data_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && memcmp(result_test,RESERVE_PROOFS_SYNC_CHECK_ALL_UPDATE_TEST_DATA,sizeof(RESERVE_PROOFS_SYNC_CHECK_ALL_UPDATE_TEST_DATA)-1) == 0)
  {
    color_print("PASSED! Test for server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_all_update","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_all_update","red");
  }
  RESET_ERROR_MESSAGES;

  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  delete_database(database_name,0);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_RESERVE_PROOFS_DATA_1,0);
  RESET_ERROR_MESSAGES;
  memcpy(data_test,SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA)-1);
  if (sign_data(data_test) == 1 && send_and_receive_data_socket(result_test,XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,data_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && memcmp(result_test,RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA,sizeof(RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA)-1) == 0)
  {
    color_print("PASSED! Test for server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_download_file_update","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_download_file_update","red");
  }
  RESET_ERROR_MESSAGES;

  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  delete_database(database_name,0);
  insert_document_into_collection_json(database_name,"reserve_bytes_1","{\"block_height\":\"521850\",\"reserve_bytes_data_hash\":\"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000\",\"reserve_bytes\":\"DATA\"}",0);
  RESET_ERROR_MESSAGES;
  memcpy(data_test,SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_TEST_DATA)-1);
  if (sign_data(data_test) == 1 && send_and_receive_data_socket(result_test,XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,data_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && memcmp(result_test,RESERVE_BYTES_SYNC_CHECK_ALL_UPDATE_TEST_DATA_RESULTS,sizeof(RESERVE_BYTES_SYNC_CHECK_ALL_UPDATE_TEST_DATA_RESULTS)-1) == 0)
  {
    color_print("PASSED! Test for server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_all_update","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_all_update","red");
  }
  RESET_ERROR_MESSAGES;

  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  delete_database(database_name,0);
  insert_document_into_collection_json(database_name,"reserve_bytes_1","{\"block_height\":\"521850\",\"reserve_bytes_data_hash\":\"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000\",\"reserve_bytes\":\"DATA\"}",0);
  RESET_ERROR_MESSAGES;
  memcpy(data_test,SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA)-1);
  if (sign_data(data_test) == 1 && send_and_receive_data_socket(result_test,XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,data_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && memcmp(result_test,RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA,sizeof(RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA)-1) == 0)
  {
    color_print("PASSED! Test for server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_download_file_update","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_download_file_update","red");
  }
  RESET_ERROR_MESSAGES;

  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  delete_database(database_name,0);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_1,0);
  RESET_ERROR_MESSAGES;
  memcpy(data_test,SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_TEST_DATA)-1);
  if (sign_data(data_test) == 1 && send_and_receive_data_socket(result_test,XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,data_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && memcmp(result_test,DELEGATES_SYNC_CHECK_ALL_UPDATE_TEST_DATA,sizeof(DELEGATES_SYNC_CHECK_ALL_UPDATE_TEST_DATA)-1) == 0)
  {
    color_print("PASSED! Test for server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_sync_check_update","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_sync_check_update","red");
  }
  RESET_ERROR_MESSAGES;

  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  delete_database(database_name,0);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_SHARED_DELEGATE_RESERVE_PROOFS_DELEGATES_DATA_1,0);
  RESET_ERROR_MESSAGES;
  memcpy(data_test,SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA)-1);
  if (sign_data(data_test) == 1 && send_and_receive_data_socket(result_test,XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,data_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && memcmp(result_test,DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA,sizeof(DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA)-1) == 0)
  {
    color_print("PASSED! Test for server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_download_file_update","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_download_file_update","red");
  }
  RESET_ERROR_MESSAGES;

  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  delete_database(database_name,0);
  insert_document_into_collection_json(database_name,"statistics",DATABASE_COLLECTION_STATISTICS_DATA,0);
  RESET_ERROR_MESSAGES;
  memcpy(data_test,SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE_TEST_DATA)-1);
  if (sign_data(data_test) == 1 && send_and_receive_data_socket(result_test,XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,data_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && memcmp(result_test,STATISTICS_SYNC_CHECK_ALL_UPDATE_TEST_DATA,sizeof(STATISTICS_SYNC_CHECK_ALL_UPDATE_TEST_DATA)-1) == 0)
  {
    color_print("PASSED! Test for server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_sync_check_update","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_sync_check_update","red");
  }
  RESET_ERROR_MESSAGES;

  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  delete_database(database_name,0);
  insert_document_into_collection_json(database_name,"statistics",DATABASE_COLLECTION_STATISTICS_DATA,0);
  RESET_ERROR_MESSAGES;
  memcpy(data_test,SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA)-1);
  if (sign_data(data_test) == 1 && send_and_receive_data_socket(result_test,XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,data_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && memcmp(result_test,STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA,sizeof(STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA)-1) == 0)
  {
    color_print("PASSED! Test for server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_download_file_update","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_download_file_update","red");
  }
  RESET_ERROR_MESSAGES;
  delete_database(database_name,0);

  // check all errors for each test
  network_functions_test_error_settings = 2;
  RESET_ERROR_MESSAGES;
  CHECK_SERVER_FUNCTIONS_ERROR("server_receive_data_socket_network_data_nodes_to_network_data_nodes_database_sync_check",SERVER_RECEIVE_DATA_SOCKET_NETWORK_DATA_NODES_TO_NETWORK_DATA_NODES_DATABASE_SYNC_CHECK_TEST_DATA,1,xcash_wallet_public_address,TEST_WALLET,"Could not verify the message");
  test_settings = 0;
  CHECK_SERVER_FUNCTIONS_ERROR("server_receive_data_socket_node_to_block_verifiers_get_reserve_bytes_database_hash","{\r\n \"message_settings\": \"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATABASE_HASH\",\r\n \"block_height\": \"100\",\r\n}",1,xcash_wallet_public_address,TEST_WALLET,"Invalid block height");
  test_settings = 1;
  CHECK_SERVER_FUNCTIONS_ERROR("server_receive_data_socket_node_to_block_verifiers_get_reserve_bytes_database_hash",SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATABASE_HASH_TEST_DATA,0,"","","Could not get the previous blocks reserve bytes");
  CHECK_SERVER_FUNCTIONS_ERROR("server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_download_file_update",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA,0,"","","Could not get the database data hash for the reserve proofs database");
  CHECK_SERVER_FUNCTIONS_ERROR("server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_download_file_update",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA,0,"","","Could not get the database data hash for the reserve bytes database");
  CHECK_SERVER_FUNCTIONS_ERROR("server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_download_file_update",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA,0,"","","Could not get the database data hash for the delegates database");
  CHECK_SERVER_FUNCTIONS_ERROR("server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_download_file_update",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA,0,"","","Could not get the database data hash for the statistics database");

  // write the end test message
  fprintf(stderr,"\033[1;33m\n\n%s\nblock verifiers synchronize server functions test - Passed test: %d, Failed test: %d\n%s\n\n\n\033[0m",TEST_OUTLINE,count_test,BLOCK_VERIFIERS_SYNCHRONIZE_SERVER_FUNCTIONS_TOTAL_TEST-count_test,TEST_OUTLINE);
  return count_test;

  #undef GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_TEST_DATA
  #undef GET_CURRENT_BLOCK_VERIFIERS_LIST_TEST_DATA
  #undef RESERVE_BYTES_SYNC_CHECK_ALL_UPDATE_TEST_DATA
  #undef GET_RESERVE_BYTES_DATABASE_HASH_TEST_DATA
  #undef GET_RESERVE_BYTES_DATABASE_HASH_LIVE_SYNCING_TEST_DATA
  #undef RESERVE_PROOFS_SYNC_CHECK_ALL_UPDATE_TEST_DATA
  #undef RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA
  #undef RESERVE_BYTES_SYNC_CHECK_ALL_UPDATE_TEST_DATA_RESULTS
  #undef RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA
  #undef DELEGATES_SYNC_CHECK_ALL_UPDATE_TEST_DATA
  #undef DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA
  #undef STATISTICS_SYNC_CHECK_ALL_UPDATE_TEST_DATA
  #undef STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA
}