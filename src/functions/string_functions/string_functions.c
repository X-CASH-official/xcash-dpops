#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/random.h>
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

int parse_json_data(const char* DATA, const char* FIELD_NAME, char *result, const size_t RESULT_TOTAL_LENGTH)
{
  // Constants
  const size_t MAXIMUM_AMOUNT = RESULT_TOTAL_LENGTH >= MAXIMUM_BUFFER_SIZE ? MAXIMUM_BUFFER_SIZE : RESULT_TOTAL_LENGTH+BUFFER_SIZE;

  // Variables
  char str[BUFFER_SIZE];
  char* message = (char*)calloc(MAXIMUM_AMOUNT,sizeof(char)); 
  char* str1;
  char* str2;
  size_t start;

  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;


  // define macros
  #define PARSE_JSON_DATA_ERROR(info) \
  if (debug_settings == 1) \
  { \
  memcpy(error_message.function[error_message.total],"parse_json_data",15); \
  snprintf(error_message.data[error_message.total],sizeof(error_message.data),"Could not parse the message: %s", info); \
  error_message.total++; \
  print_error_message(current_date_and_time,current_UTC_date_and_time,str);\
  } \
  pointer_reset(message); \
  return 0;

  // reset the variables
  memset(result,0,strlen(result));
  memset(str,0,sizeof(str));

  memcpy(str,"\"",sizeof(char));
  memcpy(str+1,FIELD_NAME,strnlen(FIELD_NAME,sizeof(str)));
  memcpy(str+strlen(str),"\"",sizeof(char));

  // check if the field is in the data
  if (strstr(DATA,str) != NULL)
  { 
    memset(str,0,sizeof(str));

    // modify the field to add the field syntax
    memcpy(str,"\"",sizeof(char));
    memcpy(str+1,FIELD_NAME,strnlen(FIELD_NAME,sizeof(str)));
    memcpy(str+strlen(str),"\": ",3);

    // get the start of the field's data
    start = strnlen(str,sizeof(str));
    
    // get the pointers location to the start of the field
    if ((str1 = strstr(DATA,str)) == NULL)
    {
       // an error has occured, get the error message
       str1 = strstr(DATA,"\"message\": ");
       start = 11;
       if (str1 == NULL)
       {
         PARSE_JSON_DATA_ERROR("No 'message' tag");
       }
    }

    // get the end location of the data
    if ((str2 = strstr(str1,"\r\n")) == NULL)
    {
      PARSE_JSON_DATA_ERROR("no \\r\\n at the end");
    }
    
    // get the length of the field's data
    const size_t LENGTH = str2 - str1 - start;
    if (LENGTH <= 0)
    {
      PARSE_JSON_DATA_ERROR("empty field");
    }

    // copy the field's data
    memcpy(message,&str1[start],LENGTH);

    // remove all the formating from the result, if it is not a database document
    if (strstr(message,"username") == NULL && strstr(message,"total_vote_count") == NULL && strstr(message,"public_address_created_reserve_proof") == NULL && strstr(message,"reserve_bytes_data_hash") == NULL)
    {
      string_replace(message,MAXIMUM_AMOUNT,"\"","");
      string_replace(message,MAXIMUM_AMOUNT,",","");
      string_replace(message,MAXIMUM_AMOUNT,"[","");
      string_replace(message,MAXIMUM_AMOUNT,"]","");
      string_replace(message,MAXIMUM_AMOUNT,"{","");
      string_replace(message,MAXIMUM_AMOUNT,"}","");
    }
    else
    {
      string_replace(message,MAXIMUM_AMOUNT,"\"{\"","{\"");
    }
    memcpy(result,message,strnlen(message,RESULT_TOTAL_LENGTH));
  }
  else
  {
    char err_str[1000];
    snprintf(err_str,1000,"Field %s not in the message", FIELD_NAME);
    PARSE_JSON_DATA_ERROR(err_str);
  }
  pointer_reset(message);
  return 1;

  #undef PARSE_JSON_DATA_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: create_json_data_from_database_document_array
Description: Create json data from a database_document struct
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

int create_json_data_from_database_document_array(const struct database_document_fields* database_data, char *result, const char* DOCUMENT_FIELDS)
{
  // Variables
  char data[BUFFER_SIZE];
  size_t count = 0;
  size_t counter = 1;
  size_t item_length;
  size_t value_length;

  memcpy(result,"{",sizeof(char)); 
  for (count = 0; count < database_data->count; count++)
  {
    memset(data,0,sizeof(data));
    memcpy(data,database_data->item[count],strnlen(database_data->item[count],sizeof(data)));
    memcpy(data+strlen(data),"|",sizeof(char));
    if (strstr(DOCUMENT_FIELDS,data) == NULL)
    {
      // get the length of the item and the value
      item_length = strnlen(database_data->item[count],sizeof(data));
      value_length = strnlen(database_data->value[count],sizeof(data));
      // copy the item and the value to the json string
      memcpy(result+counter,"\"",sizeof(char));
      counter++;
      memcpy(result+counter,database_data->item[count],item_length);
      counter += item_length;
      memcpy(result+counter,"\":\"",3);
      counter += 3; 
      memcpy(result+counter,database_data->value[count],value_length);
      counter += value_length;
      memcpy(result+counter,"\"",sizeof(char));
      counter++;
      memcpy(result+counter,",",sizeof(char));
      counter++;
    }
  }
  memcpy(result+counter-1,"}",sizeof(char));
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: create_json_data_from_database_multiple_documents_array
Description: Create json data from a database_multiple_documents struct
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

int create_json_data_from_database_multiple_documents_array(const struct database_multiple_documents_fields* database_data, char *result, const char* DOCUMENT_FIELDS)
{
  // Variables
  char data[BUFFER_SIZE];
  size_t count = 0;
  size_t counter;
  size_t data_count = 1;
  size_t item_length;
  size_t value_length; 

  memcpy(result,"[",sizeof(char)); 
  
  for (count = 0; count < database_data->document_count; count++)
  {
    memcpy(result+data_count,"{",sizeof(char)); 
    data_count++;
    for (counter = 0; counter < database_data->database_fields_count; counter++)
    {
      memset(data,0,sizeof(data));
      memcpy(data,database_data->item[count][counter],strnlen(database_data->item[count][counter],sizeof(data)));
      memcpy(data+strlen(data),"|",sizeof(char));
      if (strstr(DOCUMENT_FIELDS,data) == NULL)
      {
        // get the length of the item and the value
        item_length = strnlen(database_data->item[count][counter],sizeof(data));
        value_length = strnlen(database_data->value[count][counter],sizeof(data));
        // copy the item and the value to the json string
        memcpy(result+data_count,"\"",sizeof(char));
        data_count++;
        memcpy(result+data_count,database_data->item[count][counter],item_length);
        data_count += item_length;
        memcpy(result+data_count,"\":\"",3);
        data_count += 3; 
        memcpy(result+data_count,database_data->value[count][counter],value_length);
        data_count += value_length;
        memcpy(result+data_count,"\"",sizeof(char));
        data_count++;
        memcpy(result+data_count,",",sizeof(char));
        data_count++;
      }      
    }
    memcpy(result+data_count-1,"},",2);
    data_count += 1;    
  }
  memcpy(result+data_count-1,"]",sizeof(char));
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: create_json_data_from_database_multiple_documents_array
Description: Create json data from a delegates struct
Parameters:
  delegates - A delegates struct
  result - Where the result is stored
  document_fields - The document fields to not include in the json data
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int create_json_data_from_delegates_array(const struct delegates* delegates, char *result, const char* DOCUMENT_FIELDS)
{
  // Variables
  int count = 0;
  
  memset(result,0,strlen(result));
  memcpy(result,"[",sizeof(char)); 
  
  for (count = 0; count < MAXIMUM_AMOUNT_OF_DELEGATES; count++)
  {
    if (strncmp(delegates[count].public_address,"",1) != 0)
    {
      memcpy(result+strlen(result),"{",sizeof(char));
      if (strstr(DOCUMENT_FIELDS,"public_address|") == NULL)
      {
        memcpy(result+strlen(result),"\"public_address\":\"",18);
        memcpy(result+strlen(result),delegates[count].public_address,strlen(delegates[count].public_address));
        memcpy(result+strlen(result),"\",",2);
      }
      if (strstr(DOCUMENT_FIELDS,"total_vote_count|") == NULL)
      {
        memcpy(result+strlen(result),"\"total_vote_count\":\"",20);
        memcpy(result+strlen(result),delegates[count].total_vote_count,strlen(delegates[count].total_vote_count));
        memcpy(result+strlen(result),"\",",2);
      }
      if (strstr(DOCUMENT_FIELDS,"IP_address|") == NULL)
      {
        memcpy(result+strlen(result),"\"IP_address\":\"",14);
        memcpy(result+strlen(result),delegates[count].IP_address,strlen(delegates[count].IP_address));
        memcpy(result+strlen(result),"\",",2);
      }
      if (strstr(DOCUMENT_FIELDS,"delegate_name|") == NULL)
      {
        memcpy(result+strlen(result),"\"delegate_name\":\"",17);
        memcpy(result+strlen(result),delegates[count].delegate_name,strlen(delegates[count].delegate_name));
        memcpy(result+strlen(result),"\",",2);
      }
      if (strstr(DOCUMENT_FIELDS,"about|") == NULL)
      {
        memcpy(result+strlen(result),"\"about\":\"",9);
        memcpy(result+strlen(result),delegates[count].about,strlen(delegates[count].about));
        memcpy(result+strlen(result),"\",",2);
      }
      if (strstr(DOCUMENT_FIELDS,"website|") == NULL)
      {
        memcpy(result+strlen(result),"\"website\":\"",11);
        memcpy(result+strlen(result),delegates[count].website,strlen(delegates[count].website));
        memcpy(result+strlen(result),"\",",2);
      }
      if (strstr(DOCUMENT_FIELDS,"team|") == NULL)
      {
        memcpy(result+strlen(result),"\"team\":\"",8);
        memcpy(result+strlen(result),delegates[count].team,strlen(delegates[count].team));
        memcpy(result+strlen(result),"\",",2);
      }
      if (strstr(DOCUMENT_FIELDS,"shared_delegate_status|") == NULL)
      {
        memcpy(result+strlen(result),"\"shared_delegate_status\":\"",26);
        memcpy(result+strlen(result),delegates[count].shared_delegate_status,strlen(delegates[count].shared_delegate_status));
        memcpy(result+strlen(result),"\",",2);
      }
      if (strstr(DOCUMENT_FIELDS,"delegate_fee|") == NULL)
      {
        memcpy(result+strlen(result),"\"delegate_fee\":\"",16);
        memcpy(result+strlen(result),delegates[count].delegate_fee,strlen(delegates[count].delegate_fee));
        memcpy(result+strlen(result),"\",",2);
      }
      if (strstr(DOCUMENT_FIELDS,"server_specs|") == NULL)
      {
        memcpy(result+strlen(result),"\"server_specs\":\"",16);
        memcpy(result+strlen(result),delegates[count].server_specs,strlen(delegates[count].server_specs));
        memcpy(result+strlen(result),"\",",2);
      }
      if (strstr(DOCUMENT_FIELDS,"block_verifier_score|") == NULL)
      {
        memcpy(result+strlen(result),"\"block_verifier_score\":\"",24);
        memcpy(result+strlen(result),delegates[count].block_verifier_score,strlen(delegates[count].block_verifier_score));
        memcpy(result+strlen(result),"\",",2);
      }
      if (strstr(DOCUMENT_FIELDS,"online_status|") == NULL)
      {
        memcpy(result+strlen(result),"\"online_status\":\"",17);
        memcpy(result+strlen(result),delegates[count].online_status,strlen(delegates[count].online_status));
        memcpy(result+strlen(result),"\",",2);
      }
      if (strstr(DOCUMENT_FIELDS,"block_verifier_total_rounds|") == NULL)
      {
        memcpy(result+strlen(result),"\"block_verifier_total_rounds\":\"",31);
        memcpy(result+strlen(result),delegates[count].block_verifier_total_rounds,strlen(delegates[count].block_verifier_total_rounds));
        memcpy(result+strlen(result),"\",",2);
      }
      if (strstr(DOCUMENT_FIELDS,"block_verifier_online_total_rounds|") == NULL)
      {
        memcpy(result+strlen(result),"\"block_verifier_online_total_rounds\":\"",38);
        memcpy(result+strlen(result),delegates[count].block_verifier_online_total_rounds,strlen(delegates[count].block_verifier_online_total_rounds));
        memcpy(result+strlen(result),"\",",2);
      }
      if (strstr(DOCUMENT_FIELDS,"block_verifier_online_percentage|") == NULL)
      {
        memcpy(result+strlen(result),"\"block_verifier_online_percentage\":\"",36);
        memcpy(result+strlen(result),delegates[count].block_verifier_online_percentage,strlen(delegates[count].block_verifier_online_percentage));
        memcpy(result+strlen(result),"\",",2);
      }
      if (strstr(DOCUMENT_FIELDS,"block_producer_total_rounds|") == NULL)
      {
        memcpy(result+strlen(result),"\"block_producer_total_rounds\":\"",31);
        memcpy(result+strlen(result),delegates[count].block_producer_total_rounds,strlen(delegates[count].block_producer_total_rounds));
        memcpy(result+strlen(result),"\",",2);
      }
      if (strstr(DOCUMENT_FIELDS,"block_producer_block_heights|") == NULL)
      {
        memcpy(result+strlen(result),"\"block_producer_block_heights\":\"",32);
        memcpy(result+strlen(result),delegates[count].block_producer_block_heights,strlen(delegates[count].block_producer_block_heights));
        memcpy(result+strlen(result),"\",",2);
      }
      memcpy(result+strlen(result)-1,"},",2);
    }
  }
  memcpy(result+strlen(result)-1,"]",sizeof(char));
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: create_json_data_from_votes_array
Description: Create json data from a votes struct
Parameters:
  votes - A votes struct
  result - Where the result is stored
  document_fields - The document fields to not include in the json data
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int create_json_data_from_votes_array(const struct votes* votes, char *result, const char* DOCUMENT_FIELDS)
{
  // Variables
  int count = 0;
  
  memset(result,0,strlen(result));
  memcpy(result,"[",sizeof(char)); 
  
  for (count = 0; count < MAXIMUM_AMOUNT_OF_DELEGATES; count++)
  {
    if (strncmp(votes[count].public_address_created_reserve_proof,"",1) != 0)
    {
      memcpy(result+strlen(result),"{",sizeof(char));
      if (strstr(DOCUMENT_FIELDS,"public_address_created_reserve_proof|") == NULL)
      {
        memcpy(result+strlen(result),"\"public_address_created_reserve_proof\":\"",40);
        memcpy(result+strlen(result),votes[count].public_address_created_reserve_proof,strlen(votes[count].public_address_created_reserve_proof));
        memcpy(result+strlen(result),"\",",2);
      }
      if (strstr(DOCUMENT_FIELDS,"public_address_voted_for|") == NULL)
      {
        memcpy(result+strlen(result),"\"public_address_voted_for\":\"",28);
        memcpy(result+strlen(result),votes[count].public_address_voted_for,strlen(votes[count].public_address_voted_for));
        memcpy(result+strlen(result),"\",",2);
      }
      if (strstr(DOCUMENT_FIELDS,"total|") == NULL)
      {
        memcpy(result+strlen(result),"\"total\":\"",9);
        memcpy(result+strlen(result),votes[count].total,strlen(votes[count].total));
        memcpy(result+strlen(result),"\",",2);
      }
      if (strstr(DOCUMENT_FIELDS,"reserve_proof|") == NULL)
      {
        memcpy(result+strlen(result),"\"reserve_proof\":\"",17);
        memcpy(result+strlen(result),votes[count].reserve_proof,strlen(votes[count].reserve_proof));
        memcpy(result+strlen(result),"\",",2);
      }
      memcpy(result+strlen(result)-1,"},",2);
    }
  }
  memcpy(result+strlen(result)-1,"]",sizeof(char));
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
  const size_t MAXIMUM_AMOUNT = strlen(DATA) >= MAXIMUM_BUFFER_SIZE ? MAXIMUM_BUFFER_SIZE : strlen(DATA)+SMALL_BUFFER_SIZE;

  // Variables
  char buffer[1024];
  char* datacopy1 = (char*)calloc(MAXIMUM_AMOUNT,sizeof(char)); 
  // since were going to be changing where datacopy1 is referencing, we need to create a copy to pointer_reset
  char* datacopy2 = datacopy1; 
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  size_t count = 0;

  // check if the memory needed was allocated on the heap successfully
  if (datacopy1 == NULL)
  {
    memcpy(error_message.function[error_message.total],"string_count",12);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,buffer);  
    exit(0);
  }

  // get the occurences of the string 
  memcpy(datacopy1,DATA,strnlen(DATA,MAXIMUM_AMOUNT));
  while((datacopy1 = strstr(datacopy1, STRING)) != NULL)
  {
    count++;
    datacopy1+= strlen(STRING);
  } 

  pointer_reset(datacopy2);
  return count;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: parse_http_response
Description: Parses a http response for the response body
Parameters:
  result - The data to parse, and will return the parsed data
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int parse_http_response(char *result)
{
  // Variables
  char* data;
  char str[BUFFER_SIZE];

  // reset the variables
  memset(str,0,sizeof(str));

  data = strstr(result,"[") != NULL ? strstr(result,"[") : strstr(result,"{");

  if (data == NULL)
  {
    return 0;
  }
  
  memcpy(str,data,strnlen(data,sizeof(str)));
  memset(result,0,strlen(result));
  memcpy(result,str,strlen(str));
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: string_replace
Description: String replace
Parameters:
  data - The string to replace the data
  DATA_TOTAL_LENGTH - The maximum size of data
  STR1 - The string to be replaced
  STR2 - The string to replace the other string
Return: The result string
-----------------------------------------------------------------------------------------------------------
*/

void string_replace(char *data, const size_t DATA_TOTAL_LENGTH, const char* STR1, const char* STR2)
{ 
  // Constants
  const size_t slen = strlen(STR1);
  const size_t rlen = strlen(STR2);

  // Variables  
  char* buf = calloc((strlen(data) + 1)*2,sizeof(char));
  char* find;
  char* b = data;
    
  if (buf == NULL || slen == 0)
  {
    pointer_reset(buf);
    return;
  }
    
  while((find = strstr(b, STR1)))
  {   
    memcpy(buf+strlen(buf), b, find - b);  /* copy up to occurrence */
    memcpy(buf+strlen(buf), STR2, rlen);   /* add replacement */
    b = find + slen;
  }
  // copy the end of the string
  memcpy(buf+strlen(buf), b, strlen(b));
   
  // replace the original string with the new string
  memset(data,0,strlen(data));
  memcpy(data,buf,strnlen(buf,DATA_TOTAL_LENGTH));
    
  pointer_reset(buf);
  return;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: string_replace
Description: String replace only a specific amount of string occurences
Parameters:
  data - The string to replace the data
  DATA_TOTAL_LENGTH - The maximum size of data
  STR1 - The string to be replaced
  STR2 - The string to replace the other string
  COUNT - The number of string occurences to replace
Return: The result string
-----------------------------------------------------------------------------------------------------------
*/

void string_replace_limit(char *data, const size_t DATA_TOTAL_LENGTH, const char* STR1, const char* STR2, const int COUNT)
{ 
  // Constants
  const size_t slen = strlen(STR1);
  const size_t rlen = strlen(STR2);
  int count2 = 0;

  // Variables  
  char* buf = calloc((strlen(data) + 1)*2,sizeof(char));
  char* find;
  char* b = data;
    
  if (buf == NULL || slen == 0)
  {
    pointer_reset(buf);
    return;
  }
  
  while((find = strstr(b, STR1)))
  {
    if (count2 == COUNT)
    {
      break;
    }
    memcpy(buf+strlen(buf), b, find - b);  /* copy up to occurrence */
    memcpy(buf+strlen(buf), STR2, rlen);   /* add replacement */
    b = find + slen;
    count2++;
  }
  // copy the end of the string
  memcpy(buf+strlen(buf), b, strlen(b));
   
  // replace the original string with the new string
  memset(data,0,strlen(data));
  memcpy(data,buf,strnlen(buf,DATA_TOTAL_LENGTH));
    
  pointer_reset(buf);
  return;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: check_for_invalid_strings
Description: Checks for invalid strings
Parameters:
  MESSAGE - The message
Return: 0 if the string is not valid, 1 if the string is valid
-----------------------------------------------------------------------------------------------------------
*/

int check_for_invalid_strings(const char* MESSAGE)
{
  return strstr(MESSAGE,"\"") == NULL && strstr(MESSAGE,",") == NULL && strstr(MESSAGE,":") == NULL ? 1 : 0;
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
  // Constants
  const size_t MAXIMUM_AMOUNT = strlen(RESERVE_BYTES) >= MAXIMUM_BUFFER_SIZE ? MAXIMUM_BUFFER_SIZE : strlen(RESERVE_BYTES)+SMALL_BUFFER_SIZE;

  // Variables
  char buffer[1024];
  char* data = (char*)calloc(MAXIMUM_AMOUNT,sizeof(char));
  // since were going to be changing where data is referencing, we need to create a copy to pointer_reset
  char* datacopy = data; 
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    memcpy(error_message.function[error_message.total],"parse_reserve_bytes_data",15);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,buffer);  
    exit(0);
  }

  memcpy(data,RESERVE_BYTES,strnlen(RESERVE_BYTES,MAXIMUM_AMOUNT));

  // error check
  if (ITEM > (int)string_count(RESERVE_BYTES,BLOCKCHAIN_DATA_SEGMENT_STRING))
  {
    pointer_reset(datacopy);
    return 0;
  }

  for (count = 0; count < ITEM; count++)
  {
    data = strstr(data,BLOCKCHAIN_DATA_SEGMENT_STRING) + strlen(BLOCKCHAIN_DATA_SEGMENT_STRING);
  }  
  memset(result,0,strlen(result));
  memcpy(result,data,strnlen(data,MAXIMUM_AMOUNT) - strnlen(strstr(data,BLOCKCHAIN_DATA_SEGMENT_STRING),MAXIMUM_AMOUNT));

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
  // define macros
  #define STRING "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
  #define MAXIMUM_COUNT 1000
  
  // Variables
  char data2[BUFFER_SIZE];
  int count;
  int count2;
  int count3 = 0;

  memset(result,0,strlen(result));

  do
  {
    memset(data2,0,sizeof(data2));
    
    count2 = (int)getrandom(data2,sizeof(data2)-1,0);

    // only use specific bytes
    for (count = 0; count < count2; count++)
    {
      if (strstr(STRING,&data2[count]) != NULL)
      {
        memcpy(result+strlen(result),&data2[count],sizeof(char));
      }

      // check if the random string is at the maximum number of bytes
      if (strlen(result) == LENGTH)
      {
        break;
      }
    }
    count3++;
  } while (strlen(result) != LENGTH && count3 != MAXIMUM_COUNT);

  return strlen(result) == LENGTH ? 1 : 0;

  #undef STRING
  #undef MAXIMUM_COUNT
}