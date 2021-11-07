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
#include "define_macros_test.h"
#include "initialize_and_reset_structs_define_macros.h"

#include "blockchain_functions.h"
#include "block_verifiers_synchronize_check_functions.h"
#include "block_verifiers_synchronize_functions.h"
#include "block_verifiers_server_functions_test.h"
#include "block_verifiers_thread_server_functions.h"
#include "block_verifiers_update_functions.h"
#include "database_functions.h"
#include "delegate_server_functions.h"
#include "delegate_website_functions_test.h"
#include "insert_database_functions.h"
#include "read_database_functions.h"
#include "update_database_functions.h"
#include "delete_database_functions.h"
#include "count_database_functions.h"
#include "file_functions.h"
#include "network_daemon_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "network_wallet_functions.h"
#include "organize_functions.h"
#include "server_functions.h"
#include "shared_delegates_website_functions.h"
#include "shared_delegate_website_thread_server_functions.h"
#include "string_functions.h"
#include "thread_functions.h"
#include "vrf.h"
#include "crypto_vrf.h"
#include "VRF_functions.h"
#include "sha512EL.h"

#include "variables_test.h"
#include "network_functions_test.h"
#include "reset_variables_allocated_on_the_heap_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: get_program_memory_usage
Description: Gets the current memory usage of the program
Parameters:
   process_id_file - The file to read the memory usage from
Return: 0 if an error has occured, otherwise the programs memory usage in KB
-----------------------------------------------------------------------------------------------------------
*/

