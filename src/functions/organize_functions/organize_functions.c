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
  return strcmp(*(char* const*)STRING2, *(char* const*)STRING1);
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
  int settings;
  const struct delegates* delegates1 = (const struct delegates*)DELEGATES1;
  const struct delegates* delegates2 = (const struct delegates*)DELEGATES2;

  /*
  First sort by if the delegate are online or offline
  Next sort by how many total votes the delegate have
  Then sort by the public address

  This way it will be a correct order with no two delegates being exactly the same
  */

  // check the delegates online status
  if ((settings = strcmp(delegates2->online_status,delegates1->online_status)) != 0)
  {
    return settings < 0 ? -1 : 1;
  }

  // the delegates have the same online status so check the delegates total vote count
  sscanf(delegates1->total_vote_count, "%lld", &count);
  sscanf(delegates2->total_vote_count, "%lld", &count2);

  if (count != count2)
  {
    return count2 - count < 0 ? -1 : 1;
  }

  // the delegates have the same total vote count so sort the delegate by public address
  return strcmp(delegates1->public_address,delegates2->public_address);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: organize_delegates
Description: Organize the delegates in descending order of total_vote_count
Parameters:
  struct delegates - struct delegates
Return: 0 if an error has occured, otherwise the amount of delegates in the struct delegates
-----------------------------------------------------------------------------------------------------------
*/

