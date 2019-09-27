#include <stdio.h>
#include <stdlib.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

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
Name: create_database_connection
Description: Creates a database connection
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int create_database_connection(void)
{
  // Variables
  mongoc_uri_t* uri;
  bson_t* command = NULL;
  bson_t reply;
  bson_error_t error;

  // define macros
  #define database_reset_all \
  mongoc_uri_destroy(uri); \
  bson_destroy(&reply); \
  bson_destroy(command);

  // create a connection to the database
  uri = mongoc_uri_new_with_error(DATABASE_CONNECTION, &error);
  if (!uri)
  {
    return 0;
  }
  database_client = mongoc_client_new_from_uri(uri);
  if (!database_client)
  {
    database_reset_all;
    return 0;
  }
  command = BCON_NEW("ping", BCON_INT32(1));
  if (!mongoc_client_command_simple(database_client, "admin", command, NULL, &reply, &error))
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
    print_error_message;  
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
  mongoc_client_t* database_client_thread = NULL;
  mongoc_collection_t* collection;
  mongoc_cursor_t* document_settings = NULL;
  bson_error_t error;
  bson_t* document = NULL; 
  char* message;
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  char* settings = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message_copy1;
  char* message_copy2;
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
    print_error_message;  
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

  // get the parameter count
  result->count = string_count(DATA,":") - 2;

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

  // get the parameter count
  result->database_fields_count = string_count(DATA,":") - 2;

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
  mongoc_client_t* database_client_thread = NULL;
  mongoc_collection_t* collection;
  mongoc_cursor_t* document_settings = NULL;
  bson_error_t error;
  bson_t* document = NULL; 
  char* message;
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
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
    print_error_message;  
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
  mongoc_client_t* database_client_thread = NULL;
  mongoc_collection_t* collection;
  mongoc_cursor_t* document_settings = NULL;
  bson_t* document = NULL;  
  bson_t* document_options = NULL;
  char* message;
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
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
    print_error_message;  
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
      pointer_reset(data);
      return 0;
    }
    // set the collection
    collection = mongoc_client_get_collection(database_client_thread, DATABASE, COLLECTION);
  }
  
  document = bson_new();
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

      if ((strncmp(DATA,"",BUFFER_SIZE) == 0) || (strncmp(DATA,"",BUFFER_SIZE) != 0 && strstr(data,DATA) != NULL))
      {
        string_replace(data,BUFFER_SIZE," }, ",", ");

        // parse the json data      
        database_multiple_documents_parse_json_data(data,result,counter);
        counter++;
        result->document_count++;
      }
     
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



