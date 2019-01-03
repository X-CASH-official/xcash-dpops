#include "stdio.h"
#include "stdlib.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "string.h"
#include <netdb.h> 
#include "netinet/in.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#include "xcash_proof_of_stake_functions_and_macros.h"
#include "xcash_proof_of_stake_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Global variables
-----------------------------------------------------------------------------------------------------------
*/

const char STR1_TEST [BUFFER_SIZE] = "test string 1";
const char* STR2_TEST = "test string 2";
char string1_test [BUFFER_SIZE];
char* string2_test;
char* result_test;
char* data_test;
int count_test = 0;

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

int append_string_test()
{  
  #define APPEND_STRING_TOTAL_TEST 4

  // reset the variables
  memset(&string1_test,0,sizeof(string1_test)); 
  memset(string2_test,0,strlen(string2_test)); 
  count_test = 0;

  // write the start test message
  color_print(TEST_OUTLINE,"blue");
  printf("\033[1;34mappend_string test - Total test: %d\033[0m\n",APPEND_STRING_TOTAL_TEST);
  color_print(TEST_OUTLINE,"blue");
  printf("\n");

  // run the test
  
  // test for using append_string on an empty statically allocated char
  append_string(string1_test,STR1_TEST);
  if (strcmp(string1_test,STR1_TEST) == 0)
  {
    color_print("PASSED! Test for using append_string on an empty statically allocated char","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for using append_string on an empty statically allocated char","red");
  }

  // test for using append_string on a statically allocated char
  append_string(string1_test," ");
  append_string(string1_test,STR1_TEST);
  if (strcmp(string1_test,"test string 1 test string 1") == 0)
  {
    color_print("PASSED! Test for using append_string on a statically allocated char","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for using append_string on a statically allocated char","red");
  }
    
  // test for using append_string on an empty dynamically allocated char
  append_string(string2_test,STR2_TEST);
  if (strcmp(string2_test,STR2_TEST) == 0)
  {
    color_print("PASSED! Test for using append_string on an empty dynamically allocated char","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for using append_string on an empty dynamically allocated char","red");
  }

  // test for using append_string on a dynamically allocated char
  append_string(string2_test," ");
  append_string(string2_test,STR2_TEST);
  if (strcmp(string2_test,"test string 2 test string 2") == 0)
  {
    color_print("PASSED! Test for using append_string on a dynamically allocated char","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for using append_string on a dynamically allocated char","red");
  }



  // write the end test message
  if (count_test == APPEND_STRING_TOTAL_TEST)
  {
    printf("\n");
    color_print(TEST_OUTLINE,"green");
    printf("\033[1;32mappend_string test - Passed test: %d, Failed test: 0\033[0m\n",APPEND_STRING_TOTAL_TEST);
    color_print(TEST_OUTLINE,"green");
    printf("\n\n");
  }
  else
  {
    printf("\n");
    color_print(TEST_OUTLINE,"red");
    printf("\033[1;31mappend_string test - Passed test: %d, Failed test: %d\033[0m\n",count_test,APPEND_STRING_TOTAL_TEST-count_test);
    color_print(TEST_OUTLINE,"red");
    printf("\n\n");
  } 
  return count_test;
}

