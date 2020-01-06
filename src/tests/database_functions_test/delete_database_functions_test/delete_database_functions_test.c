#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "define_macro_functions.h"
#include "define_macros_test.h"

#include "insert_database_functions.h"
#include "delete_database_functions.h"
#include "thread_functions.h"

#include "variables_test.h"
#include "delete_database_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: delete_database_functions_test
Description: Test the delete database functions
Return: The number of passed delete database functions test
-----------------------------------------------------------------------------------------------------------
*/

int delete_database_functions_test(void)
{   
  // Variables 
  pthread_t thread_id;

  // define macros
  #define DELETE_DATABASE_FUNCTIONS_TEST 4
  #define MESSAGE "{\"message_settings\": \"XCASH_DPOPS_TEST_DATA\"}"

  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST,0);

  // write the start test message
  color_print(TEST_OUTLINE,"blue");
  fprintf(stderr,"\033[1;34mdelete database functions test - Total test: %d\033[0m\n",DELETE_DATABASE_FUNCTIONS_TEST);
  color_print(TEST_OUTLINE,"blue");
  fprintf(stderr,"\n");

  // reset the variables
  memset(data_test,0,sizeof(data_test));
  count_test = 0;

  // delete a document from the collection
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,MESSAGE,0);
  if (delete_document_from_collection(database_name,DATABASE_COLLECTION_TEST,MESSAGE,0) == 0)
  {
    color_print("FAILED! Test for delete_document_from_collection","red");
  }
  else
  {
    color_print("PASSED! Test for ddelete_document_from_collection","green");
    count_test++;
  }

  // delete a document from the collection on a separate thread
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,MESSAGE,0);
  if (insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,MESSAGE,0) == 0)
  {
    color_print("FAILED! Test for delete_document_from_collection_thread","red");
  }
  else
  {
    struct delete_document_from_collection_thread_parameters delete_document_from_collection_thread_parameters = {database_name,DATABASE_COLLECTION_TEST,MESSAGE};
    pthread_create(&thread_id, NULL, &delete_document_from_collection_thread,(void *)&delete_document_from_collection_thread_parameters);
    if (thread_settings(thread_id) == 0)
    {
      color_print("FAILED! Test for delete_document_from_collection_thread","red");
    }
    else
    {
      color_print("PASSED! Test for delete_document_from_collection_thread","green");
      count_test++;
    }
  }  

  // delete a collection from the database
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,MESSAGE,0);
  if (delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST,0) == 0)
  {
    color_print("FAILED! Test for delete_collection_from_database","red");
  }
  else
  {
    color_print("PASSED! Test for delete_collection_from_database","green");
    count_test++;
  }

  // delete a collection from the database on a separate thread
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,MESSAGE,0);
  if (insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,MESSAGE,0) == 0)
  {
    color_print("FAILED! Test for delete_collection_from_database_thread","red");
  }
  else
  {
    struct delete_collection_from_database_thread_parameters delete_collection_from_database_thread_parameters = {database_name,DATABASE_COLLECTION_TEST};
    pthread_create(&thread_id, NULL, &delete_collection_from_database_thread,(void *)&delete_collection_from_database_thread_parameters);
    if (thread_settings(thread_id) == 0)
    {
      color_print("FAILED! Test for delete_collection_from_database_thread","red");
    }
    else
    {
      color_print("PASSED! Test for delete_collection_from_database_thread","green");
      count_test++;
    }  
  }
  
  // write the end test message
  if (count_test == DELETE_DATABASE_FUNCTIONS_TEST)
  {
    fprintf(stderr,"\n");
    color_print(TEST_OUTLINE,"green");
    fprintf(stderr,"\033[1;32mdelete database functions test - Passed test: %d, Failed test: 0\033[0m\n",DELETE_DATABASE_FUNCTIONS_TEST);
    color_print(TEST_OUTLINE,"green");
    fprintf(stderr,"\n\n");
  }
  else
  {
    fprintf(stderr,"\n");
    color_print(TEST_OUTLINE,"red");
    fprintf(stderr,"\033[1;31mdelete database functions test - Passed test: %d, Failed test: %d\033[0m\n",count_test,DELETE_DATABASE_FUNCTIONS_TEST-count_test);
    color_print(TEST_OUTLINE,"red");
    fprintf(stderr,"\n\n");
  } 
  return count_test;
  
  #undef DELETE_DATABASE_FUNCTIONS_TEST
  #undef MESSAGE
}