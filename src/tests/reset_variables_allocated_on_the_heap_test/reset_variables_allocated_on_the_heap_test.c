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

#include "database_functions.h"
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
#include "blockchain_functions.h"
#include "server_functions.h"
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

size_t get_program_memory_usage(char* process_id_file)
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
  file = fopen(process_id_file,"r");
  if (file != NULL)
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

  // Variables
  char data3[BUFFER_SIZE];
  char validate_data_settings[41][10000] = {MAIN_NODES_TO_NODES_PART_4_OF_ROUND_CREATE_NEW_BLOCK,MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_START_BLOCK,MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_CREATE_NEW_BLOCK,BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA,NODES_TO_NODES_VOTE_RESULTS,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS,NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST,NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST,NETWORK_DATA_NODE_TO_NODE_SEND_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST,NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST,BLOCK_VERIFIERS_TO_NETWORK_DATA_NODE_BLOCK_VERIFIERS_CURRENT_TIME,NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_BLOCK_VERIFIERS_CURRENT_TIME,NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE,NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE,NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE,NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF,NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATABASE_HASH,NODE_TO_BLOCK_VERIFIERS_CHECK_IF_CURRENT_BLOCK_VERIFIER,BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES,NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE,BLOCK_VERIFIERS_TO_NODES_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD,NETWORK_DATA_NODES_TO_NETWORK_DATA_NODES_DATABASE_SYNC_CHECK,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD};
  char* process_id_file = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char** data = (char**)calloc(5 * sizeof(char*),sizeof(char*));
  char** settings = (char**)calloc(5 * sizeof(char*),sizeof(char*));
  int previous_system_memory_usage;
  int current_system_memory_usage;
  int current_memory_usage = 0;
  size_t count = 0;
  size_t count2 = 0;
  size_t counter;
  struct database_document_fields database_data;
  struct database_multiple_documents_fields database_multiple_documents_fields;
  struct delegates delegates[MAXIMUM_AMOUNT_OF_DELEGATES];
  struct votes votes[MAXIMUM_AMOUNT_OF_DELEGATES];
  unsigned char vrf_public_key[crypto_vrf_PUBLICKEYBYTES];
  unsigned char vrf_secret_key[crypto_vrf_SECRETKEYBYTES];
  unsigned char vrf_proof[crypto_vrf_PROOFBYTES];
  unsigned char vrf_beta[crypto_vrf_OUTPUTBYTES];
  char* transactions[5];

  // define macros
  #define RESET_VARAIBLES_ALLOCATED_ON_THE_HEAP_TEST 58
  #define RESET_ERROR_MESSAGES \
  memset(error_message.function[0],0,strlen(error_message.function[0])); \
  memset(error_message.data[0],0,strlen(error_message.data[0])); \
  error_message.total = 0;
  #define GET_PUBLIC_ADDRESS_DATA "{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_address\"}"
  #define GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA "{\r\n  \"id\": \"0\",\r\n  \"jsonrpc\": \"2.0\",\r\n  \"result\": {\r\n    \"blockhashing_blob\": \"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\",\r\n    \"blocktemplate_blob\": \"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\",\r\n    \"difficulty\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,\r\n    \"expected_reward\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,\r\n    \"height\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,\r\n    \"prev_hash\": \"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\",\r\n    \"reserved_offset\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,\r\n    \"status\": \"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\",\r\n    \"untrusted\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\r\n  }\r\n}"
  #define DATA1 "{\"username\":\"XCASH\",\"most_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_total_rounds\":\"5\",\"best_block_verifier_online_percentage_delegate_name\":\"DELEGATE_NAME\",\"best_block_verifier_online_percentage\":\"10\",\"most_block_producer_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_block_producer_total_rounds\":\"15\"}"
  #define DATA2 "[{\"username\":\"XCASH\",\"most_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_total_rounds\":\"5\",\"best_block_verifier_online_percentage_delegate_name\":\"DELEGATE_NAME\",\"best_block_verifier_online_percentage\":\"10\",\"most_block_producer_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_block_producer_total_rounds\":\"15\"},{\"username\":\"XCASH\",\"most_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_total_rounds\":\"5\",\"best_block_verifier_online_percentage_delegate_name\":\"DELEGATE_NAME\",\"best_block_verifier_online_percentage\":\"10\",\"most_block_producer_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_block_producer_total_rounds\":\"15\"}]" 
  #define NETWORK_BLOCK_HEIGHT "2813049"
  #define VARINT_ENCODED_VALUE_1 0xb2f58199a302 // random value
  #define VARINT_DECODED_VALUE_1 78167177906 // random value
  #define DATA_HASH_TEXT "X-CASH Proof Of Stake"
  #define MESSAGE "{\"MESSAGE\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\"}"
  #define DATABASE_COLLECTION_STATISTICS_TEST_DATA "{\"username\":\"XCASH\",\"most_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_total_rounds\":\"5\",\"best_block_verifier_online_percentage_delegate_name\":\"DELEGATE_NAME\",\"best_block_verifier_online_percentage\":\"10\",\"most_block_producer_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_block_producer_total_rounds\":\"15\"}"
 
  #define CHECK_RESET_VARIABLES_ON_THE_HEAP(FUNCTION_NAME, CODE) \
  previous_system_memory_usage = get_program_memory_usage(process_id_file); \
  for (count2 = 0; count2 <= 1000; count2++) \
  { \
    fprintf(stderr,"Testing %s: %zu / 1000",FUNCTION_NAME,count2); \
    fprintf(stderr,"\r"); \
    CODE; \
    RESET_ERROR_MESSAGES; \
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

  #define VARINT_DECODE_CODE varint_decode(VARINT_ENCODED_VALUE_1);
  
  #define VARINT_ENCODE_CODE \
  memset(result_test,0,sizeof(result_test)); \
  varint_encode(VARINT_DECODED_VALUE_1,result_test,sizeof(result_test));

  #define NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_CODE \
  RESET_BLOCKCHAIN_DATA; \
  network_block_string_to_blockchain_data(NETWORK_BLOCK,NETWORK_BLOCK_HEIGHT,BLOCK_VERIFIERS_TOTAL_AMOUNT);
  
  #define VERIFY_NETWORK_BLOCK_DATA_CODE verify_network_block_data(0,0,0,NETWORK_BLOCK_HEIGHT,"",BLOCK_VERIFIERS_TOTAL_AMOUNT);

  #define BLOCKCHAIN_DATA_TO_NETWORK_BLOCK_STRING_CODE blockchain_data_to_network_block_string(data_test,BLOCK_VERIFIERS_TOTAL_AMOUNT);

  #define CREATE_DATABASE_CONNECTION_CODE \
  mongoc_client_destroy(database_client); \
  create_database_connection();

  #define GET_DATABASE_DATA_HASH_CODE get_database_data_hash(data_test,database_name,DATABASE_COLLECTION_TEST);

  #define GET_DATABASE_DATA_CODE get_database_data(data_test,database_name,DATABASE_COLLECTION_TEST,0);

  #define COUNT_DOCUMENTS_IN_COLLECTION_CODE count_documents_in_collection(database_name,DATABASE_COLLECTION_TEST,MESSAGE,0);

  #define COUNT_ALL_DOCUMENTS_IN_COLLECTION_CODE count_all_documents_in_collection(database_name,DATABASE_COLLECTION_TEST,0);

  #define DELETE_DOCUMENT_FROM_COLLECTION_CODE \
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,MESSAGE,0); \
  delete_document_from_collection(database_name,DATABASE_COLLECTION_TEST,MESSAGE,0);

  #define DELETE_COLLECTION_FROM_DATABASE_CODE \
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,MESSAGE,0); \
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST,0);

  #define INSERT_DOCUMENT_INTO_COLLECTION_ARRAY_CODE insert_document_into_collection_array(database_name,DATABASE_COLLECTION_TEST,data,settings,5);

  #define INSERT_DOCUMENT_INTO_COLLECTION_JSON_CODE insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,MESSAGE,0);

  #define INSERT_MULTIPLE_DOCUMENTS_INTO_COLLECTION_JSON_CODE insert_multiple_documents_into_collection_json(database_name,DATABASE_COLLECTION_TEST,MESSAGE,sizeof(MESSAGE)-1,0);

  #define READ_DOCUMENT_FROM_COLLECTION_CODE \
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE)); \
  read_document_from_collection(database_name,DATABASE_COLLECTION_TEST,MESSAGE,data_test,0);

  #define READ_DOCUMENT_FIELD_FROM_COLLECTION_CODE \
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE)); \
  read_document_field_from_collection(database_name,DATABASE_COLLECTION_TEST,MESSAGE,"MESSAGE",data_test,0);

  #define READ_DOCUMENT_ALL_FIELDS_FROM_COLLECTION_CODE \
  for (count = 0; count < 14; count++) \
  { \
    memset(database_data.item[count],0,strnlen(database_data.item[count],BUFFER_SIZE)); \
    memset(database_data.value[count],0,strnlen(database_data.value[count],BUFFER_SIZE)); \
  } \
  read_document_all_fields_from_collection(database_name,DATABASE_COLLECTION_TEST,"{\"username\":\"XCASH\"}",&database_data,0);

  #define READ_MULTIPLE_DOCUMENTS_ALL_FIELDS_FROM_COLLECTION_CODE \
  for (count = 0; count < 2; count++) \
  { \
    for (counter = 0; counter < 7; counter++) \
    { \
      memset(database_multiple_documents_fields.item[count][counter],0,strnlen(database_multiple_documents_fields.item[count][counter],BUFFER_SIZE)); \
      memset(database_multiple_documents_fields.value[count][counter],0,strnlen(database_multiple_documents_fields.value[count][counter],BUFFER_SIZE)); \
    } \
  } \
  read_multiple_documents_all_fields_from_collection(database_name,DATABASE_COLLECTION_TEST,"",&database_multiple_documents_fields,1,2,0,"",0);

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

  #define UPDATE_DOCUMENT_FROM_COLLECTION_CODE update_document_from_collection(database_name,DATABASE_COLLECTION_TEST,MESSAGE,MESSAGE,0);

  #define UPDATE_ALL_DOCUMENTS_FROM_COLLECTION_CODE update_all_documents_from_collection(database_name,DATABASE_COLLECTION_TEST,MESSAGE,0);

  #define UPDATE_DOCUMENT_FROM_COLLECTION_CODE update_document_from_collection(database_name,DATABASE_COLLECTION_TEST,MESSAGE,MESSAGE,0);

  #define UPDATE_DOCUMENT_FROM_COLLECTION_CODE update_document_from_collection(database_name,DATABASE_COLLECTION_TEST,MESSAGE,MESSAGE,0);

  #define UPDATE_DOCUMENT_FROM_COLLECTION_CODE update_document_from_collection(database_name,DATABASE_COLLECTION_TEST,MESSAGE,MESSAGE,0);

  #define CHECK_IF_BLOCKCHAIN_IS_FULLY_SYNCED_CODE check_if_blockchain_is_fully_synced();

  #define GET_BLOCK_TEMPLATE_CODE \
  memset(data_test,0,sizeof(data_test)); \
  get_block_template(data_test,0);

  #define SUBMIT_BLOCK_TEMPLATE_CODE submit_block_template("XCASH_DPOPS",0);

  #define GET_CURRENT_BLOCK_HEIGHT_CODE \
  memset(data_test,0,sizeof(data_test)); \
  get_current_block_height(data_test,0);

  #define GET_BLOCK_RESERVE_BYTE_DATA_HASH_CODE \
  memset(data_test,0,sizeof(data_test)); \
  get_block_reserve_byte_data_hash(data_test,"440875",0);

  #define GET_PREVIOUS_BLOCK_HASH_CODE \
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE)); \
  get_previous_block_hash(data_test,0);

  #define GET_PREVIOUS_BLOCK_INFORMATION_CODE \
  memset(data_test,0,sizeof(data_test)); \
  memset(result_test,0,sizeof(result_test)); \
  memset(data3,0,sizeof(data3)); \
  get_previous_block_information(result_test,data_test,data3);

  #define CHECK_FOUND_BLOCK_CODE check_found_block();

  #define SEND_HTTP_REQUEST_CODE \
  memset(data_test,0,sizeof(data_test)); \
  send_http_request(data_test,"127.0.0.1","/json_rpc",xcash_wallet_port,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,GET_PUBLIC_ADDRESS_DATA,RECEIVE_DATA_TIMEOUT_SETTINGS,"get public address",0);

  #define SEND_DATA_SOCKET_CODE \
  memset(data_test,0,sizeof(data_test)); \
  memcpy(data_test,MESSAGE,sizeof(MESSAGE)-1); \
  sign_data(data_test,0); \
  send_data_socket("127.0.0.1",SEND_DATA_PORT,data_test);

  #define SEND_AND_RECEIVE_DATA_SOCKET_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memset(data_test,0,sizeof(data_test)); \
  memcpy(data_test,MESSAGE,sizeof(MESSAGE)-1); \
  sign_data(data_test,0); \
  send_and_receive_data_socket(result_test,"127.0.0.1",SEND_DATA_PORT,(const char*)data_test,TOTAL_CONNECTION_TIME_SETTINGS,"XCASH_PROOF_OF_STAKE_TEST_DATA",0);

  #define SIGN_DATA_USING_WALLET_SIGNING_KEY_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,"{\r\n \"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\",\r\n}",60); \
  sign_data(result_test,0);

  #define VERIFY_DATA_USING_WALLET_SIGNING_KEY_CODE verify_data(result_test,0,1);

  #define SIGN_DATA_USING_ECDSA_BLOCK_VERIFIER_SIGNING_KEY_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,"{\r\n \"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA_2\",\r\n}",62); \
  sign_data(result_test,0);

  #define VERIFY_DATA_USING_ECDSA_BLOCK_VERIFIER_SIGNING_KEY_CODE verify_data(result_test,0,1);

  #define VALIDATE_DATA_CODE validate_data(validate_data_settings[(rand() % ((sizeof(validate_data_settings)/sizeof(validate_data_settings[0])) - 0 + 1)) + 0]);

  #define GET_PUBLIC_ADDRESS_CODE get_public_address(0);

  #define CHECK_RESERVE_PROOFS_CODE \
  memset(data_test,0,sizeof(data_test)); \
  check_reserve_proofs(data_test,CHECK_RESERVE_PROOF_TEST_PUBLIC_ADDRESS,CHECK_RESERVE_PROOF_TEST_RESERVE_PROOF,0);

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

  #define STRING_REPLACE_CODE \
  memset(result_test,0,sizeof(result_test)); \
  memcpy(result_test,"{\r\n \"MESSAGE\": \"string_replace_test\",\r\n}",40);
  string_replace(result_test,sizeof(result_test),"string_replace_test","string_replace");

  #define CREATE_RANDOM_VRF_KEYS_CODE create_random_VRF_keys((unsigned char*)vrf_public_key,(unsigned char*)vrf_secret_key);

  #define CRYPTO_VRF_IS_VALID_KEY_CODE count = crypto_vrf_is_valid_key((const unsigned char*)vrf_public_key);

  #define CRYPTO_VRF_KEYPAIR_FROM_SEED_CODE crypto_vrf_keypair_from_seed((unsigned char*)vrf_public_key, (unsigned char*)vrf_secret_key, (const unsigned char*)data2);

  #define CRYPTO_VRF_PROVE_CODE crypto_vrf_prove((unsigned char*)vrf_proof,(const unsigned char*)vrf_secret_key,alpha_string,sizeof(alpha_string));

  #define CRYPTO_VRF_PROOF_TO_HASH_CODE crypto_vrf_proof_to_hash((unsigned char*)vrf_beta,(const unsigned char*)vrf_proof);

  #define CRYPTO_VRF_VERIFY_CODE count = crypto_vrf_verify((unsigned char*)vrf_beta,(const unsigned char*)vrf_public_key,(const unsigned char*)vrf_proof,alpha_string,sizeof(alpha_string));

  #define CRYPTO_HASH_SHA512_CODE crypto_hash_sha512((unsigned char*)data_test,(const unsigned char*)DATA_HASH_TEXT,sizeof(DATA_HASH_TEXT)-1);

  #define SIGN_NETWORK_BLOCK_STRING_CODE sign_network_block_string(data_test,"X-CASH");



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
  memcpy(database_data.value[2],"5",1);
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
  memcpy(database_multiple_documents_fields.value[0][2],"5",1);
  memcpy(database_multiple_documents_fields.value[0][3],"DELEGATE_NAME",13);
  memcpy(database_multiple_documents_fields.value[0][4],"10",2);
  memcpy(database_multiple_documents_fields.value[0][5],"DELEGATE_NAME",13);  
  memcpy(database_multiple_documents_fields.value[0][6],"15",2);
  memcpy(database_multiple_documents_fields.value[1][0],"XCASH",5);  
  memcpy(database_multiple_documents_fields.value[1][1],"DELEGATE_NAME",13);
  memcpy(database_multiple_documents_fields.value[1][2],"5",1);
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
  for (count = 0; (int)count < MAXIMUM_AMOUNT_OF_DELEGATES; count++)
  {
    delegates[count].public_address = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    delegates[count].total_vote_count = (char*)calloc(100,sizeof(char));
    delegates[count].IP_address = (char*)calloc(256,sizeof(char));
    delegates[count].delegate_name = (char*)calloc(MAXIMUM_BUFFER_SIZE_DELEGATES_NAME+1,sizeof(char));
    delegates[count].about = (char*)calloc(1025,sizeof(char));
    delegates[count].website = (char*)calloc(256,sizeof(char));
    delegates[count].team = (char*)calloc(256,sizeof(char));
    delegates[count].pool_mode = (char*)calloc(10,sizeof(char));
    delegates[count].fee_structure = (char*)calloc(11,sizeof(char));
    delegates[count].server_settings = (char*)calloc(1025,sizeof(char));
    delegates[count].block_verifier_score = (char*)calloc(10,sizeof(char));
    delegates[count].online_status = (char*)calloc(10,sizeof(char));
    delegates[count].block_verifier_total_rounds = (char*)calloc(10,sizeof(char));
    delegates[count].block_verifier_online_total_rounds = (char*)calloc(10,sizeof(char));
    delegates[count].block_verifier_online_percentage = (char*)calloc(10,sizeof(char));
    delegates[count].block_producer_total_rounds = (char*)calloc(10,sizeof(char));
    delegates[count].block_producer_block_heights = (char*)calloc(50000,sizeof(char));
    delegates[count].public_key = (char*)calloc(VRF_PUBLIC_KEY_LENGTH+1,sizeof(char));
    if (delegates[count].public_address == NULL || delegates[count].total_vote_count == NULL || delegates[count].IP_address == NULL || delegates[count].delegate_name == NULL || delegates[count].about == NULL || delegates[count].website == NULL || delegates[count].team == NULL || delegates[count].pool_mode == NULL || delegates[count].fee_structure == NULL || delegates[count].server_settings == NULL || delegates[count].block_verifier_score == NULL || delegates[count].online_status == NULL || delegates[count].block_verifier_total_rounds == NULL || delegates[count].block_verifier_online_total_rounds == NULL || delegates[count].block_verifier_online_percentage == NULL || delegates[count].block_producer_total_rounds == NULL || delegates[count].block_producer_block_heights == NULL || delegates[count].public_key == NULL)
    {
      color_print("Could not allocate the memory needed on the heap","red");
      exit(0);
    }
  }

  memcpy(delegates[0].public_address,"XCA1h3yJ318hJGTFeXfYyEcyE7G4hX7jrCbvz21VecJGhf64Tw51yWii2Q1e76fJbB26Ea8CRipmdW6ZHQcRrLKx3cxRkr5M12",XCASH_WALLET_LENGTH);
  memcpy(delegates[0].total_vote_count,"0",1);
  memcpy(delegates[0].IP_address,"delegates.xcash.foundation",26);
  memcpy(delegates[0].delegate_name,"delegates_xcash_foundation",26);
  memcpy(delegates[0].about,"Official X-Network node",23);
  memcpy(delegates[0].website,"delegates.xcash.foundation",26);
  memcpy(delegates[0].team,"X-Network Team",14);
  memcpy(delegates[0].pool_mode,"false",5);
  memcpy(delegates[0].fee_structure,"0",1);
  memcpy(delegates[0].server_settings,"Operating System = Ubuntu 18.04 CPU = 32 threads (Intel 2xE5 - 2660 - 2.2GHz RAM = 256GB DDR3 Hard drive = 2x240 GB SSD + 4x2TB SATA Bandwidth Transfer = Unlimited Bandwidth Speed = 500 Mbps upload and 500 Mbps download",219);
  memcpy(delegates[0].block_verifier_score,"0",1);
  memcpy(delegates[0].online_status,"true",4);
  memcpy(delegates[0].block_verifier_total_rounds,"0",1);
  memcpy(delegates[0].block_verifier_online_total_rounds,"0",1);
  memcpy(delegates[0].block_verifier_online_percentage,"0",1);
  memcpy(delegates[0].block_producer_total_rounds,"0",1);
  memcpy(delegates[0].block_producer_block_heights,"0",1);
  memcpy(delegates[0].public_key,"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a",VRF_PUBLIC_KEY_LENGTH);

  memcpy(delegates[1].public_address,"XCA1kkdrRQ9SC7wfJ3F329giei31r13wKEKiD1ZMYf6nBiU8KE6a6LDJTZobfJ58o8A3vtGrAkzfHDd815kac7F17iPhK9zYGG",XCASH_WALLET_LENGTH);
  memcpy(delegates[1].total_vote_count,"0",1);
  memcpy(delegates[1].IP_address,"officialdelegate.xcash.foundation",33);
  memcpy(delegates[1].delegate_name,"officialdelegate_xcash_foundation",33);
  memcpy(delegates[1].about,"Official X-Network Shared Delegate",34);
  memcpy(delegates[1].website,"officialdelegate.xcash.foundation",33);
  memcpy(delegates[1].team,"X-Network Team",14);
  memcpy(delegates[1].pool_mode,"true",4);
  memcpy(delegates[1].fee_structure,"0",1);
  memcpy(delegates[1].server_settings,"Operating System = Ubuntu 18.04 CPU = 8 threads (Intel E3-1246 v3 - 3.50GHz RAM = 32GB DDR3 Hard drive = 2x HDD SATA 2 TB Bandwidth Transfer = Unlimited Bandwidth Speed = 1 Gbps upload and 1 Gbps download",204);
  memcpy(delegates[1].block_verifier_score,"0",1);
  memcpy(delegates[1].online_status,"true",4);
  memcpy(delegates[1].block_verifier_total_rounds,"0",1);
  memcpy(delegates[1].block_verifier_online_total_rounds,"0",1);
  memcpy(delegates[1].block_verifier_online_percentage,"0",1);
  memcpy(delegates[1].block_producer_total_rounds,"0",1);
  memcpy(delegates[1].block_producer_block_heights,"0",1);
  memcpy(delegates[1].public_key,"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a",VRF_PUBLIC_KEY_LENGTH);

  // initialize the votes struct 
  for (count = 0; count < MAXIMUM_AMOUNT_OF_DELEGATES; count++)
  {
    votes[count].public_address_created_reserve_proof = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    votes[count].public_address_voted_for = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    votes[count].total = (char*)calloc(100,sizeof(char));
    votes[count].reserve_proof = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF+1,sizeof(char));
    if (votes[count].public_address_created_reserve_proof == NULL || votes[count].public_address_voted_for == NULL || votes[count].total == NULL || votes[count].reserve_proof == NULL)
    {
      color_print("Could not allocate the memory needed on the heap","red");
      exit(0);
    }
  }

  memcpy(votes[0].public_address_created_reserve_proof,CHECK_RESERVE_PROOF_TEST_PUBLIC_ADDRESS,sizeof(CHECK_RESERVE_PROOF_TEST_PUBLIC_ADDRESS)-1);
  memcpy(votes[0].public_address_voted_for,CHECK_RESERVE_PROOF_TEST_PUBLIC_ADDRESS,sizeof(CHECK_RESERVE_PROOF_TEST_PUBLIC_ADDRESS)-1);
  memcpy(votes[0].total,"0",1);
  memcpy(votes[0].reserve_proof,CHECK_RESERVE_PROOF_TEST_RESERVE_PROOF,sizeof(CHECK_RESERVE_PROOF_TEST_RESERVE_PROOF)-1);

  memcpy(votes[1].public_address_created_reserve_proof,CHECK_RESERVE_PROOF_TEST_PUBLIC_ADDRESS,sizeof(CHECK_RESERVE_PROOF_TEST_PUBLIC_ADDRESS)-1);
  memcpy(votes[1].public_address_voted_for,CHECK_RESERVE_PROOF_TEST_PUBLIC_ADDRESS,sizeof(CHECK_RESERVE_PROOF_TEST_PUBLIC_ADDRESS)-1);
  memcpy(votes[1].total,"0",1);
  memcpy(votes[1].reserve_proof,CHECK_RESERVE_PROOF_TEST_RESERVE_PROOF,sizeof(CHECK_RESERVE_PROOF_TEST_RESERVE_PROOF)-1);

  // reset the variables
  memset(&string1_test,0,sizeof(string1_test)); 
  memset(string2_test,0,strnlen(string2_test,BUFFER_SIZE)); 
  count_test = 0;  
  
  // get the process_id_file
  memcpy(process_id_file,"/proc/",6);
  snprintf(process_id_file+6,BUFFER_SIZE-7,"%d",getpid());
  memcpy(process_id_file+strnlen(process_id_file,BUFFER_SIZE),"/status",7);

  // initialize the arrays
  for (count = 0; count < 5; count++)
  {
    data[count] = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
    settings[count] = (char*)calloc(BUFFER_SIZE,sizeof(char)); 

    if (data[count] == NULL || settings[count] == NULL)
    {
      color_print("Could not allocate the memory needed on the heap","red");
      exit(0);
    }
  }  

  // create the the arrays
  for (count = 0; count < 5; count++)
  {
    snprintf(data[count],BUFFER_SIZE,"%zu",count);
    snprintf(settings[count],BUFFER_SIZE,"%zu",count);
  }

  // initilize the transactions
  memcpy(transactions[0],"f6458824e54ea5cddd80a6bb0105ecdd6d2248629482df2c0f989db3d46f6ebd",64);
  memcpy(transactions[1],"871eb7b29c72582572041c597ff092143031bfcef5fa1fa92808dacab2ba226f",64);
  memcpy(transactions[2],"72f97600db9d7522a2a39fc690d25e1cc9a17535064b08f81bd7424a51bba931",64);
  memcpy(transactions[3],"b180489867776c7e39c07ddb0d88609dac6f748dec554e2d96b6a168785bdb44",64);
  memcpy(transactions[4],"",1);

  // set the current_round_part and current_round_part_backup_node
  pthread_rwlock_wrlock(&rwlock);
  memset(current_round_part,0,strnlen(current_round_part,BUFFER_SIZE));
  memset(current_round_part_backup_node,0,strnlen(current_round_part_backup_node,BUFFER_SIZE));
  memcpy(current_round_part,"1",1);
  memcpy(current_round_part_backup_node,"0",1); 
  pthread_rwlock_unlock(&rwlock);

  // reset the variables
  memset(data3,0,sizeof(data3));

  // write the start test message
  color_print(TEST_OUTLINE,"blue");
  fprintf(stderr,"\033[1;34mReset variables allocated on the heap test - Total test: %d\033[0m\n",RESET_VARAIBLES_ALLOCATED_ON_THE_HEAP_TEST);
  color_print(TEST_OUTLINE,"blue");
  fprintf(stderr,"\n");


  // run the test
  CHECK_RESET_VARIABLES_ON_THE_HEAP("varint_decode",VARINT_DECODE_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("varint_encode",VARINT_ENCODE_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("network_block_string_to_blockchain_data",NETWORK_BLOCK_STRING_TO_BLOCKCHAIN_DATA_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("verify_network_block_data",VERIFY_NETWORK_BLOCK_DATA_CODE);

  RESET_BLOCKCHAIN_DATA;
  CREATE_BLOCK_DATA;
  CHECK_RESET_VARIABLES_ON_THE_HEAP("blockchain_data_to_network_block_string",BLOCKCHAIN_DATA_TO_NETWORK_BLOCK_STRING_CODE);

  CHECK_RESET_VARIABLES_ON_THE_HEAP("create_database_connection",CREATE_DATABASE_CONNECTION_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("get_database_data_hash",GET_DATABASE_DATA_HASH_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("get_database_data",GET_DATABASE_DATA_CODE);

  CHECK_RESET_VARIABLES_ON_THE_HEAP("count_documents_in_collection",COUNT_DOCUMENTS_IN_COLLECTION_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("count_all_documents_in_collection",COUNT_ALL_DOCUMENTS_IN_COLLECTION_CODE);

  CHECK_RESET_VARIABLES_ON_THE_HEAP("delete_document_from_collection",DELETE_DOCUMENT_FROM_COLLECTION_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("delete_collection_from_database",DELETE_COLLECTION_FROM_DATABASE_CODE);

  CHECK_RESET_VARIABLES_ON_THE_HEAP("insert_document_into_collection_array",INSERT_DOCUMENT_INTO_COLLECTION_ARRAY_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("insert_document_into_collection_json",INSERT_DOCUMENT_INTO_COLLECTION_JSON_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("insert_multiple_documents_into_collection_json",INSERT_MULTIPLE_DOCUMENTS_INTO_COLLECTION_JSON_CODE);
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST,0);

  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,MESSAGE,0);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("read_document_from_collection",READ_DOCUMENT_FROM_COLLECTION_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("read_document_field_from_collection",READ_DOCUMENT_FIELD_FROM_COLLECTION_CODE);
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST,0);
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,DATABASE_COLLECTION_STATISTICS_TEST_DATA,0); 
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
  memcpy(database_data.value[2],"5",1);
  memcpy(database_data.value[3],"DELEGATE_NAME",13);
  memcpy(database_data.value[4],"10",2);
  memcpy(database_data.value[5],"DELEGATE_NAME",13);  
  memcpy(database_data.value[6],"15",2);
  database_data.count = 7;
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST,0);
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,DATABASE_COLLECTION_STATISTICS_TEST_DATA,0);
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,DATABASE_COLLECTION_STATISTICS_TEST_DATA,0);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("read_multiple_documents_all_fields_from_collection",READ_MULTIPLE_DOCUMENTS_ALL_FIELDS_FROM_COLLECTION_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("database_document_parse_json_data",DATABASE_DOCUMENT_PARSE_JSON_DATA_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("database_multiple_documents_parse_json_data",DATABASE_MULTIPLE_DOCUMENTS_PARSE_JSON_DATA_CODE);
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST,0);
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,DATABASE_COLLECTION_DELEGATES_TEST_DATA,0);
  
  CHECK_RESET_VARIABLES_ON_THE_HEAP("update_document_from_collection",UPDATE_DOCUMENT_FROM_COLLECTION_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("update_all_documents_from_collection",UPDATE_ALL_DOCUMENTS_FROM_COLLECTION_CODE);
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST,0);

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
  CHECK_RESET_VARIABLES_ON_THE_HEAP("check_found_block",CHECK_FOUND_BLOCK_CODE);

  CHECK_RESET_VARIABLES_ON_THE_HEAP("sign_data using wallet signing key",SIGN_DATA_USING_WALLET_SIGNING_KEY_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("verify_data using wallet signing key",VERIFY_DATA_USING_WALLET_SIGNING_KEY_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("sign_data using ECDSA block verifier signing key",SIGN_DATA_USING_ECDSA_BLOCK_VERIFIER_SIGNING_KEY_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("verify_data using ECDSA block verifier signing key",VERIFY_DATA_USING_ECDSA_BLOCK_VERIFIER_SIGNING_KEY_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("validate_data",VALIDATE_DATA_CODE);

  CHECK_RESET_VARIABLES_ON_THE_HEAP("get_public_address",GET_PUBLIC_ADDRESS_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("check_reserve_proofs",CHECK_RESERVE_PROOFS_CODE);  

  CHECK_RESET_VARIABLES_ON_THE_HEAP("parse_json_data",PARSE_JSON_DATA_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("random_string",RANDOM_STRING_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("parse_reserve_bytes_data",PARSE_RESERVE_BYTES_DATA_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("create_json_data_from_database_document_array",CREATE_JSON_DATA_FROM_DATABASE_DOCUMENT_ARRAY_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("create_json_data_from_database_multiple_documents_array",CREATE_JSON_DATA_FROM_DATABASE_MULTIPLE_DOCUMENTS_ARRAY_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("create_json_data_from_delegates_array",CREATE_JSON_DATA_FROM_DELEGATES_ARRAY_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("create_json_data_from_votes_array",CREATE_JSON_DATA_FROM_VOTES_ARRAY_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("string_count",STRING_COUNT_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("string_replace",STRING_REPLACE_CODE);

  CHECK_RESET_VARIABLES_ON_THE_HEAP("create_random_VRF_keys",CREATE_RANDOM_VRF_KEYS_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("crypto_vrf_is_valid_key",CRYPTO_VRF_IS_VALID_KEY_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("crypto_vrf_keypair_from_seed",CRYPTO_VRF_KEYPAIR_FROM_SEED_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("crypto_vrf_prove",CRYPTO_VRF_PROVE_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("crypto_vrf_proof_to_hash",CRYPTO_VRF_PROOF_TO_HASH_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("crypto_vrf_verify",CRYPTO_VRF_VERIFY_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("crypto_hash_sha512",CRYPTO_HASH_SHA512_CODE);
  CHECK_RESET_VARIABLES_ON_THE_HEAP("sign_network_block_string",SIGN_NETWORK_BLOCK_STRING_CODE);
  

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
    pointer_reset(data[count]);
    pointer_reset(settings[count]);
  }
  pointer_reset(data);
  pointer_reset(settings);

  for (count = 0; count < 5; count++)
  {
    pointer_reset(transactions[count]);
  }

  // write the end test message
  if (count_test == RESET_VARAIBLES_ALLOCATED_ON_THE_HEAP_TEST)
  {
    fprintf(stderr,"\n");
    color_print(TEST_OUTLINE,"green");
    fprintf(stderr,"\033[1;32mReset variables allocated on the heap test - Passed test: %d, Failed test: 0\033[0m\n",RESET_VARAIBLES_ALLOCATED_ON_THE_HEAP_TEST);
    color_print(TEST_OUTLINE,"green");
    fprintf(stderr,"\n\n");
  }
  else
  {
    fprintf(stderr,"\n");
    color_print(TEST_OUTLINE,"red");
    fprintf(stderr,"\033[1;31mReset variables allocated on the heap test - Passed test: %d, Failed test: 1\033[0m\n",count_test);
    color_print(TEST_OUTLINE,"red");
    fprintf(stderr,"\n\n");
    exit(0);
  } 
  pointer_reset(process_id_file);
  return count_test;

  #undef RESET_VARAIBLES_ALLOCATED_ON_THE_HEAP_TEST
  #undef RESET_ERROR_MESSAGES
  #undef GET_PUBLIC_ADDRESS_DATA
  #undef GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA
  #undef DATA1
  #undef DATA2
  #undef NETWORK_BLOCK_HEIGHT
  #undef VARINT_ENCODED_VALUE_1
  #undef VARINT_DECODED_VALUE_1
  #undef DATA_HASH_TEXT
  #undef MESSAGE
  #undef DATABASE_COLLECTION_STATISTICS_TEST_DATA
}