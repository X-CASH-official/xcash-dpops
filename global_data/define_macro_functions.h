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
if (strncmp(color,"red",3) == 0) \
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
Name: append_string
Description: Appends strings securely to stop buffer overflows, and to always null terminate the string 
Parameters:
  string1 - char*
  string2 - char*
Return: Writes the correct code
-----------------------------------------------------------------------------------------------------------
*/

#define append_string(string1,string2) \
strncat(string1,string2,BUFFER_SIZE - strnlen(string1,BUFFER_SIZE) - 1);


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