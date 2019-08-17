#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

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
  DATA - The json data to parse
  FIELD_NAME - The field you want to parse
  result - Where the result is stored
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int parse_json_data(const char* DATA, const char* FIELD_NAME, char *result)
{
  // Variables
  char str[BUFFER_SIZE];
  char* str1;
  char* str2;
  size_t start; 

  // reset the variables
  memset(result,0,strlen(result));
  memset(str,0,sizeof(str));

  // check if the field is in the data
  if (strstr(DATA,FIELD_NAME) != NULL)
  { 
    // modify the field to add the field syntax
    memcpy(str,"\"",1);
    memcpy(str+1,FIELD_NAME,strnlen(FIELD_NAME,BUFFER_SIZE));
    memcpy(str+strlen(str),"\": ",3);
    // get the start of the field's data
    start = strnlen(str,BUFFER_SIZE);
    // get the pointers location to the start of the field
    str1 = strstr(DATA,str);
    if (str1 == NULL)
    {
       // an error has occured, get the error message
       str1 = strstr(DATA,"\"message\": ");
       start = 11;
       if (str1 == NULL)
       {
         memcpy(error_message.function[error_message.total],"parse_json_data",15);
         memcpy(error_message.data[error_message.total],"Could not parse the message",27);
         error_message.total++;
         return 0;
       }
    }
    // get the end location of the data
    str2 = strstr(str1,"\r\n");
    // get the length of the field's data
    const int LENGTH = str2 - str1 - start;
    if (LENGTH <= 0)
    {
      memcpy(error_message.function[error_message.total],"parse_json_data",15);
      memcpy(error_message.data[error_message.total],"Could not parse the message",27);
      error_message.total++;
      return 0;
    }
    // copy the field's data
    memcpy(result,&str1[start],LENGTH);
 
    // remove all the formating from the result, if it is not a database document
    if (strstr(result,"username") == NULL && strstr(result,"total_vote_count") == NULL && strstr(result,"public_address_created_reserve_proof") == NULL && strstr(result,"reserve_bytes_data_hash") == NULL)
    {
      string_replace(result, "\"", "");
      string_replace(result, ",", "");
      string_replace(result, "[", "");
      string_replace(result, "]", "");
      string_replace(result, "{", "");
      string_replace(result, "}", "");
    }
    else
    {
      string_replace(result, "{ \"$numberInt\" : \"", "");
      string_replace(result, "{ \"$numberDouble\" : \"", "");
      string_replace(result, "{ \"$numberLong\" : \"", "");
      string_replace(result, ".0\" }", "");
      string_replace(result,"\"{\"","{\"");
    }
  }
  else
  {
    memcpy(error_message.function[error_message.total],"parse_json_data",15);
    memcpy(error_message.data[error_message.total],"Could not parse the message",27);
    error_message.total++;
    return 0;
  }  

  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: create_json_data_from_database_document_array
Description: Counts the occurences of a string
Parameters:
  database_data - A database_document_fields struct
  struct database_document_fields
    count - The number of items in the database document
    item[100] - The database document items
    value[100] - The database document values
  result - Where the result is stored
  document_fields - The document fields to not include in the json data
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int create_json_data_from_database_document_array(struct database_document_fields* database_data, char* result, const char* DOCUMENT_FIELDS)
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count = 0;
  size_t counter = 1;
  size_t item_length;
  size_t value_length;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    return 0;
  }

  memcpy(result,"{",1); 
  for (count = 0; count < database_data->count; count++)
  {
    memset(data,0,strlen(data));
    memcpy(data,database_data->item[count],strnlen(database_data->item[count],BUFFER_SIZE));
    memcpy(data+strlen(data),"|",1);
    if (strstr(DOCUMENT_FIELDS,data) == NULL)
    {
      // get the length of the item and the value
      item_length = strnlen(database_data->item[count],BUFFER_SIZE);
      value_length = strnlen(database_data->value[count],BUFFER_SIZE);
      // copy the item and the value to the json string
      memcpy(result+counter,"\"",1);
      counter++;
      memcpy(result+counter,database_data->item[count],item_length);
      counter += item_length;
      memcpy(result+counter,"\":\"",3);
      counter += 3; 
      memcpy(result+counter,database_data->value[count],value_length);
      counter += value_length;
      memcpy(result+counter,"\"",1);
      counter++;
      memcpy(result+counter,",",1);
      counter++;
    }
  }
  memcpy(result+counter-1,"}",1);
  pointer_reset(data);
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: create_json_data_from_database_multiple_documents_array
Description: Counts the occurences of a string
Parameters:
  database_data - A database_document_fields struct
  struct database_multiple_documents_fields
    document_count - The number of documents
    database_fields_count - The number of items in the database document
    item[100][100] - The database document items
    value[100][100] - The database document values
  result - Where the result is stored
  document_fields - The document fields to not include in the json data
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int create_json_data_from_database_multiple_documents_array(struct database_multiple_documents_fields* database_data, char* result, const char* DOCUMENT_FIELDS)
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count = 0;
  size_t counter = 0;
  size_t data_count = 1;
  size_t item_length;
  size_t value_length; 

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    return 0;
  }

  memcpy(result,"[",1); 
  
  for (count = 0; count < database_data->document_count; count++)
  {
    memcpy(result+data_count,"{",1); 
    data_count++;
    for (counter = 0; counter < database_data->database_fields_count; counter++)
    {
      memset(data,0,strlen(data));
      memcpy(data,database_data->item[count][counter],strnlen(database_data->item[count][counter],BUFFER_SIZE));
      memcpy(data+strlen(data),"|",1);
      if (strstr(DOCUMENT_FIELDS,data) == NULL)
      {
        // get the length of the item and the value
        item_length = strnlen(database_data->item[count][counter],BUFFER_SIZE);
        value_length = strnlen(database_data->value[count][counter],BUFFER_SIZE);
        // copy the item and the value to the json string
        memcpy(result+data_count,"\"",1);
        data_count++;
        memcpy(result+data_count,database_data->item[count][counter],item_length);
        data_count += item_length;
        memcpy(result+data_count,"\":\"",3);
        data_count += 3; 
        memcpy(result+data_count,database_data->value[count][counter],value_length);
        data_count += value_length;
        memcpy(result+data_count,"\"",1);
        data_count++;
        memcpy(result+data_count,",",1);
        data_count++;
      }      
    }
    memcpy(result+data_count-1,"},",2);
    data_count += 1;    
  }
  memcpy(result+data_count-1,"]",1);
  pointer_reset(data);
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: string_count
Description: Counts the occurences of a substring in a string
Parameters:
  DATA - The string to count the occurence in
  STRING - The substring to count the occurences of
