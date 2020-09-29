#include <stdio.h>
#include <stdlib.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "count_database_functions.h"
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
Name: count_documents_in_collection
Description: Counts the documents in the collection that match a specific field name and field
Parameters:
  DATABASE - The database name
  COLLECTION - The collection name
  DATA - The json data to use to search the collection for
Return: -1 if an error has occured, otherwise the amount of documents that match a specific field name and field in the collection
-----------------------------------------------------------------------------------------------------------
*/

int count_documents_in_collection(const char* DATABASE, const char* COLLECTION, const char* DATA)
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
  mongoc_client_pool_push(database_client_thread_pool, database_client_thread);

  #define COUNT_DOCUMENTS_IN_COLLECTION_ERROR(settings) \
  if ((strncmp(settings,"The database collection does not exist",BUFFER_SIZE) == 0 && test_settings == 1) || (strncmp(settings,"The database collection does not exist",BUFFER_SIZE) != 0)) \
  { \
    memcpy(error_message.function[error_message.total],"count_documents_in_collection",29); \
    memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
    error_message.total++; \
  } \
  database_reset_all; \
  return -1;

  // get a temporary connection
  if (!(database_client_thread = mongoc_client_pool_pop(database_client_thread_pool)))
  {
    return -1;
  }

  // set the collection
  collection = mongoc_client_get_collection(database_client_thread, DATABASE, COLLECTION);

  // check if the database collection exist
  if (check_if_database_collection_exist(DATABASE,COLLECTION) == 0)
  {
    COUNT_DOCUMENTS_IN_COLLECTION_ERROR("The database collection does not exist");
  }

  if (!(document = bson_new_from_json((const uint8_t *)DATA, -1, &error)))
  {
    COUNT_DOCUMENTS_IN_COLLECTION_ERROR("Could not convert the data into a database document");
  }
  
  const int COUNT = (int)mongoc_collection_count_documents(collection, document, NULL, NULL, NULL, &error);
  database_reset_all;
  return COUNT;

  #undef database_reset_all
  #undef COUNT_DOCUMENTS_IN_COLLECTION_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: count_all_documents_in_collection
Description: Counts all the documents in the collection
Parameters:
  DATABASE - The database name
  COLLECTION - The collection name
Return: -1 if an error has occured, otherwise the amount of documents in the collection
-----------------------------------------------------------------------------------------------------------
*/

int count_all_documents_in_collection(const char* DATABASE, const char* COLLECTION)
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
  mongoc_client_pool_push(database_client_thread_pool, database_client_thread);

  #define COUNT_ALL_DOCUMENTS_IN_COLLECTION_ERROR(settings) \
  if ((strncmp(settings,"The database collection does not exist",BUFFER_SIZE) == 0 && test_settings == 1) || (strncmp(settings,"The database collection does not exist",BUFFER_SIZE) != 0)) \
  { \
    memcpy(error_message.function[error_message.total],"count_all_documents_in_collection",33); \
    memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
    error_message.total++; \
  } \
  database_reset_all; \
  return -1;

  // get a temporary connection
  if (!(database_client_thread = mongoc_client_pool_pop(database_client_thread_pool)))
  {
    return -1;
  }

  // set the collection
  collection = mongoc_client_get_collection(database_client_thread, DATABASE, COLLECTION);

  // check if the database collection exist
  if (check_if_database_collection_exist(DATABASE,COLLECTION) == 0)
  {
    COUNT_ALL_DOCUMENTS_IN_COLLECTION_ERROR("The database collection does not exist");
  }

  if (!(document = bson_new()))
  {
    COUNT_ALL_DOCUMENTS_IN_COLLECTION_ERROR("Could not convert the data into a database document");
  }
  
  const int COUNT = (int)mongoc_collection_count_documents(collection, document, NULL, NULL, NULL, &error);
  database_reset_all;
  return COUNT;

  #undef database_reset_all
  #undef COUNT_ALL_DOCUMENTS_IN_COLLECTION_ERROR
}