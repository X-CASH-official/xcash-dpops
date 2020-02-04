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

  #define CREATE_DATABASE_CONNECTION_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"create_database_connection",26); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  database_reset_all; \
  return 0;

  // create a connection to the database
  uri = mongoc_uri_new_with_error(DATABASE_CONNECTION, &error);
  if (!uri)
  {
    return 0;
  }
  database_client = mongoc_client_new_from_uri(uri);
  if (!database_client)
  {
    CREATE_DATABASE_CONNECTION_ERROR("Could not create a database connection");
  }
  command = BCON_NEW("ping", BCON_INT32(1));
  if (!mongoc_client_command_simple(database_client, "admin", command, NULL, &reply, &error))
  {
    CREATE_DATABASE_CONNECTION_ERROR("Could not create a database connection");
  }
  database_reset_all;
  return 1;

  #undef database_reset_all
  #undef CREATE_DATABASE_CONNECTION_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: check_if_database_collection_exist
Description: Checks if a database collection exist
Parameters:
  DATABASE - The database name
  COLLECTION - The collection name
  THREAD_SETTINGS - 1 to use a separate thread, otherwise 0
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int check_if_database_collection_exist(const char* DATABASE, const char* COLLECTION, const int THREAD_SETTINGS)
{
   // Variables
  mongoc_client_t* database_client_thread = NULL;
  mongoc_database_t* database;
  bson_error_t error;

  // define macros
  #define database_reset_all \
  mongoc_database_destroy(database); \
  if (THREAD_SETTINGS == 1) \
  { \
    mongoc_client_pool_push(database_client_thread_pool, database_client_thread); \
  }

  // check if we need to create a database connection, or use the global database connection
  if (THREAD_SETTINGS == 0)
  {
    database = mongoc_client_get_database(database_client, DATABASE);
  }
  else
  {
    database_client_thread = mongoc_client_pool_pop(database_client_thread_pool);
    if (!database_client_thread)
    {
      return 0;
    }
    database = mongoc_client_get_database(database_client_thread, DATABASE);
  }  
   
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
Name: get_delegates_total_voters_count
Description: Counts all the delegates voters
Parameters:
  DELEGATES_PUBLIC_ADDRESS - The delegates public address
Return: 0 if an error has occured, otherwise the amount of voters for the delegate
-----------------------------------------------------------------------------------------------------------
*/

int get_delegates_total_voters_count(const char* DELEGATES_PUBLIC_ADDRESS)
{
  // Variables
  char data[1024];
  char data2[1024];
  int public_address_count;
  int count;
  int counter;

  memset(data,0,sizeof(data));
  memcpy(data,"{\"public_address_voted_for\":\"",29);
  memcpy(data+29,DELEGATES_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH);
  memcpy(data+127,"\"}",2);

  // get the count of how many public addresses voted for the delegate
  for (public_address_count = 0, count = 1, counter = 0; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
  { 
    memset(data2,0,strlen(data2));
    memcpy(data2,"reserve_proofs_",15);
    snprintf(data2+15,sizeof(data2)-16,"%d",count);
    counter = count_documents_in_collection(database_name,data2,data,1);
    if (counter != -1)
    {
      public_address_count += counter;
    }
  }
  return public_address_count;
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
  bson_t* document_options = NULL;
  char* message;
  int count = 0;

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
  #define GET_DATABASE_DATA_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"get_database_data",17); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  database_reset_all; \
  return 0;

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
    GET_DATABASE_DATA_ERROR("Could not convert the data into a database document");
  }

  // sort the documents
  if (strstr(COLLECTION,"reserve_proofs") != NULL)
  {
    document_options = BCON_NEW("sort", "{", "total", BCON_INT32(-1), "}");
  }
  else if (strstr(COLLECTION,"reserve_bytes") != NULL)
  {
    document_options = BCON_NEW("sort", "{", "block_height", BCON_INT32(1), "}");
  }
  else if (strstr(COLLECTION,"delegates") != NULL)
  {
    document_options = BCON_NEW("sort", "{", "total_vote_count", BCON_INT32(-1), "}");
  }

  memset(database_data,0,strlen(database_data));

  document_settings = mongoc_collection_find_with_opts(collection, document, document_options, NULL);
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
    count = 1;
  }

  if (count != 1)
  {
    //GET_DATABASE_DATA_ERROR("Could not get the database data");
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
  unsigned char* string = (unsigned char*)calloc(BUFFER_SIZE,sizeof(unsigned char));
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char)); // 50 MB
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  size_t count;
  size_t count2;
  size_t count3;
  size_t counter = TOTAL_RESERVE_PROOFS_DATABASES; 

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(string); \
  string = NULL;

  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  if (strncmp(COLLECTION,"reserve_bytes",BUFFER_SIZE) == 0)
  {
    // get the current reserve bytes database
    get_reserve_bytes_database(counter,0);
  }

  char database_data_hash[counter][DATA_HASH_LENGTH+1];
  for (count = 0; count < counter; count++)
  {
    memset(database_data_hash[count],0,sizeof(database_data_hash[count]));
  }

  if (strncmp(COLLECTION,"reserve_proofs",BUFFER_SIZE) == 0)
  {
    // get the data hash of the reserve proofs database
    for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
    {
      memset(data,0,strlen(data));
      memset(data2,0,strlen(data2));
      memcpy(data2,"reserve_proofs_",15);  
      snprintf(data2+15,BUFFER_SIZE-16,"%zu",count);
      if (get_database_data(data,DATABASE,data2,1) == 0)
      {
        continue;
      }

      // get the data hash of the collection  
      memset(string,0,strlen((char*)string));    
      crypto_hash_sha512(string,(const unsigned char*)data,strnlen(data,MAXIMUM_BUFFER_SIZE));
      for (count3 = 0, count2 = 0; count3 < DATA_HASH_LENGTH / 2; count3++, count2 += 2)
      {
        snprintf(database_data_hash[count-1]+count2,BUFFER_SIZE,"%02x",string[count3] & 0xFF);
      }
    }
    RESET_ERROR_MESSAGES;

    memset(data,0,strlen(data));
    memset(data_hash,0,strlen(data_hash));

    for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
    {
      memcpy(data+strlen(data),database_data_hash[count-1],DATA_HASH_LENGTH);
    }

    memset(string,0,strlen((char*)string));    
    crypto_hash_sha512(string,(const unsigned char*)data,strnlen(data,MAXIMUM_BUFFER_SIZE));
    for (count3 = 0, count2 = 0; count3 < DATA_HASH_LENGTH / 2; count3++, count2 += 2)
    {
      snprintf(data_hash+count2,BUFFER_SIZE,"%02x",string[count3] & 0xFF);
    }
  }
  else if (strncmp(COLLECTION,"reserve_bytes",BUFFER_SIZE) == 0)
  {
    // get the data hash of the reserve bytes database
    for (count = 1; count <= counter; count++)
    {
      memset(data,0,strlen(data));
      memset(data2,0,strlen(data2));
      memcpy(data2,"reserve_bytes_",14);  
      snprintf(data2+14,BUFFER_SIZE-15,"%zu",count);
      get_database_data(data,DATABASE,data2,1);

      // get the data hash of the collection  
      memset(string,0,strlen((char*)string));    
      crypto_hash_sha512(string,(const unsigned char*)data,strnlen(data,MAXIMUM_BUFFER_SIZE));
      for (count3 = 0, count2 = 0; count3 < DATA_HASH_LENGTH / 2; count3++, count2 += 2)
      {
        snprintf(database_data_hash[count-1]+count2,BUFFER_SIZE,"%02x",string[count3] & 0xFF);
      }
    }
    RESET_ERROR_MESSAGES;

    memset(data,0,strlen(data));
    memset(data_hash,0,strlen(data_hash));

    for (count = 1; count <= counter; count++)
    {
      memcpy(data+strlen(data),database_data_hash[count-1],DATA_HASH_LENGTH);
    }

    memset(string,0,strlen((char*)string));    
    crypto_hash_sha512(string,(const unsigned char*)data,strnlen(data,MAXIMUM_BUFFER_SIZE));
    for (count3 = 0, count2 = 0; count3 < DATA_HASH_LENGTH / 2; count3++, count2 += 2)
    {
      snprintf(data_hash+count2,BUFFER_SIZE,"%02x",string[count3] & 0xFF);
    }
  }
  else if (strncmp(COLLECTION,"ALL",BUFFER_SIZE) == 0)
  {
    // get the data hash of the reserve proofs database
    for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
    {
      memset(data,0,strlen(data));
      memset(data2,0,strlen(data2));
      memcpy(data2,"reserve_proofs_",15);  
      snprintf(data2+15,BUFFER_SIZE-16,"%zu",count);
      get_database_data(data,DATABASE,data2,1);

      // get the data hash of the collection  
      memset(string,0,strlen((char*)string));    
      crypto_hash_sha512(string,(const unsigned char*)data,strnlen(data,MAXIMUM_BUFFER_SIZE));
      for (count3 = 0, count2 = 0; count3 < DATA_HASH_LENGTH / 2; count3++, count2 += 2)
      {
        snprintf(database_data_hash[count-1]+count2,BUFFER_SIZE,"%02x",string[count3] & 0xFF);
      }
    }
    RESET_ERROR_MESSAGES;

    for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
    {
      memcpy(data3+strlen(data3),database_data_hash[count-1],DATA_HASH_LENGTH);
    }

    for (count = 0; count < TOTAL_RESERVE_PROOFS_DATABASES; count++)
    {
      memset(database_data_hash[count],0,sizeof(database_data_hash[count]));
    }

    // get the data hash of the reserve bytes database
    get_reserve_bytes_database(counter,0);
    for (count = 1; count <= counter; count++)
    {
      memset(data,0,strlen(data));
      memset(data2,0,strlen(data2));
      memcpy(data2,"reserve_bytes_",14);  
      snprintf(data2+14,BUFFER_SIZE-15,"%zu",count);
      get_database_data(data,DATABASE,data2,1);

      // get the data hash of the collection  
      memset(string,0,strlen((char*)string));    
      crypto_hash_sha512(string,(const unsigned char*)data,strnlen(data,MAXIMUM_BUFFER_SIZE));
      for (count3 = 0, count2 = 0; count3 < DATA_HASH_LENGTH / 2; count3++, count2 += 2)
      {
        snprintf(database_data_hash[count-1]+count2,BUFFER_SIZE,"%02x",string[count3] & 0xFF);
      }
    }
    RESET_ERROR_MESSAGES;

    for (count = 1; count <= counter; count++)
    {
      memcpy(data3+strlen(data3),database_data_hash[count-1],DATA_HASH_LENGTH);
    }

    // get the data hash of the delegates database
    memset(data,0,strlen(data));
    get_database_data(data,DATABASE,"delegates",1);

    // get the data hash of the collection  
    memset(string,0,strlen((char*)string));    
    crypto_hash_sha512(string,(const unsigned char*)data,strnlen(data,MAXIMUM_BUFFER_SIZE));
    for (count3 = 0, count2 = 0; count3 < DATA_HASH_LENGTH / 2; count3++, count2 += 2)
    {
      snprintf(data3+strlen(data3)+count2,BUFFER_SIZE,"%02x",string[count3] & 0xFF);
    }

    // get the data hash of the delegates database
    memset(data,0,strlen(data));
    get_database_data(data,DATABASE,"statistics",1);

    // get the data hash of the collection  
    memset(string,0,strlen((char*)string));    
    crypto_hash_sha512(string,(const unsigned char*)data,strnlen(data,MAXIMUM_BUFFER_SIZE));
    for (count3 = 0, count2 = 0; count3 < DATA_HASH_LENGTH / 2; count3++, count2 += 2)
    {
      snprintf(data3+strlen(data3)+count2,BUFFER_SIZE,"%02x",string[count3] & 0xFF);
    }

    // combine all of the database data hashes
    memset(data,0,strlen(data));
    memcpy(data,data_hash,strnlen(data_hash,MAXIMUM_BUFFER_SIZE));
    memset(data_hash,0,strlen(data_hash));
    memset(string,0,strlen((char*)string));    
    crypto_hash_sha512(string,(const unsigned char*)data3,strnlen(data3,MAXIMUM_BUFFER_SIZE));
    for (count3 = 0, count2 = 0; count3 < DATA_HASH_LENGTH / 2; count3++, count2 += 2)
    {
      snprintf(data_hash+count2,BUFFER_SIZE,"%02x",string[count3] & 0xFF);
    }
  }
  else if (memcmp(COLLECTION,"reserve_proofs_",15) == 0 || memcmp(COLLECTION,"reserve_bytes_",14) == 0 || strncmp(COLLECTION,"delegates",BUFFER_SIZE) == 0 || strncmp(COLLECTION,"statistics",BUFFER_SIZE) == 0 || strncmp(COLLECTION,"XCASH_PROOF_OF_STAKE_TEST_DATA",BUFFER_SIZE) == 0)
  {
    // get the data hash of the reserve proofs database
    memset(data,0,strlen(data));
    memset(data_hash,0,strlen(data_hash));
    get_database_data(data,DATABASE,COLLECTION,1);

    // get the data hash of the collection  
    memset(string,0,strlen((char*)string));    
    crypto_hash_sha512(string,(const unsigned char*)data,strnlen(data,MAXIMUM_BUFFER_SIZE));
    for (count3 = 0, count2 = 0; count3 < DATA_HASH_LENGTH / 2; count3++, count2 += 2)
    {
      snprintf(data_hash+count2,BUFFER_SIZE,"%02x",string[count3] & 0xFF);
    }
  }
  pointer_reset_all;
  return 1;

  #undef pointer_reset_all
}