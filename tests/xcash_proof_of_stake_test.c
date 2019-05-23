#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "define_macro_functions.h"
#include "define_macros.h"

#include "define_macros_functions.h"

#include "define_macros_test.h"
#include "variables_test.h"
#include "analysing_code_test.h"
#include "database_functions_test.h"
#include "file_functions_test.h"
#include "network_functions_test.h"
#include "blockchain_functions_test.h"
#include "reset_variables_allocated_on_the_heap_test.h"
#include "string_functions_test.h"
#include "VRF_functions_test.h"
#include "xcash_proof_of_stake_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: test
Description: Run the test
-----------------------------------------------------------------------------------------------------------
*/

void test()
{
  // Variables
  int xcash_proof_of_stake_total_passed_test = 0;

  // define macros
  #define XCASH_PROOF_OF_STAKE_TOTAL_TEST 85

  // write the test message
  printf("Starting Test\n\n");

  // initialize the variables
  string2_test = (char*)calloc(BUFFER_SIZE,sizeof(char));
  result_test = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  data_test = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // check if the memory needed was allocated on the heap successfully
  if (string2_test == NULL || result_test == NULL || data_test == NULL)
  {
    if (string2_test != NULL)
    {
      pointer_reset(string2_test);
    }
    if (result_test != NULL)
    {
      pointer_reset(result_test);
    }
    if (data_test != NULL)
    {
      pointer_reset(data_test);
    }
    return;
  } 

  // run the tests
  //xcash_proof_of_stake_total_passed_test += blockchain_functions_test();
  //xcash_proof_of_stake_total_passed_test += database_test(); 
  //xcash_proof_of_stake_total_passed_test += file_functions_test();
  //xcash_proof_of_stake_total_passed_test += network_functions_test();
  xcash_proof_of_stake_total_passed_test += string_functions_test();
  //xcash_proof_of_stake_total_passed_test += VRF_functions_test();
  //xcash_proof_of_stake_total_passed_test += reset_variables_allocated_on_the_heap_test();
  //xcash_proof_of_stake_total_passed_test += analysing_code_test();

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
  return;

  #undef XCASH_PROOF_OF_STAKE_TOTAL_TEST
}