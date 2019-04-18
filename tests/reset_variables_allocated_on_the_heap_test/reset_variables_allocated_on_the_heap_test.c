#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "define_macros_functions.h"
#include "file_functions.h"
#include "network_daemon_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "network_wallet_functions.h"
#include "blockchain_functions.h"
#include "server_functions.h"
#include "string_functions.h"
#include "thread_functions.h"

#include "define_macros_test.h"
#include "variables_test.h"
#include "network_functions_test.h"
#include "reset_variables_allocated_on_the_heap_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: get_program_memory_usage
Description: Gets the current memory usage of the program
Parameters:
   process_id_file - The file to read the memory usage from
Return: 0 if an error has occured, otherwise the programs memory usage in KB
-----------------------------------------------------------------------------------------------------------
*/

size_t get_program_memory_usage(char* process_id_file)
{
  // Variables
  FILE* file;
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t programs_memory_usage = 0;

  if (data == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // read the current system memory usage
  file = fopen(process_id_file,"r");
  if (file != NULL)
  {
    while(fgets(data,BUFFER_SIZE,file) != NULL)
    {
      if (strstr(data,"VmSize:") != NULL)
      {
        string_replace(data,"VmSize:","");
        string_replace(data,"	",""); // tab
        string_replace(data," ",""); // space
        string_replace(data,"kB","");
        string_replace(data,"\n","");
        programs_memory_usage = atoi(data);
      }
      memset(data,0,strnlen(data,BUFFER_SIZE));
    }
    fclose(file);
  }
  pointer_reset(data);
  return programs_memory_usage;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: reset_variables_allocated_on_the_heap_test
Description: Test each function to make sure that the variables are reset that were allocated on the heap
Return: The number of passed reset_variables_allocated_on_the_heap test
-----------------------------------------------------------------------------------------------------------
*/

int reset_variables_allocated_on_the_heap_test()
{
  // Constants
  const char STR1_TEST [BUFFER_SIZE] = "test string 1";
  const char* STR2_TEST = "test string 2";

  // Variables
  char* process_id_file = (char*)calloc(BUFFER_SIZE,sizeof(char));
  int previous_system_memory_usage = 0;
  int current_system_memory_usage = 0;
  int current_memory_usage = 0;
  size_t count = 0;
  size_t count2 = 0;
  size_t counter = 0;
  pthread_t thread_id;
  int settings2 = 1;

  // define macros
  #define RESET_VARAIBLES_ALLOCATED_ON_THE_HEAP_TEST 11
  #define GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA "{\r\n  \"id\": \"0\",\r\n  \"jsonrpc\": \"2.0\",\r\n  \"result\": {\r\n    \"blockhashing_blob\": \"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\",\r\n    \"blocktemplate_blob\": \"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\",\r\n    \"difficulty\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,\r\n    \"expected_reward\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,\r\n    \"height\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,\r\n    \"prev_hash\": \"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\",\r\n    \"reserved_offset\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,\r\n    \"status\": \"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\",\r\n    \"untrusted\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\r\n  }\r\n}"
  
  if (process_id_file == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // reset the variables
  memset(&string1_test,0,sizeof(string1_test)); 
  memset(string2_test,0,strnlen(string2_test,BUFFER_SIZE)); 
  count_test = 0;  
  
  // get the process_id_file
  memcpy(process_id_file,"/proc/",6);
  sprintf(process_id_file+6,"%d",getpid());
  memcpy(process_id_file+strnlen(process_id_file,BUFFER_SIZE),"/status",7);

  // write the start test message
  color_print(TEST_OUTLINE,"blue");
  printf("\033[1;34mreset_variables_allocated_on_the_heap test - Total test: %d\033[0m\n",RESET_VARAIBLES_ALLOCATED_ON_THE_HEAP_TEST);
  color_print(TEST_OUTLINE,"blue");
  printf("\n");

  // run the test

  // append_string   
  // read the current system memory usage
  previous_system_memory_usage = get_program_memory_usage(process_id_file);
  for (count = 0; count <= 1000; count++)
  {
    // test for using append_string on an empty statically allocated char
    append_string(string1_test,STR1_TEST);
    // test for using append_string on a statically allocated char
    append_string(string1_test," ");
    append_string(string1_test,STR1_TEST);
    // test for using append_string on an empty dynamically allocated char
    append_string(string2_test,STR2_TEST);
    // test for using append_string on a dynamically allocated char
    append_string(string2_test," ");
    memset(&string1_test,0,sizeof(string1_test)); 
    memset(string2_test,0,strnlen(string2_test,BUFFER_SIZE)); 
    if (count == 0)
    {    
      current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
    }
    if (count == 10)
    {
      current_system_memory_usage = get_program_memory_usage(process_id_file);
      if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
      {
        color_print("FAILED! append_string has not reset all variables allocated on the heap","red");
        settings2 = 0;
        break;
      }      
    }
    if (count == 100)
    {
      current_system_memory_usage = get_program_memory_usage(process_id_file);
      if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
      {
        color_print("FAILED! append_string has not reset all variables allocated on the heap","red");
        settings2 = 0;
        break;
      }  
    }
    if (count == 1000)
    {
      current_system_memory_usage = get_program_memory_usage(process_id_file);
      if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 250 && current_memory_usage > 0)
      {
        color_print("FAILED! append_string has not reset all variables allocated on the heap","red");
        settings2 = 0;
        break;
      }
      else
      {
        color_print("PASSED! append_string has reset all variables allocated on the heap","green");
        count_test++;
      } 
    }    
  }




  // parse_json_data   
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
      memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
      append_string(result_test,GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA);
      parse_json_data(result_test,"blocktemplate_blob",data_test);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! parse_json_data has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! parse_json_data has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 250 && current_memory_usage > 0)
        {
          color_print("FAILED! parse_json_data has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! parse_json_data has reset all variables allocated on the heap","green");
          count_test++;
        } 
      }    
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // random_string   
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
      random_string(result_test,RANDOM_STRING_LENGTH);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! random_string has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! random_string has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 250 && current_memory_usage > 0)
        {
          color_print("FAILED! random_string has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! random_string has reset all variables allocated on the heap","green");
          count_test++;
        } 
      }    
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }




  // string_count   
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
      memcpy(result_test,TEST_OUTLINE,strnlen(TEST_OUTLINE,BUFFER_SIZE));
      string_count(result_test,"-");
      string_count(result_test,"--");
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! string_count has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! string_count has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 250 && current_memory_usage > 0)
        {
          color_print("FAILED! string_count has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! string_count has reset all variables allocated on the heap","green");
          count_test++;
        } 
      }    
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // string_replace 
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
      append_string(result_test,"{\r\n \"message_settings\": \"string_replace_test\",\r\n}");
      string_replace(result_test,"string_replace_test","string_replace");
      memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
      append_string(result_test,"{\r\n \"message_settings\": \"string_replace_test\",\r\n}");
      string_replace(result_test,"\"","\\\"");
      memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
      append_string(result_test,"{\r\n \"message_settings\": \"string_replace_test\",\r\n}");
      string_replace(result_test,"_test","");
      memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
      append_string(result_test,"{\r\n \"message_settings\": \"string_replace\",\r\n}");
      string_replace(result_test,"string_replace_test","");
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! string_replace has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! string_replace has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 250 && current_memory_usage > 0)
        {
          color_print("FAILED! string_replace has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! string_replace has reset all variables allocated on the heap","green");
          count_test++;
        } 
      }    
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // varint_decode  
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      varint_decode(0xb2750199a302);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! varint_decode has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! varint_decode has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! varint_decode has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! varint_decode has reset all variables allocated on the heap","green");
          count_test++;
        } 
      }    
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // get_block_template 
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for get_block_template: %zu / 1000",count);
      fprintf(stderr,"\r");
      memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
      get_block_template(data_test,"0",0);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! get_block_template has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      } 
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! get_block_template has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      } 
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! get_block_template has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
        else
        {
          color_print("PASSED! get_block_template has reset all variables allocated on the heap","green");
          count_test++;
        }      
      } 
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // get_current_block_height 
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for get_current_block_height: %zu / 1000",count);
      fprintf(stderr,"\r");
      memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
      get_current_block_height(data_test,0);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! get_current_block_height has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      } 
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! get_current_block_height has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      } 
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! get_current_block_height has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
        else
        {
          color_print("PASSED! get_current_block_height has reset all variables allocated on the heap","green");
          count_test++;
        }      
      } 
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // get_previous_block_hash 
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for get_previous_block_hash: %zu / 1000",count);
      fprintf(stderr,"\r");
      memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
      get_previous_block_hash(data_test,0);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! get_previous_block_hash has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      } 
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! get_previous_block_hash has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      } 
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! get_previous_block_hash has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
        else
        {
          color_print("PASSED! get_previous_block_hash has reset all variables allocated on the heap","green");
          count_test++;
        }      
      } 
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // set the current_round_part and current_round_part_backup_node
  memset(current_round_part,0,strnlen(current_round_part,BUFFER_SIZE));
  memset(current_round_part_backup_node,0,strnlen(current_round_part_backup_node,BUFFER_SIZE));
  memcpy(current_round_part,"1",1);
  memcpy(current_round_part_backup_node,"0",1); 

  // sign_data 
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for sign_data: %zu / 1000",count);
      fprintf(stderr,"\r");
      memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
      memcpy(data_test,"{\r\n \"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\",\r\n}",60);
      sign_data(data_test,0);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! sign_data has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      } 
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! sign_data has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      } 
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! sign_data has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
        else
        {
          color_print("PASSED! sign_data has reset all variables allocated on the heap","green");
          count_test++;
        }      
      } 
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // verify_data 
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for verify_data: %zu / 1000",count);
      fprintf(stderr,"\r");
      memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
      memcpy(data_test,"{\r\n \"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\",\r\n}",60);
      sign_data(data_test,0);
      verify_data(data_test,0,1,1);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! verify_data has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      } 
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! verify_data has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      } 
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! verify_data has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
        else
        {
          color_print("PASSED! verify_data has reset all variables allocated on the heap","green");
          count_test++;
        }      
      } 
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // set the server_message
  memset(server_message,0,strnlen(server_message,BUFFER_SIZE));
  memcpy(server_message,"XCASH_PROOF_OF_STAKE_TEST_DATA",30);
  // create the server
  pthread_create(&thread_id, NULL, &create_server_on_separate_thread,NULL);
  pthread_detach(thread_id);
  sleep(1);

  // send_and_receive_data_socket 
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for send_and_receive_data_socket: %zu / 1000",count);
      fprintf(stderr,"\r");
      memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
      memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
      memcpy(data_test,"{\r\n \"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\",\r\n}",60);
      sign_data(data_test,0);
      send_and_receive_data_socket(result_test,"127.0.0.1",SEND_DATA_PORT,data_test,TOTAL_CONNECTION_TIME_SETTINGS,"XCASH_PROOF_OF_STAKE_TEST_DATA",0);
      verify_data(result_test,0,1,1);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! send_and_receive_data_socket has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      } 
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! send_and_receive_data_socket has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      } 
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! send_and_receive_data_socket has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
        else
        {
          color_print("PASSED! send_and_receive_data_socket has reset all variables allocated on the heap","green");
          count_test++;
        }      
      } 
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }
  

  // write the end test message
  if (count_test == RESET_VARAIBLES_ALLOCATED_ON_THE_HEAP_TEST)
  {
    printf("\n");
    color_print(TEST_OUTLINE,"green");
    printf("\033[1;32mreset_variables_allocated_on_the_heap test - Passed test: %d, Failed test: 0\033[0m\n",RESET_VARAIBLES_ALLOCATED_ON_THE_HEAP_TEST);
    color_print(TEST_OUTLINE,"green");
    printf("\n\n");
  }
  else
  {
    printf("\n");
    color_print(TEST_OUTLINE,"red");
    printf("\033[1;31mreset_variables_allocated_on_the_heap test - Passed test: %d, Failed test: 1\033[0m\n",count_test);
    color_print(TEST_OUTLINE,"red");
    printf("\n\n");
    exit(0);
  } 
  pointer_reset(process_id_file);
  return count_test;

  #undef RESET_VARAIBLES_ALLOCATED_ON_THE_HEAP_TEST
  #undef GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA
}