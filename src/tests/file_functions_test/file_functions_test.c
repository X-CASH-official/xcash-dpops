#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"

#include "file_functions.h"
#include "thread_functions.h"

#include "variables_test.h"
#include "file_functions_test.h"
#include <mongoc/mongoc.h>
#include <bson/bson.h>

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: file_functions_test
Description: Test the read_file and write_file functions
Return: The number of passed file_functions test
-----------------------------------------------------------------------------------------------------------
*/

int file_functions_test(void)
{   
  // Variables
  pthread_t thread_id;

  // define macros
  #define READ_AND_WRITE_FILE_TEST 6
  #define NODES_PUBLIC_ADDRESS_LIST_FILE_NAME_COPY "nodes_public_address_list_copy.txt"
  #define MESSAGE "XCASH_PROOF_OF_STAKE_TEST_DATA"

  // reset the variables
  memset(data_test,0,strlen(data_test));
  memset(result_test,0,strlen(result_test));
  count_test = 0;

  // write the start test message
  color_print(TEST_OUTLINE,"blue");
  fprintf(stderr,"\033[1;34mfile functions test - Total test: %d\033[0m\n",READ_AND_WRITE_FILE_TEST);
  color_print(TEST_OUTLINE,"blue");
  fprintf(stderr,"\n");

  // run the test

  // write the file
  memcpy(data_test,MESSAGE,sizeof(MESSAGE)-1);
  if (write_file(data_test,NODES_PUBLIC_ADDRESS_LIST_FILE_NAME_COPY) == 0)
  {
    color_print("FAILED! Test for writing the file","red");
  }
  else
  {
    color_print("PASSED! Test for writing the file","green");
    count_test++;
  }

  // append and read the file
  if (append_file("|",NODES_PUBLIC_ADDRESS_LIST_FILE_NAME_COPY) == 0 || append_file(data_test,NODES_PUBLIC_ADDRESS_LIST_FILE_NAME_COPY) == 0 || read_file((unsigned char*)result_test,NODES_PUBLIC_ADDRESS_LIST_FILE_NAME_COPY) == 0 || memcmp(result_test,"XCASH_PROOF_OF_STAKE_TEST_DATA|XCASH_PROOF_OF_STAKE_TEST_DATA",61) != 0)
  {
    color_print("FAILED! Test for appending the file","red");
    color_print("FAILED! Test for reading the file","red");
  }
  else
  {
    color_print("PASSED! Test for appending the file","green");
    color_print("PASSED! Test for reading the file","green");
    count_test += 2;
  }
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

  // append the file
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  struct append_file_thread_parameters append_file_thread_parameters = {MESSAGE,NODES_PUBLIC_ADDRESS_LIST_FILE_NAME_COPY};
  pthread_create(&thread_id, NULL, &append_file_thread,(void *)&append_file_thread_parameters);
  if (thread_settings(thread_id) == 0)
  {
    color_print("FAILED! Test for appending the file on a seperate thread","red");
  }
  if (strncmp(data_test,MESSAGE,BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for appending the file on a seperate thread","red");
  }
  color_print("PASSED! Test for appending the file on a seperate thread","green");
  count_test++;

  // read the file
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  struct read_file_thread_parameters read_file_thread_parameters = {(unsigned char*)data_test,NODES_PUBLIC_ADDRESS_LIST_FILE_NAME_COPY};
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
    fprintf(stderr,"\n");
    color_print(TEST_OUTLINE,"green");
    fprintf(stderr,"\033[1;32mfile functions test - Passed test: %d, Failed test: 0\033[0m\n",READ_AND_WRITE_FILE_TEST);
    color_print(TEST_OUTLINE,"green");
    fprintf(stderr,"\n\n");
  }
  else
  {
    fprintf(stderr,"\n");
    color_print(TEST_OUTLINE,"red");
    fprintf(stderr,"\033[1;31mfile functions test - Passed test: %d, Failed test: %d\033[0m\n",count_test,READ_AND_WRITE_FILE_TEST-count_test);
    color_print(TEST_OUTLINE,"red");
    fprintf(stderr,"\n\n");
  } 
  return count_test;

  #undef READ_AND_WRITE_FILE_TEST
  #undef NODES_PUBLIC_ADDRESS_LIST_FILE_NAME_COPY
  #undef MESSAGE
}