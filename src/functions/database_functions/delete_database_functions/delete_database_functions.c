#include <stdio.h>
#include <stdlib.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "delete_database_functions.h"
#include "database_functions.h"
#include "network_functions.h"
#include "string_functions.h"
#include "vrf.h"
#include "crypto_vrf.h"
#include "VRF_functions.h"
#include "sha512EL.h"
#include "cached_hashes.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: delete_document_from_collection
Description: Deletes a document from the collection
Parameters:
  DATABASE - The database name
  COLLECTION - The collection name
  DATA - The json data to use to delete the document
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int delete_document_from_collection(const char* DATABASE, const char* COLLECTION, const char* DATA)
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
  
  #define DELETE_DOCUMENT_FROM_COLLECTION_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"delete_document_from_collection",31); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  database_reset_all; \
  return 0;

  // get a temporary connection
  if (!(database_client_thread = mongoc_client_pool_pop(database_client_thread_pool)))
  {
    return 0;
  }

  // we need to rehash this db next time
  del_hash(database_client_thread, COLLECTION);

  // set the collection
  collection = mongoc_client_get_collection(database_client_thread, DATABASE, COLLECTION);

  if (!(document = bson_new_from_json((const uint8_t *)DATA, -1, &error)))
  {
    DELETE_DOCUMENT_FROM_COLLECTION_ERROR("Could not convert the data into a database document");
  }
  
  mongoc_collection_delete_one(collection, document, NULL, NULL, &error);


  database_reset_all;
  return 1;

  #undef database_reset_all
  #undef DELETE_DOCUMENT_FROM_COLLECTION_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: delete_collection_from_database
Description: Deletes a collection from the database
Parameters:
  DATABASE - The database name
  COLLECTION - The collection name
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int delete_collection_from_database(const char* DATABASE, const char* COLLECTION)
{
   // Variables
  mongoc_client_t* database_client_thread = NULL;
  mongoc_collection_t* collection;
  bson_error_t error;

  // define macros
  #define database_reset_all \
  mongoc_collection_destroy(collection); \
  mongoc_client_pool_push(database_client_thread_pool, database_client_thread);
  
  #define DELETE_COLLECTION_FROM_DATABASE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"delete_collection_from_database",31); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  database_reset_all; \
  return 0;

  // get a temporary connection
  if (!(database_client_thread = mongoc_client_pool_pop(database_client_thread_pool)))
  {
    return 0;
  }

  // set the collection
  collection = mongoc_client_get_collection(database_client_thread, DATABASE, COLLECTION);
   
  if (!mongoc_collection_drop(collection, &error))
  {    
    DELETE_COLLECTION_FROM_DATABASE_ERROR("Could not delete the database collection from the database");
  }

  // we need to rehash this db next time
  del_hash(database_client_thread, COLLECTION);

  database_reset_all;
  return 1;

  #undef database_reset_all
  #undef DELETE_COLLECTION_FROM_DATABASE_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: delete_database
Description: Deletes a database
Parameters:
  DATABASE - The database name
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int delete_database(const char* DATABASE)
{
   // Variables
  mongoc_client_t* database_client_thread = NULL;
  mongoc_database_t* database;
  bson_error_t error;

  // define macros
  #define database_reset_all \
  mongoc_database_destroy(database); \
  mongoc_client_pool_push(database_client_thread_pool, database_client_thread);
  
  #define DELETE_DATABASE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"delete_database",15); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  database_reset_all; \
  return 0;

  // get a temporary connection
  if (!(database_client_thread = mongoc_client_pool_pop(database_client_thread_pool)))
  {
    return 0;
  }
  database = mongoc_client_get_database(database_client_thread, DATABASE);
   
  mongoc_database_drop(database, &error);

  // clear hashes db
  drop_all_hashes(database_client_thread);

  database_reset_all;
  return 1;

  #undef database_reset_all
  #undef DELETE_DATABASE_ERROR
}