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
  #define DATABASE_DATA 14
  #define MESSAGE "{\r\n \"message_settings_1\": \"XCASH_DPOPS_TEST_DATA_1\",\r\n \"message_settings_2\": \"XCASH_DPOPS_TEST_DATA_2\",\r\n}"
  #define CREATE_JSON_DATA_FROM_DATABASE_DOCUMENT_ARRAY_TEST_DATA "{\"username\":\"XCASH\",\"most_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_total_rounds\":\"5\",\"best_block_verifier_online_percentage_delegate_name\":\"DELEGATE_NAME\",\"best_block_verifier_online_percentage\":\"10\",\"most_block_producer_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_block_producer_total_rounds\":\"15\",\"most_VRF_node_public_and_private_key_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_VRF_node_public_and_private_key_total_rounds\":\"5\",\"most_VRF_node_random_data_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_VRF_node_random_data_total_rounds\":\"10\",\"total_XCASH_proof_of_stake_rounds\":\"15\",\"total_coins_in_proof_of_stake\":\"5\",\"total_circulating_supply_percentage_in_proof_of_stake\":\"10\"}"
  #define CREATE_JSON_DATA_FROM_DATABASE_MULTIPLE_DOCUMENTS_ARRAY_TEST_DATA "[{\"username\":\"XCASH\",\"most_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_total_rounds\":\"5\",\"best_block_verifier_online_percentage_delegate_name\":\"DELEGATE_NAME\",\"best_block_verifier_online_percentage\":\"10\",\"most_block_producer_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_block_producer_total_rounds\":\"15\",\"most_VRF_node_public_and_private_key_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_VRF_node_public_and_private_key_total_rounds\":\"5\",\"most_VRF_node_random_data_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_VRF_node_random_data_total_rounds\":\"10\",\"total_XCASH_proof_of_stake_rounds\":\"15\",\"total_coins_in_proof_of_stake\":\"5\",\"total_circulating_supply_percentage_in_proof_of_stake\":\"10\"},{\"username\":\"XCASH\",\"most_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_total_rounds\":\"5\",\"best_block_verifier_online_percentage_delegate_name\":\"DELEGATE_NAME\",\"best_block_verifier_online_percentage\":\"10\",\"most_block_producer_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_block_producer_total_rounds\":\"15\",\"most_VRF_node_public_and_private_key_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_VRF_node_public_and_private_key_total_rounds\":\"5\",\"most_VRF_node_random_data_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_VRF_node_random_data_total_rounds\":\"10\",\"total_XCASH_proof_of_stake_rounds\":\"15\",\"total_coins_in_proof_of_stake\":\"5\",\"total_circulating_supply_percentage_in_proof_of_stake\":\"10\"}]"
  #define CREATE_JSON_DATA_FROM_DELEGATES_ARRAY_TEST_DATA "[{\"public_address\":\"XCA1h3yJ318hJGTFeXfYyEcyE7G4hX7jrCbvz21VecJGhf64Tw51yWii2Q1e76fJbB26Ea8CRipmdW6ZHQcRrLKx3cxRkr5M12\",\"total_vote_count\":\"0\",\"IP_address\":\"delegates.xcash.foundation\",\"delegate_name\":\"delegates_xcash_foundation\",\"about\":\"Official X-Network node\",\"website\":\"delegates.xcash.foundation\",\"team\":\"X-Network Team\",\"shared_delegate_status\":\"false\",\"delegate_fee\":\"0\",\"server_specs\":\"Operating System = Ubuntu 18.04 CPU = 32 threads (Intel 2xE5 - 2660 - 2.2GHz RAM = 256GB DDR3 Hard drive = 2x240 GB SSD + 4x2TB SATA Bandwidth Transfer = Unlimited Bandwidth Speed = 500 Mbps upload and 500 Mbps download\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"0\"},{\"public_address\":\"XCA1kkdrRQ9SC7wfJ3F329giei31r13wKEKiD1ZMYf6nBiU8KE6a6LDJTZobfJ58o8A3vtGrAkzfHDd815kac7F17iPhK9zYGG\",\"total_vote_count\":\"0\",\"IP_address\":\"officialdelegate.xcash.foundation\",\"delegate_name\":\"officialdelegate_xcash_foundation\",\"about\":\"Official X-Network Shared Delegate\",\"website\":\"officialdelegate.xcash.foundation\",\"team\":\"X-Network Team\",\"shared_delegate_status\":\"true\",\"delegate_fee\":\"0\",\"server_specs\":\"Operating System = Ubuntu 18.04 CPU = 8 threads (Intel E3-1246 v3 - 3.50GHz RAM = 32GB DDR3 Hard drive = 2x HDD SATA 2 TB Bandwidth Transfer = Unlimited Bandwidth Speed = 1 Gbps upload and 1 Gbps download\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"0\"}]"
  #define CREATE_JSON_DATA_FROM_VOTES_ARRAY_TEST_DATA "[{\"public_address_created_reserve_proof\":\"XCA1va68ZVYVDRzX3mpqVD8iWbxviCsUigMkyogxAWSQR5rk16g7dYjbcKDwsAggp8Z1dNjPgcCzkcV7hDg99Wtk8nyUBBK4K7\",\"public_address_voted_for\":\"XCA1va68ZVYVDRzX3mpqVD8iWbxviCsUigMkyogxAWSQR5rk16g7dYjbcKDwsAggp8Z1dNjPgcCzkcV7hDg99Wtk8nyUBBK4K7\",\"total\":\"0\",\"reserve_proof\":\"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118PRh4AT7VvXjWBm8SAPTf55QJP1E3fkm8f3oe3VWeT5o8YybH9113USPdfBXLfpWTHYMCJAmGa2CcFiyHn5Gj9PCuHaKB3VHdqTEy6shyi4bW8FuTLyhTpDcnS7uEAyQcijSuHEUcgTwUSNSwuzvT113ka91zQXMcjYwZ39zAJVS16DuJZNRWnQaiNJVsracFDmUsXCCWcMx5HpKDNn5N3H1qSCxhV4CdUN2cB8Z2iirSgiL2frFA1DrVCKJm5kNHSANEGjHe4mw5L6L2Yeabna74FLszbBPKso42fpctQ8Djj25hqj6pEQqHY4tTaAYfkVRuB4m8DU4aNZN1Ak9vkBxvZtVDRzX3mpqVD8iWbxviCsUigMkyogxAWSQR5rjh4uUTRP7QHCpCm1o34Qc1vrDsLzXWbvcwC1g1VUh3S5dDKX2FXGJT4DX2REwtCNCZX2MZE2wGcbLRZ3vj4jQ6NzwTqT\"},{\"public_address_created_reserve_proof\":\"XCA1va68ZVYVDRzX3mpqVD8iWbxviCsUigMkyogxAWSQR5rk16g7dYjbcKDwsAggp8Z1dNjPgcCzkcV7hDg99Wtk8nyUBBK4K7\",\"public_address_voted_for\":\"XCA1va68ZVYVDRzX3mpqVD8iWbxviCsUigMkyogxAWSQR5rk16g7dYjbcKDwsAggp8Z1dNjPgcCzkcV7hDg99Wtk8nyUBBK4K7\",\"total\":\"0\",\"reserve_proof\":\"ReserveProofV11BZ23sBt9sZJeGccf84mzyAmNCP3KzYbE1111112VKmH111118PRh4AT7VvXjWBm8SAPTf55QJP1E3fkm8f3oe3VWeT5o8YybH9113USPdfBXLfpWTHYMCJAmGa2CcFiyHn5Gj9PCuHaKB3VHdqTEy6shyi4bW8FuTLyhTpDcnS7uEAyQcijSuHEUcgTwUSNSwuzvT113ka91zQXMcjYwZ39zAJVS16DuJZNRWnQaiNJVsracFDmUsXCCWcMx5HpKDNn5N3H1qSCxhV4CdUN2cB8Z2iirSgiL2frFA1DrVCKJm5kNHSANEGjHe4mw5L6L2Yeabna74FLszbBPKso42fpctQ8Djj25hqj6pEQqHY4tTaAYfkVRuB4m8DU4aNZN1Ak9vkBxvZtVDRzX3mpqVD8iWbxviCsUigMkyogxAWSQR5rjh4uUTRP7QHCpCm1o34Qc1vrDsLzXWbvcwC1g1VUh3S5dDKX2FXGJT4DX2REwtCNCZX2MZE2wGcbLRZ3vj4jQ6NzwTqT\"}]"
  
  // variables
  int count;
  int counter;
  struct database_document_fields database_data;
  struct database_multiple_documents_fields database_multiple_documents_fields;
  struct delegates delegates[MAXIMUM_AMOUNT_OF_DELEGATES];
  struct votes votes[MAXIMUM_AMOUNT_OF_DELEGATES];

  // reset the variables
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  count_test = 0;

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

  // initialize the delegates struct 
  INITIALIZE_DELEGATES_STRUCT_TEST_DATA;

  // initialize the votes struct 
  INITIALIZE_VOTES_STRUCT_TEST_DATA;
 
  // write the start test message
  fprintf(stderr,"\033[1;34m%s\nstring functions functions test - Total test: %d\n%s\n\n\033[0m",TEST_OUTLINE,STRING_FUNCTIONS_TOTAL_TEST,TEST_OUTLINE);
 
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
  memset(result_test,0,sizeof(result_test));
  if (random_string(result_test,RANDOM_STRING_LENGTH) == 1)
  {   
    color_print("PASSED! Test for random_string","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for random_string","red");
  }

  // test create_json_data_from_database_document_array
  memset(result_test,0,sizeof(result_test));
  if (create_json_data_from_database_document_array(&database_data,result_test,"") == 1 && strncmp(result_test,CREATE_JSON_DATA_FROM_DATABASE_DOCUMENT_ARRAY_TEST_DATA,BUFFER_SIZE) == 0)
  {
    color_print("PASSED! Test for create_json_data_from_database_document_array","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for create_json_data_from_database_document_array","red");
  }

  // test create_json_data_from_database_multiple_documents_array
  memset(result_test,0,sizeof(result_test));
  if (create_json_data_from_database_multiple_documents_array(&database_multiple_documents_fields,result_test,"") == 1 && strncmp(result_test,CREATE_JSON_DATA_FROM_DATABASE_MULTIPLE_DOCUMENTS_ARRAY_TEST_DATA,BUFFER_SIZE) == 0)
  {
    color_print("PASSED! Test for create_json_data_from_database_multiple_documents_array","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for create_json_data_from_database_multiple_documents_array","red");
  }

  // test create_json_data_from_delegates_array
  memset(result_test,0,sizeof(result_test));
  if (create_json_data_from_delegates_array(delegates,result_test,"") == 1 && strncmp(result_test,CREATE_JSON_DATA_FROM_DELEGATES_ARRAY_TEST_DATA,BUFFER_SIZE) == 0)
  {
    color_print("PASSED! Test for create_json_data_from_database_multiple_documents_array","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for create_json_data_from_database_multiple_documents_array","red");
  }

  // test create_json_data_from_votes_array
  memset(result_test,0,sizeof(result_test));
  if (create_json_data_from_votes_array(votes,result_test,"") == 1 && strncmp(result_test,CREATE_JSON_DATA_FROM_VOTES_ARRAY_TEST_DATA,BUFFER_SIZE) == 0)
  {
    color_print("PASSED! Test for create_json_data_from_database_multiple_documents_array","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for create_json_data_from_database_multiple_documents_array","red");
  }

  // test for parse_reserve_bytes_data
  memset(result_test,0,sizeof(result_test));
  if (parse_reserve_bytes_data(result_test,NETWORK_BLOCK,8) == 1 && strncmp(result_test,"d65cd4647f61953d6fa26fd80d1646ec96822abb5d3a87bcf84fc33daf32c0754932b5ec9abcbda86a053ce11508c08281a2fa386ed87edbd442d71934a6450d",VRF_BETA_LENGTH) == 0)
  {
    color_print("PASSED! Test for parse_reserve_bytes_data","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for parse_reserve_bytes_data","red");
  }

  // test for counting occurences of a substring in a string
  memset(result_test,0,sizeof(result_test));
  memcpy(result_test,TEST_OUTLINE,sizeof(TEST_OUTLINE)-1);
  if (string_count(result_test,"-") == strnlen(TEST_OUTLINE,BUFFER_SIZE) && string_count(result_test,"--") == strnlen(TEST_OUTLINE,BUFFER_SIZE)/2)  
  {
    color_print("PASSED! Test for string_count","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for string_count","red");
  }

  // test for parse_http_response
  memset(result_test,0,sizeof(result_test));
  memcpy(result_test,HTTP_RESPONSE_DATA,sizeof(HTTP_RESPONSE_DATA)-1);
  if (parse_http_response(result_test) == 1 && strncmp(result_test,"{\"public_address\":\"" TEST_WALLET_1 "\",\"current_delegate_rank\":\"2\",\"total_votes\":\"0\",\"online_percentage\":\"0\",\"total_blocks_found\":\"1\",\"total_xcash_from_blocks_found\":\"15\",\"total_payments\":\"1\",\"total_voters\":\"0\",\"fee\":\"0.000000\",\"minimum_amount\":\"0\"}",sizeof(result_test)) == 0)  
  {
    color_print("PASSED! Test for parse_http_response","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for parse_http_response","red");
  }

  // test for string replace
  memset(result_test,0,sizeof(result_test));
  memcpy(result_test,"{\r\n \"message_settings\": \"string_replace_test\",\r\n}",49);
  string_replace(result_test,sizeof(result_test),"string_replace_test","string_replace");
  if (strncmp(result_test,"{\r\n \"message_settings\": \"string_replace\",\r\n}",BUFFER_SIZE) == 0)
  {
    memset(result_test,0,sizeof(result_test));
    memcpy(result_test,"{\r\n \"message_settings\": \"string_replace_test\",\r\n}",49);
    string_replace(result_test,sizeof(result_test),"\"","\\\"");
     if (strncmp(result_test,"{\r\n \\\"message_settings\\\": \\\"string_replace_test\\\",\r\n}",BUFFER_SIZE) == 0)
     {
       memset(result_test,0,sizeof(result_test));
       memcpy(result_test,"{\r\n \"message_settings\": \"string_replace_test\",\r\n}",49);
       string_replace(result_test,sizeof(result_test),"_test","");
       if (strncmp(result_test,"{\r\n \"message_settings\": \"string_replace\",\r\n}",BUFFER_SIZE) == 0)
       {
         memset(result_test,0,sizeof(result_test));
         memcpy(result_test,"{\r\n \"message_settings\": \"string_replace\",\r\n}",44);
         string_replace(result_test,sizeof(result_test),"string_replace_test","");
         if (strncmp(result_test,"{\r\n \"message_settings\": \"string_replace\",\r\n}",BUFFER_SIZE) == 0)
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

  // reset the database_data struct
  for (count = 0; count < DATABASE_DATA; count++)
  {
    pointer_reset(database_data.item[count]);
    pointer_reset(database_data.value[count]);
  }

  // reset the database_multiple_documents_fields struct
  for (count = 0; count < 2; count++)
  {
    for (counter = 0; counter < DATABASE_DATA; counter++)
    {
      pointer_reset(database_multiple_documents_fields.item[count][counter]);
      pointer_reset(database_multiple_documents_fields.value[count][counter]);
    }
  }

  // reset the delegates struct
  for (count = 0; (int)count < MAXIMUM_AMOUNT_OF_DELEGATES; count++)
  {
    pointer_reset(delegates[count].public_address);
    pointer_reset(delegates[count].total_vote_count);
    pointer_reset(delegates[count].IP_address);
    pointer_reset(delegates[count].delegate_name);
    pointer_reset(delegates[count].about);
    pointer_reset(delegates[count].website);
    pointer_reset(delegates[count].team);
    pointer_reset(delegates[count].shared_delegate_status);
    pointer_reset(delegates[count].delegate_fee);
    pointer_reset(delegates[count].server_specs);
    pointer_reset(delegates[count].block_verifier_score);
    pointer_reset(delegates[count].online_status);
    pointer_reset(delegates[count].block_verifier_total_rounds);
    pointer_reset(delegates[count].block_verifier_online_total_rounds);
    pointer_reset(delegates[count].block_verifier_online_percentage);
    pointer_reset(delegates[count].block_producer_total_rounds);
    pointer_reset(delegates[count].block_producer_block_heights);
    pointer_reset(delegates[count].public_key);
  }

  // reset the votes struct
  for (count = 0; count < MAXIMUM_AMOUNT_OF_DELEGATES; count++)
  {
    pointer_reset(votes[count].public_address_created_reserve_proof);
    pointer_reset(votes[count].public_address_voted_for);
    pointer_reset(votes[count].total);
    pointer_reset(votes[count].reserve_proof);
  }
 
  // write the end test message
  fprintf(stderr,"\033[1;33m\n\n%s\nstring functions functions test - Passed test: %d, Failed test: %d\n%s\n\n\n\033[0m",TEST_OUTLINE,count_test,STRING_FUNCTIONS_TOTAL_TEST-count_test,TEST_OUTLINE);
  return count_test;
  
  #undef DATABASE_DATA
  #undef MESSAGE
  #undef DATA1
  #undef DATA2
}