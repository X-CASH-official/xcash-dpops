#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "define_macros_functions.h"
#include "network_daemon_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "network_wallet_functions.h"
#include "server_functions.h"
#include "string_functions.h"

#include "define_macros_test.h"
#include "variables_test.h"
#include "network_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: network_daemon_test
Description: Test the network_daemon functions
Return: The number of passed network_daemon test
-----------------------------------------------------------------------------------------------------------
*/

void network_daemon_test()
{  
  // Variables
  char* transactions[5];
  size_t count;

  for (count = 0; count < 5; count++)
  {
    transactions[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));
  }

  for (count = 0; count < 5; count++)
  {
    if (transactions[count] == NULL)
    {
      color_print("Could not allocate the memory needed on the heap","red");
      exit(0);
    }
  }

  // reset the variables
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));

  // run the test

  // test the get_block_template function
  if (get_block_template(data_test,0) == 1)
  {   
    color_print("PASSED! Test for getting the block template","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for getting the block template","red");
  }

  // test the verify_blockchain_network_transactions function
  memcpy(transactions[0],"f6458824e54ea5cddd80a6bb0105ecdd6d2248629482df2c0f989db3d46f6ebd",64);
  memcpy(transactions[1],"871eb7b29c72582572041c597ff092143031bfcef5fa1fa92808dacab2ba226f",64);
  memcpy(transactions[2],"72f97600db9d7522a2a39fc690d25e1cc9a17535064b08f81bd7424a51bba931",64);
  memcpy(transactions[3],"b180489867776c7e39c07ddb0d88609dac6f748dec554e2d96b6a168785bdb44",64);
  memcpy(transactions[4],"",64);
  if (verify_blockchain_network_transactions(transactions,5,0,0) == 1)
  {   
    color_print("PASSED! Test for verifying transactions","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for verifying transactions","red");
  }

  // test the get_current_block_height function
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (get_current_block_height(data_test,0) == 1)
  {   
    color_print("PASSED! Test for getting the current block height","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for getting the current block height","red");
  }

  // test the get_block_settings function
  sscanf(data_test, "%zu", &count);
  count--;
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  sprintf(data_test,"%zu",count);
  if (get_block_settings(data_test,0) == 1)
  {   
    color_print("PASSED! Test for getting the getting the block settings","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for getting the getting the block settings","red");
  }  

  // test the get_previous_block_hash function
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (get_previous_block_hash(data_test,0) == 1)
  {   
    color_print("PASSED! Test for getting the previous block hash","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for getting the previous block hash","red");
  }

  for (count = 0; count < 5; count++)
  {
    pointer_reset(transactions[count]);
  }
}



/*
-----------------------------------------------------------------------------------------------------------
Name: create_server_on_separate_thread
Description: Creates the server on a separate thread
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

void* create_server_on_separate_thread()
{
  create_server(0);
  pthread_exit((void *)(intptr_t)1);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: general_network_test
Description: Test the network functions
Return: The number of passed general_network test
-----------------------------------------------------------------------------------------------------------
*/

