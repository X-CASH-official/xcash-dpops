#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "define_macro_functions.h"
#include "define_macros_test.h"

#include "insert_database_functions.h"
#include "delete_database_functions.h"
#include "thread_functions.h"

#include "variables_test.h"
#include "insert_database_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: insert_database_functions_test
Description: Test the insert database functions
Return: The number of passed insert database functions test
-----------------------------------------------------------------------------------------------------------
*/

int insert_database_functions_test(void)
{   
  // Variables 
  pthread_t thread_id;

  // define macros
  #define INSERT_DATABASE_FUNCTIONS_TEST 5
  #define DATA_COUNT 5
  #define MESSAGE "{\"message_settings\": \"XCASH_DPOPS_TEST_DATA\"}"
  #define MESSAGE_SETTINGS "{\"message_settings\": \"XCASH_DPOPS_DATA\"}"

  // reset the variables
  memset(data_test,0,sizeof(data_test));
  count_test = 0;

  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST,0);

  // write the start test message
  fprintf(stderr,"\033[1;34m%s\ninsert database functions test - Total test: %d\n%s\n\n\033[0m",TEST_OUTLINE,INSERT_DATABASE_FUNCTIONS_TEST,TEST_OUTLINE);
  
  // insert a document into the collection using json data
  if (insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,MESSAGE,0) == 0)
  {
    color_print("FAILED! Test for insert_document_into_collection_json","red");
  }
  else
  {
    color_print("PASSED! Test for insert_document_into_collection_json","green");
    count_test++;
  }
  
  // insert a document into the collection using json data on a separate thread
  struct insert_document_into_collection_json_thread_parameters insert_document_into_collection_json_thread_parameters = {database_name,DATABASE_COLLECTION_TEST,MESSAGE};
  pthread_create(&thread_id, NULL, &insert_document_into_collection_json_thread,(void *)&insert_document_into_collection_json_thread_parameters);
  if (thread_settings(thread_id) == 0)
  {
    color_print("FAILED! Test for insert_document_into_collection_json_thread","red");
  }
  else
  {
    color_print("PASSED! Test for insert_document_into_collection_json_thread","green");
    count_test++;
  }

  // insert multiple documents into the collection using json data
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(data_test,DATABASE_COLLECTION_STATISTICS_TEST_DATA,strnlen(DATABASE_COLLECTION_STATISTICS_TEST_DATA,BUFFER_SIZE));
  memcpy(data_test+strlen(data_test),",",1);
  memcpy(data_test+strlen(data_test),DATABASE_COLLECTION_STATISTICS_TEST_DATA,strnlen(DATABASE_COLLECTION_STATISTICS_TEST_DATA,BUFFER_SIZE));
  if (insert_multiple_documents_into_collection_json(database_name,DATABASE_COLLECTION_TEST,data_test,BUFFER_SIZE,0) == 1)
  {
    color_print("PASSED! Test for insert_multiple_documents_into_collection_json","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for insert_multiple_documents_into_collection_json","red");    
  }

  // insert multiple documents into the collection using json data on a separate thread
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(data_test,DATABASE_COLLECTION_STATISTICS_TEST_DATA,strnlen(DATABASE_COLLECTION_STATISTICS_TEST_DATA,BUFFER_SIZE));
  memcpy(data_test+strlen(data_test),",",1);
  memcpy(data_test+strlen(data_test),DATABASE_COLLECTION_STATISTICS_TEST_DATA,strnlen(DATABASE_COLLECTION_STATISTICS_TEST_DATA,BUFFER_SIZE));
  struct insert_multiple_documents_into_collection_json_thread_parameters insert_multiple_documents_into_collection_json_thread_parameters = {database_name,DATABASE_COLLECTION_TEST,data_test,BUFFER_SIZE};
  pthread_create(&thread_id, NULL, &insert_multiple_documents_into_collection_json_thread,(void *)&insert_multiple_documents_into_collection_json_thread_parameters);
  if (thread_settings(thread_id) == 1)
  {
    color_print("PASSED! Test for insert_multiple_documents_into_collection_json_thread","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for insert_multiple_documents_into_collection_json_thread","red");    
  }

  // write the end test message
  fprintf(stderr,"\033[1;33m\n\n%s\ninsert database functions test - Passed test: %d, Failed test: %d\n%s\n\n\n\033[0m",TEST_OUTLINE,count_test,INSERT_DATABASE_FUNCTIONS_TEST-count_test,TEST_OUTLINE);
  return count_test;
  
  #undef DATA_COUNT
  #undef MESSAGE
  #undef MESSAGE_SETTINGS
}