/*
-----------------------------------------------------------------------------------------------------------
Name: get_block_verifiers
Description: Gets the top 100 delegates from the database and sorts them by total_vote_count
Parameters:
  result - A database_multiple_documents_fields struct to hold the data
  struct database_multiple_documents_fields
    document_count - The number of documents
    database_fields_count - The number of items in the database document
    item[100][100] - The database document items
    value[100][100] - The database document values
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int get_block_verifiers(struct database_multiple_documents_fields *result)
{
  // Constants
  const bson_t* current_document;

  // Variables
  mongoc_collection_t* collection;
  mongoc_cursor_t* document_settings = NULL;
  bson_t* document = NULL;  
  bson_t* document_options = NULL;
  char* message;
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;
  size_t count2;
  size_t counter = 0;
  struct database_multiple_documents_fields* database_multiple_documents_fields = NULL;

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define database_reset_all \
  bson_destroy(document); \
  bson_destroy(document_options); \
  mongoc_cursor_destroy(document_settings); \
  mongoc_collection_destroy(collection);

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    memcpy(error_message.function[error_message.total],"read_multiple_documents_all_fields_from_collection",50);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  // initialize the database_multiple_documents_fields struct 
  for (count = 0; count < MAXIMUM_AMOUNT_OF_DELEGATES; count++)
  {
    for (count2 = 0; count2 < TOTAL_DELEGATES_DATABASE_FIELDS; count2++)
    {
      // allocate more for the about and the block_producer_block_heights
      if (count2+1 != TOTAL_DELEGATES_DATABASE_FIELDS)
      {
        database_multiple_documents_fields->item[count][count2] = (char*)calloc(100,sizeof(char));
        database_multiple_documents_fields->value[count][count2] = (char*)calloc(50000,sizeof(char));
      }
      else if (count2 == 4)
      {
        database_multiple_documents_fields->item[count][count2] = (char*)calloc(100,sizeof(char));
        database_multiple_documents_fields->value[count][count2] = (char*)calloc(1025,sizeof(char));
      }
      else
      {
        database_multiple_documents_fields->item[count][count2] = (char*)calloc(100,sizeof(char));
        database_multiple_documents_fields->value[count][count2] = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
      }
      
      if (database_multiple_documents_fields->item[count][count2] == NULL || database_multiple_documents_fields->value[count][count2] == NULL)
      {
        memcpy(error_message.function[error_message.total],"check_delegates_online_status_timer_thread",42);
        memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
        error_message.total++;
        print_error_message;  
        exit(0);
      }
    } 
  } 
  database_multiple_documents_fields->document_count = 0;
  database_multiple_documents_fields->database_fields_count = 0;

  // set the collection
  collection = mongoc_client_get_collection(database_client, DATABASE_NAME, DATABASE_COLLECTION);
  
  document = bson_new();
  if (!document)
  {
    pointer_reset(data);
    database_reset_all;
    return 0;
  }
 
  document_settings = mongoc_collection_find_with_opts(collection, document, document_options, NULL);
  while (mongoc_cursor_next(document_settings, &current_document))
  {
    message = bson_as_canonical_extended_json(current_document, NULL);
    memset(data,0,strnlen(data,BUFFER_SIZE));
    memcpy(data,message,strnlen(message,BUFFER_SIZE));
    bson_free(message); 
      
    string_replace(data,BUFFER_SIZE," }, ",", ");

    // parse the json data      
    database_multiple_documents_parse_json_data(data,database_multiple_documents_fields,counter);
    counter++;
    database_multiple_documents_fields->document_count++;
  }
  
  pointer_reset(data);
  database_reset_all;
  return 1;

  #undef database_reset_all
}



/*
-----------------------------------------------------------------------------------------------------------
Name: update_document_from_collection
Description: Updates a document from the collection
Parameters:
  DATABASE - The database name
  COLLECTION - The collection name
  DATA - The json data to use to search the collection for
  FIELD_NAME_AND_DATA - The json data to use to update the document
  THREAD_SETTINGS - 1 to use a separate thread, otherwise 0
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int update_document_from_collection(const char* DATABASE, const char* COLLECTION, const char* DATA, const char* FIELD_NAME_AND_DATA, const int THREAD_SETTINGS)
{
  // Variables
  mongoc_client_t* database_client_thread = NULL;
  mongoc_collection_t* collection;
  bson_error_t error;
  bson_t* update = NULL;
  bson_t* update_settings = NULL;
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // define macros
  #define database_reset_all \
  bson_destroy(update); \
  bson_destroy(update_settings); \
  mongoc_collection_destroy(collection); \
  if (THREAD_SETTINGS == 1) \
  { \
    mongoc_client_pool_push(database_client_thread_pool, database_client_thread); \
  }

  // check if the memory needed was allocated on the heap successfully
  if (data2 == NULL)
  {
    memcpy(error_message.function[error_message.total],"update_document_from_collection",31);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
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
      pointer_reset(data2);
      return 0;
    }
    // set the collection
    collection = mongoc_client_get_collection(database_client_thread, DATABASE, COLLECTION);
  }
  
  update = bson_new_from_json((const uint8_t *)DATA, -1, &error);
  if (!update)
  {
    pointer_reset(data2);
    database_reset_all;
    return 0;
  }
 
  const size_t FIELD_NAME_AND_DATA_LENGTH = strnlen(FIELD_NAME_AND_DATA,BUFFER_SIZE)-1;
  memcpy(data2,"{\"$set\":",8);
  memcpy(data2+8,FIELD_NAME_AND_DATA,FIELD_NAME_AND_DATA_LENGTH);
  memcpy(data2+8+FIELD_NAME_AND_DATA_LENGTH,"}}",2);

  update_settings = bson_new_from_json((const uint8_t *)data2, -1, &error);
  if (!update_settings)
  {
    pointer_reset(data2);
    database_reset_all;
    return 0;
  }
  
  if (!mongoc_collection_update_one(collection, update, update_settings, NULL, NULL, &error))
  {
    pointer_reset(data2);
    database_reset_all;
    return 0;
  }

  pointer_reset(data2);
  database_reset_all;
  return 1;

  #undef database_reset_all
}



/*
-----------------------------------------------------------------------------------------------------------
Name: update_all_documents_from_collection
Description: Updates all documents in a collection
Parameters:
  DATABASE - The database name
  COLLECTION - The collection name
  DATA - The json data to use to update the documents
  THREAD_SETTINGS - 1 to use a separate thread, otherwise 0
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int update_all_documents_from_collection(const char* DATABASE, const char* COLLECTION, const char* DATA, const int THREAD_SETTINGS)
{
  // Variables
  mongoc_client_t* database_client_thread = NULL;
  mongoc_collection_t* collection;
  bson_error_t error;
  bson_t* update = NULL;
  bson_t* update_settings = NULL;
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // define macros
  #define database_reset_all \
  bson_destroy(update); \
  bson_destroy(update_settings); \
  mongoc_collection_destroy(collection); \
  if (THREAD_SETTINGS == 1) \
  { \
    mongoc_client_pool_push(database_client_thread_pool, database_client_thread); \
  }

  // check if the memory needed was allocated on the heap successfully
  if (data2 == NULL)
  {
    memcpy(error_message.function[error_message.total],"update_all_documents_from_collection",36);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
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
      pointer_reset(data2);
      return 0;
    }
    // set the collection
    collection = mongoc_client_get_collection(database_client_thread, DATABASE, COLLECTION);
  }
  
  // set the document to empty so it will get each document in the collection  
  update = bson_new();
  if (!update)
  {
    pointer_reset(data2);
    database_reset_all;
    return 0;
  }
 
  const size_t DATA_LENGTH = strnlen(DATA,BUFFER_SIZE)-1;
  memcpy(data2,"{\"$set\":",8);
  memcpy(data2+8,DATA,DATA_LENGTH);
  memcpy(data2+8+DATA_LENGTH,"}}",2);

  update_settings = bson_new_from_json((const uint8_t *)data2, -1, &error);
  if (!update_settings)
  {
    pointer_reset(data2);
    database_reset_all;
    return 0;
  }
  
  if (!mongoc_collection_update_many(collection, update, update_settings, NULL, NULL, &error))
  {
    pointer_reset(data2);
    database_reset_all;
    return 0;
  }

  pointer_reset(data2);
  database_reset_all;
  return 1;

  #undef database_reset_all
}



/*
-----------------------------------------------------------------------------------------------------------
Name: delete_document_from_collection
Description: Deletes a document from the collection
Parameters:
  DATABASE - The database name
  COLLECTION - The collection name
  DATA - The json data to use to delete the document
  THREAD_SETTINGS - 1 to use a separate thread, otherwise 0
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int delete_document_from_collection(const char* DATABASE, const char* COLLECTION, const char* DATA, const int THREAD_SETTINGS)
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
    return 0;
  }
  
  if (!mongoc_collection_delete_one(collection, document, NULL, NULL, &error))
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
Name: delete_collection_from_database
Description: Deletes a collection from the database
Parameters:
  DATABASE - The database name
  COLLECTION - The collection name
  THREAD_SETTINGS - 1 to use a separate thread, otherwise 0
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int delete_collection_from_database(const char* DATABASE, const char* COLLECTION, const int THREAD_SETTINGS)
{
   // Variables
  mongoc_client_t* database_client_thread = NULL;
  mongoc_collection_t* collection;
  bson_error_t error;

  // define macros
  #define database_reset_all \
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
   
  if (!mongoc_collection_drop(collection, &error))
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



/*
-----------------------------------------------------------------------------------------------------------
Name: get_database_data
Description: Gets the database data
Parameters:
  database_data - The database data
  DATABASE - The database name
  COLLECTION - The collection name
  THREAD_SETTINGS - 1 to use a separate thread, otherwise 0
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int get_database_data(char *database_data, const char* DATABASE, const char* COLLECTION, const int THREAD_SETTINGS)
{
  // Constants
  const bson_t* current_document;

  // Variables
  mongoc_client_t* database_client_thread = NULL;
  mongoc_collection_t* collection = NULL;
  mongoc_cursor_t* document_settings = NULL;
  bson_t* document = NULL;  
  char* message;

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

  document = bson_new();
  if (!document)
  {
    database_reset_all;
    return 0;
  }

  memset(database_data,0,strlen(database_data));

  document_settings = mongoc_collection_find_with_opts(collection, document, NULL, NULL);
  while (mongoc_cursor_next(document_settings, &current_document))
  { 
    // get the current document  
    message = bson_as_canonical_extended_json(current_document, NULL);
    if (strnlen(database_data,MAXIMUM_BUFFER_SIZE) == 0)
    {
      memcpy(database_data+strnlen(database_data,MAXIMUM_BUFFER_SIZE),"{",1);
    }
    else
    {
      memcpy(database_data+strnlen(database_data,MAXIMUM_BUFFER_SIZE),",{",2);
    }
    memcpy(database_data+strnlen(database_data,MAXIMUM_BUFFER_SIZE),&message[51],strnlen(message,BUFFER_SIZE) - 51);    
    bson_free(message);
  }
  
  database_reset_all;
  return 1;

  #undef database_reset_all
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_database_data_hash
Description: Gets a database data hash
Parameters:
  data_hash - The data hash
  DATABASE - The database name
  COLLECTION - The collection name. If reserve_proofs or reserve_bytes without a number it will get a database hash of all of the reserve_proofs or reserve_bytes
  THREAD_SETTINGS - 1 to use a separate thread, otherwise 0
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int get_database_data_hash(char *data_hash, const char* DATABASE, const char* COLLECTION, const int THREAD_SETTINGS)
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
  unsigned char* string = (unsigned char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char)); // 50 MB
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* reserve_proofs_data_hash[TOTAL_RESERVE_PROOFS_DATABASES];
  char* reserve_bytes_data_hash[10000];
  size_t count;
  size_t count2;
  size_t count3;
  size_t counter;

  // define macros
  #define pointer_reset_all \
  pthread_rwlock_wrlock(&rwlock); \
  database_settings = 1; \
  pthread_rwlock_unlock(&rwlock); \
  free(string); \
  string = NULL; \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; \
  for (count = 0; count < TOTAL_RESERVE_PROOFS_DATABASES; count++) \
  { \
    pointer_reset(reserve_proofs_data_hash[count]); \
  } \
  for (count = 0; count < 10000; count++) \
  { \
    pointer_reset(reserve_bytes_data_hash[count]); \
  }

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
  if (string == NULL || data == NULL || data2 == NULL)
  {
     if (string != NULL)
    {
      pointer_reset(string);
    }
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    memcpy(error_message.function[error_message.total],"get_database_data_hash",22);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  } 

  // initialize the reserve_proofs_data_hash
  for (count = 0; count < TOTAL_RESERVE_PROOFS_DATABASES; count++)
  {
    reserve_proofs_data_hash[count] = (char*)calloc(DATA_HASH_LENGTH+1,sizeof(char));

    // check if the memory needed was allocated on the heap successfully
    if (reserve_proofs_data_hash[count] == NULL)
    {
      memcpy(error_message.function[error_message.total],"get_database_data_hash",22);
      memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
      error_message.total++;
      print_error_message;  
      exit(0);
    }
  }

  // initialize the reserve_bytes_data_hash
  for (count = 0; count < 10000; count++)
  {
    reserve_bytes_data_hash[count] = (char*)calloc(DATA_HASH_LENGTH+1,sizeof(char));

    // check if the memory needed was allocated on the heap successfully
    if (reserve_bytes_data_hash[count] == NULL)
    {
      memcpy(error_message.function[error_message.total],"get_database_data_hash",22);
      memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
      error_message.total++;
      print_error_message;  
      exit(0);
    }
  }

  // check if we need to create a database connection, or use the global database connection
  if (THREAD_SETTINGS == 1)
  {
    database_client_thread = mongoc_client_pool_pop(database_client_thread_pool);
    if (!database_client_thread)
    {
      pointer_reset_all;
      return 0;
    }
  }
  
  document = bson_new();
  if (!document)
  {
    pointer_reset_all;
    database_reset_all;
    return 0;
  }

  // sort the documents
  if (strstr(COLLECTION,"reserve_proofs") != NULL)
  {
    document_options = BCON_NEW("sort", "{", "public_address_created_reserve_proof", BCON_INT32(-1), "}");
  }
  else if (strstr(COLLECTION,"reserve_bytes") != NULL)
  {
    document_options = BCON_NEW("sort", "{", "block_height", BCON_INT32(-1), "}");
  }
  else if (strstr(COLLECTION,"delegates") != NULL)
  {
    document_options = BCON_NEW("sort", "{", "public_address", BCON_INT32(-1), "}");
  }

  // have the database not add any new documents
  pthread_rwlock_wrlock(&rwlock);
  database_settings = 0;
  pthread_rwlock_unlock(&rwlock);

  // reserve proofs all
  if (strncmp(COLLECTION,"reserve_proofs",BUFFER_SIZE) == 0)
  {  
    document_options = BCON_NEW("sort", "{", "", BCON_INT32(-1), "}");    
    for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
    {
      memset(data2,0,strlen(data2));
      memcpy(data2,"reserve_proofs_",15);  
      snprintf(data2+15,BUFFER_SIZE-16,"%zu",count);

      // set the collection
      if (THREAD_SETTINGS == 0)
      {
        collection = mongoc_client_get_collection(database_client, DATABASE, data2);
      }
      else
      {
         collection = mongoc_client_get_collection(database_client_thread, DATABASE, data2);
      }

      memset(data,0,strnlen(data,MAXIMUM_BUFFER_SIZE));
      count2 = 0;

      document_settings = mongoc_collection_find_with_opts(collection, document, document_options, NULL);
      while (mongoc_cursor_next(document_settings, &current_document))
      { 
        // get the current document  
        message = bson_as_canonical_extended_json(current_document, NULL);
        if (strnlen(data,MAXIMUM_BUFFER_SIZE) == 0)
        {
          memcpy(data+strnlen(data,MAXIMUM_BUFFER_SIZE),"{",1);
        }
        else
        {
          memcpy(data+strnlen(data,MAXIMUM_BUFFER_SIZE),",{",2);
        }
        memcpy(data+strnlen(data,MAXIMUM_BUFFER_SIZE),&message[51],strnlen(message,BUFFER_SIZE) - 51);    
        bson_free(message);
      }
      // get the data hash of the collection  
      memset(string,0,strlen((char*)string));    
      crypto_hash_sha512(string,(const unsigned char*)data,strnlen(data,MAXIMUM_BUFFER_SIZE));
      memset(reserve_proofs_data_hash[count-1],0,strlen(reserve_proofs_data_hash[count-1]));
      for (count3 = 0, count2 = 0; count3 < DATA_HASH_LENGTH / 2; count3++, count2 += 2)
      {
        snprintf(reserve_proofs_data_hash[count-1]+count2,BUFFER_SIZE,"%02x",string[count3] & 0xFF);
      }
    }

    // get the data hash of the all of the reserve proofs data hash
    memset(data,0,strlen(data));
    for (count = 0; count < TOTAL_RESERVE_PROOFS_DATABASES; count++)
    {
      memcpy(data+strlen(data),reserve_proofs_data_hash[count],DATA_HASH_LENGTH);
    }

    // get the data hash of the collection
    memset(string,0,strlen((char*)string));
    crypto_hash_sha512(string,(const unsigned char*)data,strnlen(data,MAXIMUM_BUFFER_SIZE));
  }



  // reserve bytes all
  else if (strncmp(COLLECTION,"reserve_bytes",BUFFER_SIZE) == 0)
  { 
    document_options = BCON_NEW("sort", "{", "", BCON_INT32(-1), "}");
    sscanf(current_block_height,"%zu", &count3);
    if (count3 < XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT)
    {      
      pointer_reset_all;
      database_reset_all;
      return 0;
    }
    counter = ((count3 - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME) + 1;
    for (count = 1; count <= counter; count++)
    {
      memset(data2,0,strlen(data2));
      memcpy(data2,"reserve_bytes_",14);  
      snprintf(data2+14,BUFFER_SIZE-15,"%zu",count);

      // set the collection
      if (THREAD_SETTINGS == 0)
      {
        collection = mongoc_client_get_collection(database_client, DATABASE, data2);
      }
      else
      {
         collection = mongoc_client_get_collection(database_client_thread, DATABASE, data2);
      }

      memset(data,0,strlen(data));
      count2 = 0;

      document_settings = mongoc_collection_find_with_opts(collection, document, document_options, NULL);
      while (mongoc_cursor_next(document_settings, &current_document))
      { 
        // get the current document  
        message = bson_as_canonical_extended_json(current_document, NULL);
        if (strnlen(data,MAXIMUM_BUFFER_SIZE) == 0)
        {
          memcpy(data+strnlen(data,MAXIMUM_BUFFER_SIZE),"{",1);
        }
        else
        {
          memcpy(data+strnlen(data,MAXIMUM_BUFFER_SIZE),",{",2);
        }
        memcpy(data+strnlen(data,MAXIMUM_BUFFER_SIZE),&message[51],strnlen(message,BUFFER_SIZE) - 51);    
        bson_free(message);
      }
      // get the data hash of the collection  
      memset(string,0,strlen((char*)string)); 
      crypto_hash_sha512(string,(const unsigned char*)data,strnlen(data,MAXIMUM_BUFFER_SIZE));
      memset(reserve_bytes_data_hash[count-1],0,strlen(reserve_bytes_data_hash[count-1]));
      for (count3 = 0, count2 = 0; count3 < DATA_HASH_LENGTH / 2; count3++, count2 += 2)
      {
        snprintf(reserve_bytes_data_hash[count-1]+count2,BUFFER_SIZE,"%02x",string[count3] & 0xFF);
      }
    }

    // get the data hash of the all of the reserve proofs data hash
    memset(data,0,strlen(data));
    for (count = 0; count < counter; count++)
    {
      memcpy(data+strlen(data),reserve_bytes_data_hash[count],DATA_HASH_LENGTH);
    }

    // get the data hash of the collection
    memset(string,0,strlen((char*)string));
    crypto_hash_sha512(string,(const unsigned char*)data,strnlen(data,MAXIMUM_BUFFER_SIZE));
  }



  else
  {
    // set the collection
    if (THREAD_SETTINGS == 0)
    {
      collection = mongoc_client_get_collection(database_client, DATABASE, COLLECTION);
    }
    else
    {
       collection = mongoc_client_get_collection(database_client_thread, DATABASE, COLLECTION);
    }

    memset(data,0,strnlen(data,MAXIMUM_BUFFER_SIZE));
    count2 = 0;

    document_settings = mongoc_collection_find_with_opts(collection, document, document_options, NULL);
    while (mongoc_cursor_next(document_settings, &current_document))
    { 
      // get the current document  
      message = bson_as_canonical_extended_json(current_document, NULL);
      if (strnlen(data,MAXIMUM_BUFFER_SIZE) == 0)
      {
        memcpy(data+strnlen(data,MAXIMUM_BUFFER_SIZE),"{",1);
      }
      else
      {
        memcpy(data+strnlen(data,MAXIMUM_BUFFER_SIZE),",{",2);
      }
      memcpy(data+strnlen(data,MAXIMUM_BUFFER_SIZE),&message[51],strnlen(message,BUFFER_SIZE) - 51);    
      bson_free(message);
    }
    // get the data hash of the collection
    memset(string,0,strlen((char*)string));
    crypto_hash_sha512(string,(const unsigned char*)data,strnlen(data,MAXIMUM_BUFFER_SIZE));
  }

  // convert the data hash to a string
  memset(data_hash,0,strlen(data_hash));
  for (count = 0, count2 = 0; count < DATA_HASH_LENGTH / 2; count++, count2 += 2)
  {
    snprintf(data_hash+count2,BUFFER_SIZE,"%02x",string[count] & 0xFF);
  }

  pointer_reset_all;
  database_reset_all;
  return 1;

  #undef pointer_reset_all
  #undef database_reset_all
}



/*
-----------------------------------------------------------------------------------------------------------
Name: update_delegates_online_status
Description: Updates all of the delegates online status
Parameters:
  DATABASE - The database name
  COLLECTION - The collection name
  THREAD_SETTINGS - 1 to use a separate thread, otherwise 0
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int update_delegates_online_status(const char* DATABASE, const char* COLLECTION, const int THREAD_SETTINGS)
{
  // Constants
  const bson_t* current_document;

  // Variables
  mongoc_client_t* database_client_thread = NULL;
  mongoc_collection_t* collection;
  mongoc_cursor_t* document_settings = NULL;
  bson_t* document = NULL;  
  bson_error_t error;
  bson_t* update = NULL;
  bson_t* update_settings = NULL;
  char* message;
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* public_address = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* IP_address = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message_copy1;
  char* message_copy2;

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; \
  free(public_address); \
  public_address = NULL; \
  free(IP_address); \
  IP_address = NULL; 

  #define database_reset_all \
  bson_destroy(document); \
  bson_destroy(update); \
  bson_destroy(update_settings); \
  mongoc_cursor_destroy(document_settings); \
  mongoc_collection_destroy(collection); \
  if (THREAD_SETTINGS == 1) \
  { \
    mongoc_client_pool_push(database_client_thread_pool, database_client_thread); \
  }

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL || public_address == NULL || IP_address == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    if (public_address != NULL)
    {
      pointer_reset(public_address);
    }
    if (IP_address != NULL)
    {
      pointer_reset(IP_address);
    }
    memcpy(error_message.function[error_message.total],"update_delegates_online_status",30);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message; 
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
  
  document = bson_new();
  if (!document)
  {
    pointer_reset_all;
    database_reset_all;
    return 0;
  }
 
  document_settings = mongoc_collection_find_with_opts(collection, document, NULL, NULL);
  while (mongoc_cursor_next(document_settings, &current_document))
  { 
    // get the current document
    memset(data,0,strnlen(data,BUFFER_SIZE));
    memset(data2,0,strnlen(data2,BUFFER_SIZE));
    message = bson_as_canonical_extended_json(current_document, NULL);
    memcpy(data,message,strnlen(message,BUFFER_SIZE));
    bson_free(message);

    if (strstr(data,"public_address") != NULL && strstr(data,"IP_address") != NULL && strstr(data,"online_status") != NULL)
    {
      // get the public_address and IP_address
      memcpy(data2,data,strnlen(data,BUFFER_SIZE));
      message_copy1 = strstr(data,", \"public_address\" : \"") + 22;
      message_copy2 = strstr(message_copy1,"\"");
      memcpy(public_address,message_copy1,message_copy2 - message_copy1);
      message_copy1 = strstr(data2,", \"IP_address\" : \"") + 18;
      message_copy2 = strstr(message_copy1,"\"");
      memcpy(IP_address,message_copy1,message_copy2 - message_copy1);

      // create the data to use to search the collection for
      memset(data,0,strnlen(data,BUFFER_SIZE));
      memcpy(data,"{\"public_address\": \"",20);
      memcpy(data+20,public_address,strnlen(public_address,BUFFER_SIZE));
      memcpy(data+20+strnlen(public_address,BUFFER_SIZE),"\"}",2);

      update = bson_new_from_json((const uint8_t *)data, -1, &error);
      if (!update)
      {
        pointer_reset_all;
        database_reset_all;
        return 0;
      }

      // get the online status of the delegate
      memset(data2,0,strnlen(data2,BUFFER_SIZE));
      if (get_delegate_online_status(IP_address) == 1)
      {
        memcpy(data2,"{\"$set\":{\"online_status\": \"true\"}}",34);
      }
      else
      {
        memcpy(data2,"{\"$set\":{\"online_status\": \"false\"}}",35);
      }

      update_settings = bson_new_from_json((const uint8_t *)data2, -1, &error);
      if (!update_settings)
      {
        pointer_reset_all;
        database_reset_all;
        return 0;
      }

      // update the document  
      if (!mongoc_collection_update_one(collection, update, update_settings, NULL, NULL, &error))
      {
        pointer_reset_all;
        database_reset_all;
        return 0;
      }
    }    
  }

  pointer_reset_all;
  database_reset_all;
  return 1;

  #undef pointer_reset_all
  #undef database_reset_all
}