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
#include "update_database_functions.h"
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
    memcpy(database_data+strlen(database_data),&message[142],strnlen(message,BUFFER_SIZE) - 142);    
    bson_free(message);
    count = 1;
  }

  // if the database collection is empty, have it say empty for database syncing
  if (count != 1)
  {
    memset(database_data,0,strlen(database_data));
    memcpy(database_data,DATABASE_EMPTY_STRING,sizeof(DATABASE_EMPTY_STRING)-1);
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
  char data2[SMALL_BUFFER_SIZE];
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
  memset(data2,0,sizeof(data2));

  // get a temporary connection
  if (!(database_client_thread = mongoc_client_pool_pop(database_client_thread_pool)))
  {
    return 0;
  }

  // set the collection
  collection = mongoc_client_get_collection(database_client_thread, DATABASE, COLLECTION);

  // get all of the database collections
  memcpy(data,"{\"dbHash\":1,\"collections\":[\"",28);

  if (strncmp(COLLECTION,"reserve_bytes",BUFFER_SIZE) == 0)
  {
    // get the current reserve bytes database
    get_reserve_bytes_database(count2,0);

    for (count = 1; count <= count2; count++)
    {
      memcpy(data+strlen(data),"reserve_bytes_",14);
      snprintf(data+strlen(data),MAXIMUM_NUMBER_SIZE,"%zu",count);
      if (count != count2)
      {
        memcpy(data+strlen(data),"\",\"",3);
      }
    }
  }
  else if (strncmp(COLLECTION,"reserve_proofs",BUFFER_SIZE) == 0)
  {
    for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
    {
      memcpy(data+strlen(data),"reserve_proofs_",15);
      snprintf(data+strlen(data),MAXIMUM_NUMBER_SIZE,"%zu",count);

      memset(data2,0,sizeof(data2));
      memcpy(data2,"reserve_proofs_",15);
      snprintf(data2+strlen(data2),MAXIMUM_NUMBER_SIZE,"%zu",count+1);
      if (check_if_database_collection_exist(database_name,data2) == 1)
      {
        memcpy(data+strlen(data),"\",\"",3);
      }
      else
      {
        break;
      }      
    }    
  }
  else
  {
    memcpy(data+strlen(data),COLLECTION,strnlen(COLLECTION,sizeof(data)));
  }
  memcpy(data+strlen(data),"\"]}",3);

  command = strncmp(COLLECTION,"ALL",3) == 0 ? BCON_NEW ("dbHash", BCON_INT32 (1)) : bson_new_from_json((const uint8_t *)data, -1, &error);
  
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



/*
-----------------------------------------------------------------------------------------------------------
Name: get_database_collection_size
Description: Gets the database collection size
Parameters:
  DATABASE - The database name
  COLLECTION - The collection name. If reserve_proofs or reserve_bytes without a number it will get a database hash of all of the reserve_proofs or reserve_bytes
Return: 0 if an error has occured, otherwise the database collection size in bytes
-----------------------------------------------------------------------------------------------------------
*/

size_t get_database_collection_size(const char* DATABASE, const char* COLLECTION)
{
  if (check_if_database_collection_exist(DATABASE,COLLECTION) == 0)
  {
    return 0;
  }

  // Variables
  char data[BUFFER_SIZE];
  char data2[SMALL_BUFFER_SIZE];
  char* message;
  char* message2;
  char* message3;
  size_t count;
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
  memset(data2,0,sizeof(data2));

  // get a temporary connection
  if (!(database_client_thread = mongoc_client_pool_pop(database_client_thread_pool)))
  {
    return 0;
  }

  // set the collection
  collection = mongoc_client_get_collection(database_client_thread, DATABASE, COLLECTION);
  
  command = BCON_NEW("collStats",BCON_UTF8(COLLECTION));

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

  message2 = strstr(data,"\"size\"");
  message3 = strstr(message2,",");
  memcpy(data2,&message2[9],(strlen(message2) - strlen(message3)) - 9);
  
  sscanf(data2, "%zu", &count);

  database_reset_all;  
  return count;

  #undef database_reset_all
}



/*
-----------------------------------------------------------------------------------------------------------
Name: reserve_proofs_delegate_check
Description: Checks the reserve proofs and delegates data
-----------------------------------------------------------------------------------------------------------
*/

void reserve_proofs_delegate_check(void)
{
  // Structures
  struct delegate_total_vote_count {
    char public_address[MAXIMUM_AMOUNT_OF_DELEGATES][XCASH_WALLET_LENGTH+1];
    long long int total_vote_count[MAXIMUM_AMOUNT_OF_DELEGATES];
  };

  // Constants
  const bson_t* current_document;

  // Variables
  mongoc_client_t* database_client_thread = NULL;
  mongoc_collection_t* collection = NULL;
  mongoc_cursor_t* document_settings = NULL;
  bson_t* document = NULL; 
  char* message;
  char* message_copy1;
  char* message_copy2;
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  char buffer[BUFFER_SIZE];
  struct delegate_total_vote_count delegate_total_vote_count;
  int count;
  int count2;
  long long int current_total;

  // define macros
  #define DATABASE_FIELD_NAME_DELEGATES ", \"public_address\" : \""
  #define DATABASE_FIELD_NAME_RESERVE_PROOFS_1 ", \"public_address_voted_for\" : \""
  #define DATABASE_FIELD_NAME_RESERVE_PROOFS_2 ", \"total\" : \""

  #define database_reset_all \
  bson_destroy(document); \
  mongoc_cursor_destroy(document_settings); \
  mongoc_collection_destroy(collection); \
  mongoc_client_pool_push(database_client_thread_pool, database_client_thread);

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));
  memset(buffer,0,sizeof(buffer));

  // initialize the struct delegate_total_vote_count
  for (count = 0; count < MAXIMUM_AMOUNT_OF_DELEGATES; count++)
  {
    memset(delegate_total_vote_count.public_address[count],0,sizeof(delegate_total_vote_count.public_address[count]));
    delegate_total_vote_count.total_vote_count[count] = 0;
  }

  // get a temporary connection
  if (!(database_client_thread = mongoc_client_pool_pop(database_client_thread_pool)))
  {
    return;
  }

  // set the collection
  collection = mongoc_client_get_collection(database_client_thread, database_name, "delegates");

  if (!(document = bson_new()))
  {
    database_reset_all;
    return;
  }

  document_settings = mongoc_collection_find_with_opts(collection, document, NULL, NULL);

  count = 0;
  while (mongoc_cursor_next(document_settings, &current_document))
  {
    // get the current document  
    message = bson_as_canonical_extended_json(current_document, NULL);
    memset(data2,0,sizeof(data2));
    memcpy(data2,message,strnlen(message,sizeof(data2)));    
    bson_free(message);
    
    // parse the public_address
    if (strstr(data2,DATABASE_FIELD_NAME_DELEGATES) == NULL)
    {
      database_reset_all;
      return;
    }
    message_copy1 = strstr(data2,DATABASE_FIELD_NAME_DELEGATES) + strnlen(DATABASE_FIELD_NAME_DELEGATES,BUFFER_SIZE);
    if (message_copy1 == NULL)
    {
      database_reset_all;
      return;
    }
    message_copy2 = strstr(message_copy1,"\"");
    if (message_copy2 == NULL)
    {
      database_reset_all;
      return;
    }
    memset(data,0,sizeof(data));
    memcpy(data,message_copy1,message_copy2 - message_copy1); 

    memcpy(delegate_total_vote_count.public_address[count],data,XCASH_WALLET_LENGTH);
    count++;
  }


  for (count = 0; count < TOTAL_RESERVE_PROOFS_DATABASES; count++)
  {
    memset(data3,0,sizeof(data3));
    memcpy(data3,"reserve_proofs_",15);
    snprintf(data3+15,MAXIMUM_NUMBER_SIZE,"%d",count);

    // set the collection
    collection = mongoc_client_get_collection(database_client_thread, database_name, data3);

    // check if the database collection exist
    if (check_if_database_collection_exist(database_name,data3) == 0)
    {
      continue;
    }

    if (!(document = bson_new()))
    {
      database_reset_all;
      return;
    }

    document_settings = mongoc_collection_find_with_opts(collection, document, NULL, NULL);

    while (mongoc_cursor_next(document_settings, &current_document))
    { 
      // get the current document  
      message = bson_as_canonical_extended_json(current_document, NULL);
      memset(data2,0,sizeof(data2));
      memcpy(data2,message,strnlen(message,sizeof(data2)));    
      bson_free(message);

      // parse the public_address_voted_for
      if (strstr(data2,DATABASE_FIELD_NAME_RESERVE_PROOFS_1) == NULL)
      {
        database_reset_all;
        return;
      }
      message_copy1 = strstr(data2,DATABASE_FIELD_NAME_RESERVE_PROOFS_1) + strnlen(DATABASE_FIELD_NAME_RESERVE_PROOFS_1,BUFFER_SIZE);
      if (message_copy1 == NULL)
      {
        database_reset_all;
        return;
      }
      message_copy2 = strstr(message_copy1,"\"");
      if (message_copy2 == NULL)
      {
        database_reset_all;
        return;
      }
      memset(data,0,sizeof(data));
      memcpy(data,message_copy1,message_copy2 - message_copy1);       
    
      // parse the total
      if (strstr(data2,DATABASE_FIELD_NAME_RESERVE_PROOFS_2) == NULL)
      {
        database_reset_all;
        return;
      }
      message_copy1 = strstr(data2,DATABASE_FIELD_NAME_RESERVE_PROOFS_2) + strnlen(DATABASE_FIELD_NAME_RESERVE_PROOFS_2,BUFFER_SIZE);
      if (message_copy1 == NULL)
      {
        database_reset_all;
        return;
      }
      message_copy2 = strstr(message_copy1,"\"");
      if (message_copy2 == NULL)
      {
        database_reset_all;
        return;
      }
      memset(buffer,0,sizeof(buffer));
      memcpy(buffer,message_copy1,message_copy2 - message_copy1); 

      sscanf(buffer,"%lld", &current_total);

      for (count2 = 0; count2 < MAXIMUM_AMOUNT_OF_DELEGATES; count2++)
      {
        if (strncmp(data,delegate_total_vote_count.public_address[count2],BUFFER_SIZE) == 0)
        {
          delegate_total_vote_count.total_vote_count[count2] += current_total;
          break;
        }
      }
    }
  }

  for (count2 = 0; count2 < MAXIMUM_AMOUNT_OF_DELEGATES; count2++)
  {
    if (strlen(delegate_total_vote_count.public_address[count2]) == XCASH_WALLET_LENGTH)
    {
      memset(data,0,sizeof(data));
      memset(data2,0,sizeof(data2));
      memset(data3,0,sizeof(data3));

      memcpy(data,"{\"public_address\":\"",19);
      memcpy(data+strlen(data),delegate_total_vote_count.public_address[count2],XCASH_WALLET_LENGTH);
      memcpy(data+strlen(data),"\"}",2);

      memcpy(data2,"{\"total_vote_count\":\"",21);
      snprintf(data2+strlen(data2),MAXIMUM_NUMBER_SIZE,"%lld",delegate_total_vote_count.total_vote_count[count2]);
      memcpy(data2+strlen(data2),"\"}",2);

      update_document_from_collection(database_name,"delegates",data,data2);
    }
  }
  
  database_reset_all;
  return;

  #undef DATABASE_FIELD_NAME_DELEGATES
  #undef DATABASE_FIELD_NAME_RESERVE_PROOFS_1
  #undef DATABASE_FIELD_NAME_RESERVE_PROOFS_2
  #undef database_reset_all
}
