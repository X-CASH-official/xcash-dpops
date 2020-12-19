#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "variables.h"

#include "count_database_functions.h"
#include "read_database_functions.h"
#include "network_functions.h"
#include "network_wallet_functions.h"
#include "string_functions.h"
#include "VRF_functions.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: get_public_address
Description: Gets the public address of your wallet
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int get_public_address(void)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char data[SMALL_BUFFER_SIZE];

  // define macros
  #define GET_PUBLIC_ADDRESS_DATA "{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_address\"}"
  #define GET_PUBLIC_ADDRESS_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"get_public_address",18); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));
  
  if (send_http_request(data,"127.0.0.1","/json_rpc",xcash_wallet_port,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,GET_PUBLIC_ADDRESS_DATA,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) <= 0)
  {  
    GET_PUBLIC_ADDRESS_ERROR("Could not get the public address");
  }

  memset(xcash_wallet_public_address,0,sizeof(xcash_wallet_public_address));
  if (parse_json_data(data,"address",xcash_wallet_public_address,sizeof(xcash_wallet_public_address)) == 0)
  {
    GET_PUBLIC_ADDRESS_ERROR("Could not get the public address");
  }

  // check if the returned data is valid
  if (strnlen(xcash_wallet_public_address,BUFFER_SIZE) != XCASH_WALLET_LENGTH && strncmp(xcash_wallet_public_address,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0)
  {
     GET_PUBLIC_ADDRESS_ERROR("Could not get the public address");
  }
  return 1;

  #undef GET_PUBLIC_ADDRESS_DATA
  #undef GET_PUBLIC_ADDRESS_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_total_amount
Description: Gets the total amount of your wallet
Return: 0 if an error has occured, otherwise the total amount
-----------------------------------------------------------------------------------------------------------
*/

long long int get_total_amount(void)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char data[SMALL_BUFFER_SIZE];
  char data2[SMALL_BUFFER_SIZE];
  long long int total_amount = 0;

  // define macros
  #define GET_TOTAL_AMOUNT_DATA "{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_balance\"}"
  #define GET_TOTAL_AMOUNT_ERROR \
  memcpy(error_message.function[error_message.total],"get_total_amount",16); \
  memcpy(error_message.data[error_message.total],"Could not get the total amount",30); \
  error_message.total++; \
  return 0;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  
  if (send_http_request(data,"127.0.0.1","/json_rpc",xcash_wallet_port,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,GET_TOTAL_AMOUNT_DATA,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) <= 0 || parse_json_data(data,"balance",data2,sizeof(data2)) == 0)
  {  
    GET_TOTAL_AMOUNT_ERROR;
  }

  sscanf(data2,"%lld", &total_amount);
  return total_amount;

  #undef GET_TOTAL_AMOUNT_DATA
  #undef GET_TOTAL_AMOUNT_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: send_payment
Description: Sends a payment
Parameters:
  DATA - The data
  tx_hash - The transaction hash of the payment
  tx_key - The transaction key of the payment
  SETTINGS - 0 to not send the payment, 1 to send the payment
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int send_payment(const char* DATA, char *tx_hash, char *tx_key, const int SETTINGS)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char data[5000];
  char message[5000];

  memset(data,0,sizeof(data));
  memset(message,0,sizeof(message));

  memcpy(message,DATA,strnlen(DATA,sizeof(message))-2);
  SETTINGS == 1 ? memcpy(message+strlen(message),",\"do_not_relay\":false}}",23) : memcpy(message+strlen(message),",\"do_not_relay\":true}}",22);

  return send_http_request(data,"127.0.0.1","/json_rpc",xcash_wallet_port,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,message,SEND_PAYMENT_TIMEOUT_SETTINGS) <= 0 || parse_json_data(data,"tx_hash_list",tx_hash,BUFFER_SIZE) == 0 || parse_json_data(data,"tx_key_list",tx_key,BUFFER_SIZE) == 0 ? 0 : 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: check_reserve_proof
Description: Checks a reserve proof
Parameters:
  result - The amount for the reserve proof
  PUBLIC_ADDRESS - The public address that created the reserve proof
  RESERVE_PROOF - The reserve proof
Return: -1 if an error has occured, 0 if the reserve proof is invalid, 1 if the reserve proof is valid
-----------------------------------------------------------------------------------------------------------
*/

int check_reserve_proofs(char *result, const char* PUBLIC_ADDRESS, const char* RESERVE_PROOF)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);
  const size_t RESERVE_PROOF_LENGTH = strnlen(RESERVE_PROOF,BUFFER_SIZE);

  // Variables
  char data[BUFFER_SIZE];
  char data2[SMALL_BUFFER_SIZE];
  char data3[SMALL_BUFFER_SIZE];
  int count;
  
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  // create the message
  memcpy(data,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"check_reserve_proof\",\"params\":{\"address\":\"",78);
  memcpy(data+78,PUBLIC_ADDRESS,XCASH_WALLET_LENGTH);
  memcpy(data+176,"\",\"message\":\"\",\"signature\":\"",28);
  memcpy(data+204,RESERVE_PROOF,RESERVE_PROOF_LENGTH);
  memcpy(data+204+RESERVE_PROOF_LENGTH,"\"}}",3);

  if ((count = send_http_request(data2,"127.0.0.1","/json_rpc",xcash_wallet_port,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS)) <= 0 || strstr(data2,"\"good\"") == NULL || strstr(data2,"\"spent\"") == NULL)
  { 
    // if the transaction failed then it could be an overload error and should be ignored, but any other error should be considered an invalid reserve proof
    return ((count <= 0) || (strstr(data2,"\"Failed\"") != NULL && strstr(data2,"\"transaction\"") != NULL)) ? -1 : 0;
  }

  // check if the reserve proof is valid
  if (strstr(data2,"\"good\": true") == NULL || strstr(data2,"\"spent\": 0") == NULL)
  { 
    return 0;
  }

  // parse the message
  return parse_json_data(data2,"total",result, BUFFER_SIZE) == 0 ? 0 : 1;
}