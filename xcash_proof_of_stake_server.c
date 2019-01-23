#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <unistd.h>
#include <signal.h>
#include "sys/types.h"
#include "sys/socket.h"
#include <netdb.h> 
#include "netinet/in.h"
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h> 

#include "xcash_proof_of_stake_functions_and_macros.h"
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

  // iniltize the global variables
  xcash_wallet_public_address = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  nodes_public_address_list = (char*)calloc(BUFFER_SIZE,sizeof(char));
  nodes_name_list = (char*)calloc(BUFFER_SIZE,sizeof(char));
  nodes_public_address_list_received_data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  server_message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  current_consensus_nodes_IP_address = (char*)calloc(BUFFER_SIZE,sizeof(char));
  main_nodes_public_address = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  current_round_part = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  current_round_part_backup_node = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // set the current_round_part and current_round_part_backup_node to an empty string, this way the node will start at the begining of a round
  memcpy(current_round_part,"",1); 
  memcpy(current_round_part_backup_node,"",1);

  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char)); 

  // write the message
  color_print("X-CASH Proof Of Stake, Version 1.0.0\n","green");

  // set the server_message
  memcpy(server_message,"CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS",48);

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
  if (get_current_consensus_nodes_IP_address() == 1)
  {
       
    
  }
  else
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