Return: The number of occurences of the substring in the string, otherwise 0 if an error has occured
-----------------------------------------------------------------------------------------------------------
*/

size_t string_count(const char* DATA, const char* STRING)
{
  // Constants
  const size_t STRING_LENGTH = strlen(STRING);
  
  // Variables
  char* datacopy1 = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  // since were going to be changing where datacopy1 is referencing, we need to create a copy to pointer_reset
  char* datacopy2 = datacopy1; 
  size_t count = 0;

  // check if the memory needed was allocated on the heap successfully
  if (datacopy1 == NULL)
  {
    memcpy(error_message.function[error_message.total],"string_count",12);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
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
      memcpy(error_message.function[error_message.total],"string_replace",14);
      memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
      error_message.total++;
      print_error_message;  
      exit(0);
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
Name: parse_reserve_bytes_data
Description: Parses the reserve bytes data to read any item in the reserve bytes
Parameters:
  result - The result
  RESERVE_BYTES - The reserve bytes
  ITEM - The item to get the reserve bytes data for
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int parse_reserve_bytes_data(char *result, const char* RESERVE_BYTES, const int ITEM)
{  
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  // since were going to be changing where data is referencing, we need to create a copy to pointer_reset
  char* datacopy = data; 
  int count;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    memcpy(error_message.function[error_message.total],"parse_reserve_bytes_data",15);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  memcpy(data,RESERVE_BYTES,strnlen(RESERVE_BYTES,BUFFER_SIZE));

  // error check
  if (ITEM > (int)string_count(RESERVE_BYTES,BLOCKCHAIN_DATA_SEGMENT_STRING))
  {
    return 0;
  }

  for (count = 0; count < ITEM; count++)
  {
    data = strstr(data,BLOCKCHAIN_DATA_SEGMENT_STRING) + strlen(BLOCKCHAIN_DATA_SEGMENT_STRING);
  }  
  memset(result,0,strlen(result));
  memcpy(result,data,strnlen(data,BUFFER_SIZE) - strnlen(strstr(data,BLOCKCHAIN_DATA_SEGMENT_STRING),BUFFER_SIZE));

  pointer_reset(datacopy);
  return 1;
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
    memcpy(error_message.function[error_message.total],"random_string",13);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
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