#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "define_macros_test.h"

#include "database_functions.h"
#include "insert_database_functions.h"
#include "read_database_functions.h"
#include "update_database_functions.h"
#include "delete_database_functions.h"
#include "count_database_functions.h"
#include "thread_functions.h"

#include "variables_test.h"
#include "database_functions_test.h"
#include "network_functions.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: database_test
Description: Test the database functions
Return: The number of passed database test
-----------------------------------------------------------------------------------------------------------
*/

int database_test(void)
{   
  // Variables 
  pthread_t thread_id;

  // define macros
  #define MESSAGE "{\"message_settings\" : \"XCASH_DPOPS_TEST_DATA\" }"
  #define DATA_HASH "1e3c10c2d5396ac7dcce22ad7488db8c6d9ba6ec07660283f5e2198cba6507c8a136536e8c4ee16f43cf04a19fa84b5575b34e3186426b2b6db3101e9e25abd9"
  #define DATA_HASH_SPECIFIC_RESERVE_PROOFS_COLLECTION "793d24d87408f8950863f32fd8741a2aa17b63d4073437634a6319063cc5195cbe07f0e93b5f3d380a8be3944b95fd077cf00bfb90b3f426cc909a0b7f22b419"
  #define DATA_HASH_SPECIFIC_RESERVE_BYTES_COLLECTION "f72d2a9763eeff6d19e93a7e2341d9e0af7a71c239af3272c466ef67c3ed1ad14c64bf84ebca0f69f4210a09ea184c8713d5dec27b2033fb2460b1ee9be4636b"
  #define DATA_HASH_DELEGATES_COLLECTION "208aad7142cf0aea98859528b826dbaa863829c97590ce9f5b1cc604cd84b4140bc1ceee35b0b0dd67c1110026a7c25550caed8f084851a69b7e6aa91f8c2fd8"
  #define DATA_HASH_STATISTICS_COLLECTION "162d01beab70213c2607eb2c77593e4a3fc90c8069dd633ec0ff98700329313840c9231771fd5b82721e4d176b1f62d756e2f9558fda81aff48e3a820a47908a"
  #define DATA_HASH_ALL_RESERVE_PROOFS_COLLECTIONS "7d79ede6a779f483e03bca6a3670c2b9a2272240c74498d7bd77e5ce314a4a6ee8c002344af315416dc52b5dfe5d9878333972cbb161aeb93ec8e0ea5bd8f2e1"
  #define DATA_HASH_ALL_RESERVE_BYTES_COLLECTIONS "1eba4b3c56c6d25f91b5ce6340c49f6a8d1a69f21b4f02584863aa55b27edd453e6352ab0ae838f5ddbde1c1ed06d0684c00f4bfa8d5b0b9b74b61b909b96daf"
  #define DATA_HASH_ALL_COLLECTIONS "56aac64d6320c14db5f03e964962449e34252aadc704989e1969c8d133172664b13818d04a95595deed8d1b9c133ffcdf5ad2c7ace7ecf66e204772da355c743"

  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST);

  // write the start test message
  fprintf(stderr,"\033[1;34m%s\ndatabase functions test - Total test: %d\n%s\n\n\033[0m",TEST_OUTLINE,DATABASE_FUNCTIONS_TEST,TEST_OUTLINE);

  // reset the variables
  memset(data_test,0,sizeof(data_test));
  count_test = 0;

  // test check_if_database_collection_exist
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,DATABASE_COLLECTION_DELEGATES_TEST_DATA);
  if (check_if_database_collection_exist(database_name,DATABASE_COLLECTION_TEST) == 1)
  {
    color_print("PASSED! Test for check_if_database_collection_exist","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for check_if_database_collection_exist","red");
  }

  // get the database data hash
  delete_database(database_name);
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,DATABASE_COLLECTION_STATISTICS_TEST_DATA);
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,DATABASE_COLLECTION_STATISTICS_TEST_DATA);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",RESERVE_PROOFS_TEST_DATA);
  insert_document_into_collection_json(database_name,"reserve_proofs_10",RESERVE_PROOFS_TEST_DATA);
  insert_document_into_collection_json(database_name,"reserve_bytes_1",RESERVE_BYTES_TEST_DATA);
  insert_document_into_collection_json(database_name,"reserve_bytes_10",RESERVE_BYTES_TEST_DATA);
  insert_document_into_collection_json(database_name,"delegates",DELEGATES_TEST_DATA);
  insert_document_into_collection_json(database_name,"statistics",DATABASE_COLLECTION_STATISTICS_DATA);
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (get_database_data_hash(data_test,database_name,DATABASE_COLLECTION_TEST) == 1 && strncmp(data_test,DATA_HASH,DATA_HASH_LENGTH) == 0)
  {
    color_print("PASSED! Test for get_database_data_hash for XCASH_PROOF_OF_STAKE_TEST_DATA collection","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for get_database_data_hash for XCASH_PROOF_OF_STAKE_TEST_DATA collection","red");
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (get_database_data_hash(data_test,database_name,"reserve_proofs_1") == 1 && strncmp(data_test,DATA_HASH_SPECIFIC_RESERVE_PROOFS_COLLECTION,DATA_HASH_LENGTH) == 0)
  {
    color_print("PASSED! Test for get_database_data_hash for a specific reserve proofs collection","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for get_database_data_hash for a specific reserve proofs collection","red");
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (get_database_data_hash(data_test,database_name,"reserve_bytes_1") == 1 && strncmp(data_test,DATA_HASH_SPECIFIC_RESERVE_BYTES_COLLECTION,DATA_HASH_LENGTH) == 0)
  {
    color_print("PASSED! Test for get_database_data_hash for a specific reserve bytes collection","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for get_database_data_hash for a specific reserve bytes collection","red");
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (get_database_data_hash(data_test,database_name,"delegates") == 1 && strncmp(data_test,DATA_HASH_DELEGATES_COLLECTION,DATA_HASH_LENGTH) == 0)
  {
    color_print("PASSED! Test for get_database_data_hash for delegates collection","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for get_database_data_hash for delegates collection","red");
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (get_database_data_hash(data_test,database_name,"statistics") == 1 && strncmp(data_test,DATA_HASH_STATISTICS_COLLECTION,DATA_HASH_LENGTH) == 0)
  {
    color_print("PASSED! Test for get_database_data_hash for statistics collection","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for get_database_data_hash for statistics collection","red");
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (get_database_data_hash(data_test,database_name,"reserve_proofs") == 1 && strncmp(data_test,DATA_HASH_ALL_RESERVE_PROOFS_COLLECTIONS,DATA_HASH_LENGTH) == 0)
  {
    color_print("PASSED! Test for get_database_data_hash for all reserve proofs collections","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for get_database_data_hash for all reserve proofs collections","red");
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (get_database_data_hash(data_test,database_name,"reserve_bytes") == 1 && strncmp(data_test,DATA_HASH_ALL_RESERVE_BYTES_COLLECTIONS,DATA_HASH_LENGTH) == 0)
  {
    color_print("PASSED! Test for get_database_data_hash for all reserve bytes collections","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for get_database_data_hash for all reserve bytes collections","red");
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (get_database_data_hash(data_test,database_name,"ALL") == 1 && strncmp(data_test,DATA_HASH_ALL_COLLECTIONS,DATA_HASH_LENGTH) == 0)
  {
    color_print("PASSED! Test for get_database_data_hash for ALL collections","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for get_database_data_hash for ALL collections","red");
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  delete_database(database_name);

  // get the database data hash on a separate thread
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST);
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,DATABASE_COLLECTION_STATISTICS_TEST_DATA);
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,DATABASE_COLLECTION_STATISTICS_TEST_DATA);
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  struct get_database_data_hash_thread_parameters get_database_data_hash_thread_parameters = {data_test,database_name,DATABASE_COLLECTION_TEST};
  pthread_create(&thread_id, NULL, &get_database_data_hash_thread,(void *)&get_database_data_hash_thread_parameters);
  if (thread_settings(thread_id) == 1 && strncmp(data_test,DATA_HASH,DATA_HASH_LENGTH) == 0)
  {
    color_print("PASSED! Test for get_database_data_hash_thread","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for get_database_data_hash_thread","red");
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST);

  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,MESSAGE);
  // get the database data
  memset(result_test,0,strlen(result_test));
  if (get_database_data(result_test,database_name,DATABASE_COLLECTION_TEST) == 1 && strncmp(result_test,MESSAGE,BUFFER_SIZE) == 0)
  {
    color_print("PASSED! Test for get_database_data","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for get_database_data","red");
  }

  // get the database data on a separate thread
  memset(result_test,0,strlen(result_test));
  struct get_database_data_thread_parameters get_database_data_thread_parameters = {result_test,database_name,DATABASE_COLLECTION_TEST};
  pthread_create(&thread_id, NULL, &get_database_data_thread,(void *)&get_database_data_thread_parameters);
  if (thread_settings(thread_id) == 1 && strncmp(result_test,MESSAGE,BUFFER_SIZE) == 0)
  {
    color_print("PASSED! Test for get_database_data_thread","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for get_database_data_thread","red");
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST);

  // write the end test message
  fprintf(stderr,"\033[1;33m\n\n%s\ndatabase functions test - Passed test: %d, Failed test: %d\n%s\n\n\n\033[0m",TEST_OUTLINE,count_test,DATABASE_FUNCTIONS_TEST-count_test,TEST_OUTLINE);
  return count_test;
  
  #undef MESSAGE
  #undef DATA_HASH
}