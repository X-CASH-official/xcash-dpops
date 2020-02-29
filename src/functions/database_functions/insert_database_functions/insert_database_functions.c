#include <stdio.h>
#include <stdlib.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "insert_database_functions.h"
#include "database_functions.h"
#include "network_functions.h"
#include "string_functions.h"
#include "vrf.h"
#include "crypto_vrf.h"
#include "VRF_functions.h"
#include "sha512EL.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: insert_document_into_collection_json
Description: Inserts a document into the collection in the database from json data
Parameters:
  DATABASE - The database name
  COLLECTION - The collection name
  DATA - The json data to insert into the collection
  THREAD_SETTINGS - 1 to use a separate thread, otherwise 0
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int insert_document_into_collection_json(const char* DATABASE, const char* COLLECTION, const char* DATA, const int THREAD_SETTINGS)
{
  // Variables
  mongoc_client_t* database_client_thread = NULL;
  mongoc_collection_t* collection;
  bson_error_t error;
  bson_t* document = NULL;

  // define macros
  #define database_reset_all \
  bson_destroy(document); \
  mongoc_collection_destroy(collection); \
  if (THREAD_SETTINGS == 1) \
  { \
    mongoc_client_pool_push(database_client_thread_pool, database_client_thread); \
  }
  #define INSERT_DOCUMENT_INTO_COLLECTION_JSON_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"insert_document_into_collection_json",36); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  database_reset_all; \
  return 0;

  // check if we need to create a database connection, or use the global database connection
  if (THREAD_SETTINGS == 0)
  {
    // set the collection
    collection = mongoc_client_get_collection(database_client, DATABASE, COLLECTION);
  }
  else
  {
    if (!(database_client_thread = mongoc_client_pool_pop(database_client_thread_pool)))
    {
      return 0;
    }
    // set the collection
    collection = mongoc_client_get_collection(database_client_thread, DATABASE, COLLECTION);
  }

  if (!(document = bson_new_from_json((const uint8_t *)DATA, -1, &error)))
  {
    INSERT_DOCUMENT_INTO_COLLECTION_JSON_ERROR("Could not convert the data into a database document");
  }

  sync_database_threads;
    
  if (!mongoc_collection_insert_one(collection, document, NULL, NULL, &error))
  {
    INSERT_DOCUMENT_INTO_COLLECTION_JSON_ERROR("Could not insert the document into the database collection");
  }
  database_reset_all;
  return 1;

  #undef database_reset_all
  #undef INSERT_DOCUMENT_INTO_COLLECTION_JSON_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: insert_multiple_documents_into_collection_json
Description: Inserts a document into the collection in the database from json data
Parameters:
  DATABASE - The database name
  COLLECTION - The collection name
  DATA - The json data to insert into the collection
  DATA_TOTAL_LENGTH - The maximum length of DATA 
  THREAD_SETTINGS - 1 to use a separate thread, otherwise 0
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int insert_multiple_documents_into_collection_json(const char* DATABASE, const char* COLLECTION, const char* DATA, const size_t DATA_TOTAL_LENGTH, const int THREAD_SETTINGS)
{
  // Constants
  const size_t MAXIMUM_AMOUNT = DATA_TOTAL_LENGTH >= MAXIMUM_BUFFER_SIZE ? MAXIMUM_BUFFER_SIZE : DATA_TOTAL_LENGTH+SMALL_BUFFER_SIZE;

  // Variables
  char buffer[1024];
  char* data2 = (char*)calloc(MAXIMUM_AMOUNT,sizeof(char));
  char* data3 = (char*)calloc(MAXIMUM_AMOUNT,sizeof(char));
  // since were going to be changing where data2 is referencing, we need to create a copy to pointer_reset
  char* datacopy = data2; 
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  mongoc_client_t* database_client_thread = NULL;
  mongoc_collection_t* collection;
  bson_error_t error;
  size_t count;
  size_t count2;

  // define macros
  #define pointer_reset_all \
  free(datacopy); \
  datacopy = NULL; \
  free(data3); \
  data3 = NULL; 

  #define database_reset_all \
  mongoc_collection_destroy(collection); \
  if (THREAD_SETTINGS == 1) \
  { \
    mongoc_client_pool_push(database_client_thread_pool, database_client_thread); \
  }
  #define INSERT_MULTIPLE_DOCUMENTS_INTO_COLLECTION_JSON_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"insert_multiple_documents_into_collection_json",46); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  pointer_reset_all; \
  database_reset_all;

  if (data2 == NULL || data3 == NULL)
  {
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    if (data3 != NULL)
    {
      pointer_reset(data3);
    }
    memcpy(error_message.function[error_message.total],"insert_multiple_documents_into_collection_json",46);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,buffer);  
    exit(0);
  }

  memset(buffer,0,sizeof(buffer));

  // check if we need to create a database connection, or use the global database connection
  if (THREAD_SETTINGS == 0)
  {
    // set the collection
    collection = mongoc_client_get_collection(database_client, DATABASE, COLLECTION);
  }
  else
  {
    if (!(database_client_thread = mongoc_client_pool_pop(database_client_thread_pool)))
    {
      pointer_reset_all;
      return 0;
    }
    // set the collection
    collection = mongoc_client_get_collection(database_client_thread, DATABASE, COLLECTION);
  }

  // create a copy of the data since were going to be changing where the data is referencing
  append_string(data2,DATA,MAXIMUM_AMOUNT);

  // count how many documents are in the data
  count = string_count(DATA,"},{") + 1;

  for (count2 = 0; count2 < count; count2++)
  {
    // get the document
    if ((count2+1) != count)
    {
      memset(data3,0,strlen(data3));
      memcpy(data3,data2,strnlen(data2,MAXIMUM_AMOUNT) - strnlen(strstr(data2,"},{"),MAXIMUM_AMOUNT)+1);
      data2 = strstr(data2,"},{") + 2;     
    }
    else
    {
      memset(data3,0,strlen(data3));
      memcpy(data3,data2,strnlen(data2,MAXIMUM_AMOUNT));
    }

    bson_t* document = bson_new_from_json((const uint8_t *)data3, -1, &error);
    if (!document)
    {
      INSERT_MULTIPLE_DOCUMENTS_INTO_COLLECTION_JSON_ERROR("Could not convert the data into a database document");
      bson_destroy(document);
      return 0;
    }

    sync_database_threads;

    if (!mongoc_collection_insert_one(collection, document, NULL, NULL, &error))
    {
      INSERT_MULTIPLE_DOCUMENTS_INTO_COLLECTION_JSON_ERROR("Could not insert the document into the database collection");
      bson_destroy(document);
      return 0;
    }
    bson_destroy(document);
  }
  
  pointer_reset_all;
  database_reset_all;
  return 1;

  #undef pointer_reset_all
  #undef database_reset_all
  #undef INSERT_MULTIPLE_DOCUMENTS_INTO_COLLECTION_JSON_ERROR
}