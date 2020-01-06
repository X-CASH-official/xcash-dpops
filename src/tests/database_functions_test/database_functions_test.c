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
  #define DATABASE_FUNCTIONS_TEST 5
  #define MESSAGE "{\"message_settings\" : \"XCASH_DPOPS_TEST_DATA\" }"
  #define DATA_HASH "1e3c10c2d5396ac7dcce22ad7488db8c6d9ba6ec07660283f5e2198cba6507c8a136536e8c4ee16f43cf04a19fa84b5575b34e3186426b2b6db3101e9e25abd9FAILED"

  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST,0);

  // write the start test message
  color_print(TEST_OUTLINE,"blue");
  fprintf(stderr,"\033[1;34mdatabase functions test - Total test: %d\033[0m\n",DATABASE_FUNCTIONS_TEST);
  color_print(TEST_OUTLINE,"blue");
  fprintf(stderr,"\n");

  // reset the variables
  memset(data_test,0,sizeof(data_test));
  count_test = 0;

  // test create_database_connection
  mongoc_client_destroy(database_client);
  if (create_database_connection() == 1)
  {
    color_print("PASSED! Test for create_database_connection","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for create_database_connection","red");
  }

  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,DATABASE_COLLECTION_DELEGATES_TEST_DATA,0);

  // get the database data hash
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST,0);
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,DATABASE_COLLECTION_STATISTICS_TEST_DATA,0);
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,DATABASE_COLLECTION_STATISTICS_TEST_DATA,0);
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (get_database_data_hash(data_test,database_name,DATABASE_COLLECTION_TEST) == 1 && memcmp(data_test,DATA_HASH,DATA_HASH_LENGTH) == 0)
  {
    color_print("PASSED! Test for get_database_data_hash","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for get_database_data_hash","red");
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST,0);

  // get the database data hash on a separate thread
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST,0);
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,DATABASE_COLLECTION_STATISTICS_TEST_DATA,0);
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,DATABASE_COLLECTION_STATISTICS_TEST_DATA,0);
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  struct get_database_data_hash_thread_parameters get_database_data_hash_thread_parameters = {data_test,database_name,DATABASE_COLLECTION_TEST};
  pthread_create(&thread_id, NULL, &get_database_data_hash_thread,(void *)&get_database_data_hash_thread_parameters);
  if (thread_settings(thread_id) == 1 && memcmp(data_test,DATA_HASH,DATA_HASH_LENGTH) == 0)
  {
    color_print("PASSED! Test for get_database_data_hash_thread","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for get_database_data_hash_thread","red");
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST,0);

  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,MESSAGE,0);
  // get the database data
  memset(result_test,0,strlen(result_test));
  if (get_database_data(result_test,database_name,DATABASE_COLLECTION_TEST,0) == 1 && strncmp(result_test,MESSAGE,BUFFER_SIZE) == 0)
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
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST,0);

  // write the end test message
  if (count_test == DATABASE_FUNCTIONS_TEST)
  {
    fprintf(stderr,"\n");
    color_print(TEST_OUTLINE,"green");
    fprintf(stderr,"\033[1;32mdatabase functions test - Passed test: %d, Failed test: 0\033[0m\n",DATABASE_FUNCTIONS_TEST);
    color_print(TEST_OUTLINE,"green");
    fprintf(stderr,"\n\n");
  }
  else
  {
    fprintf(stderr,"\n");
    color_print(TEST_OUTLINE,"red");
    fprintf(stderr,"\033[1;31mdatabase functions test - Passed test: %d, Failed test: %d\033[0m\n",count_test,DATABASE_FUNCTIONS_TEST-count_test);
    color_print(TEST_OUTLINE,"red");
    fprintf(stderr,"\n\n");
  } 
  return count_test;
  
  #undef DATABASE_FUNCTIONS_TEST
  #undef MESSAGE
}