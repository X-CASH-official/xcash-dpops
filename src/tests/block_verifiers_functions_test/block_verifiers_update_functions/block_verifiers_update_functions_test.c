#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "insert_database_functions.h"
#include "read_database_functions.h"
#include "delete_database_functions.h"
#include "count_database_functions.h"
#include "block_verifiers_update_functions.h"
#include "define_macros_test.h"

#include "variables_test.h"
#include "block_verifiers_update_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: block_verifiers_update_functions_test
Description: Test the block verifiers update functions
Return: The number of passed block_verifiers_update_functions test
-----------------------------------------------------------------------------------------------------------
*/

int block_verifiers_update_functions_test(void)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char message[BUFFER_SIZE];
  char message2[BUFFER_SIZE];
  int count;

  // reset the variables
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  count_test = 0;
  network_functions_test_error_settings = 2;
  delete_database(database_name);
  RESET_ERROR_MESSAGES;
 
  // write the start test message
  fprintf(stderr,"\033[1;34m%s\nblock verifiers update functions test - Total test: %d\n%s\n\n\033[0m",TEST_OUTLINE,BLOCK_VERIFIERS_UPDATE_FUNCTIONS_TOTAL_TEST,TEST_OUTLINE);
 
  // run the test
  // update_block_verifiers_list
  for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++)
  {
    memset(previous_block_verifiers_list.block_verifiers_name[count],0,sizeof(previous_block_verifiers_list.block_verifiers_name[count]));
    memset(previous_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_public_address[count]));
    memset(previous_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(previous_block_verifiers_list.block_verifiers_public_key[count]));
    memset(previous_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_IP_address[count]));
    memset(current_block_verifiers_list.block_verifiers_name[count],0,sizeof(current_block_verifiers_list.block_verifiers_name[count]));
    memset(current_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_public_address[count]));
    memset(current_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(current_block_verifiers_list.block_verifiers_public_key[count]));
    memset(current_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_IP_address[count]));
    memset(next_block_verifiers_list.block_verifiers_name[count],0,sizeof(next_block_verifiers_list.block_verifiers_name[count]));
    memset(next_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_public_address[count]));
    memset(next_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(next_block_verifiers_list.block_verifiers_public_key[count]));
    memset(next_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_IP_address[count]));
  }
  memcpy(previous_block_verifiers_list.block_verifiers_name[0],"delegate_1",10);
  memcpy(previous_block_verifiers_list.block_verifiers_public_address[0],TEST_WALLET,XCASH_WALLET_LENGTH);
  memcpy(previous_block_verifiers_list.block_verifiers_IP_address[0],XCASH_DPOPS_delegates_IP_address,9);
  memcpy(previous_block_verifiers_list.block_verifiers_public_key[0],NEXT_BLOCK_VERIFIERS_PUBLIC_KEY,VRF_PUBLIC_KEY_LENGTH);
  memcpy(current_block_verifiers_list.block_verifiers_name[0],"delegate_2",10);
  memcpy(current_block_verifiers_list.block_verifiers_public_address[0],TEST_WALLET,XCASH_WALLET_LENGTH);
  memcpy(current_block_verifiers_list.block_verifiers_IP_address[0],XCASH_DPOPS_delegates_IP_address,9);
  memcpy(current_block_verifiers_list.block_verifiers_public_key[0],NEXT_BLOCK_VERIFIERS_PUBLIC_KEY,VRF_PUBLIC_KEY_LENGTH);
  memcpy(next_block_verifiers_list.block_verifiers_name[0],"delegate_3",10);
  memcpy(next_block_verifiers_list.block_verifiers_public_address[0],TEST_WALLET,XCASH_WALLET_LENGTH);
  memcpy(next_block_verifiers_list.block_verifiers_IP_address[0],XCASH_DPOPS_delegates_IP_address,9);
  memcpy(next_block_verifiers_list.block_verifiers_public_key[0],NEXT_BLOCK_VERIFIERS_PUBLIC_KEY,VRF_PUBLIC_KEY_LENGTH);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_1);
  update_block_verifiers_list();
  if (strncmp(previous_block_verifiers_list.block_verifiers_name[0],"delegate_2",BUFFER_SIZE) == 0 && strncmp(current_block_verifiers_list.block_verifiers_name[0],"delegate_3",BUFFER_SIZE) == 0 && strncmp(next_block_verifiers_list.block_verifiers_public_address[0],TEST_WALLET_1,BUFFER_SIZE) == 0)
  {
    color_print("PASSED! Test for update_block_verifiers_list","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for update_block_verifiers_list","red");
  }

  // add_block_verifiers_round_statistics
  for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++)
  {
    memset(previous_block_verifiers_list.block_verifiers_name[count],0,sizeof(previous_block_verifiers_list.block_verifiers_name[count]));
    memset(previous_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_public_address[count]));
    memset(previous_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(previous_block_verifiers_list.block_verifiers_public_key[count]));
    memset(previous_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_IP_address[count]));
    memset(current_block_verifiers_list.block_verifiers_name[count],0,sizeof(current_block_verifiers_list.block_verifiers_name[count]));
    memset(current_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_public_address[count]));
    memset(current_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(current_block_verifiers_list.block_verifiers_public_key[count]));
    memset(current_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_IP_address[count]));
    memset(next_block_verifiers_list.block_verifiers_name[count],0,sizeof(next_block_verifiers_list.block_verifiers_name[count]));
    memset(next_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_public_address[count]));
    memset(next_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(next_block_verifiers_list.block_verifiers_public_key[count]));
    memset(next_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_IP_address[count]));
  }
  memcpy(previous_block_verifiers_list.block_verifiers_name[0],"delegate_1",10);
  memcpy(previous_block_verifiers_list.block_verifiers_public_address[0],TEST_WALLET_1,XCASH_WALLET_LENGTH);
  memcpy(previous_block_verifiers_list.block_verifiers_IP_address[0],XCASH_DPOPS_delegates_IP_address,9);
  memcpy(previous_block_verifiers_list.block_verifiers_public_key[0],NEXT_BLOCK_VERIFIERS_PUBLIC_KEY,VRF_PUBLIC_KEY_LENGTH);
  memset(data_test,0,sizeof(data_test));
  memset(result_test,0,sizeof(result_test));
  memset(data,0,sizeof(data));
  if (add_block_verifiers_round_statistics(XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT_TEST) == 1 && read_document_field_from_collection(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_1 "\"}","block_verifier_total_rounds",data_test) == 1 && strncmp(data_test,"1",sizeof(data_test)) == 0 && read_document_field_from_collection(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_1 "\"}","block_verifier_online_total_rounds",result_test) == 1 && strncmp(result_test,"1",sizeof(result_test)) == 0 && read_document_field_from_collection(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_1 "\"}","block_verifier_online_percentage",data) == 1 && strncmp(data,"100",sizeof(data)) == 0)
  {
    color_print("PASSED! Test for add_block_verifiers_round_statistics for every block verifier","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for add_block_verifiers_round_statistics for every block verifier","red");
  }

  delete_database(database_name);
  insert_document_into_collection_json(database_name,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_1);
  RESET_ERROR_MESSAGES;
  memset(data_test,0,sizeof(data_test));
  memset(result_test,0,sizeof(result_test));
  memset(current_round_part_backup_node,0,sizeof(current_round_part_backup_node));
  memcpy(current_round_part_backup_node,"0",1);
  memset(main_nodes_list.block_producer_public_address,0,sizeof(main_nodes_list.block_producer_public_address));
  memcpy(main_nodes_list.block_producer_public_address,TEST_WALLET_1,XCASH_WALLET_LENGTH);
  if (add_block_verifiers_round_statistics(XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT_TEST) == 1 && read_document_field_from_collection(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_1 "\"}","block_producer_total_rounds",data_test) == 1 && strncmp(data_test,"1",sizeof(data_test)) == 0 && read_document_field_from_collection(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_1 "\"}","block_producer_block_heights",result_test) == 1 && strncmp(result_test,"|" XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT_TEST "|" XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT_TEST,sizeof(result_test)) == 0)
  {
    color_print("PASSED! Test for add_block_verifiers_round_statistics for the block producer","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for add_block_verifiers_round_statistics for the block producer","red");
  }

  // add_round_statistics
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_1 "\",\"total_vote_count\":\"120000000\",\"IP_address\":\"192.168.1.201\",\"delegate_name\":\"delegate_name_1\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"server_settings\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"1\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}");
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_20 "\",\"total_vote_count\":\"120000000\",\"IP_address\":\"192.168.1.201\",\"delegate_name\":\"delegate_name_2\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"server_settings\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"2\",\"block_verifier_online_total_rounds\":\"2\",\"block_verifier_online_percentage\":\"100\",\"block_producer_total_rounds\":\"1\",\"block_producer_block_heights\":\"\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}");
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_19 "\",\"total_vote_count\":\"120000000\",\"IP_address\":\"192.168.1.201\",\"delegate_name\":\"delegate_name_3\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"server_settings\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"3\",\"block_verifier_online_total_rounds\":\"1\",\"block_verifier_online_percentage\":\"33\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}");
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_18 "\",\"total_vote_count\":\"120000000\",\"IP_address\":\"192.168.1.201\",\"delegate_name\":\"delegate_name_4\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"server_settings\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"4\",\"block_verifier_online_total_rounds\":\"1\",\"block_verifier_online_percentage\":\"25\",\"block_producer_total_rounds\":\"2\",\"block_producer_block_heights\":\"\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}");
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_15 "\",\"total_vote_count\":\"120000000\",\"IP_address\":\"192.168.1.201\",\"delegate_name\":\"delegate_name_5\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"server_settings\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"5\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}");
  insert_document_into_collection_json(database_name,"statistics",DATABASE_COLLECTION_STATISTICS_DATA);
  RESET_ERROR_MESSAGES;
  memset(data_test,0,sizeof(data_test));
  memset(result_test,0,sizeof(result_test));
  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));
  memset(message2,0,sizeof(message2));
  if (add_round_statistics() == 1 && read_document_field_from_collection(database_name,"statistics","{\"username\":\"XCASH\"}","most_total_rounds_delegate_name",data_test) == 1 && strncmp(data_test,"delegate_name_5",sizeof(data_test)) == 0 && read_document_field_from_collection(database_name,"statistics","{\"username\":\"XCASH\"}","most_total_rounds",result_test) == 1 && strncmp(result_test,"5",sizeof(result_test)) == 0 && read_document_field_from_collection(database_name,"statistics","{\"username\":\"XCASH\"}","best_block_verifier_online_percentage_delegate_name",data) == 1 && strncmp(data,"delegate_name_2",sizeof(data)) == 0 && read_document_field_from_collection(database_name,"statistics","{\"username\":\"XCASH\"}","best_block_verifier_online_percentage",data2) == 1 && strncmp(data2,"100",sizeof(data2)) == 0 && read_document_field_from_collection(database_name,"statistics","{\"username\":\"XCASH\"}","most_block_producer_total_rounds_delegate_name",message) == 1 && strncmp(message,"delegate_name_4",sizeof(message)) == 0 && read_document_field_from_collection(database_name,"statistics","{\"username\":\"XCASH\"}","most_block_producer_total_rounds",message2) == 1 && strncmp(message2,"2",sizeof(message2)) == 0)
  {
    color_print("PASSED! Test for add_round_statistics","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for add_round_statistics","red");
  }

  // calculate_main_nodes_roles
  for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++)
  {
    memset(previous_block_verifiers_list.block_verifiers_name[count],0,sizeof(previous_block_verifiers_list.block_verifiers_name[count]));
    memset(previous_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_public_address[count]));
    memset(previous_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(previous_block_verifiers_list.block_verifiers_public_key[count]));
    memset(previous_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_IP_address[count]));
    memset(current_block_verifiers_list.block_verifiers_name[count],0,sizeof(current_block_verifiers_list.block_verifiers_name[count]));
    memset(current_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_public_address[count]));
    memset(current_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(current_block_verifiers_list.block_verifiers_public_key[count]));
    memset(current_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_IP_address[count]));
    memset(next_block_verifiers_list.block_verifiers_name[count],0,sizeof(next_block_verifiers_list.block_verifiers_name[count]));
    memset(next_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_public_address[count]));
    memset(next_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(next_block_verifiers_list.block_verifiers_public_key[count]));
    memset(next_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_IP_address[count]));
  }
  for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++)
  {
    memset(data,0,strlen(data));
    memcpy(data,"public_address_",15);  
    snprintf(data+15,BUFFER_SIZE-16,"%d",count);
    memcpy(current_block_verifiers_list.block_verifiers_name[count],"delegate_name_1",15);
    memcpy(current_block_verifiers_list.block_verifiers_public_address[count],data,strnlen(data,sizeof(current_block_verifiers_list.block_verifiers_name[count])));
    memcpy(current_block_verifiers_list.block_verifiers_IP_address[count],XCASH_DPOPS_delegates_IP_address,9);
    memcpy(current_block_verifiers_list.block_verifiers_public_key[count],NEXT_BLOCK_VERIFIERS_PUBLIC_KEY,VRF_PUBLIC_KEY_LENGTH);
  }
  if (calculate_main_nodes_roles() == 1 && strncmp(main_nodes_list.block_producer_public_address,"public_address_92",sizeof(main_nodes_list.block_producer_public_address)) == 0 && strncmp(main_nodes_list.block_producer_backup_block_verifier_1_public_address,"public_address_0",sizeof(main_nodes_list.block_producer_backup_block_verifier_1_public_address)) == 0 && strncmp(main_nodes_list.block_producer_backup_block_verifier_2_public_address,"public_address_27",sizeof(main_nodes_list.block_producer_backup_block_verifier_2_public_address)) == 0 && strncmp(main_nodes_list.block_producer_backup_block_verifier_3_public_address,"public_address_97",sizeof(main_nodes_list.block_producer_backup_block_verifier_3_public_address)) == 0 && strncmp(main_nodes_list.block_producer_backup_block_verifier_4_public_address,"public_address_49",sizeof(main_nodes_list.block_producer_backup_block_verifier_4_public_address)) == 0 && strncmp(main_nodes_list.block_producer_backup_block_verifier_5_public_address,"public_address_61",sizeof(main_nodes_list.block_producer_backup_block_verifier_5_public_address)) == 0)
  {
    color_print("PASSED! Test for calculate_main_nodes_roles","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for calculate_main_nodes_roles","red");
  }

  // get_delegates_online_status
  delete_database(database_name);
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_1 "\",\"total_vote_count\":\"120000000\",\"IP_address\":\"127.0.0.1\",\"delegate_name\":\"delegate_name_1\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"server_settings\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}");
  insert_document_into_collection_json(database_name,"delegates","{\"public_address\":\"" TEST_WALLET_1 "\",\"total_vote_count\":\"120000000\",\"IP_address\":\"\",\"delegate_name\":\"delegate_name_1\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"server_settings\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}");
  RESET_ERROR_MESSAGES;
  if (get_delegates_online_status() == 1)
  {
    color_print("PASSED! Test for get_delegates_online_status","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for get_delegates_online_status","red");
  }  

  // write the end test message
  fprintf(stderr,"\033[1;33m\n\n%s\nblock verifiers update functions test - Passed test: %d, Failed test: %d\n%s\n\n\n\033[0m",TEST_OUTLINE,count_test,BLOCK_VERIFIERS_UPDATE_FUNCTIONS_TOTAL_TEST-count_test,TEST_OUTLINE);
  return count_test;
}