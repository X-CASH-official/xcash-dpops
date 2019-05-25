#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "variables.h"

#include "define_macros_functions.h"
#include "network_functions.h"
#include "network_wallet_functions.h"
#include "string_functions.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: get_public_address
Description: Gets the public address of your wallet
Parameters:
  MESSAGE_SETTINGS - 1 to print the messages, otherwise 0. This is used for the testing flag to not print any success or error messages
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int get_public_address(const int MESSAGE_SETTINGS)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // define macros
  #define GET_PUBLIC_ADDRESS_DATA "{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_address\"}"

  if (data == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }


  if (send_http_request(data,"127.0.0.1","/json_rpc",XCASH_WALLET_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,GET_PUBLIC_ADDRESS_DATA,RECEIVE_DATA_TIMEOUT_SETTINGS,"get public address",MESSAGE_SETTINGS) <= 0)
  {  
    pointer_reset(data);   
    return 0;
  }
  
  if (parse_json_data(data,"address",xcash_wallet_public_address) == 0)
  {
    pointer_reset(data); 
    return 0;
  }
  
  // check if the returned data is valid
  if (strnlen(xcash_wallet_public_address,BUFFER_SIZE) != XCASH_WALLET_LENGTH && strncmp(xcash_wallet_public_address,XCASH_WALLET_PREFIX,3) != 0)
  {
     pointer_reset(data); 
     return 0;
  }
  
  pointer_reset(data); 
  return 1;

  #undef GET_PUBLIC_ADDRESS_DATA
}



/*
-----------------------------------------------------------------------------------------------------------
Name: data_verify
Description: Verifies data
Parameters:
  MESSAGE_SETTINGS - 1 to print the messages, otherwise 0. This is used for the testing flag to not print any success or error messages
  PUBLIC_ADDRESS - The public address
  DATA_SIGNATURE - The data signature
  DATA - The data
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int data_verify(const int MESSAGE_SETTINGS, const char* PUBLIC_ADDRESS, const char* DATA_SIGNATURE, const char* DATA)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  if (data == NULL || data2 == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // create the message
  memcpy(data2,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"verify\",\"params\":{\"data\":\"",62);
  memcpy(data2+strlen(data2),DATA,strlen(DATA));
  memcpy(data2+strlen(data2),"\",\"address\":\"",13);
  memcpy(data2+strlen(data2),PUBLIC_ADDRESS,strlen(PUBLIC_ADDRESS));
  memcpy(data2+strlen(data2),"\",\"signature\":\"",15);
  memcpy(data2+strlen(data2),DATA_SIGNATURE,strlen(DATA_SIGNATURE));
  memcpy(data2+strlen(data2),"\"}}",3);

  if (send_http_request(data,"127.0.0.1","/json_rpc",XCASH_WALLET_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,data2,RECEIVE_DATA_TIMEOUT_SETTINGS,"verify data",MESSAGE_SETTINGS) <= 0)
  {  
    pointer_reset_all;
    return 0;
  }
  
  if (strstr(data,"\"good\": true") == NULL)
  {
    pointer_reset_all;
    return 0;
  }
  
  pointer_reset_all;
  return 1;

  #undef pointer_reset_all
}



/*
-----------------------------------------------------------------------------------------------------------
Name: check_reserve_proof
Description: Checks a reserve proof
Parameters:
  result - The amount for the reserve proof
  public_address - The public address that created the reserve proof
  reserve_proof - The reserve proof
  HTTP_SETTINGS - 1 to print the messages, otherwise 0. This is used for the testing flag to not print any success or error messages
Return: 0 if an error has occured or if the reserve proof is invalid, 1 if the reserve proof is valid
-----------------------------------------------------------------------------------------------------------
*/

int check_reserve_proofs(char *result, char* public_address, char* reserve_proof, const int HTTP_SETTINGS)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);
  const size_t RESERVE_PROOF_LENGTH = strnlen(reserve_proof,BUFFER_SIZE);

  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;
  size_t counter;

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  if (data == NULL || data2 == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // create the message
  memcpy(data,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"check_reserve_proof\",\"params\":{\"address\":\"",78);
  memcpy(data+78,public_address,XCASH_WALLET_LENGTH);
  memcpy(data+176,"\",\"message\":\"\",\"signature\":\"",28);
  memcpy(data+204,reserve_proof,RESERVE_PROOF_LENGTH);
  memcpy(data+204+RESERVE_PROOF_LENGTH,"\"}}",3);

  if (send_http_request(data2,"127.0.0.1","/json_rpc",XCASH_WALLET_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,data,RECEIVE_DATA_TIMEOUT_SETTINGS,"check reserve proof",HTTP_SETTINGS) <= 0)
  {  
    pointer_reset_all;   
    return 0;
  }

  // check if the reserve proof is valid
  if (strstr(data2,"\"good\": true") == NULL || strstr(data2,"\"spent\": 0") == NULL)
  {  
    pointer_reset_all;   
    return 0;
  }

  // parse the message
  if (parse_json_data(data2,"total",result) == 0)
  {
    pointer_reset_all;   
    return 0;
  }
  
  pointer_reset_all;
  return 1;
}