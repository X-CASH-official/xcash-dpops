#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "define_macro_functions.h"
#include "define_macros_test.h"

#include "insert_database_functions.h"
#include "update_database_functions.h"
#include "delete_database_functions.h"
#include "thread_functions.h"

#include "variables_test.h"
#include "update_database_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: update_database_functions_test
Description: Test the update database functions
Return: The number of passed update database functions test
-----------------------------------------------------------------------------------------------------------
*/

int update_database_functions_test(void)
{   
  // Variables 
  pthread_t thread_id;

  // define macros
  #define UPDATE_DATABASE_FUNCTIONS_TEST 4
  #define MESSAGE "{\"message_settings\": \"XCASH_DPOPS_TEST_DATA\"}"
  #define MESSAGE_SETTINGS "{\"message_settings\": \"XCASH_DPOPS_DATA\"}"

  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST,0);

  // write the start test message
  color_print(TEST_OUTLINE,"blue");
  fprintf(stderr,"\033[1;34mupdate database functions test - Total test: %d\033[0m\n",UPDATE_DATABASE_FUNCTIONS_TEST);
  color_print(TEST_OUTLINE,"blue");
  fprintf(stderr,"\n");

  // reset the variables
  memset(data_test,0,sizeof(data_test));
  count_test = 0;

  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,MESSAGE,0);
  // update a document in the collection
  if (update_document_from_collection(database_name,DATABASE_COLLECTION_TEST,MESSAGE,MESSAGE_SETTINGS,0) == 0)
  {
    color_print("FAILED! Test for update_document_from_collection","red");
  }
  else
  {
    color_print("PASSED! Test for update_document_from_collection","green");
    count_test++;
  }

  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST,0);
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,MESSAGE,0);
  // update a document in the collection on a separate thread
  struct update_document_from_collection_thread_parameters update_document_from_collection_thread_parameters = {database_name,DATABASE_COLLECTION_TEST,MESSAGE,MESSAGE_SETTINGS};
  pthread_create(&thread_id, NULL, &update_document_from_collection_thread,(void *)&update_document_from_collection_thread_parameters);
  if (thread_settings(thread_id) == 0)
  {
    color_print("FAILED! Test for update_document_from_collection_thread","red");
  }
  else
  {
    color_print("PASSED! Test for update_document_from_collection_thread","green");
    count_test++;
  }

  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST,0);
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,MESSAGE,0);
  // update all document in the collection
  if (update_all_documents_from_collection(database_name,DATABASE_COLLECTION_TEST,MESSAGE,0) == 0)
  {
    color_print("FAILED! Test for update_all_documents_from_collection","red");
  }
  else
  {
    color_print("PASSED! Test for update_all_documents_from_collection","green");
    count_test++;
  }

  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST,0);
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,MESSAGE,0);
  // update all document in the collection on a separate thread
  struct update_all_documents_from_collection_thread_parameters update_all_documents_from_collection_thread_parameters = {database_name,DATABASE_COLLECTION_TEST,MESSAGE};
  pthread_create(&thread_id, NULL, &update_all_documents_from_collection_thread,(void *)&update_all_documents_from_collection_thread_parameters);
  if (thread_settings(thread_id) == 0)
  {
    color_print("FAILED! Test for update_all_documents_from_collection_thread","red");
  }
  else
  {
    color_print("PASSED! Test for update_all_documents_from_collection_thread","green");
    count_test++;
  }

  // write the end test message
  if (count_test == UPDATE_DATABASE_FUNCTIONS_TEST)
  {
    fprintf(stderr,"\n");
    color_print(TEST_OUTLINE,"green");
    fprintf(stderr,"\033[1;32mupdate database functions test - Passed test: %d, Failed test: 0\033[0m\n",UPDATE_DATABASE_FUNCTIONS_TEST);
    color_print(TEST_OUTLINE,"green");
    fprintf(stderr,"\n\n");
  }
  else
  {
    fprintf(stderr,"\n");
    color_print(TEST_OUTLINE,"red");
    fprintf(stderr,"\033[1;31mupdate database functions test - Passed test: %d, Failed test: %d\033[0m\n",count_test,UPDATE_DATABASE_FUNCTIONS_TEST-count_test);
    color_print(TEST_OUTLINE,"red");
    fprintf(stderr,"\n\n");
  } 
  return count_test;
  
  #undef UPDATE_DATABASE_FUNCTIONS_TEST
  #undef MESSAGE
  #undef MESSAGE_SETTINGS
}