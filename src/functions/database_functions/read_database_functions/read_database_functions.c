#include <stdio.h>
#include <stdlib.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "read_database_functions.h"
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
Name: read_document_from_collection
Description: Reads a document from the collection
Parameters:
  DATABASE - The database name
  COLLECTION - The collection name
  DATA - The json data to use to search the collection for
  result - The document read from the collection
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int read_document_from_collection(const char* DATABASE, const char* COLLECTION, const char* DATA, char *result)
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
  mongoc_client_pool_push(database_client_thread_pool, database_client_thread);
  
  #define READ_DOCUMENT_FROM_COLLECTION_ERROR(settings) \
  if ((strncmp(settings,"The database collection does not exist",BUFFER_SIZE) == 0 && test_settings == 1) || (strncmp(settings,"The database collection does not exist",BUFFER_SIZE) != 0)) \
  { \
    memcpy(error_message.function[error_message.total],"read_document_from_collection",29); \
    memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
    error_message.total++; \
  } \
  database_reset_all; \
  return 0;

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
    READ_DOCUMENT_FROM_COLLECTION_ERROR("The database collection does not exist");
  }
  
  if (!(document = bson_new_from_json((const uint8_t *)DATA, -1, &error)))
  {
    READ_DOCUMENT_FROM_COLLECTION_ERROR("Could not convert the data into a database document");
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
    READ_DOCUMENT_FROM_COLLECTION_ERROR("Could not read the document from the database collection");
  }

  database_reset_all;
  return 1;

  #undef database_reset_all
  #undef READ_DOCUMENT_FROM_COLLECTION_ERROR
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
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int read_document_field_from_collection(const char* DATABASE, const char* COLLECTION, const char* DATA, const char* FIELD_NAME, char *result)
{
  // Constants
  const bson_t* current_document;

  // Variables
  char* message;
  char data2[BUFFER_SIZE];
  char settings[BUFFER_SIZE];
  char* message_copy1;
  char* message_copy2;
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
  mongoc_client_pool_push(database_client_thread_pool, database_client_thread);
  
  #define READ_DOCUMENT_FIELD_FROM_COLLECTION_ERROR(settings) \
  if ((strncmp(settings,"The database collection does not exist",BUFFER_SIZE) == 0 && test_settings == 1) || (debug_settings == 1 && test_settings == 0)) \
  { \
    memcpy(error_message.function[error_message.total],"read_document_field_from_collection",35); \
    memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
    error_message.total++; \
    fprintf(stderr,"Could not read field %s of collection %s with data %s\n\n",FIELD_NAME,COLLECTION,DATA); \
  } \
  database_reset_all; \
  return 0;

  memset(data2,0,sizeof(data2));
  memset(settings,0,sizeof(settings));

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
    READ_DOCUMENT_FIELD_FROM_COLLECTION_ERROR("The database collection does not exist");
  }
  
  if (!(document = bson_new_from_json((const uint8_t *)DATA, -1, &error)))
  {    
    READ_DOCUMENT_FIELD_FROM_COLLECTION_ERROR("Could not convert the data into a database document");
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

    if (strstr(data2,settings) == NULL)
    {
      READ_DOCUMENT_FIELD_FROM_COLLECTION_ERROR("Could not read the document field from the database collection");
    }
    message_copy1 = strstr(data2,settings) + strnlen(settings,BUFFER_SIZE);
    if (message_copy1 == NULL)
    {
      READ_DOCUMENT_FIELD_FROM_COLLECTION_ERROR("Could not read the document field from the database collection");
    }
    message_copy2 = strstr(message_copy1,"\"");
    if (message_copy2 == NULL)
    {
      READ_DOCUMENT_FIELD_FROM_COLLECTION_ERROR("Could not read the document field from the database collection");
    }
    memset(result,0,strlen(result));
    memcpy(result,message_copy1,message_copy2 - message_copy1); 
  }
  else
  {
    READ_DOCUMENT_FIELD_FROM_COLLECTION_ERROR("Could not read the document field from the database collection");
  }
  database_reset_all; 
  return 1;
  
  #undef database_reset_all
  #undef READ_DOCUMENT_FIELD_FROM_COLLECTION_ERROR
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

  // define macros
  #define DATABASE_DOCUMENT_PARSE_JSON_DATA_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"database_document_parse_json_data",33); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  // get the first item 
  if (strstr(DATA,",") == NULL)
  {
    DATABASE_DOCUMENT_PARSE_JSON_DATA_ERROR("Could not parse the json data for the database document");
  }
  data2 = strstr(DATA,",") + 3;
  if (strstr(data2,"\"") == NULL)
  {
    DATABASE_DOCUMENT_PARSE_JSON_DATA_ERROR("Could not parse the json data for the database document");
  }
  data3 = strstr(data2,"\"");
  memcpy(result->item[0],data2,strnlen(data2,BUFFER_SIZE)-strnlen(data3,BUFFER_SIZE)); 
  
  for (count = 0; count < result->count; count++)
  {
    if (data3 == NULL)
    {
      DATABASE_DOCUMENT_PARSE_JSON_DATA_ERROR("Could not parse the json data for the database document");
    }
    data2 = data3+5;
    if (strstr(data2,"\"") == NULL)
    {
      DATABASE_DOCUMENT_PARSE_JSON_DATA_ERROR("Could not parse the json data for the database document");
    }
    data3 = strstr(data2,"\"");
    memcpy(result->value[count],data2,strnlen(data2,BUFFER_SIZE)-strnlen(data3,BUFFER_SIZE));
      
    // only get the item if its not the last count
    if (count+1 != result->count)
    { 
      if (data3 == NULL)
      {
        DATABASE_DOCUMENT_PARSE_JSON_DATA_ERROR("Could not parse the json data for the database document");
      }
      data2 = data3+4;
      if (strstr(data2,"\"") == NULL)
      {
       DATABASE_DOCUMENT_PARSE_JSON_DATA_ERROR("Could not parse the json data for the database document");
      }
      data3 = strstr(data2,"\"");
      memcpy(result->item[count+1],data2,strnlen(data2,BUFFER_SIZE)-strnlen(data3,BUFFER_SIZE));
    }    
  } 
  return 1;

  #undef DATABASE_DOCUMENT_PARSE_JSON_DATA_ERROR
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

  // define macros
  #define DATABASE_MULTIPLE_DOCUMENTS_PARSE_JSON_DATA_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"database_document_parse_json_data",33); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  // get the first item 
  if (strstr(DATA,",") == NULL)
  {
    DATABASE_MULTIPLE_DOCUMENTS_PARSE_JSON_DATA_ERROR("Could not parse the json data for the database document");
  } 
  data2 = strstr(DATA,",") + 3;
  if (strstr(data2,"\"") == NULL)
  {
    DATABASE_MULTIPLE_DOCUMENTS_PARSE_JSON_DATA_ERROR("Could not parse the json data for the database document");
  }
  data3 = strstr(data2,"\"");
  memcpy(result->item[document_count][0],data2,strnlen(data2,BUFFER_SIZE)-strnlen(data3,BUFFER_SIZE)); 
  
  for (count = 0; count < result->database_fields_count; count++)
  {
    if (data3 == NULL)
    {
      DATABASE_MULTIPLE_DOCUMENTS_PARSE_JSON_DATA_ERROR("Could not parse the json data for the database document");
    }
    data2 = data3+5;
    if (strstr(data2,"\"") == NULL)
    {
      DATABASE_MULTIPLE_DOCUMENTS_PARSE_JSON_DATA_ERROR("Could not parse the json data for the database document");
    }
    data3 = strstr(data2,"\"");
    memcpy(result->value[document_count][count],data2,strnlen(data2,BUFFER_SIZE)-strnlen(data3,BUFFER_SIZE));
      
    // only get the item if its not the last count
    if (count+1 != result->database_fields_count)
    { 
      if (data3 == NULL)
      {
        DATABASE_MULTIPLE_DOCUMENTS_PARSE_JSON_DATA_ERROR("Could not parse the json data for the database document");
      }
      data2 = data3+4;
      if (strstr(data2,"\"") == NULL)
      {
       DATABASE_MULTIPLE_DOCUMENTS_PARSE_JSON_DATA_ERROR("Could not parse the json data for the database document");
      }
      data3 = strstr(data2,"\"");
      memcpy(result->item[document_count][count+1],data2,strnlen(data2,BUFFER_SIZE)-strnlen(data3,BUFFER_SIZE));
    }    
  } 
  return 1;

  #undef DATABASE_MULTIPLE_DOCUMENTS_PARSE_JSON_DATA_ERROR
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
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int read_document_all_fields_from_collection(const char* DATABASE, const char* COLLECTION, const char* DATA, struct database_document_fields *result)
{
  // Constants
  const bson_t* current_document;

  // Variables
  char* message;
  char data[BUFFER_SIZE];
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
  mongoc_client_pool_push(database_client_thread_pool, database_client_thread);
  
  #define READ_DOCUMENT_ALL_FIELDS_FROM_COLLECTION_ERROR(settings) \
  if ((strncmp(settings,"The database collection does not exist",BUFFER_SIZE) == 0 && test_settings == 1) || (strncmp(settings,"The database collection does not exist",BUFFER_SIZE) != 0)) \
  { \
    memcpy(error_message.function[error_message.total],"read_document_all_fields_from_collection",40); \
    memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
    error_message.total++; \
  } \
  database_reset_all; \
  return 0;

  memset(data,0,sizeof(data));

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
  else if (strstr(COLLECTION,"public_addresses_payments") != NULL)
  {
    result->count = TOTAL_PUBLIC_ADDRESSES_PAYMENTS_DATABASE_FIELDS;
  }
  else if (strstr(COLLECTION,"public_addresses") != NULL)
  {
    result->count = TOTAL_PUBLIC_ADDRESSES_DATABASE_FIELDS;
  }
  else if (strstr(COLLECTION,"blocks_found") != NULL)
  {
    result->count = TOTAL_BLOCKS_FOUND_DATABASE_FIELDS;
  }

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
    READ_DOCUMENT_ALL_FIELDS_FROM_COLLECTION_ERROR("The database collection does not exist");
  }
  
  if (!(document = bson_new_from_json((const uint8_t *)DATA, -1, &error)))
  {    
    READ_DOCUMENT_ALL_FIELDS_FROM_COLLECTION_ERROR("Could not convert the data into a database document");
  }
 
  document_settings = mongoc_collection_find_with_opts(collection, document, NULL, NULL);
  while (mongoc_cursor_next(document_settings, &current_document))
  {
    message = bson_as_canonical_extended_json(current_document, NULL);
    memcpy(data,message,strnlen(message,BUFFER_SIZE));
    bson_free(message);
    
    string_replace(data,sizeof(data)," }, ",", ");

    count = 1;
  }

  if (count == 1)
  {
    // parse the json data
    if (database_document_parse_json_data(data,result) == 0)
    {
      READ_DOCUMENT_ALL_FIELDS_FROM_COLLECTION_ERROR("Could not read all of the document fields from the database collection");
    }
  }  
  else
  {
    READ_DOCUMENT_ALL_FIELDS_FROM_COLLECTION_ERROR("Could not read all of the document fields from the database collection");
  }
  
  database_reset_all;
  return 1;

  #undef database_reset_all
  #undef READ_DOCUMENT_ALL_FIELDS_FROM_COLLECTION_ERROR
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
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int read_multiple_documents_all_fields_from_collection(const char* DATABASE, const char* COLLECTION, const char* DATA, struct database_multiple_documents_fields *result, const size_t DOCUMENT_COUNT_START, const size_t DOCUMENT_COUNT_TOTAL, const int DOCUMENT_OPTIONS, const char* DOCUMENT_OPTIONS_DATA)
{
  // Constants
  const bson_t* current_document;

  // Variables
  char* message;
  char data[BUFFER_SIZE];
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
  mongoc_client_pool_push(database_client_thread_pool, database_client_thread);
  
  #define READ_MULTIPLE_DOCUMENTS_ALL_FIELDS_FROM_COLLECTION_ERROR(settings) \
  if ((strncmp(settings,"The database collection does not exist",BUFFER_SIZE) == 0 && test_settings == 1) || (strncmp(settings,"The database collection does not exist",BUFFER_SIZE) != 0)) \
  { \
    memcpy(error_message.function[error_message.total],"read_multiple_documents_all_fields_from_collection",50); \
    memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
    error_message.total++; \
  } \
  database_reset_all; \
  return 0;

  memset(data,0,sizeof(data));

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
  else if (strstr(COLLECTION,"public_addresses_payments") != NULL)
  {
    result->database_fields_count = TOTAL_PUBLIC_ADDRESSES_PAYMENTS_DATABASE_FIELDS;
  }
  else if (strstr(COLLECTION,"public_addresses") != NULL)
  {
    result->database_fields_count = TOTAL_PUBLIC_ADDRESSES_DATABASE_FIELDS;
  }
  else if (strstr(COLLECTION,"blocks_found") != NULL)
  {
    result->database_fields_count = TOTAL_BLOCKS_FOUND_DATABASE_FIELDS;
  }

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
    READ_MULTIPLE_DOCUMENTS_ALL_FIELDS_FROM_COLLECTION_ERROR("The database collection does not exist");
  }

  if (!(document = memcmp(DATA,"",1) == 0 ? bson_new() : bson_new_from_json((const uint8_t *)DATA, -1, &error)))
  {
    READ_MULTIPLE_DOCUMENTS_ALL_FIELDS_FROM_COLLECTION_ERROR("Could not convert the data into a database document");
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
      string_replace(data,sizeof(data)," }, ",", ");   

      // parse the json data  
      if (database_multiple_documents_parse_json_data(data,result,counter) == 0)
      {
        READ_MULTIPLE_DOCUMENTS_ALL_FIELDS_FROM_COLLECTION_ERROR("Could not read all of the fields for all of the documents in the database collection");
      }
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
    READ_MULTIPLE_DOCUMENTS_ALL_FIELDS_FROM_COLLECTION_ERROR("Could not read all of the fields for all of the documents in the database collection");
  }
  database_reset_all;
  return 1;

  #undef database_reset_all
  #undef READ_MULTIPLE_DOCUMENTS_ALL_FIELDS_FROM_COLLECTION_ERROR
}