#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "define_macro_functions.h"
#include "define_macros_test.h"

#include "insert_database_functions.h"
#include "count_database_functions.h"
#include "delete_database_functions.h"
#include "thread_functions.h"

#include "variables_test.h"
#include "count_database_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: count_database_functions_test
Description: Test the count database functions
Return: The number of passed count database functions test
-----------------------------------------------------------------------------------------------------------
*/

int count_database_functions_test(void)
{   
  // Variables 
  pthread_t thread_id;

  // define macros
  #define COUNT_DATABASE_FUNCTIONS_TEST 4
  #define MESSAGE "{\"message_settings\": \"XCASH_DPOPS_TEST_DATA\"}"

  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST,0);
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,MESSAGE,0);

  // reset the variables
  count_test = 0;

  // write the start test message
  color_print(TEST_OUTLINE,"blue");
  fprintf(stderr,"\033[1;34mcount database functions test - Total test: %d\033[0m\n",COUNT_DATABASE_FUNCTIONS_TEST);
  color_print(TEST_OUTLINE,"blue");
  fprintf(stderr,"\n");

  // count how many documents have "message_settings":"XCASH_DPOPS_TEST_DATA" in the collection
  int count1 = count_documents_in_collection(database_name,DATABASE_COLLECTION_TEST,MESSAGE,0);
  int count2 = count_all_documents_in_collection(database_name,DATABASE_COLLECTION_TEST,0);
  if ((count1 == count2) && (count1 != -1 || count2 != -1))
  {
    color_print("PASSED! Test for count_documents_in_collection","green");
    color_print("PASSED! Test for count_all_documents_in_collection","green");
    count_test+= 2;   
  }
  else
  {
    color_print("FAILED! Test for count_documents_in_collection","red");
    color_print("FAILED! Test for count_all_documents_in_collection","red");
  }

  // count how many documents have "message_settings":"XCASH_DPOPS_TEST_DATA" in the collection on a separate thread
  struct count_documents_in_collection_thread_parameters count_documents_in_collection_thread_parameters = {database_name,DATABASE_COLLECTION_TEST,MESSAGE};
  pthread_create(&thread_id, NULL, &count_documents_in_collection_thread,(void *)&count_documents_in_collection_thread_parameters);
  count1 = thread_settings(thread_id);
  struct count_all_documents_in_collection_thread_parameters count_all_documents_in_collection_thread_parameters = {database_name,DATABASE_COLLECTION_TEST};
  pthread_create(&thread_id, NULL, &count_all_documents_in_collection_thread,(void *)&count_all_documents_in_collection_thread_parameters);
  count2 = thread_settings(thread_id);
  if ((count1 == count2) && (count1 != -1 || count2 != -1))
  {
    color_print("PASSED! Test for count_documents_in_collection_thread","green");
    color_print("PASSED! Test for count_all_documents_in_collection_thread","green");
    count_test+= 2;   
  }
  else
  {
    color_print("FAILED! Test for count_documents_in_collection_thread","red");
    color_print("FAILED! Test for count_all_documents_in_collection_thread","red");
  }
  
  // write the end test message
  if (count_test == COUNT_DATABASE_FUNCTIONS_TEST)
  {
    fprintf(stderr,"\n");
    color_print(TEST_OUTLINE,"green");
    fprintf(stderr,"\033[1;32mcount database functions test - Passed test: %d, Failed test: 0\033[0m\n",COUNT_DATABASE_FUNCTIONS_TEST);
    color_print(TEST_OUTLINE,"green");
    fprintf(stderr,"\n\n");
  }
  else
  {
    fprintf(stderr,"\n");
    color_print(TEST_OUTLINE,"red");
    fprintf(stderr,"\033[1;31mcount database functions test - Passed test: %d, Failed test: %d\033[0m\n",count_test,COUNT_DATABASE_FUNCTIONS_TEST-count_test);
    color_print(TEST_OUTLINE,"red");
    fprintf(stderr,"\n\n");
  } 
  return count_test;
  
  #undef DATA_COUNT
  #undef MESSAGE
  #undef MESSAGE_SETTINGS
}