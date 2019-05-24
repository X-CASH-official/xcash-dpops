#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"

#include "database_functions.h"
#include "thread_functions.h"

#include "define_macros_test.h"
#include "variables_test.h"
#include "database_functions_test.h"
#include "network_functions.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: database_test
Description: Test the database functions
Return: The number of passed database test
-----------------------------------------------------------------------------------------------------------
*/

int database_test()
{   
  // Variables  
  int count;

  // define macros
  #define DATABASE_TEST 25

  // reset the variables
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  count_test = 0;

  // write the start test message
  color_print(TEST_OUTLINE,"blue");
  printf("\033[1;34mdatabase test - Total test: %d\033[0m\n",DATABASE_TEST);
  color_print(TEST_OUTLINE,"blue");
  printf("\n");

  // run the test


  // write the end test message
  if (count_test == DATABASE_TEST)
  {
    printf("\n");
    color_print(TEST_OUTLINE,"green");
    printf("\033[1;32mdatabase test - Passed test: %d, Failed test: 0\033[0m\n",DATABASE_TEST);
    color_print(TEST_OUTLINE,"green");
    printf("\n\n");
  }
  else
  {
    printf("\n");
    color_print(TEST_OUTLINE,"red");
    printf("\033[1;31mdatabase test - Passed test: %d, Failed test: %d\033[0m\n",count_test,DATABASE_TEST-count_test);
    color_print(TEST_OUTLINE,"red");
    printf("\n\n");
  }   
}