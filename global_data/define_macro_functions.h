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
  lightblue = Start of a new round
  blue = Start of a new part of a round
  purple = Voting information
  yellow = Alerts when you are one of the main nodes
  white = General information
-----------------------------------------------------------------------------------------------------------
*/

#define color_print(string,color) \
if (memcmp(color,"red",3) == 0) \
{ \
  printf("\033[1;31m%s\033[0m\n",string); \
} \
else if (memcmp(color,"green",5) == 0) \
{ \
  printf("\033[1;32m%s\033[0m\n",string); \
} \
else if (memcmp(color,"yellow",6) == 0) \
{ \
  printf("\033[1;33m%s\033[0m\n",string); \
} \
else if (memcmp(color,"blue",4) == 0) \
{ \
  printf("\033[1;34m%s\033[0m\n",string); \
} \
else if (memcmp(color,"purple",6) == 0) \
{ \
  printf("\033[1;35m%s\033[0m\n",string); \
} \
else if (memcmp(color,"lightblue",9) == 0) \
{ \
  printf("\033[1;36m%s\033[0m\n",string); \
} \
else \
{ \
  printf("%s",string); \
}



/*
-----------------------------------------------------------------------------------------------------------
Name: print_error_message
Description: Prints all of the functions and error messages
-----------------------------------------------------------------------------------------------------------
*/

#define print_error_message \
printf("\n\n"); \
color_print(TEST_OUTLINE,"red"); \
printf("\033[1;31m%s: Error\033[0m\n",error_message.function[0]); \
get_current_UTC_time; \
printf("\033[1;31m%s\033[0m",asctime(current_UTC_date_and_time)); \
color_print(TEST_OUTLINE,"red"); \
printf("\033[1;31mFunction Calls:\033[0m\n"); \
for (*error_message_count = 0; *error_message_count < error_message.total; (*error_message_count)++) \
{ \
  printf("\033[1;31m#%d %s: %s\033[0m\n",*error_message_count+1,error_message.function[*error_message_count],error_message.data[*error_message_count]); \
} \
for (*error_message_count = 0; *error_message_count < DATABASE_ARRAY_COUNT; (*error_message_count)++) \
{ \
  memset(error_message.function[*error_message_count],0,strlen(error_message.function[*error_message_count])); \
  memset(error_message.data[*error_message_count],0,strlen(error_message.data[*error_message_count])); \
} \
error_message.total = 0;



/*
-----------------------------------------------------------------------------------------------------------
Name: print_start_message
Parameters:
  string - the message
Description: Prints the start message of a section
-----------------------------------------------------------------------------------------------------------
*/

#define print_start_message(string) \
printf("\n"); \
color_print(TEST_OUTLINE,"blue"); \
printf("\033[1;34m%s\033[0m\n",string); \
get_current_UTC_time; \
printf("\033[1;34m%s\033[0m",asctime(current_UTC_date_and_time)); \
color_print(TEST_OUTLINE,"blue");



/*
-----------------------------------------------------------------------------------------------------------
Name: get_current_UTC_time
Description: Gets the current UTC time
-----------------------------------------------------------------------------------------------------------
*/

#define get_current_UTC_time \
time(&current_date_and_time); \
current_UTC_date_and_time = gmtime(&current_date_and_time);



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_block_verifiers_minutes
Description: Syncs the block verifiers to a specific minute
-----------------------------------------------------------------------------------------------------------
*/

#define sync_block_verifiers_minutes(minutes) \
do \
{ \
  usleep(200000); \
  get_current_UTC_time; \
} while (current_UTC_date_and_time->tm_min % 5 != minutes); 



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_block_verifiers_seconds
Description: Syncs the block verifiers to a specific second
-----------------------------------------------------------------------------------------------------------
*/

#define sync_block_verifiers_seconds(seconds) \
do \
{ \
  usleep(200000); \
  get_current_UTC_time; \
} while (current_UTC_date_and_time->tm_sec % 30 != seconds);




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