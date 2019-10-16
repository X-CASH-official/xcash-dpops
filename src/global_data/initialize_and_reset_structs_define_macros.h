#ifndef INITIALIZE_AND_RESET_STRUCTS_DEFINE_MACROS_H_   /* Include guard */
#define INITIALIZE_AND_RESET_STRUCTS_DEFINE_MACROS_H_

/*
-----------------------------------------------------------------------------------------------------------
Using define statements instead of constants for increased efficiency
-----------------------------------------------------------------------------------------------------------
*/

// delegates struct
#define INITIALIZE_DELEGATES_STRUCT(count,maximum_amount,function_name,buffer) \
for (count = 0; (int)count < maximum_amount; count++) \
{ \
  delegates[count].public_address = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char)); \
  delegates[count].total_vote_count = (char*)calloc(100,sizeof(char)); \
  delegates[count].IP_address = (char*)calloc(256,sizeof(char)); \
  delegates[count].delegate_name = (char*)calloc(31,sizeof(char)); \
  delegates[count].about = (char*)calloc(1025,sizeof(char)); \
  delegates[count].website = (char*)calloc(256,sizeof(char)); \
  delegates[count].team = (char*)calloc(256,sizeof(char)); \
  delegates[count].pool_mode = (char*)calloc(10,sizeof(char)); \
  delegates[count].fee_structure = (char*)calloc(11,sizeof(char)); \
  delegates[count].server_settings = (char*)calloc(1025,sizeof(char)); \
  delegates[count].block_verifier_score = (char*)calloc(10,sizeof(char)); \
  delegates[count].online_status = (char*)calloc(10,sizeof(char)); \
  delegates[count].block_verifier_total_rounds = (char*)calloc(10,sizeof(char)); \
  delegates[count].block_verifier_online_total_rounds = (char*)calloc(10,sizeof(char)); \
  delegates[count].block_verifier_online_percentage = (char*)calloc(10,sizeof(char)); \
  delegates[count].block_producer_total_rounds = (char*)calloc(10,sizeof(char)); \
  delegates[count].block_producer_block_heights = (char*)calloc(50000,sizeof(char)); \
  if (delegates[count].public_address == NULL || delegates[count].total_vote_count == NULL || delegates[count].IP_address == NULL || delegates[count].delegate_name == NULL || delegates[count].about == NULL || delegates[count].website == NULL || delegates[count].team == NULL || delegates[count].pool_mode == NULL || delegates[count].fee_structure == NULL || delegates[count].server_settings == NULL || delegates[count].block_verifier_score == NULL || delegates[count].online_status == NULL || delegates[count].block_verifier_total_rounds == NULL || delegates[count].block_verifier_online_total_rounds == NULL || delegates[count].block_verifier_online_percentage == NULL || delegates[count].block_producer_total_rounds == NULL || delegates[count].block_producer_block_heights == NULL) \
  { \
    memcpy(error_message.function[error_message.total],function_name,sizeof(function_name)-1); \
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48); \
    error_message.total++; \
    print_error_message(current_date_and_time,current_UTC_date_and_time,buffer); \
    exit(0); \
  } \
}

#define POINTER_RESET_DELEGATES_STRUCT(count,maximum_amount) \
for (count = 0; (int)count < maximum_amount; count++) \
{ \
  pointer_reset(delegates[count].public_address); \
  pointer_reset(delegates[count].total_vote_count); \
  pointer_reset(delegates[count].IP_address); \
  pointer_reset(delegates[count].delegate_name); \
  pointer_reset(delegates[count].about); \
  pointer_reset(delegates[count].website); \
  pointer_reset(delegates[count].team); \
  pointer_reset(delegates[count].pool_mode); \
  pointer_reset(delegates[count].fee_structure); \
  pointer_reset(delegates[count].server_settings); \
  pointer_reset(delegates[count].block_verifier_score); \
  pointer_reset(delegates[count].online_status); \
  pointer_reset(delegates[count].block_verifier_total_rounds); \
  pointer_reset(delegates[count].block_verifier_online_total_rounds); \
  pointer_reset(delegates[count].block_verifier_online_percentage); \
  pointer_reset(delegates[count].block_producer_total_rounds); \
  pointer_reset(delegates[count].block_producer_block_heights); \
}



// votes struct
#define INITIALIZE_VOTES_STRUCT(count,maximum_amount,function_name,buffer) \
for (count = 0; (int)count < maximum_amount; count++) \
{ \
  votes[count].public_address_created_reserve_proof = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char)); \
  votes[count].public_address_voted_for = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char)); \
  votes[count].total = (char*)calloc(100,sizeof(char)); \
  votes[count].reserve_proof = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF+1,sizeof(char)); \
  if (votes[count].public_address_created_reserve_proof == NULL || votes[count].public_address_voted_for == NULL || votes[count].total == NULL || votes[count].reserve_proof == NULL) \
  { \
    memcpy(error_message.function[error_message.total],function_name,sizeof(function_name)-1); \
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48); \
    error_message.total++; \
    print_error_message(current_date_and_time,current_UTC_date_and_time,buffer); \
    exit(0); \
  } \
}

