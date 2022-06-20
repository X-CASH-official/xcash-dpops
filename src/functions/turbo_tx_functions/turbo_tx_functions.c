#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <arpa/nameser.h>
#include <resolv.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"
#include "define_macros_test.h"

#include "network_functions.h"
#include "turbo_tx_functions.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: get_block_verifiers_transactions
Description: Gets the block verifiers transactions
-----------------------------------------------------------------------------------------------------------
*/

int get_block_verifiers_transactions(char *data)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char* data2 = (char*)calloc(TX_VERIFICATION_MAXIMUM_SIZE,sizeof(char)); // 215 MB
  // since were going to be changing where data2 is referencing, we need to create a copy to pointer_reset
  char* data3 = data2;
  char message[BUFFER_SIZE];

  // define macros
  #define MESSAGE "\"double_spend_seen\": false,"
  #define GET_BLOCK_VERIFIERS_TRANSACTIONS_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"get_block_verifiers_transactions",32); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  pointer_reset(data3); \
  return 0;

  if (send_http_request(data2,XCASH_DPOPS_delegates_IP_address,"/get_transaction_pool",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,"",SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) <= 0)
  {
    GET_BLOCK_VERIFIERS_TRANSACTIONS_ERROR("Could not get the block verifiers transactions");
  }

  // skip any double spend = true tx
  memset(data,0,strlen(data));

  while((data2 = strstr(data2, MESSAGE)) != NULL)
  {
    // get the tx id
    data2 = strstr(data2, "\"id_hash\": \"") + 12;
    memset(message,0,sizeof(message));
    memcpy(message,data2,TRANSACTION_HASH_LENGTH);
    memcpy(message+strlen(message),"|",1);

    // make sure the tx is not already in the tx list
    if (strstr(data, message) == NULL)
    {
      // add the tx to the tx list      
      memcpy(data+strlen(data),message,strlen(message));
    }
  }
  
  pointer_reset(data3);
  return 1;
  
  #undef MESSAGE
  #undef GET_BLOCK_VERIFIERS_TRANSACTIONS_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: verify_block_verifiers_transactions
Description: Verifies the block verifiers transactions
Return: 0 if an error has occured or invalid, 1 if valid
-----------------------------------------------------------------------------------------------------------
*/

int verify_block_verifiers_transactions(void)
{
  // Variables
  size_t count;
  size_t count2;
  size_t valid_tx_count;
  int settings = 1;

  color_print("Turbo tx - Verify block producer tx","yellow");

  // verify all of the transactions
  for (valid_tx_count = 0,count = 0; count < blockchain_data.transaction_amount; count++)
  {
    for (count2 = 0; count2 < BLOCK_VERIFIERS_AMOUNT; count2++)
    {
      if (strstr(turbo_tx_list[count2],blockchain_data.transactions[count]) != NULL)
      {
        valid_tx_count++;
      }
    }

    if (valid_tx_count >= BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      fprintf(stderr,"\033[1;32m%zu / %d block verifiers validate tx %s\033[0m\n",valid_tx_count,BLOCK_VERIFIERS_VALID_AMOUNT,blockchain_data.transactions[count]); 
    }
    else
    {
      settings = 0;
      fprintf(stderr,"\033[1;31m%zu / %d block verifiers validate tx %s\033[0m\n",valid_tx_count,BLOCK_VERIFIERS_VALID_AMOUNT,blockchain_data.transactions[count]);
    }
  }
  fprintf(stderr,"\n");
  return settings;
}
