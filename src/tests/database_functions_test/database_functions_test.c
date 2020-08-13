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
  #define DATA_HASH_SPECIFIC_RESERVE_PROOFS_COLLECTION "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000075ac78b518b997f798f9d52540c9011f"
  #define DATA_HASH_SPECIFIC_RESERVE_BYTES_COLLECTION "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000064bfb449381660ce9f2f70fe58234921"
  #define DATA_HASH_DELEGATES_COLLECTION "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005d0543cbfbf03c0ca92862ec7c07bab0"
  #define DATA_HASH_STATISTICS_COLLECTION "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000a7acbe295605ef75b73129d637178ca6"
  #define DATA_HASH_ALL_RESERVE_PROOFS_COLLECTIONS "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000d41d8cd98f00b204e9800998ecf8427e"
  #define DATA_HASH_ALL_RESERVE_BYTES_COLLECTIONS "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000064bfb449381660ce9f2f70fe58234921"
  #define DATA_HASH_ALL_COLLECTIONS "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000019ac46867a5e67ca1347db33273f295f"

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
  insert_document_into_collection_json(database_name,"reserve_proofs_1",RESERVE_PROOFS_TEST_DATA);
  insert_document_into_collection_json(database_name,"reserve_proofs_10",RESERVE_PROOFS_TEST_DATA);
  insert_document_into_collection_json(database_name,"reserve_bytes_1",RESERVE_BYTES_TEST_DATA);
  insert_document_into_collection_json(database_name,"reserve_bytes_10",RESERVE_BYTES_TEST_DATA);
  insert_document_into_collection_json(database_name,"delegates",DELEGATES_TEST_DATA);
  insert_document_into_collection_json(database_name,"statistics",DATABASE_COLLECTION_STATISTICS_DATA);
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

  delete_database(database_name);
  insert_document_into_collection_json(database_name,"reserve_proofs_1",RESERVE_PROOFS_TEST_DATA);
  insert_document_into_collection_json(database_name,"reserve_proofs_10",RESERVE_PROOFS_TEST_DATA);
  insert_document_into_collection_json(database_name,"reserve_bytes_1",RESERVE_BYTES_TEST_DATA);
  insert_document_into_collection_json(database_name,"reserve_bytes_10",RESERVE_BYTES_TEST_DATA);
  insert_document_into_collection_json(database_name,"delegates",DELEGATES_TEST_DATA);
  insert_document_into_collection_json(database_name,"statistics",DATABASE_COLLECTION_STATISTICS_DATA);
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
    color_print(result_test,"yellow");
    color_print("FAILED! Test for get_database_data_thread","red");
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST);

  // write the end test message
  fprintf(stderr,"\033[1;33m\n\n%s\ndatabase functions test - Passed test: %d, Failed test: %d\n%s\n\n\n\033[0m",TEST_OUTLINE,count_test,DATABASE_FUNCTIONS_TEST-count_test,TEST_OUTLINE);
  return count_test;
  
  #undef MESSAGE
}