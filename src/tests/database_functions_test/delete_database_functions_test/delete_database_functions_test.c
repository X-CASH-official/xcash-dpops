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
  #define DELETE_DATABASE_FUNCTIONS_TEST 6
  #define MESSAGE "{\"message_settings\": \"XCASH_DPOPS_TEST_DATA\"}"

  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST,0);

  // write the start test message
  fprintf(stderr,"\033[1;34m%s\ndelete database functions test - Total test: %d\n%s\n\n\033[0m",TEST_OUTLINE,DELETE_DATABASE_FUNCTIONS_TEST,TEST_OUTLINE);

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
    color_print("PASSED! Test for delete_document_from_collection","green");
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

  // delete a database
  insert_document_into_collection_json(DATABASE_NAME_TEST,DATABASE_COLLECTION_TEST,MESSAGE,0);
  if (delete_database(DATABASE_NAME_TEST,0) == 0)
  {
    color_print("FAILED! Test for delete_database","red");
  }
  else
  {
    color_print("PASSED! Test for delete_database","green");
    count_test++;
  }

  // delete a database on a separate thread
  insert_document_into_collection_json(DATABASE_NAME_TEST,DATABASE_COLLECTION_TEST,MESSAGE,0);
  struct delete_database_thread_parameters delete_database_thread_parameters = {DATABASE_NAME_TEST};
  pthread_create(&thread_id, NULL, &delete_database_thread,(void *)&delete_database_thread_parameters);
  if (thread_settings(thread_id) == 0)
  {
    color_print("FAILED! Test for delete_database_thread","red");
  }
  else
  {
    color_print("PASSED! Test for delete_database_thread","green");
    count_test++;
  }
  
  // write the end test message
  fprintf(stderr,"\033[1;33m\n\n%s\nblock verifiers server functions test - Passed test: %d, Failed test: %d\n%s\n\n\n\033[0m",TEST_OUTLINE,count_test,DELETE_DATABASE_FUNCTIONS_TEST-count_test,TEST_OUTLINE);
  return count_test;
  
  #undef DELETE_DATABASE_FUNCTIONS_TEST
  #undef MESSAGE
}