size_t get_program_memory_usage(const char* PROCESS_ID_FILE)
{
  // Variables
  FILE* file;
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t programs_memory_usage = 0;

  if (data == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // read the current system memory usage
  if ((file = fopen(PROCESS_ID_FILE,"r")) != NULL)
  {
    while(fgets(data,BUFFER_SIZE,file) != NULL)
    {
      if (strstr(data,"VmSize:") != NULL)
      {
        string_replace(data,BUFFER_SIZE,"VmSize:","");
        string_replace(data,BUFFER_SIZE,"\t","");
        string_replace(data,BUFFER_SIZE," ","");
        string_replace(data,BUFFER_SIZE,"kB","");
        string_replace(data,BUFFER_SIZE,"\n","");
        sscanf(data, "%zu", &programs_memory_usage);
      }
      memset(data,0,strnlen(data,BUFFER_SIZE));
    }
    fclose(file);
  }
  pointer_reset(data);
  return programs_memory_usage;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: reset_variables_allocated_on_the_heap_test
Description: Test each function to make sure that the variables are reset that were allocated on the heap
Return: The number of passed reset_variables_allocated_on_the_heap test
-----------------------------------------------------------------------------------------------------------
*/

int reset_variables_allocated_on_the_heap_test(void)
{
  // Constants
  const unsigned char data2[crypto_vrf_SEEDBYTES] = {0x4c,0xcd,0x08,0x9b,0x28,0xff,0x96,0xda,0x9d,0xb6,0xc3,0x46,0xec,0x11,0x4e,0x0f,0x5b,0x8a,0x31,0x9f,0x35,0xab,0xa6,0x24,0xda,0x8c,0xf6,0xed,0x4f,0xb8,0xa6,0xfb};
  const unsigned char alpha_string[1] = {"\x72"};
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);
  const char validate_data_settings[37][10000] = {MAIN_NODES_TO_NODES_PART_4_OF_ROUND_CREATE_NEW_BLOCK,MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_START_BLOCK,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA,NODES_TO_NODES_VOTE_RESULTS,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS,NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST,NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST,NETWORK_DATA_NODE_TO_NODE_SEND_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST,NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST,BLOCK_VERIFIERS_TO_NETWORK_DATA_NODE_BLOCK_VERIFIERS_CURRENT_TIME,NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_BLOCK_VERIFIERS_CURRENT_TIME,NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE,NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE,NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF,NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATABASE_HASH,NODE_TO_BLOCK_VERIFIERS_CHECK_IF_CURRENT_BLOCK_VERIFIER,BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES,BLOCK_VERIFIERS_TO_NODES_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD,NETWORK_DATA_NODES_TO_NETWORK_DATA_NODES_DATABASE_SYNC_CHECK,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD};

  // Variables
  char data3[BUFFER_SIZE];
  char* process_id_file = (char*)calloc(BUFFER_SIZE,sizeof(char));
  int previous_system_memory_usage;
  int current_system_memory_usage;
  int current_memory_usage = 0;
  size_t count = 0;
  size_t count2 = 0;
  size_t counter;
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  struct reserve_proof reserve_proof;
  struct database_document_fields database_data;
  struct database_multiple_documents_fields database_multiple_documents_fields;
  struct delegates delegates[MAXIMUM_AMOUNT_OF_DELEGATES];
  struct votes votes[MAXIMUM_AMOUNT_OF_DELEGATES];
  struct voters voters[MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE];
  unsigned char vrf_public_key[crypto_vrf_PUBLICKEYBYTES];
  unsigned char vrf_secret_key[crypto_vrf_SECRETKEYBYTES];
  unsigned char vrf_proof[crypto_vrf_PROOFBYTES];
  unsigned char vrf_beta[crypto_vrf_OUTPUTBYTES];
  char* transactions[5];
  size_t block_height;

  // define macros
  #define GET_PUBLIC_ADDRESS_DATA "{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_address\"}"
  #define GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA "{\r\n  \"id\": \"0\",\r\n  \"jsonrpc\": \"2.0\",\r\n  \"result\": {\r\n    \"blockhashing_blob\": \"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\",\r\n    \"blocktemplate_blob\": \"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\",\r\n    \"difficulty\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,\r\n    \"expected_reward\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,\r\n    \"height\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,\r\n    \"prev_hash\": \"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\",\r\n    \"reserved_offset\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,\r\n    \"status\": \"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\",\r\n    \"untrusted\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\r\n  }\r\n}"
  #define DATA1 "{\"username\":\"XCASH\",\"most_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_total_rounds\":\"5\",\"best_block_verifier_online_percentage_delegate_name\":\"DELEGATE_NAME\",\"best_block_verifier_online_percentage\":\"10\",\"most_block_producer_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_block_producer_total_rounds\":\"15\"}"
  #define DATA2 "[{\"username\":\"XCASH\",\"most_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_total_rounds\":\"5\",\"best_block_verifier_online_percentage_delegate_name\":\"DELEGATE_NAME\",\"best_block_verifier_online_percentage\":\"10\",\"most_block_producer_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_block_producer_total_rounds\":\"15\"},{\"username\":\"XCASH\",\"most_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_total_rounds\":\"5\",\"best_block_verifier_online_percentage_delegate_name\":\"DELEGATE_NAME\",\"best_block_verifier_online_percentage\":\"10\",\"most_block_producer_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_block_producer_total_rounds\":\"15\"}]" 
  #define NETWORK_BLOCK_HEIGHT "2813049"
  #define VARINT_ENCODED_VALUE_1 0xb2f58199a302 // random value
  #define VARINT_DECODED_VALUE_1 78167177906 // random value
  #define DATA_HASH_TEXT "X-CASH Proof Of Stake"
  #define MESSAGE "{\"MESSAGE\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\"}"
  #define SEND_DATA_SOCKET_TEST_MESSAGE "{\r\n \"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\",\r\n}"
  #define DATABASE_COLLECTION_STATISTICS_TEST_DATA "{\"username\":\"XCASH\",\"most_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_total_rounds\":\"5\",\"best_block_verifier_online_percentage_delegate_name\":\"DELEGATE_NAME\",\"best_block_verifier_online_percentage\":\"10\",\"most_block_producer_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_block_producer_total_rounds\":\"15\"}"
 
  #define CHECK_RESET_VARIABLES_ON_THE_HEAP(FUNCTION_NAME, CODE) \
  previous_system_memory_usage = get_program_memory_usage(process_id_file); \
  for (count2 = 0; count2 <= 1000; count2++) \
  { \
    fprintf(stderr,"Testing %s: %zu / 1000",FUNCTION_NAME,count2); \
    fprintf(stderr,"\r"); \
    CODE; \
    memset(error_message.function[0],0,strlen(error_message.function[0])); \
    memset(error_message.data[0],0,strlen(error_message.data[0])); \
    error_message.total = 0; \
    if (count2 == 0) \
    { \
      current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage; \
    } \
    if (count2 == 10) \
    { \
      current_system_memory_usage = get_program_memory_usage(process_id_file); \
      if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0) \
      { \
        fprintf(stderr,"\033[1;31mFAILED! Test for %s     \033[0m\n",FUNCTION_NAME); \
        break; \
      } \
    } \
    if (count2 == 100) \
    { \
      current_system_memory_usage = get_program_memory_usage(process_id_file); \
      if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0) \
      { \
        fprintf(stderr,"\033[1;31mFAILED! Test for %s     \033[0m\n",FUNCTION_NAME); \
        break; \
      } \
    } \
    if (count2 == 1000) \
    { \
      current_system_memory_usage = get_program_memory_usage(process_id_file); \
      if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0) \
      { \
        fprintf(stderr,"\033[1;31mFAILED! Test for %s     \033[0m\n",FUNCTION_NAME); \
        break; \
      } \
      else \
      { \
        fprintf(stderr,"\033[1;32mPASSED! Test for %s     \033[0m\n",FUNCTION_NAME); \
        count_test++; \
      } \
    } \
  }

  #define RESET_INVALID_RESERVE_PROOFS_DATA \
  for (count = 0; (int)count < invalid_reserve_proofs.count; count++) \
  { \
    pointer_reset(invalid_reserve_proofs.block_verifier_public_address[count]); \
    pointer_reset(invalid_reserve_proofs.public_address_created_reserve_proof[count]); \
    pointer_reset(invalid_reserve_proofs.public_address_voted_for[count]); \
    pointer_reset(invalid_reserve_proofs.reserve_proof[count]); \
  } \
  invalid_reserve_proofs.count = 0;

  #define VARINT_DECODE_CODE varint_decode(VARINT_ENCODED_VALUE_1);
  
  #define VARINT_ENCODE_CODE \
  memset(result_test,0,sizeof(result_test)); \
  varint_encode(VARINT_DECODED_VALUE_1,result_test,sizeof(result_test));

  #define GET_GENERATED_SUPPLY_CODE get_generated_supply(521850);

  #define NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_CODE \
  RESET_BLOCKCHAIN_DATA; \
  network_block_string_to_blockchain_data(NETWORK_BLOCK,NETWORK_BLOCK_HEIGHT,BLOCK_VERIFIERS_TOTAL_AMOUNT);
  
  #define VERIFY_NETWORK_BLOCK_DATA_CODE verify_network_block_data(0,0,NETWORK_BLOCK_HEIGHT,BLOCK_VERIFIERS_TOTAL_AMOUNT);

  #define BLOCKCHAIN_DATA_TO_NETWORK_BLOCK_STRING_CODE blockchain_data_to_network_block_string(data_test,BLOCK_VERIFIERS_TOTAL_AMOUNT);

  #define GET_DATABASE_DATA_HASH_CODE get_database_data_hash(data_test,database_name,DATABASE_COLLECTION_TEST);

  #define GET_DATABASE_DATA_CODE get_database_data(data_test,database_name,DATABASE_COLLECTION_TEST);

  #define COUNT_DOCUMENTS_IN_COLLECTION_CODE count_documents_in_collection(database_name,DATABASE_COLLECTION_TEST,MESSAGE);

  #define COUNT_ALL_DOCUMENTS_IN_COLLECTION_CODE count_all_documents_in_collection(database_name,DATABASE_COLLECTION_TEST);

  #define DELETE_DOCUMENT_FROM_COLLECTION_CODE \
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,MESSAGE); \
  delete_document_from_collection(database_name,DATABASE_COLLECTION_TEST,MESSAGE);

  #define DELETE_COLLECTION_FROM_DATABASE_CODE \
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,MESSAGE); \
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST);

  #define DELETE_DATABASE_CODE \
  insert_document_into_collection_json(DATABASE_NAME_TEST,DATABASE_COLLECTION_TEST,MESSAGE); \
  delete_database(DATABASE_NAME_TEST);

  #define INSERT_DOCUMENT_INTO_COLLECTION_JSON_CODE insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,MESSAGE);

  #define INSERT_MULTIPLE_DOCUMENTS_INTO_COLLECTION_JSON_CODE insert_multiple_documents_into_collection_json(database_name,DATABASE_COLLECTION_TEST,MESSAGE,sizeof(MESSAGE)-1);

  #define READ_DOCUMENT_FROM_COLLECTION_CODE \
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE)); \
  read_document_from_collection(database_name,DATABASE_COLLECTION_TEST,MESSAGE,data_test);

  #define READ_DOCUMENT_FIELD_FROM_COLLECTION_CODE \
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE)); \
  read_document_field_from_collection(database_name,DATABASE_COLLECTION_TEST,MESSAGE,"MESSAGE",data_test);

  #define READ_DOCUMENT_ALL_FIELDS_FROM_COLLECTION_CODE \
  for (count = 0; count < 14; count++) \
  { \
    memset(database_data.item[count],0,strnlen(database_data.item[count],BUFFER_SIZE)); \
    memset(database_data.value[count],0,strnlen(database_data.value[count],BUFFER_SIZE)); \
  } \
  read_document_all_fields_from_collection(database_name,DATABASE_COLLECTION_TEST,"{\"username\":\"XCASH\"}",&database_data);

  #define READ_MULTIPLE_DOCUMENTS_ALL_FIELDS_FROM_COLLECTION_CODE \
  for (count = 0; count < 2; count++) \
  { \
    for (counter = 0; counter < 7; counter++) \
    { \
      memset(database_multiple_documents_fields.item[count][counter],0,strnlen(database_multiple_documents_fields.item[count][counter],BUFFER_SIZE)); \
      memset(database_multiple_documents_fields.value[count][counter],0,strnlen(database_multiple_documents_fields.value[count][counter],BUFFER_SIZE)); \
    } \
  } \
  read_multiple_documents_all_fields_from_collection(database_name,DATABASE_COLLECTION_TEST,"",&database_multiple_documents_fields,1,2,0,"");

  #define DATABASE_DOCUMENT_PARSE_JSON_DATA_CODE \
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE)); \
  memcpy(data_test,DATABASE_COLLECTION_STATISTICS_PARSE_JSON_TEST_DATA,sizeof(DATABASE_COLLECTION_STATISTICS_PARSE_JSON_TEST_DATA)-1); \
  for (count = 0; count < 14; count++) \
  { \
    memset(database_data.item[count],0,strnlen(database_data.item[count],BUFFER_SIZE)); \
    memset(database_data.value[count],0,strnlen(database_data.value[count],BUFFER_SIZE)); \
  } \
  database_data.count = 7; \
  database_document_parse_json_data(data_test,&database_data);

  #define DATABASE_MULTIPLE_DOCUMENTS_PARSE_JSON_DATA_CODE \
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE)); \
  memcpy(data_test,DATABASE_COLLECTION_STATISTICS_PARSE_JSON_TEST_DATA,sizeof(DATABASE_COLLECTION_STATISTICS_PARSE_JSON_TEST_DATA)-1); \
  for (count = 0; count < 2; count++) \
  { \
    for (counter = 0; counter < 7; counter++) \
    { \
      memset(database_multiple_documents_fields.item[count][counter],0,strnlen(database_multiple_documents_fields.item[count][counter],BUFFER_SIZE)); \
      memset(database_multiple_documents_fields.value[count][counter],0,strnlen(database_multiple_documents_fields.value[count][counter],BUFFER_SIZE)); \
    } \
  } \
  database_multiple_documents_fields.document_count = 0; \
  database_multiple_documents_fields.database_fields_count = 7; \
  database_multiple_documents_parse_json_data(data_test,&database_multiple_documents_fields,0); \
  database_multiple_documents_fields.document_count = 1; \
  database_multiple_documents_fields.database_fields_count = 7; \
  database_multiple_documents_parse_json_data(data_test,&database_multiple_documents_fields,1);

  #define UPDATE_DOCUMENT_FROM_COLLECTION_CODE update_document_from_collection(database_name,DATABASE_COLLECTION_TEST,MESSAGE,MESSAGE);

  #define UPDATE_ALL_DOCUMENTS_FROM_COLLECTION_CODE update_all_documents_from_collection(database_name,DATABASE_COLLECTION_TEST,MESSAGE);

  #define UPDATE_DOCUMENT_FROM_COLLECTION_CODE update_document_from_collection(database_name,DATABASE_COLLECTION_TEST,MESSAGE,MESSAGE);

  #define UPDATE_DOCUMENT_FROM_COLLECTION_CODE update_document_from_collection(database_name,DATABASE_COLLECTION_TEST,MESSAGE,MESSAGE);

  #define UPDATE_DOCUMENT_FROM_COLLECTION_CODE update_document_from_collection(database_name,DATABASE_COLLECTION_TEST,MESSAGE,MESSAGE);

  #define CHECK_IF_BLOCKCHAIN_IS_FULLY_SYNCED_CODE check_if_blockchain_is_fully_synced();

  #define GET_BLOCK_TEMPLATE_CODE \
  memset(data_test,0,sizeof(data_test)); \
  get_block_template(data_test);

  #define SUBMIT_BLOCK_TEMPLATE_CODE submit_block_template("XCASH_DPOPS");

  #define GET_CURRENT_BLOCK_HEIGHT_CODE \
  memset(data_test,0,sizeof(data_test)); \
  get_current_block_height(data_test);

  #define GET_BLOCK_RESERVE_BYTE_DATA_HASH_CODE \
  memset(data_test,0,sizeof(data_test)); \
  get_block_reserve_byte_data_hash(data_test,"440875");

  #define GET_PREVIOUS_BLOCK_HASH_CODE \
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE)); \
  get_previous_block_hash(data_test);

  #define GET_PREVIOUS_BLOCK_INFORMATION_CODE \
  memset(data_test,0,sizeof(data_test)); \
  memset(result_test,0,sizeof(result_test)); \
  memset(data3,0,sizeof(data3)); \
  get_previous_block_information(result_test,data_test,data3);

  #define SEND_HTTP_REQUEST_CODE \
  memset(data_test,0,sizeof(data_test)); \
  send_http_request(data_test,XCASH_DPOPS_delegates_IP_address,"/json_rpc",xcash_wallet_port,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,GET_PUBLIC_ADDRESS_DATA,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SEND_DATA_SOCKET_CODE \
  memset(data_test,0,sizeof(data_test)); \
  memcpy(data_test,SEND_DATA_SOCKET_TEST_MESSAGE,sizeof(SEND_DATA_SOCKET_TEST_MESSAGE)-1); \
  sign_data(data_test); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,data_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SEND_AND_RECEIVE_DATA_SOCKET_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memset(data_test,0,sizeof(data_test)); \
  memcpy(data_test,SEND_DATA_SOCKET_TEST_MESSAGE,sizeof(SEND_DATA_SOCKET_TEST_MESSAGE)-1); \
  sign_data(data_test); \
  send_and_receive_data_socket(result_test,sizeof(result_test),XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,(const char*)data_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SIGN_DATA_USING_WALLET_SIGNING_KEY_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,"{\r\n \"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\",\r\n}",60); \
  sign_data(result_test);

  #define VERIFY_DATA_USING_WALLET_SIGNING_KEY_CODE verify_data(result_test,1);

  #define SIGN_DATA_USING_ECDSA_BLOCK_VERIFIER_SIGNING_KEY_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,"{\r\n \"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA_2\",\r\n}",62); \
  sign_data(result_test);

  #define VERIFY_DATA_USING_ECDSA_BLOCK_VERIFIER_SIGNING_KEY_CODE verify_data(result_test,1);

  #define VALIDATE_DATA_CODE validate_data(validate_data_settings[(rand() % ((sizeof(validate_data_settings)/sizeof(validate_data_settings[0])) - 0 + 1)) + 0]);

  #define GET_PUBLIC_ADDRESS_CODE get_public_address();

  #define CHECK_RESERVE_PROOFS_CODE \
  memset(data_test,0,sizeof(data_test)); \
  check_reserve_proofs(data_test,CHECK_RESERVE_PROOF_TEST_PUBLIC_ADDRESS,CHECK_RESERVE_PROOF_TEST_RESERVE_PROOF);

  #define ORGANIZE_DELEGATES_CODE \
  RESET_DELEGATES_STRUCT; \
  organize_delegates(delegates);

  #define PARSE_JSON_DATA_CODE \
  memcpy(result_test,"{\r\n \"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\",\r\n}",60);
  parse_json_data(result_test,"message_settings",data_test,sizeof(data_test));

  #define RANDOM_STRING_CODE \
  memset(result_test,0,sizeof(result_test)); \
  random_string(result_test,RANDOM_STRING_LENGTH);

  #define PARSE_RESERVE_BYTES_DATA_CODE \
  memset(result_test,0,sizeof(result_test)); \
  parse_reserve_bytes_data(result_test,data_test,5);

  #define CREATE_JSON_DATA_FROM_DATABASE_DOCUMENT_ARRAY_CODE \
  memset(result_test,0,sizeof(result_test)); \
  create_json_data_from_database_document_array(&database_data,result_test,"");

  #define CREATE_JSON_DATA_FROM_DATABASE_MULTIPLE_DOCUMENTS_ARRAY_CODE \
  memset(data_test,0,sizeof(data_test)); \
  create_json_data_from_database_multiple_documents_array(&database_multiple_documents_fields,data_test,"");

  #define CREATE_JSON_DATA_FROM_DELEGATES_ARRAY_CODE \
  memset(result_test,0,sizeof(result_test)); \
  create_json_data_from_delegates_array(delegates,result_test,"");

  #define CREATE_JSON_DATA_FROM_VOTES_ARRAY_CODE \
  memset(result_test,0,sizeof(result_test)); \
  create_json_data_from_votes_array(votes,result_test,"");

  #define STRING_COUNT_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,TEST_OUTLINE,sizeof(TEST_OUTLINE)-1); \
  string_count(result_test,"-");

  #define PARSE_HTTP_RESPONSE_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,HTTP_RESPONSE_DATA,sizeof(HTTP_RESPONSE_DATA)-1); \
  parse_http_response(result_test);

  #define STRING_REPLACE_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,"{\r\n \"MESSAGE\": \"string_replace_test\",\r\n}",40); \
  string_replace(result_test,sizeof(result_test),"string_replace_test","string_replace");

  #define CREATE_RANDOM_VRF_KEYS_CODE create_random_VRF_keys((unsigned char*)vrf_public_key,(unsigned char*)vrf_secret_key);

  #define CRYPTO_VRF_IS_VALID_KEY_CODE count = crypto_vrf_is_valid_key((const unsigned char*)vrf_public_key);

  #define CRYPTO_VRF_KEYPAIR_FROM_SEED_CODE crypto_vrf_keypair_from_seed((unsigned char*)vrf_public_key, (unsigned char*)vrf_secret_key, (const unsigned char*)data2);

  #define CRYPTO_VRF_PROVE_CODE crypto_vrf_prove((unsigned char*)vrf_proof,(const unsigned char*)vrf_secret_key,alpha_string,sizeof(alpha_string));

  #define CRYPTO_VRF_PROOF_TO_HASH_CODE crypto_vrf_proof_to_hash((unsigned char*)vrf_beta,(const unsigned char*)vrf_proof);

  #define CRYPTO_VRF_VERIFY_CODE count = crypto_vrf_verify((unsigned char*)vrf_beta,(const unsigned char*)vrf_public_key,(const unsigned char*)vrf_proof,alpha_string,sizeof(alpha_string));

  #define CRYPTO_HASH_SHA512_CODE crypto_hash_sha512((unsigned char*)data_test,(const unsigned char*)DATA_HASH_TEXT,sizeof(DATA_HASH_TEXT)-1);

  #define SIGN_NETWORK_BLOCK_STRING_CODE sign_network_block_string(data_test,"X-CASH");

  #define SYNC_CHECK_RESERVE_PROOFS_DATABASE_FROM_A_RANDOM_BLOCK_VERIFIER_CODE sync_check_reserve_proofs_database(1);

  #define SYNC_CHECK_RESERVE_BYTES_DATABASE_FROM_A_RANDOM_BLOCK_VERIFIER_CODE sync_check_reserve_bytes_database(1,0);

  #define SYNC_CHECK_DELEGATES_DATABASE_FROM_A_RANDOM_BLOCK_VERIFIER_CODE sync_check_delegates_database(1);

  #define SYNC_CHECK_STATISTICS_DATABASE_FROM_A_RANDOM_BLOCK_VERIFIER_CODE sync_check_statistics_database(1);

  #define SYNC_CHECK_RESERVE_PROOFS_DATABASE_FROM_A_RANDOM_NETWORK_DATA_NODE_CODE sync_check_reserve_proofs_database(3);

  #define SYNC_CHECK_RESERVE_BYTES_DATABASE_FROM_A_RANDOM_NETWORK_DATA_NODE_CODE sync_check_reserve_bytes_database(3,0);

  #define SYNC_CHECK_DELEGATES_DATABASE_FROM_A_RANDOM_NETWORK_DATA_NODE_CODE sync_check_delegates_database(3);

  #define SYNC_CHECK_STATISTICS_DATABASE_FROM_A_RANDOM_NETWORK_DATA_NODE_CODE sync_check_statistics_database(3);

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS_TEST_DATA)-1); \
  sign_data(result_test); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_NETWORK_DATA_NODES_BLOCK_VERIFIERS_CURRENT_TIME_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_NETWORK_DATA_NODES_BLOCK_VERIFIERS_CURRENT_TIME_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_NETWORK_DATA_NODES_BLOCK_VERIFIERS_CURRENT_TIME_TEST_DATA)-1); \
  sign_data(result_test); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_ONLINE_STATUS_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_ONLINE_STATUS_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_ONLINE_STATUS_TEST_DATA)-1); \
  sign_data(result_test); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_START_BLOCK_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_START_BLOCK_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_START_BLOCK_TEST_DATA)-1); \
  sign_data(result_test); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_CREATE_NEW_BLOCK_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_CREATE_NEW_BLOCK_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_CREATE_NEW_BLOCK_TEST_DATA)-1); \
  sign_data(result_test); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIER_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIER_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIER_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK_TEST_DATA)-1); \
  sign_data(result_test); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_TEST_DATA)-1); \
  sign_data(result_test); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NODE_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NODE_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NODE_TEST_DATA)-1); \
  sign_data(result_test); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA_TEST_DATA)-1); \
  sign_data(result_test); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE_TEST_DATA)-1); \
  sign_data(result_test); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SYNC_RESERVE_PROOFS_DATABASE_CODE sync_reserve_proofs_database(0,XCASH_DPOPS_delegates_IP_address);

  #define SYNC_RESERVE_BYTES_DATABASE_CODE sync_reserve_bytes_database(0,1,XCASH_DPOPS_delegates_IP_address);

  #define SYNC_DELEGATES_DATABASE_CODE sync_delegates_database(0,XCASH_DPOPS_delegates_IP_address);

  #define SYNC_STATISTICS_DATABASE_CODE sync_statistics_database(0,XCASH_DPOPS_delegates_IP_address);

  #define SYNC_ALL_BLOCK_VERIFIERS_LIST_FOR_BLOCK_VERIFIERS_CODE sync_all_block_verifiers_list(1,1);

  #define SYNC_ALL_BLOCK_VERIFIERS_LIST_FOR_NETWORK_DATA_NODES_CODE sync_all_block_verifiers_list(1,1);

  #define SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_TEST_DATA)-1); \
  sign_data(result_test); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST_TEST_DATA)-1); \
  sign_data(result_test); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_NETWORK_DATA_NODES_TO_NETWORK_DATA_NODES_DATABASE_SYNC_CHECK_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_NETWORK_DATA_NODES_TO_NETWORK_DATA_NODES_DATABASE_SYNC_CHECK_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_NETWORK_DATA_NODES_TO_NETWORK_DATA_NODES_DATABASE_SYNC_CHECK_TEST_DATA)-1); \
  sign_data(result_test); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATABASE_HASH_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATABASE_HASH_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATABASE_HASH_TEST_DATA)-1); \
  sign_data(result_test); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_CHECK_IF_CURRENT_BLOCK_VERIFIER_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_CHECK_IF_CURRENT_BLOCK_VERIFIER_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_CHECK_IF_CURRENT_BLOCK_VERIFIER_TEST_DATA)-1); \
  sign_data(result_test); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_TEST_DATA)-1); \
  sign_data(result_test); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA)-1); \
  sign_data(result_test); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_TEST_DATA)-1); \
  sign_data(result_test); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA)-1); \
  sign_data(result_test); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_TEST_DATA)-1); \
  sign_data(result_test); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA)-1); \
  sign_data(result_test); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE_TEST_DATA)-1); \
  sign_data(result_test); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE_TEST_DATA)-1); \
  sign_data(result_test); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define CHECK_RESERVE_PROOFS_TIMER_CREATE_MESSAGE_CODE \
  memset(data_test,0,sizeof(data_test)); \
  check_reserve_proofs_timer_create_message(data_test);

  #define CHECK_RESERVE_PROOFS_TIMER_GET_DATABASE_DATA_CODE check_reserve_proofs_timer_get_database_data(1);

  #define CHECK_RESERVE_PROOFS_TIMER_UPDATE_DELEGATES_TOTAL_VOTE_COUNT_CODE \
  delete_database(database_name); \
  INITIALIZE_DATABASE_DATA(2); \
  RESET_INVALID_RESERVE_PROOFS_DATA; \
  invalid_reserve_proofs.block_verifier_public_address[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char)); \
  invalid_reserve_proofs.public_address_created_reserve_proof[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char)); \
  invalid_reserve_proofs.public_address_voted_for[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char)); \
  invalid_reserve_proofs.reserve_proof[0] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char)); \
  invalid_reserve_proofs.block_verifier_public_address[1] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char)); \
  invalid_reserve_proofs.public_address_created_reserve_proof[1] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char)); \
  invalid_reserve_proofs.public_address_voted_for[1] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char)); \
  invalid_reserve_proofs.reserve_proof[1] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char)); \
  memcpy(invalid_reserve_proofs.block_verifier_public_address[0],TEST_WALLET_6,XCASH_WALLET_LENGTH); \
  memcpy(invalid_reserve_proofs.public_address_voted_for[0],TEST_WALLET_1,XCASH_WALLET_LENGTH); \
  memcpy(invalid_reserve_proofs.public_address_created_reserve_proof[0],TEST_WALLET_1,XCASH_WALLET_LENGTH); \
  invalid_reserve_proofs.reserve_proof_amount[0] = 120000000; \
  memcpy(invalid_reserve_proofs.reserve_proof[0],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti",537); \
  memcpy(invalid_reserve_proofs.block_verifier_public_address[1],TEST_WALLET_1,XCASH_WALLET_LENGTH); \
  memcpy(invalid_reserve_proofs.public_address_voted_for[1],TEST_WALLET_6,XCASH_WALLET_LENGTH); \
  memcpy(invalid_reserve_proofs.public_address_created_reserve_proof[1],TEST_WALLET_11,XCASH_WALLET_LENGTH); \
  invalid_reserve_proofs.reserve_proof_amount[1] = 1000000000; \
  memcpy(invalid_reserve_proofs.reserve_proof[1],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NTiwbZLCstwWzpKy8J6J8M1uq9qf5PbWha7C68N8V7qshnzUP113UoSQuy78bpD5CeY2H3ViztSqDBFmmMiNu6uCQGkbu4NrZhZk2T7y7T1ixLCAxYfKRtd2pAAbuAWeBVCNzvmKB8YaneMgZ3mH113kXRg67xZRBtVcawf1CTYHCVuhpur8F6nAGxwFeNoRAdr6rnGwa1SUVdqrXypfmJJyzpPUGTSriAJqtRSBKJqAgQDPz4aV99aLMHHKWAv3eKaGAvBZ3obk2GuVBnHeUUEHAihQ5ECndKS4dw8YxMogdY9o8PvrhZgt2gnX4YrUiF5VbGBcBUsxbag1Ak9oZfTH2d1WeyMntcv1Mb28Nzc73UvMbck3VyBedN57gV1LS8WNV1SeHAFBiGgeBcuveGee9VZYQvGRfiWzkJf9JfYWoQ4dyxh13BrXpp3rF1nWv3UXxMrJJX8SRtQy5zrLAywENZ",538); \
  invalid_reserve_proofs.count = 2; \
  memset(result_test,0,sizeof(result_test)); \
  memset(data_test,0,sizeof(data_test)); \
  check_reserve_proofs_timer_update_delegates_total_vote_count(0);

  #define CHECK_RESERVE_PROOFS_TIMER_UPDATE_DELEGATES_SCORE_CODE \
  delete_database(database_name); \
  INITIALIZE_DATABASE_DATA(2); \
  RESET_INVALID_RESERVE_PROOFS_DATA; \
  invalid_reserve_proofs.block_verifier_public_address[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char)); \
  invalid_reserve_proofs.public_address_created_reserve_proof[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char)); \
  invalid_reserve_proofs.public_address_voted_for[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char)); \
  invalid_reserve_proofs.reserve_proof[0] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char)); \
  memcpy(invalid_reserve_proofs.block_verifier_public_address[0],TEST_WALLET_1,XCASH_WALLET_LENGTH); \
  memcpy(invalid_reserve_proofs.public_address_voted_for[0],TEST_WALLET_6,XCASH_WALLET_LENGTH); \
  memcpy(invalid_reserve_proofs.public_address_created_reserve_proof[0],TEST_WALLET_6,XCASH_WALLET_LENGTH); \
  invalid_reserve_proofs.reserve_proof_amount[0] = 0; \
  memcpy(invalid_reserve_proofs.reserve_proof[0],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr1p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti",537); \
  invalid_reserve_proofs.count = 1; \
  memset(data_test,0,sizeof(data_test)); \
  check_reserve_proofs_timer_update_delegates_score(0);

  #define CHECK_RESERVE_PROOFS_TIMER_UPDATE_DATABASE_CODE \
  delete_database(database_name); \
  INITIALIZE_DATABASE_DATA(2); \
  RESET_INVALID_RESERVE_PROOFS_DATA; \
  invalid_reserve_proofs.block_verifier_public_address[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char)); \
  invalid_reserve_proofs.public_address_created_reserve_proof[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char)); \
  invalid_reserve_proofs.public_address_voted_for[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char)); \
  invalid_reserve_proofs.reserve_proof[0] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char)); \
  invalid_reserve_proofs.block_verifier_public_address[1] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char)); \
  invalid_reserve_proofs.public_address_created_reserve_proof[1] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char)); \
  invalid_reserve_proofs.public_address_voted_for[1] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char)); \
  invalid_reserve_proofs.reserve_proof[1] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char)); \
  memcpy(invalid_reserve_proofs.block_verifier_public_address[0],TEST_WALLET_6,XCASH_WALLET_LENGTH); \
  memcpy(invalid_reserve_proofs.public_address_voted_for[0],TEST_WALLET_1,XCASH_WALLET_LENGTH); \
  memcpy(invalid_reserve_proofs.public_address_created_reserve_proof[0],TEST_WALLET_1,XCASH_WALLET_LENGTH); \
  invalid_reserve_proofs.reserve_proof_amount[0] = 120000000; \
  memcpy(invalid_reserve_proofs.reserve_proof[0],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti",537); \
  memcpy(invalid_reserve_proofs.block_verifier_public_address[1],TEST_WALLET_1,XCASH_WALLET_LENGTH); \
  memcpy(invalid_reserve_proofs.public_address_voted_for[1],TEST_WALLET_6,XCASH_WALLET_LENGTH); \
  memcpy(invalid_reserve_proofs.public_address_created_reserve_proof[1],TEST_WALLET_11,XCASH_WALLET_LENGTH); \
  invalid_reserve_proofs.reserve_proof_amount[1] = 1000000000; \
  memcpy(invalid_reserve_proofs.reserve_proof[1],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NTiwbZLCstwWzpKy8J6J8M1uq9qf5PbWha7C68N8V7qshnzUP113UoSQuy78bpD5CeY2H3ViztSqDBFmmMiNu6uCQGkbu4NrZhZk2T7y7T1ixLCAxYfKRtd2pAAbuAWeBVCNzvmKB8YaneMgZ3mH113kXRg67xZRBtVcawf1CTYHCVuhpur8F6nAGxwFeNoRAdr6rnGwa1SUVdqrXypfmJJyzpPUGTSriAJqtRSBKJqAgQDPz4aV99aLMHHKWAv3eKaGAvBZ3obk2GuVBnHeUUEHAihQ5ECndKS4dw8YxMogdY9o8PvrhZgt2gnX4YrUiF5VbGBcBUsxbag1Ak9oZfTH2d1WeyMntcv1Mb28Nzc73UvMbck3VyBedN57gV1LS8WNV1SeHAFBiGgeBcuveGee9VZYQvGRfiWzkJf9JfYWoQ4dyxh13BrXpp3rF1nWv3UXxMrJJX8SRtQy5zrLAywENZ",538); \
  invalid_reserve_proofs.count = 2; \
  memset(result_test,0,sizeof(result_test)); \
  memset(data_test,0,sizeof(data_test)); \
  check_reserve_proofs_timer_update_database();

  #define SELECT_RANDOM_UNIQUE_RESERVE_PROOF_CODE \
  memset(reserve_proof.block_verifier_public_address,0,sizeof(reserve_proof.block_verifier_public_address)); \
  memset(reserve_proof.public_address_created_reserve_proof,0,sizeof(reserve_proof.public_address_created_reserve_proof)); \
  memset(reserve_proof.public_address_voted_for,0,sizeof(reserve_proof.public_address_voted_for)); \
  memset(reserve_proof.reserve_proof_amount,0,sizeof(reserve_proof.reserve_proof_amount)); \
  memset(reserve_proof.reserve_proof,0,sizeof(reserve_proof.reserve_proof)); \
  select_random_unique_reserve_proof(&reserve_proof);

  #define SEND_INVALID_RESERVE_PROOF_TO_BLOCK_VERIFIERS_CODE send_invalid_reserve_proof_to_block_verifiers(&reserve_proof);

  #define BLOCK_VERIFIERS_ADD_RESERVE_PROOF_CHECK_IF_DATA_IS_VALID_CODE \
  memset(reserve_proof.block_verifier_public_address,0,sizeof(reserve_proof.block_verifier_public_address)); \
  memset(reserve_proof.public_address_created_reserve_proof,0,sizeof(reserve_proof.public_address_created_reserve_proof)); \
  memset(reserve_proof.public_address_voted_for,0,sizeof(reserve_proof.public_address_voted_for)); \
  memset(reserve_proof.reserve_proof_amount,0,sizeof(reserve_proof.reserve_proof_amount)); \
  memset(reserve_proof.reserve_proof,0,sizeof(reserve_proof.reserve_proof)); \
  memcpy(result_test,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF|" TEST_WALLET "|ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti|" TEST_WALLET_1 "|SigV1N2NWu1Yzg4aVrhJUjZVcRZ52CcgUghX7x5Fn9W5drC4g8B9tZn7q4yJEKbiJMRFeNRPmSPT3rBSVkJnFWAGK5PA3|",872); \
  block_verifiers_add_reserve_proof_check_if_data_is_valid(result_test,&reserve_proof);

  #define ADD_RESERVE_PROOF_REMOVE_PREVIOUS_VOTE_CODE \
  delete_database(database_name); \
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_1); \
  insert_document_into_collection_json(database_name,"reserve_proofs_1",DATABASE_COLLECTION_RESERVE_PROOFS_DATA_1); \
  memcpy(result_test,"{\"public_address_created_reserve_proof\":\"" TEST_WALLET_1 "\"}",141); \
  add_reserve_proof_remove_previous_vote((const char*)result_test);

  #define UPDATE_BLOCK_VERIFIERS_LIST_CODE \
  for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++) \
  { \
    memset(previous_block_verifiers_list.block_verifiers_name[count],0,sizeof(previous_block_verifiers_list.block_verifiers_name[count])); \
    memset(previous_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_public_address[count])); \
    memset(previous_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(previous_block_verifiers_list.block_verifiers_public_key[count])); \
    memset(previous_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_IP_address[count])); \
    memset(current_block_verifiers_list.block_verifiers_name[count],0,sizeof(current_block_verifiers_list.block_verifiers_name[count])); \
    memset(current_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_public_address[count])); \
    memset(current_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(current_block_verifiers_list.block_verifiers_public_key[count])); \
    memset(current_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_IP_address[count])); \
    memset(next_block_verifiers_list.block_verifiers_name[count],0,sizeof(next_block_verifiers_list.block_verifiers_name[count])); \
    memset(next_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_public_address[count])); \
    memset(next_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(next_block_verifiers_list.block_verifiers_public_key[count])); \
    memset(next_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_IP_address[count])); \
  } \
  memcpy(previous_block_verifiers_list.block_verifiers_name[0],"delegate_1",10); \
  memcpy(previous_block_verifiers_list.block_verifiers_public_address[0],TEST_WALLET,XCASH_WALLET_LENGTH); \
  memcpy(previous_block_verifiers_list.block_verifiers_IP_address[0],XCASH_DPOPS_delegates_IP_address,9); \
  memcpy(previous_block_verifiers_list.block_verifiers_public_key[0],NEXT_BLOCK_VERIFIERS_PUBLIC_KEY,VRF_PUBLIC_KEY_LENGTH); \
  memcpy(current_block_verifiers_list.block_verifiers_name[0],"delegate_2",10); \
  memcpy(current_block_verifiers_list.block_verifiers_public_address[0],TEST_WALLET,XCASH_WALLET_LENGTH); \
  memcpy(current_block_verifiers_list.block_verifiers_IP_address[0],XCASH_DPOPS_delegates_IP_address,9); \
  memcpy(current_block_verifiers_list.block_verifiers_public_key[0],NEXT_BLOCK_VERIFIERS_PUBLIC_KEY,VRF_PUBLIC_KEY_LENGTH); \
  memcpy(next_block_verifiers_list.block_verifiers_name[0],"delegate_3",10); \
  memcpy(next_block_verifiers_list.block_verifiers_public_address[0],TEST_WALLET,XCASH_WALLET_LENGTH); \
  memcpy(next_block_verifiers_list.block_verifiers_IP_address[0],XCASH_DPOPS_delegates_IP_address,9); \
  memcpy(next_block_verifiers_list.block_verifiers_public_key[0],NEXT_BLOCK_VERIFIERS_PUBLIC_KEY,VRF_PUBLIC_KEY_LENGTH); \
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_1); \
  update_block_verifiers_list();

  #define ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_CODE \
  for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++) \
  { \
    memset(previous_block_verifiers_list.block_verifiers_name[count],0,sizeof(previous_block_verifiers_list.block_verifiers_name[count])); \
    memset(previous_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_public_address[count])); \
    memset(previous_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(previous_block_verifiers_list.block_verifiers_public_key[count])); \
    memset(previous_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_IP_address[count])); \
    memset(current_block_verifiers_list.block_verifiers_name[count],0,sizeof(current_block_verifiers_list.block_verifiers_name[count])); \
    memset(current_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_public_address[count])); \
    memset(current_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(current_block_verifiers_list.block_verifiers_public_key[count])); \
    memset(current_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_IP_address[count])); \
    memset(next_block_verifiers_list.block_verifiers_name[count],0,sizeof(next_block_verifiers_list.block_verifiers_name[count])); \
    memset(next_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_public_address[count])); \
    memset(next_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(next_block_verifiers_list.block_verifiers_public_key[count])); \
    memset(next_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_IP_address[count])); \
  } \
  memcpy(previous_block_verifiers_list.block_verifiers_name[0],"delegate_1",10); \
  memcpy(previous_block_verifiers_list.block_verifiers_public_address[0],TEST_WALLET_1,XCASH_WALLET_LENGTH); \
  memcpy(previous_block_verifiers_list.block_verifiers_IP_address[0],XCASH_DPOPS_delegates_IP_address,9); \
  memcpy(previous_block_verifiers_list.block_verifiers_public_key[0],NEXT_BLOCK_VERIFIERS_PUBLIC_KEY,VRF_PUBLIC_KEY_LENGTH); \
  add_block_verifiers_round_statistics(XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT_TEST);

  #define ADD_ROUND_STATISTICS_CODE \
  delete_database(database_name); \
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_1 "\",\"total_vote_count\":\"120000000\",\"IP_address\":\"192.168.1.201\",\"delegate_name\":\"delegate_name_1\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"shared_delegate_status\":\"solo\",\"delegate_fee\":\"\",\"server_specs\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"1\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}"); \
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_20 "\",\"total_vote_count\":\"120000000\",\"IP_address\":\"192.168.1.201\",\"delegate_name\":\"delegate_name_2\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"shared_delegate_status\":\"solo\",\"delegate_fee\":\"\",\"server_specs\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"2\",\"block_verifier_online_total_rounds\":\"2\",\"block_verifier_online_percentage\":\"100\",\"block_producer_total_rounds\":\"1\",\"block_producer_block_heights\":\"\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}"); \
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_19 "\",\"total_vote_count\":\"120000000\",\"IP_address\":\"192.168.1.201\",\"delegate_name\":\"delegate_name_3\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"shared_delegate_status\":\"solo\",\"delegate_fee\":\"\",\"server_specs\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"3\",\"block_verifier_online_total_rounds\":\"1\",\"block_verifier_online_percentage\":\"33\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}"); \
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_18 "\",\"total_vote_count\":\"120000000\",\"IP_address\":\"192.168.1.201\",\"delegate_name\":\"delegate_name_4\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"shared_delegate_status\":\"solo\",\"delegate_fee\":\"\",\"server_specs\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"4\",\"block_verifier_online_total_rounds\":\"1\",\"block_verifier_online_percentage\":\"25\",\"block_producer_total_rounds\":\"2\",\"block_producer_block_heights\":\"\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}"); \
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_15 "\",\"total_vote_count\":\"120000000\",\"IP_address\":\"192.168.1.201\",\"delegate_name\":\"delegate_name_5\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"shared_delegate_status\":\"solo\",\"delegate_fee\":\"\",\"server_specs\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"5\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}"); \
  insert_document_into_collection_json(database_name,"statistics",DATABASE_COLLECTION_STATISTICS_DATA); \
  RESET_ERROR_MESSAGES; \
  add_round_statistics();

  #define CALCULATE_MAIN_NODES_ROLES_CODE \
  for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++) \
  { \
    memset(previous_block_verifiers_list.block_verifiers_name[count],0,sizeof(previous_block_verifiers_list.block_verifiers_name[count])); \
    memset(previous_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_public_address[count])); \
    memset(previous_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(previous_block_verifiers_list.block_verifiers_public_key[count])); \
    memset(previous_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_IP_address[count])); \
    memset(current_block_verifiers_list.block_verifiers_name[count],0,sizeof(current_block_verifiers_list.block_verifiers_name[count])); \
    memset(current_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_public_address[count])); \
    memset(current_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(current_block_verifiers_list.block_verifiers_public_key[count])); \
    memset(current_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_IP_address[count])); \
    memset(next_block_verifiers_list.block_verifiers_name[count],0,sizeof(next_block_verifiers_list.block_verifiers_name[count])); \
    memset(next_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_public_address[count])); \
    memset(next_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(next_block_verifiers_list.block_verifiers_public_key[count])); \
    memset(next_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_IP_address[count])); \
  } \
  for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++)
  { \
    memset(data_test,0,strlen(data_test)); \
    memcpy(data_test,"public_address_",15); \
    snprintf(data_test+15,BUFFER_SIZE-16,"%zu",count); \
    memcpy(current_block_verifiers_list.block_verifiers_name[count],"delegate_name_1",15); \
    memcpy(current_block_verifiers_list.block_verifiers_public_address[count],data_test,strnlen(data_test,sizeof(current_block_verifiers_list.block_verifiers_name[count]))); \
    memcpy(current_block_verifiers_list.block_verifiers_IP_address[count],XCASH_DPOPS_delegates_IP_address,9); \
    memcpy(current_block_verifiers_list.block_verifiers_public_key[count],NEXT_BLOCK_VERIFIERS_PUBLIC_KEY,VRF_PUBLIC_KEY_LENGTH); \
  } \
  calculate_main_nodes_roles();

  #define GET_DELEGATES_ONLINE_STATUS_CODE \
  delete_database(database_name); \
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_1 "\",\"total_vote_count\":\"120000000\",\"IP_address\":\"127.0.0.1\",\"delegate_name\":\"delegate_name_1\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"shared_delegate_status\":\"solo\",\"delegate_fee\":\"\",\"server_specs\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}"); \
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_1 "\",\"total_vote_count\":\"120000000\",\"IP_address\":\"\",\"delegate_name\":\"delegate_name_1\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"shared_delegate_status\":\"solo\",\"delegate_fee\":\"\",\"server_specs\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}"); \
  RESET_ERROR_MESSAGES; \
  get_delegates_online_status();

  #define DELEGATES_SERVER_FUNCTIONS_TEST_CODE send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_DELEGATES_WEBSITE_GET_STATISTICS_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_DELEGATES_WEBSITE_GET_STATISTICS_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_DELEGATES_WEBSITE_GET_STATISTICS_TEST_DATA)-1); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_TEST_DATA)-1); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_STATISTICS_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_STATISTICS_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_STATISTICS_TEST_DATA)-1); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_INFORMATION_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_INFORMATION_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_INFORMATION_TEST_DATA)-1); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_VOTERS_LIST_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_VOTERS_LIST_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_VOTERS_LIST_TEST_DATA)-1); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_GET_ROUND_STATISTICS_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_GET_ROUND_STATISTICS_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_GET_ROUND_STATISTICS_TEST_DATA)-1); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_SHARED_DELEGATES_WEBSITE_GET_STATISTICS_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_SHARED_DELEGATES_WEBSITE_GET_STATISTICS_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_SHARED_DELEGATES_WEBSITE_GET_STATISTICS_TEST_DATA)-1); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_GET_BLOCKS_FOUND_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_GET_BLOCKS_FOUND_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_GET_BLOCKS_FOUND_TEST_DATA)-1); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_INFORMATION_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_INFORMATION_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_INFORMATION_TEST_DATA)-1); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_TEST_DATA,sizeof(SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_TEST_DATA)-1); \
  send_data_socket(XCASH_DPOPS_delegates_IP_address,SEND_DATA_PORT,result_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);

  #define ADD_BLOCK_TO_BLOCKS_FOUND_CODE \
  sscanf(current_block_height, "%zu", &block_height); \
  block_height--; \
  snprintf(result_test,sizeof(result_test)-1,"%zu",block_height); \
  memcpy(data_test,"{\"block_height\":\"",17); \
  memcpy(data_test+strlen(data_test),result_test,strnlen(result_test,sizeof(data_test))); \
  memcpy(data_test+strlen(data_test),"\"}",2); \
  add_block_to_blocks_found();

  #define GET_DELEGATES_TOTAL_VOTERS_COUNT_CODE get_delegates_total_voters_count(xcash_wallet_public_address);

  #define GET_DELEGATES_TOTAL_VOTERS_CODE get_delegates_total_voters(voters);

  #define CALCULATE_BLOCK_REWARD_FOR_EACH_DELEGATE_CODE calculate_block_reward_for_each_delegate(1000000000000);

  #define PAYMENT_TIMER_SEND_PAYMENT_AND_UPDATE_DATABASES_CODE payment_timer_send_payment_and_update_databases(TEST_WALLET_1,"5000000000000","10000000000000","0000000000000000000000000000000000000000000000000000000000000000","0000000000000000000000000000000000000000000000000000000000000000");

  #define PAYMENT_TIMER_UPDATE_INACTIVITY_COUNT_CODE payment_timer_update_inactivity_count(TEST_WALLET_1,"0","0");


  // initialize the data
  if (process_id_file == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // initialize the database_document_fields struct 
  for (count = 0; count < 16; count++)
  {
    database_data.item[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));
    database_data.value[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));
    
    if (database_data.item[count] == NULL || database_data.value[count] == NULL)
    {
      color_print("Could not allocate the memory needed on the heap","red");
      exit(0);
    }
  }

  memcpy(database_data.item[0],"username",8);
  memcpy(database_data.item[1],"most_total_rounds_delegate_name",31);
  memcpy(database_data.item[2],"most_total_rounds",17);
  memcpy(database_data.item[3],"best_block_verifier_online_percentage_delegate_name",51);
  memcpy(database_data.item[4],"best_block_verifier_online_percentage",37);
  memcpy(database_data.item[5],"most_block_producer_total_rounds_delegate_name",46);
  memcpy(database_data.item[6],"most_block_producer_total_rounds",32);

  memcpy(database_data.value[0],"XCASH",5);  
  memcpy(database_data.value[1],"DELEGATE_NAME",13);
  memcpy(database_data.value[2],"5",sizeof(char));
  memcpy(database_data.value[3],"DELEGATE_NAME",13);
  memcpy(database_data.value[4],"10",2);
  memcpy(database_data.value[5],"DELEGATE_NAME",13);  
  memcpy(database_data.value[6],"15",2);

  database_data.count = 7;

  // initialize the database_multiple_documents_fields struct 
  for (count = 0; count < 2; count++)
  {
    for (counter = 0; counter < 7; counter++)
    {
      database_multiple_documents_fields.item[count][counter] = (char*)calloc(BUFFER_SIZE,sizeof(char));
      database_multiple_documents_fields.value[count][counter] = (char*)calloc(BUFFER_SIZE,sizeof(char));

      if (database_multiple_documents_fields.item[count][counter] == NULL || database_multiple_documents_fields.value[count][counter] == NULL)
      {
        color_print("Could not allocate the memory needed on the heap","red");
        exit(0);
      }
    }
  }

  memcpy(database_multiple_documents_fields.item[0][0],"username",8);
  memcpy(database_multiple_documents_fields.item[0][1],"most_total_rounds_delegate_name",31);
  memcpy(database_multiple_documents_fields.item[0][2],"most_total_rounds",17);
  memcpy(database_multiple_documents_fields.item[0][3],"best_block_verifier_online_percentage_delegate_name",51);
  memcpy(database_multiple_documents_fields.item[0][4],"best_block_verifier_online_percentage",37);
  memcpy(database_multiple_documents_fields.item[0][5],"most_block_producer_total_rounds_delegate_name",46);
  memcpy(database_multiple_documents_fields.item[0][6],"most_block_producer_total_rounds",32);
  memcpy(database_multiple_documents_fields.item[1][0],"username",8);
  memcpy(database_multiple_documents_fields.item[1][1],"most_total_rounds_delegate_name",31);
  memcpy(database_multiple_documents_fields.item[1][2],"most_total_rounds",17);
  memcpy(database_multiple_documents_fields.item[1][3],"best_block_verifier_online_percentage_delegate_name",51);
  memcpy(database_multiple_documents_fields.item[1][4],"best_block_verifier_online_percentage",37);
  memcpy(database_multiple_documents_fields.item[1][5],"most_block_producer_total_rounds_delegate_name",46);
  memcpy(database_multiple_documents_fields.item[1][6],"most_block_producer_total_rounds",32);

  memcpy(database_multiple_documents_fields.value[0][0],"XCASH",5);  
  memcpy(database_multiple_documents_fields.value[0][1],"DELEGATE_NAME",13);
  memcpy(database_multiple_documents_fields.value[0][2],"5",sizeof(char));
  memcpy(database_multiple_documents_fields.value[0][3],"DELEGATE_NAME",13);
  memcpy(database_multiple_documents_fields.value[0][4],"10",2);
  memcpy(database_multiple_documents_fields.value[0][5],"DELEGATE_NAME",13);  
  memcpy(database_multiple_documents_fields.value[0][6],"15",2);
  memcpy(database_multiple_documents_fields.value[1][0],"XCASH",5);  
  memcpy(database_multiple_documents_fields.value[1][1],"DELEGATE_NAME",13);
  memcpy(database_multiple_documents_fields.value[1][2],"5",sizeof(char));
  memcpy(database_multiple_documents_fields.value[1][3],"DELEGATE_NAME",13);
  memcpy(database_multiple_documents_fields.value[1][4],"10",2);
  memcpy(database_multiple_documents_fields.value[1][5],"DELEGATE_NAME",13);  
  memcpy(database_multiple_documents_fields.value[1][6],"15",2);

  database_multiple_documents_fields.document_count = 2;
  database_multiple_documents_fields.database_fields_count = 7;

  for (count = 0; count < 5; count++)
  {
    transactions[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));
  }

  // initialize the delegates struct 
  INITIALIZE_DELEGATES_STRUCT_TEST_DATA;

  // initialize the votes struct 
  INITIALIZE_VOTES_STRUCT_TEST_DATA;

  // initialize the voters struct
  INITIALIZE_VOTERS_STRUCT(count,MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE,"reset_variables_allocated_on_the_heap_test",data_test,current_date_and_time,current_UTC_date_and_time);

  // reset the variables
  memset(data_test,0,sizeof(data_test));
  memset(result_test,0,sizeof(result_test));
  memset(&string1_test,0,sizeof(string1_test)); 
  memset(string2_test,0,strnlen(string2_test,BUFFER_SIZE)); 
  count_test = 0;  
  
  // get the process_id_file
  memcpy(process_id_file,"/proc/",6);
  snprintf(process_id_file+6,BUFFER_SIZE-7,"%d",getpid());
  memcpy(process_id_file+strnlen(process_id_file,BUFFER_SIZE),"/status",7);

  // initilize the transactions
  memcpy(transactions[0],"f6458824e54ea5cddd80a6bb0105ecdd6d2248629482df2c0f989db3d46f6ebd",64);
  memcpy(transactions[1],"871eb7b29c72582572041c597ff092143031bfcef5fa1fa92808dacab2ba226f",64);
  memcpy(transactions[2],"72f97600db9d7522a2a39fc690d25e1cc9a17535064b08f81bd7424a51bba931",64);
  memcpy(transactions[3],"b180489867776c7e39c07ddb0d88609dac6f748dec554e2d96b6a168785bdb44",64);
  memcpy(transactions[4],"",sizeof(char));

  // set the current_round_part and current_round_part_backup_node
  memset(current_round_part,0,sizeof(current_round_part));
  memset(current_round_part_backup_node,0,sizeof(current_round_part_backup_node));
  memcpy(current_round_part,"1",sizeof(char));
  memcpy(current_round_part_backup_node,"0",sizeof(char)); 

  // reset the variables
  memset(data3,0,sizeof(data3));
  network_functions_test_error_settings = 0;

  // write the start test message
  fprintf(stderr,"\033[1;34m%s\nreset variables allocated on the heap functions test - Total test: %d\n%s\n\n\033[0m",TEST_OUTLINE,RESET_VARAIBLES_ALLOCATED_ON_THE_HEAP_TEST,TEST_OUTLINE);


  // run the test
  CHECK_RESET_VARIABLES_ON_THE_HEAP("varint_decode",VARINT_DECODE_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("varint_encode",VARINT_ENCODE_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("get_generated_supply",GET_GENERATED_SUPPLY_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("network_block_string_to_blockchain_data",NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_CODE);
  INITIALIZE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_TEST;
  CHECK_RESET_VARIABLES_ON_THE_HEAP("verify_network_block_data",VERIFY_NETWORK_BLOCK_DATA_CODE);

  RESET_BLOCKCHAIN_DATA;
  CREATE_BLOCK_DATA;
  CHECK_RESET_VARIABLES_ON_THE_HEAP("blockchain_data_to_network_block_string",BLOCKCHAIN_DATA_TO_NETWORK_BLOCK_STRING_CODE);

  CHECK_RESET_VARIABLES_ON_THE_HEAP("get_database_data_hash",GET_DATABASE_DATA_HASH_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("get_database_data",GET_DATABASE_DATA_CODE);

  CHECK_RESET_VARIABLES_ON_THE_HEAP("count_documents_in_collection",COUNT_DOCUMENTS_IN_COLLECTION_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("count_all_documents_in_collection",COUNT_ALL_DOCUMENTS_IN_COLLECTION_CODE);

  CHECK_RESET_VARIABLES_ON_THE_HEAP("delete_document_from_collection",DELETE_DOCUMENT_FROM_COLLECTION_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("delete_collection_from_database",DELETE_COLLECTION_FROM_DATABASE_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("delete_database",DELETE_DATABASE_CODE);

  CHECK_RESET_VARIABLES_ON_THE_HEAP("insert_document_into_collection_json",INSERT_DOCUMENT_INTO_COLLECTION_JSON_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("insert_multiple_documents_into_collection_json",INSERT_MULTIPLE_DOCUMENTS_INTO_COLLECTION_JSON_CODE);
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST);

  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,MESSAGE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("read_document_from_collection",READ_DOCUMENT_FROM_COLLECTION_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("read_document_field_from_collection",READ_DOCUMENT_FIELD_FROM_COLLECTION_CODE);
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST);
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,DATABASE_COLLECTION_STATISTICS_TEST_DATA); 
  CHECK_RESET_VARIABLES_ON_THE_HEAP("read_document_all_fields_from_collection",READ_DOCUMENT_ALL_FIELDS_FROM_COLLECTION_CODE);
  for (count2 = 0; count2 < 7; count2++)
  {
    memset(database_data.item[count2],0,strnlen(database_data.item[count2],BUFFER_SIZE));
    memset(database_data.value[count2],0,strnlen(database_data.value[count2],BUFFER_SIZE));
  }
  memcpy(database_data.item[0],"username",8);
  memcpy(database_data.item[1],"most_total_rounds_delegate_name",31);
  memcpy(database_data.item[2],"most_total_rounds",17);
  memcpy(database_data.item[3],"best_block_verifier_online_percentage_delegate_name",51);
  memcpy(database_data.item[4],"best_block_verifier_online_percentage",37);
  memcpy(database_data.item[5],"most_block_producer_total_rounds_delegate_name",46);
  memcpy(database_data.item[6],"most_block_producer_total_rounds",32);
  memcpy(database_data.value[0],"XCASH",5);  
  memcpy(database_data.value[1],"DELEGATE_NAME",13);
  memcpy(database_data.value[2],"5",sizeof(char));
  memcpy(database_data.value[3],"DELEGATE_NAME",13);
  memcpy(database_data.value[4],"10",2);
  memcpy(database_data.value[5],"DELEGATE_NAME",13);  
  memcpy(database_data.value[6],"15",2);
  database_data.count = 7;
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST);
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,DATABASE_COLLECTION_STATISTICS_TEST_DATA);
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,DATABASE_COLLECTION_STATISTICS_TEST_DATA);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("read_multiple_documents_all_fields_from_collection",READ_MULTIPLE_DOCUMENTS_ALL_FIELDS_FROM_COLLECTION_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("database_document_parse_json_data",DATABASE_DOCUMENT_PARSE_JSON_DATA_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("database_multiple_documents_parse_json_data",DATABASE_MULTIPLE_DOCUMENTS_PARSE_JSON_DATA_CODE);
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST);
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,DATABASE_COLLECTION_DELEGATES_TEST_DATA);
  
  CHECK_RESET_VARIABLES_ON_THE_HEAP("update_document_from_collection",UPDATE_DOCUMENT_FROM_COLLECTION_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("update_all_documents_from_collection",UPDATE_ALL_DOCUMENTS_FROM_COLLECTION_CODE);
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST);

  CHECK_RESET_VARIABLES_ON_THE_HEAP("send_http_request",SEND_HTTP_REQUEST_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("send_data_socket",SEND_DATA_SOCKET_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("send_and_receive_data_socket",SEND_AND_RECEIVE_DATA_SOCKET_CODE);

  CHECK_RESET_VARIABLES_ON_THE_HEAP("check_if_blockchain_is_fully_synced",CHECK_IF_BLOCKCHAIN_IS_FULLY_SYNCED_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("get_block_template",GET_BLOCK_TEMPLATE_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("submit_block_template",SUBMIT_BLOCK_TEMPLATE_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("get_current_block_height",GET_CURRENT_BLOCK_HEIGHT_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("get_block_reserve_byte_data_hash",GET_BLOCK_RESERVE_BYTE_DATA_HASH_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("get_previous_block_hash",GET_PREVIOUS_BLOCK_HASH_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("get_previous_block_information",GET_PREVIOUS_BLOCK_INFORMATION_CODE);

  CHECK_RESET_VARIABLES_ON_THE_HEAP("sign_data using wallet signing key",SIGN_DATA_USING_WALLET_SIGNING_KEY_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("verify_data using wallet signing key",VERIFY_DATA_USING_WALLET_SIGNING_KEY_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("sign_data using ECDSA block verifier signing key",SIGN_DATA_USING_ECDSA_BLOCK_VERIFIER_SIGNING_KEY_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("verify_data using ECDSA block verifier signing key",VERIFY_DATA_USING_ECDSA_BLOCK_VERIFIER_SIGNING_KEY_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("validate_data",VALIDATE_DATA_CODE);

  CHECK_RESET_VARIABLES_ON_THE_HEAP("get_public_address",GET_PUBLIC_ADDRESS_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("check_reserve_proofs",CHECK_RESERVE_PROOFS_CODE);
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST);

  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,ORGANIZE_DELEGATES_TEST_DATA_1);
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,ORGANIZE_DELEGATES_TEST_DATA_2);
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,ORGANIZE_DELEGATES_TEST_DATA_3);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("organize_delegates",ORGANIZE_DELEGATES_CODE);
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST);  

  CHECK_RESET_VARIABLES_ON_THE_HEAP("parse_json_data",PARSE_JSON_DATA_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("random_string",RANDOM_STRING_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("parse_reserve_bytes_data",PARSE_RESERVE_BYTES_DATA_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("create_json_data_from_database_document_array",CREATE_JSON_DATA_FROM_DATABASE_DOCUMENT_ARRAY_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("create_json_data_from_database_multiple_documents_array",CREATE_JSON_DATA_FROM_DATABASE_MULTIPLE_DOCUMENTS_ARRAY_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("create_json_data_from_delegates_array",CREATE_JSON_DATA_FROM_DELEGATES_ARRAY_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("create_json_data_from_votes_array",CREATE_JSON_DATA_FROM_VOTES_ARRAY_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("string_count",STRING_COUNT_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("parse_http_response",PARSE_HTTP_RESPONSE_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("string_replace",STRING_REPLACE_CODE);

  CHECK_RESET_VARIABLES_ON_THE_HEAP("create_random_VRF_keys",CREATE_RANDOM_VRF_KEYS_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("crypto_vrf_is_valid_key",CRYPTO_VRF_IS_VALID_KEY_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("crypto_vrf_keypair_from_seed",CRYPTO_VRF_KEYPAIR_FROM_SEED_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("crypto_vrf_prove",CRYPTO_VRF_PROVE_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("crypto_vrf_proof_to_hash",CRYPTO_VRF_PROOF_TO_HASH_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("crypto_vrf_verify",CRYPTO_VRF_VERIFY_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("crypto_hash_sha512",CRYPTO_HASH_SHA512_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("sign_network_block_string",SIGN_NETWORK_BLOCK_STRING_CODE);

  insert_document_into_collection_json(database_name,"reserve_proofs_1",RESERVE_PROOFS_TEST_DATA);
  insert_document_into_collection_json(database_name,"reserve_bytes_1",RESERVE_BYTES_TEST_DATA);
  insert_document_into_collection_json(database_name,"delegates",DELEGATES_TEST_DATA);
  insert_document_into_collection_json(database_name,"statistics",DATABASE_COLLECTION_STATISTICS_DATA);
  INITIALIZE_NETWORK_DATA_NODES_TEST
  CHECK_RESET_VARIABLES_ON_THE_HEAP("sync_check_reserve_proofs_database_from_a_random_block_verifier",SYNC_CHECK_RESERVE_PROOFS_DATABASE_FROM_A_RANDOM_BLOCK_VERIFIER_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("sync_check_reserve_bytes_database_from_a_random_block_verifier",SYNC_CHECK_RESERVE_BYTES_DATABASE_FROM_A_RANDOM_BLOCK_VERIFIER_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("sync_check_delegates_database_from_a_random_block_verifier",SYNC_CHECK_DELEGATES_DATABASE_FROM_A_RANDOM_BLOCK_VERIFIER_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("sync_check_statistics_database_from_a_random_block_verifier",SYNC_CHECK_STATISTICS_DATABASE_FROM_A_RANDOM_BLOCK_VERIFIER_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("sync_check_reserve_proofs_database_from_a_random_network_data_node",SYNC_CHECK_RESERVE_PROOFS_DATABASE_FROM_A_RANDOM_NETWORK_DATA_NODE_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("sync_check_reserve_bytes_database_from_a_random_network_data_node",SYNC_CHECK_RESERVE_BYTES_DATABASE_FROM_A_RANDOM_NETWORK_DATA_NODE_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("sync_check_delegates_database_from_a_random_network_data_node",SYNC_CHECK_DELEGATES_DATABASE_FROM_A_RANDOM_NETWORK_DATA_NODE_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("sync_check_statistics_database_from_a_random_network_data_node",SYNC_CHECK_STATISTICS_DATABASE_FROM_A_RANDOM_NETWORK_DATA_NODE_CODE);
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST); 
  for (count = 0; count < NETWORK_DATA_NODES_AMOUNT; count++)
  {
    memset(network_data_nodes_list.network_data_nodes_public_address[count],0,sizeof(network_data_nodes_list.network_data_nodes_public_address[count]));
    memset(network_data_nodes_list.network_data_nodes_IP_address[count],0,sizeof(network_data_nodes_list.network_data_nodes_IP_address[count]));
  }
  INITIALIZE_NETWORK_DATA_NODES

  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_block_verifiers_to_block_verifiers_invalid_reserve_proofs",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_block_verifiers_to_network_data_nodes_block_verifiers_current_time",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_NETWORK_DATA_NODES_BLOCK_VERIFIERS_CURRENT_TIME_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_block_verifiers_to_block_verifiers_online_status",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_ONLINE_STATUS_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_main_network_data_node_to_block_verifier_start_block",SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_START_BLOCK_CODE);
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_main_network_data_node_to_block_verifier_create_new_block",SERVER_RECEIVE_DATA_SOCKET_MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIER_CREATE_NEW_BLOCK_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_block_verifier_to_main_network_data_node_create_new_block",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIER_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_main_node_to_node_message_part_4",SERVER_RECEIVE_DATA_SOCKET_MAIN_NODE_TO_NODE_MESSAGE_PART_4_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_node_to_node",SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NODE_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_block_verifiers_to_block_verifiers_vrf_data",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_block_verifiers_to_block_verifiers_block_blob_signature",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE_CODE);
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST);

  insert_document_into_collection_json(database_name,"reserve_proofs_1",RESERVE_PROOFS_TEST_DATA);
  insert_document_into_collection_json(database_name,"reserve_bytes_1",RESERVE_BYTES_TEST_DATA);
  insert_document_into_collection_json(database_name,"delegates",DELEGATES_TEST_DATA);
  insert_document_into_collection_json(database_name,"statistics",DATABASE_COLLECTION_STATISTICS_DATA);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("sync_reserve_proofs_database",SYNC_RESERVE_PROOFS_DATABASE_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("sync_reserve_bytes_database",SYNC_RESERVE_BYTES_DATABASE_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("sync_delegates_database",SYNC_DELEGATES_DATABASE_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("sync_statistics_database",SYNC_STATISTICS_DATABASE_CODE);
  network_data_node_settings = 0;
  INITIALIZE_NETWORK_DATA_NODES_TEST
  CHECK_RESET_VARIABLES_ON_THE_HEAP("sync_all_block_verifiers_list for block verifiers",SYNC_ALL_BLOCK_VERIFIERS_LIST_FOR_BLOCK_VERIFIERS_CODE);
  network_data_node_settings = 1;
  CHECK_RESET_VARIABLES_ON_THE_HEAP("sync_all_block_verifiers_list for network data nodes",SYNC_ALL_BLOCK_VERIFIERS_LIST_FOR_NETWORK_DATA_NODES_CODE);
  INITIALIZE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_TEST
  network_data_node_settings = 0;
  for (count = 0; count < NETWORK_DATA_NODES_AMOUNT; count++)
  {
    memset(network_data_nodes_list.network_data_nodes_public_address[count],0,sizeof(network_data_nodes_list.network_data_nodes_public_address[count]));
    memset(network_data_nodes_list.network_data_nodes_IP_address[count],0,sizeof(network_data_nodes_list.network_data_nodes_IP_address[count]));
  }
  INITIALIZE_NETWORK_DATA_NODES
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST);

  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_node_to_network_data_nodes_get_previous_current_next_block_verifiers_list",SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_node_to_network_data_nodes_get_current_block_verifiers_list",SERVER_RECEIVE_DATA_SOCKET_NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_network_data_nodes_to_network_data_nodes_database_sync_check",SERVER_RECEIVE_DATA_SOCKET_NETWORK_DATA_NODES_TO_NETWORK_DATA_NODES_DATABASE_SYNC_CHECK_CODE);
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,RESERVE_BYTES_TEST_DATA);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_node_to_block_verifiers_get_reserve_bytes_database_hash",SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATABASE_HASH_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_node_to_block_verifiers_check_if_current_block_verifier",SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_CHECK_IF_CURRENT_BLOCK_VERIFIER_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_all_update",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_download_file_update",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_all_update",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_download_file_update",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_sync_check_update",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_download_file_update",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_sync_check_update",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_download_file_update",SERVER_RECEIVE_DATA_SOCKET_BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE_CODE);
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST);

  INITIALIZE_NETWORK_DATA_NODES_TEST;
  INITIALIZE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_TEST;
  insert_document_into_collection_json(database_name,"reserve_proofs_1",RESERVE_PROOFS_TEST_DATA);
  insert_document_into_collection_json(database_name,"reserve_bytes_1",RESERVE_BYTES_TEST_DATA);
  insert_document_into_collection_json(database_name,"delegates",DELEGATES_TEST_DATA);
  insert_document_into_collection_json(database_name,"statistics",DATABASE_COLLECTION_STATISTICS_DATA);
  RESET_INVALID_RESERVE_PROOFS_DATA;
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
  memcpy(invalid_reserve_proofs.public_address_created_reserve_proof[0],TEST_WALLET_1,XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.reserve_proof[0],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB31qaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti",537);
  memcpy(invalid_reserve_proofs.public_address_voted_for[1],TEST_WALLET,XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.public_address_created_reserve_proof[1],TEST_WALLET_1,XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.reserve_proof[1],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB31qaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti",537);
  invalid_reserve_proofs.count = 2;
  CHECK_RESET_VARIABLES_ON_THE_HEAP("check_reserve_proofs_timer_create_message",CHECK_RESERVE_PROOFS_TIMER_CREATE_MESSAGE_CODE);

  delete_database(database_name);
  INITIALIZE_DATABASE_DATA(2);
  RESET_INVALID_RESERVE_PROOFS_DATA;
  invalid_reserve_proofs.block_verifier_public_address[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_created_reserve_proof[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_voted_for[0] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.reserve_proof[0] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));
  invalid_reserve_proofs.block_verifier_public_address[1] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_created_reserve_proof[1] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.public_address_voted_for[1] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
  invalid_reserve_proofs.reserve_proof[1] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char));
  memcpy(invalid_reserve_proofs.block_verifier_public_address[0],TEST_WALLET_1,XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.public_address_voted_for[0],TEST_WALLET,XCASH_WALLET_LENGTH);
  memcpy(invalid_reserve_proofs.public_address_created_reserve_proof[0],TEST_WALLET,XCASH_WALLET_LENGTH);
  invalid_reserve_proofs.reserve_proof_amount[0] = 0;
  memcpy(invalid_reserve_proofs.reserve_proof[0],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5jPey2PF2RPr1p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed4jJUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti",537);
  memcpy(invalid_reserve_proofs.reserve_proof[1],"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NTiwbZLCstwWzpKy8J6J8M1uq9qf5PbWha7C68N8V7qshnzUP113UoSQuy78bpD5CeY2H3ViztSqDBFmmMiNu6uCQGkbu4NrZhZk2T7y7T1ixLCAxYfKRtd2pAAbuAWeBVCNzvmKB8YaneMgZ3mH113kXRg67xZRBtVcawf1CTYHCVuhpur8F6nAGxwFeNoRAdr6rnGwa1SUVdqrXypfmJJyzpPUGTSriAJqtRSBKJqAgQDPz4aV99aLMHHKWAv3eKaGAvBZ3obk2GuVBnHeUUEHAihQ5ECndKS4dw8YxMogdY9o8PvrhZgt2gnX4YrUiF5VbGBcBUsxbag1Ak9oZfTH2d1WeyMntcv1Mb28Nzc73UvMbck3VyBedN57gV1LS8WNV1SeHAFBiGgeBcuveGee9VZYQvGRfiWzkJf9JfYWoQ4dyxh13BrXpp3rF1nWv3UXxMrJJX8SRtQy5zrLAywENZ",538);
  invalid_reserve_proofs.count = 2;
  CHECK_RESET_VARIABLES_ON_THE_HEAP("check_reserve_proofs_timer_get_database_data",CHECK_RESERVE_PROOFS_TIMER_GET_DATABASE_DATA_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("check_reserve_proofs_timer_update_delegates_total_vote_count",CHECK_RESERVE_PROOFS_TIMER_UPDATE_DELEGATES_TOTAL_VOTE_COUNT_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("check_reserve_proofs_timer_update_delegates_score",CHECK_RESERVE_PROOFS_TIMER_UPDATE_DELEGATES_SCORE_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("check_reserve_proofs_timer_update_database",CHECK_RESERVE_PROOFS_TIMER_UPDATE_DATABASE_CODE);

  delete_database(database_name);
  INITIALIZE_DATABASE_DATA(2);
  RESET_INVALID_RESERVE_PROOFS_DATA;
  CHECK_RESET_VARIABLES_ON_THE_HEAP("select_random_unique_reserve_proof",SELECT_RANDOM_UNIQUE_RESERVE_PROOF_CODE);

  delete_database(database_name);
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
  memcpy(reserve_proof.block_verifier_public_address,TEST_WALLET_1,XCASH_WALLET_LENGTH);
  memcpy(reserve_proof.public_address_created_reserve_proof,TEST_WALLET_1,XCASH_WALLET_LENGTH);
  memcpy(reserve_proof.public_address_voted_for,TEST_WALLET_1,XCASH_WALLET_LENGTH);
  memcpy(reserve_proof.reserve_proof_amount,"120000000",9);
  memcpy(reserve_proof.reserve_proof,"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118NDPqYHviiubTHpa5j1ey2PF2RPr7p92nUY5PYcCqPwkM3Vezb1BvSAu2zX5kKMuJYo2q837KH4HAXkXbdgF6wa13pkkpuMxv74keNZLAeeM9wmSuJvSHmMvVjfo6u6iCWMDRESRouQ359NvpAZN71D9fSivgK7K7WkbNzftkUZ6V7Uza6K9eihTgu7hSB3AqaTm7cK9uTb5Fzg9LyJbC4phfGYM7bazM2UrVfitZtbEkKuhPxnzFzKkWtdYBB59zUo1uS4UUR8faS25sjfc2cPjZUfbEZsiJVo7EDNs3d1KdhTN5TdNxZK6MZgVB77jE9ed41JUrNSrqfWg1BwigbN9smQicoi9yYwujuGaHEzEnLBwQeLFxJJQj31qRQb4ZijEBGrMxvcmybhPKiHA3LBARnBREJxkQ39dp2HRfEfR1G7z6RGhS9o1KQCF3MAwomCMCuj69SpeovPEYwQb5uVXti",537);
  
  delete_database(database_name);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("block_verifiers_add_reserve_proof_check_if_data_is_valid",BLOCK_VERIFIERS_ADD_RESERVE_PROOF_CHECK_IF_DATA_IS_VALID_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("add_reserve_proof_remove_previous_vote",ADD_RESERVE_PROOF_REMOVE_PREVIOUS_VOTE_CODE);
  
  delete_database(database_name);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("update_block_verifiers_list",UPDATE_BLOCK_VERIFIERS_LIST_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("add_block_verifiers_round_statistics",ADD_BLOCK_VERIFIERS_ROUND_STATISTICS_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("add_round_statistics",ADD_ROUND_STATISTICS_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("calculate_main_nodes_roles",CALCULATE_MAIN_NODES_ROLES_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("get_delegates_online_status",GET_DELEGATES_ONLINE_STATUS_CODE);












  delete_database(database_name);
  insert_document_into_collection_json(database_name,"reserve_bytes_1",RESERVE_BYTES_TEST_DATA);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",RESERVE_PROOFS_TEST_DATA);
  insert_document_into_collection_json(database_name,"delegates",DELEGATES_TEST_DATA);
  insert_document_into_collection_json(database_name,"statistics",DATABASE_COLLECTION_STATISTICS_DATA);
  sleep(2);

  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memset(data3,0,sizeof(data3));
  memcpy(result_test,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF|XCA1v18Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3|RESERVE_PROOF_TEST_DATA|",165);
  memcpy(result_test+strlen(result_test),xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(result_test+strlen(result_test),"|",sizeof(char));
  memcpy(data_test,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"sign\",\"params\":{\"data\":\"",60);
  memcpy(data_test+60,result_test,strnlen(result_test,sizeof(data_test)));
  memcpy(data_test+strlen(data_test),"\"}}",3);  
  send_http_request(data3,XCASH_DPOPS_delegates_IP_address,"/json_rpc",xcash_wallet_port,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,data_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  memset(data_test,0,sizeof(data_test));
  parse_json_data(data3,"signature",data_test,MAXIMUM_BUFFER_SIZE);
  memcpy(result_test+strlen(result_test),data_test,XCASH_SIGN_DATA_LENGTH);
  memcpy(result_test+strlen(result_test),"|",sizeof(char));
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_node_to_block_verifiers_add_reserve_proof",DELEGATES_SERVER_FUNCTIONS_TEST_CODE);

  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memset(data3,0,sizeof(data3));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE|delegate_name_1|127.0.0.1|",69);
  memcpy(result_test+strlen(result_test),NEXT_BLOCK_VERIFIERS_PUBLIC_KEY,VRF_PUBLIC_KEY_LENGTH);
  memcpy(result_test+strlen(result_test),"|",sizeof(char));
  memcpy(result_test+strlen(result_test),xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(result_test+strlen(result_test),"|",sizeof(char));
  memcpy(data_test,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"sign\",\"params\":{\"data\":\"",60);
  memcpy(data_test+60,result_test,strnlen(result_test,sizeof(data_test)));
  memcpy(data_test+strlen(data_test),"\"}}",3);  
  send_http_request(data3,XCASH_DPOPS_delegates_IP_address,"/json_rpc",xcash_wallet_port,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,data_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  memset(data_test,0,sizeof(data_test));
  parse_json_data(data3,"signature",data_test,MAXIMUM_BUFFER_SIZE);
  memcpy(result_test+strlen(result_test),data_test,XCASH_SIGN_DATA_LENGTH);
  memcpy(result_test+strlen(result_test),"|",sizeof(char));
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_nodes_to_block_verifiers_register_delegates",DELEGATES_SERVER_FUNCTIONS_TEST_CODE);

  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  memset(data3,0,sizeof(data3));
  memcpy(result_test,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE|about|data|",52);
  memcpy(result_test+strlen(result_test),xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(result_test+strlen(result_test),"|",sizeof(char));
  memcpy(data_test,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"sign\",\"params\":{\"data\":\"",60);
  memcpy(data_test+60,result_test,strnlen(result_test,sizeof(data_test)));
  memcpy(data_test+strlen(data_test),"\"}}",3);  
  send_http_request(data3,XCASH_DPOPS_delegates_IP_address,"/json_rpc",xcash_wallet_port,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,data_test,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
  memset(data_test,0,sizeof(data_test));
  parse_json_data(data3,"signature",data_test,MAXIMUM_BUFFER_SIZE);
  memcpy(result_test+strlen(result_test),data_test,XCASH_SIGN_DATA_LENGTH);
  memcpy(result_test+strlen(result_test),"|",sizeof(char));
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_nodes_to_block_verifiers_update_delegates",DELEGATES_SERVER_FUNCTIONS_TEST_CODE);
  sleep(60);
  delete_database(database_name);

  insert_document_into_collection_json(database_name,"reserve_bytes_1",RESERVE_BYTES_TEST_DATA);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",RESERVE_PROOFS_TEST_DATA);
  insert_document_into_collection_json(database_name,"delegates",DELEGATES_TEST_DATA);
  insert_document_into_collection_json(database_name,"statistics",DATABASE_COLLECTION_STATISTICS_DATA);
  sleep(2);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_delegates_website_get_statistics",SERVER_RECEIVE_DATA_SOCKET_DELEGATES_WEBSITE_GET_STATISTICS_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_get_delegates",SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_get_delegates_statistics",SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_STATISTICS_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_get_delegates_information",SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_INFORMATION_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_get_delegates_voters_list",SERVER_RECEIVE_DATA_SOCKET_GET_DELEGATES_VOTERS_LIST_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_get_round_statistics",SERVER_RECEIVE_DATA_SOCKET_GET_ROUND_STATISTICS_CODE);
  sleep(60);
  delete_database(DATABASE_NAME_TEST);

  insert_document_into_collection_json(database_name,"reserve_bytes_1",SERVER_RECEIVE_DATA_SOCKET_NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATABASE_HASH_TEST_DATA);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",RESERVE_PROOFS_TEST_DATA);
  insert_document_into_collection_json(database_name,"delegates",DELEGATES_TEST_DATA);
  insert_document_into_collection_json(database_name,"statistics",DATABASE_COLLECTION_STATISTICS_DATA);
  insert_document_into_collection_json(database_name,"blocks_found","{\"block_height\":\"440875\",\"block_hash\":\"0000000000000000000000000000000000000000000000000000000000000000\",\"block_date_and_time\":\"10\",\"block_reward\":\"15\",\"block_count\":\"10\"}");
  insert_document_into_collection_json(database_name,"public_addresses","{\"public_address\":\"" TEST_WALLET "\",\"current_total\":\"5\",\"total\":\"10\",\"inactivity_count\":\"15\"}");
  insert_document_into_collection_json(database_name,"public_addresses_payments","{\"public_address\":\"" TEST_WALLET "\",\"payment_name\":\"\",\"payment_address\":\"" TEST_WALLET "\",\"date_and_time\":\"5\",\"date_and_time\":\"5\",\"total\":\"10\",\"tx_hash\":\"TX_HASH\",\"tx_key\":\"TX_KEY\"}");
  sleep(2);
  memset(result_test,0,sizeof(result_test));
  memcpy(result_test,"{\"public_address\":\"",19);
  memcpy(result_test+strlen(result_test),xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(result_test+strlen(result_test),"\",\"total_vote_count\":\"0\",\"IP_address\":\"127.0.0.1\",\"delegate_name\":\"delegate_name_1\",\"about\":\"about\",\"website\":\"website\",\"team\":\"team\",\"shared_delegate_status\":\"solo\",\"delegate_fee\":\"0\",\"server_specs\":\"server settings\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"440875\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}",536);
  insert_document_into_collection_json(database_name,"delegates",result_test);
  sleep(2);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_shared_delegates_website_get_statistics",SERVER_RECEIVE_DATA_SOCKET_SHARED_DELEGATES_WEBSITE_GET_STATISTICS_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_get_blocks_found",SERVER_RECEIVE_DATA_SOCKET_GET_BLOCKS_FOUND_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_get_public_address_information",SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_INFORMATION_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("server_receive_data_socket_get_public_address_payment_information",SERVER_RECEIVE_DATA_SOCKET_GET_PUBLIC_ADDRESS_PAYMENT_INFORMATION_CODE);
  sleep(60);
  delete_database(DATABASE_NAME_TEST);

  CHECK_RESET_VARIABLES_ON_THE_HEAP("add_block_to_blocks_found",ADD_BLOCK_TO_BLOCKS_FOUND_CODE);
  
  delete_database(DATABASE_NAME_TEST);
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
  CHECK_RESET_VARIABLES_ON_THE_HEAP("get_delegates_total_voters_count",GET_DELEGATES_TOTAL_VOTERS_COUNT_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("get_delegates_total_voters",GET_DELEGATES_TOTAL_VOTERS_CODE);

  insert_document_into_collection_json(database_name,"public_addresses","{\"public_address\":\"" TEST_WALLET_1 "\",\"current_total\":\"5000000000000\",\"total\":\"0\",\"inactivity_count\":\"10\"}");
  insert_document_into_collection_json(database_name,"public_addresses","{\"public_address\":\"" TEST_WALLET_20 "\",\"current_total\":\"0\",\"total\":\"10000000000000\",\"inactivity_count\":\"0\"}");
  CHECK_RESET_VARIABLES_ON_THE_HEAP("calculate_block_reward_for_each_delegate",CALCULATE_BLOCK_REWARD_FOR_EACH_DELEGATE_CODE);

  delete_database(database_name);
  insert_document_into_collection_json(database_name,"public_addresses","{\"public_address\":\"" TEST_WALLET_1 "\",\"current_total\":\"5000000000000\",\"total\":\"10000000000000\",\"inactivity_count\":\"10\"}");
  insert_document_into_collection_json(database_name,"public_addresses","{\"public_address\":\"" TEST_WALLET_20 "\",\"current_total\":\"0\",\"total\":\"10000000000000\",\"inactivity_count\":\"0\"}");
  CHECK_RESET_VARIABLES_ON_THE_HEAP("payment_timer_send_payment_and_update_databases",PAYMENT_TIMER_SEND_PAYMENT_AND_UPDATE_DATABASES_CODE);

  delete_database(database_name);
  insert_document_into_collection_json(database_name,"public_addresses","{\"public_address\":\"" TEST_WALLET_1 "\",\"current_total\":\"0\",\"total\":\"10000000000000\",\"inactivity_count\":\"0\"}");
  CHECK_RESET_VARIABLES_ON_THE_HEAP("payment_timer_update_inactivity_count",PAYMENT_TIMER_UPDATE_INACTIVITY_COUNT_CODE);


  sleep(60);
  delete_database(DATABASE_NAME_TEST);

  for (count = 0; count < 7; count++)
  {
    pointer_reset(database_data.item[count]);
    pointer_reset(database_data.value[count]);
  }

  for (count = 0; count < 2; count++)
  {
    for (counter = 0; counter < 7; counter++)
    {
      pointer_reset(database_multiple_documents_fields.item[count][counter]);
      pointer_reset(database_multiple_documents_fields.value[count][counter]);
    }
  }

  for (count = 0; count < 5; count++)
  {
    pointer_reset(transactions[count]);
  }

  POINTER_RESET_VOTERS_STRUCT(count,MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE);

  POINTER_RESET_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES);

  POINTER_RESET_VOTES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES);

  // write the end test message
  fprintf(stderr,"\033[1;33m\n\n%s\nreset variables allocated on the heap functions test - Passed test: %d, Failed test: %d\n%s\n\n\n\033[0m",TEST_OUTLINE,count_test,RESET_VARAIBLES_ALLOCATED_ON_THE_HEAP_TEST-count_test,TEST_OUTLINE);
  pointer_reset(process_id_file);
  return count_test;
  
  #undef GET_PUBLIC_ADDRESS_DATA
  #undef GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA
  #undef DATA1
  #undef DATA2
  #undef NETWORK_BLOCK_HEIGHT
  #undef VARINT_ENCODED_VALUE_1
  #undef VARINT_DECODED_VALUE_1
  #undef DATA_HASH_TEXT
  #undef MESSAGE
  #undef SEND_DATA_SOCKET_TEST_MESSAGE
  #undef DATABASE_COLLECTION_STATISTICS_TEST_DATA
  #undef RESET_INVALID_RESERVE_PROOFS_DATA
}
