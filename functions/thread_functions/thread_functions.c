#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macros.h"
#include "structures.h"

#include "database_functions.h"
#include "file_functions.h"
#include "thread_functions.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: read_file_thread
Description: Reads a file on a separate thread
Parameters:
  parameters - A pointer to the read_file_thread_parameters struct
  struct read_file_thread_parameters
    result - The data read from the file
    FILE_NAME - The file name
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

void* read_file_thread(void* parameters)
{
  struct read_file_thread_parameters* data = parameters;
  int settings = read_file(data->result, data->FILE_NAME);
  pthread_exit((void *)(intptr_t)settings);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: write_file_thread
Description: Writes a file on a separate thread
Parameters:
  parameters - A pointer to the write_file_thread_parameters struct
  struct write_file_thread_parameters
    DATA - The data to write to the file
    FILE_NAME - The file name
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

void* write_file_thread(void* parameters)
{
  struct write_file_thread_parameters* data = parameters;
  int settings = write_file(data->DATA, data->FILE_NAME);
  pthread_exit((void *)(intptr_t)settings);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: insert_document_into_collection_json_thread
Description: Inserts a document into the collection in the database from json data on a separate thread
Parameters:
  parameters - A pointer to the insert_document_into_collection_json_thread_parameters struct
  struct insert_document_into_collection_json_thread_parameters
    DATABASE - The database name
    COLLECTION - The collection name
    DATA - The json data to insert into the collection
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

void* insert_document_into_collection_json_thread(void* parameters)
{
  struct insert_document_into_collection_json_thread_parameters* data = parameters;
  int settings = insert_document_into_collection_json(data->DATABASE, data->COLLECTION, data->DATA, 1);
  pthread_exit((void *)(intptr_t)settings);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: insert_multiple_documents_into_collection_json_thread
Description: Inserts multiple documents into the collection in the database from json data on a separate thread
Parameters:
  parameters - A pointer to the insert_document_into_collection_json_thread_parameters struct
  struct insert_document_into_collection_json_thread_parameters
    DATABASE - The database name
    COLLECTION - The collection name
    DATA - The json data to insert into the collection
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

void* insert_multiple_documents_into_collection_json_thread(void* parameters)
{
  struct insert_multiple_documents_into_collection_json_thread_parameters* data = parameters;
  int settings = insert_multiple_documents_into_collection_json(data->DATABASE, data->COLLECTION, data->DATA, 1);
  pthread_exit((void *)(intptr_t)settings);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: read_document_from_collection_thread
Description: Reads a document from the collection on a separate thread
Parameters:
  parameters - A pointer to the read_document_from_collection_thread_parameters struct
  struct read_document_from_collection_thread_parameters
    DATABASE - The database name
    COLLECTION - The collection name
    DATA - The json data to use to search the collection for
    result - The document read from the collection
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

void* read_document_from_collection_thread(void* parameters)
{
  struct read_document_from_collection_thread_parameters* data = parameters;
  int settings = read_document_from_collection(data->DATABASE, data->COLLECTION, data->DATA, data->result, 1);
  pthread_exit((void *)(intptr_t)settings);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: read_document_field_from_collection_thread
Description: Reads a field from a document from the collection on a separate thread
Parameters:
  parameters - A pointer to the read_document_field_from_collection_thread_parameters struct
  struct read_document_field_from_collection_thread_parameters
    DATABASE - The database name
    COLLECTION - The collection name
    DATA - The json data to use to search the collection for
    FIELD_NAME - The field of the document data to read
    result - The document data read from the collection
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

void* read_document_field_from_collection_thread(void* parameters)
{
  struct read_document_field_from_collection_thread_parameters* data = parameters;
  int settings = read_document_field_from_collection(data->DATABASE, data->COLLECTION, data->DATA, data->FIELD_NAME, data->result, 1);
  pthread_exit((void *)(intptr_t)settings);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: read_document_all_fields_from_collection_thread
Description: Reads all fields from a document from the collection on a separate thread
Parameters:
  parameters - A pointer to the read_document_all_fields_from_collection_thread_parameters struct
  struct read_document_all_fields_from_collection_thread_parameters
    DATABASE - The database name
    COLLECTION - The collection name
    DATA - The json data to use to search the collection for
    result - A pointer to the database_document_fields struct
      struct database_document_fields
        count - The number of items in the database document
        item[100] - The database document items
        value[100] - The database document values
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

void* read_document_all_fields_from_collection_thread(void* parameters)
{
  struct read_document_all_fields_from_collection_thread_parameters* data = parameters;
  int settings = read_document_all_fields_from_collection(data->DATABASE, data->COLLECTION, data->DATA, data->result, 1);
  pthread_exit((void *)(intptr_t)settings);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: read_multiple_documents_all_fields_from_collection_thread
Description: Reads all fields from multiple documents from the collection on a separate thread
Parameters:
  parameters - A pointer to the read_multiple_documents_all_fields_from_collection_thread_parameters struct
  struct read_multiple_documents_all_fields_from_collection_thread_parameters
    DATABASE - The database name
    COLLECTION - The collection name 
    FIELD_NAME_ITEM - The field name to filter the results for, set to "" for all items
    FIELD_NAME_VALUE - The field value to filter the results for, set to "" for all items
    result - A database_fields struct to hold the data
    struct database_multiple_documents_fields
      document_count - The number of documents
      database_fields_count - The number of items in the database document
      item[100][100] - The database document items
      value[100][100] - The database document values
    DOCUMENT_COUNT_START - The document to start at when reading the data
    DOCUMENT_COUNT_TOTAL - The total amount of documents to read
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

void* read_multiple_documents_all_fields_from_collection_thread(void* parameters)
{
  struct read_multiple_documents_all_fields_from_collection_thread_parameters* data = parameters;
  int settings = read_multiple_documents_all_fields_from_collection(data->DATABASE, data->COLLECTION, data->DATA, data->result, data->DOCUMENT_COUNT_START, data->DOCUMENT_COUNT_TOTAL, data->DOCUMENT_OPTIONS, data->DOCUMENT_OPTIONS_DATA, 1);
  pthread_exit((void *)(intptr_t)settings);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: update_document_from_collection_thread
Description: Updates a document from the collection on a separate thread
Parameters:
  parameters - A pointer to the update_document_from_collection_thread_parameters struct
  struct update_document_from_collection_thread_parameters
    DATABASE - The database name
    COLLECTION - The collection name
    DATA - The json data to use to search the collection for
    FIELD_NAME_AND_DATA - The json data to use to update the document
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

void* update_document_from_collection_thread(void* parameters)
{
  struct update_document_from_collection_thread_parameters* data = parameters;
  int settings = update_document_from_collection(data->DATABASE, data->COLLECTION, data->DATA, data->FIELD_NAME_AND_DATA, 1);
  pthread_exit((void *)(intptr_t)settings);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: update_all_documents_from_collection_thread
Description: Updates all documents in a collection on a separate thread
Parameters:
  parameters - A pointer to the update_all_documents_from_collection_thread_parameters struct
  struct update_all_documents_from_collection_thread_parameters
    DATABASE - The database name
    COLLECTION - The collection name
    DATA - The json data to use to update the documents
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

void* update_all_documents_from_collection_thread(void* parameters)
{
  struct update_all_documents_from_collection_thread_parameters* data = parameters;
  int settings = update_all_documents_from_collection(data->DATABASE, data->COLLECTION, data->DATA, 1);
  pthread_exit((void *)(intptr_t)settings);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: delete_document_from_collection_thread
Description: Deletes a document from the collection on a separate thread
Parameters:
  parameters - A pointer to the delete_document_from_collection_thread_parameters struct
  struct delete_document_from_collection_thread_parameters
    DATABASE - The database name
    COLLECTION - The collection name
    DATA - The json data to use to delete the document
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

void* delete_document_from_collection_thread(void* parameters)
{
  struct delete_document_from_collection_thread_parameters* data = parameters;
  int settings = delete_document_from_collection(data->DATABASE, data->COLLECTION, data->DATA, 1);
  pthread_exit((void *)(intptr_t)settings);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: delete_collection_from_database_thread
Description: Deletes a collection from the database on a separate thread
Parameters:
  parameters - A pointer to the delete_collection_from_database_thread_parameters struct
  struct delete_collection_from_database_thread_parameters
    DATABASE - The database name
    COLLECTION - The collection name
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

void* delete_collection_from_database_thread(void* parameters)
{
  struct delete_collection_from_database_thread_parameters* data = parameters;
  int settings = delete_collection_from_database(data->DATABASE, data->COLLECTION, 1);
  pthread_exit((void *)(intptr_t)settings);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: count_documents_in_collection_thread
Description: Counts the documents in the collection that match a specific field name and field on a separate thread
Parameters:
  parameters - A pointer to the count_documents_in_collection_thread_parameters struct
  struct count_documents_in_collection_thread_parameters
    DATABASE - The database name
    COLLECTION - The collection name
    DATA - The json data to use to search the collection for
Return: -1 if an error has occured, otherwise the amount of documents that match a specific field name and field in the collection
-----------------------------------------------------------------------------------------------------------
*/

void* count_documents_in_collection_thread(void* parameters)
{
  struct count_documents_in_collection_thread_parameters* data = parameters;
  int settings = count_documents_in_collection(data->DATABASE, data->COLLECTION, data->DATA, 1);
  pthread_exit((void *)(intptr_t)settings);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: count_all_documents_in_collection_thread
Description: Counts all the documents in the collection on a separate thread
Parameters:
  parameters - A pointer to the count_all_documents_in_collection_thread_parameters struct
  struct count_all_documents_in_collection_thread_parameters
    DATABASE - The database name
    COLLECTION - The collection name
Return: -1 if an error has occured, otherwise the amount of documents in the collection
-----------------------------------------------------------------------------------------------------------
*/

void* count_all_documents_in_collection_thread(void* parameters)
{
  struct count_all_documents_in_collection_thread_parameters* data = parameters;
  int settings = count_all_documents_in_collection(data->DATABASE, data->COLLECTION, 1);
  pthread_exit((void *)(intptr_t)settings);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_database_data_thread
Description: Gets the database data on a separate thread
Parameters:
  parameters - A pointer to the get_database_data_thread_parameters struct
  struct get_database_data_thread_parameters
    database_data - The database data
    DATABASE - The database name
    COLLECTION - The collection name. If reserve_proofs or reserve_bytes without a number it will get a database hash of all of the reserve_proofs or reserve_bytes
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

void* get_database_data_thread(void* parameters)
{
  struct get_database_data_thread_parameters* data = parameters;
  int settings = get_database_data(data->database_data, data->DATABASE, data->COLLECTION, 1);
  pthread_exit((void *)(intptr_t)settings);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_database_data_hash_thread
Description: Gets a database data hash on a separate thread
Parameters:
  parameters - A pointer to the get_database_data_hash_thread_parameters struct
  struct get_database_data_hash_thread_parameters
    data_hash - The data hash
    DATABASE - The database name
    COLLECTION - The collection name. If reserve_proofs or reserve_bytes without a number it will get a database hash of all of the reserve_proofs or reserve_bytes
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

void* get_database_data_hash_thread(void* parameters)
{
  struct get_database_data_hash_thread_parameters* data = parameters;
  int settings = get_database_data_hash(data->data_hash, data->DATABASE, data->COLLECTION, 1);
  pthread_exit((void *)(intptr_t)settings);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: update_delegates_online_status_thread
Description: Updates all of the delegates online status on a separate thread
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

void* update_delegates_online_status_thread(void* parameters)
{
  struct update_delegates_online_status_thread_parameters* data = parameters;
  int settings = update_delegates_online_status(data->DATABASE, data->COLLECTION, 1);
  pthread_exit((void *)(intptr_t)settings);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: thread_settings
Description: Waits for a thread to return a value, and returns the value from the thread
Parameters:
   thread_id - The thread id of the thread
Return: The value that the thread returned
-----------------------------------------------------------------------------------------------------------
*/

int thread_settings(pthread_t thread_id)
{
  void* thread_settings;
  pthread_join(thread_id, &thread_settings);
  int settings = (intptr_t)thread_settings;
  return settings;
}