int parse_json_data_test()
{  
  #define PARSE_JSON_DATA_TOTAL_TEST 66
 
  #define SIGN_RPC_CALL_TEST_DATA "{\r\n  \"id\": \"0\",\r\n  \"jsonrpc\": \"2.0\",\r\n  \"result\": {\r\n    \"signature\": \"SIGN_RPC_CALL_TEST_DATA\"\r\n  }\r\n}"
  #define VERIFY_RPC_CALL_TEST_DATA "{\r\n  \"id\": \"0\",\r\n  \"jsonrpc\": \"2.0\",\r\n  \"result\": {\r\n    \"good\": VERIFY_RPC_CALL_TEST_DATA\r\n  }\r\n}"
  #define GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA "{\r\n  \"id\": \"0\",\r\n  \"jsonrpc\": \"2.0\",\r\n  \"result\": {\r\n    \"blockhashing_blob\": \"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\",\r\n    \"blocktemplate_blob\": \"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\",\r\n    \"difficulty\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,\r\n    \"expected_reward\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,\r\n    \"height\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,\r\n    \"prev_hash\": \"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\",\r\n    \"reserved_offset\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,\r\n    \"status\": \"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\",\r\n    \"untrusted\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\r\n  }\r\n}"
 
  /*
  message_settings - The type of the message
  public_address - The public address of the node that is sending the data
  data - A random 100 character string. This is the data that the signature is used for. The random data  will create a different signature for every message, even if the message data is the same.
  signature - The signature of the data, used for verifiying that the sender of the message is the sender.
  message - The message.
  main_nodes_public_address - The public address of the main node for that part of the round.
  main_node_IP_address - The IP address of the main node for that part of the round.
  */
  #define TESTNODE_TO_NODES "{\"message_settings\": \"TESTNODE_TO_NODES\",\r\n \"public_address\": \"TESTNODE_TO_NODES\",\r\n \"data\": \"TESTNODE_TO_NODES\",\r\n \"signature\": \"TESTNODE_TO_NODES\",\r\n \"message\": \"TESTNODE_TO_NODES\",\r\n \"main_nodes_public_address\": \"TESTNODE_TO_NODES\",\r\n \"main_node_IP_address\": \"TESTNODE_TO_NODES\"\r\n}"
 
  /*
  message_settings - The type of the message
  public_address - The public address of the node that is sending the data
  data - A random 100 character string. This is the data that the signature is used for. The random data  will create a different signature for every message, even if the message data is the same.
  signature - The signature of the data, used for verifiying that the sender of the message is the sender.
  message - The message.
  */
  #define TESTNODE_TO_MAINNODES "{\"message_settings\": \"TESTNODE_TO_MAINNODES\",\r\n \"public_address\": \"TESTNODE_TO_MAINNODES\",\r\n \"data\": \"TESTNODE_TO_MAINNODES\",\r\n \"signature\": \"TESTNODE_TO_MAINNODES\",\r\n \"message\": \"TESTNODE_TO_MAINNODES\",\r\n}"
 
  /*
  message_settings - The type of the message
  public_address - The public address of the node that is sending the data
  data - A random 100 character string. This is the data that the signature is used for. The random data  will create a different signature for every message, even if the message data is the same.
  signature - The signature of the data, used for verifiying that the sender of the message is the sender.
  message - The message.
  vote_data - The SHA 256 data that the node received.
  */
  #define NODES_TO_NODES "{\"message_settings\": \"NODES_TO_NODES\",\r\n \"public_address\": \"NODES_TO_NODES\",\r\n \"data\": \"NODES_TO_NODES\",\r\n \"signature\": \"NODES_TO_NODES\",\r\n \"message\": \"NODES_TO_NODES\",\r\n \"vote_data\": \"NODES_TO_NODES\",\r\n}"
 
  /*
  message_settings - The type of the message
  public_address - The public address of the node that is sending the data
  data - A random 100 character string. This is the data that the signature is used for. The random data  will create a different signature for every message, even if the message data is the same.
  signature - The signature of the data, used for verifiying that the sender of the message is the sender.
  message - The message.
  vote_data - TRUE or FALSE
  */
  #define NODES_TO_TESTNODES "{\"message_settings\": \"NODES_TO_TESTNODES\",\r\n \"public_address\": \"NODES_TO_TESTNODES\",\r\n \"data\": \"NODES_TO_TESTNODES\",\r\n \"signature\": \"NODES_TO_TESTNODES\",\r\n \"message\": \"NODES_TO_TESTNODES\",\r\n \"vote_data\": \"NODES_TO_TESTNODES\",\r\n}"
 
  /*
  message_settings - The type of the message
  public_address - The public address of the node that is sending the data
  data - A random 100 character string. This is the data that the signature is used for. The random data  will create a different signature for every message, even if the message data is the same.
  signature - The signature of the data, used for verifiying that the sender of the message is the sender.
  message - The message.
  public_key - The VRF public key, this is also the alpha_string for this part of the round.
  proof - The VRF proof.
  beta_string - The VRF beta_string.
  */
  #define MAINNODE_TO_NODES_PART_1 "{\"message_settings\": \"MAINNODE_TO_NODES_PART_1\",\r\n \"public_address\": \"MAINNODE_TO_NODES_PART_1\",\r\n \"data\": \"MAINNODE_TO_NODES_PART_1\",\r\n \"signature\": \"MAINNODE_TO_NODES_PART_1\",\r\n \"message\": \"MAINNODE_TO_NODES_PART_1\",\r\n \"public_key\": \"MAINNODE_TO_NODES_PART_1\",\r\n \"proof\": \"MAINNODE_TO_NODES_PART_1\"\r\n \"beta_string\": \"MAINNODE_TO_NODES_PART_1\",\r\n}"
 
  /*
  message_settings - The type of the message
  public_address - The public address of the node that is sending the data
  data - A random 100 character string. This is the data that the signature is used for. The random data  will create a different signature for every message, even if the message data is the same.
  signature - The signature of the data, used for verifiying that the sender of the message is the sender.
  message - The message.
  previous_block_hash - The previous block hash, used to create the VRF alpha_string.
  random_character_string - A random 100 character string, used to create the VRF alpha_string.
  public_key - The VRF public key.
  alpha_string - The VRF alpha_string.
  proof - The VRF proof.
  beta_string - The VRF beta_string.
  */
  #define MAINNODE_TO_NODES_PART_2 "{\"message_settings\": \"MAINNODE_TO_NODES_PART_2\",\r\n \"public_address\": \"MAINNODE_TO_NODES_PART_2\",\r\n \"data\": \"MAINNODE_TO_NODES_PART_2\",\r\n \"signature\": \"MAINNODE_TO_NODES_PART_2\",\r\n \"message\": \"MAINNODE_TO_NODES_PART_2\",\r\n \"previous_block_hash\": \"MAINNODE_TO_NODES_PART_2\",\r\n \"random_character_string\": \"MAINNODE_TO_NODES_PART_2\",\r\n \"public_key\": \"MAINNODE_TO_NODES_PART_2\",\r\n \"alpha_string\": \"MAINNODE_TO_NODES_PART_2\",\r\n \"proof\": \"MAINNODE_TO_NODES_PART_2\"\r\n \"beta_string\": \"MAINNODE_TO_NODES_PART_2\",\r\n}"
 
  /*
  message_settings - The type of the message
  public_address - The public address of the node that is sending the data
  data - A random 100 character string. This is the data that the signature is used for. The random data  will create a different signature for every message, even if the message data is the same.
  signature - The signature of the data, used for verifiying that the sender of the message is the sender.
  message - The message.
  previous_block_hash - The previous block hash, used to create the VRF alpha_string.
  random_character_string - A random 100 character string, used to create the VRF alpha_string.
  public_key - The VRF public key.
  alpha_string - The VRF alpha_string.
  proof - The VRF proof.
  beta_string - The VRF beta_string.
  */
  #define MAINNODE_TO_NODES_PART_3 "{\"message_settings\": \"MAINNODE_TO_NODES_PART_3\",\r\n \"public_address\": \"MAINNODE_TO_NODES_PART_3\",\r\n \"data\": \"MAINNODE_TO_NODES_PART_3\",\r\n \"signature\": \"MAINNODE_TO_NODES_PART_3\",\r\n \"message\": \"MAINNODE_TO_NODES_PART_3\",\r\n \"previous_block_hash\": \"MAINNODE_TO_NODES_PART_3\",\r\n \"random_character_string\": \"MAINNODE_TO_NODES_PART_3\",\r\n \"public_key\": \"MAINNODE_TO_NODES_PART_3\",\r\n \"alpha_string\": \"MAINNODE_TO_NODES_PART_3\",\r\n \"proof\": \"MAINNODE_TO_NODES_PART_3\"\r\n \"beta_string\": \"MAINNODE_TO_NODES_PART_3\",\r\n}"
 
  /*
  message_settings - The type of the message
  public_address - The public address of the node that is sending the data
  data - A random 100 character string. This is the data that the signature is used for. The random data  will create a different signature for every message, even if the message data is the same.
  signature - The signature of the data, used for verifiying that the sender of the message is the sender.
  message - The message.
  public_key - The VRF public key.
  alpha_string - The VRF alpha_string.
  proof - The VRF proof.
  beta_string - The VRF beta_string.
  */
  #define MAINNODE_TO_NODES_PART_4 "{\"message_settings\": \"MAINNODE_TO_NODES_PART_4\",\r\n \"public_address\": \"MAINNODE_TO_NODES_PART_4\",\r\n \"data\": \"MAINNODE_TO_NODES_PART_4\",\r\n \"signature\": \"MAINNODE_TO_NODES_PART_4\",\r\n \"message\": \"MAINNODE_TO_NODES_PART_4\",\r\n \"public_key\": \"MAINNODE_TO_NODES_PART_4\",\r\n \"alpha_string\": \"MAINNODE_TO_NODES_PART_4\",\r\n \"proof\": \"MAINNODE_TO_NODES_PART_4\"\r\n \"beta_string\": \"MAINNODE_TO_NODES_PART_4\",\r\n}"
 
  // reset the variables
  memset(result_test,0,strlen(result_test));
  memset(data_test,0,strlen(data_test));
  count_test = 0;
 
  // write the start test message
  color_print(TEST_OUTLINE,"blue");
  printf("\033[1;34mparse_json_data test - Total test: %d\033[0m\n",PARSE_JSON_DATA_TOTAL_TEST);
  color_print(TEST_OUTLINE,"blue");
  printf("\n");
 
  // run the test
 
  // test parsing the return data for the sign RPC call for the signature field
  append_string(result_test,SIGN_RPC_CALL_TEST_DATA);
  if (parse_json_data(result_test,(char*)"signature",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"SIGN_RPC_CALL_TEST_DATA") == 0)
    {
      color_print("PASSED! Test for parsing the return data for the sign RPC call for the signature field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for the sign RPC call for the signature field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for the sign RPC call for the signature field","red");
  }
 
  // test parsing the return data for the verify RPC call for the good field
  memset(result_test,0,strlen(result_test));
  memset(data_test,0,strlen(data_test));
  append_string(result_test,VERIFY_RPC_CALL_TEST_DATA);
  if (parse_json_data(result_test,(char*)"good",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"VERIFY_RPC_CALL_TEST_DATA") == 0)
    {
      color_print("PASSED! Test for parsing the return data for the verify RPC call for the good field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for the verify RPC call for the good field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for the verify RPC call for the good field","red");
  }
 
  // test parsing the return data for the get_block_template RPC call for the blocktemplate_blob field
  memset(result_test,0,strlen(result_test));
  memset(data_test,0,strlen(data_test));
  append_string(result_test,GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA);
  if (parse_json_data(result_test,(char*)"blocktemplate_blob",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA") == 0)
    {
      color_print("PASSED! Test for parsing the return data for the get_block_template RPC call for the blocktemplate_blob field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for the get_block_template RPC call for the blocktemplate_blob field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for the get_block_template RPC call for the blocktemplate_blob field","red");
  }
  printf("\n");
 
 
 
  // test for parsing the return data for when test nodes send nodes data
  memset(result_test,0,strlen(result_test));
  memset(data_test,0,strlen(data_test));
  append_string(result_test,TESTNODE_TO_NODES);
  if (parse_json_data(result_test,(char*)"message_settings",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"TESTNODE_TO_NODES") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when test nodes send nodes data for the message_settings field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when test nodes send nodes data for the message_settings field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when test nodes send nodes data for the message_settings field","red");
  }
  if (parse_json_data(result_test,(char*)"public_address",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"TESTNODE_TO_NODES") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when test nodes send nodes data for the public_address field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when test nodes send nodes data for the public_address field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when test nodes send nodes data for the public_address field","red");
  }
  if (parse_json_data(result_test,(char*)"data",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"TESTNODE_TO_NODES") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when test nodes send nodes data for the data field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when test nodes send nodes data for the data field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when test nodes send nodes data for the data field","red");
  }
  if (parse_json_data(result_test,(char*)"signature",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"TESTNODE_TO_NODES") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when test nodes send nodes data for the signature field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when test nodes send nodes data for the signature field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when test nodes send nodes data for the signature field","red");
  }
  if (parse_json_data(result_test,(char*)"message",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"TESTNODE_TO_NODES") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when test nodes send nodes data for the message field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when test nodes send nodes data for the message field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when test nodes send nodes data for the message field","red");
  }
  if (parse_json_data(result_test,(char*)"main_nodes_public_address",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"TESTNODE_TO_NODES") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when test nodes send nodes data for the main_nodes_public_address field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when test nodes send nodes data for the main_nodes_public_address field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when test nodes send nodes data for the main_nodes_public_address field","red");
  }
  if (parse_json_data(result_test,(char*)"main_node_IP_address",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"TESTNODE_TO_NODES") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when test nodes send nodes data for the main_node_IP_address field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when test nodes send nodes data for the main_node_IP_address field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when test nodes send nodes data for the main_node_IP_address field","red");
  }
  printf("\n");
 
 
 
// test for parsing the return data for when test nodes send main nodes data
  memset(result_test,0,strlen(result_test));
  memset(data_test,0,strlen(data_test));
  append_string(result_test,TESTNODE_TO_MAINNODES);
  if (parse_json_data(result_test,(char*)"message_settings",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"TESTNODE_TO_MAINNODES") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when test nodes send main nodes data for the message_settings field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when test nodes send main nodes data for the message_settings field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when test nodes send main nodes data for the message_settings field","red");
  }
  if (parse_json_data(result_test,(char*)"public_address",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"TESTNODE_TO_MAINNODES") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when test nodes send main nodes data for the public_address field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when test nodes send main nodes data for the public_address field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when test nodes send main nodes data for the public_address field","red");
  }
  if (parse_json_data(result_test,(char*)"data",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"TESTNODE_TO_MAINNODES") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when test nodes send main nodes data for the data field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when test nodes send main nodes data for the data field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when test nodes send main nodes data for the data field","red");
  }
  if (parse_json_data(result_test,(char*)"signature",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"TESTNODE_TO_MAINNODES") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when test nodes send main nodes data for the signature field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when test nodes send main nodes data for the signature field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when test nodes send main nodes data for the signature field","red");
  }
  if (parse_json_data(result_test,(char*)"message",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"TESTNODE_TO_MAINNODES") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when test nodes send main nodes data for the message field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when test nodes send main nodes data for the message field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when test nodes send main nodes data for the message field","red");
  }  
  printf("\n");
 
 
 
  // test for parsing the return data for when nodes send nodes data
  memset(result_test,0,strlen(result_test));
  memset(data_test,0,strlen(data_test));
  append_string(result_test,NODES_TO_NODES);
  if (parse_json_data(result_test,(char*)"message_settings",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"NODES_TO_NODES") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when nodes send nodes data for the message_settings field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when nodes send nodes data for the message_settings field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when nodes send nodes data for the message_settings field","red");
  }
  if (parse_json_data(result_test,(char*)"public_address",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"NODES_TO_NODES") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when nodes send nodes data for the public_address field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when nodes send nodes data for the public_address field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when nodes send nodes data for the public_address field","red");
  }
  if (parse_json_data(result_test,(char*)"data",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"NODES_TO_NODES") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when nodes send nodes data for the data field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when nodes send nodes data for the data field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when nodes send nodes data for the data field","red");
  }
  if (parse_json_data(result_test,(char*)"signature",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"NODES_TO_NODES") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when nodes send nodes data for the signature field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when nodes send nodes data for the signature field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when nodes send nodes data for the signature field","red");
  }
  if (parse_json_data(result_test,(char*)"message",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"NODES_TO_NODES") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when nodes send nodes data for the message field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when nodes send nodes data for the message field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when nodes send nodes data for the message field","red");
  }
  if (parse_json_data(result_test,(char*)"vote_data",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"NODES_TO_NODES") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when nodes send nodes data for the vote_data field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when nodes send nodes data for the vote_data field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when nodes send nodes data for the vote_data field","red");
  }
  printf("\n");
 
 
 
  // test for parsing the return data for when nodes send test nodes data
  memset(result_test,0,strlen(result_test));
  memset(data_test,0,strlen(data_test));
  append_string(result_test,NODES_TO_TESTNODES);
  if (parse_json_data(result_test,(char*)"message_settings",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"NODES_TO_TESTNODES") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when nodes send test nodes data for the message_settings field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when nodes send test nodes data for the message_settings field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when nodes send test nodes data for the message_settings field","red");
  }
  if (parse_json_data(result_test,(char*)"public_address",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"NODES_TO_TESTNODES") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when nodes send test nodes data for the public_address field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when nodes send test nodes data for the public_address field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when nodes send test nodes data for the public_address field","red");
  }
  if (parse_json_data(result_test,(char*)"data",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"NODES_TO_TESTNODES") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when nodes send test nodes data for the data field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when nodes send test nodes data for the data field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when nodes send test nodes data for the data field","red");
  }
  if (parse_json_data(result_test,(char*)"signature",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"NODES_TO_TESTNODES") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when nodes send test nodes data for the signature field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when nodes send test nodes data for the signature field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when nodes send test nodes data for the signature field","red");
  }
  if (parse_json_data(result_test,(char*)"message",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"NODES_TO_TESTNODES") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when nodes send test nodes data for the message field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when nodes send test nodes data for the message field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when nodes send test nodes data for the message field","red");
  }
  if (parse_json_data(result_test,(char*)"vote_data",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"NODES_TO_TESTNODES") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when nodes send test nodes data for the vote_data field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when nodes send test nodes data for the vote_data field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when nodes send test nodes data for the vote_data field","red");
  }
  printf("\n");
 
 
 
  // test for parsing the return data for when the main node sends data to the nodes for part 1 of the round
  memset(result_test,0,strlen(result_test));
  memset(data_test,0,strlen(data_test));
  append_string(result_test,MAINNODE_TO_NODES_PART_1);
  if (parse_json_data(result_test,(char*)"message_settings",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_1") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 1 of the round for the message_settings field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 1 of the round for the message_settings field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 1 of the round for the message_settings field","red");
  }
  if (parse_json_data(result_test,(char*)"public_address",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_1") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 1 of the round for the public_address field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 1 of the round for the public_address field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 1 of the round for the public_address field","red");
  }
  if (parse_json_data(result_test,(char*)"data",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_1") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 1 of the round for the data field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 1 of the round for the data field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 1 of the round for the data field","red");
  }
  if (parse_json_data(result_test,(char*)"signature",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_1") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 1 of the round for the signature field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 1 of the round for the signature field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 1 of the round for the signature field","red");
  }
  if (parse_json_data(result_test,(char*)"message",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_1") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 1 of the round for the message field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 1 of the round for the message field","red");
    }
  }
  else
  {
    color_print("FAILED! TTest for parsing the return data for when the main node sends data to the nodes for part 1 of the round for the message field","red");
  }
  if (parse_json_data(result_test,(char*)"public_key",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_1") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 1 of the round for the public_key field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 1 of the round for the public_key field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 1 of the round for the public_key field","red");
  }
  if (parse_json_data(result_test,(char*)"proof",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_1") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 1 of the round for the proof field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 1 of the round for the proof field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 1 of the round for the proof field","red");
  }
  if (parse_json_data(result_test,(char*)"beta_string",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_1") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 1 of the round for the beta_string field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 1 of the round for the beta_string field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 1 of the round for the beta_string field","red");
  }
  printf("\n");
 
 
 
  // test for parsing the return data for when the main node sends data to the nodes for part 2 of the round
  memset(result_test,0,strlen(result_test));
  memset(data_test,0,strlen(data_test));
  append_string(result_test,MAINNODE_TO_NODES_PART_2);
  if (parse_json_data(result_test,(char*)"message_settings",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_2") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the message_settings field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the message_settings field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the message_settings field","red");
  }
  if (parse_json_data(result_test,(char*)"public_address",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_2") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the public_address field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the public_address field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the public_address field","red");
  }
  if (parse_json_data(result_test,(char*)"data",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_2") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the data field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the data field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the data field","red");
  }
  if (parse_json_data(result_test,(char*)"signature",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_2") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the signature field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the signature field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the signature field","red");
  }
  if (parse_json_data(result_test,(char*)"message",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_2") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the message field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the message field","red");
    }
  }
  else
  {
    color_print("FAILED! TTest for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the message field","red");
  }
  if (parse_json_data(result_test,(char*)"previous_block_hash",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_2") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the previous_block_hash field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the previous_block_hash field","red");
    }
  }
  else
  {
    color_print("FAILED! TTest for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the previous_block_hash field","red");
  }
  if (parse_json_data(result_test,(char*)"random_character_string",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_2") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the random_character_string field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the random_character_string field","red");
    }
  }
  else
  {
    color_print("FAILED! TTest for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the random_character_string field","red");
  }
  if (parse_json_data(result_test,(char*)"public_key",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_2") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the public_key field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the public_key field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the public_key field","red");
  }
  if (parse_json_data(result_test,(char*)"alpha_string",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_2") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the alpha_string field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the alpha_string field","red");
    }
  }
  else
  {
    color_print("FAILED! TTest for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the alpha_string field","red");
  }
  if (parse_json_data(result_test,(char*)"proof",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_2") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the proof field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the proof field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the proof field","red");
  }
  if (parse_json_data(result_test,(char*)"beta_string",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_2") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the beta_string field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the beta_string field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 2 of the round for the beta_string field","red");
  }
  printf("\n");
 
 
 
  // test for parsing the return data for when the main node sends data to the nodes for part 3 of the round
  memset(result_test,0,strlen(result_test));
  memset(data_test,0,strlen(data_test));
  append_string(result_test,MAINNODE_TO_NODES_PART_3);
  if (parse_json_data(result_test,(char*)"message_settings",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_3") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the message_settings field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the message_settings field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the message_settings field","red");
  }
  if (parse_json_data(result_test,(char*)"public_address",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_3") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the public_address field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the public_address field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the public_address field","red");
  }
  if (parse_json_data(result_test,(char*)"data",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_3") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the data field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the data field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the data field","red");
  }
  if (parse_json_data(result_test,(char*)"signature",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_3") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the signature field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the signature field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the signature field","red");
  }
  if (parse_json_data(result_test,(char*)"message",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_3") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the message field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the message field","red");
    }
  }
  else
  {
    color_print("FAILED! TTest for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the message field","red");
  }
  if (parse_json_data(result_test,(char*)"previous_block_hash",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_3") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the previous_block_hash field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the previous_block_hash field","red");
    }
  }
  else
  {
    color_print("FAILED! TTest for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the previous_block_hash field","red");
  }
  if (parse_json_data(result_test,(char*)"random_character_string",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_3") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the random_character_string field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the random_character_string field","red");
    }
  }
  else
  {
    color_print("FAILED! TTest for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the random_character_string field","red");
  }
  if (parse_json_data(result_test,(char*)"public_key",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_3") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the public_key field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the public_key field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the public_key field","red");
  }
  if (parse_json_data(result_test,(char*)"alpha_string",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_3") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the alpha_string field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the alpha_string field","red");
    }
  }
  else
  {
    color_print("FAILED! TTest for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the alpha_string field","red");
  }
  if (parse_json_data(result_test,(char*)"proof",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_3") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the proof field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the proof field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the proof field","red");
  }
  if (parse_json_data(result_test,(char*)"beta_string",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_3") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the beta_string field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the beta_string field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 3 of the round for the beta_string field","red");
  }
  printf("\n");
 
 
 
  // test for parsing the return data for when the main node sends data to the nodes for part 4 of the round
  memset(result_test,0,strlen(result_test));
  memset(data_test,0,strlen(data_test));
  append_string(result_test,MAINNODE_TO_NODES_PART_4);
  if (parse_json_data(result_test,(char*)"message_settings",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_4") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 4 of the round for the message_settings field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 4 of the round for the message_settings field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 4 of the round for the message_settings field","red");
  }
  if (parse_json_data(result_test,(char*)"public_address",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_4") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 4 of the round for the public_address field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 4 of the round for the public_address field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 4 of the round for the public_address field","red");
  }
  if (parse_json_data(result_test,(char*)"data",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_4") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 4 of the round for the data field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 4 of the round for the data field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 4 of the round for the data field","red");
  }
  if (parse_json_data(result_test,(char*)"signature",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_4") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 4 of the round for the signature field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 4 of the round for the signature field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 4 of the round for the signature field","red");
  }
  if (parse_json_data(result_test,(char*)"message",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_4") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 4 of the round for the message field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 4 of the round for the message field","red");
    }
  }
  else
  {
    color_print("FAILED! TTest for parsing the return data for when the main node sends data to the nodes for part 4 of the round for the message field","red");
  }
  if (parse_json_data(result_test,(char*)"public_key",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_4") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 4 of the round for the public_key field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 4 of the round for the public_key field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 4 of the round for the public_key field","red");
  }
  if (parse_json_data(result_test,(char*)"alpha_string",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_4") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 4 of the round for the alpha_string field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 4 of the round for the alpha_string field","red");
    }
  }
  else
  {
    color_print("FAILED! TTest for parsing the return data for when the main node sends data to the nodes for part 4 of the round for the alpha_string field","red");
  }
  if (parse_json_data(result_test,(char*)"proof",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_4") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 4 of the round for the proof field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 4 of the round for the proof field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 4 of the round for the proof field","red");
  }
  if (parse_json_data(result_test,(char*)"beta_string",data_test) != 0)
  {
    // check if the data is correct
    if (strcmp(data_test,"MAINNODE_TO_NODES_PART_4") == 0)
    {
      color_print("PASSED! Test for parsing the return data for when the main node sends data to the nodes for part 4 of the round for the beta_string field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 4 of the round for the beta_string field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for when the main node sends data to the nodes for part 4 of the round for the beta_string field","red");
  }
  printf("\n");
 
 
 
  // write the end test message
  if (count_test == PARSE_JSON_DATA_TOTAL_TEST)
  {
    printf("\n");
    color_print(TEST_OUTLINE,"green");
    printf("\033[1;32mparse_json_data test - Passed test: %d, Failed test: 0\033[0m\n",PARSE_JSON_DATA_TOTAL_TEST);
    color_print(TEST_OUTLINE,"green");
    printf("\n\n");
  }
  else
  {
    printf("\n");
    color_print(TEST_OUTLINE,"red");
    printf("\033[1;31mparse_json_data test - Passed test: %d, Failed test: %d\033[0m\n",count_test,PARSE_JSON_DATA_TOTAL_TEST-count_test);
    color_print(TEST_OUTLINE,"red");
    printf("\n\n");
  }
  return count_test;
}


