#include <stdio.h>
#include <stdlib.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "read_database_functions.h"
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
Name: read_document_from_collection
Description: Reads a document from the collection
Parameters:
  DATABASE - The database name
  COLLECTION - The collection name
  DATA - The json data to use to search the collection for
  result - The document read from the collection
  THREAD_SETTINGS - 1 to use a separate thread, otherwise 0
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int read_document_from_collection(const char* DATABASE, const char* COLLECTION, const char* DATA, char *result, const int THREAD_SETTINGS)
{
  // Constants
  const bson_t* current_document;

  // Variables
  mongoc_client_t* database_client_thread = NULL;
  mongoc_collection_t* collection;
  mongoc_cursor_t* document_settings = NULL;
  bson_error_t error;
  bson_t* document = NULL;  
  char* message;
  int count = 0;

  // define macros
  #define database_reset_all \
  bson_destroy(document); \
  mongoc_cursor_destroy(document_settings); \
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
 
  document_settings = mongoc_collection_find_with_opts(collection, document, NULL, NULL);
  while (mongoc_cursor_next(document_settings, &current_document))
  {
    message = bson_as_canonical_extended_json(current_document, NULL);
    memcpy(result,message,strnlen(message,BUFFER_SIZE));
    bson_free(message);
    count = 1;
  }

  if (count != 1)
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
Name: read_document_field_from_collection
Description: Reads a field from a document from the collection
Parameters:
  DATABASE - The database name
  COLLECTION - The collection name
  DATA - The json data to use to search the collection for
  FIELD_NAME - The field of the document data to read
  result - The document data read from the collection
  THREAD_SETTINGS - 1 to use a separate thread, otherwise 0
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int read_document_field_from_collection(const char* DATABASE, const char* COLLECTION, const char* DATA, const char* FIELD_NAME, char *result, const int THREAD_SETTINGS)
{
  // Constants
  const bson_t* current_document;

  // Variables
  char* message;
  char buffer[1024];
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  char* settings = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message_copy1;
  char* message_copy2;
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  mongoc_client_t* database_client_thread = NULL;
  mongoc_collection_t* collection;
  mongoc_cursor_t* document_settings = NULL;
  bson_error_t error;
  bson_t* document = NULL; 
  int count = 0;

  // define macros
  #define pointer_reset_all \
  free(data2); \
  data2 = NULL; \
  free(settings); \
  settings = NULL; 

  #define database_reset_all \
  bson_destroy(document); \
  mongoc_cursor_destroy(document_settings); \
  mongoc_collection_destroy(collection); \
  if (THREAD_SETTINGS == 1) \
  { \
    mongoc_client_pool_push(database_client_thread_pool, database_client_thread); \
  }

  // check if the memory needed was allocated on the heap successfully
  if (data2 == NULL || settings == NULL)
  {
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    if (settings != NULL)
    {
      pointer_reset(settings);
    }
    memcpy(error_message.function[error_message.total],"read_document_field_from_collection",35);
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
  
  document = bson_new_from_json((const uint8_t *)DATA, -1, &error);
  if (!document)
  {    
    pointer_reset_all;
    database_reset_all;
    return 0;
  }

  document_settings = mongoc_collection_find_with_opts(collection, document, NULL, NULL);
  while (mongoc_cursor_next(document_settings, &current_document))
  {
    message = bson_as_canonical_extended_json(current_document, NULL);
    memcpy(data2,message,strnlen(message,BUFFER_SIZE));
    bson_free(message);
    count = 1;
  }

  if (count == 1)
  {
    // parse the json data
    const size_t FIELD_NAME_LENGTH = strnlen(FIELD_NAME,BUFFER_SIZE);
    memcpy(settings,", \"",3);
    memcpy(settings+3,FIELD_NAME,FIELD_NAME_LENGTH);
    memcpy(settings+3+FIELD_NAME_LENGTH,"\" : \"",5);

    message_copy1 = strstr(data2,settings) + strnlen(settings,BUFFER_SIZE);
    message_copy2 = strstr(message_copy1,"\"");
    memset(result,0,strlen(result));
    memcpy(result,message_copy1,message_copy2 - message_copy1);
  }
  else
  {
    pointer_reset_all; 
    database_reset_all; 
    return 0;
  }

  pointer_reset_all; 
  database_reset_all; 
  return 1;

  #undef pointer_reset_all
  #undef database_reset_all
}



/*
-----------------------------------------------------------------------------------------------------------
Name: database_document_parse_json_data_
Description: Parses the json data from the database
Parameters:
  DATA - The json data from the database
  result - A database_document_fields struct
  struct database_document_fields
    count - The number of items in the database document
    item[100] - The database document items
    value[100] - The database document values
  database_fields - The database fields to not include in the database array
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int database_document_parse_json_data(const char* DATA, struct database_document_fields *result)
{
  // Variables
  char* data2;
  char* data3;
  size_t count = 0;

  // get the first item  
  data2 = strstr(DATA,",") + 3;
  data3 = strstr(data2,"\"");
  memcpy(result->item[0],data2,strnlen(data2,BUFFER_SIZE)-strnlen(data3,BUFFER_SIZE)); 
  
  for (count = 0; count < result->count; count++)
  {
    data2 = data3+5;
    data3 = strstr(data2,"\"");
    memcpy(result->value[count],data2,strnlen(data2,BUFFER_SIZE)-strnlen(data3,BUFFER_SIZE));
      
    // only get the item if its not the last count
    if (count+1 != result->count)
    { 
      data2 = data3+4;
      data3 = strstr(data2,"\"");
      memcpy(result->item[count+1],data2,strnlen(data2,BUFFER_SIZE)-strnlen(data3,BUFFER_SIZE));
    }    
  } 
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: database_multiple_documents_parse_json_data
Description: Parses the json data from the database
Parameters:
  DATA - The json data from the database
  result - A database_document_fields struct
  struct database_multiple_documents_fields
    document_count - The number of documents
    database_fields_count - The number of items in the database document
    item[100][100] - The database document items
    value[100][100] - The database document values
  document_count - The count of the document
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int database_multiple_documents_parse_json_data(const char* DATA, struct database_multiple_documents_fields *result, const int document_count)
{
  // Variables
  char* data2;
  char* data3;
  size_t count = 0;

  // get the first item  
  data2 = strstr(DATA,",") + 3;
  data3 = strstr(data2,"\"");
  memcpy(result->item[document_count][0],data2,strnlen(data2,BUFFER_SIZE)-strnlen(data3,BUFFER_SIZE)); 
  
  for (count = 0; count < result->database_fields_count; count++)
  {
    data2 = data3+5;
    data3 = strstr(data2,"\"");
    memcpy(result->value[document_count][count],data2,strnlen(data2,BUFFER_SIZE)-strnlen(data3,BUFFER_SIZE));
      
    // only get the item if its not the last count
    if (count+1 != result->database_fields_count)
    { 
      data2 = data3+4;
      data3 = strstr(data2,"\"");
      memcpy(result->item[document_count][count+1],data2,strnlen(data2,BUFFER_SIZE)-strnlen(data3,BUFFER_SIZE));
    }    
  } 
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: read_document_all_fields_from_collection
Description: Reads all fields from a document from the collection
Parameters:
  DATABASE - The database name
  COLLECTION - The collection name 
  DATA - The json data to use to search the collection for 
  result - A database_fields struct to hold the data
  struct database_document_fields
    count - The number of items in the database document
    item[100] - The database document items
    value[100] - The database document values
  THREAD_SETTINGS - 1 to use a separate thread, otherwise 0
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int read_document_all_fields_from_collection(const char* DATABASE, const char* COLLECTION, const char* DATA, struct database_document_fields *result, const int THREAD_SETTINGS)
{
  // Constants
  const bson_t* current_document;

  // Variables
  char* message;
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char buffer[1024];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  mongoc_client_t* database_client_thread = NULL;
  mongoc_collection_t* collection;
  mongoc_cursor_t* document_settings = NULL;
  bson_error_t error;
  bson_t* document = NULL; 
  int count = 0;

  // define macros
  #define database_reset_all \
  bson_destroy(document); \
  mongoc_cursor_destroy(document_settings); \
  mongoc_collection_destroy(collection); \
  if (THREAD_SETTINGS == 1) \
  { \
    mongoc_client_pool_push(database_client_thread_pool, database_client_thread); \
  }

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    memcpy(error_message.function[error_message.total],"read_document_all_fields_from_collection",40);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,buffer);  
    exit(0);
  } 

  // set the database_fields_count
  if (strstr(COLLECTION,"reserve_bytes") != NULL)
  {
    result->count = TOTAL_RESERVE_BYTES_DATABASE_FIELDS;
  }
  else if (strstr(COLLECTION,"reserve_proofs") != NULL)
  {
    result->count = TOTAL_RESERVE_PROOFS_DATABASE_FIELDS;
  }
  else if (strstr(COLLECTION,"delegates") != NULL)
  {
    result->count = TOTAL_DELEGATES_DATABASE_FIELDS;
  }
  else if (strstr(COLLECTION,"statistics") != NULL)
  {
    result->count = TOTAL_STATISTICS_DATABASE_FIELDS;
  }
  else if (strstr(COLLECTION,"public_addresses") != NULL)
  {
    result->count = TOTAL_PUBLIC_ADDRESSES_DATABASE_FIELDS;
  }
  else if (strstr(COLLECTION,"blocks_found") != NULL)
  {
    result->count = TOTAL_BLOCKS_FOUND_DATABASE_FIELDS;
  }
  else if (strstr(COLLECTION,"XCASH_PROOF_OF_STAKE_TEST_DATA") != NULL)
  {
    result->count = 7;
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
      pointer_reset(data);
      return 0;
    }
    // set the collection
    collection = mongoc_client_get_collection(database_client_thread, DATABASE, COLLECTION);
  }
  
  document = bson_new_from_json((const uint8_t *)DATA, -1, &error);
  if (!document)
  {    
    pointer_reset(data);
    database_reset_all;
    return 0;
  }
 
  document_settings = mongoc_collection_find_with_opts(collection, document, NULL, NULL);
  while (mongoc_cursor_next(document_settings, &current_document))
  {
    message = bson_as_canonical_extended_json(current_document, NULL);
    memcpy(data,message,strnlen(message,BUFFER_SIZE));
    bson_free(message);
    
    string_replace(data,BUFFER_SIZE," }, ",", ");

    count = 1;
  }

  if (count == 1)
  {
    // parse the json data
    database_document_parse_json_data(data,result);
  }  
  else
  {
    pointer_reset(data);
    database_reset_all;
    return 0;
  }
  

  pointer_reset(data);
  database_reset_all;
  return 1;

  #undef database_reset_all
}



/*
-----------------------------------------------------------------------------------------------------------
Name: read_multiple_documents_all_fields_from_collection
Description: Reads all fields from a document from the collection
Parameters:
  DATABASE - The database name
  COLLECTION - The collection name 
  DATA - The json data to use to search the collection for 
  result - A database_multiple_documents_fields struct to hold the data
  struct database_multiple_documents_fields
    document_count - The number of documents
    database_fields_count - The number of items in the database document
    item[100][100] - The database document items
    value[100][100] - The database document values
  DOCUMENT_COUNT_START - The document to start at when reading the data
  DOCUMENT_COUNT_TOTAL - The total amount of documents to read
  DOCUMENT_OPTIONS - 1 to use the sort document option, 0 to not use the document option
  DOCUMENT_OPTIONS_DATA - The item to sort the documents in the collection
  THREAD_SETTINGS - 1 to use a separate thread, otherwise 0
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int read_multiple_documents_all_fields_from_collection(const char* DATABASE, const char* COLLECTION, const char* DATA, struct database_multiple_documents_fields *result, const size_t DOCUMENT_COUNT_START, const size_t DOCUMENT_COUNT_TOTAL, const int DOCUMENT_OPTIONS, const char* DOCUMENT_OPTIONS_DATA, const int THREAD_SETTINGS)
{
  // Constants
  const bson_t* current_document;

  // Variables
  char* message;
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char buffer[1024];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  mongoc_client_t* database_client_thread = NULL;
  mongoc_collection_t* collection;
  mongoc_cursor_t* document_settings = NULL;
  bson_error_t error;
  bson_t* document = NULL;  
  bson_t* document_options = NULL;
  size_t count = 1;
  size_t counter = 0;

  // define macros
  #define database_reset_all \
  bson_destroy(document); \
  bson_destroy(document_options); \
  mongoc_cursor_destroy(document_settings); \
  mongoc_collection_destroy(collection); \
  if (THREAD_SETTINGS == 1) \
  { \
    mongoc_client_pool_push(database_client_thread_pool, database_client_thread); \
  }

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    memcpy(error_message.function[error_message.total],"read_multiple_documents_all_fields_from_collection",50);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,buffer);  
    exit(0);
  }

  // set the database_fields_count
  if (strstr(COLLECTION,"reserve_bytes") != NULL)
  {
    result->database_fields_count = TOTAL_RESERVE_BYTES_DATABASE_FIELDS;
  }
  else if (strstr(COLLECTION,"reserve_proofs") != NULL)
  {
    result->database_fields_count = TOTAL_RESERVE_PROOFS_DATABASE_FIELDS;
  }
  else if (strstr(COLLECTION,"delegates") != NULL)
  {
    result->database_fields_count = TOTAL_DELEGATES_DATABASE_FIELDS;
  }
  else if (strstr(COLLECTION,"statistics") != NULL)
  {
    result->database_fields_count = TOTAL_STATISTICS_DATABASE_FIELDS;
  }
  else if (strstr(COLLECTION,"public_addresses") != NULL)
  {
    result->database_fields_count = TOTAL_PUBLIC_ADDRESSES_DATABASE_FIELDS;
  }
  else if (strstr(COLLECTION,"blocks_found") != NULL)
  {
    result->database_fields_count = TOTAL_BLOCKS_FOUND_DATABASE_FIELDS;
  }
  else if (strstr(COLLECTION,"XCASH_PROOF_OF_STAKE_TEST_DATA") != NULL)
  {
    result->database_fields_count = 7;
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
      pointer_reset(data);
      return 0;
    }
    // set the collection
    collection = mongoc_client_get_collection(database_client_thread, DATABASE, COLLECTION);
  }

  if (memcmp(DATA,"",1) == 0)
  {
    document = bson_new();
  }
  else
  {
    document = bson_new_from_json((const uint8_t *)DATA, -1, &error);
  }

  if (!document)
  {
    pointer_reset(data);
    database_reset_all;
    return 0;
  }

  if (DOCUMENT_OPTIONS == 1)
  {
    document_options = BCON_NEW("sort", "{", DOCUMENT_OPTIONS_DATA, BCON_INT32(-1), "}");
  }
 
  document_settings = mongoc_collection_find_with_opts(collection, document, document_options, NULL);
  while (mongoc_cursor_next(document_settings, &current_document))
  {    
    if (count >= DOCUMENT_COUNT_START)
    {
      message = bson_as_canonical_extended_json(current_document, NULL);
      memset(data,0,strnlen(data,BUFFER_SIZE));
      memcpy(data,message,strnlen(message,BUFFER_SIZE));
      bson_free(message); 
      string_replace(data,BUFFER_SIZE," }, ",", ");

      // parse the json data      
      database_multiple_documents_parse_json_data(data,result,counter);
      counter++;
      result->document_count++;
      
      // check if that is the total amount of documents to read
      if (counter == DOCUMENT_COUNT_TOTAL)
      {
        break;
      }     
    }
    count++;    
  }

  if (counter == 0)
  {
    pointer_reset(data);
    database_reset_all;
    return 0;
  }

  pointer_reset(data);
  database_reset_all;
  return 1;

  #undef database_reset_all
}