void general_network_test()
{  
  // Variables
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* public_address = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* string = (char*)calloc(BUFFER_SIZE,sizeof(char));
  int settings = 1;
  pthread_t thread_id;

  // define macros
  #define MESSAGE "{\r\n \"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\",\r\n}"

  #define pointer_reset_all \
  free(message); \
  message = NULL; \
  free(public_address); \
  public_address = NULL; \
  free(string); \
  string = NULL;

  // check if the memory needed was allocated on the heap successfully
  if (message == NULL || public_address == NULL || string == NULL)
  {
    if (message != NULL)
    {
      pointer_reset(message);
    }
    if (public_address != NULL)
    {
      pointer_reset(public_address);
    }
    if (string != NULL)
    {
      pointer_reset(string);
    }
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // reset the variables
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memset(server_message,0,strnlen(server_message,BUFFER_SIZE));

  // set the server_message
  memcpy(server_message,"XCASH_PROOF_OF_STAKE_TEST_DATA",30);

  // run the test

  // set the current_round_part and current_round_part_backup_node
  memset(current_round_part,0,strnlen(current_round_part,BUFFER_SIZE));
  memset(current_round_part_backup_node,0,strnlen(current_round_part_backup_node,BUFFER_SIZE));
  memcpy(current_round_part,"1",1);
  memcpy(current_round_part_backup_node,"0",1); 
  
  // test for creating the server
  if (pthread_create(&thread_id, NULL, &create_server_on_separate_thread,NULL) != 0)
  {
    color_print("FAILED! Test for creating the server","red");
    color_print("FAILED! Test for sending and receving data using sockets","red");
    settings = 0;
  }
  if (settings == 1)
  {
    if (pthread_detach(thread_id) != 0)
    {      
      color_print("FAILED! Test for creating the server","red");
      color_print("FAILED! Test for sending and receving data using sockets","red");
      settings = 0;
    }  
  }
  sleep(1);



  // test for sending and receiving data using sockets
  // create the message
  memset(message,0,strnlen(message,BUFFER_SIZE));
  memcpy(message,MESSAGE,strnlen(MESSAGE,BUFFER_SIZE));

  // sign_data
  if (settings == 1)
  {
    if (sign_data(message,0) == 0)
    { 
      color_print("FAILED! Test for creating the server","red");
      color_print("FAILED! Test for sending and receving data using sockets","red");
    }
  }
 
  if (settings == 1)
  {
    memset(string,0,strlen(string));
    if (send_and_receive_data_socket(string,"127.0.0.1",SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"XCASH_PROOF_OF_STAKE_TEST_DATA",0) <= 0)
    {
      color_print("FAILED! Test for creating the server","red");
      color_print("FAILED! Test for sending and receving data using sockets","red");
      settings = 0;
    }
  }
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));

  // verify the message
  if (settings == 1)
  {
    if (verify_data(string,0,1,1) == 0)
    {   
      color_print("FAILED! Test for creating the server","red");
      color_print("FAILED! Test for sending and receving data using sockets","red");
      settings = 0;
    }
  }

  // parse the data
  if (settings == 1)
  {
    if (parse_json_data(string,"message_settings",data_test) == 0)
    {
      color_print("FAILED! Test for creating the server","red");
      color_print("FAILED! Test for sending and receving data using sockets","red");
      settings = 0;
    }
  }
  
  // check if the received data is correct
  if (settings == 1)
  {
    if (strncmp(data_test,"XCASH_PROOF_OF_STAKE_TEST_DATA",BUFFER_SIZE) == 0)
    {
      color_print("PASSED! Test for creating the server","green");
      color_print("PASSED! Test for sending and receving data using sockets","green");
      count_test += 2;
    }
    else
    {
      color_print("FAILED! Test for creating the server","red");
      color_print("FAILED! Test for sending and receving data using sockets","red");
      settings = 0;
    }
  }

  pointer_reset_all;

  #undef MESSAGE
  #undef pointer_reset_all
}



/*
-----------------------------------------------------------------------------------------------------------
Name: network_security_test
Description: Test the network_security functions
Return: The number of passed network_security test
-----------------------------------------------------------------------------------------------------------
*/

void network_security_test()
{  
  // define macros
  #define MESSAGE "{\r\n \"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\",\r\n}"
  
  // reset the variables
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memset(server_message,0,strnlen(server_message,BUFFER_SIZE));

  // set the server_message
  memcpy(server_message,"XCASH_PROOF_OF_STAKE_TEST_DATA",30);

  // set the current_round_part and current_round_part_backup_node
  memset(current_round_part,0,strnlen(current_round_part,BUFFER_SIZE));
  memset(current_round_part_backup_node,0,strnlen(current_round_part_backup_node,BUFFER_SIZE));
  memcpy(current_round_part,"1",1);
  memcpy(current_round_part_backup_node,"0",1);
  
  // create the message
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memcpy(result_test,MESSAGE,strnlen(MESSAGE,BUFFER_SIZE));

  // test the sign_data functions
  if (sign_data(result_test,0) == 1)
  {   
    color_print("PASSED! Test for sign data to send data securely","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sign data to send data securely","red");
  }
  
  // test the verify_data functions
  if (verify_data(result_test,0,1,1) == 1)
  {   
    color_print("PASSED! Test for verify data to receive data securely","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for verify data to receive data securely","red");
  }
  #undef MESSAGE
}



/*
-----------------------------------------------------------------------------------------------------------
Name: network_wallet_test
Description: Test the network_wallet functions
Return: The number of passed network_wallet test
-----------------------------------------------------------------------------------------------------------
*/

