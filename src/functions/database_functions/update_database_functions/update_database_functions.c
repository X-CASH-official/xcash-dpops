#include <stdio.h>
#include <stdlib.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "update_database_functions.h"
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
Name: update_document_from_collection
Description: Updates a document from the collection
Parameters:
  DATABASE - The database name
  COLLECTION - The collection name
  DATA - The json data to use to search the collection for
  FIELD_NAME_AND_DATA - The json data to use to update the document
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int update_document_from_collection(const char* DATABASE, const char* COLLECTION, const char* DATA, const char* FIELD_NAME_AND_DATA)
{
  // check if the data is over the MAXIMUM_DATABASE_WRITE_SIZE
  if (strlen(FIELD_NAME_AND_DATA) > MAXIMUM_DATABASE_WRITE_SIZE)
  {
    return 0;
  }

  // Variables
  char data2[BUFFER_SIZE];
  mongoc_client_t* database_client_thread = NULL;
  mongoc_collection_t* collection;
  bson_error_t error;
  bson_t* update = NULL;
  bson_t* update_settings = NULL;

  // define macros
  #define database_reset_all \
  bson_destroy(update); \
  bson_destroy(update_settings); \
  mongoc_collection_destroy(collection); \
  mongoc_client_pool_push(database_client_thread_pool, database_client_thread);

  #define UPDATE_DOCUMENT_FROM_COLLECTION_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"update_document_from_collection",31); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  database_reset_all; \
  return 0;

  memset(data2,0,sizeof(data2));

  // get a temporary connection
  if (!(database_client_thread = mongoc_client_pool_pop(database_client_thread_pool)))
  {
    return 0;
  }

  // set the collection
  collection = mongoc_client_get_collection(database_client_thread, DATABASE, COLLECTION);

  // check if the database collection exist
  if (check_if_database_collection_exist(DATABASE,COLLECTION) == 0)
  {
    UPDATE_DOCUMENT_FROM_COLLECTION_ERROR("The database collection does not exist");
  }
  
  if (!(update = bson_new_from_json((const uint8_t *)DATA, -1, &error)))
  {
    UPDATE_DOCUMENT_FROM_COLLECTION_ERROR("Could not convert the data into a database document");
  }
 
  const size_t FIELD_NAME_AND_DATA_LENGTH = strnlen(FIELD_NAME_AND_DATA,BUFFER_SIZE)-1;
  memcpy(data2,"{\"$set\":",8);
  memcpy(data2+8,FIELD_NAME_AND_DATA,FIELD_NAME_AND_DATA_LENGTH);
  memcpy(data2+8+FIELD_NAME_AND_DATA_LENGTH,"}}",2);

  if (!(update_settings = bson_new_from_json((const uint8_t *)data2, -1, &error)))
  {
    UPDATE_DOCUMENT_FROM_COLLECTION_ERROR("Could not convert the data into a database document");
  }
  
  if (!mongoc_collection_update_one(collection, update, update_settings, NULL, NULL, &error))
  {
    UPDATE_DOCUMENT_FROM_COLLECTION_ERROR("Could not update the document in the database collection");
  }
  database_reset_all;
  return 1;

  #undef database_reset_all
  #undef UPDATE_DOCUMENT_FROM_COLLECTION_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: update_all_documents_from_collection
Description: Updates all documents in a collection
Parameters:
  DATABASE - The database name
  COLLECTION - The collection name
  DATA - The json data to use to update the documents
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int update_all_documents_from_collection(const char* DATABASE, const char* COLLECTION, const char* DATA)
{
  // check if the data is over the MAXIMUM_DATABASE_WRITE_SIZE
  if (strlen(DATA) > MAXIMUM_DATABASE_WRITE_SIZE)
  {
    return 0;
  }

  // Variables
  char data2[BUFFER_SIZE];
  mongoc_client_t* database_client_thread = NULL;
  mongoc_collection_t* collection;
  bson_error_t error;
  bson_t* update = NULL;
  bson_t* update_settings = NULL;

  // define macros
  #define database_reset_all \
  bson_destroy(update); \
  bson_destroy(update_settings); \
  mongoc_collection_destroy(collection); \
  mongoc_client_pool_push(database_client_thread_pool, database_client_thread);
  
  #define UPDATE_ALL_DOCUMENTS_FROM_COLLECTION_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"update_all_documents_from_collection",36); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  database_reset_all; \
  return 0;

  memset(data2,0,sizeof(data2));

  // get a temporary connection
  if (!(database_client_thread = mongoc_client_pool_pop(database_client_thread_pool)))
  {
    return 0;
  }

  // set the collection
  collection = mongoc_client_get_collection(database_client_thread, DATABASE, COLLECTION);

  // check if the database collection exist
  if (check_if_database_collection_exist(DATABASE,COLLECTION) == 0)
  {
    UPDATE_ALL_DOCUMENTS_FROM_COLLECTION_ERROR("The database collection does not exist");
  }
  
  // set the document to empty so it will get each document in the collection  
  if (!(update = bson_new()))
  {
    UPDATE_ALL_DOCUMENTS_FROM_COLLECTION_ERROR("Could not convert the data into a database document");
  }
 
  const size_t DATA_LENGTH = strnlen(DATA,BUFFER_SIZE)-1;
  memcpy(data2,"{\"$set\":",8);
  memcpy(data2+8,DATA,DATA_LENGTH);
  memcpy(data2+8+DATA_LENGTH,"}}",2);

  if (!(update_settings = bson_new_from_json((const uint8_t *)data2, -1, &error)))
  {
    UPDATE_ALL_DOCUMENTS_FROM_COLLECTION_ERROR("Could not convert the data into a database document");
  }
  
  if (!mongoc_collection_update_many(collection, update, update_settings, NULL, NULL, &error))
  {
    UPDATE_ALL_DOCUMENTS_FROM_COLLECTION_ERROR("Could not update the document in the database collection");
  }
  database_reset_all;
  return 1;

  #undef database_reset_all
  #undef UPDATE_ALL_DOCUMENTS_FROM_COLLECTION_ERROR
}