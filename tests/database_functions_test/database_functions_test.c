#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"

#include "database_functions.h"
#include "thread_functions.h"

#include "define_macros_test.h"
#include "variables_test.h"
#include "database_functions_test.h"
#include "network_functions.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: database_test
Description: Test the database functions
Return: The number of passed database test
-----------------------------------------------------------------------------------------------------------
*/

int database_test()
{   
  // Variables  
  int count;
  int counter;
  pthread_t thread_id;

  // define macros
  #define DATABASE_TEST 30
  #define DATA_COUNT 5
  #define DATABASE_COLLECTION "XCASH_PROOF_OF_STAKE_TEST_DATA"
  #define MESSAGE "{\"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\"}"
  #define MESSAGE_SETTINGS "{\"message_settings\": \"XCASH_PROOF_OF_STAKE_DATA\"}"
  #define DATABASE_COLLECTION_STATISTICS_DATA "{\"username\":\"XCASH\",\"most_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_total_rounds\":\"5\",\"best_block_verifier_online_percentage_delegate_name\":\"DELEGATE_NAME\",\"best_block_verifier_online_percentage\":\"10\",\"most_block_producer_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_block_producer_total_rounds\":\"15\"}"
  #define DATABASE_COLLECTION_DELEGATES_DATA "{\"public_address\":\"XCA\",\"total_vote_count\":\"XCA\",\"IP_address\":\"127.0.0.1\",\"delegate_name\":\"XCA\",\"about\":\"XCA\",\"website\":\"XCA\",\"team\":\"XCA\",\"pool_mode\":\"XCA\",\"fee_structure\":\"XCA\",\"server_settings\":\"XCA\",\"block_verifier_score\":\"XCA\",\"online_status\":\"XCA\",\"block_verifier_total_rounds\":\"XCA\",\"block_verifier_online_total_rounds\":\"XCA\",\"block_verifier_online_percentage\":\"XCA\",\"block_producer_total_rounds\":\"XCA\",\"block_producer_block_heights\":\"XCA\"}"
  #define DATA_HASH "1e3c10c2d5396ac7dcce22ad7488db8c6d9ba6ec07660283f5e2198cba6507c8a136536e8c4ee16f43cf04a19fa84b5575b34e3186426b2b6db3101e9e25abd9FAILED"

  // Variables
  char** data = (char**)calloc(DATA_COUNT * sizeof(char*),sizeof(char*));
  char** settings = (char**)calloc(DATA_COUNT * sizeof(char*),sizeof(char*));

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || settings == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (settings != NULL)
    {
      pointer_reset(settings);
    }
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // reset the variables
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  count_test = 0;

  // write the start test message
  color_print(TEST_OUTLINE,"blue");
  printf("\033[1;34mdatabase test - Total test: %d\033[0m\n",DATABASE_TEST);
  color_print(TEST_OUTLINE,"blue");
  printf("\n");

  // run the test

  // initialize the arrays
  for (count = 0; count < DATA_COUNT; count++)
  {
    data[count] = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
    settings[count] = (char*)calloc(BUFFER_SIZE,sizeof(char)); 

    if (data[count] == NULL || settings[count] == NULL)
    {
      color_print("Could not allocate the memory needed on the heap","red");
      exit(0);
    }
  }  

  // create the the arrays
  for (count = 0; count < DATA_COUNT; count++)
  {
    sprintf(data[count],"%d",count);
    sprintf(settings[count],"%d",count);
  }

  // insert a document into the collection using arrays
  if (insert_document_into_collection_array(DATABASE_NAME,DATABASE_COLLECTION,data,settings,DATA_COUNT) == 0)
  {
    color_print("FAILED! Test for inserting a document into a collection using an array","red");
  }
  else
  {
    color_print("PASSED! Test for inserting a document into a collection using an array","green");
    count_test++;
  }

  for (count = 0; count < DATA_COUNT; count++)
  {
    pointer_reset(data[count]);
    pointer_reset(settings[count]);
  }
  pointer_reset(data);
  pointer_reset(settings);
  
  // insert a document into the collection using json data
  if (insert_document_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,MESSAGE,0) == 0)
  {
    color_print("FAILED! Test for inserting a document into a collection using json data","red");
  }
  else
  {
    color_print("PASSED! Test for inserting a document into a collection using json data","green");
    count_test++;
  }  

  // update a document in the collection
  if (update_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,MESSAGE,MESSAGE_SETTINGS,0) == 0)
  {
    color_print("FAILED! Test for updating a document in a collection","red");
  }
  else
  {
    color_print("PASSED! Test for updating a document in a collection","green");
    count_test++;
  }

  // insert a document into the collection using json data on a separate thread
  struct insert_document_into_collection_json_thread_parameters insert_document_into_collection_json_thread_parameters = {DATABASE_NAME,DATABASE_COLLECTION,MESSAGE};
  pthread_create(&thread_id, NULL, &insert_document_into_collection_json_thread,(void *)&insert_document_into_collection_json_thread_parameters);
  if (thread_settings(thread_id) == 0)
  {
    color_print("FAILED! Test for inserting a document into a collection using json data on a separate thread","red");
  }
  else
  {
    color_print("PASSED! Test for inserting a document into a collection using json data on a separate thread","green");
    count_test++;
  }

  // update a document in the collection on a separate thread
  struct update_document_from_collection_thread_parameters update_document_from_collection_thread_parameters = {DATABASE_NAME,DATABASE_COLLECTION,MESSAGE,MESSAGE_SETTINGS};
  pthread_create(&thread_id, NULL, &update_document_from_collection_thread,(void *)&update_document_from_collection_thread_parameters);
  if (thread_settings(thread_id) == 0)
  {
    color_print("FAILED! Test for updating a document in a collection on a separate thread","red");
  }
  else
  {
    color_print("PASSED! Test for updating a document in a collection on a separate thread","green");
    count_test++;
  }

  // read a document in the collection
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (read_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,MESSAGE_SETTINGS,data_test,0) == 1)
  {
    if (strstr(data_test,"XCASH_PROOF_OF_STAKE_DATA") != NULL)
    {
      color_print("PASSED! Test for reading a document from a collection","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for reading a document from a collection","red");
    }    
  }
  else
  {
    color_print("FAILED! Test for reading a document from a collection","red");
  }

  // read a document in the collection on a separate thread
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  struct read_document_from_collection_thread_parameters read_document_from_collection_thread_parameters = {DATABASE_NAME,DATABASE_COLLECTION,MESSAGE_SETTINGS,data_test};
  pthread_create(&thread_id, NULL, &read_document_from_collection_thread,(void *)&read_document_from_collection_thread_parameters);
  if (thread_settings(thread_id) == 1)
  {
    if (strstr(data_test,"XCASH_PROOF_OF_STAKE_DATA") != NULL)
    {
      color_print("PASSED! Test for reading a document on a separate thread","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for reading a document on a separate thread","red");
    }    
  }
  else
  {
    color_print("FAILED! Test for reading a document on a separate thread","red");
  }

  // read a document in the collection and parse a field
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (read_document_field_from_collection(DATABASE_NAME,DATABASE_COLLECTION,MESSAGE_SETTINGS,"message_settings",data_test,0) == 1)
  {
    if (strncmp(data_test,"XCASH_PROOF_OF_STAKE_DATA",BUFFER_SIZE) == 0)
    {
      color_print("PASSED! Test for reading a document from a collection and parsing a field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for reading a document from a collection and parsing a field","red");
    }    
  }
  else
  {
    color_print("FAILED! Test for reading a document from a collection and parsing a field","red");
  }

  // read a document in the collection and parse a field on a separate thread
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  struct read_document_field_from_collection_thread_parameters read_document_field_from_collection_thread_parameters = {DATABASE_NAME,DATABASE_COLLECTION,MESSAGE_SETTINGS,"message_settings",data_test};
  pthread_create(&thread_id, NULL, &read_document_field_from_collection_thread,(void *)&read_document_field_from_collection_thread_parameters);
  if (thread_settings(thread_id) == 1)
  {
    if (strncmp(data_test,"XCASH_PROOF_OF_STAKE_DATA",BUFFER_SIZE) == 0)
    {
      color_print("PASSED! Test for reading a document from a collection and parsing a field on a separate thread","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for reading a document from a collection and parsing a field on a separate thread","red");
    }    
  }
  else
  {
    color_print("FAILED! Test for reading a document from a collection and parsing a field on a separate thread","red");
  }

   // read a document in the collection and parse all fields
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  delete_collection_from_database(DATABASE_NAME,DATABASE_COLLECTION,0);
  insert_document_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,DATABASE_COLLECTION_STATISTICS_DATA,0);
  struct database_document_fields database_data;

  // initialize the database_document_fields struct 
  for (count = 0; count < 7; count++)
  {
    database_data.item[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));
    database_data.value[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));

    if (database_data.item[count] == NULL || database_data.value[count] == NULL)
    {
      color_print("Could not allocate the memory needed on the heap","red");
      exit(0);
    }
  }
  database_data.count = 0;

  if (read_document_all_fields_from_collection(DATABASE_NAME,DATABASE_COLLECTION,"{\"username\":\"XCASH\"}",&database_data,0) == 1)
  {
    if (strncmp(database_data.item[0],"username",BUFFER_SIZE) == 0 && strncmp(database_data.value[0],"XCASH",BUFFER_SIZE) == 0 &&
        strncmp(database_data.item[1],"most_total_rounds_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_data.value[1],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_data.item[2],"most_total_rounds",BUFFER_SIZE) == 0 && strncmp(database_data.value[2],"5",BUFFER_SIZE) == 0 &&
        strncmp(database_data.item[3],"best_block_verifier_online_percentage_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_data.value[3],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_data.item[4],"best_block_verifier_online_percentage",BUFFER_SIZE) == 0 && strncmp(database_data.value[4],"10",BUFFER_SIZE) == 0 &&
        strncmp(database_data.item[5],"most_block_producer_total_rounds_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_data.value[5],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_data.item[6],"most_block_producer_total_rounds",BUFFER_SIZE) == 0 && strncmp(database_data.value[6],"15",BUFFER_SIZE) == 0)
    {
      color_print("PASSED! Test for reading a document from a collection and parsing all fields","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for reading a document from a collection and parsing all fields","red");
    }    
  }
  else
  {
    color_print("FAILED! Test for reading a document from a collection and parsing all fields","red");
  }

  for (count = 0; count < 7; count++)
  {
    pointer_reset(database_data.item[count]);
    pointer_reset(database_data.value[count]);
  }

  // read a document in the collection and parse all fields on a separate thread
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  delete_collection_from_database(DATABASE_NAME,DATABASE_COLLECTION,0);
  insert_document_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,DATABASE_COLLECTION_STATISTICS_DATA,0);
  
  // initialize the database_document_fields struct 
  for (count = 0; count < 7; count++)
  {
    database_data.item[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));
    database_data.value[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));

    if (database_data.item[count] == NULL || database_data.value[count] == NULL)
    {
      color_print("Could not allocate the memory needed on the heap","red");
      exit(0);
    }
  }
  database_data.count = 0;

  struct read_document_all_fields_from_collection_thread_parameters read_document_all_fields_from_collection_thread_parameters = {DATABASE_NAME,DATABASE_COLLECTION,"{\"username\":\"XCASH\"}",&database_data};
  pthread_create(&thread_id, NULL, &read_document_all_fields_from_collection_thread,(void *)&read_document_all_fields_from_collection_thread_parameters);
  if (thread_settings(thread_id) == 1)
  {
    if (strncmp(database_data.item[0],"username",BUFFER_SIZE) == 0 && strncmp(database_data.value[0],"XCASH",BUFFER_SIZE) == 0 &&
        strncmp(database_data.item[1],"most_total_rounds_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_data.value[1],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_data.item[2],"most_total_rounds",BUFFER_SIZE) == 0 && strncmp(database_data.value[2],"5",BUFFER_SIZE) == 0 &&
        strncmp(database_data.item[3],"best_block_verifier_online_percentage_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_data.value[3],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_data.item[4],"best_block_verifier_online_percentage",BUFFER_SIZE) == 0 && strncmp(database_data.value[4],"10",BUFFER_SIZE) == 0 &&
        strncmp(database_data.item[5],"most_block_producer_total_rounds_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_data.value[5],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_data.item[6],"most_block_producer_total_rounds",BUFFER_SIZE) == 0 && strncmp(database_data.value[6],"15",BUFFER_SIZE) == 0)
    {
      color_print("PASSED! Test for reading a document from a collection and parsing all fields on a separate thread","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for reading a document from a collection and parsing all fields on a separate thread","red");
    }    
  }
  else
  {
    color_print("FAILED! Test for reading a document from a collection and parsing all fields on a separate thread","red");
  }
  

  for (count = 0; count < 7; count++)
  {
    pointer_reset(database_data.item[count]);
    pointer_reset(database_data.value[count]);
  }
  
  // read multiple documents in the collection and parse all fields
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  delete_collection_from_database(DATABASE_NAME,DATABASE_COLLECTION,0);
  insert_document_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,DATABASE_COLLECTION_STATISTICS_DATA,0);
  insert_document_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,DATABASE_COLLECTION_STATISTICS_DATA,0);
  struct database_multiple_documents_fields database_multiple_documents_fields;

  // initialize the database_multiple_documents_fields struct 
  for (count = 0; count < 2; count++)
  {
    for (counter = 0; counter < 7; counter++)
    {
      database_multiple_documents_fields.item[count][counter] = (char*)calloc(BUFFER_SIZE,sizeof(char));
      database_multiple_documents_fields.value[count][counter] = (char*)calloc(BUFFER_SIZE,sizeof(char));

      if (database_multiple_documents_fields.item[count][counter] == NULL || database_multiple_documents_fields.value[count][counter] == NULL)
      {
        color_print("Could not allocate the memory needed on the heap","red");
        exit(0);
      }
    }
  }
  database_multiple_documents_fields.document_count = 0;
  database_multiple_documents_fields.database_fields_count = 0;

  if (read_multiple_documents_all_fields_from_collection(DATABASE_NAME,DATABASE_COLLECTION,"",&database_multiple_documents_fields,1,2,0,"",0) == 1)
  {
    if (strncmp(database_multiple_documents_fields.item[0][0],"username",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][0],"XCASH",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][1],"most_total_rounds_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][1],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][2],"most_total_rounds",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][2],"5",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][3],"best_block_verifier_online_percentage_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][3],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][4],"best_block_verifier_online_percentage",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][4],"10",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][5],"most_block_producer_total_rounds_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][5],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][6],"most_block_producer_total_rounds",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][6],"15",BUFFER_SIZE) == 0 &&        
        strncmp(database_multiple_documents_fields.item[1][0],"username",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][0],"XCASH",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][1],"most_total_rounds_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][1],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][2],"most_total_rounds",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][2],"5",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][3],"best_block_verifier_online_percentage_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][3],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][4],"best_block_verifier_online_percentage",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][4],"10",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][5],"most_block_producer_total_rounds_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][5],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][6],"most_block_producer_total_rounds",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][6],"15",BUFFER_SIZE) == 0)
    {
      color_print("PASSED! Test for reading multiple documents from a collection and parsing all fields","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for reading multiple documents from a collection and parsing all fields","red");
    }    
  }
  else
  {
    color_print("FAILED! Test for reading multiple documents from a collection and parsing all fields","red");
  }

  // read multiple documents in the collection using the sort document option and parse all fields
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  delete_collection_from_database(DATABASE_NAME,DATABASE_COLLECTION,0);
  insert_document_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,DATABASE_COLLECTION_STATISTICS_DATA,0);
  insert_document_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,DATABASE_COLLECTION_STATISTICS_DATA,0);

  // reset the database_multiple_documents_fields struct 
  for (count = 0; count < 2; count++)
  {
    for (counter = 0; counter < 7; counter++)
    {
      memset(database_multiple_documents_fields.item[count][counter],0,strlen(database_multiple_documents_fields.item[count][counter]));
      memset(database_multiple_documents_fields.value[count][counter],0,strlen(database_multiple_documents_fields.value[count][counter]));
    }
  }
  database_multiple_documents_fields.document_count = 0;
  database_multiple_documents_fields.database_fields_count = 0;

  if (read_multiple_documents_all_fields_from_collection(DATABASE_NAME,DATABASE_COLLECTION,"",&database_multiple_documents_fields,1,2,1,"data",0) == 1)
  {
     if (strncmp(database_multiple_documents_fields.item[0][0],"username",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][0],"XCASH",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][1],"most_total_rounds_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][1],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][2],"most_total_rounds",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][2],"5",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][3],"best_block_verifier_online_percentage_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][3],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][4],"best_block_verifier_online_percentage",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][4],"10",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][5],"most_block_producer_total_rounds_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][5],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][6],"most_block_producer_total_rounds",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][6],"15",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][0],"username",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][0],"XCASH",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][1],"most_total_rounds_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][1],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][2],"most_total_rounds",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][2],"5",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][3],"best_block_verifier_online_percentage_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][3],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][4],"best_block_verifier_online_percentage",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][4],"10",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][5],"most_block_producer_total_rounds_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][5],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][6],"most_block_producer_total_rounds",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][6],"15",BUFFER_SIZE) == 0)
    {
      color_print("PASSED! Test for reading multiple documents from a collection using the sort document option and parsing all fields","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for reading multiple documents from a collection using the sort document option and parsing all fields","red");
    }    
  }
  else
  {
    color_print("FAILED! Test for reading multiple documents from a collection using the sort document option and parsing all fields","red");
  }

  for (count = 0; count < 2; count++)
  {
    for (counter = 0; counter < 7; counter++)
    {
      pointer_reset(database_multiple_documents_fields.item[count][counter]);
      pointer_reset(database_multiple_documents_fields.value[count][counter]);
    }
  }

  // read multiple documents in the collection and parse all fields on a separate thread
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  delete_collection_from_database(DATABASE_NAME,DATABASE_COLLECTION,0);
  insert_document_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,DATABASE_COLLECTION_STATISTICS_DATA,0);
  insert_document_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,DATABASE_COLLECTION_STATISTICS_DATA,0);

  // initialize the database_multiple_documents_fields struct 
  for (count = 0; count < 2; count++)
  {
    for (counter = 0; counter < 7; counter++)
    {
      database_multiple_documents_fields.item[count][counter] = (char*)calloc(BUFFER_SIZE,sizeof(char));
      database_multiple_documents_fields.value[count][counter] = (char*)calloc(BUFFER_SIZE,sizeof(char));

      if (database_multiple_documents_fields.item[count][counter] == NULL || database_multiple_documents_fields.value[count][counter] == NULL)
      {
        color_print("Could not allocate the memory needed on the heap","red");
        exit(0);
      }
    }
  }
  database_multiple_documents_fields.document_count = 0;
  database_multiple_documents_fields.database_fields_count = 0;

  struct read_multiple_documents_all_fields_from_collection_thread_parameters read_multiple_documents_all_fields_from_collection_thread_parameters = {DATABASE_NAME,DATABASE_COLLECTION,"",&database_multiple_documents_fields,1,2,0,""};
  pthread_create(&thread_id, NULL, &read_multiple_documents_all_fields_from_collection_thread,(void *)&read_multiple_documents_all_fields_from_collection_thread_parameters);
  if (thread_settings(thread_id) == 1)
  { 
     if (strncmp(database_multiple_documents_fields.item[0][0],"username",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][0],"XCASH",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][1],"most_total_rounds_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][1],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][2],"most_total_rounds",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][2],"5",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][3],"best_block_verifier_online_percentage_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][3],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][4],"best_block_verifier_online_percentage",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][4],"10",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][5],"most_block_producer_total_rounds_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][5],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][6],"most_block_producer_total_rounds",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][6],"15",BUFFER_SIZE) == 0 &&        
        strncmp(database_multiple_documents_fields.item[1][0],"username",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][0],"XCASH",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][1],"most_total_rounds_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][1],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][2],"most_total_rounds",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][2],"5",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][3],"best_block_verifier_online_percentage_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][3],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][4],"best_block_verifier_online_percentage",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][4],"10",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][5],"most_block_producer_total_rounds_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][5],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][6],"most_block_producer_total_rounds",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][6],"15",BUFFER_SIZE) == 0)
    {
      color_print("PASSED! Test for reading multiple documents from a collection and parsing all fields on a separate thread","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for reading multiple documents from a collection and parsing all fields on a separate thread","red");
    } 
  }
  else
  {
    color_print("FAILED! Test for reading multiple documents from a collection and parsing all fields on a separate thread","red");
  }
  
  for (count = 0; count < 2; count++)
  {
    for (counter = 0; counter < 7; counter++)
    {
      pointer_reset(database_multiple_documents_fields.item[count][counter]);
      pointer_reset(database_multiple_documents_fields.value[count][counter]);
    }
  }

  // update all document in the collection
  if (update_all_documents_from_collection(DATABASE_NAME,DATABASE_COLLECTION,MESSAGE,0) == 0)
  {
    color_print("FAILED! Test for updating all documents in a collection","red");
  }
  else
  {
    color_print("PASSED! Test for updating all documents in a collection","green");
    count_test++;
  }

  // update all document in the collection on a separate thread
  struct update_all_documents_from_collection_thread_parameters update_all_documents_from_collection_thread_parameters = {DATABASE_NAME,DATABASE_COLLECTION,MESSAGE};
  pthread_create(&thread_id, NULL, &update_all_documents_from_collection_thread,(void *)&update_all_documents_from_collection_thread_parameters);
  if (thread_settings(thread_id) == 0)
  {
    color_print("FAILED! Test for updating all documents in a collection on a separate thread","red");
  }
  else
  {
    color_print("PASSED! Test for updating all documents in a collection on a separate thread","green");
    count_test++;
  }

  // count how many documents have "message_settings":"XCASH_PROOF_OF_STAKE_TEST_DATA" in the collection
  int count1 = count_documents_in_collection(DATABASE_NAME,DATABASE_COLLECTION,MESSAGE,0);
  int count2 = count_all_documents_in_collection(DATABASE_NAME,DATABASE_COLLECTION,0);
  if ((count1 == count2) && (count1 != -1 || count2 != -1))
  {
    color_print("PASSED! Test for counting documents in a collection that match a specific field name and field","green");
    color_print("PASSED! Test for counting all documents in a collection","green");
    count_test+= 2;   
  }
  else
  {
     color_print("FAILED! Test for counting documents in a collection that match a specific field name and field","red");
     color_print("FAILED! Test for counting all documents in a collection","red");
  }

  // count how many documents have "message_settings":"XCASH_PROOF_OF_STAKE_TEST_DATA" in the collection on a separate thread
  struct count_documents_in_collection_thread_parameters count_documents_in_collection_thread_parameters = {DATABASE_NAME,DATABASE_COLLECTION,MESSAGE};
  pthread_create(&thread_id, NULL, &count_documents_in_collection_thread,(void *)&count_documents_in_collection_thread_parameters);
  count1 = thread_settings(thread_id);
  struct count_all_documents_in_collection_thread_parameters count_all_documents_in_collection_thread_parameters = {DATABASE_NAME,DATABASE_COLLECTION};
  pthread_create(&thread_id, NULL, &count_all_documents_in_collection_thread,(void *)&count_all_documents_in_collection_thread_parameters);
  count2 = thread_settings(thread_id);
  if ((count1 == count2) && (count1 != -1 || count2 != -1))
  {
    color_print("PASSED! Test for counting documents in a collection that match a specific field name and field on a separate thread","green");
    color_print("PASSED! Test for counting all documents in a collection on a separate thread","green");
    count_test+= 2;   
  }
  else
  {
     color_print("FAILED! Test for counting documents in a collection that match a specific field name and field on a separate thread","red");
     color_print("FAILED! Test for counting all documents in a collection on a separate thread","red");
  }

  // delete a document from the collection
  if (delete_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,MESSAGE,0) == 0)
  {
    color_print("FAILED! Test for deleting a document from a collection","red");
  }
  else
  {
    color_print("PASSED! Test for deleting a document from a collection","green");
    count_test++;
  }

  // delete a document from the collection on a separate thread
  if (insert_document_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,MESSAGE,0) == 0)
  {
    color_print("FAILED! Test for deleting a document from a collection on a separate thread","red");
  }
  else
  {
    struct delete_document_from_collection_thread_parameters delete_document_from_collection_thread_parameters = {DATABASE_NAME,DATABASE_COLLECTION,MESSAGE};
    pthread_create(&thread_id, NULL, &delete_document_from_collection_thread,(void *)&delete_document_from_collection_thread_parameters);
    if (thread_settings(thread_id) == 0)
    {
      color_print("FAILED! Test for deleting a document from a collection on a separate thread","red");
    }
    else
    {
      color_print("PASSED! Test for deleting a document from a collection on a separate thread","green");
      count_test++;
    }
  }  

  // delete a collection from the database
  if (delete_collection_from_database(DATABASE_NAME,DATABASE_COLLECTION,0) == 0)
  {
    color_print("FAILED! Test for deleting a collection from a database","red");
  }
  else
  {
    color_print("PASSED! Test for deleting a collection from a database","green");
    count_test++;
  }

  // delete a collection from the database on a separate thread
  if (insert_document_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,MESSAGE,0) == 0)
  {
    color_print("FAILED! Test for deleting a collection from a database on a separate thread","red");
  }
  else
  {
    struct delete_collection_from_database_thread_parameters delete_collection_from_database_thread_parameters = {DATABASE_NAME,DATABASE_COLLECTION};
    pthread_create(&thread_id, NULL, &delete_collection_from_database_thread,(void *)&delete_collection_from_database_thread_parameters);
    if (thread_settings(thread_id) == 0)
    {
      color_print("FAILED! Test for deleting a collection from a database on a separate thread","red");
    }
    else
    {
      color_print("PASSED! Test for deleting a collection from a database on a separate thread","green");
      count_test++;
    }  
  }

  insert_document_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,DATABASE_COLLECTION_DELEGATES_DATA,0);

  // get the database data hash
  delete_collection_from_database(DATABASE_NAME,DATABASE_COLLECTION,0);
  insert_document_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,DATABASE_COLLECTION_STATISTICS_DATA,0);
  insert_document_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,DATABASE_COLLECTION_STATISTICS_DATA,0);
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (get_database_data_hash(data_test,DATABASE_NAME,DATABASE_COLLECTION,0) == 1 && memcmp(data_test,DATA_HASH,DATA_HASH_LENGTH) == 0)
  {
    color_print("PASSED! Test for getting the database data hash","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for getting the database data hash","red");
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  delete_collection_from_database(DATABASE_NAME,DATABASE_COLLECTION,0);

  // get the database data hash on a separate thread
  delete_collection_from_database(DATABASE_NAME,DATABASE_COLLECTION,0);
  insert_document_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,DATABASE_COLLECTION_STATISTICS_DATA,0);
  insert_document_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,DATABASE_COLLECTION_STATISTICS_DATA,0);
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  struct get_database_data_hash_thread_parameters get_database_data_hash_thread_parameters = {data_test,DATABASE_NAME,DATABASE_COLLECTION};
  pthread_create(&thread_id, NULL, &get_database_data_hash_thread,(void *)&get_database_data_hash_thread_parameters);
  if (thread_settings(thread_id) == 1 && memcmp(data_test,DATA_HASH,DATA_HASH_LENGTH) == 0)
  {
    color_print("PASSED! Test for getting the database data hash on a separate thread","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for getting the database data hash on a separate thread","red");
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  delete_collection_from_database(DATABASE_NAME,DATABASE_COLLECTION,0);

  // insert multiple documents into the collection using json data
  delete_collection_from_database(DATABASE_NAME,DATABASE_COLLECTION,0);
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(data_test,DATABASE_COLLECTION_STATISTICS_DATA,strnlen(DATABASE_COLLECTION_STATISTICS_DATA,BUFFER_SIZE));
  memcpy(data_test+strlen(data_test),",",1);
  memcpy(data_test+strlen(data_test),DATABASE_COLLECTION_STATISTICS_DATA,strnlen(DATABASE_COLLECTION_STATISTICS_DATA,BUFFER_SIZE));
  if (insert_multiple_documents_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,data_test,0) == 1 && get_database_data_hash(data_test,DATABASE_NAME,DATABASE_COLLECTION,0) == 1 && memcmp(data_test,DATA_HASH,DATA_HASH_LENGTH) == 0)
  {
    color_print("PASSED! Test for inserting multiple documents into a collection using json data","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for inserting multiple documents into a collection using json data","red");    
  }

  // insert multiple documents into the collection using json data on a separate thread
  delete_collection_from_database(DATABASE_NAME,DATABASE_COLLECTION,0);
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(data_test,DATABASE_COLLECTION_STATISTICS_DATA,strnlen(DATABASE_COLLECTION_STATISTICS_DATA,BUFFER_SIZE));
  memcpy(data_test+strlen(data_test),",",1);
  memcpy(data_test+strlen(data_test),DATABASE_COLLECTION_STATISTICS_DATA,strnlen(DATABASE_COLLECTION_STATISTICS_DATA,BUFFER_SIZE));
  struct insert_multiple_documents_into_collection_json_thread_parameters insert_multiple_documents_into_collection_json_thread_parameters = {DATABASE_NAME,DATABASE_COLLECTION,data_test};
  pthread_create(&thread_id, NULL, &insert_multiple_documents_into_collection_json_thread,(void *)&insert_multiple_documents_into_collection_json_thread_parameters);
  if (thread_settings(thread_id) == 1 && get_database_data_hash(data_test,DATABASE_NAME,DATABASE_COLLECTION,0) == 1 && memcmp(data_test,DATA_HASH,DATA_HASH_LENGTH) == 0)
  {
    color_print("PASSED! Test for inserting multiple documents into a collection using json data on a separate thread","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for inserting multiple documents into a collection using json data on a separate thread","red");    
  }

  // get the database data
  memset(result_test,0,strlen(result_test));
  if (get_database_data(result_test,DATABASE_NAME,DATABASE_COLLECTION,0) == 1 && strncmp("{\"username\" : \"XCASH\", \"most_total_rounds_delegate_name\" : \"DELEGATE_NAME\", \"most_total_rounds\" : \"5\", \"best_block_verifier_online_percentage_delegate_name\" : \"DELEGATE_NAME\", \"best_block_verifier_online_percentage\" : \"10\", \"most_block_producer_total_rounds_delegate_name\" : \"DELEGATE_NAME\", \"most_block_producer_total_rounds\" : \"15\" },{\"username\" : \"XCASH\", \"most_total_rounds_delegate_name\" : \"DELEGATE_NAME\", \"most_total_rounds\" : \"5\", \"best_block_verifier_online_percentage_delegate_name\" : \"DELEGATE_NAME\", \"best_block_verifier_online_percentage\" : \"10\", \"most_block_producer_total_rounds_delegate_name\" : \"DELEGATE_NAME\", \"most_block_producer_total_rounds\" : \"15\" }",result_test,BUFFER_SIZE) == 0)
  {
    color_print("PASSED! Test for getting the database data","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for getting the database data","red");
  }

  // get the database data on a separate thread
  memset(result_test,0,strlen(result_test));
  struct get_database_data_thread_parameters get_database_data_thread_parameters = {result_test,DATABASE_NAME,DATABASE_COLLECTION};
  pthread_create(&thread_id, NULL, &get_database_data_thread,(void *)&get_database_data_thread_parameters);
  if (thread_settings(thread_id) == 1 && strncmp("{\"username\" : \"XCASH\", \"most_total_rounds_delegate_name\" : \"DELEGATE_NAME\", \"most_total_rounds\" : \"5\", \"best_block_verifier_online_percentage_delegate_name\" : \"DELEGATE_NAME\", \"best_block_verifier_online_percentage\" : \"10\", \"most_block_producer_total_rounds_delegate_name\" : \"DELEGATE_NAME\", \"most_block_producer_total_rounds\" : \"15\" },{\"username\" : \"XCASH\", \"most_total_rounds_delegate_name\" : \"DELEGATE_NAME\", \"most_total_rounds\" : \"5\", \"best_block_verifier_online_percentage_delegate_name\" : \"DELEGATE_NAME\", \"best_block_verifier_online_percentage\" : \"10\", \"most_block_producer_total_rounds_delegate_name\" : \"DELEGATE_NAME\", \"most_block_producer_total_rounds\" : \"15\" }",result_test,BUFFER_SIZE) == 0)
  {
    color_print("PASSED! Test for getting the database data on a separate thread","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for getting the database data on a separate thread","red");
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  delete_collection_from_database(DATABASE_NAME,DATABASE_COLLECTION,0);
  
  // write the end test message
  if (count_test == DATABASE_TEST)
  {
    printf("\n");
    color_print(TEST_OUTLINE,"green");
    printf("\033[1;32mdatabase test - Passed test: %d, Failed test: 0\033[0m\n",DATABASE_TEST);
    color_print(TEST_OUTLINE,"green");
    printf("\n\n");
  }
  else
  {
    printf("\n");
    color_print(TEST_OUTLINE,"red");
    printf("\033[1;31mdatabase test - Passed test: %d, Failed test: %d\033[0m\n",count_test,DATABASE_TEST-count_test);
    color_print(TEST_OUTLINE,"red");
    printf("\n\n");
  }   
  return count_test;

  #undef DATABASE_TEST  
  #undef DATA_COUNT
  #undef DATABASE_COLLECTION
  #undef MESSAGE
  #undef MESSAGE_SETTINGS
  #undef DATABASE_COLLECTION_STATISTICS_DATA
}