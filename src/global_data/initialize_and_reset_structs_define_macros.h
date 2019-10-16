#ifndef INITIALIZE_AND_RESET_STRUCTS_DEFINE_MACROS_H_   /* Include guard */
#define INITIALIZE_AND_RESET_STRUCTS_DEFINE_MACROS_H_

/*
-----------------------------------------------------------------------------------------------------------
Using define statements instead of constants for increased efficiency
-----------------------------------------------------------------------------------------------------------
*/

// initialize and reset structs
#define INITIALIZE_DELEGATES_STRUCT(count,maximum_amount,function_name,buffer) \
for (count = 0; count < maximum_amount; count++) \
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
#endif