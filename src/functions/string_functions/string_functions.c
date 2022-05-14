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
  // define macro
  #define PARSE_JSON_DATA_ERROR(_msg) \
  if (debug_settings == 1) \
  { \
    strcpy(error_message.function[error_message.total], __func__); \
    strcpy(error_message.data[error_message.total], _msg); \
    error_message.total++; \
  } \
  return 0;

  // Sanity check
  if (!DATA || !DATA[0] || !FIELD_NAME || !FIELD_NAME[0] || !result)
  {
    char message[BUFFER_SIZE];
    sprintf(message, "Invalid parameters passed in - DATA: %p, FIELD_NAME: %p, result: %p", DATA, FIELD_NAME, result);
    PARSE_JSON_DATA_ERROR(message);
  }

  // Constants
  const char* value_loc = NULL;

  // Variables
  bool database_doc = false;
  char substr[BUFFER_SIZE];
  size_t field_len = strlen(FIELD_NAME);
  field_len = field_len < BUFFER_SIZE - 5 ? field_len : BUFFER_SIZE - 5;

  // wrap the search field in quotes
  memset(substr, 0, sizeof(substr));
  strcpy(&substr[0], "\"");
  memcpy(&substr[1], FIELD_NAME, field_len);
  strcpy(&substr[field_len + 1], "\"");

  // Search for the parameter we need
  if ((value_loc = strstr(DATA, substr)) == NULL) {
    char message[BUFFER_SIZE];
    sprintf(message, "Cannot find FIELD_NAME '%s' (DATA is %40.40s)", FIELD_NAME, DATA);
    PARSE_JSON_DATA_ERROR(message);
  }

  // Jump forward to just past the quoted field name
  value_loc += field_len + 2;

  if (value_loc[0] == ':') {
    value_loc += 2; // skip past the ": "
  }
  else { // Something's wrong we found the field but no colon - let's see if we can find a message
    if ((value_loc = strstr(DATA, "\"message\": ")) == NULL) {
      char message[BUFFER_SIZE];
      sprintf(message, "Cannot find FIELD_NAME '%s' nor the \"message\" tag (DATA is %40.40s)", FIELD_NAME, DATA);
      PARSE_JSON_DATA_ERROR(message);
    }
    // ok, well, the message tag has been found - let's skip past it and grab the data
    value_loc += 11;
  }

  // grab the end of the line
  const char* value_end = strstr(value_loc, "\r");
  if (value_end == NULL) {
    char message[BUFFER_SIZE];
    sprintf(message, "Cannot find field data '%s' (DATA length is %ld)", FIELD_NAME, strlen(DATA));
    PARSE_JSON_DATA_ERROR(message);
  }

  // Calculate the length of the result data
  size_t value_len = value_end - value_loc;

  // Always strip the leading quote if there
  if (value_loc[0]=='"') {
    value_loc++;
    value_len--;
  }

  // Bounds check and limit if necessary
  value_len = value_len < RESULT_TOTAL_LENGTH ? value_len : RESULT_TOTAL_LENGTH;

  // Determine if this is a database doc (check for db keywords)
  if (value_len > 7) {
    const char *str1 = "username", *str2 = "total_vote_count", *str3 = "public_address_created_reserve_proof", *str4 = "reserve_bytes_data_hash";
    const size_t str1_len = 8, str2_len = 16, str3_len = 36, str4_len = 23;
    const size_t str1_limit = value_len - str1_len;
    const size_t str2_limit = value_len - str2_len;
    const size_t str3_limit = value_len - str3_len;
    const size_t str4_limit = value_len - str4_len;
    size_t pos = 0, search_len = value_len - 7;
    for (pos = 0; pos < search_len; pos++) {
      if ((pos < str1_limit && memcmp(&value_loc[pos], str1, str1_len) == 0) ||
          (pos < str2_limit && memcmp(&value_loc[pos], str2, str2_len) == 0) ||
          (pos < str3_limit && memcmp(&value_loc[pos], str3, str3_len) == 0) ||
          (pos < str4_limit && memcmp(&value_loc[pos], str4, str4_len) == 0) )
      {
        database_doc = true;
        break;
      }
    }
  }

  // Copy in final result
  if (database_doc)
    memcpy(result, value_loc, value_len); // Just a straight copy if it's a database document
  else
    value_len = remove_characters(value_loc, value_len, "\",[]{}", 6, result); // remove all the formating from the result 

  result[value_len] = 0; // Just in case we stray into uninitialized space

  #undef PARSE_JSON_DATA_ERROR

  return 1;
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
  if (!DATA || !STRING) { // Catch bad calls
    return 0;
  }

  size_t count = 0;
  size_t pos = 0;
  size_t str_len = strlen(STRING);
  size_t data_len = strlen(DATA);
  size_t comp_range = data_len - str_len + 1;

  if (data_len < comp_range) {
    return 0;
  }

  for (pos = 0; pos < comp_range; pos++) {
    if (memcmp(&DATA[pos], STRING, str_len) == 0) {
      count ++;
      pos += str_len;
    }
  }

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
    
  while((find = strstr(b, STR1)) != NULL)
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
  
  while((find = strstr(b, STR1)) != NULL)
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
  // define macro
  #define PARSE_RESERVE_ERROR(_msg) \
  if (debug_settings == 1) \
  { \
    strcpy(error_message.function[error_message.total], __func__); \
    strcpy(error_message.data[error_message.total], _msg); \
    error_message.total++; \
  } \
  return 0;

  // Sanity check
  if (!RESERVE_BYTES || !RESERVE_BYTES[0] || !result)
  {
    char message[BUFFER_SIZE];
    sprintf(message, "Invalid parameters passed in - RESERVE_BYTES: %p, result: %p", RESERVE_BYTES, result);
    PARSE_RESERVE_ERROR(message);
  }

  // Constants
  const size_t bc_ds_len = strlen(BLOCKCHAIN_DATA_SEGMENT_STRING);
  const char* start_segment = RESERVE_BYTES;

  // Variables
  size_t segment_len = 0;
  char* end_segment = NULL;
  int count = 0;
  int ds_count = string_count(RESERVE_BYTES, BLOCKCHAIN_DATA_SEGMENT_STRING);

  // error check
  if (ITEM > ds_count)
  {
    char message[BUFFER_SIZE];
    sprintf(message, "Blockchain Data Segment %d was requested, but there are only %d in the reserve bytes\n", ITEM, ds_count);
    PARSE_RESERVE_ERROR(message);
  }

  for (count = 0; start_segment != NULL && count < ITEM; count++) {
    start_segment = strstr(start_segment, BLOCKCHAIN_DATA_SEGMENT_STRING) + bc_ds_len;
  }

  if (start_segment == NULL)
  {
    char message[BUFFER_SIZE];
    sprintf(message, "Failed to find the %d blockchain data segment", count);
    PARSE_RESERVE_ERROR(message);
  }

  end_segment = strstr(start_segment, BLOCKCHAIN_DATA_SEGMENT_STRING);
  if (end_segment == NULL) {
    segment_len = strlen(start_segment);
  }
  else {
    segment_len = end_segment - start_segment;
  }

  memset(result, 0, strlen(result));
  memcpy(result, start_segment, segment_len);

  #undef PARSE_RESERVE_ERROR

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

/*
-----------------------------------------------------------------------------------------------------------
Name: remove_characters
Description: takes a character array and removes any single character found in that aray from the source,
             placing the newly stripped string into the target. The assumption is that the target has
			 enough room to hold the stripped string. This function does not null terminate.
Parameters:
  source - pointer to the string to be stripped (does not need to be null terminated)
  source_len - the length of the string to be stripped
  chars[] - the list of characters to be removed
  array_len - the size of the chars array
  target - the location where the stripped source string is written
Return: the length of the stripped string placed into target
-----------------------------------------------------------------------------------------------------------
*/


size_t remove_characters(const char* source, const size_t source_len, const char chars[], const size_t array_len, char* target)
{
  size_t offset = 0;
  size_t src_pos = 0;
  size_t tgt_pos = 0;

  for (src_pos = 0; src_pos < source_len; src_pos++) {
    for (offset = 0; offset < array_len; offset++) {
      if (source[src_pos] == chars[offset]) {
        src_pos++; // found one! Skip it
        offset=-1; // restart the search
      }
    }

    if (src_pos < source_len)
      target[tgt_pos++] = source[src_pos];
  }
  
  return tgt_pos;
}
