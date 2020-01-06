#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "define_macros_test.h"

#include "string_functions.h"

#include "variables_test.h"
#include "string_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: string_functions_test
Description: Test the string functions
Return: The number of passed string_functions test
-----------------------------------------------------------------------------------------------------------
*/

int string_functions_test(void)
{
  // define macros
  #define STRING_FUNCTIONS_TOTAL_TEST 7
  #define DATABASE_DATA 14
  #define MESSAGE "{\r\n \"message_settings_1\": \"XCASH_DPOPS_TEST_DATA_1\",\r\n \"message_settings_2\": \"XCASH_DPOPS_TEST_DATA_2\",\r\n}"
  #define DATA1 "{\"username\":\"XCASH\",\"most_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_total_rounds\":\"5\",\"best_block_verifier_online_percentage_delegate_name\":\"DELEGATE_NAME\",\"best_block_verifier_online_percentage\":\"10\",\"most_block_producer_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_block_producer_total_rounds\":\"15\",\"most_VRF_node_public_and_private_key_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_VRF_node_public_and_private_key_total_rounds\":\"5\",\"most_VRF_node_random_data_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_VRF_node_random_data_total_rounds\":\"10\",\"total_XCASH_proof_of_stake_rounds\":\"15\",\"total_coins_in_proof_of_stake\":\"5\",\"total_circulating_supply_percentage_in_proof_of_stake\":\"10\"}"
  #define DATA2 "[{\"username\":\"XCASH\",\"most_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_total_rounds\":\"5\",\"best_block_verifier_online_percentage_delegate_name\":\"DELEGATE_NAME\",\"best_block_verifier_online_percentage\":\"10\",\"most_block_producer_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_block_producer_total_rounds\":\"15\",\"most_VRF_node_public_and_private_key_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_VRF_node_public_and_private_key_total_rounds\":\"5\",\"most_VRF_node_random_data_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_VRF_node_random_data_total_rounds\":\"10\",\"total_XCASH_proof_of_stake_rounds\":\"15\",\"total_coins_in_proof_of_stake\":\"5\",\"total_circulating_supply_percentage_in_proof_of_stake\":\"10\"},{\"username\":\"XCASH\",\"most_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_total_rounds\":\"5\",\"best_block_verifier_online_percentage_delegate_name\":\"DELEGATE_NAME\",\"best_block_verifier_online_percentage\":\"10\",\"most_block_producer_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_block_producer_total_rounds\":\"15\",\"most_VRF_node_public_and_private_key_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_VRF_node_public_and_private_key_total_rounds\":\"5\",\"most_VRF_node_random_data_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_VRF_node_random_data_total_rounds\":\"10\",\"total_XCASH_proof_of_stake_rounds\":\"15\",\"total_coins_in_proof_of_stake\":\"5\",\"total_circulating_supply_percentage_in_proof_of_stake\":\"10\"}]"
 
  // variables
  int count;
  int counter;
  struct database_document_fields database_data;
  struct database_multiple_documents_fields database_multiple_documents_fields;

  // reset the variables
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  count_test = 0;
 
  // write the start test message
  color_print(TEST_OUTLINE,"blue");
  fprintf(stderr,"\033[1;34mstring functions test - Total test: %d\033[0m\n",STRING_FUNCTIONS_TOTAL_TEST);
  color_print(TEST_OUTLINE,"blue");
  fprintf(stderr,"\n");
 
  // run the test
 
  // test parse_json_data
  memcpy(result_test,MESSAGE,sizeof(MESSAGE)-1);
  if (parse_json_data(result_test,"message_settings_1",data_test,sizeof(data_test)) == 1 && strncmp(data_test,"XCASH_DPOPS_TEST_DATA_1",sizeof(data_test)) == 0)
  {
    memset(data_test,0,sizeof(data_test));
    if (parse_json_data(result_test,"message_settings_2",data_test,sizeof(data_test)) == 1 && strncmp(data_test,"XCASH_DPOPS_TEST_DATA_2",sizeof(data_test)) == 0)
    {
      color_print("PASSED! Test for parse_json_data","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parse_json_data","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parse_json_data","red");
  }

  // test for random_string
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  if (random_string(result_test,RANDOM_STRING_LENGTH) == 1)
  {   
    color_print("PASSED! Test for random_string","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for random_string","red");
  }

  // initialize the database_document_fields struct 
  for (count = 0; count < DATABASE_DATA; count++)
  {
    database_data.item[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));
    database_data.value[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));

    if (database_data.item[count] == NULL || database_data.value[count] == NULL)
    {
      color_print("Could not allocate the memory needed on the heap","red");
      exit(0);
    }
  }

  memcpy(database_data.item[0],"username",8);
  memcpy(database_data.item[1],"most_total_rounds_delegate_name",31);
  memcpy(database_data.item[2],"most_total_rounds",17);
  memcpy(database_data.item[3],"best_block_verifier_online_percentage_delegate_name",51);
  memcpy(database_data.item[4],"best_block_verifier_online_percentage",37);
  memcpy(database_data.item[5],"most_block_producer_total_rounds_delegate_name",46);
  memcpy(database_data.item[6],"most_block_producer_total_rounds",32);
  memcpy(database_data.item[7],"most_VRF_node_public_and_private_key_total_rounds_delegate_name",63);
  memcpy(database_data.item[8],"most_VRF_node_public_and_private_key_total_rounds",49);
  memcpy(database_data.item[9],"most_VRF_node_random_data_total_rounds_delegate_name",52);
  memcpy(database_data.item[10],"most_VRF_node_random_data_total_rounds",38);
  memcpy(database_data.item[11],"total_XCASH_proof_of_stake_rounds",33);
  memcpy(database_data.item[12],"total_coins_in_proof_of_stake",29);
  memcpy(database_data.item[13],"total_circulating_supply_percentage_in_proof_of_stake",53);

  memcpy(database_data.value[0],"XCASH",5);  
  memcpy(database_data.value[1],"DELEGATE_NAME",13);
  memcpy(database_data.value[2],"5",1);
  memcpy(database_data.value[3],"DELEGATE_NAME",13);
  memcpy(database_data.value[4],"10",2);
  memcpy(database_data.value[5],"DELEGATE_NAME",13);  
  memcpy(database_data.value[6],"15",2);
  memcpy(database_data.value[7],"DELEGATE_NAME",13);  
  memcpy(database_data.value[8],"5",1);
  memcpy(database_data.value[9],"DELEGATE_NAME",13);  
  memcpy(database_data.value[10],"10",2);  
  memcpy(database_data.value[11],"15",2);
  memcpy(database_data.value[12],"5",1);
  memcpy(database_data.value[13],"10",2);

  database_data.count = DATABASE_DATA;

  if (create_json_data_from_database_document_array(&database_data,result_test,"") == 1)
  {
    if (strncmp(result_test,DATA1,BUFFER_SIZE) == 0)  
    {
      color_print("PASSED! Test for create_json_data_from_database_document_array","green");
      count_test++;
    }
    else
    { 
      color_print("FAILED! Test for create_json_data_from_database_document_array","red");
    }
  }
  else
  {
    color_print("FAILED! Test for create_json_data_from_database_document_array","red");
  }

  for (count = 0; count < DATABASE_DATA; count++)
  {
    pointer_reset(database_data.item[count]);
    pointer_reset(database_data.value[count]);
  }



  // create_json_data_from_database_multiple_documents_array
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));

  // initialize the database_multiple_documents_fields struct 
  for (count = 0; count < 2; count++)
  {
    for (counter = 0; counter < DATABASE_DATA; counter++)
    {
      database_multiple_documents_fields.item[count][counter] = (char*)calloc(BUFFER_SIZE,sizeof(char));
      database_multiple_documents_fields.value[count][counter] = (char*)calloc(BUFFER_SIZE,sizeof(char));

      if (database_multiple_documents_fields.item[count][counter] == NULL || database_multiple_documents_fields.value[count][counter] == NULL)
      {
        color_print("Could not allocate the memory needed on the heap","red");
        exit(0);
      }
    }
  }

  memcpy(database_multiple_documents_fields.item[0][0],"username",8);
  memcpy(database_multiple_documents_fields.item[0][1],"most_total_rounds_delegate_name",31);
  memcpy(database_multiple_documents_fields.item[0][2],"most_total_rounds",17);
  memcpy(database_multiple_documents_fields.item[0][3],"best_block_verifier_online_percentage_delegate_name",51);
  memcpy(database_multiple_documents_fields.item[0][4],"best_block_verifier_online_percentage",37);
  memcpy(database_multiple_documents_fields.item[0][5],"most_block_producer_total_rounds_delegate_name",46);
  memcpy(database_multiple_documents_fields.item[0][6],"most_block_producer_total_rounds",32);
  memcpy(database_multiple_documents_fields.item[0][7],"most_VRF_node_public_and_private_key_total_rounds_delegate_name",63);
  memcpy(database_multiple_documents_fields.item[0][8],"most_VRF_node_public_and_private_key_total_rounds",49);
  memcpy(database_multiple_documents_fields.item[0][9],"most_VRF_node_random_data_total_rounds_delegate_name",52);
  memcpy(database_multiple_documents_fields.item[0][10],"most_VRF_node_random_data_total_rounds",38);
  memcpy(database_multiple_documents_fields.item[0][11],"total_XCASH_proof_of_stake_rounds",33);
  memcpy(database_multiple_documents_fields.item[0][12],"total_coins_in_proof_of_stake",29);
  memcpy(database_multiple_documents_fields.item[0][13],"total_circulating_supply_percentage_in_proof_of_stake",53);
  memcpy(database_multiple_documents_fields.item[1][0],"username",8);
  memcpy(database_multiple_documents_fields.item[1][1],"most_total_rounds_delegate_name",31);
  memcpy(database_multiple_documents_fields.item[1][2],"most_total_rounds",17);
  memcpy(database_multiple_documents_fields.item[1][3],"best_block_verifier_online_percentage_delegate_name",51);
  memcpy(database_multiple_documents_fields.item[1][4],"best_block_verifier_online_percentage",37);
  memcpy(database_multiple_documents_fields.item[1][5],"most_block_producer_total_rounds_delegate_name",46);
  memcpy(database_multiple_documents_fields.item[1][6],"most_block_producer_total_rounds",32);
  memcpy(database_multiple_documents_fields.item[1][7],"most_VRF_node_public_and_private_key_total_rounds_delegate_name",63);
  memcpy(database_multiple_documents_fields.item[1][8],"most_VRF_node_public_and_private_key_total_rounds",49);
  memcpy(database_multiple_documents_fields.item[1][9],"most_VRF_node_random_data_total_rounds_delegate_name",52);
  memcpy(database_multiple_documents_fields.item[1][10],"most_VRF_node_random_data_total_rounds",38);
  memcpy(database_multiple_documents_fields.item[1][11],"total_XCASH_proof_of_stake_rounds",33);
  memcpy(database_multiple_documents_fields.item[1][12],"total_coins_in_proof_of_stake",29);
  memcpy(database_multiple_documents_fields.item[1][13],"total_circulating_supply_percentage_in_proof_of_stake",53);

  memcpy(database_multiple_documents_fields.value[0][0],"XCASH",5);  
  memcpy(database_multiple_documents_fields.value[0][1],"DELEGATE_NAME",13);
  memcpy(database_multiple_documents_fields.value[0][2],"5",1);
  memcpy(database_multiple_documents_fields.value[0][3],"DELEGATE_NAME",13);
  memcpy(database_multiple_documents_fields.value[0][4],"10",2);
  memcpy(database_multiple_documents_fields.value[0][5],"DELEGATE_NAME",13);  
  memcpy(database_multiple_documents_fields.value[0][6],"15",2);
  memcpy(database_multiple_documents_fields.value[0][7],"DELEGATE_NAME",13);  
  memcpy(database_multiple_documents_fields.value[0][8],"5",1);
  memcpy(database_multiple_documents_fields.value[0][9],"DELEGATE_NAME",13);  
  memcpy(database_multiple_documents_fields.value[0][10],"10",2);  
  memcpy(database_multiple_documents_fields.value[0][11],"15",2);
  memcpy(database_multiple_documents_fields.value[0][12],"5",1);
  memcpy(database_multiple_documents_fields.value[0][13],"10",2);
  memcpy(database_multiple_documents_fields.value[1][0],"XCASH",5);  
  memcpy(database_multiple_documents_fields.value[1][1],"DELEGATE_NAME",13);
  memcpy(database_multiple_documents_fields.value[1][2],"5",1);
  memcpy(database_multiple_documents_fields.value[1][3],"DELEGATE_NAME",13);
  memcpy(database_multiple_documents_fields.value[1][4],"10",2);
  memcpy(database_multiple_documents_fields.value[1][5],"DELEGATE_NAME",13);  
  memcpy(database_multiple_documents_fields.value[1][6],"15",2);
  memcpy(database_multiple_documents_fields.value[1][7],"DELEGATE_NAME",13);  
  memcpy(database_multiple_documents_fields.value[1][8],"5",1);
  memcpy(database_multiple_documents_fields.value[1][9],"DELEGATE_NAME",13);  
  memcpy(database_multiple_documents_fields.value[1][10],"10",2);  
  memcpy(database_multiple_documents_fields.value[1][11],"15",2);
  memcpy(database_multiple_documents_fields.value[1][12],"5",1);
  memcpy(database_multiple_documents_fields.value[1][13],"10",2);

  database_multiple_documents_fields.document_count = 2;
  database_multiple_documents_fields.database_fields_count = DATABASE_DATA;

  if (create_json_data_from_database_multiple_documents_array(&database_multiple_documents_fields,result_test,"") == 1)
  {
    if (strncmp(result_test,DATA2,BUFFER_SIZE) == 0)  
    {
      color_print("PASSED! Test for create_json_data_from_database_multiple_documents_array","green");
      count_test++;
    }
    else
    { 
      color_print("FAILED! Test for create_json_data_from_database_multiple_documents_array","red");
    }
  }
  else
  {
    color_print("FAILED! Test for create_json_data_from_database_multiple_documents_array","red");
  }

  for (count = 0; count < 2; count++)
  {
    for (counter = 0; counter < DATABASE_DATA; counter++)
    {
      pointer_reset(database_multiple_documents_fields.item[count][counter]);
      pointer_reset(database_multiple_documents_fields.value[count][counter]);
    }
  }  

  // test for parse_reserve_bytes_data
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  if (parse_reserve_bytes_data(result_test,NETWORK_BLOCK,8) == 1 && memcmp(result_test,"d65cd4647f61953d6fa26fd80d1646ec96822abb5d3a87bcf84fc33daf32c0754932b5ec9abcbda86a053ce11508c08281a2fa386ed87edbd442d71934a6450d",VRF_BETA_LENGTH) == 0)
  {
    color_print("PASSED! Test for parse_reserve_bytes_data","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for parse_reserve_bytes_data","red");
  }

  // test for counting occurences of a substring in a string
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memcpy(result_test,TEST_OUTLINE,strnlen(TEST_OUTLINE,BUFFER_SIZE));
  if (string_count(result_test,"-") == strnlen(TEST_OUTLINE,BUFFER_SIZE) && string_count(result_test,"--") == strnlen(TEST_OUTLINE,BUFFER_SIZE)/2)  
  {
    color_print("PASSED! Test for string_count","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for string_count","red");
  }

  // test for string replace
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memcpy(result_test,"{\r\n \"message_settings\": \"string_replace_test\",\r\n}",49);
  if (string_replace(result_test,BUFFER_SIZE,"string_replace_test","string_replace") == 1 && strncmp(result_test,"{\r\n \"message_settings\": \"string_replace\",\r\n}",BUFFER_SIZE) == 0)
  {
    memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
    memcpy(result_test,"{\r\n \"message_settings\": \"string_replace_test\",\r\n}",49);
     if (string_replace(result_test,BUFFER_SIZE,"\"","\\\"") == 1 && strncmp(result_test,"{\r\n \\\"message_settings\\\": \\\"string_replace_test\\\",\r\n}",BUFFER_SIZE) == 0)
     {
       memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
       memcpy(result_test,"{\r\n \"message_settings\": \"string_replace_test\",\r\n}",49);
       if (string_replace(result_test,BUFFER_SIZE,"_test","") == 1 && strncmp(result_test,"{\r\n \"message_settings\": \"string_replace\",\r\n}",BUFFER_SIZE) == 0)
       {
         memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
         memcpy(result_test,"{\r\n \"message_settings\": \"string_replace\",\r\n}",44);
         if (string_replace(result_test,BUFFER_SIZE,"string_replace_test","") == 0 && strncmp(result_test,"{\r\n \"message_settings\": \"string_replace\",\r\n}",BUFFER_SIZE) == 0)
         {    
           color_print("PASSED! Test for string_replace","green");
           count_test++;
         }
         else
         {
           color_print("FAILED! Test for string_replace","red");
         }
       }
       else
       {
         color_print("FAILED! Test for string_replace","red");
       }
     }
     else
     {
       color_print("FAILED! Test for string_replace","red");
     }
  }
  else
  {
    color_print("FAILED! Test for string_replace","red");
  }
 
  // write the end test message
  if (count_test == STRING_FUNCTIONS_TOTAL_TEST)
  {
    fprintf(stderr,"\n\n");
    color_print(TEST_OUTLINE,"green");
    fprintf(stderr,"\033[1;32mstring functions test - Passed test: %d, Failed test: 0\033[0m\n",STRING_FUNCTIONS_TOTAL_TEST);
    color_print(TEST_OUTLINE,"green");
    fprintf(stderr,"\n\n");
  }
  else
  {
    fprintf(stderr,"\n\n");
    color_print(TEST_OUTLINE,"red");
    fprintf(stderr,"\033[1;31mstring functions test - Passed test: %d, Failed test: %d\033[0m\n",count_test,STRING_FUNCTIONS_TOTAL_TEST-count_test);
    color_print(TEST_OUTLINE,"red");
    fprintf(stderr,"\n\n");
  }
  return count_test;

  #undef STRING_FUNCTIONS_TOTAL_TEST
  #undef DATABASE_DATA
  #undef MESSAGE
  #undef DATA1
  #undef DATA2
}