#include <stdio.h>
#include <stdlib.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "insert_database_functions.h"
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
Name: insert_document_into_collection_json
Description: Inserts a document into the collection in the database from json data
Parameters:
  DATABASE - The database name
  COLLECTION - The collection name
  DATA - The json data to insert into the collection
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int insert_document_into_collection_json(const char* DATABASE, const char* COLLECTION, const char* DATA)
{
  // check if the data is over the MAXIMUM_DATABASE_WRITE_SIZE
  if (strlen(DATA) > MAXIMUM_DATABASE_WRITE_SIZE)
  {
    return 0;
  }

  // Variables
  unsigned char string[BUFFER_SIZE];
  char data_hash[DATA_HASH_LENGTH+1];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  char* message;
  mongoc_client_t* database_client_thread = NULL;
  mongoc_collection_t* collection;
  bson_error_t error;
  bson_t* document = NULL;

  // define macros
  #define database_reset_all \
  bson_destroy(document); \
  mongoc_collection_destroy(collection); \
  mongoc_client_pool_push(database_client_thread_pool, database_client_thread);
  
  #define INSERT_DOCUMENT_INTO_COLLECTION_JSON_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"insert_document_into_collection_json",36); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  database_reset_all; \
  return 0;

  memset(string,0,sizeof(string));
  memset(data_hash,0,sizeof(data_hash));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  memcpy(data2,DATA,strnlen(DATA,sizeof(data2)));

  // format the data
  string_replace(data2,sizeof(data2),"\r\n","");
  string_replace(data2,sizeof(data2),"\n","");
  string_replace(data2,sizeof(data2),"\" : \"","\":\"");
  string_replace(data2,sizeof(data2),"\", \"","\",\"");
  string_replace(data2,sizeof(data2),"\" }","\"}");

  /* get the data hash of the document
  If the database collection is reserve proofs than the id is the public address created reserve proof
  If the database collection is reserve bytes than the id is the reserve bytes data hash
  If the database collection is delegates than the id is the public key
  If the database collection is statistics than the id is 0

  remote data
  If the database collection is remote_data than the id is address
  If the database collection is remote_data_delegates than the id is public_address
  */

  if (strstr(COLLECTION,"reserve_proofs") != NULL && strstr(data2,"\"public_address_created_reserve_proof\":\"") != NULL)
  {
    message = strstr(data2,"\"public_address_created_reserve_proof\":\"") + 40;
    memcpy(data_hash,"000000000000000000000000000000",DATA_HASH_LENGTH-XCASH_WALLET_LENGTH);
    memcpy(data_hash+strlen(data_hash),message,XCASH_WALLET_LENGTH);
  }
  else if (strstr(COLLECTION,"reserve_bytes") != NULL && strstr(data2,"\"reserve_bytes_data_hash\":\"") != NULL)
  {
    message = strstr(data2,"\"reserve_bytes_data_hash\":\"") + 27;
    memcpy(data_hash,message,DATA_HASH_LENGTH);
  }
  else if (strstr(COLLECTION,"statistics") != NULL)
  {
    memcpy(data_hash,"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",DATA_HASH_LENGTH);
  }
  else if (strstr(COLLECTION,"remote_data_delegates") != NULL && strstr(data2,"\"public_address\":\"") != NULL)
  {
    message = strstr(data2,"\"public_address\":\"") + 18;
    memcpy(data_hash,"000000000000000000000000000000",DATA_HASH_LENGTH-XCASH_WALLET_LENGTH);
    memcpy(data_hash+strlen(data_hash),message,XCASH_WALLET_LENGTH);
  }
  else if (strstr(COLLECTION,"remote_data") != NULL && strstr(data2,"\"address\":\"") != NULL)
  {
    message = strstr(data2,"\"address\":\"") + 11;
    memcpy(data_hash,"000000000000000000000000000000",DATA_HASH_LENGTH-XCASH_WALLET_LENGTH);
    memcpy(data_hash+strlen(data_hash),message,XCASH_WALLET_LENGTH);
  }
  else if (strstr(COLLECTION,"delegates") != NULL && strstr(data2,"\"public_key\":\"") != NULL)
  {
    message = strstr(data2,"\"public_key\":\"") + 14;
    memcpy(data_hash,"0000000000000000000000000000000000000000000000000000000000000000",DATA_HASH_LENGTH-VRF_PUBLIC_KEY_LENGTH);
    memcpy(data_hash+strlen(data_hash),message,VRF_PUBLIC_KEY_LENGTH);
  }
  else
  {
    random_string(data_hash,DATA_HASH_LENGTH);
  }

  // error check
  if (strlen(data_hash) != DATA_HASH_LENGTH)
  {
    return 0;
  }

  // add the data hash to the _id field 
  memcpy(data2,",",sizeof(char));
  memcpy(data3,"{\"_id\":\"",8);
  memcpy(data3+strlen(data3),data_hash,DATA_HASH_LENGTH);
  memcpy(data3+strlen(data3),"\"",sizeof(char));
  memcpy(data3+strlen(data3),data2,strnlen(data2,sizeof(data3)));

  // get a temporary connection
  if (!(database_client_thread = mongoc_client_pool_pop(database_client_thread_pool)))
  {
    return 0;
  }


  // we need to rehash this db next time
  del_hash(database_client_thread, COLLECTION);

  // set the collection
  collection = mongoc_client_get_collection(database_client_thread, DATABASE, COLLECTION);

  if (!(document = bson_new_from_json((const uint8_t *)data3, -1, &error)))
  {
    INSERT_DOCUMENT_INTO_COLLECTION_JSON_ERROR("Could not convert the data into a database document");
  }
    
  if (!mongoc_collection_insert_one(collection, document, NULL, NULL, &error))
  {
    INSERT_DOCUMENT_INTO_COLLECTION_JSON_ERROR("Could not insert the document into the database collection");
  }

  
  database_reset_all;
  return 1;

  #undef database_reset_all
  #undef INSERT_DOCUMENT_INTO_COLLECTION_JSON_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: insert_multiple_documents_into_collection_json
Description: Inserts a document into the collection in the database from json data
Parameters:
  DATABASE - The database name
  COLLECTION - The collection name
  DATA - The json data to insert into the collection
  DATA_TOTAL_LENGTH - The maximum length of DATA 
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int insert_multiple_documents_into_collection_json(const char* DATABASE, const char* COLLECTION, const char* DATA, const size_t DATA_TOTAL_LENGTH)
{
  // Constants
  const size_t MAXIMUM_AMOUNT = DATA_TOTAL_LENGTH >= MAXIMUM_BUFFER_SIZE ? MAXIMUM_BUFFER_SIZE : DATA_TOTAL_LENGTH+SMALL_BUFFER_SIZE;

  // Variables
  char buffer[1024];
  unsigned char string[BUFFER_SIZE];
  char data_hash[DATA_HASH_LENGTH+1];
  char string2[BUFFER_SIZE];
  char string3[BUFFER_SIZE];
  char* data2 = (char*)calloc(MAXIMUM_AMOUNT,sizeof(char));
  char* data3 = (char*)calloc(MAXIMUM_AMOUNT,sizeof(char));
  char* message;
  // since were going to be changing where data2 is referencing, we need to create a copy to pointer_reset
  char* datacopy = data2; 
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  mongoc_client_t* database_client_thread = NULL;
  mongoc_collection_t* collection;
  bson_error_t error;
  size_t count;
  size_t count2;

  // define macros
  #define pointer_reset_all \
  free(datacopy); \
  datacopy = NULL; \
  free(data3); \
  data3 = NULL;

  #define database_reset_all \
  mongoc_collection_destroy(collection); \
  mongoc_client_pool_push(database_client_thread_pool, database_client_thread);
  
  #define INSERT_MULTIPLE_DOCUMENTS_INTO_COLLECTION_JSON_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"insert_multiple_documents_into_collection_json",46); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  pointer_reset_all; \
  database_reset_all;

  if (data2 == NULL || data3 == NULL)
  {
    memcpy(error_message.function[error_message.total],"insert_multiple_documents_into_collection_json",46);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,buffer);  
    exit(0);
  }

  memset(buffer,0,sizeof(buffer));
  memset(string,0,sizeof(string));
  memset(string2,0,sizeof(string2));
  memset(string3,0,sizeof(string3));
  memset(data_hash,0,sizeof(data_hash));

  // get a temporary connection
  if (!(database_client_thread = mongoc_client_pool_pop(database_client_thread_pool)))
  {
    pointer_reset_all;
    return 0;
  }

  // we need to rehash this db next time
  del_hash(database_client_thread, COLLECTION);

  // set the collection
  collection = mongoc_client_get_collection(database_client_thread, DATABASE, COLLECTION);

  // create a copy of the data since were going to be changing where the data is referencing
  append_string(data2,DATA,MAXIMUM_AMOUNT);

  // count how many documents are in the data
  count = string_count(DATA,"},{") + 1;

  for (count2 = 0; count2 < count; count2++)
  {
    // get the document
    if ((count2+1) != count)
    {
      memset(data3,0,strlen(data3));
      memcpy(data3,data2,strnlen(data2,MAXIMUM_AMOUNT) - strnlen(strstr(data2,"},{"),MAXIMUM_AMOUNT)+1);
      data2 = strstr(data2,"},{") + 2;     
    }
    else
    {
      memset(data3,0,strlen(data3));
      memcpy(data3,data2,strnlen(data2,MAXIMUM_AMOUNT));
    }

    // check if the data is over the MAXIMUM_DATABASE_WRITE_SIZE
    if (strlen(data3) > MAXIMUM_DATABASE_WRITE_SIZE)
    {
      INSERT_MULTIPLE_DOCUMENTS_INTO_COLLECTION_JSON_ERROR("Could not convert the data into a database document");
      return 0;
    }

    // format the data
    string_replace(data3,MAXIMUM_AMOUNT,"\r\n","");
    string_replace(data3,MAXIMUM_AMOUNT,"\n","");
    string_replace(data3,MAXIMUM_AMOUNT,"\" : \"","\":\"");
    string_replace(data3,MAXIMUM_AMOUNT,"\", \"","\",\"");
    string_replace(data3,MAXIMUM_AMOUNT,"\" }","\"}");

    /* get the data hash of the document
    If the database collection is reserve proofs than the id is the public address created reserve proof
    If the database collection is reserve bytes than the id is the reserve bytes data hash
    If the database collection is delegates than the id is the public key
    If the database collection is statistics than the id is 0

    remote data
    If the database collection is remote_data than the id is address
    If the database collection is remote_data_delegates than the id is public_address
    */

    if (strstr(COLLECTION,"reserve_proofs") != NULL && strstr(data3,"\"public_address_created_reserve_proof\":\"") != NULL)
    {
      message = strstr(data3,"\"public_address_created_reserve_proof\":\"") + 40;
      memcpy(data_hash,"000000000000000000000000000000",DATA_HASH_LENGTH-XCASH_WALLET_LENGTH);
      memcpy(data_hash+strlen(data_hash),message,XCASH_WALLET_LENGTH);
    }
    else if (strstr(COLLECTION,"reserve_bytes") != NULL && strstr(data3,"\"reserve_bytes_data_hash\":\"") != NULL)
    {
      message = strstr(data3,"\"reserve_bytes_data_hash\":\"") + 27;
      memcpy(data_hash,message,DATA_HASH_LENGTH);
    }
    else if (strstr(COLLECTION,"statistics") != NULL)
    {
      memcpy(data_hash,"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",DATA_HASH_LENGTH);
    }
    else if (strstr(COLLECTION,"remote_data_delegates") != NULL && strstr(data3,"\"public_address\":\"") != NULL)
    {
      message = strstr(data3,"\"public_address\":\"") + 18;
      memcpy(data_hash,"000000000000000000000000000000",DATA_HASH_LENGTH-XCASH_WALLET_LENGTH);
      memcpy(data_hash+strlen(data_hash),message,XCASH_WALLET_LENGTH);
    }
    else if (strstr(COLLECTION,"remote_data") != NULL && strstr(data3,"\"address\":\"") != NULL)
    {
      message = strstr(data3,"\"address\":\"") + 11;
      memcpy(data_hash,"000000000000000000000000000000",DATA_HASH_LENGTH-XCASH_WALLET_LENGTH);
      memcpy(data_hash+strlen(data_hash),message,XCASH_WALLET_LENGTH);
    }
    else if (strstr(COLLECTION,"delegates") != NULL && strstr(data3,"\"public_key\":\"") != NULL)
    {
      message = strstr(data3,"\"public_key\":\"") + 14;
      memcpy(data_hash,"0000000000000000000000000000000000000000000000000000000000000000",DATA_HASH_LENGTH-VRF_PUBLIC_KEY_LENGTH);
      memcpy(data_hash+strlen(data_hash),message,VRF_PUBLIC_KEY_LENGTH);
    }
    else
    {
      random_string(data_hash,DATA_HASH_LENGTH);
    }
    

    // error check
    if (strlen(data_hash) != DATA_HASH_LENGTH)
    {
      pointer_reset_all;
      return 0;
    }

    memset(string2,0,sizeof(string2));
    memset(string3,0,sizeof(string3));

    // add the data hash to the _id field
    memcpy(string2,data3,strnlen(data3,sizeof(string2)));
    memcpy(string2,",",sizeof(char));
    memcpy(string3,"{\"_id\":\"",8);
    memcpy(string3+strlen(string3),data_hash,DATA_HASH_LENGTH);
    memcpy(string3+strlen(string3),"\"",sizeof(char));
    memcpy(string3+strlen(string3),string2,strnlen(string2,sizeof(string3)));

    bson_t* document = bson_new_from_json((const uint8_t *)string3, -1, &error);
    if (!document)
    {
      INSERT_MULTIPLE_DOCUMENTS_INTO_COLLECTION_JSON_ERROR("Could not convert the data into a database document");
      bson_destroy(document);
      return 0;
    }

    memset(data_hash,0,sizeof(data_hash));

    if (!mongoc_collection_insert_one(collection, document, NULL, NULL, &error))
    {
      INSERT_MULTIPLE_DOCUMENTS_INTO_COLLECTION_JSON_ERROR("Could not insert the document into the database collection");
      bson_destroy(document);
      return 0;
    }
    bson_destroy(document);
  }
  
  pointer_reset_all;
  database_reset_all;
  return 1;

  #undef pointer_reset_all
  #undef database_reset_all
  #undef INSERT_MULTIPLE_DOCUMENTS_INTO_COLLECTION_JSON_ERROR
}
