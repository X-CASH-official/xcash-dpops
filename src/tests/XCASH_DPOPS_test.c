#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "define_macros_test.h"

#include "variables_test.h"
#include "analysing_code_test.h"
#include "blockchain_functions_test.h"
#include "block_verifiers_server_functions_test.h"
#include "block_verifiers_synchronize_server_functions_test.h"
#include "database_functions_test.h"
#include "delegate_server_functions_test.h"
#include "delegate_website_functions_test.h"
#include "insert_database_functions_test.h"
#include "read_database_functions_test.h"
#include "update_database_functions_test.h"
#include "delete_database_functions_test.h"
#include "count_database_functions_test.h"
#include "file_functions_test.h"
#include "network_daemon_functions_test.h"
#include "network_functions_test.h"
#include "network_security_functions_test.h"
#include "network_wallet_functions_test.h"
#include "organize_functions_test.h"
#include "reset_variables_allocated_on_the_heap_test.h"
#include "shared_delegate_website_functions_test.h"
#include "shared_delegate_website_thread_server_functions_test.h"
#include "string_functions_test.h"
#include "VRF_functions_test.h"
#include "XCASH_DPOPS_test.h"

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

void test(void)
{
  // Variables
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int XCASH_DPOPS_total_passed_test = 0;  

  // initialize the variables
  memset(string2_test,0,sizeof(string2_test));
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));

  // print the start message
  memcpy(data_test,TEST_OUTLINE,sizeof(TEST_OUTLINE)-1);
  memcpy(data_test+strlen(data_test),"\n",1);
  memcpy(data_test+strlen(data_test),"Starting XCASH_DPOPS Test",25);
  color_print(data_test,"yellow");
  get_current_UTC_time(current_date_and_time,current_UTC_date_and_time); 
  memset(data_test,0,sizeof(data_test));
  strftime(data_test,sizeof(data_test),"%a %d %b %Y %H:%M:%S UTC",&current_UTC_date_and_time);
  memcpy(data_test+strlen(data_test),"\n",1);
  memcpy(data_test+strlen(data_test),TEST_OUTLINE,sizeof(TEST_OUTLINE)-1);
  memcpy(data_test+strlen(data_test),"\n",1);
  color_print(data_test,"yellow");
  
  // run the tests
  XCASH_DPOPS_total_passed_test += network_functions_test();
  XCASH_DPOPS_total_passed_test += shared_delegate_website_thread_server_functions_test();
  /*XCASH_DPOPS_total_passed_test += blockchain_functions_test();
  XCASH_DPOPS_total_passed_test += database_test(); 
  XCASH_DPOPS_total_passed_test += insert_database_functions_test(); 
  XCASH_DPOPS_total_passed_test += read_database_functions_test();
  XCASH_DPOPS_total_passed_test += update_database_functions_test();
  XCASH_DPOPS_total_passed_test += delete_database_functions_test();
  XCASH_DPOPS_total_passed_test += count_database_functions_test();
  XCASH_DPOPS_total_passed_test += file_functions_test();
  XCASH_DPOPS_total_passed_test += network_daemon_functions_test();
  XCASH_DPOPS_total_passed_test += network_functions_test();
  XCASH_DPOPS_total_passed_test += network_security_functions_test();
  XCASH_DPOPS_total_passed_test += network_wallet_functions_test();
  XCASH_DPOPS_total_passed_test += organize_functions_test();
  XCASH_DPOPS_total_passed_test += string_functions_test();
  XCASH_DPOPS_total_passed_test += VRF_functions_test();
  XCASH_DPOPS_total_passed_test += block_verifiers_server_functions_test();
  XCASH_DPOPS_total_passed_test += block_verifiers_synchronize_server_functions_test();
  XCASH_DPOPS_total_passed_test += delegate_server_functions_test();
  XCASH_DPOPS_total_passed_test += delegate_website_functions_test();
  XCASH_DPOPS_total_passed_test += shared_delegate_website_functions_test();
  XCASH_DPOPS_total_passed_test += shared_delegate_website_thread_server_functions_test();
  XCASH_DPOPS_total_passed_test += reset_variables_allocated_on_the_heap_test();*/
  //XCASH_DPOPS_total_passed_test += analysing_code_test();

  // write the end test message
  if (XCASH_DPOPS_total_passed_test == XCASH_DPOPS_TOTAL_TEST)
  {
    fprintf(stderr,"\n");
    color_print(TEST_OUTLINE,"green");
    fprintf(stderr,"\033[1;32mXCASH DPOPS test results - Passed test: %d, Failed test: 0\033[0m\n",XCASH_DPOPS_TOTAL_TEST);
    color_print(TEST_OUTLINE,"green");
    fprintf(stderr,"\n\n");
  }
  else
  {
    fprintf(stderr,"\n");
    color_print(TEST_OUTLINE,"red");
    fprintf(stderr,"\033[1;31mXCASH DPOPS test results - Passed test: %d, Failed test: %d\033[0m\n",XCASH_DPOPS_total_passed_test,XCASH_DPOPS_TOTAL_TEST-XCASH_DPOPS_total_passed_test);
    color_print(TEST_OUTLINE,"red");
    fprintf(stderr,"\n\n");
  }
  return;
}