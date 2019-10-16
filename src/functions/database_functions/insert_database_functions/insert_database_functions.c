#include <stdio.h>
#include <stdlib.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "insert_database_functions.h"
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
Name: insert_document_into_collection_array
Description: Inserts a document into the collection in the database from an array
Parameters:
  DATABASE - The database name
  COLLECTION - The collection name
  field_name_array - The field name to insert into the document
  field_data_array - The field data to insert into the document
  DATA_COUNT - The size of the array
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int insert_document_into_collection_array(const char* DATABASE, const char* COLLECTION, char** field_name_array, char** field_data_array, const size_t DATA_COUNT)
{
  // Variables
  mongoc_collection_t* collection;
  bson_error_t error;
  bson_oid_t oid;
  bson_t* document;
  size_t count;

  // define macros
  #define database_reset_all \
  bson_destroy(document); \
  mongoc_collection_destroy(collection);

  // set the collection
  collection = mongoc_client_get_collection(database_client, DATABASE, COLLECTION);  

  document = bson_new();
  bson_oid_init(&oid, NULL);
  BSON_APPEND_OID(document, "_id", &oid);
  for (count = 0; count < DATA_COUNT; count++)
  {
    BSON_APPEND_UTF8(document, field_name_array[count], field_data_array[count]);
  }

  if (!mongoc_collection_insert_one(collection, document, NULL, NULL, &error))
  {
    database_reset_all;
    return 0;
  } 
  database_reset_all;
  return 1;

  #undef database_reset_all
}



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
  mongoc_client_t* database_client_thread;
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
    return 0;
  }
    
  if (!mongoc_collection_insert_one(collection, document, NULL, NULL, &error))
  {
    database_reset_all;
    return 0;
  }
  database_reset_all;
  return 1;

  #undef database_reset_all
}



/*
-----------------------------------------------------------------------------------------------------------
Name: insert_multiple_documents_into_collection_json
Description: Inserts a document into the collection in the database from json data
Parameters:
  DATABASE - The database name
  COLLECTION - The collection name
  DATA - The json data to insert into the collection
  THREAD_SETTINGS - 1 to use a separate thread, otherwise 0
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int insert_multiple_documents_into_collection_json(const char* DATABASE, const char* COLLECTION, const char* DATA, const size_t DATA_TOTAL_LENGTH, const int THREAD_SETTINGS)
{
  // Variables
  char buffer[1024];
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  // since were going to be changing where data2 is referencing, we need to create a copy to pointer_reset
  char* datacopy = data2; 
  mongoc_client_t* database_client_thread = NULL;
  mongoc_collection_t* collection;
  bson_error_t error;
  bson_t* document = NULL;
  size_t count;
  size_t count2;

  // define macros
  #define pointer_reset_all \
  free(datacopy); \
  datacopy = NULL; \
  free(data3); \
  data3 = NULL; 

  #define database_reset_all \
  bson_destroy(document); \
  mongoc_collection_destroy(collection); \
  if (THREAD_SETTINGS == 1) \
  { \
    mongoc_client_pool_push(database_client_thread_pool, database_client_thread); \
  }

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
      pointer_reset_all;
      return 0;
    }
    // set the collection
    collection = mongoc_client_get_collection(database_client_thread, DATABASE, COLLECTION);
  }

  // create a copy of the data since were going to be changing where the data is referencing
  append_string(data2,DATA,DATA_TOTAL_LENGTH);

  // count how many documents are in the data
  count = string_count(DATA,"},{") + 1;

  for (count2 = 0; count2 < count; count2++)
  {
    // get the document
    if ((count2+1) != count)
    {
      memset(data3,0,strlen(data3));
      memcpy(data3,data2,strnlen(data2,BUFFER_SIZE) - strnlen(strstr(data2,"},{"),BUFFER_SIZE)+1);
      data2 = strstr(data2,"},{") + 2;     
    }
    else
    {
      memset(data3,0,strlen(data3));
      memcpy(data3,data2,strnlen(data2,BUFFER_SIZE));
    }

    document = bson_new_from_json((const uint8_t *)data3, -1, &error);
    if (!document)
    {
      pointer_reset_all;
      database_reset_all;
      return 0;
    }

    if (!mongoc_collection_insert_one(collection, document, NULL, NULL, &error))
    {
      pointer_reset_all;
      database_reset_all;
      return 0;
    }
  }
  
  pointer_reset_all;
  database_reset_all;
  return 1;

  #undef pointer_reset_all
  #undef database_reset_all
}