/*int send_and_receive_data_socket_test()
{  
  #define SEND_AND_RECEIVE_DATA_SOCKET_TOTAL_TEST 1

  // reset the variables
  memset(result_test,0,strlen(result_test));
  memset(data_test,0,strlen(data_test));
  count_test = 0;

  // write the start test message
  color_print(TEST_OUTLINE,"blue");
  printf("\033[1;34msend data socket test - Total test: %d\033[0m\n",SEND_AND_RECEIVE_DATA_SOCKET_TOTAL_TEST);
  color_print(TEST_OUTLINE,"blue");
  printf("\n");

  // run the test
  
  // test for sending data and receiving data using sockets
  

  // test for using append_string on a dynamically allocated char
  append_string(string2_test," ");
  append_string(string2_test,STR2_TEST);
  if (strcmp(string2_test,"test string 2 test string 2") == 0)
  {
    color_print("PASSED! Test for using append_string on a dynamically allocated char","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for using append_string on a dynamically allocated char","red");
  }



  // write the end test message
  if (count_test == APPEND_STRING_TOTAL_TEST)
  {
    printf("\n");
    color_print(TEST_OUTLINE,"green");
    printf("\033[1;32mappend_string test - Passed test: %d, Failed test: 0\033[0m\n",APPEND_STRING_TOTAL_TEST);
    color_print(TEST_OUTLINE,"green");
    printf("\n\n");
  }
  else
  {
    printf("\n");
    color_print(TEST_OUTLINE,"red");
    printf("\033[1;31mappend_string test - Passed test: %d, Failed test: %d\033[0m\n",count_test,APPEND_STRING_TOTAL_TEST-count_test);
    color_print(TEST_OUTLINE,"red");
    printf("\n\n");
  } 
  return SEND_AND_RECEIVE_DATA_SOCKET_TOTAL_TEST - count_test;
}*/

