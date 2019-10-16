#include <stdio.h>
#include <stdlib.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "count_database_functions.h"
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
  THREAD_SETTINGS - 1 to use a separate thread, otherwise 0
Return: -1 if an error has occured, otherwise the amount of documents that match a specific field name and field in the collection
-----------------------------------------------------------------------------------------------------------
*/

int count_documents_in_collection(const char* DATABASE, const char* COLLECTION, const char* DATA, const int THREAD_SETTINGS)
{
  // Variables
  mongoc_client_t* database_client_thread = NULL;
  mongoc_collection_t* collection;
  bson_error_t error;
  bson_t* document;

  // define macros
  #define database_reset_all \
  bson_destroy(document); \
  mongoc_collection_destroy(collection); \
  if (THREAD_SETTINGS == 1) \
  { \
    mongoc_client_pool_push(database_client_thread_pool, database_client_thread); \
  }

   // check if we need to create a database connection, or use the global database connection
  if (THREAD_SETTINGS == 0)
  {
    // set the collection
    collection = mongoc_client_get_collection(database_client, DATABASE, COLLECTION);
  }
  else
  {
    database_client_thread = mongoc_client_pool_pop(database_client_thread_pool);
    if (!database_client_thread)
    {
      return 0;
    }
    // set the collection
    collection = mongoc_client_get_collection(database_client_thread, DATABASE, COLLECTION);
  }

  document = bson_new_from_json((const uint8_t *)DATA, -1, &error);
  if (!document)
  {
    database_reset_all;
    return -1;
  }
  
  const int count = mongoc_collection_count_documents(collection, document, NULL, NULL, NULL, &error);
  if (count < 0)
  {
    database_reset_all;
    return -1;
  }
  database_reset_all;
  return count;

  #undef database_reset_all
}



/*
-----------------------------------------------------------------------------------------------------------
Name: count_all_documents_in_collection
Description: Counts all the documents in the collection
Parameters:
  DATABASE - The database name
  COLLECTION - The collection name
  THREAD_SETTINGS - 1 to use a separate thread, otherwise 0
Return: -1 if an error has occured, otherwise the amount of documents in the collection
-----------------------------------------------------------------------------------------------------------
*/

int count_all_documents_in_collection(const char* DATABASE, const char* COLLECTION, const int THREAD_SETTINGS)
{
  // Variables
  mongoc_client_t* database_client_thread = NULL;
  mongoc_collection_t* collection;
  bson_error_t error;
  bson_t* document;

  // define macros
  #define database_reset_all \
  bson_destroy(document); \
  mongoc_collection_destroy(collection); \
  if (THREAD_SETTINGS == 1) \
  { \
    mongoc_client_pool_push(database_client_thread_pool, database_client_thread); \
  }

  // check if we need to create a database connection, or use the global database connection
  if (THREAD_SETTINGS == 0)
  {
    // set the collection
    collection = mongoc_client_get_collection(database_client, DATABASE, COLLECTION);
  }
  else
  {
    database_client_thread = mongoc_client_pool_pop(database_client_thread_pool);
    if (!database_client_thread)
    {
      return -1;
    }
    // set the collection
    collection = mongoc_client_get_collection(database_client_thread, DATABASE, COLLECTION);
  }

  document = bson_new();
  if (!document)
  {
    database_reset_all;
    return -1;
  }
  
  const int count = mongoc_collection_count_documents(collection, document, NULL, NULL, NULL, &error);
  if (count < 0)
  {
    database_reset_all;
    return -1;
  }
  database_reset_all;
  return count;

  #undef database_reset_all
}