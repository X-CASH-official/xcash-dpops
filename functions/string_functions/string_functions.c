#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"

#include "string_functions.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: parse_json_data
Description: Parses json data
Parameters:
  data - The json data to parse
  field - The field you want to parse
  result - Where the result is stored
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int parse_json_data(const char* DATA, const char* FIELD_NAME, char *result)
{
  // Constants
  const size_t STRING_LENGTH = strnlen(FIELD_NAME,BUFFER_SIZE);  
 
  // Variables
  char* str = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* str1;
  char* str2;
  int settings = 1;
  size_t start; 

  // check if the memory needed was allocated on the heap successfully
  if (str == NULL)
  {
    return 0;
  }

  // check if the field is in the data
  if (strstr(DATA,FIELD_NAME) != NULL)
  { 
    // modify the field to add the field syntax
    memcpy(str,"\"",1);
    memcpy(str+1,FIELD_NAME,STRING_LENGTH);
    memcpy(str+1+STRING_LENGTH,"\": ",3);
    // get the start of the field's data
    start = strnlen(str,BUFFER_SIZE);
    // get the pointers location to the start of the field
    str1 = strstr(DATA,str);
    if (str1 == NULL)
    {
       // an error has occured, get the error message
       settings = 0;
       str1 = strstr(DATA,"\"message\": ");
       start = 11;
       if (str1 == NULL)
       {
         // their is no error message
         memcpy(result,"An error has occured",20);
         pointer_reset(str);
         return settings;
       }
    }
    // get the end location of the data
    str2 = strstr(str1,"\r\n");
    // get the length of the field's data
    const int LENGTH = str2 - str1 - start;
    if (LENGTH <= 0)
    {
      memcpy(result,"An error has occured",20);
      pointer_reset(str);
      return settings;
    }
    // copy the field's data
    memcpy(result,&str1[start],LENGTH);
 
    // remove all the formating from the result
    string_replace(result, "\"", "");
    string_replace(result, ",", "");
    string_replace(result, "[", "");
    string_replace(result, "]", "");
    string_replace(result, "{", "");
    string_replace(result, "}", "");
  }

  pointer_reset(str);  
  return settings;
}




/*
-----------------------------------------------------------------------------------------------------------
Name: string_count
Description: Counts the occurences of a substring in a string
Parameters:
  data - The string to count the occurence in
  string - The substring to count the occurences of
Return: The number of occurences of the substring in the string, otherwise 0 if an error has occured
-----------------------------------------------------------------------------------------------------------
*/

size_t string_count(const char* DATA, const char* STRING)
{
  // Constants
  const size_t STRING_LENGTH = strnlen(STRING,BUFFER_SIZE);
  
  // Variables
  char* datacopy1 = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  // since were going to be changing where datacopy1 is referencing, we need to create a copy to pointer_reset
  char* datacopy2 = datacopy1; 
  size_t count = 0;

  // check if the memory needed was allocated on the heap successfully
  if (datacopy1 == NULL)
  {
    return 0;
  }

  // get the occurences of the string 
  memcpy(datacopy1,DATA,strnlen(DATA,BUFFER_SIZE));
  while((datacopy1 = strstr(datacopy1, STRING)) != NULL)
  {
    count++;
    datacopy1+= STRING_LENGTH;
  } 

  pointer_reset(datacopy2);
  return count;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: string_replace
Description: String replace
Parameters:
  data - The string to replace the data
  STR1 - The string to be replaced
  STR2 - The string to replace the other string
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int string_replace(char *data, const char* STR1, const char* STR2)
{  
  // check if the str to replace exist in the string
  if (strstr(data,STR1) != NULL)
  { 
    // Variables
    char* datacopy = (char*)calloc(BUFFER_SIZE,sizeof(char));
    char* string;
    size_t data_length;
    size_t str2_length;
    size_t start;
    size_t total = 0;
    size_t count = 0; 

    // define macros
    #define REPLACE_STRING "|REPLACE_STRING|" 

    // check if the memory needed was allocated on the heap successfully
    if (datacopy == NULL)
    {
      return 0;
    } 

    // get the occurences of STR1   
    total = string_count(data,(char*)STR1);

    // replace the string with the REPLACE_STRING
    for (count = 0; count < total; count++)
    {
      // reset the variables
      memset(datacopy,0,strnlen(datacopy,BUFFER_SIZE));
      data_length = strnlen(data,BUFFER_SIZE);
      str2_length = strnlen(REPLACE_STRING,BUFFER_SIZE);
      start = data_length - strnlen(strstr(data,STR1),BUFFER_SIZE);
   
      // get a pointer to where the rest of the data string should be copied to
      string = strstr(data,STR1)+strnlen(STR1,BUFFER_SIZE);
           
      // copy the bytes before STR1 to the new string
      memcpy(datacopy,data,start);
      // copy STR2 to the new string
      memcpy(datacopy+start,REPLACE_STRING,str2_length);
      // copy the bytes after STR1 to the new string
      memcpy(datacopy+start+str2_length,string,strnlen(string,BUFFER_SIZE));
      // copy the new string to the string pointer
      memset(data,0,data_length);
      memcpy(data,datacopy,strnlen(datacopy,BUFFER_SIZE));
    }
    // replace the REPLACE_STRING with STR2
    for (count = 0; count < total; count++)
    {
      // reset the variables
      memset(datacopy,0,strnlen(datacopy,BUFFER_SIZE));
      data_length = strnlen(data,BUFFER_SIZE);
      str2_length = strnlen(STR2,BUFFER_SIZE);
      start = data_length - strnlen(strstr(data,REPLACE_STRING),BUFFER_SIZE);
   
      // get a pointer to where the rest of the data string should be copied to
      string = strstr(data,REPLACE_STRING)+strnlen(REPLACE_STRING,BUFFER_SIZE);
           
      // copy the bytes before REPLACE_STRING to the new string
      memcpy(datacopy,data,start);
      // copy STR2 to the new string
      memcpy(datacopy+start,STR2,str2_length);
      // copy the bytes after REPLACE_STRING to the new string
      memcpy(datacopy+start+str2_length,string,strnlen(string,BUFFER_SIZE));
      // copy the new string to the string pointer
      memset(data,0,data_length);
      memcpy(data,datacopy,strnlen(datacopy,BUFFER_SIZE));
    }
    pointer_reset(datacopy);
    return 1;
  }
  else
  {
    return 0;
  } 

  #undef REPLACE_STRING
}




/*
-----------------------------------------------------------------------------------------------------------
Name: random_string
Description: Creates a random string of specified length
Parameters:
  result - The string where you want the random string to be saved to
  LENGTH - The length of the random string
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int random_string(char *result, const size_t LENGTH)
{  
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;

  // define macros
  #define STRING "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789" 
  #define MINIMUM 0
  #define MAXIMUM 61

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    return 0;
  }
  
  memcpy(data,STRING,62);
  for (count = 0; count < LENGTH; count++)
  {
    memcpy(result+count,&data[((rand() % (MAXIMUM - MINIMUM + 1)) + MINIMUM)],1);
  }
  pointer_reset(data); 
  return 1;
  
  #undef STRING
  #undef MINIMUM
  #undef MAXIMUM  
}