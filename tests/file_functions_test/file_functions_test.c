#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"

#include "file_functions.h"
#include "thread_functions.h"

#include "define_macros_test.h"
#include "variables_test.h"
#include "file_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: read_and_write_file_test
Description: Test the read_file and write_file functions
Return: The number of passed read_and_write_file test
-----------------------------------------------------------------------------------------------------------
*/

int read_and_write_file_test()
{   
  // Variables
  pthread_t thread_id;

  // define macros
  #define READ_AND_WRITE_FILE_TEST 4
  #define NODES_PUBLIC_ADDRESS_LIST_FILE_NAME_COPY "nodes_public_address_list_copy.txt"
  #define MESSAGE "XCASH_PROOF_OF_STAKE_TEST_DATA"

  // reset the variables
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  count_test = 0;

  // write the start test message
  color_print(TEST_OUTLINE,"blue");
  printf("\033[1;34mread_and_write_file test - Total test: %d\033[0m\n",READ_AND_WRITE_FILE_TEST);
  color_print(TEST_OUTLINE,"blue");
  printf("\n");

  // run the test

  // write the file
  memcpy(data_test,MESSAGE,30);
  if (write_file(data_test,NODES_PUBLIC_ADDRESS_LIST_FILE_NAME_COPY) == 0)
  {
    color_print("FAILED! Test for writing the file","red");
  }
  else
  {
    color_print("PASSED! Test for writing the file","green");
    count_test++;
  }

  // read the file
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (read_file(data_test,NODES_PUBLIC_ADDRESS_LIST_FILE_NAME_COPY) == 0)
  {
    color_print("FAILED! Test for reading the file","red");
  }
  if (strncmp(data_test,MESSAGE,BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for reading the file","red");
  }
  color_print("PASSED! Test for reading the file","green");
  count_test++;
  remove(NODES_PUBLIC_ADDRESS_LIST_FILE_NAME_COPY);



  // write the file using a seperate thread  
  // create a struct for the parameters
  struct write_file_thread_parameters write_file_thread_parameters = {MESSAGE,NODES_PUBLIC_ADDRESS_LIST_FILE_NAME_COPY};
  pthread_create(&thread_id, NULL, &write_file_thread,(void *)&write_file_thread_parameters);
  if (thread_settings(thread_id) == 0)
  {
    color_print("FAILED! Test for writing the file on a seperate thread","red");
  }
  else
  {
    color_print("PASSED! Test for writing the file on a seperate thread","green");
    count_test++;
  }

   // read the file
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  struct read_file_thread_parameters read_file_thread_parameters = {data_test,NODES_PUBLIC_ADDRESS_LIST_FILE_NAME_COPY};
  pthread_create(&thread_id, NULL, &read_file_thread,(void *)&read_file_thread_parameters);
  if (thread_settings(thread_id) == 0)
  {
    color_print("FAILED! Test for reading the file on a seperate thread","red");
  }
  if (strncmp(data_test,MESSAGE,BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for reading the file on a seperate thread","red");
  }
  color_print("PASSED! Test for reading the file on a seperate thread","green");
  count_test++;
  remove(NODES_PUBLIC_ADDRESS_LIST_FILE_NAME_COPY);



  // write the end test message
  if (count_test == READ_AND_WRITE_FILE_TEST)
  {
    printf("\n");
    color_print(TEST_OUTLINE,"green");
    printf("\033[1;32mread_and_write_file test - Passed test: %d, Failed test: 0\033[0m\n",READ_AND_WRITE_FILE_TEST);
    color_print(TEST_OUTLINE,"green");
    printf("\n\n");
  }
  else
  {
    printf("\n");
    color_print(TEST_OUTLINE,"red");
    printf("\033[1;31mread_and_write_file test - Passed test: %d, Failed test: %d\033[0m\n",count_test,READ_AND_WRITE_FILE_TEST-count_test);
    color_print(TEST_OUTLINE,"red");
    printf("\n\n");
  } 
  return count_test;

  #undef READ_AND_WRITE_FILE_TEST
  #undef NODES_PUBLIC_ADDRESS_LIST_FILE_NAME_COPY
  #undef MESSAGE
}