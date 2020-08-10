#include <stdio.h>
#include <stdlib.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"
#include "define_macros_test.h"

#include "database_functions.h"
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
Name: check_if_database_collection_exist
Description: Checks if a database collection exist
Parameters:
  DATABASE - The database name
  COLLECTION - The collection name
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int check_if_database_collection_exist(const char* DATABASE, const char* COLLECTION)
{
   // Variables
  mongoc_client_t* database_client_thread = NULL;
  mongoc_database_t* database;
  bson_error_t error;

  // define macros
  #define database_reset_all \
  mongoc_database_destroy(database); \
  mongoc_client_pool_push(database_client_thread_pool, database_client_thread);

  // get a temporary connection
  if (!(database_client_thread = mongoc_client_pool_pop(database_client_thread_pool)))
  {
    return 0;
  }

  // set the datbase
  database = mongoc_client_get_database(database_client_thread, DATABASE);
   
  if (!mongoc_database_has_collection(database,COLLECTION,&error))
  {    
    database_reset_all;
    return 0;
  }
  database_reset_all;
  return 1;

  #undef database_reset_all
  #undef CHECK_IF_DATABASE_COLLECTION_EXIST_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_database_data
Description: Gets the database data
Parameters:
  database_data - The database data
  DATABASE - The database name
  COLLECTION - The collection name
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int get_database_data(char *database_data, const char* DATABASE, const char* COLLECTION)
{
  // Constants
  const bson_t* current_document;

  // Variables
  mongoc_client_t* database_client_thread = NULL;
  mongoc_collection_t* collection = NULL;
  mongoc_cursor_t* document_settings = NULL;
  bson_t* document = NULL;  
  bson_t* document_options = NULL;
  char* message;
  int count = 0;

  // define macros
  #define database_reset_all \
  bson_destroy(document); \
  bson_destroy(document_options); \
  mongoc_cursor_destroy(document_settings); \
  mongoc_collection_destroy(collection); \
  mongoc_client_pool_push(database_client_thread_pool, database_client_thread);
  
  #define GET_DATABASE_DATA_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"get_database_data",17); \
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

  if (!(document = bson_new()))
  {
    GET_DATABASE_DATA_ERROR("Could not convert the data into a database document");
  }

  // sort the documents
  document_options = strstr(COLLECTION,"reserve_proofs") != NULL ? BCON_NEW("sort", "{", "total", BCON_INT32(-1), "}") : strstr(COLLECTION,"reserve_bytes") != NULL ? BCON_NEW("sort", "{", "block_height", BCON_INT32(1), "}") : strstr(COLLECTION,"delegates") != NULL ? BCON_NEW("sort", "{", "total_vote_count", BCON_INT32(-1), "}") : NULL;

  memset(database_data,0,strlen(database_data));

  document_settings = mongoc_collection_find_with_opts(collection, document, document_options, NULL);
  while (mongoc_cursor_next(document_settings, &current_document))
  { 
    // get the current document  
    message = bson_as_canonical_extended_json(current_document, NULL);
    strnlen(database_data,MAXIMUM_BUFFER_SIZE) == 0 ? memcpy(database_data+strnlen(database_data,MAXIMUM_BUFFER_SIZE),"{",1) : memcpy(database_data+strnlen(database_data,MAXIMUM_BUFFER_SIZE),",{",2);
    memcpy(database_data+strnlen(database_data,MAXIMUM_BUFFER_SIZE),&message[142],strnlen(message,BUFFER_SIZE) - 142);    
    bson_free(message);
    count = 1;
  }

  if (count != 1)
  {
    if (test_settings == 1)
    {
      GET_DATABASE_DATA_ERROR("Could not get the database data");
    }
    database_reset_all;
    return 0;
  }
  
  database_reset_all;
  return 1;

  #undef database_reset_all
  #undef GET_DATABASE_DATA_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_database_data_hash
Description: Gets a database data hash
Parameters:
  data_hash - The data hash
  DATABASE - The database name
  COLLECTION - The collection name. If reserve_proofs or reserve_bytes without a number it will get a database hash of all of the reserve_proofs or reserve_bytes
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int get_database_data_hash(char *data_hash, const char* DATABASE, const char* COLLECTION)
{
  // Variables
  char data[BUFFER_SIZE];
  char* message;
  char* message2;
  size_t count;
  size_t count2;
  mongoc_client_t* database_client_thread = NULL;
  mongoc_collection_t* collection;
  bson_error_t error;
  bson_t* command;
  bson_t document;

  // define macros
  #define database_reset_all \
  bson_destroy(command); \
  bson_destroy(&document); \
  mongoc_collection_destroy(collection); \
  mongoc_client_pool_push(database_client_thread_pool, database_client_thread);

  memset(data,0,sizeof(data));

  // get a temporary connection
  if (!(database_client_thread = mongoc_client_pool_pop(database_client_thread_pool)))
  {
    return 0;
  }

  // set the collection
  collection = mongoc_client_get_collection(database_client_thread, DATABASE, COLLECTION);

  // get all of the database collections

  if (strncmp(COLLECTION,"reserve_bytes",BUFFER_SIZE) == 0)
  {
    // get the current reserve bytes database
    get_reserve_bytes_database(count2,0);

    for (count = 1; count <= count2; count++)
    {
      memcpy(data+strlen(data),"reserve_bytes_",14);
      snprintf(data+strlen(data),sizeof(data)-15,"%zu",count);
      if (count != count2)
      {
        memcpy(data+strlen(data),",",sizeof(char));
      }
    }
  }
  else if (strncmp(COLLECTION,"reserve_proofs",BUFFER_SIZE) == 0)
  {
    for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
    {
      memcpy(data+strlen(data),"reserve_proofs_",15);
      snprintf(data+strlen(data),sizeof(data)-16,"%zu",count);
      if (count != TOTAL_RESERVE_PROOFS_DATABASES)
      {
        memcpy(data+strlen(data),",",sizeof(char));
      }
    }
  }
  else
  {
    memcpy(data+strlen(data),COLLECTION,strnlen(COLLECTION,sizeof(data)));
  }
  
  command = strncmp(COLLECTION,"ALL",3) == 0 ? BCON_NEW ("dbHash", BCON_INT32 (1)) : BCON_NEW ("dbHash",BCON_INT32 (1),"collections","[",data,"]");
  memset(data,0,sizeof(data));

  if (!command)
  {
    database_reset_all;
    return 0;
  }

  if (!mongoc_collection_command_simple(collection, command, NULL, &document, &error))
  {
    database_reset_all;
    return 0;
  }

  if ((message = bson_as_json(&document, NULL)) == NULL)
  {
    database_reset_all;
    return 0;
  }
  
  memcpy(data,message,strnlen(message,sizeof(data)));
  bson_free(message);

  message2 = strstr(data,"\"md5\"");

  memset(data_hash,0,strlen(data_hash));
  memcpy(data_hash,"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",96);
  memcpy(data_hash+96,&message2[9],32);

  database_reset_all;  
  return 1;

  #undef database_reset_all
}