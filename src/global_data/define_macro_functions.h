#ifndef DEFINE_MACRO_FUNCTIONS_H_   /* Include guard */
#define DEFINE_MACRO_FUNCTIONS_H_

#include "define_macros.h"
#include "variables.h"
#include "file_functions.h"
#include <mongoc/mongoc.h>
#include <bson/bson.h>


/*
-----------------------------------------------------------------------------------------------------------
Using define macros instead of functions for increased efficiency
-----------------------------------------------------------------------------------------------------------
*/



/*
-----------------------------------------------------------------------------------------------------------
Name: color_print
Description: Prints a string in a color
Parameters:
  string - char*
  color - char*
Return: Writes the correct code

Color available values:
  red = Error
  green = Success
  blue = Start of a new round and general round information
  yellow = block producer information
  white = General information
-----------------------------------------------------------------------------------------------------------
*/

#define color_print(string,color) \
if (log_file_settings == 0) \
{ \
  if (strncmp(color,"red",BUFFER_SIZE) == 0) \
  { \
    fprintf(stderr,"\033[1;31m%s\033[0m\n",string); \
  } \
  else if (strncmp(color,"green",BUFFER_SIZE) == 0) \
  { \
    fprintf(stderr,"\033[1;32m%s\033[0m\n",string); \
  } \
  else if (strncmp(color,"yellow",BUFFER_SIZE) == 0) \
  { \
    fprintf(stderr,"\033[1;33m%s\033[0m\n",string); \
  } \
  else if (strncmp(color,"blue",BUFFER_SIZE) == 0) \
  { \
    fprintf(stderr,"\033[1;34m%s\033[0m\n",string); \
  } \
  else if (strncmp(color,"purple",BUFFER_SIZE) == 0) \
  { \
    fprintf(stderr,"\033[1;35m%s\033[0m\n",string); \
  } \
  else if (strncmp(color,"lightblue",BUFFER_SIZE) == 0) \
  { \
    fprintf(stderr,"\033[1;36m%s\033[0m\n",string); \
  } \
  else \
  { \
    fprintf(stderr,"%s\n",string); \
  } \
} \
else if (log_file_settings == 1) \
{ \
  append_file(string,log_file); \
  append_file("\n",log_file); \
} \
else if (log_file_settings == 2) \
{ \
  if (strncmp(color,"red",BUFFER_SIZE) == 0) \
  { \
    append_file("\033[1;31m",log_file); \
    append_file(string,log_file); \
    append_file("\n\033[0m",log_file); \
  } \
  else if (strncmp(color,"green",BUFFER_SIZE) == 0) \
  { \
    append_file("\033[1;32m",log_file); \
    append_file(string,log_file); \
    append_file("\n\033[0m",log_file); \
  } \
  else if (strncmp(color,"yellow",BUFFER_SIZE) == 0) \
  { \
    append_file("\033[1;33m",log_file); \
    append_file(string,log_file); \
    append_file("\n\033[0m",log_file); \
  } \
  else if (strncmp(color,"blue",BUFFER_SIZE) == 0) \
  { \
    append_file("\033[1;34m",log_file); \
    append_file(string,log_file); \
    append_file("\n\033[0m",log_file); \
  } \
  else if (strncmp(color,"purple",BUFFER_SIZE) == 0) \
  { \
    append_file("\033[1;35m",log_file); \
    append_file(string,log_file); \
    append_file("\n\033[0m",log_file); \
  } \
  else if (strncmp(color,"lightblue",BUFFER_SIZE) == 0) \
  { \
    append_file("\033[1;36m",log_file); \
    append_file(string,log_file); \
    append_file("\n\033[0m",log_file); \
  } \
  else \
  { \
    append_file(string,log_file); \
    append_file("\n",log_file); \
  } \
}



/*
-----------------------------------------------------------------------------------------------------------
Name: append_string
Description: append string
Parameters:
  data1 - data1
  data2 - data2
Return: Writes the correct code
-----------------------------------------------------------------------------------------------------------
*/

#define append_string(data1,data2,data1_length) \
memcpy(data1+strlen(data1),data2,strnlen(data2,(data1_length) - strlen(data1) - 1));



/*
-----------------------------------------------------------------------------------------------------------
Name: print_error_message
Description: Prints all of the functions and error messages
-----------------------------------------------------------------------------------------------------------
*/

#define print_error_message(current_date_and_time,current_UTC_date_and_time,buffer) \
if (error_message.total != 0) \
{ \
  memset(buffer,0,sizeof(buffer)); \
  memcpy(buffer,"\n\n",2); \
  memcpy(buffer+2,TEST_OUTLINE,sizeof(TEST_OUTLINE)-1); \
  memcpy(buffer+strlen(buffer),"\n",sizeof(char)); \
  memcpy(buffer+strlen(buffer),error_message.function[0],strlen(error_message.function[0])); \
  memcpy(buffer+strlen(buffer),": Error",7); \
  color_print(buffer,"red"); \
  get_current_UTC_time(current_date_and_time,current_UTC_date_and_time); \
  memset(buffer,0,sizeof(buffer)); \
  strftime(buffer,sizeof(buffer),"%a %d %b %Y %H:%M:%S UTC",&current_UTC_date_and_time); \
  color_print(buffer,"red"); \
  memset(buffer,0,sizeof(buffer)); \
  memcpy(buffer,TEST_OUTLINE,sizeof(TEST_OUTLINE)-1); \
  memcpy(buffer+strlen(buffer),"\nFunction Calls:\n",17); \
  for (error_message_count = 0; error_message_count < error_message.total; error_message_count++) \
  { \
    memcpy(buffer+strlen(buffer),"#",sizeof(char)); \
    sprintf(buffer+strlen(buffer),"%d",error_message_count+1); \
    memcpy(buffer+strlen(buffer)," ",sizeof(char)); \
    memcpy(buffer+strlen(buffer),error_message.function[error_message_count],strnlen(error_message.function[error_message_count],sizeof(buffer))); \
    memcpy(buffer+strlen(buffer),": ",2); \
    memcpy(buffer+strlen(buffer),error_message.data[error_message_count],strnlen(error_message.data[error_message_count],sizeof(buffer))); \
    memcpy(buffer+strlen(buffer),"\n",sizeof(char)); \
  } \
  color_print(buffer,"red"); \
  for (error_message_count = 0; error_message_count < TOTAL_DELEGATES_DATABASE_FIELDS; error_message_count++) \
  { \
    memset(error_message.function[error_message_count],0,strlen(error_message.function[error_message_count])); \
    memset(error_message.data[error_message_count],0,strlen(error_message.data[error_message_count])); \
  } \
  memset(buffer,0,sizeof(buffer)); \
  error_message.total = 0; \
}



