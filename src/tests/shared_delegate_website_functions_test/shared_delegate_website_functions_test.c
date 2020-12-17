#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "define_macros_test.h"

#include "shared_delegate_website_thread_server_functions.h"
#include "insert_database_functions.h"
#include "delete_database_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "string_functions.h"
#include "variables_test.h"
#include "shared_delegate_website_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: shared_delegate_website_functions_test
Description: Test the shared_delegate website functions
Return: The number of passed shared_delegate_website_functions test
-----------------------------------------------------------------------------------------------------------
*/

int shared_delegate_website_functions_test(void)
{
  // Variables
  char data[BUFFER_SIZE];
  int count;

  // define macros
  #define GET_BLOCKS_FOUND_TEST_DATA_RESULTS "[{\"block_height\":\"521850\",\"block_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\",\"block_date_and_time\":\"10\",\"block_reward\":\"15\"},{\"block_height\":\"440850\",\"block_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\",\"block_date_and_time\":\"10\",\"block_reward\":\"10\"}]"
  #define GET_PUBLIC_ADDRESS_INFORMATION_DATA_RESULTS "{\"public_address\":\"" TEST_WALLET "\",\"current_total\":\"5\",\"total\":\"10\",\"inactivity_count\":\"15\"}"
  #define GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_DATA_RESULTS "[{\"public_address\":\"" TEST_WALLET "\",\"payment_name\":\"\",\"payment_address\":\"" TEST_WALLET "\",\"date_and_time\":\"5\",\"total\":\"10\",\"tx_hash\":\"TX_HASH\",\"tx_key\":\"TX_KEY\"},{\"public_address\":\"" TEST_WALLET "\",\"payment_name\":\"\",\"payment_address\":\"" TEST_WALLET "\",\"date_and_time\":\"10\",\"total\":\"10\",\"tx_hash\":\"TX_HASH\",\"tx_key\":\"TX_KEY\"}]"
  
  memset(database_name,0,sizeof(database_name));
  memcpy(database_name,DATABASE_NAME_TEST,sizeof(DATABASE_NAME_TEST)-1);
  memset(shared_delegates_database_name,0,sizeof(shared_delegates_database_name));
  memcpy(shared_delegates_database_name,DATABASE_NAME_TEST,sizeof(DATABASE_NAME_TEST)-1);
  delete_database(database_name);
  shared_delegates_website = 1;
  delegates_website = 0;
  insert_document_into_collection_json(database_name,"reserve_bytes_1",RESERVE_BYTES_TEST_DATA);
  insert_document_into_collection_json(database_name,"statistics",DATABASE_COLLECTION_STATISTICS_DATA);
  insert_document_into_collection_json(database_name,"blocks_found","{\"block_height\":\"521850\",\"block_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\",\"block_date_and_time\":\"10\",\"block_reward\":\"15\"}");
  insert_document_into_collection_json(database_name,"blocks_found","{\"block_height\":\"440850\",\"block_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\",\"block_date_and_time\":\"10\",\"block_reward\":\"10\"}");
  insert_document_into_collection_json(database_name,"public_addresses","{\"public_address\":\"" TEST_WALLET "\",\"current_total\":\"5\",\"total\":\"10\",\"inactivity_count\":\"15\"}");
  insert_document_into_collection_json(database_name,"public_addresses","{\"public_address\":\"" TEST_WALLET_20 "\",\"current_total\":\"5\",\"total\":\"10\",\"inactivity_count\":\"15\"}");
  insert_document_into_collection_json(database_name,"public_addresses_payments","{\"public_address\":\"" TEST_WALLET "\",\"payment_name\":\"\",\"payment_address\":\"" TEST_WALLET "\",\"date_and_time\":\"5\",\"total\":\"10\",\"tx_hash\":\"TX_HASH\",\"tx_key\":\"TX_KEY\"}");
  insert_document_into_collection_json(database_name,"public_addresses_payments","{\"public_address\":\"" TEST_WALLET "\",\"payment_name\":\"\",\"payment_address\":\"" TEST_WALLET "\",\"date_and_time\":\"10\",\"total\":\"10\",\"tx_hash\":\"TX_HASH\",\"tx_key\":\"TX_KEY\"}");
  memset(result_test,0,sizeof(result_test));
  memcpy(result_test,"{\"public_address\":\"",19);
  memcpy(result_test+strlen(result_test),xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(result_test+strlen(result_test),"\",\"total_vote_count\":\"2000000000\",\"IP_address\":\"127.0.0.1\",\"delegate_name\":\"delegate_name_1\",\"about\":\"about\",\"website\":\"website\",\"team\":\"team\",\"shared_delegate_status\":\"false\",\"delegate_fee\":\"0\",\"server_specs\":\"server settings\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"1\",\"block_verifier_online_total_rounds\":\"1\",\"block_verifier_online_percentage\":\"100\",\"block_producer_total_rounds\":\"2\",\"block_producer_block_heights\":\"|440875|440876\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}",556);
  insert_document_into_collection_json(database_name,"delegates",result_test);
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_20 "\",\"total_vote_count\":\"1000000000\",\"IP_address\":\"192.168.1.220\",\"delegate_name\":\"delegate_name_20\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"shared_delegate_status\":\"false\",\"delegate_fee\":\"\",\"server_specs\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}");
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_20 "\",\"total_vote_count\":\"100000000\",\"IP_address\":\"192.168.1.220\",\"delegate_name\":\"delegate_name_20\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"shared_delegate_status\":\"false\",\"delegate_fee\":\"\",\"server_specs\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}");
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_20 "\",\"total_vote_count\":\"10000000\",\"IP_address\":\"192.168.1.220\",\"delegate_name\":\"delegate_name_20\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"shared_delegate_status\":\"false\",\"delegate_fee\":\"\",\"server_specs\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}");
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_20 "\",\"total_vote_count\":\"5000000000\",\"IP_address\":\"192.168.1.220\",\"delegate_name\":\"delegate_name_20\",\"about\":\"about\",\"website\":\"\",\"team\":\"\",\"shared_delegate_status\":\"false\",\"delegate_fee\":\"\",\"server_specs\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}");
  memset(result_test,0,sizeof(result_test));
  memcpy(result_test,"{\"public_address_created_reserve_proof\":\"" TEST_WALLET "\",\"public_address_voted_for\":\"",169);
  memcpy(result_test+strlen(result_test),xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(result_test+strlen(result_test),"\",\"total\":\"2000000000\",\"reserve_proof\":\"DATA\"}",46);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",result_test);
  RESET_ERROR_MESSAGES;

  // reset the variables
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memset(data,0,sizeof(data));
  count_test = 0;
  network_functions_test_settings = 0;
  network_functions_test_error_settings = 2;
 
  // write the start test message
  fprintf(stderr,"\033[1;34m%s\nshared_delegate website functions test - Total test: %d\n%s\n\n\033[0m",TEST_OUTLINE,SHARED_DELEGATE_WEBSITE_FUNCTIONS_TOTAL_TEST,TEST_OUTLINE);
 
  // run the test
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,SERVER_RECEIVE_DATA_SOCKET_SHARED_DELEGATES_WEBSITE_GET_STATISTICS_TEST_DATA,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strstr(data_test,xcash_wallet_public_address) != NULL && strstr(data_test,"\"total_votes\":\"2000000000\"") != NULL && strstr(data_test,"\"online_percentage\":\"100\"") != NULL && strstr(data_test,"\"total_blocks_found\":\"2\"") != NULL && strstr(data_test,"\"total_xcash_from_blocks_found\":\"25\"") != NULL && strstr(data_test,"\"total_payments\":\"2\"") != NULL && strstr(data_test,"\"fee\":\"0.000000\"") != NULL && strstr(data_test,"\"minimum_amount\":\"0\"") != NULL)
  {
    color_print("PASSED! Test for server_receive_data_socket_shared_delegates_website_get_statistics","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for server_receive_data_socket_shared_delegates_website_get_statistics","red");
  }
  RESET_ERROR_MESSAGES;
  
  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,SERVER_RECEIVE_DATA_SOCKET_GET_BLOCKS_FOUND_TEST_DATA,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,GET_BLOCKS_FOUND_TEST_DATA_RESULTS,sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for server_receive_data_socket_get_blocks_found","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for server_receive_data_socket_get_blocks_found","red");
  }
  RESET_ERROR_MESSAGES;

  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_INFORMATION_TEST_DATA,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,GET_PUBLIC_ADDRESS_INFORMATION_DATA_RESULTS,sizeof(data_test)) == 0) 
  {
    color_print("PASSED! Test for server_receive_data_socket_get_public_address_information","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for server_receive_data_socket_get_public_address_information","red");
  }
  RESET_ERROR_MESSAGES;

  memset(data_test,0,sizeof(data_test));
  if (send_and_receive_data_socket(data_test,sizeof(data_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_TEST_DATA,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 1 && parse_http_response(data_test) == 1 && strncmp(data_test,GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_DATA_RESULTS,sizeof(data_test)) == 0)
  {
    color_print("PASSED! Test for server_receive_data_socket_get_public_address_payment_information","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for server_receive_data_socket_get_public_address_payment_information","red");
  }
  RESET_ERROR_MESSAGES;



  // check all errors for each test
  delete_database(database_name);
  RESET_ERROR_MESSAGES;
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));

  // server_receive_data_socket_shared_delegates_website_get_statistics
  CHECK_WEBSITE_SERVER_FUNCTIONS_ERROR(SERVER_RECEIVE_DATA_SOCKET_SHARED_DELEGATES_WEBSITE_GET_STATISTICS_TEST_DATA,"server_receive_data_socket_shared_delegates_website_get_statistics","Could not get the shared delegates statistics");  
  RESET_ERROR_MESSAGES;

  // server_receive_data_socket_get_blocks_found
  CHECK_WEBSITE_SERVER_FUNCTIONS_ERROR(SERVER_RECEIVE_DATA_SOCKET_GET_BLOCKS_FOUND_TEST_DATA,"server_receive_data_socket_get_blocks_found","The delegate has not found any blocks");  
  RESET_ERROR_MESSAGES;

  CHECK_WEBSITE_SERVER_FUNCTIONS_ERROR(SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_INFORMATION_TEST_DATA,"server_receive_data_socket_get_public_address_information","The public address could not be found in the database for the delegate");  
  RESET_ERROR_MESSAGES;

  CHECK_WEBSITE_SERVER_FUNCTIONS_ERROR(SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_TEST_DATA,"server_receive_data_socket_get_public_address_payment_information","There is no payment data for the public address");  
  RESET_ERROR_MESSAGES;
  
  // write the end test message
  fprintf(stderr,"\033[1;33m\n\n%s\nshared_delegate website functions test - Passed test: %d, Failed test: %d\n%s\n\n\n\033[0m",TEST_OUTLINE,count_test,SHARED_DELEGATE_WEBSITE_FUNCTIONS_TOTAL_TEST-count_test,TEST_OUTLINE);
  return count_test;
  
  #undef GET_BLOCKS_FOUND_TEST_DATA_RESULTS
  #undef GET_PUBLIC_ADDRESS_INFORMATION_DATA_RESULTS
  #undef GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_DATA_RESULTS
}