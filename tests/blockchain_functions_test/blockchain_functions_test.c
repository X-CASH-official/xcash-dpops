#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "define_macro_functions.h"
#include "define_macros.h"

#include "blockchain_functions.h"

#include "define_macros_test.h"
#include "variables_test.h"
#include "blockchain_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: varint_decode_test
Description: Test the varint_decode function
Return: The number of passed varint_decode test
-----------------------------------------------------------------------------------------------------------
*/

int varint_decode_test()
{   
  // Variables

  // define macros
  #define READ_AND_WRITE_FILE_TEST 1
  #define NODES_PUBLIC_ADDRESS_LIST_FILE_NAME_COPY "nodes_public_address_list_copy.txt"
  #define MESSAGE "XCASH_PROOF_OF_STAKE_TEST_DATA"

  // reset the variables
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  count_test = 0;

  // write the start test message
  color_print(TEST_OUTLINE,"blue");
  printf("\033[1;34mvarint_decode test - Total test: %d\033[0m\n",READ_AND_WRITE_FILE_TEST);
  color_print(TEST_OUTLINE,"blue");
  printf("\n");

  // run the test
  if (varint_decode(0xb2750199a302) == 78167177906)
  {
    color_print("PASSED! Test for varint decode","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for varint decode","red");
  }
  


  // write the end test message
  if (count_test == READ_AND_WRITE_FILE_TEST)
  {
    printf("\n");
    color_print(TEST_OUTLINE,"green");
    printf("\033[1;32mvarint_decode test - Passed test: %d, Failed test: 0\033[0m\n",READ_AND_WRITE_FILE_TEST);
    color_print(TEST_OUTLINE,"green");
    printf("\n\n");
  }
  else
  {
    printf("\n");
    color_print(TEST_OUTLINE,"red");
    printf("\033[1;31mvarint_decode test - Passed test: %d, Failed test: %d\033[0m\n",count_test,READ_AND_WRITE_FILE_TEST-count_test);
    color_print(TEST_OUTLINE,"red");
    printf("\n\n");
  } 
  return count_test;

  #undef READ_AND_WRITE_FILE_TEST
  #undef NODES_PUBLIC_ADDRESS_LIST_FILE_NAME_COPY
  #undef MESSAGE
}