/*
-----------------------------------------------------------------------------------------------------------
Name: print_start_message
Parameters:
  current_date_and_time -  The current date and time
  current_UTC_date_and_time - The current UTC date and time
  string - the message
  buffer - the buffer to print the time
Description: Prints the start message of a section
-----------------------------------------------------------------------------------------------------------
*/

#define print_start_message(current_date_and_time,current_UTC_date_and_time,string,buffer) \
memset(buffer,0,sizeof(buffer)); \
memcpy(buffer,"\n",sizeof(char)); \
memcpy(buffer+1,TEST_OUTLINE,sizeof(TEST_OUTLINE)-1); \
memcpy(buffer+strlen(buffer),"\n",sizeof(char)); \
memcpy(buffer+strlen(buffer),string,strnlen(string,sizeof(buffer))); \
color_print(buffer,"blue"); \
get_current_UTC_time(current_date_and_time,current_UTC_date_and_time); \
memset(buffer,0,sizeof(buffer)); \
strftime(buffer,sizeof(buffer),"%a %d %b %Y %H:%M:%S UTC\n",&current_UTC_date_and_time); \
memcpy(buffer+strlen(buffer),TEST_OUTLINE,sizeof(TEST_OUTLINE)-1); \
color_print(buffer,"blue"); \
memset(buffer,0,sizeof(buffer));



/*
-----------------------------------------------------------------------------------------------------------
Name: get_current_reserve_bytes_database
Description: gets the reserve bytes database
Parameters:
  count - count
  settings - 0 to get the current blocks reserve bytes database, 1 to get the previous blocks reserve bytes database
Return: Writes the correct code
-----------------------------------------------------------------------------------------------------------
*/

#define get_reserve_bytes_database(count,settings) \
sscanf(current_block_height,"%zu", &count); \
if ((test_settings) == 1) \
{ \
  count = 1; \
} \
else \
{ \
  if ((settings) == 1) \
  { \
    count--; \
  } \
  if (count-1 == XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT || count == XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) \
  { \
    count = 1; \
  } \
  else \
  { \
    count = ((count - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME) + 1; \
  } \
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_random_network_data_node
Description: gets a random network data node
Parameters:
  count - count
Return: Writes the correct code
-----------------------------------------------------------------------------------------------------------
*/

#define get_random_network_data_node(count) \
do \
{ \
  count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT); \
} while (memcmp(network_data_nodes_list.network_data_nodes_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0 || network_data_nodes_list.online_status[count] == 0); \



/*
-----------------------------------------------------------------------------------------------------------
Name: reset_synced_block_verifiers_vote_settings
Description: Resets the synced block verifiers vote settings
Return: Writes the correct code
-----------------------------------------------------------------------------------------------------------
*/

#define reset_synced_block_verifiers_vote_settings \
synced_block_verifiers.vote_settings_true = 1; \
synced_block_verifiers.vote_settings_false = 0; \
synced_block_verifiers.vote_settings_connection_timeout = 0;



/*
-----------------------------------------------------------------------------------------------------------
Name: get_current_UTC_time
Description: Gets the current UTC time
Parameters:
  current_date_and_time -  The current date and time
  current_UTC_date_and_time - The current UTC date and time
-----------------------------------------------------------------------------------------------------------
*/

#define get_current_UTC_time(current_date_and_time,current_UTC_date_and_time) \
time(&current_date_and_time); \
gmtime_r(&current_date_and_time,&current_UTC_date_and_time);



/*
-----------------------------------------------------------------------------------------------------------
Name: ERROR_DATA_MESSAGE
Description: Sends an error message back to the delegate
Return: Writes the correct code
-----------------------------------------------------------------------------------------------------------
*/

#define ERROR_DATA_MESSAGE \
char message_error[BUFFER_SIZE_NETWORK_BLOCK_DATA]; \
memset(message_error,0,sizeof(message_error)); \
memcpy(message_error,ERROR_DATA,sizeof(ERROR_DATA)-1); \
send_data(CLIENT_SOCKET,(unsigned char*)message_error,0,1,""); \
return;




/*
-----------------------------------------------------------------------------------------------------------
Name: pointer_reset
Description: Reset the memory used by the pointer, and sets the pointer to NULL to avoid a dangling pointer
Parameters:
  pointer - Any pointer type
Return: Writes the correct code
-----------------------------------------------------------------------------------------------------------
*/

#define pointer_reset(pointer) \
free(pointer); \
pointer = NULL;
#endif