#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"
#include "initialize_and_reset_structs_define_macros.h"

#include "read_database_functions.h"
#include "count_database_functions.h"
#include "organize_functions.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: organize_invalid_reserve_proofs_settings
Description: organize invalid reserve proofs
-----------------------------------------------------------------------------------------------------------
*/

int organize_invalid_reserve_proofs_settings(const void* STRING1, const void* STRING2)
{
  return (strcmp((const char*)STRING1, (const char*)STRING2));
}



/*
-----------------------------------------------------------------------------------------------------------
Name: organize_delegates_settings
Description: organize delegates
-----------------------------------------------------------------------------------------------------------
*/

int organize_delegates_settings(const void* DELEGATES1, const void* DELEGATES2)
{
  // Variables
  long long int count;
  long long int count2;
  long long int settings;
  const struct delegates* delegates1 = (const struct delegates*)DELEGATES1;
  const struct delegates* delegates2 = (const struct delegates*)DELEGATES2;

  // make sure the network data nodes are always the first delegates
  if (production_settings == 1 && (memcmp(delegates1->public_address,NETWORK_DATA_NODE_1_PUBLIC_ADDRESS_PRODUCTION,XCASH_WALLET_LENGTH) == 0 || memcmp(delegates1->public_address,NETWORK_DATA_NODE_2_PUBLIC_ADDRESS_PRODUCTION,XCASH_WALLET_LENGTH) == 0 || memcmp(delegates1->public_address,NETWORK_DATA_NODE_3_PUBLIC_ADDRESS_PRODUCTION,XCASH_WALLET_LENGTH) == 0 || memcmp(delegates1->public_address,NETWORK_DATA_NODE_4_PUBLIC_ADDRESS_PRODUCTION,XCASH_WALLET_LENGTH) == 0 || memcmp(delegates1->public_address,NETWORK_DATA_NODE_5_PUBLIC_ADDRESS_PRODUCTION,XCASH_WALLET_LENGTH) == 0))
  {
    return -1;
  }
  if (production_settings == 0 && (memcmp(delegates1->public_address,NETWORK_DATA_NODE_1_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0 || memcmp(delegates1->public_address,NETWORK_DATA_NODE_2_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0 || memcmp(delegates1->public_address,NETWORK_DATA_NODE_3_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0 || memcmp(delegates1->public_address,NETWORK_DATA_NODE_4_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0 || memcmp(delegates1->public_address,NETWORK_DATA_NODE_5_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0))
  {
    return -1;
  }

  if ((settings = strcmp(delegates2->online_status,delegates1->online_status)) == 0)
  {
    sscanf(delegates1->total_vote_count, "%lld", &count);
    sscanf(delegates2->total_vote_count, "%lld", &count2);
    return count2 == count ? 0 : count2 - count < 0 ? -1 : 1;
  }
  return settings;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: organize_delegates
Description: Organize the delegates in descending order of total_vote_count
Parameters:
  struct delegates - struct delegates
  DATABASE_COLLECTION - The database collection to read the delegates from
Return: 0 if an error has occured, otherwise the amount of delegates in the struct delegates
-----------------------------------------------------------------------------------------------------------
*/

int organize_delegates(struct delegates* delegates, const char* DATABASE_COLLECTION)
{
  // Variables
  char data[4096];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time; 
  int count = 0;
  int count2 = 0;
  struct database_multiple_documents_fields database_multiple_documents_fields;
  int document_count = 0;

  // define macros
  #define ORGANIZE_DELEGATES_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"organize_delegates",18); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  if (network_functions_test_error_settings != 2) \
  { \
    print_error_message(current_date_and_time,current_UTC_date_and_time,data); \
  } \
  POINTER_RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,count2,TOTAL_DELEGATES_DATABASE_FIELDS+1); \
  return 0;

  memset(data,0,sizeof(data));

  // get how many documents are in the database
  document_count = count_all_documents_in_collection(database_name,DATABASE_COLLECTION,1);

  // initialize the database_multiple_documents_fields struct 
  for (count = 0; count < document_count; count++)
  {
    for (count2 = 0; count2 < TOTAL_DELEGATES_DATABASE_FIELDS+1; count2++)
    {
       // allocate more for the about and the block_producer_block_heights
       if (count2+2 == TOTAL_DELEGATES_DATABASE_FIELDS)
       {
         database_multiple_documents_fields.item[count][count2] = (char*)calloc(100,sizeof(char));
         database_multiple_documents_fields.value[count][count2] = (char*)calloc(50000,sizeof(char));
       }
       else if (count2 == 4)
       {
         database_multiple_documents_fields.item[count][count2] = (char*)calloc(100,sizeof(char));
         database_multiple_documents_fields.value[count][count2] = (char*)calloc(1025,sizeof(char));
       }
       else
       {
         database_multiple_documents_fields.item[count][count2] = (char*)calloc(100,sizeof(char));
         database_multiple_documents_fields.value[count][count2] = (char*)calloc(BUFFER_SIZE_NETWORK_BLOCK_DATA,sizeof(char));
       }

       if (database_multiple_documents_fields.item[count][count2] == NULL || database_multiple_documents_fields.value[count][count2] == NULL)
       {
         memcpy(error_message.function[error_message.total],"update_block_verifiers_list",27);
         memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
         error_message.total++;
         print_error_message(current_date_and_time,current_UTC_date_and_time,data);  
         exit(0);
       }
     }      
   } 
  database_multiple_documents_fields.document_count = 0;
  database_multiple_documents_fields.database_fields_count = TOTAL_DELEGATES_DATABASE_FIELDS;

  // get all of the delegates  
  if (read_multiple_documents_all_fields_from_collection(database_name,DATABASE_COLLECTION,"",&database_multiple_documents_fields,1,document_count,0,"",1) == 0)
  {
    ORGANIZE_DELEGATES_ERROR("Could not get the delegates from the database");
  }

  // convert the database_multiple_documents_fields to delegates struct
  for (count = 0; count < (int)database_multiple_documents_fields.document_count; count++)
  {
    memcpy(delegates[count].public_address,database_multiple_documents_fields.value[count][0],strnlen(database_multiple_documents_fields.value[count][0],BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memcpy(delegates[count].total_vote_count,database_multiple_documents_fields.value[count][1],strnlen(database_multiple_documents_fields.value[count][1],BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memcpy(delegates[count].IP_address,database_multiple_documents_fields.value[count][2],strnlen(database_multiple_documents_fields.value[count][2],BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memcpy(delegates[count].delegate_name,database_multiple_documents_fields.value[count][3],strnlen(database_multiple_documents_fields.value[count][3],BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memcpy(delegates[count].about,database_multiple_documents_fields.value[count][4],strnlen(database_multiple_documents_fields.value[count][4],1025));
    memcpy(delegates[count].website,database_multiple_documents_fields.value[count][5],strnlen(database_multiple_documents_fields.value[count][5],BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memcpy(delegates[count].team,database_multiple_documents_fields.value[count][6],strnlen(database_multiple_documents_fields.value[count][6],BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memcpy(delegates[count].pool_mode,database_multiple_documents_fields.value[count][7],strnlen(database_multiple_documents_fields.value[count][7],BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memcpy(delegates[count].fee_structure,database_multiple_documents_fields.value[count][8],strnlen(database_multiple_documents_fields.value[count][8],BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memcpy(delegates[count].server_settings,database_multiple_documents_fields.value[count][9],strnlen(database_multiple_documents_fields.value[count][9],BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memcpy(delegates[count].block_verifier_score,database_multiple_documents_fields.value[count][10],strnlen(database_multiple_documents_fields.value[count][10],BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memcpy(delegates[count].online_status,database_multiple_documents_fields.value[count][11],strnlen(database_multiple_documents_fields.value[count][11],BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memcpy(delegates[count].block_verifier_total_rounds,database_multiple_documents_fields.value[count][12],strnlen(database_multiple_documents_fields.value[count][12],BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memcpy(delegates[count].block_verifier_online_total_rounds,database_multiple_documents_fields.value[count][13],strnlen(database_multiple_documents_fields.value[count][13],BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memcpy(delegates[count].block_verifier_online_percentage,database_multiple_documents_fields.value[count][14],strnlen(database_multiple_documents_fields.value[count][14],BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memcpy(delegates[count].block_producer_total_rounds,database_multiple_documents_fields.value[count][15],strnlen(database_multiple_documents_fields.value[count][15],BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memcpy(delegates[count].block_producer_block_heights,database_multiple_documents_fields.value[count][16],strnlen(database_multiple_documents_fields.value[count][16],50000));
    memcpy(delegates[count].public_key,database_multiple_documents_fields.value[count][17],VRF_PUBLIC_KEY_LENGTH);
  }
  
  // organize the delegates by total_vote_count
  qsort(delegates,database_multiple_documents_fields.document_count,sizeof(struct delegates),organize_delegates_settings);

  POINTER_RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,count2,TOTAL_DELEGATES_DATABASE_FIELDS+1);

  return database_multiple_documents_fields.document_count;
  
  #undef ORGANIZE_DELEGATES_ERROR
}