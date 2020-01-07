#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "define_macros_test.h"

#include "delete_database_functions.h"
#include "insert_database_functions.h"
#include "organize_functions.h"

#include "variables_test.h"
#include "organize_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: organize_functions_test
Description: Test the organize functions
Return: The number of passed organize_functions test
-----------------------------------------------------------------------------------------------------------
*/

int organize_functions_test(void)
{
  // define macros
  #define ORGANIZE_FUNCTIONS_TOTAL_TEST 3
  #define ORGANIZE_DELEGATES_TEST_DATA_1 "{\"public_address\":\"XCA1h3yJ318hJGTFeXfYyEcyE7G4hX7jrCbvz21VecJGhf64Tw51yWii2Q1e76fJbB26Ea8CRipmdW6ZHQcRrLKx3cxRkr5M12\",\"total_vote_count\":\"15\",\"IP_address\":\"delegates.xcash.foundation\",\"delegate_name\":\"delegates_xcash_foundation\",\"about\":\"Official X-Network node\",\"website\":\"delegates.xcash.foundation\",\"team\":\"X-Network Team\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"server_settings\":\"Operating System = Ubuntu 18.04 CPU = 32 threads (Intel 2xE5 - 2660 - 2.2GHz RAM = 256GB DDR3 Hard drive = 2x240 GB SSD + 4x2TB SATA Bandwidth Transfer = Unlimited Bandwidth Speed = 500 Mbps upload and 500 Mbps download\",\"block_verifier_score\":\"0\",\"online_status\":\"false\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}"
  #define ORGANIZE_DELEGATES_TEST_DATA_2 "{\"public_address\":\"XCA1h3yJ318hJGTFeXfYyEcyE7G4hX7jrCbvz21VecJGhf64Tw51yWii2Q1e76fJbB26Ea8CRipmdW6ZHQcRrLKx3cxRkr5M12\",\"total_vote_count\":\"5\",\"IP_address\":\"delegates.xcash.foundation\",\"delegate_name\":\"delegates_xcash_foundation\",\"about\":\"Official X-Network node\",\"website\":\"delegates.xcash.foundation\",\"team\":\"X-Network Team\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"server_settings\":\"Operating System = Ubuntu 18.04 CPU = 32 threads (Intel 2xE5 - 2660 - 2.2GHz RAM = 256GB DDR3 Hard drive = 2x240 GB SSD + 4x2TB SATA Bandwidth Transfer = Unlimited Bandwidth Speed = 500 Mbps upload and 500 Mbps download\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}"
  #define ORGANIZE_DELEGATES_TEST_DATA_3 "{\"public_address\":\"XCA1h3yJ318hJGTFeXfYyEcyE7G4hX7jrCbvz21VecJGhf64Tw51yWii2Q1e76fJbB26Ea8CRipmdW6ZHQcRrLKx3cxRkr5M12\",\"total_vote_count\":\"10\",\"IP_address\":\"delegates.xcash.foundation\",\"delegate_name\":\"delegates_xcash_foundation\",\"about\":\"Official X-Network node\",\"website\":\"delegates.xcash.foundation\",\"team\":\"X-Network Team\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"server_settings\":\"Operating System = Ubuntu 18.04 CPU = 32 threads (Intel 2xE5 - 2660 - 2.2GHz RAM = 256GB DDR3 Hard drive = 2x240 GB SSD + 4x2TB SATA Bandwidth Transfer = Unlimited Bandwidth Speed = 500 Mbps upload and 500 Mbps download\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\",\"public_key\":\"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a\"}"

  // variables
  char reserve_proofs[2][BUFFER_SIZE_RESERVE_PROOF] = {"RESERVE_PROOF_TEST_DATA_2","RESERVE_PROOF_TEST_DATA"};
  struct delegates delegates[MAXIMUM_AMOUNT_OF_DELEGATES];
  int count;

  // reset the variables
  memset(result_test,0,sizeof(result_test));
  memset(data_test,0,sizeof(data_test));
  count_test = 0;

  // initialize the delegates struct
  for (count = 0; (int)count < MAXIMUM_AMOUNT_OF_DELEGATES; count++)
  {
    delegates[count].public_address = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    delegates[count].total_vote_count = (char*)calloc(100,sizeof(char));
    delegates[count].IP_address = (char*)calloc(256,sizeof(char));
    delegates[count].delegate_name = (char*)calloc(MAXIMUM_BUFFER_SIZE_DELEGATES_NAME+1,sizeof(char));
    delegates[count].about = (char*)calloc(1025,sizeof(char));
    delegates[count].website = (char*)calloc(256,sizeof(char));
    delegates[count].team = (char*)calloc(256,sizeof(char));
    delegates[count].pool_mode = (char*)calloc(10,sizeof(char));
    delegates[count].fee_structure = (char*)calloc(11,sizeof(char));
    delegates[count].server_settings = (char*)calloc(1025,sizeof(char));
    delegates[count].block_verifier_score = (char*)calloc(10,sizeof(char));
    delegates[count].online_status = (char*)calloc(10,sizeof(char));
    delegates[count].block_verifier_total_rounds = (char*)calloc(10,sizeof(char));
    delegates[count].block_verifier_online_total_rounds = (char*)calloc(10,sizeof(char));
    delegates[count].block_verifier_online_percentage = (char*)calloc(10,sizeof(char));
    delegates[count].block_producer_total_rounds = (char*)calloc(10,sizeof(char));
    delegates[count].block_producer_block_heights = (char*)calloc(50000,sizeof(char));
    delegates[count].public_key = (char*)calloc(VRF_PUBLIC_KEY_LENGTH+1,sizeof(char));
    if (delegates[count].public_address == NULL || delegates[count].total_vote_count == NULL || delegates[count].IP_address == NULL || delegates[count].delegate_name == NULL || delegates[count].about == NULL || delegates[count].website == NULL || delegates[count].team == NULL || delegates[count].pool_mode == NULL || delegates[count].fee_structure == NULL || delegates[count].server_settings == NULL || delegates[count].block_verifier_score == NULL || delegates[count].online_status == NULL || delegates[count].block_verifier_total_rounds == NULL || delegates[count].block_verifier_online_total_rounds == NULL || delegates[count].block_verifier_online_percentage == NULL || delegates[count].block_producer_total_rounds == NULL || delegates[count].block_producer_block_heights == NULL || delegates[count].public_key == NULL)
    {
      color_print("Could not allocate the memory needed on the heap","red");
      exit(0);
    }
  }

  memcpy(delegates[0].public_address,"XCA1h3yJ318hJGTFeXfYyEcyE7G4hX7jrCbvz21VecJGhf64Tw51yWii2Q1e76fJbB26Ea8CRipmdW6ZHQcRrLKx3cxRkr5M12",XCASH_WALLET_LENGTH);
  memcpy(delegates[0].total_vote_count,"15",2);
  memcpy(delegates[0].IP_address,"delegates.xcash.foundation",26);
  memcpy(delegates[0].delegate_name,"delegates_xcash_foundation",26);
  memcpy(delegates[0].about,"Official X-Network node",23);
  memcpy(delegates[0].website,"delegates.xcash.foundation",26);
  memcpy(delegates[0].team,"X-Network Team",14);
  memcpy(delegates[0].pool_mode,"false",5);
  memcpy(delegates[0].fee_structure,"0",1);
  memcpy(delegates[0].server_settings,"Operating System = Ubuntu 18.04 CPU = 32 threads (Intel 2xE5 - 2660 - 2.2GHz RAM = 256GB DDR3 Hard drive = 2x240 GB SSD + 4x2TB SATA Bandwidth Transfer = Unlimited Bandwidth Speed = 500 Mbps upload and 500 Mbps download",219);
  memcpy(delegates[0].block_verifier_score,"0",1);
  memcpy(delegates[0].online_status,"false",5);
  memcpy(delegates[0].block_verifier_total_rounds,"0",1);
  memcpy(delegates[0].block_verifier_online_total_rounds,"0",1);
  memcpy(delegates[0].block_verifier_online_percentage,"0",1);
  memcpy(delegates[0].block_producer_total_rounds,"0",1);
  memcpy(delegates[0].block_producer_block_heights,"0",1);
  memcpy(delegates[0].public_key,"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a",VRF_PUBLIC_KEY_LENGTH);

  memcpy(delegates[1].public_address,"XCA1h3yJ318hJGTFeXfYyEcyE7G4hX7jrCbvz21VecJGhf64Tw51yWii2Q1e76fJbB26Ea8CRipmdW6ZHQcRrLKx3cxRkr5M12",XCASH_WALLET_LENGTH);
  memcpy(delegates[1].total_vote_count,"5",1);
  memcpy(delegates[1].IP_address,"delegates.xcash.foundation",26);
  memcpy(delegates[1].delegate_name,"delegates_xcash_foundation",26);
  memcpy(delegates[1].about,"Official X-Network node",23);
  memcpy(delegates[1].website,"delegates.xcash.foundation",26);
  memcpy(delegates[1].team,"X-Network Team",14);
  memcpy(delegates[1].pool_mode,"false",5);
  memcpy(delegates[1].fee_structure,"0",1);
  memcpy(delegates[1].server_settings,"Operating System = Ubuntu 18.04 CPU = 32 threads (Intel 2xE5 - 2660 - 2.2GHz RAM = 256GB DDR3 Hard drive = 2x240 GB SSD + 4x2TB SATA Bandwidth Transfer = Unlimited Bandwidth Speed = 500 Mbps upload and 500 Mbps download",219);
  memcpy(delegates[1].block_verifier_score,"0",1);
  memcpy(delegates[1].online_status,"true",4);
  memcpy(delegates[1].block_verifier_total_rounds,"0",1);
  memcpy(delegates[1].block_verifier_online_total_rounds,"0",1);
  memcpy(delegates[1].block_verifier_online_percentage,"0",1);
  memcpy(delegates[1].block_producer_total_rounds,"0",1);
  memcpy(delegates[1].block_producer_block_heights,"0",1);
  memcpy(delegates[1].public_key,"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a",VRF_PUBLIC_KEY_LENGTH);

  memcpy(delegates[2].public_address,"XCA1h3yJ318hJGTFeXfYyEcyE7G4hX7jrCbvz21VecJGhf64Tw51yWii2Q1e76fJbB26Ea8CRipmdW6ZHQcRrLKx3cxRkr5M12",XCASH_WALLET_LENGTH);
  memcpy(delegates[2].total_vote_count,"10",2);
  memcpy(delegates[2].IP_address,"delegates.xcash.foundation",26);
  memcpy(delegates[2].delegate_name,"delegates_xcash_foundation",26);
  memcpy(delegates[2].about,"Official X-Network node",23);
  memcpy(delegates[2].website,"delegates.xcash.foundation",26);
  memcpy(delegates[2].team,"X-Network Team",14);
  memcpy(delegates[2].pool_mode,"false",5);
  memcpy(delegates[2].fee_structure,"0",1);
  memcpy(delegates[2].server_settings,"Operating System = Ubuntu 18.04 CPU = 32 threads (Intel 2xE5 - 2660 - 2.2GHz RAM = 256GB DDR3 Hard drive = 2x240 GB SSD + 4x2TB SATA Bandwidth Transfer = Unlimited Bandwidth Speed = 500 Mbps upload and 500 Mbps download",219);
  memcpy(delegates[2].block_verifier_score,"0",1);
  memcpy(delegates[2].online_status,"true",4);
  memcpy(delegates[2].block_verifier_total_rounds,"0",1);
  memcpy(delegates[2].block_verifier_online_total_rounds,"0",1);
  memcpy(delegates[2].block_verifier_online_percentage,"0",1);
  memcpy(delegates[2].block_producer_total_rounds,"0",1);
  memcpy(delegates[2].block_producer_block_heights,"0",1);
  memcpy(delegates[2].public_key,"ad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a",VRF_PUBLIC_KEY_LENGTH);
 
  // write the start test message
  color_print(TEST_OUTLINE,"blue");
  fprintf(stderr,"\033[1;34morganize functions test - Total test: %d\033[0m\n",ORGANIZE_FUNCTIONS_TOTAL_TEST);
  color_print(TEST_OUTLINE,"blue");
  fprintf(stderr,"\n");
 
  // run the test
 
  // test for organize_invalid_reserve_proofs_settings
  qsort((char*)reserve_proofs,sizeof(reserve_proofs)/sizeof(reserve_proofs[0]),sizeof(reserve_proofs[0]),organize_invalid_reserve_proofs_settings);
  if (strncmp(reserve_proofs[0],"RESERVE_PROOF_TEST_DATA",sizeof(reserve_proofs[0])) == 0 && strncmp(reserve_proofs[1],"RESERVE_PROOF_TEST_DATA_2",sizeof(reserve_proofs[1])) == 0)
  {   
    color_print("PASSED! Test for organize_invalid_reserve_proofs_settings","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for organize_invalid_reserve_proofs_settings","red");
  }

  // test for organize_delegates_settings
  qsort(delegates,MAXIMUM_AMOUNT_OF_DELEGATES,sizeof(struct delegates),organize_delegates_settings);
  if (memcmp(delegates[0].total_vote_count,"10",2) == 0 && memcmp(delegates[1].total_vote_count,"5",1) == 0 && memcmp(delegates[2].total_vote_count,"15",2) == 0)
  {   
    color_print("PASSED! Test for organize_delegates_settings","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for organize_delegates_settings","red");
  }

  // reset the delegates struct
  for (count = 0; (int)count < MAXIMUM_AMOUNT_OF_DELEGATES; count++)
  {
    memset(delegates[count].public_address,0,strlen(delegates[count].public_address));
    memset(delegates[count].total_vote_count,0,strlen(delegates[count].total_vote_count));
    memset(delegates[count].IP_address,0,strlen(delegates[count].IP_address));
    memset(delegates[count].delegate_name,0,strlen(delegates[count].delegate_name));
    memset(delegates[count].about,0,strlen(delegates[count].about));
    memset(delegates[count].website,0,strlen(delegates[count].website));
    memset(delegates[count].team,0,strlen(delegates[count].team));
    memset(delegates[count].pool_mode,0,strlen(delegates[count].pool_mode));
    memset(delegates[count].fee_structure,0,strlen(delegates[count].fee_structure));
    memset(delegates[count].server_settings,0,strlen(delegates[count].server_settings));
    memset(delegates[count].block_verifier_score,0,strlen(delegates[count].block_verifier_score));
    memset(delegates[count].online_status,0,strlen(delegates[count].online_status));
    memset(delegates[count].block_verifier_total_rounds,0,strlen(delegates[count].block_verifier_total_rounds));
    memset(delegates[count].block_verifier_online_total_rounds,0,strlen(delegates[count].block_verifier_online_total_rounds));
    memset(delegates[count].block_verifier_online_percentage,0,strlen(delegates[count].block_verifier_online_percentage));
    memset(delegates[count].block_producer_total_rounds,0,strlen(delegates[count].block_producer_total_rounds));
    memset(delegates[count].block_producer_block_heights,0,strlen(delegates[count].block_producer_block_heights));
    memset(delegates[count].public_key,0,strlen(delegates[count].public_key));
  }

  // insert the delegates into the database
  if (insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,ORGANIZE_DELEGATES_TEST_DATA_1,0) == 1 && insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,ORGANIZE_DELEGATES_TEST_DATA_2,0) == 1 && insert_document_into_collection_json(database_name,DATABASE_COLLECTION_TEST,ORGANIZE_DELEGATES_TEST_DATA_3,0) == 1)
  {
    // test for organize_delegates_settings
    if (organize_delegates(delegates,DATABASE_COLLECTION_TEST) != 0 && memcmp(delegates[0].total_vote_count,"10",2) == 0 && memcmp(delegates[1].total_vote_count,"5",1) == 0 && memcmp(delegates[2].total_vote_count,"15",2) == 0)
    {   
      color_print("PASSED! Test for organize_delegates","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for organize_delegates","red");
    }
  }
  else
  {
    color_print("FAILED! Test for organize_delegates","red");
  }
  delete_collection_from_database(database_name,DATABASE_COLLECTION_TEST,0);
  
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
    pointer_reset(delegates[count].pool_mode);
    pointer_reset(delegates[count].fee_structure);
    pointer_reset(delegates[count].server_settings);
    pointer_reset(delegates[count].block_verifier_score);
    pointer_reset(delegates[count].online_status);
    pointer_reset(delegates[count].block_verifier_total_rounds);
    pointer_reset(delegates[count].block_verifier_online_total_rounds);
    pointer_reset(delegates[count].block_verifier_online_percentage);
    pointer_reset(delegates[count].block_producer_total_rounds);
    pointer_reset(delegates[count].block_producer_block_heights);
    pointer_reset(delegates[count].public_key);
  }
 
  // write the end test message
  if (count_test == ORGANIZE_FUNCTIONS_TOTAL_TEST)
  {
    fprintf(stderr,"\n\n");
    color_print(TEST_OUTLINE,"green");
    fprintf(stderr,"\033[1;32morganize functions test - Passed test: %d, Failed test: 0\033[0m\n",ORGANIZE_FUNCTIONS_TOTAL_TEST);
    color_print(TEST_OUTLINE,"green");
    fprintf(stderr,"\n\n");
  }
  else
  {
    fprintf(stderr,"\n\n");
    color_print(TEST_OUTLINE,"red");
    fprintf(stderr,"\033[1;31morganize functions test - Passed test: %d, Failed test: %d\033[0m\n",count_test,ORGANIZE_FUNCTIONS_TOTAL_TEST-count_test);
    color_print(TEST_OUTLINE,"red");
    fprintf(stderr,"\n\n");
  }
  return count_test;

  #undef ORGANIZE_FUNCTIONS_TOTAL_TEST
  #undef MAXIMUM_DELEGATES_AMOUNT
  #undef ORGANIZE_DELEGATES_TEST_DATA_1
  #undef ORGANIZE_DELEGATES_TEST_DATA_2
  #undef ORGANIZE_DELEGATES_TEST_DATA_3
}