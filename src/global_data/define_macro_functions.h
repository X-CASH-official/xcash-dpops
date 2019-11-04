#ifndef DEFINE_MACRO_FUNCTIONS_H_   /* Include guard */
#define DEFINE_MACRO_FUNCTIONS_H_

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
  fprintf(stderr,"%s",string); \
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
memcpy(data1+strlen(data1),data2,strnlen(data2,data1_length - strlen(data1) - 1));



/*
-----------------------------------------------------------------------------------------------------------
Name: print_error_message
Description: Prints all of the functions and error messages
-----------------------------------------------------------------------------------------------------------
*/

#define print_error_message(current_date_and_time,current_UTC_date_and_time,buffer) \
fprintf(stderr,"\n\n"); \
color_print(TEST_OUTLINE,"red"); \
fprintf(stderr,"\033[1;31m%s: Error\033[0m\n",error_message.function[0]); \
get_current_UTC_time(current_date_and_time,current_UTC_date_and_time); \
memset(buffer,0,sizeof(buffer)); \
strftime(buffer,sizeof(buffer),"%a %d %b %Y %H:%M:%S UTC\n",&current_UTC_date_and_time); \
fprintf(stderr,"\033[1;31m%s\033[0m",buffer); \
color_print(TEST_OUTLINE,"red"); \
fprintf(stderr,"\033[1;31mFunction Calls:\033[0m\n"); \
for (error_message_count = 0; error_message_count < error_message.total; error_message_count++) \
{ \
  fprintf(stderr,"\033[1;31m#%d %s: %s\033[0m\n",error_message_count+1,error_message.function[error_message_count],error_message.data[error_message_count]); \
} \
for (error_message_count = 0; error_message_count < TOTAL_DELEGATES_DATABASE_FIELDS; error_message_count++) \
{ \
  memset(error_message.function[error_message_count],0,strlen(error_message.function[error_message_count])); \
  memset(error_message.data[error_message_count],0,strlen(error_message.data[error_message_count])); \
} \
memset(buffer,0,sizeof(buffer)); \
error_message.total = 0;



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
fprintf(stderr,"\n"); \
color_print(TEST_OUTLINE,"blue"); \
fprintf(stderr,"\033[1;34m%s\033[0m\n",string); \
get_current_UTC_time(current_date_and_time,current_UTC_date_and_time); \
memset(buffer,0,sizeof(buffer)); \
strftime(buffer,sizeof(buffer),"%a %d %b %Y %H:%M:%S UTC\n",&current_UTC_date_and_time); \
fprintf(stderr,"\033[1;34m%s\033[0m",buffer); \
color_print(TEST_OUTLINE,"blue");



/*
-----------------------------------------------------------------------------------------------------------
Name: get_current_reserve_bytes_database
Description: gets the reserve bytes database
Parameters:
  count - count
Return: Writes the correct code
-----------------------------------------------------------------------------------------------------------
*/

#define get_reserve_bytes_database(count,settings) \
sscanf(current_block_height,"%zu", &count); \
if (settings == 1) \
{ \
  count--; \
} \
if (count-1 == XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT || count == XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) \
{ \
  count = 1; \
} \
else \
{ \
  count = (((count - 1) - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME) + 1; \
}




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
Name: sync_block_verifiers_minutes
Description: Syncs the block verifiers to a specific minute
Parameters:
  current_date_and_time -  The current date and time
  current_UTC_date_and_time - The current UTC date and time
  minutes - The minutes
-----------------------------------------------------------------------------------------------------------
*/

#define sync_block_verifiers_minutes(current_date_and_time,current_UTC_date_and_time,minutes) \
do \
{ \
  usleep(200000); \
  get_current_UTC_time(current_date_and_time,current_UTC_date_and_time); \
} while (current_UTC_date_and_time.tm_min % BLOCK_TIME != minutes); 



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_block_verifiers_seconds
Description: Syncs the block verifiers to a specific second
Parameters:
  current_date_and_time -  The current date and time
  current_UTC_date_and_time - The current UTC date and time
  seconds - The seconds
-----------------------------------------------------------------------------------------------------------
*/

#define sync_block_verifiers_seconds(current_date_and_time,current_UTC_date_and_time,seconds) \
do \
{ \
  usleep(200000); \
  get_current_UTC_time(current_date_and_time,current_UTC_date_and_time); \
} while (current_UTC_date_and_time.tm_sec != seconds);



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_block_verifiers_minutes_and_seconds
Description: Syncs the block verifiers to a specific minute and second
Parameters:
  current_date_and_time -  The current date and time
  current_UTC_date_and_time - The current UTC date and time
  minutes - The minutes
  seconds - The seconds
-----------------------------------------------------------------------------------------------------------
*/

#define sync_block_verifiers_minutes_and_seconds(current_date_and_time,current_UTC_date_and_time,minutes,seconds) \
do \
{ \
  usleep(200000); \
  get_current_UTC_time(current_date_and_time,current_UTC_date_and_time); \
} while (current_UTC_date_and_time.tm_min % BLOCK_TIME != minutes || current_UTC_date_and_time.tm_sec != seconds);




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