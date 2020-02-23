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
#include "block_verifiers_synchronize_check_functions_test.h"
#include "block_verifiers_synchronize_functions_test.h"
#include "block_verifiers_synchronize_server_functions_test.h"
#include "block_verifiers_thread_server_functions_test.h"
#include "block_verifiers_update_functions_test.h"
#include "database_functions_test.h"
#include "delete_database_functions.h"
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
#include "optimizations_functions_test.h"
#include "organize_functions_test.h"
#include "reset_variables_allocated_on_the_heap_test.h"
#include "shared_delegate_website_functions_test.h"
#include "shared_delegate_website_thread_server_functions_test.h"
#include "string_functions_test.h"
#include "VRF_functions_test.h"
#include "XCASH_DPOPS_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Name: Global Variables
-----------------------------------------------------------------------------------------------------------
*/

char string1_test[BUFFER_SIZE];
char string2_test[BUFFER_SIZE];
char result_test[BUFFER_SIZE];
char data_test[BUFFER_SIZE];
int count_test;



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

void test(int settings)
{
  // Variables
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int XCASH_DPOPS_total_passed_test = 0;
  int total_test = XCASH_DPOPS_TOTAL_TEST;

  // initialize the variables
  memset(string2_test,0,sizeof(string2_test));
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  network_functions_test_error_settings = 1;
  network_functions_test_server_messages_settings = 0;
  test_settings = 1;
  debug_settings = 1;

  // set the database to the test database
  memset(database_name,0,sizeof(database_name));
  memcpy(database_name,DATABASE_NAME_TEST,sizeof(DATABASE_NAME_TEST)-1);
  memset(shared_delegates_database_name,0,sizeof(shared_delegates_database_name));
  memcpy(shared_delegates_database_name,DATABASE_NAME_TEST,sizeof(DATABASE_NAME_TEST)-1);

  // reset the test database
  delete_database(DATABASE_COLLECTION_TEST,0);

  // print the start message
  memcpy(data_test,TEST_OUTLINE,sizeof(TEST_OUTLINE)-1);
  memcpy(data_test+strlen(data_test),"\n",1);
  memcpy(data_test+strlen(data_test),"Starting XCASH_DPOPS Test\n\n\nSome errors may happen during the test towards the last test since they are run 1000 times.\nPlease ignore these errors and only look at the end test results for each section and the last section to see if you have any errors.",25);
  color_print(data_test,"yellow");
  get_current_UTC_time(current_date_and_time,current_UTC_date_and_time); 
  memset(data_test,0,sizeof(data_test));
  strftime(data_test,sizeof(data_test),"%a %d %b %Y %H:%M:%S UTC",&current_UTC_date_and_time);
  memcpy(data_test+strlen(data_test),"\n",1);
  memcpy(data_test+strlen(data_test),TEST_OUTLINE,sizeof(TEST_OUTLINE)-1);
  memcpy(data_test+strlen(data_test),"\n",1);
  color_print(data_test,"yellow");
  
  // run the tests
  if (settings == 2)
  {
    XCASH_DPOPS_total_passed_test += network_functions_test();
    XCASH_DPOPS_total_passed_test += optimizations_functions_test();
    total_test = OPTIMIZATIONS_TOTAL_TEST + NETWORK_FUNCTIONS_TEST;
  }
  else
  {
    XCASH_DPOPS_total_passed_test += blockchain_functions_test();
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
    //XCASH_DPOPS_total_passed_test += optimizations_functions_test();
    XCASH_DPOPS_total_passed_test += organize_functions_test();
    XCASH_DPOPS_total_passed_test += string_functions_test();
    XCASH_DPOPS_total_passed_test += VRF_functions_test();
    XCASH_DPOPS_total_passed_test += block_verifiers_server_functions_test();
    XCASH_DPOPS_total_passed_test += block_verifiers_synchronize_check_functions_test();
    XCASH_DPOPS_total_passed_test += block_verifiers_synchronize_functions_test();
    XCASH_DPOPS_total_passed_test += block_verifiers_synchronize_server_functions_test();
    XCASH_DPOPS_total_passed_test += block_verifiers_thread_server_functions_test();
    XCASH_DPOPS_total_passed_test += block_verifiers_update_functions_test();
    XCASH_DPOPS_total_passed_test += delegate_server_functions_test();
    XCASH_DPOPS_total_passed_test += delegate_website_functions_test();
    XCASH_DPOPS_total_passed_test += shared_delegate_website_functions_test();
    XCASH_DPOPS_total_passed_test += shared_delegate_website_thread_server_functions_test();
    if (settings == 0)
    {
      XCASH_DPOPS_total_passed_test += reset_variables_allocated_on_the_heap_test();
    }
    else
    {
      total_test -= RESET_VARAIBLES_ALLOCATED_ON_THE_HEAP_TEST;
    }
  }

  // reset the variables
  memset(string2_test,0,sizeof(string2_test));
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));

  // delete the database
  delete_database(DATABASE_NAME_TEST,0);

  // write the end test message
  memcpy(data_test,TEST_OUTLINE,sizeof(TEST_OUTLINE)-1);
  memcpy(data_test+strlen(data_test),"\n",1);
  memcpy(data_test+strlen(data_test),"XCASH_DPOPS Test Results - Passed test: ",40);
  snprintf(data_test+strlen(data_test),sizeof(data_test)-1,"%d",XCASH_DPOPS_total_passed_test);
  memcpy(data_test+strlen(data_test),", Failed test: ",15);
  snprintf(data_test+strlen(data_test),sizeof(data_test)-1,"%d",total_test-XCASH_DPOPS_total_passed_test);
  color_print(data_test,"yellow");
  get_current_UTC_time(current_date_and_time,current_UTC_date_and_time); 
  memset(data_test,0,sizeof(data_test));
  strftime(data_test,sizeof(data_test),"%a %d %b %Y %H:%M:%S UTC",&current_UTC_date_and_time);
  memcpy(data_test+strlen(data_test),"\n",1);
  memcpy(data_test+strlen(data_test),TEST_OUTLINE,sizeof(TEST_OUTLINE)-1);
  memcpy(data_test+strlen(data_test),"\n",1);
  color_print(data_test,"yellow");
  return;
}