#define POINTER_RESET_VOTES_STRUCT(count,maximum_amount) \
for (count = 0; count < maximum_amount; count++) \
{ \
  pointer_reset(votes[count].public_address_created_reserve_proof); \
  pointer_reset(votes[count].public_address_voted_for); \
  pointer_reset(votes[count].total); \
  pointer_reset(votes[count].reserve_proof); \
} \



// delegates_online_status struct
#define INITIALIZE_DELEGATES_ONLINE_STATUS_STRUCT(count,maximum_amount,function_name,buffer) \
for (count = 0; (int)count < maximum_amount; count++) \
{ \
  delegates_online_status[count].public_address = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char)); \
  delegates_online_status[count].settings = 0; \
  if (delegates_online_status[count].public_address == NULL) \
  { \
    memcpy(error_message.function[error_message.total],function_name,sizeof(function_name)-1); \
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48); \
    error_message.total++; \
    print_error_message(current_date_and_time,current_UTC_date_and_time,buffer); \
    exit(0); \
  } \
}

#define POINTER_RESET_DELEGATES_ONLINE_STATUS_STRUCT(count,maximum_amount) \
for (count = 0; count < maximum_amount; count++) \
{ \
  pointer_reset(delegates_online_status[count].public_address); \
} \



// database_document_fields struct
#define INITIALIZE_DATABASE_DOCUMENT_FIELDS_STRUCT(count,maximum_amount,function_name,buffer) \
for (count = 0; (int)count < maximum_amount; count++) \
{ \
  database_data.item[count] = (char*)calloc(BUFFER_SIZE,sizeof(char)); \
  database_data.value[count] = (char*)calloc(BUFFER_SIZE,sizeof(char)); \
  if (database_data.item[count] == NULL || database_data.value[count] == NULL) \
  { \
    memcpy(error_message.function[error_message.total],function_name,sizeof(function_name)-1); \
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48); \
    error_message.total++; \
    print_error_message(current_date_and_time,current_UTC_date_and_time,buffer); \
    exit(0); \
  } \
} \
database_data.count = 0;

#define POINTER_RESET_DATABASE_DOCUMENT_FIELDS_STRUCT(count) \
for (count = 0; (int)count < (int)database_data.count; count++) \
{ \
  pointer_reset(database_data.item[count]); \
  pointer_reset(database_data.value[count]); \
} 



// database_multiple_documents_fields struct
#define INITIALIZE_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,count2,document_count,maximum_amount,function_name,buffer) \
for (count = 0; (int)count < document_count; count++) \
{ \
  for (count2 = 0; count2 < maximum_amount; count2++) \
  { \
    database_multiple_documents_fields.item[count][count2] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char)); \
    database_multiple_documents_fields.value[count][count2] = (char*)calloc(BUFFER_SIZE_RESERVE_PROOF,sizeof(char)); \
    if (database_multiple_documents_fields.item[count][count2] == NULL || database_multiple_documents_fields.value[count][count2] == NULL) \
    { \
      memcpy(error_message.function[error_message.total],function_name,sizeof(function_name)-1); \
      memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48); \
      error_message.total++; \
      print_error_message(current_date_and_time,current_UTC_date_and_time,buffer); \
      exit(0); \
    } \
  } \
} \
database_multiple_documents_fields.document_count = 0; \
database_multiple_documents_fields.database_fields_count = 0;

#define RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,count2,maximum_amount) \
for (count = 0; (int)count < (int)database_multiple_documents_fields.document_count; count++) \
{ \
  for (count2 = 0; (int)count2 < maximum_amount; count2++) \
  { \
    memset(database_multiple_documents_fields.item[count][count2],0,strlen(database_multiple_documents_fields.value[count][count2])); \
    memset(database_multiple_documents_fields.value[count][count2],0,strlen(database_multiple_documents_fields.value[count][count2])); \
  } \
}

#define POINTER_RESET_DATABASE_MULTIPLE_DOCUMENTS_FIELDS_STRUCT(count,count2,maximum_amount) \
for (count = 0; (int)count < (int)database_multiple_documents_fields.document_count; count++) \
{ \
  for (count2 = 0; (int)count2 < maximum_amount; count2++) \
  { \
    pointer_reset(database_multiple_documents_fields.item[count][count2]); \
    pointer_reset(database_multiple_documents_fields.value[count][count2]); \
  } \
}

#endif