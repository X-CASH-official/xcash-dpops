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
  #define DATA_HASH "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000008fb47efae4253533cc04467def75eb70"
  #define DATA_HASH_SPECIFIC_RESERVE_PROOFS_COLLECTION "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000042ae832cc1c124b37875629a0307961"
  #define DATA_HASH_SPECIFIC_RESERVE_BYTES_COLLECTION "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000f0183e1f40c427df35b8e1063a83f040"
  #define DATA_HASH_DELEGATES_COLLECTION "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000039e1a0c6ca4d4f1c0db37bca848c5c85"
  #define DATA_HASH_STATISTICS_COLLECTION "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000bc618f4f1e9cab65d9c53cd496b2a3d3"
  #define DATA_HASH_ALL_RESERVE_PROOFS_COLLECTIONS "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000d41d8cd98f00b204e9800998ecf8427e"
  #define DATA_HASH_ALL_RESERVE_BYTES_COLLECTIONS "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000f0183e1f40c427df35b8e1063a83f040"
  #define DATA_HASH_ALL_COLLECTIONS "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000a37799518241e1f8d1fd3c8e2f1f3f49"

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
  memset(result_test,0,sizeof(result_test));
  if (get_database_data(result_test,database_name,DATABASE_COLLECTION_TEST) == 1 && strncmp(result_test,MESSAGE,BUFFER_SIZE) == 0)
  {
    color_print("PASSED! Test for get_database_data","green");
    count_test++;
  }
  else
  {
    color_print(result_test,"yellow");
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