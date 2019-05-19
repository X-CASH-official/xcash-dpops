#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "network_daemon_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "thread_server_functions.h"
#include "vrf.h"
#include "crypto_vrf.h"
#include "VRF_functions.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: total_connection_time_thread
Description: Closes the forked process after a certain connection timeout
Parameters:
  parameters - A pointer to the total_connection_time_thread_parameters struct
  struct total_connection_time_thread_parameters
    process_id - The process id of the forked process
    client_address - The client's IP address for the forked process
    port - The client's connected port for the forked process
    data_received - 1 if data was received in the timeout time, otherwise 0
    message_settings - 1 to print the messages, otherwise 0. This is used for the testing flag to not print any success or error messages
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

void* total_connection_time_thread(void* parameters)
{ 
  // Variables
  char* string = (char*)calloc(BUFFER_SIZE,sizeof(char));
  struct total_connection_time_thread_parameters* data = parameters;
  int client_address_length = strnlen(data->client_address,BUFFER_SIZE);
  int data_port_length = strnlen(data->port,BUFFER_SIZE);

  // check if the memory needed was allocated on the heap successfully
  if (string == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  sleep(TOTAL_CONNECTION_TIME_SETTINGS);    
  if (data->message_settings == 1)
  {
    printf("Total connection time has been reached for %s on port %s\n", data->client_address,data->port); 
    if (data->data_received == 1)
    {
      memcpy(string,"Client at ",10);
      memcpy(string+10,data->client_address,client_address_length);
      memcpy(string+10+client_address_length," on port ",9);
      memcpy(string+19+client_address_length,data->port,data_port_length);
      memcpy(string+19+client_address_length+data_port_length," has sent data in the timeout time",34);
      color_print(string,"green");
    }
    else
    {
      memcpy(string,"Client at ",10);
      memcpy(string+10,data->client_address,client_address_length);
      memcpy(string+10+client_address_length," on port ",9);
      memcpy(string+19+client_address_length,data->port,data_port_length);
      memcpy(string+19+client_address_length+data_port_length," did not send any data in the timeout time",42);
      color_print(string,"red"); 
    }
  }
  pointer_reset(string);
  // close the client connection
  kill((intptr_t)data->process_id, SIGKILL); 
  pthread_exit((void *)(intptr_t)1);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: current_block_height_timer_thread
Description: Gets the current block height and determines if a new round has started
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

void* current_block_height_timer_thread()
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  time_t current_date_and_time;
  struct tm* current_UTC_date_and_time; 
  size_t count;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  for (;;)
  {
    start:
    // pause 200 milliseconds and then check the time. If it is a possible block time check if their is a new block
    usleep(200000);
    time(&current_date_and_time);
    current_UTC_date_and_time = gmtime(&current_date_and_time);
    if (current_UTC_date_and_time->tm_min % 5 == 0)
    {
      // try for the next 5 seconds and if not then a new block is not going to be added to the network
      for (count = 0; count < 5; count++)
      {
        get_current_block_height(data,0);
        if (memcmp(data,current_block_height,strlen(current_block_height)) != 0)
        {      
          // replace the current_block_height variable
          memset(current_block_height,0,strlen(current_block_height));
          memcpy(current_block_height,data,strnlen(data,BUFFER_SIZE));

          start_new_round();
          goto start;
        }
        sleep(1);
      }
    }
  }
  pointer_reset(data);
  pthread_exit((void *)(intptr_t)1);
}


/*
-----------------------------------------------------------------------------------------------------------
Name: mainnode_timeout_thread
Description: Closes the forked process after a certain connection timeout. This is so the node knows if it should send a message to the consensus node that the main node never sent any data, and if a round change should occur
Parameters:
  parameters - A pointer to the mainnode_timeout_thread_parameters struct
  struct mainnode_timeout_thread_parameters
    process_id - The process id of the forked process
    data_received - 1 if the node has received data from the main node, otherwise 0
    main_nodes_public_address - The public address of the main node for that part of the round
    current_round_part - The current round part (1-4).
    current_round_part_backup_node - The current main node in the current round part (0-5)
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

void* mainnode_timeout_thread(void* parameters)
{
  // Variables
  char* string = (char*)calloc(BUFFER_SIZE,sizeof(char));
  struct mainnode_timeout_thread_parameters* data = parameters;
  size_t main_node_length = strnlen(data->main_node,BUFFER_SIZE);

  // check if the memory needed was allocated on the heap successfully
  if (string == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  sleep(TOTAL_CONNECTION_TIME_SETTINGS_MAIN_NODE_TIMEOUT);  
  printf("Total connection time for the main node for current round part %s and current round part backup node %s has been reached",current_round_part,current_round_part_backup_node);
  if (data->data_received == 1)
  {
    memcpy(string,"Received data from main node, ",30);
    memcpy(string+30,data->main_node,main_node_length);
    memcpy(string+30+main_node_length," backup node number ",20);
    memcpy(string+50+main_node_length,data->current_round_part_backup_node,1);
    memcpy(string+51+main_node_length," in current round part ",23);
    memcpy(string+74+main_node_length,data->current_round_part,1);
    color_print(string,"green");     
  }
  else
  {
    memcpy(string,"main node, ",11);
    memcpy(string+11,data->main_node,main_node_length);
    memcpy(string+11+main_node_length," backup node number ",20);
    memcpy(string+31+main_node_length,data->current_round_part_backup_node,1);
    memcpy(string+32+main_node_length," in current round part ",23);
    memcpy(string+55+main_node_length,data->current_round_part,1);
    memcpy(string+56+main_node_length," did not send any data before the timeout\nSending the NODES_TO_CONSENSUS_NODE_MAIN_NODE_SOCKET_TIMEOUT_ROUND_CHANGE message to the consensus node",145);
    color_print(string,"red");  

    // set the next server message since a backup node will have to be selected
    memset(server_message,0,strnlen(server_message,BUFFER_SIZE));
    memcpy(server_message,"CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS",48); 

    // create the message
    memcpy(string,"{\r\n \"message_settings\": \"NODES_TO_CONSENSUS_NODE_MAIN_NODE_SOCKET_TIMEOUT_ROUND_CHANGE\",\r\n}",91);

    // sign_data
    if (sign_data(string,0) == 0)
    { 
      color_print("Could not sign_data\nFunction: mainnode_timeout_thread\nSend Message: NODES_TO_CONSENSUS_NODE_MAIN_NODE_SOCKET_TIMEOUT_ROUND_CHANGE","red");
    }
 
    // send the message to the consensus node
    if (send_data_socket(current_consensus_nodes_IP_address,SEND_DATA_PORT,string,"sending NODES_TO_CONSENSUS_NODE_MAIN_NODE_SOCKET_TIMEOUT_ROUND_CHANGE to the consensus node",0) == 0)
    {
      color_print("Could not send data to the consensus node\n\nFunction: mainnode_timeout_thread\nSend Message: NODES_TO_CONSENSUS_NODE_MAIN_NODE_SOCKET_TIMEOUT_ROUND_CHANGE","red");
    } 
  }
  pointer_reset(string);

  // close the client connection, since the consensus node is still connected
  kill((intptr_t)data->process_id, SIGTERM);

  // reset the mainnode_timeout_thread_parameters
  data->process_id = 0;
  data->data_received = 0;
  data->main_node = "";
  data->current_round_part = "";
  data->current_round_part_backup_node = "";

  pthread_exit((void *)(intptr_t)1);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: node_to_node_message_timeout_thread
Description: Closes the forked process after a certain connection timeout. This is so the block verifiers have waited a certain amount of time, and will now count all of the votes
Parameters:
  parameters - A pointer to the node_to_node_timeout_thread_parameters struct
  struct node_to_node_timeout_thread_parameters
    process_id - The process id of the forked process
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

void* node_to_node_message_timeout_thread(void* parameters)
{
  // Variables
  char* string = (char*)calloc(BUFFER_SIZE,sizeof(char));
  struct node_to_node_timeout_thread_parameters* data = parameters;

  // check if the memory needed was allocated on the heap successfully
  if (string == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  sleep(TOTAL_CONNECTION_TIME_SETTINGS_MAIN_NODE_TIMEOUT);  
  printf("Total connection time for all block verifiers to send data to all other block verifiers for current round part %s and current round part backup node %s has been reached",current_round_part,current_round_part_backup_node); 
  
  // create the message
  memcpy(string,"{\r\n \"message_settings\": \"NODES_TO_CONSENSUS_NODE_VOTE_RESULTS\",\r\n  \"vote_result\": \"",84);

  // verify the VRF data
  if (crypto_vrf_verify((unsigned char*)current_round_part_consensus_node_data.vrf_beta_string,(const unsigned char*)current_round_part_consensus_node_data.vrf_public_key,(const unsigned char*)current_round_part_consensus_node_data.vrf_proof,(const unsigned char*)current_round_part_consensus_node_data.vrf_alpha_string,(unsigned long long)strnlen(current_round_part_consensus_node_data.vrf_alpha_string,BUFFER_SIZE)) == 0)
  {
    memcpy(string+84,"TRUE",4);
  }
  else
  {
    memcpy(string+84,"FALSE",5);
  }
  memcpy(string+strnlen(string,BUFFER_SIZE),"\",\r\n  \"vrf_public_key\": \"",25);
  memcpy(string+strnlen(string,BUFFER_SIZE),current_round_part_consensus_node_data.vrf_public_key,VRF_PUBLIC_KEY_LENGTH);
  memcpy(string+strnlen(string,BUFFER_SIZE),"\",\r\n  \"vrf_alpha_string\": \"",27);
  memcpy(string+strnlen(string,BUFFER_SIZE),current_round_part_consensus_node_data.vrf_alpha_string,strnlen(current_round_part_consensus_node_data.vrf_alpha_string,BUFFER_SIZE));
  memcpy(string+strnlen(string,BUFFER_SIZE),"\",\r\n  \"vrf_proof\": \"",20);
  memcpy(string+strnlen(string,BUFFER_SIZE),current_round_part_consensus_node_data.vrf_proof,VRF_PROOF_LENGTH);
  memcpy(string+strnlen(string,BUFFER_SIZE),"\",\r\n  \"vrf_beta_string\": \"",26);
  memcpy(string+strnlen(string,BUFFER_SIZE),current_round_part_consensus_node_data.vrf_beta_string,VRF_BETA_LENGTH);
  memcpy(string+strnlen(string,BUFFER_SIZE),"\",\r\n  \"block_blob\": \"",21);
  memcpy(string+strnlen(string,BUFFER_SIZE),current_round_part_consensus_node_data.block_blob,strnlen(current_round_part_consensus_node_data.block_blob,BUFFER_SIZE));
  memcpy(string+strnlen(string,BUFFER_SIZE),"\"}",2);

  // sign_data
  if (sign_data(string,0) == 0)
  { 
    color_print("Could not sign_data\nFunction: node_to_node_message_timeout_thread\nSend Message: NODES_TO_CONSENSUS_NODE_VOTE_RESULTS","red");
  }
 
  // send the message to the consensus node
  if (send_data_socket(current_consensus_nodes_IP_address,SEND_DATA_PORT,string,"sending NODES_TO_CONSENSUS_NODE_VOTE_RESULTS to the consensus node",0) == 0)
  {
    color_print("Could not send data to the consensus node\n\nFunction: node_to_node_message_timeout_thread\nSend Message: NODES_TO_CONSENSUS_NODE_VOTE_RESULTS","red");
  } 

  // set the next server message since a backup node will have to be selected
  memset(server_message,0,strnlen(server_message,BUFFER_SIZE));
  memcpy(server_message,"CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS",48); 
  
  pointer_reset(string);

  // close the client connection, since the consensus node is still connected
  kill((intptr_t)data->process_id, SIGTERM);

  // reset the node_to_node_timeout_thread_parameters
  data->process_id = 0;

  pthread_exit((void *)(intptr_t)1);
}