void test()
{
  // Constants
  #define XCASH_PROOF_OF_STAKE_TOTAL_TEST 70

  // Variables
  int xcash_proof_of_stake_total_passed_test = 0;

  // write the test message
  color_print("X-CASH Proof Of Stake\nVersion 1.0.0","green");
  printf("\nStarting Test\n\n");

  // initialize the variables
  string2_test = (char*)calloc(BUFFER_SIZE,sizeof(char));
  result_test = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  data_test = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // run the tests
  xcash_proof_of_stake_total_passed_test += append_string_test();
  xcash_proof_of_stake_total_passed_test += parse_json_data_test();
  //send_and_receive_data_socket_test();

  // write the end test message
  if (xcash_proof_of_stake_total_passed_test == XCASH_PROOF_OF_STAKE_TOTAL_TEST)
  {
    printf("\n");
    color_print(TEST_OUTLINE,"green");
    printf("\033[1;32mX-CASH proof of stake test results - Passed test: %d, Failed test: 0\033[0m\n",XCASH_PROOF_OF_STAKE_TOTAL_TEST);
    color_print(TEST_OUTLINE,"green");
    printf("\n\n");
  }
  else
  {
    printf("\n");
    color_print(TEST_OUTLINE,"red");
    printf("\033[1;31mX-CASH proof of stake test results - Passed test: %d, Failed test: %d\033[0m\n",xcash_proof_of_stake_total_passed_test,XCASH_PROOF_OF_STAKE_TOTAL_TEST-xcash_proof_of_stake_total_passed_test);
    color_print(TEST_OUTLINE,"red");
    printf("\n\n");
  }

  pointer_reset(string2_test);
  pointer_reset(result_test);
  pointer_reset(data_test);

}