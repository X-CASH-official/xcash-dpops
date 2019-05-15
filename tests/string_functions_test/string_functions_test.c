#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"

#include "string_functions.h"

#include "define_macros_test.h"
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

int string_functions_test()
{  
  // Variables
  int settings = 1;

  // define macros
  #define STRING_FUNCTIONS_TOTAL_TEST 33
 
  #define SIGN_RPC_CALL_TEST_DATA "{\r\n  \"id\": \"0\",\r\n  \"jsonrpc\": \"2.0\",\r\n  \"result\": {\r\n    \"xcash_proof_of_stake_signature\": \"SIGN_RPC_CALL_TEST_DATA\"\r\n  }\r\n}"
  #define VERIFY_RPC_CALL_TEST_DATA "{\r\n  \"id\": \"0\",\r\n  \"jsonrpc\": \"2.0\",\r\n  \"result\": {\r\n    \"good\": VERIFY_RPC_CALL_TEST_DATA\r\n  }\r\n}"
  #define GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA "{\r\n  \"id\": \"0\",\r\n  \"jsonrpc\": \"2.0\",\r\n  \"result\": {\r\n    \"blockhashing_blob\": \"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\",\r\n    \"blocktemplate_blob\": \"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\",\r\n    \"difficulty\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,\r\n    \"expected_reward\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,\r\n    \"height\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,\r\n    \"prev_hash\": \"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\",\r\n    \"reserved_offset\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,\r\n    \"status\": \"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\",\r\n    \"untrusted\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\r\n  }\r\n}"
 
/*
  This message is sent from the Consensus node in the node_to_consensus_node_send_current_consensus_node_IP_address function and received by the nodes in the get_current_consensus_node_IP_address function.
 
  The purpose of this message is for the Consensus node to send an the current consensus nodes public address to the node. 
 
  message_settings - The type of the message
  current_consensus_node_IP_address - The current consensus nodes IP address.
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define CONSENSUS_NODE_TO_NODE_RECEIVE_CURRENT_CONSENSUS_NODE_IP_ADDRESS "{\r\n \"message_settings\": \"CONSENSUS_NODE_TO_NODE_RECEIVE_CURRENT_CONSENSUS_NODE_IP_ADDRESS\",\r\n \"current_consensus_node_IP_address\": \"CONSENSUS_NODE_TO_NODE_RECEIVE_CURRENT_CONSENSUS_NODE_IP_ADDRESS\",\r\n \"public_address\": \"CONSENSUS_NODE_TO_NODE_RECEIVE_CURRENT_CONSENSUS_NODE_IP_ADDRESS\",\r\n \"previous_block_hash\": \"CONSENSUS_NODE_TO_NODE_RECEIVE_CURRENT_CONSENSUS_NODE_IP_ADDRESS\",\r\n \"current_round_part\": \"CONSENSUS_NODE_TO_NODE_RECEIVE_CURRENT_CONSENSUS_NODE_IP_ADDRESS\",\r\n \"current_round_part_backup_node\": \"CONSENSUS_NODE_TO_NODE_RECEIVE_CURRENT_CONSENSUS_NODE_IP_ADDRESS\",\r\n \"data\": \"CONSENSUS_NODE_TO_NODE_RECEIVE_CURRENT_CONSENSUS_NODE_IP_ADDRESS\",\r\n \"xcash_proof_of_stake_signature\": \"CONSENSUS_NODE_TO_NODE_RECEIVE_CURRENT_CONSENSUS_NODE_IP_ADDRESS\",\r\n}"



/*
  This message is sent from the nodes in the get_current_consensus_node_IP_address function and received by the consensus node in the node_to_consensus_node_send_current_consensus_node_IP_address function.
 
  The purpose of this message is for the node to let the consensus node know that it needs the current consensus node IP address.
 
  message_settings - The type of the message
  message - The last time that the node has updated the block verifiers list
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define NODE_TO_CONSENSUS_NODE_SEND_CURRENT_CONSENSUS_NODE_IP_ADDRESS "{\r\n \"message_settings\": \"NODE_TO_CONSENSUS_NODE_SEND_CURRENT_CONSENSUS_NODE_IP_ADDRESS\",\r\n \"message\": \"NODE_TO_CONSENSUS_NODE_SEND_CURRENT_CONSENSUS_NODE_IP_ADDRESS\",\r\n \"public_address\": \"NODE_TO_CONSENSUS_NODE_SEND_CURRENT_CONSENSUS_NODE_IP_ADDRESS\",\r\n \"previous_block_hash\": \"NODE_TO_CONSENSUS_NODE_SEND_CURRENT_CONSENSUS_NODE_IP_ADDRESS\",\r\n \"current_round_part\": \"NODE_TO_CONSENSUS_NODE_SEND_CURRENT_CONSENSUS_NODE_IP_ADDRESS\",\r\n \"current_round_part_backup_node\": \"NODE_TO_CONSENSUS_NODE_SEND_CURRENT_CONSENSUS_NODE_IP_ADDRESS\",\r\n \"data\": \"NODE_TO_CONSENSUS_NODE_SEND_CURRENT_CONSENSUS_NODE_IP_ADDRESS\",\r\n \"xcash_proof_of_stake_signature\": \"NODE_TO_CONSENSUS_NODE_SEND_CURRENT_CONSENSUS_NODE_IP_ADDRESS\",\r\n}"



/*
  This message is sent from the Consensus node in the node_to_consensus_node_send_updated_node_list function and received by the nodes in the get_updated_node_list function.
 
  The purpose of this message is for the Consensus node to send an updated list of nodes to the node. 
 
  message_settings - The type of the message
  nodes_name_list - The list of the nodes name list ({"node1":"node1_name"}) or UPDATED_NODE_LIST
  nodes_public_address_list - the list of the nodes public address ({"node1":"node1_public_address"}) or UPDATED_NODE_LIST
  nodes_IP_address_list - the list of the nodes IP addresses ({"node1":"node1_IP_address"}) or UPDATED_NODE_LIST
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define CONSENSUS_NODE_TO_NODE_RECEIVE_UPDATED_NODE_LIST "{\r\n \"message_settings\": \"CONSENSUS_NODE_TO_NODE_RECEIVE_UPDATED_NODE_LIST\",\r\n \"nodes_name_list\": \"CONSENSUS_NODE_TO_NODE_RECEIVE_UPDATED_NODE_LIST\",\r\n \"nodes_public_address_list\": \"CONSENSUS_NODE_TO_NODE_RECEIVE_UPDATED_NODE_LIST\",\r\n \"nodes_IP_address_list\": \"CONSENSUS_NODE_TO_NODE_RECEIVE_UPDATED_NODE_LIST\",\r\n \"public_address\": \"CONSENSUS_NODE_TO_NODE_RECEIVE_UPDATED_NODE_LIST\",\r\n \"previous_block_hash\": \"CONSENSUS_NODE_TO_NODE_RECEIVE_UPDATED_NODE_LIST\",\r\n \"current_round_part\": \"CONSENSUS_NODE_TO_NODE_RECEIVE_UPDATED_NODE_LIST\",\r\n \"current_round_part_backup_node\": \"CONSENSUS_NODE_TO_NODE_RECEIVE_UPDATED_NODE_LIST\",\r\n \"data\": \"CONSENSUS_NODE_TO_NODE_RECEIVE_UPDATED_NODE_LIST\",\r\n \"xcash_proof_of_stake_signature\": \"CONSENSUS_NODE_TO_NODE_RECEIVE_UPDATED_NODE_LIST\",\r\n}"



/*
  This message is sent from the nodes in the get_updated_node_list function and received by the consensus node in the node_to_consensus_node_send_updated_node_list function.
 
  The purpose of this message is for the node to get an updated list of nodes from the consensus node.
 
  message_settings - The type of the message
  nodes_updated_time - The nodes_updated_time that the node last updated the nodes list
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define NODE_TO_CONSENSUS_NODE_SEND_UPDATED_NODE_LIST "{\r\n \"message_settings\": \"NODE_TO_CONSENSUS_NODE_SEND_UPDATED_NODE_LIST\",\r\n \"nodes_updated_time\": \"NODE_TO_CONSENSUS_NODE_SEND_UPDATED_NODE_LIST\",\r\n \"public_address\": \"NODE_TO_CONSENSUS_NODE_SEND_UPDATED_NODE_LIST\",\r\n \"previous_block_hash\": \"NODE_TO_CONSENSUS_NODE_SEND_UPDATED_NODE_LIST\",\r\n \"current_round_part\": \"NODE_TO_CONSENSUS_NODE_SEND_UPDATED_NODE_LIST\",\r\n \"current_round_part_backup_node\": \"NODE_TO_CONSENSUS_NODE_SEND_UPDATED_NODE_LIST\",\r\n \"data\": \"NODE_TO_CONSENSUS_NODE_SEND_UPDATED_NODE_LIST\",\r\n \"xcash_proof_of_stake_signature\": \"NODE_TO_CONSENSUS_NODE_SEND_UPDATED_NODE_LIST\",\r\n}"



/*
  This message is sent from the Consensus node in the send_data_socket_consensus_node_to_node function and received by the nodes in the receive_data_socket_consensus_node_to_node function.
 
  The purpose of this message is for the consensus node to let the nodes know who is the main node for this part of the round.
 
  message_settings - The type of the message.
  main_nodes_public_address - The public address of the main node for that part of the round.
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS "{\r\n \"message_settings\": \"CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS\",\r\n \"main_nodes_public_address\": \"CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS\",\r\n \"public_address\": \"CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS\",\r\n \"previous_block_hash\": \"CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS\",\r\n \"current_round_part\": \"CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS\",\r\n \"current_round_part_backup_node\": \"CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS\",\r\n \"data\": \"CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS\",\r\n \"xcash_proof_of_stake_signature\": \"CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS\",\r\n}"
 
 
 
 /*
  This message is sent from the nodes in the mainnode_timeout function and received by the Consensus node in the main_node_timeout_from_node function.
 
  The purpose of this message is for the node to let the consensus node know that the main node did not send the data for its part of the round, so the node is voting for a round change so a backup main node can start that part of the round.
 
  message_settings - The type of the message.
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define NODES_TO_CONSENSUS_NODE_MAIN_NODE_SOCKET_TIMEOUT_ROUND_CHANGE "{\r\n \"message_settings\": \"NODES_TO_CONSENSUS_NODE_MAIN_NODE_SOCKET_TIMEOUT_ROUND_CHANGE\",\r\n \"public_address\": \"NODES_TO_CONSENSUS_NODE_MAIN_NODE_SOCKET_TIMEOUT_ROUND_CHANGE\",\r\n \"previous_block_hash\": \"NODES_TO_CONSENSUS_NODE_MAIN_NODE_SOCKET_TIMEOUT_ROUND_CHANGE\",\r\n \"current_round_part\": \"NODES_TO_CONSENSUS_NODE_MAIN_NODE_SOCKET_TIMEOUT_ROUND_CHANGE\",\r\n \"current_round_part_backup_node\": \"NODES_TO_CONSENSUS_NODE_MAIN_NODE_SOCKET_TIMEOUT_ROUND_CHANGE\",\r\n \"data\": \"NODES_TO_CONSENSUS_NODE_MAIN_NODE_SOCKET_TIMEOUT_ROUND_CHANGE\",\r\n \"xcash_proof_of_stake_signature\": \"NODES_TO_CONSENSUS_NODE_MAIN_NODE_SOCKET_TIMEOUT_ROUND_CHANGE\",\r\n}"
 
 
 
/*
  This message is sent from the Consensus node in the send_data_socket_consensus_node_to_mainnode function and received by the main node in the consensus_node_to_main_node function.
 
  The purpose of this message is for the consensus node to let the main node that they are a main node, and they need to start this part of the round.
 
  message_settings - The type of the message.
  message - the part of the round (1-4).
  VRF_block_blob - This is full VRF_block_blob that will be added to the block_blob in the reserve bytes, and is only included in the message to the block producer in part 4 of the round. For the part of the rounds 1-3 the VRF_block_blob ""
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. The random data will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
*/
  #define CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND "{\r\n \"message_settings\": \"CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND\",\r\n \"message\": \"CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND\",\r\n \"vrf_beta_string_part_3_of_round\": \"CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND\",\r\n \"public_address\": \"CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND\",\r\n \"previous_block_hash\": \"CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND\",\r\n \"current_round_part\": \"CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND\",\r\n \"current_round_part_backup_node\": \"CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND\",\r\n \"data\": \"CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND\",\r\n \"xcash_proof_of_stake_signature\": \"CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND\",\r\n}"
 
 
 
/*
  This message is sent from the main node in the send_data_socket_consensus_node_to_mainnode function and received by the nodes in the mainnode_to_node_message_part_1 function.
 
  The purpose of this message is for the main node to send the created data for the part of the round to the nodes, so they can verify the data.
 
  message_settings - The type of the message
  vrf_public_key - The VRF public key.
  vrf_alpha_string - The VRF alpha string. (this is the vrf_public_key for this part of the round)
  vrf_proof - The VRF proof.
  vrf_beta_string - The VRF beta_string.
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define MAIN_NODES_TO_NODES_PART_1_OF_ROUND "{\r\n \"message_settings\": \"MAIN_NODES_TO_NODES_PART_1_OF_ROUND\",\r\n \"vrf_public_key\": \"MAIN_NODES_TO_NODES_PART_1_OF_ROUND\",\r\n \"vrf_alpha_string\": \"MAIN_NODES_TO_NODES_PART_1_OF_ROUND\",\r\n \"vrf_proof\": \"MAIN_NODES_TO_NODES_PART_1_OF_ROUND\",\r\n \"vrf_beta_string\": \"MAIN_NODES_TO_NODES_PART_1_OF_ROUND\",\r\n \"public_address\": \"MAIN_NODES_TO_NODES_PART_1_OF_ROUND\",\r\n \"previous_block_hash\": \"MAIN_NODES_TO_NODES_PART_1_OF_ROUND\",\r\n \"current_round_part\": \"MAIN_NODES_TO_NODES_PART_1_OF_ROUND\",\r\n \"current_round_part_backup_node\": \"MAIN_NODES_TO_NODES_PART_1_OF_ROUND\",\r\n \"data\": \"MAIN_NODES_TO_NODES_PART_1_OF_ROUND\",\r\n \"xcash_proof_of_stake_signature\": \"MAIN_NODES_TO_NODES_PART_1_OF_ROUND\",\r\n}"
 
 
 
/*
  This message is sent from the main node in the send_data_socket_consensus_node_to_mainnode function and received by the nodes in the mainnode_to_node_message_part_2 function.
 
  The purpose of this message is for the main node to send the created data for the part of the round to the nodes, so they can verify the data.
 
  message_settings - The type of the message
  vrf_random_character_string - A random 100 character string, used to create the VRF alpha_string.
  vrf_public_key - The VRF public key.
  vrf_alpha_string - The VRF alpha string. (this is the previous_block_hash|vrf_random_character_string)
  vrf_proof - The VRF proof.
  vrf_beta_string - The VRF beta_string.
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define MAIN_NODES_TO_NODES_PART_2_OF_ROUND "{\r\n \"message_settings\": \"MAIN_NODES_TO_NODES_PART_2_OF_ROUND\",\r\n \"vrf_random_character_string\": \"MAIN_NODES_TO_NODES_PART_2_OF_ROUND\",\r\n \"vrf_public_key\": \"MAIN_NODES_TO_NODES_PART_2_OF_ROUND\",\r\n \"vrf_alpha_string\": \"MAIN_NODES_TO_NODES_PART_2_OF_ROUND\",\r\n \"vrf_proof\": \"MAIN_NODES_TO_NODES_PART_2_OF_ROUND\",\r\n \"vrf_beta_string\": \"MAIN_NODES_TO_NODES_PART_2_OF_ROUND\",\r\n \"public_address\": \"MAIN_NODES_TO_NODES_PART_2_OF_ROUND\",\r\n \"previous_block_hash\": \"MAIN_NODES_TO_NODES_PART_2_OF_ROUND\",\r\n \"current_round_part\": \"MAIN_NODES_TO_NODES_PART_2_OF_ROUND\",\r\n \"current_round_part_backup_node\": \"MAIN_NODES_TO_NODES_PART_2_OF_ROUND\",\r\n \"data\": \"MAIN_NODES_TO_NODES_PART_2_OF_ROUND\",\r\n \"xcash_proof_of_stake_signature\": \"MAIN_NODES_TO_NODES_PART_2_OF_ROUND\",\r\n}"
 
 
 
/*
  This message is sent from the main node in the send_data_socket_consensus_node_to_mainnode function and received by the nodes in the mainnode_to_node_message_part_3 function.
 
  The purpose of this message is for the main node to send the created data for the part of the round to the nodes, so they can verify the data.
 
  message_settings - The type of the message
  vrf_public_key - The same VRF public key from part 1 of the round.
  vrf_alpha_string - The VRF alpha string. (this is the vrf_beta_string created in round 2)
  vrf_proof - The VRF proof.
  vrf_beta_string - The VRF beta_string.
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define MAIN_NODES_TO_NODES_PART_3_OF_ROUND "{\r\n \"message_settings\": \"MAIN_NODES_TO_NODES_PART_3_OF_ROUND\",\r\n \"vrf_random_character_string\": \"MAIN_NODES_TO_NODES_PART_3_OF_ROUND\",\r\n \"vrf_public_key\": \"MAIN_NODES_TO_NODES_PART_3_OF_ROUND\",\r\n \"vrf_alpha_string\": \"MAIN_NODES_TO_NODES_PART_3_OF_ROUND\",\r\n \"vrf_proof\": \"MAIN_NODES_TO_NODES_PART_3_OF_ROUND\",\r\n \"vrf_beta_string\": \"MAIN_NODES_TO_NODES_PART_3_OF_ROUND\",\r\n \"public_address\": \"MAIN_NODES_TO_NODES_PART_3_OF_ROUND\",\r\n \"previous_block_hash\": \"MAIN_NODES_TO_NODES_PART_3_OF_ROUND\",\r\n \"current_round_part\": \"MAIN_NODES_TO_NODES_PART_3_OF_ROUND\",\r\n \"current_round_part_backup_node\": \"MAIN_NODES_TO_NODES_PART_3_OF_ROUND\",\r\n \"data\": \"MAIN_NODES_TO_NODES_PART_3_OF_ROUND\",\r\n \"xcash_proof_of_stake_signature\": \"MAIN_NODES_TO_NODES_PART_3_OF_ROUND\",\r\n}"
 
 
 
/*
  This message is sent from the main node in the send_data_socket_consensus_node_to_mainnode function and received by the nodes in the mainnode_to_node_message_part_4 function.
 
  The purpose of this message is for the block producer to send the created block for the part of the round to the nodes, so they can verify the block.
 
  message_settings - The type of the message
  block_blob - The block_blob.
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define MAIN_NODES_TO_NODES_PART_4_OF_ROUND "{\r\n \"message_settings\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND\",\r\n \"block_blob\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND\",\r\n \"public_address\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND\",\r\n \"previous_block_hash\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND\",\r\n \"current_round_part\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND\",\r\n \"current_round_part_backup_node\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND\",\r\n \"data\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND\",\r\n \"xcash_proof_of_stake_signature\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND\",\r\n}"
 
 
 
/*
  This message is sent from the nodes in the mainnode_to_node_message_part_1 - 4 function and received by the nodes in the node_to_node function.
 
  The purpose of this message is for the nodes to verify what the other nodes received from the main node.
 
  message_settings - The type of the message
  vote_settings - Valid or Invalid if the data was not verified.
  vote_data - The SHA2-512 data that the node received.
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define NODES_TO_NODES_VOTE_RESULTS "{\r\n \"message_settings\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n \"vote_settings\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n \"vote_data\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n \"public_address\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n \"previous_block_hash\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n \"current_round_part\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n \"current_round_part_backup_node\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n \"data\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n \"xcash_proof_of_stake_signature\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n}"
 
 
 
/*
  This message is sent from the nodes in the node_to_node_message_timeout function and received by the consensus node in the node_to_node_vote function.
 
  The purpose of this message is for the nodes to let the consensus node know what the vote count is for that part of the round.
 
  message_settings - The type of the message
  vote_result - TRUE or FALSE
  vrf_public_key - The VRF public key.
  vrf_alpha_string - The VRF alpha string.
  vrf_proof - The VRF proof.
  vrf_beta_string - The VRF beta_string.
  block_blob - The block blob.
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define NODES_TO_CONSENSUS_NODE_VOTE_RESULTS "{\r\n \"message_settings\": \"NODES_TO_CONSENSUS_NODE_VOTE_RESULTS\",\r\n \"vote_result\": \"NODES_TO_CONSENSUS_NODE_VOTE_RESULTS\",\r\n \"public_address\": \"NODES_TO_CONSENSUS_NODE_VOTE_RESULTS\",\r\n \"previous_block_hash\": \"NODES_TO_CONSENSUS_NODE_VOTE_RESULTS\",\r\n \"current_round_part\": \"NODES_TO_CONSENSUS_NODE_VOTE_RESULTS\",\r\n \"current_round_part_backup_node\": \"NODES_TO_CONSENSUS_NODE_VOTE_RESULTS\",\r\n \"data\": \"NODES_TO_CONSENSUS_NODE_VOTE_RESULTS\",\r\n \"xcash_proof_of_stake_signature\": \"NODES_TO_CONSENSUS_NODE_VOTE_RESULTS\",\r\n}"
 
 
 
/*
  This message is sent from the Consensus node in the send_vote_list_to_nodes function and received by the nodes in the receive_vote_list function.
 
  The purpose of this message is for the nodes to get the list of the current top 100 delegates for that 24 hour time period, so they know what delegates are enabled.
 
  message_settings - The type of the message
 nodes_name_list - The list of the nodes name list ({"node1":"node1_name"}) or UPDATED_NODE_LIST
  nodes_public_address_list - the list of the nodes public address ({"node1":"node1_public_address"}) or UPDATED_NODE_LIST
  nodes_IP_address_list - the list of the nodes IP addresses ({"node1":"node1_IP_address"}) or UPDATED_NODE_LIST
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define CONSENSUS_NODE_TO_NODES_LIST_OF_ENABLED_NODES "{\r\n \"message_settings\": \"CONSENSUS_NODE_TO_NODES_LIST_OF_ENABLED_NODES\",\r\n \"nodes_name_list\": \"CONSENSUS_NODE_TO_NODES_LIST_OF_ENABLED_NODES\",\r\n \"nodes_public_address_list\": \"CONSENSUS_NODE_TO_NODES_LIST_OF_ENABLED_NODES\",\r\n \"nodes_IP_address_list\": \"CONSENSUS_NODE_TO_NODES_LIST_OF_ENABLED_NODES\",\r\n \"public_address\": \"CONSENSUS_NODE_TO_NODES_LIST_OF_ENABLED_NODES\",\r\n \"previous_block_hash\": \"CONSENSUS_NODE_TO_NODES_LIST_OF_ENABLED_NODES\",\r\n \"current_round_part\": \"CONSENSUS_NODE_TO_NODES_LIST_OF_ENABLED_NODES\",\r\n \"current_round_part_backup_node\": \"CONSENSUS_NODE_TO_NODES_LIST_OF_ENABLED_NODES\",\r\n \"data\": \"CONSENSUS_NODE_TO_NODES_LIST_OF_ENABLED_NODES\",\r\n \"xcash_proof_of_stake_signature\": \"CONSENSUS_NODE_TO_NODES_LIST_OF_ENABLED_NODES\",\r\n}"


 
/*
  This message is sent from the Consensus node in the send_round_change function and received by the nodes and main nodes in the receive_round_change function.
 
  The purpose of this message is for the Consensus node to let the nodes and the main node know that there is a round change, and they should restart.
 
  message_settings - The type of the message
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_ROUND_CHANGE "{\r\n \"message_settings\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_ROUND_CHANGE\",\r\n \"public_address\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_ROUND_CHANGE\",\r\n \"previous_block_hash\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_ROUND_CHANGE\",\r\n \"current_round_part\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_ROUND_CHANGE\",\r\n \"current_round_part_backup_node\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_ROUND_CHANGE\",\r\n \"data\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_ROUND_CHANGE\",\r\n \"xcash_proof_of_stake_signature\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_ROUND_CHANGE\",\r\n}"
 
 
 
/*
  This message is sent from the Consensus node in the check_if_consensus_node_is_offline function and received by the nodes and main nodes in the receive_change_consensus_node function.
 
  The purpose of this message is for the Consensus node to let the nodes and the main node know that there is a consensus node change, and they should restart.
 
  message_settings - The type of the message 
  consensus_node_IP_address - The IP address of the new consensus node.
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CHANGE "{\r\n \"message_settings\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CHANGE\",\r\n \"consensus_node_IP_address\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CHANGE\",\r\n \"public_address\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CHANGE\",\r\n \"previous_block_hash\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CHANGE\",\r\n \"current_round_part\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CHANGE\",\r\n \"current_round_part_backup_node\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CHANGE\",\r\n \"data\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CHANGE\",\r\n \"xcash_proof_of_stake_signature\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CHANGE\",\r\n}"
 
 
 
/*
  This message is sent from the Consensus node in the send_consensus_node_needs_to_add_a_block_to_the_network_message function and received by the nodes and main nodes in the server_receive_data_socket_consensus_node_to_node_and_main_node_restart function.
 
  The purpose of this message is for the Consensus node to let the nodes and the main node know that the Consensus node will now create the blocks on the network, due to a possible hack, and they should restart.
 
  message_settings - The type of the message
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CREATE_NEW_BLOCK_MESSAGE "{\r\n \"message_settings\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CREATE_NEW_BLOCK_MESSAGE\",\r\n \"public_address\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CREATE_NEW_BLOCK_MESSAGE\",\r\n \"previous_block_hash\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CREATE_NEW_BLOCK_MESSAGE\",\r\n \"current_round_part\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CREATE_NEW_BLOCK_MESSAGE\",\r\n \"current_round_part_backup_node\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CREATE_NEW_BLOCK_MESSAGE\",\r\n \"data\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CREATE_NEW_BLOCK_MESSAGE\",\r\n \"xcash_proof_of_stake_signature\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CREATE_NEW_BLOCK_MESSAGE\",\r\n}"
 
 
 
 /*
  This message is sent from the Consensus node in the xcash_proof_of_stake function and received by the consensus node private key in the receive_xcash_proof_of_stake_settings function.
 
  The purpose of this message is for the consensus node to get the xcash_proof_of_stake_settings
 
  message_settings - The type of the message
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_RECEIVE_XCASH_PROOF_OF_STAKE_SETTINGS "{\r\n \"message_settings\": \"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_RECEIVE_XCASH_PROOF_OF_STAKE_SETTINGS\",\r\n \"public_address\": \"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_RECEIVE_XCASH_PROOF_OF_STAKE_SETTINGS\",\r\n \"previous_block_hash\": \"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_RECEIVE_XCASH_PROOF_OF_STAKE_SETTINGS\",\r\n \"current_round_part\": \"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_RECEIVE_XCASH_PROOF_OF_STAKE_SETTINGS\",\r\n \"current_round_part_backup_node\": \"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_RECEIVE_XCASH_PROOF_OF_STAKE_SETTINGS\",\r\n \"data\": \"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_RECEIVE_XCASH_PROOF_OF_STAKE_SETTINGS\",\r\n \"xcash_proof_of_stake_signature\": \"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_RECEIVE_XCASH_PROOF_OF_STAKE_SETTINGS\",\r\n}"

 
 
 
/*
  This message is sent from the Consensus node private key in the receive_xcash_proof_of_stake_settings function and received by the consensus node in the xcash_proof_of_stake function.
 
  The purpose of this message is for the consensus node private key to return the receive_xcash_proof_of_stake_settings to the consensus node.
 
  message_settings - The type of the message
  xcash_proof_of_stake_settings - The xcash_proof_of_stake_settings (0 or 1)
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS "{\r\n \"message_settings\": \"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS\",\r\n \"xcash_proof_of_stake_settings\": \"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS\",\r\n \"public_address\": \"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS\",\r\n \"previous_block_hash\": \"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS\",\r\n \"current_round_part\": \"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS\",\r\n \"current_round_part_backup_node\": \"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS\",\r\n \"data\": \"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS\",\r\n \"xcash_proof_of_stake_signature\": \"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS\",\r\n}"

 
 
 
/*
  This message is sent from the consensus node in the consensus_node_create_new_block function and received by the block validation node in the consensus_node_create_new_block function.
 
  The purpose of this message is for the consensus node to create a block without the delegates
 
  message_settings - The type of the message
  block_blob - The block blob
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK "{\r\n \"message_settings\": \"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK\",\r\n \"block_blob\": \"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK\",\r\n \"public_address\": \"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK\",\r\n \"previous_block_hash\": \"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK\",\r\n \"current_round_part\": \"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK\",\r\n \"current_round_part_backup_node\": \"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK\",\r\n \"data\": \"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK\",\r\n \"xcash_proof_of_stake_signature\": \"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK\",\r\n}"

 
 
 
 /*
  This message is sent from the block validation node in the consensus_node_create_new_block function and received by the consensus node in the consensus_node_create_new_block function.
 
  The purpose of this message is for the block validation node to sign the block_blob that the consensus node created so the block will be valid and can be added to the network to create a block without the delegates
 
  message_settings - The type of the message
  block_blob_signature - The block blob signature
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK "{\r\n \"message_settings\": \"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK\",\r\n \"block_blob_signature\": \"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK\",\r\n \"public_address\": \"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK\",\r\n \"previous_block_hash\": \"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK\",\r\n \"current_round_part\": \"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK\",\r\n \"current_round_part_backup_node\": \"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK\",\r\n \"data\": \"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK\",\r\n \"xcash_proof_of_stake_signature\": \"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK\",\r\n}"

 
 
 
/*
  This message is sent from the Consensus node in the send_new_part_of_round function and received by the nodes and main nodes in the receive_new_part_of_round function.
 
  The purpose of this message is for the consensus node to tell the nodes and main nodes that a new part of the round is starting
 
  message_settings - The type of the message
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEW_PART_OF_ROUND "{\r\n \"message_settings\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEW_PART_OF_ROUND\",\r\n \"public_address\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEW_PART_OF_ROUND\",\r\n \"previous_block_hash\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEW_PART_OF_ROUND\",\r\n \"current_round_part\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEW_PART_OF_ROUND\",\r\n \"current_round_part_backup_node\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEW_PART_OF_ROUND\",\r\n \"data\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEW_PART_OF_ROUND\",\r\n \"xcash_proof_of_stake_signature\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEW_PART_OF_ROUND\",\r\n}"
 
 
 
/*
  This message is sent from the Consensus node in the send_next_round function and received by the nodes and main nodes in the receive_new_round function.
 
  The purpose of this message is for the consensus node to tell the nodes and main nodes that a new round is starting
 
  message_settings - The type of the message
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEXT_ROUND "{\r\n \"message_settings\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEXT_ROUND\",\r\n \"public_address\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEXT_ROUND\",\r\n \"previous_block_hash\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEXT_ROUND\",\r\n \"current_round_part\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEXT_ROUND\",\r\n \"current_round_part_backup_node\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEXT_ROUND\",\r\n \"data\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEXT_ROUND\",\r\n \"xcash_proof_of_stake_signature\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEXT_ROUND\",\r\n}"
 
 
 
  /*
  This message is sent from the consensus node in the create_new_block function and received by the block validators in the create_new_block function.
 
  The purpose of this message is for the consensus node to submit the block to the network that the block producer has created.
 
  message_settings - The type of the message  
  block_blob - The block blob
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CREATE_NEW_BLOCK "{\r\n \"message_settings\": \"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CREATE_NEW_BLOCK\",\r\n \"block_blob\": \"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CREATE_NEW_BLOCK\",\r\n \"public_address\": \"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CREATE_NEW_BLOCK\",\r\n \"previous_block_hash\": \"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CREATE_NEW_BLOCK\",\r\n \"current_round_part\": \"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CREATE_NEW_BLOCK\",\r\n \"current_round_part_backup_node\": \"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CREATE_NEW_BLOCK\",\r\n \"data\": \"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CREATE_NEW_BLOCK\",\r\n \"xcash_proof_of_stake_signature\": \"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CREATE_NEW_BLOCK\",\r\n}"

 
 
 
  /*
  This message is sent from the consensus node private key in the create_new_block function and received by the consensus node in the create_new_block function.
 
  The purpose of this message is for the block validator to sign the block_blob so the block will be valid and can be added to the network
 
  message_settings - The type of the message
  block_blob_signature - The block blob signature
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CREATE_NEW_BLOCK "{\r\n \"message_settings\": \"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CREATE_NEW_BLOCK\",\r\n \"block_blob_signature\": \"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CREATE_NEW_BLOCK\",\r\n \"public_address\": \"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CREATE_NEW_BLOCK\",\r\n \"previous_block_hash\": \"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CREATE_NEW_BLOCK\",\r\n \"current_round_part\": \"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CREATE_NEW_BLOCK\",\r\n \"current_round_part_backup_node\": \"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CREATE_NEW_BLOCK\",\r\n \"data\": \"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CREATE_NEW_BLOCK\",\r\n \"xcash_proof_of_stake_signature\": \"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CREATE_NEW_BLOCK\",\r\n}"

 
 
 
/*
  This message is sent from the Consensus node in the send_recalculating_votes_to_nodes_and_main_nodes function and received by the nodes and main nodes in the receive_recalculating_votes function.
 
  The purpose of this message is for the Consensus node to let the nodes and the main node know that the Consensus node will now recalculate the votes, and they should restart.
 
  message_settings - The type of the message
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_RECALCULATING_VOTES "{\r\n \"message_settings\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_RECALCULATING_VOTES\",\r\n \"public_address\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_RECALCULATING_VOTES\",\r\n \"previous_block_hash\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_RECALCULATING_VOTES\",\r\n \"current_round_part\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_RECALCULATING_VOTES\",\r\n \"current_round_part_backup_node\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_RECALCULATING_VOTES\",\r\n \"data\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_RECALCULATING_VOTES\",\r\n \"xcash_proof_of_stake_signature\": \"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_RECALCULATING_VOTES\",\r\n}"
 
 
 
/*
  This message is sent from the VOTING WEBSITE API PROGRAM and received by the consensus node private key in the send_xcash_proof_of_stake_settings function.
 
  The purpose of this message is for the consensus node private key to change the xcash_proof_of_stake_settings to 1
 
  message_settings - The type of the message
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define VOTING_WEBSITE_API_PROGRAM_TO_BLOCK_VALIDATION_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS "{\r\n \"message_settings\": \"VOTING_WEBSITE_API_PROGRAM_TO_BLOCK_VALIDATION_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS\",\r\n \"public_address\": \"VOTING_WEBSITE_API_PROGRAM_TO_BLOCK_VALIDATION_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS\",\r\n \"previous_block_hash\": \"VOTING_WEBSITE_API_PROGRAM_TO_BLOCK_VALIDATION_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS\",\r\n \"current_round_part\": \"VOTING_WEBSITE_API_PROGRAM_TO_BLOCK_VALIDATION_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS\",\r\n \"current_round_part_backup_node\": \"VOTING_WEBSITE_API_PROGRAM_TO_BLOCK_VALIDATION_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS\",\r\n \"data\": \"VOTING_WEBSITE_API_PROGRAM_TO_BLOCK_VALIDATION_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS\",\r\n \"xcash_proof_of_stake_signature\": \"VOTING_WEBSITE_API_PROGRAM_TO_BLOCK_VALIDATION_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS\",\r\n}"

 
  // reset the variables
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  count_test = 0;
 
  // write the start test message
  color_print(TEST_OUTLINE,"blue");
  printf("\033[1;34mstring functions test - Total test: %d\033[0m\n",STRING_FUNCTIONS_TOTAL_TEST);
  color_print(TEST_OUTLINE,"blue");
  printf("\n");
 
  // run the test
 
  // test parsing the return data for the sign RPC call for the xcash_proof_of_stake_signature field
  memcpy(result_test,SIGN_RPC_CALL_TEST_DATA,strnlen(SIGN_RPC_CALL_TEST_DATA,BUFFER_SIZE));
  if (parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) != 0)
  {
    // check if the data is correct
    if (strncmp(data_test,"SIGN_RPC_CALL_TEST_DATA",BUFFER_SIZE) == 0)
    {
      color_print("PASSED! Test for parsing the return data for the sign RPC call for the xcash_proof_of_stake_signature field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for the sign RPC call for the xcash_proof_of_stake_signature field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for the sign RPC call for the xcash_proof_of_stake_signature field","red");
  }
 
  // test parsing the return data for the verify RPC call for the good field
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,VERIFY_RPC_CALL_TEST_DATA,strnlen(VERIFY_RPC_CALL_TEST_DATA,BUFFER_SIZE));
  if (parse_json_data(result_test,"good",data_test) != 0)
  {
    // check if the data is correct
    if (strncmp(data_test,"VERIFY_RPC_CALL_TEST_DATA",BUFFER_SIZE) == 0)
    {
      color_print("PASSED! Test for parsing the return data for the verify RPC call for the good field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for the verify RPC call for the good field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for the verify RPC call for the good field","red");
  }
 
  // test parsing the return data for the get_block_template RPC call for the blocktemplate_blob field
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,strnlen(GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,BUFFER_SIZE));
  if (parse_json_data(result_test,"blocktemplate_blob",data_test) != 0)
  {
    // check if the data is correct
    if (strncmp(data_test,"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA",BUFFER_SIZE) == 0)
    {
      color_print("PASSED! Test for parsing the return data for the get_block_template RPC call for the blocktemplate_blob field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for the get_block_template RPC call for the blocktemplate_blob field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for the get_block_template RPC call for the blocktemplate_blob field","red");
  }

  // test for parsing CONSENSUS_NODE_TO_NODE_RECEIVE_CURRENT_CONSENSUS_NODE_IP_ADDRESS
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,CONSENSUS_NODE_TO_NODE_RECEIVE_CURRENT_CONSENSUS_NODE_IP_ADDRESS,strnlen(CONSENSUS_NODE_TO_NODE_RECEIVE_CURRENT_CONSENSUS_NODE_IP_ADDRESS,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODE_RECEIVE_CURRENT_CONSENSUS_NODE_IP_ADDRESS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_consensus_node_IP_address",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODE_RECEIVE_CURRENT_CONSENSUS_NODE_IP_ADDRESS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODE_RECEIVE_CURRENT_CONSENSUS_NODE_IP_ADDRESS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODE_RECEIVE_CURRENT_CONSENSUS_NODE_IP_ADDRESS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODE_RECEIVE_CURRENT_CONSENSUS_NODE_IP_ADDRESS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODE_RECEIVE_CURRENT_CONSENSUS_NODE_IP_ADDRESS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODE_RECEIVE_CURRENT_CONSENSUS_NODE_IP_ADDRESS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODE_RECEIVE_CURRENT_CONSENSUS_NODE_IP_ADDRESS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (settings == 0)
  {
    color_print("FAILED! Test for parsing CONSENSUS_NODE_TO_NODE_RECEIVE_CURRENT_CONSENSUS_NODE_IP_ADDRESS","red");
  }
  else
  {
    color_print("PASSED! Test for parsing CONSENSUS_NODE_TO_NODE_RECEIVE_CURRENT_CONSENSUS_NODE_IP_ADDRESS","green");
    count_test++;
  }
  settings = 1;

  // test for parsing NODE_TO_CONSENSUS_NODE_SEND_CURRENT_CONSENSUS_NODE_IP_ADDRESS
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,NODE_TO_CONSENSUS_NODE_SEND_CURRENT_CONSENSUS_NODE_IP_ADDRESS,strnlen(NODE_TO_CONSENSUS_NODE_SEND_CURRENT_CONSENSUS_NODE_IP_ADDRESS,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"NODE_TO_CONSENSUS_NODE_SEND_CURRENT_CONSENSUS_NODE_IP_ADDRESS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"NODE_TO_CONSENSUS_NODE_SEND_CURRENT_CONSENSUS_NODE_IP_ADDRESS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"NODE_TO_CONSENSUS_NODE_SEND_CURRENT_CONSENSUS_NODE_IP_ADDRESS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"NODE_TO_CONSENSUS_NODE_SEND_CURRENT_CONSENSUS_NODE_IP_ADDRESS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"NODE_TO_CONSENSUS_NODE_SEND_CURRENT_CONSENSUS_NODE_IP_ADDRESS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"NODE_TO_CONSENSUS_NODE_SEND_CURRENT_CONSENSUS_NODE_IP_ADDRESS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"NODE_TO_CONSENSUS_NODE_SEND_CURRENT_CONSENSUS_NODE_IP_ADDRESS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (settings == 0)
  {
    color_print("FAILED! Test for parsing NODE_TO_CONSENSUS_NODE_SEND_CURRENT_CONSENSUS_NODE_IP_ADDRESS","red");
  }
  else
  {
    color_print("PASSED! Test for parsing NODE_TO_CONSENSUS_NODE_SEND_CURRENT_CONSENSUS_NODE_IP_ADDRESS","green");
    count_test++;
  }
  settings = 1;

  // test for parsing CONSENSUS_NODE_TO_NODE_RECEIVE_UPDATED_NODE_LIST
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,CONSENSUS_NODE_TO_NODE_RECEIVE_UPDATED_NODE_LIST,strnlen(CONSENSUS_NODE_TO_NODE_RECEIVE_UPDATED_NODE_LIST,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODE_RECEIVE_UPDATED_NODE_LIST",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"nodes_name_list",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODE_RECEIVE_UPDATED_NODE_LIST",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"nodes_public_address_list",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODE_RECEIVE_UPDATED_NODE_LIST",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"nodes_IP_address_list",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODE_RECEIVE_UPDATED_NODE_LIST",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODE_RECEIVE_UPDATED_NODE_LIST",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODE_RECEIVE_UPDATED_NODE_LIST",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODE_RECEIVE_UPDATED_NODE_LIST",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODE_RECEIVE_UPDATED_NODE_LIST",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODE_RECEIVE_UPDATED_NODE_LIST",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODE_RECEIVE_UPDATED_NODE_LIST",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (settings == 0)
  {
    color_print("FAILED! Test for parsing CONSENSUS_NODE_TO_NODE_RECEIVE_UPDATED_NODE_LIST","red");
  }
  else
  {
    color_print("PASSED! Test for parsing CONSENSUS_NODE_TO_NODE_RECEIVE_UPDATED_NODE_LIST","green");
    count_test++;
  }
  settings = 1;

    // test for parsing NODE_TO_CONSENSUS_NODE_SEND_UPDATED_NODE_LIST
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,NODE_TO_CONSENSUS_NODE_SEND_UPDATED_NODE_LIST,strnlen(NODE_TO_CONSENSUS_NODE_SEND_UPDATED_NODE_LIST,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"NODE_TO_CONSENSUS_NODE_SEND_UPDATED_NODE_LIST",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"nodes_updated_time",data_test) == 0 || strncmp(data_test,"NODE_TO_CONSENSUS_NODE_SEND_UPDATED_NODE_LIST",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"NODE_TO_CONSENSUS_NODE_SEND_UPDATED_NODE_LIST",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"NODE_TO_CONSENSUS_NODE_SEND_UPDATED_NODE_LIST",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"NODE_TO_CONSENSUS_NODE_SEND_UPDATED_NODE_LIST",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"NODE_TO_CONSENSUS_NODE_SEND_UPDATED_NODE_LIST",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"NODE_TO_CONSENSUS_NODE_SEND_UPDATED_NODE_LIST",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"NODE_TO_CONSENSUS_NODE_SEND_UPDATED_NODE_LIST",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (settings == 0)
  {
    color_print("FAILED! Test for parsing NODE_TO_CONSENSUS_NODE_SEND_UPDATED_NODE_LIST","red");
  }
  else
  {
    color_print("PASSED! Test for parsing NODE_TO_CONSENSUS_NODE_SEND_UPDATED_NODE_LIST","green");
    count_test++;
  }
  settings = 1;

  // test for parsing CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS,strnlen(CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"main_nodes_public_address",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (settings == 0)
  {
    color_print("FAILED! Test for parsing CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS","red");
  }
  else
  {
    color_print("PASSED! Test for parsing CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS","green");
    count_test++;
  }
  settings = 1;

  // test for parsing NODES_TO_CONSENSUS_NODE_MAIN_NODE_SOCKET_TIMEOUT_ROUND_CHANGE
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,NODES_TO_CONSENSUS_NODE_MAIN_NODE_SOCKET_TIMEOUT_ROUND_CHANGE,strnlen(NODES_TO_CONSENSUS_NODE_MAIN_NODE_SOCKET_TIMEOUT_ROUND_CHANGE,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"NODES_TO_CONSENSUS_NODE_MAIN_NODE_SOCKET_TIMEOUT_ROUND_CHANGE",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"NODES_TO_CONSENSUS_NODE_MAIN_NODE_SOCKET_TIMEOUT_ROUND_CHANGE",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"NODES_TO_CONSENSUS_NODE_MAIN_NODE_SOCKET_TIMEOUT_ROUND_CHANGE",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"NODES_TO_CONSENSUS_NODE_MAIN_NODE_SOCKET_TIMEOUT_ROUND_CHANGE",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"NODES_TO_CONSENSUS_NODE_MAIN_NODE_SOCKET_TIMEOUT_ROUND_CHANGE",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"NODES_TO_CONSENSUS_NODE_MAIN_NODE_SOCKET_TIMEOUT_ROUND_CHANGE",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"NODES_TO_CONSENSUS_NODE_MAIN_NODE_SOCKET_TIMEOUT_ROUND_CHANGE",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (settings == 0)
  {
    color_print("FAILED! Test for parsing NODES_TO_CONSENSUS_NODE_MAIN_NODE_SOCKET_TIMEOUT_ROUND_CHANGE","red");
  }
  else
  {
    color_print("PASSED! Test for parsing NODES_TO_CONSENSUS_NODE_MAIN_NODE_SOCKET_TIMEOUT_ROUND_CHANGE","green");
    count_test++;
  }
  settings = 1;

  // test for parsing CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND,strnlen(CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"message",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"vrf_beta_string_part_3_of_round",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (settings == 0)
  {
    color_print("FAILED! Test for parsing CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND","red");
  }
  else
  {
    color_print("PASSED! Test for parsing CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND","green");
    count_test++;
  }
  settings = 1;

  // test for parsing MAIN_NODES_TO_NODES_PART_1_OF_ROUND
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,MAIN_NODES_TO_NODES_PART_1_OF_ROUND,strnlen(MAIN_NODES_TO_NODES_PART_1_OF_ROUND,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_1_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"vrf_public_key",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_1_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"vrf_alpha_string",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_1_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"vrf_proof",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_1_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"vrf_beta_string",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_1_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_1_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_1_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_1_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_1_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_1_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_1_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (settings == 0)
  {
    color_print("FAILED! Test for parsing MAIN_NODES_TO_NODES_PART_1_OF_ROUND","red");
  }
  else
  {
    color_print("PASSED! Test for parsing MAIN_NODES_TO_NODES_PART_1_OF_ROUND","green");
    count_test++;
  }
  settings = 1;

  // test for parsing MAIN_NODES_TO_NODES_PART_2_OF_ROUND
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,MAIN_NODES_TO_NODES_PART_2_OF_ROUND,strnlen(MAIN_NODES_TO_NODES_PART_2_OF_ROUND,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_2_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"vrf_public_key",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_2_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"vrf_random_character_string",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_2_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"vrf_alpha_string",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_2_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"vrf_proof",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_2_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"vrf_beta_string",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_2_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_2_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_2_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_2_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_2_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_2_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_2_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (settings == 0)
  {
    color_print("FAILED! Test for parsing MAIN_NODES_TO_NODES_PART_2_OF_ROUND","red");
  }
  else
  {
    color_print("PASSED! Test for parsing MAIN_NODES_TO_NODES_PART_2_OF_ROUND","green");
    count_test++;
  }
  settings = 1;

  // test for parsing MAIN_NODES_TO_NODES_PART_3_OF_ROUND
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,MAIN_NODES_TO_NODES_PART_3_OF_ROUND,strnlen(MAIN_NODES_TO_NODES_PART_3_OF_ROUND,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_3_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"vrf_public_key",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_3_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"vrf_alpha_string",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_3_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"vrf_proof",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_3_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"vrf_beta_string",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_3_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_3_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_3_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_3_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_3_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_3_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_3_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (settings == 0)
  {
    color_print("FAILED! Test for parsing MAIN_NODES_TO_NODES_PART_3_OF_ROUND","red");
  }
  else
  {
    color_print("PASSED! Test for parsing MAIN_NODES_TO_NODES_PART_3_OF_ROUND","green");
    count_test++;
  }
  settings = 1;

  // test for parsing MAIN_NODES_TO_NODES_PART_4_OF_ROUND
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,MAIN_NODES_TO_NODES_PART_4_OF_ROUND,strnlen(MAIN_NODES_TO_NODES_PART_4_OF_ROUND,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"block_blob",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (settings == 0)
  {
    color_print("FAILED! Test for parsing MAIN_NODES_TO_NODES_PART_4_OF_ROUND","red");
  }
  else
  {
    color_print("PASSED! Test for parsing MAIN_NODES_TO_NODES_PART_4_OF_ROUND","green");
    count_test++;
  }
  settings = 1;

    // test for parsing NODES_TO_NODES_VOTE_RESULTS
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,NODES_TO_NODES_VOTE_RESULTS,strnlen(NODES_TO_NODES_VOTE_RESULTS,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"NODES_TO_NODES_VOTE_RESULTS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"vote_settings",data_test) == 0 || strncmp(data_test,"NODES_TO_NODES_VOTE_RESULTS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"vote_data",data_test) == 0 || strncmp(data_test,"NODES_TO_NODES_VOTE_RESULTS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"NODES_TO_NODES_VOTE_RESULTS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"NODES_TO_NODES_VOTE_RESULTS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"NODES_TO_NODES_VOTE_RESULTS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"NODES_TO_NODES_VOTE_RESULTS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"NODES_TO_NODES_VOTE_RESULTS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"NODES_TO_NODES_VOTE_RESULTS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (settings == 0)
  {
    color_print("FAILED! Test for parsing NODES_TO_NODES_VOTE_RESULTS","red");
  }
  else
  {
    color_print("PASSED! Test for parsing NODES_TO_NODES_VOTE_RESULTS","green");
    count_test++;
  }
  settings = 1;

  // test for parsing NODES_TO_CONSENSUS_NODE_VOTE_RESULTS
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,NODES_TO_CONSENSUS_NODE_VOTE_RESULTS,strnlen(NODES_TO_CONSENSUS_NODE_VOTE_RESULTS,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"NODES_TO_CONSENSUS_NODE_VOTE_RESULTS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"vote_result",data_test) == 0 || strncmp(data_test,"NODES_TO_CONSENSUS_NODE_VOTE_RESULTS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"NODES_TO_CONSENSUS_NODE_VOTE_RESULTS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"NODES_TO_CONSENSUS_NODE_VOTE_RESULTS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"NODES_TO_CONSENSUS_NODE_VOTE_RESULTS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"NODES_TO_CONSENSUS_NODE_VOTE_RESULTS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"NODES_TO_CONSENSUS_NODE_VOTE_RESULTS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"NODES_TO_CONSENSUS_NODE_VOTE_RESULTS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (settings == 0)
  {
    color_print("FAILED! Test for parsing NODES_TO_CONSENSUS_NODE_VOTE_RESULTS","red");
  }
  else
  {
    color_print("PASSED! Test for parsing NODES_TO_CONSENSUS_NODE_VOTE_RESULTS","green");
    count_test++;
  }
  settings = 1;

  // test for parsing CONSENSUS_NODE_TO_NODES_LIST_OF_ENABLED_NODES
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,CONSENSUS_NODE_TO_NODES_LIST_OF_ENABLED_NODES,strnlen(CONSENSUS_NODE_TO_NODES_LIST_OF_ENABLED_NODES,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_LIST_OF_ENABLED_NODES",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"nodes_name_list",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_LIST_OF_ENABLED_NODES",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"nodes_public_address_list",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_LIST_OF_ENABLED_NODES",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"nodes_IP_address_list",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_LIST_OF_ENABLED_NODES",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_LIST_OF_ENABLED_NODES",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_LIST_OF_ENABLED_NODES",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_LIST_OF_ENABLED_NODES",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_LIST_OF_ENABLED_NODES",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_LIST_OF_ENABLED_NODES",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_LIST_OF_ENABLED_NODES",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (settings == 0)
  {
    color_print("FAILED! Test for parsing CONSENSUS_NODE_TO_NODES_LIST_OF_ENABLED_NODES","red");
  }
  else
  {
    color_print("PASSED! Test for parsing CONSENSUS_NODE_TO_NODES_LIST_OF_ENABLED_NODES","green");
    count_test++;
  }
  settings = 1;

  // test for parsing CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_ROUND_CHANGE
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_ROUND_CHANGE,strnlen(CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_ROUND_CHANGE,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_ROUND_CHANGE",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_ROUND_CHANGE",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_ROUND_CHANGE",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_ROUND_CHANGE",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_ROUND_CHANGE",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_ROUND_CHANGE",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_ROUND_CHANGE",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (settings == 0)
  {
    color_print("FAILED! Test for parsing CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_ROUND_CHANGE","red");
  }
  else
  {
    color_print("PASSED! Test for parsing CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_ROUND_CHANGE","green");
    count_test++;
  }
  settings = 1;

  // test for parsing CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CHANGE
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CHANGE,strnlen(CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CHANGE,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CHANGE",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"consensus_node_IP_address",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CHANGE",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CHANGE",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CHANGE",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CHANGE",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CHANGE",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CHANGE",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CHANGE",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CHANGE",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (settings == 0)
  {
    color_print("FAILED! Test for parsing CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CHANGE","red");
  }
  else
  {
    color_print("PASSED! Test for parsing CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CHANGE","green");
    count_test++;
  }
  settings = 1;

  // test for parsing CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CREATE_NEW_BLOCK_MESSAGE
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CREATE_NEW_BLOCK_MESSAGE,strnlen(CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CREATE_NEW_BLOCK_MESSAGE,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CREATE_NEW_BLOCK_MESSAGE",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CREATE_NEW_BLOCK_MESSAGE",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CREATE_NEW_BLOCK_MESSAGE",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CREATE_NEW_BLOCK_MESSAGE",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CREATE_NEW_BLOCK_MESSAGE",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CREATE_NEW_BLOCK_MESSAGE",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CREATE_NEW_BLOCK_MESSAGE",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (settings == 0)
  {
    color_print("FAILED! Test for parsing CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CREATE_NEW_BLOCK_MESSAGE","red");
  }
  else
  {
    color_print("PASSED! Test for parsing CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CREATE_NEW_BLOCK_MESSAGE","green");
    count_test++;
  }
  settings = 1;

  // test for parsing CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_RECEIVE_XCASH_PROOF_OF_STAKE_SETTINGS
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_RECEIVE_XCASH_PROOF_OF_STAKE_SETTINGS,strnlen(CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_RECEIVE_XCASH_PROOF_OF_STAKE_SETTINGS,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_RECEIVE_XCASH_PROOF_OF_STAKE_SETTINGS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_RECEIVE_XCASH_PROOF_OF_STAKE_SETTINGS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_RECEIVE_XCASH_PROOF_OF_STAKE_SETTINGS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_RECEIVE_XCASH_PROOF_OF_STAKE_SETTINGS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_RECEIVE_XCASH_PROOF_OF_STAKE_SETTINGS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_RECEIVE_XCASH_PROOF_OF_STAKE_SETTINGS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_RECEIVE_XCASH_PROOF_OF_STAKE_SETTINGS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (settings == 0)
  {
    color_print("FAILED! Test for parsing CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_RECEIVE_XCASH_PROOF_OF_STAKE_SETTINGS","red");
  }
  else
  {
    color_print("PASSED! Test for parsing CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_RECEIVE_XCASH_PROOF_OF_STAKE_SETTINGS","green");
    count_test++;
  }
  settings = 1;

  // test for parsing BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS,strnlen(BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"xcash_proof_of_stake_settings",data_test) == 0 || strncmp(data_test,"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (settings == 0)
  {
    color_print("FAILED! Test for parsing BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS","red");
  }
  else
  {
    color_print("PASSED! Test for parsing BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS","green");
    count_test++;
  }
  settings = 1;

  // test for parsing CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK,strnlen(CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"block_blob",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (settings == 0)
  {
    color_print("FAILED! Test for parsing CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK","red");
  }
  else
  {
    color_print("PASSED! Test for parsing CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK","green");
    count_test++;
  }
  settings = 1;

  // test for parsing BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK,strnlen(BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"block_blob_signature",data_test) == 0 || strncmp(data_test,"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (settings == 0)
  {
    color_print("FAILED! Test for parsing BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK","red");
  }
  else
  {
    color_print("PASSED! Test for parsing BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK","green");
    count_test++;
  }
  settings = 1;

  // test for parsing CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEW_PART_OF_ROUND
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEW_PART_OF_ROUND,strnlen(CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEW_PART_OF_ROUND,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEW_PART_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEW_PART_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEW_PART_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEW_PART_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEW_PART_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEW_PART_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEW_PART_OF_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (settings == 0)
  {
    color_print("FAILED! Test for parsing CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEW_PART_OF_ROUND","red");
  }
  else
  {
    color_print("PASSED! Test for parsing CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEW_PART_OF_ROUND","green");
    count_test++;
  }
  settings = 1;

  // test for parsing CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEXT_ROUND
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEXT_ROUND,strnlen(CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEXT_ROUND,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEXT_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEXT_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEXT_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEXT_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEXT_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEXT_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEXT_ROUND",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (settings == 0)
  {
    color_print("FAILED! Test for parsing CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEXT_ROUND","red");
  }
  else
  {
    color_print("PASSED! Test for parsing CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEXT_ROUND","green");
    count_test++;
  }
  settings = 1;

  // test for parsing CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CREATE_NEW_BLOCK
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CREATE_NEW_BLOCK,strnlen(CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CREATE_NEW_BLOCK,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"block_blob",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (settings == 0)
  {
    color_print("FAILED! Test for parsing CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CREATE_NEW_BLOCK","red");
  }
  else
  {
    color_print("PASSED! Test for parsing CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CREATE_NEW_BLOCK","green");
    count_test++;
  }
  settings = 1;

  // test for parsing BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CREATE_NEW_BLOCK
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CREATE_NEW_BLOCK,strnlen(BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CREATE_NEW_BLOCK,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"block_blob_signature",data_test) == 0 || strncmp(data_test,"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (settings == 0)
  {
    color_print("FAILED! Test for parsing BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CREATE_NEW_BLOCK","red");
  }
  else
  {
    color_print("PASSED! Test for parsing BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CREATE_NEW_BLOCK","green");
    count_test++;
  }
  settings = 1;

  // test for parsing CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_RECALCULATING_VOTES
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_RECALCULATING_VOTES,strnlen(CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_RECALCULATING_VOTES,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_RECALCULATING_VOTES",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_RECALCULATING_VOTES",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_RECALCULATING_VOTES",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_RECALCULATING_VOTES",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_RECALCULATING_VOTES",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_RECALCULATING_VOTES",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_RECALCULATING_VOTES",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (settings == 0)
  {
    color_print("FAILED! Test for parsing CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_RECALCULATING_VOTES","red");
  }
  else
  {
    color_print("PASSED! Test for parsing CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_RECALCULATING_VOTES","green");
    count_test++;
  }
  settings = 1;

  // test for parsing VOTING_WEBSITE_API_PROGRAM_TO_BLOCK_VALIDATION_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,VOTING_WEBSITE_API_PROGRAM_TO_BLOCK_VALIDATION_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS,strnlen(VOTING_WEBSITE_API_PROGRAM_TO_BLOCK_VALIDATION_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"VOTING_WEBSITE_API_PROGRAM_TO_BLOCK_VALIDATION_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"VOTING_WEBSITE_API_PROGRAM_TO_BLOCK_VALIDATION_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"VOTING_WEBSITE_API_PROGRAM_TO_BLOCK_VALIDATION_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"VOTING_WEBSITE_API_PROGRAM_TO_BLOCK_VALIDATION_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"VOTING_WEBSITE_API_PROGRAM_TO_BLOCK_VALIDATION_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"VOTING_WEBSITE_API_PROGRAM_TO_BLOCK_VALIDATION_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"VOTING_WEBSITE_API_PROGRAM_TO_BLOCK_VALIDATION_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS",BUFFER_SIZE) != 0)
  {
    settings = 0;
  }
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (settings == 0)
  {
    color_print("FAILED! Test for parsing VOTING_WEBSITE_API_PROGRAM_TO_BLOCK_VALIDATION_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS","red");
  }
  else
  {
    color_print("PASSED! Test for parsing VOTING_WEBSITE_API_PROGRAM_TO_BLOCK_VALIDATION_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS","green");
    count_test++;
  }
  settings = 1;

  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  if (random_string(result_test,RANDOM_STRING_LENGTH) == 1)
  {   
    color_print("PASSED! Test for creating a random string","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for creating a random string","red");
  }

  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memcpy(result_test,TEST_OUTLINE,strnlen(TEST_OUTLINE,BUFFER_SIZE));

  // test for counting occurences of a substring in a string
  if (string_count(result_test,"-") == strnlen(TEST_OUTLINE,BUFFER_SIZE) && string_count(result_test,"--") == strnlen(TEST_OUTLINE,BUFFER_SIZE)/2)  
  {
    color_print("PASSED! Test for counting occurences of a substring in a string","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for counting occurences of a substring in a string","red");
  }

  // test for string replace
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  append_string(result_test,"{\r\n \"message_settings\": \"string_replace_test\",\r\n}");
  if (string_replace(result_test,"string_replace_test","string_replace") == 1 && strncmp(result_test,"{\r\n \"message_settings\": \"string_replace\",\r\n}",BUFFER_SIZE) == 0)
  {
    memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
    append_string(result_test,"{\r\n \"message_settings\": \"string_replace_test\",\r\n}");
     if (string_replace(result_test,"\"","\\\"") == 1 && strncmp(result_test,"{\r\n \\\"message_settings\\\": \\\"string_replace_test\\\",\r\n}",BUFFER_SIZE) == 0)
     {
       memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
       append_string(result_test,"{\r\n \"message_settings\": \"string_replace_test\",\r\n}");
       if (string_replace(result_test,"_test","") == 1 && strncmp(result_test,"{\r\n \"message_settings\": \"string_replace\",\r\n}",BUFFER_SIZE) == 0)
       {
         memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
         append_string(result_test,"{\r\n \"message_settings\": \"string_replace\",\r\n}");
         if (string_replace(result_test,"string_replace_test","") == 0 && strncmp(result_test,"{\r\n \"message_settings\": \"string_replace\",\r\n}",BUFFER_SIZE) == 0)
         {    
           color_print("PASSED! Test for string replace","green");
           count_test++;
         }
         else
         {
           color_print("FAILED! Test for string replace","red");
         }
       }
       else
       {
         color_print("FAILED! Test for string replace","red");
       }
     }
     else
     {
       color_print("FAILED! Test for string replace","red");
     }
  }
  else
  {
    color_print("FAILED! Test for string replace","red");
  }
 
  // write the end test message
  if (count_test == STRING_FUNCTIONS_TOTAL_TEST)
  {
    printf("\n\n");
    color_print(TEST_OUTLINE,"green");
    printf("\033[1;32mstring functions test - Passed test: %d, Failed test: 0\033[0m\n",STRING_FUNCTIONS_TOTAL_TEST);
    color_print(TEST_OUTLINE,"green");
    printf("\n\n");
  }
  else
  {
    printf("\n\n");
    color_print(TEST_OUTLINE,"red");
    printf("\033[1;31mstring functions test - Passed test: %d, Failed test: %d\033[0m\n",count_test,STRING_FUNCTIONS_TOTAL_TEST-count_test);
    color_print(TEST_OUTLINE,"red");
    printf("\n\n");
  }
  return count_test;

  #undef STRING_FUNCTIONS_TOTAL_TEST
  #undef SIGN_RPC_CALL_TEST_DATA
  #undef VERIFY_RPC_CALL_TEST_DATA
  #undef GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA
  #undef CONSENSUS_NODE_TO_NODE_RECEIVE_CURRENT_CONSENSUS_NODE_IP_ADDRESS
  #undef NODE_TO_CONSENSUS_NODE_SEND_CURRENT_CONSENSUS_NODE_IP_ADDRESS
  #undef CONSENSUS_NODE_TO_NODE_RECEIVE_UPDATED_NODE_LIST
  #undef NODE_TO_CONSENSUS_NODE_SEND_UPDATED_NODE_LIST
  #undef CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS
  #undef NODES_TO_CONSENSUS_NODE_MAIN_NODE_SOCKET_TIMEOUT_ROUND_CHANGE
  #undef CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND
  #undef MAIN_NODES_TO_NODES_PART_1_OF_ROUND
  #undef MAIN_NODES_TO_NODES_PART_2_OF_ROUND
  #undef MAIN_NODES_TO_NODES_PART_3_OF_ROUND
  #undef MAIN_NODES_TO_NODES_PART_4_OF_ROUND
  #undef NODES_TO_NODES_VOTE_RESULTS
  #undef NODES_TO_CONSENSUS_NODE_VOTE_RESULTS
  #undef CONSENSUS_NODE_TO_NODES_LIST_OF_ENABLED_NODES
  #undef CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_ROUND_CHANGE
  #undef CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CHANGE
  #undef CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CREATE_NEW_BLOCK_MESSAGE
  #undef CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_RECEIVE_XCASH_PROOF_OF_STAKE_SETTINGS
  #undef BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS
  #undef CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK
  #undef BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CONSENSUS_NODE_CREATE_NEW_BLOCK
  #undef CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEW_PART_OF_ROUND
  #undef CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEXT_ROUND
  #undef CONSENSUS_NODE_TO_BLOCK_VALIDATION_NODE_CREATE_NEW_BLOCK
  #undef BLOCK_VALIDATION_NODE_TO_CONSENSUS_NODE_CREATE_NEW_BLOCK
  #undef CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_RECALCULATING_VOTES
  #undef VOTING_WEBSITE_API_PROGRAM_TO_BLOCK_VALIDATION_NODE_SEND_XCASH_PROOF_OF_STAKE_SETTINGS
}