int organize_delegates(struct delegates* delegates)
{
  // Variables
  char data[4096];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time; 
  int count = 0;
  int count2 = 0;
  struct database_multiple_documents_fields database_multiple_documents_fields;
  struct delegates delegates_copy[MAXIMUM_AMOUNT_OF_DELEGATES];
  int document_count = 0;

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define ORGANIZE_DELEGATES_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"organize_delegates",18); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  if (network_functions_test_error_settings != 2) \
  { \
    print_error_message(current_date_and_time,current_UTC_date_and_time,data); \
  } \
  RESET_DELEGATES; \
  POINTER_RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,count2,TOTAL_DELEGATES_DATABASE_FIELDS+1); \
  return 0;

  #define RESET_DELEGATES \
  for (count = 0; (int)count < MAXIMUM_AMOUNT_OF_DELEGATES; count++) \
  { \
    pointer_reset(delegates_copy[count].public_address); \
    pointer_reset(delegates_copy[count].total_vote_count); \
    pointer_reset(delegates_copy[count].IP_address); \
    pointer_reset(delegates_copy[count].delegate_name); \
    pointer_reset(delegates_copy[count].about); \
    pointer_reset(delegates_copy[count].website); \
    pointer_reset(delegates_copy[count].team); \
    pointer_reset(delegates_copy[count].shared_delegate_status); \
    pointer_reset(delegates_copy[count].fee_structure); \
    pointer_reset(delegates_copy[count].server_settings); \
    pointer_reset(delegates_copy[count].block_verifier_score); \
    pointer_reset(delegates_copy[count].online_status); \
    pointer_reset(delegates_copy[count].block_verifier_total_rounds); \
    pointer_reset(delegates_copy[count].block_verifier_online_total_rounds); \
    pointer_reset(delegates_copy[count].block_verifier_online_percentage); \
    pointer_reset(delegates_copy[count].block_producer_total_rounds); \
    pointer_reset(delegates_copy[count].block_producer_block_heights); \
    pointer_reset(delegates_copy[count].public_key); \
 }

  memset(data,0,sizeof(data));

  // initialize the delegates struct
  for (count = 0; (int)count < MAXIMUM_AMOUNT_OF_DELEGATES; count++)
  {
    delegates_copy[count].public_address = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    delegates_copy[count].total_vote_count = (char*)calloc(100,sizeof(char));
    delegates_copy[count].IP_address = (char*)calloc(256,sizeof(char));
    delegates_copy[count].delegate_name = (char*)calloc(MAXIMUM_BUFFER_SIZE_DELEGATES_NAME+1,sizeof(char));
    delegates_copy[count].about = (char*)calloc(1025,sizeof(char));
    delegates_copy[count].website = (char*)calloc(256,sizeof(char));
    delegates_copy[count].team = (char*)calloc(256,sizeof(char));
    delegates_copy[count].shared_delegate_status = (char*)calloc(10,sizeof(char));
    delegates_copy[count].fee_structure = (char*)calloc(11,sizeof(char));
    delegates_copy[count].server_settings = (char*)calloc(1025,sizeof(char));
    delegates_copy[count].block_verifier_score = (char*)calloc(10,sizeof(char));
    delegates_copy[count].online_status = (char*)calloc(10,sizeof(char));
    delegates_copy[count].block_verifier_total_rounds = (char*)calloc(10,sizeof(char));
    delegates_copy[count].block_verifier_online_total_rounds = (char*)calloc(10,sizeof(char));
    delegates_copy[count].block_verifier_online_percentage = (char*)calloc(10,sizeof(char));
    delegates_copy[count].block_producer_total_rounds = (char*)calloc(10,sizeof(char));
    delegates_copy[count].block_producer_block_heights = (char*)calloc(100000,sizeof(char));
    delegates_copy[count].public_key = (char*)calloc(VRF_PUBLIC_KEY_LENGTH+1,sizeof(char));
    if (delegates_copy[count].public_address == NULL || delegates_copy[count].total_vote_count == NULL || delegates_copy[count].IP_address == NULL || delegates_copy[count].delegate_name == NULL || delegates_copy[count].about == NULL || delegates_copy[count].website == NULL || delegates_copy[count].team == NULL || delegates_copy[count].shared_delegate_status == NULL || delegates_copy[count].fee_structure == NULL || delegates_copy[count].server_settings == NULL || delegates_copy[count].block_verifier_score == NULL || delegates_copy[count].online_status == NULL || delegates_copy[count].block_verifier_total_rounds == NULL || delegates_copy[count].block_verifier_online_total_rounds == NULL || delegates_copy[count].block_verifier_online_percentage == NULL || delegates_copy[count].block_producer_total_rounds == NULL || delegates_copy[count].block_producer_block_heights == NULL || delegates_copy[count].public_key == NULL)
    {
      memcpy(error_message.function[error_message.total],"organize_delegates",18);
      memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
      error_message.total++;
      print_error_message(current_date_and_time,current_UTC_date_and_time,data);
      exit(0);
    }
  }

  // get how many documents are in the database
  if ((document_count = count_all_documents_in_collection(database_name,DATABASE_COLLECTION)) <= 0)
  {
    memcpy(error_message.function[error_message.total],"organize_delegates",18);
    memcpy(error_message.data[error_message.total],"Could not count how many delegates are in the database",54);
    error_message.total++;
    if (network_functions_test_error_settings != 2)
    {
      print_error_message(current_date_and_time,current_UTC_date_and_time,data);
    }
    RESET_DELEGATES;
    return 0;
  }

  // initialize the database_multiple_documents_fields struct 
  for (count = 0; count < document_count; count++)
  {
    for (count2 = 0; count2 < TOTAL_DELEGATES_DATABASE_FIELDS+1; count2++)
    {
       // allocate more for the about and the block_producer_block_heights
       if (count2+2 == TOTAL_DELEGATES_DATABASE_FIELDS)
       {
         database_multiple_documents_fields.item[count][count2] = (char*)calloc(100,sizeof(char));
         database_multiple_documents_fields.value[count][count2] = (char*)calloc(100000,sizeof(char));
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
  if (read_multiple_documents_all_fields_from_collection(database_name,DATABASE_COLLECTION,"",&database_multiple_documents_fields,1,document_count,0,"") == 0)
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
    memcpy(delegates[count].shared_delegate_status,database_multiple_documents_fields.value[count][7],strnlen(database_multiple_documents_fields.value[count][7],BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memcpy(delegates[count].fee_structure,database_multiple_documents_fields.value[count][8],strnlen(database_multiple_documents_fields.value[count][8],BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memcpy(delegates[count].server_settings,database_multiple_documents_fields.value[count][9],strnlen(database_multiple_documents_fields.value[count][9],BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memcpy(delegates[count].block_verifier_score,database_multiple_documents_fields.value[count][10],strnlen(database_multiple_documents_fields.value[count][10],BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memcpy(delegates[count].online_status,database_multiple_documents_fields.value[count][11],strnlen(database_multiple_documents_fields.value[count][11],BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memcpy(delegates[count].block_verifier_total_rounds,database_multiple_documents_fields.value[count][12],strnlen(database_multiple_documents_fields.value[count][12],BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memcpy(delegates[count].block_verifier_online_total_rounds,database_multiple_documents_fields.value[count][13],strnlen(database_multiple_documents_fields.value[count][13],BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memcpy(delegates[count].block_verifier_online_percentage,database_multiple_documents_fields.value[count][14],strnlen(database_multiple_documents_fields.value[count][14],BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memcpy(delegates[count].block_producer_total_rounds,database_multiple_documents_fields.value[count][15],strnlen(database_multiple_documents_fields.value[count][15],BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memcpy(delegates[count].block_producer_block_heights,database_multiple_documents_fields.value[count][16],strnlen(database_multiple_documents_fields.value[count][16],100000));
    memcpy(delegates[count].public_key,database_multiple_documents_fields.value[count][17],VRF_PUBLIC_KEY_LENGTH);
  }
  
  // organize the delegates by total_vote_count
  qsort(delegates,database_multiple_documents_fields.document_count,sizeof(struct delegates),organize_delegates_settings);

  // make sure the network data nodes are always the first delegates
  for (count = 0; count < (int)database_multiple_documents_fields.document_count; count++)
  {
    if (production_settings == 1)
    {
      if (strncmp(delegates[count].public_address,NETWORK_DATA_NODE_1_PUBLIC_ADDRESS_PRODUCTION,XCASH_WALLET_LENGTH) == 0)
      {
        memcpy(delegates_copy[0].public_address,delegates[count].public_address,strlen(delegates[count].public_address));
        memcpy(delegates_copy[0].total_vote_count,delegates[count].total_vote_count,strlen(delegates[count].total_vote_count));
        memcpy(delegates_copy[0].IP_address,delegates[count].IP_address,strlen(delegates[count].IP_address));
        memcpy(delegates_copy[0].delegate_name,delegates[count].delegate_name,strlen(delegates[count].delegate_name));
        memcpy(delegates_copy[0].about,delegates[count].about,strlen(delegates[count].about));
        memcpy(delegates_copy[0].website,delegates[count].website,strlen(delegates[count].website));
        memcpy(delegates_copy[0].team,delegates[count].team,strlen(delegates[count].team));
        memcpy(delegates_copy[0].fee_structure,delegates[count].fee_structure,strlen(delegates[count].fee_structure));
        memcpy(delegates_copy[0].block_verifier_score,delegates[count].block_verifier_score,strlen(delegates[count].block_verifier_score));
        memcpy(delegates_copy[0].online_status,delegates[count].online_status,strlen(delegates[count].online_status));
        memcpy(delegates_copy[0].block_verifier_total_rounds,delegates[count].block_verifier_total_rounds,strlen(delegates[count].block_verifier_total_rounds));
        memcpy(delegates_copy[0].block_verifier_online_total_rounds,delegates[count].block_verifier_online_total_rounds,strlen(delegates[count].block_verifier_online_total_rounds));
        memcpy(delegates_copy[0].block_verifier_online_percentage,delegates[count].block_verifier_online_percentage,strlen(delegates[count].block_verifier_online_percentage));
        memcpy(delegates_copy[0].block_producer_total_rounds,delegates[count].block_producer_total_rounds,strlen(delegates[count].block_producer_total_rounds));
        memcpy(delegates_copy[0].block_producer_block_heights,delegates[count].block_producer_block_heights,strlen(delegates[count].block_producer_block_heights));
        memcpy(delegates_copy[0].public_key,delegates[count].public_key,strlen(delegates[count].public_key));
      }
      else if (strncmp(delegates[count].public_address,NETWORK_DATA_NODE_2_PUBLIC_ADDRESS_PRODUCTION,XCASH_WALLET_LENGTH) == 0)
      {
        memcpy(delegates_copy[1].public_address,delegates[count].public_address,strlen(delegates[count].public_address));
        memcpy(delegates_copy[1].total_vote_count,delegates[count].total_vote_count,strlen(delegates[count].total_vote_count));
        memcpy(delegates_copy[1].IP_address,delegates[count].IP_address,strlen(delegates[count].IP_address));
        memcpy(delegates_copy[1].delegate_name,delegates[count].delegate_name,strlen(delegates[count].delegate_name));
        memcpy(delegates_copy[1].about,delegates[count].about,strlen(delegates[count].about));
        memcpy(delegates_copy[1].website,delegates[count].website,strlen(delegates[count].website));
        memcpy(delegates_copy[1].team,delegates[count].team,strlen(delegates[count].team));
        memcpy(delegates_copy[1].fee_structure,delegates[count].fee_structure,strlen(delegates[count].fee_structure));
        memcpy(delegates_copy[1].block_verifier_score,delegates[count].block_verifier_score,strlen(delegates[count].block_verifier_score));
        memcpy(delegates_copy[1].online_status,delegates[count].online_status,strlen(delegates[count].online_status));
        memcpy(delegates_copy[1].block_verifier_total_rounds,delegates[count].block_verifier_total_rounds,strlen(delegates[count].block_verifier_total_rounds));
        memcpy(delegates_copy[1].block_verifier_online_total_rounds,delegates[count].block_verifier_online_total_rounds,strlen(delegates[count].block_verifier_online_total_rounds));
        memcpy(delegates_copy[1].block_verifier_online_percentage,delegates[count].block_verifier_online_percentage,strlen(delegates[count].block_verifier_online_percentage));
        memcpy(delegates_copy[1].block_producer_total_rounds,delegates[count].block_producer_total_rounds,strlen(delegates[count].block_producer_total_rounds));
        memcpy(delegates_copy[1].block_producer_block_heights,delegates[count].block_producer_block_heights,strlen(delegates[count].block_producer_block_heights));
        memcpy(delegates_copy[1].public_key,delegates[count].public_key,strlen(delegates[count].public_key));
      }
      else if (strncmp(delegates[count].public_address,NETWORK_DATA_NODE_3_PUBLIC_ADDRESS_PRODUCTION,XCASH_WALLET_LENGTH) == 0)
      {
        memcpy(delegates_copy[2].public_address,delegates[count].public_address,strlen(delegates[count].public_address));
        memcpy(delegates_copy[2].total_vote_count,delegates[count].total_vote_count,strlen(delegates[count].total_vote_count));
        memcpy(delegates_copy[2].IP_address,delegates[count].IP_address,strlen(delegates[count].IP_address));
        memcpy(delegates_copy[2].delegate_name,delegates[count].delegate_name,strlen(delegates[count].delegate_name));
        memcpy(delegates_copy[2].about,delegates[count].about,strlen(delegates[count].about));
        memcpy(delegates_copy[2].website,delegates[count].website,strlen(delegates[count].website));
        memcpy(delegates_copy[2].team,delegates[count].team,strlen(delegates[count].team));
        memcpy(delegates_copy[2].fee_structure,delegates[count].fee_structure,strlen(delegates[count].fee_structure));
        memcpy(delegates_copy[2].block_verifier_score,delegates[count].block_verifier_score,strlen(delegates[count].block_verifier_score));
        memcpy(delegates_copy[2].online_status,delegates[count].online_status,strlen(delegates[count].online_status));
        memcpy(delegates_copy[2].block_verifier_total_rounds,delegates[count].block_verifier_total_rounds,strlen(delegates[count].block_verifier_total_rounds));
        memcpy(delegates_copy[2].block_verifier_online_total_rounds,delegates[count].block_verifier_online_total_rounds,strlen(delegates[count].block_verifier_online_total_rounds));
        memcpy(delegates_copy[2].block_verifier_online_percentage,delegates[count].block_verifier_online_percentage,strlen(delegates[count].block_verifier_online_percentage));
        memcpy(delegates_copy[2].block_producer_total_rounds,delegates[count].block_producer_total_rounds,strlen(delegates[count].block_producer_total_rounds));
        memcpy(delegates_copy[2].block_producer_block_heights,delegates[count].block_producer_block_heights,strlen(delegates[count].block_producer_block_heights));
        memcpy(delegates_copy[2].public_key,delegates[count].public_key,strlen(delegates[count].public_key));
      }
      else if (strncmp(delegates[count].public_address,NETWORK_DATA_NODE_4_PUBLIC_ADDRESS_PRODUCTION,XCASH_WALLET_LENGTH) == 0)
      {
        memcpy(delegates_copy[3].public_address,delegates[count].public_address,strlen(delegates[count].public_address));
        memcpy(delegates_copy[3].total_vote_count,delegates[count].total_vote_count,strlen(delegates[count].total_vote_count));
        memcpy(delegates_copy[3].IP_address,delegates[count].IP_address,strlen(delegates[count].IP_address));
        memcpy(delegates_copy[3].delegate_name,delegates[count].delegate_name,strlen(delegates[count].delegate_name));
        memcpy(delegates_copy[3].about,delegates[count].about,strlen(delegates[count].about));
        memcpy(delegates_copy[3].website,delegates[count].website,strlen(delegates[count].website));
        memcpy(delegates_copy[3].team,delegates[count].team,strlen(delegates[count].team));
        memcpy(delegates_copy[3].fee_structure,delegates[count].fee_structure,strlen(delegates[count].fee_structure));
        memcpy(delegates_copy[3].block_verifier_score,delegates[count].block_verifier_score,strlen(delegates[count].block_verifier_score));
        memcpy(delegates_copy[3].online_status,delegates[count].online_status,strlen(delegates[count].online_status));
        memcpy(delegates_copy[3].block_verifier_total_rounds,delegates[count].block_verifier_total_rounds,strlen(delegates[count].block_verifier_total_rounds));
        memcpy(delegates_copy[3].block_verifier_online_total_rounds,delegates[count].block_verifier_online_total_rounds,strlen(delegates[count].block_verifier_online_total_rounds));
        memcpy(delegates_copy[3].block_verifier_online_percentage,delegates[count].block_verifier_online_percentage,strlen(delegates[count].block_verifier_online_percentage));
        memcpy(delegates_copy[3].block_producer_total_rounds,delegates[count].block_producer_total_rounds,strlen(delegates[count].block_producer_total_rounds));
        memcpy(delegates_copy[3].block_producer_block_heights,delegates[count].block_producer_block_heights,strlen(delegates[count].block_producer_block_heights));
        memcpy(delegates_copy[3].public_key,delegates[count].public_key,strlen(delegates[count].public_key));
      }
      else if (strncmp(delegates[count].public_address,NETWORK_DATA_NODE_5_PUBLIC_ADDRESS_PRODUCTION,XCASH_WALLET_LENGTH) == 0)
      {
        memcpy(delegates_copy[4].public_address,delegates[count].public_address,strlen(delegates[count].public_address));
        memcpy(delegates_copy[4].total_vote_count,delegates[count].total_vote_count,strlen(delegates[count].total_vote_count));
        memcpy(delegates_copy[4].IP_address,delegates[count].IP_address,strlen(delegates[count].IP_address));
        memcpy(delegates_copy[4].delegate_name,delegates[count].delegate_name,strlen(delegates[count].delegate_name));
        memcpy(delegates_copy[4].about,delegates[count].about,strlen(delegates[count].about));
        memcpy(delegates_copy[4].website,delegates[count].website,strlen(delegates[count].website));
        memcpy(delegates_copy[4].team,delegates[count].team,strlen(delegates[count].team));
        memcpy(delegates_copy[4].fee_structure,delegates[count].fee_structure,strlen(delegates[count].fee_structure));
        memcpy(delegates_copy[4].block_verifier_score,delegates[count].block_verifier_score,strlen(delegates[count].block_verifier_score));
        memcpy(delegates_copy[4].online_status,delegates[count].online_status,strlen(delegates[count].online_status));
        memcpy(delegates_copy[4].block_verifier_total_rounds,delegates[count].block_verifier_total_rounds,strlen(delegates[count].block_verifier_total_rounds));
        memcpy(delegates_copy[4].block_verifier_online_total_rounds,delegates[count].block_verifier_online_total_rounds,strlen(delegates[count].block_verifier_online_total_rounds));
        memcpy(delegates_copy[4].block_verifier_online_percentage,delegates[count].block_verifier_online_percentage,strlen(delegates[count].block_verifier_online_percentage));
        memcpy(delegates_copy[4].block_producer_total_rounds,delegates[count].block_producer_total_rounds,strlen(delegates[count].block_producer_total_rounds));
        memcpy(delegates_copy[4].block_producer_block_heights,delegates[count].block_producer_block_heights,strlen(delegates[count].block_producer_block_heights));
        memcpy(delegates_copy[4].public_key,delegates[count].public_key,strlen(delegates[count].public_key));
      }
    }
    else if (production_settings == 0)
    {
      if (strncmp(delegates[count].public_address,NETWORK_DATA_NODE_1_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0)
      {
        memcpy(delegates_copy[0].public_address,delegates[count].public_address,strlen(delegates[count].public_address));
        memcpy(delegates_copy[0].total_vote_count,delegates[count].total_vote_count,strlen(delegates[count].total_vote_count));
        memcpy(delegates_copy[0].IP_address,delegates[count].IP_address,strlen(delegates[count].IP_address));
        memcpy(delegates_copy[0].delegate_name,delegates[count].delegate_name,strlen(delegates[count].delegate_name));
        memcpy(delegates_copy[0].about,delegates[count].about,strlen(delegates[count].about));
        memcpy(delegates_copy[0].website,delegates[count].website,strlen(delegates[count].website));
        memcpy(delegates_copy[0].team,delegates[count].team,strlen(delegates[count].team));
        memcpy(delegates_copy[0].fee_structure,delegates[count].fee_structure,strlen(delegates[count].fee_structure));
        memcpy(delegates_copy[0].block_verifier_score,delegates[count].block_verifier_score,strlen(delegates[count].block_verifier_score));
        memcpy(delegates_copy[0].online_status,delegates[count].online_status,strlen(delegates[count].online_status));
        memcpy(delegates_copy[0].block_verifier_total_rounds,delegates[count].block_verifier_total_rounds,strlen(delegates[count].block_verifier_total_rounds));
        memcpy(delegates_copy[0].block_verifier_online_total_rounds,delegates[count].block_verifier_online_total_rounds,strlen(delegates[count].block_verifier_online_total_rounds));
        memcpy(delegates_copy[0].block_verifier_online_percentage,delegates[count].block_verifier_online_percentage,strlen(delegates[count].block_verifier_online_percentage));
        memcpy(delegates_copy[0].block_producer_total_rounds,delegates[count].block_producer_total_rounds,strlen(delegates[count].block_producer_total_rounds));
        memcpy(delegates_copy[0].block_producer_block_heights,delegates[count].block_producer_block_heights,strlen(delegates[count].block_producer_block_heights));
        memcpy(delegates_copy[0].public_key,delegates[count].public_key,strlen(delegates[count].public_key));
      }
      else if (strncmp(delegates[count].public_address,NETWORK_DATA_NODE_2_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0)
      {
        memcpy(delegates_copy[1].public_address,delegates[count].public_address,strlen(delegates[count].public_address));
        memcpy(delegates_copy[1].total_vote_count,delegates[count].total_vote_count,strlen(delegates[count].total_vote_count));
        memcpy(delegates_copy[1].IP_address,delegates[count].IP_address,strlen(delegates[count].IP_address));
        memcpy(delegates_copy[1].delegate_name,delegates[count].delegate_name,strlen(delegates[count].delegate_name));
        memcpy(delegates_copy[1].about,delegates[count].about,strlen(delegates[count].about));
        memcpy(delegates_copy[1].website,delegates[count].website,strlen(delegates[count].website));
        memcpy(delegates_copy[1].team,delegates[count].team,strlen(delegates[count].team));
        memcpy(delegates_copy[1].fee_structure,delegates[count].fee_structure,strlen(delegates[count].fee_structure));
        memcpy(delegates_copy[1].block_verifier_score,delegates[count].block_verifier_score,strlen(delegates[count].block_verifier_score));
        memcpy(delegates_copy[1].online_status,delegates[count].online_status,strlen(delegates[count].online_status));
        memcpy(delegates_copy[1].block_verifier_total_rounds,delegates[count].block_verifier_total_rounds,strlen(delegates[count].block_verifier_total_rounds));
        memcpy(delegates_copy[1].block_verifier_online_total_rounds,delegates[count].block_verifier_online_total_rounds,strlen(delegates[count].block_verifier_online_total_rounds));
        memcpy(delegates_copy[1].block_verifier_online_percentage,delegates[count].block_verifier_online_percentage,strlen(delegates[count].block_verifier_online_percentage));
        memcpy(delegates_copy[1].block_producer_total_rounds,delegates[count].block_producer_total_rounds,strlen(delegates[count].block_producer_total_rounds));
        memcpy(delegates_copy[1].block_producer_block_heights,delegates[count].block_producer_block_heights,strlen(delegates[count].block_producer_block_heights));
        memcpy(delegates_copy[1].public_key,delegates[count].public_key,strlen(delegates[count].public_key));
      }
      else if (strncmp(delegates[count].public_address,NETWORK_DATA_NODE_3_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0)
      {
        memcpy(delegates_copy[2].public_address,delegates[count].public_address,strlen(delegates[count].public_address));
        memcpy(delegates_copy[2].total_vote_count,delegates[count].total_vote_count,strlen(delegates[count].total_vote_count));
        memcpy(delegates_copy[2].IP_address,delegates[count].IP_address,strlen(delegates[count].IP_address));
        memcpy(delegates_copy[2].delegate_name,delegates[count].delegate_name,strlen(delegates[count].delegate_name));
        memcpy(delegates_copy[2].about,delegates[count].about,strlen(delegates[count].about));
        memcpy(delegates_copy[2].website,delegates[count].website,strlen(delegates[count].website));
        memcpy(delegates_copy[2].team,delegates[count].team,strlen(delegates[count].team));
        memcpy(delegates_copy[2].fee_structure,delegates[count].fee_structure,strlen(delegates[count].fee_structure));
        memcpy(delegates_copy[2].block_verifier_score,delegates[count].block_verifier_score,strlen(delegates[count].block_verifier_score));
        memcpy(delegates_copy[2].online_status,delegates[count].online_status,strlen(delegates[count].online_status));
        memcpy(delegates_copy[2].block_verifier_total_rounds,delegates[count].block_verifier_total_rounds,strlen(delegates[count].block_verifier_total_rounds));
        memcpy(delegates_copy[2].block_verifier_online_total_rounds,delegates[count].block_verifier_online_total_rounds,strlen(delegates[count].block_verifier_online_total_rounds));
        memcpy(delegates_copy[2].block_verifier_online_percentage,delegates[count].block_verifier_online_percentage,strlen(delegates[count].block_verifier_online_percentage));
        memcpy(delegates_copy[2].block_producer_total_rounds,delegates[count].block_producer_total_rounds,strlen(delegates[count].block_producer_total_rounds));
        memcpy(delegates_copy[2].block_producer_block_heights,delegates[count].block_producer_block_heights,strlen(delegates[count].block_producer_block_heights));
        memcpy(delegates_copy[2].public_key,delegates[count].public_key,strlen(delegates[count].public_key));
      }
      else if (strncmp(delegates[count].public_address,NETWORK_DATA_NODE_4_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0)
      {
        memcpy(delegates_copy[3].public_address,delegates[count].public_address,strlen(delegates[count].public_address));
        memcpy(delegates_copy[3].total_vote_count,delegates[count].total_vote_count,strlen(delegates[count].total_vote_count));
        memcpy(delegates_copy[3].IP_address,delegates[count].IP_address,strlen(delegates[count].IP_address));
        memcpy(delegates_copy[3].delegate_name,delegates[count].delegate_name,strlen(delegates[count].delegate_name));
        memcpy(delegates_copy[3].about,delegates[count].about,strlen(delegates[count].about));
        memcpy(delegates_copy[3].website,delegates[count].website,strlen(delegates[count].website));
        memcpy(delegates_copy[3].team,delegates[count].team,strlen(delegates[count].team));
        memcpy(delegates_copy[3].fee_structure,delegates[count].fee_structure,strlen(delegates[count].fee_structure));
        memcpy(delegates_copy[3].block_verifier_score,delegates[count].block_verifier_score,strlen(delegates[count].block_verifier_score));
        memcpy(delegates_copy[3].online_status,delegates[count].online_status,strlen(delegates[count].online_status));
        memcpy(delegates_copy[3].block_verifier_total_rounds,delegates[count].block_verifier_total_rounds,strlen(delegates[count].block_verifier_total_rounds));
        memcpy(delegates_copy[3].block_verifier_online_total_rounds,delegates[count].block_verifier_online_total_rounds,strlen(delegates[count].block_verifier_online_total_rounds));
        memcpy(delegates_copy[3].block_verifier_online_percentage,delegates[count].block_verifier_online_percentage,strlen(delegates[count].block_verifier_online_percentage));
        memcpy(delegates_copy[3].block_producer_total_rounds,delegates[count].block_producer_total_rounds,strlen(delegates[count].block_producer_total_rounds));
        memcpy(delegates_copy[3].block_producer_block_heights,delegates[count].block_producer_block_heights,strlen(delegates[count].block_producer_block_heights));
        memcpy(delegates_copy[3].public_key,delegates[count].public_key,strlen(delegates[count].public_key));
      }
      else if (strncmp(delegates[count].public_address,NETWORK_DATA_NODE_5_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0)
      {
        memcpy(delegates_copy[4].public_address,delegates[count].public_address,strlen(delegates[count].public_address));
        memcpy(delegates_copy[4].total_vote_count,delegates[count].total_vote_count,strlen(delegates[count].total_vote_count));
        memcpy(delegates_copy[4].IP_address,delegates[count].IP_address,strlen(delegates[count].IP_address));
        memcpy(delegates_copy[4].delegate_name,delegates[count].delegate_name,strlen(delegates[count].delegate_name));
        memcpy(delegates_copy[4].about,delegates[count].about,strlen(delegates[count].about));
        memcpy(delegates_copy[4].website,delegates[count].website,strlen(delegates[count].website));
        memcpy(delegates_copy[4].team,delegates[count].team,strlen(delegates[count].team));
        memcpy(delegates_copy[4].fee_structure,delegates[count].fee_structure,strlen(delegates[count].fee_structure));
        memcpy(delegates_copy[4].block_verifier_score,delegates[count].block_verifier_score,strlen(delegates[count].block_verifier_score));
        memcpy(delegates_copy[4].online_status,delegates[count].online_status,strlen(delegates[count].online_status));
        memcpy(delegates_copy[4].block_verifier_total_rounds,delegates[count].block_verifier_total_rounds,strlen(delegates[count].block_verifier_total_rounds));
        memcpy(delegates_copy[4].block_verifier_online_total_rounds,delegates[count].block_verifier_online_total_rounds,strlen(delegates[count].block_verifier_online_total_rounds));
        memcpy(delegates_copy[4].block_verifier_online_percentage,delegates[count].block_verifier_online_percentage,strlen(delegates[count].block_verifier_online_percentage));
        memcpy(delegates_copy[4].block_producer_total_rounds,delegates[count].block_producer_total_rounds,strlen(delegates[count].block_producer_total_rounds));
        memcpy(delegates_copy[4].block_producer_block_heights,delegates[count].block_producer_block_heights,strlen(delegates[count].block_producer_block_heights));
        memcpy(delegates_copy[4].public_key,delegates[count].public_key,strlen(delegates[count].public_key));
      }
    }
  }

  for (count = 0, count2 = NETWORK_DATA_NODES_AMOUNT; count < (int)database_multiple_documents_fields.document_count; count++)
  {
    if ((production_settings == 1 && (strncmp(delegates[count].public_address,NETWORK_DATA_NODE_1_PUBLIC_ADDRESS_PRODUCTION,XCASH_WALLET_LENGTH) == 0 || strncmp(delegates[count].public_address,NETWORK_DATA_NODE_2_PUBLIC_ADDRESS_PRODUCTION,XCASH_WALLET_LENGTH) == 0 || strncmp(delegates[count].public_address,NETWORK_DATA_NODE_3_PUBLIC_ADDRESS_PRODUCTION,XCASH_WALLET_LENGTH) == 0 || strncmp(delegates[count].public_address,NETWORK_DATA_NODE_4_PUBLIC_ADDRESS_PRODUCTION,XCASH_WALLET_LENGTH) == 0 || strncmp(delegates[count].public_address,NETWORK_DATA_NODE_5_PUBLIC_ADDRESS_PRODUCTION,XCASH_WALLET_LENGTH) == 0)) || (production_settings == 0 && (strncmp(delegates[count].public_address,NETWORK_DATA_NODE_1_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0 || strncmp(delegates[count].public_address,NETWORK_DATA_NODE_2_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0 || strncmp(delegates[count].public_address,NETWORK_DATA_NODE_3_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0 || strncmp(delegates[count].public_address,NETWORK_DATA_NODE_4_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0 || strncmp(delegates[count].public_address,NETWORK_DATA_NODE_5_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0)))
    {
      // skip the network data nodes since they have already been added
      continue;
    }

    memcpy(delegates_copy[count2].public_address,delegates[count].public_address,strlen(delegates[count].public_address));
    memcpy(delegates_copy[count2].total_vote_count,delegates[count].total_vote_count,strlen(delegates[count].total_vote_count));
    memcpy(delegates_copy[count2].IP_address,delegates[count].IP_address,strlen(delegates[count].IP_address));
    memcpy(delegates_copy[count2].delegate_name,delegates[count].delegate_name,strlen(delegates[count].delegate_name));
    memcpy(delegates_copy[count2].about,delegates[count].about,strlen(delegates[count].about));
    memcpy(delegates_copy[count2].website,delegates[count].website,strlen(delegates[count].website));
    memcpy(delegates_copy[count2].team,delegates[count].team,strlen(delegates[count].team));
    memcpy(delegates_copy[count2].fee_structure,delegates[count].fee_structure,strlen(delegates[count].fee_structure));
    memcpy(delegates_copy[count2].block_verifier_score,delegates[count].block_verifier_score,strlen(delegates[count].block_verifier_score));
    memcpy(delegates_copy[count2].online_status,delegates[count].online_status,strlen(delegates[count].online_status));
    memcpy(delegates_copy[count2].block_verifier_total_rounds,delegates[count].block_verifier_total_rounds,strlen(delegates[count].block_verifier_total_rounds));
    memcpy(delegates_copy[count2].block_verifier_online_total_rounds,delegates[count].block_verifier_online_total_rounds,strlen(delegates[count].block_verifier_online_total_rounds));
    memcpy(delegates_copy[count2].block_verifier_online_percentage,delegates[count].block_verifier_online_percentage,strlen(delegates[count].block_verifier_online_percentage));
    memcpy(delegates_copy[count2].block_producer_total_rounds,delegates[count].block_producer_total_rounds,strlen(delegates[count].block_producer_total_rounds));
    memcpy(delegates_copy[count2].block_producer_block_heights,delegates[count].block_producer_block_heights,strlen(delegates[count].block_producer_block_heights));
    memcpy(delegates_copy[count2].public_key,delegates[count].public_key,strlen(delegates[count].public_key));
    count2++;
  }

  // copy the delegates_copy struct to the delegates struct
  for (count = 0; count < (int)database_multiple_documents_fields.document_count; count++)
  {
    memset(delegates[count].public_address,0,strlen(delegates[count].public_address));
    memset(delegates[count].total_vote_count,0,strlen(delegates[count].total_vote_count));
    memset(delegates[count].IP_address,0,strlen(delegates[count].IP_address));
    memset(delegates[count].delegate_name,0,strlen(delegates[count].delegate_name));
    memset(delegates[count].about,0,strlen(delegates[count].about));
    memset(delegates[count].website,0,strlen(delegates[count].website));
    memset(delegates[count].team,0,strlen(delegates[count].team));
    memset(delegates[count].fee_structure,0,strlen(delegates[count].fee_structure));
    memset(delegates[count].block_verifier_score,0,strlen(delegates[count].block_verifier_score));
    memset(delegates[count].online_status,0,strlen(delegates[count].online_status));
    memset(delegates[count].block_verifier_total_rounds,0,strlen(delegates[count].block_verifier_total_rounds));
    memset(delegates[count].block_verifier_online_total_rounds,0,strlen(delegates[count].block_verifier_online_total_rounds));
    memset(delegates[count].block_verifier_online_percentage,0,strlen(delegates[count].block_verifier_online_percentage));
    memset(delegates[count].block_producer_total_rounds,0,strlen(delegates[count].block_producer_total_rounds));
    memset(delegates[count].block_producer_block_heights,0,strlen(delegates[count].block_producer_block_heights));
    memset(delegates[count].public_key,0,strlen(delegates[count].public_key));

    memcpy(delegates[count].public_address,delegates_copy[count].public_address,strlen(delegates_copy[count].public_address));
    memcpy(delegates[count].total_vote_count,delegates_copy[count].total_vote_count,strlen(delegates_copy[count].total_vote_count));
    memcpy(delegates[count].IP_address,delegates_copy[count].IP_address,strlen(delegates_copy[count].IP_address));
    memcpy(delegates[count].delegate_name,delegates_copy[count].delegate_name,strlen(delegates_copy[count].delegate_name));
    memcpy(delegates[count].about,delegates_copy[count].about,strlen(delegates_copy[count].about));
    memcpy(delegates[count].website,delegates_copy[count].website,strlen(delegates_copy[count].website));
    memcpy(delegates[count].team,delegates_copy[count].team,strlen(delegates_copy[count].team));
    memcpy(delegates[count].fee_structure,delegates_copy[count].fee_structure,strlen(delegates_copy[count].fee_structure));
    memcpy(delegates[count].block_verifier_score,delegates_copy[count].block_verifier_score,strlen(delegates_copy[count].block_verifier_score));
    memcpy(delegates[count].online_status,delegates_copy[count].online_status,strlen(delegates_copy[count].online_status));
    memcpy(delegates[count].block_verifier_total_rounds,delegates_copy[count].block_verifier_total_rounds,strlen(delegates_copy[count].block_verifier_total_rounds));
    memcpy(delegates[count].block_verifier_online_total_rounds,delegates_copy[count].block_verifier_online_total_rounds,strlen(delegates_copy[count].block_verifier_online_total_rounds));
    memcpy(delegates[count].block_verifier_online_percentage,delegates_copy[count].block_verifier_online_percentage,strlen(delegates_copy[count].block_verifier_online_percentage));
    memcpy(delegates[count].block_producer_total_rounds,delegates_copy[count].block_producer_total_rounds,strlen(delegates_copy[count].block_producer_total_rounds));
    memcpy(delegates[count].block_producer_block_heights,delegates_copy[count].block_producer_block_heights,strlen(delegates_copy[count].block_producer_block_heights));
    memcpy(delegates[count].public_key,delegates_copy[count].public_key,strlen(delegates_copy[count].public_key));
  }

  RESET_DELEGATES;
  POINTER_RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,count2,TOTAL_DELEGATES_DATABASE_FIELDS+1);

  return database_multiple_documents_fields.document_count;
  
  #undef ORGANIZE_DELEGATES_ERROR
  #undef RESET_DELEGATES
}