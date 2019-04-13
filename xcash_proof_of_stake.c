#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "network_wallet_functions.h"
#include "server_functions.h"

#include "xcash_proof_of_stake_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Main function
-----------------------------------------------------------------------------------------------------------
*/

int main(int parameters_count, char* parameters[])
{
  // iniltize the random number generator
  srand(time(0));

  // Variables
  FILE* file;
  size_t count = 0;

  // iniltize the global variables
  xcash_wallet_public_address = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  nodes_public_address_list_received_data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  server_message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  current_consensus_nodes_IP_address = (char*)calloc(BUFFER_SIZE,sizeof(char));
  main_nodes_public_address = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  vrf_public_key_part_1 = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  vrf_alpha_string_part_2 = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  current_vote_results = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  vote_results_valid = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  vote_results_invalid = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  current_round_part = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  current_round_part_backup_node = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // check if the memory needed was allocated on the heap successfully
  if (xcash_wallet_public_address == NULL || nodes_public_address_list_received_data == NULL || server_message == NULL || current_consensus_nodes_IP_address == NULL || main_nodes_public_address == NULL || vrf_public_key_part_1 == NULL || vrf_alpha_string_part_2 == NULL || current_vote_results == NULL || vote_results_valid == NULL || vote_results_invalid == NULL || current_round_part == NULL || current_round_part_backup_node == NULL)
  {
    if (xcash_wallet_public_address != NULL)
    {
      pointer_reset(xcash_wallet_public_address);
    }
    if (nodes_public_address_list_received_data != NULL)
    {
      pointer_reset(nodes_public_address_list_received_data);
    }
    if (server_message != NULL)
    {
      pointer_reset(server_message);
    }
    if (current_consensus_nodes_IP_address != NULL)
    {
      pointer_reset(current_consensus_nodes_IP_address);
    }
    if (main_nodes_public_address != NULL)
    {
      pointer_reset(main_nodes_public_address);
    }
    if (vrf_public_key_part_1 != NULL)
    {
      pointer_reset(vrf_public_key_part_1);
    }
    if (vrf_alpha_string_part_2 != NULL)
    {
      pointer_reset(vrf_alpha_string_part_2);
    }
    if (current_vote_results != NULL)
    {
      pointer_reset(current_vote_results);
    }
    if (vote_results_valid != NULL)
    {
      pointer_reset(vote_results_valid);
    }
    if (vote_results_invalid != NULL)
    {
      pointer_reset(vote_results_invalid);
    }
    if (current_round_part != NULL)
    {
      pointer_reset(current_round_part);
    }
    if (current_round_part_backup_node != NULL)
    {
      pointer_reset(current_round_part_backup_node);
    }
    return 0;
  } 

  // initialize the block_verifiers_list struct 
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    block_verifiers_list.block_verifiers_name[count] = (char*)calloc(BLOCK_VERIFIERS_NAME_TOTAL_LENGTH,sizeof(char));
    block_verifiers_list.block_verifiers_public_address[count] = (char*)calloc(XCASH_WALLET_LENGTH,sizeof(char));
    block_verifiers_list.block_verifiers_IP_address[count] = (char*)calloc(BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH,sizeof(char));
  }

  // set the current_round_part, current_round_part_backup_node and server message, this way the node will start at the begining of a round
  memset(current_round_part,0,strnlen(current_round_part,BUFFER_SIZE));
  memset(current_round_part_backup_node,0,strnlen(current_round_part_backup_node,BUFFER_SIZE));
  memcpy(current_round_part,"1",1);
  memcpy(current_round_part_backup_node,"0",1);
  memset(server_message,0,strnlen(server_message,BUFFER_SIZE));
  memcpy(server_message,"CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS|CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND",96);

  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char)); 

  // write the message
  color_print("X-CASH Proof Of Stake, Version 1.0.0\n","green");

  // get the wallets public address
  printf("Getting the public address\n\n");
  if (get_public_address(0) == 1)
  {  
    // print the public address
    memcpy(data,"Successfully received the public address:",41);
    memcpy(data+41,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
    memcpy(data+41+XCASH_WALLET_LENGTH,"\n",1);
    color_print(data,"green");
  }
  else
  {
    color_print("Could not get the wallets public address\n","red");
    exit(0);
  }
  
  // check if the program needs to run the test
  if (parameters_count == 2)
  {
    if (strncmp(parameters[1],"--test",BUFFER_SIZE) == 0)
    {
      test();
    }
    else
    {
      color_print("Invalid parameters\n","red");
      printf(INVALID_PARAMETERS_ERROR_MESSAGE);
    }
    exit(0);
  }

  // check if the files are created and if not create them with default values
  file = fopen(NODES_UPDATED_TIME_FILE_NAME,"r");
  if (file == NULL)
  {
    file = fopen(NODES_UPDATED_TIME_FILE_NAME,"w");
    fprintf(file,"0");
    fclose(file);
  }
  else
  {    
    fclose(file);
  }

  // get the current consensus nodes IP address
  printf("Getting the current consensus nodes IP address\n\n");
  if (get_current_consensus_nodes_IP_address() == 1)
  {
    memcpy(data,"Successfully received the current consensus nodes IP address:\n",61);
    if (strncmp(current_consensus_nodes_IP_address,CONSENSUS_NODES_IP_ADDRESS,BUFFER_SIZE) == 0)
    {
      const size_t CONSENSUS_NODES_IP_ADDRESS_LENGTH = strnlen(CONSENSUS_NODES_IP_ADDRESS,BUFFER_SIZE);
      memcpy(data,"Successfully received the current consensus nodes IP address:\nConsensus nodes IP address:",90);
      memcpy(data+90,CONSENSUS_NODES_IP_ADDRESS,CONSENSUS_NODES_IP_ADDRESS_LENGTH);
      memcpy(data+90+CONSENSUS_NODES_IP_ADDRESS_LENGTH,"\n",1);      
    }
    else if (strncmp(current_consensus_nodes_IP_address,CONSENSUS_BACKUP_NODES_IP_ADDRESS,BUFFER_SIZE) == 0)
    {
      const size_t CONSENSUS_BACKUP_NODES_IP_ADDRESS_LENGTH = strnlen(CONSENSUS_BACKUP_NODES_IP_ADDRESS,BUFFER_SIZE);
      memcpy(data,"Successfully received the current consensus nodes IP address:\nConsensus backup nodes IP address:",97);
      memcpy(data+97,CONSENSUS_BACKUP_NODES_IP_ADDRESS,CONSENSUS_BACKUP_NODES_IP_ADDRESS_LENGTH);
      memcpy(data+97+CONSENSUS_BACKUP_NODES_IP_ADDRESS_LENGTH,"\n",1);      
    }
    else
    {
      color_print("Could not get the current consensus nodes IP address\n","red");
      exit(0); 
    }
    color_print(data,"green");
  }
  else
  {
    color_print("Could not get the current consensus nodes IP address\n","red");
    exit(0);    
  }
  
  // get the updated node list
  printf("Getting the updated node list\n\n");
  if (get_current_consensus_nodes_IP_address() == 0)
  {
    color_print("Could not get the updated node list\n","red");
    exit(0);    
  }

  // start the server
  if (create_server(1) == 0)
  {
    color_print("Could not start the server","red");
    exit(0);
  }

  return 0;   
}