void network_wallet_test()
{ 
  // define macros
  #define CHECK_RESERVE_PROOF_TEST_PUBLIC_ADDRESS "XCA1va68ZVYVDRzX3mpqVD8iWbxviCsUigMkyogxAWSQR5rk16g7dYjbcKDwsAggp8Z1dNjPgcCzkcV7hDg99Wtk8nyUBBK4K7"
  #define CHECK_RESERVE_PROOF_TEST_RESERVE_PROOF "ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118PRh4AT7VvXjWBm8SAPTf55QJP1E3fkm8f3oe3VWeT5o8YybH9113USPdfBXLfpWTHYMCJAmGa2CcFiyHn5Gj9PCuHaKB3VHdqTEy6shyi4bW8FuTLyhTpDcnS7uEAyQcijSuHEUcgTwUSNSwuzvT113ka91zQXMcjYwZ39zAJVS16DuJZNRWnQaiNJVsracFDmUsXCCWcMx5HpKDNn5N3H1qSCxhV4CdUN2cB8Z2iirSgiL2frFA1DrVCKJm5kNHSANEGjHe4mw5L6L2Yeabna74FLszbBPKso42fpctQ8Djj25hqj6pEQqHY4tTaAYfkVRuB4m8DU4aNZN1Ak9vkBxvZtVDRzX3mpqVD8iWbxviCsUigMkyogxAWSQR5rjh4uUTRP7QHCpCm1o34Qc1vrDsLzXWbvcwC1g1VUh3S5dDKX2FXGJT4DX2REwtCNCZX2MZE2wGcbLRZ3vj4jQ6NzwTqT"
  #define CHECK_RESERVE_PROOF_TEST_AMOUNT "10"
  
  // test the get_public_address function
  memset(xcash_wallet_public_address,0,strnlen(xcash_wallet_public_address,BUFFER_SIZE));
  if (get_public_address(0) == 1)
  {   
    color_print("PASSED! Test for sending an HTTP request and getting the public address of the opened X-CASH wallet","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sending an HTTP request and getting the public address of the opened X-CASH wallet","red");
  }

  // test the data_verify function
  if (data_verify(0,"XCA1rWqiYtmHPZssWpCpnsPuQd1uXP91zEVDYkEYdqzb2tDKZCZUaDahWignGvx38ugboroBRRMBS6M3NxDZJEdM5W2KU1BNDK","SigV1AFPX9VxwFgbNmLn2AmZbvG6n342EGfJYbDDZMtFRDkPUZc7bPYriHMYGQ4E996AU64RCsKKqG95X9UXRS1pDETyF","X-CASH") == 1)
  {   
    color_print("PASSED! Test for data verify","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for data verify","red");
  }

  // test the sign_network_block_string function
  memset(data_test,0,strlen(data_test));
  if (sign_network_block_string(data_test,"X-CASH",0) == 1 && data_verify(0,xcash_wallet_public_address,data_test,"X-CASH") == 1)
  {   
    color_print("PASSED! Test for sign_network_block_string","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sign_network_block_string","red");
  }

  // test the check_reserve_proofs function
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (check_reserve_proofs(data_test,CHECK_RESERVE_PROOF_TEST_PUBLIC_ADDRESS,CHECK_RESERVE_PROOF_TEST_RESERVE_PROOF,0) == 1 && memcmp(data_test,CHECK_RESERVE_PROOF_TEST_AMOUNT,2) == 0)
  {   
    color_print("PASSED! Test for check reserve proofs","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for check reserve proofs","red");
  }

  #undef CHECK_RESERVE_PROOF_TEST_PUBLIC_ADDRESS
  #undef CHECK_RESERVE_PROOF_TEST_RESERVE_PROOF
  #undef CHECK_RESERVE_PROOF_TEST_AMOUNT
}



/*
-----------------------------------------------------------------------------------------------------------
Name: network_test
Description: Runs all of the network test
Return: The number of passed network test
-----------------------------------------------------------------------------------------------------------
*/

int network_functions_test()
{
  // define macros
  #define NETWORK_TOTAL_TEST 13

  // reset the varaibles
  count_test = 0;

  // write the start test message
  color_print(TEST_OUTLINE,"blue");
  printf("\033[1;34mnetwork test - Total test: %d\033[0m\n",NETWORK_TOTAL_TEST);
  color_print(TEST_OUTLINE,"blue");
  printf("\n");

  // run the test
  network_daemon_test();
  general_network_test();
  network_security_test();
  network_wallet_test();

  // write the end test message
  if (count_test == NETWORK_TOTAL_TEST)
  {
    printf("\n");
    color_print(TEST_OUTLINE,"green");
    printf("\033[1;32mnetwork test - Passed test: %d, Failed test: 0\033[0m\n",NETWORK_TOTAL_TEST);
    color_print(TEST_OUTLINE,"green");
    printf("\n\n");
  }
  else
  {
    printf("\n");
    color_print(TEST_OUTLINE,"red");
    printf("\033[1;31mnetwork test - Passed test: %d, Failed test: %d\033[0m\n",count_test,NETWORK_TOTAL_TEST-count_test);
    color_print(TEST_OUTLINE,"red");
    printf("\n\n");
  }
  return count_test;

  #undef NETWORK_TOTAL_TEST
}
